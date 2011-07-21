/***************************************************************************
 * $Id: jcutils.h,v 1.6 2008/08/14 08:25:27 browserdevjtbase Exp $
 * $Revision: 1.6 $
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
 * $Log: jcutils.h,v $
 * Revision 1.6  2008/08/14 08:25:27  browserdevjtbase
 * Core Updates
 *
 * Revision 1.5  2007/12/27 15:05:01  browserdevjtbase
 * Core Update.
 *
 * Revision 1.4  2007/01/12 11:18:05  browserdevjtbase
 * Update from the main base
 *
 * Revision 1.3  2006/12/28 09:47:59  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.7  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.5  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.4  2005/08/20 10:36:20  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/04 08:00:32  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/

#ifndef _JCUTILS_H
#define _JCUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @file jcutils.h
 * @ingroup general
 * @brief Defines the generic macro's and mapping of the jc_tcs function to the specific function
 *
 * When <b>HAVE_UNICODE</b> is enabled then JC_CHAR datatype uses multi-byte data type say unsigned short.
 * In this case all jc_tcsXX function's are mapped to jc_wstrXX functions. If <b>HAVE_UNICODE</b> is not defined
 * JC_CHAR datatypes is considered as JC_INT8 and jc_tcsXX function's are mapped to jc_strXX functions.
 */

/**
 * @brief Defines the datatypes for storing the unknown datatype.
 */
typedef void *				JC_HANDLE ;

#ifndef JCF_ALLOW_PLATFORM_VALUE
/**
 * @brief Defines the boolean data type with the E_FALSE & E_TRUE.
 */
/*
typedef enum e_boolean
{
	E_FALSE,
	E_TRUE
} JC_BOOLEAN ;
*/
typedef JC_INT8			JC_BOOLEAN ;
#define E_FALSE			0
#define E_TRUE			1
#endif

/** @cond */

#define PARAM_INTENTIONALLY_NOT_USED(x) ((void)(x))
#define NET_MODULE						TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0100)
#define TASK_MODULE						TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0200)
#define QUEUE_MODULE					TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0300)
#define SEMAPHORE_MODULE				TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0400)
#define FILE_MODULE 					TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0500)
#define TIMER_MODULE					TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0600)
#define TAPI_MODULE						TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0700)
#define DATA_CONN_MODULE				TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0800)
#define AUDIO_MODULE					TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0900)
#define FLOAT_MODULE					TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0A00)
#define MATH_MODULE						TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0B00)
#define OTHER_MODULE					TERMINAL_MODULE_CODE(ABSTRACT_APPLICATION, 0x0C00)


#define HAVE_UNICODE

#ifdef HAVE_UNICODE

#define jc_tcslen 		jc_wstrlen
#define jc_tcscpy 		jc_wstrcpy
#define jc_tcscmp 		jc_wstrcmp
#define jc_tcsncmp 		jc_wstrncmp
#define jc_tcscasecmp 	jc_wstrcasecmp
#define jc_tcsncasecmp 	jc_wstrncasecmp
#define jc_tcscat 		jc_wstrcat
#define jc_tcsncat		jc_wstrncat
#define jc_tcsncpy		jc_wstrncpy
#define jc_tcsrchr		jc_wstrrchr
#define jc_tcsstr		jc_wstrstr
#define jc_tcschr		jc_wstrchr
#define jc_itot			jc_itow
//#define jc_tcstod		jc_wstrtod
#else

#define jc_tcslen 		jc_strlen
#define jc_tcscpy 		jc_strcpy
#define jc_tcscmp 		jc_strcmp
#define jc_tcscasecmp 	jc_strcasecmp
#define jc_tcsncasecmp 	jc_strncasecmp
#define jc_tcscat 		jc_strcat
#define jc_tcsncat		jc_strncat
#define jc_tcsncpy		jc_strncpy
#define jc_tcsrchr		jc_strrchr
#define jc_tcsstr		jc_strstr
#define jc_tcschr		jc_strchr
#define jc_itot			jc_itoa
//#define jc_tcstod		jc_strtod
#define	jc_ttol			jc_strtol
#endif

/** @endcond */

/**
 * @brief Specifies the macro to convert the specified value to the lower case.
 */
#define TOLOWER(x)				((x > 64 && x < 91) ? x + 32 : x)

/**
 * @brief Specifies the macro to calculate the maximum of the given two numbers.
 */
#define jc_max(x,y)				((x) > (y) ? (x) : (y)) 

/**
 * @brief Specifies the macro to calculate the minimum of the given two numbers.
 */
#define jc_min(x,y)				((x) < (y) ? (x) : (y)) 

/**
 * @brief
 */
#define JC_MOD_4(x)				((x) & 0x03)
#define JC_MOD_8(x)				((x) & 0x07)

#define JC_DIV_8(x)				((x) >> 3)

#define JC_MAX_CHAR_SIZE		0xFF

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


