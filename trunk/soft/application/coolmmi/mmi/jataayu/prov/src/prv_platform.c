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
 * @file		prv_platform.c
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
#include "prv_platform.h"
#include "prv_main.h"

#include <jdi_wspdecoder.h>
#include <jdi_httpdecoder.h>
#include "prv_str_id.h"
#ifdef  __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
#include "custom_nvram_editor_data_item.h"
#include "custom_mmi_default_value.h"
#include "nvram_user_defs.h"
#include "jprofilehandlertypes.h"
#include "nvramtype.h"
#include "dataaccountdef.h"
#include "nvramprot.h"
#include "prv_mmi.h"


// this function will form the CS compatible profile information from
// gstProvContext.pstProfList
void prv_FormCSProfiles()
{
	ST_PROV_INFO_LIST	*pstProfNode;
	mmi_trace(1,"JPRV_LOG:prv_FormCSProfiles()");
	
	pstProfNode = gstProvContext.pstProfList;

   while(pstProfNode)
   	{
		 prv_FormCSDataConnProfile(pstProfNode);
		 prv_FormCSProxyProfile(pstProfNode);
		 pstProfNode = pstProfNode->pstNext;
   	}
}

void prv_FormCSDataConnProfile(ST_PROV_INFO_LIST	*pstProfNode)
{
	ST_CS_DATACONN_PROFILE * pstCSDataConn = NULL;

	mmi_trace(1,"JPRV_LOG:prv_FormCSDataConnProfile()");

	pstCSDataConn  = (ST_CS_DATACONN_PROFILE *)jdd_MemAlloc(sizeof(ST_CS_DATACONN_PROFILE),1);
	jc_strcpy(pstCSDataConn->apn ,pstProfNode->stProfile.apn);
	jc_strcpy(pstCSDataConn->nap_id , pstProfNode->stProfile.nap_id);
	jc_strcpy(pstCSDataConn->username , pstProfNode->stProfile.username);
	jc_strcpy(pstCSDataConn->password ,pstProfNode->stProfile.password);
	pstCSDataConn->bear_type = pstProfNode->stProfile.bear_type;
	pstCSDataConn->gprs_auth_type = pstProfNode->stProfile.gprs_auth_type;
	prv_AddToCSProfiles((void *) pstCSDataConn, E_CS_DATACONN_PROFILE);
}


void prv_FormCSProxyProfile(ST_PROV_INFO_LIST	*pstProfNode)
{
	ST_CS_PROXY_PROFILE * pstCSProxyProfile = NULL;

	mmi_trace(1,"JPRV_LOG:prv_FormCSProxyProfile()");

	pstCSProxyProfile = (ST_CS_PROXY_PROFILE *)jdd_MemAlloc(sizeof(ST_CS_PROXY_PROFILE),1);
	pstCSProxyProfile->nSimID = gstProvContext.pstProvInfo->nSimID ;
	jc_strcpy(pstCSProxyProfile->homepage,pstProfNode->stProfile.homepage);
	jc_strcpy(pstCSProxyProfile->proxy_ip,pstProfNode->stProfile.proxy_ip);
	jc_strcpy(pstCSProxyProfile->dns1,pstProfNode->stProfile.dns1);
	jc_strcpy(pstCSProxyProfile->dns2,pstProfNode->stProfile.dns2);
	jc_strcpy(pstCSProxyProfile->nap_id ,pstProfNode->stProfile.nap_id); 
	jc_strcpy(pstCSProxyProfile->proxy_id ,pstProfNode->stProfile.proxy_id); 
	pstCSProxyProfile->port = pstProfNode->stProfile.port;
	pstCSProxyProfile->conn_mode = pstProfNode->stProfile.conn_mode;
	pstCSProxyProfile->profile_type = pstProfNode->eProfType;
	mmi_trace(1,"JPRV_LOG:prv_FormCSProxyProfile profile %d", pstCSProxyProfile->profile_type);
	prv_AddToCSProfiles((void *) pstCSProxyProfile, E_CS_PROXY_PROFILE);
}

void  prv_AddToCSProfiles(void * pProfile, E_CS_PROFILE_TYPE eCSProfileType)
{
	
	ST_CS_PROFILE_NODE* pstNewNode = JC_NULL;
	ST_CS_PROFILE_NODE* pstTemp = JC_NULL ;

	mmi_trace(12,"JPRV_LOG:prv_AddToCSProfiles called");

	pstNewNode = (ST_CS_PROFILE_NODE*)jdd_MemAlloc (1, sizeof(ST_CS_PROFILE_NODE)) ;
	
	if(!pstNewNode)
	  return;	
	
	pstNewNode->pNext = JC_NULL;
	pstNewNode->pPrev = JC_NULL;
	pstNewNode->IsProfileDiscarded = E_FALSE;
	pstNewNode->pProfile = pProfile;
	pstNewNode->eCSProfileType = eCSProfileType;
		
	if(gstProvContext.pstCsProfInfo== JC_NULL)
	{
		mmi_trace(12,"JPRV_LOG:added as root node");
		gstProvContext.pstCsProfInfo = pstNewNode;
	}
	else
	{
		pstTemp = gstProvContext.pstCsProfInfo;
		while(pstTemp->pNext != JC_NULL)
			pstTemp = pstTemp->pNext;
		mmi_trace(12,"JPRV_LOG:added to the link list");
		pstNewNode->pPrev = pstTemp;
		pstTemp->pNext = pstNewNode;	
	}
}



void prv_DeleteAllCSProfiles()
{
	ST_CS_PROFILE_NODE* pstTempNode, *pstPrevNode ;
	mmi_trace(1,"JPRV_LOG:prv_DeleteAllCSProfiles()");

	pstTempNode = gstProvContext.pstCsProfInfo ;

	while ( pstTempNode )
	{
		pstPrevNode = pstTempNode;
		pstTempNode = pstTempNode->pNext ;
		jdd_MemFree(pstPrevNode->pProfile);
		jdd_MemFree (pstPrevNode);
	}
	gstProvContext.pstCsProfInfo = JC_NULL;
}



void prv_FormCSProfDetailsBuff()
{
	mmi_trace(1,"JPRV_LOG:prv_FormCSProfDetailsBuff()");
	if( gstProvContext.pstCurrCsProfInfo->eCSProfileType == E_CS_DATACONN_PROFILE)
	{
		prv_FormCSDataConnProfDeatilsBuff();
	}
	else
	{
		prv_FormCSProxyProfDeatilsBuff();
	}
}

//void prv_GetNextNewCSProfile()
void prv_SkipRedudantCSDataConnProfile()
{
//	JC_BOOLEAN bSkipProfile = E_TRUE;

		mmi_trace(1, "JPRV_LOG: prv_SkipRedudantCSDataConnProfile");
 	if(gstProvContext.pstCurrCsProfInfo &&
		gstProvContext.pstCurrCsProfInfo->eCSProfileType== E_CS_DATACONN_PROFILE)
	{
		ST_CS_PROFILE_NODE * pstTemp = gstProvContext.pstCurrCsProfInfo->pPrev;
		while(pstTemp != NULL)
		{
			if(pstTemp->eCSProfileType == E_CS_DATACONN_PROFILE )
			{
				//compare contents 
				if(prv_CmpCSDataConnProfiles((ST_CS_DATACONN_PROFILE *)(pstTemp->pProfile), 
					(ST_CS_DATACONN_PROFILE *)(gstProvContext.pstCurrCsProfInfo->pProfile)) == E_TRUE &&
					pstTemp->IsProfileDiscarded == E_FALSE)
				{
					mmi_trace(1, "JPRV_LOG: skiping redundant profile");
					gstProvContext.pstCurrCsProfInfo = gstProvContext.pstCurrCsProfInfo->pNext;
					break;
				}
					
			}
			pstTemp = pstTemp->pPrev;
		}
	}
}


