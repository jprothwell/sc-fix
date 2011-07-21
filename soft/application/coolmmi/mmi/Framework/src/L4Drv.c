/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * L4Drv.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Timer/Clock control.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 *	L4Drv.c
 *
 * Project:
 * --------
 *   MediaTeck GSM
 *
 * Description:
 * ------------
 
 *
 * Author:
 * -------
 * -------
 * 				
 *
 * HISTORY
 * -------
 * Created On 13 Dec
*******************************************************************************/

#include "mmi_trace.h"

//------- Include Files -----------------------------
#ifdef MMI_ON_HARDWARE_P

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
/*... Add More MMI header */
#include "globalconstants.h"
#include "queuegprot.h"
#include "oslmemory.h"
 
#include "audioinc.h"
#include "timerevents.h"
#include "taskinit.h"
#include "keybrd.h"
#include "debuginitdef.h"
#undef __NEWSIMULATOR 
#include "l4dr.h"
#include "gpioinc.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "touchscreengprot.h"
#endif 
#include "screenrotationgprot.h"
#include "kbd_table.h"
#include "mmitaskprot.h"
#include "simdetectiongexdcl.h"
#include "mmi.h"
#include "mmiapi.h"

/***************************************************************************** 
* Define
*****************************************************************************/
/* the recommended value: 12 or 24 */
//tangjie del begin 20060708
#if 0
#define SIMULTANEOUS_TIMER_NUM			(12)

/* MSB(Most Significant Bit) of g_timer_table.timer_id[i] is align_timer_mask */
#define NO_ALIGNMENT_TIMER_MASK			(0x8000)
#define ALIGNMENT_TIMER_MASK				(0)

/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef void (*L4FuncPtr)(oslParaType*);
typedef struct 
{
	U16 		eventID;
	L4FuncPtr 	entryFuncPtr; 
}L4eventInfo;

typedef struct timertable
{
	/* store the timer_id. MSB(Most Significant Bit) is align_timer_mask */
	U16					timer_id[SIMULTANEOUS_TIMER_NUM];
	/* store the event_id that returns from evshed_set_event() */
	eventid				event_id[SIMULTANEOUS_TIMER_NUM];
	/* store the timer_expiry_func */
	oslTimerFuncPtr	callback_func[SIMULTANEOUS_TIMER_NUM];
	/* point to the next TIMERTABLE data */
	struct timertable	*next;
} TIMERTABLE;


/***************************************************************************** 
* Local Variable
*****************************************************************************/
/*
 * In most case, the timers won't run simultaneously.
 * We run-time allocate the memory to store the timer information. 
 */
static TIMERTABLE g_timer_table = {0};
static U16			g_timer_table_size = 0;
static U16			g_timer_table_used = 0;
#endif
//tangjie del end 20060708
static oslTaskFuncPtr get_clocktime_callback_func;
static oslTaskFuncPtr set_clocktime_callback_func;

/***************************************************************************** 
* Local Function
*****************************************************************************/

/***************************************************************************** 
* Global Variable
*****************************************************************************/
//tangjie del begin 20060708
#if 0

extern stack_timer_struct         base_timer1;
extern stack_timer_struct         base_timer2;
extern event_scheduler            *event_scheduler1_ptr, *event_scheduler2_ptr;
extern oslMsgqid mmi_ext_qid;
extern U8 keypad_flag;
extern drv_get_key_func keypad_ptr;


