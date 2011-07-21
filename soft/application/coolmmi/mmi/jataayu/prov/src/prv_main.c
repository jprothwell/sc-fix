#define MODULE_NAME "JPROV"
#define FILE_NAME "prv_main.c"

/***************************************************************************
 *
 * File Name : prv_mmi.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND 
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************/

/** 
 * @file		prv_mmi.c
 * @ingroup		provisioning
 * @brief	
 *
 **/



#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#endif

#include "mmi_trace.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "globaldefs.h"
#include "eventsgprot.h"
#include "wgui_status_icons.h"
#include "commonscreens.h"

#include "ddl.h"
#include "jcal.h"


#include "jdi_cutils_tcsstr.h"
#include "jdi_provisionapi.h"
#include "jdi_provisionstructures.h"
#include "jdi_hashalg.h"
#include "prv_defines.h"
#include "prv_main.h"

#include "prv_platform.h"
#include <jdi_wspdecoder.h>
#include <jdi_httpdecoder.h>
#include "prv_str_id.h"
#include "provisionerrors.h"

#include "custom_nvram_editor_data_item.h"
#include "custom_mmi_default_value.h"
#include "nvram_user_defs.h"
#include "jprofilehandlertypes.h"
#include "nvramtype.h"
#include "dataaccountdef.h"
#include "nvramprot.h"
#include "prv_mmi.h"
#include "prv_mmi.h"


#ifdef  __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "cfw.h"


//#define CS_OTA

/* ===========================================================================
 *  FORWARD DECLARATIONS
 * ===========================================================================
 */
ST_PROV_CONTEXT gstProvContext = {JC_NULL, } ;
//extern ST_COMM_PUSH_IND gstPushData;

VOID cfw_IMSItoASC(UINT8 *InPut,UINT8 *OutPut, UINT8 *OutLen);


/***************************************************************************
 * Global Declarations
 ***************************************************************************/
static const JC_INT8  *gpcBrwAppID				= (const JC_INT8*)"w2" ;
static const JC_INT8  *gpcMMSAppID				= (const JC_INT8*)"w4" ;
#if 1
static const JC_INT8  *gpcNullString			= (const JC_INT8*)"(null)" ;
static const JC_INT8  *gpcGPRSString			= (const JC_INT8*)"GSM-GPRS" ;
static const JC_INT8  *gpcCSDString				= (const JC_INT8*)"GSM-CSD" ;
static const JC_INT8  *gpcModemString			= (const JC_INT8*)"ANALOG_MODEM" ;
static const JC_INT8  *gpcX31String				= (const JC_INT8*)"X.31" ;
#endif

static const JC_INT8  *gpcCoWspString			= (const JC_INT8*)"CO-WSP" ;
static const JC_INT8  *gpcClWspString			= (const JC_INT8*)"CL-WSP" ;
static const JC_INT8  *gpcCoSecWspString		= (const JC_INT8*)"CO-SEC-WSP" ;
static const JC_INT8  *gpcClSecWspString		= (const JC_INT8*)"CL-SEC-WSP" ;

#if 0
static const JC_INT8  *gpcOtaAuthTypePAP		= (const JC_INT8*)"PAP" ;
static const JC_INT8  *gpcOtaAuthTypeCHAP		= (const JC_INT8*)"CHAP" ;
#endif



/* ===========================================================================
 *  PUBLIC FUNCTIONS
 * ===========================================================================
 */

void prv_AddToProvList(ST_COMM_PUSH_IND *pstProvPushData)
{
	ST_PROV_INFO* pstNewNode = JC_NULL;
	ST_PROV_INFO* pstTemp = JC_NULL ;

	mmi_trace(12,"JPRV_LOG:prv_AddToProvList called");

	pstNewNode = (ST_PROV_INFO*)jdd_MemAlloc (1, sizeof(ST_PROV_INFO)) ;
	
	if(!pstNewNode)
	  return;	
	
	pstNewNode->pstNext = JC_NULL;
	pstNewNode->bIsHTTPHeader = E_FALSE;
	pstNewNode->nSimID = jc_atoi( pstProvPushData->pcPPGHostAddress ); 
	
	pstNewNode->pucBuffer = pstProvPushData->pucBuffer;

	pstNewNode->uiBufferLen = pstProvPushData->uiLength;
	if(pstProvPushData->uiLength > 0)
	{
		pstNewNode->pucBuffer = jdd_MemAlloc(1,pstProvPushData->uiLength);
		jc_memcpy(pstNewNode->pucBuffer, pstProvPushData->pucBuffer, pstProvPushData->uiLength);
	}

	pstNewNode->uiHeaderLen = pstProvPushData->uiHeaderLength;
	if(pstProvPushData->uiHeaderLength > 0)
	{
		pstNewNode->pucHeader =	jdd_MemAlloc(1,pstProvPushData->uiHeaderLength);
		jc_memcpy(pstNewNode->pucHeader, pstProvPushData->pucHeader, pstProvPushData->uiHeaderLength);
	}
		
	if(gstProvContext.pstProvInfo == JC_NULL)
	{
		mmi_trace(12,"JPRV_LOG:added as root node");
		gstProvContext.pstProvInfo = pstNewNode;
	}
	else
	{
		pstTemp = gstProvContext.pstProvInfo;
		while(pstTemp->pstNext != JC_NULL)
			pstTemp = pstTemp->pstNext;
		mmi_trace(12,"JPRV_LOG:added to the link list");
		pstTemp->pstNext = pstNewNode;	
	}

}



void DeleteHeadProvList()
{
	ST_PROV_INFO  *pstTempNode ;  
	pstTempNode = gstProvContext.pstProvInfo;  
	mmi_trace(1, "JPRV_LOG:DeleteHeadProvList()");
	
	if(pstTempNode)
	{
		gstProvContext.pstProvInfo = pstTempNode -> pstNext ;  
		mmi_trace(1,"JPRV_LOG:head node %d",gstProvContext.pstProvInfo);
		jdd_MemFree(pstTempNode->pucHeader);
		jdd_MemFree(pstTempNode->pucBuffer);
		jdd_MemFree ( pstTempNode ) ;  
	}
}
void ConvertIMSIToSemiOctet (const JC_UINT8 *pucIMISI, JC_UINT8 *pucIMSISemiOctet, JC_UINT8 *pucLength)
{
	JC_UINT8	ucIMISIndex = 0, ucSOIndex = 0, ucLength = 0 ;
	
	ucLength = jc_strlen((const char *)pucIMISI) ;
	pucIMSISemiOctet[ucSOIndex] = (ucLength%2) ? 0x09 : 0x01 ;
	pucIMSISemiOctet[ucSOIndex++] |= (pucIMISI[ucIMISIndex++] - '0')<<4;
	while(ucIMISIndex < ucLength)
	{
		pucIMSISemiOctet[ucSOIndex] = (pucIMISI[ucIMISIndex++] - '0');
		if (ucIMISIndex < ucLength)
		{
			pucIMSISemiOctet[ucSOIndex++] |= (pucIMISI[ucIMISIndex++] - '0')<<4;
		}
		else
		{
			pucIMSISemiOctet[ucSOIndex++] |= 0xF0;
		}
	}
	*pucLength = ucSOIndex ;
	while(ucSOIndex < 8)
	{
		pucIMSISemiOctet[ucSOIndex++] = 0xFF ;
	}
}

/**
 * @ingroup app_OTA
 * @brief   Processes the bootstrap File
 */
void prv_ProcessProvInfo()
{
	JC_RETCODE				rCode = JC_OK;
	JC_HANDLE				vDecHandle = JC_NULL ;
	ST_MIME_HEADERS 		*pstReqHeader = JC_NULL ;
	U_PROV_CALLBACK 		uProvCallBack = {JC_NULL, } ;
	ST_PROV_INFO			*pstProvInfo = gstProvContext.pstProvInfo;
	mmi_trace(1, "JPRV_LOG:prv_ProcessDataFromFile invoked");
	//stProvInfo has provision info
	if (pstProvInfo->pucBuffer && pstProvInfo->pucHeader)
	{
		if (E_FALSE == pstProvInfo->bIsHTTPHeader)
		{
			rCode = jdi_WSPDecInitialize (pstProvInfo->pucHeader, pstProvInfo->uiHeaderLen, 
						E_FALSE, JC_NULL, &vDecHandle) ;
			mmi_trace(1, "JPRV_LOG:jdi_WSPDecInitialize returns %d", rCode);
			if (JC_OK == rCode)
			{
				rCode = jdi_WSPDecAllHeaders (vDecHandle, E_TRUE, &pstReqHeader) ;
				mmi_trace(1, "JPRV_LOG:jdi_WSPDecAllHeaders returns %d", rCode);
			}
		}
		else
		{
			rCode = jdi_HTTPDecInitialize (pstProvInfo->pucHeader, pstProvInfo->uiHeaderLen, 
						E_FALSE, JC_NULL, &vDecHandle) ;
			mmi_trace(1, "JPRV_LOG:jdi_HTTPDecInitialize returns %d", rCode);
			if (JC_OK == rCode)
			{
				rCode = jdi_HTTPDecAllHeaders (vDecHandle, E_TRUE, &pstReqHeader) ;
				mmi_trace(1, "JPRV_LOG:jdi_HTTPDecAllHeaders returns %d", rCode);
			}
		}
		if (JC_OK == rCode && JC_NULL == gstProvContext.vProvHandle)
		{
			rCode = jdi_ProvisionInitialize (&(gstProvContext.vProvHandle)) ;
			mmi_trace(1, "JPRV_LOG:jdi_ProvisionInitialize returns %d", rCode);
		}
		if (JC_OK == rCode)
		{
			rCode  = jdi_ProvisionProcess (gstProvContext.vProvHandle,
				pstProvInfo->pucBuffer, pstProvInfo->uiBufferLen, pstReqHeader, &(gstProvContext.eProvType), 
				&(gstProvContext.pcProvURL), &(gstProvContext.eSecType)) ;
			mmi_trace(1, "JPRV_LOG:jdi_ProvisionProcess returns %d sectype[%d] provtype[%d]", rCode, gstProvContext.eSecType,gstProvContext.eProvType);
			if (JC_OK == rCode && (E_USERPIN == gstProvContext.eSecType || E_NETWPIN == gstProvContext.eSecType))
			{
				uProvCallBack.pfHmacSha = (PROV_CALLBACK_HMAC)jdi_HMACSHA1DigestHex ;
				rCode = jdi_ProvisionRegisterCallback (gstProvContext.vProvHandle,
						E_AUTHENTICATE_HMAC_SHA_CALLBACK, &uProvCallBack) ;
				mmi_trace(1, "JPRV_LOG:jdi_ProvisionRegisterCallback returns %d", rCode);
			}
			
		}
		if (JC_NULL != vDecHandle)
		{
			if (E_FALSE == pstProvInfo->bIsHTTPHeader)
			{
				jdi_WSPDecDeInitialize (vDecHandle) ;
			}
			else
			{
				jdi_HTTPDecDeInitialize (vDecHandle) ;
			}
		}
	}
	else
	{
		rCode = JC_ERR_NULL_POINTER ;
	}

	if (JC_OK == rCode)
	{
		prv_DeleteProfList();
		
		if (E_NETWPIN == gstProvContext.eSecType)
		{
			UINT8  pIMSI [32]={0}, pIMSIasc[32] ={0,};
			UINT32 ret;
			UINT8 OutLen =  0;
			mmi_trace(1, "JPRV_LOG:network pin enabled ");
			
			ret = CFW_CfgGetIMSI(pIMSI, pstProvInfo->nSimID);
				
			cfw_IMSItoASC(pIMSI, pIMSIasc, &OutLen);
			
			mmi_trace(1,"JPRV_LOG: IMSI for sim0 %s %d", pIMSIasc, OutLen);

			jc_memset(pIMSI, 32, 0);
			OutLen = 0;

			ConvertIMSIToSemiOctet (pIMSIasc, pIMSI, &OutLen);

			if(OutLen > 0)
			{
				//jdi_CUtilsCharToTchar(JC_NULL, pIMSIasc, &gstProvContext.pmPinValue);
				//prv_CheckAuthentication();
				rCode = prv_CheckNETPINAuthentication(pIMSI , OutLen);
			}
			else // considering as "no security defined"
			{
				prv_FormProfList();
			}
		}
		else if (E_USERPIN == gstProvContext.eSecType)
		{
			mmi_trace(1, "JPRV_LOG:enter user pin screen ");
			jdd_AssertOnFail((JC_NULL == gstProvContext.pmPinValue), "JPRV_LOG:inavlid pin") ;
			gstProvContext.pmPinValue = (JC_CHAR *)jdd_MemAlloc (
				sizeof (JC_CHAR), WAP_MAX_PROFILE_PASSWD_LEN) ;
			//mmi_trace(1,"JPRV_LOG:gstProvContext.pmPinValue %d",gstProvContext.pmPinValue );
			gstProvContext.ucNumOfAttempts = 0;
			prv_MMIDisplayEnterPINScreen();
		}
		else // no security defined
		{
			prv_FormProfList();
		}
	}
	if (JC_OK != rCode)
	{
		prv_DeInitContext () ;
		prv_MMIDisplayProvFailScreen();
	}
	
}



