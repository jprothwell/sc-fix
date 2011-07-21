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
 * HardwareHandler.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for Handles the Events Sent from Hardware queue
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

/**************************************************************

	FILENAME	: HardwareHandler.c

  	PURPOSE		: Handles the Events Sent from Hardware queue.

 

	AUTHOR		: coolsand Engineers

	DATE		: .


**************************************************************/

#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
//#include "customcfg.h"

//#ifdef FIRST_PASS
//#include "buildcfg.h"
//#endif

//#include "custdatares.h"
#include "protocolevents.h"
#include "hardwareeventhandler.h"
#include "hardwareevents.h"
/* Applications should be independant. This is required for customizing 
packaging of applications.
#include "phonebook.h"*/
#include "eventsgprot.h"
#include "queuegprot.h"
#include "gpioinc.h"
#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
#endif

#ifdef __MMI_IRDA_SUPPORT__
#include "irdammigprots.h"
#endif

#ifdef MMI_ON_HARDWARE_P
#include "l4dr.h"
#endif
#include "keybrd.h"

#undef __NEWSIMULATOR 

extern void DateTimerIndication(void* p) ;
extern void GpioDetectInd(void * info);
#if defined(MOTION_SENSOR_SUPPORT)
extern void GsensorRotateHdlr(void * info);
#endif
extern void VolumeChangeIndication(void * info);


/**********************************************************

	FUNCTION NAME:			InitHardwareEvents

	PURPOSE:				Initialize the ProtocolEvents 
	
	INPUT PARAMETERS:		nil

	OUTPUT PARAMETERS:		nil

	RETURNS:				void

 

**********************************************************/
#if defined(MOTION_SENSOR_SUPPORT)
extern void EntryIdleShakeChangeWallpaper(void);
#endif


void InitHardwareEvents(void)
{

#ifdef MMI_ON_HARDWARE_P
	 
	SetProtocolEventHandler(L4SetClockRSP,MSG_ID_MMI_EQ_SET_RTC_TIME_RSP);
	SetProtocolEventHandler(L4GetClockTimeRSP,MSG_ID_MMI_EQ_GET_RTC_TIME_RSP);//tangjie add 20060708
	SetProtocolEventHandler(DateTimerIndication,MSG_ID_MMI_EQ_CLOCK_TICK_IND);
	SetProtocolEventHandler(GpioDetectInd, MSG_ID_MMI_EQ_GPIO_DETECT_IND);
	SetProtocolEventHandler(GpioCtrlReqHdlr,MSG_ID_MMIAPI_DEVICE_GET_CTRL_REQ);
	SetProtocolEventHandler(GpioPlayReqHdlr, MSG_ID_MMIAPI_DEVICE_PLAY_REQ);
	
	#ifdef __MMI_USB_SUPPORT__
	SetProtocolEventHandler(UsbDetectIndHdlr,MSG_ID_MMI_EQ_USBDETECT_IND );
    #endif
#endif
#if defined(MMI_2D_SENSOR_SUPPORT) 
	SetProtocolEventHandler((PsFuncPtr)GpioDetectInd,MSG_ID_2D_SENSOR_IND);
#endif
#if defined(MOTION_SENSOR_SUPPORT)
	SetProtocolEventHandler((PsFuncPtr)EntryIdleShakeChangeWallpaper, MSG_ID_MMI_EQ_WALLPAPER_CHANGE_IND );
	SetProtocolEventHandler((PsFuncPtr)GsensorRotateHdlr, MSG_ID_MMI_EQ_GSENSOR_ROTATE_IND );
    //SetProtocolEventHandler((PsFuncPtr)ImageViewerChangeHdlr, MSG_ID_MMI_EQ_IMAGEVIEWER_CHANGE_IND );
#endif
	/* GPIO device Detect indication event handler */
	SetProtocolEventHandler( GpioDetectInd, PRT_EQ_GPIO_DETECT_IND);
	#ifdef __MMI_USB_SUPPORT__
	SetProtocolEventHandler(UsbDetectIndHdlr, PRT_EQ_USB_DETECT_IND );
	#endif
	 
	//SetProtocolEventHandler( VolumeChangeIndication, MSG_ID_MMI_EQ_VOLUME_CHANGE_IND);

   #ifdef __MMI_IRDA_SUPPORT__
   mmi_irda_init_protocol_event();  
   #endif
}


// CSD end


