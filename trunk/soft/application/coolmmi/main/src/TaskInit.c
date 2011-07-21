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
 * TaskInit.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Initilize the task structure for the entire application. Functions to allocate task resources, 
 *   create task communication interfaces and synchronization mechanism for common resource sharing.
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

#include "globalconstants.h"

#ifdef MMI_ON_WIN32

#include "stdc.h"
#include "thrdosinterface.h"
#include "debuginitdef.h"
#include "eventsgprot.h"
#include "wrappergprot.h"
#include "allappgprot.h"
#include "wrappergprot.h"
#include "oslgexdcl.h"
#include "taskinit.h"
#include "queuegprot.h"
#include "mmitaskprot.h"
#include "mmi_trace.h"

/***************************************************************************** 
* Define
*****************************************************************************/

/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Local Variable
*****************************************************************************/

/***************************************************************************** 
* Local Function
*****************************************************************************/

/***************************************************************************** 
* Global Variable
*****************************************************************************/
osl_task_info   			task_info_g1[WIN32_TOTAL_TASKS];
oslTimerid 					  TimerCallBackHdlr;
CRITICAL_SECTION 			TimerCS;
oslSoftTimerid 				TimerId[MAX_TIMERS];
extern BOOL MCI_LcdSizeSet(INT32 LcdWidth, INT32 LcdHeight); 
extern void Dummy_task(oslEntryType * entry_param);
const osl_task_create_info	task_create_tbl[WIN32_TOTAL_TASKS] = 
{
	{"DUM1 TASK", "DUM1 Q", 127, 128, 0, Dummy_task},
	{"ADP TASK", "ADP Q", 127, 4096, MAX_PRT_NODES, BAL_MmiTask},
	{"MMI TASK", "MMI Q", 127, 4096, MAX_PRT_NODES, BAL_MmiMainTaskEntry},
	{"DUM2 TASK", "DUM2 Q", 127, 128, 0, Dummy_task},
};

/***************************************************************************** 
* Global Function
*****************************************************************************/
extern void TimerCallBack(void *param);

/**************************************************************

	FUNCTION NAME		: Dummy_task()

  	PURPOSE				: read a messages & diapatche it to proper handler

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

	REMARKS				: This is used to read a messages & diapatche it to proper handler

***************************************************************/
void Dummy_task(oslEntryType * entry_param)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
  
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   
}

/*****************************************************************************
* FUNCTION
*  InitApplication
* DESCRIPTION
*   Create/Inititialize trace mechanism,task queues, semaphores,Threads etc..
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  void
*****************************************************************************/
void InitApplication()
{
#ifndef MMI_ON_HARDWARE_P
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	int i;
  
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  mmi_trace_init(); // Add by zhuoxz,2009-3-24
	for(i=0;i<MAX_TIMERS; i++)
	{
		TimerId[i]=NULL;
	}
	InitializeCriticalSection(&TimerCS);
	TimerCallBackHdlr=OslCreateTimer("TIMER");
	OslSetTimer(TimerCallBackHdlr,TimerCallBack, 0, TIMER_CALLBACK_RESOLUTION); //100 msec soft timer
	for(i=0;i<WIN32_TOTAL_TASKS-9;i++)
	{
		task_info_g1[i+9].task_ext_qid=OslCreateMsgQ(task_create_tbl[i].task_qname,
							sizeof(MYQUEUE),
							task_create_tbl[i].task_ext_qsize);
		task_info_g1[i+9].task_id = osl_create_task (task_create_tbl[i].task_name,
							task_create_tbl[i].task_priority,
							task_create_tbl[i].task_stack_size,
							task_create_tbl[i].task_entry_func,
							(void *)(i+9),0);
		
		task_info_g1[i+9].task_name=task_create_tbl[i].task_name;
		task_info_g1[i+9].task_priority=task_create_tbl[i].task_priority;
		task_info_g1[i+9].task_stack_size=task_create_tbl[i].task_stack_size;
		task_info_g1[i+9].task_entry_func=task_create_tbl[i].task_entry_func;

		task_info_g1[i+9].task_qname=task_create_tbl[i].task_qname;
		task_info_g1[i+9].task_ext_qsize=task_create_tbl[i].task_ext_qsize;
	}
#endif
}

