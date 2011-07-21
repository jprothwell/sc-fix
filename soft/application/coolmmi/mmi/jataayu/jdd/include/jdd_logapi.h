/***************************************************************************
 * $Id: jdd_logapi.h,v 1.3 2006/11/16 11:32:26 browserdevjtbase Exp $
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
 * $Log: jdd_logapi.h,v $
 * Revision 1.3  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.11  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.9  2005/12/31 12:15:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/12/22 08:37:20  kavithadevhtmlbrow
 * Log TCS Updated
 *
 * Revision 1.7  2005/12/16 13:29:40  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2005/12/12 10:42:25  kumardevhtmlbrow
 * Updated the log and provided few logging in transport module
 *
 * Revision 1.5  2005/12/09 15:15:36  kumardevhtmlbrow
 * New LOG abstraction
 *
 *
 ***************************************************************************/

/**
 * @file jdd_logapi.h
 * @ingroup Log
 * @brief Defines the JDD logging macro
 *
 * <p> The JDD log sub-system enables the flexible use of logs in source code.
 * It provides the following family of macros for logging:
 *   - LOG_LEVEL_FATALx
 *   - LOG_LEVEL_WARNINGx
 *   - LOG_LEVEL_INFORMATIONx
 *
 * where, x is a digit from 1 to 6, that denotes the number of additional
 * parameters. </p>
 *
 * <p> Logging can be selectively or completely excluded from compilation
 * with the help of the following configuration flags:
 * <ul>
 * <li> __MODULE_ID__ (Enable logging only for specific modules in the source, which is controlled by the switch)
 * <li> JC_LOG_LEVEL (Enable only the logs of specified level and below)
 * <li> JC_LOG_FILE (Enable specifying the source full file path or file name or NULL)
 * </ul>
 * </p>
 * @note The jdd_Logxxx functions should never be called directly in the source.
 * Instead the appropriate log macros must be used.
 */

#ifndef _JDD_LOG_API_H
#define _JDD_LOG_API_H
//#define JDD_LOG_ENABLED
//#include "mmi_trace.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Used to disable the log level
 */
#define LOG_LEVEL_NONE				0
/**
 * @brief Used to enable the log for fatal
 */
#define LOG_LEVEL_FATAL				1
/**
 * @brief Used to enable the log for warning and fatal message
 */
#define LOG_LEVEL_WARNING			2
/**
 * @brief Used to enable the log for information, warning and fatal message
 */
#define LOG_LEVEL_INFORMATION		3

/**
 * @brief Used to disable the source file name of the log message
 */
#define LOG_FILE_NONE				0
/**
 * @brief Used to enable the full path of the source file name using __FILE__ macro for the log message.
 */
#define LOG_FILE_NAME				1
/**
 * @brief Used to enable only the file information in the log message.
 */
#define LOG_FILE_MODULE_INFO		2

/** @cond */

#define LOG_BINARY_DUMP				0
#define LOG_BINARY_MEM				0
#define LOG_BINARY_HEX				0

#define JC_LOG_LEVEL				LOG_LEVEL_INFORMATION
#define JC_LOG_FILE					LOG_FILE_MODULE_INFO

#if (JC_LOG_FILE == LOG_FILE_NAME)
    #define __FILE_ID__			__FILE__
#elif (JC_LOG_FILE == LOG_FILE_MODULE_INFO)
    #define __FILE_ID__			FILE_NAME
#else
	#define __FILE_ID__			NULL
#endif
//#define DETAILED_LOG mmi_trace(1,(PCSTR)"JAT File:%s Line:%d",__FILE__,__LINE__) 
#define DETAILED_LOG 

/*	Original Log	*/
#if 0
#define JDD_LOG0(logInfo, pucFormat)  DETAILED_LOG ;	mmi_trace(1, (PCSTR)pucFormat)

#define JDD_LOG1(logInfo, pucFormat, p1) DETAILED_LOG ; mmi_trace(1, (PCSTR)pucFormat, p1)

#define JDD_LOG2(logInfo, pucFormat, p1, p2) DETAILED_LOG ; mmi_trace(1, (PCSTR)pucFormat, p1, p2)

#define JDD_LOG3(logInfo, pucFormat, p1, p2, p3) DETAILED_LOG ; mmi_trace(1, (PCSTR)pucFormat, p1, p2, p3)

