/******************************************************************************/
/*              Copyright (C) 2005, CII Technologies, Inc.                    */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    bal_init.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*                   */
/******************************************************************************/
#include "csw.h"
#include "stdkey.h"
#include "event.h"
#include "base_prv.h"
#include "dm.h"
#include "sxr_sbx.h"
#include "chip_id.h"
#include "bal_fstrace.h"


BOOL BAL_ApsEvtParse (COS_EVENT* pEvent);
#if 0 
extern UINT32 _enalbe_flash_rw;
UINT32 _enalbe_flash_rw = 1;

BOOL BAL_GetFlashVersion()
{
    return _enalbe_flash_rw;
}
#endif 

//#define CSW_TRACE(x, fmt, ...)   {}

UINT16 g_PreKeyCode = 0;
UINT16 g_PreKeyPressTime = 0;
extern VOID DM_CheckPowerOnCause();

#define DM_REGISTER_HANDLER_ARRAY_SIZE DM_DEVID_END_+1

PRIVATE PFN_DEVICE_HANDLER g_devHandlerArray[DM_REGISTER_HANDLER_ARRAY_SIZE];

extern UINT32 _Aps_ServiceEvProc(CFW_EVENT* pEvent, VOID* pUserData);
//extern void TCpIPInit(void);
extern BOOL BAL_ApsTaskMain (COS_EVENT* pEvent);

extern UINT8 dm_GetStdKey(UINT8 key);
extern BOOL tm_AlramProc(COS_EVENT* pEv);
extern BOOL pm_PrvInfoInit();
extern BOOL pm_BatteryInit();
extern BOOL dm_PowerOnCheck(UINT16* pCause);
extern BOOL dm_Init();
extern BOOL TM_PowerOn();
extern BOOL pm_BatteryMonitorProc(COS_EVENT * pEv);
extern VOID audio_ParseMsg( COS_EVENT* pEvent);
extern UINT8 DM_BuzzerSongMsg( COS_EVENT* pEvent);
//
// 
//
BOOL BAL_DevHandlerInit()
{
    UINT8 i = 0;
    
    for(i=0; i< DM_REGISTER_HANDLER_ARRAY_SIZE; i++)
        g_devHandlerArray[i] = NULL;

    g_PreKeyCode = 0;

    return TRUE;
}

//
// Install IRS Handler
//
BOOL DM_RegisterHandler(UINT8 nDeviceId, PFN_DEVICE_HANDLER pfnDevHandler)
{
    UINT32 ret = TRUE; //Modify from ERR_SUCCESS -> TRUE

    g_devHandlerArray[nDeviceId] = pfnDevHandler;    
    
    return ret;
}

BOOL dm_SendGpioMessage( COS_EVENT* pEv)
{
    return TRUE;
}
BOOL turn_key=TRUE;
BOOL dm_SendKeyMessage( COS_EVENT* pEv)
{
#if 0
  if(g_devHandlerArray[DM_DEVID_KEY])
        g_devHandlerArray[DM_DEVID_KEY](pEv);
    else
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    return TRUE;
#endif
    
	sxr_SbxInfo_t  Mail;
	Mail.NbAvailMsg=0;
	if (COS_GetDefaultMmiTaskHandle())
	    sxr_GetSbxInfo( ((TASK_HANDLE*)COS_GetDefaultMmiTaskHandle())->nMailBoxId, &Mail);

	if ((Mail.NbAvailMsg >30)&&(turn_key==FALSE)) 
	{
      CSW_TRACE(BASE_BAL_TS_ID, TSTXT("\n !!keyboard mailbox full \n") );
      //turn_key=TRUE; // simon suggest that discard the all key message when the mail box nearly full
      return FALSE;
	}
  if(pEv->nEventId==EV_KEY_DOWN)
      turn_key=TRUE;
  if(pEv->nEventId==EV_KEY_UP)
      turn_key=FALSE;	

	
	if(g_devHandlerArray[DM_DEVID_KEY])
	    g_devHandlerArray[DM_DEVID_KEY](pEv);
	else
	{
	    COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
	}
	return TRUE;
}

BOOL dm_SendDeviceStatusMessage( COS_EVENT* pEv)
{
    if(g_devHandlerArray[DM_DEVID_DM_STATUS]) 
        g_devHandlerArray[DM_DEVID_DM_STATUS](pEv);
    else
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    return TRUE;
}

BOOL dm_SendPowerOnMessage( COS_EVENT* pEv)
{
    if(g_devHandlerArray[DM_DEVID_POWER_ON])
        g_devHandlerArray[DM_DEVID_POWER_ON](pEv);
    else
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    return TRUE;
}