JC_RETCODE prv_CheckNETPINAuthentication(UINT8 * pIMSIasc , UINT8 OutLen)
{
	JC_RETCODE rCode = JC_ERR_GENERAL_FAILURE;

	rCode = jdi_ProvisionAuthenticate (gstProvContext.vProvHandle, 
			 pIMSIasc, OutLen) ;
	
	if( rCode == JC_OK)
	{
		mmi_trace(1,"JPRV_LOG:authentication success");
		prv_FormProfList();
	}
	return rCode;
}


void prv_DeleteProfList()
{
	ST_PROV_INFO_LIST	*pstProfList = JC_NULL ;
	while (JC_NULL != gstProvContext.pstProfList)
	{
		pstProfList = gstProvContext.pstProfList ;
		gstProvContext.pstProfList = gstProvContext.pstProfList->pstNext ;
		jdd_MemFree (pstProfList) ;
	}
}

void prv_FormProfList()
{
	JC_RETCODE rCode ;
	rCode = prv_HandleContinuousAndBootstrapProvisioning();
	mmi_trace(1,"JPRV_LOG:prv_HandleContinuousAndBootstrapProvisioning returns %d", rCode);
	
	if(JC_OK != rCode)
	{
		prv_DeInitContext () ;
		prv_MMIDisplayProvFailScreen();
	}
	else
	{
		if(E_BOOTSTRAP_FLAG== gstProvContext.eProvType)
		{
			prv_FormCSProfiles();
			gstProvContext.pstCurrCsProfInfo = gstProvContext.pstCsProfInfo;
			//prv_GetNextNewCSProfile();
			prv_MMIDisplayNewSettingsSummaryScreen();
		}
		else
		{
			prv_MMIDisplayContProvProfDetails();
		}
	
	}
}