/*****************************************************************************
* FUNCTION
*  TimerCallBack
* DESCRIPTION
*   Soft Timer Handler for Windows.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*  void
*****************************************************************************/
void TimerCallBack(void *param)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 i;
	void *tmr_callback_arg = NULL;
	oslTaskFuncPtr tmr_callback = NULL;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
   
	EnterCriticalSection(&TimerCS);
	for(i=0;i<MAX_TIMERS; i++)
	{
		if(TimerId[i]!=NULL)
		{
			TimerId[i]->tmr_count -= TIMER_CALLBACK_RESOLUTION;
			if(TimerId[i]->tmr_count == 0)
			{
				tmr_callback_arg = TimerId[i]->tmr_callback_arg;
				tmr_callback = TimerId[i]->tmr_callback;
				OslMfree(TimerId[i]);
				TimerId[i] = NULL;
				LeaveCriticalSection(&TimerCS);
				/* 
				 * Before execute timer_expiry_callback, leave the 
				 * criticalsection first. It avoids the deadlock that
				 * if start/stop the timer in tmr_callback().
				 */
				if (tmr_callback != NULL)
				{
					tmr_callback(tmr_callback_arg);
				}
				/*
				 * Before continuing search the next 
				 * timer_expiry_callback, enter the critical section first.
				 */
				EnterCriticalSection(&TimerCS);
			}
		}
	}
	LeaveCriticalSection(&TimerCS);
}


/* end,2009-4-29 */
UINT8 wap_mms_profile_setting_first = 0;//used for set default wap/mms profile  

#endif /* MMI_ON_WIN32 */

#ifdef MMI_ON_HARDWARE_P
/*************************************************************************
 * Include Statements
 *************************************************************************/
#include "mmi_features.h"
#include "mmi_data_types.h"
#include "kal_release.h"
#include "stack_types.h"
#include "taskinit.h"
#include "mmitask.h"
#include "mmitaskprot.h"
#include "applicationp_version.h"
#include "csw_ver.h"
#include "csw.h"
#include "mmi.h"
#include "uim.h"
#include "drv.h"
#include "dm.h"
#include "mmi_trace.h"
#include "lcd_sw.h"
#include "mci.h"
extern int sprintf(char *buf, const char *fmt, ...);
extern U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer );


void BAL_SetMMIDefaultValue()
{

}

void BAL_BtTaskEntry(void *pData);
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
#include "filesystemdef.h"
extern BOOL g_hasMountedTFlash2FS ;
extern BOOL g_tflashCard2Exist ;

BOOL checkTCard2Exist(void)
{
    return g_hasMountedTFlash2FS;
}

