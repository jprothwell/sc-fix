/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    cfw_sms_main.c                                                */
/*                                                                            */
/* Description:                                                               */
/*           All APIs are used in cfw_sms_mmi.c                               */
/*                                                                            */
/* Revision :                                                                 */
/*   1.0.0 : 2006-02-20,  Wang Dahai, dahai_wang@ciitec.com                   */
/*                                                                            */
/* Remark: Search "???" for more current problems reserved                    */
/*                                                                            */
/******************************************************************************/
#include <cos.h>
#include "csw_debug.h"

#include "cfw_sms_prv.h"
//Here, define a golobal variable for longer msg
extern UINT32* gpLongerMsgPara;
extern UINT32* gpLongerMsgParaIndex;
extern CFW_SMS_LONG_INFO* gpWriteLongerMsgPara;
extern UINT8   gnWriteLongerMsgParaCount;
//for long sms
extern PSTR pSMSDir;
extern PSTR pSIMFilename;
extern PSTR pMEFilename;
extern BOOL gSMSInitFlag;

#ifdef SMSEX
    UINT32 sms_db_InitEX(void);
#endif

#ifdef CFW_MULTI_SIM	
extern CFW_SMS_INIT_STATUS gNewSMSInd[CFW_SIM_COUNT];
#else
extern CFW_SMS_INIT_STATUS gNewSMSInd;
#endif

#if (CFW_SMS_DUMP_ENABLE==0) && (CFW_SMS_SRV_ENABLE==1)

/**********************************************************************************/
/**********************************************************************************/
UINT32 SRVAPI CFW_SmsMoInit(
		UINT16 nUti
#ifdef CFW_MULTI_SIM
		, CFW_SIM_ID nSimID
#endif
		)
{
    CFW_SMS_INIT_HANDLE*  pSmsMOInfo = NULL;
    CSW_PROFILE_FUNCTION_ENTER(CFW_SmsMoInit);
#ifdef CFW_MULTI_SIM
    CSW_TRACE(CFW_SMS_TS_ID, TSTXT("Start CFW_SmsMoInit, SimID:%d\n"),nSimID);
#endif
    //Malloc for Handle
    CSW_TRACE(CFW_SMS_TS_ID, TSTXT("Start CFW_SmsMoInit, SMS_Disp_GetTime : %d Secn  \n"),(UINT32)(SMS_Disp_GetTime()/256));
    pSmsMOInfo = (CFW_SMS_INIT_HANDLE*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_INIT_HANDLE));
    if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_SmsMoInit pSmsMOInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsMoInit);
		return ERR_CMS_MEMORY_FULL;
      }
    SUL_ZeroMemory8(pSmsMOInfo, SIZEOF(CFW_SMS_INIT_HANDLE));
    
    //Register CallBack into system 
#ifndef CFW_MULTI_SIM
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_InitAoProc);
#else
	pSmsMOInfo->hAo = CFW_RegisterAo(CFW_APP_SRV_ID, pSmsMOInfo, sms_InitAoProc, nSimID);
#endif
    CFW_SetUTI(pSmsMOInfo->hAo,nUti,0);
    CFW_SetAoProcCode(pSmsMOInfo->hAo,CFW_AO_PROC_CODE_CONTINUE);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SmsMoInit);
    return ERR_SUCCESS;
}

UINT32 SRVAPI CFW_SmsMtInit(VOID) 
{
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsMtInit);
    CFW_RegisterCreateAoProc(API_SMSPP_RECEIVE_IND, (PFN_EV_IND_TRIGGER_PORC)sms_mt_Reg);
    CSW_PROFILE_FUNCTION_EXIT(CFW_SmsMtInit);
    return ERR_SUCCESS;
}

HAO sms_mt_Reg(CFW_EV* pEvent) 
{
    CFW_SMS_MT_INFO*    pSmsMTInfo = NULL; //Point to MT private data.
    CSW_PROFILE_FUNCTION_ENTER(sms_mt_Reg);
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId= CFW_SIM_COUNT;
	nSimId = pEvent->nFlag;
#endif
    pEvent = pEvent;
    pSmsMTInfo = (CFW_SMS_MT_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_MT_INFO));
    if(!pSmsMTInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Reg pSmsMTInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(sms_mt_Reg);
		return ERR_NO_MORE_MEMORY;
	}
    SUL_ZeroMemory8(pSmsMTInfo, SIZEOF(CFW_SMS_MT_INFO));
    
    //Register CallBack into system 
#ifndef CFW_MULTI_SIM
    pSmsMTInfo->hAo = CFW_RegisterAo(CFW_SMS_MT_SRV_ID, pSmsMTInfo, sms_MTAoProc);
#else
    pSmsMTInfo->hAo = CFW_RegisterAo(CFW_SMS_MT_SRV_ID, pSmsMTInfo, sms_MTAoProc, nSimId);
#endif
    
    CSW_PROFILE_FUNCTION_EXIT(sms_mt_Reg);
    return pSmsMTInfo->hAo;
}

#ifndef CFW_MULTI_SIM
HAO sms_cb_Reg(CFW_EV* pEvent) 
{
    CFW_SMS_CB_INFO*    pSmsCBInfo = NULL; //Point to MT private data.
    CSW_PROFILE_FUNCTION_ENTER(sms_cb_Reg);
    
    pEvent = pEvent;
    pSmsCBInfo = (CFW_SMS_CB_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_CB_INFO));
    if(!pSmsCBInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Reg pSmsCBInfo malloc error!!! \n ");
		CSW_PROFILE_FUNCTION_EXIT(sms_cb_Reg);
		return ERR_NO_MORE_MEMORY;
	}
    SUL_ZeroMemory8(pSmsCBInfo, SIZEOF(CFW_SMS_CB_INFO));
    
    //Register CallBack into system 
    pSmsCBInfo->hAo = CFW_RegisterAo(CFW_SMS_CB_ID, pSmsCBInfo, sms_CBAoProc);
    
    CSW_PROFILE_FUNCTION_EXIT(sms_cb_Reg);  
    return pSmsCBInfo->hAo;
}
#endif

///////////////////////////////////////////////////////////////////////////////
//                         Local Proc For SMS MO APIs                        //
//                          Invoked by CFW_RegisterAo                        //
///////////////////////////////////////////////////////////////////////////////
UINT32 sms_ReadAoProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32           ret=0x0,nGetUti=0x0,nState = 0x0;    //MO State
	CFW_SMS_MO_INFO* pSmsMOInfo = NULL;   //Point to MO private data.
	CFW_EV           sMeEvent;
	UINT8            nSmsStatus = 0x0;
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID  nSimId= CFW_SIM_COUNT;
#endif
	CSW_PROFILE_FUNCTION_ENTER(sms_ReadAoProc);
#ifdef CFW_MULTI_SIM
	CFW_GetSimID(hAo, &nSimId);
#endif

	//Zero memory
	SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
	//Get private date
	pSmsMOInfo = CFW_GetAoUserData(hAo);
	//Verify memory
	if(!pSmsMOInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 1,get private data fail! \n ");
		CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
		CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_MEMORY_FAILURE,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
		CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_MEMORY_FAILURE,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
		CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
		CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
		return ERR_NO_MORE_MEMORY;
	}

   //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
	
     CSW_TRACE(CFW_SMS_TS_ID,"sms_ReadAoProc, nState = 0x%x\n", nState);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
	{
		if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)  
#ifndef CFW_MULTI_SIM
			||(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_SR) 
			)
		{
			ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal);
#else
			)
		{
			ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal, nSimId);

#endif			
			if(ret == ERR_SUCCESS)
			{
				CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
			}
			else
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 3,get private data fail! \n ");
				CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
				CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
				return ERR_CMS_UNKNOWN_ERROR;
			}
			break;
		}
		else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  
#ifndef CFW_MULTI_SIM
			 ||(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)  \
			 ||(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR)
#endif			 
			 )
		{
			//Malloc for the read data.
			sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
			if(!(UINT8*)sMeEvent.nParam1)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc sMeEvent.nParam1 malloc error!!! \n ");
				CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_MEMORY_FULL, pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_MEMORY_FULL, pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
				CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
				return ERR_NO_MORE_MEMORY;
			}
			SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
			ret = CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
			CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc,5, pSmsMOInfo->nIndex is %d, sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",
				pSmsMOInfo->nIndex,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);
			if(ret == ERR_SUCCESS)
			{
				//Go to CFW_SM_STATE_WAIT directly!
				CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
				pEvent = &sMeEvent;
				pEvent->nTransId = pSmsMOInfo->nUTI_Internal;
			}
			else  //other return value
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 6,CFW_MeReadMessage, ret =0x%x\n ", ret);
				CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
				CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
				CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
				return ERR_CMS_UNKNOWN_ERROR;
			}
		}
	}
        case CFW_SM_STATE_WAIT:
	{
		if(((pEvent->nEventId == EV_CFW_SIM_READ_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)) && \
			(pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
		{
			UINT32 sOutData=0x0;
			if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc \n ");
				if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) || (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME))
				{
					CFW_SMS_MULTIPART_INFO sLongerMsg;
					//Zero memory
					SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
					
					//Just get SMS status
					ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,0,NULL,&nSmsStatus,NULL);
					if(ret != ERR_SUCCESS || nSmsStatus == 0)
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 7,sms_tool_DecomposePDU, ret =0x%x\n ", ret);
						CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_INVALID_CHARACTER_IN_PDU,pSmsMOInfo->nIndex,nGetUti|(nSimId<< 24),0xf0);
#endif
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);

            if( NULL != (VOID*)(pEvent->nParam1) )
            {
              CSW_SMS_FREE(pEvent->nParam1);
              pEvent->nParam1 = (UINT32)NULL;
            }
						CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
						CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
						return ERR_CMS_UNKNOWN_ERROR;
					}
					//Parse the data
					CSW_TRACE(CFW_SMS_TS_ID, "befor sms_mo_Parse_PDU, nSmsStatus=0x%x, pEvent->nType=0x%x\n ", nSmsStatus, pEvent->nType);
					ret = sms_mo_Parse_PDU(pSmsMOInfo->nFormat,pSmsMOInfo->nPath,nSmsStatus,pSmsMOInfo->nLocation,(PVOID)pEvent->nParam1,&pEvent->nType,&sOutData,&sLongerMsg);
					CSW_TRACE(CFW_SMS_TS_ID, "after sms_mo_Parse_PDU, nSmsStatus=0x%x, ret=0x%x, sLongerMsg.count=%d\n ", nSmsStatus, ret, sLongerMsg.count);

					CSW_TRACE(CFW_SMS_TS_ID, "after sms_mo_Parse_PDU, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);

					if(ret != ERR_SUCCESS)//other data, such as Status report or Command
					{
						//[[xueww[mod] 2007.07.30
						CSW_TRACE(CFW_SMS_TS_ID, "sms_mo_Parse_PDU, error ret =0x%x\n ", ret);
						CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
						if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
						{
							CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
						}
            else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
            {
              if( NULL != (VOID*)pEvent->nParam1 )
              {
                CSW_SMS_FREE(pEvent->nParam1);
                pEvent->nParam1 = (UINT32)NULL;
              }
            }
						CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
						CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
						
						CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
						return ERR_CMS_UNKNOWN_ERROR;
					}
					if(sLongerMsg.count > 0) // a longer msg
					{
						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 222, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
						CSW_TRACE(CFW_SMS_TS_ID,"sms_ReadAoProc, 9,The message is a long message!!!,the count is %d\r\n",sLongerMsg.count);
						UINT16 nConcatSmsNextRec=0x0;

						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 333, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
						((CFW_SMS_NODE*)sOutData)->nConcatPrevIndex = sLongerMsg.id;
						nConcatSmsNextRec = sLongerMsg.current;
						nConcatSmsNextRec = (nConcatSmsNextRec << 8) | sLongerMsg.count;
						((CFW_SMS_NODE*)sOutData)->nConcatNextIndex = nConcatSmsNextRec;
						//]]xueww[mod] 2007.03.14
						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 444, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
					}
					else // a normal short message
					{
						((CFW_SMS_NODE*)sOutData)->nConcatPrevIndex = 0xffff ;
						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 555, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
					}
					
					//Set value
					((CFW_SMS_NODE*)sOutData)->nConcatCurrentIndex   = pSmsMOInfo->nIndex;
					CSW_TRACE(CFW_SMS_TS_ID, "33, nSmsStatus=0x%x, ret=0x%x, sLongerMsg.count=%d\n ", nSmsStatus, ret, sLongerMsg.count);
					((CFW_SMS_NODE*)sOutData)->nStatus               = nSmsStatus;
					((CFW_SMS_NODE*)sOutData)->nType                 = pEvent->nType;
					((CFW_SMS_NODE*)sOutData)->nStorage              = pSmsMOInfo->nLocation;

					//add by qidd for bug 13605	---begin
					pSmsMOInfo->sOutData                             = ((CFW_SMS_NODE*)sOutData);
					//add by qidd for bug 13605	---end
					
					CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 666, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);

					//If the unread SMS is read by this function, the SMS message status will be changed to read status by CFW automaticly
					if(nSmsStatus == CFW_SMS_STORED_STATUS_UNREAD)
					{
						CFW_EV  sMeEvent1;
						CFW_SMS_STORAGE_INFO  sStorageInfo;

						//Zero memory
						SUL_ZeroMemory8(&sMeEvent1,SIZEOF(CFW_EV));
						SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));

						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 777, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
#ifndef CFW_MULTI_SIM
						CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
						CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 888, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
						
						if(sStorageInfo.unReadRecords > 0)
							sStorageInfo.unReadRecords--;
						if(sStorageInfo.readRecords < sStorageInfo.totalSlot)
							sStorageInfo.readRecords++;
#ifndef CFW_MULTI_SIM
						CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
						CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif

						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 999, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
						
						*((UINT8*)pEvent->nParam1) = CFW_SMS_STORED_STATUS_READ;

						if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, AAA, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
							//Write it with CFW_SMS_STORED_STATUS_READ
#ifndef CFW_MULTI_SIM
							ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
							ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
							CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, BBB, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
						}
						else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
						{
							UINT32 nTime=0x0, nAppInt32=0x0;
							
							CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, CCC, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
#ifndef CFW_MULTI_SIM
							ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent1);
#else
							ret = CFW_MeWriteMessage(nSimId,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent1);
#endif
							CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, DDD, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);
							CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 10,CFW_MeWriteMessage ,pSmsMOInfo->nIndex is %d, sMeEvent1.nParam1 is (0x)%x , sMeEvent1.nParam2 is (0x)%x , sMeEvent1.nType is (0x)%x\n ",pSmsMOInfo->nIndex,sMeEvent1.nParam1,sMeEvent1.nParam2,sMeEvent1.nType);
						}
						
						if(ret != ERR_SUCCESS)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 11,save status write me/sim ret=0x%x\n ", ret);
							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti|(nSimId<<24),0xf0);
#endif
							if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
							{
								CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
							}
              else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
              {
								CSW_SMS_FREE((UINT8*)pEvent->nParam1);
              }
              

							CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
							return ERR_CMS_UNKNOWN_ERROR;
						}
						CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, FFF, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);

						pSmsMOInfo->nReadSmsStatus = CFW_SMS_STORED_STATUS_UNREAD;
					}
					if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
					{
            if(sMeEvent.nParam1)
            {
              CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
              sMeEvent.nParam1 = (UINT32)NULL;
            }
          }	
          else if( EV_CFW_SIM_READ_MESSAGE_RSP == pEvent->nEventId)
          {
            if(pEvent->nParam1)
            {
              CSW_SMS_FREE((UINT8*)(pEvent->nParam1));
              pEvent->nParam1 = (UINT32)NULL;
            }			
          } 		  
					CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, GGG, dcs = 0x%x \n ", ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)sOutData)->pNode)->dcs);

					if(sLongerMsg.count > 0) // a longer msg
					{
						//[[xueww[mod] 2007.06.13
						pEvent->nParam2 = sLongerMsg.current;
						pEvent->nParam2 =( (pEvent->nParam2) << 16) |sLongerMsg.count;
						//]]xueww[mod] 2007.06.13
					}
					else
					{
						pEvent->nParam2 = 0;
					}
					CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 11.5pEvent->nParam2=0x%x\n ", pEvent->nParam2);
					
					pEvent->nType = 0;
				}
#ifndef CFW_MULTI_SIM
				else // read Status Report or Broadcast message
				{
          if( EV_CFW_SIM_READ_MESSAGE_RSP == pEvent->nEventId)
          {
            if(pEvent->nParam1)
            {
              CSW_SMS_FREE((UINT8*)(pEvent->nParam1));
              pEvent->nParam1 = (UINT32)NULL;
            }			
          } 		  
					CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 12,not surport this kind of storage\n ");
					//??? later
					CFW_GetUTI(hAo, &nGetUti);
					CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
					CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
					CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
					return ERR_CMS_UNKNOWN_ERROR;
				}
#endif				
			}
			//Read a NULL SIM
			else if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1 == 0))
			{
				sOutData = ERR_CMS_INVALID_MEMORY_INDEX;
				pEvent->nParam2 = pSmsMOInfo->nIndex;
				pEvent->nType = 0xf0;
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
			CFW_GetUTI(hAo, &nGetUti);
			
			CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 13,post event to mmi, param1 =0x%x, param2=0x%x, type = 0x%x\n ", 
				sOutData,pEvent->nParam2,pEvent->nType);
			
			//modify by qidd for bug 13605	---begin
			if(!((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)  &&  (pSmsMOInfo->nReadSmsStatus == CFW_SMS_STORED_STATUS_UNREAD)))
			{
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,sOutData,pEvent->nParam2,nGetUti,pEvent->nType);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,sOutData,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
			//modify by qidd for bug 13605	---end
				CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc read CFW_UnRegisterAO\n ");
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
			}
		}		
		else if((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) && (pSmsMOInfo->nUTI_Internal == pEvent->nTransId))
		{
			if(pEvent->nType == 0)
			{//modify a UTI problem,have a branch without GET UTI from AO.
				CFW_GetUTI(hAo, &nGetUti);
			//modify by qidd for bug 13605	---begin
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP, (UINT32)(pSmsMOInfo->sOutData), pEvent->nParam2,nGetUti,pEvent->nType);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP, (UINT32)(pSmsMOInfo->sOutData), pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
			//modify by qidd for bug 13605	---end
			
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
			}
			else if(pEvent->nType == 0xf0)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 14,post event to mmi, param1 =0x%x, param2=0x%x, type = 0x%x\n ", 
					ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,pEvent->nType);
				CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti|(nSimId<<24),pEvent->nType);
#endif
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);

				//add by qidd for bug 13605	---begin
				CSW_SMS_FREE(pSmsMOInfo->sOutData);
				pSmsMOInfo->sOutData=NULL;

				//add by qidd for bug 13605	---end	
				
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
			}
#ifndef CFW_MULTI_SIM
			else
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_ReadAoProc, 15,post event to mmi, param1 =0x%x, param2=0x%x, type = 0x%x\n ", 
					ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,pEvent->nType);
				CFW_GetUTI(hAo, &nGetUti);
				CFW_PostNotifyEvent(EV_CFW_SMS_READ_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
				
				//add by qidd for bug 13605	---begin
				CSW_SMS_FREE(pSmsMOInfo->sOutData);
				pSmsMOInfo->sOutData=NULL;
				//add by qidd for bug 13605	---end	
				
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
			}
#endif			
		}

	}
	break;
	default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_ReadAoProc  error\n");
		break;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_ReadAoProc);
    return ERR_SUCCESS;
}


UINT32 sms_DleteAoProc(HAO hAo, CFW_EV* pEvent)
{
    UINT32           nState = 0x0,nGetUti=0x0,ret=0x0;    //MO State
    CFW_SMS_MO_INFO* pSmsMOInfo = NULL; //Point to MO private data.
    CFW_EV           sMeEvent;
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId= CFW_SIM_COUNT;
#endif
    CSW_PROFILE_FUNCTION_ENTER(sms_DleteAoProc);

    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));

    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsMOInfo)
    {
        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
        CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
        CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
        return ERR_NO_MORE_MEMORY;
    }
#ifdef CFW_MULTI_SIM
	CFW_GetSimID(hAo, &nSimId);