BOOL dm_SendArlarmMessage( COS_EVENT* pEv)
{
    if(g_devHandlerArray[DM_DEVID_ALARM])
        g_devHandlerArray[DM_DEVID_ALARM](pEv);
    else
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    return TRUE;
}

BOOL dm_SendPMMessage( COS_EVENT* pEv)
{
    CSW_TRACE(BASE_BAL_TS_ID, " dm_SendPMMessage()\n");
    if(g_devHandlerArray[DM_DEVID_PM])
    {
        g_devHandlerArray[DM_DEVID_PM](pEv);
    }
    else
    {
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
    }

    return TRUE;
}

BOOL dm_SendGpadcMessage( COS_EVENT* pEv)
{
    if(g_devHandlerArray[DM_DEVID_GPADC])
        g_devHandlerArray[DM_DEVID_GPADC](pEv);
    else
        COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    return TRUE;
}
BOOL dm_SendDevTaskessage( COS_EVENT* pEv)
{
    BOOL ret = TRUE;
    
    ret = COS_SendEvent( BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) , pEv, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

    return ret;
}

BOOL dm_StartDevTaskTimer(UINT8 nTimerId, UINT8 nMode, UINT32 nElapse)
{
    return COS_SetTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY) ,nTimerId, nMode,nElapse); 
}

BOOL dm_StopDevTaskTimer(UINT8 nTimerId)
{
   return COS_KillTimerEX(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY),nTimerId);
}

//
// Internal Tasks for BAL
//

VOID BAL_SysTask (void* pData)
{
    COS_EVENT ev;
    BOOL bRet;
    
    bRet = TRUE;
    SUL_ZeroMemory32( &ev, SIZEOF(COS_EVENT) );
    
    CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_SysTask Start. \n") );

    for(;;)
    {
        COS_WaitEvent(BAL_TH(BAL_SYS_TASK_PRIORITY), &ev, COS_WAIT_FOREVER);
    
        if(ev.nEventId == EV_TIMER)
        {
            ev.nEventId  = EV_KEY_PRESS;
            ev.nParam1  = dm_GetStdKey(g_PreKeyCode);
            ev.nParam2  = 0;
            ev.nParam3  = 0;
            COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);            
        }
        else if(ev.nEventId == EV_CSW_PRV_GPIO)
        {
            UINT32 gpio = ev.nParam1;
            CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_SysTask: GPIO[%d] \n"), ev.nParam1);
           // gpio -= 8;//shawn
            if(gpio<=DM_DEVID_GPIO_15)
            {
                if(g_devHandlerArray[gpio])
                {
                    UINT32 data[4]={0,};
                    data[0] = ev.nEventId;
                    data[1] = ev.nParam1;
                    data[2] = ev.nParam2;
                    g_devHandlerArray[gpio]((COS_EVENT*)data);
                }
                else 
                {
                    ev.nEventId = EV_DM_GPIO_IND;
                    ev.nParam1  = gpio;
                    ev.nParam2  = ev.nParam2;
                    ev.nParam3  = 0;
                    COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);                
                }
            }
        }
        else if(ev.nEventId == EV_DM_EXTI_IND)
        {
             CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_SysTask: EXTI \n"), ev.nParam1);
             if(g_devHandlerArray[DM_DEVID_EXTI])
                g_devHandlerArray[DM_DEVID_EXTI](NULL);
             else
             { 
                ev.nEventId = EV_DM_EXTI_IND;
                ev.nParam1  = 0;
                ev.nParam2  = 0;
                ev.nParam3  = 0;
                COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            }
        } 
        else if(ev.nEventId == EV_TIM_ALRAM_IND)
        {
            tm_AlramProc(&ev);
        }else if(ev.nEventId == EV_CSW_PRV_RECORD_END)
        {
             CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_SysTask: EXTI \n"), ev.nParam1);
             if(g_devHandlerArray[DM_DEVID_RECORD_END])
                g_devHandlerArray[DM_DEVID_RECORD_END](NULL);
             else
             { 
                ev.nEventId = EV_DM_RECORD_END_IND;
                ev.nParam1  = 0;
                ev.nParam2  = 0;
                ev.nParam3  = 0;
                COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            }

        }else if(ev.nEventId == EV_CSW_PRV_STREAM_END)
        {
             CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_SysTask: EXTI \n"), ev.nParam1);
             if(g_devHandlerArray[DM_DEVID_STREAM_END])
                g_devHandlerArray[DM_DEVID_STREAM_END](NULL);
             else
             { 
                ev.nEventId = EV_DM_STREAM_END_IND;
                ev.nParam1  = 0;
                ev.nParam2  = 0;
                ev.nParam3  = 0;
                COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
            }

        }
        #if 1	
			 else if(ev.nEventId == EV_CSW_PRV_SPI) /*zj add, for spi */
			 {
				 	if(g_devHandlerArray[DM_DEVID_SPI])
				 	{
              UINT32 data[4]={0,};
              data[0] = ev.nEventId;
              data[1] = ev.nParam1;
              data[2] = ev.nParam2;
              data[3] = ev.nParam3;
              g_devHandlerArray[DM_DEVID_SPI]((COS_EVENT*)data);
				 	}
					else
          {
              ev.nEventId = DM_DEVID_SPI;
              ev.nParam1  = 0;
              ev.nParam2  = 0;
              ev.nParam3  = 0;
              COS_SendEvent(COS_GetDefaultMmiTaskHandle(), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);                
          }
				   	
			 }
			 #endif

    } // for(;;) loop
}