void MMI_initTFlashSecond(void)
{
	INT32 iRet = 0;
	UINT8 sectflash[20] = {0};
	INT i = 0;
        
	/*WeiD Modify Start For 6208 Ver: TBM780  on 2007-8-17 15:0 */
	AnsiiToUnicodeString(sectflash, TFLASH_SECOND_ROOT_NAME);
	/*WeiD Modify End  For 6208 Ver: TBM780  on 2007-8-17 15:0 */

	iRet = FS_MakeDir(sectflash, 0);
       
	if ( iRet != ERR_SUCCESS )
	{
		if ( iRet != ERR_FS_FILE_EXIST )
		{
			TRACE_EF(1, "[MMI_initTFlashSecond]sectflash dir isn't exist ! create dir & mount failed! ");    
			return;
		}
              TRACE_EF(1, "[MMI_initTFlashSecond] sectflash dir has exist ! ");      
	}
        
	iRet = FS_HasFormatted("mmc1", 0x01);
    
	switch ( iRet )
	{
		case ERR_FS_OPEN_DEV_FAILED: //打不开设备（卡没插上）
			g_tflashCard2Exist = FALSE;
			TRACE_EF(1, "[MMI_initTFlashSecond] Can't open device(sectflash card)!. ");
			return;

		case ERR_FS_UNKNOWN_FILESYSTEM://不支持的文件系统，需要格式化
			g_tflashCard2Exist = TRUE;
			TRACE_EF(1, "[MMI_initTFlashSecond] ERR_FS_UNKNOWN_FILESYSTEM : File system is not supported.");
			return;

		case ERR_FS_NOT_SUPPORT://不支持的文件系统，需要格式化
			g_tflashCard2Exist = TRUE;
			TRACE_EF(1, "[MMI_initTFlashSecond] ERR_FS_NOT_SUPPORT  : not supported fat32.");
			return;

		case ERR_FS_NOT_FORMAT:	//卡没格式化，需要格式化
			g_tflashCard2Exist = TRUE;                    
			TRACE_EF(1, "[MMI_initTFlashSecond] ERR_FS_NOT_FORMAT : No format!");
			return;

		case ERR_FS_HAS_FORMATED:

			g_tflashCard2Exist = TRUE;
                     
			TRACE_EF(1, "[MMI_initTFlashSecond] tfalsh FS_HAS_FORMATED,"
						" g_tflashCard2Exist =%d", g_tflashCard2Exist);
			for ( i = 0; i < 1; i++ )
			{
				TRACE_EF(1, "[MMI_initTFlashSecond] mount sectflash card >>>>>>%d<<<<<<", i+1);
				iRet = FS_Mount( "mmc1", sectflash, 0, FS_TYPE_FAT );

				if ( iRet == ERR_SUCCESS )
				{
                                    g_hasMountedTFlash2FS = TRUE;  
                                    TRACE_EF(1, "[MMI_initTFlashSecond] sectflash dir has mounted successfully !"
                                    	 " g_hasMountedTFlash2FS= %d", g_hasMountedTFlash2FS);
                                    
                                    break;
				}
			}
                 
			return;

		default:
			TRACE_EF(1, "mount sectflash : unknown Error!");
			break;
	}
}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
/* please move these wap/mms code to wap/mms module */
#ifdef JATAAYU_SUPPORT
#include "ddlbase.h"
#include "ddldefines.h"
#include "jcutils.h"
#include "jcerror.h"
#include "jcevent.h"
#include "jdi_transport.h"
#include "jdd_timerapi.h"
#include "jwap_mms_common.h"
#include "debuginitdef.h"

UINT8 wap_mms_profile_setting_first = 0;//used for set default wap/mms profile  
void WAP_MMS_initFlash(void)
{
	UINT8 folderName[48];
	INT32 result=-1;
	AnsiiToUnicodeString(folderName,WAPMMS_REGION_PATH_NAME);
	result=FS_MakeDir(folderName,0);
	MMI_ASSERT(!(ERR_SUCCESS != result&&ERR_FS_FILE_EXIST !=result ));
	//while(ERR_SUCCESS != result&&ERR_FS_FILE_EXIST !=result );

	result = FS_HasFormatted("WAP_MMS", FS_TYPE_FAT) ;
	if(result !=ERR_FS_HAS_FORMATED)
	{
		mmi_trace(1,"WAP_MMS_initFlash,WAP_MMS region is not formated.");
		result = FS_Format("WAP_MMS",FS_TYPE_FAT, FS_FORMAT_NORMAL);
		MMI_ASSERT(ERR_SUCCESS==result);
		//while(ERR_SUCCESS != result);
             wap_mms_profile_setting_first = 1;
	}

	result = FS_Mount("WAP_MMS", folderName,0, FS_TYPE_FAT );
	MMI_ASSERT(ERR_SUCCESS==result);
	//while(ERR_SUCCESS != result);

#if 0
	 UINT8 folderName1[48];
	strcpy(folderName1,WAPMMS_REGION_PATH_NAME);
	strcat(folderName1,"/abc.txt");
	memset(folderName,0x0,sizeof(folderName));
	AnsiiToUnicodeString(folderName,folderName1);
	chenheHandle=FS_Open(folderName,(FS_O_RDWR | FS_O_CREAT | FS_O_TRUNC),0);
	mmi_trace(1,"WAP_MMS_initFlash,create a test file is %d",chenheHandle);
#endif
	return;	

} 