void prv_FormContinousProfDeatilsBuff()
{
	JC_INT8 pcTempStr[256] ={0,};
	
	mmi_trace(1, "prv_FormContinousProfDeatilsBuff Invoked");
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));

	//jdi_CUtilsTcsStrCat("\nDownload settings from", gstProvContext.szScreeBuff) ;
	jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_DOWNLOAD_SETTINGS)) ;
	if(gstProvContext.pstContProfList->stProfile.profile_name[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nProfile Name:\n", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_PROFILE_NAME)) ;	
		jdi_CUtilsTcsStrCat(":\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff,gstProvContext.pstContProfList->stProfile.profile_name);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}

	if(gstProvContext.pstContProfList->stProfile.apn[0]!='\0')
	{
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_APN)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat(gstProvContext.pstContProfList->stProfile.apn,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}

	if(gstProvContext.pstContProfList->stProfile.username[0]!='\0')
	{
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff,(JC_CHAR*)GetString(STR_ID_PROV_USERNAME)) ;			
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;	
		//jdi_CUtilsTcsStrCat("\nUserName:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat(gstProvContext.pstContProfList->stProfile.username,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	
	if(gstProvContext.pstContProfList->stProfile.password[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nPasswd:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_PASSWORD)) ;			
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;	
	
		jdi_CUtilsTcsStrCat(gstProvContext.pstContProfList->stProfile.password,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}

	if(gstProvContext.pstContProfList->stProfile.proxy_ip[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nProxyIP:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_PROXYIP)) ;			
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;	
		
		jdi_CUtilsTcsStrCat(gstProvContext.pstContProfList->stProfile.proxy_ip,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	if(gstProvContext.pstContProfList->stProfile.port)
	{
		jc_memset(pcTempStr,0,256);
		jc_itoa(gstProvContext.pstContProfList->stProfile.port,pcTempStr,10 );
		//jdi_CUtilsTcsStrCat("\nPort:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_PORT)) ;			
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		
		jdi_CUtilsTcsStrCat(pcTempStr,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	
}


void prv_FormNewSettingsSummaryList()
{
	ST_CS_PROFILE_NODE * pstProfileNode = gstProvContext.pstCsProfInfo;
	JC_INT8 ucDataConnProfileCnt = 0, ucWAPProxyProfileCnt = 0,ucMMSProxyProfileCnt = 0  ;
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));

	//jc_wstrcat(gstProvContext.szScreeBuff,  (JC_CHAR*)L"Settings For \n");
	jc_wstrcat(gstProvContext.szScreeBuff,  (JC_CHAR*)GetString(STR_ID_PROV_SETTINGSFOR));
	jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	

	while(pstProfileNode)
	{
		if(pstProfileNode->eCSProfileType == E_CS_DATACONN_PROFILE)
		{
			ucDataConnProfileCnt ++;
		}
		else if(pstProfileNode->eCSProfileType == E_CS_PROXY_PROFILE)
		{
			if(((ST_CS_PROXY_PROFILE*)(pstProfileNode->pProfile))->profile_type == E_PROF_BROWSER )
				ucWAPProxyProfileCnt ++;
			else if(((ST_CS_PROXY_PROFILE*)(pstProfileNode->pProfile))->profile_type == E_PROF_MMS)
				ucMMSProxyProfileCnt ++;
		}
		pstProfileNode = pstProfileNode->pNext;
	}

	if(ucDataConnProfileCnt > 0)
	{
		jc_wstrcat(gstProvContext.szScreeBuff,  (JC_CHAR*)GetString(STR_ID_PROV_DATA_ACCOUNT));
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;
	}
	if(ucWAPProxyProfileCnt > 0)
	{
		jc_wstrcat(gstProvContext.szScreeBuff,  (JC_CHAR*)GetString(STR_ID_PROV_BROW_PROFILE));
    	jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;
	}
	if(ucMMSProxyProfileCnt > 0)
	{
		jc_wstrcat(gstProvContext.szScreeBuff,  (JC_CHAR*)GetString(STR_ID_PROV_MMS_PROFILE));
    	jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;
	}

}


void LogProfileList()
{
	ST_PROV_INFO_LIST	*pstTempProfList = gstProvContext.pstProfList;
	while(pstTempProfList)
	{	
		JC_INT8	pctemp [255] = {0, } ;
		
		mmi_trace(1,"JPRV_LOG:**************profile start***************");
		mmi_trace(1,"JPRV_LOG:eProfType %d",pstTempProfList->eProfType);
		jdi_CUtilsStrTcsNCpy (pstTempProfList->stProfile.profile_name, 50, pctemp) ;
		mmi_trace(1,"JPRV_LOG:profile_name %s",pctemp);
		mmi_trace(1,"JPRV_LOG:homepage %s",pstTempProfList->stProfile.homepage);
		mmi_trace(1,"JPRV_LOG:apn %s",pstTempProfList->stProfile.apn);
		mmi_trace(1,"JPRV_LOG:username %s",pstTempProfList->stProfile.username);
		mmi_trace(1,"JPRV_LOG:password %s",pstTempProfList->stProfile.password);
		mmi_trace(1,"JPRV_LOG:proxy_ip %s",pstTempProfList->stProfile.proxy_ip);
		mmi_trace(1,"JPRV_LOG:dialup_number %s",pstTempProfList->stProfile.dialup_number);
		mmi_trace(1,"JPRV_LOG:dns1 %s",pstTempProfList->stProfile.dns1);
		mmi_trace(1,"JPRV_LOG:dns2 %s",pstTempProfList->stProfile.dns2);
		mmi_trace(1,"JPRV_LOG:nap_id %s",pstTempProfList->stProfile.nap_id);
		mmi_trace(1,"JPRV_LOG:proxy_id %s",pstTempProfList->stProfile.proxy_id);
		mmi_trace(1,"JPRV_LOG:baudrate %d",pstTempProfList->stProfile.baudrate);
		mmi_trace(1,"JPRV_LOG:timeout %d",pstTempProfList->stProfile.timeout);
		mmi_trace(1,"JPRV_LOG:port %d",pstTempProfList->stProfile.port);
		mmi_trace(1,"JPRV_LOG:bear_type %d",pstTempProfList->stProfile.bear_type);
		mmi_trace(1,"JPRV_LOG:conn_mode %d",pstTempProfList->stProfile.conn_mode);
		mmi_trace(1,"JPRV_LOG:csd_call_type %d",pstTempProfList->stProfile.csd_call_type);
		mmi_trace(1,"JPRV_LOG:**************profile end***************");		
		pstTempProfList = pstTempProfList->pstNext;
		
	}
}
JC_RETCODE prv_HandleContinuousAndBootstrapProvisioning (void)
{
	JC_RETCODE				rCode = JC_OK ;
	ST_PROFILE_DETAIL_INFO  *pstProfileDetail = JC_NULL ;
	ST_PROVISION_INFO		*pstProvisionInfo = JC_NULL ;

	mmi_trace(1,"JPRV_LOG: prv_HandleContinuousAndBootstrapProvisioning");
	if (E_CONTINUOUS_PROVISION_FLAG == gstProvContext.eProvType && 
		JC_NULL != gstProvContext.pcProvURL)
	{
		mmi_trace(1,"JPRV_LOG: E_CONTINUOUS_PROVISION_FLAG");
		pstProvisionInfo = (ST_PROVISION_INFO *)gstProvContext.vProvHandle;	
		pstProfileDetail = pstProvisionInfo->pstProvDetails ;
		rCode = prv_FillPxLogicalInfo (pstProfileDetail->pstPxLogical, 
			&(gstProvContext.pstContProfList)) ;

		mmi_trace(1,"prv_FillPxLogicalInfo returns %d", rCode);
	
	}
	else
	{
		mmi_trace(1,"JPRV_LOG: E_BOOTSTRAP_FLAG");
		rCode = prv_FillApplicationInfo (&gstProvContext.pstProfList) ;
		mmi_trace(1,"prv_FillApplicationInfo returns %d", rCode);
		prv_updateMissingProfileInfo(&(gstProvContext.pstProfList->stProfile));
	}
	LogProfileList();
	mmi_trace(1,"JPRV_LOG: prv_HandleContinuousAndBootstrapProvisioning returns %d", rCode);
	return rCode ;
}


void prv_ResetContinousProvdata()
{
	ST_PROV_INFO		*pstProvInfo = JC_NULL ;
	pstProvInfo = gstProvContext.pstProvInfo ;
	
	if (JC_NULL != pstProvInfo)
	{
		if (0 != pstProvInfo->uiBufferLen)
		{
			jdd_MemFree (pstProvInfo->pucBuffer) ;
			pstProvInfo->pucBuffer = JC_NULL;
			pstProvInfo->uiBufferLen = 0;
		}
		if (0 != pstProvInfo->uiHeaderLen)
		{
			jdd_MemFree (pstProvInfo->pucHeader) ;
			pstProvInfo->pucHeader = JC_NULL;
			pstProvInfo->uiHeaderLen = 0;
		}
	}
	/*if (JC_NULL != gstProvContext.vCommHandle)
	{
		prv_DeInitCommunicator () ;
	}*/
	gstProvContext.eSecType = E_SEC_NOT_DEFINED ;

}

JC_RETCODE prv_DeInitContext ()
{
	ST_PROV_INFO_LIST	*pstProfListHead = JC_NULL ;
//	ST_PROV_INFO		*pstProvInfo = JC_NULL ;

	mmi_trace(1,"JPRV_LOG: prv_DeInitContext");

	DeleteHeadProvList();
	prv_DeleteAllCSProfiles();
	prv_DeleteProfList();
	while (JC_NULL != gstProvContext.pstContProfList)
	{
		pstProfListHead = gstProvContext.pstContProfList ; 
		
		gstProvContext.pstContProfList = (gstProvContext.pstContProfList)->pstNext ;
		jdd_MemFree (pstProfListHead) ;
	}
	/*
	pstProvInfo = gstProvContext.pstProveInfo ;

	if (JC_NULL != pstProvInfo)
	{
		if (0 != pstProvInfo->uiBufferLen)
		{
			jdd_MemFree (pstProvInfo->pucBuffer) ;
		}
		if (0 != pstProvInfo->uiHeaderLen)
		{
			jdd_MemFree (pstProvInfo->pucHeader) ;
		}
	}*/

 /*
	while (JC_NULL != gstProvContext.pstProfList)
	{
		pstProfListHead = gstProvContext.pstProfList ; 
		gstProvContext.pstProfList = (gstProvContext.pstProfList)->pstNext ;
		jdd_MemFree (pstProfListHead) ;
	}*/
	if (JC_NULL != gstProvContext.vProvHandle)
	{
		jdi_ProvisionDeinitialize (gstProvContext.vProvHandle) ;
	}
	if (JC_NULL != gstProvContext.vCommHandle)
	{
		prv_DeInitCommunicator () ;
	}
	
	

	/*
	if (JC_NULL != pstProvInfo)
	{
		gstProvContext.pstProveInfo = pstProvInfo->pstNext ;
		jdd_MemFree (pstProvInfo) ;
		pstProvInfo = JC_NULL ;
	}*/

	gstProvContext.eSecType = E_SEC_NOT_DEFINED ;

	jdd_MemFree (gstProvContext.pmPinValue) ;
	gstProvContext.pmPinValue = JC_NULL ;
	gstProvContext.ucNumOfAttempts = 0 ;
	jc_memset((void  *)&gstProvContext, '\0', sizeof (ST_PROV_CONTEXT));  
	return JC_OK ;
}

JC_RETCODE prv_DeInitCommunicator (void)
{
	mmi_trace(1,"JPRV_LOG: prv_DeInitCommunicator");
	if (JC_NULL != gstProvContext.vCommHandle)
	{
		jdi_CommunicationDeinitialize (gstProvContext.vCommHandle) ;
		gstProvContext.vCommHandle = JC_NULL ;
	}
	return JC_OK ;
}
/*****************************************************************************
* 
* @fn 			: prv_FillPxLogicalInfo
*
* @brief		: Fills provisioning (PXLOGICAL) information into profile structure
*
* @param[in]	: pPxLogic	Provisioning (PXLOGICAL) information
* @param[out]	: ppProfList	Profile list to be filled with provisioning information
*
* @retval		: JC_OK	On Successful and Others on failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_FillPxLogicalInfo (ST_PROV_PXLOGICAL *pstPxLogic, 
										 ST_PROV_INFO_LIST **ppstProfList) 
{

	JC_RETCODE				rCode = JC_OK ;	
	ST_PROV_INFO_LIST		*pstProfList = JC_NULL ;
	ST_PROV_PXPHYSICAL		*pstPxPhys = JC_NULL ;
	ST_PROV_PORT			*pstTempPort = JC_NULL ;
	ST_PROV_INFO_LIST		*pstTempProfList = JC_NULL ;
	mmi_trace(1, "prv_FillPxLogicalInfo invoked");

	if (JC_NULL != pstPxLogic && JC_NULL != ppstProfList)
	{
		pstPxPhys = pstPxLogic->pstPxlogicalPxphysical ;
		pstTempPort = pstPxLogic->pstPxlogicalPort ;
		while (JC_NULL != pstPxPhys)
		{
			rCode = prv_FillPxPhysicalInfo (pstPxPhys, &pstProfList) ;
			if (JC_OK == rCode)
			{
				if (JC_NULL == *ppstProfList)
				{
					*ppstProfList = pstProfList ;
				}
				else if (JC_NULL != pstTempProfList)
				{
					pstTempProfList->pstNext = pstProfList ;
				}

				while (JC_NULL != pstProfList)
				{
					if (JC_NULL != pstPxLogic->pcStartPage)
					{
						jc_strncpy (pstProfList->stProfile.homepage, 
							pstPxLogic->pcStartPage, E_DC_MAX_URL_LEN) ;
					}
					if (JC_NULL != pstPxLogic->pcProxyId && 0 < jc_strlen(pstPxLogic->pcProxyId))
					{
						jdi_CUtilsCharToTcharMem (pstPxLogic->pcProxyId, E_DC_MAX_PROFILE_NAME, 
										(JC_CHAR  *)pstProfList->stProfile.profile_name);
					}
					if (JC_NULL != pstTempPort)
					{
						prv_FillPortServiceInfo (pstTempPort, pstProfList) ;
					}
					if (JC_NULL == pstProfList->pstNext)
					{
						pstTempProfList = pstProfList ;
					}
					jc_strncpy(pstProfList->stProfile.proxy_id,
						pstPxLogic->pcProxyId ,E_DC_MAX_PROXYID_LEN);
					pstProfList = pstProfList->pstNext ;
				}
				pstProfList = JC_NULL ;
			}
			else
			{
				break ;
			}
			pstPxPhys = pstPxPhys->pstNextPxphysical ;
		}
	}
	return rCode ;

}

/*****************************************************************************
* 
* @fn 			: prv_FillPxPhysicalInfo
*
* @brief		: Fills provisioning (PXPHYSICAL) information into profile structure
*
* @param[in]	: pPxPhys	Provisioning (PXPHYSICAL) information
* @param[out]	: ppProfList	Profile list to be filled with provisioning information
*
* @retval		: JC_OK	On Successful and Others on failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_FillPxPhysicalInfo (ST_PROV_PXPHYSICAL *pstPxPhys, 
										  ST_PROV_INFO_LIST **ppstProfList)
{
	JC_RETCODE				rCode = JC_OK ;	
	ST_PROV_INFO_LIST		*pstProfList = JC_NULL ;
	ST_PROV_NAPDEF			*pstNapDef = JC_NULL ;
	ST_PROV_VALUES			*pstToNapid = JC_NULL ;
	ST_PROV_PORT			*pstTempPort = JC_NULL ;
	ST_PROV_INFO_LIST		*pstTempProfList = JC_NULL ;
	mmi_trace(1, "JPRV_LOG:prv_FillPxPhysicalInfo invoked");

	if (JC_NULL != pstPxPhys && JC_NULL != ppstProfList)
	{
		pstToNapid = pstPxPhys->pstToNapid ;
		pstTempPort = pstPxPhys->pstPxphysicalPort ;
		while (JC_NULL != pstToNapid)
		{
			prv_GetNapDef (pstToNapid, &pstNapDef) ;
			if (JC_NULL != pstNapDef)
			{
				rCode = prv_FillNapDefInfo (pstNapDef, &pstProfList) ;
				if (JC_OK == rCode)
				{
					if (JC_NULL != pstPxPhys->pcPxaddr)
					{
						jc_strncpy (pstProfList->stProfile.proxy_ip, 
							pstPxPhys->pcPxaddr, E_DC_MAX_IP_LEN) ;
					}
					if (JC_NULL != pstTempPort)
					{
						prv_FillPortServiceInfo (pstTempPort, pstProfList) ;
					}
					if (JC_NULL == *ppstProfList)
					{
						*ppstProfList = pstProfList ;
						pstTempProfList = pstProfList ;
					}
					else if (JC_NULL != pstTempProfList)
					{
						pstTempProfList->pstNext = pstProfList ;
						pstTempProfList = pstTempProfList->pstNext ; 
					}
				}
			}
			pstToNapid = pstToNapid->pstNext ;
		}
	}
	return rCode ;
}


/*****************************************************************************
* 
* @fn 			: prv_FillApplicationInfo
*
* @brief		: Fills all provisioning information into profile structure
*
* @param[in]	: No parameter.
* @param[out]	: ppProfList	Profile list to be filled with provisioning information
*
* @retval		: JC_OK	On Successful and Others on failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_FillApplicationInfo (ST_PROV_INFO_LIST **ppstProfList) 
{
	JC_RETCODE				rCode = JC_OK ;
	ST_PROV_INFO_LIST		*pstProfList = JC_NULL ;
	ST_PROV_PXLOGICAL		*pstPxLogic = JC_NULL ;
	ST_PROV_APPLICATION		*pstAppList = JC_NULL ;
	JC_INT8 				*pcURL = JC_NULL ;
	E_PROV_PROFILE_TYPE		eProfType ;
	ST_PROVISION_INFO		*pstProvInfo = JC_NULL;
	ST_PROFILE_DETAIL_INFO  *pstProfileDetail = JC_NULL ;
	ST_PROV_INFO_LIST		*pstTempProfList = gstProvContext.pstProfList ;
	ST_PROV_NAPDEF			*pstNapDef = JC_NULL ;
	JC_INT8	 				*pcName = JC_NULL ;

	mmi_trace(1,"JPRV_LOG: prv_FillApplicationInfo invoked");

	pstProvInfo = (ST_PROVISION_INFO *)gstProvContext.vProvHandle ;	
	pstProfileDetail = pstProvInfo->pstProvDetails ;

	pstAppList = pstProfileDetail->pstApplication ;

	if (JC_NULL == pstAppList)
	{
		mmi_trace(1,"JPRV_LOG: prv_FillApplicationInfo JC_NULL == pstAppList");
		rCode = prv_FillPxLogicalInfo (pstProfileDetail->pstPxLogical, &pstProfList) ;
		
		if (JC_OK == rCode)
		{
			*ppstProfList = pstProfList ;
		}
	}
	else
	{
		mmi_trace(1,"JPRV_LOG: prv_FillApplicationInfo JC_NULL != pstAppList");
		while (JC_NULL != pstAppList)
		{
			if (JC_NULL != pstAppList->pcAppid)
			{
				if (JC_NULL != jc_strstr (pstAppList->pcAppid, gpcBrwAppID))
				{	
					eProfType = E_PROF_BROWSER ;
				}
				else if (JC_NULL != jc_strstr (pstAppList->pcAppid, gpcMMSAppID))
				{
					eProfType = E_PROF_MMS ;
				}
				else
				{
					pstAppList = pstAppList->pstNextApplication ;
					continue ;
				}
			}
			else
			{
				eProfType = E_PROF_BROWSER ;
			}


			if (E_PROF_BROWSER == eProfType)
			{
				if (JC_NULL != pstAppList->pstApplicationResource && 
					JC_NULL != pstAppList->pstApplicationResource->pcUri)
				{
					pcURL = pstAppList->pstApplicationResource->pcUri ;
				}
			}
			else
			{
				if (JC_NULL != pstAppList->pstAddr &&
					JC_NULL != pstAppList->pstAddr->pcValue)
				{
					pcURL = pstAppList->pstAddr->pcValue ;
				}
				else if (JC_NULL != pstAppList->pstApplicationAppaddr &&
					JC_NULL != pstAppList->pstApplicationAppaddr->pcAddr)
				{
					pcURL = pstAppList->pstApplicationAppaddr->pcAddr ;
				}
			}

			if (JC_NULL != pstAppList->pstToProxy && JC_NULL != pstAppList->pstToProxy->pcValue)
			{
				prv_GetPxLogic (pstAppList->pstToProxy, &pstPxLogic) ;
				if (JC_NULL != pstPxLogic)
				{
					rCode = prv_FillPxLogicalInfo (pstPxLogic, &pstProfList) ;
					pcName = pstAppList->pcName ;
				}
				else
				{
					rCode = prv_FillPxLogicalInfo (pstProfileDetail->pstPxLogical, &pstProfList) ;
				}
			}
			else if (JC_NULL != pstAppList->pstToNapid && JC_NULL != pstAppList->pstToNapid->pcValue)
			{
				prv_GetNapDef (pstAppList->pstToNapid, &pstNapDef) ;
				if (JC_NULL != pstNapDef)
				{
					rCode = prv_FillNapDefInfo (pstNapDef, &pstProfList) ;
				}
			}
			if (JC_OK == rCode)
			{
				if (JC_NULL != pstProfList)
				{
					pstProfList->eProfType = eProfType ;
					/*if (E_PROF_MMS == eProfType)
					{
						pstProfList->stProfile.profile_type = E_DC_TYPE_MMS ;
					}
					else
					{
						pstProfList->stProfile.profile_type = E_DC_TYPE_WEB ;
					}*/
					
					if (JC_NULL == *ppstProfList)
					{
						*ppstProfList = pstProfList ;
					}
					else
					{
						pstTempProfList->pstNext = pstProfList ;
					}

					while (JC_NULL != pstProfList)
					{
						if (JC_NULL != pcURL)
						{
							jc_strncpy(pstProfList->stProfile.homepage, pcURL, E_DC_MAX_URL_LEN) ;
						}
						if (JC_NULL != pcName)
						{
							jdi_CUtilsCharToTcharMem (pcName, E_DC_MAX_PROFILE_NAME, 
										(JC_CHAR  *)pstProfList->stProfile.profile_name);
						}
						if (JC_NULL == pstProfList->pstNext)
						{
							pstTempProfList = pstProfList ;
						}
						pstProfList = pstProfList->pstNext ;
					}
					pstProfList = JC_NULL ;
				}
				else
				{
					rCode = JC_ERR_INVALID_PARAMETER ;
				}
			}

			pstAppList = pstAppList->pstNextApplication ;
		}
	}
	return rCode ;
}