#endif

   //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
     CSW_TRACE(CFW_SMS_TS_ID,"sms_DleteAoProc, nState = 0x%x\n", nState);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
            {
                ret=0x0;
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
                {
                    if(pSmsMOInfo->nIndex) // delete by index, read it first, got its status
                    {
#ifndef CFW_MULTI_SIM
                        ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal);
#else
                        ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                        if(ret == ERR_SUCCESS)
                        {
                            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                        }
                        else
                        {
                            CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti | (nSimId<<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                    }
                    else //by status, get the number of specified status
                    {
#ifndef CFW_MULTI_SIM
                        ret = CFW_SimDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,pSmsMOInfo->nUTI_Internal);
#else
                        ret = CFW_SimDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                        if(ret == ERR_SUCCESS)
                        {
                            CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                        }
                        else 
                        {
                            CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti | (nSimId<<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                    }
                    break;
                }
                else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  
#ifndef CFW_MULTI_SIM
                    ||(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)   
                   ||(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR)
#endif                   
                   )
                {
                    if(pSmsMOInfo->nIndex) // delete by index, read it first, got its status
                    {
                        sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
                        if(!(UINT8*)sMeEvent.nParam1)
                        {
				CSW_TRACE(CFW_SMS_TS_ID, "sms_mt_Reg pSmsMOInfo malloc error!!! \n ");
                            CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_NO_MORE_MEMORY,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_NO_MORE_MEMORY,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
                            return ERR_NO_MORE_MEMORY;
                        }
                        SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
                        CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
                        CSW_TRACE(CFW_SMS_TS_ID, "---sms_DleteAoProc--CFW_MeReadMessage ,pSmsMOInfo->nIndex is %d , sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",pSmsMOInfo->nIndex ,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);
                        pEvent = &sMeEvent;
                        if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
                        {
                            //Just get SMS status
                            sms_tool_DecomposePDU((PVOID)pEvent->nParam1,0,NULL,&(pSmsMOInfo->nStatus),NULL);
                        }
                        //Read a NULL SIM
                        else  
                        {
                            CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
                            CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                        CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
                    }
                    else //by status, get the number of specified status
                    {
                        sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_STORAGE_INFO)));
                        if((UINT8*)sMeEvent.nParam1 == NULL)
                        {
				CSW_TRACE(CFW_SMS_TS_ID, "sms_DleteAoProc sMeEvent.nParam1 malloc error!!! \n ");
                            CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_NO_MORE_MEMORY,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_NO_MORE_MEMORY,pSmsMOInfo->nIndex,nGetUti | (nSimId<<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                        SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1, SIZEOF(CFW_SMS_STORAGE_INFO));
                        //Get ME totalSlot and usedSlot
#ifndef CFW_MULTI_SIM
                        CFW_MeGetStorageInfo(CFW_SMS_STORAGE_ME,pSmsMOInfo->nStatus,&sMeEvent);
#else
                        CFW_MeGetStorageInfo(nSimId, pSmsMOInfo->nStatus,&sMeEvent);
#endif
                        CSW_TRACE(CFW_SMS_TS_ID, "---sms_DleteAoProc--CFW_MeGetStorageInfo ,pSmsMOInfo->nStatus is %d , sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",pSmsMOInfo->nStatus ,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);
                     
                        CSW_TRACE(CFW_SMS_TS_ID, "After CFW_MeGetStorageInfo usedSlot = %d \n ",((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->usedSlot);
                        CSW_TRACE(CFW_SMS_TS_ID, "After CFW_MeGetStorageInfo totalSlot = %d \n ",((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->totalSlot);
                        CSW_TRACE(CFW_SMS_TS_ID, "After CFW_MeGetStorageInfo unReadRecords = %d \n ",((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->unReadRecords);
                        CSW_TRACE(CFW_SMS_TS_ID, "After CFW_MeGetStorageInfo readRecords = %d \n ",((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->readRecords);
                        CSW_TRACE(CFW_SMS_TS_ID, "After CFW_MeGetStorageInfo sentRecords = %d \n ",((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->sentRecords);
                        CSW_TRACE(CFW_SMS_TS_ID, "After CFW_MeGetStorageInfo unsentRecords = %d \n ",((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->unsentRecords);
                        CSW_TRACE(CFW_SMS_TS_ID, "After CFW_MeGetStorageInfo unknownRecords = %d \n ",((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->unknownRecords);
                        CSW_TRACE(CFW_SMS_TS_ID, "After CFW_MeGetStorageInfo storageId = %d \n ",((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->storageId);

                 
			if(sMeEvent.nType == 0) // use nListCount to record the number
				pSmsMOInfo->nListCount = ((CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1))->usedSlot;
			else
			{
				CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP, 0, pSmsMOInfo->nIndex, nGetUti,0);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP, 0, pSmsMOInfo->nIndex, nGetUti |(nSimId<<24),0);
#endif
				CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
				CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
				return ERR_SUCCESS;
			}
                           
                         CSW_TRACE(CFW_SMS_TS_ID, "Delete   ME message by status, pSmsMOInfo->nStatus =(0x) %x  \n ",pSmsMOInfo->nStatus);
                         CSW_TRACE(CFW_SMS_TS_ID, "Delete   ME message by status, pSmsMOInfo->nListCount =(0x) %x  \n ", pSmsMOInfo->nListCount );
                        CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
                    }
#ifndef CFW_MULTI_SIM
                    ret = CFW_MeDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,&sMeEvent);
#else
                    ret = CFW_MeDeleteMessage(nSimId, pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,&sMeEvent);
#endif
                    CSW_TRACE(CFW_SMS_TS_ID, "sms_DleteAoProc  CFW_MeDeleteMessage ,pSmsMOInfo->nIndex is %d ,pSmsMOInfo->nStatus is %d, sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",pSmsMOInfo->nIndex ,pSmsMOInfo->nStatus,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);
                    if(ret == ERR_SUCCESS)
                    {
                        //Go to CFW_SM_STATE_WAIT directly!
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                        pEvent = &sMeEvent;
                        pEvent->nTransId = pSmsMOInfo->nUTI_Internal;
                    }
                    else  //other return value
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<< 24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
            }
        case CFW_SM_STATE_WAIT:
            {
                // delete by index in SIM, after reading it, got its status      
                if((pEvent->nEventId == EV_CFW_SIM_READ_MESSAGE_RSP) && (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
                {
                    if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
                    {
                        //Just get SMS status
                        sms_tool_DecomposePDU((PVOID)pEvent->nParam1,0,NULL,&(pSmsMOInfo->nStatus),NULL);
                        if(pEvent->nParam1)
                        {
                          CSW_SMS_FREE((UINT8*)(pEvent->nParam1));
                          pEvent->nParam1 = (UINT32)NULL;
                        }			
                    }
                    //Read a NULL SIM or read fail
                    else 
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
#ifndef CFW_MULTI_SIM
                    ret = CFW_SimDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,pSmsMOInfo->nUTI_Internal);
#else
                    ret = CFW_SimDeleteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nStatus,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                    if(ret == ERR_SUCCESS)
                    {
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                    }
                    else 
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti|(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                    break;
                }
                else if(((pEvent->nEventId == EV_CFW_SIM_DELETE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_DELETE_MESSAGE_RSP)) && \
                        (pEvent->nTransId  == pSmsMOInfo->nUTI_Internal))
                {
                    UINT32 uDelCount = 0;//added by fengwei 20080715 for delete all wrong.
                   if(pEvent->nType == 0)
                    {
                        uDelCount = pEvent->nParam1;//added by fengwei 20080715 for delete all wrong
                        pEvent->nParam1 = pSmsMOInfo->nIndex;
                        //加上容错处理,SMS最大个数不超过10000
                        if(pEvent->nParam1 > 10000)
                        {
                            pEvent->nParam1 = ERR_CMS_INVALID_MEMORY_INDEX;
                            pEvent->nType = 0xf0;
                        }
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
                
                CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti,pEvent->nType);
#else
                CFW_PostNotifyEvent(EV_CFW_SMS_DELETE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);

		 /*modified by fengwei 20080709 begin
                we should post EV_CFW_SMS_DELETE_MESSAGE_RSP first, 
                then, if needed, post EV_CFW_SMS_INFO_IND*/
		 if (0 == pEvent->nType)
		 {
                        UINT8 nNewCount=0x0;
                        CFW_SMS_STORAGE_INFO  sStorageInfo;
                        CFW_SMS_LONG_INFO_NODE sSmsLongInfoNode;

                        //Delete SIM by status, get the number of specified status sms
                        if((pEvent->nEventId == EV_CFW_SIM_DELETE_MESSAGE_RSP) && (0 == pSmsMOInfo->nIndex))
                            pSmsMOInfo->nListCount = uDelCount;// (UINT16)pEvent->nParam1;modified by fengwei 20080715 for delete all wrong
                        CSW_TRACE(CFW_SMS_TS_ID, "Delete a message ,type =0, pSmsMOInfo->nIndex = %d  \n ", pSmsMOInfo->nIndex);
                        CSW_TRACE(CFW_SMS_TS_ID, "Delete a message ,type =0, pSmsMOInfo->nListCount = %d  \n ", pSmsMOInfo->nListCount);

                        SUL_ZeroMemory8(&sSmsLongInfoNode,SIZEOF(sSmsLongInfoNode));
                        if((0 == pSmsMOInfo->nIndex) && pSmsMOInfo->nListCount) //delete by status
                        {
#ifndef CFW_MULTI_SIM
                            ret = CFW_CfgGetSmsFullForNew(&nNewCount);
#else
                            ret = CFW_CfgGetSmsFullForNew(&nNewCount, nSimId);
#endif
                            if(nNewCount >= 1)
                            {
                                CSW_TRACE(CFW_SMS_TS_ID, "the value of nNewCount is %u\n",nNewCount);
                                CFW_GetUTI(hAo, &nGetUti);

				  //deleted by fengwei 20080715 maybe wo don't need notify
                                //CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,0,pSmsMOInfo->nLocation,nGetUti,0);
#ifndef CFW_MULTI_SIM
                                sms_mo_SmsPPSendMMAReq();
#else
                                sms_mo_SmsPPSendMMAReq(nSimId);
#endif
                                if(nNewCount >= pSmsMOInfo->nListCount)
                                    nNewCount = (UINT8)(nNewCount - pSmsMOInfo->nListCount);
                                else
                                    nNewCount = 0;
#ifndef CFW_MULTI_SIM
                                CFW_CfgSetSmsFullForNew(nNewCount);
#else
                                CFW_CfgSetSmsFullForNew(nNewCount, nSimId);
#endif
                            }
                            //Delete msg with status, then update CFW_SMS_STORAGE_INFO
                            SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
                            CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
                            CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
                            //当短信存储空间满时, 删除短信后(比如调用了CFW_SmsDeleteMessage函数),并且没有正在等待存储的短信,那么该参数为0.
                           if(sStorageInfo.usedSlot == sStorageInfo.totalSlot)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,0,pSmsMOInfo->nLocation,nGetUti,0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,0,pSmsMOInfo->nLocation,nGetUti |(nSimId<<24),0);
#endif
                            }
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
#ifndef CFW_MULTI_SIM
                            CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation); 
#else
                            CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId); 
#endif
                            //Update longer sms para if it is
                            sSmsLongInfoNode.nConcatCurrentIndex = pSmsMOInfo->nIndex;
                            sSmsLongInfoNode.nStatus = pSmsMOInfo->nStatus;
#if USING_FS_SAVE_LONG_PARA                        
                            sms_tool_SetLongMSGPara((UINT8)pSmsMOInfo->nLocation,(UINT8*)&sSmsLongInfoNode,0,LONG_SMS_PARA_DELETE);
#endif
                        }
						
			  //modified by fengwei 20080704  for bug 8714 
                        //else if(pSmsMOInfo->nStatus) 
                        else if (0 != pSmsMOInfo->nIndex)//delete by index
                        {
#ifndef CFW_MULTI_SIM
                            ret = CFW_CfgGetSmsFullForNew(&nNewCount);
#else
                            ret = CFW_CfgGetSmsFullForNew(&nNewCount, nSimId);
#endif
                            if(nNewCount >= 1)
                            {
                                CSW_TRACE(CFW_SMS_TS_ID, "the value of nNewCount is %u\n",nNewCount);
                                CFW_GetUTI(hAo, &nGetUti);

				  //deleted by fengwei 20080715 maybe wo don't need notify
                                //CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,0,pSmsMOInfo->nLocation,nGetUti,0);
#ifndef CFW_MULTI_SIM
                                sms_mo_SmsPPSendMMAReq();
                                nNewCount--;
                                CFW_CfgSetSmsFullForNew(nNewCount);
                            }
                            //Delete a msg, then update CFW_SMS_STORAGE_INFO
                            SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                            CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                            //当短信存储空间满时, 删除短信后(比如调用了CFW_SmsDeleteMessage函数),并且没有正在等待存储的短信,那么该参数为0.
                            if(sStorageInfo.usedSlot == sStorageInfo.totalSlot)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
                                CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,0,pSmsMOInfo->nLocation,nGetUti,0);
                            }
#else
                                sms_mo_SmsPPSendMMAReq(nSimId);
                                nNewCount--;
                                CFW_CfgSetSmsFullForNew(nNewCount, nSimId);
                            }
                            //Delete a msg, then update CFW_SMS_STORAGE_INFO
                            SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
                            CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
                            //当短信存储空间满时, 删除短信后(比如调用了CFW_SmsDeleteMessage函数),并且没有正在等待存储的短信,那么该参数为0.
                            if(sStorageInfo.usedSlot == sStorageInfo.totalSlot)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
                                CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,0,pSmsMOInfo->nLocation,nGetUti |(nSimId<<24),0);
                            }
#endif
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
#ifndef CFW_MULTI_SIM
                            CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);                        
#else
                            CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
                            //Update longer sms para if it is
                            sSmsLongInfoNode.nConcatCurrentIndex = pSmsMOInfo->nIndex;
                            sSmsLongInfoNode.nStatus = pSmsMOInfo->nStatus;

                        }
                    }
		     //modified by fengwei 20080709 end.
		     
                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
            }
            break;
        }
	default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_DleteAoProc  error\n");
		break;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_DleteAoProc);
    return ERR_SUCCESS;
}

UINT32 sms_ListAoProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32           nGetUti=0x0,nState = 0x0,ret=0x0;    //MO State
	CFW_SMS_MO_INFO* pSmsMOInfo = NULL; //Point to MO private data.
	CFW_EV           sMeEvent;
    CFW_SMS_STORAGE_INFO  sStorageInfo;

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId= CFW_SIM_COUNT;
#endif
    CSW_PROFILE_FUNCTION_ENTER(sms_ListAoProc);

    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
#ifdef CFW_MULTI_SIM
	CFW_GetSimID(hAo, &nSimId);
#endif
    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsMOInfo)
    {
        CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
        CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
        return ERR_NO_MORE_MEMORY;
    }

   //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
	
//modify by qidd for bug 13978	---begin
#ifndef CFW_MULTI_SIM
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_ME);
#else
        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_ME, nSimId);
#endif
//modify by qidd for bug 13978	---end

     CSW_TRACE(CFW_SMS_TS_ID,"sms_ListAoProc, nState = 0x%x, location = 0x%x,usedslot = 0x%x\n",
							     nState,pSmsMOInfo->nLocation,sStorageInfo.usedSlot);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
        {
#ifndef CFW_MULTI_SIM
            if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
            {
                //??? Think about later

                CFW_GetUTI(hAo, &nGetUti);
                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                return ERR_CMS_UNKNOWN_ERROR;
            }
            else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)  				
                ||(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_SR))
            {
                ret = CFW_SimListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,pSmsMOInfo->nListCount,pSmsMOInfo->nListStartIndex,pSmsMOInfo->nUTI_Internal);
#else
            if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) 
            {
                ret = CFW_SimListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,pSmsMOInfo->nListCount,pSmsMOInfo->nListStartIndex,pSmsMOInfo->nUTI_Internal, nSimId);

#endif                    
                if(ret == ERR_SUCCESS)
                {
                    CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                }
				
#ifndef CFW_MULTI_SIM
                else if(ret == ERR_CFW_UTI_IS_BUSY)
                {
                    CFW_GetUTI(hAo, &nGetUti);

                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
#endif
                else
                {
                    CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId <<24),0xf0);
#endif
                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
            break;
            }
            else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)   
#ifndef CFW_MULTI_SIM
                   || (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)   
                   || (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR)
#endif                       
                   )
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
								CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc pListOutData malloc error!!! \n ");
                                CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                                return ERR_NO_MORE_MEMORY;
                            }
                            SUL_ZeroMemory8(pListOutData, SMS_MO_ONE_PDU_SIZE+4+4);

                            sMeEvent.nParam1 = (UINT32)pListOutData;
                            pEvent = &sMeEvent;
                        }
#ifndef CFW_MULTI_SIM
                        ret = CFW_MeListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
#else
                        ret = CFW_MeListMessage(nSimId,pSmsMOInfo->nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
#endif
                        CSW_TRACE(CFW_SMS_TS_ID,"CFW_MeListMessage ret[0x%x]\r\n",ret);
                        CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc  CFW_MeListMessage ,pSmsMOInfo->nStatus is %d ,pSmsMOInfo->nListStartIndex is %d, sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",
							pSmsMOInfo->nStatus ,pSmsMOInfo->nListStartIndex,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);

                        //(ret == ERR_SUCCESS) means that CFW_MeListMessage returns success
                        //pEvent->nParam2 & 0xffff0000) == 0 :  means the sms got is not the last one
                        //pEvent->nParam2  == 0x10001        :  means to get the last(max index) sms in db
                        if((ret == ERR_SUCCESS) &&  \
                           (((pEvent->nParam2 & 0xffff0000) == 0) || (pEvent->nParam2  == 0x10001)))
                        {
                            UINT32 nPduOutData=0x0;
                            //Just get the size and status of each PDU
                            ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,8,&nPduOutData,&nStatus,&sLongerMsg);
                            if(ret != ERR_SUCCESS)
                            {
                                CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ret,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_SMS_FREE(pListOutData);
                                CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                                return ERR_CMS_UNKNOWN_ERROR;
                            }
                            nPduRealSize = ((CFW_SMS_PDU_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->nDataSize;
                            if(nPduRealSize > 200)
                                nPduRealSize = SMS_MO_ONE_PDU_SIZE;
                            CSW_SMS_FREE((UINT8*)nPduOutData);
                            nPduOutData = 0x0;

                            //Then Get the sLongerMsg
                            if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || \
                                (nStatus == CFW_SMS_STORED_STATUS_READ) )
                            {
                                sms_tool_DecomposePDU((PVOID)pEvent->nParam1,2,&nPduOutData,&nStatus,&sLongerMsg);
                                CSW_SMS_FREE((UINT8*)nPduOutData);
                            }//Submitted message.
                            else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || 
                                    (nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || 
                                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || 
                                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || 
                                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
                            {
                                //Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
                                //Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO  
                                sms_tool_DecomposePDU((PVOID)pEvent->nParam1,4,&nPduOutData,&nStatus,&sLongerMsg);
                                CSW_SMS_FREE((UINT8*)nPduOutData);
                            }

                            //??? Using sLongerMsg

                            //Malloc the real size for a PDU, 1 means to add status byte
                            pPduDataWithRealSize = (UINT8*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_NODE) + nPduRealSize + 1);
                            if(!pPduDataWithRealSize)
                            {
								CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc pPduDataWithRealSize malloc error!!! \n ");
                                CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                                return ERR_NO_MORE_MEMORY;
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
				 					CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc pListAddr malloc error!!! \n ");
                                    CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                    CSW_SMS_FREE(pListOutData);
                                    CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                                    return ERR_NO_MORE_MEMORY;
                                }
                                SUL_ZeroMemory8(pListAddr, (4*pSmsMOInfo->nListCount + 4));
                                pSmsMOInfo->nListAddr = (UINT32)pListAddr;
                            }

                            //Set addr(i) each time
                            *((UINT32*)pSmsMOInfo->nListAddr + pSmsMOInfo->i + 1) = (UINT32)pPduDataWithRealSize;
                        }
                        if(ret != ERR_SUCCESS)
                        {
                            CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM 
                            CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_SMS_FREE(pListOutData);
                            CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }

                        //(H16bit)pEvent->nParam2: 0 means continue to list; 1 means list over .
                        //(L16bit)pEvent->nParam2: The index listed.
                        //Continue to list
                        //pEvent->nParam2 & 0xffff0000) == 0 :  means the sms got is not the last one
                        //pEvent->nParam2  == 0x10001        :  means to get the last(max index) sms in db
                        if(((pEvent->nParam2 & 0xffff0000) == 0) || (pEvent->nParam2  == 0x10001))
                        {
                            nTmp = (UINT32)pPduDataWithRealSize;
                            //Set value and copy data
                            if(sLongerMsg.count > 0) // a longer msg
                            {
                                UINT16 nConcatSmsNextRec=0x0;
								//[[xueww[mod] 2007.03.14
								((CFW_SMS_NODE*)nTmp)->nConcatPrevIndex = sLongerMsg.id;
								nConcatSmsNextRec = sLongerMsg.current;
								nConcatSmsNextRec = (nConcatSmsNextRec << 8) | sLongerMsg.count;
								((CFW_SMS_NODE*)nTmp)->nConcatNextIndex = nConcatSmsNextRec;
								//]]xueww[mod] 2007.03.14
                            }
                            else // a normal short message
                            {
                                ((CFW_SMS_NODE*)nTmp)->nConcatPrevIndex  =   0xffff;
                            }

							//Set value
                            ((CFW_SMS_NODE*)nTmp)->nConcatCurrentIndex   =  (UINT16)(pEvent->nParam2 & 0x0000ffff);
                            if(pEvent->nParam2  == 0x10001)
                        	{
			                    CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc nListStartIndex=%d \n ",pSmsMOInfo->nListStartIndex);
#ifdef CFW_MULTI_SIM
								//modify by qidd for bug 13978
						 ((CFW_SMS_NODE*)nTmp)->nConcatCurrentIndex   = pSmsMOInfo->nListStartIndex + (nSimId)*(SMS_ME_MAX_INDEX/(CFW_SIM_COUNT));

								/*
							if (1 == sStorageInfo.usedSlot )
	                                             ((CFW_SMS_NODE*)nTmp)->nConcatCurrentIndex   = pSmsMOInfo->nListStartIndex + (nSimId)*(SMS_ME_MAX_INDEX/(CFW_SIM_COUNT));
							else
	                                             ((CFW_SMS_NODE*)nTmp)->nConcatCurrentIndex   = pSmsMOInfo->nListStartIndex;
	                                             */
								//modify by qidd for bug 13978	---end

#else
                                ((CFW_SMS_NODE*)nTmp)->nConcatCurrentIndex   = SMS_ME_MAX_INDEX;
#endif
                        	}
                            ((CFW_SMS_NODE*)nTmp)->nStatus  =   nStatus;
                            ((CFW_SMS_NODE*)nTmp)->nStorage =   pSmsMOInfo->nLocation;
                            //Pointer to 
                            ((CFW_SMS_NODE*)nTmp)->pNode    =  (UINT8*)((UINT8*)nTmp + SIZEOF(CFW_SMS_NODE));
                            //copy the pdu with (real size +1), including status byte
                            SUL_MemCopy8((UINT8*)((CFW_SMS_NODE*)nTmp)->pNode, (UINT8*)((UINT8*)pEvent->nParam1),(nPduRealSize+1));

                            pSmsMOInfo->nListStartIndex = (UINT16)((pEvent->nParam2 & 0x0000ffff) + 1);
		                    CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc pSmsMOInfo->nListStartIndex=%d \n ",pSmsMOInfo->nListStartIndex);
                            //Get the size of list PDU(add total)
                            pSmsMOInfo->nListSize = (UINT16)(pSmsMOInfo->nListSize + nPduRealSize + SIZEOF(CFW_SMS_NODE) + 1);
                        }
                        
                        //got the last one
                        //pEvent->nParam2 & 0xffff0000) == 0x00010000 : 用户要求的count大于或等于该状态sms实际存在的count, 列到最后一个(非max index的最后的一个)
                        //pEvent->nParam2  == 0x10001                 : 用户要求的count大于或等于该状态sms实际存在的count, 列到最后一个(max index的最后的一个)
                        //(pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))
                        //                                            : 用户要求的count小于该状态sms实际存在的count, 列到需求的最后一个
                        if( ((pEvent->nParam2 & 0xffff0000) == 0x00010000) || 
                            (pEvent->nParam2  == 0x10001)                  || 
                            (((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))) )
                        {
                            CFW_GetUTI(hAo, &nGetUti);
                            //if list over and the comment is null
                            if((pSmsMOInfo->i == 0) && ( *((UINT8*)pEvent->nParam1) == 0))
                            {

#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                CSW_SMS_FREE((UINT32*)pSmsMOInfo->nListAddr);
                            }
                            else
                            {
                                //Set addr count
                                if((pEvent->nParam2 & 0xffff0000) == 0x00010000)
                                    *((UINT32*)pSmsMOInfo->nListAddr) = (UINT32)(pSmsMOInfo->i);
                                if(pEvent->nParam2  == 0x10001)
                                {                                        
                                    *((UINT32*)pSmsMOInfo->nListAddr) = (UINT32)(pSmsMOInfo->i + 1);
                                    pSmsMOInfo->nListStartIndex = SMS_ME_MAX_INDEX +1;
                                }
                                if(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))) 
                                    *((UINT32*)pSmsMOInfo->nListAddr) = (UINT32)(pSmsMOInfo->i + 1);
                                pEvent->nParam2 = (UINT32)(*((UINT32*)pSmsMOInfo->nListAddr)<<16) + (pSmsMOInfo->nListStartIndex-1);

#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,pSmsMOInfo->nListAddr ,pEvent->nParam2,nGetUti,pEvent->nType);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,pSmsMOInfo->nListAddr ,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                            }
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            if(pListOutData !=  NULL)
                                CSW_SMS_FREE(pListOutData);
                            CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                            return ERR_SUCCESS;
                        }
                    }//End for(pSmsMOInfo->i=0;pSmsMOInfo->i<pSmsMOInfo->nListCount;pSmsMOInfo->i++)
                }
                else//(pSmsMOInfo->nListCount == 0)
                {
#ifndef CFW_MULTI_SIM
                    ret = CFW_MeListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,0,pSmsMOInfo->nListStartIndex,&sMeEvent);
#else
                    ret = CFW_MeListMessage(nSimId, pSmsMOInfo->nStatus,0,pSmsMOInfo->nListStartIndex,&sMeEvent);
#endif
                    CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc CFW_MeListMessage ,pSmsMOInfo->nStatus is %d ,pSmsMOInfo->nListStartIndex is %d, sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",pSmsMOInfo->nStatus ,pSmsMOInfo->nListStartIndex,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);

                    if(ret == ERR_SUCCESS)
                    {
                        CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,0,sMeEvent.nParam2,nGetUti,sMeEvent.nType);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,0,sMeEvent.nParam2,nGetUti |(nSimId <<24),sMeEvent.nType);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                        return ERR_SUCCESS;
                    }
                    else 
                    {
                        CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
            }
        }
        case CFW_SM_STATE_WAIT:
        {
            if((pEvent->nEventId == EV_CFW_SIM_LIST_MESSAGE_RSP) && (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
            {
                if(pEvent->nType == 0)
                {

                    if(pSmsMOInfo->nListCount != 0x0) 
                    {
                        //(HOUINT16)pEvent->nParam2 == 0: indicate list operation don’t complete， so there are more SMS to list.
                        //(HOUINT16)pEvent->nParam2 == 1: indicate list operation complete.
                        //if not the last one
                        if((pEvent->nParam2 & 0xffff0000) == 0)
                        {
                            UINT8*  pPduDataWithRealSize=NULL; 
                            UINT8   nStatus=0x0;
                            UINT16  nPduRealSize=0x0; 
                            UINT32  nPduOutData=0x0;
                            UINT32  nTmp=0x0;
                            CFW_SMS_MULTIPART_INFO sLongerMsg;

                            //Zero memory
                            SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
                            //Just get the size ,status of each PDU
                            ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,8,&nPduOutData,&nStatus,&sLongerMsg);
                            if(ret == ERR_SUCCESS)
                            {
                                nPduRealSize = ((CFW_SMS_PDU_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->nDataSize;
                                if(nPduRealSize > 200)
                                    nPduRealSize = SMS_MO_ONE_PDU_SIZE;
                                CSW_SMS_FREE((UINT8*)nPduOutData);
                            }
                            else// maybe Status report or EMS
                            {
                                //pSmsMOInfo->i++;
    								            CSW_TRACE(CFW_SMS_TS_ID, "sms_tool_DecomposePDU Failed Maybe a status report\n ");
              								if(pEvent->nParam1)
              								{
              									CSW_SMS_FREE((VOID *) pEvent->nParam1);
              									pEvent->nParam1 = (UINT32)NULL;
              								}

                                break;//xueww[mod] 2007.09.04

                                //nPduRealSize = SMS_MO_ONE_PDU_SIZE;
                                //nStatus      = 0;
//                                    CFW_GetUTI(hAo, &nGetUti);
//                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
//                                    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
//                                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
//                                    return ERR_CMS_UNKNOWN_ERROR;
                            }
                            nPduOutData  = 0x0;

                            //Then Get the sLongerMsg
                            if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || \
                                (nStatus == CFW_SMS_STORED_STATUS_READ) )
                            {
                                ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,2,&nPduOutData,&nStatus,&sLongerMsg);
                                if(nPduOutData)
                                    CSW_SMS_FREE((UINT8*)nPduOutData);
                            }//Submitted message.
                            else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || 
                                    (nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || 
                                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || 
                                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || 
                                    (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
                            {
                                //Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
                                //Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO  
                                ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,4,&nPduOutData,&nStatus,&sLongerMsg);
                                if(nPduOutData)
                                    CSW_SMS_FREE((UINT8*)nPduOutData);
                            }
							//[[xueww[-] 2007.09.04
							/*
                            else// nStatus = 0;
                            {
                                nStatus = CFW_SMS_STORED_STATUS_READ;
                            }
							*/
							//]]xueww[-] 2007.09.04

                           //Malloc the real size for a PDU, 1 means to add status byte
                            pPduDataWithRealSize = (UINT8*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_NODE) + nPduRealSize + 1);
                            if(!pPduDataWithRealSize)
                            {
                								CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc pPduDataWithRealSize malloc error!!! \n ");
                								if(pEvent->nParam1)
                								{
                									CSW_SMS_FREE((VOID *) pEvent->nParam1);
                									pEvent->nParam1 = (UINT32)NULL;
                								}

                                CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti|(nSimId<<24),0xf0);
#endif
                                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                                return ERR_NO_MORE_MEMORY;
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

                  									if(pEvent->nParam1)
                  									{
                  										CSW_SMS_FREE((VOID *) pEvent->nParam1);
                  										pEvent->nParam1 = (UINT32)NULL;
                  									}
                  									CSW_TRACE(CFW_SMS_TS_ID, "sms_ListAoProc pListAddr malloc error!!! \n ");
                                    CFW_GetUTI(hAo, &nGetUti);

#ifndef CFW_MULTI_SIM
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
                                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                    CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                                    return ERR_NO_MORE_MEMORY;
                                }
                                SUL_ZeroMemory8(pListAddr, (4*pSmsMOInfo->nListCount + 4));
                                pSmsMOInfo->nListAddr = (UINT32)pListAddr;
                            }

                            //Set addr(i) each time
                            *((UINT32*)pSmsMOInfo->nListAddr + pSmsMOInfo->i + 1) = (UINT32)pPduDataWithRealSize;

                            nTmp = (UINT32)pPduDataWithRealSize;
                            if(sLongerMsg.count > 0) // a longer msg
                            {
                                UINT16 nConcatSmsNextRec=0x0;
                								//[[xueww[mod] 2007.03.14
                                                ((CFW_SMS_NODE*)nTmp)->nConcatPrevIndex = sLongerMsg.id;
                								nConcatSmsNextRec = sLongerMsg.current;
                								nConcatSmsNextRec = (nConcatSmsNextRec << 8) | sLongerMsg.count;
                                                ((CFW_SMS_NODE*)nTmp)->nConcatNextIndex = nConcatSmsNextRec;
                								//]]xueww[mod] 2007.03.14
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
                            pSmsMOInfo->i++;
                        }
                        
                        //got the last one
                        if( ((pEvent->nParam2 & 0xffff0000) == 0x00010000) ||  
                                 (((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount))) )
                        {
                            CFW_GetUTI(hAo, &nGetUti);
                            //if list over and the comment is null
                            if((pSmsMOInfo->i == 0) && ( pEvent->nParam1 == 0))
                            {

#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                CSW_SMS_FREE((UINT32*)pSmsMOInfo->nListAddr);
                            }
                            else
                            {
                                //Set addr count
                                *((UINT32*)pSmsMOInfo->nListAddr) = (UINT32)pSmsMOInfo->i;
//                                    pEvent->nParam2 = (UINT32)((pSmsMOInfo->nListSize)<<16) + (pSmsMOInfo->nListStartIndex-1);
                                pEvent->nParam2 = (UINT32)((pSmsMOInfo->i)<<16) + (pSmsMOInfo->nListStartIndex-1);
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,pSmsMOInfo->nListAddr ,pEvent->nParam2,nGetUti,pEvent->nType);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,pSmsMOInfo->nListAddr ,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                            }
                            CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);

              							if(pEvent->nParam1)
              							{
              								CSW_SMS_FREE((VOID *) pEvent->nParam1);
              								pEvent->nParam1 = (UINT32)NULL;
              							}
                            return ERR_SUCCESS;
                        }
            						else//added by fengwei 20080701, we should free the event when the list not comlete
            						{
                
            						    CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
            						}
                    }
                    else //count is 0
                    {
            						if(pEvent->nParam1)
            						{
            							CSW_SMS_FREE((VOID *) pEvent->nParam1);
            							pEvent->nParam1 = (UINT32)NULL;
            						}

                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,0,pEvent->nParam2,nGetUti,pEvent->nType);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,0,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
                        return ERR_SUCCESS;
                    }//end if(pSmsMOInfo->nListCount)

					if(pEvent->nParam1)
					{
						CSW_SMS_FREE((VOID *) pEvent->nParam1);
						pEvent->nParam1 = (UINT32)NULL;
					}


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
                    else if(pEvent->nParam1 == ERR_CMS_INVALID_STATUS)
                    {
                        pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
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
                    CFW_GetUTI(hAo, &nGetUti);
					CSW_TRACE(CFW_SMS_TS_ID,"sms_ListAoProc  pEvent->nParam1=0x%x\n",pEvent->nParam1);
#ifndef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,pEvent->nParam1,0,nGetUti,pEvent->nType);
#else
                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_RSP,pEvent->nParam1,0,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                }
            }
			break;
		}
		default:
			CSW_TRACE(CFW_SMS_TS_ID,"sms_ListAoProc  error\n");
			break;
	}
    CSW_PROFILE_FUNCTION_EXIT(sms_ListAoProc);
    return ERR_SUCCESS;
}

