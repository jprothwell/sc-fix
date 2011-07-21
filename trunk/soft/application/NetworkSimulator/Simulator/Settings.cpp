/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/**************************************************************
	FILENAME	: Settings.cpp
  	PURPOSE		: Setting Plugins for Network Simulator
	REMARKS		: nil
	AUTHOR		: 
	DATE		: Jun 16,03
**************************************************************/

/* Header files require for Settings.cpp */
#include "Afx.h"
#include "stdafx.h"
#include "string.h"
#include "stdlib.h"
#include "MemAllocator.h"
#include "GlobalConstants.h"
#include "QueueGProt.h"
#include "settings.h"

#include "winbase.h"
#include "windows.h"
#include "GlobalDefines.h"
#include "TaskInit.h"
#include "NSRThread.h"


/*	Global Variable Declearation	*/
CString t1="Settings";
CString t2="Sim Lock";
CString t3="Phone Lock";
CString t4="Gprs Type";
CString t5="NW Band";

int BootupDone=0;
int ReadValueINI=0;
int VALIDATE_PIN=0;
int SIM_LOCK;
int PHONE_LOCK;
void *gBuffer_Settings;
U8 phonepswd[MAX_SIM_PASSWD_LEN];

#define PASSWORD_FILE "passwordfile"

void WritePasswordData_file();
void ReadPasswordData_file();


extern CNSRThread *myNSRThread;

/*****************************************************************************
* FUNCTION
*	SetBufferForSettings
* DESCRIPTION
*	Fills the incoming message in setting global buffer
* PARAMETERS
*	void * - input buffer pointer.
* RETURNS
*	void
*****************************************************************************/
void SetBufferForSettings( void * dataBuf )
{
	gBuffer_Settings = dataBuf ;
}
	
/*****************************************************************************
* FUNCTION
*	CheckSecurityBootup
* DESCRIPTION
*	Verify the PIN and retrun TRUE in case of SUCCESS else FALSE
* PARAMETERS
*	void 
* RETURNS
*	void*	-	SMU_LOCK_RESPONSE_STRUCT for the PIN verfiy response
*****************************************************************************/
void *CheckSecurityBootup()
{
  SMU_LOCK_REQUEST_STRUCT* lockreq = (SMU_LOCK_REQUEST_STRUCT*)gBuffer_Settings;
  
  SMU_LOCK_RESPONSE_STRUCT *lockresponse;	
  lockresponse = (SMU_LOCK_RESPONSE_STRUCT *)CMemAllocator::MyMallocDataPtr(sizeof(SMU_LOCK_RESPONSE_STRUCT));
  int def=78;
  
  if (atoi((const char *)lockreq->passwd)==1111)
  {	lockresponse->result=SETTINGS_TRUE;
  
		if 	(lockreq->opcode==1)
    {	WritePrivateProfileString (t1,t2,_T("0"),CCoolModule::GetModulePath()+(LPCTSTR)INI_FILE_PATH);                      
    }
    if (lockreq->opcode==2)
    {	
      WritePrivateProfileString (t1,t2,_T("1"),CCoolModule::GetModulePath()+(LPCTSTR)INI_FILE_PATH);  
    }
  }
  else
    lockresponse->result=SETTINGS_FALSE;
  return lockresponse;
}

/*****************************************************************************
* FUNCTION
*	BootupFunctions
* DESCRIPTION
*	This API is used to get the status of SIM(Lock/Open)
* PARAMETERS
*	void 
* RETURNS
*	void*	-	smu_password_required structure holds the SIM status.
*****************************************************************************/
void *BootupFunctions()
{	
	smu_password_required* bootup_response;
	int def=78;
	
	bootup_response = (smu_password_required *)CMemAllocator::MyMallocDataPtr(sizeof(smu_password_required));
 	if (!ReadValueINI)
	{	
   		SIM_LOCK=GetPrivateProfileInt(t1,t2,def,(LPCTSTR)INI_FILE_PATH); 
		if (SIM_LOCK==1) bootup_response->type=2;
		if (SIM_LOCK==0) {	bootup_response->type=0;	
							BootupDone=1;}
		ReadValueINI++;
	}
  bootup_response->type=100;  // add by zhuoxz,2008-09-23
	return bootup_response;	 
}

