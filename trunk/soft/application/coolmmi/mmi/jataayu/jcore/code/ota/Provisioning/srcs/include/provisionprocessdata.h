/***************************************************************************
 * $Id: ProvisionProcessData.h,v 1.7 2007/10/03 09:38:54 lumitdevpclient Exp $
 * $Revision: 1.7 $
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
 * $Log: provisionprocessdata.h
 *
 * Revision 1.0  2005/08/24 13:36:43  anvardevpclient
 * Created the header file.
 *  
 ***************************************************************************/
#ifndef _PROVISION_PROCESS_DATA_H
#define _PROVISION_PROCESS_DATA_H

#ifdef JOTA_PROVISION_CORE_LIBRARY



JC_RETCODE GenerateProvStructure (JC_UINT8 *pucBuffer, JC_UINT32 uiBufferLen,
								  JC_INT8 *pcContentType,ST_PROFILE_DETAIL_INFO **ppstProfDetailInfo);

JC_RETCODE ParseAndGenerateXMLTree (JC_UINT8 *pucBuffer, JC_UINT32 uiBufferLen,
								    JC_INT8 *pcContentType,ST_XML_TREE *pstXml);

JC_RETCODE FillProvisionStructure (ST_XML_TREE *pstXml,
								   ST_PROFILE_DETAIL_INFO *pstProfDetailInfo);

E_PROV_CHARNAME GetGlobalCharacteristicBlockID (ST_XML_TAG **ppstXmlTag);

JC_INT8* GetAttributeContent (ST_XML_TAG *pstXmlTag, const JC_INT8 *pAttrType);

E_PROV_CHARNAME GetGlobalCharacteristicID (JC_INT8 *pcCharName);

E_PROV_CHARNAME GetSubCharacteristicID (JC_INT8 *pcCharName) ;

JC_RETCODE  FillApplicationCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								ST_PROFILE_DETAIL_INFO *pstProfDetailInfo);

JC_RETCODE  FillBootstrapCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								ST_PROFILE_DETAIL_INFO *pstProfDetailInfo);

JC_RETCODE  FillPxLogicalCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								ST_PROFILE_DETAIL_INFO *pstProfDetailInfo);

JC_RETCODE  FillNapdefCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								ST_PROFILE_DETAIL_INFO *pstProfDetailInfo);

JC_RETCODE  FillAccessCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								ST_PROFILE_DETAIL_INFO *pstProfDetailInfo);

JC_RETCODE  FillClientIdentityCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								ST_PROFILE_DETAIL_INFO *pstProfDetailInfo);

JC_RETCODE  FillVendorConfigCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								ST_PROFILE_DETAIL_INFO *pstProfDetailInfo);

/**< Lumit added */

JC_RETCODE  prAppendToValuesStruct (ST_PROV_VALUES **ppstValueStruct, JC_INT8 *pucValue) ;

JC_BOOLEAN IsGlobalCharacteristicBlock (JC_INT8 *pcCharName) ;

JC_RETCODE AddNewVendBlockToList (ST_PROV_VENDORCONFIG **ppstVendHead,
								ST_PROV_VENDORCONFIG *pstVendChar) ;

JC_RETCODE AddNewAccessBlockToList (ST_PROV_ACCESS **ppstAccessHead,
								ST_PROV_ACCESS *pstAccessChar) ;

JC_RETCODE AddNewBootstrapBlockToList (ST_PROV_BOOTSTRAP **ppstBootstrapHead,
								ST_PROV_BOOTSTRAP *ppstBootstrapChar) ;

JC_RETCODE AddNewApplicationBlockToList (ST_PROV_APPLICATION **ppstAppHead,
								ST_PROV_APPLICATION *pstAppChar) ;

JC_RETCODE AddNewPxLogicalBlockToList (ST_PROV_PXLOGICAL **ppstPxLogHead,
								ST_PROV_PXLOGICAL *pstPxLogChar) ;

JC_RETCODE AddNewNapdefBlockToList (ST_PROV_NAPDEF **ppstNapdefHead,
								ST_PROV_NAPDEF *pstNapdefChar) ;

/**< subchar prototype declarations */

JC_RETCODE FillSubCharsInApplicationBlock (ST_XML_TAG **ppstXmlTag, JC_INT8 *pcCharName, 
								ST_PROV_APPLICATION *pstAppChar) ;

JC_RETCODE FillSubCharsInPxLogicalBlock (ST_XML_TAG **ppstXmlTag, JC_INT8 *pcSubCharType,
										   ST_PROV_PXLOGICAL *pstProfPxLogical) ;

JC_RETCODE FillSubCharsInNapdefBlock (ST_XML_TAG **ppstXmlTag, JC_INT8 *pcSubCharType,
										   ST_PROV_NAPDEF *pstProfNapdef) ;

