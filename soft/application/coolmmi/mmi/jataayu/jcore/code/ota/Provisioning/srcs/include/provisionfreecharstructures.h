/***************************************************************************
 * $Id: Provisionfreecharstructures.h,v 1.3 2007/01/31 05:39:24 lumitdevpclient Exp $
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
 * $Log: Provisionfreecharstructures.h,v $
 * Revision 1.3  2007/01/31 05:39:24  lumitdevpclient
 * Compiler warning and Lint warning fixed as reported by TTPCom
 *
 * Revision 1.2  2006/01/04 06:50:14  lumitdevpclient
 * committ after fixing some continuous provisioning related discripancies
 *
 * Revision 1.9  2005/09/17 06:40:14  arundevpclient
 * Integrated with the new changes in the common utils.
 *
 * Revision 1.6  2005/09/09 05:38:43  lumitdevpclient
 * commit on 9th September
 *
 * Revision 1.5  2005/08/19 13:45:58  guestpclient
 * Code release on 19th August before changing on the comments on TTPcom
 *
 * Revision 1.4  2005/08/09 15:10:31  guestpclient
 * Modified and changed to suit for the doxygen style and new coding standards.
 *
 *
 * Revision 1.1  2005/08/06 10:52:26  arundevpclient
 * Modified it to doxygen standard
 *
 *  
 ***************************************************************************/

#ifndef _PROV_FREE_CHAR_STRUCTURES_H
#define _PROV_FREE_CHAR_STRUCTURES_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

/** 
 * @brief declares the structure for fixing the internal memory management.
 * @datatype To store the start and end node of a mem allocated XML block.
 */
typedef struct st_xml_list 
{
	ST_XML_TAG			*pstStartTag ;
	ST_XML_TAG			*pstEndTag ;
	struct st_xml_list	*pstNext;
}ST_XML_LIST ;


void prFreeCharPxlogical (ST_PROV_PXLOGICAL *pPxlogicalChar) ;

void prFreeCharPxphysical (ST_PROV_PXPHYSICAL *pPxphysicalChar) ;

void prFreeCharAppaddr (ST_PROV_APPADDR *pAppaddrChar) ;

void prFreeCharAccess (ST_PROV_ACCESS *pAccessChar) ;

void prFreeCharApplication (ST_PROV_APPLICATION *pApplicationChar) ;

void prFreeCharAppauth (ST_PROV_APPAUTH *pAppauthChar) ;

void prFreeCharVendorconfig (ST_PROV_VENDORCONFIG *pVendorconfigChar) ;

void prFreeCharValidity (ST_PROV_VALIDITY *pValidityChar) ;

void prFreeCharResource (ST_PROV_RESOURCE *pResourceChar) ;

void prFreeCharPxauthinfo (ST_PROV_PXAUTHINFO *pPxauthinfoChar) ;

void prFreeCharPort (ST_PROV_PORT *pPortChar) ;

void prFreeCharNapdef (ST_PROV_NAPDEF *pNapdefChar) ;

void prFreeCharNapauthinfo (ST_PROV_NAPAUTHINFO *pNapauthinfoChar) ;

void prFreeCharClientidentity (ST_PROV_CLIENTIDENTITY *pClientidentityChar) ;

void prFreeCharBootstrap (ST_PROV_BOOTSTRAP *pBootstrapChar) ;

void prFreeVALUE (ST_PROV_VALUES *pValueStruct) ;

void prFreeNameValue (ST_PROV_NAME_VALUES *pstValueStruct) ;

void prFreeTag (ST_XML_TAG *pNode) ;

void prFreeLIST (ST_PROV_LIST *pList) ;

void prFreeCondition (ST_PROV_CONDITION *pCondList) ;

JC_RETCODE prFreeGetDataBuffer(JC_INT8 *pcDataType, JC_HANDLE vDataBuffer) ;

/**
 * Arun added on 12/09/2005 
 * Added functions to track the memory leak by internal XML block allocation.
 */

JC_RETCODE prAddPointerIntoList ( ST_XML_TAG *pstStartPtr, ST_XML_TAG *pstEndPtr) ;

void prFreePointerList ( void ) ;

void prDeletePointerFromList (ST_XML_TAG *pstStartPtr) ;

/*** Lumit Added ***/
JC_RETCODE prFreeAttributeList (ST_XML_ATTRIBUTE *pstAttribList) ;

/**
 * Lumit added on 26/10/2005 to free the Profile Info structure
 */
void prFreeProfileAccess (ST_PROFILE_ACCESS *pstProfAccess) ;
void prFreeProfileBootstrap (ST_PROFILE_BOOTSTRAP *pstProfBootstrap) ;
void prFreeProfileNapdef (ST_PROFILE_NAPDEF *pstProfNapdef) ;
void prFreeProfilePxlogical (ST_PROFILE_PXLOGICAL *pstProfPxlogical) ;
void prFreeProfileVendorconfig (ST_PROFILE_VENDORCONFIG *pstProfVendorconfig) ;
void prFreeProfileClientidentity (ST_PROFILE_CLIENTIDENTITY *pstProfClientidentity) ;
void prFreeProfileApplication (ST_PROFILE_APPLICATION *pstProfApplication) ;

#endif /**< JOTA_PROVISION_CORE_LIBRARY */

#endif /**< _PROV_FREE_CHAR_STRUCTURES_H */


/*** END OF FILE ***/
