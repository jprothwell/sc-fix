/***************************************************************************
 * $Id: jdi_cutils_uriparse.h,v 1.11 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.11 $
 * $Date: 2008/06/16 05:52:17 $
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
 * $Log: jdi_cutils_uriparse.h,v $
 * Revision 1.11  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.17  2007/12/28 12:36:07  kumardevhtmlbrow
 * doxygen changes.
 *
 * Revision 1.16  2007/08/08 07:43:56  kavithadevhtmlbrow
 * jdi_CUtilsTcsDomainCompare added
 *
 * Revision 1.15  2007/07/13 10:14:58  kavithadevhtmlbrow
 * Scheme not handled
 *
 * Revision 1.14  2007/04/18 10:32:25  kumardevhtmlbrow
 * URI Unescape problem.
 *
 * Revision 1.13  2007/02/20 05:15:15  kumardevhtmlbrow
 * URI Parse to handle query & fragment
 *
 * Revision 1.12  2007/01/30 15:17:45  kumardevhtmlbrow
 * Puny code integration
 *
 * Revision 1.11  2007/01/09 14:27:30  kumardevhtmlbrow
 * Added the utility function
 *
 * Revision 1.10  2006/12/22 13:12:58  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.9  2006/12/20 09:01:30  kumardevhtmlbrow
 * Update ...
 *
 * Revision 1.8  2006/08/02 05:00:57  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.6  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/08/17 09:41:37  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/08/17 06:22:00  kumardevhtmlbrow
 * Updated the jdd_MemAlloc to jdi_MemAlloc function.
 *
 * Revision 1.3  2005/07/25 08:10:11  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.2  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/05/16 08:19:09  kumardevhtmlbrow
 * Added URI Parse function.
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_cutils_uriparse.h
 * @ingroup CommonUtils
 * @brief It contains the enumerations, structures and function used for parsing a URI
 *
 * The file contains the enums, structures and functions used to parse a URI and identify 
 * information such as the host name, path name, scheme used etc.
 */

#ifndef _JDI_CUTILS_URIPARSE_H
#define _JDI_CUTILS_URIPARSE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Specifies the modes of parsing a URI
 */
typedef enum _uri_parse
{
	E_URI_PARSE_SCHEME_HOST, /**< Specifies that only host name is to be parsed */
	E_URI_PARSE_SCHEME_HOST_PATH /**< Specifies that both host name and path are to be parsed */
} E_URI_PARSE ;

/**
 * @brief The following enumeration is used to specify the URL info in case URI is made of
 *		  characters. The URI information is formed by parsing the URI 
 */
typedef struct _str_uri_info
{
	JC_INT8			*pcScheme ; /**< Specifies the Scheme used in the URI (for ex HTTP) */
	JC_INT8			*pcHostName ; /**< The host name specified in the URI */
	JC_INT8			*pcPath ; /**< The path name specified in the URI */
	JC_INT32		iPortNumber ; /**< The Port number specified in the URI  */
    JC_INT8         *pcAnchor ; /**< The anchor specified in the URI */
    JC_INT8         *pcQuery ; /**< The query specified in the URI */
} ST_STR_URI_INFO ;

/**
 * @brief The following enumeration is used to specify the URL info in case URI is made of
 *		  unicode characters. The URI information is formed by parsing the URI 
 */
typedef struct _tcs_uri_info
{
	JC_CHAR			*pmScheme ; /**< Specifies the Scheme used in the URI (for ex HTTP) */
	JC_CHAR			*pmHostName ; /**< The host name specified in the URI */
	JC_CHAR			*pmPath ; /**< The path name specified in the URI */
	JC_INT32		iPortNumber ; /**< The Port number specified in the URI  */
	JC_CHAR         *pmAnchor ; /**< The anchor specified in the URI */
    JC_CHAR         *pmQuery ; /**< The query specified in the URI */
} ST_TCS_URI_INFO ;

/**
 * @brief The following enumeration is used to specify the starting and ending index of 
 *		  scheme, host, path and Query in the URI that is parsed. 
 */