#define JDD_LOG4(logInfo, pucFormat, p1, p2, p3, p4) DETAILED_LOG ; mmi_trace(1, (PCSTR)pucFormat, p1, p2, p3, p4)

#define JDD_LOG5(logInfo, pucFormat, p1, p2, p3, p4, p5) DETAILED_LOG ; mmi_trace(1, (PCSTR)pucFormat, p1, p2, p3, p4, p5)

#define JDD_LOG_TCS(logInfo, pucMsg, pmValue) \
	jdd_LogMsgTcs(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pmValue)

#define JDD_LOG_HEX(logInfo, pucMsg, pucDumpInfo, iLength) \
	jdd_LogDumpHex(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pucDumpInfo, iLength)

#define JDD_LOG_MEM(logInfo, pucMsg, pucDumpInfo, iLength) \
	jdd_LogDumpMem(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pucDumpInfo, iLength)

#endif

/*	Log to File	*/
#if 0
void wapBrowserLog(char *aFmt, ... ) ;

#define JDD_LOG0(logInfo, pucFormat)  wapBrowserLog ("JAT_LOG: File:%s Line:%d " pucFormat,__FILE__,__LINE__)

#define JDD_LOG1(logInfo, pucFormat, p1) wapBrowserLog ("JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1)

#define JDD_LOG2(logInfo, pucFormat, p1, p2) wapBrowserLog ("JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1, p2)

#define JDD_LOG3(logInfo, pucFormat, p1, p2, p3) wapBrowserLog ("JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1, p2, p3)

#define JDD_LOG4(logInfo, pucFormat, p1, p2, p3, p4) wapBrowserLog ("JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1, p2, p3, p4)

#define JDD_LOG5(logInfo, pucFormat, p1, p2, p3, p4, p5) wapBrowserLog ("JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1, p2, p3, p4, p5)

#define JDD_LOG_TCS(logInfo, pucMsg, pmValue) \
	jdd_LogMsgTcs(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pmValue)

#define JDD_LOG_HEX(logInfo, pucMsg, pucDumpInfo, iLength) \
	jdd_LogDumpHex(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pucDumpInfo, iLength)

#define JDD_LOG_MEM(logInfo, pucMsg, pucDumpInfo, iLength) \
	jdd_LogDumpMem(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pucDumpInfo, iLength)
#endif


/*	Log with single line prefixed with one identifier	with file name and line number */
#if 0
#define JDD_LOG0(logInfo, pucFormat)  DETAILED_LOG ;	mmi_trace(1, "JAT_LOG: File:%s Line:%d " pucFormat,__FILE__,__LINE__)

#define JDD_LOG1(logInfo, pucFormat, p1) DETAILED_LOG ; mmi_trace(1, "JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1)

#define JDD_LOG2(logInfo, pucFormat, p1, p2) DETAILED_LOG ; mmi_trace(1, "JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1, p2)

#define JDD_LOG3(logInfo, pucFormat, p1, p2, p3) DETAILED_LOG ; mmi_trace(1, "JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1, p2, p3)

#define JDD_LOG4(logInfo, pucFormat, p1, p2, p3, p4) DETAILED_LOG ; mmi_trace(1, "JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1, p2, p3, p4)

#define JDD_LOG5(logInfo, pucFormat, p1, p2, p3, p4, p5) DETAILED_LOG ; mmi_trace(1, "JAT_LOG: File:%s Line:%d " pucFormat, __FILE__,__LINE__, p1, p2, p3, p4, p5)

#define JDD_LOG_TCS(logInfo, pucMsg, pmValue) \
	jdd_LogMsgTcs(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pmValue)

#define JDD_LOG_HEX(logInfo, pucMsg, pucDumpInfo, iLength) \
	jdd_LogDumpHex(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pucDumpInfo, iLength)

#define JDD_LOG_MEM(logInfo, pucMsg, pucDumpInfo, iLength) \
	jdd_LogDumpMem(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pucDumpInfo, iLength)
#endif

/*	Log with single line prefixed with one identifier	*/
#ifdef JDD_LOG_ENABLED
#define JDD_LOG0(logInfo, pucFormat)  DETAILED_LOG ;	mmi_trace(12, "JAT_LOG: " pucFormat)

#define JDD_LOG1(logInfo, pucFormat, p1) DETAILED_LOG ; mmi_trace(12, "JAT_LOG: " pucFormat, p1)

