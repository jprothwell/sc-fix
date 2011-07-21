/***************************************************************************
 * $Id: ProvisionXMLConverter.h,v 1.3 2007/01/31 05:39:24 lumitdevpclient Exp $
 * $Revision: 1.3 $
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
 * $Log: provisionXMLConverter.h
 *
 * Revision 1.0  2005/08/24 13:36:43  lumitdevpclient
 * Created the header file.
 *  
 ***************************************************************************/
#ifndef _PROVISION_XML_CONVERTER_H
#define _PROVISION_XML_CONVERTER_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

#define MAX_FILE_PATH		200

#define CHARACTERISTIC_TYPE "characteristic type"
#define CHARACTERISTIC		"characteristic"
#define RINGTONES			"RINGTONES"



JC_RETCODE prCountLengthOfXMLBuffer (ST_PROFILE_DETAIL_INFO *pstProvDetail, JC_INT32 *piXmlBufferLen) ;
JC_RETCODE prConvertToXMLBuffer (ST_PROFILE_DETAIL_INFO *pstProvDetail, JC_UINT8 **ppucXMLBuffer) ;

JC_INT32 prGetBootstrapDataCount (ST_PROV_BOOTSTRAP *pstBootstrap) ;
JC_INT32 prGetPxLogicalDataCount (ST_PROV_PXLOGICAL *pstPxLogical) ;
JC_INT32 prGetApplicationDataCount (ST_PROV_APPLICATION *pstApplication) ;
JC_INT32 prGetNapdefDataCount (ST_PROV_NAPDEF *pstNapdef) ;
JC_INT32 prGetClientIdentityDataCount (ST_PROV_CLIENTIDENTITY *pstClientIdentity) ;
JC_INT32 prGetVendorConfigDataCount (ST_PROV_VENDORCONFIG *pstVendorConfig) ;
JC_INT32 prGetAccessDataCount (ST_PROV_ACCESS *pstAccess) ;

JC_INT32 prGetPortDataCount (ST_PROV_PORT *pstPort) ;
JC_INT32 prGetPxAuthInfoDataCount (ST_PROV_PXAUTHINFO *pstPxAuthInfo) ;
JC_INT32 prGetNapAuthInfoDataCount (ST_PROV_NAPAUTHINFO *pstNapAuthInfo) ;
JC_INT32 prGetValidityDataCount (ST_PROV_VALIDITY *pstValidity) ;
JC_INT32 prGetAppAuthDataCount (ST_PROV_APPAUTH *pstAppAuth) ;
JC_INT32 prGetAppAddrDataCount (ST_PROV_APPADDR *pstAppAddr) ;
JC_INT32 prGetPxPhysicalDataCount (ST_PROV_PXPHYSICAL *pstPxPhysical) ;
JC_INT32 prGetResourceDataCount (ST_PROV_RESOURCE *pstResource) ;

JC_RETCODE prGetCountFromValueStruct (ST_PROV_VALUES *pstValueStruct, JC_INT8 *pcValueName) ;


void prFillBootstrapDataToBuffer (ST_PROV_BOOTSTRAP *pstBootstrap, JC_INT8 **pcBuffer) ;
void prFillPxLogicalDataToBuffer (ST_PROV_PXLOGICAL *pstPxLogical, JC_INT8 **ppcBuffer) ;
void prFillApplicationDataToBuffer (ST_PROV_APPLICATION *pstApplication, JC_INT8 **ppcBuffer) ;
void prFillNapdefDataToBuffer (ST_PROV_NAPDEF *pstNapdef, JC_INT8 **ppcBuffer) ;
void prFillVendorConfigDataToBuffer (ST_PROV_VENDORCONFIG *pstVendorConfig, JC_INT8 **ppcBuffer) ;
void prFillClientIdentityDataToBuffer (ST_PROV_CLIENTIDENTITY *pstClientIdentity, JC_INT8 **ppcBuffer) ;
void prFillAccessDataToBuffer (ST_PROV_ACCESS *pstAccess, JC_INT8 **ppcBuffer) ;

JC_INT32 prFillResourceDataToBuffer (ST_PROV_RESOURCE *pstResource, JC_INT8 **ppcBuffer) ;
JC_INT32 prFillAppAddrDataToBuffer (ST_PROV_APPADDR *pstAppAddr, JC_INT8 **ppcBuffer) ;
JC_INT32 prFillAppAuthDataToBuffer (ST_PROV_APPAUTH *pstAppAuth, JC_INT8 **ppcBuffer) ;
JC_INT32 prFillNapAuthInfoDataToBuffer (ST_PROV_NAPAUTHINFO *pstNapAuthInfo, JC_INT8 **ppcBuffer) ;
JC_INT32 prFillValidityDataToBuffer (ST_PROV_VALIDITY *pstValidity, JC_INT8 **ppcBuffer) ;
JC_INT32 prFillPxAuthInfoDataToBuffer (ST_PROV_PXAUTHINFO *pstPxAuthInfo, JC_INT8 **ppcBuffer) ;
JC_INT32 prFillPxPhysicalDataToBuffer (ST_PROV_PXPHYSICAL *pstPxPhysical, JC_INT8 **ppcBuffer) ;

JC_INT32 prFillPortDataToBuffer (ST_PROV_PORT *pstPort, JC_INT8 **ppcBuffer, JC_BOOLEAN bFlag) ;

JC_INT32 prFillParamValueToBuffer (JC_INT8 *pcParmValue, JC_INT8 *pcParmType, JC_INT8 **ppcBuffer, JC_INT32 iCount) ;
JC_INT32 prFillValueStructToBuffer (ST_PROV_VALUES *pstValue, JC_INT8 *pcParmType, JC_INT8 **ppcBuffer, JC_INT32 iCount) ;
/**< Added by Arun on 24/11 */
JC_INT32 prFillNameValueStructToBuffer (ST_PROV_NAME_VALUES *pstValue, JC_INT8 **ppcBuffer, JC_INT32 iCount) ;
JC_INT32 prGetCountFromNameValueStruct (ST_PROV_NAME_VALUES *pstValueStruct) ;





#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROVISION_XML_CONVERTER_H */


/*** END OF FILE ***/
