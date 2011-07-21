/***************************************************************************
 * $Id: jdd_timerapi.h,v 1.6 2008/03/26 11:11:46 browserdevjtbase Exp $
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
 * $Log: jdd_timerapi.h,v $
 * Revision 1.6  2008/03/26 11:11:46  browserdevjtbase
 * Updated the comments.
 *
 * Revision 1.5  2007/03/20 06:20:58  browserdevjtbase
 * Update to comments
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
 * Revision 1.8  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2006/01/17 14:39:12  kumardevhtmlbrow
 * Updated the comments ...
 *
 * Revision 1.5  2006/01/11 12:48:32  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.4  2005/11/24 10:58:47  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.2  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/


#ifndef _JDD_TIMER_H
#define _JDD_TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file jdd_timerapi.h
 * @ingroup Timer
 * @brief Defines the time and date utils abstraction. 
 * 
 */

/**
 * @struct ST_TIME
 * @brief Structure for time elements
 */

typedef struct _st_time 
{
	JC_INT32     iSec ;						/**< seconds */
	JC_INT32     iMin ;						/**< minutes */
	JC_INT32     iHour ;					/**< hours */
	JC_INT32     iDayOfMonth ;				/**< day of the month */
	JC_INT32     iMonth ;					/**< month */
	JC_INT32     iYear ;					/**< year */
	JC_INT32     iDayOfTheWeek ;			/**< day of the week */
	JC_INT32     iDayInTheYear ;			/**< day in the year */
	JC_INT32     iDayLightSavingTime ;		/**< -ve (Not Known)
												,0(In Effect), +ve(Not In Effect) 
												daylight saving time */
} ST_TIME ;


/**
 * @brief Defines the prototype which shall be used by the timer event to notify
 * the timer expiry. The uiCallbackArg parameter specifies the callback argument,
 * which is specified during the timer create. The uiTimerID parameter specifies,
 * which timer is expired.
 */
typedef void (* JC_TIMER_CALLBACK) (JC_UINT32 uiCallbackArg, JC_UINT32 uiTimerID) ; 

/**
 * @brief This API is used to initialize the timer.
 * Not all platforms requires the timer to be initialized before creating. But if any 
 * platform required the initialize can be carried out at this function.
 * 
 * @note This API should be invoked as part of the startup code. 
 */
JC_RETCODE jdd_TimerInitialize (void) ;

/**
 * @brief This API is used to create the timer.
 * @param[in] uiTimerValueMilliSec Specifies the timer expiry value in milliseconds.
 * @param[in] cbTimerCallback Specifies the long pointer to the function, which needs to be invoked on the timer expiry.
 * @param[in] uiCallbackArg Specifies the first argument which needs to be used when cbTimerCallback is invoked.
 * @param[in] pcTimerName Specifies the name associated to this timer.
 * @param[out] puiTimerID Returns the identifier associated with this timer. This identifier 
 *						  should be passed as the second argument when cbTimerCallback is invoked.
 * This function creates a timer and sets the time for expiry.
 * Once the timer expires the call back routine associated with it will be invoked 
 * with the uiCallbackArg and *puiTimerID parameters.
 * @note If the timer is not stopped, timer should invoke the cbTimerCallback function for 
 * every timer expiry after it is created.
 * @see jdd_TimerDestroy
 * @see jdd_TimerStop
 */
JC_RETCODE jdd_TimerCreate (JC_UINT32			uiTimerValueMilliSec, 
							JC_TIMER_CALLBACK	cbTimerCallback, 
							JC_UINT32			uiCallbackArg, 
							JC_INT8				*pcTimerName,
							JC_UINT32			*puiTimerID) ; 

/**
 * @brief This API is used to destroy the timer.
 * @param[in] uiTimerID Specifies the timer ID, which needs to be destroyed. 
 * This function destroys the created timer and frees any resource associated with the timer.
 */
JC_RETCODE jdd_TimerDestroy (JC_UINT32 uiTimerID) ;

/**
 * @brief This API is used to set the new parameter to the stopped timer.
 * @param[in] uiTimerID Specifies the timer ID, which needs to be reset
 * @param[in] uiTimerValueMilliSec Specifies the timer expiry value in milliseconds.
 * @param[in] cbTimerCallback Specifies the long pointer to the function, which needs to be invoked on the timer expiry.
 * @param[in] uiCallbackArg Specifies the first argument which needs to be used when cbTimerCallback is invoked.
 * 
 * This function is similar to jdd_TimerCreate, the only difference is that timer identifier
 * is reused in this case.
 * @see jdd_TimerCreate
 */
JC_RETCODE jdd_TimerReset (JC_UINT32			uiTimerID,
						   JC_UINT32			uiTimerValueMilliSec, 
						   JC_TIMER_CALLBACK	cbTimerCallback, 
						   JC_UINT32			uiCallbackArg) ;
/**
 * @brief This API is used to stop the timer.
 * @param[in] uiTimerID Specifies the timer ID, which needs to be stopped. 
 * This function stops the created timer and time is not destroyed. In order to reuse 
 * the timer #jdd_TimerReset function should be used.
 */
JC_RETCODE jdd_TimerStop (JC_UINT32 uiTimerID) ;

/**
 * @brief This API is used to suspend the timer.
 * @param[in] uiTimerID Specifies the timer ID, which needs to be suspended. 
 * This function suspends the created timer and timer is not destroyed. In order to resume 
 * the timer #jdd_TimerResume function should be used.
 */
