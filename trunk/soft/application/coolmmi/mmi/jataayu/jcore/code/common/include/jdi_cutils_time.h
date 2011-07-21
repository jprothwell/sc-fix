/***************************************************************************
 * $Id: jdi_cutils_time.h,v 1.5 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.5 $
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
 * $Log: jdi_cutils_time.h,v $
 * Revision 1.5  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.11  2007/12/28 12:36:07  kumardevhtmlbrow
 * doxygen changes.
 *
 * Revision 1.10  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.9  2006/01/11 12:48:32  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/09/17 15:08:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2005/09/12 12:20:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/08/23 16:34:37  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/08/23 08:25:55  kumardevhtmlbrow
 * Updated the time function to handle GMT format also.
 *
 * Revision 1.4  2005/08/20 10:36:07  kumardevhtmlbrow
 * Added time utils
 *
 * Revision 1.3  2005/06/24 14:23:09  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/06/22 09:32:32  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/06/21 06:02:40  kumardevhtmlbrow
 * time utils
 *
 *  
 ***************************************************************************/

/*
 * @file jdi_cutils_time.h
 * @ingroup CommonUtils
 * @brief It contains the declarations of the structures and function to be used for time
 *		  utility operations
 *
 * This file contains the declarations of the structure and the various functions such as get time,
 * check for leap year etc.
 */

#ifndef _JDI_CUTILS_TIME_H
#define _JDI_CUTILS_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Specifies the time structure that is to be used to when the get time function is called.
 */
typedef struct _cutils_time
{
	JC_UINT16	uhYear ; /**< Specifies the year */
	JC_UINT8	ucMonth ; /**< Specifies the month */
	JC_UINT8	ucDay ; /**< Specifies the Day */
	JC_UINT8	ucHour ; /**< Specifies the hour */
	JC_UINT8	ucMinute ; /**< Specifies the minute */
	JC_UINT8	ucSeconds ; /**< Specifies the second */
} ST_CUTILS_TIME ;

/**
 * @brief	  This function is used to convert the current time (in milliseconds) to time structure
 * @param[in]	uiCurrTime Specifies the current time in milliseconds
 * @param[in,out] pstTime Specifies the time structure which the function will fill and return
 * @retval		void
 * @note		The parameter which should not be NULL is pstTime
 *
 * The function converts the current time (specified in milli seconds) into the time structure.
 */
void jdi_CUtilsGetTime (JC_UINT32		uiCurrTime,
						ST_CUTILS_TIME	*pstTime) ;

/**
 * @brief	  This function checks if a particular year is a leap year or not
 * @param[in]	uiYear Specifies the year
 * @retval		JC_BOOLEAN Specifying whether the given year is leap year or not
 *
 * The function checks if the given year is leap year or not.
 */
JC_BOOLEAN jdi_CUtilsIsLeapYear (JC_UINT32 uiYear) ;

/**
 * @brief	  This function returns the timestamp value for a given date.
 * @param[in]	pcExpires Specifies the date for which the timestamp value has to be returned
 * @retval		JC_INT64 Specifying the timestamp value for the given date
 * @note		The parameter which should not be NULL is pcExpires
 *
 * This function returns the timestamp for the given date.
 */
JC_INT64 jdi_CUtilsCreateTimestamp (JC_INT8 *pcExpires) ;


#ifdef __cplusplus
}
#endif

#endif

/* End of File */


