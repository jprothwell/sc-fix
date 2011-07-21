
/***************************************************************************
 * $Id: ProvisionGetSetdata.h,v 1.2 2006/01/04 06:50:14 lumitdevpclient Exp $
 * $Revision: 1.2 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
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
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *  
 ***************************************************************************/


#ifndef _PROVISION_GETSETDATA_H
#define _PROVISION_GETSETDATA_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

#include <jdi_mimeinfo.h>



/**
 * Structure definations.
 */

typedef struct _string_token
{
	JC_INT8		*pcToken;
	struct _string_token	*pstNext;	
}ST_STR_TOKEN_LIST;


typedef struct _prov_parm_condition
{
	JC_INT8					*pcParameter ;
	/**
	 * The below index is for the index wise manipulation
	 * of the Parameter values those contain multiple
	 * values, though not implemented for the time being.
	 */
	JC_INT32				*iparmIndex ;
	struct _string_token	*pstTokenList ;
	struct _prov_condition	*pstConditionList;
}ST_PARM_CONDITION ;

/**
 * Function declarations
 */

JC_RETCODE prGetAllStringTokens (JC_INT8* pcString, JC_INT8 cDelimiter, ST_STR_TOKEN_LIST **ppstTokens, JC_UINT32 *puiCount) ;

JC_RETCODE prParseCondition (JC_INT8 *pucCondition, JC_UINT32 uiCondLen, 
							 ST_PROV_CONDITION **ppstCondList, JC_UINT32 *puiCount) ;
JC_RETCODE prAddConditionToList (ST_PROV_CONDITION **ppstCondList, JC_INT8 *ppucKey, 
							JC_INT8 *ppucValue) ;
void ReleaseConditionList (ST_PROV_CONDITION *pstCondList) ;
void ReleaseStringTokenList (ST_STR_TOKEN_LIST *pstStringToken) ;

JC_RETCODE prFormConditionForGetData (JC_INT8 *pcParmName, ST_STR_TOKEN_LIST *pstToken, ST_PROV_CONDITION *pstCond, ST_PARM_CONDITION **ppstParmCond) ;

/**< Functions related to Get Set Manipulation */


JC_RETCODE prCheckAndGetSetTheBlock (ST_PROFILE_DETAIL_INFO *pstProvDetails, ST_PARM_CONDITION *pstParmCondition, 
									 JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnValue, JC_UINT32 *uiValueSize) ;

/**< Functions to get & set the global characteristic fields */
JC_RETCODE prGetSetApplicationBlock (ST_PROV_APPLICATION *pstApplication, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetPxLogicalBlock (ST_PROV_PXLOGICAL *pstPxLogical, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetNapdefBlock (ST_PROV_NAPDEF *pstNapdef, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetAccessBlock (ST_PROV_ACCESS *pstAccess, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetBootstrapBlock (ST_PROV_BOOTSTRAP *pstBootstrap, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetClientidentityBlock (ST_PROV_CLIENTIDENTITY *pstClientidentity, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetVendorconfigBlock (ST_PROV_VENDORCONFIG *pstVendorconfig, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;

/**< Functions to get & set the Sub characteristic fields */
JC_RETCODE prGetSetPxPhysicalBlock (ST_PROV_PXPHYSICAL *pstPxPhysical, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetPortBlock (ST_PROV_PORT *pstPort, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetPxAuthinfoBlock (ST_PROV_PXAUTHINFO *pstAuthinfo, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetNapAuthinfoBlock (ST_PROV_NAPAUTHINFO *pstNapAuthinfo, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetValidityBlock (ST_PROV_VALIDITY *pstValidity, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetAppauthBlock (ST_PROV_APPAUTH *pstAppauth, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetResourceBlock (ST_PROV_RESOURCE *pstResource, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;
JC_RETCODE prGetSetAppaddrBlock (ST_PROV_APPADDR *pstAppaddr, 
				ST_PARM_CONDITION *pstParmCondition, JC_BOOLEAN bGetOrSet, JC_HANDLE *pvReturnVal) ;



#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROVISION_GETSETDATA_H */


/*** END OF FILE ***/
