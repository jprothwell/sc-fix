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
 *  MiscFunctions.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Misc Functions
 *	 (1) Power On/Off
 *	 (2) Analog clock 
 *  (3) Others - UI
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/****************************************************************************
* Include Files                                                                
*****************************************************************************/
#include "mmi_features.h"
#define __NEWSIMULATOR
#include "eventsgprot.h"
#include "fontres.h"
#include "csp.h"
#include "gdi_include.h"                          
#include "gui_setting.h"
#include "custdataprots.h"
#include "alarmframeworkprot.h"
#include "idleappdef.h"
#include "historygprot.h"
#include "miscfunctions.h"
#include "simdetectiongprot.h"
#include "unicodexdcl.h"
#include "globaldefs.h"
#include "led.h"
#include "gpioinc.h"
#include "simdetectiongexdcl.h"
#include "timerevents.h"
#include "idleappprot.h"
#include "profilegprots.h"
#include "fat_fs.h"
#include "fmt_def.h"    
#include "filesystemdef.h"						/* file function */
#include "filemanagergprot.h"					/* file path / file error  */
#include "filemanagerdef.h"					/* error string id */
#include "filemgr.h"
#include "wpssprotos.h"							/* phone setup interface */
#include "debuginitdef.h"						/* mmi_assert */
#include "nvramtype.h"							/* nvram data type */
#include "nvramprot.h"							/* nvram access fucntions */	
#include "nvramenum.h"							/* nvram id define */
#include "callmanagementiddef.h"				/* for mt screen id */
#include "mtpnp_ad_resdef.h"
#if defined(__MMI_POWER_ON_OFF_DISPLAY__)
#include "poweronoffdisplaydefs.h"
#endif
#ifdef MMI_ON_HARDWARE_P
#include "gpioinc.h"
#endif

#include "wgui_status_icons.h"
#include "wgui_categories_defs.h"
#include "wgui_categories_sublcd.h"
#include "wgui_categories_multimedia.h"
#include "sublcdhistorygprot.h" 				/* for forcesublcdscreen() */
#include "profilesgexdcl.h"					/* for profile global var */
#include "statusiconres.h"
#include "scrmemmgrgprot.h"

#ifdef __MMI_TOUCH_SCREEN__
#include "kal_non_specific_general_types.h"
#include "touchscreengprot.h"
#endif /* __MMI_TOUCH_SCREEN__ */

/*modified by wuzc at 20061219 for fs*/
#include "fat_fs.h"   // tangjie del 20060522
/*modified by wuzc at 20061219 for fs*/
#undef __NEWSIMULATOR

#include "mmi_trace.h"
#include "filemanagergprot.h"

/***************************************************************************** 
* Define
*****************************************************************************/


/***************************************************************************** 
* Typedef 
*****************************************************************************/


/*****************************************************************************
* Global Variabl                                                          
*****************************************************************************/
#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
PUBLIC BOOL g_DirectMode = FALSE;
#endif


BOOL	g_pwron_is_terminate_diplay		= FALSE;
BOOL	g_pwron_is_image				= FALSE;
BOOL	g_pwron_is_successful_play		= FALSE;
BOOL	g_pwron_is_entered 				= FALSE;


/*****************************************************************************
* Global Function                                                           
*****************************************************************************/
extern void ShowLowPowerShutdownScr(void);
extern void EntryFMMenu(void);
extern void ReqReadDailNum(void);
extern void wgui_set_animation(S32 x,S32 y,UI_image_ID_type img);

extern void InitAllApplications(void);

extern void CallBackPowerOnAnimationComplete(GDI_RESULT ret);
extern void RegisterCategory166AnimationCompleteCallback(void (*f)(S32 result));
extern void PowerOnBatteryIndicationComplete(void);

extern BOOL IsClamClose(void);
extern void ShowCategory310Screen(UI_string_type string);
extern U8 GetHandsetInPhone();
/***************************************************************************** 
* Local Variable
*****************************************************************************/
#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
static BOOL firstKeyReceived = FALSE;
#endif


/*****************************************************************************
* Local Function 
*****************************************************************************/
void mmi_pwron_exit_animation_screen(void);
void exit_custom_startup_screen(void);
extern pBOOL isInCall(void);

/*****************************************************************************
*
*	Power On
*
*****************************************************************************/

/*****************************************************************************
* FUNCTION
*	 goto_opening_screen
* DESCRIPTION
*   go to opening screen 
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void goto_opening_screen(void)
{
// TODO: 为何要关闭下面的代码?
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if(GetAbnormalReset() == 1)	/* abnormal reset */
	{
//		TurnOnBacklight(0);
//		ResetAbnormalReset();

		/*  to initialize SMS/CPHS after abnormal reset */
//		CSPModifyHiliteHandlers();
		ClearAllKeyHandler();
		ClearKeyHandler(KEY_END,KEY_LONG_PRESS);
		InitAllApplications();		
		BeforeEntryIdleScr();
	}
	else 
	{	
		mmi_trace(g_sw_MAINENTRY ,"MMI_IDLE: Func: %s isInCall()=%d", __FUNCTION__, isInCall());

	    if (isInCall())
	    {
	        /* is in call, after call end, call management will lead to idle screen */
			return;
	        
	    }

		// TODO: 为何要关闭下面的代码?		
		 
		ClearAllKeyHandler();
		ClearKeyHandler(KEY_END,KEY_LONG_PRESS);
		InitAllApplications();


//		StartTimer(POWER_ON_ANIMATION_DEDLAY, 1500, exit_custom_startup_screen);
		exit_custom_startup_screen();
	}
}



/*****************************************************************************
* FUNCTION
*	 exit_custom_startup_screen
* DESCRIPTION
*   exit power on animation screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void exit_custom_startup_screen(void)
{	
	mmi_pwron_stop_animation(); //add by panxu
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	mmi_trace(g_sw_MAINENTRY,"MMI_IDLE: Func: %s GetAbnormalReset()=%d", __FUNCTION__, GetAbnormalReset());
	if (GetAbnormalReset() == 0)	 //dyj del 20060514
	{
		coolsand_UI_hide_animations();
#if 0 //dyj del 20060514
		CSPModifyHiliteHandlers(); /* CSP Functionality needs to be here after all
											applications have initialised */
#endif	
		ShowWelcomeScr();  //dyj del 20060514	
		// TODO: 为何要关闭下面的代码?
		//ShowIdleAppSubLcd();
	}
}


#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
/*****************************************************************************
* FUNCTION
*	 CallBackPowerOnAnimationCompleteWrapper
* DESCRIPTION
*   wrapper function to enumate poweron animation play finish
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void CallBackPowerOnAnimationCompleteWrapper(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   // TODO: 为何要关闭下面的代码?
   // CallBackPowerOnAnimationComplete(0);  //dyj del 20060514
}



/*****************************************************************************
* FUNCTION
*	 DirectModeToFM
* DESCRIPTION
*   Direct entry factory mode
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void DirectModeToFM(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    g_DirectMode = TRUE;

    /* Low battery shutdown */
    if( ((g_charbat_context.batteryStatus == PMIC_VBAT_STATUS || 		/* Battery Status */
	 		 g_charbat_context.batteryStatus == PMIC_CHARGER_OUT ) && 
	 		 g_charbat_context.isChargerConnected == 0) &&
			 (g_charbat_context.chargerLevel == BATTERY_LOW_POWEROFF) ) /* Battery Level */
    {
	   ShowLowPowerShutdownScr();
	   return;
    }

    coolsand_UI_hide_animations();
    EntryFMMenu();
}