/*****************************************************************************
* 
* @fn 			: prv_GetPxLogic
*
* @brief		: Gives provisioning information (PXLOGICAL)
*
* @param[in]	: pstToProxy	Provisioning information
* @param[out]	: ppPxLogic	Provisioning information (PXLOGICAL)
*
* @retval		: void
*
******************************************************************************/
void prv_GetPxLogic (ST_PROV_VALUES *pstToProxy, ST_PROV_PXLOGICAL **ppstPxLogic)
{
	ST_PROVISION_INFO		*pstProvInfo = JC_NULL;
	ST_PROFILE_DETAIL_INFO  *pstProfileDetail = JC_NULL ;
	ST_PROV_PXLOGICAL		*pstPxLogic = JC_NULL ;

	mmi_trace(1,"JPRV_LOG: prv_GetPxLogic invoked");
	
	pstProvInfo = (ST_PROVISION_INFO *)gstProvContext.vProvHandle ;	
	pstProfileDetail = pstProvInfo->pstProvDetails ;
	pstPxLogic = pstProfileDetail->pstPxLogical ;

	while (JC_NULL != pstPxLogic)
	{
		if (0 == jc_strcasecmp(pstToProxy->pcValue, pstPxLogic->pcProxyId))
		{
			*ppstPxLogic = pstPxLogic ;
			break ;
		}
		pstPxLogic = pstPxLogic->pstNextPxlogical ;
	}
}

/*****************************************************************************
* 
* @fn 			: prv_GetNapDef
*
* @brief		: Gives provisioning information (NAPDEF)
*
* @param[in]	: pstToNapid	Provisioning information
* @param[out]	: ppstNapDef	Provisioning information (NAPDEF)
*
* @retval		: void
*
******************************************************************************/
void prv_GetNapDef (ST_PROV_VALUES *pstToNapid, ST_PROV_NAPDEF **ppstNapDef)
{
	ST_PROVISION_INFO		*pstProvInfo = JC_NULL;
	ST_PROFILE_DETAIL_INFO  *pstProfileDetail = JC_NULL ;
	ST_PROV_NAPDEF			*pstNapDef = JC_NULL ;
	mmi_trace(1,"JPRV_LOG: prv_GetNapDef invoked");
	
	pstProvInfo = (ST_PROVISION_INFO *)gstProvContext.vProvHandle ;	
	pstProfileDetail = pstProvInfo->pstProvDetails ;
	pstNapDef = pstProfileDetail->pstNapdef ;

	while (JC_NULL != pstNapDef)
	{
		if (0 == jc_strcasecmp(pstToNapid->pcValue, pstNapDef->pcNapid))
		{
			*ppstNapDef = pstNapDef ;
			break ;
		}
		pstNapDef = pstNapDef->pstNextNapdef ;
	}
}


/*****************************************************************************
* 
* @fn 			: prv_FillNapDefInfo
*
* @brief		: Fills provisioning (NAPDEF) information into profile structure
*
* @param[in]	: pNapDef	Provisioning (NAPDEF) information
* @param[out]	: ppProfList	Profile list to be filled with provisioning information
*
* @retval		: JC_OK	On Successful and Others on failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_FillNapDefInfo (ST_PROV_NAPDEF *pstNapDef, ST_PROV_INFO_LIST **ppstProfList)
{
	mmi_trace(1,"JPRV_LOG: prv_FillNapDefInfo invoked");

	JC_RETCODE				rCode = JC_OK ;
	ST_PROV_INFO_LIST		*pstProfile = JC_NULL ;
	ST_PROV_VALUES			*pstDNS = JC_NULL ;

	if (JC_NULL == ppstProfList || JC_NULL == pstNapDef)
	{
		return JC_ERR_INVALID_PARAMETER ;
	}

	pstProfile = (ST_PROV_INFO_LIST *) jdd_MemAlloc (1, sizeof (ST_PROV_INFO_LIST)) ;
	if (JC_NULL != pstProfile)
	{
		pstProfile->stProfile.conn_mode = E_DC_CONN_MODE_MAX ; 
		pstProfile->stProfile.bear_type = E_DC_BEARER_MAX ;
		pstProfile->stProfile.csd_call_type = E_DC_CSD_CALL_MAX ;
		#if 0
		pstProfile->stProfile.gprs_auth_type = GPRS_AUTH_NONE ;
		#endif
		jdi_CUtilsCharToTcharMem (pstNapDef->pcNapid, E_DC_MAX_PROFILE_NAME, 
							(JC_CHAR  *)pstProfile->stProfile.profile_name);
		jc_strncpy(pstProfile->stProfile.nap_id,pstNapDef->pcNapid ,E_DC_MAX_NAPID_LEN);
		if (JC_NULL != pstNapDef->pstNapdefNapAuthInfo)
		{
			if (JC_NULL != pstNapDef->pstNapdefNapAuthInfo->pcAuthName)
			{
				if (0 != jc_strcasecmp (pstNapDef->pstNapdefNapAuthInfo->pcAuthName, 
					gpcNullString))
				{
					jc_strncpy (pstProfile->stProfile.username, 
						pstNapDef->pstNapdefNapAuthInfo->pcAuthName,
						MAX_DATA_CONN_USERNAME_SIZE) ;
				}
			}
			if (JC_NULL != pstNapDef->pstNapdefNapAuthInfo->pcAuthSecret)
			{
				if (0 != jc_strcasecmp (pstNapDef->pstNapdefNapAuthInfo->pcAuthSecret, 
					gpcNullString))
				{
					jc_strncpy (pstProfile->stProfile.password, 
						pstNapDef->pstNapdefNapAuthInfo->pcAuthSecret, 
						MAX_DATA_CONN_PASSWORD_SIZE) ;
				}
			}
			if (JC_NULL != pstNapDef->pstNapdefNapAuthInfo->pcAuthType)
			{
			#if 0
				if (0 == jc_strcasecmp (pstNapDef->pstNapdefNapAuthInfo->pcAuthType, 
					gpcOtaAuthTypePAP))
				{
					pstProfile->stProfile.gprs_auth_type = GPRS_AUTH_PAP ;
				}
				else if (0 == jc_strcasecmp (pstNapDef->pstNapdefNapAuthInfo->pcAuthType, 
					gpcOtaAuthTypeCHAP))
				{
					pstProfile->stProfile.gprs_auth_type = GPRS_AUTH_CHAP ;
				}
			#endif	
			}
		}
	
		if (JC_NULL != pstNapDef->pstBearer)
		{
			if (0 == jc_strcasecmp (pstNapDef->pstBearer->pcValue, gpcGPRSString))
			{
				if (JC_NULL != pstNapDef->pcNapAddress)
				{
					jc_strncpy (pstProfile->stProfile.apn, 
						pstNapDef->pcNapAddress, 
						MAX_DATA_CONN_APN_SIZE) ;
				}
				pstProfile->stProfile.bear_type = E_DC_BEARER_GPRS ;
			}
			else if (0 == jc_strcasecmp (pstNapDef->pstBearer->pcValue, gpcCSDString))
			{
				jc_strncpy (pstProfile->stProfile.dialup_number, pstNapDef->pcNapAddress, 
					E_DC_MAX_PHONE_NUMBER_LEN) ;
				if (JC_NULL != pstNapDef->pcLinkSpeed)
				{
					pstProfile->stProfile.baudrate = jc_atoi (pstNapDef->pcLinkSpeed) ;
				}
				pstProfile->stProfile.bear_type = E_DC_BEARER_CSD ;

				if (0 == jc_strcasecmp (pstNapDef->pcCallType, gpcModemString))
				{
					pstProfile->stProfile.csd_call_type = E_DC_CSD_CALL_ANALOG ;
				}
				else if (0 == jc_strcasecmp (pstNapDef->pcCallType, gpcX31String))
				{
					pstProfile->stProfile.csd_call_type = E_DC_CSD_CALL_ISDN ;
				}
			}
			else
			{
				rCode = JC_ERR_INVALID_PARAMETER ;
			}
			if (JC_NULL != pstNapDef->pcFirstRetryTimeout)
			{
				pstProfile->stProfile.timeout = jc_atoi (pstNapDef->pcFirstRetryTimeout) ;
			}
		}

		pstDNS = pstNapDef->pstDnsAddr ;
		if (JC_NULL != pstDNS && JC_NULL != pstDNS->pcValue)
		{
			jc_strncpy(pstProfile->stProfile.dns1, pstDNS->pcValue, E_DC_MAX_IP_LEN);
			pstDNS = pstDNS->pstNext ;
			if (JC_NULL != pstDNS && JC_NULL != pstDNS->pcValue)
			{
				jc_strncpy(pstProfile->stProfile.dns2, pstDNS->pcValue, E_DC_MAX_IP_LEN);
			}
		}


		(*ppstProfList) = pstProfile ;
		pstProfile->pstNext = JC_NULL ;
	}
	else
	{
		rCode = JC_ERR_MEMORY_ALLOCATION ;
	}

	return rCode ;
	
}

/*****************************************************************************
* 
* @fn 			: prv_FillPortServiceInfo
*
* @brief		: Gives provisioning information (NAPDEF)
*
* @param[in]	: pstToNapid	Provisioning information
* @param[out]	: pstProfile	Profile list to be filled with provisioning information
*
* @retval		: void
*
******************************************************************************/
void prv_FillPortServiceInfo (ST_PROV_PORT *pstPortService, ST_PROV_INFO_LIST *pstProfile)
{
	mmi_trace(1,"JPRV_LOG: prv_FillPortServiceInfo invoked");


	ST_PROV_VALUES		*pstService = JC_NULL ;

	if (JC_NULL != pstPortService)
	{
		pstService = pstPortService->pstService ;
		if (JC_NULL != pstPortService->pcPortNumber)
		{
			pstProfile->stProfile.port = jc_atoi (pstPortService->pcPortNumber) ;
		}
		if (JC_NULL != pstService)
		{
			if (JC_NULL != pstService->pcValue)
			{
				if (0 == jc_strcasecmp (pstService->pcValue, gpcCoWspString))
				{
					pstProfile->stProfile.conn_mode = E_DC_CONNECTION_ORIENTED ;
				}
				else if (0 == jc_strcasecmp (pstService->pcValue, gpcClWspString))
				{
					pstProfile->stProfile.conn_mode = E_DC_CONNECTION_ORIENTED ; 
				}
				else if (0 == jc_strcasecmp (pstService->pcValue, gpcCoSecWspString))
				{
					pstProfile->stProfile.conn_mode = E_DC_CONNECTION_ORIENTED_SECURE ;
				}
				else if (0 == jc_strcasecmp (pstService->pcValue, gpcClSecWspString))
				{
					pstProfile->stProfile.conn_mode = E_DC_CONNECTION_ORIENTED_SECURE ;
				}	
			}
		}
	}

}

void prv_CheckmaxProfiles()
{
	JC_INT32 iMaxProfiles = 0 , iTotalProfiles = 0;
	JC_BOOLEAN bProfileExist = E_FALSE;
	JC_INT8 index = 0;
	 //call platform API to save settings
	 mmi_trace(1,"JPRV_LOG: sprv_CheckmaxProfiles");
	 if(gstProvContext.pstCurrCsProfInfo->eCSProfileType == E_CS_DATACONN_PROFILE)
	 {
	 	ST_CS_DATACONN_PROFILE* pstDataConn = (ST_CS_DATACONN_PROFILE*)(gstProvContext.pstCurrCsProfInfo->pProfile) ;

		iMaxProfiles =  cs_GetMaxDataConnProfiles();
		iTotalProfiles =  cs_GetTotalDataConnProfiles();
		bProfileExist = cs_DataConnProfileExist(pstDataConn, &index);

		// if dataconn with same napid exist and if all profiles are full , none is empty
		if((bProfileExist== E_FALSE) && iMaxProfiles == iTotalProfiles)
		{
			 mmi_trace(1,"JPRV_LOG: all profiles filled");
			prv_MMIDisplayReplaceConfirmationScreen();		
		}
		else
		{
			prv_MMIDisplaySaveConfirmationScreen();
		}
	 }
	 else // if its a proxy profile
	 {


		ST_CS_PROXY_PROFILE *pstProxyProfile = (ST_CS_PROXY_PROFILE *)(gstProvContext.pstCurrCsProfInfo->pProfile);
		iTotalProfiles = cs_GetTotalProxyProfiles(pstProxyProfile->profile_type, pstProxyProfile->nSimID);
		iMaxProfiles = cs_GetMaxProxyProfiles(pstProxyProfile->profile_type);
		// all profiles are full , none is empty
		bProfileExist = cs_ProxyProfileExist(pstProxyProfile, &index) ;
		// if proxy with same proxyid exist and if all profiles are full , none is empty
	 	if((bProfileExist== E_FALSE) && iTotalProfiles == iMaxProfiles )
	 	{
	    	 mmi_trace(1,"JPRV_LOG: all profiles filled");
			prv_MMIDisplayReplaceConfirmationScreen();
		}
		else
		{
			prv_MMIDisplaySaveConfirmationScreen();
		}
		
	 }

}

/*****************************************************************************
* 
* @fn 			: prv_SaveSettings
*
* @brief		: This function initialises the UI for the provisioning process.
*
* @param[in]	: No Parameter.
* @param[out]	: No Parameter.
*
* @retval		: void.
*
******************************************************************************/



void prv_SaveSettings (void)
{
	 //call platform API to save settings
	 mmi_trace(1,"JPRV_LOG: saved settings sucessfully");
	 if(gstProvContext.pstCurrCsProfInfo->eCSProfileType == E_CS_DATACONN_PROFILE)
	 {
	 	ST_CS_DATACONN_PROFILE* pstDataConn = (ST_CS_DATACONN_PROFILE*)(gstProvContext.pstCurrCsProfInfo->pProfile) ;

		if(pstDataConn->bear_type != E_DC_BEARER_CSD)
		{
	 		cs_SaveDataConnProfile(pstDataConn, 0xFF);
			prv_MMIDisplaySaveSuccessScreen();
		}
		// if the profile is not other than GPRS profile
		else
		{
			prv_MMIDisplayInvalidSettingsPopUp();
			gstProvContext.pstCurrCsProfInfo->IsProfileDiscarded = E_TRUE;
			prv_SkipNextProxyProfile();
			prv_ShowNextCSProfile();
		}
		
	 }
	 else // if its a proxy profile
	 {

		ST_CS_PROXY_PROFILE *pstProxyProfile = (ST_CS_PROXY_PROFILE *)(gstProvContext.pstCurrCsProfInfo->pProfile);
 		cs_SaveProxyProfile(pstProxyProfile, 0xFF);
		prv_MMIDisplaySaveSuccessScreen();
	 }
}