/***************************************************************************** 
* Global Function
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*  L4KeyHandle
* DESCRIPTION
*   This function is to process the keypad handler.
*
* PARAMETERS
*  a  IN     paraBuff
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4KeyHandle(void *paraBuff)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	kbd_data k;
	U32 msg_count;
	mmi_eq_keypad_detect_ind_struct *p;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(paraBuff !=NULL)
	{
		drv_get_key_func new_key_ptr;
		p = (mmi_eq_keypad_detect_ind_struct *)paraBuff;
		
		new_key_ptr = (drv_get_key_func)(p->func);

		if(new_key_ptr != keypad_ptr)
			ClearKeyEvents();
		
		keypad_ptr = new_key_ptr;
	}	

	while(1)
	{		
		if( (*(keypad_ptr))(&k) == MMI_TRUE)
		{
			if (k.Keydata[0]!=KEY_INVALID)
			{
				static U8 is_allow_key_action = MMI_TRUE;
#if defined(__MMI_SCREEN_ROTATE__)
				static mmi_frm_screen_rotate_enum key_rotate = MMI_FRM_SCREEN_ROTATE_0;
#endif
#if defined(__MMI_SLEEP_OUT_BACKLIGHT_ON__)          	
				extern U8 lcd_backlight; 
#endif
#if defined(__MMI_TOUCH_SCREEN__)
				kal_bool is_pen_enabled, is_pen_down;
				mmi_pen_get_state(&is_pen_enabled, &is_pen_down);
#endif
			
#if defined(__MMI_SLEEP_OUT_BACKLIGHT_ON__)          	
				if ( (lcd_backlight==0)  && (k.Keyevent==WM_KEYPRESS) )
				{
					TurnOnBacklight(1); // timer backlight control
					is_allow_key_action = MMI_FALSE;
				}
				else 
#endif
#if defined(__MMI_TOUCH_SCREEN__)
				if (is_pen_down && (k.Keyevent==WM_KEYPRESS))
				{
					is_allow_key_action = MMI_FALSE;
				}
				else
#endif
				if ( (k.Keyevent==WM_KEYRELEASE) && (is_allow_key_action == MMI_FALSE) )
				{
					is_allow_key_action = MMI_TRUE;
				}
				else if (is_allow_key_action == MMI_TRUE)
				{
					if (k.Keyevent==WM_KEYPRESS)
					{
#ifdef __MMI_TOUCH_SCREEN__
						mmi_pen_disable();
#endif
#ifdef __MMI_SCREEN_ROTATE__
						key_rotate = mmi_frm_get_screen_rotate();
#endif
					}

#ifdef __MMI_SCREEN_ROTATE__
					switch (key_rotate)
					{
						case MMI_FRM_SCREEN_ROTATE_270:
							switch (k.Keydata[0])
							{
								case DEVICE_KEY_UP:
									k.Keydata[0] = DEVICE_KEY_LEFT;
									break;
								case DEVICE_KEY_DOWN:
									k.Keydata[0] = DEVICE_KEY_RIGHT;
									break;
								case DEVICE_KEY_LEFT:	
									k.Keydata[0] = DEVICE_KEY_DOWN;
									break;
								case DEVICE_KEY_RIGHT:
									k.Keydata[0] = DEVICE_KEY_UP;
									break;
							}
							break;
					}
#endif

					TurnOnBacklight(MMI_TRUE);
					ProcessKeyEvent(k.Keyevent, k.Keydata[0]);

					if (k.Keyevent==WM_KEYRELEASE)
					{
#ifdef __MMI_TOUCH_SCREEN__
						mmi_pen_enable();
#endif
#ifdef __MMI_SCREEN_ROTATE__
						key_rotate = MMI_FRM_SCREEN_ROTATE_0;
#endif
					}
				}
			}

			msg_get_ext_queue_info(mmi_ext_qid, &msg_count);
   		if(msg_count >0 || OslNumOfCircularQMsgs() > 0)
      	{
   			keypad_flag = 1;
   			break;
      	}
		}
		else
		{
			keypad_flag = 0;
			break;
		}
	} /* while(1) */
}