#define JDD_LOG2(logInfo, pucFormat, p1, p2) DETAILED_LOG ; mmi_trace(12, "JAT_LOG: " pucFormat, p1, p2)

#define JDD_LOG3(logInfo, pucFormat, p1, p2, p3) DETAILED_LOG ; mmi_trace(12, "JAT_LOG: " pucFormat, p1, p2, p3)

#define JDD_LOG4(logInfo, pucFormat, p1, p2, p3, p4) DETAILED_LOG ; mmi_trace(12, "JAT_LOG: " pucFormat, p1, p2, p3, p4)

#define JDD_LOG5(logInfo, pucFormat, p1, p2, p3, p4, p5) DETAILED_LOG ; mmi_trace(12, "JAT_LOG: " pucFormat, p1, p2, p3, p4, p5)

#define JDD_LOG_TCS(logInfo, pucMsg, pmValue) \
	jdd_LogMsgTcs(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pmValue)

#define JDD_LOG_HEX(logInfo, pucMsg, pucDumpInfo, iLength) \
	jdd_LogDumpHex(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pucDumpInfo, iLength)

#define JDD_LOG_MEM(logInfo, pucMsg, pucDumpInfo, iLength) \
	jdd_LogDumpMem(logInfo, __MODULE_ID__, __FILE_ID__, __LINE__, pucMsg, pucDumpInfo, iLength)
#else

/** @cond */
#define JDD_LOG0(pucFormat)
#define JDD_LOG1(pucFormat, p1)
#define JDD_LOG2(pucFormat, p1, p2)
#define JDD_LOG3(pucFormat, p1, p2, p3)
#define JDD_LOG4(pucFormat, p1, p2, p3, p4)
#define JDD_LOG5(pucFormat, p1, p2, p3, p4, p5)

#define JDD_LOG_TCS(pucMsg, pmValue)
#define JDD_LOG_HEX(pucMsg, pucDumpInfo, iLength)
#define JDD_LOG_MEM(pucMsg, pucDumpInfo, iLength)
#endif

/** @endcond */

//#if	((JC_LOG_LEVEL >= LOG_LEVEL_INFORMATION) && (__MODULE_ID__ != 0))
#ifdef JDD_LOG_ENABLED
//#if 1
/**
 * @brief Used to log the information message with one parameter.
 */
#define JDD_LOG_INFO0(pucFormat)                     JDD_LOG0(LOG_LEVEL_INFORMATION, pucFormat)
/**
 * @brief Used to log the information message with two parameter, where the first parameter should be format value.
 */
#define JDD_LOG_INFO1(pucFormat, p1)                 JDD_LOG1(LOG_LEVEL_INFORMATION, pucFormat, p1)
/**
 * @brief Used to log the information message with three parameter, where the first parameter should be format value.
 */
#define JDD_LOG_INFO2(pucFormat, p1, p2)             JDD_LOG2(LOG_LEVEL_INFORMATION, pucFormat, p1, p2)
/**
 * @brief Used to log the information message with four parameter, where the first parameter should be format value.
 */
#define JDD_LOG_INFO3(pucFormat, p1, p2, p3)         JDD_LOG3(LOG_LEVEL_INFORMATION, pucFormat, p1, p2, p3)
/**
 * @brief Used to log the information message with five parameter, where the first parameter should be format value.
 */
#define JDD_LOG_INFO4(pucFormat, p1, p2, p3, p4)     JDD_LOG4(LOG_LEVEL_INFORMATION, pucFormat, p1, p2, p3, p4)
/**
 * @brief Used to log the information message with six parameter, where the first parameter should be format value.
 */
#define JDD_LOG_INFO5(pucFormat, p1, p2, p3, p4, p5) JDD_LOG5(LOG_LEVEL_INFORMATION, pucFormat, p1, p2, p3, p4, p5)
/**
 * @brief Used to log the information message with two parameter, where the first parameter should be JC_INT8 string
 * and second parameter should be of type JC_CHAR *
 */
#define JDD_LOG_INFO_TCS(pucMsg, pmValue)				JDD_LOG_TCS(LOG_LEVEL_INFORMATION, pucMsg, pmValue)

#if (LOG_BINARY_HEX)
#define JDD_LOG_INFO_HEX(pucMsg, pucDumpInfo, iLength)	JDD_LOG_HEX(LOG_LEVEL_INFORMATION, pucDumpInfo, iLength)
#else
/** @cond */
#define JDD_LOG_INFO_HEX(pucMsg, pucDumpInfo, iLength)
/** @endcond */
#endif

