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
 *	 mmi_data_types.h
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
 * -------
 *                      
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
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
#include "stdafx.h"

	#define ST_SUCCESS		1
	#define ST_FAILURE		0	 	

//	#define FALSE   		0
//	#define TRUE     		1
	typedef double 				DOUBLE;
	typedef unsigned short      UINT16;
	typedef unsigned char       UINT8;
	typedef short      			INT16;
//typedef unsigned int UINT32;

	typedef UINT8*              PUINT8;
	typedef UINT32*             PUINT32;
	typedef INT32*              PINT32;
	typedef UINT16*             PUINT16;
	typedef INT16*              PINT16;

  typedef unsigned char     		uint8;
typedef signed char             int8;
typedef unsigned short int      uint16;
typedef signed short int        int16;
typedef unsigned int            uint32;
typedef signed int              int32;
	typedef char				S8;			
	typedef char				*PS8;		
	typedef unsigned char  		U8;			
  typedef unsigned char  		u8;
	typedef unsigned char		*PU8;		

	typedef unsigned short 		pBOOL; 		

	typedef short int			S16; 		
	typedef short int			*PS16; 		
	typedef unsigned short int	U16; 		
	typedef unsigned short int	*PU16;		
					
	typedef int					S32;		
	typedef int					*PS32;		
	typedef unsigned int		U32;		
	typedef unsigned int		u32;
	typedef unsigned int		*PU32;		
	typedef unsigned char		byte;
	typedef unsigned long		dword;

	typedef S8				s8;				/*	signed 8-bit data		*/
	typedef S16				s16;			/*	signed 16-bit data		*/
	typedef S32				s32;			/*	signed 32-bit data		*/
	typedef unsigned short		word;
	typedef unsigned long		dword;
	typedef unsigned short WCHAR;

	typedef float				float32;

   typedef unsigned __int64		U64;
   typedef __int64            	S64;

	typedef unsigned int		UINT;		


	typedef unsigned long		STRING_ID;
	typedef unsigned long		IMAGE_ID;
	typedef unsigned long		MENU_ID;
	typedef unsigned long		screen_ID;

	#ifndef MMI_ON_WIN32
		#ifndef CONST
		#define CONST const;
		#endif
	#endif

	typedef void (*oslTimerFuncPtr)(void*);

	typedef struct _win32_msgqid {
		S8	       			q_name[20];					/* Queue name             	*/
		U32					q_queue_size;         		/* Total size of queue    	*/
		U32					q_max_messages;				/* Messages in queue      	*/
		U32					q_max_message_size;			/* Size of each message   	*/
		volatile U32		q_messages;					/* Available words        	*/
		PU32			   	q_start;					/* Start of queue area    	*/
		PU32				q_message_size;				/* Start queue size area	*/
		volatile U32		q_read;						/* Read pointer          		*/
		volatile U32		q_write;					/* Write pointer          		*/
		volatile U32		q_tasks_waiting_write; 		/* Number of waiting tasks	*/
		volatile U32		q_tasks_waiting_read;  		/* Number of waiting tasks	*/
		HANDLE				q_semaphore_read;
		HANDLE				q_semaphore_write;
		CRITICAL_SECTION	q_cs;
	} win32_msgqid;

	typedef	win32_msgqid *oslMsgqid;

	typedef struct _win32_timerid {
		S8 	tmr_name[20];
		U32 	tmr_handler;
		U32	tmr_timeout;
		oslTimerFuncPtr tmr_callback;
		void	*tmr_callback_arg;
	} win32_timerid;

	typedef win32_timerid	*oslTimerid;

	typedef HANDLE oslTaskid;
	typedef void (*oslTaskFuncPtr)(void*);

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
		oslTimerFuncPtr		tmr_callback;
		void	*tmr_callback_arg;
		U32		tmr_timeout;
		U32 	tmr_count;
		OSLTIMERSTATE	tmr_state;
	} win32_soft_timerid;

	typedef	win32_soft_timerid *oslSoftTimerid;
	
	typedef struct {
	   PS8				task_name;
	   PS8				task_qname;
	   U8				task_priority;
	   U32				task_stack_size;
	   oslTaskid		task_id;
	   oslMsgqid		task_ext_qid;
	   U32				task_ext_qsize;	
	   oslTaskFuncPtr	task_entry_func;
	} win32_task_info;

	typedef win32_task_info osl_task_info;

	typedef struct {
	   PS8				task_name;
	   PS8				task_qname;
	   U8            	task_priority;
	   U32				task_stack_size;
	   U32				task_ext_qsize;
	   oslTaskFuncPtr	task_entry_func;
	} win32_task_create_info;

	typedef win32_task_create_info osl_task_create_info;
	typedef void osl_entry_struct;
	
	typedef enum {
		MMI_FALSE 	= 0,
		MMI_TRUE 	= 1
	} MMI_BOOL;
	
#endif /* WIN32 */
	



/****************************************************************************
*
* Data Type for Hardware Platform (Target, MNT, AMNT)
*
*****************************************************************************/
#ifdef MMI_ON_HARDWARE_P
#include "cswtype.h"

 


	#define ST_SUCCESS		1
	#define ST_FAILURE		0 	
	#define MMI_BOOL 		kal_bool
	#define MMI_FALSE		KAL_FALSE
	#define MMI_TRUE 		KAL_TRUE
	

/*
	typedef char					S8;			
	typedef char					*PS8;		
	typedef unsigned char  			U8;			
	typedef unsigned char				*PU8;		

	typedef unsigned short 			pBOOL; 		

	typedef short					S16; 		
	typedef short					*PS16; 		
	typedef unsigned short  			U16; 		
	typedef unsigned short				*PU16;		
					
	typedef long					S32;		
	typedef long					*PS32;		
	typedef unsigned long				U32;		
	typedef unsigned long				*PU32;		

    typedef int						INT; 
	typedef char					S8;			
	typedef char					*PS8;		
	typedef unsigned char  			U8;			
	typedef unsigned char			*PU8;		

	typedef unsigned short 			pBOOL; 		

	typedef short int				S16; 		
	typedef short int				*PS16; 		
	typedef unsigned short int		U16; 		
	typedef unsigned short int		*PU16;		
					
	typedef int						S32;		
	typedef int						*PS32;		
	typedef unsigned int			U32;		
	typedef unsigned int			*PU32;		
	
	typedef float					float32;

#ifdef WIN32
   typedef unsigned __int64  		U64;
   typedef __int64           		S64;
#else
   typedef unsigned long long		U64;
   typedef long long   S64;
#endif
*/

	typedef unsigned int			UINT;		

	typedef unsigned long			STRING_ID;
	typedef unsigned long			IMAGE_ID;
	typedef unsigned long			MENU_ID;
	typedef unsigned long			screen_ID;

	#ifndef MMI_ON_WIN32
		#ifndef CONST
		#define CONST const
		#endif
	#endif

	typedef void (*oslTimerFuncPtr)(void*);
	typedef void (*oslTaskFuncPtr)(void*);

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