/*****************************************************************************
* FUNCTION
*  mmi_frm_set_key_handle_in_high_frequency
* DESCRIPTION
*   This function is to process the keypad handler in high frequency mode
*
* PARAMETERS
*  a  IN     is_enable
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_frm_set_key_handle_in_high_frequency(MMI_BOOL is_enable)
{
   g_process_key_in_high_frequency = is_enable;
}
#endif
//tangjie del end 20060708

/*****************************************************************************
* FUNCTION
*  L4SetClock
* DESCRIPTION
*   This function is to set the clock request.
*
* PARAMETERS
*  a  IN     void
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4SetClock(rtc_format_struct datetime, oslTaskFuncPtr FuncRspPtr)
{      
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   ilm_struct *ilm_ptr = NULL;
   mmi_eq_set_rtc_time_req_struct *local_data;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   /* store the set time clock callback function */
	set_clocktime_callback_func = FuncRspPtr;
     
	 ilm_ptr = allocate_ilm( MOD_MMI );
	ilm_ptr->msg_id = (U16) MSG_ID_MMI_EQ_SET_RTC_TIME_REQ;
	// local_data =  (mmi_eq_set_rtc_time_req_struct *) 
	               //construct_local_para( sizeof(mmi_eq_set_rtc_time_req_struct), TD_CTRL );  //tangjie del 20060707
	local_data = OslMalloc(sizeof(mmi_eq_set_rtc_time_req_struct));   //tangjie add 20060707
	 
	 local_data->set_type = RTC_SETTING_TYPE_DATETIME;
   	 local_data->rtc_type = RTC_TIME_CLOCK_IND;
   	 local_data->info.alarm_format = 0;
	 local_data->info.alarm_index = 0;
	 local_data->info.type = 0;
	 local_data->info.text[0] = '\0';
	 local_data->info.recurr = 0;
	local_data->info.data_time.rtc_year = datetime.rtc_year;
	local_data->info.data_time.rtc_wday = datetime.rtc_wday;
	local_data->info.data_time.rtc_mon = datetime.rtc_mon;
	local_data->info.data_time.rtc_day = datetime.rtc_day;
	local_data->info.data_time.rtc_hour = datetime.rtc_hour;
	local_data->info.data_time.rtc_min = datetime.rtc_min;
	local_data->info.data_time.rtc_sec = datetime.rtc_sec;

	 ilm_ptr->local_para_ptr = (local_para_struct *)local_data;
	 ilm_ptr->peer_buff_ptr = NULL;   
	 SEND_ILM( MOD_MMI, MOD_L4C, MMI_L4C_SAP, ilm_ptr);
}


/*****************************************************************************
* FUNCTION
*  L4SetClockRSP
* DESCRIPTION
*   This function is to get to the result of clock set response.
*
* PARAMETERS
*  a  IN     buf
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4SetClockRSP(void *buf)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if (	set_clocktime_callback_func!=NULL)
	{
		set_clocktime_callback_func(buf);
	}
}

/*****************************************************************************
* FUNCTION
*  L4GetClockTime
* DESCRIPTION
*   This function is to request to get the clock.
*
* PARAMETERS
*  a  IN     void
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4GetClockTime(oslTaskFuncPtr FuncRspPtr)
{      
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	 ilm_struct *ilm_ptr = NULL;
	 mmi_eq_get_rtc_time_req_struct *local_data;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   /* store the get time clock callback function */
      get_clocktime_callback_func = FuncRspPtr;

	 ilm_ptr = allocate_ilm( MOD_MMI );
	 mmi_trace(g_sw_IM, TSTXT("L4GetClockTime:MSG_ID_MMI_EQ_GET_RTC_TIME_REQ\n"));
	 local_data =OslMalloc(sizeof(mmi_eq_get_rtc_time_req_struct));    //tangji add 20060710	 
        local_data->rtc_type = RTC_TIME_CLOCK_IND;

	// ilm_ptr->msg_id = (U16) MSG_ID_MMI_EQ_GET_RTC_TIME_REQ;       //tangjie del 20060710
	 ilm_ptr->msg_id = MSG_ID_MMI_EQ_GET_RTC_TIME_REQ;                   //tangjie add 20060710
	 mmi_trace(g_sw_IM, TSTXT(",ilm_ptr->msg_id=%x,MSG_ID_MMI_EQ_GET_RTC_TIME_REQ=%x    :\n"),ilm_ptr->msg_id,MSG_ID_MMI_EQ_GET_RTC_TIME_REQ);

	 ilm_ptr->local_para_ptr = (local_para_struct *)local_data;
	 ilm_ptr->peer_buff_ptr = NULL;   

	 SEND_ILM( MOD_MMI, MOD_L4C, MMI_L4C_SAP, ilm_ptr);
}


