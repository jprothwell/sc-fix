/***************************************************************************
 * $Id: ProvisionCommonWrapper.h,v 1.3 2007/01/31 05:39:24 lumitdevpclient Exp $
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
 * $Log: ProvisionCommonWrapper.h,v $
 * Revision 1.3  2007/01/31 05:39:24  lumitdevpclient
 * Compiler warning and Lint warning fixed as reported by TTPCom
 *
 * Revision 1.2  2006/01/04 06:50:14  lumitdevpclient
 * committ after fixing some continuous provisioning related discripancies
 *
 *
 * Revision 1.1  2005/12/17 10:52:26  arundevpclient
 * First version of conversions are written for WAP-4 version to HTML version
 *
 *  
 ***************************************************************************/


#ifndef _JDI_PROVISIONING_WRAPPER_H
#define _JDI_PROVISIONING_WRAPPER_H

#ifdef JOTA_PROVISION_CORE_LIBRARY

/**
 * The provisioning wrappers declare over common utils for internal purpose
 * To support any change in future for the exising common functionality.
 * The depenadent modules with jOTA are
 * 1. XML dependancy
 * 2. WBXML decoder
 * 3. Common Utils
 */

/**
 * Here we will put the DEPENDENT things under a compile time option
 * For WAPBroswer-v-4 version of XML and WBXML parser
 */
#ifdef JOTA_CORE_LIBRARY_FOR_WAP_4

	/**< Headers dependency */
	#include <miscutils.h>
	#include <xmlerror.h>
	#include <jdi_XML.h>
	#include <jdi_WBXML.h>	

	/**< Dependency with XML/WBXML parser*/
	/**
	 * Here the structure names and their member variable are different in old
	 * and new version of the XML parser. So to compile with the old one we 
	 * need to map old member names. Check header files of XML for more info.
	 */
	#define		pstRoot  root
	#define		pcName   name
	#define		pstParent   parent_id
	#define		pstNext     next
	#define		pmValue    value

	#define		ST_XML_TREE    XML
	#define		ST_XML_TAG   TAG
	#define		ST_XML_ATTRIBUTE   ATTRIBUTE

	#define		jdi_XMLFreeTree(x,y)	jdi_XMLFreeTree(y)

	#define		jdd_AssertOnFail(x, y)
	#define		jdi_MemAlloc(x, y, z)	jdd_MemAlloc(y,z)
	#define		jdi_MemFree(x,y)		jdd_MemFree(y)


	/**< Dependency with commonutililites */

	#define jdi_CUtilsTcharToChar(vMemHandle, pmSrc, ppcDest)\
			jdi_UtilsTcharToChar(ppcDest,pmSrc)

	#define jdi_CUtilsCharToTchar(vMemHandle,pcSrc,ppmDest)\
			jdi_UtilsCharToTchar(ppmDest,pcSrc) 

	#define jdi_CUtilsStrDuplicate(vMemHandle,pcStr)\
			jdi_UtilsStrdup (pcStr)

	#define jdi_CUtilsStrCaseStr(pcSrc,pcSubStr)\
			jdi_UtilsLocateString(pcSrc,pcSubStr)

	#define jdi_CUtilsStrIndexOf(pcSrc,cChar,uiStart)\
			jdi_UtilsIndexOf (cChar,pcSrc,uiStart)

	#define	jdi_CUtilsStrCopyIndex(vMemHandle,pcStr,iBegin,iEnd)\
			jdi_UtilsStrCopyIndex(pcStr,iBegin,iEnd)

	#define jdi_CUtilsReadFromFile(vMemHandle,pmFileName,ppucData,puiLength)\
			jdi_UtilsReadFromFile(pmFileName,ppucData,puiLength)

	/* 
	JC_RETCODE jdi_CUtilsTcharToChar (JC_HANDLE	vMemHandle, const JC_CHAR *pmSrc, JC_INT8 **ppcDest) ; 

	JC_RETCODE jdi_CUtilsCharToTchar (JC_HANDLE	vMemHandle, const JC_INT8 *pcSrc, JC_CHAR **ppmDest) ;
		
	JC_INT8 *jdi_CUtilsStrDuplicate (JC_HANDLE vMemHandle, const JC_INT8 *pcStr) ;

	JC_INT8 *jdi_CUtilsStrCaseStr (const JC_INT8	*pcSrc, const JC_INT8	*pcSubStr) ;

	JC_INT32 jdi_CUtilsStrIndexOf (const JC_INT8 *pcSrc, JC_INT8 cChar,  JC_UINT32 uiStart) ;

	JC_INT8* jdi_CUtilsStrCopyIndex (JC_HANDLE vMemHandle, const JC_INT8* pcStr, JC_INT32 iBegin, JC_INT32 iEnd) ;

	JC_BOOLEAN jdi_CUtilsStrStartsWith (const JC_INT8	*pcSrc,	const JC_INT8	*pcSubStr) ;

	JC_RETCODE jdi_CUtilsReadFromFile (JC_HANDLE vMemHandle, JC_CHAR *pmFileName, JC_UINT8	 **ppucData, JC_UINT32	*puiLength) ;
	*/

#else /**< For WAPBroswer-v-4 version of XML and WBXML parser */

	#include <jdi_memmgr.h>
	#include <jdi_cutils.h>
	#include <jdi_xml.h>
	#include <jdi_xmldatatype.h>

#endif /**< JOTA_CORE_LIBRARY_FOR_WAP_4 */
#endif /**< JOTA_PROVISION_CORE_LIBRARY */
#endif /**< _JDI_PROVISIONING_WRAPPER_H */


/*** END OF FILE ***/
