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
 *	 MMI_Data_Types.h
 *
 * Project:
 * --------
 *
 * Description:
 * ------------
 *  MMI data type define
 *
 * Author:
 * -------
 *                      
 *============================================================================
 *             HISTORY
 *------------------------------------------------------------------------------
 * 
 *
 *------------------------------------------------------------------------------
 *============================================================================
 ****************************************************************************/
#ifndef _PIXCOM_DATA_TYPES_H
#define _PIXCOM_DATA_TYPES_H

#ifdef __ASCII
#define ENCODING_LENGTH			1
#endif

#ifdef __UCS2_ENCODING
#define ENCODING_LENGTH			2
#endif
 
 #ifdef MMI_ON_HARDWARE_P
#define CONST_HARDWARE const
#else
#define CONST_HARDWARE
#endif


/****************************************************************************
*
* Data Type for Win32 Platform (PC_Simulator)
*
*****************************************************************************/
#ifdef MMI_ON_WIN32
//#include "stdafx.h"

	#define FALSE    0
	#define TRUE     1
	#define ST_SUCCESS			1
	#define	ST_FAILURE			0 	

	typedef double 			DOUBLE;
	typedef float			FLOAT;
/*
	typedef char			S8;			
	typedef char			*PS8;		
	typedef unsigned char   U8;			
	typedef unsigned char	*PU8;		

	typedef unsigned short 	pBOOL; 		

	typedef short			S16; 		
	typedef short			*PS16; 		
	typedef unsigned short  U16; 		
	typedef unsigned short  *PU16;		
					
	typedef long			S32;		
	typedef long		   *PS32;		
	typedef unsigned long	U32;		
	typedef unsigned long	*PU32;		
*/
	typedef char					S8;			
	typedef char					*PS8;		
	typedef unsigned char  		U8;			
	typedef unsigned char		*PU8;		

	typedef unsigned short 		pBOOL; 		

	typedef short int	S16; 		
	typedef short int	*PS16; 		
	typedef unsigned short int	U16; 		
	typedef unsigned short int	*PU16;		
					
	typedef int			S32;		
	typedef int			*PS32;		
	typedef unsigned int		U32;		
	typedef unsigned int		*PU32;		

	typedef float			float32;

   typedef unsigned __int64	U64;
   typedef __int64            S64;

	typedef unsigned int	UINT;		

	typedef unsigned long			STRING_ID;
	typedef unsigned long			IMAGE_ID;
	typedef unsigned long			MENU_ID;
	typedef unsigned long			screen_ID;

	#ifndef MMI_ON_WIN32
	typedef unsigned char	BOOL;
	#define CONST const;
	#endif

	typedef void (* oslTimerFuncPtr)(void *);

	typedef struct _win32_msgqid {
		S8	                q_name[20];		/* Queue name             */
	    U32		            q_queue_size;         /* Total size of queue    */
	    U32		            q_max_messages;           /* Messages in queue      */
	    U32					q_max_message_size;       /* Size of each message   */
	    volatile U32		q_messages;          /* Available words        */
		PU32			    q_start;              /* Start of queue area    */
		PU32				q_message_size;		/*	Start queue	size area	*/
	    volatile U32        q_read;               /* Read pointer           */
	    volatile U32        q_write;              /* Write pointer          */
	    volatile U32        q_tasks_waiting_write; /* Number of waiting tasks*/
		volatile U32        q_tasks_waiting_read;  /* Number of waiting tasks*/
		HANDLE				q_semaphore_read;
		HANDLE				q_semaphore_write;
		CRITICAL_SECTION	q_cs;
	} win32_msgqid;

	typedef	win32_msgqid *	oslMsgqid;

	typedef struct _win32_timerid {
		S8 tmr_name[20];
		U32 tmr_handler;
		U32	tmr_timeout;
		oslTimerFuncPtr tmr_callback;
		void * tmr_callback_arg;
	} win32_timerid;

	typedef	win32_timerid *	oslTimerid;

	typedef HANDLE oslTaskid;
	typedef void (* oslTaskFuncPtr)(void *);

	typedef enum {
	   OSL_NO_WAIT,
	   OSL_INFINITE_WAIT
	} OSLWAITMODE;

	typedef enum {
		OSL_STOP,
		OSL_EXPIRE,
		OSL_RUN
	} OSLTIMERSTATE;

	typedef enum {
	   OSL_SUCCESS,
	   OSL_ERROR,
	   OSL_Q_FULL,
	   OSL_Q_EMPTY,
	   OSL_SEM_NOT_AVAILABLE,
	   OSL_WOULD_BLOCK,
	   OSL_MESSAGE_TOO_BIG,
	   OSL_INVALID_ID,
	   OSL_NOT_INITIALIZED,
	   OSL_INVALID_LENGHT,
	   OSL_NULL_ADDRESS,
	   OSL_NOT_RECEIVE,
	   OSL_NOT_SEND,
	   OSL_MEMORY_NOT_VALID
	} OSLSTATUS;

	typedef struct _win32_soft_timerid {
		oslTimerFuncPtr tmr_callback;
		void * tmr_callback_arg;
		U32	tmr_timeout;
		U32 tmr_count;
		OSLTIMERSTATE tmr_state;
	} win32_soft_timerid;

	typedef	win32_soft_timerid *	oslSoftTimerid;
	
	typedef struct {
	   PS8			task_name;
	   PS8			task_qname;
	   U8			task_priority;
	   U32			task_stack_size;
	   oslTaskid   task_id;
	   oslMsgqid   task_ext_qid;
	   U32			task_ext_qsize;	
	   oslTaskFuncPtr    task_entry_func;
	} win32_task_info;

	typedef win32_task_info osl_task_info;

	typedef struct {
	   PS8             task_name;
	   PS8             task_qname;
	   U8            task_priority;
	   U32            task_stack_size;
	   U32            task_ext_qsize;
	   oslTaskFuncPtr	task_entry_func;
	} win32_task_create_info;

	typedef win32_task_create_info osl_task_create_info;
	typedef void osl_entry_struct;
	
	typedef enum {
			MMI_FALSE=0,
			MMI_TRUE=1
		} MMI_BOOL;
	
