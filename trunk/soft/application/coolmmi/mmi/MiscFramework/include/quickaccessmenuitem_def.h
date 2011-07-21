/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2002
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * QuickAccessMenuItem_def.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for struct, defines & globle variables
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

	FILENAME	: QuickAccessMenuItem_def.h

  	PURPOSE		: struct, defines & globle variables

 

 

	DATE		: Feb 28, 2003

**************************************************************/
#ifndef _QUICK_ACCESS_DEF_H
#define _QUICK_ACCESS_DEF_H

#define KEYPADUNLOCK_TIMEOUT				2000
#define KEYPAD_UNLOCK_SCR_TIMEOUT			1000

/*Timeout value for emergency call transiant screen*/
#define EMERGENCY_NO_DIALED_TRANSIANT_SCR_TIMEOUT  5000

/* */
#define EMERGENCY_NUMBER_LENGTH							41   

enum STR_QUICK_ACCESS_LIST
{
	KPAD_SCR_UNLOCKED_MESSAGE_RUNNING_STR	=		IDLE_APP_BASE+350	,				
		
	KPAD_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR,
	KPAD_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR2, 

	KPAD_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR,
	KPAD_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR2

#ifdef __MMI_TOUCH_SCREEN__
	,TOUCH_SCR_UNLOCKED_FIRST_HELP_MESSAGE_RUNNING_STR
	,TOUCH_SCR_UNLOCKED_HELP_MESSAGE_RUNNING_STR
#endif
};

enum IMG_QUICK_ACCESS_LIST
{
	IMG_KPAD_SCR_UNLOCKED_HELP_MESSAGE	=		IDLE_APP_BASE+350	
#ifdef __MMI_TOUCH_SCREEN__
	,IMG_TOUCH_SCR_PEN_CLICK_AREA
#endif
};

enum SCR_QUICK_ACCESS_LIST
{
	KPAD_SCR_UNLOCK_MESSAGE		=		IDLE_APP_BASE+350,
	KPAD_SCR_UNLOCK_HELP_MESSAGE,					

	KPAD_SCR_UNLOCK_FIRST_HELP_MESSAGE,
	EMERENCY_CALL_SCR_DISPLAY_ID					
};




#endif

