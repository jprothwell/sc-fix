/***************************************************************************
 * $Id: jdi_cookie.h,v 1.20 2009/04/09 14:14:01 sriramdevhtmlbrow Exp $
 * $Revision: 1.20 $
 * $Date: 2009/04/09 14:14:01 $
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
 * $Log: jdi_cookie.h,v $
 * Revision 1.20  2009/04/09 14:14:01  sriramdevhtmlbrow
 * Added new interface jdi_CookieIsEnabled to check whether cookies are enabled or not.
 *
 * Revision 1.19  2009/02/26 05:14:39  sudhadevhtmlbrow
 
 *
 * Revision 1.18  2008/07/08 06:05:40  sriramdevhtmlbrow
 * Added new interface jdi_CookieSetVersion and enum E_SET_COOKIE_VERSION for setting the version of the cookie received from the server.
 *
 * Revision 1.17  2006/12/01 07:02:15  kumardevhtmlbrow
 * Update for handling maximum cookie size.
 *
 * Revision 1.16  2006/11/14 06:08:53  kumardevhtmlbrow
 * Update
 *
 * Revision 1.15  2006/11/10 05:44:03  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.14  2006/07/24 04:37:27  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.13  2006/01/24 12:02:05  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.12  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.11  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.10  2005/08/31 15:12:11  kumardevhtmlbrow
 * no message
 *
 * Revision 1.9  2005/08/31 13:27:49  kumardevhtmlbrow
 * no message
 *
 * Revision 1.8  2005/08/12 10:57:42  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/06/23 09:49:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/05/21 11:20:48  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.4  2005/05/19 15:00:26  kumardevhtmlbrow
 * Updated the history & cookie
 *
 * Revision 1.3  2005/05/04 06:40:17  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/04/15 15:51:08  kumardevhtmlbrow
 * Changed the parameter for cookie clear function .
 *
 *  
 ***************************************************************************/

#ifndef _JDI_COOKIE_H
#define _JDI_COOKIE_H

typedef enum cookieControlAction
{
	E_ACCEPT_COOKIE,
	E_DENY_COOKIE,
	E_ACCEPT_SESSION_COOKIE 
} E_COOKIE_CONTROL_ACTION ;

typedef enum cookieDesc
{
	E_DISABLE_SESSION_COOKIE,
	E_ENABLE_SESSION_COOKIE,
	E_DISABLE_ALL_COOKIE,
	E_ENABLE_ALL_COOKIE
} E_COOKIE_DESC ;

typedef struct cookieList
{
	JC_INT8				*pcString ;
	void				*vRes ;
	struct cookieList	*pstNext ;
} ST_COOKIE_LIST ;

/**
 * @brief Specifies the version number of the cookie received from the server.
 */
typedef enum e_set_cookie_version
{
	E_SET_COOKIE_2, /** < Specifies that the cookie received from server is version 2 cookie (Set-Cookie2 header). */
	E_SET_COOKIE_1  /** < Specifies that the cookie received from server is version 1 cookie (Set-Cookie header). */
} E_SET_COOKIE_VERSION ;

#ifdef __cplusplus
extern "C" {
#endif


#define JC_ERR_COOKIE_DISABLED			TERMINAL_ERR_CODE(COOKIE_MODULE, 0x01)
#define JC_ERR_COOKIE_NOT_FOUND			TERMINAL_ERR_CODE(COOKIE_MODULE, 0x02)
#define JC_ERR_COOKIE_INVALID_URL		TERMINAL_ERR_CODE(COOKIE_MODULE, 0x03)
#define JC_ERR_COOKIE_STORAGE_DISABLED	TERMINAL_ERR_CODE(COOKIE_MODULE, 0x04)


JC_RETCODE jdi_CookieInitialize (JC_UINT32		uiMaxDomain,
								 JC_UINT32		uiMaxCookie, 
								 JC_UINT32		uiMaxCookieSize,
								 const JC_CHAR	*pmFileName,
								 JC_HANDLE		vMemHandle,
								 JC_HANDLE		*pvHandle) ;
JC_RETCODE jdi_CookieDeinitialize (JC_HANDLE vHandle, const JC_CHAR *pmFileName) ;
void	   jdi_CookieClear (JC_HANDLE vHandle) ;
JC_RETCODE jdi_CookieSetRestrictions (JC_HANDLE vHandle,
									  JC_INT8 *pcDomain,
									  E_COOKIE_CONTROL_ACTION eAction) ;
JC_RETCODE jdi_CookieEnable (JC_HANDLE vHandle,
							 E_COOKIE_DESC eCookieDesc) ;
JC_RETCODE jdi_CookieSet (JC_HANDLE vHandle,
						  const JC_INT8 *pcURL,
						  JC_INT64 lDateTime,
						  JC_INT8 *pcString) ;
JC_RETCODE jdi_CookieGet (JC_HANDLE vHandle,
						  const JC_INT8 *pcURL,
						  ST_COOKIE_LIST **ppstCookieLists) ;
JC_RETCODE jdi_CookieStorePersistent (JC_HANDLE vHandle, const JC_CHAR *pmFileName) ;

void jdi_CookieFreeList (const JC_HANDLE vHandle, ST_COOKIE_LIST *pstCookieLists) ;

/**
 * @brief			Sets the cookie version.
 * @param[in]		vHandle, pointer to the cookie interface
 * @param[in]       eVersion, Version number of the cookie received from server
 * @return			void
 */
void jdi_CookieSetVersion (JC_HANDLE				vHandle,
						   E_SET_COOKIE_VERSION		eVersion) ;

/**
 * @brief			This function checks whether cookie is enabled or not
 * @param[in]		vHandle,pointer to the cookie interface
 * @return			Returns boolean value whether cookie is enabled or not
 **/
JC_BOOLEAN jdi_CookieIsEnabled (JC_HANDLE	vHandle) ;

#ifdef __cplusplus
}
#endif



#endif

/* END OF FILE */