void prv_ReplaceSettings (void)
{
	 //call platform API to save settings
	 mmi_trace(1,"JPRV_LOG: saved settings sucessfully");
	 if(gstProvContext.pstCurrCsProfInfo->eCSProfileType == E_CS_DATACONN_PROFILE)
	 {
	 	ST_CS_DATACONN_PROFILE* pstDataConn = (ST_CS_DATACONN_PROFILE*)(gstProvContext.pstCurrCsProfInfo->pProfile) ;

		if(pstDataConn->bear_type != E_DC_BEARER_CSD)
		{
			memset(gpszDataConnList, 0, sizeof(ST_CS_DATACONN_PROFILE_NAME)*MAX_DATA_ACCOUNT_GPRS_LIMIT);
			cs_GetDataConnProfileList(gpszDataConnList);
			prv_MMIShowDataConnProfileList();
		}
		// if the profile is not other than GPRS profile
		else
		{
			prv_MMIDisplayInvalidSettingsPopUp();
			gstProvContext.pstCurrCsProfInfo->IsProfileDiscarded = E_TRUE;
			prv_SkipNextProxyProfile();
			prv_ShowNextCSProfile();
		}
		
	 }
	 else // if its a proxy profile
	 {
			ST_CS_PROXY_PROFILE *pstProxyProfile = (ST_CS_PROXY_PROFILE *)(gstProvContext.pstCurrCsProfInfo->pProfile);
	 		memset(gpszProxyProfileList, 0, sizeof(ST_CS_PROXY_PROFILE_NAME)*NVRAM_WAP_PROFILE_MAX);
	 		cs_GetProxyProfileList(gpszProxyProfileList, pstProxyProfile->profile_type, pstProxyProfile->nSimID);
			prv_MMIShowProxyProfileList();

	 }
	 
}


void prv_ReplaceProxyProfile()
{
	ST_CS_PROXY_PROFILE *pstProxyProfile = (ST_CS_PROXY_PROFILE *)(gstProvContext.pstCurrCsProfInfo->pProfile);
	mmi_trace(1,"JPRV_LOG: prv_ReplaceProxyProfile %d",giSelectProfileIndex);
	cs_ReplaceProxyProfile(pstProxyProfile,
				giSelectProfileIndex);
	prv_MMIDisplaySaveSuccessScreen();
}

void prv_ReplaceDataConnProfile()
{
	ST_CS_DATACONN_PROFILE* pstDataConn = (ST_CS_DATACONN_PROFILE*)(gstProvContext.pstCurrCsProfInfo->pProfile) ;
	mmi_trace(1,"JPRV_LOG: prv_ReplaceDataConnProfile %d",giSelectProfileIndex);
	cs_ReplaceDataConnProfile(pstDataConn,
				giSelectProfileIndex);
	prv_MMIDisplaySaveSuccessScreen();

}
void prv_DiscardSettings (void)
{
	 //call platform API to save settings
	 mmi_trace(1,"JPRV_LOG: settings discarded");
	 //prv_DeInitContext();
	 gstProvContext.pstCurrCsProfInfo->IsProfileDiscarded = E_TRUE;

	prv_SkipNextProxyProfile();
	 
	 prv_MMIDisplayDiscardSuccessScreen();
}


// skip next proxy profile if the corresponding data connection is ignored
void prv_SkipNextProxyProfile()
{
	if(gstProvContext.pstCurrCsProfInfo->eCSProfileType == E_CS_DATACONN_PROFILE)
	{
	   ST_CS_PROFILE_NODE * pstCSProfileNode = gstProvContext.pstCurrCsProfInfo->pNext;
	   if(pstCSProfileNode && 
		   pstCSProfileNode->eCSProfileType == E_CS_PROXY_PROFILE&& 
		   !(jc_strcmp(((ST_CS_PROXY_PROFILE *)(pstCSProfileNode->pProfile))->nap_id, 
		   ((ST_CS_PROXY_PROFILE *)(pstCSProfileNode->pProfile))->nap_id))
		   )
	   {
			mmi_trace(1,"JPRV_LOG: discarding next proxy profile");
			gstProvContext.pstCurrCsProfInfo =	gstProvContext.pstCurrCsProfInfo->pNext;
	   }
	}
}

/*****************************************************************************
* 
* @fn 			: prv_DownloadContinousProvData
*
* @brief		: This function downloads 'continuous' provisioning data
*
* @param[in]	: pProfile	Profile information received
*
* @retval		: JC_OK	On Successful and others on failure specific error code.
*
******************************************************************************/
void prv_DownloadContinousProvData ()
{
	ST_COMM_FETCH_URL		stFetchUrl = {JC_NULL, };
	JC_RETCODE				rCode = JC_OK ;
	JC_TASK_INFO			stAppTaskInfo ;
	JC_TASK_INFO			stTransportTaskInfo ;
	ST_COMM_PROFILE			stCommProfile = {E_COMM_WSP_CONNECTION_ORIENTED, };
	ST_GENERIC_DATA_CONN	stCommDataConn = {E_DATA_CONN_CSD_BEARER_TYPE, };
	ST_PROV_INFO			*pstProvInfo = gstProvContext.pstProvInfo ;
	
	ST_PROV_INFO_LIST *pstProfile = gstProvContext.pstContProfList;

	mmi_trace(1,"JPRV_LOG: prv_DownloadContinousProvData inovoked");
	//pstProvInfo = prv_GetProvInfo (0, E_TRUE) ;
	if (JC_NULL == pstProfile || JC_NULL == pstProvInfo)
	{
		//jdd_AssertOnFail(0,"Invalid State");
		mmi_trace(1, "JC_NULL == pstProfile || JC_NULL == pstProvInfo");
	}
	else
	{
			//stCommDataConn.uiProfileID = pstProfile->stProfile.profile_id;
			stAppTaskInfo.iTaskID = (JC_INT32) E_TASK_WAP_PROVISIONING_CORE ;
			stAppTaskInfo.iAppID = (JC_INT32) E_TASK_WAP_PROVISIONING_UI ;
			stTransportTaskInfo.iTaskID = (JC_INT32) E_TASK_TRANSPORT ;
			stTransportTaskInfo.iAppID = (JC_INT32) E_TASK_TRANSPORT ;
			prv_DeInitCommunicator () ;
			rCode = jdi_CommunicationInitialize (&stAppTaskInfo, &stTransportTaskInfo, &gstProvContext, 
						prv_CBCommProvIndication, JC_NULL, &(gstProvContext.vCommHandle)) ;
			prv_updateMissingProfileInfo (&pstProfile->stProfile) ;
			if (0 != pstProfile->stProfile.port)
			{
				stCommProfile.uiPortNumber = pstProfile->stProfile.port ;
			}
			else
			{
				if (E_DC_CONNECTION_ORIENTED == pstProfile->stProfile.conn_mode)
				{
					stCommProfile.uiPortNumber = PRV_PORT_9201 ;
				}
				else if (E_DC_CONNECTION_ORIENTED_SECURE == pstProfile->stProfile.conn_mode)
				{
					stCommProfile.uiPortNumber = PRV_PORT_9203 ;
				}
			}

			if ('\0' != *pstProfile->stProfile.proxy_ip)
			{
				jc_strcpy (stCommProfile.acHostName, pstProfile->stProfile.proxy_ip ) ;
			}

			
			/* Fill Data Conn Info */
			stCommDataConn.eBearerType = E_DATA_CONN_GPRS_BEARER_TYPE ;

			jc_strncpy (stCommDataConn.uDataConnType.stGPRSDataConn.acAPN, pstProfile->stProfile.apn, MAX_DATA_CONN_USERNAME_SIZE) ;
			mmi_trace (1,"JPRV_LOG:settings: Apn name :[%s]",stCommDataConn.uDataConnType.stGPRSDataConn.acAPN);
			jc_strncpy (stCommDataConn.acUserName, pstProfile->stProfile.username, MAX_DATA_CONN_USERNAME_SIZE) ;
			jc_strncpy (stCommDataConn.acPassword, pstProfile->stProfile.password, MAX_DATA_CONN_PASSWORD_SIZE) ;

			

			if (E_DC_CONN_MODE_MAX != pstProfile->stProfile.conn_mode)
			{
				switch (pstProfile->stProfile.conn_mode)
				{
					case E_DC_CONNECTION_ORIENTED:
						stCommProfile.eConnMode = E_COMM_WSP_CONNECTION_ORIENTED ;
						break;
					case E_DC_CONNECTION_ORIENTED_SECURE:
						stCommProfile.eConnMode = E_COMM_WSP_CONNECTION_ORIENTED_SECURE ;
						break;
					case E_DC_WPHTTP_PROXY:
						stCommProfile.eConnMode = E_COMM_WPHTTP_PROXY ;
						break;
					case E_DC_WPHTTP_DIRECT:
						stCommProfile.eConnMode = E_COMM_WPHTTP_DIRECT ;
						break;
					default:
						stCommProfile.eConnMode = E_COMM_WSP_CONNECTION_ORIENTED ;
						break;
				}
			}
			else
			{
				switch (pstProfile->stProfile.port)
				{
					case PRV_PORT_9200:
					case PRV_PORT_9201:
						stCommProfile.eConnMode = E_COMM_WSP_CONNECTION_ORIENTED ;
						break ;

					case PRV_PORT_443:	
					case PRV_PORT_9202:
					case PRV_PORT_9203:
						stCommProfile.eConnMode = E_COMM_WSP_CONNECTION_ORIENTED_SECURE ;
						break ;

					case PRV_PORT_80:
					case PRV_PORT_9401:
					case PRV_PORT_3128:
					case PRV_PORT_8080:
					case PRV_PORT_8008:
						stCommProfile.eConnMode = E_COMM_WPHTTP_PROXY ;
						break ;

					default:
						if (0 == jc_strlen(stCommProfile.acHostName))
						{
							stCommProfile.eConnMode = E_COMM_WPHTTP_DIRECT ;
						}
						else
						{
							stCommProfile.eConnMode = E_COMM_WPHTTP_PROXY ;
						}
						break ;
				}
			}
		mmi_trace(1,"JPRV_LOG: eConnMode  %d port %d host %s",stCommProfile.eConnMode,stCommProfile.uiPortNumber,stCommProfile.acHostName);		

		// for testing 
			//pstProvInfo->nSimID = 0;
#ifdef __MMI_MULTI_SIM__
			stCommDataConn.uiConnID = WAP_MMS_UICONNID_SIM1_FLAG<<pstProvInfo->nSimID ;	
			mmi_trace(1,"JPRV_LOG: nSimID %d",stCommDataConn.uiConnID);
			stCommDataConn.uiConnID |= WAP_MMS_UICONNID_PROV_FLAG ;		
#else
			stCommDataConn.uiConnID = 3 ;	 /* 3 for provisioning */
#endif

		// hard coding for testing . remove later
		//stCommProfile.eConnMode = E_COMM_WPHTTP_DIRECT ;
		rCode = jdi_CommunicationProfile (gstProvContext.vCommHandle, &stCommProfile) ;
		mmi_trace(1,"JPRV_LOG: jdi_CommunicationProfile returns %d",rCode);
		if(JC_OK == rCode)
		{
			rCode = jdi_CommunicationDataConnection (gstProvContext.vCommHandle, &stCommDataConn) ;
			mmi_trace(1,"JPRV_LOG: jdi_CommunicationDataConnection returns %d",rCode);
		}
	}

	if (JC_OK == rCode)
	{
		stFetchUrl.eCommFetchType = E_COMM_FETCH_GET ;
		stFetchUrl.eCommRequestType = E_COMM_MAIN_REQUEST ;
		rCode = jdi_CUtilsCharToTchar (JC_NULL, gstProvContext.pcProvURL, &(stFetchUrl.pmURL)) ; 
		if (JC_OK == rCode)
		{
			mmi_trace(1,"JPRV_LOG: jdi_CommunicationFetchURL inovoked");
			if(gstProvContext.vCommHandle)
			{
				rCode =  jdi_CommunicationFetchURL (gstProvContext.vCommHandle, &stFetchUrl,
									&(pstProvInfo->uiRequestID)) ;
			}
			else
			{
				mmi_trace(1, "gstProvContext.vCommHandle is NULL");
			}
			mmi_trace(1,"JPRV_LOG: jdi_CommunicationFetchURL returns %d",rCode);
			jdd_MemFree (stFetchUrl.pmURL) ;
		}
	}
	if (JC_OK != rCode)
	{
	   mmi_trace(1, "JPRV_LOG:error occured[%d]", rCode);
	}
	//return rCode ;
}