//add mmi_features.h for complile resource
#include "mmi_features.h"


/****************************************************************************
*
* Data Type for Customization Tool
*
*****************************************************************************/
#ifdef CUSTOMIZE_TOOL

	#define ST_SUCCESS		1
	#define ST_FAILURE		0 	
#if 0
	typedef char				S8;			
	typedef char				*PS8;		
	typedef unsigned char   	U8;			
	typedef unsigned char		*PU8;		

	typedef unsigned short 		pBOOL; 		

	typedef short				S16; 		
	typedef short				*PS16; 		
	typedef unsigned short  	U16; 		
	typedef unsigned short		*PU16;		
					
	typedef long				S32;		
	typedef long				*PS32;		
	typedef unsigned long		U32;		
	typedef unsigned long		*PU32;		
	typedef float				float32;

	typedef unsigned int		UINT;		
#endif 
	typedef unsigned long		STRING_ID;
	typedef unsigned long		IMAGE_ID;
	typedef unsigned long		MENU_ID;
	typedef unsigned long		screen_ID;

	typedef unsigned char		byte;
	typedef unsigned short		word;
	typedef unsigned long		dword;
	typedef char				sbyte;
	typedef short				sword;
	typedef long				sdword;

	typedef void 				(*oslTimerFuncPtr)(void*);

	typedef struct _win32_msgqid {
		S8						q_name[20];					/* Queue name			*/
	   U32		         		q_queue_size;  				/* Total size of queue		*/
	   U32		         		q_max_messages;				/* Messages in queue		*/
	   U32						q_max_message_size;			/* Size of each message	*/
	   volatile U32				q_messages;          		/* Available words      		*/
		PU32			    	q_start;              		/* Start of queue area    	*/
		PU32					q_message_size;				/* Start  queue  size  area  	*/
	   volatile U32     		q_read;              		/* Read pointer           		*/
	   volatile U32      		q_write;              		/* Write pointer          		*/
	   volatile U32      		q_tasks_waiting_write; 		/* Number of waiting tasks	*/
		volatile U32      		q_tasks_waiting_read;  		/* Number of waiting tasks	*/
		HANDLE					q_semaphore_read;
		HANDLE					q_semaphore_write;
		CRITICAL_SECTION		q_cs;
	} win32_msgqid;

	typedef	win32_msgqid	*oslMsgqid;

	typedef struct _win32_timerid {
		S8 		tmr_name[20];
		U32 	tmr_handler;
		U32		tmr_timeout;
		oslTimerFuncPtr 	tmr_callback;
		void		*tmr_callback_arg;
	} win32_timerid;

	typedef	win32_timerid	*oslTimerid;

	typedef HANDLE oslTaskid;
	typedef void (*oslTaskFuncPtr)(void*);


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
	   PS8				task_name;
	   PS8				task_qname;
	   U8				task_priority;
	   U32				task_stack_size;
	   oslTaskid  	 	task_id;
	   oslMsgqid   		task_ext_qid;
	   U32				task_ext_qsize;	
	   oslTaskFuncPtr 	task_entry_func;
	} win32_task_info;

	typedef win32_task_info osl_task_info;

	typedef struct {
	   PS8				task_name;
	   PS8				task_qname;
	   U8				task_priority;
	   U32				task_stack_size;
	   U32				task_ext_qsize;
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
	APP_MAINMENU = APP_DEFAULT_BASE,
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
	APP_SECURITY_SETUP,
	APP_IMPS
}APPLICATION_DEFAULT;

/* Application under APPLICATION_DEPLOY enum can be deployed or undeployed. */
typedef enum {
	APP_SHORTCUTS = 1,
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
	APP_MESSAGE_CLUB,
	APP_ABREPEATER,
	APP_FMSCHEDULEREC,
	APP_EBOOK,
#ifdef JATAAYU_SUPPORT
	APP_JATAAYU_WAP,
	APP_JATAAYU_MMS,
#endif
#ifdef __MMI_MAINLCD_220X176__
	APP_TOOLS,
#endif
     APP_FTP,
#if defined(__MMI_ANALOGTV__)
        APP_ATV,
#endif
//add newly above!     
     APP_MAX
} APPLICATION_DEPLOY;

 
typedef enum {
	RES_IMAGE,
	RES_MEDIA,
	RES_AUDIO  
} RESOURCE_TYPE_LIST;
 


 
typedef enum _image_type_enum
{
	SYSTEM_IMAGE,
	EXT_IMAGE,
	NFB_PROGRESS_IMAGE
}image_type_enum;
 

/****************************************************************************
*
* Resource Base ID
*
* Note: We define the the starting value of resource enum ID 
*       for each application instead of using one big enum definition.
*       As a result, we can avoid re-compile of the whole plutommi 
*       when add/remove string/image enum ID.
* 
* Note: The maximum value of resource ID is 65535 due to U16 datatype.
*
*****************************************************************************/

#define RESOURCE_BASE_RANGE(name, count)			\
					RESOURCE_BASE_##name,				\
					RESOURCE_BASE_##name##_END = RESOURCE_BASE_##name + count - 1

/* 
 * Declare the range of resource ID of each application.
 *
 * For example:
 *   RESOURCE_BASE_MAIN_MENU = 14000, 
 *   RESOURCE_BASE_MAIN_MENU_END = 14599,
 *   RESOURCE_BASE_PHONE_BOOK = 14600,
 *   RESOURCE_BASE_PHONE_BOOK_END = 15199,
 */

