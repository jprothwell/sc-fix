#define MODULE_NAME "JDD_TIMER"
#define FILE_NAME "jdd_timerapi.c"
#ifdef JATAAYU_SUPPORT
/***************************************************************************
 *
 * File Name jdd_timerapi.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
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
 ***************************************************************************/
 /** 
 * @file    jdd_timerapi.c
 * @ingroup jdd_timerapi
 * @brief   Describes the timer related API's.
 *
 **/
 
 
 /***************************************************************************
 * System Include Files
 **************************************************************************/

#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#else
#include "cswtype.h"
#endif
#include "mmi_trace.h"
#include "cos.h"
#include "errorcode.h"
#include "event.h" 
#include "tm.h"
  
#ifdef MMI_ON_HARDWARE_P
 #include "sxr_tim.h"
#endif /*MMI_ON_HARDWARE_P*/
#include "cos.h"
#include "stack_config.h"
#include "mmitask.h"
#include "stack_common.h"
#include "stack_msgs.h"
 /***************************************************************************
 * User Include Files
 **************************************************************************/
# include <ddl.h>
# include <jcal.h>

 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					TIMER_MODULE
#else
#define __MODULE_ID__					0
#endif

 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/

/**
 * @brief	This structure is initialized in #jdd_TimerCreate and used during
 *			platform registration as callback argument. Instance  of this
 *			structure will be used as timer id in all the timer APIs.
 *			Same is received by	timer callback function when timer expires 
 *			and is used to invoke application callback function.
 *			All necessary platform specific information need to be included 
 *			in this structure.  
 */
 
typedef struct jc_timer
{
	JC_TIMER_CALLBACK	cbTimerCallback ; /* Application Callback Function */
	JC_UINT32 			uiCallbackArg ;   /* Argument for cbTimerCallback */
	JC_UINT32 			uiTimerId ;		  /* Platform Specific TimerID */ 
       JC_UINT32 			uiTimerValueMilliSec ; /* Expiry Time in millisecond */
       JC_BOOLEAN  		bIsSingleShotTimer; /* Used if platform can handle
											 * both single shot and recurrent
											 * timers */
	/* Platform Specific Variables Start */
	/* Platform Specific Variables End */
} JC_TIMER ;



 /***************************************************************************
 * Local Function Prototypes
 **************************************************************************/
 
 
 /***************************************************************************
 * All Local Function Definitions
 **************************************************************************/



  
static JC_INT32 leap_year (JC_UINT32 year)
{
	JC_INT32 ret_val ;

	if ((year % 4) == 0)
	{
		if (((year % 100) == 0) && ((year % 400) != 0))
		{
			ret_val = 0 ;
		}
		else
		{
			ret_val = 1 ;
		}
	}
	else
	{
		ret_val = 0 ;
	}
	return ret_val ;
}
 
 //////////add new timer event  begin///////////

  #define MAX_WAP_TIMER  10
 JC_TIMER wap_timer_evt[MAX_WAP_TIMER];
 
 static void WapTimerInit(void)
 {
     UINT32 i;
    for (i=0;i<MAX_WAP_TIMER;i++)
    {
        wap_timer_evt[i].cbTimerCallback  = NULL;
        wap_timer_evt[i].uiCallbackArg     = 0;
    }	
  }
 