//
// Device Backgroud task. 
//
VOID BAL_DevMonitorTask (VOID* pData)
{
    COS_EVENT ev;
    UINT16 Cause ;
    SUL_ZeroMemory32( &ev, SIZEOF(COS_EVENT) );
    //
    // Init battery and charge routines.
    //
    pm_PrvInfoInit();
    pm_BatteryInit();
    #if ( CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
    DM_CheckPowerOnCause();
    #endif
    dm_PowerOnCheck(&Cause);
    dm_Init();
    TM_PowerOn();
    CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_DevMonitorTask Start.\n"));
    
    for(;;)
    {
        COS_WaitEvent(BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY), &ev, COS_WAIT_FOREVER);
        if(ev.nEventId == EV_TIMER)
        {
            CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_DevMonitorTask: Timer id:%d \n"), ev.nParam1);
            //Change the Timer name
            //if(ev.nParam1==BAL_BATTERY_TIMER1 || ev.nParam1 == BAL_BATTERY_TIMER2 || ev.nParam1 == PRV_DM_LPCLKCALIB_TIMER_ID)
            if(ev.nParam1 == PRV_BAL_BATTERY_TIMER2 || ev.nParam1 == PRV_DM_LPCLKCALIB_TIMER_ID)
            {
                pm_BatteryMonitorProc(&ev);
            }
            else if(ev.nParam1==PRV_DM_AUDIO_TIMER_ID)
            {
                audio_ParseMsg(  &ev );
            }
            else if(ev.nParam1==PRV_TM_POWERON_ALM_TIMER_ID)
            {
                tm_AlramProc(&ev);
            }
            #ifdef USE_BUZZER
            else if(ev.nParam1 == PRV_DM_BUZZER_TIMER_ID)
            {
                DM_BuzzerSongMsg(  &ev );   //Longman 09.14, edit 10.26
            }
            #endif

            else if(ev.nParam1 == PRV_CFW_TRACE_TO_FLASH_TIMER_ID)
            {
                bal_fsTraceProcess();
            }
        }
        else if(ev.nEventId == EV_CSW_PRV_LPS_WAKEUP)
        {
            pm_BatteryMonitorProc(&ev);
        }
        else
        {
            #ifdef USE_BUZZER
            if(ev.nEventId == SYS_EV_MASK_BUZZER_FILE)
            {
                DM_BuzzerSongMsg(  &ev );	//Longman 09.14, edit 10.26
            }
            #endif
            if(ev.nEventId == SYS_EV_MASK_AUDIO_TONE)
            {
                audio_ParseMsg( &ev );
            }
            else if( EV_DM_AUDIO_CONTROL_IND == ev.nEventId )
            {
               if( 0x01 == ev.nParam1 )
               {
                  DM_StartAudioEX();
               }
               else
               {
                  DM_StopAudioEX();
               }
            }
            else if( EV_DM_SET_AUDIO_VOLUME_IND == ev.nEventId )
            {
              DM_SetAudioVolumeEX((DM_SpeakerGain)(ev.nParam1));
            }
            else if( EV_DM_SET_MIC_GAIN_IND == ev.nEventId )
            {
              DM_SetMicGainEX((DM_MicGain)(ev.nParam1));
            }
            else if( EV_DM_SET_AUDIO_MODE_IND == ev.nEventId )
            {
              DM_SetAudioModeEX((UINT8)ev.nParam1);
            }
            CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_DevMonitorTask: %d \n"), ev.nEventId );
        }
    } // for(;;) loop 
}
//shawn
PRIVATE HANDLE bal_GetTaskHandle(UINT8 nTaskId)
{
    HANDLE hTask = 0;
    if( nTaskId == CSW_TASK_ID_SYS)
    {
        hTask = BAL_TH(BAL_SYS_TASK_PRIORITY);
    }  
    else if( nTaskId == CSW_TASK_ID_APS)
    {
        hTask = BAL_TH(BAL_CFW_ADV_TASK_PRIORITY);
    }
    else if( nTaskId == CSW_TASK_ID_DEV_MONITOR)
    {
        hTask = BAL_TH(BAL_DEV_MONITOR_TASK_PRIORITY);
    } 
    else if( nTaskId == CSW_TASK_ID_MMI_DEFAULT)
    {
        hTask = COS_GetDefaultMmiTaskHandle();
    }
    else if( nTaskId == CSW_TASK_ID_BG)
    {
        hTask = BAL_TH(COS_BK_TASK_PRI);
    }
    
    return hTask;
}