typedef enum 
{
	/* HACK. The value of PHONE_SETUP_BASE=13000 is fixed in NVRAM definition */
	RESOURCE_BASE_FIRST_VALUE				= 13999, 
	RESOURCE_BASE_RANGE(MAIN_MENU,			600),
	RESOURCE_BASE_RANGE(PHONE_BOOK,			600),
	RESOURCE_BASE_RANGE(SHORTCUTS,			50),
	RESOURCE_BASE_RANGE(FUN_GAMES,			300),
	/* Service */
	RESOURCE_BASE_RANGE(SERVICES,			100),
	RESOURCE_BASE_RANGE(INFO_NUM,			100),
	RESOURCE_BASE_RANGE(DATA_ACCOUNT,		200),
	/* Organizer */
	RESOURCE_BASE_RANGE(ORGANIZER,			100),
	RESOURCE_BASE_RANGE(TO_DO_LIST,			100),
	RESOURCE_BASE_RANGE(ALARM,				50),
	RESOURCE_BASE_RANGE(CALCULATOR,			50),
	RESOURCE_BASE_RANGE(UNIT_CONVERTER,		50),
	RESOURCE_BASE_RANGE(CURRENCY_CONVERTER,	50),
	RESOURCE_BASE_RANGE(WORLD_CLOCK,			200),
	RESOURCE_BASE_RANGE(HEALTH_CALORIE,		100),
	RESOURCE_BASE_RANGE(HEALTH_MENSTRUAL,	50),
	RESOURCE_BASE_RANGE(HEALTH_BMI,			50),
	/* Message */
	RESOURCE_BASE_RANGE(MESSAGES,			1500),
	RESOURCE_BASE_RANGE(CHAT,				200),
	/* XXX unused */
	RESOURCE_BASE_RANGE(CONNECTS,			100),
	/* Setting */
	RESOURCE_BASE_RANGE(SETTINGS,			300),
	RESOURCE_BASE_RANGE(SECURITY_SETUP,		300),
	/* Call Management */
	RESOURCE_BASE_RANGE(CALL_MANAGEMENT,		700),
	/* Idle App */
	RESOURCE_BASE_RANGE(IDLE_APP,			500),
	RESOURCE_BASE_RANGE(CATEGORY_SCREENS,	700),//20350
	/* Demo App */
	RESOURCE_BASE_RANGE(DEMO_APP,			700),
#if 0 /* HACK. The value of PHONE_SETUP_BASE is fixed in NVRAM definition */
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif 
	/* Misc */
	RESOURCE_BASE_RANGE(SIM_DETECTION,		1500),
	RESOURCE_BASE_RANGE(SAT,					500),
	RESOURCE_BASE_RANGE(EMAIL,				300),
	RESOURCE_BASE_RANGE(IMPS,				300),
	/* Used for FDL/BDL/SDL/Call fwd/call waiting*/
	RESOURCE_BASE_RANGE(CALL_NEW,			700),
	RESOURCE_BASE_RANGE(CALL_HISTORY,		300),
	RESOURCE_BASE_RANGE(PROFILES,			600),
	RESOURCE_BASE_RANGE(CHARGER,				200),
	/* XXX unused */
	RESOURCE_BASE_RANGE(CALL_BARRING,		100),
	RESOURCE_BASE_RANGE(GLOBAL,				300),
	RESOURCE_BASE_RANGE(NETWORK_SETUP,		200),
	RESOURCE_BASE_RANGE(DOWNLOAD,			100),
	RESOURCE_BASE_RANGE(VOICE_MEMO,			200),
	RESOURCE_BASE_RANGE(INPUT_METHOD,		200),
	RESOURCE_BASE_RANGE(CTK,					200),
	RESOURCE_BASE_RANGE(ENGINEER_MODE,		1000),
	/* Calendar */
	RESOURCE_BASE_RANGE(CAL,					300),
	RESOURCE_BASE_RANGE(FACTORY_MODE,		300),
	RESOURCE_BASE_RANGE(COMPOSE_RING_TONE_ENGINE, 200),
	RESOURCE_BASE_RANGE(COMPOSE_RINGTONE,	100),
	RESOURCE_BASE_RANGE(WAP,					1500),
	RESOURCE_BASE_RANGE(AUDIO_PLAYER,		500),
	RESOURCE_BASE_RANGE(SOUND_RECORDER,		300),
	RESOURCE_BASE_RANGE(FILE_MANAGER,		600),
	RESOURCE_BASE_RANGE(JAVA,				500),
	/* Download application. XXX. To be removed */
	RESOURCE_BASE_RANGE(NV_RAM_FILE_IMAGE,	2000),
	RESOURCE_BASE_RANGE(EXT_DEVICE,			300),
	RESOURCE_BASE_RANGE(MMEDIA,				100),
	RESOURCE_BASE_RANGE(EXTRA_APP,			100),
	/* Download application. XXX. To be removed */
	RESOURCE_BASE_RANGE(NV_RAM_FILE_EXT_MELODY, 1000),
	RESOURCE_BASE_RANGE(GAME,					3000),
	RESOURCE_BASE_RANGE(MMIAPI,				100),
	RESOURCE_BASE_RANGE(VOBJECT,				50),
	RESOURCE_BASE_RANGE(VCARD,				50),
	RESOURCE_BASE_RANGE(VCALENDAR,			50),
	RESOURCE_BASE_RANGE(STOPWATCH,			100),
	RESOURCE_BASE_RANGE(PICTURE_EDITOR,		200),
	RESOURCE_BASE_RANGE(CAMERA,				600),
	RESOURCE_BASE_RANGE(IMAGE_VIEWER,		100),
	RESOURCE_BASE_RANGE(VDOPLY,				500),
	RESOURCE_BASE_RANGE(VDOREC,				500),
	RESOURCE_BASE_RANGE(FM_RADIO,			300),
	/* Voice Command - SD */
	RESOURCE_BASE_RANGE(VRSD,				100),
	/* Voice Command - SI */
	RESOURCE_BASE_RANGE(VRSI,				200),
	RESOURCE_BASE_RANGE(PHOEDT,				300),
	RESOURCE_BASE_RANGE(MESSAGE_CLUB,		1000),
	RESOURCE_BASE_RANGE(POC,					200),
	RESOURCE_BASE_RANGE(AB_REPEATER,			200),
	RESOURCE_BASE_RANGE(FM_SCHEDULE_REC,		200),
	
	/*SP*/ //add by niej 
	RESOURCE_BASE_RANGE(SP,100),
	/*USB*/ //add by chenhe 
	RESOURCE_BASE_RANGE(JASPER_USB,100),

	RESOURCE_BASE_RANGE(EBOOK,			100),  //added by guojian 20080521
	RESOURCE_BASE_RANGE(MAIN_MENU_BI_DEGREE,			100),  //added by guojian 20080521
#ifdef JATAAYU_SUPPORT
	RESOURCE_BASE_RANGE(JAT_WAP,			200), 
	RESOURCE_BASE_RANGE(JAT_MMS,			400),
	RESOURCE_BASE_RANGE(JATAAYU_PROVISIONING,			200),//fengzb
#endif
#ifdef __MMI_NOTEPAD__
	RESOURCE_BASE_RANGE(NOTEPAD,                     20),
#endif

	RESOURCE_BASE_RANGE(ZYX_IPTV,                     20),

#ifdef __MMI_MULTI_SIM__
	RESOURCE_BASE_RANGE(MERCURY, 800), //add by wangzl for double sim
#endif
#if defined(__MMI_ANALOGTV__)
	RESOURCE_BASE_RANGE(ANALOGTV, 800), //add by wangzl for double sim
#endif

#ifdef _KK_SUPPORT_
	RESOURCE_BASE_RANGE(KK,			100),      

#endif //_KK_SUPPORT_
#ifdef __ZYX_PLATFORM_SUPPORT__
	RESOURCE_BASE_RANGE(ZYX_PLATFORM, 100),
#endif
	RESOURCE_BASE_RANGE(END, 0)
} RESOURCE_BASE_ENUM;