#endif /* WIN32 */
	



/****************************************************************************
*
* Data Type for Hardware Platform (Target, MNT, AMNT)
*
*****************************************************************************/
#ifdef MMI_ON_HARDWARE_P

	#define FALSE    0
	#define TRUE     1
	#define ST_SUCCESS			1
	#define	ST_FAILURE			0 	
	#define MMI_BOOL kal_bool
	#define MMI_FALSE KAL_FALSE
	#define MMI_TRUE KAL_TRUE
	
	typedef double 			DOUBLE;
	typedef float			FLOAT;

/*
	typedef char			S8;			
	typedef char			*PS8;		
	typedef unsigned char   U8;			
	typedef unsigned char	*PU8;		

	typedef unsigned short 	pBOOL; 		

	typedef short			S16; 		
	typedef short			*PS16; 		
	typedef unsigned short  U16; 		
	typedef unsigned short  *PU16;		
					
	typedef long			S32;		
	typedef long		   *PS32;		
	typedef unsigned long	U32;		
	typedef unsigned long	*PU32;		
*/

	typedef char					S8;			
	typedef char					*PS8;		
	typedef unsigned char  		U8;			
	typedef unsigned char		*PU8;		

	typedef unsigned short 		pBOOL; 		

	typedef short int	S16; 		
	typedef short int	*PS16; 		
	typedef unsigned short int	U16; 		
	typedef unsigned short int	*PU16;		
					
	typedef int			S32;		
	typedef int			*PS32;		
	typedef unsigned int		U32;		
	typedef unsigned int		*PU32;		

	typedef float			float32;

#ifdef WIN32
   typedef unsigned __int64   U64;
   typedef __int64            S64;
#else
   typedef unsigned long long U64;
   typedef long long   S64;
#endif

	typedef unsigned int	UINT;		

	typedef unsigned long			STRING_ID;
	typedef unsigned long			IMAGE_ID;
	typedef unsigned long			MENU_ID;
	typedef unsigned long			screen_ID;

	#ifndef MMI_ON_WIN32
		typedef unsigned char	BOOL;
		#define CONST const;
	#endif

	typedef void (* oslTimerFuncPtr)(void *);
	typedef void (* oslTaskFuncPtr)(void *);

	typedef enum {
	   OSL_SUCCESS,
	   OSL_ERROR,
	   OSL_Q_FULL,
	   OSL_Q_EMPTY,
	   OSL_SEM_NOT_AVAILABLE,
	   OSL_WOULD_BLOCK,
	   OSL_MESSAGE_TOO_BIG,
	   OSL_INVALID_ID,
	   OSL_NOT_INITIALIZED,
	   OSL_INVALID_LENGHT,
	   OSL_NULL_ADDRESS,
	   OSL_NOT_RECEIVE,
	   OSL_NOT_SEND,
	   OSL_MEMORY_NOT_VALID
	} OSLSTATUS;

	