/*****************************************************************************
* FUNCTION
*  L4GetClockTimeRSP
* DESCRIPTION
*   This function is to receive the clock response.
*
* PARAMETERS
*  a  IN     buf
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4GetClockTimeRSP(void *buf)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
       mmi_trace(g_sw_FRAMEWORK, TSTXT(" L4GetClockTimeRSP get_clocktime_callback_func= %x\n"),get_clocktime_callback_func);


	if (get_clocktime_callback_func != NULL)
	{
		get_clocktime_callback_func(buf);
	}
}

//tangjie del begin 20060708
#if 0

/*****************************************************************************
* FUNCTION
*  L4InitTimer
* DESCRIPTION
*   This function is to init the timer while task create. 
*
* PARAMETERS
*  a  IN     void
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4InitTimer(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	TIMERTABLE	*p;
	TIMERTABLE	*pp;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* Try to free TIMERTABLE list exclude g_timer_table */
	p = g_timer_table.next;
	pp = NULL;
	do
	{	
		if (p != NULL)
		{
			pp = p->next;
			OslMfree(p);
		}
		p = pp;
	} while (p != NULL);
	/* reset g_timer_talbe */ 
	memset(&g_timer_table, 0, sizeof(TIMERTABLE));
	g_timer_table_size = SIMULTANEOUS_TIMER_NUM;
	g_timer_table_used = 0;

   /* Initiate the clock time callback function. */
   get_clocktime_callback_func = NULL;
   set_clocktime_callback_func = NULL;

   /* Initate the no alignment stack timer */
	stack_init_timer (&base_timer1, "MMI_Base_Timer1", MOD_MMI);

   /* Create a no alignment timer schedule */
   event_scheduler1_ptr = new_evshed(&base_timer1,
								   L4StartBaseTimer, L4StopBaseTimer,
								   0 , kal_evshed_get_mem, kal_evshed_free_mem, 0);

   /* Initate the alignment stack timer */
   stack_init_timer (&base_timer2, "MMI_Base_Timer2", MOD_MMI);

   /* Create an alignment timer schedule */
   event_scheduler2_ptr = new_evshed(&base_timer2,
								   L4StartBaseTimer, L4StopBaseTimer,
								   0 , kal_evshed_get_mem, kal_evshed_free_mem, 255);
}


/*****************************************************************************
* FUNCTION
*  L4StopBaseTimer
* DESCRIPTION
*   This function is to assign the stop timer base. 
*
* PARAMETERS
*  a  IN     base_timer_ptr
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4StopBaseTimer(void *base_timer_ptr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	stack_stop_timer((stack_timer_struct *)base_timer_ptr);
}


/*****************************************************************************
* FUNCTION
*  L4StartBaseTimer
* DESCRIPTION
*   This function is to assign the start timer base. 
*
* PARAMETERS
*  a  IN     base_timer_ptr
*  b  IN     time_out
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4StartBaseTimer(void *base_timer_ptr,unsigned int time_out)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   stack_start_timer((stack_timer_struct *)base_timer_ptr, 0, time_out);
}


/*****************************************************************************
* FUNCTION
*  L4StartTimer
* DESCRIPTION
*   This function is to execute the timer expire. 
*
* PARAMETERS
*  a  IN     p
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4CallBackTimer(void *p)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U32					nTimerId = (U32)p;
	TIMERTABLE			*pTable = &g_timer_table;
	U32					i = 0;
	oslTimerFuncPtr	pTimerExpiry = NULL;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   MMI_ASSERT(nTimerId < MAX_TIMERS);

	/* find the nTimerId in TIMERTABLE list */
	do
	{
		/* MSB(Most Significant Bit) of timer_id[i] is align_timer_mask */
		if ((pTable->timer_id[i] & (~NO_ALIGNMENT_TIMER_MASK)) == (U16)nTimerId)
		{
			/* find out nTimerId */
   		if (pTable->callback_func[i] != NULL)
			{
				pTimerExpiry = pTable->callback_func[i];
				MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_L4DRV_CBTIMER_HDLR, nTimerId, pTimerExpiry));
				g_timer_table_used --;
				pTable->event_id[i] = 0;
				pTable->timer_id[i] = 0;
				pTable->callback_func[i] = NULL;
				/*
				 * we process g_timer_table_used, event_id and timer_id ... first 
				 * because the user may call stoptimer() in the timer_expiry_func
				 */
				pTimerExpiry(p);
			}
			break;
		}
		i++;
		if (i >= SIMULTANEOUS_TIMER_NUM )
		{
			pTable = pTable->next;
			i = 0;
		}
	} while (pTable != NULL);
	/* can't find nTimerId, do nothing */

	mmi_frm_fetch_msg_from_extQ_to_circularQ();
	if ( (keypad_flag==1) && g_process_key_in_high_frequency )
	{
	   L4KeyHandle(NULL);
	}
}


/*****************************************************************************
* FUNCTION
*  L4TimerUsePreciseTick
* DESCRIPTION
*  Typically we round timer period to multiple of 100ms. However, some timers need to be
*	more accurate.
*
* PARAMETERS
*  nTimerId  IN     timer ID
*	
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*  
*****************************************************************************/
MMI_BOOL L4TimerUsePreciseTick(unsigned short nTimerId)
{
	switch (nTimerId)
	{
#ifdef __MMI_TOUCH_SCREEN__
		case PEN_POLLING_TIMER:
			return MMI_TRUE;
#endif
		default:
			return MMI_FALSE;
	}
}

/*****************************************************************************
* FUNCTION
*  L4StartTimer
* DESCRIPTION
*   This function is to start timer. To added for two kinds of timer, one is for exactly time, another allow to delay.
*
* PARAMETERS
*  a  IN     nTimerId
*  b  IN 	TimerExpiry
*  c  IN	funcArg
*  d  IN	nTimeDurationInTicks
*  e  IN     alignment
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4StartTimer(unsigned short nTimerId, oslTimerFuncPtr TimerExpiry, void * funcArg, unsigned long nTimeDurationInTicks, unsigned char alignment)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	TIMERTABLE	*pTable = NULL;
	U16			i = 0;
   U32			temp;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if (TimerExpiry == NULL)
   {	/* If TimerExpiry is NULL, we don't start the timer */
		MMI_ASSERT(0);
   	return ;
   }
   
   MMI_ASSERT(nTimerId<MAX_TIMERS);

	if (L4TimerUsePreciseTick(nTimerId))
	{
		temp = ( nTimeDurationInTicks * KAL_TICKS_10_MSEC ) / 10;
		if (temp == 0)
		{
			temp = KAL_TICKS_10_MSEC;
		}
   	alignment = TIMER_IS_NO_ALIGNMENT;
	}
	else
	{
		if (nTimeDurationInTicks == 1000)
		{
			temp = KAL_TICKS_1_SEC-4;
		}
		else  
		{
			temp = (nTimeDurationInTicks / 100) * KAL_TICKS_100_MSEC;
		}

		if (temp==0) 
		{
			temp = KAL_TICKS_100_MSEC; 
		}
	} /* if (L4TimerUsePreciseTick(nTimerId)) */

   /* 
    * Because the handset doesn't camp on the network in flight mode, 
    * it influences MMI alignment timer are inaccurate. 
    * We change all of MMI timer to non-alignment timer in flight mode.
    */
   #if 0  //dyj del 20060515
   if ( mmi_bootup_get_active_flight_mode() == 1 )
   {
   	alignment = TIMER_IS_NO_ALIGNMENT;
   }
   #endif
	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_L4DRV_STARTTIMER_HDLR, nTimerId, TimerExpiry, temp, alignment));

	pTable = &g_timer_table;
	if (g_timer_table_used >= g_timer_table_size)
	{
		/*
		 * TIMERTABLE list doesn't have enough space, allocate the memory 
		 *
		 * If we need to allocate the memeory, it means that MMI may have 
		 * such many timers run simultaneously. We won't free the memory 
		 * after we allocate more memory in TIMERTABLE list.
		 */
		do
		{
			if (pTable->next == NULL)
			{
				pTable->next = OslMalloc(sizeof(TIMERTABLE));
				memset(pTable->next, 0, sizeof(TIMERTABLE));
				g_timer_table_size += SIMULTANEOUS_TIMER_NUM;
				pTable = pTable->next;
				i = 0;
				break;
			}
			pTable = pTable-> next;
		} while (pTable != NULL);
	}
	else
	{
		/* find the empty record in g_timer_table list */
		i = 0;
		do
		{
			if (pTable->event_id[i] == NULL)
			{	/* find the empty space */
				break;
			}
			i++;
			if (i >= SIMULTANEOUS_TIMER_NUM )
			{
				pTable = pTable->next;
				i = 0;
			}
		} while (pTable != NULL);

		if (pTable == NULL)
		{
			/* Can't find the empty space in TIMERTABLE list, assert!!! */
			MMI_ASSERT(0);
		}
	} /* if (g_timer_table_used >= g_timer_table_size) */

	/* 
	 * already find the empty record, and then start timer 
	 *
	 * event_sheduler1 = NO alignment scherulder
	 * event_sheduler2 = alignment scherulder (low power)
	 */
	if (alignment == TIMER_IS_NO_ALIGNMENT)
	{
		/* MSB(Most Significant Bit) is align_timer_mask */
		pTable->timer_id[i] = nTimerId | NO_ALIGNMENT_TIMER_MASK;
   	pTable->event_id[i] = evshed_set_event (event_scheduler1_ptr,
						(kal_timer_func_ptr)L4CallBackTimer, (void *)nTimerId, temp);
		pTable->callback_func[i] = TimerExpiry;
		g_timer_table_used ++;
	}
	else if (alignment == TIMER_IS_ALIGNMENT)
	{
		/* MSB(Most Significant Bit) is align_timer_mask */
		pTable->timer_id[i] = nTimerId | ALIGNMENT_TIMER_MASK;
   	pTable->event_id[i] = evshed_set_event (event_scheduler2_ptr,
						(kal_timer_func_ptr)L4CallBackTimer, (void *)nTimerId,temp );
		pTable->callback_func[i] = TimerExpiry;
		g_timer_table_used ++;
	}
}