/*****************************************************************************
* FUNCTION
*	 KeyPatternIdentifier
* DESCRIPTION
*   Key Pattern Identifier
* PARAMETERS
*	 nil
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void KeyPatternIdentifier(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 Keycode, Keytype;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	GetkeyInfo(&Keycode,&Keytype);

	if (Keycode == KEY_POUND && Keytype == KEY_EVENT_DOWN)
	{
		firstKeyReceived = TRUE;
	}
	else if (Keycode == KEY_STAR && Keytype == KEY_EVENT_DOWN && firstKeyReceived == TRUE)
	{
		firstKeyReceived = FALSE;
		ClearKeyHandler(KEY_POUND, KEY_EVENT_DOWN);
		ClearKeyHandler(KEY_STAR, KEY_EVENT_DOWN);
		DirectModeToFM();
	}
	else
	{
		firstKeyReceived = FALSE;
	}
}

#endif /* __DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__ */



/*****************************************************************************
* FUNCTION
*	 mmi_pwron_play_video_callback
* DESCRIPTION
*   play power on video result callback function
* PARAMETERS
*	 result		IN 	play video's reult
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_pwron_play_video_callback(MDI_RESULT result)
{
// TODO: 为何要关闭下面的代码?	
#if 1
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_ANIMATION_FINISH));

	if(g_pwron_is_terminate_diplay)
	{
		/* means already terminated by time out, shall skip this callback */
		return;
	}

	/* treat as gif animation */
	/* change to GIF's return */
	if(result < 0)
	{
		/* return false, will force to enter the next procedure of power on */
		CallBackPowerOnAnimationComplete(-1);
	}
	else
	{
		/* play finish successfully - reset flag to PWRON_DISPLAY_SUCCESS */
		g_pwron_is_successful_play = TRUE;
		
		/* call before access NVRAM */
		CallBackPowerOnAnimationComplete(GDI_GIF_LAST_FRAME);
	}
#endif
}



/*****************************************************************************
* FUNCTION
*	 mmi_pwron_show_image_callback
* DESCRIPTION
*   show power on image result callback function
* PARAMETERS
*	 result		IN 	play video's reult
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_pwron_show_image_callback(GDI_RESULT result)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_IMAGE_FINISH));

	if(g_pwron_is_terminate_diplay)
	{
		/* means already terminated by time out, shall skip this callback */
		return;
	}
	
	if(result < 0 && result != GDI_IMAGE_IS_STILL_IMAGE)
	{
		/* return false, will force to enter the next procedure of power on */
		CallBackPowerOnAnimationComplete(result);
	}
	 
	else if (result == GDI_IMAGE_IS_STILL_IMAGE)
	{
		g_pwron_is_successful_play = TRUE;
		CallBackPowerOnAnimationComplete(result);
	}
	 
	else
	{
		/* play finish successfully */
		g_pwron_is_successful_play = TRUE;

		/* call before access NVRAM */
		CallBackPowerOnAnimationComplete(GDI_SUCCEED);
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_pwron_froce_terminate_display
* DESCRIPTION
*   force terminate power on display
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
static void mmi_pwron_froce_terminate_display(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_ANIMATION_TIMEOUT));
	mmi_trace(1," mmi_pwron_froce_terminate_display");
	/* if never successfully play once, force stop */
	if(!g_pwron_is_successful_play)
	{
		g_pwron_is_terminate_diplay 	= TRUE;
		g_pwron_is_successful_play 	= TRUE;

		/* mimic gif last frame callback */
		CallBackPowerOnAnimationComplete(GDI_GIF_LAST_FRAME);

		/* start another timer to try */
		coolsand_UI_start_timer(200, mmi_pwron_froce_terminate_display);
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_pwron_stop_animation
* DESCRIPTION
*   this function is used to stop power on animation screen, called when ready 
*   to enter idle screen, it is used to solve NVRAM & timer interlace problem
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
void mmi_pwron_stop_animation(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	pwron_flag;
 	
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if(GetExitScrnID() != SCR_OPEN_SCREEN_ID)
		return;
   
	/* stop force teminate timer */
	coolsand_UI_cancel_timer(mmi_pwron_froce_terminate_display);

	if(g_pwron_is_image)
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
	if(!g_pwron_is_successful_play)
	{
		#if defined(__MMI_POWER_ON_OFF_DISPLAY__)
			/* restore default */
			// TODO: 为何要关闭下面的代码?
			//PhnsetRstPwrOnDisplayDefault();  //dyj del 20060514
		#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */
	}
	
	pwron_flag = PWRON_DISPLAY_SUCCESS;
	// TODO: 为何要关闭下面的代码?
	 
}


/*****************************************************************************
* FUNCTION
*	 mmi_pwron_draw_sublcd_dummy_screen
* DESCRIPTION
*	 dummy screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if defined(__MMI_VIDEO__)
static void mmi_pwron_draw_sublcd_dummy_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   /* draw nothing, just for category224's sublcd screen have a screen to goback. */
}
#endif /* __MMI_VIDEO__ */



/*****************************************************************************
* FUNCTION
*	 mmi_pwron_entry_animation_screen
* DESCRIPTION
*   start play poweron animation
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
/*JINZH Add Start For 6249 Ver: TBM780  on 2007-8-17 15:53 */
extern U8 prevMode;
extern U8 gactivatedprofile;
/*JINZH Add End  For 6249 Ver: TBM780  on 2007-8-17 15:53 */
void mmi_pwron_setaudiomode()
{
	/*JINZH Add Start For 6249 Ver: TBM780  on 2007-8-17 16:50 */
	if( 1 == GetHandsetInPhone())  
	{
		SendAudioModeRequest(AUD_MODE_HEADSET); 
        ActivateHeadset(); 
	} 
	else if(POWERON_SILENT != gcurrentprofile.toneSetup.powerOnTone) /*if(gactivatedprofile==4)   deleted by jinzh:20070818*///chenhe add ,070824.
	{
		if(g_pwr_context[0].PowerOnMode == POWER_ON_KEYPAD)//chenhe add for exception power on without any sound
		{
			SendAudioModeRequest(AUD_MODE_LOUDSPK); 
		}
	}
	/*JINZH Add End  For 6249 Ver: TBM780  on 2007-8-17 16:50 */
}
#if defined(__MMI_NOKIA_STYLE_N800__)

U8 g_pwr_nokia_setted=0;
extern void DisplayPopup(U8 * string, U16 imageId, U8 imageOnBottom, U32 popupDuration, U8 toneId);