#if (LOG_BINARY_MEM)
#define JDD_LOG_INFO_MEM(pucMsg, pucDumpInfo, iLength)	JDD_LOG_MEM(LOG_LEVEL_INFORMATION, pucDumpInfo, iLength)
#else
/** @cond */
#define JDD_LOG_INFO_MEM(pucMsg, pucDumpInfo, iLength)
/** @endcond */
#endif

#else

/** @cond */
#define JDD_LOG_INFO0(pucFormat)
#define JDD_LOG_INFO1(pucFormat, p1)
#define JDD_LOG_INFO2(pucFormat, p1, p2)
#define JDD_LOG_INFO3(pucFormat, p1, p2, p3)
#define JDD_LOG_INFO4(pucFormat, p1, p2, p3, p4)
#define JDD_LOG_INFO5(pucFormat, p1, p2, p3, p4, p5)

#define JDD_LOG_INFO_TCS(pucMsg, pmValue)
#define JDD_LOG_INFO_HEX(pucMsg, pucDumpInfo, iLength)
#define JDD_LOG_INFO_MEM(pucMsg, pucDumpInfo, iLength)
/** @endcond */
#endif /* LOG_LEVEL_INFORMATION */

#if ((JC_LOG_LEVEL >= LOG_LEVEL_WARNING) && (0 != __MODULE_ID__))
/**
 * @brief Used to log the warning message with one parameter.
 */
#define JDD_LOG_WARNING0(pucFormat)                     JDD_LOG0(LOG_LEVEL_WARNING, pucFormat)
/**
 * @brief Used to log the warning message with two parameter, where the first parameter should be format value.
 */
#define JDD_LOG_WARNING1(pucFormat, p1)                 JDD_LOG1(LOG_LEVEL_WARNING, pucFormat, p1)
/**
 * @brief Used to log the warning message with three parameter, where the first parameter should be format value.
 */
#define JDD_LOG_WARNING2(pucFormat, p1, p2)             JDD_LOG2(LOG_LEVEL_WARNING, pucFormat, p1, p2)
/**
 * @brief Used to log the warning message with four parameter, where the first parameter should be format value.
 */
#define JDD_LOG_WARNING3(pucFormat, p1, p2, p3)         JDD_LOG3(LOG_LEVEL_WARNING, pucFormat, p1, p2, p3)
/**
 * @brief Used to log the warning message with five parameter, where the first parameter should be format value.
 */
#define JDD_LOG_WARNING4(pucFormat, p1, p2, p3, p4)     JDD_LOG4(LOG_LEVEL_WARNING, pucFormat, p1, p2, p3, p4)
/**
 * @brief Used to log the warning message with six parameter, where the first parameter should be format value.
 */
#define JDD_LOG_WARNING5(pucFormat, p1, p2, p3, p4, p5) JDD_LOG5(LOG_LEVEL_WARNING, pucFormat, p1, p2, p3, p4, p5)
/**
 * @brief Used to log the warning message with two parameter, where the first parameter should be JC_INT8 string
 * and second parameter should be of type JC_CHAR *
 */
#define JDD_LOG_WARNING_TCS(pucMsg, pmValue)				JDD_LOG_TCS(LOG_LEVEL_WARNING, pucMsg, pmValue)

#if (LOG_BINARY_HEX)
#define JDD_LOG_WARNING_HEX(pucMsg, pucDumpInfo, iLength)	JDD_LOG_HEX(LOG_LEVEL_WARNING, pucDumpInfo, iLength)
#else
/** @cond */
#define JDD_LOG_WARNING_HEX(pucMsg, pucDumpInfo, iLength)
/** @endcond */
#endif

#if (LOG_BINARY_MEM)
#define JDD_LOG_WARNING_MEM(pucMsg, pucDumpInfo, iLength)	JDD_LOG_MEM(LOG_LEVEL_WARNING, pucDumpInfo, iLength)
#else
/** @cond */
#define JDD_LOG_WARNING_MEM(pucMsg, pucDumpInfo, iLength)
/** @endcond */
#endif

#else