UINT32 sms_CopyAoProc(HAO hAo, CFW_EV* pEvent)
{
    UINT32           nGetUti=0x0,nState = 0x0,ret=0x0;    //MO State
    CFW_SMS_MO_INFO* pSmsMOInfo = NULL; //Point to MO private data.
    CFW_EV        sMeEvent;
    UINT8            nStatus=0x0,nTmp=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_CopyAoProc);
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId=CFW_SIM_COUNT;
	CFW_GetSimID(hAo, &nSimId);
#endif

    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    nStatus = pSmsMOInfo->nStatus;//added by fengwei 20080625 for bug 8709
    //Verify memory
    if(!pSmsMOInfo)
    {
        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24), 0xf0);
#endif
        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
        CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
        return ERR_NO_MORE_MEMORY;
    }

   //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
     CSW_TRACE(CFW_SMS_TS_ID,"sms_CopyAoProc, nState = 0x%x\n", nState);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
            {
               if(pSmsMOInfo->nListOption == CFW_SMS_COPY_ME2SM) 
               {
                    if((pEvent == (CFW_EV*)0xffffffff) || (pEvent->nType == 0))
                    {
                        CFW_SMS_MULTIPART_INFO sLongerMsg;

                        //Zero memory
                        SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
                        if(pSmsMOInfo->i == 0)
                        {
                            //This is a temp memory. just for CFW_MeListMessage.
                            pSmsMOInfo->pData = (UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4);
                            if(!pSmsMOInfo->pData)
                            {
					CSW_TRACE(CFW_SMS_TS_ID, "sms_CopyAoProc pSmsMOInfo->pData malloc error!!! \n ");
                                CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                                return ERR_NO_MORE_MEMORY;
                            }
                            SUL_ZeroMemory8(pSmsMOInfo->pData, SMS_MO_ONE_PDU_SIZE+4+4);
                        }
                        sMeEvent.nParam1 = (UINT32)(pSmsMOInfo->pData);
                        pEvent = &sMeEvent;

#ifndef CFW_MULTI_SIM
                        ret = CFW_MeListMessage(CFW_SMS_STORAGE_ME,nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
#else
                        ret = CFW_MeListMessage(nSimId, nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
#endif
                        if(ret != ERR_SUCCESS)
                        {
                            //Free the memory 
                            CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                            CFW_GetUTI(hAo, &nGetUti);
                            if(pSmsMOInfo->i)
                            {
								//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
								//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti,0);
                            }
                            else
                            {
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                            }
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti |(nSimId<<24),0);
                            }
                            else
                            {
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
                            }

#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                        if(sMeEvent.nType == 0)
                        {
                            //(H16bit)pEvent->nParam2: 0 means continue to list; 1 means list over .
                            //(L16bit)pEvent->nParam2: The index listed.
                            //Continue to list
                            if((ret == ERR_SUCCESS) && ((pEvent->nParam2 & 0xffff0000) == 0))
                            {
                                UINT8 uStatus=0x0;
                                //Just get SMS status
                                sms_tool_DecomposePDU((UINT8*)(pEvent->nParam1),0,NULL,&uStatus,NULL);
                                *((UINT8*)(pEvent->nParam1)) = uStatus;
#ifndef CFW_MULTI_SIM
                                ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM,0,(UINT8*)(pEvent->nParam1),SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
                                ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM,0,(UINT8*)(pEvent->nParam1),SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                                if(ret == ERR_SUCCESS )
                                {
                                    CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                                }
                                else
                                {
                                    //Free the memory 
                                    CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                                    CFW_GetUTI(hAo, &nGetUti);
                                    if(pSmsMOInfo->i)
                                    {
										//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                        pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
										//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti,0);
                                    }
                                    else
                                    {
                                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                                    }
#else
                                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti |(nSimId<<24),0);
                                    }
                                    else
                                    {
                                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId <<24),0xf0);
                                    }

#endif
                                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                    CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                                    return ERR_CMS_UNKNOWN_ERROR;
                                }
                                pSmsMOInfo->i++;
                                pSmsMOInfo->nListStartIndex = (UINT16)((pEvent->nParam2 & 0x0000ffff) + 1);
                            }
                            //got the last one,post msg
                            if(pSmsMOInfo->nListCount)
                                  //deleted by fengwei 20080626 for copy msg 2 less than expected
				   //nTmp = (UINT8)(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1)));
			          nTmp = (UINT8)(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount + 1)));
                            else
                                nTmp = 0;
                            if( ((pEvent->nParam2 & 0xffff0000) == 0x00010000) ||  nTmp)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
                                //if list over and the comment is null
                                if((pSmsMOInfo->i == 0) && ( *((UINT8*)pEvent->nParam1) == 0))
                                {
                                    //Free the memory 
                                    CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
#ifndef CFW_MULTI_SIM
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti,0xf0);
#else
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti |(nSimId<<24),0xf0);
#endif
                                }
                                else
                                {
                                    //Free the memory 
                                    CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                                    if((pEvent->nParam2 & 0xffff0000) == 0x00010000)
                                        pEvent->nParam1 = (UINT32)(pSmsMOInfo->i);
                                    if(nTmp) 
                                        pEvent->nParam1 = (UINT32)(pSmsMOInfo->i + 1);
									//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                    pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
									//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pEvent->nParam1 ,pEvent->nParam2,nGetUti,pEvent->nType);
#else
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pEvent->nParam1 ,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                                }
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                                return ERR_SUCCESS;
                            }
                        }
                        else if(sMeEvent.nType == 0xf0)
                        {
                            //Free the memory 
                            CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                            CFW_GetUTI(hAo, &nGetUti);
                            if(pSmsMOInfo->i)
                            {
								//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
								//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti,0);
                            }
                            else
                            {
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti,0xf0);
                            }
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti |(nSimId<<24),0);
                            }
                            else
                            {
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti |(nSimId<<24),0xf0);
                            }

#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                    }
                    if(pEvent->nType == 0xf0)
                    {
                        //Free the memory 
                        CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                        CFW_GetUTI(hAo, &nGetUti);
                        if(pSmsMOInfo->i)
                        {
							//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                            pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-2);
							//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,(pSmsMOInfo->i-1),pEvent->nParam2,nGetUti,0);
                        }
                        else
                        {
                            CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti,0xf0);
                        }
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,(pSmsMOInfo->i-1),pEvent->nParam2,nGetUti |(nSimId<<24),0);
                        }
                        else
                        {
                            CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti |(nSimId<<24),0xf0);
                        }

#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
                else if(pSmsMOInfo->nListOption == CFW_SMS_COPY_SM2ME)
                {
                    if(pSmsMOInfo->nListCount)
                        nStatus = pSmsMOInfo->nStatus;
                    else
                    {
                        CFW_SMS_STORAGE_INFO  sStorageInfo;
                        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
                        //Get max slot in SIM
#ifndef CFW_MULTI_SIM
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
#else
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM, nSimId);
#endif
                        nStatus = 0x7f;
                        pSmsMOInfo->nListCount = sStorageInfo.totalSlot;
                    }
#ifndef CFW_MULTI_SIM
                    ret = CFW_SimListMessage(CFW_SMS_STORAGE_SM,nStatus,pSmsMOInfo->nListCount,pSmsMOInfo->nListStartIndex,pSmsMOInfo->nUTI_Internal);
#else
                    ret = CFW_SimListMessage(CFW_SMS_STORAGE_SM,nStatus,pSmsMOInfo->nListCount,pSmsMOInfo->nListStartIndex,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                    if(ret == ERR_SUCCESS)
                    {
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                    }
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti|(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
                break;
            }
        case CFW_SM_STATE_WAIT:
            {
                if((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) && (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
                {
                    if(pEvent->nType == 0)
                    {
                        //added by fengwei 20080626 for missing update of StorageInfo begin
                        CFW_SMS_STORAGE_INFO  sStorageInfo;
		         UINT8 nLastMsgStatus = 0;
			sms_tool_DecomposePDU(pSmsMOInfo->pData, 0, NULL, &nLastMsgStatus, NULL);
		         //Update storage info, this is rsp of writing msg to sm, so we update sm storage
			 SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
		        CFW_CfgGetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM);		     
#else
		        CFW_CfgGetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM, nSimId);		     
#endif
			
			 if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
				sStorageInfo.usedSlot++;
			 if(CFW_SMS_STORED_STATUS_UNREAD == nLastMsgStatus)
				sStorageInfo.unReadRecords++;
			 else if(CFW_SMS_STORED_STATUS_READ == nLastMsgStatus)
				sStorageInfo.readRecords++;
			 else if(CFW_SMS_STORED_STATUS_UNSENT == nLastMsgStatus)
				sStorageInfo.unsentRecords++;
			 else if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == nLastMsgStatus)        || 
			    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == nLastMsgStatus)   || 
			    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == nLastMsgStatus)  || 
			    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == nLastMsgStatus))
				sStorageInfo.sentRecords++;
			 CSW_TRACE(CFW_SMS_TS_ID, "usedSlot = %u, unReadRecords = %u, readRecords = %u, unsentRecords = %u, sentRecords = %u\t\n",
			 	                  sStorageInfo.usedSlot, sStorageInfo.unReadRecords, sStorageInfo.readRecords, sStorageInfo.unsentRecords, sStorageInfo.sentRecords);
#ifndef CFW_MULTI_SIM
			 CFW_CfgSetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM);
#else
			 CFW_CfgSetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM, nSimId);
#endif
		        //added by fengwei 20080626 for missing update of StorageInfo end
			  
                        //0: Save to specified memory location and the memory is not full. 1:full
                        pSmsMOInfo->nPath = (UINT8)(pEvent->nParam2);

                        sMeEvent.nParam1 = (UINT32)(pSmsMOInfo->pData);
                        pEvent = &sMeEvent;
#ifndef CFW_MULTI_SIM
                        ret = CFW_MeListMessage(CFW_SMS_STORAGE_ME,nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
#else
                        ret = CFW_MeListMessage(nSimId, nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
#endif
                        if(ret != ERR_SUCCESS)
                        {
                            //Free the memory 
                            CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                            CFW_GetUTI(hAo, &nGetUti);
                            if(pSmsMOInfo->i)
                            {
								//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
								//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti,0);
                            }
                            else
                            {
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                            }
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti |(nSimId<<24),0);
                            }
                            else
                            {
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
                            }
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                        if(sMeEvent.nType == 0)
                        {
                            //(H16bit)pEvent->nParam2: 0 means continue to list; 1 means list over .
                            //(L16bit)pEvent->nParam2: The index listed.
                            //Continue to list
                            if((ret == ERR_SUCCESS) && ((pEvent->nParam2 & 0xffff0000) == 0))
                            {
                                UINT8 uStatus=0x0;
                                //Just get SMS status
                                sms_tool_DecomposePDU((UINT8*)(pSmsMOInfo->pData),0,NULL,&uStatus,NULL);
                                *((UINT8*)(pEvent->nParam1)) = uStatus;
#ifndef CFW_MULTI_SIM
                                ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM,0,(UINT8*)(pEvent->nParam1),SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
                                ret = CFW_SimWriteMessage(CFW_SMS_STORAGE_SM,0,(UINT8*)(pEvent->nParam1),SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                                if(ret == ERR_SUCCESS )
                                {
                                    //deleted by fengwei 20080627 for copy msg wrong, this moment we still have some msg to copy, and we have send req to sim, 
                                    //so we should stay in waiting for rsp state.
                                    //CFW_SetAoState(hAo, CFW_SM_STATE_IDLE);why should we set Ao state to IDLE?
                                }
                                else
                                {
                                    //Free the memory 
                                    CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                                    CFW_GetUTI(hAo, &nGetUti);
                                    if(pSmsMOInfo->i)
                                    {
										//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                        pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
										//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti,0);
                                    }
                                    else
                                    {
                                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                                    }
#else
                                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti |(nSimId<<24),0);
                                    }
                                    else
                                    {
                                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
                                    }

#endif
                                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                    CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                                    return ERR_CMS_UNKNOWN_ERROR;
                                }
                                pSmsMOInfo->i++;
                                pSmsMOInfo->nListStartIndex = (UINT16)((pEvent->nParam2 & 0x0000ffff) + 1);
                            }
                            //got the last one,post msg
                            if(pSmsMOInfo->nListCount)
                                  //modify by fengwei 20080626 for copy msg 2 less than expected, this moment we have just receive rsp of last write req,
                                  //but i have been counted one more than the count of rsp, so we must compare i with nListCount + 1
				   //nTmp = (UINT8)(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1)));
			          nTmp = (UINT8)(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount + 1)));
                            else
                                nTmp = 0;
			      CSW_TRACE(CFW_SMS_TS_ID, "after CFW_SimWriteMessage, pEvent->nParam2 = %x\n", pEvent->nParam2);
                            if( ((pEvent->nParam2 & 0xffff0000) == 0x00010000) ||  nTmp)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
                                //if list over and the comment is null
                                if((pSmsMOInfo->i == 0) && ( *((UINT8*)pEvent->nParam1) == 0))
                                {
                                    //Free the memory 
                                    CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
#ifndef CFW_MULTI_SIM
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti,0xf0);
#else
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti |(nSimId <<24),0xf0);
#endif
                                }
                                else
                                {
                                    //Free the memory 
                                    CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                                    if((pEvent->nParam2 & 0xffff0000) == 0x00010000)
                                        pEvent->nParam1 = (UINT32)(pSmsMOInfo->i);
                                    if(nTmp) 
                                        pEvent->nParam1 = (UINT32)(pSmsMOInfo->i + 1);
									//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                    pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
									//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pEvent->nParam1 ,pEvent->nParam2,nGetUti,pEvent->nType);
#else
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pEvent->nParam1 ,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                                }
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                                return ERR_SUCCESS;
                            }
                        }
                        else if(sMeEvent.nType == 0xf0)
                        {
                            //Free the memory 
                            CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                            CFW_GetUTI(hAo, &nGetUti);
                            if(pSmsMOInfo->i)
                            {
								//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
								//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti,0);
                            }
                            else
                            {
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti,0xf0);
                            }
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i,pEvent->nParam2,nGetUti |(nSimId<<24),0);
                            }
                            else
                            {
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti |(nSimId<<24),0xf0);
                            }

#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                    }
                    else if(pEvent->nType == 0xf0)
                    {
                        //Free the memory 
                        CSW_SMS_FREE((UINT8*)(pSmsMOInfo->pData));
                        CFW_GetUTI(hAo, &nGetUti);
                        if(pSmsMOInfo->i)
                        {
							//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                            pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-2);
							//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,(pSmsMOInfo->i-1),pEvent->nParam2,nGetUti,0);
                        }
                        else
                        {
                            CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti,0xf0);
                        }
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,(pSmsMOInfo->i-1),pEvent->nParam2,nGetUti |(nSimId<<24),0);
                        }
                        else
                        {
                            CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti |(nSimId<<24),0xf0);
                        }

#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
                if((pEvent->nEventId == EV_CFW_SIM_LIST_MESSAGE_RSP) && (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
                {
                    if(pEvent->nType == 0)
                    {
                        if((pEvent->nParam2 & 0xffff0000) == 0)
                        {
                            UINT32 nTime=0x0, nAppInt32=0x0;
                            UINT8  uStatus=0x0;
                            //Just get SMS status
                            sms_tool_DecomposePDU((UINT8*)(pEvent->nParam1),0,NULL,&uStatus,NULL);
                            *((UINT8*)(pEvent->nParam1)) = uStatus;
#ifndef CFW_MULTI_SIM
                            ret = CFW_MeWriteMessage(CFW_SMS_STORAGE_ME,0,(UINT8*)(pEvent->nParam1),SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#else
                            ret = CFW_MeWriteMessage(nSimId, 0, (UINT8*)(pEvent->nParam1),SMS_MO_ONE_PDU_SIZE, nTime, nAppInt32,&sMeEvent);
#endif
                            if(ret == ERR_SUCCESS)
                            {
                                if(sMeEvent.nType == 0)
                                {
                                    //added by fengwei 20080626 for missing update of StorageInfo begin
	                                CFW_SMS_STORAGE_INFO  sStorageInfo;
			                //Update storage info, this is rsp of writing msg to ME, so we update sm storage
					 SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
				        CFW_CfgGetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_ME);		     
#else
				        CFW_CfgGetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_ME, nSimId);    
#endif
					
					 if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
						sStorageInfo.usedSlot++;
					 if(CFW_SMS_STORED_STATUS_UNREAD == nStatus)
						sStorageInfo.unReadRecords++;
					 else if(CFW_SMS_STORED_STATUS_READ == nStatus)
						sStorageInfo.readRecords++;
					 else if(CFW_SMS_STORED_STATUS_UNSENT == nStatus)
						sStorageInfo.unsentRecords++;
					 else if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == nStatus)        || 
					    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == nStatus)   || 
					    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == nStatus)  || 
					    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == nStatus))
						sStorageInfo.sentRecords++;
					 CSW_TRACE(CFW_SMS_TS_ID, "usedSlot = %u, unReadRecords = %u, readRecords = %u, unsentRecords = %u, sentRecords = %u\t\n",
					 	                  sStorageInfo.usedSlot, sStorageInfo.unReadRecords, sStorageInfo.readRecords, sStorageInfo.unsentRecords, sStorageInfo.sentRecords);
#ifndef CFW_MULTI_SIM
					 CFW_CfgSetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_ME);
#else
					 CFW_CfgSetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_ME, nSimId);
#endif
				        //added by fengwei 20080626 for missing update of StorageInfo end
                                    pSmsMOInfo->nPath = (UINT8)(sMeEvent.nParam2);
                                }
                                else if(sMeEvent.nType == 0xf0)
                                {
									//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                    sMeEvent.nParam2 = (UINT32)((sMeEvent.nParam2 << 16) + (pSmsMOInfo->nListStartIndex-1));
									//]]xueww[mod] 2007.03.02
                                    CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i ,sMeEvent.nParam2,nGetUti,sMeEvent.nType);
#else
                                    CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pSmsMOInfo->i ,sMeEvent.nParam2,nGetUti |(nSimId<<24),sMeEvent.nType);
#endif
                                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                    CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                                    return ERR_CMS_UNKNOWN_ERROR;
                                }
                            }
                            else
                            {
                                CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,sMeEvent.nParam2,nGetUti,sMeEvent.nType);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,sMeEvent.nParam2,nGetUti |(nSimId<<24), sMeEvent.nType);
#endif
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                                return ERR_CMS_UNKNOWN_ERROR;
                            }
                            pSmsMOInfo->i++;
                            pSmsMOInfo->nListStartIndex = (UINT16)((pEvent->nParam2 & 0x0000ffff) + 1);
                        }
                        //got the last one,post msg
                        if(pSmsMOInfo->nListCount)
                            nTmp = (UINT8)(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == pSmsMOInfo->nListCount));
                        else
                            nTmp = 0;
                        if(((pEvent->nParam2 & 0xffff0000) == 0x00010000) ||  nTmp)
                        {
                            CFW_GetUTI(hAo, &nGetUti);
                            //if list over and the comment is null
                            if((pSmsMOInfo->i == 0) && (pEvent->nParam1 == 0))
                            {
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti |(nSimId<<24),0xf0);
#endif
                            }
                            else
                            {
                                //Free the memory 
                                CSW_SMS_FREE((UINT8*)(pEvent->nParam1));
                                pEvent->nParam1 = (UINT32)(pSmsMOInfo->i);
								//[[xueww[mod] 2007.03.02 //'>>' ====> '<<'
                                pEvent->nParam2 = (UINT32)((pSmsMOInfo->nPath << 16) + pSmsMOInfo->nListStartIndex-1);
								//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pEvent->nParam1 ,pEvent->nParam2,nGetUti,pEvent->nType);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,pEvent->nParam1 ,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                            }
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                            return ERR_SUCCESS;
                        }
                    }
                    else if(pEvent->nType == 0xf0)
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_COPY_MESSAGE_RSP,ERR_CMS_INVALID_MEMORY_INDEX,0,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
                break;
            }
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_CopyAoProc  error\n");
		break;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_CopyAoProc);
    return ERR_SUCCESS;
}

#ifndef CFW_MULTI_SIM
UINT32 sms_ListHeaderAoProc(HAO hAo, CFW_EV* pEvent)
{
    UINT32           nGetUti=0x0,nState = 0x0,ret=0x0;    //MO State
    CFW_SMS_MO_INFO* pSmsMOInfo = NULL; //Point to MO private data.
    CFW_EV        sMeEvent;
    CSW_PROFILE_FUNCTION_ENTER(sms_ListHeaderAoProc);

    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));

    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsMOInfo)
    {
        CFW_GetUTI(hAo, &nGetUti);
        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
        CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
        return ERR_NO_MORE_MEMORY;
    }

   //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
     CSW_TRACE(CFW_SMS_TS_ID,"sms_ListHeaderAoProc, nState = 0x%x\n", nState);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
            {
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
                {
                    //??? Think about later
                    CFW_GetUTI(hAo, &nGetUti);
                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
                else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)  || 
                   (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_SR))
               {
                    ret = CFW_SimListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,pSmsMOInfo->nListCount,pSmsMOInfo->nListStartIndex,pSmsMOInfo->nUTI_Internal);
                    if(ret == ERR_SUCCESS)
                    {
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                    }
                    else if(ret == ERR_CFW_UTI_IS_BUSY)
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                break;
                }
                else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  || 
                        (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)  || 
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
					CSW_TRACE(CFW_SMS_TS_ID, "sms_ListHeaderAoProc pListOutData malloc error!!! \n ");
                                    CFW_GetUTI(hAo, &nGetUti);
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
                                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                    CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                                    return ERR_NO_MORE_MEMORY;
                                }
                                SUL_ZeroMemory8(pListOutData, SMS_MO_ONE_PDU_SIZE+4+4);
                                sMeEvent.nParam1 = (UINT32)pListOutData;
                                pEvent = &sMeEvent;
                            }
                            ret = CFW_MeListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,1,pSmsMOInfo->nListStartIndex,&sMeEvent);
                            CSW_TRACE(CFW_SMS_TS_ID, "sms_ListHeaderAoProc  CFW_MeListMessage ,pSmsMOInfo->nStatus is %d ,pSmsMOInfo->nListStartIndex is %d, sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",pSmsMOInfo->nStatus ,pSmsMOInfo->nListStartIndex,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);
                            if((ret == ERR_SUCCESS) &&  
                               (((pEvent->nParam2 & 0xffff0000) == 0) || (pEvent->nParam2  == 0x10001)))
//                            if((ret == ERR_SUCCESS) && ((pEvent->nParam2 & 0xffff0000) == 0))
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
						CSW_TRACE(CFW_SMS_TS_ID, "sms_ListHeaderAoProc pListAddr malloc error!!! \n ");
                                        CFW_GetUTI(hAo, &nGetUti);
                                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
                                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                        CSW_SMS_FREE(pListOutData);
                                        CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                                        return ERR_NO_MORE_MEMORY;
                                    }
                                    SUL_ZeroMemory8(pListAddr, (pSmsMOInfo->nListCount*SIZEOF(CFW_SMS_HEADER)));
                                    pSmsMOInfo->nListAddr = (UINT32)pListAddr;
                                }
                                pSmsHeader = (CFW_SMS_HEADER*)((UINT8*)pSmsMOInfo->nListAddr + pSmsMOInfo->i*SIZEOF(CFW_SMS_HEADER));
                            }
                            if(ret != ERR_SUCCESS)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
                                CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_SMS_FREE(pListOutData);
                                CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                                return ERR_CMS_UNKNOWN_ERROR;
                            }

                            //(H16bit)pEvent->nParam2: 0 means continue to list; 1 means list over .
                            //(L16bit)pEvent->nParam2: The index listed.
                            //Continue to list
                            if(((pEvent->nParam2 & 0xffff0000) == 0) || (pEvent->nParam2  == 0x10001))
