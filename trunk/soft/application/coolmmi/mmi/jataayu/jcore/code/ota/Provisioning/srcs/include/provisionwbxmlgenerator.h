/***************************************************************************
 * $Id: provisionWBXMLGenerator.h,v 1.3 2007/01/31 05:39:24 lumitdevpclient Exp $
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
 * $Log: provisionWBXMLGenerator.h
 *
 * Revision 1.0  2005/08/24 13:36:43  lumitdevpclient
 * Created the header file.
 *  
 ***************************************************************************/
#ifndef _PROVISION_WBXML_GENERATOR_H
#define _PROVISION_WBXML_GENERATOR_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

#define MAX_FILE_PATH		200

#define CHARACTERISTIC_TYPE "characteristic type"
#define CHARACTERISTIC		"characteristic"
#define RINGTONES			"RINGTONES"



JC_RETCODE prConvertToWBXMLBuffer (ST_PROFILE_DETAIL_INFO *pstProvDetail, 
								   ST_PROVISION_WORKSPACE *pstProvWorkspace, 
								   JC_BOOLEAN	*pbIsOverFlow) ;




JC_RETCODE prFillBootstrapDataToWBXMLBuffer (ST_PROV_BOOTSTRAP *pstBootstrap, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillPxLogicalDataToWBXMLBuffer (ST_PROV_PXLOGICAL *pstPxLogical, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillApplicationDataToWBXMLBuffer (ST_PROV_APPLICATION *pstApplication, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillNapdefDataToWBXMLBuffer (ST_PROV_NAPDEF *pstNapdef, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillVendorConfigDataToWBXMLBuffer (ST_PROV_VENDORCONFIG *pstVendorConfig, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillClientIdentityDataToWBXMLBuffer (ST_PROV_CLIENTIDENTITY *pstClientIdentity, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillAccessDataToWBXMLBuffer (ST_PROV_ACCESS *pstAccess, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillResourceDataToWBXMLBuffer (ST_PROV_RESOURCE *pstResource, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillAppAddrDataToWBXMLBuffer (ST_PROV_APPADDR *pstAppAddr, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillAppAuthDataToWBXMLBuffer (ST_PROV_APPAUTH *pstAppAuth, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillNapAuthInfoDataToWBXMLBuffer (ST_PROV_NAPAUTHINFO *pstNapAuthInfo, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillValidityDataToWBXMLBuffer (ST_PROV_VALIDITY *pstValidity, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillPxAuthInfoDataToWBXMLBuffer (ST_PROV_PXAUTHINFO *pstPxAuthInfo, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillPxPhysicalDataToWBXMLBuffer (ST_PROV_PXPHYSICAL *pstPxPhysical, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;


JC_RETCODE prFillPortDataToWBXMLBuffer (ST_PROV_PORT *pstPort, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;


JC_RETCODE prFillParamValueToWBXMLBuffer (JC_INT8 *pcParmValue, JC_UINT8 ucParmType, ST_PROVISION_WORKSPACE *pstProvWorkspace, JC_BOOLEAN bSwithPage) ;

JC_RETCODE prFillValueStructToWBXMLBuffer (ST_PROV_VALUES *pstValue, JC_UINT8 ucParmType, ST_PROVISION_WORKSPACE *pstProvWorkspace, JC_BOOLEAN bSwithPage) ;

/**< Added by Arun on 24/11 */
JC_RETCODE prFillNameValueStructToWBXMLBuffer (ST_PROV_NAME_VALUES *pstValue, ST_PROVISION_WORKSPACE *pstProvWorkspace) ;

JC_RETCODE prFillCharValueToWBXMLBuffer (JC_UINT8 ucParmType, ST_PROVISION_WORKSPACE *pstProvWorkspace, JC_BOOLEAN bSwitchPage) ;


#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROVISION_WBXML_GENERATOR_H */


/*** END OF FILE ***/