JC_RETCODE jdd_TimerSuspend (JC_UINT32 uiTimerID) ;

/**
 * @brief This API is used to resume the timer.
 * @param[in] uiTimerID Specifies the timer ID, which needs to be resumed. 
 * This function resumes the suspended timer and timer is not destroyed. 
 */
JC_RETCODE jdd_TimerResume (JC_UINT32 uiTimerID) ;

/**
 * @brief This function causes the currently executing thread to sleep for the specified number 
 * of milliseconds.
 * @param[in] uiValue Specifies the time in milliseconds, for which the specific process/thread 
 * shall be suspended. 
 * 
 * @warning It is recommented not to used this function as it blocks the executing thread. 
 * Alternative solution is to create timer for that particular period and continue the 
 * operation on timer expiry.
 */
void jdd_TimerSystemSleep (JC_UINT32 uiValue) ;

/**
 * @brief This function returns the current time of the system.
 * @retval JC_UINT32 Returns the 
 * The time function returns the number of seconds elapsed since midnight (00:00:00), 
 * January 1, 1970, according to the system clock.
 */
JC_INT64 jdd_TimerGetCurrentTime (void) ;

/**
 * @brief This function returns the current time of the system in milliseconds.
 * @retval JC_UINT32 Returns the 
 * The time function returns the number of milliseconds elapsed since midnight (00:00:00), 
 * January 1, 1970, according to the system clock.
 */
JC_INT64 jdd_TimerGetCurrentTimeMS (void) ;



/**
 * @brief This function returns the number of milliseconds that have elapsed 
 * since the system was started.
 */
JC_UINT32 jdd_TimerSystemGetTickCount (void) ;

/**
 * @brief This function is used to deinitialize the timer.
 */
void jdd_TimerDeInitialize (void) ;


/**
 * @brief Get the Constituent Time elements from the calendar time value. 
 *		  This is a direct mapping to the ANSI routine gmtime() in <time.h>
 * @param [in] *plTime is the pointer to calendar time value. The time is represented as seconds elapsed 
 *              since midnight (00:00:00), January 1, 1970, coordinated universal time (UTC).
 * @param [out] pstTime pointer to the ST_TIME structure 
 *
 * @note The time fields in the structure ST_TIME holds the evaluated value of
 *      the time in UTC. The fields that are set are:
 
	pstTime->iSec : Seconds after minute (0 - 59)
	pstTime->iMin : Minutes after hour (0 - 59)
	pstTime->iHour : Hours since midnight (0 - 23)
 * which is specified during the timer create. The uiTimerID parameter specifies,
 * which timer is expired.
 * @note The field pstTime->lGMTAndLocalDiffInSec is set with the difference in seconds between 
 *      GMT and local time. Default value is 28,800.
 */

void jdd_TimerGetCurrentLocalTime( const JC_INT64 *plTime, 
									  ST_TIME *pstTime ) ;

/**
 * @brief Get the Constituent Time elements from the calendar time value. 
 *		  This is a direct mapping to the ANSI routine gmtime() in <time.h>
 * @param [in] *plTime is the pointer to calendar time value. The time is represented as seconds elapsed 
 *              since midnight (00:00:00), January 1, 1970, coordinated universal time (UTC).
 * @param [out] pstTime pointer to the ST_TIME structure 
 *
 * @note The time fields in the structure ST_TIME holds the evaluated value of
 *      the time in UTC. The fields that are set are:
 
	pstTime->iSec : Seconds after minute (0 - 59)
	pstTime->iMin : Minutes after hour (0 - 59)
	pstTime->iHour : Hours since midnight (0 - 23)
	pstTime->iDayOfMonth : Day of month (1 - 31)
	pstTime->iMonth : Month (0 - 11; January = 0)
	pstTime->iYear : Year (current year minus 1900)
	pstTime->iDayOfTheWeek : Day of week (0 - 6; Sunday = 0)
	pstTime->iDayInTheYear : Day of year (0 - 365; January 1 = 0)
	pstTime->iDayLightSavingTime : Always 0 
 */

void jdd_TimerGetGMTime ( const JC_INT64 *plTime, 
							ST_TIME *pstTime ) ;

/**
 * @brief Converts a supplied ST_TIME structure ( possibly incomplete fields ) expressed as local time
 *        to fill its elements with normalized values and then returns the calendar time value.
 *		  This is a direct mapping to the ANSI routine mktime() in <time.h>
 * @param [in] pstTime pointer to the ST_TIME structure 
 *
 * @return calendar time value in JC_INT64
 *
 * @note Normalization would mean that, this API ignores the pstTime->iDayOfTheWeek
 *      and pstTime->iDayInTheYear and recomputes them from the other pstTime fields
 *      provided. If the fields are outside their interval, they are normalized ( i.e. 40 october
 *      is changed to 9 November ). If the constituent time fields can not be represented as
 *      calendar time (seconds since the epoch), this API returns -1 without setting 
 *      pstTime->iDayOfTheWeek and pstTime->iDayInTheYear 
 */

JC_INT64 jdd_TimerGetMKTime( ST_TIME *pstTime ) ;

/**
 * @brief Returns the Time difference in seconds, between GMT and Local Time
 *
 * @return  JC_INT64 - Time Difference in seconds
 *
 * @note	This is a locale specific information depending on the 
 *			Geographical location configured on the platform
 */

JC_INT64  jdd_TimerGetGMTLocalTimeDiff(void) ;	

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