//                            if((pEvent->nParam2 & 0xffff0000) == 0)
                            {
                                UINT8   nStatus=0x0;
                                UINT32  nPduOutData=0x0;
                                CFW_SMS_MULTIPART_INFO sLongerMsg;

                                //Zero memory
                                SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
                                //Just get SMS status
                                sms_tool_DecomposePDU(pListOutData,0,NULL,&nStatus,NULL);
                                if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || 
                                    (nStatus == CFW_SMS_STORED_STATUS_READ) )
                                {
                                    ret = sms_tool_DecomposePDU(pListOutData,2,&nPduOutData,&nStatus,&sLongerMsg);
                                    if(ret == ERR_SUCCESS)
                                    {
                                        SUL_MemCopy8(&(pSmsHeader->nTime),&(((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->scts),SIZEOF(TM_SMS_TIME_STAMP));
                                        SUL_MemCopy8(pSmsHeader->pTelNumber,((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->oa,TEL_NUMBER_MAX_LEN);
                                        pSmsHeader->nNumber_size = ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->oa_size;
                                        pSmsHeader->nTelNumberType = ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->tooa;
                                    }
                                    CSW_SMS_FREE((UINT8*)nPduOutData);
                                }//Submitted message.
                                else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || 
                                        (nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || 
                                        (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || 
                                        (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || 
                                        (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
                                {
                                    //Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
                                    //Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO  
                                    ret = sms_tool_DecomposePDU(pListOutData,4,&nPduOutData,&nStatus,&sLongerMsg);
                                    if(ret == ERR_SUCCESS)
                                    {
                                        SUL_MemCopy8(pSmsHeader->pTelNumber,((CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->da,TEL_NUMBER_MAX_LEN);
                                        pSmsHeader->nNumber_size   = ((CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->da_size;
                                        pSmsHeader->nTelNumberType = ((CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->toda;
                                    }
                                    CSW_SMS_FREE((UINT8*)nPduOutData);
                                }
                                //Set value and copy data
                                if(sLongerMsg.count > 0) // a longer msg
                                {
                                    UINT16 nConcatSmsPrevRec=0x0, nConcatSmsNextRec=0x0;
#if USING_FS_SAVE_LONG_PARA                        
                                    sms_tool_GetLongMSGPara((UINT8)pSmsMOInfo->nLocation,(UINT16)(pEvent->nParam2 & 0x0000ffff), &nConcatSmsPrevRec, &nConcatSmsNextRec);
#endif
                                    pSmsHeader->nConcatPrevIndex  =   nConcatSmsPrevRec;
                                    pSmsHeader->nConcatNextIndex  =   nConcatSmsNextRec;
                                }
                                else // a normal short message
                                {
                                    pSmsHeader->nConcatPrevIndex  =   0xffff;
                                }
                                //Set value
                                pSmsHeader->nConcatCurrentIndex   =  (UINT16)(pEvent->nParam2 & 0x0000ffff);
                                if(pEvent->nParam2  == 0x10001)
                                    pSmsHeader->nConcatCurrentIndex  = SMS_ME_MAX_INDEX;
                                pSmsHeader->nStatus  =   nStatus;
                                pSmsHeader->nStorage =   pSmsMOInfo->nLocation;
                                pSmsMOInfo->nListStartIndex = (UINT16)((pEvent->nParam2 & 0x0000ffff) + 1);
                            }
                            
                            //got the last one
                            if( ((pEvent->nParam2 & 0xffff0000) == 0x00010000) || 
                                (pEvent->nParam2  == 0x10001)                  || 
                                (((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))) )
                            {
                                CFW_GetUTI(hAo, &nGetUti);
                                //if list over and the comment is null
                                if((pSmsMOInfo->i == 0) && ( *((UINT8*)pEvent->nParam1) == 0))
                                {
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti,0xf0);
                                    CSW_SMS_FREE((UINT32*)pSmsMOInfo->nListAddr);
                                }
                                else
                                {
                                    //Set addr count
                                    UINT16 nArrayCount=0x0;
                                    if((pEvent->nParam2 & 0xffff0000) == 0x00010000)
                                        nArrayCount = pSmsMOInfo->i;
                                    if(pEvent->nParam2  == 0x10001)
                                    {                                        
                                        nArrayCount = (UINT32)(pSmsMOInfo->i + 1);
                                        pSmsMOInfo->nListStartIndex = SMS_ME_MAX_INDEX +1;
                                    }
                                    if(((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount-1))) 
                                        nArrayCount = (UINT16)(pSmsMOInfo->i + 1);
                                    pEvent->nParam2 = (UINT32)((nArrayCount<<16) + (pSmsMOInfo->nListStartIndex-1));
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,pSmsMOInfo->nListAddr,pEvent->nParam2,nGetUti,pEvent->nType);
                                }
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                if(pListOutData !=  NULL)
                                    CSW_SMS_FREE(pListOutData);
                                CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                                return ERR_SUCCESS;
                            }
                        }//End for(pSmsMOInfo->i=0;pSmsMOInfo->i<pSmsMOInfo->nListCount;pSmsMOInfo->i++)
                    }
                    else//(pSmsMOInfo->nListCount == 0)
                    {
                        ret = CFW_MeListMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nStatus,0,pSmsMOInfo->nListStartIndex,&sMeEvent);
                        CSW_TRACE(CFW_SMS_TS_ID, "sms_ListHeaderAoProc  CFW_MeListMessage ,pSmsMOInfo->nStatus is %d ,pSmsMOInfo->nListStartIndex is %d, sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",pSmsMOInfo->nStatus ,pSmsMOInfo->nListStartIndex,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);

                        if(ret == ERR_SUCCESS)
                        {
                            CFW_GetUTI(hAo, &nGetUti);
                            CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,0,sMeEvent.nParam2,nGetUti,sMeEvent.nType);
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                            return ERR_SUCCESS;
                        }
                        else 
                        {
                            CFW_GetUTI(hAo, &nGetUti);
                            CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                            return ERR_CMS_UNKNOWN_ERROR;
                        }
                    }
                }
            }
        case CFW_SM_STATE_WAIT:
            {
                if((pEvent->nEventId == EV_CFW_SIM_LIST_MESSAGE_RSP) && (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
                {
                    if(pEvent->nType == 0)
                    {
                        if(pSmsMOInfo->nListCount != 0x0) 
                        {
                            //(HOUINT16)pEvent->nParam2 == 0: indicate list operation don’t complete， so there are more SMS to list.
                            //(HOUINT16)pEvent->nParam2 == 1: indicate list operation complete.
                            //if not the last one
                            if((pEvent->nParam2 & 0xffff0000) == 0)
                            {
                                UINT8   nStatus=0x0;
                                UINT32  nPduOutData=0x0;
                                CFW_SMS_HEADER* pSmsHeader=NULL; 
                                CFW_SMS_MULTIPART_INFO sLongerMsg;

                                //Zero memory
                                SUL_ZeroMemory8(&sLongerMsg,SIZEOF(CFW_SMS_MULTIPART_INFO));
                                //Just do one time
                                if(pSmsMOInfo->i == 0)
                                {
                                    UINT32* pListAddr=NULL;
                                    pListAddr = (UINT32*)CSW_SMS_MALLOC(pSmsMOInfo->nListCount*SIZEOF(CFW_SMS_HEADER));
                                    if(!pListAddr)
                                    {
						CSW_TRACE(CFW_SMS_TS_ID, "sms_ListHeaderAoProc  pListAddr malloc error!!! \n ");
                                        CFW_GetUTI(hAo, &nGetUti);
                                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
                                        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
                                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                        CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                                        return ERR_NO_MORE_MEMORY;
                                    }
                                    SUL_ZeroMemory8(pListAddr, (pSmsMOInfo->nListCount*SIZEOF(CFW_SMS_HEADER)));
                                    pSmsMOInfo->nListAddr = (UINT32)pListAddr;
                                }
                                pSmsHeader = (CFW_SMS_HEADER*)((UINT8*)pSmsMOInfo->nListAddr + pSmsMOInfo->i*SIZEOF(CFW_SMS_HEADER));
                                //Just get SMS status,status of each PDU
                                ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,8,&nPduOutData,&nStatus,&sLongerMsg);
                                if(ret == ERR_SUCCESS)
                                    CSW_SMS_FREE((UINT8*)nPduOutData);
                                else// maybe Status report or EMS
                                    nStatus = 0;
                                nPduOutData = 0x0;
                                if((nStatus == CFW_SMS_STORED_STATUS_UNREAD) || 
                                    (nStatus == CFW_SMS_STORED_STATUS_READ) )
                                {
                                    ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,2,&nPduOutData,&nStatus,&sLongerMsg);
                                    if(ret == ERR_SUCCESS)
                                    {
                                        SUL_MemCopy8(&(pSmsHeader->nTime),&(((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->scts),SIZEOF(TM_SMS_TIME_STAMP));
                                        SUL_MemCopy8(&(pSmsHeader->pTelNumber),((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->oa,TEL_NUMBER_MAX_LEN);
                                        pSmsHeader->nNumber_size   = ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->oa_size;
                                        pSmsHeader->nTelNumberType = ((CFW_SMS_TXT_DELIVERED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->tooa;
                                    }
                                    if(nPduOutData)
                                        CSW_SMS_FREE((UINT8*)nPduOutData);
                                }//Submitted message.
                                else if((nStatus == CFW_SMS_STORED_STATUS_UNSENT)               || 
                                        (nStatus == CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)      || 
                                        (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || 
                                        (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV) || 
                                        (nStatus == CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE) )
                                {
                                    //Text Mode, Submitted message. when  CFW_CfgSetSmsShowTextModeParam(1)
                                    //Parse pEvent->nParam1 to CFW_SMS_TXT_SUBMITTED_NO_HRD_INFO  
                                    ret = sms_tool_DecomposePDU((PVOID)pEvent->nParam1,4,&nPduOutData,&nStatus,&sLongerMsg);
                                    if(ret == ERR_SUCCESS)
                                    {
                                        SUL_MemCopy8(pSmsHeader->pTelNumber,((CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->da,TEL_NUMBER_MAX_LEN);
                                        pSmsHeader->nNumber_size   = ((CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->da_size;
                                        pSmsHeader->nTelNumberType = ((CFW_SMS_TXT_SUBMITTED_WITH_HRD_INFO*)((CFW_SMS_NODE*)nPduOutData)->pNode)->toda;
                                    }
                                    if(nPduOutData)
                                        CSW_SMS_FREE((UINT8*)nPduOutData);
                                }
                                else
                                {
                                    nStatus = CFW_SMS_STORED_STATUS_READ;
                                }
                                
                                //Set value and copy data
                                if(sLongerMsg.count > 0) // a longer msg
                                {
                                    UINT16 nConcatSmsPrevRec=0x0, nConcatSmsNextRec=0x0;
#if USING_FS_SAVE_LONG_PARA                        
                                    sms_tool_GetLongMSGPara((UINT8)pSmsMOInfo->nLocation,(UINT16)(pEvent->nParam2 & 0x0000ffff), &nConcatSmsPrevRec, &nConcatSmsNextRec);
#endif
                                    pSmsHeader->nConcatPrevIndex  = nConcatSmsPrevRec;
                                    pSmsHeader->nConcatNextIndex  = nConcatSmsNextRec;
                                }
                                else // a normal short message
                                {
                                    pSmsHeader->nConcatPrevIndex  = 0xffff;
                                }
                                //Set value
                                pSmsHeader->nConcatCurrentIndex   = (UINT16)(pEvent->nParam2 & 0x0000ffff);
                                pSmsHeader->nStatus  =   nStatus;
                                pSmsHeader->nStorage =   pSmsMOInfo->nLocation;
                                pSmsMOInfo->i++;
                                pSmsMOInfo->nListStartIndex = (UINT16)((pEvent->nParam2 & 0x0000ffff) + 1);
                            }
                            
                            //got the last one
                            if( ((pEvent->nParam2 & 0xffff0000) == 0x00010000) ||  
                                     (((pEvent->nParam2 & 0xffff0000) == 0x0) && (pSmsMOInfo->i == (pSmsMOInfo->nListCount))) )
                            {
                                CFW_GetUTI(hAo, &nGetUti);
                                //if list over and the comment is null
                                if((pSmsMOInfo->i == 0) && ( pEvent->nParam1 == 0))
                                {
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,0,nGetUti,0xf0);
                                    CSW_SMS_FREE((UINT32*)pSmsMOInfo->nListAddr);
                                }
                                else
                                {
                                    pEvent->nParam2 = (UINT32)((pSmsMOInfo->i)<<16) + (pSmsMOInfo->nListStartIndex-1);
                                    CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,pSmsMOInfo->nListAddr,pEvent->nParam2,nGetUti,pEvent->nType);
                                }
                                CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                                return ERR_SUCCESS;
                            }
                        }
                        else //count is 0
                        {
                            CFW_GetUTI(hAo, &nGetUti);
                            CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,0,pEvent->nParam2,nGetUti,pEvent->nType);
                            CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
                            return ERR_SUCCESS;
                        }//end if(pSmsMOInfo->nListCount)

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
                        else if(pEvent->nParam1 == ERR_CMS_INVALID_STATUS)
                        {
                            pEvent->nParam1 = ERR_CMS_UNKNOWN_ERROR;
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
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_LIST_MESSAGE_HRD_RSP,pEvent->nParam1,0,nGetUti,pEvent->nType);
                        CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                }
            break;
        }
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_ListHeaderAoProc  error\n");
		break;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_ListHeaderAoProc);
    return ERR_SUCCESS;
}
#endif

UINT32 sms_WriteAoProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32           nState=0x0,nGetUti=0x0;    //MO State
	CFW_SMS_MO_INFO* pSmsMOInfo = NULL; //Point to MO private data.
	CFW_EV           sMeEvent,sComposeEvent;
	CSW_PROFILE_FUNCTION_ENTER(sms_WriteAoProc);
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId=CFW_SIM_COUNT;
	CFW_GetSimID(hAo, &nSimId);
#endif
	//Zero memory
	SUL_ZeroMemory8(&sComposeEvent,SIZEOF(CFW_EV));
	SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));

	//Get private date
	pSmsMOInfo = CFW_GetAoUserData(hAo);
	//Verify memory
	if(!pSmsMOInfo)
	{
		CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
		CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
		CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
		CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
		CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
		return ERR_NO_MORE_MEMORY;
	}

	//Get current State
	if(pEvent == (CFW_EV*)0xffffffff)
		nState = CFW_SM_STATE_IDLE;
	else 
		nState = CFW_GetAoState(hAo);
     CSW_TRACE(CFW_SMS_TS_ID,"sms_WriteAoProc, nState = 0x%x\n", nState);
	//SMS MO State machine process
	switch(nState)
	{
		case CFW_SM_STATE_IDLE:
		{
			UINT32 ret=0x0;
		    CSW_TRACE(CFW_SMS_TS_ID,"sms_WriteAoProc, nFormat = 0x%x\n, nLocation = 0x%x\n",
				          pSmsMOInfo->nFormat, pSmsMOInfo->nLocation);
			if(pSmsMOInfo->nFormat)  //Write Text
			{
				if(pEvent == (CFW_EV*)0xffffffff)  //First come
				{
#ifndef CFW_MULTI_SIM
					if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) ||  
						(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  
						 ||(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME) )
					{
#endif						 
						UINT8  nConcat=0x0;
						//should be deliver msg
						if((pSmsMOInfo->nStatus & CFW_SMS_STORED_STATUS_UNREAD) || 
							(pSmsMOInfo->nStatus & CFW_SMS_STORED_STATUS_READ))
						{
							nConcat =0x08;// 0000 1000   bit3=1:for write, bit0=0,bit1=0:Deliver
						} //should be submit msg
						else
						{
							nConcat =0x09; // 0000 1001  bit3=1:for write, bit0=1,bit1=0:Submit
						}
#ifndef CFW_MULTI_SIM
						ret = sms_tool_ComposePDU(nConcat,pSmsMOInfo->nStatus, pSmsMOInfo->pData,pSmsMOInfo->nDataSize,
						              &pSmsMOInfo->sNumber,&sComposeEvent);
					}
					else  //Write Status Report or Broadcast Message
					{
						sComposeEvent.nParam1 = (UINT32)(pSmsMOInfo->pData);
						sComposeEvent.nType   = 0x0;
						ret = ERR_SUCCESS;
					}
#else
						ret = sms_tool_ComposePDU(nConcat,pSmsMOInfo->nStatus, pSmsMOInfo->pData,pSmsMOInfo->nDataSize,
						              &pSmsMOInfo->sNumber, nSimId, &sComposeEvent);
#endif					
					if(ret == ERR_SUCCESS )
					{
						pSmsMOInfo->pTmpData_Addr = (UINT8*)(sComposeEvent.nParam1);
						pSmsMOInfo->pTmpPara_Addr = (UINT8*)(sComposeEvent.nParam2);
						if(sComposeEvent.nType == 0x0) //for a normal SMS
						{
							pSmsMOInfo->isLongerMsg = 0;  // a normal msg or the last part of a longer msg
							//To SIM
							if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)    
#ifndef CFW_MULTI_SIM
								||(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_SR)
#endif								
								)
							{
#ifndef CFW_MULTI_SIM
								ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)(sComposeEvent.nParam1),SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
								ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)(sComposeEvent.nParam1),
									SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif

								if(ret == ERR_SUCCESS )
								    	CFW_SetAoState(pSmsMOInfo->hAo, CFW_SM_STATE_WAIT);
								else
								    	goto flag_fail;
								break;
							}
							//To ME
							else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)   
#ifndef CFW_MULTI_SIM
							     ||  (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)   
							     ||  (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR) 
							   ||   (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
#endif							   
							   )
							{
								UINT32 nTime=0x0, nAppInt32=0x0;
#ifndef CFW_MULTI_SIM
								UINT8  nFlag_Storage_SM_ME=0x0;
								if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
								{
								    	nFlag_Storage_SM_ME = 1;
								    	pSmsMOInfo->nLocation = CFW_SMS_STORAGE_ME;
								}
								ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)(sComposeEvent.nParam1),SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#else
								ret = CFW_MeWriteMessage(nSimId, pSmsMOInfo->nIndex,(UINT8*)(sComposeEvent.nParam1),SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#endif
								CSW_TRACE(CFW_SMS_TS_ID, "sms_WriteAoProc  CFW_MeWriteMessage ,pSmsMOInfo->nIndex is %d , sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",
								        pSmsMOInfo->nIndex  ,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);
								if(ret == ERR_SUCCESS)
								{
									if(0 == sMeEvent.nType)
									{
										//Free the memory located by CFW_ComposePDU_ASYN
										//CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
										//Go to CFW_SM_STATE_WAIT directly!
										CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
										pEvent = &sMeEvent;
										pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
									}
									else if(0xf0 == sMeEvent.nType)
									{
#ifndef CFW_MULTI_SIM
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
											break;
										}
										else
#endif											
											goto flag_fail;
									}
								}
								else
								{
								    	goto flag_fail;
								}
							}
						}
						else if(CFW_SIM_SMS_SUPPORT_LONG_MSG && (sComposeEvent.nType == 0x01)) //for a longer SMS
						{
							ret=0x0;
							CFW_SMS_STORAGE_INFO  sStorageInfo;

							//sComposeEvent
							//pEvent->nParam1 指向一个176byte固定结构
							//pEvent->nParam2 为一个32位数组,each part is 8 bit,as following
							//   _____________________________________
							//  |           |           |          |            |
							//  | part A | part B | part C | part D  |
							//  |______|______|______|_______|
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

							CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_WriteAoProc Write StorageID is %d ! \n"),pSmsMOInfo->nLocation);
							CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_WriteAoProc Write a Longer sms, Total(Curr):%d (%d)! \n"),pSmsMOInfo->nLongerMsgCount,pSmsMOInfo->nLongerMsgCurrent);
							pSmsMOInfo->isLongerMsg = 1; //a longer msg
#ifndef CFW_MULTI_SIM
							//Verify storage
							if((pSmsMOInfo->nLocation != CFW_SMS_STORAGE_ME)       && 
								(pSmsMOInfo->nLocation != CFW_SMS_STORAGE_SM)       && 
								(pSmsMOInfo->nLocation != CFW_SMS_STORAGE_SM_ME))
							{
								goto flag_fail;
							}
#endif							
							//if add new, verify there is free slot or not                                   
							if(pSmsMOInfo->nIndex == 0) //Add new
							{
//dual sim mode, we only have storage ME and SM, we don't support any other storage type.							
#ifndef CFW_MULTI_SIM
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
											CFW_GetUTI(hAo, &nGetUti);
											CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_D0_SIM_SMS_STORAGE_FULL,0,nGetUti,0xf0);
											CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
											CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
											return ERR_CMS_UNKNOWN_ERROR;
										}
									}     
								}
								else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME) || 
								        (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) )
								{
#endif								
									//Verify the free slot
									SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
									CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
									CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
									CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_WriteAoProc has free slot: %d ! \n"),(sStorageInfo.totalSlot - sStorageInfo.usedSlot));
									if(pSmsMOInfo->nLongerMsgCount > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
									{
                    if( CFW_SMS_STORAGE_ME == pSmsMOInfo->nLocation )
                    {
                      pSmsMOInfo->nLocation = CFW_SMS_STORAGE_SM;
    									SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
    									CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
    									CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
    									CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_WriteAoProc has free slot: %d ! \n"),(sStorageInfo.totalSlot - sStorageInfo.usedSlot));
    									if(pSmsMOInfo->nLongerMsgCount > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
    									{
    										CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
    										CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
    										CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
    										CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_D0_SIM_SMS_STORAGE_FULL,0,nGetUti,0xf0);
#else
    										CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_D0_SIM_SMS_STORAGE_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
    										CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
    										CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
    										return ERR_CMS_UNKNOWN_ERROR;
                      }
                    }
                    else 
                    {
                      pSmsMOInfo->nLocation = CFW_SMS_STORAGE_ME;
    									SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
    									CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
    									CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
    									CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_WriteAoProc has free slot: %d ! \n"),(sStorageInfo.totalSlot - sStorageInfo.usedSlot));
    									if(pSmsMOInfo->nLongerMsgCount > (sStorageInfo.totalSlot - sStorageInfo.usedSlot))
    									{
    										CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
    										CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
    										CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
    										CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_D0_SIM_SMS_STORAGE_FULL,0,nGetUti,0xf0);
#else
    										CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_D0_SIM_SMS_STORAGE_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
    										CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
    										CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
    										return ERR_CMS_UNKNOWN_ERROR;
                      }
                    }
                    
									}     
#ifndef CFW_MULTI_SIM
								}
								else
								{
								    	goto flag_fail;
								}
#endif								
							}// end of if(pSmsMOInfo->nIndex == 0)

							//Write long msg to ME
							if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)//To ME
							{
								UINT32 nTime=0x0, nAppInt32=0x0;
								UINT8 i=0x0;
								for(i=0;i<pSmsMOInfo->nLongerMsgCount;i++)
								{
#ifndef CFW_MULTI_SIM
									ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->pTmpData,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#else
									ret = CFW_MeWriteMessage(nSimId, pSmsMOInfo->nIndex,pSmsMOInfo->pTmpData,
										SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#endif
									CSW_TRACE(CFW_SMS_TS_ID, " sms_WriteAoProc  CFW_MeWriteMessage long,pSmsMOInfo->nIndex is %d , sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",pSmsMOInfo->nIndex  ,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);
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
#ifndef CFW_MULTI_SIM
											CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
											CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
											if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
												sStorageInfo.usedSlot++;
											if(CFW_SMS_STORED_STATUS_UNREAD == pSmsMOInfo->nStatus)
												sStorageInfo.unReadRecords++;
											else if(CFW_SMS_STORED_STATUS_READ == pSmsMOInfo->nStatus)
												sStorageInfo.readRecords++;
											else if(CFW_SMS_STORED_STATUS_UNSENT == pSmsMOInfo->nStatus)
												sStorageInfo.unsentRecords++;
											else if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == pSmsMOInfo->nStatus)        || 
												(CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == pSmsMOInfo->nStatus)   || 
												(CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == pSmsMOInfo->nStatus)  || 
												(CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == pSmsMOInfo->nStatus))
												sStorageInfo.sentRecords++;
#ifndef CFW_MULTI_SIM
											CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
											CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
											//Malloc CFW_SMS_LONG_INFO for pStru_SmsLongInfo
											pTmpSmsLongInfo = (CFW_SMS_LONG_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_LONG_INFO));
											if(!pTmpSmsLongInfo)
											{
												CSW_TRACE(CFW_SMS_TS_ID, "sms_WriteAoProc  pTmpSmsLongInfo malloc error!!! \n ");
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
											//[[xueww[+] 2007.03.02 //can turn back the index of every msg
											CSW_TRACE(CFW_SMS_TS_ID, "The pSmsMOInfo->nLongerMsgCurrent is %d\n",pSmsMOInfo->nLongerMsgCurrent);
											CSW_TRACE(CFW_SMS_TS_ID, "The pSmsMOInfo->nLongerMsgCount is %d\n",pSmsMOInfo->nLongerMsgCount);
											if((i + 1) < pSmsMOInfo->nLongerMsgCount)
											{
												//[[xueww[mod] 2007.06.19
												UINT16 nParam = 0x0;
												UINT8  nSign = 0X0;
												
												CFW_GetUTI(hAo, &nGetUti);
												sMeEvent.nParam1 = (UINT32)(sMeEvent.nParam1 + (pSmsMOInfo->nLocation << 16)); 

												//if it is a long msg,the HIUINT8(LOUINT16 of param2) will be the MR of the msg, 
												//HIUINT4(LOUINT8(LOUINT16 of param2)) will be the current count of multi-part message
												//LOUINT4(LOUINT8(LOUINT16 of param2)) will be the total count of multi-part message
												nParam = pSmsMOInfo->nMR;
												nSign = (i+1);
												nSign = (nSign << 4) | pSmsMOInfo->nLongerMsgCount;
												nParam = (nParam << 8) | nSign;
												
												if(sMeEvent.nParam2 == 1)
												{
													sMeEvent.nParam2 = 0x10000 + nParam;//(i+1);
												}
												else
												{
													sMeEvent.nParam2 = nParam;//i+1;
												}
												//]]xueww[mod] 2007.06.19
#ifndef CFW_MULTI_SIM
												CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,sMeEvent.nParam1,sMeEvent.nParam2,nGetUti,sMeEvent.nType);
#else
												CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,sMeEvent.nParam1,sMeEvent.nParam2,nGetUti |(nSimId<<24),sMeEvent.nType);
#endif
											}
											//]]xueww[+] 2007.03.02
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
									//Go to CFW_SM_STATE_WAIT directly!
									CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
									pEvent = &sMeEvent;
									pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
								}//end of if(pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)
							} //end of if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  

							//Write long msg to SIM ,Save the first part of a longer msg
							if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) //To SIM
							{
#if CFW_SIM_SMS_SUPPORT_LONG_MSG
								if(pSmsMOInfo->nIndex == 0)//Write a new long sms
								{
#ifndef CFW_MULTI_SIM
									ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->pTmpData,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
									ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,
										pSmsMOInfo->pTmpData,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
									CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_WriteAoProc,After CFW_SimWriteMessage, ret = %d \n"),ret);
									if(ret == ERR_SUCCESS)
									{
										CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
										break;
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
#else 			
								goto flag_fail;
#endif
							}//end of if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) //To SIM


						}
						else if(sComposeEvent.nType == 0xf0) //compose error
						{
							goto flag_fail;
						}
						CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
					}
					else
					{
						goto flag_fail;
					}
				}
			}
			else //Write PDU
			{
			    //added by fengwei 20080624 for bug 8697 write msg with length =200, formate = pdu begin
			    if (pSmsMOInfo->nDataSize > SMS_MO_ONE_PDU_SIZE)
			    {
			        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_INVALID_LENGTH,0,nGetUti,0xf0);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_INVALID_LENGTH,0,nGetUti |(nSimId<<24),0xf0);