/*****************************************************************************
* FUNCTION
*  L4StopTimer
* DESCRIPTION
*   This function is to stop timer. To added for two kinds of timer, one is for exactly time, another allow to delay.
*
* PARAMETERS
*  a  IN      nTimerId
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void L4StopTimer(unsigned short nTimerId)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	TIMERTABLE	*pTable = &g_timer_table;
	U16			i = 0;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_ASSERT(nTimerId<MAX_TIMERS);
	MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_L4DRV_STOPTIMER_HDLR, nTimerId));

	/* find the nTimerId in TIMERTABLE list */
	do
	{
		/* MSB(Most Significant Bit) of timer_id[i] is align_timer_mask */
		if ((pTable->timer_id[i] & (~NO_ALIGNMENT_TIMER_MASK)) == nTimerId)
		{
			/* find the nTimerId */
			if (pTable->event_id[i] != NULL)
			{
				if ((pTable->timer_id[i] & NO_ALIGNMENT_TIMER_MASK) == NO_ALIGNMENT_TIMER_MASK)
				{	/* NO_ALIGNMENT_TIMER */
					evshed_cancel_event (event_scheduler1_ptr, &(pTable->event_id[i]));
				}
				else
				{	/* ALIGNMENT_TIMER */
					evshed_cancel_event (event_scheduler2_ptr, &(pTable->event_id[i]));
				}

				g_timer_table_used --;
				pTable->event_id[i] = NULL;
				pTable->timer_id[i] = 0;
				pTable->callback_func[i] = NULL;
			}
			break;
		}
		i++;
		if (i >= SIMULTANEOUS_TIMER_NUM )
		{
			pTable = pTable->next;
			i = 0;
		}
	} while (pTable != NULL);
	
	/* if can't find nTimerId, do nothing */

}
#endif
//tangjie del end 20060708
#else

#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
/*... Add More MMI header */
#include "globalconstants.h"
#include "queuegprot.h"
#include "oslmemory.h"
#undef __NEWSIMULATOR 


/*****************************************************************************
* FUNCTION
*  mmi_frm_set_key_handle_in_high_frequency
* DESCRIPTION
*   The dummy function for PC simulator
*
* PARAMETERS
*  a  IN     is_enable
* RETURNS
*  VOID.
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_frm_set_key_handle_in_high_frequency(MMI_BOOL is_enable)
{
   /* do nothing */
}



#endif /* MMI_ON_HARDWARE_P */

