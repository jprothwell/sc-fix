
/***************************************************************************
 * $Id: ProvisionJOTACore.h,v 1.5 2007/09/27 12:29:38 lumitdevpclient Exp $
 * $Revision: 1.5 $
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


#ifndef _PROVISION_JOTA_CORE_H
#define _PROVISION_JOTA_CORE_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

#include <jdi_mimeinfo.h>


/**< Macro definitions */

#define PROV_MIMETYPE_CONTENT_TYPE_NAME		"Content-Type"
#define PROV_MIMETYPE_SEC_NAME				"SEC"
#define	PROV_MIMETYPE_MAC_NAME				"MAC"

/**< End of Macro definitions */

JC_RETCODE ValidateAndUpdateProfileInfo (JC_HANDLE vHandle,E_PROV_DATA_TYPE *peProvType,JC_INT8 **ppcProvURL) ;

JC_RETCODE GetProvURLValue (ST_PROV_BOOTSTRAP *pstBootstrap, JC_INT8 **ppProvURLValue) ;

JC_RETCODE prGetCharacteristicCount (ST_PROFILE_DETAIL_INFO *pstProvDetail, 
									 JC_INT8 *pcCharacteristicName, JC_UINT32 *puiCount) ;

JC_RETCODE prCountApplicationChar (ST_PROV_APPLICATION *pstApplnChar, JC_UINT32 *puiCharCount) ;
JC_RETCODE prCountAccessChar (ST_PROV_ACCESS *pstAccessChar, JC_UINT32 *puiCharCount) ;
JC_RETCODE prCountBootstrapChar (ST_PROV_BOOTSTRAP *pstBootstrapChar, JC_UINT32 *puiCharCount) ;
JC_RETCODE prCountPxLogicalChar (ST_PROV_PXLOGICAL *pstPxlogicalChar, JC_UINT32 *puiCharCount) ;
JC_RETCODE prCountNapdefChar (ST_PROV_NAPDEF *pstNapdefChar, JC_UINT32 *puiCharCount) ;
JC_RETCODE prCountVendorConfigChar (ST_PROV_VENDORCONFIG *pstVendorConfChar, JC_UINT32 *puiCharCount) ;



/* Functions Used for jdi_ProvisionGetProfileDetailInfo */

JC_BOOLEAN prIsProfileExist (ST_PROV_APPLICATION *pstApplnChar, JC_INT8 *pcAppId,
							 JC_INT8 *pcAppName, ST_PROV_APPLICATION **ppstApplication) ;

JC_RETCODE prFillApplicationToProfileInfo (ST_PROFILE_APPLICATION **ppstProfileAppln, ST_PROV_APPLICATION *pstAppChar) ;
JC_RETCODE prFillBootstrapToProfileInfo (ST_PROFILE_BOOTSTRAP **ppstProfileBootstrap, ST_PROV_BOOTSTRAP *pstBootstrapChar) ;
JC_RETCODE prFillPxlogicalToProfileInfo (ST_PROFILE_PXLOGICAL **ppstProfilePxlogical, ST_PROV_PXLOGICAL *pstPxlogicalChar) ;
JC_RETCODE prFillNapdefToProfileInfo (ST_PROFILE_NAPDEF **ppstProfileNapdef, ST_PROV_NAPDEF *pstNapdefChar) ;
JC_RETCODE prFillAccessToProfileInfo (ST_PROFILE_ACCESS **ppstProfileAccess, ST_PROV_ACCESS *pstAccessChar) ;
JC_RETCODE prFillVendorconfToProfileInfo (ST_PROFILE_VENDORCONFIG **ppstProfileVendorconf, 
										  ST_PROV_VENDORCONFIG *pstVendorconfChar) ;
JC_RETCODE prFillClientIdToProfileInfo (ST_PROFILE_CLIENTIDENTITY **ppstProfileClientId, 
										ST_PROV_CLIENTIDENTITY *pstClientIdChar) ;

JC_RETCODE	prFillConnectionDataToProfileInfo (ST_PROFILE_DETAIL_INFO *pstProfileDetail,
											   ST_PROFILE_INFO **ppstProfileInfo) ;

JC_BOOLEAN prIsProxyMatchInDoc (ST_PROV_PXLOGICAL *pstPxlogicalChar, JC_INT8 *pcProxyId, 
							 ST_PROV_PXLOGICAL **ppstPxlogical) ;
JC_BOOLEAN prIsNapidMatchInDoc (ST_PROV_NAPDEF *pstNapdefChar, JC_INT8 *pcNapId, 
							 ST_PROV_NAPDEF **ppstNapdef) ;
JC_BOOLEAN prIsAppIdExistInAccess (ST_PROV_ACCESS *pstAccessChar, JC_INT8 *pcAppId, 
							 ST_PROV_ACCESS **ppstAccess) ;

JC_RETCODE prFillNapdefFromNapidToProfileInfo (ST_PROFILE_NAPDEF **ppstProfileNapdef, 
											   ST_PROV_NAPDEF *pstNapdef, ST_PROV_PXPHYSICAL *pstPxPhysical) ;

JC_BOOLEAN prGetProxyFromNapid (ST_PROV_PXLOGICAL *pstPxlogical, JC_INT8 *pcNapid, 
							 ST_PROV_PXLOGICAL **ppstPxlogical) ;

JC_RETCODE prFillDefaultBlocksToProfileInfo (ST_PROFILE_DETAIL_INFO *pstDetailInfo, 
											 ST_PROFILE_INFO **ppstProfileInfo) ;

JC_BOOLEAN prIsProxyPresentInBootstrap (ST_PROV_BOOTSTRAP *pstBootstrapChar,
							 ST_PROV_BOOTSTRAP **ppstBootstrap) ;

JC_RETCODE prGetDefaultConnectionSetting (ST_PROFILE_DETAIL_INFO *pstProfileDetail, 
										  ST_PROFILE_INFO **ppstProfileInfo) ;

JC_RETCODE prFillProfileInfoStructure (ST_PROFILE_DETAIL_INFO *pstProfileDetail, 
										  ST_PROFILE_INFO **ppstProfileInfo) ;


/**< To retrieve MIME Header values */
JC_RETCODE prGetMIMEheaderValues (ST_MIME_HEADERS	*pstHeadersList,
								  JC_INT8			**ppcContentType,
								  JC_INT8			**ppcMacValue,
								  JC_INT32			*piSecValue ) ;


/**< Lumit added on 19th Oct'06 as to ignore the PxLogical without valid NAP */

JC_RETCODE prIgnorePxLogicalwithoutValidNap (ST_PROFILE_DETAIL_INFO *ppstProfDetails) ;

/**< Lumit added on 14th Sep'07 as to support Access Rule */
JC_RETCODE prCheckAccessRule (ST_PROFILE_INFO **ppstProfileInfo) ;

#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROVISION_JOTA_CORE_H */


/*** END OF FILE ***/