/*****************************************************************************
* FUNCTION
*	ValidatePinBootup
* DESCRIPTION
*	
* PARAMETERS
*	void 
* RETURNS
*	void*	
*****************************************************************************/
void *ValidatePinBootup()
{
	smu_verify_pin_req_struct* bootuppinreq = (smu_verify_pin_req_struct*)gBuffer_Settings;
	if (atoi((const char *)bootuppinreq->pin)==1111)
		VALIDATE_PIN=1;
	else
		VALIDATE_PIN=0;
	
	return NULL;

}

/*****************************************************************************
* FUNCTION
*	CheckSimLock
* DESCRIPTION
*	Check the status of SIM(Lock/Open).
* PARAMETERS
*	void 
* RETURNS
*	void*	- smu_check_pin_status_rsp_struct holds status of SIM
*****************************************************************************/
void *CheckSimLock(void)
{	
	int def=78;
	smu_check_pin_status_rsp_struct *rsp_sim_status;
	rsp_sim_status = (smu_check_pin_status_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(smu_check_pin_status_rsp_struct));
	SIM_LOCK=GetPrivateProfileInt(t1,t2,def,(LPCTSTR)INI_FILE_PATH); 
	if (SIM_LOCK==1)
		rsp_sim_status->status=1;
	if (SIM_LOCK==0)
		rsp_sim_status->status=2;
	return rsp_sim_status;
}


/*****************************************************************************
* FUNCTION
*	CheckPinBS
* DESCRIPTION
*	Write smu_password_required structure to PRT Queue, Set the phone boot 
*	status to 1 on PIN verifications.
* PARAMETERS
*	void 
* RETURNS
*	void*	-	NULL
*****************************************************************************/
void *CheckPinBS(int nSimNum)
{	 
	if (BootupDone==0)
	{	
		MYQUEUE stMsgQueue;
		smu_verify_pin_req_struct* bootuppinreq = (smu_verify_pin_req_struct*)gBuffer_Settings;
		smu_password_required* bootup_response;
		bootup_response = (smu_password_required *)CMemAllocator::MyMallocDataPtr(sizeof(smu_password_required));
		if (atoi((const char *)bootuppinreq->pin)==1111) VALIDATE_PIN=1;
		else VALIDATE_PIN=0;
		if (VALIDATE_PIN==1) {	bootup_response->type=0;
								BootupDone=1;}
		if (VALIDATE_PIN==0) bootup_response->type=2;
		stMsgQueue.oslSrcId = nSimNum;
		stMsgQueue.oslDestId = MOD_PRT;
		stMsgQueue.oslMsgId = 1474;
		stMsgQueue.oslDataPtr = bootup_response;
		stMsgQueue.oslPeerBuffPtr = NULL;		
		myNSRThread->WriteToSocket(&stMsgQueue);
        }
	return NULL;
}
				/*****************************************************************************
* FUNCTION
*	SettingGPRSGetConnectType
* DESCRIPTION
*	Return the stored value of GPRS connect.
* PARAMETERS
*	void 
* RETURNS
*	void*	-	NULL
*****************************************************************************/
void * SettingGPRSGetConnectType(void)
{
	nw_gprs_connect_type_rsp_struct *GPRSResponse;	
	int type = 0;
	int def = 48;

	GPRSResponse = (nw_gprs_connect_type_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(nw_gprs_connect_type_rsp_struct));
  	
	type = GetPrivateProfileInt(t1,t4,def,(LPCTSTR)INI_FILE_PATH); 
	
	GPRSResponse->result = SETTINGS_TRUE;
	GPRSResponse->type = type;
	
	return (void *)GPRSResponse;
}


/*****************************************************************************
* FUNCTION
*	SettingGPRSSetConnectType
* DESCRIPTION
*	Stores the value of GPRS connect type
* PARAMETERS
*	void 
* RETURNS
*	void*	-	NULL
*****************************************************************************/
void * SettingGPRSSetConnectType(void)
{
	nw_gprs_connect_type_rsp_struct *GPRSResponse ;
		
	nw_gprs_connect_type_req_struct *GPRSReq = (nw_gprs_connect_type_req_struct*)gBuffer_Settings;
	
	GPRSResponse = (nw_gprs_connect_type_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(nw_gprs_connect_type_rsp_struct));

	if (GPRSReq->type == 0)
		WritePrivateProfileString (t1,t4,_T("0"),(LPCTSTR)INI_FILE_PATH);
	else
		WritePrivateProfileString (t1,t4,_T("1"),(LPCTSTR)INI_FILE_PATH);  
	
	return (void *)GPRSResponse;
}