#endif /* MMI_ON_HARDWARE_P */




/****************************************************************************
*
* Data Type for Customization Tool
*
*****************************************************************************/
#ifdef CUSTOMIZE_TOOL

#define FALSE    0
#define TRUE     1
	#define ST_SUCCESS			1
	#define	ST_FAILURE			0 	

typedef char			S8;			
typedef char			*PS8;		
typedef unsigned char   U8;			
typedef unsigned char	*PU8;		

typedef unsigned short 	pBOOL; 		

typedef short			S16; 		
typedef short			*PS16; 		
typedef unsigned short  U16; 		
typedef unsigned short  *PU16;		
					
typedef long			S32;		
typedef long		   *PS32;		
typedef unsigned long	U32;		
typedef unsigned long	*PU32;		
typedef float			float32;

typedef unsigned int	UINT;		

	typedef unsigned long			STRING_ID;
	typedef unsigned long			IMAGE_ID;
	typedef unsigned long			MENU_ID;
	typedef unsigned long			screen_ID;

	typedef unsigned char	byte;
	typedef unsigned short	word;
	typedef unsigned long	dword;
	typedef char			sbyte;
	typedef short			sword;
	typedef long			sdword;

	typedef void (* oslTimerFuncPtr)(void *);

  typedef struct _win32_msgqid {
    S8	                q_name[20];		/* Queue name             */
    U32		            q_queue_size;         /* Total size of queue    */
    U32		            q_max_messages;           /* Messages in queue      */
    U32					q_max_message_size;       /* Size of each message   */
    volatile U32		q_messages;          /* Available words        */
    PU32			    q_start;              /* Start of queue area    */
    PU32				q_message_size;		/*	Start queue	size area	*/
    volatile U32        q_read;               /* Read pointer           */
    volatile U32        q_write;              /* Write pointer          */
    volatile U32        q_tasks_waiting_write; /* Number of waiting tasks*/
    volatile U32        q_tasks_waiting_read;  /* Number of waiting tasks*/
    HANDLE				q_semaphore_read;
    HANDLE				q_semaphore_write;
    CRITICAL_SECTION	q_cs;
  } win32_msgqid;

	typedef	win32_msgqid *	oslMsgqid;

	typedef struct _win32_timerid {
		S8 tmr_name[20];
		U32 tmr_handler;
		U32	tmr_timeout;
		oslTimerFuncPtr tmr_callback;
		void * tmr_callback_arg;
	} win32_timerid;

	typedef	win32_timerid *	oslTimerid;

	typedef HANDLE oslTaskid;
	typedef void (* oslTaskFuncPtr)(void *);


	typedef enum {
	   OSL_NO_WAIT,
	   OSL_INFINITE_WAIT
	} OSLWAITMODE;

	typedef enum {
	   OSL_SUCCESS,
	   OSL_ERROR,
	   OSL_Q_FULL,
	   OSL_Q_EMPTY,
	   OSL_SEM_NOT_AVAILABLE,
	   OSL_WOULD_BLOCK,
	   OSL_MESSAGE_TOO_BIG,
	   OSL_INVALID_ID,
	   OSL_NOT_INITIALIZED,
	   OSL_INVALID_LENGHT,
	   OSL_NULL_ADDRESS,
	   OSL_NOT_RECEIVE,
	   OSL_NOT_SEND,
	   OSL_MEMORY_NOT_VALID
	} OSLSTATUS;

	typedef struct {
	   PS8			task_name;
	   PS8			task_qname;
	   U8			task_priority;
	   U32			task_stack_size;
	   oslTaskid   task_id;
	   oslMsgqid   task_ext_qid;
	   U32			task_ext_qsize;	
	   oslTaskFuncPtr    task_entry_func;
	} win32_task_info;

	typedef win32_task_info osl_task_info;

	typedef struct {
	   PS8             task_name;
	   PS8             task_qname;
	   U8            task_priority;
	   U32            task_stack_size;
	   U32            task_ext_qsize;
	   oslTaskFuncPtr	task_entry_func;
	} win32_task_create_info;

	typedef win32_task_create_info osl_task_create_info;
	typedef void osl_entry_struct;
	
#endif /* CUSTOMIZE_TOOL */






/****************************************************************************
*
* Applcation Enum
*
*****************************************************************************/
	/* Ranges of hash defines given to various Application */