#endif
				CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
				CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
				return ERR_SUCCESS;
			    }
			    //added by fengwei 20080624 for bug 8697 write msg with length =200, formate = pdu end
				if(pEvent == (CFW_EV*)0xffffffff)  //First come
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
														
							if((pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_UNREAD)  && 
							(pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_READ))
							{
								CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
								CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
								CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
								return ERR_SUCCESS;
							}
						}
						else if(((nTP_MTI) & 0x03) == 0x01)//SMS SUBMIT or SUBMIT REPORT
						{
							//Get MR
							pSmsMOInfo->nMR  = *(pSmsMOInfo->pData+nSCA_Length+2);// Get MR from the PDU data

							if((pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_UNSENT)                && 
							(pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ)       && 
								(pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV)  && 
								(pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE) && 
								(pSmsMOInfo->nStatus != CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE))
							{
								CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
								CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
								CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
								return ERR_SUCCESS;
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
							CSW_TRACE(CFW_SMS_TS_ID, "sms_WriteAoProc  pWritePDU malloc error!!! \n ");
							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pWritePDU, SMS_MO_ONE_PDU_SIZE);
						//Set the status
						*pWritePDU = pSmsMOInfo->nStatus;
						//Copy data
						SUL_MemCopy8((UINT8*)(pWritePDU+1),(UINT8*)pSmsMOInfo->pData,pSmsMOInfo->nDataSize);
#ifndef CFW_MULTI_SIM
						if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_ME)
						{
							pSmsMOInfo->nLocation = CFW_SMS_STORAGE_ME;
						}
						//To SIM
						if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)  || 
							(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM_SR))
						{ 
							ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pWritePDU,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
						//To SIM
						if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)  
						{ 
							ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pWritePDU,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
							if(ret == ERR_SUCCESS)
							{
								CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
							}
							else
							{
								CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
								CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
								CSW_SMS_FREE(pWritePDU);
								CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
								return ERR_SUCCESS;
							}
							break;
						}
						//To ME
#ifndef CFW_MULTI_SIM
						else if((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)  || 
							(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_BM)  || 
							(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME_SR))
						{
							UINT32 nTime=0x0, nAppInt32=0x0;
							ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pWritePDU,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#else
						else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
						{
							UINT32 nTime=0x0, nAppInt32=0x0;
							ret = CFW_MeWriteMessage(nSimId, pSmsMOInfo->nIndex,pWritePDU,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#endif
							CSW_TRACE(CFW_SMS_TS_ID, "sms_WriteAoProc  CFW_MeWriteMessage PDU,pSmsMOInfo->nIndex is %d , sMeEvent.nParam1 is (0x)%x , sMeEvent.nParam2 is (0x)%x , sMeEvent.nType is (0x)%x\n ",pSmsMOInfo->nIndex  ,sMeEvent.nParam1,sMeEvent.nParam2,sMeEvent.nType);
							if(ret == ERR_SUCCESS)
							{
								//Go to CFW_SM_STATE_WAIT directly!
								CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
								pEvent = &sMeEvent;
								pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
							}
							else
							{
								CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
								CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
								CSW_SMS_FREE(pWritePDU);
								CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
								return ERR_SUCCESS;
							}
						}
#ifndef CFW_MULTI_SIM
						else//Other storage
						{
							CFW_GetUTI(hAo, &nGetUti);
							CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti,0xf0);
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_SMS_FREE(pWritePDU);
							CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
							return ERR_SUCCESS;
						}
#endif
						//Free pWritePDUSize
						CSW_SMS_FREE(pWritePDU);
					}
					else // Write a longer PDU msg
					{
						//??? later
						CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR, pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR, pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
						CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
						CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
						return ERR_SUCCESS;
					}
				}
			}//end  Write PDU
		}
		case CFW_SM_STATE_WAIT:
		{
			if(((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_WRITE_MESSAGE_RSP)) && 
				(pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
			{
				CSW_TRACE(CFW_SMS_TS_ID, "sms_WriteAoProc  pEvent->nType = 0x%x\n ", pEvent->nType);
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
#ifndef CFW_MULTI_SIM
							CFW_CfgGetDefaultSmsParam(&sInfo);
							sInfo.mr = pSmsMOInfo->nMR;
							CFW_CfgSetDefaultSmsParam(&sInfo);
#else
							CFW_CfgGetDefaultSmsParam(&sInfo, nSimId);
							sInfo.mr = pSmsMOInfo->nMR;
							CFW_CfgSetDefaultSmsParam(&sInfo, nSimId);
#endif
							//[[xueww[mod] 2007.03.02
							pEvent->nParam1 = (UINT32)(pEvent->nParam1+ (pSmsMOInfo->nLocation << 16)); 
							//]]xueww[mod] 2007.03.02
						}
						else if(CFW_SMS_STORAGE_SM == pSmsMOInfo->nLocation)
						{
							CFW_SMS_STORAGE_INFO  sStorageInfo;
							CFW_SMS_LONG_INFO*    pTmpSmsLongInfo=NULL;   
							UINT32                ret=0x0;
							//use nListStartIndex to save the first index
							if(1 == pSmsMOInfo->nLongerMsgCurrent)
								pSmsMOInfo->nListStartIndex = (UINT16)pEvent->nParam1;
							//Update storage info
							SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
							CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
							CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
							if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
								sStorageInfo.usedSlot++;
							if(CFW_SMS_STORED_STATUS_UNREAD == pSmsMOInfo->nStatus)
								sStorageInfo.unReadRecords++;
							else if(CFW_SMS_STORED_STATUS_READ == pSmsMOInfo->nStatus)
								sStorageInfo.readRecords++;
							else if(CFW_SMS_STORED_STATUS_UNSENT == pSmsMOInfo->nStatus)
								sStorageInfo.unsentRecords++;
							else if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == pSmsMOInfo->nStatus)        || 
							    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == pSmsMOInfo->nStatus)   || 
							    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == pSmsMOInfo->nStatus)  || 
							    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == pSmsMOInfo->nStatus))
								sStorageInfo.sentRecords++;
#ifndef CFW_MULTI_SIM
							CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
							CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
							//Malloc CFW_SMS_LONG_INFO for pStru_SmsLongInfo
							pTmpSmsLongInfo = (CFW_SMS_LONG_INFO*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_LONG_INFO));
							if(!pTmpSmsLongInfo)
							{
								CSW_TRACE(CFW_SMS_TS_ID, "sms_WriteAoProc  pTmpSmsLongInfo malloc error!!! \n ");
								goto flag_fail;
							}
							SUL_ZeroMemory8((UINT8*)pTmpSmsLongInfo, (SIZEOF(CFW_SMS_LONG_INFO)));
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
							CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_WriteAoProc,pSmsMOInfo->nLongerMsgCurrent =%d \n"),pSmsMOInfo->nLongerMsgCurrent);
							if(pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)
							{
								CFW_SMS_PARAMETER  sInfo;
								//Verify MR
								if(pSmsMOInfo->nMR == 0xff)
									pSmsMOInfo->nMR = 0x01;
								else
									pSmsMOInfo->nMR++;
								//Get MR
#ifndef CFW_MULTI_SIM
								CFW_CfgGetDefaultSmsParam(&sInfo);
								sInfo.mr = pSmsMOInfo->nMR;
								CFW_CfgSetDefaultSmsParam(&sInfo);
#else
								CFW_CfgGetDefaultSmsParam(&sInfo, nSimId);
								sInfo.mr = pSmsMOInfo->nMR;
								CFW_CfgSetDefaultSmsParam(&sInfo, nSimId);

#endif
								//[[xueww[mod] 2007.03.02
								pEvent->nParam1 = (UINT32)(pEvent->nParam1+ (pSmsMOInfo->nLocation << 16)); 
								//]]xueww[mod] 2007.03.02
							}
							else//continue to write
							{
								//[[xueww[+] 2007.03.02 //can turn back the index of every msg
								pEvent->nParam1 = (UINT32)(pEvent->nParam1+ (pSmsMOInfo->nLocation << 16)); 

								//[[xueww[mod] 2007.06.19
								UINT16 nParam = 0x0;
								UINT8  nSign = 0X0;
												
								CFW_GetUTI(hAo, &nGetUti);

								//if it is a long msg,the HIUINT8(LOUINT16 of param2) will be the MR of the msg, 
								//HIUINT4(LOUINT8(LOUINT16 of param2)) will be the current count of multi-part message
								//LOUINT4(LOUINT8(LOUINT16 of param2)) will be the total count of multi-part message
								nParam = pSmsMOInfo->nMR;
								nSign = pSmsMOInfo->nLongerMsgCurrent;
								nSign = (nSign << 4) | pSmsMOInfo->nLongerMsgCount;
								nParam = (nParam << 8) | nSign;
								
								if(pEvent->nParam2 == 1)
								{
										pEvent->nParam2 = 0x10000 + nParam;
										//pEvent->nParam2 = 0x10000 + pSmsMOInfo->nLongerMsgCurrent;
								}
								else
								{
										pEvent->nParam2 = nParam;
										//pEvent->nParam2 = pSmsMOInfo->nLongerMsgCurrent;
								}
								//]]xueww[mod] 2007.06.19
#ifndef CFW_MULTI_SIM
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti,pEvent->nType);
#else
								CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);
#endif
								//]]xueww[+] 2007.03.02
								//offset
								pSmsMOInfo->pTmpPara += 4;
								pSmsMOInfo->pTmpData += pSmsMOInfo->nLongerEachPartSize;
								//Set value
								pSmsMOInfo->nLongerMsgCurrent   = *(pSmsMOInfo->pTmpPara + 3);  //part A
								pSmsMOInfo->nLongerEachPartSize = *(pSmsMOInfo->pTmpPara );     //part D

								if(pSmsMOInfo->nIndex == 0)//Write a new long sms
								{
#ifndef CFW_MULTI_SIM
									ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pSmsMOInfo->pTmpData,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
									ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,
										(UINT8*)pSmsMOInfo->pTmpData,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
									CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_WriteAoProc Write a Longer sms,After CFW_SimWriteMessage ,ret = %d! \n"),ret);
									if(ret == ERR_SUCCESS)
									{
										CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
										break;
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
						if((pSmsMOInfo->nIndex == 0) && 
							((pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM) || (pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)))
						{
							CFW_SMS_STORAGE_INFO  sStorageInfo;
							//Update storage info
							SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
							CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
							CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
							if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
								sStorageInfo.usedSlot++;
							if(CFW_SMS_STORED_STATUS_UNREAD == pSmsMOInfo->nStatus)
								sStorageInfo.unReadRecords++;
							else if(CFW_SMS_STORED_STATUS_READ == pSmsMOInfo->nStatus)
								sStorageInfo.readRecords++;
							else if(CFW_SMS_STORED_STATUS_UNSENT == pSmsMOInfo->nStatus)
								sStorageInfo.unsentRecords++;
							else if((CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ == pSmsMOInfo->nStatus)        || 
							    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV == pSmsMOInfo->nStatus)   || 
							    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE == pSmsMOInfo->nStatus)  || 
							    (CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE == pSmsMOInfo->nStatus))
								sStorageInfo.sentRecords++;
#ifndef CFW_MULTI_SIM
							CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
							CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
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
				     //added by fengwei 20080627 begin. when the storage is full after writing, we should notify AT.
				     CFW_SMS_STORAGE_INFO  sStorageInfo;
				     //get storage info
				     SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
					//added by fengwei 20080627 end. when the storage is full after writing, we should notify AT.
					
					CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
					CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
					CFW_GetUTI(hAo, &nGetUti);
					//[[xueww[mod] 2007.03.02 //if it is a long msg,the LOUINT16 of param2 will be the current count of multi-part message
					UINT16 nParam = 0x0;
					UINT8  nSign = 0X0;

					if(pSmsMOInfo->isLongerMsg) //the last part of a longer msg
					{
						//if it is a long msg,the HIUINT8(LOUINT16 of param2) will be the MR of the msg, 
						//HIUINT4(LOUINT8(LOUINT16 of param2)) will be the current count of multi-part message
						//LOUINT4(LOUINT8(LOUINT16 of param2)) will be the total count of multi-part message
						nParam = pSmsMOInfo->nMR - 1;
						nSign = pSmsMOInfo->nLongerMsgCurrent;
						nSign = (nSign << 4) | pSmsMOInfo->nLongerMsgCount;
						nParam = (nParam << 8) | nSign;

						if(pEvent->nParam2 == 1)
						{
							pEvent->nParam2 = 0x10000 + nParam;
							//pEvent->nParam2 = 0x10000 + pSmsMOInfo->nLongerMsgCurrent;
						}
						else
						{
							pEvent->nParam2 = nParam;
							//pEvent->nParam2 = pSmsMOInfo->nLongerMsgCurrent;
						}
					}
					else //a normal msg
					{
						if(pEvent->nParam2 == 1)
						{
							pEvent->nParam2 = 0x10000;
						}
						else
						{
							pEvent->nParam2 = 0x0;
						}
					}
												
					//]]xueww[mod] 2007.03.02
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti,pEvent->nType);

					//added by fengwei 20080627 begin. when the storage is full after writing, we should notify AT.
					if (sStorageInfo.totalSlot == sStorageInfo.usedSlot)
					{
					    CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 1, pSmsMOInfo->nLocation, nGetUti, 0);
					}
#else
					CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);

					//added by fengwei 20080627 begin. when the storage is full after writing, we should notify AT.
					if (sStorageInfo.totalSlot == sStorageInfo.usedSlot)
					{
					    CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 1, pSmsMOInfo->nLocation, nGetUti |(nSimId<<24), 0);
					}
#endif
					//added by fengwei 20080627 end. when the storage is full after writing, we should notify AT.

					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
					CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
				}
				//[[xueww[+] 2007.09.13
				else
				{
				    //added by fengwei 20080627 begin. when the storage is full after writing, we should notify AT.
				     CFW_SMS_STORAGE_INFO  sStorageInfo;
				     //get storage info
				     SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
					//added by fengwei 20080627 end. when the storage is full after writing, we should notify AT.
					
					CFW_GetUTI(hAo, &nGetUti);
					CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti,pEvent->nType);

					//added by fengwei 20080627 begin. when the storage is full after writing, we should notify AT.
					if (sStorageInfo.totalSlot == sStorageInfo.usedSlot)
					{
					    CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 1, pSmsMOInfo->nLocation, nGetUti, 0);
					}
#else
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
					//added by fengwei 20080627 end. when the storage is full after writing, we should notify AT.
					
					CFW_GetUTI(hAo, &nGetUti);
					CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,pEvent->nParam1,pEvent->nParam2,nGetUti |(nSimId<<24),pEvent->nType);

					//added by fengwei 20080627 begin. when the storage is full after writing, we should notify AT.
					if (sStorageInfo.totalSlot == sStorageInfo.usedSlot)
					{
					    CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND, 1, pSmsMOInfo->nLocation, nGetUti |(nSimId<<24), 0);
					}
#endif
					//added by fengwei 20080627 end. when the storage is full after writing, we should notify AT.
					
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
					CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
				}
				//]]xueww[+] 2007.09.13
			}
		}
		break;
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_WriteAoProc  error\n");
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
	return ERR_SUCCESS;

	flag_fail:
	CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
	CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
	CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
	CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
	CFW_PostNotifyEvent(EV_CFW_SMS_WRITE_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
	CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
	CSW_PROFILE_FUNCTION_EXIT(sms_WriteAoProc);
	return ERR_CMS_UNKNOWN_ERROR;
}

#ifdef CFW_MULTI_SIM	
extern bool g_SMSConcat[CFW_SIM_COUNT];
#else
extern bool g_SMSConcat;
#endif

UINT32 sms_SendProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32  nState=0x0,ret = 0x0, nGetUti=0x0;    //MO State
	CFW_SMS_MO_INFO* pSmsMOInfo = NULL; //Point to MO private data.
	CFW_EV        sComposeEvent;
	
	CSW_PROFILE_FUNCTION_ENTER(sms_SendProc);
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId=CFW_SIM_COUNT;
	CFW_GetSimID(hAo, &nSimId);
#endif

	SUL_ZeroMemory8(&sComposeEvent,SIZEOF(CFW_EV));
	//Get private date
	pSmsMOInfo = CFW_GetAoUserData(hAo);
	//Verify memory
	if(!pSmsMOInfo)
	{
		CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
		CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti,0xf0);
#else
		CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_MEMORY_FULL,0,nGetUti |(nSimId<<24),0xf0);
#endif
		CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
		CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
		return ERR_NO_MORE_MEMORY;
	}

	//Get current State
	if(pEvent == (CFW_EV*)0xffffffff)
		nState = CFW_SM_STATE_IDLE;
	else 
		nState = CFW_GetAoState(hAo);
     CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc, nState = 0x%x pSmsMOInfo->nFormat %d\n", nState,pSmsMOInfo->nFormat);
	//SMS MO State machine process
	switch(nState)
	{
		case CFW_SM_STATE_IDLE:
		{
			ret=0x0;
			if(pSmsMOInfo->nFormat)  //Compose PDU Synchronous
			{
				UINT8 nConcat=0x0;
				nConcat =0x00;// 0000 0000   bit3=0:for Send,bit2=0:no other sms(bit2=1: keep connection), don't care other bit.
#ifndef CFW_MULTI_SIM
				ret = sms_tool_ComposePDU(nConcat,CFW_SMS_STORED_STATUS_UNSENT, pSmsMOInfo->pData,
					pSmsMOInfo->nDataSize,&pSmsMOInfo->sNumber,&sComposeEvent);
#else
				ret = sms_tool_ComposePDU(nConcat,CFW_SMS_STORED_STATUS_UNSENT, pSmsMOInfo->pData,
					pSmsMOInfo->nDataSize,&pSmsMOInfo->sNumber, nSimId, &sComposeEvent);
#endif
				pEvent = &sComposeEvent;
				CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_SendProc pEvent->nType = 0x%x\n"),pEvent->nType);
				if(ret == ERR_SUCCESS )
				{
					if(pEvent->nType == 0x0) //Send normal Text sms
					{
						api_SmsPPSendReq_t* pSendSmsReq = NULL;
						ret=0x0;
						UINT8 nStruSize=0x0;

						nStruSize = (UINT8)(SIZEOF(api_SmsPPSendReq_t) + ((api_SmsPPSendReq_t*)pEvent->nParam1)->DataLen);
						pSmsMOInfo->isLongerMsg = 0;  // a normal msg 
						//Get MR
						pSmsMOInfo->nMR = (UINT8)pEvent->nParam2;
						//Systen will Free pSendSmsReq
						pSendSmsReq = (api_SmsPPSendReq_t*)CFW_MemAllocMessageStructure(nStruSize);
						if(!pSendSmsReq)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc  pSendSmsReq malloc error!!! \n ");
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pSendSmsReq, nStruSize);
						SUL_MemCopy8((UINT8*)pSendSmsReq,(UINT8*)pEvent->nParam1,nStruSize);
						CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pSendSmsReq, nStruSize, 16);
#ifndef CFW_MULTI_SIM
						pSendSmsReq->Concat     = g_SMSConcat;
						pSendSmsReq->Path       = 0x00;   //add this line for bug 16277,if we find the result,we should remove this line.
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq);
#else
						pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 

						pSendSmsReq->Path       = 0x00;   //add this line for bug 16277,if we find the result,we should remove this line.
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq, nSimId);
#endif
						CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc, Concat = 0x%x\n", pSendSmsReq->Concat);
						
						//hameina[mod] save for send fail.
						//CSW_SMS_FREE((UINT8*)(pEvent->nParam1));
						pSmsMOInfo->pSendSmsReq = (api_SmsPPSendReq_t *)(pEvent->nParam1);
						if(ret == ERR_SUCCESS )
						{
							CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
						}
						else 
						{
	    CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc nret: %d\r\n",ret);
							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_CMS_UNKNOWN_ERROR;
						}
					}
					//
					//Modify by lixp for the support of long sms
					//at 20060721
#if CFW_SIM_SMS_SUPPORT_LONG_MSG
					else if((pEvent->nType == 0x01)) //for a longer SMS
					{
						//sComposeEvent
						//pEvent->nParam1 为一个api_SmsPPSendReq_t结构体数组
						//pEvent->nParam2 为一个32位数组,each part is 8 bit,as following
						//   _____________________________________
						//  |        |        |        |         |
						//  | part A | part B | part C | part D  |
						//  |________|________|________|_________|
						// A : Current index
						// B : Total count
						// C : MR
						// D : Current api_SmsPPSendReq_t Size
						api_SmsPPSendReq_t* pSendSmsReq = NULL;
						ret=0x0;
						UINT8  nStruSize=0x0;

						//Point pEvent->nParam1/2 to MT handle
						pSmsMOInfo->pTmpData      = (UINT8*)(pEvent->nParam1);
						pSmsMOInfo->pTmpPara      = (UINT8*)(pEvent->nParam2);
						pSmsMOInfo->pTmpData_Addr = (UINT8*)(pEvent->nParam1);
						pSmsMOInfo->pTmpPara_Addr = (UINT8*)(pEvent->nParam2);

						
						//Set value
						pSmsMOInfo->isLongerMsg       = 1;  // a longer msg 
						pSmsMOInfo->nLongerMsgCurrent = *(pSmsMOInfo->pTmpPara + 3);  //part A
						pSmsMOInfo->nLongerMsgCount   = *(pSmsMOInfo->pTmpPara + 2);  //part B
						pSmsMOInfo->nMR               = *(pSmsMOInfo->pTmpPara + 1);  //part C
						nStruSize                     = *(pSmsMOInfo->pTmpPara);      //part D
						CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc,1, nStruSize = %d\n",nStruSize);
						//CSW_TC_MEMBLOCK(CFW_SMS_TS_ID,pSmsMOInfo->pTmpData_Addr, pSmsMOInfo->nLongerMsgCount*156, 16);
						//CSW_TC_MEMBLOCK(CFW_SMS_TS_ID,pSmsMOInfo->pTmpPara_Addr, pSmsMOInfo->nLongerMsgCount*4, 16);
						//Send the first part of a longer msg,System will Free pSendSmsReq
						pSendSmsReq = (api_SmsPPSendReq_t*)CFW_MemAllocMessageStructure(nStruSize);
						if(!pSendSmsReq)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc 2  pSendSmsReq malloc error!!! \n ");
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pSendSmsReq, nStruSize);
						SUL_MemCopy8((UINT8*)pSendSmsReq,pSmsMOInfo->pTmpData,nStruSize);
//						CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pSendSmsReq, nStruSize, 16);
#ifndef CFW_MULTI_SIM
						if((FALSE == g_SMSConcat)&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = g_SMSConcat; 
						}
						else if ((FALSE == g_SMSConcat)&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent != pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = TRUE; 
						}
						else
						{
							pSendSmsReq->Concat     = g_SMSConcat; 
						}
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq);
#else
						if((FALSE == g_SMSConcat[nSimId])&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 
						}
						else if ((FALSE == g_SMSConcat[nSimId])&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent != pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = TRUE; 
						}
						else
						{
							pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 
						}
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq, nSimId);
#endif
						CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc, Concat = 0x%x\n", pSendSmsReq->Concat);
						if(ret == ERR_SUCCESS )
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc,2,call CFW_SendSclMessage success! \n");
							pSmsMOInfo->pTmpPara += 4;
							pSmsMOInfo->pTmpData += nStruSize;
							CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
						}   
						else 
						{
							CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
							CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
							//pSmsMOInfo->pTmpPara_Addr = NULL;
							//pSmsMOInfo->pTmpData_Addr = NULL;
							
							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_CMS_UNKNOWN_ERROR;
						}
					}
#else
					else if((pEvent->nType == 0x01)) //Doesn't support longer SMS
					{
						CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
						CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
						CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
						return ERR_SUCCESS;
					}