/** @cond */
#define JDD_LOG_WARNING0(pucFormat)
#define JDD_LOG_WARNING1(pucFormat, p1)
#define JDD_LOG_WARNING2(pucFormat, p1, p2)
#define JDD_LOG_WARNING3(pucFormat, p1, p2, p3)
#define JDD_LOG_WARNING4(pucFormat, p1, p2, p3, p4)
#define JDD_LOG_WARNING5(pucFormat, p1, p2, p3, p4, p5)

#define JDD_LOG_WARNING_TCS(pucMsg, pmValue)
#define JDD_LOG_WARNING_HEX(pucMsg, pucDumpInfo, iLength)
#define JDD_LOG_WARNING_MEM(pucMsg, pucDumpInfo, iLength)
/** @endcond */

#endif /* LOG_LEVEL_WARNING */

#if ((JC_LOG_LEVEL >= LOG_LEVEL_FATAL) && (0 != __MODULE_ID__))
/**
 * @brief Used to log the fatal message with one parameter.
 */
#define JDD_LOG_FATAL0(pucFormat)                     JDD_LOG0(LOG_LEVEL_FATAL, pucFormat)
/**
 * @brief Used to log the fatal message with two parameter, where the first parameter should be format value.
 */
#define JDD_LOG_FATAL1(pucFormat, p1)                 JDD_LOG1(LOG_LEVEL_FATAL, pucFormat, p1)
/**
 * @brief Used to log the fatal message with three parameter, where the first parameter should be format value.
 */
#define JDD_LOG_FATAL2(pucFormat, p1, p2)             JDD_LOG2(LOG_LEVEL_FATAL, pucFormat, p1, p2)
/**
 * @brief Used to log the fatal message with four parameter, where the first parameter should be format value.
 */
#define JDD_LOG_FATAL3(pucFormat, p1, p2, p3)         JDD_LOG3(LOG_LEVEL_FATAL, pucFormat, p1, p2, p3)
/**
 * @brief Used to log the fatal message with five parameter, where the first parameter should be format value.
 */
#define JDD_LOG_FATAL4(pucFormat, p1, p2, p3, p4)     JDD_LOG4(LOG_LEVEL_FATAL, pucFormat, p1, p2, p3, p4)
/**
 * @brief Used to log the fatal message with six parameter, where the first parameter should be format value.
 */
#define JDD_LOG_FATAL5(pucFormat, p1, p2, p3, p4, p5) JDD_LOG5(LOG_LEVEL_FATAL, pucFormat, p1, p2, p3, p4, p5)
/**
 * @brief Used to log the fatal message with two parameter, where the first parameter should be JC_INT8 string
 * and second parameter should be of type JC_CHAR *
 */
#define JDD_LOG_FATAL_TCS(pucMsg, pmValue)				JDD_LOG_TCS(LOG_LEVEL_FATAL, pucMsg, pmValue)
#if (LOG_BINARY_HEX)
#define JDD_LOG_FATAL_HEX(pucMsg, pucDumpInfo, iLength)	JDD_LOG_HEX(LOG_LEVEL_FATAL, pucDumpInfo, iLength)
#else
/** @cond */
#define JDD_LOG_FATAL_HEX(pucMsg, pucDumpInfo, iLength)
/** @endcond */
#endif

#if (LOG_BINARY_MEM)
#define JDD_LOG_FATAL_MEM(pucMsg, pucDumpInfo, iLength)	JDD_LOG_MEM(LOG_LEVEL_FATAL, pucDumpInfo, iLength)
#else
/** @cond */
#define JDD_LOG_FATAL_MEM(pucMsg, pucDumpInfo, iLength)
/** @endcond */
#endif

#else

/** @cond */
#define JDD_LOG_FATAL0(pucFormat)
#define JDD_LOG_FATAL1(pucFormat, p1)
#define JDD_LOG_FATAL2(pucFormat, p1, p2)
#define JDD_LOG_FATAL3(pucFormat, p1, p2, p3)
#define JDD_LOG_FATAL4(pucFormat, p1, p2, p3, p4)
#define JDD_LOG_FATAL5(pucFormat, p1, p2, p3, p4, p5)

#define JDD_LOG_FATAL_TCS(pucMsg, pmValue)
#define JDD_LOG_FATAL_HEX(pucMsg, pucDumpInfo, iLength)
#define JDD_LOG_FATAL_MEM(pucMsg, pucDumpInfo, iLength)
/** @endcond */

#endif /* LOG_LEVEL_FATAL */