typedef struct _uri_index
{
	JC_INT32		iSchemeStartIndex ; /**< Specifies the starting index of Scheme in the URI */
	JC_INT32		iSchemeEndIndex ; /**< Specifies the ending index of scheme in the URI */
	JC_INT32		iAuthorityStartIndex ;
	JC_INT32		iAuthorityEndIndex ;
	JC_INT32		iHostStartIndex ; /**< Specifies the starting index of Host name in the URI */
	JC_INT32		iHostEndIndex ; /**< Specifies the ending index of Host name in the URI */
	JC_INT32		iPathStartIndex ; /**< Specifies the starting index of the Path name in the URI */
	JC_INT32		iPathEndIndex ; /**< Specifies the ending index of the path name in the URI */
	JC_INT32		iQueryStartIndex ; /**< Specifies the starting index of the Query string in the URI */
	JC_INT32		iQueryEndIndex ; /**< Specifies the ending index of the Query string in the URI */
	JC_INT32		iFragmentStartIndex ;
	JC_INT32		iFragmentEndIndex ;
} ST_URI_INDEX ;

/**
 * @brief	  The function is used to parse a URI (in character string format) and return the 
 *			  URI information to the calling function
 * @param[in]	vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pcURI Specifies the URI in character string format to be parsed.
 * @param[in]	eURIParse Specifies the mode of URI parse
 * @param[in,out]	pstURIInfo The function returns the URI information in this structure
 * @param[in,out]	pstURIIndex Specifies the index information of the content that is parsed from the URI
 * @retval		JC_OK if the URI is parsed successfully and the URI info and URI index is retrieved
 * @retval		JC_ERR_MEMORY_ALLOCATION Specifies memory allocation error.
 * @note		The parameter which should not be NULL are pcURI, pstURIInfo and pstURIIndex
 *
 * The function is used to parse a URI (in character string format) and identify information such
 * as the Scheme used, host name, path etc. The function also returns the index of the scheme,
 * host, path and query in the URI.
 */
JC_RETCODE jdi_CUtilsStrURIParse (JC_HANDLE			vMemHandle, 
								  const JC_INT8		*pcURI, 
								  E_URI_PARSE		eURIParse, 
								  ST_STR_URI_INFO	*pstURIInfo,
								  ST_URI_INDEX		*pstURIIndex) ;

/**
 * @brief	  The function is used to free the URI Info structure that was allocated while parsing
 *			  a URI (in character string format)
 * @param[in]	vMemHandle Specifies the memory handle used by the common utils module
 * @param[in,out]	pstURIInfo URI info structure to be freed.
 * @retval		void
 * @note		The parameter which should not be NULL is pstURIInfo
 *
 * The function is used to free all the members of the URI info structure which were allocated memory
 * while parsing a URI (in Character string format).
 */
void jdi_CUtilsStrURIFree (JC_HANDLE		vMemHandle,
						   ST_STR_URI_INFO	*pstURIInfo) ;

/**
 * @brief	  The function is used to parse a URI (in unicode string format) and return the 
 *			  URI information to the calling function
 * @param[in]	vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pmURI Specifies the URI in unicode string format to be parsed.
 * @param[in]	eURIParse Specifies the mode of URI parse
 * @param[in,out]	pstURIInfo The function returns the URI information in this structure
 * @param[in,out]	pstURIIndex Specifies the index information of the content that is parsed from the URI
 * @retval		JC_OK if the URI is parsed successfully and the URI info and URI index is retrieved
 * @retval		JC_ERR_MEMORY_ALLOCATION Specifies memory allocation error.
 * @note		The parameter which should not be NULL are pmURI, pstURIInfo and pstURIIndex
 *
 * The function is used to parse a URI (in unicode string format) and identify information such
 * as the Scheme used, host name, path etc. The function also returns the index of the scheme,
 * host, path and query in the URI.
 */
JC_RETCODE jdi_CUtilsTcsURIParse (JC_HANDLE			vMemHandle, 
								  const JC_CHAR		*pmURI, 
								  E_URI_PARSE		eURIParse, 
								  ST_TCS_URI_INFO	*pstURIInfo, 
								  ST_URI_INDEX		*pstURIIndex) ;

/**
 * @brief			API parses the Host part of the URI
 * @param[in]		pmURI Specifies the URI string
 * @param[in,out]	piHostStartIndex Function returns the start of the host string in this param.
 * @param[in,out]	piHostEndIndex Function returns the end of the host string in this param.
 * @retval JC_OK Returns on successfully parsing the Host URI.
 *
 * The API is used to parse the Host part of the URI (in unicode string format) and return the start
 * and the end index of the host string.
 */

JC_RETCODE jdi_CUtilsTcsURIParseHost (const JC_CHAR		*pmURI, 
									  JC_INT32			*piHostStartIndex,
									  JC_INT32			*piHostEndIndex) ;

/**
 * @brief	  The function is used to free the URI Info structure that was allocated while parsing
 *			  a URI (in unicode string format)
 * @param[in]	vMemHandle Specifies the memory handle used by the common utils module
 * @param[in,out]	pstURIInfo URI info structure to be freed.
 * @retval		void
 * @note		The parameter which should not be NULL is pstURIInfo
 *
 * The function is used to free all the members of the URI info structure which were allocated memory
 * while parsing a URI (in unicode string format).
 */
void jdi_CUtilsTcsURIFree (JC_HANDLE		vMemHandle,
						   ST_TCS_URI_INFO	*pstURIInfo) ;

/**
 * @brief	    Constructs a URI from ST_STR_URI_INFO 
 * @param[in]	vMemHandle Memory handle for allocation
 * @param[in]	pstURIInfo Specifies the parts of URI
 * @param[out]	ppcURI      Newly constructed URI
 * @retval		JC_OK      Indicate Success
 * @retval		JC_ERR_MEMORY_ALLOCATION Indicate memory allocation error.
 *
 * Application shall free memory allocated for ppcURI using jdi_MemFree.
 */
JC_RETCODE jdi_CUtilsStrURIConstruct (JC_HANDLE			vMemHandle, 
								      ST_STR_URI_INFO	*pstURIInfo,
								      JC_INT8   		**ppcURI
                                     ) ;

/**
 * @brief	    Constructs Host from ST_STR_URI_INFO 
 * @param[in]	vMemHandle Memory handle for allocation
 * @param[in]	pstURIInfo Specifies the parts of URI
 * @param[out]	ppcHost    Host of given URI
 * @retval		JC_OK      Indicate Success
 * @retval		JC_ERR_MEMORY_ALLOCATION Indicate memory allocation error.
 *
 * Application shall free memory allocated for ppcHost using jdi_MemFree.
 * Returned host is hostname:port format. port is not appeneded if it is default
 * port for the given scheme.
 * 
 */
JC_RETCODE jdi_CUtilsStrURIConstructHost (JC_HANDLE			vMemHandle, 
								          ST_STR_URI_INFO	*pstURIInfo,
								          JC_INT8   		**ppcHost
                                         ) ;

/**
 * @brief	    Constructs port from ST_STR_URI_INFO 
 * @param[in]	vMemHandle Memory handle for allocation
 * @param[in]	pstURIInfo Specifies the parts of URI
 * @param[out]	ppcPort    Port of given URI
 * @retval		JC_OK      Indicate Success
 * @retval		JC_ERR_MEMORY_ALLOCATION Indicate memory allocation error.
 *
 * Application shall free memory allocated for ppcPort using jdi_MemFree.
 * 
 */
JC_RETCODE jdi_CUtilsStrURIConstructPort (JC_HANDLE			vMemHandle, 
    							          ST_STR_URI_INFO	*pstURIInfo,
								          JC_INT8   		**ppcPort
                                         ) ;

/**
 * @brief	    Parse Host and update given ST_STR_URI_INFO 
 * @param[in]	vMemHandle Memory handle for allocation
 * @param[in]	pcHost     Host to be parsed
 * @param[in]	pstURIInfo Specifies the parts of URI
 * @retval		JC_OK      Indicate Success
 * @retval		JC_ERR_MEMORY_ALLOCATION Indicate memory allocation error.
 * 
 */