#endif
					else if(pEvent->nType == 0xf0)   //compose error
					{
						CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
						CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
						CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
						return ERR_SUCCESS;
					}
					CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
				}
				else 
				{
					CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
					CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
					CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
					CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
					return ERR_CMS_UNKNOWN_ERROR;
				}
			}
			else //Send PDU directly
			{
#ifndef CFW_MULTI_SIM
				ret = sms_mo_ComposeLongPDU(pSmsMOInfo);
#else
				ret = sms_mo_ComposeLongPDU(pSmsMOInfo, nSimId);
#endif
				if(ret != ERR_SUCCESS)
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc ret =%d\n ",ret);
					CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
					CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
					CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
					CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
					return ERR_CMS_UNKNOWN_ERROR;
				}
				
			}
			
		}
		break;
		case CFW_SM_STATE_WAIT:
		{
			if(pEvent->nEventId == API_SMSPP_ACK_IND)
			{
#ifdef CFW_CUSTOM_IMEI_COUNT
				CFW_CustomSendIMEIProc(TRUE,nSimId);
#endif
				CFW_SMS_PARAMETER  sInfo;
				UINT32             nP2=0x0;

				SUL_ZeroMemory8(&sInfo, SIZEOF(CFW_SMS_PARAMETER));
				//Verify MR
				if(pSmsMOInfo->nMR == 0xff)
					pSmsMOInfo->nMR = 0x0;
				else
					pSmsMOInfo->nMR++;
				//Get MR
#ifndef CFW_MULTI_SIM
				CFW_CfgGetDefaultSmsParam(&sInfo);
				sInfo.mr = pSmsMOInfo->nMR;
				CFW_CfgSetDefaultSmsParam(&sInfo);
#else
				CFW_CfgGetDefaultSmsParam(&sInfo, nSimId);
				sInfo.mr = pSmsMOInfo->nMR;
				CFW_CfgSetDefaultSmsParam(&sInfo, nSimId);
#endif
				//Post MR
				if(pSmsMOInfo->nMR == 0)
					pSmsMOInfo->nMR = 0x1;
				CFW_GetUTI(pSmsMOInfo->hAo, &nGetUti);
				nP2 = (pSmsMOInfo->nLongerMsgCount << 16) + pSmsMOInfo->nLongerMsgCurrent;
#ifndef CFW_MULTI_SIM
				CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,(pSmsMOInfo->nMR-1),nP2,nGetUti,0);
#else
				CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,(pSmsMOInfo->nMR-1),nP2,nGetUti |(nSimId<<24),0);
#endif
				//if a normal msg or the last part of a longer msg
				if((!pSmsMOInfo->isLongerMsg) || 
					((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)))
				{
					CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
					CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
					CSW_SMS_FREE(pSmsMOInfo->pSendSmsReq);
					pSmsMOInfo->pTmpPara_Addr = NULL;
					pSmsMOInfo->pTmpData_Addr = NULL;
					pSmsMOInfo->pSendSmsReq = NULL;

					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
					CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
					CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
					return ERR_SUCCESS;
				}
				else  // a longer msg
				{
					if(pSmsMOInfo->nFormat) //Text longer msg
					{
						api_SmsPPSendReq_t* pSendSmsReq = NULL;
						ret=0x0;
						UINT8  nStruSize=0x0;

						//Set value
						pSmsMOInfo->nLongerMsgCurrent = *(pSmsMOInfo->pTmpPara + 3);  //part A
						pSmsMOInfo->nMR               = *(pSmsMOInfo->pTmpPara + 1);  //part C
						nStruSize                     = *(pSmsMOInfo->pTmpPara );     //part D
						//Send the first part of a longer msg,System will Free pSendSmsReq
						pSendSmsReq = (api_SmsPPSendReq_t*)CFW_MemAllocMessageStructure(nStruSize);
						if(!pSendSmsReq)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc 3  pSendSmsReq malloc error!!! \n ");
							CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
							CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
							pSmsMOInfo->pTmpPara_Addr = NULL;
							pSmsMOInfo->pTmpData_Addr = NULL;
							
							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pSendSmsReq, nStruSize);
						SUL_MemCopy8((UINT8*)pSendSmsReq,pSmsMOInfo->pTmpData,nStruSize);
						//CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pSendSmsReq, nStruSize, 16);
#ifndef CFW_MULTI_SIM
						if((FALSE == g_SMSConcat)&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = g_SMSConcat; 
						}
						else if ((FALSE == g_SMSConcat)&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent != pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = TRUE; 
						}
						else
						{
							pSendSmsReq->Concat     = g_SMSConcat; 
						}
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq);
#else
						if((FALSE == g_SMSConcat[nSimId])&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 
						}
						else if ((FALSE == g_SMSConcat[nSimId])&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent != pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = TRUE; 
						}
						else
						{
							pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 
						}
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq, nSimId);
#endif
						CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc, Concat = 0x%x\n", pSendSmsReq->Concat);
						if(ret == ERR_SUCCESS)
						{
							pSmsMOInfo->pTmpPara += 4;
							pSmsMOInfo->pTmpData += nStruSize;
							CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
						}   
						else 
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc,5,call CFW_SendSclMessage failed! \n");
							CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
							CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
							pSmsMOInfo->pTmpPara_Addr = NULL;
							pSmsMOInfo->pTmpData_Addr = NULL;
							
							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_CMS_UNKNOWN_ERROR;
						}
					}
#ifndef CFW_MULTI_SIM
					else  //PDU longer msg
					{
						ret=0x0;
						UINT16 nStruSize=0x0;
						api_SmsPPSendReq_t* pSendSmsReq = NULL;

						nStruSize = (UINT16)(SIZEOF(api_SmsPPSendReq_t) + pSmsMOInfo->nLongerEachPartSize);
						pSendSmsReq = (api_SmsPPSendReq_t*)CFW_MemAllocMessageStructure(nStruSize);
						if(!pSendSmsReq)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc 4  pSendSmsReq malloc error!!! \n ");
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pSendSmsReq, nStruSize);
						//Copy all the data to compose a Send data
						SUL_MemCopy8((UINT8*)(pSendSmsReq),(UINT8*)pSmsMOInfo->pSendSmsReq,nStruSize);

						pSmsMOInfo->nLongerMsgCurrent++;
						//Set Longer Current
						pSendSmsReq->Data[pSmsMOInfo->nLongerIndexOffset + 5] = pSmsMOInfo->nLongerMsgCurrent;
						if(pSmsMOInfo->nLongerMsgCurrent != pSmsMOInfo->nLongerMsgCount)
						{
							//pSendSmsReq->Concat  = 1;
#ifdef CFW_MULTI_SIM
							pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 
#else
							pSendSmsReq->Concat     = g_SMSConcat;
#endif
							//copy user data content
							SUL_MemCopy8((UINT8*)(pSendSmsReq->Data + pSmsMOInfo->nLongerIndexOffset + pSendSmsReq->Data[pSmsMOInfo->nLongerIndexOffset + 0] + 1),pSmsMOInfo->pData,(0x8C - pSendSmsReq->Data[pSmsMOInfo->nLongerIndexOffset + 0] - 1));
							//pData offset
							pSmsMOInfo->pData = pSmsMOInfo->pData + 0x8C - pSendSmsReq->Data[pSmsMOInfo->nLongerIndexOffset + 0] - 1;
						}
						else//the last part of longer msg
						{
							//pSendSmsReq->Concat  = 0;
#ifdef CFW_MULTI_SIM
							pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 
#else
							pSendSmsReq->Concat     = g_SMSConcat;
#endif
							pSendSmsReq->DataLen = (UINT8)(pSmsMOInfo->nLongerIndexOffset + pSmsMOInfo->nLongerLastUserDataSize);
							//Set TP UDL
							if(pSmsMOInfo->nDCS == 0) //GSM 7 bit, Total number os septets
								pSendSmsReq->Data[pSmsMOInfo->nLongerIndexOffset - 1] = (UINT8)((8*pSmsMOInfo->nLongerLastUserDataSize) / 7 + 1);
							else   //UCS2 or 8-bit
								pSendSmsReq->Data[pSmsMOInfo->nLongerIndexOffset - 1] = pSmsMOInfo->nLongerLastUserDataSize;
							//copy user data content after head
							SUL_MemCopy8((UINT8*)(pSendSmsReq->Data + pSmsMOInfo->nLongerIndexOffset + pSendSmsReq->Data[pSmsMOInfo->nLongerIndexOffset + 0] + 1),pSmsMOInfo->pData,(pSmsMOInfo->nLongerLastUserDataSize - pSendSmsReq->Data[pSmsMOInfo->nLongerIndexOffset + 0] - 1));
						}
						//Set MR
						pSendSmsReq->Data[1] = pSmsMOInfo->nMR;
						CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pSendSmsReq, nStruSize, 16);
						CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc, Concat = 0x%x\n", pSendSmsReq->Concat);
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq);
						if(ret == ERR_SUCCESS )
						{
							CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
						}
						else 
						{
							CSW_SMS_FREE(pSmsMOInfo->pSendSmsReq);
							pSmsMOInfo->pSendSmsReq = NULL;
							
							CFW_GetUTI(hAo, &nGetUti);
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_CMS_UNKNOWN_ERROR;
						}
					}
#endif					
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONTINUE);
				}
			}
			else if(pEvent->nEventId == API_SMSPP_ERROR_IND)
			{
				//Parse the error code later
				ret = sms_mo_Parse_SmsPPErrorInd(hAo,(api_SmsPPErrorInd_t *)pEvent->nParam1);
					CFW_GetUTI(hAo, &nGetUti);
				if(ret != ERR_SUCCESS && ERR_CMS_RESOURCES_UNAVAILABLE != ret)
				{
					pSmsMOInfo->nResendCnt=0;
					if(NULL!= pSmsMOInfo->pTmpPara_Addr)
					{
						CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
						pSmsMOInfo->pTmpPara_Addr = NULL;
						
					}
					if(NULL!= pSmsMOInfo->pTmpData_Addr)
					{

						CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
						pSmsMOInfo->pTmpData_Addr = NULL;
					}
					if(NULL!= pSmsMOInfo->pSendSmsReq)
					{
						CSW_SMS_FREE(pSmsMOInfo->pSendSmsReq);
						pSmsMOInfo->pSendSmsReq = NULL;
					}
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
					CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_SINGLE|CFW_AO_PROC_CODE_CONSUMED);
					CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
				}
				else if(ERR_CMS_RESOURCES_UNAVAILABLE == ret)
				{
				
				CSW_TRACE(CFW_SMS_TS_ID, "pSmsMOInfo->isLongerMsg is %d, pSmsMOInfo->nFormat=%d\n ",pSmsMOInfo->isLongerMsg, pSmsMOInfo->nFormat);
				//here we need to resend the PP SMS
				//Text longer msg
					if(pSmsMOInfo->isLongerMsg &&pSmsMOInfo->nFormat 
						&& pSmsMOInfo->nResendCnt < CFW_SMS_MAX_RESEND_COUNT) //hadn't resent yet
					{
						pSmsMOInfo->nResendCnt++;
						api_SmsPPSendReq_t* pSendSmsReq = NULL;
						ret=0x0;
						UINT8  nStruSize=0x0;

						//restore to the param which had been send
						pSmsMOInfo->pTmpPara -= 4;
						nStruSize                     = *(pSmsMOInfo->pTmpPara );     //part D
						pSmsMOInfo->pTmpData -= nStruSize;
						//Send the first part of a longer msg,System will Free pSendSmsReq
						pSendSmsReq = (api_SmsPPSendReq_t*)CFW_MemAllocMessageStructure(nStruSize);
						if(!pSendSmsReq)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc 5  pSendSmsReq malloc error!!! \n ");
							CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
							CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
							//pSmsMOInfo->pTmpPara_Addr = NULL;
							//pSmsMOInfo->pTmpData_Addr = NULL;

							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);

							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pSendSmsReq, nStruSize);
						SUL_MemCopy8((UINT8*)pSendSmsReq,pSmsMOInfo->pTmpData,nStruSize);
						CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pSendSmsReq, nStruSize, 16);
#ifndef CFW_MULTI_SIM
						if((FALSE == g_SMSConcat)&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = g_SMSConcat; 
						}
						else if ((FALSE == g_SMSConcat)&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent != pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = TRUE; 
						}
						else
						{
							pSendSmsReq->Concat     = g_SMSConcat; 
						}
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq);
#else
						if((FALSE == g_SMSConcat[nSimId])&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent == pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 
						}
						else if ((FALSE == g_SMSConcat[nSimId])&&
							((pSmsMOInfo->isLongerMsg) && (pSmsMOInfo->nLongerMsgCurrent != pSmsMOInfo->nLongerMsgCount)))
						{
							pSendSmsReq->Concat     = TRUE; 
						}
						else
						{
							pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 
						}
						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq, nSimId);
#endif

						CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc, Concat = 0x%x\n", pSendSmsReq->Concat);
						if(ret == ERR_SUCCESS)
						{
							pSmsMOInfo->pTmpPara += 4;
							pSmsMOInfo->pTmpData += nStruSize;
							CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
						}   
						else 
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc,5,call CFW_SendSclMessage error ret=0x%x \n" , ret);
							CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
							CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
							//pSmsMOInfo->pTmpPara_Addr = NULL;
							//pSmsMOInfo->pTmpData_Addr = NULL;
							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_CMS_UNKNOWN_ERROR;
						}
						return ERR_SUCCESS;
					}
					//hadn't resent yet)//send normal msg fail
					else if(!pSmsMOInfo->isLongerMsg&&pSmsMOInfo->nFormat
						&& pSmsMOInfo->nResendCnt < CFW_SMS_MAX_RESEND_COUNT) //hadn't resend yet
					{
						api_SmsPPSendReq_t* pSendSmsReq = NULL;
						UINT8 nStruSize = 0;
						
						pSmsMOInfo->nResendCnt++;
						
						nStruSize = (UINT8)(SIZEOF(api_SmsPPSendReq_t) + pSmsMOInfo->pSendSmsReq->DataLen);
						pSendSmsReq = (api_SmsPPSendReq_t*)CFW_MemAllocMessageStructure(nStruSize);
						if(!pSendSmsReq)
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc 6  pSendSmsReq malloc error!!! \n ");
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							CSW_SMS_FREE(pSmsMOInfo->pSendSmsReq);
							return ERR_NO_MORE_MEMORY;
						}
						SUL_ZeroMemory8(pSendSmsReq, nStruSize);
						SUL_MemCopy8((UINT8*)pSendSmsReq,(UINT8*)( pSmsMOInfo->pSendSmsReq),nStruSize);
						CSW_TC_MEMBLOCK(CFW_SMS_TS_ID, (UINT8 *) pSmsMOInfo->pSendSmsReq, nStruSize, 16);
#ifndef CFW_MULTI_SIM
						pSendSmsReq->Concat     = g_SMSConcat;

						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq);
#else
						pSendSmsReq->Concat     = g_SMSConcat[nSimId]; 

						ret = CFW_SendSclMessage(API_SMSPP_SEND_REQ,pSendSmsReq, nSimId);
#endif
						CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc, Concat = 0x%x\n", pSendSmsReq->Concat);
						if(ret == ERR_SUCCESS)
						{
							CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_CONSUMED);
							CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
						}   
						else 
						{
							CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc,5,call CFW_SendSclMessage error ret=0x%x \n",ret);
							CSW_SMS_FREE(pSmsMOInfo->pSendSmsReq);
							CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti,0xf0);
#else
							CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ERR_CMS_UNKNOWN_ERROR,0,nGetUti |(nSimId<<24),0xf0);
#endif
							CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
							CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
							return ERR_CMS_UNKNOWN_ERROR;
						}
					}
					else
					{
						pSmsMOInfo->nResendCnt = 0;
						if(NULL!= pSmsMOInfo->pTmpPara_Addr)
						{
							CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
							pSmsMOInfo->pTmpPara_Addr = NULL;
							
						}
						if(NULL!= pSmsMOInfo->pTmpData_Addr)
						{

							CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
							pSmsMOInfo->pTmpData_Addr = NULL;
						}


						if(NULL!= pSmsMOInfo->pSendSmsReq)
						{
							CSW_SMS_FREE(pSmsMOInfo->pSendSmsReq);
							pSmsMOInfo->pSendSmsReq = NULL;
						}
						CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc,6666,send EV_CFW_SMS_SEND_MESSAGE_RSP to MMI, P1=0X%X\n", ret);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ret,0,nGetUti,0xf0);
#else
						CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ret,0,nGetUti |(nSimId<<24),0xf0);
#endif
						CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
					}
				}
				
				else
				{
					pSmsMOInfo->nResendCnt = 0;
					if(NULL!= pSmsMOInfo->pTmpPara_Addr)
					{
						CSW_SMS_FREE(pSmsMOInfo->pTmpPara_Addr);
						pSmsMOInfo->pTmpPara_Addr = NULL;
						
					}
					if(NULL!= pSmsMOInfo->pTmpData_Addr)
					{

						CSW_SMS_FREE(pSmsMOInfo->pTmpData_Addr);
						pSmsMOInfo->pTmpData_Addr = NULL;
					}


					if(NULL!= pSmsMOInfo->pSendSmsReq)
					{
						CSW_SMS_FREE(pSmsMOInfo->pSendSmsReq);
						pSmsMOInfo->pSendSmsReq = NULL;
					}
					CSW_TRACE(CFW_SMS_TS_ID, "sms_SendProc,777,send EV_CFW_SMS_SEND_MESSAGE_RSP to MMI, P1=0X%X\n", ret);
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ret,0,nGetUti,0xf0);
#else
					CFW_PostNotifyEvent(EV_CFW_SMS_SEND_MESSAGE_RSP,ret,0,nGetUti |(nSimId<<24),0xf0);
#endif
					CFW_UnRegisterAO(CFW_SMS_MO_SRV_ID,hAo);
				}
				CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);

				return ERR_SUCCESS;
			}
		}
		break;
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc  error\n");
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(sms_SendProc);
	return ERR_SUCCESS;
}


UINT32 sms_SetUnRead2ReadProc(HAO hAo, CFW_EV* pEvent)
{
    UINT32           ret=0x0,nGetUti=0x0,nState = 0x0;    //MO State
    CFW_SMS_MO_INFO* pSmsMOInfo = NULL;   //Point to MO private data.
    CFW_EV           sMeEvent;
    UINT8            nSmsStatus = 0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_SetUnRead2ReadProc);
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId= CFW_SIM_COUNT;
	CFW_GetSimID(hAo, &nSimId);
#endif	
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsMOInfo)
    {
        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
        CSW_PROFILE_FUNCTION_EXIT(sms_SetUnRead2ReadProc);
        return ERR_NO_MORE_MEMORY;
    }

   //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
     CSW_TRACE(CFW_SMS_TS_ID,"sms_SetUnRead2ReadProc, nState = 0x%x\n", nState);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
            {
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
                {
#ifndef CFW_MULTI_SIM
                    ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal);
#else
                    ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                    if(ret == ERR_SUCCESS)
                    {
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                    }
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetUnRead2ReadProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                break;
                }
                else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
                {
                    //Malloc for the read data.
                    sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
                    if(!(UINT8*)sMeEvent.nParam1)
                    {
       			CSW_TRACE(CFW_SMS_TS_ID, "sms_SetUnRead2ReadProc   sMeEvent.nParam1  malloc error!!! \n ");
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetUnRead2ReadProc);
                        return ERR_NO_MORE_MEMORY;
                    }
                    SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
                    ret = CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
                    if(ret == ERR_SUCCESS)
                    {
                        //Go to CFW_SM_STATE_WAIT directly!
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                        pEvent = &sMeEvent;
                        pEvent->nTransId = pSmsMOInfo->nUTI_Internal;
                    }
                    else  //other return value
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetUnRead2ReadProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
            }
        case CFW_SM_STATE_WAIT:
            {
                if(((pEvent->nEventId == EV_CFW_SIM_READ_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)) && 
                   (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
                {
                    if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
                    {
                        //Just get SMS status 
                       ret =  sms_tool_DecomposePDU((PVOID)pEvent->nParam1,1,NULL,&nSmsStatus,NULL);
			CSW_TRACE(CFW_SMS_TS_ID, "call SMS_TOOL_DECOMPOSEPDU END, ret=0x%x\n", ret);
                        if(nSmsStatus == CFW_SMS_STORED_STATUS_UNREAD)
                        {
                            *((UINT8*)pEvent->nParam1) = CFW_SMS_STORED_STATUS_READ;
                            if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
                            {
                                //Write it with CFW_SMS_STORED_STATUS_READ
#ifndef CFW_MULTI_SIM
                                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
                                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                            }
                            else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
                            {
                                UINT32 nTime=0x0, nAppInt32=0x0,nTmp=0x0;
                                nTmp = sMeEvent.nParam1;
#ifndef CFW_MULTI_SIM
                                ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#else
                                ret = CFW_MeWriteMessage(nSimId, pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#endif
                                CSW_SMS_FREE((UINT8*)(nTmp));
                            }
                            if(ret != ERR_SUCCESS)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_SetUnRead2ReadProc);
                                return ERR_CMS_UNKNOWN_ERROR;
                            }
                            pEvent = &sMeEvent;
                            pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
                            pSmsMOInfo->nReadSmsStatus = CFW_SMS_STORED_STATUS_UNREAD;
                        }
                        else
                        {
                            if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
                                CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
                            CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,pSmsMOInfo->nIndex,nGetUti |(nSimId <<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_SetUnRead2ReadProc);
                            return ERR_CMS_UNKNOWN_ERROR;
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
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,pEvent->nParam1,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,pEvent->nParam1,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                }
                
                if(((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_WRITE_MESSAGE_RSP)) && 
                   (pSmsMOInfo->nUTI_Internal == pEvent->nTransId) )
                {
                    if(pEvent->nType == 0)
                    {
                        CFW_SMS_STORAGE_INFO  sStorageInfo;

                        //Zero memory
                        SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
#else
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
#endif
                        if(sStorageInfo.unReadRecords > 0)
                            sStorageInfo.unReadRecords--;
                        if(sStorageInfo.readRecords < sStorageInfo.totalSlot)
                            sStorageInfo.readRecords++;
#ifndef CFW_MULTI_SIM
                        CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                        pEvent->nParam1 = pSmsMOInfo->nIndex + (pSmsMOInfo->nLocation << 16);  
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,pEvent->nParam1,0,nGetUti,pEvent->nType);
#else
                        CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
                        pEvent->nParam1 = pSmsMOInfo->nIndex + (pSmsMOInfo->nLocation << 16);  
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,pEvent->nParam1,0,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
#ifndef CFW_MULTI_SIM
                    else if(pEvent->nType == 0xf0)
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
#endif					
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNREAD2READ_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId <<24),pEvent->nType);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                }
                        
            }
            break;
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_SendProc  error\n");
		break;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_SetUnRead2ReadProc);
    return ERR_SUCCESS;
}



UINT32 sms_SetRead2UnReadProc(HAO hAo, CFW_EV* pEvent)
{
    UINT32           ret=0x0,nGetUti=0x0,nState = 0x0;    //MO State
    CFW_SMS_MO_INFO* pSmsMOInfo = NULL;   //Point to MO private data.
    CFW_EV           sMeEvent;
    UINT8            nSmsStatus = 0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_SetRead2UnReadProc);
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId= CFW_SIM_COUNT;
	CFW_GetSimID(hAo, &nSimId);
#endif	
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsMOInfo)
    {
        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
        CSW_PROFILE_FUNCTION_EXIT(sms_SetRead2UnReadProc);
        return ERR_NO_MORE_MEMORY;
    }

   //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
     CSW_TRACE(CFW_SMS_TS_ID,"sms_SetRead2UnReadProc, nState = 0x%x\n", nState);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
            {
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
                {
#ifndef CFW_MULTI_SIM
                    ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal);
#else
                    ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                    if(ret == ERR_SUCCESS)
                    {
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                    }
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetRead2UnReadProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                break;
                }
                else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
                {
                    //Malloc for the read data.
                    sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
                    if(!(UINT8*)sMeEvent.nParam1)
                    {
       			CSW_TRACE(CFW_SMS_TS_ID, "sms_SetRead2UnReadProc   sMeEvent.nParam1  malloc error!!! \n ");
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetRead2UnReadProc);
                        return ERR_NO_MORE_MEMORY;
                    }
                    SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
                    ret = CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
                    if(ret == ERR_SUCCESS)
                    {
                        //Go to CFW_SM_STATE_WAIT directly!
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                        pEvent = &sMeEvent;
                        pEvent->nTransId = pSmsMOInfo->nUTI_Internal;
                    }
                    else  //other return value
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetRead2UnReadProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
            }
        case CFW_SM_STATE_WAIT:
            {
                if(((pEvent->nEventId == EV_CFW_SIM_READ_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)) && 
                   (pEvent->nTransId == pSmsMOInfo->nUTI_Internal))
                {
                    if((pEvent->nType == 0) && (*(UINT8*)pEvent->nParam1))
                    {
                        //Just get SMS status 
                        sms_tool_DecomposePDU((PVOID)pEvent->nParam1,1,NULL,&nSmsStatus,NULL);
                        if(nSmsStatus == CFW_SMS_STORED_STATUS_READ)
                        {
                            *((UINT8*)pEvent->nParam1) = CFW_SMS_STORED_STATUS_UNREAD;
                            if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
                            {
                                //Write it with CFW_SMS_STORED_STATUS_READ
#ifndef CFW_MULTI_SIM
                                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
                                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                            }
                            else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
                            {
                                UINT32 nTime=0x0, nAppInt32=0x0,nTmp=0x0;
                                nTmp = sMeEvent.nParam1;
#ifndef CFW_MULTI_SIM
                                ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#else
                                ret = CFW_MeWriteMessage(nSimId, pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#endif
                                CSW_SMS_FREE((UINT8*)(nTmp));
                            }
                            if(ret != ERR_SUCCESS)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_SetRead2UnReadProc);
                                return ERR_CMS_UNKNOWN_ERROR;
                            }
                            pEvent = &sMeEvent;
                            pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
                            pSmsMOInfo->nReadSmsStatus = CFW_SMS_STORED_STATUS_UNREAD;
                        }
                        else
                        {
                            if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
                                CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
                            CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_SetRead2UnReadProc);
                            return ERR_CMS_UNKNOWN_ERROR;
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
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,pEvent->nParam1,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,pEvent->nParam1,pSmsMOInfo->nIndex,nGetUti |(nSimId <<24),pEvent->nType);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                }
                
                if(((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_WRITE_MESSAGE_RSP)) && 
                   (pSmsMOInfo->nUTI_Internal == pEvent->nTransId) )
                {
                    if(pEvent->nType == 0)
                    {
                        CFW_SMS_STORAGE_INFO  sStorageInfo;

                        //Zero memory
                        SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                        if(sStorageInfo.unReadRecords > 0)
                            sStorageInfo.unReadRecords--;
                        if(sStorageInfo.readRecords < sStorageInfo.totalSlot)
                            sStorageInfo.readRecords++;
                        CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                        pEvent->nParam1 = pSmsMOInfo->nIndex + (pSmsMOInfo->nLocation << 16);  
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,pEvent->nParam1,0,nGetUti,pEvent->nType);
#else
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
                        if(sStorageInfo.unReadRecords > 0)
                            sStorageInfo.unReadRecords--;
                        if(sStorageInfo.readRecords < sStorageInfo.totalSlot)
                            sStorageInfo.readRecords++;
                        CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
                        pEvent->nParam1 = pSmsMOInfo->nIndex + (pSmsMOInfo->nLocation << 16);  
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,pEvent->nParam1,0,nGetUti |(nSimId<<24),pEvent->nType);

