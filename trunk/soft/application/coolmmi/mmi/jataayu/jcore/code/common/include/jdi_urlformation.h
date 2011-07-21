/****************************************************************
 * $Id: jdi_urlformation.h,v 1.7 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.7 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 * Please note that any and all title and/or intellectual property 
 * rights in and to this Software or any part of this	(including 
 * without limitation any images, photographs, animations,	video, 
 * audio, music, text and/or "applets," incorporated into	   the 
 * Software), herein mentioned to as "Software", the  accompanying 
 * printed materials, and any copies of the Software, are owned by 
 * Jataayu Software (P) Ltd., Bangalore ("Jataayu") or	 Jataayu's 
 * suppliers as the case may be. The Software is protected		by 
 * copyright, including without limitation by applicable copyright 
 * laws, international treaty provisions,	other	  intellectual 
 * property laws and applicable laws in the country in	which  the 
 * Software is being used. You shall not modify,adapt or translate 
 * the Software,without prior express written consent from Jataayu.
 * You shall not reverse engineer, decompile,	disassemble		or 
 * otherwise alter the Software,except and only to the extent that 
 * such activity is  expressly  permitted   by   applicable	   law 
 * notwithstanding this limitation. Unauthorized  reproduction  or 
 * redistribution of this program or any  portion of it may result 
 * in severe civil and criminal penalties and will  be  prosecuted 
 * to the maximum extent possible under the law.  Jataayu reserves 
 * all rights not expressly granted.
 *
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL  REPRESENTATION  AND   WARRANTIES,   EITHER 
 * EXPRESS OR IMPLIED, INCLUDING  BUT  NOT  LIMITED  TO THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, 
 * AND/OR FITNESS FOR A  PARTICULAR  PURPOSE  OR  USE,  TITLE    OR 
 * INFRINGEMENT ARE  EXPRESSLY  DISCLAIMED  TO  THE  FULLEST EXTENT 
 * PERMITTED BY LAW. YOU ASSUME THE ENTIRE RISK AS TO THE  ACCURACY 
 * AND THE USE OF THIS SOFTWARE.JATAAYU SHALL NOT BE LIABLE FOR ANY 
 * CONSEQUENTIAL,  INCIDENTAL,  INDIRECT,  EXEMPLARY,   SPECIAL  OR 
 * PUNITIVE DAMAGES INCLUDING  WITHOUT LIMITATION ANY LOSS OF DATA, 
 * OR; LOSS OF PROFIT,SAVINGS BUSINESS OR GOODWILL OR OTHER SIMILAR 
 * LOSS RESULTING FROM  OR OUT OF  THE USE OR INABILITY TO USE THIS 
 * SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 ******************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdi_urlformation.h,v $
 * Revision 1.7  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.7  2006/12/21 07:24:59  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.6  2006/12/20 09:01:30  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.5  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/06/23 14:31:05  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/06/11 07:52:45  kumardevhtmlbrow
 * Added the url formation
 *
 *  
 *****************************************************************/

#ifndef _JDI_URL_FORMATION
#define _JDI_URL_FORMATION

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief		Resolves a relative URL in a document to it Absolute form
 *
 * @param[in]	pmBaseURL, The base URL for the document, which can be absent
 * @param[in]	pmCurrentURL, The current URL which needs to be resolved 
 * @param[in]   vMemHandle, Handle to the memory manager
 * @param[out]  ppmNextURL, The resolved URL for the given base and current URL
 * @retval		JC_ERR_MEMORY_ALLOCATION in case is the memory allocation fails
 * @retval		JC_OK in casse of success
 *
 * THis API resolves a relative URL to its absolute form depending upon different
 * condition
 * 
 * The caller needs to provide the current URL to the API and the base URL can 
 * also be NULL.
 **/
JC_RETCODE jdi_URLFormNext (const JC_CHAR *pmBaseURL, 
							const JC_CHAR *pmCurrentURL, 
							JC_HANDLE	   vMemHandle,
							JC_CHAR **ppmNextURL) ;

/**
 * @brief		Checks if the scheme for the URL is handled or not
 *
 * @param[in]	pmURL, The URL which needs to be checked for.
 * @retval		E_TRUE in case the scheme is handled
 * @retval		E_FALSE in case the scheme is not handled.
 *
 * This API checks if the scheme for a URL is handled or not.
 **/
JC_BOOLEAN jdi_URLIsSchemeNotHandled (const JC_CHAR *pmURL) ;


/**
 * @brief		Checks if the given URL is an absolute URL
 *
 * @param[in]	pmURL, The URL which needs to be checked for.
 * @retval		E_TRUE in case the URL is absolute
 * @retval		E_FALSE in case the URL is not absolute
 *
 * This API checks if the given URL is absolute or not
 **/
JC_BOOLEAN jdi_URLIsAbsoluteURL (const JC_CHAR *pmURL) ;

/**
 * @brief		Checks if the given URL is a blank URL
 *
 * @param[in]	pmURL, The URL which needs to be checked for.
 * @retval		E_TRUE in case the URL is blank
 * @retval		E_FALSE in case the URL is not blank
 *
 * This API checks if the given URL is blank or not
 **/
JC_BOOLEAN jdi_URLCheckBlankURL (const JC_CHAR *pmURL) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */



