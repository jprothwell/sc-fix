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
 *   ShutdownApp.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for shutdowns whole system operation
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
/****************************************************************************
* Include Files                                                                
*****************************************************************************/
#include "mmi_features.h"
#define __NEWSIMULATOR
#include "buildcfg.h"
#include "frameworkstruct.h"
#include "debuginitdef.h"
#include "allappgprot.h"
#include "gdi_include.h" 
#include "wgui_categories.h"
#include "timerevents.h"
#include "eventsgprot.h"
#include "audioinc.h"
#include "protocolevents.h"
#include "mainmenudef.h"
#include "debuginitdef.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "profilesgexdcl.h"
#include "keybrd.h"
#include "led.h"
#include "gpioinc.h"
#include "alarmframeworkprot.h"     //dyj del 20060514
#include "bootup.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "globalscrenum.h"
#include "messagesexdcl.h"  					/* extern void deinitmessagesapp (void) */
#include "callmanagementgprot.h"  			/* extern void deinitcm(void) */
#include "wgui_categories_multimedia.h"
#include "fat_fs.h"
#include "filesystemdef.h"						/* file function */
#include "filemanagergprot.h"					/* file path / file error */
#include "filemanagerdef.h"					/* error string id */
#include "filemgr.h"
#include "wpssprotos.h"							/* phone setup interface */
#include "debuginitdef.h"						/* mmi_assert */
#include "nvramtype.h"							/* nvram data type */
#include "nvramprot.h"							/* nvram access fucntions */	
#include "nvramenum.h"							/* nvram id define */
#include "sublcdhistorygprot.h"				/* forcesublcdscreen() */

#include "mmi_trace.h"
#ifdef MMI_ON_HARDWARE_P
#include "l4dr.h"
#include "lcd_sw.h"
#include "lcd_if.h"
#endif
#include "dm.h"
#include "simdetectiondef.h"
#undef __NEWSIMULATOR

/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/*****************************************************************************
* Global Variabl                                                          
*****************************************************************************/


/*****************************************************************************
* Global Function                                                           
*****************************************************************************/
extern void QuitSystemOperation(void);					/* system shutdown operation */
extern void ShutdownSystemOperation(void);			/* kill all the object */
extern void SimOffRsp(void*);
extern void PowerOffRsp(void*);
extern void NwPlmnSetRSP(void*);
extern void	LcdOffRsp(void*);
extern void CallBackPowerOffAnimationComplete(GDI_RESULT ret);
extern void DeInitApps(void);
extern void	DeInitAllApplications(void);
extern void DeInitInsertApp(void);
extern void SPOFDeInit(void);
extern void SystemShutdownTimeout(void);
extern void KillWindowObject(void);			/* kill all semaphore and thread objects	*/
extern U16 	KillObjects(void);				/* kill windown object */
extern void PhnsetPwrOffDisplayDefaultID(void);
extern BOOL USBIsIn(void);

#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
extern INT32 VDS_FlushCacheAll(VOID);
#endif

#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)   
 extern BOOL tgt_FlushFactorySettings(VOID);      
#endif

#ifdef __MMI_TODOLIST__
extern void DeInitToDoListApp(void);
#endif
extern void DeInitTimers(void);  
extern void StopPowerOnOffLED(void);
#if defined(__MMI_SAVE_CURRENT_TIME__)
extern void SaveCurrentSystemTime(void);
#endif

/***************************************************************************** 
* Local Variable
*****************************************************************************/
static U8 	gNetworkDeattachRspFlag = 0;
static U8 	gWapPowerOffRspFlag = 0;
static U8 	gShutDownAudioPlayFinishRspFlag = 0;
U8 	gPowerOffPeriod = 0;
static BOOL	g_shutdown_is_terminate_diplay;
static BOOL	g_shutdown_is_successful_play = TRUE;
static BOOL	g_shutdown_is_image;
static BOOL	g_shutdown_is_still_image = FALSE; 
#ifdef MMI_ON_WIN32
BOOL flagMMIStart = FALSE;
#endif

/*****************************************************************************
* Local Function 
*****************************************************************************/
static void mmi_shutdown_froce_terminate_display(void);





 
/**************************************************************

	FUNCTION NAME		: WapBrowserPoweroffRsp(void)

  	PURPOSE			: request to deregister network

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS			: void

**************************************************************/
static void RequestRacDetach()
{
//micha0120 begin

#ifdef MMI_ON_HARDWARE_P
	MYQUEUE Message;

	SetProtocolEventHandler(NwPlmnSetRSP,MSG_ID_MMI_NW_PWROFF_DETACH_RSP);		
		
	Message.oslSrcId=MOD_MMI;
	Message.oslDestId=MOD_L4C;
	Message.oslMsgId = MSG_ID_MMI_NW_PWROFF_DETACH_REQ;
      

	Message.oslDataPtr = (oslParaType *)NULL;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
#endif

//micha0120 end

    
   /* Move here for the timer duration is waiting network response */
   /* Not included WAP Shutdown */
   //StartTimer(SYSTEM_SHUTDOWN, POWER_OFF_TIMER_STAGE_1, SystemShutdownTimeout);
}

#ifdef WAP_SUPPORT  

/**************************************************************

	FUNCTION NAME		: WapBrowserPoweroffRsp(void)

  	PURPOSE				: After the WAP browser is shutdown, deregister network

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

**************************************************************/
static void WapBrowserPoweroffRsp (void* inMsg)  
{
     
    //RequestRacDetach();
    PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "[WapBrowserPoweroffRsp] Shutdown Proc: WAP Back"));
    gWapPowerOffRspFlag = 1;
}

/**************************************************************

	FUNCTION NAME		: WapBrowserPoweroffRsp(void)

  	PURPOSE				: After the WAP browser is shutdown, deregister network

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
static void RequestWapShutdown()
{
    MYQUEUE Message;
    SetProtocolEventHandler(WapBrowserPoweroffRsp,
	                      MSG_ID_WAP_BROWSER_POWEROFF_RSP);	

    Message.oslSrcId = MOD_MMI;
    Message.oslDestId = MOD_WAP;
    Message.oslMsgId = MSG_ID_WAP_BROWSER_POWEROFF_REQ;

    Message.oslDataPtr = (oslParaType *) NULL;
    Message.oslPeerBuffPtr = NULL;
    OslMsgSendExtQueue(&Message);

    /* 
     * The WAP browser should send back MSG_ID_WAP_BROWSER_POWEROFF_RSP 
     * in limited time.
     */ 
}

#endif /* WAP_SUPPORT */
 
/**************************************************************

	FUNCTION NAME		: ShutDownAudioFinishRes(void)

  	PURPOSE				: handler fn for power off tone finish  ind

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 ShutDownAudioFinishRes(void* info)
{
#ifdef MMI_ON_HARDWARE_P

	//mmi_eq_play_audio_finish_ind_struct *audioFinishInd	= (mmi_eq_play_audio_finish_ind_struct*)info;

	//if( audioFinishInd->identifier == gcurrentprofile.toneSetup.powerOffTone )
	{
		PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "[ShutDownAudioFinishRes] Shutdown Proc: Audio Back"));

		gShutDownAudioPlayFinishRspFlag = 1;
		gNetworkDeattachRspFlag = 1;	/*chenhe,for i have delete this msg for some bug(some sim card will leads to the variable be 1 immediately,so when power off music is end,value it 1*/		

		/*
		if( gNetworkDeattachRspFlag == 1)
		   StopTimer(SYSTEM_SHUTDOWN);
	    */
	}
#endif
//	mmi_trace(0,"ShutDownAudioFinishRes,g_shutdown_is_still_image is %d,g_shutdown_is_successful_play is %d",g_shutdown_is_still_image,g_shutdown_is_successful_play);
	if(g_shutdown_is_still_image && g_shutdown_is_successful_play)
		CallBackPowerOffAnimationComplete(GDI_GIF_LAST_FRAME);

	return MMI_FALSE;
}



/*****************************************************************************
* FUNCTION
*	 mmi_shutdown_play_video_callback
* DESCRIPTION
*   play shutdown video result callback function
* PARAMETERS
*	 S16 		IN 	remain repeat play count
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_shutdown_play_video_callback(MDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(g_shutdown_is_terminate_diplay)
	{
		/* means already terminated by time out, shall skip this callback */
		return;
	}

	/* treat as gif animation */
	/* change to GIF's return */
	
	if(result < 0)
	{
 	  	gShutDownAudioPlayFinishRspFlag = 1;		
		CallBackPowerOffAnimationComplete(-1);
	}
	else
	{
		/* play finish successfully */
		g_shutdown_is_successful_play = TRUE;
		
		/* notify successful play once */
 	  	gShutDownAudioPlayFinishRspFlag = 1;
		CallBackPowerOffAnimationComplete(GDI_GIF_LAST_FRAME);
	}
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_shutdown_show_image_callback
* DESCRIPTION
*   show power off image result callback function
* PARAMETERS
*	 result		IN 	play video's reult
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_shutdown_show_image_callback(GDI_RESULT result)
{
	TRACE_SYS_FUNCTION();
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   mmi_trace(1,"mmi_shutdown_show_image_callback,result is %d",result);
	if(g_shutdown_is_terminate_diplay)
	{
		/* means already terminated by time out, shall skip this callback */
		return;
	}
	gShutDownAudioPlayFinishRspFlag = 1;
	if(result < 0)
	{
		CallBackPowerOffAnimationComplete(result);
	}
	else if(result == GDI_IMAGE_IS_STILL_IMAGE)
	{
		g_shutdown_is_successful_play = TRUE;
		g_shutdown_is_still_image = TRUE;
		CallBackPowerOffAnimationComplete(result);
	}
	else
	{
		TRACE_SYS_FUNC("CallBackPowerOffAnimationComplete");
		/* play finish successfully */
		g_shutdown_is_successful_play = TRUE;
		
		CallBackPowerOffAnimationComplete(result);
	}
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_shutdown_froce_terminate_display
* DESCRIPTION
*   force terminate shutdown display
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_shutdown_froce_terminate_display(void)
{
	TRACE_SYS_FUNCTION();
	TRACE_EF(g_sw_SYSTEM, "g_shutdown_is_successful_play = %d", g_shutdown_is_successful_play);
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* if can successful play once (can loop) , do not need force teminate */
	if(!g_shutdown_is_successful_play)
	{
		g_shutdown_is_terminate_diplay 	= TRUE;
		g_shutdown_is_successful_play		= TRUE;

		CallBackPowerOffAnimationComplete(-1);
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_shutdown_stop_animation
* DESCRIPTION
*   this function is used to stop power off animation screen, called when ready 
*   to shutdown.
*	 
*	 [Note]
*	 when ready to enter power on screen, it will enter goto_opening_screen(),
*	 in this function, it will call InitAllApplications(), there are NVRAM access 
*   in this function, which will cause NVRAM & timer interlace problem.
*
*	 we have to stop all animation timer when we are sure poweron procdure success.
*	 we may call this function in mmi_ready_to_idle_screen_ind.
*	
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_shutdown_stop_animation(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U8		pwroff_flag;
	S16 	error;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	/* stop force teminate timer */
	coolsand_UI_cancel_timer(mmi_shutdown_froce_terminate_display);

	if(g_shutdown_is_image)
	{
		/* stop image playing */
		StopCategory166Animation();	
	}
	else
	{
		#if defined(__MMI_VIDEO__)
			/* stop video playing */
			StopCategory224Video();
		#endif /* __MMI_VIDEO__ */ 
	}

	/* write proper value to NVRAM */
	if(!g_shutdown_is_successful_play)
	{
		#if defined(__MMI_POWER_ON_OFF_DISPLAY__)
			/* restore default */
			/*modified start by wuzc at 20061122*/
			PhnsetRstPwrOffDisplayDefault();  //dyj del 20060514
			/*modified end by wuzc at 20061122*/
		#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */		
	}
	
	pwroff_flag = PWROFF_DISPLAY_SUCCESS;
	WriteValue(NVRAM_POWER_OFF_DISPLAY_SUCCESS, &pwroff_flag, DS_SHORT, &error);
	
}



/*****************************************************************************
* FUNCTION
*	 mmi_shutdown_entry_animation_screen
* DESCRIPTION
*   start play power off animation
*	 (1) GIF animation
*	 (2) Still image
*	 (3) Video clip
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#ifdef __MMI_NOKIA_STYLE_N800__
extern U8 g_pwr_nokia_setted ;
#endif
void mmi_shutdown_entry_animation_screen(void)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	BOOL			is_file;
	U16			disp_id;
	PS8			filename_ptr;
	FS_HANDLE	fs_handle;
	U16			res_type;
	S16 			error;
	PS8                  error_str;
	U8				pwroff_flag;
	U8				pwron_flag;
	U16			force_stop_time;
	
	#if defined(__MMI_VIDEO__)
		BOOL			play_audio;
	#endif /* __MMI_VIDEO__ */
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	TRACE_SYS_FUNCTION();
	
	/* init */
	filename_ptr = NULL;
	is_file		 = FALSE;
	g_shutdown_is_successful_play = FALSE;
	g_shutdown_is_image = TRUE;
		
	/*del start by wuzc at 20061128*/
	#if 0
	TurnOnBacklight(0);
	#endif
	/*del end by wuzc at 20061128*/

	/* 
		if shutdown while power on display is playing, we still dont know 
		it will play successfully or not, so set the flag to 0xff, let it check next time
	*/
	ReadValue(NVRAM_POWER_ON_DISPLAY_SUCCESS, &pwron_flag, DS_SHORT, &error);

	if(pwron_flag == PWRON_DISPLAY_PLAYING)
	{
		pwron_flag = PWRON_DISPLAY_SUCCESS;
		WriteValue(NVRAM_POWER_ON_DISPLAY_SUCCESS, &pwron_flag, DS_SHORT, &error);
	}
	
	/* this timer is used to avoid gif/video play too long, will force to enter idle */
	ReadValue(NVRAM_POWER_OFF_FORCE_STOP_TIME, &force_stop_time, DS_SHORT, &error);

	/* not set in NVRAM, use defulat 15 sec limit */
	TRACE_EF(g_sw_SYSTEM,"force_stop_time == 0x%0x", force_stop_time);
	if(force_stop_time == 0xffff)
	{
		force_stop_time = 15000;
	}
	
	coolsand_UI_start_timer(force_stop_time, mmi_shutdown_froce_terminate_display);
	g_shutdown_is_terminate_diplay = FALSE;

	/*******************************************/
	/* Valid power off display checking 		 */
	/*******************************************/

	/* Valid power off display restoration checking */
	ReadValue(NVRAM_POWER_OFF_DISPLAY_SUCCESS, &pwroff_flag, DS_SHORT, &error);

	#if defined(__MMI_POWER_ON_OFF_DISPLAY__)
		TRACE_SYS_FUNC("(__MMI_POWER_ON_OFF_DISPLAY__)");
		if(pwron_flag == PWRON_DISPLAY_FAILED)
		{
			/* if not 0xff, means something wrong happened during playing power off display */
			/* we will restore power off display to default settings */
			//PhnsetRstPwrOffDisplayDefault(); //qiff del for GREENSTONE bug: 9365
		}
	#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */

	/* set the flag to 0x01 , if play successfully to end, will reset back to 0xff */
	pwroff_flag = PWROFF_DISPLAY_PLAYING;
	WriteValue(NVRAM_POWER_OFF_DISPLAY_SUCCESS, &pwroff_flag, DS_SHORT, &error);


#if defined(__MMI_POWER_ON_OFF_DISPLAY__)	
	TRACE_SYS_FUNC("(__MMI_POWER_ON_OFF_DISPLAY__)");
	/*******************************************/
	/* Get animation resource from phone setup */
	/*******************************************/
	PhnsetGetPwrOffDisp(&disp_id);
	mmi_trace(g_sw_SYSTEM, "disp_id = 0x%0x",disp_id); //0x35f5
	if(disp_id == 0xff)	/* invalid format */
	{
		/* reset as default */
		PhnsetRstPwrOffDisplayDefault();

		/* this id must be a valid id */
		PhnsetGetPwrOffDisp(&disp_id);
	}
	else if(disp_id == 0x00 		/* source from file */
			|| disp_id == 0x01)		/* short file name*/
	{
		disp_id = 0x00;
	
		/* filename buffer is allocated in phnset */
		filename_ptr 	= PhnsetGetPwrOffFileName();
		fs_handle = MMI_FS_Open((U8*)filename_ptr, FS_READ_ONLY);//modified by zhoumn@2007/07/25

		if(fs_handle < FS_NO_ERROR)
		{
			/* open file failed */
			/* reset as default */
			
		#ifdef MMI_ON_HARDWARE_P
		#if (CSW_EXTENDED_API_PMD_USB == 0)
			if(USBIsIn() )
			{
				PhnsetPwrOffDisplayDefaultID();
			}
			else
		#endif
				{
					PhnsetRstPwrOffDisplayDefault();
				}
		#endif

			
			/* this id must be a valid id */
			PhnsetGetPwrOffDisp(&disp_id);
			filename_ptr = NULL;
		}
		else
		{
			/* open file successful */
			MMI_FS_Close(fs_handle); //modified by zhoumn@2007/07/25
		}

	    if(!mmi_fmgr_util_file_limit_check(FMGR_LIMIT_POWER_ON_OFF_DSPL, filename_ptr, &error_str))
		{
			/* limit check failed */
			/* reset to default */
		#ifdef MMI_ON_HARDWARE_P
		#if (CSW_EXTENDED_API_PMD_USB == 0)
			if(USBIsIn())
			{
				PhnsetPwrOffDisplayDefaultID();
			}
			else
		#endif
				{
					PhnsetRstPwrOffDisplayDefault();
				}
		#endif

			/* this id must be a valid id */
			PhnsetGetPwrOffDisp(&disp_id);
			filename_ptr = NULL;
		}

	}
	else
	{
		/* source from intenal resource */
		filename_ptr = NULL;
	}

	/*******************************************/
	/* Check if is image or video					 */
	/*******************************************/
	/* get file resource type */
	if(filename_ptr != NULL)
	{
		/* source from file */
		res_type = mmi_fmgr_get_file_group(filename_ptr);//opened by zhoumn@2007/07/25

		if(res_type == FMGR_GROUP_IMAGE)
		{
			g_shutdown_is_image = TRUE;
		}
		else if(res_type == FMGR_GROUP_VIDEO)
		{
			g_shutdown_is_image = FALSE;
		}
		else
		{
			/* unsupported file format ext - restore default */
			PhnsetRstPwrOffDisplayDefault();

			/* this id must be a valid id */
			PhnsetGetPwrOffDisp(&disp_id);
			filename_ptr = NULL;
		}
	}				

	mmi_trace(g_sw_SYSTEM, "disp_id = 0x%0x", disp_id);
	
	/* check internal resource type */
	if(filename_ptr == NULL)
	{
		 res_type = PhnsetGetDisplayType(disp_id);
	
		if(res_type == PHNSET_TYPE_DISP_IMAGE)
		{
			g_shutdown_is_image = TRUE;
		}
		else if(res_type == PHNSET_TYPE_DISP_VIDEO)
		{
			g_shutdown_is_image = FALSE;
		}
		else
		{
			MMI_ASSERT(0);
		}
	}

#else

	/* play from internal build in resource */
	g_shutdown_is_image 	= TRUE;
	disp_id 					= IMG_ID_PHNSET_OFF_ANIMATION_DEFAULT1;//__CUSTPACK_MULTIBIN__
	filename_ptr			= NULL;
	
#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */

#ifdef __MMI_NOKIA_STYLE_N800__
       ReadValue(NVRAM_PWR_NOKIA_SETTED, &g_pwr_nokia_setted, DS_BYTE, &error);

        switch(g_pwr_nokia_setted)
        {
            case 1:
                disp_id = IMG_ID_NOKIA_PWR_LOGO;
                g_shutdown_is_image 	= TRUE;
                filename_ptr		= NULL;
                break;

            default:
                break;
        }
#endif
	/*******************************************/
	/* Start to play									 */
	/*******************************************/
	if(g_shutdown_is_image)
	{
		TRACE_SYS_FUNC("(g_shutdown_is_image)");
		/* Play GIF animation or still image */
		ShowCategory166Screen(disp_id, filename_ptr, mmi_shutdown_show_image_callback);	
		ShowCategory313Screen(IMG_SUBLCD_SYSTEM_SHUTDOWN, NULL);

		/* play power off tone */
		TRACE_EF(g_sw_SYSTEM, "gcurrentprofile.toneSetup.powerOffTone = %d", gcurrentprofile.toneSetup.powerOffTone);
		if(gcurrentprofile.toneSetup.powerOffTone != POWEROFF_SILENT)
  		{
  		  	playRequestedTone(POWER_OFF_TONE);   //dyj del 20060514 //add by panxu 20061228
			
  	  		/* Shall set interrupt event handler */
			/* mdi will register event handler itself, shall not preempt it */
			SetInterruptEventHandler(ShutDownAudioFinishRes, NULL, PRT_L4AUD_AUDIO_PLAY_FINISH_IND);
		}
		else
		{
	 	  	gShutDownAudioPlayFinishRspFlag = 1;
			PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "[ShutdownSystemOperation] Shutdown Proc: Audio Back"));
		}
	}
	else
	{
		/* Play video clip */
		#if defined(__MMI_VIDEO__)

			/* check silent mode */
			play_audio = (gcurrentprofile.toneSetup.powerOffTone != POWEROFF_SILENT)?TRUE:FALSE;

			/* set volume as ring tone volume*/
			mdi_audio_set_volume(MDI_VOLUME_MEDIA, GetRingVolumeLevel());
				
			/* entry video play screen */
			ShowCategory224Screen(	disp_id, 									/* video id */
											filename_ptr,								/* filename */
											0, 											/* repeat_count */ /* 0 means infinite */
											TRUE,											/* is_visaul_update */
											play_audio, 								/* is_play_audio */
											FALSE,										/* is_lcd_sleep */	/* already in non sleep mode */
											GDI_COLOR_BLACK,							/* bg_color */
											mmi_shutdown_play_video_callback,	/* (*play_finish_callback)(S16) */
											NULL);										/* gui_buffer */			
		#else
			MMI_ASSERT(0);
		#endif /* __MMI_VIDEO__ */
	
	}

}



/**************************************************************

	FUNCTION NAME		: ShutdownSystemOperation(void)

  	PURPOSE				: it shutdowns whole system opeartion 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ShutdownSystemOperation(void)
{
	/*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	StopTimer(SYSTEM_SHUTDOWN);
	TRACE_SYS_FUNCTION();
#ifdef __MMI_TOUCH_SCREEN__
    mmi_pen_disable_quick_launch( );
#endif
   gPowerOffPeriod = 1;

#ifdef MMI_ON_WIN32
	if (!flagMMIStart)
	{
//		if (using_dll_ui != TRUE)
//		{
//			exit(0);
//		}
	//	else
//		{
			return;
//		}
  
	}
#endif

	
/*modified by wuzc at 20061121*/
	TurnOnBacklight(1);
/*modified by wuzc at 20061121*/
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);


	ClearAllKeyHandler();
	/* stops one sec timer */
	StopTimer(TIMER_REAL_TIME_CLOCK);

	/*Lisen added to stop background music*/
/*Guojian Modify Start For 10462 Ver: GreenStone  on 2008-11-27 16:4 */
//	mdi_audio_suspend_background_play();
	mdi_audio_stop_all();//chenhe to fix bug 11341 
#ifdef MMI_ON_HARDWARE_P
	DM_Close_All_Audio();
#endif
/*Guojian Modify End  10462 dd Ver: GreenStone  on 2008-11-27 16:4 */



/*Panxu Del Start For 6022,6085 Ver: TBM780  on 2007-8-7 15:27 */
	#if 0
	gShutDownAudioPlayFinishRspFlag = 1; //add by panxu for stop audio 200706022
	#endif
/*Panxu Del End  For 6022,6085 Ver: TBM780  on 2007-8-7 15:27 */
	#if 0
	gNetworkDeattachRspFlag=1;
	gWapPowerOffRspFlag = 1;
#endif
	/* clear history, key, timer, protocol, hardware events & other objects */

	DeInitFramework();
#if defined(TGT_GALLITE_G800)
	AlmDeInit();  //dyj del 20060514
#endif
	ClearAllProtocolEventHandler();
	DeInitTimers();  
	DeInitApps();
#if !defined(TGT_GALLITE_G800)
	AlmDeInit();  //dyj del 20060514
#endif
	/*	Temporary: Should be done in a more formal manner	*/
	terminate_UI_demo();
	


	/* dispalay shutdown animation screen */
	mmi_shutdown_entry_animation_screen();
	
	TRACE_SYS_FUNC("!!!mmi_shutdown_entry_animation_screen();");
	#ifdef __MMI_RUNNING_LED__
	StartLEDPatternPowerOff();
	#endif

     
	/*
	#ifdef WAP_SUPPORT 	
	RequestWapShutdown();
	#else
	RequestRacDetach();
	#endif
	*/
#if defined(__MMI_SAVE_CURRENT_TIME__)
	SaveCurrentSystemTime();
#endif

   	 
   /* Send RAC detech and WAP shutdown at the same time */
	RequestRacDetach();
	#ifdef WAP_SUPPORT 	
	RequestWapShutdown();
	#else

	gWapPowerOffRspFlag = 1;
	#endif
	  #ifdef __NATIVE_JAVA__
	if(checkVMOn())
	{
		#ifdef MMI_ON_HARDWARE_P
		gcj_NativeJavaQuit(1);
		#endif				
		COS_Sleep(1000);

	}
      #endif
	  

	TRACE_SYS_FUNC("SYSTEM_SHUTDOWN");
	
	StartTimer(SYSTEM_SHUTDOWN, POWER_OFF_TIMER_STAGE_1, SystemShutdownTimeout); 
}



/**************************************************************

	FUNCTION NAME		: SystemShutdownTimeout(void)

  	PURPOSE				: Call back fn on timeout of timer SYSTEM_SHUTDOWN

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void SystemShutdownTimeout(void)
{
	TRACE_SYS_FUNCTION();
	StopTimer(SYSTEM_SHUTDOWN);
	gNetworkDeattachRspFlag=1;
	gShutDownAudioPlayFinishRspFlag = 1;
	gWapPowerOffRspFlag = 1;

#if 0 //move to QuisystemOperation
	/* turn off backlight to prevent blank screen */
	SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0);

	mmi_shutdown_stop_animation();/* stop shut down animations */ 

	/*stop music again, cause ExitCategoryVdoplyScreen will resume it*/
	mdi_audio_suspend_background_play();
  	/* disable key pad tone */
  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);
#endif 
	StartTimer(SYSTEM_SHUTDOWN, POWER_OFF_TIMER_STAGE_2, QuitSystemOperation);  

#ifdef MMI_ON_WIN32
	QuitSystemOperation();	
#endif
}


/**************************************************************

	FUNCTION NAME		: QuitSystemOperation(void)

 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void QuitSystemOperation(void)
{

#ifdef MMI_ON_HARDWARE_P
	MYQUEUE Message;
#endif
	TRACE_SYS_FUNCTION();
	/* turn off backlight to prevent blank screen */
	SetGPIOLevelReq(GPIO_DEV_LED_MAINLCD, LED_LIGHT_LEVEL0);

	mmi_shutdown_stop_animation();/* stop shut down animations */ 

	/*stop music again, cause ExitCategoryVdoplyScreen will resume it*/
	mdi_audio_suspend_background_play();
  	/* disable key pad tone */
  	mmi_frm_kbd_set_tone_state(MMI_KEY_TONE_DISABLED);


	/* exit poweroff anmation screen by enter a dummy screen */
	EntryNewScreen(GLOBAL_SCR_DUMMY, NULL, NULL, NULL);

	/*stop music again, cause ExitCategoryVdoplyScreen will resume it*/
//	mdi_audio_suspend_background_play();

	gPowerOffPeriod = 1;

	PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "[QuitSystemOperation] ShutDown Proc: Poweroff! "));

	/* stops shutdown timer */
   StopTimer(SYSTEM_SHUTDOWN);

#ifdef MMI_ON_WIN32

	/* kill all the semaphore and thread objects	*/
	KillObjects();	

	/* kill windown object	*/
	KillWindowObject();

#else 
	#ifdef __MMI_RUNNING_LED__
	StopLEDPatternPowerOff();
    #endif
	/* power off lcd */
	lcd_power_on(MAIN_LCD, 0);

	#ifdef __MMI_SUBLCD__
		lcd_power_on(SUB_LCD, 0);
	#endif

	/* Message for Power off Device */
 
#if 1
	SetProtocolEventHandler(PowerOffRsp,PRT_POWER_OFF_RSP);
	Message.oslSrcId = MOD_MMI;
	Message.oslDestId = MOD_L4C;
	Message.oslMsgId = PRT_POWER_OFF_REQ;
	Message.oslDataPtr = NULL;
	Message.oslPeerBuffPtr= NULL;
	OslMsgSendExtQueue(&Message);
#endif
 
 
#if 0
KeyPadLCDBackLightStop();
DM_DeviceSwithOff();
#endif
 
#endif
		OslDumpDataInFile();
}



