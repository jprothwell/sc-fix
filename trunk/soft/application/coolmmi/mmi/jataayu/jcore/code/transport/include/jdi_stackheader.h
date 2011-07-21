
/***************************************************************************
*
* File Name : jdi_stackheader.h
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
*
* File Description
* ----------------
*
* Purpose			:
*
*
*
* 
* Created By		: Mallikarjun
* Created Date		: 
*
* Current Revision	:
*
*
***************************************************************************
*
*
* Revision Details
* ----------------
* 
* 1. Modified By, Modified Date, Purpose
* 2. Modified By, Modified Date, Purpose
* 3. Modified By, Modified Date, Purpose
*
*
*
***************************************************************************/

/**
 * @file jdi_stackheader.h
 * @ingroup WAPStack
 * @brief Defines the error codes, enums, structures and the functions which are exposed to use
 *		  the WSP functions
 *
 * This file defines the various functions along with enums and structures which are exposed 
 * for using the WSP functions. In this document this layer will be addressed as a wap stack.  
 */

#ifndef _JDI_STACKHEADER_H
#define _JDI_STACKHEADER_H

#include <jdi_mimeinfo.h>

/**
 * @brief This structure is used to specify the stack header information. A raw header content
 *		  can be parsed based on the header name and stored in the form of linked list by using
 *		  this structure.
 */
typedef struct _stack_headers
{
	JC_INT8 				*name ; /**< Specifies the header name */ 
	JC_INT8					*value ; /**< Specifies the value for the header name */
	struct _stack_headers	*next ; /**< Pointer to the next stack header */
} STACK_HEADERS ;

/**
 * @brief	The application needs to use this function to get the value of a header from 
 *			a raw header buffer (which is not parsed)
 * @param[in]	pcHeader Specifies the header which is to be parsed
 * @param[in]	iHeaderLen Specifies the length of the header
 * @param[in]	pcHeaderName Specifies the name of the header which is to be parsed and
 *				if found the value of this header name is to be returned.
 * @param[out]	ppcHeaderValue The value of the header is returned in this field.
 * @return		JC_RETCODE, Function returns JC_OK if the header is parsed sucessfully, 
				else returns the specific error code.
 *
 * The wap stack parses the given header and searches for the header name "pcHeaderName". If
 * found the function returns the header value to the application.
 */

JC_RETCODE jdi_WSPGetHeaderValue (JC_INT8			*pcHeader,
								  JC_INT32			iHeaderLen,
								  const JC_INT8		*pcHeaderName,
								  JC_INT8			**ppcHeaderValue) ;

/**
 * @brief	The application needs to use this function to parse a header content.
 * @param[in]	pucHeader Specifies the header which is to be parsed
 * @param[in]	iHeaderLength Specifies the length of the header
 * @return		linked list of STACK_HEADERS nodes.
 *
 * The wap stack parses the given header and based on the various header names & the values it
 * forms a linked list of STACK_HEADERS nodes. The function then returns this linked list to the 
 * application.
 */

STACK_HEADERS * jdi_WSPParseHeaders (JC_UINT8		*pucHeader,
									 JC_INT32		iHeaderLength) ;

/**
 * @brief	The application needs to use this function to free a linked list of stack headers.
 * @param[in]	pstStackHeaders Pointer to the starting node of the linked list of stack headers.
 * @return		JC_RETCODE, Function returns JC_OK if the headers are freed sucessfully, 
				else returns the specific error code.
 *
 * The application needs to use this function to free a linked list of stack headers.
 */

JC_RETCODE jdi_WSPFreeOutputHeaders (STACK_HEADERS	*pstStackHeaders) ;

#endif


