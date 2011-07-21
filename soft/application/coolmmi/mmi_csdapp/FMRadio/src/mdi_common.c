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

/*******************************************************************************
 * Filename:
 * ---------
 *	 mdi_common.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *	 MDI (Media Device Interface) common functions
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
 #include "mmi_features.h"
 #include "mmi_trace.h"

/***************************************************************************** 
* Include
*****************************************************************************/
#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"

#include "mmi_data_types.h"	/* mmi data type */
#include "gdi_include.h"		/* gdi */
#include "mdi_datatype.h"		/* mdi datatype */
#include "mdi_include.h"
#include "mdi_audio.h"			/* audio */
#if defined(__MMI_VIDEO_PLAYER__) || defined(__MMI_VIDEO_RECORDER__)
#include "mdi_video.h"			/* video */
#endif
#ifdef __MMI_CAMERA__
#include "mdi_camera.h"			/* camera */
#endif

#include "debuginitdef.h"		/* debug */

//#ifdef __MMI_TARGET__
#include "med_api.h"				/* media task */
#include "med_main.h"			/* media task */	
#include "med_struct.h"			/* media task */
//#endif /* __MMI_TARGET__ */




/***************************************************************************** 
* Define
*****************************************************************************/


/***************************************************************************** 
* Typedef 
*****************************************************************************/
typedef struct
{
	module_type 	mod_id;
	// TODO: shall split into audio_owner, video_owner, camera_owner in the future 
} mdi_cnxt_struct;

/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/

/***************************************************************************** 
* Local Function
*****************************************************************************/
mdi_cnxt_struct g_mdi_cntx =
{
	MOD_MMI	/* mod_id */
};


/*****************************************************************************
* FUNCTION
*	 mdi_switch_device_ownership()
* DESCRIPTION
*  This function is used for multiple tasks to get owner shipof multimedia device,
*	such as audio, camera and video, etc.
*	The previous action on this device will be forced to terminate.
* PARAMETERS
*	 mod_id		[IN]		owner, one of MOD_MMI, MOD_J2ME, etc
*	 device		[IN]		device
* RETURNS
*   void
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
#if 0
void mdi_switch_device_ownership(module_type mod_id, MDI_DEVICE_TYPE device)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	MMI_ASSERT(device != 0);

	// TODO: shall split and check each device in the future
	if(device)
	{
		/* call this function will terminate ALL current using device and restore it to idle state */
//		mdi_audio_block(mod_id);
	#ifdef MMI_ON_HARDWARE_P
		media_aud_block(MOD_MMI, (kal_uint16)mod_id, (kal_uint8)AUD_BLOCK_ALL);
	#endif
		/* call each device for state recovery since MEDIA/Driver is terminated */
		mdi_audio_blocked_recovery();
		
	#if defined(__MMI_VIDEO_PLAYER__) || defined(__MMI_VIDEO_RECORDER__)
		mdi_video_blocked_recovery();
	#endif

	#if defined(__MMI_CAMERA__)
		mdi_camera_blocked_recovery();
	#endif
	}

	g_mdi_cntx.mod_id = mod_id;	
}
#endif



/*****************************************************************************
* FUNCTION
*	 mdi_get_device_ownership()
* DESCRIPTION
*   get current device ownership
* PARAMETERS
*	 device		[IN]		device
* RETURNS
*   module_type
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
module_type mdi_get_device_ownership(MDI_DEVICE_TYPE device)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	return g_mdi_cntx.mod_id;
}


/*****************************************************************************
* FUNCTION
*	 mdi_check_device_ownership()
* DESCRIPTION
*   check current device ownership
* PARAMETERS
*	 void
* RETURNS
*   BOOL
* GLOBALS AFFECTED
*   nil
*****************************************************************************/
BOOL mdi_check_device_ownership()
{
	FMR_TRACE();
#ifdef MMI_ON_HARDWARE_P
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	module_type module_id = stack_get_active_module_id();

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (g_mdi_cntx.mod_id==module_id)
	{
		return TRUE;
	}
	else 
	{
		if (MOD_WAP==module_id)
		{
			PRINT_INFORMATION(("mdi_check_device_ownership, WAP access MDI\n"));
			return TRUE;
		}
		else 
		{
			return FALSE;
		}
	}
#else
	return TRUE;
#endif
}