/**************************************************************

	FUNCTION NAME		: PowerOffRsp

  	PURPOSE				: Res handling of Power Off req.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
extern BOOL gSystemResetFlag;
extern UINT8 isChargerIn(void);

void PowerOffRsp(void* info)
{
	TRACE_SYS_FUNCTION();		
	KeyPadLCDBackLightStop();
#ifdef MMI_ON_HARDWARE_P
{
		mmi_eq_power_off_rsp_struct *p=(mmi_eq_power_off_rsp_struct*) info;
		if(p->result==TRUE)
			PRINT_INFORMATION(("@@@@@ ShutDown Process Success!!!!!  @@@@@"));
		else
			PRINT_INFORMATION(("@@@@@ ShutDown Process Failure!!!!!  @@@@@"));
		//wangzl:modify B if go here for switch sim, then reset the system

		/*wangrui move the (0 == isChargerIn()  condition  to DM_util.c, 2008.12.17 for fixbug10731*///chenhe 20081017 for bug 10028 , persuaded by kongdeqi&& (0 == isChargerIn())
		if(!gSystemResetFlag ) 
		{
			DM_DeviceSwithOff();
		}
		else
		{
                    gSystemResetFlag = MMI_FALSE;
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)      
                    VDS_FlushCacheAll(); 
#endif    
#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)   
       tgt_FlushFactorySettings();
#endif
                    DM_Reset();
		}
		//wangzl:modify E
}
#endif									
}


/**************************************************************

	FUNCTION NAME		: NwPlmnSetRSP

  	PURPOSE				: response handling of network deregister req

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void NwPlmnSetRSP(void* info)
{
return; //chenhe,don't stop power off animiation when this rsp
#ifdef MMI_ON_HARDWARE_P
	mmi_nw_pwroff_detach_rsp_struct *p=(mmi_nw_pwroff_detach_rsp_struct*) info;
	if(p->result.flag == L4C_OK)
	{
		TRACE_SYS_FUNCTION();
		/*
		if( gShutDownAudioPlayFinishRspFlag == 1)
			StopTimer(SYSTEM_SHUTDOWN);
	   */
		
      PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "[NwPlmnSetRSP] Shutdown Proc: DeAttach Rsp Back"));
		gNetworkDeattachRspFlag=1;

		if(g_shutdown_is_still_image && g_shutdown_is_successful_play)
			CallBackPowerOffAnimationComplete(GDI_GIF_LAST_FRAME);

	}
	else
      PRINT_INFORMATION_2 ((MMI_TRACE_G7_MISC, "[NwPlmnSetRSP] Shutdown Proc: DeAttach Rsp Fail"));

	
#endif	
}


/**************************************************************

	FUNCTION NAME		: CallBackPowerOffAnimationComplete

  	PURPOSE				: Call back fn for Power off animation.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void CallBackPowerOffAnimationComplete(GDI_RESULT ret)
{

   
TRACE_EF(g_sw_SYSTEM, "ret = %d", ret);
	if( ret < 0 ) 
   {
		/* Broken animation image */
		QuitSystemOperation();
		return;
	}

TRACE_EF(g_sw_SYSTEM, "gNetworkDeattachRspFlag = %d, gShutDownAudioPlayFinishRspFlag = %d, gWapPowerOffRspFlag = %d",
	gNetworkDeattachRspFlag, gShutDownAudioPlayFinishRspFlag, gWapPowerOffRspFlag);

	if(gNetworkDeattachRspFlag && gShutDownAudioPlayFinishRspFlag && gWapPowerOffRspFlag )
	{
		gNetworkDeattachRspFlag=0;
		gShutDownAudioPlayFinishRspFlag = 0;
		gWapPowerOffRspFlag = 0;
		QuitSystemOperation();
	}
}


pBOOL IsPowerOffPeriod(void)
{
   if( gPowerOffPeriod == 1 )
      return MMI_TRUE;
   else
      return MMI_FALSE;
}

/**************************************************************

	FUNCTION NAME		: DeInitAllApplications()

  	PURPOSE				: De-initialization

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeInitAllApplications(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   #if 1   //dyj del 20060514
	SPOFDeInit();  
	//DeInitDateTimeSetup();
	DeInitMessagesApp();
#ifdef __MMI_TODOLIST__
	DeInitToDoListApp();
#endif     
	DeInitCM();
#endif
}

/**************************************************************

	FUNCTION NAME		: DeInitInsertApp()

  	PURPOSE				: DeInitialization

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DeInitInsertApp(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_TODOLIST__
	//DeInitToDoListApp();  //dyj del 20060514
#endif

}

void DeInitApps(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	extern U8 gInitAllAppFlag;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(gInitAllAppFlag==1)
		DeInitAllApplications();
	else if(!HasValidSIM())
		DeInitInsertApp();
}

void DeInitTimers(void)
{
	U16 timer_id;

	for(timer_id=KEY_TIMER_ID_NONE;timer_id<MAX_TIMERS;timer_id++)
		StopTimer(timer_id);
}