/*****************************************************************************
* FUNCTION
*	SettingGetNetworkBand
* DESCRIPTION
*	Return the stored value of Band.
* PARAMETERS
*	void 
* RETURNS
*	void*	-	NULL
*****************************************************************************/
void * SettingGetNetworkBand(void)
{
	nw_gprs_connect_type_rsp_struct *NetworkBandResp;	
	int Band = 0;
	int def = 50;

	NetworkBandResp = (nw_gprs_connect_type_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(nw_gprs_connect_type_rsp_struct));
  	
	Band = GetPrivateProfileInt(t1,t5,def,(LPCTSTR)INI_FILE_PATH); 
	
	NetworkBandResp->result = SETTINGS_TRUE;
	NetworkBandResp->type = Band;
	
	return (void *)NetworkBandResp;
}


/*****************************************************************************
* FUNCTION
*	SettingSetNetworkBand
* DESCRIPTION
*	Stores the value of Network Band
* PARAMETERS
*	void 
* RETURNS
*	void*	-	NULL
*****************************************************************************/
void * SettingSetNetworkBand(void)
{
	nw_gprs_connect_type_rsp_struct *NetworkSetBandResponse ;
		
	nw_gprs_connect_type_req_struct *NetworkSetBandReq = (nw_gprs_connect_type_req_struct*)gBuffer_Settings;
	
	NetworkSetBandResponse = (nw_gprs_connect_type_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(nw_gprs_connect_type_rsp_struct));

	switch (NetworkSetBandReq->type) {
		case 2:		WritePrivateProfileString (t1,t5,_T("2"),(LPCTSTR)INI_FILE_PATH);break;
		case 8:		WritePrivateProfileString (t1,t5,_T("8"),(LPCTSTR)INI_FILE_PATH);break;
		case 10:	WritePrivateProfileString (t1,t5,_T("10"),(LPCTSTR)INI_FILE_PATH);break;
		case 16:	WritePrivateProfileString (t1,t5,_T("16"),(LPCTSTR)INI_FILE_PATH);break;
		case 128:	WritePrivateProfileString (t1,t5,_T("128"),(LPCTSTR)INI_FILE_PATH);break;;
		case 136:	WritePrivateProfileString (t1,t5,_T("136"),(LPCTSTR)INI_FILE_PATH);break;
		case 144:	WritePrivateProfileString (t1,t5,_T("144"),(LPCTSTR)INI_FILE_PATH);break;
		default : break;
	}

	NetworkSetBandResponse->result = SETTINGS_TRUE;
	NetworkSetBandResponse->type = 0;
	
	return (void *)NetworkSetBandResponse;
}

/*****************************************************************************
* FUNCTION
*	GetPasswordData
* DESCRIPTION
*	Plugin to set password data.
* PARAMETERS
*	void 
* RETURNS
*	void
*****************************************************************************/
void* GetPasswordData(void *dataBuff)
{
	smu_verify_pin_req_struct *phoneLocReq = (smu_verify_pin_req_struct *)dataBuff;
	smu_check_pin_status_rsp_struct *phoneLocRsp
			= ( smu_check_pin_status_rsp_struct *)CMemAllocator::MyMallocDataPtr(sizeof(smu_check_pin_status_rsp_struct ));

	ReadPasswordData_file();

	if(strlen((const char *)phonepswd) > 0) {
		if(strcmp((const char *)phoneLocReq->pin, (const char *)phonepswd) == 0) {
			strcpy((char *)phonepswd, (const char *)phoneLocReq->new_pin);
			WritePasswordData_file();
			phoneLocRsp->result = SETTINGS_TRUE;
		}
		else {
			phoneLocRsp->result = SETTINGS_FALSE;
		}
	}
	else {
		strcpy((char *)phonepswd, (const char *)phoneLocReq->new_pin);
		WritePasswordData_file();
		phoneLocRsp->result = SETTINGS_TRUE;
	}

	return (void *)phoneLocRsp;
}


void WritePasswordData_file()
{
	FILE *fp;
	fp = fopen( PASSWORD_FILE, "wb" );
	if(fp!=NULL)
	{
		fwrite(&phonepswd, sizeof(phonepswd), 1, fp );
		fclose(fp);
	}
}


void ReadPasswordData_file()
{
	FILE *fp;
	fp = fopen( PASSWORD_FILE, "rb" );
	if(fp!=NULL)
	{
		fread(&phonepswd, sizeof(phonepswd), 1, fp );
		fclose(fp);
	}
}