#define  APP_DEFAULT_BASE	101	  /* UNDEPLOYABLE Application base id */


/* Application under APPLICATION_DEFAULT enum can not be undeployed. */
typedef enum {
	APP_MAINMENU=APP_DEFAULT_BASE,
	APP_IDLEAPP,
	APP_BOOTUP,
	APP_SIMTOOLKIT,
	APP_EMAIL,
	APP_GLOBALDEFS,
	APP_CHARGER,
	APP_UIFRAMEWORK,
	APP_PHONEBOOK,
	APP_MESSAGES,
	APP_SETTINGS,
	APP_CALLMANAGEMENT,
	APP_CALLHISTORY,
	APP_CALLSETUP,
	APP_PROFILES,
	APP_NETWORKSETUP,
	APP_WORLDCLOCK,
	APP_DEMOAPP,
	APP_SECURITY_SETUP				
}APPLICATION_DEFAULT;

/* Application under APPLICATION_DEPLOY enum can be deployed or undeployed. */
typedef enum {
	APP_SHORTCUTS=1,
	APP_FUNANDGAMES,
	APP_SERVICES,
	APP_ORGANIZER,
	APP_CALLBARRING,		
	APP_DATAACCOUNT,
	APP_DOWNLOAD,
	APP_TODOLIST,
	APP_INPUTMETHOD,
	APP_VOICEMEMO,
	APP_ENGINEERMODE,
	APP_CALENDAR,
	APP_FACTORYMODE,
	APP_RINGTONECOMPOSER,
	APP_GAMES,
	APP_WAP,
	APP_ALARM,
	APP_CALCULATOR,
	APP_UNITCONVETER,
	APP_CURRENCYCONVERTER,
	APP_HEALTHBMI,
	APP_HEALTHCALORIE,
	APP_HEALTHMENSTRUAL,
	APP_STOPWATCH,	
	APP_PHONESETUP,
	APP_PICTURE_EDITOR,
	APP_AUDIOPLAYER,
	APP_FILEMANAGER,
	APP_SOUNDRECORDER,
	APP_CAMERA,
	APP_IMAGEVIEWER,
	APP_EXTDEVICE,
	APP_JAVA,
	APP_MULTIMEDIA,
	APP_EXTRA,
	APP_VDOPLY,
	APP_VDOREC,
	APP_CHAT,
	APP_FMRDO,
	APP_PHOEDT,
	APP_MESSAGE_CLUB
} APPLICATION_DEPLOY;
	 
/* 120304 Calvin added */
typedef enum {
	RES_IMAGE,
	RES_MEDIA,
	RES_AUDIO //101205 audio resource Calvin add
} RESOURCE_TYPE_LIST;
/* Calvin end */

/* 101205 audio resource Calvin move here */
typedef enum _image_type_enum
{
	SYSTEM_IMAGE,
	EXT_IMAGE,
	NFB_PROGRESS_IMAGE
}image_type_enum;
/* 101205 audio resource Calvin end */


/****************************************************************************
*
* Resource Base ID
*
*****************************************************************************/
/****************************************************************************
* Main Menu [0-999]
*****************************************************************************/
#define  MAIN_MENU_BASE				0

/****************************************************************************
* PhoneBook [1000-1999]
*****************************************************************************/
#define  PHONE_BOOK_BASE			1000

/****************************************************************************
* Shortcut [2000-2999]
*****************************************************************************/
#define  SHORTCUTS_BASE				2000	

/****************************************************************************
* Fun&Game [3000-3999]
*****************************************************************************/
#define  FUN_GAMES_BASE				3000

/****************************************************************************
* Service [4000-4999]
*****************************************************************************/
#define  SERVICES_BASE				4000
#define	DATA_ACCOUNT_BASE			SERVICES_BASE+500
#define	INFO_NUM_BASE				SERVICES_BASE+100

/****************************************************************************
* Organizer [5000-5999]
*****************************************************************************/
#define  ORGANIZER_BASE				5000  	  
#define  TO_DO_LIST_BASE			5100
#define  ALARM_BASE					5200
#define  CALCULATOR_BASE			5300
#define	UNIT_CONVERTER				5400
#define	CURRENCY_CONVERTER		5500
#define  WORLD_CLOCK					5600
#define	HEALTH_CALORIE				5800
#define	HEALTH_MENSTRUAL			5900
#define	HEALTH_BMI					5950


/****************************************************************************
* Message [6000-6999]
*****************************************************************************/
#define  MESSAGES_BASE				6000
#define  CHAT_BASE					MESSAGES_BASE+800