static  UINT32 WapGetTimerId(void)
    {
    UINT32 TimerId;
        for (TimerId=0;TimerId<MAX_WAP_TIMER;TimerId++)
        {
        	if (wap_timer_evt[TimerId].cbTimerCallback == NULL)
        		break;
        }

    return TimerId+1;
        
    }

 static void  WapTimerCreate(UINT32 TimerId,UINT32 timeoutTenths, JC_TIMER_CALLBACK callbackFunc , JC_UINT32 callbackArg) 
{
   TASK_HANDLE * hTask = NULL;

    wap_timer_evt[TimerId-1].cbTimerCallback = callbackFunc;
    wap_timer_evt[TimerId-1].uiCallbackArg = callbackArg;
    wap_timer_evt[TimerId-1].uiTimerId    = TimerId;
    wap_timer_evt[TimerId-1].uiTimerValueMilliSec   = timeoutTenths;
    mmi_trace(1,"evt=0x%x  callbackFunc=0x%x,callbackArg=0x%x,TimerId=%d",&wap_timer_evt[TimerId-1],callbackFunc,callbackArg,TimerId);
    //kal_timerid timer = (kal_timerid)callbackFunc;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_MMI);
  // sxr_StartFunctionTimer(((timeoutTenths<<14)>>3),callbackFunc,callbackArg,0); //timeoutTenths is tenth time,ie 30 -> 3 s
    sxs_StartTimer(( wap_timer_evt[TimerId-1].uiTimerValueMilliSec<<4), MSG_ID_JAT_TIMER_IND_0 + TimerId -1, &wap_timer_evt[TimerId-1], TRUE, hTask->nMailBoxId);
}
static void  WapTimerStop(UINT32 TimerId)
{
    TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_MMI);
   sxs_StopTimer (MSG_ID_JAT_TIMER_IND_0 + TimerId -1, &wap_timer_evt[TimerId-1], hTask->nMailBoxId);

}
static void  WapTimerDestory(UINT32 TimerId)
{
    TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_MMI);
    sxs_StopTimer (MSG_ID_JAT_TIMER_IND_0 + TimerId -1, &wap_timer_evt[TimerId-1], hTask->nMailBoxId);
    wap_timer_evt[TimerId-1].cbTimerCallback = NULL;
    wap_timer_evt[TimerId-1].uiCallbackArg = 0;
}

static void WapTimerSuspend(UINT32 TimerId)
{
    TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_MMI);
	sxs_StopTimer(MSG_ID_JAT_TIMER_IND_0 + TimerId -1, &wap_timer_evt[TimerId-1], hTask->nMailBoxId);
}


static void WapTimerResume(UINT32 TimerId)
{
    TASK_HANDLE * hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_MMI);
	MMI_ASSERT(wap_timer_evt[TimerId-1].cbTimerCallback != NULL);
    sxs_StartTimer(( wap_timer_evt[TimerId-1].uiTimerValueMilliSec<<4), MSG_ID_JAT_TIMER_IND_0 + TimerId -1, &wap_timer_evt[TimerId-1], TRUE, hTask->nMailBoxId);
}


  //////////add new timer event  end///////////


/***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/**
 * @ingroup		jdd_timerapi
 * @brief		Platform Specific callback function to be invoked when
 *				timer expires
 * @param[in]	puiTimerData Callback Argument registered during 
 *				#jdd_TimerCreate
 * @note		This function can be modified according to platform 
 *				requirement.
 **/
 
 /* <retType> cbPlatformTimerCallback (<cbArgList For e.g.: puiTimerData etc>) */
 /*
{
	JC_TIMER *pstTimer ;
	JC_TIMER_CALLBACK cbAppTimerCallback ;
	
	pstTimer = (JC_TIMER *) puiTimerData ;

	if ( (JC_NULL != pstTimer) && (0 != pstTimer->uiTimerId))
	{
		cbAppTimerCallback = pstTimer->cbTimerCallback ;
		cbAppTimerCallback (pstTimer->uiCallbackArg, puiTimerData) ;
	}
	return <retType>;
}
*/