JC_BOOLEAN prv_CmpCSDataConnProfiles(ST_CS_DATACONN_PROFILE *pstProf1 , ST_CS_DATACONN_PROFILE *pstProf2)
{
 	JC_BOOLEAN bIsSame = E_FALSE;

	mmi_trace(1, "JPRV_LOG: prv_CmpCSDataConnProfiles");
	mmi_trace(1,"JPRV_LOG:pstProf1->nap_id[%s] pstProf2->nap_id[%s]", pstProf1->nap_id,pstProf2->nap_id);
	mmi_trace(1,"JPRV_LOG:pstProf1->apn[%s] pstProf2->apn[%s]", pstProf1->apn,pstProf2->apn);
	mmi_trace(1,"JPRV_LOG:pstProf1->username[%s] pstProf2->username[%s]", pstProf1->username,pstProf2->username);	
	mmi_trace(1,"JPRV_LOG:pstProf1->password[%s] pstProf2->password[%s]", pstProf1->password,pstProf2->password);
	mmi_trace(1,"JPRV_LOG:pstProf1->bear_type[%d] pstProf2->bear_type[%d]", pstProf1->bear_type,pstProf2->bear_type);
	mmi_trace(1,"JPRV_LOG:pstProf1->gprs_auth_type[%d] pstProf2->gprs_auth_type[%d]", pstProf1->gprs_auth_type,pstProf2->gprs_auth_type);
		
	if( !jc_strcmp(pstProf1->nap_id,pstProf2->nap_id) && 
		!jc_strcmp(pstProf1->apn,pstProf2->apn) && 
		!jc_strcmp(pstProf1->username,pstProf2->username) && 
		!jc_strcmp(pstProf1->password,pstProf2->password) &&
		 pstProf1->bear_type == pstProf2->bear_type &&
		 pstProf1->gprs_auth_type == pstProf2->gprs_auth_type)
	{
	   	bIsSame = E_TRUE;
	}
	mmi_trace(1, "JPRV_LOG: prv_CmpCSDataConnProfiles %d", bIsSame);
	return 	bIsSame;

}


void prv_ShowNextCSProfile()
{
	mmi_trace(1,"JPRV_LOG: prv_ShowNextCSProfile()");
	gstProvContext.pstCurrCsProfInfo = gstProvContext.pstCurrCsProfInfo->pNext;
	mmi_trace(1,"JPRV_LOG:gstProvContext.pstCurrCsProfInfo %d",gstProvContext.pstCurrCsProfInfo);
	//prv_GetNextNewCSProfile();
	prv_SkipRedudantCSDataConnProfile();
	mmi_trace(1,"JPRV_LOG:gstProvContext.pstCurrCsProfInfo %d",gstProvContext.pstCurrCsProfInfo);
	if(gstProvContext.pstCurrCsProfInfo == NULL)
	{

		prv_MMIDisplayIDLEScreen();
	}
	else
	{
		prv_MMIDisplayProvProfileDetails();
	}
}




