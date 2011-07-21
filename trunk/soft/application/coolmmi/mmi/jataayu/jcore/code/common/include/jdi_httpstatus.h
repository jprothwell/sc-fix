/***************************************************************************
 * $Id: jdi_httpstatus.h,v 1.10 2008/07/31 08:10:48 browserdevjtbase Exp $
 * $Revision: 1.10 $
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
 * $Log: jdi_httpstatus.h,v $
 * Revision 1.10  2008/07/31 08:10:48  browserdevjtbase
 * Core Updates
 *
 * Revision 1.11  2008/07/25 13:52:52  sriramdevhtmlbrow
 * Added new status code 452 for imode
 *
 * Revision 1.10  2008/06/16 11:34:32  sriramdevhtmlbrow
 * Added new http status codes defined by i-mode for MAIL and PUSH purposes under the macro IMODE_HTTP_EX_STATUS_CODE.
 *
 * Revision 1.9  2007/07/13 10:33:42  kumardevhtmlbrow
 * Spelling mistake updated.
 *
 * Revision 1.8  2006/05/23 06:05:19  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2006/01/18 06:03:25  shreyasdevhtmlbrow
 * Added the Header and footer for C++
 *
 * Revision 1.6  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.5  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/07/20 14:22:13  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/07/18 14:29:37  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_httpstatus.h
 * @ingroup CommonUtils
 * @brief It contains the declarations of the enum used by the transport to send the HTTP/WSP
 *	      status code to the application.
 *
 * This file contains the declaration of the enum which is used by the transport to send it status
 * code to the application. i.e the transport chooses the enum value based on the lower stack error
 * and sends that value to the application. 
 */

#ifndef _JDI_HTTP_STATUS_H
#define _JDI_HTTP_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The following enum specifies the various http status code (HTTP/WSP) that can be sent 
 *		  from the transport layer to the application. 
 */
typedef enum _http_status
{
	E_HTTP_STATUS_UNKNOWN				= 0 ,
	E_HTTP_OK							= 200,
	E_HTTP_CREATED						= 201,
	E_HTTP_ACCEPTED						= 202,
	E_HTTP_NON_AUTHORITATIVE			= 203,
	E_HTTP_NO_CONTENT					= 204,
	E_HTTP_RESET_CONTENT				= 205,
	E_HTTP_PARTIAL_CONTENT				= 206,
	E_HTTP_MULTIPLE_CHOICES				= 300,
	E_HTTP_MOVED_PERMANENTLY			= 301,
	E_HTTP_MOVED_TEMPORARILY			= 302,
	E_HTTP_SEE_OTHER					= 303,
	E_HTTP_NOT_MODIFIED					= 304,
	E_HTTP_USE_PROXY					= 305,
	E_HTTP_TEMPORARY_REDIRECT			= 307,
	E_HTTP_BAD_REQUEST					= 400,
	E_HTTP_UNAUTHORIZED					= 401,
	E_HTTP_PAYMENT						= 402,
	E_HTTP_FORBIDDEN					= 403,
	E_HTTP_NOT_FOUND					= 404,
	E_HTTP_METHOD_NOT_ALLOWED			= 405,
	E_HTTP_NOT_ACCEPTABLE				= 406,
	E_HTTP_PROXY_AUTHENTICATION_REQD	= 407,
	E_HTTP_REQUEST_TIMEOUT				= 408,
	E_HTTP_CONFLICT						= 409,
	E_HTTP_GONE							= 410,
	E_HTTP_LENGTH_REQD					= 411,
	E_HTTP_PRECONDITION_FAILED			= 412,
	E_HTTP_REQUEST_ENTITY_TOO_LARGE		= 413,
	E_HTTP_REQUEST_URI_TOO_LARGE		= 414,
	E_HTTP_UNSUPPORTED_MEDIA_TYPE		= 415,
	E_HTTP_REQUEST_RANGE_NOT_SATISFIABLE = 416,
	E_HTTP_EXPECTATION_FAILED			= 417,
	E_HTTP_INTERNAL_SERVER_ERROR		= 500,
	E_HTTP_NOT_IMPLEMENTED				= 501,
	E_HTTP_BAD_GATEWAY					= 502,
	E_HTTP_SERVICE_UNAVAILABLE			= 503,
	E_HTTP_GATEWAY_TIMEOUT				= 504,
	E_HTTP_BAD_VERSION					= 505,
#ifdef IMODE_HTTP_EX_STATUS_CODE
	E_HTTP_MAIL_FINISHED				= 251,
	E_HTTP_MAIL_RECEIVED				= 252,
	E_HTTP_MAIL_EMPTY					= 255,
	E_HTTP_MAIL_ID_ERROR				= 451,
	E_HTTP_MAX_SIZE_ERROR				= 452,
	E_HTTP_MAIL_INBOX_FULL				= 551,
	E_HTTP_MAIL_POST_ERROR				= 552,
	E_HTTP_MAIL_GET_ERROR				= 553,
	E_HTTP_MAIL_UNORDERED				= 554,
	E_HTTP_MAIL_UE_POST_ERROR			= 561,
#endif
	E_HTTP_NONE
} E_HTTP_STATUS ;

#ifdef __cplusplus
}
#endif

#endif