#endif
			   CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
#ifndef CFW_MULTI_SIM
                    else if(pEvent->nType == 0xf0)
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_READ2UNREAD_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                }
                        
            }
            break;
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_SetRead2UnReadProc  error\n");
		break;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_SetRead2UnReadProc);
    return ERR_SUCCESS;
}



UINT32 sms_SetUnSent2SentProc(HAO hAo, CFW_EV* pEvent)
{
    UINT32           ret=0x0,nGetUti=0x0,nState = 0x0;    //MO State
    CFW_SMS_MO_INFO* pSmsMOInfo = NULL;   //Point to MO private data.
    CFW_EV           sMeEvent;
    UINT8            nSmsStatus = 0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_SetUnSent2SentProc);
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId =  CFW_SIM_COUNT;
	CFW_GetSimID(hAo, &nSimId);
#endif	
    //Zero memory
    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsMOInfo)
    {
        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
        CSW_PROFILE_FUNCTION_EXIT(sms_SetUnSent2SentProc);
        return ERR_NO_MORE_MEMORY;
    }

   //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
     CSW_TRACE(CFW_SMS_TS_ID,"sms_SetUnSent2SentProc, nState = 0x%x\n", nState);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
            {
                if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_SM)
                {
#ifndef CFW_MULTI_SIM
                    ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal);
#else
                    ret = CFW_SimReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                    if(ret == ERR_SUCCESS)
                    {
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                    }
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetUnSent2SentProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                break;
                }
                else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
                {
                    //Malloc for the read data.
                    sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SMS_MO_ONE_PDU_SIZE+4+4));
                    if(!(UINT8*)sMeEvent.nParam1)
                    {
       			CSW_TRACE(CFW_SMS_TS_ID, "sms_SetUnSent2SentProc   sMeEvent.nParam1  malloc error!!! \n ");
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetUnSent2SentProc);
                        return ERR_NO_MORE_MEMORY;
                    }
                    SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1,SMS_MO_ONE_PDU_SIZE+4+4);
                    ret = CFW_MeReadMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,&sMeEvent);
                    if(ret == ERR_SUCCESS)
                    {
                        //Go to CFW_SM_STATE_WAIT directly!
                        CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                        pEvent = &sMeEvent;
                        pEvent->nTransId = pSmsMOInfo->nUTI_Internal;
                    }
                    else  //other return value
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_SetUnSent2SentProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }
                }
            }
        case CFW_SM_STATE_WAIT:
            {
                if(((pEvent->nEventId == EV_CFW_SIM_READ_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)) && 
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
                                //Write it with CFW_SMS_STORED_STATUS_SENT
#ifndef CFW_MULTI_SIM
                                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal);
#else
                                ret = CFW_SimWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,pSmsMOInfo->nUTI_Internal, nSimId);
#endif
                            }
                            else if(pSmsMOInfo->nLocation == CFW_SMS_STORAGE_ME)
                            {
                                UINT32 nTime=0x0, nAppInt32=0x0,nTmp=0x0;
                                nTmp = sMeEvent.nParam1;
#ifndef CFW_MULTI_SIM
                                ret = CFW_MeWriteMessage(pSmsMOInfo->nLocation,pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#else
                                ret = CFW_MeWriteMessage(nSimId, pSmsMOInfo->nIndex,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sMeEvent);
#endif
                                CSW_SMS_FREE((UINT8*)(nTmp));
                            }
                            if(ret != ERR_SUCCESS)
                            {
                                CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                                CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                                CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif
                                CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                                CSW_PROFILE_FUNCTION_EXIT(sms_SetUnSent2SentProc);
                                return ERR_CMS_UNKNOWN_ERROR;
                            }
                            pEvent = &sMeEvent;
                            pEvent->nTransId  = pSmsMOInfo->nUTI_Internal;
                            pSmsMOInfo->nReadSmsStatus = CFW_SMS_STORED_STATUS_UNREAD;
                        }
                        else
                        {
                            if(pEvent->nEventId == EV_CFW_SMS_READ_MESSAGE_RSP)
                                CSW_SMS_FREE((UINT8*)(sMeEvent.nParam1));
                            CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                            CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else
                            CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_OPERATION_NOT_ALLOWED,pSmsMOInfo->nIndex,nGetUti |(nSimId <<24),0xf0);
#endif
                            CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                            CSW_PROFILE_FUNCTION_EXIT(sms_SetUnSent2SentProc);
                            return ERR_CMS_UNKNOWN_ERROR;
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
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,pEvent->nParam1,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
#else
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,pEvent->nParam1,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),pEvent->nType);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                }
                if(((pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SMS_WRITE_MESSAGE_RSP)) && 
                   (pSmsMOInfo->nUTI_Internal == pEvent->nTransId) )
                {
                    if(pEvent->nType == 0)
                    {
                        CFW_SMS_STORAGE_INFO  sStorageInfo;

                        //Zero memory
                        SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                        if(sStorageInfo.unsentRecords > 0)
                            sStorageInfo.unsentRecords--;
                        if(sStorageInfo.sentRecords < sStorageInfo.totalSlot)
                            sStorageInfo.sentRecords++;
                        CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation);
                        pEvent->nParam1 = pSmsMOInfo->nIndex + (pSmsMOInfo->nLocation << 16);  
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,pEvent->nParam1,0,nGetUti,pEvent->nType);
#else
                        CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
                        if(sStorageInfo.unsentRecords > 0)
                            sStorageInfo.unsentRecords--;
                        if(sStorageInfo.sentRecords < sStorageInfo.totalSlot)
                            sStorageInfo.sentRecords++;
                        CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMOInfo->nLocation, nSimId);
                        pEvent->nParam1 = pSmsMOInfo->nIndex + (pSmsMOInfo->nLocation << 16);  
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,pEvent->nParam1,0,nGetUti |(nSimId<<24),pEvent->nType);

#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
#ifndef CFW_MULTI_SIM
                    else if(pEvent->nType == 0xf0)
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,pEvent->nType);
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti,0xf0);
#else						
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
                        CFW_PostNotifyEvent(EV_CFW_SMS_SET_UNSENT2SENT_RSP,ERR_CMS_UNKNOWN_ERROR,pSmsMOInfo->nIndex,nGetUti |(nSimId<<24),0xf0);
#endif					
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    }
                }
                        
            }
            break;
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_SetUnSent2SentProc  error\n");
		break;
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_SetUnSent2SentProc);
    return ERR_SUCCESS;
}


//
// SMS MT AO process entry.
//
//for SMS MT, there are the following 2 states,define in cfw.h:
//CFW_SM_STATE_IDLE
//CFW_SM_STATE_WAIT

UINT32 sms_MTAoProc(HAO hAo, CFW_EV* pEvent)
{
	UINT32              nState     = 0x0,ret=0x0;    //MT State
	UINT8               nMode      = 0x0;    //the overflow presentation mode; 0 disable (default), 1 enable 
	CFW_IND_EVENT_INFO  sEventInd;  
	CFW_SMS_MT_INFO*    pSmsMTInfo = NULL; //Point to MT private data.
	CSW_PROFILE_FUNCTION_ENTER(sms_MTAoProc);

#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId =  CFW_SIM_COUNT;
	CFW_GetSimID(hAo, &nSimId);
#endif	
	//Get private date
	pSmsMTInfo = CFW_GetAoUserData(hAo);
	//Verify memory
	if(!pSmsMTInfo)
	{
		CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, 1,get private data fail! \n ");
		CSW_PROFILE_FUNCTION_EXIT(sms_MTAoProc);
		return ERR_NO_MORE_MEMORY;
	}

	SUL_ZeroMemory8(&sEventInd,SIZEOF(CFW_IND_EVENT_INFO));
	//Get current State
	nState = CFW_GetAoState(hAo);

	//SMS MT State machine process
	CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, nState=0x%x \n ", nState);
	switch(nState)
	{
	case CFW_SM_STATE_IDLE:
		CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, pEvent->nEventId =0x%x \n ", pEvent->nEventId );
		if(pEvent->nEventId == API_SMSPP_RECEIVE_IND)
		{
			CFW_EV sMeEvent;

			CSW_TRACE(CFW_SMS_TS_ID, TSTXT("Start sms_MTAoProc SMS_Disp_GetTime : %d Sec., pSmsMTInfo->nSmsStorageID=%d\n"),
				(UINT32)(SMS_Disp_GetTime()/256),pSmsMTInfo->nSmsStorageID);
#ifdef CFW_MULTI_SIM
			if (CFW_SMS_INITED != gNewSMSInd[nSimId])
#else
			if (CFW_SMS_INITED != gNewSMSInd)
#endif
			{
#ifdef CFW_MULTI_SIM
				sms_mt_SmsPPErrorReq(0xd3,nSimId);
				gNewSMSInd[nSimId] = CFW_SMS_WAITING_MSG;
#else
				sms_mt_SmsPPErrorReq(0xd3);
				gNewSMSInd = CFW_SMS_WAITING_MSG;
#endif
			}
			else
			{
			SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
			//Store received SMS
			ret = sms_mt_Store_SmsPPReceiveInd(hAo,(api_SmsPPReceiveInd_t *)pEvent->nParam1,&sMeEvent);
			if( ERR_NO_NEED_SAVE == ret )
			{
				return ERR_SUCCESS;
			}
			else if(ret != ERR_SUCCESS)
			{
				//Unregister
				CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, 2,call sms_mt_Store_SmsPPReceiveInd = 0x%x \n ", ret);
				CFW_UnRegisterAO(CFW_SMS_MT_SRV_ID,hAo);
				CSW_PROFILE_FUNCTION_EXIT(sms_MTAoProc);
				return ERR_CFW_INVALID_PARAMETER;
			}
			
			if(pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_ME) 
			{
				pEvent = &sMeEvent;
				pEvent->nTransId = SMS_MT_UTI_MIN;
			}
#ifndef CFW_MULTI_SIM
			else if((CFW_SMS_STORAGE_SM == pSmsMTInfo->nSmsStorageID)     \
			|((CFW_SMS_STORAGE_SM_ME == pSmsMTInfo->nSmsStorageID) && (0xf0 == sMeEvent.nType))
			)
			{
				pSmsMTInfo->nSmsStorageID = CFW_SMS_STORAGE_SM;
				break;
			}
#else
			else
				break;
#endif			
			}
		}
	case CFW_SM_STATE_WAIT:
	{
		//Get the overflow presentation mode
#ifndef CFW_MULTI_SIM
		CFW_CfgGetSmsOverflowInd(&nMode);
#else
		CFW_CfgGetSmsOverflowInd(&nMode, nSimId);
#endif
		//Wait nEventId after calling CFW_SmsWriteMessage()
		if(((pEvent->nEventId == EV_CFW_SMS_WRITE_MESSAGE_RSP) || (pEvent->nEventId == EV_CFW_SIM_WRITE_MESSAGE_RSP)) && 
			(pEvent->nTransId == SMS_MT_UTI_MIN))
		{
			CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, pEvent->nType =0x%x \n ", pEvent->nType );
			if(pEvent->nType == 0) //Write OK
			{
				CFW_IND_EVENT_INFO* pEventInd=NULL;
				pSmsMTInfo->nIndex = (UINT16)pEvent->nParam1;
				//Update SmsStorageInfo
				if((pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_ME) || (pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_SM))
				{
					CFW_SMS_STORAGE_INFO  sStorageInfo;

					SUL_ZeroMemory8(&sStorageInfo, SIZEOF(CFW_SMS_STORAGE_INFO));
#ifndef CFW_MULTI_SIM
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMTInfo->nSmsStorageID);
					if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
						sStorageInfo.usedSlot++;
					if(sStorageInfo.unReadRecords < sStorageInfo.totalSlot)
						sStorageInfo.unReadRecords++;
					CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMTInfo->nSmsStorageID);
#else
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMTInfo->nSmsStorageID, nSimId);
					if(sStorageInfo.usedSlot < sStorageInfo.totalSlot)
						sStorageInfo.usedSlot++;
					if(sStorageInfo.unReadRecords < sStorageInfo.totalSlot)
						sStorageInfo.unReadRecords++;
					CFW_CfgSetSmsStorageInfo(&sStorageInfo,pSmsMTInfo->nSmsStorageID, nSimId);
#endif

					
			//when SIM has no enough memory to save a cohcat SMS, we save the whole concat SMS to ME, 
			//then switch the default memory to SIM.
					if((pSmsMTInfo->nSmsStorageID == CFW_SMS_STORAGE_ME)&&
						pSmsMTInfo->nLongerMsgCount && 
						(pSmsMTInfo->nLongerMsgCount == pSmsMTInfo->nLongerMsgCurrent))
					{
#ifndef CFW_MULTI_SIM
						CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM);
						if(sStorageInfo.usedSlot <= sStorageInfo.totalSlot)
						{
							UINT8 uOption=0;
							UINT8 uNewSmsStorage=0;
							CFW_CfgGetNewSmsOption(&uOption, &uNewSmsStorage);
							CFW_CfgSetNewSmsOption(uOption, CFW_SMS_STORAGE_SM);
						}
#else
						CFW_CfgGetSmsStorageInfo(&sStorageInfo,CFW_SMS_STORAGE_SM, nSimId);
						if(sStorageInfo.usedSlot <= sStorageInfo.totalSlot)
						{
							UINT8 uOption=0;
							UINT8 uNewSmsStorage=0;
							CFW_CfgGetNewSmsOption(&uOption, &uNewSmsStorage, nSimId);
							CFW_CfgSetNewSmsOption(uOption, CFW_SMS_STORAGE_SM, nSimId);
						}

#endif
					}
				}
				//when a new short message arrives, after saving,setting the following
				pEventInd = &sEventInd;
#ifndef CFW_MULTI_SIM
				CFW_CfgGetIndicatorEventInfo(pEventInd);
				sEventInd.sms_receive = 1;
				CFW_CfgSetIndicatorEventInfo(&sEventInd);
#else
				CFW_CfgGetIndicatorEventInfo(pEventInd, nSimId);
				sEventInd.sms_receive = 1;
				CFW_CfgSetIndicatorEventInfo(&sEventInd, nSimId);
#endif
				//Parse received SMS
				if(pSmsMTInfo->isRoutDetail)
					sms_mt_Parse_SmsPPReceiveInd(hAo);
				//if first longer msg, get its index after saving
				if((pSmsMTInfo->isLongerMsg)&& (pSmsMTInfo->nLongerMsgCurrent == 1) && gpLongerMsgParaIndex)
					((CFW_SMS_MT_LONGER_MSG_INDEX*)gpLongerMsgParaIndex)->gLongerMsgFirstIndex = pSmsMTInfo->nIndex;
				//if a longer msg, 更新pMEFilename /pSMFilename
				if(pSmsMTInfo->isLongerMsg)
				{
					if(USING_FS_SAVE_LONG_PARA)
					sms_mt_SetLongPara(hAo);
				}
				//a normal sms, or a long sms and user reassembles
				if((!pSmsMTInfo->isLongerMsg)                                ||  
					(pSmsMTInfo->isLongerMsg && !pSmsMTInfo->isListLongFirst) ||  
					(pSmsMTInfo->isLongerMsg && pSmsMTInfo->isListLongFirst && (pSmsMTInfo->nLongerMsgCurrent == pSmsMTInfo->nLongerMsgCount)))
				{
					if((pSmsMTInfo->isLongerMsg) && (pSmsMTInfo->nLongerMsgCurrent==pSmsMTInfo->nLongerMsgCount) && gpLongerMsgParaIndex)
					{
						if(pSmsMTInfo->isListLongFirst)
						{
							//if last longer msg, CFW reassembles it, then post their first index
							pSmsMTInfo->nIndex = ((CFW_SMS_MT_LONGER_MSG_INDEX*)gpLongerMsgParaIndex)->gLongerMsgFirstIndex;
							CSW_SMS_FREE(gpLongerMsgParaIndex);
							if(pSmsMTInfo->nIndex == 0)
							{
							    //
							}
						}
					}
					if((pSmsMTInfo->isLongerMsg)  && pSmsMTInfo->isListLongFirst && (pSmsMTInfo->nLongerMsgCurrent==pSmsMTInfo->nLongerMsgCount) && !gpLongerMsgPara)
					{
						//???
						//如果系统只收到长短信的一部分,然后关机,再次开机后,接收到的长短信的其他部分,此时不知道其第一条的index
						//当然可以用遍沥方法得到,目前还没有这样实现
						//pSmsMTInfo->nIndex = 0;
					}
					if((pSmsMTInfo->nSmsType == 1) && (!(pSmsMTInfo->isRoutDetail)))// 1: DELIVER SMS : Storage-Index
					{
						//Send a MSG to AT , Type = 1
						CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, 4,param1=0x%x,param2=0x%x\n ",
								pSmsMTInfo->nSmsStorageID, pSmsMTInfo->nIndex);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsMTInfo->nSmsStorageID,pSmsMTInfo->nIndex, SMS_MT_UTI, 1);
#else
						CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsMTInfo->nSmsStorageID,pSmsMTInfo->nIndex, SMS_MT_UTI |(nSimId<<24), 1);
#endif
					}
				}

				if((pSmsMTInfo->nSmsType == 0) &&(!(pSmsMTInfo->isRoutDetail)))// 3:STATUS REPORT : Storage-Index
				{
					//Send a MSG to AT , Type = 3
					CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, 5, param1=0x%x,param2=0x%x\n ",
						pSmsMTInfo->nSmsStorageID, pSmsMTInfo->nIndex);
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsMTInfo->nSmsStorageID,pSmsMTInfo->nIndex, SMS_MT_UTI, 3);
#else
					CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsMTInfo->nSmsStorageID,pSmsMTInfo->nIndex, SMS_MT_UTI |(nSimId<<24), 3);
#endif
				} 
				//Save to specified memory location successfully and the memory has been full.
				if((nMode == 1) && (pEvent->nParam2 == 1))
				{
					CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, 6, param1=1,param2=0x%x\n ",
						pSmsMTInfo->nSmsStorageID);
#ifndef CFW_MULTI_SIM
					CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,1,pSmsMTInfo->nSmsStorageID,SMS_MT_UTI,0);
#else
					CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,1,pSmsMTInfo->nSmsStorageID,SMS_MT_UTI |(nSimId<<24),0);
#endif
				}
				//Post Ack Event to SCL
				// if(pSmsMTInfo->nSmsType) //if receive a SMS
				//Both SMS and SR need SmsPPAckReq???
#ifndef CFW_MULTI_SIM
				sms_mt_SmsPPAckReq();
#else
				sms_mt_SmsPPAckReq(nSimId);
#endif
			}
			else if(pEvent->nType == 0xf0) //Write error, We think the Storage has been FUll
			{
				UINT8 nNewCount=0x0;
				if(nMode == 1)
				{
					//Post EV_CFW_SMS_INFO_IND Event to AT 
					//Buffer full and new message waiting in Memory Storage(SC, ME) for delivery to phone.
					CFW_SMS_STORAGE_INFO sStorageInfo;
#ifndef CFW_MULTI_SIM
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMTInfo->nSmsStorageID);
#else
					CFW_CfgGetSmsStorageInfo(&sStorageInfo,pSmsMTInfo->nSmsStorageID, nSimId);
#endif
					//Be sure whether the storage is full or not
					if(sStorageInfo.usedSlot == sStorageInfo.totalSlot)
					{
						UINT32 uParam2=0;
						uParam2 = pSmsMTInfo->nSmsStorageID|(pSmsMTInfo->nLongerMsgCount <<8);
						CSW_TRACE(CFW_SMS_TS_ID, "sms_MTAoProc, 5, param1=2,param2=0x%x\n ",
							uParam2);
#ifndef CFW_MULTI_SIM
						CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,2,uParam2,SMS_MT_UTI,0);
					}
				}
				ret = CFW_CfgGetSmsFullForNew(&nNewCount);
				nNewCount++;;
				ret = CFW_CfgSetSmsFullForNew(nNewCount);
				//Post Error Event to SCL
				sms_mt_SmsPPErrorReq(0xd3); //0xd3: Memory capacity exceeded
#else
						CFW_PostNotifyEvent(EV_CFW_SMS_INFO_IND,2,uParam2,SMS_MT_UTI |(nSimId<<24),0);
					}
				}
				ret = CFW_CfgGetSmsFullForNew(&nNewCount, nSimId);
				nNewCount++;;
				ret = CFW_CfgSetSmsFullForNew(nNewCount, nSimId);
				//Post Error Event to SCL
				sms_mt_SmsPPErrorReq(0xd3, nSimId); //0xd3: Memory capacity exceeded
#endif
			}
		}
		else if(( EV_CFW_SAT_RESPONSE_RSP == pEvent->nEventId ))
		{
			if( 0x90 == pEvent->nParam2 )
			{
				CSW_TRACE(CFW_SMS_TS_ID, "0x90 == pEvent->nParam1\n ");
#ifndef CFW_MULTI_SIM
				sms_mt_SmsPPAckReq();
#else
				sms_mt_SmsPPAckReq(nSimId);
#endif
			}
			else if( 0x91 == pEvent->nParam2)
			{
				CSW_TRACE(CFW_SMS_TS_ID, "0x91 == pEvent->nParam1\n ");
				return ERR_SUCCESS;
			}
			else
			{
#ifndef CFW_MULTI_SIM 
        		sms_mt_SmsPPErrorReq(0xD5);
#else
        		sms_mt_SmsPPErrorReq(0xD5,nSimId);
#endif
				CSW_TRACE(CFW_SMS_TS_ID, "pEvent->nParam1 %x\n ",pEvent->nParam1);
			}
		}
		else if(( EV_CFW_SAT_CMDTYPE_IND == pEvent->nEventId ))
		{
			CSW_TRACE(CFW_SMS_TS_ID, "EV_CFW_SAT_CMDTYPE_IND nType %x\n",pEvent->nType);
#ifndef CFW_MULTI_SIM
			CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pEvent->nParam1, 0, 1, pEvent->nType);
			sms_mt_SmsPPAckReq();
#else
			CFW_PostNotifyEvent(EV_CFW_SAT_CMDTYPE_IND, pEvent->nParam1, 0, 1 |(nSimId<<24), pEvent->nType);
			sms_mt_SmsPPAckReq(nSimId);
#endif
		}
	//Free pSmsMTInfo->pInMsg
		CSW_SMS_FREE(pSmsMTInfo->pInMsg);
		//Unregister
		CFW_UnRegisterAO(CFW_SMS_MT_SRV_ID,hAo);
		break;
    	 } 
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_MTAoProc  error\n");
		break;
	}
	CSW_PROFILE_FUNCTION_EXIT(sms_MTAoProc);
	return ERR_SUCCESS;
}   

#ifndef CFW_MULTI_SIM
UINT32 sms_CBAoProc(HAO hAo, CFW_EV* pEvent)
{
    CFW_SMS_CB_INFO*    pSmsCBInfo = NULL; //Point to CB private data.
    UINT32              ret=0x0;    //CB State
    UINT8               nFormat=0x0,nSmsStorageId=0x0,nOption=0x0,nMode=0x0;
    CSW_PROFILE_FUNCTION_ENTER(sms_CBAoProc);

    //Get private date
    pSmsCBInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsCBInfo){
    	CSW_PROFILE_FUNCTION_EXIT(sms_CBAoProc);
      return ERR_NO_MORE_MEMORY;
      }
    
    CFW_CfgGetSmsCB(&nMode,NULL,NULL);
    //nMode ==0 :Accepts CB
    if(nMode == 0)
    {
        //Get SMS StorageID
        ret= CFW_CfgGetNewSmsOption(&nOption,&nSmsStorageId);
        if (ret == ERR_SUCCESS)
        {
            pSmsCBInfo->nCBStorageID = (UINT8)(nSmsStorageId & CFW_SMS_STORAGE_BM);
            pSmsCBInfo->isRoutDetail = (UINT8)(nOption & CFW_SMS_ROUT_DETAIL_INFO); 
        }
        else
        {
		CSW_TRACE(CFW_SMS_TS_ID, "CFW_CfgGetNewSmsOption error ret=0x%x\n",ret);
        	CSW_PROFILE_FUNCTION_EXIT(sms_CBAoProc);
            return ERR_CFW_INVALID_PARAMETER;
        }
    
        if(pSmsCBInfo->nCBStorageID == CFW_SMS_STORAGE_BM)
        {
            UINT32 nTime=0x0, nAppInt32=0x0;
            CFW_EV sEvent;
            
            SUL_ZeroMemory8(&sEvent,SIZEOF(CFW_EV));
            ret = CFW_MeWriteMessage(CFW_SMS_STORAGE_BM,0,(UINT8*)pEvent->nParam1,SMS_MO_ONE_PDU_SIZE,nTime,nAppInt32,&sEvent);
            if(ret == ERR_SUCCESS)
            {
            }
            else if(ret == ERR_CFW_INVALID_PARAMETER )
            {
            	CSW_PROFILE_FUNCTION_EXIT(sms_CBAoProc);
                return ERR_CFW_INVALID_PARAMETER;
            }
         }

        //Parse received CB
        if(pSmsCBInfo->isRoutDetail)
        {
            UINT32  nStructureSize=0x0; 
            CFW_NEW_SMS_NODE*    pSmsNode    = NULL;
            //Get SMS format from user setting
            CFW_CfgGetSmsFormat(&nFormat);
            if(pEvent->nEventId == API_SMSCB_RECEIVE_IND)
            {
                api_SmsCBReceiveInd_t* pInMsg=NULL;
                pInMsg = (api_SmsCBReceiveInd_t*)pEvent->nParam1; 
                if(nFormat) //Text
                {
                    //todo
                }
                else  //Pdu
                {
                    CFW_SMS_PDU_INFO*    pSmsPDU     = NULL;  //PDU Format
                    
                    nStructureSize = (SIZEOF(CFW_NEW_SMS_NODE)+SIZEOF(CFW_SMS_PDU_INFO)+pInMsg->DataLen);
                    pSmsNode = (CFW_NEW_SMS_NODE*)CSW_SMS_MALLOC(nStructureSize);
                    if(!pSmsNode)
              			{
              				CSW_TRACE(CFW_SMS_TS_ID, "sms_CBAoProc  pSmsNode  malloc error!!! \n ");
              				CSW_PROFILE_FUNCTION_EXIT(sms_CBAoProc);
              				return ERR_NO_MORE_MEMORY;
                    }
                    SUL_ZeroMemory8(pSmsNode, nStructureSize);
                    pSmsNode->pNode = (CFW_SMS_PDU_INFO*)((UINT8*)pSmsNode + SIZEOF(CFW_NEW_SMS_NODE));
                    //Set pSmsPDU Value
                    pSmsPDU = (CFW_SMS_PDU_INFO*)(pSmsNode->pNode);
                    pSmsPDU->nDataSize = pInMsg->DataLen  ;
                    pSmsPDU->nStatus  = 0;
                    SUL_MemCopy8(pSmsPDU->pData, pInMsg->Data, (UINT32)pInMsg->DataLen);
                    pSmsNode->nConcatPrevIndex = 0xffff;
                    pSmsNode->nConcatCurrentIndex = pSmsCBInfo->nIndex;
                    pSmsNode->nStorage = pSmsCBInfo->nCBStorageID;
                    pSmsNode->nType = 0x30;

                    CFW_PostNotifyEvent(EV_CFW_NEW_SMS_IND, (UINT32)pSmsNode,0, SMS_MT_UTI, 0);
                }
            }
            else if(pEvent->nEventId == API_SMSCB_RECEIVE_IND)
            {
                //api_SmsCBPageInd_t
            }
        }
        else
        {
        }
    }
    CSW_PROFILE_FUNCTION_EXIT(sms_CBAoProc);
    return ERR_SUCCESS;
}   
#endif

