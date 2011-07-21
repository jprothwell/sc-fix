/***************************************************************************
 * $Id: jdi_mimeinfo.h,v 1.7 2008/06/16 05:52:17 browserdevjtbase Exp $
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
 * $Log: jdi_mimeinfo.h,v $
 * Revision 1.7  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.14  2007/02/20 05:15:15  kumardevhtmlbrow
 * URI Parse to handle query & fragment
 *
 * Revision 1.13  2007/02/16 11:04:58  kavithadevhtmlbrow
 * Authentication Info moved to mime info
 *
 * Revision 1.12  2006/01/24 16:13:34  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.11  2006/01/19 15:33:03  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.10  2006/01/18 06:03:17  shreyasdevhtmlbrow
 * Added the Header and footer for C++
 *
 * Revision 1.9  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.8  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/08/31 08:11:53  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/08/30 08:03:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/04 07:59:16  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/05/21 11:20:49  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.2  2005/05/13 06:08:48  kumardevhtmlbrow
 * Removed the header file that is not specific to mime info
 *
 * Revision 1.1  2005/05/10 08:02:26  kumardevhtmlbrow
 * Added Mime information header
 *
 * Revision 1.1  2005/05/10 07:56:42  kumardevhtmlbrow
 * Added HTTP Utils function
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_mimeinfo.h
 * @ingroup CommonUtils
 * @brief It contains the declarations of all the structures and enums that are used to
 *		  specify the mime info.
 *
 * The file specifies the structures and enums that are used to specify the mime info. 
 */

#ifndef _JDI_MIME_INFO_H
#define _JDI_MIME_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Specifies the maximum size of the username for the authentication
 */
#define MAX_AUTH_NAME_SIZE			255
/**
 * @brief Specifies the maximum size of the password for the authentication
 */
#define MAX_AUTH_PASSWORD_SIZE		255
/**
 * @brief Specifies the maximum size of the domain for the authentication
 */
#define MAX_AUTH_DOMAIN_SIZE		255
/**
 * @brief Specifies the maximum size of the realm for the authentication
 */
#define MAX_AUTH_REALM_SIZE			255
/**
 * @brief Specifies the maximum size of the nonce for the authentication
 */
#define MAX_AUTH_NONCE_SIZE			255
	/**
 * @brief Specifies the maximum size of the qop for the authentication
 */
#define MAX_AUTH_QOP_SIZE			255
/**
 * @brief Specifies the maximum size of the algorithm for the authentication
 */
#define MAX_AUTH_ALG_SIZE			255
/**
 * @brief Specifies the maximum size of the opaque for the authentication
 */
#define MAX_AUTH_OPAQUE_SIZE		255


/**
 * @brief The following enumeration specifies the type of header value.
 */
typedef enum _header_type
{
	E_HEADER_TYPE_CHAR, /**< Specifies that Header value is a single character */
	E_HEADER_TYPE_INT, /**< Specifies that Header value is integer */
	E_HEADER_TYPE_STRING, /**< Specifies that header value is character string */
	E_HEADER_TYPE_LONG, /**< Specifies that header value is of type long */
	E_HEADER_TYPE_TCHAR /**< Specifies that header value is unicode string */
} E_HEADER_TYPE ;


/**
 * @brief Header value can be in any of the forms mentioned in E_HEADER_TYPE. The union specifies
 *		  the various forms in which the header value can be specified.
 */
typedef union _header_value
{
	JC_INT8 				cValue; /**< Single character header value */
	JC_INT32 				iValue ; /**< Header value in the form of integer */
	JC_INT8 				*pcValue ; /**< Header value in the form of character string */
	JC_UINT64			    ulValue ; /**< Header value as long data type */
	JC_CHAR 				*pmValue ; /**< Header value in the form of unicode string */
} U_HEADER_VALUE ;


/**
 * @brief	Specifies the structure for parameters.
 */
typedef struct parameter_list
{
	JC_INT8 				*pcName; /**< Specifies the parameter name */  
	E_HEADER_TYPE			eType ; /**< Specifies the type of header */
	U_HEADER_VALUE			uHeaderValue ; /**< Specifies the header value */
	struct parameter_list 	*pstNext ; /**< Specifies pointer to the next parameter's details */
} ST_PARAMETER_LIST ;


/**
 * @brief	Specifies the mime header structure which will specify all the details related to
 *			a header. 
 */
typedef struct _mime_headers
{
	JC_INT8 				*pcName ; /**< Specifies the header name */
	E_HEADER_TYPE 			eHeaderType ; /**< Specifies the type of header */  
	U_HEADER_VALUE			uHeaderValue ; /**< Specifies the header value */
	ST_PARAMETER_LIST 		*pstParameterList ; /**< Specifies the parameter list for the header */
	struct _mime_headers 	*pstNext ; /**< Specifies the pointer of the next header details */
} ST_MIME_HEADERS ;

/**
 * @brief	Specifies the multipart information structure which will specify all the details related to
 *			a multipart content. 
 */
typedef struct _multipart_info
{
	JC_UINT8				*pucBuffer ; /**< Specifies the multipart buffer */
	JC_UINT32				uiLength ; /**< Specifies the length of the multipart buffer */
	JC_INT8					*pcContentType ; /**< Specifies the content type of the multipart content */
	ST_MIME_HEADERS			*pstHeaders ; /**< Specifies the headers of the multipart content */
	JC_HANDLE				vHandle ; /**< Specifies the handle for the particular multipart mime headers (pstHeaders) */
	struct _multipart_info	*pstNext ; /**< Specifies the pointer to the next multipart info */
} ST_MULTIPART_INFO ;

/**
 * @brief The enumeration contains the different types of authorization 
 */
typedef enum _auth_type
{
	E_AUTH_BASIC, /**< Specifies that the authorization is of type basic */
	E_AUTH_DIGEST /**< Specifies that the authorization is of type digest */
} E_AUTH_TYPE ;

/**
 * @brief This structure describes the authorization structure 
 */
typedef struct _auth_headers
{
	E_AUTH_TYPE					eAuthType ; /**< Specifies the type of authorization (basic or digest) */
	JC_CHAR						amName [MAX_AUTH_NAME_SIZE + 1] ; /**< Specifies the user name */
	JC_CHAR						amPassword [MAX_AUTH_PASSWORD_SIZE + 1] ; /**< Specifies the password */
	JC_INT8						acDomain [MAX_AUTH_DOMAIN_SIZE + 1] ; /**< Specifies the domain information (in case of digest) */
	JC_INT8						acRealm [MAX_AUTH_REALM_SIZE + 1] ; /**< Specifies the realm information (in case of digest) */
	JC_INT8						acNOnce [MAX_AUTH_NONCE_SIZE + 1] ; /**< Specifies the nonce information (in case of digest) */
	JC_INT8						acQOP [MAX_AUTH_QOP_SIZE + 1] ; /**< Specifies the QOP information (in case of digest) */
	JC_INT8						acAlgorithm [MAX_AUTH_ALG_SIZE + 1] ; /**< Specifies the Algoithm information (in case of digest) */
	JC_INT8						acOpaque [MAX_AUTH_OPAQUE_SIZE + 1] ; /**< Specifies the Opaque information (in case of digest) */
} ST_AUTH_HEADERS ;

#ifdef __cplusplus
}
#endif

#endif
/* End of File */