/* 
 * Declare MACRO to cast enum names (such as RESOURCE_BASE_MAIN_MENU) 
 * into U16 datatype to be compatible with RVCT.
 *
 * For example:
 *   #define	MAIN_MENU_BASE					((U16) RESOURCE_BASE_MAIN_MENU)
 *   #define	MAIN_MENU_BASE_MAX			((U16) RESOURCE_BASE_MAIN_MENU_END)
 */


typedef struct 
{
	char		*appname;
	unsigned short min; //	value;
	unsigned short max;
} mmi_resource_base_struct;

#if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE)
/* Table of resource base ID used for reference in debugging */
#define RESOURCE_BASE_TABLE_BEGIN()													\
			const	mmi_resource_base_struct g_mmi_resource_base_table[] = {

#define RESOURCE_BASE_TABLE_ITEM(ENUM_NAME)										\
			{#ENUM_NAME, (U16) RESOURCE_BASE_##ENUM_NAME, (U16) RESOURCE_BASE_##ENUM_NAME##_END},


#define RESOURCE_BASE_TABLE_END()													\
			};

#else /* _POPULATE_RES_C */

#define RESOURCE_BASE_TABLE_BEGIN()

#define RESOURCE_BASE_TABLE_ITEM(ENUM_NAME)

#define RESOURCE_BASE_TABLE_END()

#endif /* _POPULATE_RES_C */

/****************************************************************************
* Definition of applications
*****************************************************************************/

/* Beginning of resource table */
RESOURCE_BASE_TABLE_BEGIN()

/****************************************************************************
* Main Menu
*****************************************************************************/
#define  MAIN_MENU_BASE						((U16) RESOURCE_BASE_MAIN_MENU)
#define	MAIN_MENU_BASE_MAX					((U16) RESOURCE_BASE_MAIN_MENU_END)
RESOURCE_BASE_TABLE_ITEM(MAIN_MENU)

/****************************************************************************
* PhoneBook
*****************************************************************************/
#define  PHONE_BOOK_BASE					((U16) RESOURCE_BASE_PHONE_BOOK)
#define	PHONE_BOOK_BASE_MAX					((U16) RESOURCE_BASE_PHONE_BOOK_END)
RESOURCE_BASE_TABLE_ITEM(PHONE_BOOK)

/****************************************************************************
* Shortcut
*****************************************************************************/
#define  SHORTCUTS_BASE						((U16) RESOURCE_BASE_SHORTCUTS)
#define	SHORTCUTS_BASE_MAX					((U16) RESOURCE_BASE_SHORTCUTS_END)
RESOURCE_BASE_TABLE_ITEM(SHORTCUTS)

/****************************************************************************
* Fun&Game
*****************************************************************************/
#define  FUN_GAMES_BASE						((U16) RESOURCE_BASE_FUN_GAMES)
#define	FUN_GAMES_BASE_MAX					((U16) RESOURCE_BASE_FUN_GAMES_END)
RESOURCE_BASE_TABLE_ITEM(FUN_GAMES)

/****************************************************************************
* Service
*****************************************************************************/
#define  SERVICES_BASE						((U16) RESOURCE_BASE_SERVICES)
#define	SERVICES_BASE_MAX					((U16) RESOURCE_BASE_SERVICES_END)
RESOURCE_BASE_TABLE_ITEM(SERVICES)

#define  INFO_NUM_BASE						((U16) RESOURCE_BASE_INFO_NUM)
#define	INFO_NUM_BASE_MAX					((U16) RESOURCE_BASE_INFO_NUM_END)
RESOURCE_BASE_TABLE_ITEM(INFO_NUM)

#define  DATA_ACCOUNT_BASE					((U16) RESOURCE_BASE_DATA_ACCOUNT)
#define	DATA_ACCOUNT_BASE_MAX				((U16) RESOURCE_BASE_DATA_ACCOUNT_END)
RESOURCE_BASE_TABLE_ITEM(DATA_ACCOUNT)

/****************************************************************************
* Organizer [5000-5999]
*****************************************************************************/

#define  ORGANIZER_BASE						((U16) RESOURCE_BASE_ORGANIZER)
#define	ORGANIZER_BASE_MAX					((U16) RESOURCE_BASE_ORGANIZER_END)
RESOURCE_BASE_TABLE_ITEM(ORGANIZER)

#define  TO_DO_LIST_BASE					((U16) RESOURCE_BASE_TO_DO_LIST)
#define	TO_DO_LIST_BASE_MAX					((U16) RESOURCE_BASE_TO_DO_LIST_END)
RESOURCE_BASE_TABLE_ITEM(TO_DO_LIST)

#define  ALARM_BASE							((U16) RESOURCE_BASE_ALARM)
#define	ALARM_BASE_MAX						((U16) RESOURCE_BASE_ALARM_END)
RESOURCE_BASE_TABLE_ITEM(ALARM)

#define  CALCULATOR_BASE					((U16) RESOURCE_BASE_CALCULATOR)
#define	CALCULATOR_BASE_MAX					((U16) RESOURCE_BASE_CALCULATOR_END)
RESOURCE_BASE_TABLE_ITEM(CALCULATOR)

#define  UNIT_CONVERTER_BASE				((U16) RESOURCE_BASE_UNIT_CONVERTER)
#define	UNIT_CONVERTER						UNIT_CONVERTER_BASE /* Be backward compatible */
#define	UNIT_CONVERTER_BASE_MAX				((U16) RESOURCE_BASE_UNIT_CONVERTER_END)
RESOURCE_BASE_TABLE_ITEM(UNIT_CONVERTER)

#define  CURRENCY_CONVERTER_BASE			((U16) RESOURCE_BASE_CURRENCY_CONVERTER)
#define	CURRENCY_CONVERTER					CURRENCY_CONVERTER_BASE /* Be backward compatible */
#define	CURRENCY_CONVERTER_BASE_MAX			((U16) RESOURCE_BASE_CURRENCY_CONVERTER_END)
RESOURCE_BASE_TABLE_ITEM(CURRENCY_CONVERTER)

#define  WORLD_CLOCK_BASE					((U16) RESOURCE_BASE_WORLD_CLOCK)
#define	WORLD_CLOCK							WORLD_CLOCK_BASE /* Be backward compatible */
#define	WORLD_CLOCK_BASE_MAX				((U16) RESOURCE_BASE_WORLD_CLOCK_END)
RESOURCE_BASE_TABLE_ITEM(WORLD_CLOCK)

#define  HEALTH_CALORIE_BASE				((U16) RESOURCE_BASE_HEALTH_CALORIE)
#define	HEALTH_CALORIE						HEALTH_CALORIE_BASE /* Be backward compatible */
#define	HEALTH_CALORIE_BASE_MAX				((U16) RESOURCE_BASE_HEALTH_CALORIE_END)
RESOURCE_BASE_TABLE_ITEM(HEALTH_CALORIE)

#define  HEALTH_MENSTRUAL_BASE				((U16) RESOURCE_BASE_HEALTH_MENSTRUAL)
#define	HEALTH_MENSTRUAL					HEALTH_MENSTRUAL_BASE /* Be backward compatible */
#define	HEALTH_MENSTRUAL_BASE_MAX			((U16) RESOURCE_BASE_HEALTH_MENSTRUAL_END)
RESOURCE_BASE_TABLE_ITEM(HEALTH_MENSTRUAL)

#define  HEALTH_BMI_BASE					((U16) RESOURCE_BASE_HEALTH_BMI)
#define	HEALTH_BMI							HEALTH_BMI_BASE /* Be backward compatible */
#define	HEALTH_BMI_BASE_MAX					((U16) RESOURCE_BASE_HEALTH_BMI_END)
RESOURCE_BASE_TABLE_ITEM(HEALTH_BMI)


/****************************************************************************
* Message
*****************************************************************************/

#define  MESSAGES_BASE						((U16) RESOURCE_BASE_MESSAGES)
#define	MESSAGES_BASE_MAX					((U16) RESOURCE_BASE_MESSAGES_END)
RESOURCE_BASE_TABLE_ITEM(MESSAGES)

#define  CHAT_BASE							((U16) RESOURCE_BASE_CHAT)
#define	CHAT_BASE_MAX						((U16) RESOURCE_BASE_CHAT_END)
RESOURCE_BASE_TABLE_ITEM(CHAT)

/****************************************************************************
* Connection
*****************************************************************************/
#define  CONNECTS_BASE						((U16) RESOURCE_BASE_CONNECTS)
#define	CONNECTS_BASE_MAX					((U16) RESOURCE_BASE_CONNECTS_END)
RESOURCE_BASE_TABLE_ITEM(CONNECTS)

/****************************************************************************
* Setting
*****************************************************************************/

#define  SETTINGS_BASE						((U16) RESOURCE_BASE_SETTINGS)
#define	SETTINGS_BASE_MAX					((U16) RESOURCE_BASE_SETTINGS_END)
RESOURCE_BASE_TABLE_ITEM(SETTINGS)

#define  SECURITY_SETUP_BASE				((U16) RESOURCE_BASE_SECURITY_SETUP)
#define	SECURITY_SETUP_BASE_MAX				((U16) RESOURCE_BASE_SECURITY_SETUP_END)
RESOURCE_BASE_TABLE_ITEM(SECURITY_SETUP)

/****************************************************************************
* Call Management
*****************************************************************************/

#define  CALL_MANAGEMENT_BASE				((U16) RESOURCE_BASE_CALL_MANAGEMENT)
#define	CALL_MANAGEMENT_BASE_MAX			((U16) RESOURCE_BASE_CALL_MANAGEMENT_END)
RESOURCE_BASE_TABLE_ITEM(CALL_MANAGEMENT)

/****************************************************************************
* Idle App
*****************************************************************************/

#define  IDLE_APP_BASE						((U16) RESOURCE_BASE_IDLE_APP)
#define	IDLE_APP_BASE_MAX					((U16) RESOURCE_BASE_IDLE_APP_END)
RESOURCE_BASE_TABLE_ITEM(IDLE_APP)

/****************************************************************************
* Category Screens
*****************************************************************************/

#define  CATEGORY_SCREENS_BASE				((U16) RESOURCE_BASE_CATEGORY_SCREENS)
#define	CATEGORY_SCREENS_BASE_MAX			((U16) RESOURCE_BASE_CATEGORY_SCREENS_END)
RESOURCE_BASE_TABLE_ITEM(CATEGORY_SCREENS)


/****************************************************************************
* ANALOG_TV_SUPPORT Screens
*****************************************************************************/

#if defined(__MMI_ANALOGTV__)
#define ANALOGTV_BASE					((U16)RESOURCE_BASE_ANALOGTV)
#define ANALOGTV_BASE_MAX				((U16)RESOURCE_BASE_ANALOGTV_END)
RESOURCE_BASE_TABLE_ITEM(ANALOGTV)
#endif


/****************************************************************************
* Demo App
*****************************************************************************/

#define  DEMO_APP_BASE						((U16) RESOURCE_BASE_DEMO_APP)
#define	DEMO_APP_BASE_MAX					((U16) RESOURCE_BASE_DEMO_APP_END)
RESOURCE_BASE_TABLE_ITEM(DEMO_APP)

/****************************************************************************
* Phone Setup [13000-13999]
*****************************************************************************/
/* HACK. The value of PHONE_SETUP_BASE is fixed in NVRAM definition */

#define	PHONE_SETUP_BASE			13000	
#define	SCREENSAVER_BASE			(PHONE_SETUP_BASE + 200)
#define  WALLPAPER_BASE				(PHONE_SETUP_BASE + 300)
#define  SPOF_BASE					(PHONE_SETUP_BASE + 400)	 
#define  NITZ_BASE					(PHONE_SETUP_BASE + 500)
#define  LANG_SETTINGS_BASE			(PHONE_SETUP_BASE + 600)	
#define	HELP_BASE					(PHONE_SETUP_BASE + 700)
#define	PHNSET_ONOFF_BASE			(PHONE_SETUP_BASE + 800)
#define	FLIGHT_MODE_BASE			(PHONE_SETUP_BASE + 900)
//xiaoke 08.12.31
#ifdef __DOUBLE_SIMCARD_211__      
#define   SWITCH_SIMCARD_BASE		(PHONE_SETUP_BASE + 950)
#endif
//xiaoke 08.12.31

/****************************************************************************
* Misc
*****************************************************************************/

#define  SIM_DETECTION_BASE					((U16) RESOURCE_BASE_SIM_DETECTION)
#define	SIM_DETECTION_BASE_MAX				((U16) RESOURCE_BASE_SIM_DETECTION_END)
RESOURCE_BASE_TABLE_ITEM(SIM_DETECTION)

#define  SAT_BASE							((U16) RESOURCE_BASE_SAT)
#define	SAT_BASE_MAX						((U16) RESOURCE_BASE_SAT_END)
RESOURCE_BASE_TABLE_ITEM(SAT)

#define  EMAIL_BASE							((U16) RESOURCE_BASE_EMAIL)
#define	EMAIL_BASE_MAX						((U16) RESOURCE_BASE_EMAIL_END)
RESOURCE_BASE_TABLE_ITEM(EMAIL)

#define  IMPS_BASE							((U16) RESOURCE_BASE_IMPS)
#define	IMPS_BASE_MAX						((U16) RESOURCE_BASE_IMPS_END)
RESOURCE_BASE_TABLE_ITEM(IMPS)

#define  CALL_NEW_BASE						((U16) RESOURCE_BASE_CALL_NEW)
#define 	CALL_BASE_NEW					CALL_NEW_BASE /* Be backward compatible */
#define	CALL_NEW_BASE_MAX					((U16) RESOURCE_BASE_CALL_NEW_END)
RESOURCE_BASE_TABLE_ITEM(CALL_NEW)

#define  CALL_HISTORY_BASE					((U16) RESOURCE_BASE_CALL_HISTORY)
#define	CALL_HISTORY						CALL_HISTORY_BASE /* Be backward compatible */
#define	CALL_HISTORY_BASE_MAX				((U16) RESOURCE_BASE_CALL_HISTORY_END)
RESOURCE_BASE_TABLE_ITEM(CALL_HISTORY)

#define  PROFILES_BASE						((U16) RESOURCE_BASE_PROFILES)
#define	PROFILES_BASE_MAX					((U16) RESOURCE_BASE_PROFILES_END)
RESOURCE_BASE_TABLE_ITEM(PROFILES)

#define  CHARGER_BASE						((U16) RESOURCE_BASE_CHARGER)
#define	CHARGER_BASE_MAX					((U16) RESOURCE_BASE_CHARGER_END)
RESOURCE_BASE_TABLE_ITEM(CHARGER)

#define  CALL_BARRING_BASE					((U16) RESOURCE_BASE_CALL_BARRING)
#define	CALL_BARRING						CALL_BARRING_BASE /* Be backward compatible */
#define	CALL_BARRING_BASE_MAX				((U16) RESOURCE_BASE_CALL_BARRING_END)
RESOURCE_BASE_TABLE_ITEM(CALL_BARRING)

#define  GLOBAL_BASE						((U16) RESOURCE_BASE_GLOBAL)
#define	GLOBAL_BASE_MAX						((U16) RESOURCE_BASE_GLOBAL_END)
RESOURCE_BASE_TABLE_ITEM(GLOBAL)

#define  NETWORK_SETUP_BASE					((U16) RESOURCE_BASE_NETWORK_SETUP)
#define	NETWORK_SETUP_BASE_MAX				((U16) RESOURCE_BASE_NETWORK_SETUP_END)
RESOURCE_BASE_TABLE_ITEM(NETWORK_SETUP)

#define  DOWNLOAD_BASE						((U16) RESOURCE_BASE_DOWNLOAD)
#define	DOWNLOAD_BASE_MAX					((U16) RESOURCE_BASE_DOWNLOAD_END)
RESOURCE_BASE_TABLE_ITEM(DOWNLOAD)

#define  VOICE_MEMO_BASE					((U16) RESOURCE_BASE_VOICE_MEMO)
#define	VOICE_MEMO_BASE_MAX					((U16) RESOURCE_BASE_VOICE_MEMO_END)
RESOURCE_BASE_TABLE_ITEM(VOICE_MEMO)

#define  INPUT_METHOD_BASE					((U16) RESOURCE_BASE_INPUT_METHOD)
#define	INPUT_METHOD_BASE_MAX				((U16) RESOURCE_BASE_INPUT_METHOD_END)
RESOURCE_BASE_TABLE_ITEM(INPUT_METHOD)

#define  CTK_BASE							((U16) RESOURCE_BASE_CTK)
#define	CTK_BASE_MAX						((U16) RESOURCE_BASE_CTK_END)
RESOURCE_BASE_TABLE_ITEM(CTK)

#define  ENGINEER_MODE_BASE					((U16) RESOURCE_BASE_ENGINEER_MODE)
#define	ENGINEER_MODE_BASE_MAX				((U16) RESOURCE_BASE_ENGINEER_MODE_END)
RESOURCE_BASE_TABLE_ITEM(ENGINEER_MODE)

#define  CAL_BASE							((U16) RESOURCE_BASE_CAL)
#define	CAL_BASE_MAX						((U16) RESOURCE_BASE_CAL_END)
RESOURCE_BASE_TABLE_ITEM(CAL)

#define  FACTORY_MODE_BASE					((U16) RESOURCE_BASE_FACTORY_MODE)
#define	FACTORY_MODE_BASE_MAX				((U16) RESOURCE_BASE_FACTORY_MODE_END)
RESOURCE_BASE_TABLE_ITEM(FACTORY_MODE)

#define  COMPOSE_RING_TONE_ENGINE_BASE		((U16) RESOURCE_BASE_COMPOSE_RING_TONE_ENGINE)
#define	COMPOSE_RING_TONE_ENGINE_BASE_MAX	((U16) RESOURCE_BASE_COMPOSE_RING_TONE_ENGINE_END)
RESOURCE_BASE_TABLE_ITEM(COMPOSE_RING_TONE_ENGINE)

#define  COMPOSE_RINGTONE_BASE				((U16) RESOURCE_BASE_COMPOSE_RINGTONE)
#define	COMPOSE_RINGTONE_BASE_MAX			((U16) RESOURCE_BASE_COMPOSE_RINGTONE_END)
RESOURCE_BASE_TABLE_ITEM(COMPOSE_RINGTONE)

#define  WAP_BASE							((U16) RESOURCE_BASE_WAP)
#define	WAP_BASE_MAX						((U16) RESOURCE_BASE_WAP_END)
RESOURCE_BASE_TABLE_ITEM(WAP)

#define  AUDIO_PLAYER_BASE					((U16) RESOURCE_BASE_AUDIO_PLAYER)
#define	AUDIO_PLAYER_BASE_MAX				((U16) RESOURCE_BASE_AUDIO_PLAYER_END)
RESOURCE_BASE_TABLE_ITEM(AUDIO_PLAYER)

#define  SOUND_RECORDER_BASE				((U16) RESOURCE_BASE_SOUND_RECORDER)
#define	SOUND_RECORDER_BASE_MAX				((U16) RESOURCE_BASE_SOUND_RECORDER_END)
RESOURCE_BASE_TABLE_ITEM(SOUND_RECORDER)

#define  FILE_MANAGER_BASE					((U16) RESOURCE_BASE_FILE_MANAGER)
#define	FILE_MANAGER_BASE_MAX				((U16) RESOURCE_BASE_FILE_MANAGER_END)
RESOURCE_BASE_TABLE_ITEM(FILE_MANAGER)

#define  JAVA_BASE							((U16) RESOURCE_BASE_JAVA)
#define	JAVA_BASE_MAX						((U16) RESOURCE_BASE_JAVA_END)
RESOURCE_BASE_TABLE_ITEM(JAVA)

#define  NV_RAM_FILE_IMAGE_BASE				((U16) RESOURCE_BASE_NV_RAM_FILE_IMAGE)
#define	NV_RAM_FILE_IMAGE_ID_BASE			NV_RAM_FILE_IMAGE_BASE
#define	NV_RAM_FILE_IMAGE_BASE_MAX			((U16) RESOURCE_BASE_NV_RAM_FILE_IMAGE_END)
RESOURCE_BASE_TABLE_ITEM(NV_RAM_FILE_IMAGE)

#define  EXT_DEVICE_BASE					((U16) RESOURCE_BASE_EXT_DEVICE)
#define	EXT_DEVICE_BASE_MAX					((U16) RESOURCE_BASE_EXT_DEVICE_END)
RESOURCE_BASE_TABLE_ITEM(EXT_DEVICE)

#define  MMEDIA_BASE						((U16) RESOURCE_BASE_MMEDIA)
#define	MMEDIA_BASE_MAX						((U16) RESOURCE_BASE_MMEDIA_END)
RESOURCE_BASE_TABLE_ITEM(MMEDIA)

#define  EXTRA_APP_BASE						((U16) RESOURCE_BASE_EXTRA_APP)
#define	EXTRA_APP_BASE_MAX					((U16) RESOURCE_BASE_EXTRA_APP_END)
RESOURCE_BASE_TABLE_ITEM(EXTRA_APP)

#define  NV_RAM_FILE_EXT_MELODY_BASE		((U16) RESOURCE_BASE_NV_RAM_FILE_EXT_MELODY)
#define	NV_RAM_FILE_EXT_MELODY_ID_BASE		NV_RAM_FILE_EXT_MELODY_BASE
#define	NV_RAM_FILE_EXT_MELODY_BASE_MAX		((U16) RESOURCE_BASE_NV_RAM_FILE_EXT_MELODY_END)
RESOURCE_BASE_TABLE_ITEM(NV_RAM_FILE_EXT_MELODY)

#define  GAME_BASE							((U16) RESOURCE_BASE_GAME)
#define	GAME_BASE_MAX						((U16) RESOURCE_BASE_GAME_END)
RESOURCE_BASE_TABLE_ITEM(GAME)

#define  MMIAPI_BASE						((U16) RESOURCE_BASE_MMIAPI)
#define	MMIAPI_BASE_MAX						((U16) RESOURCE_BASE_MMIAPI_END)
RESOURCE_BASE_TABLE_ITEM(MMIAPI)

#define  VOBJECT_BASE						((U16) RESOURCE_BASE_VOBJECT)
#define	VOBJECT_BASE_MAX					((U16) RESOURCE_BASE_VOBJECT_END)
RESOURCE_BASE_TABLE_ITEM(VOBJECT)

#define  VCARD_BASE							((U16) RESOURCE_BASE_VCARD)
#define	VCARD_BASE_MAX						((U16) RESOURCE_BASE_VCARD_END)
RESOURCE_BASE_TABLE_ITEM(VCARD)

#define  VCALENDAR_BASE						((U16) RESOURCE_BASE_VCALENDAR)
#define	VCALENDAR_BASE_MAX					((U16) RESOURCE_BASE_VCALENDAR_END)
RESOURCE_BASE_TABLE_ITEM(VCALENDAR)

#define  STOPWATCH_BASE						((U16) RESOURCE_BASE_STOPWATCH)
#define	STOPWATCH_BASE_MAX					((U16) RESOURCE_BASE_STOPWATCH_END)
RESOURCE_BASE_TABLE_ITEM(STOPWATCH)

#define  PICTURE_EDITOR_BASE				((U16) RESOURCE_BASE_PICTURE_EDITOR)
#define	PICTURE_EDITOR_BASE_MAX				((U16) RESOURCE_BASE_PICTURE_EDITOR_END)
RESOURCE_BASE_TABLE_ITEM(PICTURE_EDITOR)

#define  CAMERA_BASE						((U16) RESOURCE_BASE_CAMERA)
#define	CAMERA_BASE_MAX						((U16) RESOURCE_BASE_CAMERA_END)
RESOURCE_BASE_TABLE_ITEM(CAMERA)

#define  IMAGE_VIEWER_BASE					((U16) RESOURCE_BASE_IMAGE_VIEWER)
#define	IMAGE_VIEWER_BASE_MAX				((U16) RESOURCE_BASE_IMAGE_VIEWER_END)
RESOURCE_BASE_TABLE_ITEM(IMAGE_VIEWER)

#define  VDOPLY_BASE						((U16) RESOURCE_BASE_VDOPLY)
#define	VDOPLY_BASE_MAX						((U16) RESOURCE_BASE_VDOPLY_END)
RESOURCE_BASE_TABLE_ITEM(VDOPLY)

#define  VDOREC_BASE						((U16) RESOURCE_BASE_VDOREC)
#define	VDOREC_BASE_MAX						((U16) RESOURCE_BASE_VDOREC_END)
RESOURCE_BASE_TABLE_ITEM(VDOREC)

#define  FM_RADIO_BASE						((U16) RESOURCE_BASE_FM_RADIO)
#define	FM_RADIO_BASE_MAX					((U16) RESOURCE_BASE_FM_RADIO_END)
RESOURCE_BASE_TABLE_ITEM(FM_RADIO)

#define  VRSD_BASE							((U16) RESOURCE_BASE_VRSD)
#define	VRSD_BASE_MAX						((U16) RESOURCE_BASE_VRSD_END)
RESOURCE_BASE_TABLE_ITEM(VRSD)

#define  VRSI_BASE							((U16) RESOURCE_BASE_VRSI)
#define	VRSI_BASE_MAX						((U16) RESOURCE_BASE_VRSI_END)
RESOURCE_BASE_TABLE_ITEM(VRSI)

#define  PHOEDT_BASE						((U16) RESOURCE_BASE_PHOEDT)
#define	PHOEDT_BASE_MAX						((U16) RESOURCE_BASE_PHOEDT_END)
RESOURCE_BASE_TABLE_ITEM(PHOEDT)

#define  MESSAGE_CLUB_BASE					((U16) RESOURCE_BASE_MESSAGE_CLUB)
#define	MESSAGE_CLUB_BASE_MAX				((U16) RESOURCE_BASE_MESSAGE_CLUB_END)
RESOURCE_BASE_TABLE_ITEM(MESSAGE_CLUB)

#define  POC_BASE							((U16) RESOURCE_BASE_POC)
#define	POC_BASE_MAX						((U16) RESOURCE_BASE_POC_END)
RESOURCE_BASE_TABLE_ITEM(POC)

#define  AB_REPEATER_BASE					((U16) RESOURCE_BASE_AB_REPEATER)
#define	AB_REPEATER_BASE_MAX				((U16) RESOURCE_BASE_AB_REPEATER_END)
RESOURCE_BASE_TABLE_ITEM(AB_REPEATER)

#define  FM_SCHEDULE_REC_BASE				((U16) RESOURCE_BASE_FM_SCHEDULE_REC)
#define	FM_SCHEDULE_REC_BASE_MAX			((U16) RESOURCE_BASE_FM_SCHEDULE_REC_END)
RESOURCE_BASE_TABLE_ITEM(FM_SCHEDULE_REC)


//add by niej for Sp
#define  SP_BASE				((U16) RESOURCE_BASE_SP)
#define	 SP_BASE_MAX			((U16) RESOURCE_BASE_SP_END)

//add by chenhe for jasper usb
#define  JASPER_USB_BASE				((U16) RESOURCE_BASE_JASPER_USB)
#define	 JASPER_USB_BASE_MAX			((U16) RESOURCE_BASE_JASPER_USB_END)

//added by guojian 20080521
#define  EBOOK_BASE						((U16) RESOURCE_BASE_EBOOK)
#define	EBOOK_BASE_MAX				((U16) RESOURCE_BASE_EBOOK_END)
RESOURCE_BASE_TABLE_ITEM(EBOOK)

// fengzb add begin
#ifdef JATAAYU_SUPPORT

#define JAT_WAP_BASE		((U16) RESOURCE_BASE_JAT_WAP)
#define	JAT_WAP_BASE_MAX				((U16) RESOURCE_BASE_JAT_WAP_END)
RESOURCE_BASE_TABLE_ITEM(JAT_WAP)

#define  JAT_MMS_BASE					((U16) RESOURCE_BASE_JAT_MMS)
#define	 JAT_MMS_BASE_MAX					((U16) RESOURCE_BASE_JAT_MMS_END)
RESOURCE_BASE_TABLE_ITEM(JAT_MMS)


#define JATAAYU_PROVISIONING_BASE           ((U16) RESOURCE_BASE_JATAAYU_PROVISIONING)
#define JATAAYU_PROVISIONING_BASE_MAX       ((U16) RESOURCE_BASE_JATAAYU_PROVISIONING_END)
RESOURCE_BASE_TABLE_ITEM(JATAAYU_PROVISIONING) 
#endif

// fengzb add end
/****************************************************************************
* Main Menu
*****************************************************************************/
#define  MAIN_MENU_BI_DEGREE_BASE						((U16) RESOURCE_BASE_MAIN_MENU_BI_DEGREE)
#define	MAIN_MENU_BI_DEGREE_BASE_MAX					((U16) RESOURCE_BASE_MAIN_MENU_BI_DEGREE_END)
RESOURCE_BASE_TABLE_ITEM(MAIN_MENU_BI_DEGREE)

/***************************************************************************
* Double SIM
***************************************************************************/
#ifdef __MMI_MULTI_SIM__
#define MERCURY_BASE                    ((U16) RESOURCE_BASE_MERCURY)
#define	MERCURY_BASE_MAX                ((U16) RESOURCE_BASE_MERCURY_END)
RESOURCE_BASE_TABLE_ITEM(MERCURY)
#endif
#ifdef __MMI_NOTEPAD__
#define  NOTEPAD_BASE					((U16) RESOURCE_BASE_NOTEPAD)
#define	NOTEPAD_BASE_MAX					((U16) RESOURCE_BASE_NOTEPAD_END)
RESOURCE_BASE_TABLE_ITEM(NOTEPAD)
#endif

#define  ZYX_IPTV_BASE					((U16) RESOURCE_BASE_ZYX_IPTV)
#define	ZYX_IPTV_BASE_MAX					((U16) RESOURCE_BASE_ZYX_IPTV_END)
RESOURCE_BASE_TABLE_ITEM(ZYX_IPTV)

#ifdef _KK_SUPPORT_

#define  KK_BASE				((U16) RESOURCE_BASE_KK)
#define	 KK_BASE_MAX		((U16) RESOURCE_BASE_KK_END)
RESOURCE_BASE_TABLE_ITEM(KK)
#endif
#ifdef __ZYX_PLATFORM_SUPPORT__
    #define ZYX_PLATFORM_BASE         ((U16) RESOURCE_BASE_ZYX_PLATFORM)
    #define ZYX_PLATFORM_MAX         ((U16) RESOURCE_BASE_ZYX_PLATFORM_END)
    RESOURCE_BASE_TABLE_ITEM(ZYX_PLATFORM)
#endif

RESOURCE_BASE_TABLE_ITEM(END)
/* End of resource table */
RESOURCE_BASE_TABLE_END()

#endif /* _PIXCOM_DATA_TYPES_H */