void prv_FormCSProxyProfDeatilsBuff()
{
	JC_INT8 pcTempStr[256] ={0,};

	ST_CS_PROXY_PROFILE *pstProxyProf = JC_NULL;
	
	mmi_trace(1, "JPRV_LOG:prv_FormCSProxyProfDeatilsBuff Invoked");

	pstProxyProf = (ST_CS_PROXY_PROFILE *)(gstProvContext.pstCurrCsProfInfo->pProfile);
	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	mmi_trace(1,"JPRV_LOG:pstProxyProf->profile_type %d",pstProxyProf->profile_type);
	if(pstProxyProf->profile_type == E_PROF_BROWSER)
	{
	   // browser profile
	   jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_BROW_PROFILE));
	}
	else if(pstProxyProf->profile_type == E_PROF_MMS)
	{
	   // mms profile
	   jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_MMS_PROFILE));
	}
	else
	{
		mmi_trace(1,"JPRV_LOG: unknown profile");
	}
	jdi_CUtilsTcsStrCat("\n----------\n", gstProvContext.szScreeBuff) ;	
	

	if(pstProxyProf->nap_id[0]!='\0')
	{
		JC_CHAR  * pmDest = JC_NULL ;
		//jdi_CUtilsTcsStrCat("\nNAPID:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_NAP_ID)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;	
		//jdi_CUtilsTcsStrCat(pstProxyProf->nap_id,gstProvContext.szScreeBuff);
		jdi_CUtilsUTF8ToTchar(JC_NULL,pstProxyProf->nap_id,&pmDest);
		if(pmDest)
		{
			jc_wstrcat(gstProvContext.szScreeBuff,pmDest );
			jdd_MemFree(pmDest);
		}
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}

	if(pstProxyProf->profile_type == E_PROF_BROWSER && pstProxyProf->homepage[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nHome Page:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_HOMEPAGE)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;	
		
		jdi_CUtilsTcsStrCat(pstProxyProf->homepage,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	else if(pstProxyProf->profile_type == E_PROF_MMS && pstProxyProf->homepage[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nMMSC URL:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat( gstProvContext.szScreeBuff,(JC_CHAR*)GetString( STR_ID_PROV_MMSC_URL)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;

		
		jdi_CUtilsTcsStrCat(pstProxyProf->homepage,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	
	}
	
	if(pstProxyProf->proxy_id[0]!='\0')
	{
		JC_CHAR * pmDest = JC_NULL;
		//jdi_CUtilsTcsStrCat("\nProxyID:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_PROFILE_ID)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		//jdi_CUtilsTcsStrCat(pstProxyProf->proxy_id,gstProvContext.szScreeBuff);
		jdi_CUtilsUTF8ToTchar(JC_NULL,pstProxyProf->proxy_id,&pmDest);
		if(pmDest)
		{
			jc_wstrcat(gstProvContext.szScreeBuff,pmDest );
			jdd_MemFree(pmDest);
		}
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	if(pstProxyProf->proxy_ip[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nProxy IP:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_PROXYIP)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		
		jdi_CUtilsTcsStrCat(pstProxyProf->proxy_ip,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	if(pstProxyProf->port)
	{
		jc_memset(pcTempStr,0,256);
		jc_itoa(pstProxyProf->port,pcTempStr,10 );
		//jdi_CUtilsTcsStrCat("\nPort:", gstProvContext.szScreeBuff) ;	


		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff,(JC_CHAR*)GetString( STR_ID_PROV_PORT)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;

		
		jdi_CUtilsTcsStrCat(pcTempStr,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	if(pstProxyProf->dns1[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nDNS1:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_DNS1)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		
		jdi_CUtilsTcsStrCat(pstProxyProf->dns1,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	
	if(pstProxyProf->dns2[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nDNS2:", gstProvContext.szScreeBuff) ;
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_DNS2)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		
		jdi_CUtilsTcsStrCat(pstProxyProf->dns2,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	
}
void prv_FormCSDataConnProfDeatilsBuff()
{

	ST_CS_DATACONN_PROFILE *pstDataConn = (ST_CS_DATACONN_PROFILE *)(gstProvContext.pstCurrCsProfInfo->pProfile);
	mmi_trace(1, "JPRV_LOG:prv_FormCSDataConnProfDeatilsBuff Invoked");

	jc_memset(gstProvContext.szScreeBuff, 0, PROV_MAX_SCREEN_BUFF*sizeof(JC_CHAR));
	
	//jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_MMS_PROFILE));
	jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString(STR_ID_PROV_DATA_ACCOUNT));
	
	
	jdi_CUtilsTcsStrCat("\n----------\n", gstProvContext.szScreeBuff) ;	
	


	if(pstDataConn->apn[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nAPN:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_APN)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		jdi_CUtilsTcsStrCat(pstDataConn->apn,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	
	if(pstDataConn->nap_id[0]!='\0')
	{
		JC_CHAR  * pmDest = JC_NULL ;
		//jdi_CUtilsTcsStrCat("\nNAPID:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_NAP_ID)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		//jdi_CUtilsTcsStrCat(pstDataConn->nap_id,gstProvContext.szScreeBuff);
		jdi_CUtilsUTF8ToTchar(JC_NULL,pstDataConn->nap_id,&pmDest);
		if(pmDest)
		{
			jc_wstrcat(gstProvContext.szScreeBuff,pmDest );
			jdd_MemFree(pmDest);
		}
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	if(pstDataConn->username[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nUserName:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff, (JC_CHAR*)GetString( STR_ID_PROV_USERNAME)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		
		jdi_CUtilsTcsStrCat(pstDataConn->username,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}
	
	if(pstDataConn->password[0]!='\0')
	{
		//jdi_CUtilsTcsStrCat("\nPassWord:", gstProvContext.szScreeBuff) ;	
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
		jc_wstrcat(gstProvContext.szScreeBuff,(JC_CHAR*)GetString( STR_ID_PROV_PASSWORD)) ;	
		jdi_CUtilsTcsStrCat(":", gstProvContext.szScreeBuff) ;
		
		jdi_CUtilsTcsStrCat(pstDataConn->password,gstProvContext.szScreeBuff);
		jdi_CUtilsTcsStrCat("\n", gstProvContext.szScreeBuff) ;	
	}

}

JC_BOOLEAN prv_IsProvPushAvail()
{
	mmi_trace(12,"prv_IsProvPushAvail %d",gstProvContext.pstProvInfo);
	
	if(gstProvContext.pstProvInfo)
		return E_TRUE;
	else
		return E_FALSE;
}


JC_HANDLE  prv_getCommHandle ()
{
	return gstProvContext.vCommHandle ;
}



JC_BOOLEAN cs_GetDataConnList(nvram_ef_abm_gprs_profile_struct* gprs_account_total_p)
{
	S16 error = 0;
	ReadRecord(NVRAM_EF_ABM_GPRS_PROFILE_LID, 1, gprs_account_total_p, 
								sizeof(nvram_ef_abm_gprs_profile_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, 
								&error);

	if (NVRAM_READ_SUCCESS == error)
		return E_TRUE;
	return E_FALSE;
}

JC_BOOLEAN cs_SetDataConnList(nvram_ef_abm_gprs_profile_struct* gprs_account_total_p)
{
	S16 error = 0;
	WriteRecord(NVRAM_EF_ABM_GPRS_PROFILE_LID, 1, gprs_account_total_p, 
								sizeof(nvram_ef_abm_gprs_profile_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, 
								&error);

	if (NVRAM_WRITE_SUCCESS == error)
		return E_TRUE;
	return E_FALSE;
}

JC_INT8 cs_GetValidDataConnIndex()
{
	nvram_ef_abm_gprs_profile_struct gprs_account_total[MAX_DATA_ACCOUNT_GPRS_LIMIT];
	int i = 0;
	if (!cs_GetDataConnList(gprs_account_total))
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s get data conn profile error", __FUNCTION__);
		return -1;
	}
	for (i=0; i < MAX_DATA_ACCOUNT_GPRS_LIMIT; i++)
	{
		mmi_trace(TRUE," Func: %s name[%d]:%s, leng: %d", __FUNCTION__,i, gprs_account_total[i].name, strlen(gprs_account_total[i].name));
		
		if ((gprs_account_total[i].name_dcs == DATA_ACCOUNT_ASCII) && (strlen(gprs_account_total[i].name) == 0) )
		{
			return i;
		}
		else if ((gprs_account_total[i].name_dcs == DATA_ACCOUNT_UCS) && (UCS2Strlen(gprs_account_total[i].name) == 0) )
		{
			return i;
		}
	}
	return -1;
}

extern void GetAccountId(U8 dataconn_id, U8* data_account);
JC_INT8 cs_GetDataConnID(INT8 * nap_id)
{
	int i = 0;
	nvram_ef_abm_gprs_profile_struct *gprs_account_total;
	
	gprs_account_total =  (nvram_ef_abm_gprs_profile_struct*)jdd_MemAlloc(sizeof(nvram_ef_abm_gprs_profile_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, 1);
	if (gprs_account_total == JC_NULL)
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s No enough memory error", __FUNCTION__);
		return -1;
	}
	memset(gprs_account_total, 0, sizeof(nvram_ef_abm_gprs_profile_struct)*MAX_DATA_ACCOUNT_GPRS_LIMIT);
	if(!cs_GetDataConnList(gprs_account_total))
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s Read Data conn list error", __FUNCTION__);
		jdd_MemFree(gprs_account_total);
		return -1;
	}

	for (i=0; i < MAX_DATA_ACCOUNT_GPRS_LIMIT; i++)
	{
		mmi_trace(TRUE," Func: %s name[%d]:%s, leng: %d", __FUNCTION__,i, gprs_account_total[i].name, strlen(gprs_account_total[i].name));

		if (gprs_account_total[i].name_dcs == DATA_ACCOUNT_ASCII)
		{
			if (strcmp(gprs_account_total[i].name,nap_id) == 0)
			{
				jdd_MemFree(gprs_account_total);
				return i;
			}
		}
		else if (gprs_account_total[i].name_dcs == DATA_ACCOUNT_UCS )
		{
			INT8 nap[(E_DC_MAX_NAPID_LEN+1)*ENCODING_LENGTH];
			AnsiiToUnicodeString(nap, nap_id);
			if (UCS2Strcmp((S8*)gprs_account_total[i].name, (S8*)nap) == 0)
			{
				jdd_MemFree(gprs_account_total);
				return i;
			}
		}
	}
	jdd_MemFree(gprs_account_total);
	return -1;
}
//Interface to check if there exists any data connection profile with same settings.
//It firts matches the nap_id , if it matches matches rest of fileds. 
//True - if all fields are macthed
//False- if all fields did not match
JC_BOOLEAN cs_DataConnProfileExist(ST_CS_DATACONN_PROFILE *pstDataConn, JC_UINT8 *pIndex)
{
	JC_INT8 index;

	index = cs_GetDataConnID(pstDataConn->nap_id);
	if (index != -1)
	{
		mmi_trace(TRUE,"Jdd_LOG Func: %s index:%d", __FUNCTION__, index);
		*pIndex = index;
		return E_TRUE;	}
	else
	{
		return E_FALSE;
	}

}

JC_RETCODE cs_SaveDataConnProfile(ST_CS_DATACONN_PROFILE *pstProxyProfile, INT16 index)
{
	nvram_ef_abm_gprs_profile_struct* gprs_account;
	nvram_ef_abm_gprs_profile_struct* gprs_account_total;

	mmi_trace(TRUE,"JDD_LOG Func: %s \n\t apn: %s", __FUNCTION__, pstProxyProfile->apn);
	mmi_trace(TRUE,"\t nap_id: %s", pstProxyProfile->nap_id);
	mmi_trace(TRUE,"\t username: %s \n\t password: %s", pstProxyProfile->username, pstProxyProfile->password);
	mmi_trace(TRUE,"\t bear_type: %d \n\t gprs_auth_type: %d", pstProxyProfile->bear_type, pstProxyProfile->gprs_auth_type);

	gprs_account = (nvram_ef_abm_gprs_profile_struct*)jdd_MemAlloc(sizeof(nvram_ef_abm_gprs_profile_struct), 1);
	if (gprs_account == JC_NULL)
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s No Enough memory", __FUNCTION__);	
		return 	JC_ERR_MEMORY_ALLOCATION;
	}
	memset(gprs_account, 0, sizeof(nvram_ef_abm_gprs_profile_struct));
	if (0xFF == index)
	{
		if (!cs_DataConnProfileExist(pstProxyProfile, &(gprs_account->context_id))) 
		{
			index = cs_GetValidDataConnIndex();
			if (index != -1)
				gprs_account->context_id = index;
			else
			{
				mmi_trace(TRUE,"JDD_LOG: Func: %s No valid data conn index error", __FUNCTION__);
				jdd_MemFree(gprs_account);
				return 	JC_ERR_FILE_WRITE;
			}

		}
		gprs_account->context_id += 1;
	}
	else
	{
		gprs_account->context_id = index + 1;
	}
	gprs_account->apn_length = strlen(pstProxyProfile->apn);
	if (gprs_account->apn_length <= MAX_GPRS_APN_LEN)
		memcpy(gprs_account->apn , pstProxyProfile->apn, gprs_account->apn_length);
	else
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s APN Length error", __FUNCTION__);
	}

	memcpy(gprs_account->user_name, pstProxyProfile->username, MAX_GPRS_USER_NAME_LEN);
	memcpy(gprs_account->password, pstProxyProfile->password, MAX_GPRS_PASSWORD_LEN);

	gprs_account->authentication_type = pstProxyProfile->gprs_auth_type;

	gprs_account->name_dcs = DATA_ACCOUNT_ASCII;
	gprs_account->name_length = strlen(pstProxyProfile->nap_id);
	strcpy(gprs_account->name, pstProxyProfile->nap_id);

	mmi_trace(TRUE,"jdd_log Func: %s name_length:%d, name:%s", __FUNCTION__, gprs_account->name_length, gprs_account->name);
	
	gprs_account_total = (nvram_ef_abm_gprs_profile_struct*)jdd_MemAlloc(sizeof(nvram_ef_abm_gprs_profile_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, 1);
	if (gprs_account_total == JC_NULL)
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s No Enough memory", __FUNCTION__);	
		jdd_MemFree(gprs_account);
		return 	JC_ERR_MEMORY_ALLOCATION;
	}

	memset(gprs_account_total, 0, sizeof(nvram_ef_abm_gprs_profile_struct)*MAX_DATA_ACCOUNT_GPRS_LIMIT);

	if (!cs_GetDataConnList(gprs_account_total))
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s get data conn profile error", __FUNCTION__);
		jdd_MemFree(gprs_account);
		jdd_MemFree(gprs_account_total);
		return JC_ERR_FILE_READ;
	}

	mmi_trace(TRUE,"JDD_LOG Func: %s gprs_account->context_id:%d", __FUNCTION__, gprs_account->context_id);
	memcpy(&gprs_account_total[gprs_account->context_id-1], gprs_account, sizeof(nvram_ef_abm_gprs_profile_struct));

	if (!cs_SetDataConnList(gprs_account_total))
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s save data conn profile error", __FUNCTION__);
		jdd_MemFree(gprs_account);
		jdd_MemFree(gprs_account_total);
		return JC_ERR_FILE_WRITE;
	}

	jdd_MemFree(gprs_account);
	jdd_MemFree(gprs_account_total);


  return JC_OK;
}

JC_RETCODE cs_ReplaceDataConnProfile(ST_CS_DATACONN_PROFILE *pstProxyProfile, INT8 index)
{
	return cs_SaveDataConnProfile(pstProxyProfile, index);
}


//gets the list of the data connection's napd id .
 
JC_RETCODE cs_GetDataConnProfileList(ST_CS_DATACONN_PROFILE_NAME* pszDataConnList)
{
	int i = 0;
	nvram_ef_abm_gprs_profile_struct *gprs_account_total;
	
	gprs_account_total =  (nvram_ef_abm_gprs_profile_struct*)jdd_MemAlloc(sizeof(nvram_ef_abm_gprs_profile_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, 1);
	if (gprs_account_total == JC_NULL)
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s No enough memory error", __FUNCTION__);
		return JC_ERR_MEMORY_ALLOCATION;
	}
	memset(gprs_account_total, 0, sizeof(nvram_ef_abm_gprs_profile_struct)*MAX_DATA_ACCOUNT_GPRS_LIMIT);
	if(!cs_GetDataConnList(gprs_account_total))
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s Read Data conn list error", __FUNCTION__);
		jdd_MemFree(gprs_account_total);
		return JC_ERR_FILE_READ;
	}

	for (i=0; i < MAX_DATA_ACCOUNT_GPRS_LIMIT; i++)
	{
		mmi_trace(TRUE," Func: %s name[%d]:%s, leng: %d", __FUNCTION__,i, gprs_account_total[i].name, strlen(gprs_account_total[i].name));

		if (gprs_account_total[i].name_dcs == DATA_ACCOUNT_ASCII)
		{
			INT8 nap[(E_DC_MAX_NAPID_LEN+1)*ENCODING_LENGTH] = {0};
			AnsiiNToUnicodeString(nap, gprs_account_total[i].name, E_DC_MAX_NAPID_LEN);
			memcpy(pszDataConnList[i].nap_id, nap, E_DC_MAX_NAPID_LEN*ENCODING_LENGTH);
		}
		else if (gprs_account_total[i].name_dcs == DATA_ACCOUNT_UCS )
		{
			memcpy(pszDataConnList[i].nap_id, gprs_account_total[i].name, E_DC_MAX_NAPID_LEN*ENCODING_LENGTH);
		}
	}
	jdd_MemFree(gprs_account_total);
	return JC_OK;
}

JC_BOOLEAN cs_GetMMSProfileNameList(nvram_mms_profile_name_array_struct* ph_mms_prof_name_arr_p, CFW_SIM_ID nSimID)
{
	S16 error = 0;
#ifdef __MMI_MULTI_SIM__
	    ReadRecord(
	        NVRAM_EF_MMS_PROFILE_NAMES_LID + nSimID,
	        1,
	        ph_mms_prof_name_arr_p,
	        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
	        &error);
#else
	ReadRecord(
	    NVRAM_EF_MMS_PROFILE_NAMES_LID,
	    1,
	    ph_mms_prof_name_arr_p,
	    NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
	    &error);
#endif	

	if (NVRAM_READ_SUCCESS == error)
		return E_TRUE;
	return E_FALSE;
}

JC_BOOLEAN cs_SetMMSProfileNameList(nvram_mms_profile_name_array_struct* ph_mms_prof_name_arr_p, CFW_SIM_ID nSimID)
{
	S16 error = 0;
#ifdef __MMI_MULTI_SIM__
	    WriteRecord(
	        NVRAM_EF_MMS_PROFILE_NAMES_LID + nSimID,
	        1,
	        ph_mms_prof_name_arr_p,
	        NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
	        &error);
	
#else
	WriteRecord(
	    NVRAM_EF_MMS_PROFILE_NAMES_LID,
	    1,
	    ph_mms_prof_name_arr_p,
	    NVRAM_EF_MMS_PROFILE_NAMES_SIZE,
	    &error);
#endif	

	if (NVRAM_WRITE_SUCCESS == error)
		return E_TRUE;
	return E_FALSE;
}

JC_BOOLEAN cs_GetWAPProfileNameList(nvram_wap_profile_name_array_struct* ph_wap_prof_name_arr_p, CFW_SIM_ID nSimID)
{
	S16 error = 0;
#ifdef __MMI_MULTI_SIM__
	    ReadRecord(
	        NVRAM_EF_WAP_PROFILE_NAMES_LID + nSimID,
	        1,
	        ph_wap_prof_name_arr_p,
	        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
	        &error);
#else
	ReadRecord(
	    NVRAM_EF_WAP_PROFILE_NAMES_LID,
	    1,
	    ph_wap_prof_name_arr_p,
	    NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
	    &error);
#endif	

	if (NVRAM_READ_SUCCESS == error)
		return E_TRUE;
	return E_FALSE;
}

JC_BOOLEAN cs_SetWAPProfileNameList(nvram_wap_profile_name_array_struct* ph_wap_prof_name_arr_p, CFW_SIM_ID nSimID)
{
	S16 error = 0;
#ifdef __MMI_MULTI_SIM__
	    WriteRecord(
	        NVRAM_EF_WAP_PROFILE_NAMES_LID + nSimID,
	        1,
	        ph_wap_prof_name_arr_p,
	        NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
	        &error);
#else
	WriteRecord(
	    NVRAM_EF_WAP_PROFILE_NAMES_LID,
	    1,
	    ph_wap_prof_name_arr_p,
	    NVRAM_EF_WAP_PROFILE_NAMES_SIZE,
	    &error);
#endif	

	if (NVRAM_WRITE_SUCCESS == error)
		return E_TRUE;
	return E_FALSE;
}

JC_BOOLEAN cs_SetWAPProfile(nvram_wap_profile_content_struct* wap_profile_p, CFW_SIM_ID nSimID, U8 index)
{
	S16 error = 0;
	mmi_trace(TRUE,"JDD_LOG Func: %s proxy_port:%d, homepage_url:%s ", __FUNCTION__, wap_profile_p->proxy_port, wap_profile_p->homepage_url);
	mmi_trace(TRUE,"JDD_LOG Func: %s data_account:%d, conn_type:%d ", __FUNCTION__, wap_profile_p->data_account, wap_profile_p->conn_type);
	mmi_trace(TRUE,"JDD_LOG Func: %s proxy_ip:[%d.%d.%d.%d] ", __FUNCTION__, wap_profile_p->proxy_ip[0], wap_profile_p->proxy_ip[1], wap_profile_p->proxy_ip[2], wap_profile_p->proxy_ip[3]);
	mmi_trace(TRUE,"JDD_LOG Func: %s username:%s, password:%s ", __FUNCTION__, wap_profile_p->username, wap_profile_p->password);

#ifdef __MMI_MULTI_SIM__
	    WriteRecord(
	        NVRAM_EF_WAP_PROFILE_CONTENT_LID + nSimID,
	        (U16) (index + 1),
	        wap_profile_p,
	        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
	        &error);
#else

    WriteRecord(
        NVRAM_EF_WAP_PROFILE_CONTENT_LID,
        (U16) (index + 1),
        wap_profile_p,
        NVRAM_EF_WAP_PROFILE_CONTENT_SIZE,
        &error);
#endif

	if (NVRAM_WRITE_SUCCESS == error)
		return E_TRUE;
	return E_FALSE;
}

JC_BOOLEAN cs_SetMMSProfile(nvram_mms_profile_content_struct* mms_profile_p, CFW_SIM_ID nSimID, U8 index)
{
	S16 error = 0;
	mmi_trace(TRUE,"JDD_LOG Func: %s proxy_port:%d, mmsc_url:%s ", __FUNCTION__, mms_profile_p->proxy_port, mms_profile_p->mmsc_url);
	mmi_trace(TRUE,"JDD_LOG Func: %s data_account:%d, conn_type:%d ", __FUNCTION__, mms_profile_p->data_account, mms_profile_p->conn_type);
	mmi_trace(TRUE,"JDD_LOG Func: %s proxy_ip:[%d.%d.%d.%d] ", __FUNCTION__, mms_profile_p->proxy_ip[0], mms_profile_p->proxy_ip[1], mms_profile_p->proxy_ip[2], mms_profile_p->proxy_ip[3]);
	mmi_trace(TRUE,"JDD_LOG Func: %s username:%s, password:%s ", __FUNCTION__, mms_profile_p->username, mms_profile_p->password);
	
#ifdef __MMI_MULTI_SIM__
	    WriteRecord(
	        NVRAM_EF_MMS_PROFILE_CONTENT_LID + nSimID,
	        (U16) (index + 1),
	        mms_profile_p,
	        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
	        &error);

#else

    WriteRecord(
        NVRAM_EF_MMS_PROFILE_CONTENT_LID,
        (U16) (index + 1),
        mms_profile_p,
        NVRAM_EF_MMS_PROFILE_CONTENT_SIZE,
        &error);
#endif

	if (NVRAM_WRITE_SUCCESS == error)
		return E_TRUE;
	return E_FALSE;
}

//Interface to save/update the proxy profile .
//Updation is done if the proxy_id matches and other fields differ
static S32 utf8_to_ucs2 (U16 *ucs2, U8 *utf8)
{
	unsigned char c = utf8[0];

	if (c < 0x80) 
	{
		*ucs2 = c;
		return 1;
	} 
	else if (c < 0xe0)
	{
		*ucs2 = ((U16) (c & 0x1f) << 6) | (U16) (utf8[1] ^ 0x80);
		return 2;
	} 
	else
	{
	    *ucs2 =	  ((U16) (c & 0x0f) << 12)
				| ((U16) (utf8[1] ^ 0x80) << 6)
				|  (U16) (utf8[2] ^ 0x80);
		return 3;
	}
}
static S32 utf8_to_ucs2_string(U8 *dest,int dest_size,U8* src)
{
	int pos = 0;
	
	// FIXME	1. we should check the sanity of the UTF8 string 
	// 			2. this check is not correct when dest_size < 2 or dest_size is odd
	while(*src)
	{
		src += utf8_to_ucs2((U16*)(dest+pos),src);
		pos += 2;
		if(pos>=dest_size-2) break;
	}
	dest[pos] = 0;
	dest[pos+1] = 0;
	return pos+2;
}

//Interface to check if there exists any profile with same settings.
//It firts matches the proxy_id , if it matches matches rest of fileds. 
//True - if all fields are macthed
//False- if all fields did not match
JC_BOOLEAN cs_ProxyProfileExist(ST_CS_PROXY_PROFILE *pstProxyProfile, JC_INT8* pIndex)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	int i = 0;


	mmi_trace(TRUE,"Jdd_log: Func: %s profile_type:%d, SIM:%d", __FUNCTION__, pstProxyProfile->profile_type, pstProxyProfile->nSimID);
	if (pstProxyProfile->profile_type == E_PROF_BROWSER)
	{
		nvram_wap_profile_name_array_struct ph_wap_prof_name_arr;
	    memset(&ph_wap_prof_name_arr, 0, sizeof(ph_wap_prof_name_arr));
		
		if (!cs_GetWAPProfileNameList(&ph_wap_prof_name_arr, pstProxyProfile->nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read WAP prolfile name list error", __FUNCTION__);
			return E_FALSE;
		}

		for (i=0; i < NVRAM_WAP_PROFILE_MAX; i++)
		{
			mmi_trace(TRUE,"jdd_log Func: %s profile_name[%d]:%s, dcs:%d", __FUNCTION__, i, ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name, ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs);
			
			if (ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs == PH_DCS_ASCII)
			{
				if (strcmp(pstProxyProfile->proxy_id, ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name) == 0)
				{
					mmi_trace(TRUE,"JDD_LOG: Func: %s index:%d", __FUNCTION__, i);
					*pIndex = i;
					return E_TRUE;
				}
			}
			else if (ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs == PH_DCS_UCS)
			{
				JC_CHAR    proxy_id[E_DC_MAX_PROXYID_LEN  + ENCODING_LENGTH];
				//AnsiiToUnicodeString(proxy_id, pstProxyProfile->proxy_id);
                           utf8_to_ucs2_string((U8*)proxy_id,E_DC_MAX_PROXYID_LEN *2,(U8 *)pstProxyProfile->proxy_id);
				if (UCS2Strcmp((S8*)proxy_id, (S8*)ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name) == 0)
				{
					mmi_trace(TRUE,"JDD_LOG: Func: %s index:%d", __FUNCTION__, i);
					*pIndex = i;
					return E_TRUE;
				}
			}
		}
	}
	else if (pstProxyProfile->profile_type == E_PROF_MMS)
	{
		nvram_mms_profile_name_array_struct ph_mms_prof_name_arr;
	    memset(&ph_mms_prof_name_arr, 0, sizeof(ph_mms_prof_name_arr));
		
		if (!cs_GetMMSProfileNameList(&ph_mms_prof_name_arr, pstProxyProfile->nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read MMS prolfile name list error", __FUNCTION__);
			return E_FALSE;
		}

		for (i=0; i < NVRAM_MMS_PROFILE_MAX; i++)
		{
			if (ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].dcs == PH_DCS_ASCII)
			{
				if (strcmp(pstProxyProfile->proxy_id, ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name) == 0)
				{
					mmi_trace(TRUE,"JDD_LOG: Func: %s index:%d", __FUNCTION__, i);
					*pIndex = i;
					return E_TRUE;
				}
			}
			else if (ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].dcs == PH_DCS_UCS)
			{
				JC_CHAR    proxy_id[E_DC_MAX_PROXYID_LEN  + ENCODING_LENGTH];
			//	AnsiiToUnicodeString(proxy_id, pstProxyProfile->proxy_id);
                           utf8_to_ucs2_string((U8*)proxy_id,E_DC_MAX_PROXYID_LEN *2,(U8*)pstProxyProfile->proxy_id);
				if (UCS2Strcmp((S8*)proxy_id, (S8*)ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name) == 0)
				{
					mmi_trace(TRUE,"JDD_LOG: Func: %s index:%d", __FUNCTION__, i);
					*pIndex = i;
					return E_TRUE;
				}
			}
		}
	}
   	return E_FALSE;
}


JC_INT8 cs_GetValidProfileIndex(E_PROV_PROFILE_TYPE  eProfileType, CFW_SIM_ID nSimID)
{
	if (eProfileType == E_PROF_BROWSER)
	{
		int i = 0;
		nvram_wap_profile_name_array_struct ph_wap_prof_name_arr;
	    memset(&ph_wap_prof_name_arr, 0, sizeof(ph_wap_prof_name_arr));
		
		if (!cs_GetWAPProfileNameList(&ph_wap_prof_name_arr, nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read WAP prolfile name list error", __FUNCTION__);
			return -1;
		}

		for (i=0; i < NVRAM_WAP_PROFILE_MAX; i++)
		{
			if ((ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs == PH_DCS_ASCII) && (strlen(ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name) == 0) )
			{
				return i;
			}
			else if ((ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs == PH_DCS_UCS) && (UCS2Strlen(ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name) == 0) )
			{
				return i;
			}
		}
	}
	else if (eProfileType == E_PROF_MMS)
	{
		int i = 0;
		nvram_mms_profile_name_array_struct ph_mms_prof_name_arr;
	    memset(&ph_mms_prof_name_arr, 0, sizeof(ph_mms_prof_name_arr));
		
		if (!cs_GetMMSProfileNameList(&ph_mms_prof_name_arr, nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read WAP prolfile name list error", __FUNCTION__);
			return -1;
		}

		for (i=0; i < NVRAM_MMS_PROFILE_MAX; i++)
		{
			if ((ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].dcs == PH_DCS_ASCII) && (strlen(ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name) == 0) )
			{
				return i;
			}
			else if ((ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].dcs == PH_DCS_UCS) && (UCS2Strlen(ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name) == 0) )
			{
				return i;
			}
		}
	}
	return -1;
}
void cs_ChangeMode2CSDType(E_DC_CONN_MODE conn_mode, U8 *conn_type)
{
	switch(conn_mode)	
	{
		case E_DC_CONNECTION_ORIENTED:
		{
			*conn_type =  PH_CONN_TYPE_CONNECTION_OREINTED;
			break;
		}
		case E_DC_CONNECTION_ORIENTED_SECURE:
		{
			*conn_type =  PH_CONN_TYPE_CONNECTION_OREINTED_SECURE;
			break;
		}
		case E_DC_WPHTTP_PROXY:
		case E_DC_WPHTTP_DIRECT:
		{
			*conn_type =  PH_CONN_TYPE_HTTP;
			break;
		}
		default:
		{
			mmi_trace(TRUE,"jdd_log Func: %s ERROR conn_mode: %d", __FUNCTION__, conn_mode);
			*conn_type =  PH_CONN_TYPE_CONNECTION_OREINTED;
			break;
		}
		

	}
}

void cs_ChangeIpMode(U8* pCsIp, INT8* pOriginIp)
{
	int i = 0, j = 0, k = 0;
	INT8 tmp[4]={0};

	for (i=0; i<= E_DC_MAX_IP_LEN; i++)
	{ 
		if(pOriginIp[i] != '.' && pOriginIp[i] != 0x00)
		{
			tmp[j++] = pOriginIp[i];
		} 
		else
		{
			pCsIp[k++] = atoi(tmp);
			memset(tmp, 0, sizeof(tmp));	
			j = 0;	
			if(pOriginIp[i] == 0x00)
			{
				break;
			}
		}
	}
	
	
}

void cs_ActiveProfile(U8 profile_module, CFW_SIM_ID nSimID, U8 profile_index)
{
	S16 error;

#ifdef __MMI_MULTI_SIM__
    if (profile_module == PH_BROWSER_PROFILE)
    {
            WriteValue(NVRAM_PH_ACTIVATED_WAP_PROFILE_INDEX + nSimID, &profile_index, DS_BYTE, &error);
    }
    else if (profile_module == PH_MMS_PROFILE)
    {
            WriteValue(NVRAM_PH_ACTIVATED_MMS_PROFILE_INDEX + nSimID, &profile_index, DS_BYTE, &error);
    }
#else
    if (profile_module == PH_BROWSER_PROFILE)
    {
        WriteValue(NVRAM_PH_ACTIVATED_WAP_PROFILE_INDEX, &profile_index, DS_BYTE, &error);

    }
    else if (profile_module == PH_MMS_PROFILE)
    {
        WriteValue(NVRAM_PH_ACTIVATED_MMS_PROFILE_INDEX, &profile_index, DS_BYTE, &error);
    }
#endif	

}


JC_RETCODE cs_SaveProxyProfile(ST_CS_PROXY_PROFILE *pstProxyProfile, INT16 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	INT8 dataconn_id = -1;
	U8 profile_type = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	mmi_trace(TRUE,"JDD_LOG Func: %s \n\t homepage: %s", __FUNCTION__, pstProxyProfile->homepage);
	mmi_trace(TRUE,"\t proxy_ip: %s", pstProxyProfile->proxy_ip);
	mmi_trace(TRUE,"\t dns1: %s \n\t dns2: %s", pstProxyProfile->dns1, pstProxyProfile->dns2);
	mmi_trace(TRUE,"\t proxy_id: %s \n\t nap_id: %s", pstProxyProfile->proxy_id, pstProxyProfile->nap_id);
	mmi_trace(TRUE,"\t port: %d \n\t conn_mode: %d", pstProxyProfile->port, pstProxyProfile->conn_mode);
	mmi_trace(TRUE,"\t profile_type: %d \n\t nSimID: %d", pstProxyProfile->profile_type, pstProxyProfile->nSimID);

	if (0xFF == index)
	{
		//get free index of profile
		if (!cs_ProxyProfileExist(pstProxyProfile, (JC_INT8*)&index))
		{
			JC_INT8  index_tmp = cs_GetValidProfileIndex(pstProxyProfile->profile_type, pstProxyProfile->nSimID);
			if (index_tmp != -1)
				index = index_tmp;
			else
			{
				mmi_trace(TRUE,"JDD_LOG: Func: %s No valid profile index error", __FUNCTION__);	
				return 	JC_ERR_FILE_WRITE;
			}

		}
	}
	mmi_trace(TRUE,"JDD_LOG Func: %s index:%d", __FUNCTION__, index);
	
	if (pstProxyProfile->profile_type == E_PROF_BROWSER)
	{
		nvram_wap_profile_content_struct wap_profile;
		nvram_wap_profile_name_array_struct ph_wap_prof_name_arr;

		memset(&wap_profile, 0, sizeof(nvram_wap_profile_content_struct));
		strncpy(wap_profile.homepage_url,pstProxyProfile->homepage, E_DC_MAX_URL_LEN);

		cs_ChangeIpMode(wap_profile.proxy_ip, pstProxyProfile->proxy_ip);
		wap_profile.proxy_port = pstProxyProfile->port;

		cs_ChangeMode2CSDType(pstProxyProfile->conn_mode, &(wap_profile.conn_type))	; 	

		dataconn_id = cs_GetDataConnID(pstProxyProfile->nap_id);
		if (dataconn_id != -1)
		{
			GetAccountId(dataconn_id, &(wap_profile.data_account));
		}
		else
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s No data conn profile error", __FUNCTION__);
			return JC_ERR_FILE_READ;	
		}
		mmi_trace(TRUE,"jdd_log: Func: %s dataconn_id:%d", __FUNCTION__, dataconn_id);

		if(!cs_GetWAPProfileNameList(&ph_wap_prof_name_arr, pstProxyProfile->nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read wap profile name list error", __FUNCTION__);
			return JC_ERR_FILE_READ;
		}

		//proxy_id
             ph_wap_prof_name_arr.nvram_wap_profile_name_array[index].dcs = PH_DCS_UCS;
		utf8_to_ucs2_string((U8*)ph_wap_prof_name_arr.nvram_wap_profile_name_array[index].profile_name, NVRAM_WAP_PROFILE_NAME_LEN-1, (U8*)pstProxyProfile->proxy_id);

		if (!cs_SetWAPProfileNameList(&ph_wap_prof_name_arr, pstProxyProfile->nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s write wap profile name list error", __FUNCTION__);
			return JC_ERR_FILE_WRITE;
		}

		if (!cs_SetWAPProfile(&wap_profile, pstProxyProfile->nSimID, index))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s save wap profile content error", __FUNCTION__);
			return JC_ERR_FILE_WRITE;
		}
	}
	else if (pstProxyProfile->profile_type == E_PROF_MMS)
	{
		nvram_mms_profile_content_struct mms_profile;
		nvram_mms_profile_name_array_struct ph_mms_prof_name_arr;

		memset(&mms_profile, 0, sizeof(nvram_mms_profile_content_struct));

		strncpy(mms_profile.mmsc_url,pstProxyProfile->homepage, E_DC_MAX_URL_LEN);
		cs_ChangeIpMode(mms_profile.proxy_ip, pstProxyProfile->proxy_ip);
		mms_profile.proxy_port = pstProxyProfile->port;

		cs_ChangeMode2CSDType(pstProxyProfile->conn_mode, &(mms_profile.conn_type))	; 	

		dataconn_id = cs_GetDataConnID(pstProxyProfile->nap_id);
		if (dataconn_id != -1)
		{
			GetAccountId(dataconn_id, &(mms_profile.data_account));
		}
		else
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s No data conn profile error", __FUNCTION__);
			return JC_ERR_FILE_READ;	
		}

		if(!cs_GetMMSProfileNameList(&ph_mms_prof_name_arr, pstProxyProfile->nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read MMS prilfile name list error", __FUNCTION__);
			return JC_ERR_FILE_READ;
		}
		#if 0
		if (strlen((S8*) pstProxyProfile->proxy_id)!= 0)
	    {

	            ph_mms_prof_name_arr.nvram_mms_profile_name_array[index].dcs = PH_DCS_ASCII;
	            strncpy(
	                (S8*) (ph_mms_prof_name_arr.nvram_mms_profile_name_array[index].profile_name),
	                (S8*) pstProxyProfile->proxy_id, E_DC_MAX_PROXYID_LEN);
	    }
		#endif
	    		//proxy_id
              ph_mms_prof_name_arr.nvram_mms_profile_name_array[index].dcs = PH_DCS_UCS;
		
		utf8_to_ucs2_string((U8*)ph_mms_prof_name_arr.nvram_mms_profile_name_array[index].profile_name, NVRAM_MMS_PROFILE_NAME_LEN-1,(U8*) pstProxyProfile->proxy_id);

		if (!cs_SetMMSProfileNameList(&ph_mms_prof_name_arr, pstProxyProfile->nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s write MMS prilfile name list error", __FUNCTION__);
			return JC_ERR_FILE_WRITE;
		}

		if (!cs_SetMMSProfile(&mms_profile, pstProxyProfile->nSimID, index))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s save MMS profile content error", __FUNCTION__);
			return JC_ERR_FILE_WRITE;
		}
	}
	else
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s unknown error pstProxyProfile->profile_type:%d", __FUNCTION__, pstProxyProfile->profile_type);
		return JC_ERR_FILE_WRITE;
	}


	switch(pstProxyProfile->profile_type)
	{
		case E_PROF_BROWSER:
		{
			profile_type = PH_BROWSER_PROFILE;
			break;
		}
		case E_PROF_MMS:
		{
			profile_type = PH_MMS_PROFILE;
			break;
		}
		default:
		{
			mmi_trace(TRUE,"jdd_log Func: %s ERROR profile_type:%d", __FUNCTION__, pstProxyProfile->profile_type);			
		}
		
	}
	cs_ActiveProfile(profile_type, pstProxyProfile->nSimID, index);
	return JC_OK;

}



//gets the list of the proxy profile's  proxy id .
 
JC_RETCODE cs_GetProxyProfileList(ST_CS_PROXY_PROFILE_NAME* pszProxyProfileList, E_PROV_PROFILE_TYPE  profile_type, CFW_SIM_ID nSimID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	int i = 0;

	mmi_trace(TRUE,"Jdd_log: Func: %s profile_type:%d, SIM:%d", __FUNCTION__, profile_type, nSimID);
	if (profile_type == E_PROF_BROWSER)
	{
		nvram_wap_profile_name_array_struct ph_wap_prof_name_arr;
	    memset(&ph_wap_prof_name_arr, 0, sizeof(ph_wap_prof_name_arr));
		
		if (!cs_GetWAPProfileNameList(&ph_wap_prof_name_arr, nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read WAP prolfile name list error", __FUNCTION__);
			return JC_ERR_FILE_READ;
		}

		for (i=0; i < NVRAM_WAP_PROFILE_MAX; i++)
		{
			mmi_trace(TRUE,"jdd_log Func: %s profile_name[%d]:%s, dcs:%d", __FUNCTION__, i, ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name, ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs);
			
			if (ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs == PH_DCS_ASCII)
			{
				JC_CHAR proxy_id[E_DC_MAX_PROXYID_LEN+1] = {0};
				//AnsiiNToUnicodeString(proxy_id, ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name, E_DC_MAX_PROXYID_LEN);
				utf8_to_ucs2_string((U8*)proxy_id, E_DC_MAX_PROXYID_LEN+1,(U8*)ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name);
				memcpy(pszProxyProfileList[i].proxy_id, proxy_id, E_DC_MAX_PROXYID_LEN*ENCODING_LENGTH);
			}
			else if (ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs == PH_DCS_UCS)
			{
				memcpy(pszProxyProfileList[i].proxy_id, ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name, E_DC_MAX_PROXYID_LEN*ENCODING_LENGTH);
			}
		}
	}
	else if (profile_type == E_PROF_MMS)
	{
		nvram_mms_profile_name_array_struct ph_mms_prof_name_arr;
	    memset(&ph_mms_prof_name_arr, 0, sizeof(ph_mms_prof_name_arr));
		
		if (!cs_GetMMSProfileNameList(&ph_mms_prof_name_arr, nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read MMS prolfile name list error", __FUNCTION__);
			return JC_ERR_FILE_READ;
		}

		for (i=0; i < NVRAM_MMS_PROFILE_MAX; i++)
		{
			if (ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].dcs == PH_DCS_ASCII)
			{
				JC_CHAR proxy_id[E_DC_MAX_PROXYID_LEN+1] = {0};
				//AnsiiNToUnicodeString(proxy_id, ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name, E_DC_MAX_PROXYID_LEN);
	                     utf8_to_ucs2_string((U8*)proxy_id, E_DC_MAX_PROXYID_LEN+1,(U8*)ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name);
				memcpy(pszProxyProfileList[i].proxy_id, proxy_id, E_DC_MAX_PROXYID_LEN*ENCODING_LENGTH);
			}
			else if (ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].dcs == PH_DCS_UCS)
			{
				memcpy(pszProxyProfileList[i].proxy_id, ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name, E_DC_MAX_PROXYID_LEN*ENCODING_LENGTH);
			}
		}
	}
   	return JC_OK;
}


JC_RETCODE cs_ReplaceProxyProfile(ST_CS_PROXY_PROFILE *pstProxyProfile, INT8 index)
{
	return cs_SaveProxyProfile(pstProxyProfile, index);
}


//gets the no.of maximum proxy proifles supported by platform
JC_INT32 cs_GetMaxProxyProfiles(E_PROV_PROFILE_TYPE  profile_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	mmi_trace(TRUE,"Jdd_log: Func: %s profile_type:%d,", __FUNCTION__, profile_type);
	if (profile_type == E_PROF_BROWSER)
	{
		return NVRAM_WAP_PROFILE_MAX;
	}
	else if (profile_type == E_PROF_MMS)
	{
		return NVRAM_MMS_PROFILE_MAX;
	}
	else
	{
		return -1;
	}
}

//gets the no.of maximum data conn proifles supported by platform
JC_INT32 cs_GetMaxDataConnProfiles()
{
	return MAX_DATA_ACCOUNT_GPRS_LIMIT;
}


//gets the total no.of maximum proxy proifles settings filled currently
JC_INT32 cs_GetTotalProxyProfiles(E_PROV_PROFILE_TYPE  profile_type, CFW_SIM_ID nSimID)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	int i = 0, total = 0;

	if (profile_type == E_PROF_BROWSER)
	{
		nvram_wap_profile_name_array_struct ph_wap_prof_name_arr;
	    memset(&ph_wap_prof_name_arr, 0, sizeof(ph_wap_prof_name_arr));
		
		if (!cs_GetWAPProfileNameList(&ph_wap_prof_name_arr, nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read WAP prolfile name list error", __FUNCTION__);
			return JC_ERR_FILE_READ;
		}

		for (i=0; i < NVRAM_WAP_PROFILE_MAX; i++)
		{			
			if (ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs == PH_DCS_ASCII)
			{
				if (strlen(ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name) > 0)
					total++;
			}
			else if (ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].dcs == PH_DCS_UCS)
			{
				if (UCS2Strlen(ph_wap_prof_name_arr.nvram_wap_profile_name_array[i].profile_name) > 0)
					total++;
			}
		}
	}
	else if (profile_type == E_PROF_MMS)
	{
		nvram_mms_profile_name_array_struct ph_mms_prof_name_arr;
	    memset(&ph_mms_prof_name_arr, 0, sizeof(ph_mms_prof_name_arr));
		
		if (!cs_GetMMSProfileNameList(&ph_mms_prof_name_arr, nSimID))
		{
			mmi_trace(TRUE,"JDD_LOG: Func: %s Read MMS prolfile name list error", __FUNCTION__);
			return JC_ERR_FILE_READ;
		}

		for (i=0; i < NVRAM_MMS_PROFILE_MAX; i++)
		{
			if (ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].dcs == PH_DCS_ASCII)
			{
				if (strlen(ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name) > 0)
					total++;
			}
			else if (ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].dcs == PH_DCS_UCS)
			{
				if (UCS2Strlen(ph_mms_prof_name_arr.nvram_mms_profile_name_array[i].profile_name) > 0)
					total++;
			}
		}
	}
   	return total;
}

//gets the total no.of maximum data conn proifles settings filled currently

JC_INT32 cs_GetTotalDataConnProfiles()
{
	int i = 0, total = 0;
	nvram_ef_abm_gprs_profile_struct *gprs_account_total;
	
	gprs_account_total =  (nvram_ef_abm_gprs_profile_struct*)jdd_MemAlloc(sizeof(nvram_ef_abm_gprs_profile_struct) * MAX_DATA_ACCOUNT_GPRS_LIMIT, 1);
	if (gprs_account_total == JC_NULL)
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s No enough memory error", __FUNCTION__);
		return JC_ERR_MEMORY_ALLOCATION;
	}
	memset(gprs_account_total, 0, sizeof(nvram_ef_abm_gprs_profile_struct)*MAX_DATA_ACCOUNT_GPRS_LIMIT);
	if(!cs_GetDataConnList(gprs_account_total))
	{
		mmi_trace(TRUE,"JDD_LOG: Func: %s Read Data conn list error", __FUNCTION__);
		jdd_MemFree(gprs_account_total);
		return JC_ERR_FILE_READ;
	}

	for (i=0; i < MAX_DATA_ACCOUNT_GPRS_LIMIT; i++)
	{
		if (gprs_account_total[i].name_dcs == DATA_ACCOUNT_ASCII)
		{
			if (strlen(gprs_account_total[i].name) > 0)
				total++;
		}
		else if (gprs_account_total[i].name_dcs == DATA_ACCOUNT_UCS )
		{
			if (UCS2Strlen(gprs_account_total[i].name) > 0)
				total++;
		}
	}
	jdd_MemFree(gprs_account_total);
	return total;
}
void cs_UnitTest()
{
	ST_CS_DATACONN_PROFILE DataConn;
	ST_CS_PROXY_PROFILE ProxyProfile;
	JC_RETCODE ret;
	int total = 0;

	int i = 0;
	ST_CS_DATACONN_PROFILE_NAME pszDataConnList[MAX_DATA_ACCOUNT_GPRS_LIMIT];
	ST_CS_PROXY_PROFILE_NAME pszProxyProfileList[NVRAM_WAP_PROFILE_MAX];

	memset(&DataConn, 0, sizeof(ST_CS_DATACONN_PROFILE));
	strcpy(DataConn.apn, "cmnet");
	strcpy(DataConn.username, "user");
	strcpy(DataConn.password, "password");
 	strcpy(DataConn.nap_id, "cs_dataconn1");
 	DataConn.gprs_auth_type = GPRS_AUTH_PAP;
  	
	ret = cs_SaveDataConnProfile(&DataConn, 0xFF);
	mmi_trace(TRUE,"jdd_log Func: %s cs_SaveDataConnProfile ret:%d", __FUNCTION__, ret);
	
	
	memset(&ProxyProfile, 0, sizeof(ST_CS_PROXY_PROFILE));
 	strcpy(ProxyProfile.homepage, "www.sina.com");
	strcpy(ProxyProfile.proxy_ip, "101.102.103.104");
 	strcpy(ProxyProfile.proxy_id, "cs_profile1");
 	strcpy(ProxyProfile.nap_id, "cs_dataconn1");
	
    ProxyProfile.port = 80;
    ProxyProfile.conn_mode = E_DC_CONNECTION_ORIENTED;
    ProxyProfile.profile_type = E_PROF_BROWSER;
    ProxyProfile.nSimID = CFW_SIM_1;

	ret = cs_SaveProxyProfile(&ProxyProfile, 0xFF);
	mmi_trace(TRUE,"jdd_log Func: %s sim 0 cs_SaveProxyProfile ret:%d", __FUNCTION__, ret);


	memset(pszDataConnList, 0, sizeof(ST_CS_DATACONN_PROFILE_NAME)*MAX_DATA_ACCOUNT_GPRS_LIMIT);
	cs_GetDataConnProfileList(pszDataConnList);
	for (i=0; i<MAX_DATA_ACCOUNT_GPRS_LIMIT; i++ )
	{
		U8 temp[30]={0};
		UnicodeToAnsii(temp, pszDataConnList[i].nap_id);
		mmi_trace(1,"Func: %s DATAPROFILE %d: %s", __FUNCTION__, i, temp);	
	}
   

	memset(pszProxyProfileList, 0, sizeof(ST_CS_PROXY_PROFILE_NAME)*NVRAM_WAP_PROFILE_MAX);
	cs_GetProxyProfileList(pszProxyProfileList, E_PROF_BROWSER, CFW_SIM_1);
	for (i=0; i<NVRAM_WAP_PROFILE_MAX; i++ )
	{
		U8 temp[30]={0};
		UnicodeToAnsii(temp, pszProxyProfileList[i].proxy_id);
		mmi_trace(1,"Func: %s PROXYPROFILE %d: %s", __FUNCTION__, i, temp);	
	}
	cs_ReplaceDataConnProfile(&DataConn, 3);
	cs_ReplaceProxyProfile(&ProxyProfile, 3);

	total = cs_GetMaxProxyProfiles(E_PROF_BROWSER);
	mmi_trace(TRUE,"Func: %s max proxy profile: %d", __FUNCTION__, total);
	
	total = cs_GetMaxDataConnProfiles();
	mmi_trace(TRUE,"Func: %s max dataconn profile: %d", __FUNCTION__, total);

	total = cs_GetTotalProxyProfiles(E_PROF_BROWSER, CFW_SIM_1);
	mmi_trace(TRUE,"Func: %s total proxy profile: %d", __FUNCTION__, total);

	total = cs_GetTotalDataConnProfiles();
	mmi_trace(TRUE,"Func: %s total dataconn profile: %d", __FUNCTION__, total);
}