/*****************************************************************************
*
* @fn 			:	prv_updateMissingProfileInfo
*
* @brief		:	This function updates the missing/incorrect profile information
*					to the structure passed. The missing/incorrect information are
*					port number, connection mode or bearer type.
*
* @param[in/out]:	pstDestProf	The pointer to the profile structure that needs to be updated.
*
* @retval		:	void.
*
******************************************************************************/
void prv_updateMissingProfileInfo (DATACONN_PROFILE *pstDestProf)
{
	mmi_trace(1,"JPRV_LOG: prv_updateMissingProfileInfo");
	if (0 == pstDestProf->port)
	{
		mmi_trace(1,"JPRV_LOG: 0 == pstDestProf->port");
		if (E_DC_CONNECTION_ORIENTED == pstDestProf->conn_mode)
		{
			pstDestProf->port = PRV_PORT_9201 ;
		}
		else if (E_DC_CONNECTION_ORIENTED_SECURE == pstDestProf->conn_mode)
		{
			pstDestProf->port = PRV_PORT_9203 ;
		}
		mmi_trace(1,"JPRV_LOG: port[%d]", pstDestProf->port);
	}

	if (E_DC_CONN_MODE_MAX == pstDestProf->conn_mode)
	{
		mmi_trace(1,"JPRV_LOG: E_DC_CONN_MODE_MAX");
		switch (pstDestProf->port)
		{
			case PRV_PORT_9200:
			case PRV_PORT_9201:
				pstDestProf->conn_mode = E_DC_CONNECTION_ORIENTED ;
				break ;

			case PRV_PORT_443:	
			case PRV_PORT_9202:
			case PRV_PORT_9203:
				pstDestProf->conn_mode = E_DC_CONNECTION_ORIENTED_SECURE ;
				break ;

			case PRV_PORT_80:
			case PRV_PORT_9401:
			case PRV_PORT_3128:
			case PRV_PORT_8080:
			case PRV_PORT_8008:
				pstDestProf->conn_mode = E_DC_WPHTTP_PROXY ;
				break ;

			default:
				if ('\0' == *pstDestProf->proxy_ip)
				{
					pstDestProf->conn_mode = E_DC_WPHTTP_DIRECT ;
				}
				else
				{
					pstDestProf->conn_mode = E_DC_WPHTTP_PROXY ;
				}
				break ;
		}
		mmi_trace(1,"JPRV_LOG: conn_mode %d", pstDestProf->conn_mode);
	}

	if (E_DC_BEARER_MAX == pstDestProf->bear_type)
	{
		mmi_trace(1,"JPRV_LOG: E_DC_BEARER_MAX");
		if ('\0' == *pstDestProf->apn)
		{
			pstDestProf->bear_type = E_DC_BEARER_GPRS ;
		}
		else
		{
			pstDestProf->bear_type = E_DC_BEARER_CSD ;
		}
		mmi_trace(1,"JPRV_LOG: bear_type %d", pstDestProf->bear_type);
	}

	if (E_DC_BEARER_GPRS == pstDestProf->bear_type)
	{
	#if 0
		if (GPRS_AUTH_NONE == pstDestProf->gprs_auth_type)
		{
			pstDestProf->gprs_auth_type = GPRS_AUTH_PAP ;
		}
	#endif	
	}
	else
	{
		if (E_DC_CSD_CALL_MAX == pstDestProf->csd_call_type)
		{
			pstDestProf->csd_call_type = E_DC_CSD_CALL_ANALOG ;
		}
		if (0 == pstDestProf->baudrate)
		{
			pstDestProf->baudrate =  PRV_DEFAULT_BAUDRATE ; 
		}
		if (0 == pstDestProf->timeout)
		{
			pstDestProf->timeout = PRV_DEFAULT_TIMEOUT ;
		}
	}
}
/*****************************************************************************
* 
* @fn 			: prv_CBCommProvIndication
*
* @brief		: Handles the events from the communicator.
*
* @param[in]	: pvAppArg	The callback argument.
* @param[in]	: eCommIndicationType	Event type.
* @param[in]	: pvIndication Event specific data.
*
* @retval		: JC_OK	On Successful and others on failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_CBCommProvIndication(
							void					*pvAppArg,
							E_COMM_INDICATION		eCommIndicationType,
							void					*pvIndication)
{
	JC_RETCODE	rCode = JC_OK ;

	mmi_trace(12, "JPRV_LOG: prv_CBCommProvIndication called with %d ", eCommIndicationType);
	switch (eCommIndicationType)
	{
		case E_HEADER_INDICATION:
			rCode = prv_HeaderIndication ((ST_COMM_HEADER_IND *)pvIndication) ;
			mmi_trace (0,"JPRV_LOG: prv_HeaderIndication () returned rCode as [%x]", -rCode) ;
			break ;
		case E_FETCH_INDICATION:
			rCode = prv_FetchIndication ((ST_COMM_FETCH_IND *)pvIndication) ;
			mmi_trace (0, "JPRV_LOG: prv_FetchIndication () returned rCode as [%x]", -rCode) ;
			break ;
		case E_ERROR_INDICATION:
			{
				ST_COMM_ERR_IND	*pstErroInd = (ST_COMM_ERR_IND *)pvIndication ;
				mmi_trace (0, "JPRV_LOG: E_ERROR_INDICATION recvd") ;
				if (JC_NULL != pstErroInd)
				{
					//pstErroInd->rCode
					prv_ResetContinousProvdata();
					//DeleteUptoScrID(SCR_ID_PROV_CONT_DEATILS_SCREEN);
					GoBackHistory();
					prv_MMIDisplayNetErrorPopUp();
					
				}
			}
			break ;
		case E_PROGRESS_INDICATION:
			break ;
		case E_STOP_INDICATION:
			{
				mmi_trace (0, "JPRV_LOG: E_STOP_INDICATION recvd, %d",gstProvContext.bEndKeyPressed) ;
				prv_ResetContinousProvdata();
				//U8 val = DeleteUptoScrID(SCR_ID_PROV_CONT_DEATILS_SCREEN);
				if(gstProvContext.bEndKeyPressed == E_TRUE)
				{
					gstProvContext.bEndKeyPressed = E_FALSE;
					//DeleteUptoScrID(IDLE_SCREEN_ID);
					prv_MMIDisplayIDLEScreen();
				}
				else
				{
					DeleteUptoScrID(SCR_ID_PROV_CONT_DETAILS_SCREEN);
					GoBackHistory();
				}
			}
			break ;
		case E_STOP_ALL_INDICATION:
			break ;
		case E_PROFILE_CONFIRMATION_INDICATION:
			break ;
		case E_USER_DATA_INDICATION:
			break ;
		case E_PUSH_INDICATION:
			break ;
		case E_SECURITY_INDICATION:
			break ;
		case E_SECURITY_FAILURE_INDICATION:
			break ;
		case E_CERT_RESP_INDICATION:
			break ; 
		default:
			break ;
	}
	return JC_OK ;
}

/*****************************************************************************
* 
* @fn 			: prv_HeaderIndication
*
* @brief		: Function which handles the prov header indication
*
* @param[in]	: pstHeaderIndication Pointer to header indication
*
* @retval		: JC_OK	On Successful and others on failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_HeaderIndication (ST_COMM_HEADER_IND	*pstHeaderIndication)
{
	JC_RETCODE rCode = JC_OK ;
	JC_BOOLEAN	bIsHHTPHeader = E_FALSE ;
	mmi_trace(12,"JPRV_LOG: prv_HeaderIndication recvd %d",pstHeaderIndication->uiContentLength);
	if (JC_NULL == pstHeaderIndication || JC_NULL == pstHeaderIndication->pcContentType ||
		JC_NULL == pstHeaderIndication->pucHeader || 0 >= pstHeaderIndication->uiHeaderLength)
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	else
	{
		if (E_FALSE == pstHeaderIndication->bIsBinary)
		{
			bIsHHTPHeader = E_TRUE ;
		}
		rCode = prv_AppendHeader (pstHeaderIndication->pucHeader, pstHeaderIndication->uiHeaderLength,
							bIsHHTPHeader, pstHeaderIndication->uiRequestID) ;
	}
	return rCode ;
}

/*****************************************************************************
* 
* @fn 			: prv_FetchIndication
*
* @brief		: Function which handles the prov buffer indication
*
* @param[in]	: pstFetchIndication Pointer to fetch response
*
* @retval		: JC_OK	On Successful and others on failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_FetchIndication (ST_COMM_FETCH_IND *pstFetchIndication)
{

	JC_RETCODE rCode = JC_OK ;
	mmi_trace(12,"JPRV_LOG: prv_FetchIndication recvd");
	if (JC_NULL == pstFetchIndication)
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	if (JC_OK == rCode)
	{
		rCode = prv_AppendBuffer (pstFetchIndication->pucBuffer,
							pstFetchIndication->uiCurrentLength,
							pstFetchIndication->bHasMoreData,
							pstFetchIndication->uiRequestID) ;
	}
	if (JC_OK != rCode)
	{
		prv_DeInitContext () ;
	}
	return rCode ;
}

/*****************************************************************************
* 
* @fn 			: prv_AppendBuffer
*
* @brief		: This function appends the buffer of the provisioning content.
*
* @param[in]	: pHeader	Content buffer.
*
* @param[in]	: uiHeaderLen	Content buffer length.
*
* @param[in]	: bHasMoreData  Is more data available.
*
* @param[in]	: uiRequestID	Unique request ID.
*
* @retval		: JC_OK On Success, On failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_AppendBuffer (const JC_UINT8 *pucBuffer, const JC_UINT32 uiBufferLen,
							const JC_BOOLEAN 		bHasMoreData,
							const JC_UINT32			uiRequestID) 
{
	JC_RETCODE			rCode = JC_OK;
	JC_UINT8 			*pucTempBuffer = JC_NULL ;
	JC_UINT32			uiNewBufferLen = 0 ;
	ST_PROV_INFO		*pstProvInfo = gstProvContext.pstProvInfo ;

	mmi_trace(12,"JPRV_LOG: prv_AppendBuffer");

    
	if (JC_NULL == pstProvInfo || pstProvInfo->uiRequestID != uiRequestID)
	{
		jdd_AssertOnFail(0,"Inavlid state")
	}

	if (JC_OK == rCode && 0 != uiBufferLen)
	{
		uiNewBufferLen = pstProvInfo->uiBufferLen + uiBufferLen ;
		pucTempBuffer = (JC_UINT8  *)jdd_MemAlloc(sizeof(JC_UINT8), uiNewBufferLen + 1);
		if (JC_NULL != pucTempBuffer)
		{
			if (0 != pstProvInfo->uiBufferLen)
			{
				jc_memcpy(pucTempBuffer, pstProvInfo->pucBuffer, pstProvInfo->uiBufferLen) ;
				jdd_MemFree (pstProvInfo->pucBuffer) ;
				pstProvInfo->pucBuffer = pucTempBuffer ;
				pucTempBuffer = pucTempBuffer + pstProvInfo->uiBufferLen ;
			}
			else
			{
				pstProvInfo->pucBuffer = pucTempBuffer ;
			}
			jc_memcpy(pucTempBuffer, pucBuffer, uiBufferLen) ;
			pstProvInfo->uiBufferLen = uiNewBufferLen ;
		}
		else
		{
			rCode = JC_ERR_MEMORY_ALLOCATION ;
		}
	}
	if (E_FALSE == bHasMoreData && JC_OK == rCode)
	{
		/*JC_INT32 i =0 ;

		for( i=0 ; i < pstProvInfo->uiBufferLen ; i++)
			mmi_trace(0,"%c",pstProvInfo->pucBuffer[i]);*/
		mmi_trace(12,"JPRV_LOG: buffer len %d",pstProvInfo->uiBufferLen);
		
		if (gstProvContext.vCommHandle)
		{
			mmi_trace(12, "JPRV_LOG: invoking prv_ProcessProvInfo");
			prv_ProcessProvInfo ();
		}
		else
		{
			mmi_trace(12, "gstProvContext.vCommHandle is NULL");
		}
	}

	if (JC_OK != rCode)
	{
		prv_DeInitContext () ;
	}
	return rCode ;
}