void SSCHandleSwitchPwrNokia(void)
{

    U8 tmp = 0;
    S16  error =0;   
    //read setting
        
    	ReadValue(NVRAM_PWR_NOKIA_SETTED, &tmp, DS_BYTE, &error);

        if(0xff ==tmp)
        {
            g_pwr_nokia_setted = 1;
        }
        else
        {
            if(0 == tmp) tmp = 1;
            else tmp = 0;

            g_pwr_nokia_setted = tmp;
        }
    //write to nvram
        WriteValue(NVRAM_PWR_NOKIA_SETTED, &g_pwr_nokia_setted, DS_BYTE, &error);
    //display done
        if(g_pwr_nokia_setted)
   DisplayPopup((U8*)GetString(STR_GLOBAL_DONE),
   IMG_GLOBAL_ACTIVATED,0,1500,(U8)WARNING_TONE);
        else
   DisplayPopup((U8*)GetString(STR_GLOBAL_CANCEL),
   IMG_GLOBAL_ERASED,0,1500,(U8)WARNING_TONE);

}
#endif

void mmi_pwron_entry_animation_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16			disp_id;
	PS8			filename_ptr;

	S16 		error;
	PS8          error1;

	U16			pwron_flag;
	U16			force_stop_time;
		
	#if defined(__MMI_VIDEO__)
	SubLCDHistoryNode sub_history;
	BOOL			  play_audio;
	#endif /* __MMI_VIDEO__ */

	TRACE_SYS_FUNCTION();
mmi_trace(g_sw_WpSsPwon, "gdm: in mmi_pwron_entry_animation_screen");

	#if  ( defined(__MMI_POWER_ON_OFF_DISPLAY__) && !defined (MMI_ON_WIN32) )
		FS_HANDLE	fs_handle;
		U16			res_type;	
	#endif 
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/


	/* 

		Power On Procedure
	
			Power On Display		 NAND			SIM  		  Net Search    Idle Scr
		#----------------------#----------#----------#-------------#-----------# 
										(option)	 	(option)		(option)


	*/


	/* if is entered before, means back from interrupt while playing power on display */
	if(g_pwron_is_entered)
	{
		MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_BACK2_ANIMATION));
		ClearAllKeyHandler();  
		/* MT/SMS/USSD or other inetrrupt happened during poweron display, some initialzation need to be done afterward */
		// TODO: 为何要关闭下面的代码?
		PowerOnBatteryIndicationComplete(); 
		return;
	}
#if defined(__MMI_NOKIA_STYLE_N800__)

    TurnOnBacklight(1);
#endif    
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_START_ANIMATION));

	/* init */
	filename_ptr = NULL;
	disp_id		 = 0;

	g_pwron_is_entered			= TRUE;
	g_pwron_is_image 			= TRUE;
	g_pwron_is_successful_play	= FALSE;
	

	// TODO: 为何要关闭下面的代码?
	 

	/*******************************************/
	/* Valid power on display checking 			 */
	/*******************************************/

	/* Valid power on display restoration checking */
	ReadValue(NVRAM_POWER_ON_DISPLAY_SUCCESS, &pwron_flag, DS_SHORT, &error);   

	/* 
		0xff is the default value, each time power on display play successfully, 
		it will set to 0xff as well.
	*/

	#if defined(__MMI_POWER_ON_OFF_DISPLAY__)	
		if(pwron_flag == PWRON_DISPLAY_FAILED)
		{
			/* if flag is PWRON_DISPLAY_FAILED, means something wrong happened during playing power on display */
			/* we will restore power on display to default settings */
			PhnsetRstPwrOnDisplayDefault(); //dyj del 20060515
		}
	#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */


	/* set the flag to 0x01 , if play successfully to end, will reset back to 0xff */
	pwron_flag = PWRON_DISPLAY_PLAYING;
	 


#if  ( defined(__MMI_POWER_ON_OFF_DISPLAY__) && !defined (MMI_ON_WIN32) )
	/*******************************************/
	/* Get animation resource from phone setup */
	/*******************************************/
 mmi_trace(g_sw_WpSsPwon, "gdm:mmi_pwron_entry_animation_screen, g_phnset_cntx_p->curtPwrOnID=%d", g_phnset_cntx_p->curtPwrOnID);
	PhnsetGetPwrOnDisp(&disp_id);
 mmi_trace(g_sw_WpSsPwon, "gdm:mmi_pwron_entry_animation_screen, disp_id=%d", disp_id);
	
	if(disp_id == 0xff)	/* invalid format */
	{

		/* reset as default */
		PhnsetRstPwrOnDisplayDefault();

		/* this id must be a valid id */
		PhnsetGetPwrOnDisp(&disp_id);
	}
	else if(disp_id == 0x00 		/* source from file */
			|| disp_id == 0x01)		/* short file name*/
	{
		disp_id = 0x00;

 mmi_trace(g_sw_WpSsPwon, "gdm:mmi_pwron_entry_animation_screen, here++++++++++++++");
		/* filename buffer is allocated in phnset */
		filename_ptr 	= PhnsetGetPwrOnFileName();
mmi_trace(g_sw_WpSsPwon, "gdm:mmi_pwron_entry_animation_screen,filename_ptr=");
		fs_handle = MMI_FS_Open(filename_ptr, FS_READ_ONLY); //modified by zhoumn@2007/07/25
	
		if(fs_handle < FS_NO_ERROR)
		{
			/* open file failed */
			/* reset as default */
			PhnsetRstPwrOnDisplayDefault();

			/* this id must be a valid id */
			PhnsetGetPwrOnDisp(&disp_id);
			filename_ptr = NULL;
		}
		else
		{
			/* open file successful */
			MMI_FS_Close(fs_handle); //modified by zhoumn@2007/07/25
		}

	    if(!mmi_fmgr_util_file_limit_check(FMGR_LIMIT_POWER_ON_OFF_DSPL, filename_ptr, &error1))
		{
			/* limit check failed */
			/* reset to default */
			PhnsetRstPwrOnDisplayDefault();

			/* this id must be a valid id */
			PhnsetGetPwrOnDisp(&disp_id);
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
		res_type = mmi_fmgr_get_file_group(filename_ptr); //opened by zhoumn@2007/07/25

		if(res_type == FMGR_GROUP_IMAGE)
		{
			g_pwron_is_image = TRUE;
		}
		else if(res_type == FMGR_GROUP_VIDEO)
		{
			g_pwron_is_image = FALSE;
		}
		else
		{
			/* unsupported file format ext */
			PhnsetRstPwrOnDisplayDefault();

			/* this id must be a valid id */
			PhnsetGetPwrOnDisp(&disp_id);
			filename_ptr = NULL;
		}
	}				

	
	/* check internal resource type */
	if(filename_ptr == NULL)
	{
		 res_type = PhnsetGetDisplayType(disp_id);

		if(res_type == PHNSET_TYPE_DISP_IMAGE)
		{
			g_pwron_is_image = TRUE;
		}
		else if(res_type == PHNSET_TYPE_DISP_VIDEO)
		{
			g_pwron_is_image = FALSE;
		}
		else
		{
			MMI_ASSERT(0);
		}
	}

#else

	/* play from internal build in resource */
	g_pwron_is_image 	= TRUE;
	disp_id 			= IMG_ID_PHNSET_ON_ANIMATION_DEFAULT1;//__CUSTPACK_MULTIBIN__
	filename_ptr		= NULL;
	
#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */

#if defined(__MMI_NOKIA_STYLE_N800__)

       ReadValue(NVRAM_PWR_NOKIA_SETTED, &g_pwr_nokia_setted, DS_BYTE, &error);

        switch(g_pwr_nokia_setted)
        {
            case 1:
                disp_id = IMG_ID_NOKIA_PWR_LOGO;
                g_pwron_is_image 	= TRUE;
                filename_ptr		= NULL;
                break;

            default:
                break;
        }
#endif

	/*******************************************/
	/* Start to play									 */
	/*******************************************/
	if(g_pwron_is_image)
	{
		/* Play GIF animation or still image */
		if ( ExitCategoryFunction != NULL )
			ExitCategoryFunction();
		
mmi_trace(g_sw_WpSsPwon, "gdm: in mmi_pwron_entry_animation_screen() g_pwron_is_image=%d",g_pwron_is_image);
ShowCategory166Screen(disp_id, filename_ptr, mmi_pwron_show_image_callback);	

		#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
			if(!IsClamClose())
				ShowCategory310Screen(get_string(STR_GLOBAL_LOGO));
			else
		#endif
				ShowCategory313Screen(IMG_SUBLCD_POWER_ON_SCR,NULL);

		if(g_pwr_context[0].PowerOnMode == POWER_ON_KEYPAD) //chenhe add for exception power on without any sound
		{
			/* play power on tone */
			playRequestedTone(POWER_ON_TONE);   //dyj del 20060515  //add by panxu 20061228
		}
	}
	else
	{
		/* Play video clip */
		#if defined(__MMI_VIDEO__)

			/* check silent mode */
			play_audio = (gcurrentprofile.toneSetup.powerOnTone != POWERON_SILENT)?TRUE:FALSE;

			/* set volume as ring tone volume*/
			mdi_audio_set_volume(MDI_VOLUME_MEDIA, GetRingVolumeLevel());

			/* add sublcd screen in history */
			/* 
				this is when Categry224 play finish, it's sublcd will go back history,
				If we go back to root sublcd screen, sometimes it might draw an empty 
				screen due to sim or network not ready.
				So we have to let it back to a dummy screen, whihc will draw nothing .
			*/
			
			sub_history.entryFuncPtr = mmi_pwron_draw_sublcd_dummy_screen;
			AddSubLCDHistory(&sub_history);

			/*Guojian Add Start For 10957 Ver: GreenStone  on 2008-12-22 15:34 */
			gdi_layer_clear_background(GDI_COLOR_BLACK);
			/*Guojian Add End  For 10957 Ver: GreenStone  on 2008-12-22 15:34 */
			/* entry video play screen */
			ShowCategory224Screen(disp_id, 								/* video id */
						filename_ptr,					/* filename */
						0, 								/* repeat_count */ /* 0 means infinite */
						TRUE,							/* is_visaul_update */
						play_audio, 						/* is_play_audio */
						FALSE,							/* is_lcd_sleep */	/* already in non-sleep mode */
						GDI_COLOR_BLACK,				/* bg_color */
						mmi_pwron_play_video_callback,	/* (*play_finish_callback)(S16) */
						NULL);							/* gui_buffer */
		#else
			MMI_ASSERT(0);
		#endif /* __MMI_VIDEO__ */
	
	}

	
#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
	SetKeyHandler(KeyPatternIdentifier, KEY_POUND, KEY_EVENT_DOWN);
	SetKeyHandler(KeyPatternIdentifier, KEY_STAR, KEY_EVENT_DOWN);
#endif	

	StartLEDPatternPowerOn(); 
	ReplaceNewScreenHandler(SCR_OPEN_SCREEN_ID, mmi_pwron_exit_animation_screen, mmi_pwron_entry_animation_screen);

	/* this timer is used to avoid gif/video play too long, will force to enter idle */
	ReadValue(NVRAM_POWER_ON_FORCE_STOP_TIME, &force_stop_time, DS_SHORT, &error);

	/* not set in NVRAM, use defulat 20 sec limit */
	if(force_stop_time == 0xffff)
	{
		force_stop_time = 20000;
	}

	/* POWER ON time have NVRAM-timer interlace problem, so put time at last to eliminate this problem */	
	coolsand_UI_start_timer(force_stop_time, mmi_pwron_froce_terminate_display);
	g_pwron_is_terminate_diplay = FALSE;
}



/*****************************************************************************
* FUNCTION
*	 mmi_pwron_exception_check_display
* DESCRIPTION
*   this function is for exception power on to check wether need to restore 
*	 power on diplay to default or not.
*	 we cant 100% guarantee decoder will work for all image/video from file, we 
*   use this method to restore back if decoder has assert or fatal error.
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_pwron_exception_check_display(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 		error;
	U16			pwron_flag;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	/* Valid power on display restoration checking */
	ReadValue(NVRAM_POWER_ON_DISPLAY_SUCCESS, &pwron_flag, DS_SHORT, &error);

	/* this means exception happened during previous decoding process */
	if(pwron_flag == PWRON_DISPLAY_PLAYING)
	{
		#if defined(__MMI_POWER_ON_OFF_DISPLAY__)
			PhnsetRstPwrOnDisplayDefault(); //dyj del 20060515
		#endif /* __MMI_POWER_ON_OFF_DISPLAY__ */
	}
}



/*****************************************************************************
* FUNCTION
*	 mmi_pwron_exit_animation_screen
* DESCRIPTION
*   exit power on animation screen
* PARAMETERS
*	 void
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
void mmi_pwron_exit_animation_screen(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16	pwron_flag;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	MMI_TRACE((MMI_TRACE_G7_MISC, MMI_POWER_ON_LEAVE_ANIMATION));

	 
	
	ExitSubLCDIdleScr();  

	/* if is force stopped by MT call, treat this recource as play successfully */
	if(GetActiveScreenId() == ITEMSCR_INCOMING_CALL)
	{
		pwron_flag = PWRON_DISPLAY_SUCCESS;
		// TODO: 为何要关闭下面的代码?
		 
	}
   	   
	coolsand_UI_cancel_timer(mmi_pwron_froce_terminate_display);
}

/***************************************************************************
*
*	Analog Clock
*
***************************************************************************/
/*	Analog clock implementation: Must be moved to a different file later	*/

#define ANALOG_CLOCK_DRAW_POLYGON_HANDS		1
#define ANALOG_CLOCK_SMOOTHEN_LINES			1
 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
 
	#if defined(__MMI_UI_STYLE_3__)
 
		#define ANALOG_CLOCK_HOURS_HAND_LENGTH			12
		#define ANALOG_CLOCK_MINUTES_HAND_LENGTH		16
 
	#elif defined(__MMI_UI_STYLE_4__)
		#define ANALOG_CLOCK_HOURS_HAND_LENGTH			15
		#define ANALOG_CLOCK_MINUTES_HAND_LENGTH		20
	#endif
 
#else
#define ANALOG_CLOCK_HOURS_HAND_LENGTH			(MAIN_LCD_DEVICE_HEIGHT/6)			/*20*/
#define ANALOG_CLOCK_MINUTES_HAND_LENGTH		(ANALOG_CLOCK_HOURS_HAND_LENGTH+8)	/*28*/
#endif
 