JC_RETCODE jdi_CUtilsStrURIParseHost (JC_HANDLE			vMemHandle, 
                                      JC_INT8           *pcHost, 
                                      ST_STR_URI_INFO	*pstURIInfo
                                     ) ;

/**
 * @brief	This function is used to compare the two URI
 * @param[in] pmFirstURI Specifies the first URI
 * @param[in] pmSecondURI Specifies the second URI that needs to be compared
 * This function returns E_TRUE when it is compared, otherwise E_FALSE.
 */
JC_BOOLEAN jdi_CUtilsTcsURICompare (const JC_CHAR *pmFirstURI, const JC_CHAR *pmSecondURI) ;

/**
 * @brief	This function is used to compare the two URI domain
 * @param[in] pmFirstURI Specifies the first URI/domain
 * @param[in] pmSecondURI Specifies the second URI/domain that needs to be compared
 * This function returns E_TRUE when it is compared, otherwise E_FALSE.
 */
JC_BOOLEAN jdi_CUtilsTcsDomainCompare (const JC_CHAR	*pmFirstURI, 
									   const JC_CHAR	*pmSecondURI) ;

/**
 * @brief	    This function parses the URI in JC_CHAR format and provides the index 
 *				to the different component inside the URI.If required it can also 
 *				provide the different components in ST_STR_URI_INFO structure.
 * @param[in]	vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pmURI Specifies the URI in character string format to be parsed.
 * @param[in,out]	pstURIInfo The function returns the URI information in this structure
 * @param[in,out]	pstURIIndex Specifies the index information of the content that is parsed from the URI
 * @retval		JC_OK if the URI is parsed successfully and the URI info and URI index is retrieved
 * @retval		JC_ERR_MEMORY_ALLOCATION Specifies memory allocation error.
 * @note		The parameter which should not be NULL are pmURI, pstURIInfo and pstURIIndex
 *
 */
JC_RETCODE jdi_CUtilsTcsURIGetComponent (JC_HANDLE		vMemHandle, 
									   const JC_CHAR	*pmURI, 
									   ST_URI_INDEX		*pstURIIndex, 
									   ST_TCS_URI_INFO	*pstURIInfo) ;

/**
 * @brief	    This function parses the URI in JC_INT8 format and provides the index 
 *				to the different component inside the URI.If required it can also 
 *				provide the different components in ST_STR_URI_INFO structure.
 * @param[in]	vMemHandle Specifies the memory handle used by the common utils module
 * @param[in]	pcURI Specifies the URI in character string format to be parsed.
 * @param[in,out]	pstURIInfo The function returns the URI information in this structure
 * @param[in,out]	pstURIIndex Specifies the index information of the content that is parsed from the URI
 * @retval		JC_OK if the URI is parsed successfully and the URI info and URI index is retrieved
 * @retval		JC_ERR_MEMORY_ALLOCATION Specifies memory allocation error.
 * @note		The parameter which should not be NULL are pcURI, pstURIInfo and pstURIIndex
 *
 */

JC_RETCODE jdi_CUtilsStrURIGetComponent (JC_HANDLE		vMemHandle, 
									   const JC_INT8	*pcURI, 
									   ST_URI_INDEX		*pstURIIndex,
									   ST_STR_URI_INFO	*pstURIInfo) ; 

/**
 * @brief		This function unescape the URI.
 * @param[in]	pmURL	Specifies the URI which needs to be unescaped.
 * 
 * @retval		JC_OK if the URI is unescaped successfully.
 * 
 * If the input is http://www.jataayusoft.com/query%20string/ then the result shall be
 * http://www.jataayusoft.com/query string/
 */
JC_RETCODE jdi_CUtilsTcsURIUnescape (JC_CHAR *pmURL) ;

JC_BOOLEAN	jdi_CUtilsTcsURIIsSchemeNotHandled (const JC_CHAR *pmURI) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