/**
 * @ingroup		jdd_timerapi
 * @brief		This function initializes timer module
 * @retval		JC_OK on success
 * @retval		JC_ERR_NOT_IMPLEMENTED on failure
 *
 * This function is used to initialize the timer.Not all platforms requires
 * the timer to be initialized before creating. But if any 
 * platform requires initialization, it can be carried out at this function.	
 * @note		This API should be invoked as part of the startup code. 
 * @see			#jdd_TimerDeInitialize
 **/

 JC_RETCODE jdd_TimerInitialize (void)
 {
	 JC_RETCODE      rCode = JC_OK ;
	 /*--- Platform Specific Variables ---*/

	 JDD_LOG_INFO0 ("jdd_TimerInitialize : Entry") ;

         WapTimerInit();
	 /*--- Platform Specific Implementation Start ---*/
	/*
	 * Incase of PlatForm needs timer to be a separate thread, timer thread
	 * should be initialized.
	 */
	
	/* PlatForm Specific Function for TimerInitialize */				
		
	 /*
	 if (Platform API succeeds)
	 {
		rCode = JC_OK ;
	 }
	 else
	 {
		rCode = getTimerJCFError (<platformErrValue>)  ;
		JDD_LOG_INFO1 ("jdd_TimerInitialize: Returns [%d]", rCode) ;
	 }
	 */		

	 /*--- Platform Specific Implementation End ---*/

	 JDD_LOG_INFO0 ("jdd_TimerInitialize : Exit") ;
	 return (rCode) ;
 }


 /**
 * @ingroup 	jdd_timerapi
 * @brief 		This function is used to deinitialize the timer.
 * @see			#jdd_TimerDeInitialize
 */
 
 void jdd_TimerDeInitialize (void) 
 {
	 /*--- Platform Specific Variables ---*/
	 /*-----------------------------------*/

	 JDD_LOG_INFO0 ("jdd_TimerDeInitialize : Entry") ;

	 /*--- Platform Specific Implementation Start ---*/
	/*
	 * Incase of PlatForm has timer as seperate thread, timer thread
	 * should be de-initialized.
	 */
	 
	/* PlatForm Specific Function for TimerDeInitialize */
	
	/*
	if (Platform API succeeds)
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = getTimerJCFError (<platformErrValue>) ;
		JDD_LOG_INFO1 ("jdd_TimerDeInitialize: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	 JDD_LOG_INFO0 ("jdd_TimerDeInitialize : Exit") ;
 }


/** 
 * @ingroup		jdd_timerapi
 * @brief 		This API is used to create timer.
 * @param[in]	uiTimerValueMilliSec Specifies the timer expiry value 
 *				in milliseconds.
 * @param[in]	cbTimerCallback Specifies the long pointer to the function, 
 *				which needs to be invoked on the timer expiry.
 * @param[in]	uiCallbackArg Specifies the argument which needs to be 
 *				used when cbTimerCallback is invoked.
 * @param[in]	pcTimerName Specifies the name associated to this timer.
 * @param[out]	puiTimerID Returns the identifier associated with this timer.
 *	 			This identifier	should be passed as the second argument when 
 *				cbTimerCallback is invoked.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TIMER_CREATE on failure.		
 * This function creates a timer and sets the time for expiry.
 * Once the timer expires, the call back routine associated with it will be 
 * invoked with the uiCallbackArg and *puiTimerID parameters.
 * 
 * @note If the timer is not stopped, timer should invoke the cbTimerCallback
 * function for every timer expiry after it is created.
 * 
 * @see 		#jdd_TimerDestroy
 * @see 		#jdd_TimerStop
 */

 JC_RETCODE jdd_TimerCreate (JC_UINT32			uiTimerValueMilliSec, \
							JC_TIMER_CALLBACK	cbTimerCallback, \
							JC_UINT32			uiCallbackArg, \
							JC_INT8				*pcTimerName, \
							JC_UINT32			*puiTimerID)
{
	JC_RETCODE rCode=JC_OK ;
	
	/*--- Platform Specific Variables ---*/
//	JC_TIMER*	pstTimer ;
	UINT32   nTimerId ;
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TimerCreate: Entry") ;
	
	jdd_AssertOnFail ((0 != uiTimerValueMilliSec) && \
					(JC_NULL != cbTimerCallback) && \
					(JC_NULL != puiTimerID), \
					"jdd_TimerCreate: Invalid parameter") ;
   	

		/*--- Platform Specific Implementation Start ---*/

              nTimerId= WapGetTimerId();
              WapTimerCreate(nTimerId,uiTimerValueMilliSec, cbTimerCallback,  uiCallbackArg);
	       *puiTimerID = nTimerId ;
	       mmi_trace (1, "jdd_TimerCreate:MilliSec[%d]Timer Created [%d]",uiTimerValueMilliSec, *puiTimerID);			


		/* Platform call for Timer Create */
		
		/*		
		if (Platform API Succeeds)
		 {
			pstTimer->uiTimerId = <Platform Timer ID> ;
			pstTimer->bIsSingleShotTimer = <E_TRUE/E_FALSE> ;
			rCode = JC_OK ;	
		 }
		 else
		 {
			   rCode = getTimerJCFError (<platformErrValue>) ;
			   JDD_LOG_INFO1 ("jdd_TimerCreate: Returns [%d]", rCode) ;
		 }
		*/
		
		/*--- Platform Specific Implementation End ---*/

	
	JDD_LOG_INFO0 ("jdd_TimerCreate: Exit") ;	
	return (rCode) ;
}

/**
 * @ingroup		jdd_timerapi
 * @brief		This API is used to destroy the timer.
 * @param[in]	uiTimerID Specifies the timer ID, which needs to be destroyed.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TIMER_DESTROY on failure.
 * This function destroys the created timer and frees any resource associated
 * with the timer.
 */
JC_RETCODE jdd_TimerDestroy (JC_UINT32 uiTimerID)
{
	JC_RETCODE rCode = JC_OK;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	mmi_trace (1, "jdd_TimerDestroy: Entry  %d",uiTimerID) ;
	
	jdd_AssertOnFail(0 != uiTimerID, \
				"jdd_TimerDestroy: uiTimerID is 0") ;
	
	
	/*--- Platform Specific Implementation Start ---*/
	/*Free the Array and delete the timer*/
	//DeleteTimer(uiTimerID) ;
	WapTimerDestory(uiTimerID);

	/* Platform Specific Function for Timer Destroy */

	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = getTimerJCFError (<platform variable>) ;
		JDD_LOG_INFO1 ("jdd_TimerDestroy: Returns [%d]", rCode) ;
	}
	*/

	/*--- Platform Specific Implementation End ---*/

//	jdd_MemFree ((void *)uiTimerID) ;
	JDD_LOG_INFO0 ("jdd_TimerDestroy: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_timerapi
 * @brief 		This API is used to set the new parameter to the stopped timer.
 * @param[in] 	uiTimerID Specifies the timer ID, which needs to be reset
 * @param[in] 	uiTimerValueMilliSec Specifies the timer expiry value in 
 *				milliseconds.
 * @param[in] 	cbTimerCallback Specifies the long pointer to the function, 
 *				which needs to be invoked on the timer expiry.
 * @param[in] 	uiCallbackArg Specifies the first argument which needs to be 
 *				used when cbTimerCallback is invoked.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TIMER_RESET on failure.
 * This function is similar to jdd_TimerCreate, the only difference is that 
 * timer identifier is reused in this case.
 * @see			#jdd_TimerCreate
 */
JC_RETCODE jdd_TimerReset (JC_UINT32			uiTimerID, \
						   JC_UINT32			uiTimerValueMilliSec, \
						   JC_TIMER_CALLBACK	cbTimerCallback, \
						   JC_UINT32			uiCallbackArg)

{
	JC_RETCODE rCode = JC_OK ;
	
	/*--- Platform Specific Variables ---*/
	//JC_TIMER*	pstTimer = JC_NULL;
//	BOOL bIsCreated = 0;
	/*-----------------------------------*/
	
		mmi_trace (1, "jdd_TimerReset: Entry") ;
	
	jdd_AssertOnFail(0 != uiTimerID, \
		"jdd_TimerReset: uiTimerID is invalid") ;

        WapTimerCreate(uiTimerID,uiTimerValueMilliSec, cbTimerCallback,  uiCallbackArg);
    
	/*--- Platform Specific Implementation End ---*/
	JDD_LOG_INFO1 ("jdd_TimerReset: Returns [%d]", rCode) ;
	JDD_LOG_INFO0 ("jdd_TimerReset: Exit") ;
	return (rCode) ;
}

/** 
 * @ingroup		jdd_timerapi
 * @brief		This API is used to stop the timer.
 * @param[in]	uiTimerID Specifies the timer ID, which needs to be stopped.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TIMER_STOP on failure .
 * This function stops the created timer and time is not destroyed. In order 
 * to reuse the timer #jdd_TimerReset function should be used.
 */
JC_RETCODE jdd_TimerStop (JC_UINT32 uiTimerID)
{
//	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	
	mmi_trace (1, "jdd_TimerStop: Entry") ;	
	
	jdd_AssertOnFail(0 != uiTimerID, \
				"jdd_TimerStop: uiTimerID is invalid") ;
	
	/*--- Platform Specific Implementation Start ---*/
    WapTimerStop(uiTimerID);
    return JC_OK;

}

/** 
 * @ingroup		jdd_timerapi
 * @brief		This API is used to suspend the timer.
 * @param[in]	uiTimerID Specifies the timer ID, which needs to be suspended.
 * @retval		JC_OK on success.
 * @retval		JC_ERR_TIMER_SUSPEND on failure.
 * This function suspends the created timer and timer is not destroyed. 
 * In order to resume the timer #jdd_TimerResume function should be used.
 */
JC_RETCODE jdd_TimerSuspend (JC_UINT32 uiTimerID)
{
	JC_RETCODE rCode = JC_OK ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	mmi_trace (1, "jdd_TimerSuspend: Entry %d",uiTimerID) ;    
    
	jdd_AssertOnFail(0 != uiTimerID, \
				"jdd_TimerSuspend: uiTimerID is 0") ;
				
	/*--- Platform Specific Implementation Start ---*/  
	//SuspendTimer ( uiTimerID) ;
        WapTimerSuspend(uiTimerID);


	/* PlatForm Specific Function for Timer Suspend*/
	
	/* 
	 * If Platform does not support suspend, work-around is 
	 * -> Destroy [JC_TIMER]->uiTimerID
	 * -> Change [JC_TIMER]->uiTimerID to 0
	 * -> Create new timer in jdd_TimerResume
	 */
	 
	/*		
	if (Platform API Succeeds)
	 {
			rCode = JC_OK ;		
	 }
	 else
	 {
		   rCode = getTimerJCFError (<platformErrValue>) ;
		   JDD_LOG_INFO1 ("jdd_TimerSuspend: Returns [%d]", rCode) ;
	 }
	*/

	/*--- Platform Specific Implementation End ---*/		
	JDD_LOG_INFO0 ("jdd_TimerSuspend: Exit") ;	
	return (rCode) ;
}

/**
 * @ingroup		jdd_timerapi
 * @brief		This API is used to resume the timer.
 * @param[in]	uiTimerID Specifies the timer ID, which needs to be resumed.
 * This function resumes the suspended timer and timer is not destroyed.
 */
JC_RETCODE jdd_TimerResume (JC_UINT32 uiTimerID)
{
    JC_RETCODE  rCode = JC_OK ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
    
	mmi_trace (1, "jdd_TimerResume: Entry %d",uiTimerID) ;
	
	jdd_AssertOnFail(0 != uiTimerID, \
				"jdd_TimerResume: uiTimerID is invalid") ;
				
	/*--- Platform Specific Implementation Start ---*/
	//rCode = ResumeTimer(uiTimerID) ;
	WapTimerResume(uiTimerID);
	/* Platform Specific Function for Timer Resume */
	
	/* 
	 * If Platform does not support resume, work-around is 
	 * -> Check whether [JC_TIMER]->uiTimerID is 0
	 * -> Create new timer
	 * -> Change [JC_TIMER]->uiTimerID to new timer ID
	 */
	 
	/* 
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = getTimerJCFError (<platformErrValue>) ;
		JDD_LOG_INFO1 ("jdd_TimerResume: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/
	JDD_LOG_INFO0 ("jdd_TimerResume: Exit") ;
	return (rCode) ;
}


/**
 * @ingroup		jdd_timerapi
 * @brief		This function returns the current time of the system.
 * @retval		JC_UINT64 Returns the  current time of the system.
 * The time function returns the number of seconds elapsed since 
 * midnight (00:00:00),January 1, 1970, according to the system clock.
 */
JC_INT64 jdd_TimerGetCurrentTime (void)
{
//    JC_INT64 lCurrTime =  0 ;
        BOOL ret ;
		JC_INT32 i, j ;
	JC_INT32 temp ;
	JC_UINT64 secs = 0 ;
	JC_INT8 month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} ;
 	/*--- Platform Specific Variables ---*/
	TM_SYSTEMTIME  SystemTime = {0, };
	/*-----------------------------------*/
	
	JDD_LOG_INFO0("jdd_TimerGetCurrentTime: Entry") ;
	
	/*--- Platform Specific Implementation Start ---*/
	ret = TM_GetSystemTime(&SystemTime) ;
    
    //try to get system time more times if failure
      if(ret == FALSE)
        {
          COS_Sleep(100);
          mmi_trace(1,"jdd_TimerGetCurrentTime failed_1");
          ret = TM_GetSystemTime(&SystemTime) ;
            if(ret == FALSE)
            {
              COS_Sleep(100);
              mmi_trace(1,"jdd_TimerGetCurrentTime failed_2");
              ret = TM_GetSystemTime(&SystemTime) ;
            }
        }
      
	MMI_ASSERT(ret != FALSE);
	mmi_trace(1,"jdd_TimerGetCurrentTime:  (Y:M:D:H:M:S:uM) (%d,%d,%d,%d,%d,%d,%d)", 
					SystemTime.uYear,SystemTime.uMonth,SystemTime.uDay,SystemTime.uHour,SystemTime.uMinute,SystemTime.uSecond,SystemTime.uMilliseconds) ;

	/* Platform Specific Function to get Current time*/
	for (i = 1970; i < SystemTime.uYear; i++)
	{
		secs = secs + (24 * 3600 * (365 + leap_year (i))) ;
	}
	if ((SystemTime.uMonth > 2) && (leap_year ( SystemTime.uYear)))
	{
		month[1] = 29 ;
	}
	for (j = 0; j < (SystemTime.uMonth - 1); j++)
	{
		secs = secs + (24 * 3600 * month[j]) ;
	}
	temp = month[SystemTime.uMonth-1] - SystemTime.uDay + 1 ;
	secs = secs + (month[SystemTime.uMonth - 1] * 24 * 3600 - (temp * 24 * 3600)) + 
		(SystemTime.uHour * 3600) + (SystemTime.uMinute * 60) + SystemTime.uSecond ;

	/* lCurrTime = <Platform Specific Function> */
	
	/*--- Platform Specific Implementation End ---*/
	
	mmi_trace(1,"jdd_TimerGetCurrentTime: Exit sec:0x%x", secs) ;
	return (secs) ;
}

/**
 * @ingroup		jdd_timerapi
 * @brief		This function returns the number of milliseconds that have 
 *				elapsed since the system was started.
 */
JC_UINT32 jdd_TimerSystemGetTickCount (void)
{
	JC_UINT32    uiTickCount = 0 ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TimerSystemGetTickCount: Entry") ;

	/*--- Platform Specific Implementation Start ---*/
	//uiTickCount = TM_GetTime () ;
       uiTickCount = (csw_TMGetTick())/16;
	//MMI_ASSERT(uiTickCount!=0);
	/* Platform Specific Function to Get Tick Count */
	/* uiTickCount = 	<Platform Specific Function> */
	
	/*--- Platform Specific Implementation End ---*/
	JDD_LOG_INFO1 ("jdd_TimerSystemGetTickCount: Exit Tickcount:%d", uiTickCount) ;
	return (uiTickCount) ;
}

/**
 * @ingroup		jdd_timerapi
 * @brief		This function causes the currently executing thread to sleep 
 * 				for the specified number of milliseconds.
 * @param[in]	uiValue Specifies the time in milliseconds, for which the
 *				process has to sleep
 * @warning 	It is recommented not to used this function as it blocks 
 * 				the executing thread.
 * Alternative solution is to create timer for that particular period and 
 * continue the operation on timer expiry.
 */
void jdd_TimerSystemSleep (JC_UINT32 uiValue)
{
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TimerSystemSleep: Entry") ;
	
	/*--- Platform Specific Implementation Start ---*/
	COS_Sleep(uiValue);


	/* Platform Specific Function for Sleep */
	
	/*--- Platform Specific Implementation End ---*/

	JDD_LOG_INFO0 ("jdd_TimerSystemSleep: Exit") ;
}

/**
* @ingroup		jdd_timerapi
 * @brief		Get the Constituent Time elements from the calendar time 
 *				value. This is a direct mapping to the ANSI routine gmtime() 
 *				in <time.h>
 * @param [in] 	plTime is the pointer to calendar time value. The time is 
 *              			represented as seconds elapsed since 
 *				midnight (00:00:00), January 1, 1970, 
 *				coordinated universal time (UTC).
 * @param [out]	pstTime pointer to the ST_TIME structure 
 * @note 		The time fields in the structure ST_TIME holds the evaluated
 *      		value of the time in UTC. The fields that are set are:
 *	pstTime->iSec : Seconds after minute (0 - 59)
 *	pstTime->iMin : Minutes after hour (0 - 59)
 *	pstTime->iHour : Hours since midnight (0 - 23)
 *	pstTime->iDayOfMonth : Day of month (1 - 31)
 *	pstTime->iMonth : Month (0 - 11; January = 0)
 *	pstTime->iYear : Year (current year minus 1900)
 *	pstTime->iDayOfTheWeek : Day of week (0 - 6; Sunday = 0)
 *	pstTime->iDayInTheYear : Day of year (0 - 365; January 1 = 0)
 *	pstTime->iDayLightSavingTime : Always 0 
 */

void jdd_TimerGetGMTime ( const JC_INT64 *plTime, \
							ST_TIME *pstTime ) 
{
	/*--- Platform Specific Variables ---*/
       BOOL ret;
	TM_SYSTEMTIME  SystemTime = {0,} ;
	
	/*-----------------------------------*/
	
	JDD_LOG_INFO0 ("jdd_TimerGetGMTime: Entry") ;
	
	/*--- Platform Specific Implementation Start ---*/
	ret = TM_GetSystemTime(&SystemTime) ;
        //try to get system time more times if failure
      if(ret == FALSE)
        {
          COS_Sleep(100);
          mmi_trace(1,"jdd_TimerGetGMTime failed_1");
          ret = TM_GetSystemTime(&SystemTime) ;
            if(ret == FALSE)
            {
              COS_Sleep(100);
              mmi_trace(1,"jdd_TimerGetGMTime failed_2");
              ret = TM_GetSystemTime(&SystemTime) ;
            }
        }
      
	MMI_ASSERT(ret != FALSE);
	pstTime->iSec = SystemTime.uSecond ;				
	pstTime->iMin = SystemTime.uMinute ;				
	pstTime->iHour = SystemTime.uHour ;			
	pstTime->iDayOfMonth = SystemTime.uDay ;		
	pstTime->iMonth = SystemTime.uMonth-1 ;			
	pstTime->iYear = SystemTime.uYear ;			
	pstTime->iDayOfTheWeek = SystemTime.uDayOfWeek ;
	
	/* Platform Specific Function to Get GMTime */
	
	/*--- Platform Specific Implementation End ---*/
	
	JDD_LOG_INFO0 ("jdd_TimerGetGMTime: Exit") ;
	
}

/**
 * @ingroup		jdd_timerapi
 * @brief		Converts a supplied ST_TIME structure (possibly incomplete 
 *				fields) expressed as local time to fill its elements time 
 *        		value with normalized values and then returns the calendar.This 
 *		  		is a direct mapping to the ANSI routine mktime() in <time.h>
 * @param [in]	*pstTime  pointer to the ST_TIME structure 
 * @retval		calendar time value in JC_INT64
 * @note		Normalization would mean that, this API ignores the 
 *      		pstTime->iDayOfTheWeek and pstTime->iDayInTheYear and recomputes 
 *				them from the other pstTime fields provided. If the fields are 
 *				outside their interval, they are normalized ( i.e. 40 october is 
 *				changed to 9 November ). If the constituent time fields can not 
 *				be represented as calendar time (seconds since the epoch), this 
 *				API returns -1 without setting pstTime->iDayOfTheWeek and 
 *				pstTime->iDayInTheYear 
 */

JC_INT64 jdd_TimerGetMKTime( ST_TIME *pstTime )
{
	JC_INT64    lmkTime = 0 ;
	/*--- Platform Specific Variables ---*/
	TM_SYSTEMTIME SystemTime = {0,} ;
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TimerGetMKTime: Entry") ;
	
	/*--- Platform Specific Implementation Start ---*/
	TM_GetLocalTime (&SystemTime);
	
	pstTime->iSec = SystemTime.uSecond ;				
	pstTime->iMin = SystemTime.uMinute ;				
	pstTime->iHour = SystemTime.uHour ;			
	pstTime->iDayOfMonth = SystemTime.uDay ;		
	pstTime->iMonth = SystemTime.uMonth-1 ;			
	pstTime->iYear = SystemTime.uYear ;			
	pstTime->iDayOfTheWeek = SystemTime.uDayOfWeek ;


	/* Platform Specific Function to Get MKTime */
	/* lmkTime = <Platform Specific Function> */
	
	/*--- Platform Specific Implementation End ---*/

	JDD_LOG_INFO0 ("jdd_TimerGetMKTime: Exit") ;
	return (lmkTime) ;
}			

/**
 * @ingroup		jdd_timerapi
 * @brief 		Returns the Time difference in seconds, between GMT and 
 *				Local Time
 * @return  	JC_INT64 - Time Difference in seconds
 * @note		This is a locale specific information depending on the 
 *				Geographical location configured on the platform
 */
//TBD
JC_INT64  jdd_TimerGetGMTLocalTimeDiff(void)
{
	JC_INT64    retval = 0 ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	JDD_LOG_INFO0 ("jdd_TimerGetGMTLocalTimeDiff: Entry") ;
	
	/*--- Platform Specific Implementation Start ---*/

	/* Platform Specific Function to Get Difference between GMT and LocalTime*/
	/* retval = <Platform Specific Function> */
	
	/*--- Platform Specific Implementation End ---*/

	JDD_LOG_INFO0 ("jdd_TimerGetGMTLocalTimeDiff: Exit") ;
	return (retval) ;
}			

/**
 * @brief This function returns the current time of the system in milliseconds.
 * @retval JC_UINT32 Returns the 
 * The time function returns the number of milliseconds elapsed since midnight (00:00:00), 
 * January 1, 1970, according to the system clock.
 */
JC_INT64 jdd_TimerGetCurrentTimeMS (void)
{
	return (1000 * jdd_TimerGetCurrentTime ()) ;
}

/* End of File */
#endif