JC_RETCODE FillAppAddrCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								   ST_PROV_APPADDR **ppstProfAppAddr) ;

JC_RETCODE AddNewAppAddrBlockToList (ST_PROV_APPADDR **ppstAppAddrHead,
												ST_PROV_APPADDR *pstAppAddrChar) ;

JC_RETCODE FillAppAuthCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								   ST_PROV_APPAUTH **ppstProfAppAuth) ;

JC_RETCODE AddNewAppAuthBlockToList (ST_PROV_APPAUTH **ppstAppAuthHead,
												ST_PROV_APPAUTH *pstAppAuthChar) ;

JC_RETCODE FillResourceCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								   ST_PROV_RESOURCE **ppstProfResource) ;

JC_RETCODE AddNewResourceBlockToList (ST_PROV_RESOURCE **ppstResourceHead,
												ST_PROV_RESOURCE *pstResourceChar) ;

JC_RETCODE FillPxAuthInfoCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								   ST_PROV_PXAUTHINFO **ppstProfPxAuthInfo) ;

JC_RETCODE AddNewPxAuthInfoBlockToList (ST_PROV_PXAUTHINFO **ppstPxAuthInfoHead,
												ST_PROV_PXAUTHINFO *pstPxAuthInfoChar) ;

JC_RETCODE FillPxPhysicalCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								   ST_PROV_PXPHYSICAL **ppstProfPxPhysical) ;

JC_RETCODE AddNewPxPhysicalBlockToList (ST_PROV_PXPHYSICAL **ppstPxPhysicalHead,
												ST_PROV_PXPHYSICAL *pstPxPhysicalChar) ;

JC_RETCODE FillPortCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								   ST_PROV_PORT **ppstProfPort) ;

JC_RETCODE AddNewPortBlockToList (ST_PROV_PORT **ppstPortHead,
												ST_PROV_PORT *pstPortChar) ;

JC_RETCODE FillNapAuthInfoCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								   ST_PROV_NAPAUTHINFO **ppstProfNapAuthInfo) ;

JC_RETCODE AddNewNapAuthInfoBlockToList (ST_PROV_NAPAUTHINFO **ppstNapAuthInfoHead,
												ST_PROV_NAPAUTHINFO *pstNapAuthInfoChar) ;

JC_RETCODE FillValidityCharacteristicStruct (ST_XML_TAG **ppstXmlTag,
								   ST_PROV_VALIDITY **ppstProfValidity) ;

JC_RETCODE AddNewValidityBlockToList (ST_PROV_VALIDITY **ppstValidityHead,
												ST_PROV_VALIDITY *pstValidityChar) ;

JC_BOOLEAN prCheckRedundantAppChar (ST_PROV_APPLICATION *pstAppHead,
												ST_PROV_APPLICATION *pstAppChar) ;

/**< validation of characteristic block */

JC_RETCODE prValidatePxLogicalBlock (ST_PROV_PXLOGICAL *pstPxLogicalBlock) ;

JC_RETCODE prCheckRedundantNapDefinition (ST_PROV_NAPDEF *pstNapdefHead,
												JC_INT8 *pcNapId) ;

JC_RETCODE prTruncateIllegalBlock (ST_XML_TAG **ppstXmlTag) ;

JC_RETCODE prAppendToNameValuesStruct (ST_PROV_NAME_VALUES **ppstValueStruct, JC_INT8 *pucName, JC_INT8 *pucValue) ;


/**< Arun added on 14/12/2005 */
JC_RETCODE prProvisionFreeProfileDetailInfo (ST_PROFILE_DETAIL_INFO *pstProfileDetail) ;

JC_RETCODE prProvisionGenerateBuffer (JC_HANDLE		vHandle,
										 E_MESSAGE_TYPE eMsgType,
										 JC_UINT8		**ppucBuffer,
										 JC_UINT32		*puiBufLen) ;

/**< Lumit added on 16th Oct for Wap Version extract */
JC_BOOLEAN prWapVersionSupportedOrNot (JC_INT8 *pcVersion) ;

/**< Lumit added on 18th Oct for the partial fill data */
void prUtilsSetPartialFillFlagOn () ;
JC_BOOLEAN prUtilsGetPartialFillFlag () ;

/**< Lumit added on 19th Oct for ignoring any Invalid PxPhysical block */
void prUtilsSetIgnorePxLogicalFlagOn () ;
void prUtilsReSetIgnorePxLogicalFlag () ;
JC_BOOLEAN prUtilsGetIgnorePxLogicalFlag () ;

#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROVISION_PROCESS_DATA_H */

/*** END OF FILE ***/