HANDLE CFW_bal_GetTaskHandle(UINT8 nTaskId)
{
    return bal_GetTaskHandle(nTaskId);
}
BOOL BAL_SendTaskEvent(COS_EVENT* pEvent, UINT8 nTaskId)
{
    HANDLE hTask = bal_GetTaskHandle(nTaskId);
      
    return COS_SendEvent(hTask, pEvent,COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

//#define _SPI_CAMERA_ENABLED_
//
// Backgroud task. lower than MMI tasks.
//
//#ifdef _SPI_CAMERA_ENABLED_

#ifdef __PRJ_WITH_SPICAM__ 

#define MSG_MMC_BAL_CAPTURE_REQ 0x1001
extern INT32 sensorready;
extern VOID camerap_Capture_After_Preview(UINT16 Width, UINT16 Height);
extern VOID camerap_GetImageFrame(UINT16 * buff,UINT32 size);


VOID BAL_BackgroundTask ( VOID* pData)
{
    COS_EVENT ev;
    SUL_ZeroMemory32( &ev, SIZEOF(COS_EVENT) );

    UINT16* imgBuf = NULL;
    UINT32 counts_img=0;
 
    while (1)
    {
        COS_WaitEvent(BAL_TH(COS_BK_TASK_PRI), &ev, COS_WAIT_FOREVER);

        if (ev.nEventId == MSG_MMC_BAL_CAPTURE_REQ)
        {
            imgBuf = (UINT16*)(ev.nParam1);
            hal_HstSendEvent(0x654305);
            camerap_Capture_After_Preview(240,320);

            camerap_GetImageFrame(imgBuf,240*320);
            
         //  pas6175_Capture_After_Preview(128,160);
         //  camerap_GetImageFrame(imgBuf,128*160);
            
          //  for(counts_img=0;counts_img<240*320;counts_img++)
            {
             //imgBuf[counts_img] = (imgBuf[counts_img] >> 8) |(imgBuf[counts_img] << 8);
            }
            sensorready = 1;            
        }
    }
    
}

#else
VOID BAL_BackgroundTask ( VOID* pData)
{
    COS_EVENT ev;
    SUL_ZeroMemory32( &ev, SIZEOF(COS_EVENT) );

#if 0 //20060801 bridge b   
    ev.nEventId = EV_DEVICE_STATUS_IND;
    ev.nParam1  = DM_STATUS_FLASH_INIT;
    if(bRet == TRUE)
        ev.nParam2 = 0;
    else
        ev.nParam2 = 1;
    
    dm_SendDeviceStatusMessage(&ev);
#endif ////20060801 bridge e
    for(;;)
    {
        COS_WaitEvent(BAL_TH(COS_BK_TASK_PRI), &ev, COS_WAIT_FOREVER);
        if(ev.nEventId == EV_TIMER)
        {
            ;//ret = CFW_PbkPowerOn(0x02, APP_UTI_SHELL);
        }
        else
        {
          ;
        }
    } 
}
#endif

//
// CFW Advanced task to process advanced stack protocol.
//
#ifdef CFW_TCPIP_SUPPORT

void TCpIPInit(void);
VOID netif_init(VOID);
void APS_Init(void);
VOID BAL_CFWApsTask ( VOID* pData)
{

    COS_EVENT ev;
    SUL_ZeroMemory32( &ev, SIZEOF(COS_EVENT) );
    CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_CFWApsTask Start.\n"));
#ifdef CFW_TCPIP_SUPPORT
    TCpIPInit();
    netif_init();
    APS_Init();
#endif
    for(;;)
    {
        COS_WaitEvent(BAL_TH(BAL_CFW_ADV_TASK_PRIORITY), &ev, COS_WAIT_FOREVER);
#ifdef CFW_TCPIP_SUPPORT
        // BAL_ApsTaskMain(&ev);
	      BAL_ApsEvtParse(&ev);
#endif
        
        if(ev.nEventId == EV_TIMER)
        {
            CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_CFWApsTask,Timer id:%d \n"), ev.nParam1);
        }
        else
        {
            CSW_TRACE(BASE_BAL_TS_ID, TSTXT("BAL_CFWApsTask: %d \n"), ev.nEventId );
        }
    } 

}
#endif