void BAL_TransportTaskEntry(void *pArg)
{
       COS_Sleep(500);

	mmi_trace(1, "@@@ Transport Task Started @@@") ;
	jdi_TransportTask (0, NULL) ;
}
#endif

BOOL BAL_ApplicationInit ( VOID )
{
    HANDLE hTask;
    csw_SetResourceActivity(CSW_LP_RESOURCE_MMIACTIVE, CSW_SYS_FREQ_104M);    // Require 104 MHz

    // Register the MMI version to CSW.
    csw_VerSetMmiVersion(APPLICATION_VERSION_REVISION, "CoolMMI " \
                                                STRINGIFY_VALUE(APPLICATION_VERSION_REVISION));

   	mmi_trace_init();
	MCI_TaskInit();


#ifdef __MMI_BT_SUPPORT__
     hTask = COS_CreateTask(BAL_BtTaskEntry, 
                NULL, NULL,
                MMI_BTTASK_SIZE, 
               MMI_BTTASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "BT Task"); 
	ASSERT(hTask != HNULL);
	SetMmiTaskHandle(MOD_BT, hTask);
#endif	

    hTask = COS_CreateTask(BAL_MmiTask, 
                NULL, NULL,
                ADP_TASK_STACK_SIZE, 
                ADP_TASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "Adp Task");
	ASSERT(hTask != HNULL);
	SetMmiTaskHandle(MOD_ADP, hTask);
	
	hTask = COS_CreateTask(BAL_MmiMainTaskEntry, 
				NULL, NULL,
				MMI_TASK_STACK_SIZE, 
				MMI_TASK_PRIORITY, 
				COS_CREATE_DEFAULT, 0, "MMI Main Task");  
	ASSERT(hTask != HNULL);
	SetMmiTaskHandle(MOD_MMI, hTask);		

	hTask = COS_CreateTask(BAL_MultimediaTaskEntry, 
                NULL, NULL,
                MMI_MED_TASK_STACK_SIZE, 
                MMI_MED_TASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "Multimedia Task"); 
	ASSERT(hTask != HNULL);
	SetMmiTaskHandle(MOD_MED, hTask);

	hTask = COS_CreateTask(BAL_FSTaskEntry, 
                NULL, NULL,
                MMI_FSTASK_SIZE, 
                MMI_FSTASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "FS Task"); 
	ASSERT(hTask != HNULL);
	SetMmiTaskHandle(MOD_FMT, hTask);

	hTask = COS_CreateTask(BAL_USBTaskEntry, 
            NULL, NULL,
            MMI_USBTASK_SIZE, 
            MMI_USBTASK_PRIORITY, 
            COS_CREATE_DEFAULT, 0, "USB Task"); 
	ASSERT(hTask != HNULL);
	SetMmiTaskHandle(MOD_USB, hTask);
    
#ifdef JATAAYU_SUPPORT
    hTask = COS_CreateTask(BAL_TransportTaskEntry, 
                NULL, NULL,
                TRANSPORT_TASK_STACK_SIZE, 
                JAT_TRANSPORTTASK_PRIORITY, 
                COS_CREATE_DEFAULT, 0, "Transport Task") ; 
	ASSERT(hTask != HNULL);
	SetMmiTaskHandle(MOD_TRANSPORT, hTask);
    jdd_TimerInitialize () ;
#endif

#ifdef __J2ME__
	hTask = COS_CreateTask ( JavaMainTask,
	  NULL, NULL,
#ifdef __NATIVE_JAVA__
			1024,
#else
			JAVA_TASK_STACK_SIZE, 
#endif			
			JAVA_TASK_PRIORITY, 
			COS_CREATE_DEFAULT, 0, "JavaMainTask");
	ASSERT(hTask != HNULL);
	SetMmiTaskHandle(MOD_J2ME, hTask);
#endif

    return TRUE;
}

#endif /* MMI_ON_HARDWARE_P */