/*****************************************************************************
*
* @fn 			: prv_AppendHeader
*
* @brief		: This function appends the header of the provisioning content.
*
* @param[in]	: pHeader	Header buffer.
*
* @param[in]	: uiHeaderLen	Header buffer length.
*
* @param[in]	: bIsHTTPHeader	Specify HTTP header or WSP header.
* 
* @param[in]	: uiRequestID	Unique request ID.
*
* @retval		: JC_OK On Success, On failure specific error code.
*
******************************************************************************/
JC_RETCODE prv_AppendHeader (const JC_UINT8 *pucHeader, const JC_UINT32 uiHeaderLen,
							const JC_BOOLEAN 		bIsHTTPHeader,
							const JC_UINT32			uiRequestID)
{
	JC_RETCODE			rCode = JC_OK ;
	JC_UINT8 			*pucTempHeader = JC_NULL ;
	JC_UINT32			uiNewHeaderLen = 0 ;
	ST_PROV_INFO		*pstProvInfo = gstProvContext.pstProvInfo ;

	//pstProvInfo = prv_GetProvInfo (uiRequestID, E_TRUE) ;

	if (JC_NULL != pstProvInfo)
	{
		if (0 != pstProvInfo->uiBufferLen)
		{
			jdd_MemFree (pstProvInfo->pucBuffer) ;
			pstProvInfo->uiBufferLen = 0 ;
		}
		if (0 != pstProvInfo->uiHeaderLen)
		{
			jdd_MemFree (pstProvInfo->pucHeader) ;
			pstProvInfo->uiHeaderLen = 0 ;
		}
		if (0 == uiHeaderLen)
		{
			rCode = JC_ERR_INVALID_PARAMETER ;
		}
		if (JC_OK == rCode)
		{
			uiNewHeaderLen = pstProvInfo->uiHeaderLen + uiHeaderLen ;
			pucTempHeader = (JC_UINT8  *)jdd_MemAlloc(sizeof(JC_UINT8), uiNewHeaderLen + 1);
			if (JC_NULL != pucTempHeader)
			{
				if (0 != pstProvInfo->uiHeaderLen)
				{
					jc_memcpy(pucTempHeader, pstProvInfo->pucHeader, pstProvInfo->uiHeaderLen) ;
					jdd_MemFree (pstProvInfo->pucHeader) ;
					pstProvInfo->pucHeader = pucTempHeader ;
					pucTempHeader = pucTempHeader + pstProvInfo->uiHeaderLen ;
				}
				else
				{
					pstProvInfo->pucHeader = pucTempHeader ;
				}
				jc_memcpy(pucTempHeader, pucHeader, uiHeaderLen) ;
				pstProvInfo->uiHeaderLen = uiNewHeaderLen ;
				pstProvInfo->uiRequestID = uiRequestID ;
			}
			else
			{
				rCode = JC_ERR_MEMORY_ALLOCATION ;
			}
		}
		pstProvInfo->bIsHTTPHeader = bIsHTTPHeader ;
	}
	else
	{
		jdd_AssertOnFail(0,"inavlid state")
	}

	if (JC_OK != rCode)
	{
		prv_DeInitContext() ;
	}
	return rCode ;
}

void prv_CheckAuthentication()
{
	JC_INT8					*pcPinEntry ;
	JC_RETCODE rCode = JC_ERR_GENERAL_FAILURE;
	mmi_trace(1,"JPRV_LOG:prv_CheckAuthentication  invoked %d",gstProvContext.pmPinValue );
	if (gstProvContext.pmPinValue)
	{
		jdi_CUtilsTcharToChar (JC_NULL, gstProvContext.pmPinValue, &pcPinEntry) ;
		if (pcPinEntry)
		{
			mmi_trace(1,"JPRV_LOG:jdi_ProvisionAuthenticate  called");
			rCode = jdi_ProvisionAuthenticate (gstProvContext.vProvHandle, 
				(JC_UINT8 *) pcPinEntry, jc_strlen (pcPinEntry)) ;
		}
		else
		{
			rCode = JC_ERR_AUTHENTICATION;
		}
		jdd_MemFree (pcPinEntry) ;
	}
	if( rCode == JC_OK)
	{
		mmi_trace(1,"JPRV_LOG:authentication success");
		prv_FormProfList();
	}
	else if( rCode == JC_ERR_AUTHENTICATION)
	{
		mmi_trace(1,"JPRV_LOG:authentication failed ");
		
		gstProvContext.ucNumOfAttempts ++ ;
		if(gstProvContext.ucNumOfAttempts < E_PRV_MAX_RETRIES)
		{
			DeleteUptoScrID(SCR_ID_PROV_USERPIN_SCREEN);
			DisplayPopup((PU8)GetString(STR_ID_PROV_INAVLID_PIN_TRYAGAIN), IMG_GLOBAL_ERROR, 1, 2000, 1) ;	
		}
		else
		{
			prv_MMIDisplayIDLEScreen();
			DisplayPopup((PU8)GetString(STR_ID_PROV_USEPIN_AUTH_FAIL), IMG_GLOBAL_ERROR, 1, 2000, 1) ;	
		}
		
	}
	else
	{
		mmi_trace(1,"JPRV_LOG:pin error %d ",rCode );
		prv_MMIDisplayIDLEScreen();
		DisplayPopup((PU8)GetString(STR_ID_PROV_ERR_UNKNOWN), IMG_GLOBAL_ERROR, 1, 2000, 1) ;
	}
}

void prv_HandleDownloadContinousProvData()
{
	prv_DownloadContinousProvData();
	prv_DisplayDownloadScreen();
}

void prv_HandleStopRequest()
{
	//mmi_show_in_progress(STR_GLOBAL_PLEASE_WAIT,JC_NULL);
	mmi_trace(1, "JPRV_LOG:prv_HandleStopRequest");
	if(gstProvContext.pstProvInfo->uiRequestID != 0)
	{
		mmi_trace(1,"JPRV_LOG:jdi_CommunicationStopReq called for %d",gstProvContext.pstProvInfo->uiRequestID);
		jdi_CommunicationStopReq (gstProvContext.vCommHandle, gstProvContext.pstProvInfo->uiRequestID) ;
	}
}


JC_UINT8 prv_ApplicationExitHandler(void* p)
{
	mmi_trace(1, "JPRV_LOG:prv_ApplicationExitHandler");
	prv_DeInitContext ();
	return 0;
}


void prv_ContDownloadEndKeyHandler()
{
	gstProvContext.bEndKeyPressed = E_TRUE;
	mmi_trace(1, "JPRV_LOG:prv_ContDownloadEndKeyHandler");
	prv_HandleStopRequest();
}