#define ANALOG_CLOCK_SECONDS_HAND_LENGTH		(ANALOG_CLOCK_HOURS_HAND_LENGTH+12)	/*32*/

 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
	#define ANALOG_CLOCK_HOURS_HAND_COLOR	{	255,255,255,100		}
	#define ANALOG_CLOCK_MINUTES_HAND_COLOR	{	255,255,255,100		}
	#define ANALOG_CLOCK_SECONDS_HAND_COLOR	{	255,255,255,100		}
#else
#if defined(__MMI_SUBLCD_COLOR__) 
	#define ANALOG_CLOCK_HOURS_HAND_COLOR	{	0,0,93,100		}
	#define ANALOG_CLOCK_MINUTES_HAND_COLOR	{	0,0,93,100		}
	#define ANALOG_CLOCK_SECONDS_HAND_COLOR	{	255,0,0,100		}
#else
	#define ANALOG_CLOCK_HOURS_HAND_COLOR	{	0,0,0,100		}
	#define ANALOG_CLOCK_MINUTES_HAND_COLOR	{	0,0,0,100		}
	#define ANALOG_CLOCK_SECONDS_HAND_COLOR	{	0,0,0,100		}
#endif
#endif
 
/* Moved to MiscFunctions.h
#define ANALOG_CLOCK_DIAL_WIDTH				77
#define ANALOG_CLOCK_DIAL_HEIGHT				77
*/

S32 ANALOG_CLOCK_DIAL_X=26;
S32 ANALOG_CLOCK_DIAL_Y=26;
S32 ANALOG_CLOCK_CENTER_X=64;
S32 ANALOG_CLOCK_CENTER_Y=64;

#define ANALOG_CLOCK_DRAW_POLYGON_HANDS			1
#define ANALOG_CLOCK_SMOOTHEN_LINES				1
#define SUB_ANALOG_CLOCK_HOURS_HAND_LENGTH		(SUB_LCD_DEVICE_WIDTH/4)
#define SUB_ANALOG_CLOCK_MINUTES_HAND_LENGTH	(SUB_ANALOG_CLOCK_HOURS_HAND_LENGTH+4)
#define SUB_ANALOG_CLOCK_SECONDS_HAND_LENGTH	(SUB_ANALOG_CLOCK_HOURS_HAND_LENGTH+6)
#define SUB_ANALOG_CLOCK_DIAL_WIDTH				SUB_LCD_DEVICE_WIDTH
#define SUB_ANALOG_CLOCK_DIAL_HEIGHT			SUB_LCD_DEVICE_HEIGHT

#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
S32 SUB_ANALOG_CLOCK_DIAL_X=2;
S32 SUB_ANALOG_CLOCK_DIAL_Y=2;
S32 SUB_ANALOG_CLOCK_CENTER_X=SUB_LCD_DEVICE_WIDTH>>1;
S32 SUB_ANALOG_CLOCK_CENTER_Y=SUB_LCD_DEVICE_HEIGHT>>1;
#endif

UI_image_type analog_clock_background_image=NULL;
/*
const float acm_sine_table[]=
{ 
	(float)-0.99999820,	(float)-0.99431727,	(float)-0.97773360,	(float)-0.95042917,	(float)-0.91270313,	(float)-0.86496924,	(float)-0.80775119,	(float)-0.74167587,
	(float)-0.66746803,	(float)-0.58594175,	(float)-0.49799022,	(float)-0.40457821,	(float)-0.30673042,	(float)-0.20551889,	(float)-0.10205382,	(float)0.00000000,
	(float)0.10457040,	(float)0.20799418,	(float)0.30913729,	(float)0.40689072,	(float)0.50018258,	(float)0.58798990,	(float)0.66934994,	(float)0.74337050,
	(float)0.80923998,	(float)0.86623616,	(float)0.91373403,	(float)0.95121274,	(float)0.97826142,	(float)0.99458343,	(float)0.99999980,	(float)0.99445115,
	(float)0.97799831,	(float)0.95082172,	(float)0.91321931,	(float)0.86560342,	(float)0.80849624,	(float)0.74252372,	(float)0.66840956,	(float)0.58696629,
	(float)0.49908672,	(float)0.40573486,	(float)0.30793410,	(float)0.20675662,	(float)0.10331227,	(float)-0.00126490,	(float)-0.10582843,	(float)-0.20923132,
	(float)-0.31033998,	(float)-0.40804598,	(float)-0.50127753,	(float)-0.58901256,	(float)-0.67028925,	(float)-0.74421601,	(float)-0.80998244,	(float)-0.86686752,
	(float)-0.91424734,	(float)-0.95160225,	(float)-0.97852297,	(float)-0.99471414,
};

const float acm_cosine_table[]=
{
	(float)0.00189735,	(float)0.10645731,	(float)0.20984996,	(float)0.31094114,	(float)0.40862330,	(float)0.50182489,	(float)0.58952354,	(float)0.67075845,
	(float)0.74463846,	(float)0.81035318,	(float)0.86718264,	(float)0.91450340,	(float)0.95179643,	(float)0.97865315,	(float)0.99477888,	(float)1.00000000,
	(float)0.99451749,	(float)0.97813006,	(float)0.95101742,	(float)0.91347684,	(float)0.86591997,	(float)0.80886827,	(float)0.74294728,	(float)0.66887989,
	(float)0.58747821,	(float)0.49963478,	(float)0.40631283,	(float)0.30853576,	(float)0.20737548,	(float)0.10394131,	(float)-0.00063245,	(float)-0.10519940,
	(float)-0.20861283,	(float)-0.30973870,	(float)-0.40746839,	(float)-0.50073018,	(float)-0.58850135,	(float)-0.66981977,	(float)-0.74379342,	(float)-0.80961137,
	(float)-0.86655204,	(float)-0.91399082,	(float)-0.95140769,	(float)-0.97839241,	(float)-0.99464897,	(float)-0.99999920,	(float)-0.99438440,	(float)-0.97786617,
	(float)-0.95062563,	(float)-0.91296138,	(float)-0.86528656,	(float)-0.80812388,	(float)-0.74209994,	(float)-0.66793902,	(float)-0.58645414,	(float)-0.49853857,
	(float)-0.40515651,	(float)-0.30733233,	(float)-0.20613779,	(float)-0.10268295,
};
*/
#if(ANALOG_CLOCK_DRAW_POLYGON_HANDS)

const U8 analog_clock_center_dot_image[]=
{
	 0x04, 0x00, 0x8D, 0x00, 0x00, 0x00,
	 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0C, 0x10, 0x00, 0x00, 0x00,
	 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12,
	 0x34, 0x12, 0x00, 0x00, 0x04, 0x30, 0x05, 0xF7, 0x05, 0xF7, 0x04, 0x30, 0x00, 0x00, 0x34, 0x12,
	 0x00, 0x00, 0x04, 0x30, 0xFF, 0xFF, 0x05, 0xF7, 0x05, 0xF7, 0x05, 0xF7, 0x04, 0x30, 0x00, 0x00,
	 0x00, 0x00, 0x04, 0x30, 0xFF, 0xFF, 0x05, 0xF7, 0x05, 0xF7, 0x05, 0xF7, 0x04, 0x30, 0x00, 0x00,
	 0x00, 0x00, 0x04, 0x30, 0x05, 0xF7, 0x05, 0xF7, 0x05, 0xF7, 0x05, 0xF7, 0x04, 0x30, 0x00, 0x00,
	 0x00, 0x00, 0x04, 0x30, 0x04, 0x30, 0x05, 0xF7, 0x05, 0xF7, 0x04, 0x30, 0x04, 0x30, 0x00, 0x00,
	 0x34, 0x12, 0x00, 0x00, 0x04, 0x30, 0x04, 0x30, 0x04, 0x30, 0x04, 0x30, 0x00, 0x00, 0x34, 0x12,
	 0x34, 0x12, 0x34, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 0x12, 0x34, 0x12,
};

extern void flat_triangle_fill(polygon_vertex vertices[],color_t c);
void polygon_draw(polygon_vertex vertices[],S32 n_vertices,color_t c);
#elif(ANALOG_CLOCK_SMOOTHEN_LINES)
extern void UI_antialiased_line(S32 x1,S32 y1,S32 x2,S32 y2,color_t c);
#endif

 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
 
#if defined(__MMI_UI_STYLE_3__)
 
/*	offset to (0,0)	*/
polygon_vertex	analog_clock_hour_hand_vertices[]=
{	{-0,0},
	{+0,0},
	{0,-ANALOG_CLOCK_HOURS_HAND_LENGTH},
};

/*	offset to (0,0)	*/
polygon_vertex	analog_clock_minute_hand_vertices[]=
{	{-0,0},
	{+0,0},
	{0,-ANALOG_CLOCK_MINUTES_HAND_LENGTH},
};
 
#elif defined(__MMI_UI_STYLE_4__)
/*	offset to (0,0)	*/
polygon_vertex	analog_clock_hour_hand_vertices[]=
{	{-2,0},
	{+2,0},
	{0,-ANALOG_CLOCK_HOURS_HAND_LENGTH},
};

/*	offset to (0,0)	*/
polygon_vertex	analog_clock_minute_hand_vertices[]=
{	{-1,0},
	{+1,0},
	{0,-ANALOG_CLOCK_MINUTES_HAND_LENGTH},
};
#endif
 
#else
 
/*	offset to (0,0)	*/
polygon_vertex	analog_clock_hour_hand_vertices[]=
{	
	{-3,0},
	{+3,0},
	{0,-ANALOG_CLOCK_HOURS_HAND_LENGTH},
};

/*	offset to (0,0)	*/
polygon_vertex	analog_clock_minute_hand_vertices[]=
{	
	{-2,0},
	{+2,0},
	{0,-ANALOG_CLOCK_MINUTES_HAND_LENGTH},
};
 
#endif
 

#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
/*	offset to (0,0)	*/
polygon_vertex	sub_analog_clock_hour_hand_vertices[]=
{	
	{-2,+1},
	{+2,+1},
	{0,-SUB_ANALOG_CLOCK_HOURS_HAND_LENGTH},
};

/*	offset to (0,0)	*/
polygon_vertex	sub_analog_clock_minute_hand_vertices[]=
{	
	{-2,0},
	{+2,0},
	{0,-SUB_ANALOG_CLOCK_MINUTES_HAND_LENGTH},
};
#endif

/*	Rotate about (0,0)	*/
void rotate_analog_clock_hand(polygon_vertex vertices[],S32 minute)
{	
// TODO: 为何要关闭下面的代码?	
#if 0     
	float cosa,sina,x,y;
	
	minute+=15;
	if(minute>=60) minute=minute-60;
	cosa=(float)acm_cosine_table[minute];
    sina=(float)acm_sine_table[minute];
	x=(float)vertices[0].x;
	y=(float)vertices[0].y;
	vertices[0].x=(S32)(x*cosa-y*sina);
	vertices[0].y=(S32)(x*sina+y*cosa);
	x=(float)vertices[1].x;
	y=(float)vertices[1].y;
	vertices[1].x=(S32)(x*cosa-y*sina);
	vertices[1].y=(S32)(x*sina+y*cosa);
	x=(float)vertices[2].x;
	y=(float)vertices[2].y;
	vertices[2].x=(S32)(x*cosa-y*sina);
	vertices[2].y=(S32)(x*sina+y*cosa);
#endif
}

void analog_clock_draw_hand(S32 type,S32 minute)
{	
// TODO: 为何要关闭下面的代码?	
#if 0   
	S32 radius;
	S32 x1=ANALOG_CLOCK_CENTER_X;
	S32 y1=ANALOG_CLOCK_CENTER_Y;
	S32 x2,y2;
#if(ANALOG_CLOCK_DRAW_POLYGON_HANDS)
	polygon_vertex vertices[3];
#endif
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
	if ( GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE )
	{
		x1 = SUB_ANALOG_CLOCK_CENTER_X;
		y1 = SUB_ANALOG_CLOCK_CENTER_Y;
		radius=SUB_ANALOG_CLOCK_HOURS_HAND_LENGTH;
	}
	else
#endif
	{
		x1 = ANALOG_CLOCK_CENTER_X;
		y1 = ANALOG_CLOCK_CENTER_Y;
			radius=ANALOG_CLOCK_HOURS_HAND_LENGTH;
	}

	switch (type)
	{

	case 0:	{
		color_t c = ANALOG_CLOCK_HOURS_HAND_COLOR;
#if(ANALOG_CLOCK_DRAW_POLYGON_HANDS)
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
					if ( GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE )
					{
						vertices[0]=sub_analog_clock_hour_hand_vertices[0];
						vertices[1]=sub_analog_clock_hour_hand_vertices[1];
						vertices[2]=sub_analog_clock_hour_hand_vertices[2];
					}
					else
#endif
					{
						vertices[0]=analog_clock_hour_hand_vertices[0];
						vertices[1]=analog_clock_hour_hand_vertices[1];
						vertices[2]=analog_clock_hour_hand_vertices[2];
					}
					rotate_analog_clock_hand(vertices,minute);
					vertices[0].x+=x1;
					vertices[0].y+=y1;
					vertices[1].x+=x1;
					vertices[1].y+=y1;
					vertices[2].x+=x1;
					vertices[2].y+=y1;
					flat_triangle_fill(vertices,c);
					polygon_draw(vertices,3,c);
#elif(ANALOG_CLOCK_SMOOTHEN_LINES)
					x2=x1+(S32)((float)radius*acm_cosine_table[minute]);
					y2=y1+(S32)((float)radius*acm_sine_table[minute]);
					UI_antialiased_line(x1,y1,x2,y2,c);
					UI_antialiased_line(x1+1,y1+1,x2+1,y2+1,c);
#else
					x2=x1+(S32)((float)radius*acm_cosine_table[minute]);
					y2=y1+(S32)((float)radius*acm_sine_table[minute]);
					coolsand_UI_wline(x1,y1,x2,y2,c,3);
#endif
				}
				break;
		case 1:	{
			color_t c=ANALOG_CLOCK_MINUTES_HAND_COLOR;
#if(ANALOG_CLOCK_DRAW_POLYGON_HANDS)
#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
					if ( GDI_LCD->act_handle == GDI_LCD_SUB_LCD_HANDLE )
					{
						vertices[0]=sub_analog_clock_minute_hand_vertices[0];
						vertices[1]=sub_analog_clock_minute_hand_vertices[1];
						vertices[2]=sub_analog_clock_minute_hand_vertices[2];
					}
					else
#endif
					{
						vertices[0]=analog_clock_minute_hand_vertices[0];
						vertices[1]=analog_clock_minute_hand_vertices[1];
						vertices[2]=analog_clock_minute_hand_vertices[2];
					}
					rotate_analog_clock_hand(vertices,minute);
					vertices[0].x+=x1;
					vertices[0].y+=y1;
					vertices[1].x+=x1;
					vertices[1].y+=y1;
					vertices[2].x+=x1;
					vertices[2].y+=y1;
					flat_triangle_fill(vertices,c);
					polygon_draw(vertices,3,c);
#elif(ANALOG_CLOCK_SMOOTHEN_LINES)
					x2=x1+(S32)((float)radius*acm_cosine_table[minute]);
					y2=y1+(S32)((float)radius*acm_sine_table[minute]);
					UI_antialiased_line(x1,y1,x2,y2,c);
#else
					x2=x1+(S32)((float)radius*acm_cosine_table[minute]);
					y2=y1+(S32)((float)radius*acm_sine_table[minute]);
					coolsand_UI_wline(x1,y1,x2,y2,c,1);
#endif
	}

	break;

	case 2:	{
		color_t c = ANALOG_CLOCK_SECONDS_HAND_COLOR;
#if(ANALOG_CLOCK_DRAW_POLYGON_HANDS)
					x2=x1+(S32)((float)radius*acm_cosine_table[minute]);
					y2=y1+(S32)((float)radius*acm_sine_table[minute]);
					coolsand_UI_line(x1,y1,x2,y2,c);
#elif(ANALOG_CLOCK_SMOOTHEN_LINES)
					x2=x1+(S32)((float)radius*acm_cosine_table[minute]);
					y2=y1+(S32)((float)radius*acm_sine_table[minute]);
					UI_antialiased_line(x1,y1,x2,y2,c);
#else
					x2=x1+(S32)((float)radius*acm_cosine_table[minute]);
					y2=y1+(S32)((float)radius*acm_sine_table[minute]);
					coolsand_UI_line(x1,y1,x2,y2,c);
#endif
				}
				break;
	}
#endif
}

S32 analog_clock_hours_hand_counter=0;
S32 analog_clock_minutes_hand_counter=0;

void show_analog_clock_dial(void)
{	/* For removing warning
	S32	x1=ANALOG_CLOCK_DIAL_X;
	S32 y1=ANALOG_CLOCK_DIAL_Y;
	S32	x2=ANALOG_CLOCK_DIAL_X+ANALOG_CLOCK_DIAL_WIDTH-1;
	S32 y2=ANALOG_CLOCK_DIAL_Y+ANALOG_CLOCK_DIAL_HEIGHT-1;*/
 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
	coolsand_UI_push_clip();
	coolsand_UI_set_clip(ANALOG_CLOCK_DIAL_X, ANALOG_CLOCK_DIAL_Y,
			ANALOG_CLOCK_DIAL_X + ANALOG_BACKGROUND_WIDTH - 1,
			ANALOG_CLOCK_DIAL_Y + MMI_IDLESCREEN_BAR_HEIGHT - 1);
	coolsand_UI_show_image((ANALOG_CLOCK_DIAL_X), (ANALOG_CLOCK_DIAL_Y),
			get_image(IMG_TECHNO_ANALOG_CLOCK));
#else
	coolsand_UI_reset_clip();
#endif
 
#if(ANALOG_CLOCK_DRAW_POLYGON_HANDS)
	if ( GDI_LCD->act_handle == GDI_LCD_MAIN_LCD_HANDLE )
 
#ifndef __MMI_UI_TECHNO_IDLESCREEN_BAR__
		coolsand_UI_show_transparent_image(ANALOG_CLOCK_CENTER_X-4,ANALOG_CLOCK_CENTER_Y-4,(U8*)analog_clock_center_dot_image,0);
#endif
 
	#ifdef __MMI_SUBLCD_SHOW_ANALOG_CLOCK__
	else
		#if defined(__MMI_SUBLCD_COLOR__) 
		coolsand_UI_show_transparent_image(SUB_ANALOG_CLOCK_CENTER_X-4,SUB_ANALOG_CLOCK_CENTER_Y-4,(U8*)analog_clock_center_dot_image,0);
		#else
			gdi_draw_round_rect(SUB_ANALOG_CLOCK_CENTER_X-3,SUB_ANALOG_CLOCK_CENTER_Y-3,SUB_ANALOG_CLOCK_CENTER_X+3,SUB_ANALOG_CLOCK_CENTER_Y+3,GDI_COLOR_BLACK,GDI_COLOR_BLACK,2);
		#endif
	#endif
#endif
	analog_clock_draw_hand(0,analog_clock_hours_hand_counter);
	analog_clock_draw_hand(1,analog_clock_minutes_hand_counter);
 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
	coolsand_UI_pop_clip();
#endif
 
#if 0 
/* under construction !*/
#endif
}

void show_analog_clock(void)
{	
	show_analog_clock_dial();
}

void analog_clock_initialize(void)
{
	MYTIME	t;
	S32		h, m;

 
#ifdef __MMI_UI_TECHNO_IDLESCREEN_BAR__
	ANALOG_CLOCK_CENTER_X=(MMI_SIGNAL_WIDTH + (ANALOG_BACKGROUND_WIDTH>>1));
	ANALOG_CLOCK_DIAL_X=(MMI_SIGNAL_WIDTH);
	ANALOG_CLOCK_CENTER_Y=(MMI_status_bar_height + (MMI_IDLESCREEN_BAR_HEIGHT>>1));
	ANALOG_CLOCK_DIAL_Y=(MMI_status_bar_height);
#else
	ANALOG_CLOCK_DIAL_X=(UI_device_width>>1)-(ANALOG_CLOCK_DIAL_WIDTH>>1);
	ANALOG_CLOCK_DIAL_Y=(UI_device_height>>1)-(ANALOG_CLOCK_DIAL_HEIGHT>>1);
	ANALOG_CLOCK_CENTER_X=(UI_device_width>>1);
	ANALOG_CLOCK_CENTER_Y=(UI_device_height>>1);
#endif	/* __MMI_UI_TECHNO_IDLESCREEN_BAR__ */
 

	GetDateTime(&t);
	m=t.nMin;
	analog_clock_minutes_hand_counter=m;
	h=t.nHour;
	h++;
	if(h>12) h-=12;
	analog_clock_hours_hand_counter=(h-1)*5;
	analog_clock_hours_hand_counter+=m/12;
}

void analog_clock_minute_tick(void)
{
	MYTIME	t;
	S32		h, m;
	GetDateTime(&t);
	m=t.nMin;
	analog_clock_minutes_hand_counter=m;
	h=t.nHour;
	h++;
	if(h>12) h-=12;
	analog_clock_hours_hand_counter=(h-1)*5;
	analog_clock_hours_hand_counter+=m/12;	
	if(analog_clock_minutes_hand_counter>=60) analog_clock_minutes_hand_counter=0;
	if((analog_clock_minutes_hand_counter%12)==0) 
	{//	analog_clock_hours_hand_counter++;
		if(analog_clock_hours_hand_counter>=60) analog_clock_hours_hand_counter=0;
	}
}

void analog_clock_demo_timer(void)
{
	color_t c = coolsand_UI_color(255, 255, 255);
	analog_clock_minutes_hand_counter++;

	if (analog_clock_minutes_hand_counter >= 60) analog_clock_minutes_hand_counter = 0;

	if ((analog_clock_minutes_hand_counter % 12) == 0)
	{
		analog_clock_hours_hand_counter++;

		if (analog_clock_hours_hand_counter >= 60) analog_clock_hours_hand_counter = 0;
	}
	coolsand_UI_reset_clip();
	coolsand_UI_fill_rectangle(0,0,UI_device_width-1,UI_device_height-1,c);
	show_analog_clock_dial();
#if(UI_DOUBLE_BUFFER_SUPPORT)
	coolsand_UI_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
#endif
	coolsand_UI_start_timer(200,analog_clock_demo_timer);
}

extern void PB_test_demo(void);

void initialize_UI_demo(void)
{
	// TODO: 为何要关闭下面的代码?
	 
#ifdef __MMI_DRAW_MANAGER__
	dm_init();
#endif
	/*wuzc Modify Start For GDI/GUI Portting Ver:    on 2008-3-12 10:29 */
	//initialize_gui();
	gdi_init();
	wgui_init();
	/*wuzc Modify End  For GDI/GUI Portting Ver:    on 2008-3-12 10:29 */
	initialize_category_screens();
#ifdef __MMI_TOUCH_SCREEN__
	/* We do not enable touch screen in certain cases like USB poweron 
	 * because some related tasks are not running.
	 */
	if (g_pwr_context[0].PowerOnMode == POWER_ON_KEYPAD || 
		g_pwr_context[0].PowerOnMode == POWER_ON_EXCEPTION ||
		g_pwr_context[0].PowerOnMode == POWER_ON_ALARM )
	{
		mmi_pen_init();
	}
#endif /* __MMI_TOUCH_SCREEN__ */ 

	/*	Temporary: Must be moved to boot up	application	*/

	#ifdef __MMI_MULTI_SIM__
	{
		U8 i;
#ifdef __MMI_DUAL_SIM_SINGLE_OPENED_
	for(i = 0; i < 1; i++)
#else
	for(i = 0; i < MMI_SIM_NUMBER; i++)
#endif	
      {
			ShowStatusIcon(STATUS_ICON_SIGNAL_STRENGTH + i);
			ChangeStatusIconImage(STATUS_ICON_GPRS_SERVICE,IMG_SI_GPRS_SERVICE_SIM1+ i);
			//ChangeStatusIconImage(STATUS_ICON_GPRS_ATT_NO_PDP_INDICATOR,IMG_SI_GPRS_ATT_NO_PDP_INDICATOR_SIM1+ i);
		}
	}
	#else
	ShowStatusIcon(STATUS_ICON_SIGNAL_STRENGTH);
	#endif

	ShowStatusIcon(STATUS_ICON_BATTERY_STRENGTH);
	ShowStatusIcon(STATUS_ICON_SUBLCD_SIGNAL_STRENGTH);
	ShowStatusIcon(STATUS_ICON_SUBLCD_BATTERY_STRENGTH);
#ifdef __MMI_TOUCH_SCREEN__
	ChangeStatusIconImage(STATUS_ICON_KEYPAD_LOCK, IMG_SI_KEYPAD_UNLOCK);
	UpdateStatusIcons();  
	ShowStatusIcon(STATUS_ICON_KEYPAD_LOCK);
#endif
#ifdef DOUBLE_SIM_CARD_SUPPORT   //added by renyh for greenstone @ 2008-08-15
	//modify by niej  for CS200 the SIM  icon need not dislayed, because only support one SIM 2007/08/14 begin
	{	
	/*Wangzl Add Start For  Ver: TBM_780  on 2007-8-8 10:8 */
	U8 sim_slot;
	CFW_CfgGetSimSwitch(&sim_slot);
	ChangeStatusIconImage(STATUS_ICON_SIM_SWITCH,IMG_SI_SIM_SWITCH+ sim_slot);
	ShowStatusIcon(STATUS_ICON_SIM_SWITCH);
	}
	//modify  by niej  for CS200 the SIM  icon need not dislayed, because only support one SIM 2007/08/14 End
	/*Wangzl Add End  For  Ver: TBM_780  on 2007-8-8 10:8 */
	UpdateStatusIcons();
#endif  
}

void redraw_custom_startup_screen(void)
{
	color_t c;
	S32	iwidth, iheight;

	coolsand_UI_lock_double_buffer();
	c.r = 255;
	c.b = 255;
	c.g = 255;
	c.alpha = 100;
	coolsand_UI_set_clip(0, 0, UI_device_width - 1, UI_device_height - 1);
	coolsand_UI_fill_rectangle(0, 0, UI_device_width - 1, UI_device_height - 1, c);
	coolsand_UI_measure_image(get_image(CUSTOM_LOGO_IMAGE_ID), &iwidth, &iheight);
	coolsand_UI_show_image((UI_device_width >> 1) - (iwidth >> 1), (UI_device_height >> 1) - (iheight >> 1), get_image(CUSTOM_LOGO_IMAGE_ID));
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}

void goto_custom_startup_screen(void)
{	
	redraw_custom_startup_screen();
	coolsand_UI_start_timer(DEMO_OPENING_CUSTOM_LOGO_TIMEOUT,exit_custom_startup_screen);
}

void UI_update(void)
{
#if(!BUILD_TYPE_X86WIN32_DISABLE_REDRAW)
	if(RedrawCategoryFunction!=NULL) RedrawCategoryFunction();
	if(SUBLCD_RedrawCategoryFunction!=NULL) SUBLCD_RedrawCategoryFunction();
#endif
}

void terminate_UI_demo(void)
{
	if (ExitCategoryFunction != NULL) ExitCategoryFunction();

	if (SUBLCD_ExitCategoryFunction != NULL) SUBLCD_ExitCategoryFunction();

	coolsand_UI_hide_animations();
}

byte temp_sublcd_display_flag=0;
void toggle_temp_sublcd_display(void)
{	
	if(temp_sublcd_display_flag)
	{
		temp_sublcd_display_flag=0;
		ShowCategory302Screen(NULL);
	}
	else
	{
		temp_sublcd_display_flag=1;
		ShowCategory301Screen((U8*)GetString(TEMP_SUBLCD_ANIMATION_STR_ID),TEMP_SUBLCD_ANIMATION_IMAGE_ID,NULL);
	}
}