/**
 * @brief The Log layer shall be initialized using this API.
 * @param[in] pvLogReference Specifies the reference which shall be used by the log module
 *
 * This function should be invoked by the application layer before invoking the Jataayu
 * terminal components.
 * @note This should be part of the application startup code.
 */
void jdd_LogInitialize (void *pvLogReference) ;
/**
 * @brief The Log layer shall be deinitialized using this API.
 * @param[in] pvLogReference Specifies the reference which shall be used by the log module
 *
 * This function should be invoked by the application layer after deinitializing all the
 * Jataayu terminal components.
 */
void jdd_LogDeInitialize (void *pvLogReference) ;


/**
 * @brief     JDD API for logging the message
 * @param[in] logInfo The log level (e.g. LOG_LEVEL_INFORMATION/LOG_LEVEL_WARNING/LOG_LEVEL_FATAL)
 * @param[in] iModuleNum The ID of the module where the log is placed
 * @param[in] pcFileName The full path of the source file where the log is
 *              placed, or module name or NULL if LOG_FILE_NAMES is turned off.
 * @param[in] iLineNumber The line number in the source file where the log is placed
 * @param[in] pcFormat The printf-like format string of the log statement.
 * @param[in] ... The variable arguments ...
 */
void jdd_LogMsg (JC_INT32 logInfo, JC_INT32 iModuleNum, const JC_INT8 *pcFileName, JC_INT32 iLineNumber, const JC_INT8 *pcFormat, ...) ;

/**
 * @brief     JDD API for logging for JC_CHAR format
 * @param[in] logInfo The log level (e.g. LOG_LEVEL_INFORMATION/LOG_LEVEL_WARNING/LOG_LEVEL_FATAL)
 * @param[in] iModuleNum The ID of the module where the log is placed
 * @param[in] pcFileName The full path of the source file where the log is
 *              placed, or module name or NULL if LOG_FILE_NAMES is turned off.
 * @param[in] iLineNumber The line number in the source file where the log is placed
 * @param[in] pcMsg The Message associated to the JC_CHAR string.
 * @param[in] pmValue The Value which nees to be logged
 */
void jdd_LogMsgTcs (JC_INT32 logInfo, JC_INT32 iModuleNum, const JC_INT8 *pcFileName, JC_INT32 iLineNumber, const JC_INT8 *pcMsg, const JC_CHAR *pmValue) ;

/**
 * @brief     JDD API for dumping the data in the hex format
 * @param[in] logInfo The log level (e.g. LOG_LEVEL_INFORMATION/LOG_LEVEL_WARNING/LOG_LEVEL_FATAL)
 * @param[in] iModuleNum The ID of the module where the log is placed
 * @param[in] pcFileName The full path of the source file where the log is
 *              placed, or module name or NULL if LOG_FILE_NAMES is turned off.
 * @param[in] iLineNumber The line number in the source file where the log is placed
 * @param[in] pcMsgInfo The Message associated to the data.
 * @param[in] pucDumpInfo The content which nees to be dumped as the hex format in the log
 * @param[in] iLength The Size of the content
 */
void jdd_LogDumpHex (JC_INT32 logInfo, JC_INT32 iModuleNum, const JC_INT8 *pcFileName, JC_INT32 iLineNumber, const JC_INT8 *pcMsgInfo, JC_UINT8 *pucDumpInfo, JC_INT32 iLength) ;

/**
 * @brief     JDD API for logging the data in the same content
 * @param[in] logInfo The log level (e.g. LOG_LEVEL_INFORMATION/LOG_LEVEL_WARNING/LOG_LEVEL_FATAL)
 * @param[in] iModuleNum The ID of the module where the log is placed
 * @param[in] pcFileName The full path of the source file where the log is
 *              placed, or module name or NULL if LOG_FILE_NAMES is turned off.
 * @param[in] iLineNumber The line number in the source file where the log is placed
 * @param[in] pcMsgInfo The Message associated to the data.
 * @param[in] pucDumpInfo The content which nees to be logged
 * @param[in] iLength The Size of the content
 */
void jdd_LogDumpMem (JC_INT32 logInfo, JC_INT32 iModuleNum, const JC_INT8 *pcFileName, JC_INT32 iLineNumber, const JC_INT8 *pcMsgInfo, JC_UINT8 *pucDumpInfo, JC_INT32 iLength) ;

#ifdef __cplusplus
}
#endif

#endif