/****************************************************************************
* Connection [7000-7999]
*****************************************************************************/
#define  CONNECTS_BASE				7000

/****************************************************************************
* Call [8000-8999]
*****************************************************************************/
#define  CALLS_BASE					8000                                          

/****************************************************************************
* Setting [9000-9999]
*****************************************************************************/
#define  SETTINGS_BASE				9000	
#define  SECURITY_SETUP_BASE		SETTINGS_BASE+800	  	/* [9800-9999] */

/****************************************************************************
* Call Management [10000-10999]
*****************************************************************************/
#define  CALL_MANAGEMENT_BASE		10000

/****************************************************************************
* Idle App [11000-11999]
*****************************************************************************/
#define  IDLE_APP_BASE				11000 					/* [11000-11399] */
#define	CATEGORY_SCREENS_BASE	IDLE_APP_BASE+400		/* [11400-11999] */

/****************************************************************************
* Demo App [12000-12999]
*****************************************************************************/
#define  DEMO_APP_BASE				12000 					/* [12000-12999] */

/****************************************************************************
* Phone Setup [13000-13999]
*****************************************************************************/
#define	PHONE_SETUP_BASE			13000	
#define	SCREENSAVER_BASE			PHONE_SETUP_BASE + 200
#define  WALLPAPER_BASE				PHONE_SETUP_BASE + 300
#define  SPOF_BASE					PHONE_SETUP_BASE + 400	 
#define  NITZ_BASE					PHONE_SETUP_BASE + 500
#define  LANG_SETTINGS_BASE		PHONE_SETUP_BASE + 600	
#define	HELP_BASE					PHONE_SETUP_BASE + 700


/****************************************************************************
* Misc
*****************************************************************************/
	#define		SIM_DETECTION_BASE	15000	/*Sim Detection application*/
	#define		SIM_TOOLKIT_BASE	16000	/*Sim Detection application*/
#define 	EMAIL_BASE								18000					/* E-mail application */
	#define CALL_BASE_NEW							20000/* Used for FDL/BDL/SDL/Call fwd/call waiting*/
	#define	CALL_HISTORY							21000
#define 	PROFILES_BASE							22000					/* Profiles */
#define	CHARGER_BASE							23000					/* BootUp */
#define CALL_BARRING 25000
	#define	GLOBAL_BASE					26000
	#define		NETWORK_SETUP_BASE			27000
#define	DOWNLOAD_BASE							29000
#define	VOICE_MEMO_BASE						30000  
#define	INPUT_METHOD_BASE						31000
#define	CTK_BASE									31500
#define	ENGINEER_MODE_BASE					32000
#define	CAL_BASE									33000
#define 	FACTORY_MODE_BASE						34000
#define	COMPOSE_RING_TONE_ENGINE_BASE		35000
#define	COMPOSE_RINGTONE_BASE				COMPOSE_RING_TONE_ENGINE_BASE + 200
#define 	WAP_BASE									37000
#define 	AUDIO_PLAYER_BASE						38000
#define 	SOUND_RECORDER_BASE					38500
#define 	FILE_MANAGER_BASE						39000
#define	JAVA_BASE								40000
#define	NV_RAM_FILE_IMAGE_ID_BASE			41000
#define	EXT_DEVICE_BASE						43000					/* External Device ID [43000-43999] */
#define 	MMEDIA_BASE								44000
#define 	EXTRA_APP_BASE							45000
#define 	NV_RAM_FILE_EXT_MELODY_ID_BASE  	46000
#define	GAME_BASE								47000  				/* [47000-49999] */
#define	MMIAPI_BASE								50000
#define	VOBJECT_BASE							51000
#define	VCARD_BASE								51200
#define	VCALENDAR_BASE							51600
#define 	STOPWATCH_BASE							52000
#define 	PICTURE_EDITOR_BASE					52500
#define 	CAMERA_BASE								53000
#define 	IMAGE_VIEWER_BASE						53500
#define 	VDOPLY_BASE								54000
#define 	VDOREC_BASE								54500
#define 	FM_RADIO_BASE							55000
#define 	VRSD_BASE									56000	/*Voice Command - SD*/
#define	VRSI_BASE									57000	/*Voice Command - SI*/
#define 	PHOEDT_BASE								58000					/* [58000-58999] */
#define	MESSAGE_CLUB_BASE						59000

#endif /* _PIXCOM_DATA_TYPES_H */