#ifdef  CFW_MULTI_SIM
static BOOL bMEFinish = FALSE;
#endif
//Task
// 1.Initialize ME DB, Create a new one if NULL                : sms_db_Init()
// 2.Get Storage information of SIM, including longer sms info : CFW_SIMSmsInit()
UINT32 sms_InitAoProc(HAO hAo, CFW_EV* pEvent)
{
    CFW_SMS_INIT_HANDLE*   pSmsMOInfo = NULL; 
    UINT32                 nGetUti=0x0,nState = 0x0,ret=0x0;   
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimId =  CFW_SIM_END;
	CFW_GetSimID(hAo, &nSimId);
#endif	
    CSW_PROFILE_FUNCTION_ENTER(sms_InitAoProc);  
    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsMOInfo){
    	CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
      return ERR_NO_MORE_MEMORY;
      }
    
    //Get current State
    nState = CFW_GetAoState(hAo);
   
CSW_TRACE(CFW_SIM_TS_ID, TSTXT("sms_InitAoProc, init start \n"));	
    //Get private date
    pSmsMOInfo = CFW_GetAoUserData(hAo);
    //Verify memory
    if(!pSmsMOInfo)
    {
        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
        CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti,SMS_INIT_EV_ERR_TYPE);
#else
        CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti |(nSimId<<24),SMS_INIT_EV_ERR_TYPE);
#endif
        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
        CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
        return ERR_NO_MORE_MEMORY;
    }

    //Get current State
    if(pEvent == (CFW_EV*)0xffffffff)
        nState = CFW_SM_STATE_IDLE;
    else 
        nState = CFW_GetAoState(hAo);
    CSW_TRACE(CFW_SMS_TS_ID, "sms_InitAoProc, nState=0x%x \n ", nState);
    //SMS MO State machine process
    switch(nState)
    {
        case CFW_SM_STATE_IDLE:
            {
		 
                CFW_SMS_STORAGE_INFO* pMEStorageInfo=NULL; 
                CFW_SMS_STORAGE_INFO  sStorageInfo;
                CFW_EV                sMeEvent;
                UINT32                nTmpEventP1=0x0;
                UINT8                 nIsFail=0x0;
#ifdef CFW_MULTI_SIM

		CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_InitAoProc, bMEFinish=%d \n"), bMEFinish);	
		if( TRUE == bMEFinish )
		{
	               ret = CFW_SimSmsInit(nSimId);
		CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_InitAoProc, don't need to init ME 1 ret=0x%x\n"), ret);	
	                if(ret == ERR_SUCCESS)
	                {
	                    CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
	                }
	                else 
	                {
	                    CFW_GetUTI(hAo, &nGetUti);
	                    CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti |(nSimId<<24), SMS_INIT_EV_ERR_TYPE);
	                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
	                    CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
	                    return ERR_CMS_UNKNOWN_ERROR;
	                }
	            break;			
		}
#endif
                //Zero memory
                SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
                SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));

#ifndef CFW_MULTI_SIM
                //Init ME DB
                ret = sms_db_InitEX();   
                if((ret != ERR_SUCCESS) && (ret != ERR_DB_ALREADY_EXIST))
                {
                    CFW_GetUTI(hAo, &nGetUti);
                    CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti,SMS_INIT_EV_ERR_TYPE);
                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
#endif

                //Get ME sms info, then Set them
                //??? maybe just do it one time, think about it later
                sMeEvent.nParam1 = (UINT32)((UINT8*)CSW_SMS_MALLOC(SIZEOF(CFW_SMS_STORAGE_INFO)));
                if((UINT8*)sMeEvent.nParam1 == NULL)
                {
			CSW_TRACE(CFW_SMS_TS_ID, "sms_InitAoProc  sMeEvent.nParam1  malloc error!!! \n ");
                    CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti,SMS_INIT_EV_ERR_TYPE);
#else
                    CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti |(nSimId<<24), SMS_INIT_EV_ERR_TYPE);
#endif
                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
				
#ifdef CFW_MULTI_SIM
		for(nSimId = CFW_SIM_0; nSimId< CFW_SIM_COUNT; nSimId++)	
		{
#endif	

                SUL_ZeroMemory8((UINT8*)sMeEvent.nParam1, SIZEOF(CFW_SMS_STORAGE_INFO));
                pMEStorageInfo = (CFW_SMS_STORAGE_INFO*)(sMeEvent.nParam1);
                //Get ME totalSlot and usedSlot
#ifndef CFW_MULTI_SIM
                CFW_MeGetStorageInfo(CFW_SMS_STORAGE_ME,CFW_SMS_STORED_STATUS_STORED_ALL,&sMeEvent);
#else
                CFW_MeGetStorageInfo(nSimId, CFW_SMS_STORED_STATUS_STORED_ALL,&sMeEvent);
#endif
                SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
                if(sMeEvent.nType == 0)
                {
                    sStorageInfo.storageId = CFW_SMS_STORAGE_ME;
                    sStorageInfo.totalSlot = pMEStorageInfo->totalSlot;
                    sStorageInfo.usedSlot  = pMEStorageInfo->usedSlot;
                    //Get ME unReadRecords
                    nTmpEventP1 = sMeEvent.nParam1;
                    SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
                    SUL_ZeroMemory8((UINT8*)nTmpEventP1, SIZEOF(CFW_SMS_STORAGE_INFO));
                    sMeEvent.nParam1 = nTmpEventP1;
#ifndef CFW_MULTI_SIM
                    CFW_MeGetStorageInfo(CFW_SMS_STORAGE_ME,CFW_SMS_STORED_STATUS_UNREAD,&sMeEvent);
#else
                    CFW_MeGetStorageInfo(nSimId, CFW_SMS_STORED_STATUS_UNREAD,&sMeEvent);
#endif
                    if(sMeEvent.nType == 0)
                    {
                        sStorageInfo.unReadRecords = pMEStorageInfo->usedSlot;
                        //Get ME readRecords
                        nTmpEventP1 = sMeEvent.nParam1;
                        SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
                        SUL_ZeroMemory8((UINT8*)nTmpEventP1, SIZEOF(CFW_SMS_STORAGE_INFO));
                        sMeEvent.nParam1 = nTmpEventP1;
#ifndef CFW_MULTI_SIM
                        CFW_MeGetStorageInfo(CFW_SMS_STORAGE_ME,CFW_SMS_STORED_STATUS_READ,&sMeEvent);
#else
                        CFW_MeGetStorageInfo(nSimId,CFW_SMS_STORED_STATUS_READ,&sMeEvent);
#endif
                        if(sMeEvent.nType == 0)
                        {
                            UINT8 nSentStatus=0x0;

                            sStorageInfo.readRecords = pMEStorageInfo->usedSlot;
                            //Get ME sentRecords
                            nTmpEventP1 = sMeEvent.nParam1;
                            SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
                            SUL_ZeroMemory8((UINT8*)nTmpEventP1, SIZEOF(CFW_SMS_STORAGE_INFO));
                            sMeEvent.nParam1 = nTmpEventP1;
                            nSentStatus = CFW_SMS_STORED_STATUS_SENT_NOT_SR_REQ | CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_RECV | 
                                          CFW_SMS_STORED_STATUS_SENT_SR_REQ_NOT_STORE | CFW_SMS_STORED_STATUS_SENT_SR_REQ_RECV_STORE;
#ifndef CFW_MULTI_SIM
                            CFW_MeGetStorageInfo(CFW_SMS_STORAGE_ME,nSentStatus,&sMeEvent);
#else
                            CFW_MeGetStorageInfo(nSimId, nSentStatus,&sMeEvent);
#endif
                            if(sMeEvent.nType == 0)
                            {
                                sStorageInfo.sentRecords = pMEStorageInfo->usedSlot;
                                //Get ME unsentRecords
                                nTmpEventP1 = sMeEvent.nParam1;
                                SUL_ZeroMemory8(&sMeEvent,SIZEOF(CFW_EV));
                                SUL_ZeroMemory8((UINT8*)nTmpEventP1, SIZEOF(CFW_SMS_STORAGE_INFO));
                                sMeEvent.nParam1 = nTmpEventP1;
#ifndef CFW_MULTI_SIM
                                CFW_MeGetStorageInfo(CFW_SMS_STORAGE_ME,CFW_SMS_STORED_STATUS_UNSENT,&sMeEvent);
#else
                                CFW_MeGetStorageInfo(nSimId,CFW_SMS_STORED_STATUS_UNSENT,&sMeEvent);
#endif
					CSW_TRACE(CFW_SMS_TS_ID, TSTXT("MeGetSI 0\n"));			

                                if(sMeEvent.nType == 0)
                                {
                                    sStorageInfo.unsentRecords = pMEStorageInfo->usedSlot;
#ifndef CFW_MULTI_SIM
                                    CFW_CfgSetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_ME);
#else
                                    CFW_CfgSetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_ME, nSimId);
#endif

					CSW_TRACE(CFW_SMS_TS_ID, TSTXT("MeGetSI 1 \n"));	

					//Add by lixp at 20090213 for free crash issue
#ifdef CFW_MULTI_SIM					
					if( nSimId == ( CFW_SIM_COUNT -1 ))
					{
                                    	CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
					}
#else
                                	CSW_SMS_FREE((UINT8*)sMeEvent.nParam1);
#endif					
                                }//unsentRecords
                                else
                                {
                                    nIsFail = 1;
                                }
                            }//sentRecords
                            else
                            {
                                nIsFail = 1;
                            }
                        }//readRecords
                        else
                        {
                            nIsFail = 1;
                        }
                    }//unReadRecords
                    else
                    {
                        nIsFail = 1;
                    }
                }//totalSlot and usedSlot
                else
                {
                    nIsFail = 1;
                }
				
#ifdef CFW_MULTI_SIM
			if(nIsFail)
				break;
		}
#endif

                if(nIsFail)
                {
                    CSW_SMS_FREE(pMEStorageInfo);
                    CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti,SMS_INIT_EV_ERR_TYPE);
#else
                    CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti | (nSimId<<24),SMS_INIT_EV_ERR_TYPE);
#endif
                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
                //Init SIM long sms
#ifndef CFW_MULTI_SIM
                ret = CFW_SimSmsInit();
#else
                bMEFinish = TRUE;
		CFW_GetSimID(hAo, &nSimId);
                ret = CFW_SimSmsInit(nSimId);
#endif
                if(ret == ERR_SUCCESS)
                {
                    CFW_SetAoState(hAo, CFW_SM_STATE_WAIT);
                }
                else 
                {
                    CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                    CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti,SMS_INIT_EV_ERR_TYPE);
#else
                    CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti |(nSimId<<24), SMS_INIT_EV_ERR_TYPE);
#endif
                    CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                    CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
                    return ERR_CMS_UNKNOWN_ERROR;
                }
            break;
            }//case CFW_SM_STATE_IDLE:
        case CFW_SM_STATE_WAIT:
            {
                if(pEvent->nEventId == EV_CFW_SIM_INIT_SMS_RSP) 
                {
                    if(pEvent->nType == 0)
                    {
                        CFW_SMS_INIT_INFO*     pSIMInitInfo=NULL;
                        CFW_SMS_STORAGE_INFO   sStorageInfo;
#ifdef CFW_MULTI_SIM
				nSimId = pEvent->nFlag;
#endif
                        //Zero memory
                        SUL_ZeroMemory8(&sStorageInfo,SIZEOF(CFW_SMS_STORAGE_INFO));
                        pSIMInitInfo = (CFW_SMS_INIT_INFO*)(pEvent->nParam1);
                        //Get SIM sms Storage info, then Set them
                        sStorageInfo.storageId      = CFW_SMS_STORAGE_SM;
                        sStorageInfo.totalSlot      = pSIMInitInfo->totalSlot;
                        sStorageInfo.usedSlot       = pSIMInitInfo->usedSlot;
                        sStorageInfo.readRecords    = pSIMInitInfo->readRecords;
                        sStorageInfo.unReadRecords  = pSIMInitInfo->unReadRecords;
                        sStorageInfo.sentRecords    = pSIMInitInfo->sentRecords;
                        sStorageInfo.unsentRecords  = pSIMInitInfo->unsentRecords;
                        sStorageInfo.unknownRecords = pSIMInitInfo->unknownRecords;
#ifndef CFW_MULTI_SIM
                        CFW_CfgSetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM);
#else
                        CFW_CfgSetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM, nSimId);
#endif

#if 0
//#ifdef CFW_MULTI_SIM
				SUL_MemSet8(&sStorageInfo, 0, sizeof(CFW_SMS_STORAGE_INFO));
				UINT32 nRet = CFW_CfgGetSmsStorageInfo(&sStorageInfo, CFW_SMS_STORAGE_SM, (nSimId == CFW_SIM_0?CFW_SIM_1: CFW_SIM_0));

				if(!sStorageInfo.totalSlot)//the other card not been init
				{
					CFW_SetAoProcCode(hAo, CFW_AO_PROC_CODE_MULTIPLE|CFW_AO_PROC_CODE_CONSUMED);
					return ERR_SUCCESS;
				}
#endif
                        //SMS INIT OK
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,0,nGetUti,SMS_INIT_EV_OK_TYPE);
#else
                        CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,0,nGetUti |(nSimId<<24),SMS_INIT_EV_OK_TYPE);
#endif
                        if(pEvent->nParam1)
                        {
                           //CSW_SMS_FREE(pSIMInitInfo->pLongInfo);
                           //pSIMInitInfo->pLongInfo = (UINT32)NULL;
                           CSW_SMS_FREE(pEvent->nParam1);
                           pEvent->nParam1 = (UINT32)NULL;

                        }
                        
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
                        return ERR_SUCCESS;
                    }
                    else
                    {
                        CFW_GetUTI(hAo, &nGetUti);
#ifndef CFW_MULTI_SIM
                        CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti,SMS_INIT_EV_ERR_TYPE);
#else
                        CFW_PostNotifyEvent(EV_CFW_SRV_STATUS_IND,0,1,nGetUti |(nSimId<<24),SMS_INIT_EV_ERR_TYPE);
#endif
                        CFW_UnRegisterAO(CFW_APP_SRV_ID,hAo);
                        CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
                        return ERR_CMS_UNKNOWN_ERROR;
                    }

                }//if(pEvent->nEventId == EV_CFW_SIM_INIT_SMS_RSP) 
            break;
            }// case CFW_SM_STATE_WAIT:
	 default:
		CSW_TRACE(CFW_SMS_TS_ID,"sms_MTAoProc  error\n");
		break;
    }//switch(nState)
    CSW_PROFILE_FUNCTION_EXIT(sms_InitAoProc);
    return ERR_SUCCESS;
}   


UINT32 CFW_SmsInitCompleteProc(HAO hAo, CFW_EV* pEvent)
{
	CFW_SMS_INIT *pSmsInit = NULL;
	UINT32 nRet = 0x00;
	
	CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitCompleteProc \n"));
	CSW_PROFILE_FUNCTION_ENTER(CFW_SmsInitCompleteProc);
	
	if( (hAo == 0) || (pEvent == NULL))
	{
		CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitCompleteProc  ao[0x%x] pEvent[0x%x] ERROR\n"),hAo,pEvent);
		CSW_PROFILE_FUNCTION_EXIT(CFW_SmsInitCompleteProc);
		return ERR_CFW_INVALID_PARAMETER;
	}
		
#ifdef CFW_MULTI_SIM
	CFW_SIM_ID nSimID;
	nSimID = CFW_GetSimCardID(hAo);
#endif

	pSmsInit = CFW_GetAoUserData(hAo);

	switch (pSmsInit->n_CurrStatus)
	{
	case CFW_SM_STATE_IDLE:
		{	
#ifdef CFW_MULTI_SIM
			CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitCompleteProc CFW_SM_STATE_IDLE gNewSMSInd[0x%x]= 0x%x\n"),nSimID,gNewSMSInd[nSimID]);
#else
			CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitCompleteProc CFW_SM_STATE_IDLE gNewSMSInd= 0x%x\n"),gNewSMSInd);
#endif
			if ((UINT32)pEvent == 0xFFFFFFFF)
			{
#ifdef CFW_MULTI_SIM
				if( CFW_SMS_NOINITED == gNewSMSInd[nSimID] )
				{
					gNewSMSInd[nSimID] = CFW_SMS_INITED;
					CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
					return ERR_SUCCESS;

				}
				else if( CFW_SMS_INITED == gNewSMSInd[nSimID] )
				{
					CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
					return ERR_SUCCESS;					
				}
				else if( CFW_SMS_WAITING_MSG == gNewSMSInd[nSimID] )
				{
					gNewSMSInd[nSimID] = CFW_SMS_INITED;
				}
				else
				{
					CSW_TRACE(CFW_SMS_TS_ID, TSTXT("Invalid status gNewSMSInd %x\n"),gNewSMSInd[nSimID]);
					CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
					return ERR_SUCCESS;					
				}
#else
				if( CFW_SMS_NOINITED == gNewSMSInd )
				{
					gNewSMSInd = CFW_SMS_INITED;
					CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
					return ERR_SUCCESS;

				}
				else if( CFW_SMS_INITED == gNewSMSInd )
				{
					CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
					return ERR_SUCCESS;					
				}
				else if( CFW_SMS_WAITING_MSG == gNewSMSInd )
				{
					gNewSMSInd = CFW_SMS_INITED;
				}
				else
				{
					CSW_TRACE(CFW_SMS_TS_ID, TSTXT("Invalid status gNewSMSInd %x\n"),gNewSMSInd);
					CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
					return ERR_SUCCESS;					
				}
#endif
	
				
#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS == ( nRet = sms_mo_SmsPPSendMMAReq(nSimID) ))
#else
                if (ERR_SUCCESS == ( nRet = sms_mo_SmsPPSendMMAReq() ))
#endif
				{
					pSmsInit->n_PrevStatus = pSmsInit->n_CurrStatus;
					pSmsInit->n_CurrStatus = CFW_SM_STATE_WAIT;
				}
				else
				{
					CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_mo_SmsPPSendMMAReq Error 0x%x\n"),nRet);
					CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
				}
			}
		}
		break;

	case CFW_SM_STATE_WAIT:
		{
			if (pEvent->nEventId == API_SMSPP_ACK_IND)
			{
				CSW_TRACE(CFW_SMS_TS_ID, TSTXT("Receive API_SMSPP_ACK_IND\n"));
				CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
			}
			else if (pEvent->nEventId == API_SMSPP_ERROR_IND)
			{
				CSW_TRACE(CFW_SMS_TS_ID, TSTXT("Receive API_SMSPP_ERROR_IND and We will retry\n"));
#ifdef CFW_MULTI_SIM
                if (ERR_SUCCESS != ( nRet = sms_mo_SmsPPSendMMAReq(nSimID) ))
#else
                if (ERR_SUCCESS != ( nRet = sms_mo_SmsPPSendMMAReq() ))
#endif
				{
					CSW_TRACE(CFW_SMS_TS_ID, TSTXT("sms_mo_SmsPPSendMMAReq Error 0x%x\n"),nRet);
					CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
				}
			}
			else
			{
				CFW_UnRegisterAO(CFW_APP_SRV_ID, hAo);
				CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitCompleteProc ERROR!!! \n"));
			}
		}
	break;
	default:
			CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitCompleteProc state ERROR 0x%x \n"),pSmsInit->n_CurrStatus);
		break;
	}
	
	CSW_TRACE(CFW_SMS_TS_ID, TSTXT("CFW_SmsInitCompleteProc end \n"));
	CSW_PROFILE_FUNCTION_EXIT(CFW_SmsInitCompleteProc);

	return ERR_SUCCESS;
}

#ifdef CFW_CUSTOM_IMEI_COUNT

PRIVATE UINT8 gCustomSendIMEINum[42] = {0x00,};
PRIVATE UINT8 gCustomSendIMEINumLen = 0x00;

#ifdef CFW_MULTI_SIM

PRIVATE UINT8 gCustomSendIMEINumFlag[CFW_SIM_COUNT] = {0x00,};

#else

PRIVATE UINT8 gCustomSendIMEINumFlag = 0x00;

#endif

#ifdef CFW_MULTI_SIM 

UINT32 CFW_CfgSetCustomSendIMEI(UINT8 nFlag ,CFW_SIM_ID nSimID)
{
  return ERR_SUCCESS;
}
UINT32 CFW_CfgGetCustomSendIMEI(UINT8 *pFlag , CFW_SIM_ID nSimID)
{
  *pFlag = 0;
  return ERR_SUCCESS;
}

#else

UINT32 CFW_CfgSetCustomSendIMEI(UINT8 nFlag )
{
  return ERR_SUCCESS;
}

UINT32 CFW_CfgGetCustomSendIMEI(UINT8 *pFlag )
{
  *pFlag = 0;
  return ERR_SUCCESS;
}

#endif

UINT32 CFW_RegisterCustomSendIMEI( UINT8*pNum, UINT8 nNumLen )
{
  SUL_MemCopy8(gCustomSendIMEINum,pNum,gCustomSendIMEINumLen = nNumLen);
}

VOID CFW_CustomSendIMEI(CFW_SIM_ID nSimID)
{
    //Example: UINT8 pNumber[]   = "13811189836";//{0x68, 0x31, 0x76, 0x21, 0x42, 0x81, 0xF1 };
    //UINT8 nNumberSize = 11;
    
    CFW_DIALNUMBER      sNumber;
    UINT8 pBcd[21]    = {0x00,};
    UINT8 nIMEI[30]   = {0x00,};
    UINT8 nIMEILen    = 0x00;
    UINT8 nFormat     = 0x00;
    UINT8 nFlag       = 0x00;
    
#ifdef CFW_MULTI_SIM     

    CFW_CfgGetCustomSendIMEI(&nFlag ,nSimID);

#else

    CFW_CfgGetCustomSendIMEI(&nFlag );

#endif

    if( 0x01 == nFlag )
    {
      return;
    }
    
    if( 0x00 == gCustomSendIMEINumLen )
    {
      return ;
    }
    
#ifdef CFW_MULTI_SIM

    if( 0x00 == gCustomSendIMEINumFlag[nSimID] )
    {
      gCustomSendIMEINumFlag[nSimID] = 1;
    }
    else
    {
      return;
    }
    
#else

    if( 0x00 == gCustomSendIMEINumFlag )
    {
      
      gCustomSendIMEINumFlag = 1;
    }
    else
    {
      return;
    }
    
#endif
    sNumber.nDialNumberSize = SUL_AsciiToGsmBcd(gCustomSendIMEINum, gCustomSendIMEINumLen, pBcd);
    
    sNumber.pDialNumber     = pBcd; 
    SUL_MemCopy8(nIMEI,"IMEI:",5);
    //Send MSG
#ifdef CFW_MULTI_SIM

    CFW_EmodGetIMEI(nIMEI+5,&nIMEILen,nSimID);

    CFW_CfgGetSmsFormat(&nFormat , nSimID);

    CFW_CfgSetSmsFormat(1, nSimID); //0 PDU mode; 1 text mode 
    if( CFW_SmsSendMessage(&sNumber, nIMEI, 30, 0x10, nSimID) != 0 )  // Text Mode with GSM 7bit is OK.
    {
      CFW_CfgSetSmsFormat( nFormat , nSimID );
    }
#else

    CFW_EmodGetIMEI( nIMEI + 5, &nIMEILen );

    CFW_CfgGetSmsFormat( &nFormat );

    CFW_CfgSetSmsFormat( 1 ); //0 PDU mode; 1 text mode 
    if( CFW_SmsSendMessage( &sNumber, nIMEI, 30, 0x10 ) != 0x00 )  // Text Mode with GSM 7bit is OK.
    {
      CFW_CfgSetSmsFormat( nFormat );
    }
    
#endif

}
#ifdef CFW_MULTI_SIM
VOID CFW_CustomSendIMEIProc(UINT8 nCSFlag, CFW_SIM_ID nSimID )
#else
VOID CFW_CustomSendIMEIProc(UINT8 nFlag)
#endif
{
    UINT8 nFlag       = 0x00;
    
#ifdef CFW_MULTI_SIM
    CFW_CfgGetCustomSendIMEI(&nFlag  ,nSimID );
#else
    CFW_CfgGetCustomSendIMEI(&nFlag );
#endif

    if(( 0x00 == nFlag ) && (TRUE == nCSFlag))
    {
#ifdef CFW_MULTI_SIM
      CFW_CfgSetCustomSendIMEI(1  ,nSimID );
#else
      CFW_CfgSetCustomSendIMEI(1  );
#endif
    }
    else
    {
      return;
    }
  
#ifdef CFW_MULTI_SIM
    if( 0x01 == gCustomSendIMEINumFlag[nSimID] )
    {
      gCustomSendIMEINumFlag[nSimID] = 0;
    }
    else    
    {
      return;
    }
#else
    if( 0x01 == gCustomSendIMEINumFlag )
    {
      gCustomSendIMEINumFlag = 0;
    }
    else
    {
      return;
    }
#endif
	
}

#endif
// Made by LXP
#endif // ENABLE 

