#include <cswtype.h>
#include <base.h>
#include "chip_id.h"
#include "cfw.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include <hal.h>
#else
#include "hal_sys.h"
#include "hal_timers.h"
#include "hal_key.h"
#include "hal_gpio.h"
#endif

#include "csw_csp.h"
#include <cos.h>
#include <dm.h>
#include <base_prv.h>
#include <stdkey.h>
#include <tm.h>
#include <drv.h>
#include <ts.h>
#include <sul.h>

#include "pmd_m.h"

extern void lcd_present_CS_log(void);


#define PWR_KEY_DEBOUNCE    3

/*wangrui add   macro  2008.12.17   for fixbug10731 */	
#if(  (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)|| (CHIP_ASIC_ID == CHIP_ASIC_ID_GREENSTONE) )

#define CHARGERING_CANNOT_POWEROFF 1
#endif
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
extern INT32 VDS_FlushCacheAll(VOID);
#endif
#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)   
extern  BOOL tgt_FlushFactorySettings(VOID);
#endif

PRIVATE UINT8	PD_Trig     = FALSE;
UINT32  PowerOn_Cause = DM_POWRN_ON_CAUSE_UNKOWN;
//
// Soft Reset system.
//
BOOL DM_Reset(VOID)
{
    #if 0
    #define REG_SYS_GLOBAL_BASE	0x00e00000 
    volatile UINT32 *reg = ( *(volatile UINT32*)((REG_SYS_GLOBAL_BASE+0x04) | (0xa0000000)) );
    *reg |= 1<<30;
    #endif
    hal_SoftReset();
    while(1)
       ; // only dump loop here.
    return TRUE;
}

extern BOOL TM_PowerOff(void);
/*wangrui add    2008.12.17 begin,  for fixbug10731*/	
//extern UINT32 CFW_CfgSetSoftReset(UINT8 cause);
//extern UINT32 CFW_CfgGetSoftReset(UINT8 *cause);
/*wangrui add     2008.12.17 end*/	
//
// Power Off.
//

// 1 mean charging power on than plug out adapter
// other reserved
UINT8 g_powerdown_cause = 0;
VOID DM_SetPowerDown_Cause(UINT8 cause)
{
    g_powerdown_cause = cause;
}
BOOL DM_DeviceSwithOff (VOID)
{
    TM_PowerOff();

    switch(pmd_GetChargerStatus())
    {
    case PMD_CHARGER_PLUGGED           :
    case PMD_CHARGER_PRECHARGE         :
    case PMD_CHARGER_FAST_CHARGE       :
    case PMD_CHARGER_PULSED_CHARGE     :
    case PMD_CHARGER_PULSED_CHARGE_WAIT:
    case PMD_CHARGER_FULL_CHARGE       :
    case PMD_CHARGER_ERROR             :
    case PMD_CHARGER_TIMED_OUT         :
    case PMD_CHARGER_TEMPERATURE_ERROR :
#ifdef CHARGERING_CANNOT_POWEROFF
       if(g_powerdown_cause == 1)
       {
           CFW_CfgSetSoftReset(3);
       }
       else
       {
           CFW_CfgSetSoftReset(1);
       }
       g_powerdown_cause = 0;
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)      
        VDS_FlushCacheAll(); 
#endif

#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)   
       tgt_FlushFactorySettings();
#endif

       DM_Reset();
        break;
#endif
    case PMD_CHARGER_UNKNOWN           :
    case PMD_CHARGER_UNPLUGGED         :
    if(g_powerdown_cause == 1)
    {
        CFW_CfgSetSoftReset(3);
    }
    else
    {
        CFW_CfgSetSoftReset(2);
    }
       g_powerdown_cause = 0;
#if defined(USER_DATA_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)
VDS_FlushCacheAll(); 
#endif
#if defined(FACT_SETTING_CACHE_SUPPORT) && !defined(_T_UPGRADE_PROGRAMMER)   
       tgt_FlushFactorySettings();
#endif
        hal_ShutDown();
        break;
    case PMD_CHARGER_STATUS_QTY:
    case PMD_CHARGER_OPAL2_FULL:
    case PMD_CHARGER_OPAL2_FAST:
        break;
    }

    return TRUE;
}
//////////////////////////disable watchdog///////
BOOL dm_DisableWatchDog()
{
	hal_timWatchDog_Disable();
	return TRUE;
}
BOOL dm_SetWathcDogTime(UINT32 time)
{
	hal_timWatchDog_Start(time);
	
	return TRUE;
}
BOOL dm_SetWathcDog_KeepAlive()
{
	hal_timWatchDog_KeepAlive();
	return TRUE;

}
extern UINT32 dm_AlarmDisposeInEvt( UINT8 nCause, COS_EVENT* pEvent) ;
extern BOOL DM_KeyboardInit();
extern BOOL PM_PowerOnChck(void);
extern BOOL DRV_RegSetDbgVal(UINT16 nVal);
extern VOID DM_CheckPowerOnCause(VOID);


UINT32 CFW_ShellControl(UINT8 nCommand);

BOOL dm_PowerOnCheck(UINT16* pCause)
{
  COS_EVENT ev;
  BOOL bRet;
  UINT8 Causeflag;
  bRet = TRUE;
  SUL_ZeroMemory32( (VOID *)&ev, SIZEOF(COS_EVENT) );
 #if (CHIP_ASIC_ID != CHIP_ASIC_ID_JADE)
  /*the following codes to check whether it's charger in mode again,
  for the requrement: power off with charger in , we reset the phone
  so that the next power on should be chargering but not power on normal*/
  DM_CheckPowerOnCause();
	/*wangrui modify   2008.12.17 begin,  for fixbug10731*/	
	#ifdef CHARGERING_CANNOT_POWEROFF
	CFW_CfgGetSoftReset(&Causeflag);
  if (Causeflag > 0)
  {
      CFW_CfgSetSoftReset(0);
  }
	#endif

	/*wangrui Modify   2008.12.17 end*/	
 #endif 

  ev.nEventId = EV_DM_POWER_ON_IND;
  ev.nParam1 = PowerOn_Cause;
 #ifdef __221_UNIT_TEST__
     CFW_ShellControl(CFW_CONTROL_CMD_POWER_ON);  
 #endif

  if(PowerOn_Cause&DM_POWRN_ON_CAUSE_ALARM)
  {
      #if HAVE_TM_MODULE==1
      dm_AlarmDisposeInEvt(PowerOn_Cause, &ev);
      #endif
  
      TM_ALARM* p = NULL;
      p = (TM_ALARM*)ev.nParam3;
      if( p && (p->nIndex == 17) )
      {
          ;//
      }
  }
  if(PowerOn_Cause&DM_POWRN_ON_CAUSE_CHARGE)
  {
      ;//DM_KeyboardInit() ;
  }
  if(pCause)
    *pCause = PowerOn_Cause;
  dm_SendPowerOnMessage(&ev);
  hal_SysResetShutdownFlag();
      
  return TRUE; 
}

VOID DM_CheckPowerOnCause(VOID)
{
  #ifdef PMD_TWO_BATTARY
  UINT32 i = 0;
  UINT32 keytime = 0;
  #endif
    UINT8 Causeflag;

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
  if (hal_gpio_GetVal(DRV_ON_OFF_KEY))
  {
      PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
      return ;
  }
    
	if (hal_timRTC_AlarmReached())
	{
      hal_timRTC_AlarmClearStatus();
      PowerOn_Cause = DM_POWRN_ON_CAUSE_ALARM;      
      return ;
	}
    
  if(PM_PowerOnChck()) 
  {
      PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
      return ;
  } 
  if(hal_SoftResetHappened())
  {
      PowerOn_Cause = DM_POWRN_ON_CAUSE_RESET;
      return ;
  }
  if (hal_timWatchDog_Happened())
  {
      PowerOn_Cause = DM_POWRN_ON_CAUSE_EXCEPTION;
      return ;
  }
  // if(PowerOn_Cause != DM_POWRN_ON_CAUSE_UNKOWN)
  // PowerOn_Cause &= ~DM_POWRN_ON_CAUSE_UNKOWN;
  // PowerOn_Cause |= DM_POWRN_ON_CAUSE_KEY;
#else
    HAL_SYS_RESET_CAUSE_T resetCause;
    if (hal_SysGetShutdownFlag())
    {
        resetCause = HAL_SYS_RESET_CAUSE_NORMAL;
    }
    else
    {
        resetCause = hal_SysGetResetCause();
    }
    hal_HstSendEvent(0x12345678);
    hal_HstSendEvent(resetCause);
    
    switch (resetCause)
    {
        case HAL_SYS_RESET_CAUSE_NORMAL:
            #ifndef PMD_TWO_BATTARY

            if (hal_KeyOnOffStateGet())
            {
                PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
// This could be disabled by define Macro "NO_CS_LOGO", in this case, lcd_present_CS_log will be an empty function.
#ifndef NO_CS_LOGO
                lcd_present_CS_log();
#endif

            }
            else
            {
                switch(pmd_GetChargerStatus())
                {
                case PMD_CHARGER_UNKNOWN           :
                case PMD_CHARGER_UNPLUGGED         :
                    hal_ShutDown();//PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
                    break;
                case PMD_CHARGER_PLUGGED           :
                case PMD_CHARGER_PRECHARGE         :
                case PMD_CHARGER_FAST_CHARGE       :
                case PMD_CHARGER_PULSED_CHARGE     :
                case PMD_CHARGER_PULSED_CHARGE_WAIT:
                case PMD_CHARGER_FULL_CHARGE       :
                case PMD_CHARGER_ERROR             :
                case PMD_CHARGER_TIMED_OUT         :
                case PMD_CHARGER_TEMPERATURE_ERROR :
                    PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
                    break;
                case PMD_CHARGER_STATUS_QTY:
                case PMD_CHARGER_OPAL2_FULL:
                case PMD_CHARGER_OPAL2_FAST:
                    break;
                }
            }
            #else

            for ( i = 0;i<2000;i++)
            {
               if(i == 1999)
               {
                   hal_ShutDown();
               }
                if (hal_KeyOnOffStateGet())
                {
                   if (i == 0)
                   {
                       PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
                       i = 2000;
                   }
                   else
                   {
                       keytime ++;
                   }
                   if (keytime > 400)
                   {
                       PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
                       i = 2000;
                   }
    
                }
                else
                {
                    keytime = 0;
                    switch(pmd_GetChargerStatus())
                    {
                    case PMD_CHARGER_UNKNOWN           :
                    case PMD_CHARGER_UNPLUGGED         :
                        break;
                    case PMD_CHARGER_PLUGGED           :
                    case PMD_CHARGER_PRECHARGE         :
                    case PMD_CHARGER_FAST_CHARGE       :
                    case PMD_CHARGER_PULSED_CHARGE     :
                    case PMD_CHARGER_PULSED_CHARGE_WAIT:
                    case PMD_CHARGER_FULL_CHARGE       :
                    case PMD_CHARGER_ERROR             :
                    case PMD_CHARGER_TIMED_OUT         :
                    case PMD_CHARGER_TEMPERATURE_ERROR :
                        PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
                        i = 2000;
                        break;
                    case PMD_CHARGER_STATUS_QTY:
                        break;
                    }
                }
                hal_TimDelay(5 MS_WAITING);
            }

            #endif
            break;

        case HAL_SYS_RESET_CAUSE_ALARM:
            PowerOn_Cause = DM_POWRN_ON_CAUSE_ALARM;
            break;

        case HAL_SYS_RESET_CAUSE_RESTART:
        case HAL_SYS_RESET_CAUSE_HOST_DEBUG:

       	/*wangrui add    2008.12.17 begin,  for fixbug10731*/	
#ifdef CHARGERING_CANNOT_POWEROFF
            CFW_CfgGetSoftReset(&Causeflag);
            if((Causeflag == 1) &&
               (pmd_GetChargerStatus() != PMD_CHARGER_UNPLUGGED))
            {
                PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
            }
            else
                PowerOn_Cause = DM_POWRN_ON_CAUSE_RESET;
            if(Causeflag == 2)
            {
                PowerOn_Cause = DM_POWRN_ON_CAUSE_KEY;
            }
            //if((Causeflag == 3)&&(pmd_GetChargerStatus() != PMD_CHARGER_UNPLUGGED))
            if( Causeflag == 3)
            {
                PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
            }
#endif
/*wangrui add    2008.12.17  end*/	
            switch(pmd_GetChargerStatus())
            {
                case PMD_CHARGER_PLUGGED           :
                case PMD_CHARGER_PRECHARGE         :
                case PMD_CHARGER_FAST_CHARGE       :
                case PMD_CHARGER_PULSED_CHARGE     :
                case PMD_CHARGER_PULSED_CHARGE_WAIT:
                case PMD_CHARGER_FULL_CHARGE       :
                case PMD_CHARGER_ERROR             :
                case PMD_CHARGER_TIMED_OUT         :
                case PMD_CHARGER_TEMPERATURE_ERROR :
                    PowerOn_Cause = DM_POWRN_ON_CAUSE_CHARGE;
                    break;
                case PMD_CHARGER_STATUS_QTY:
                case PMD_CHARGER_OPAL2_FULL:
                case PMD_CHARGER_OPAL2_FAST:
                default:
                    PowerOn_Cause = DM_POWRN_ON_CAUSE_RESET;
                    break;
            }
            
            break;

        case HAL_SYS_RESET_CAUSE_WATCHDOG:
        default:
            PowerOn_Cause = DM_POWRN_ON_CAUSE_EXCEPTION;
            break;
    }
#endif
}

#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
//PRIVATE PFN_DEVICE_HANDLER DM_PowerdownHandler(UINT16 cause);
#endif

//
// Power down initialization.
// set up the on-off key trigger.
//
BOOL dm_PowerdownInit(VOID)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
  DRV_GpioSetMode(9, 1);
	// hal_SetUsrVectorMsk(HAL_GPIO_IRQ, HAL_GPIO_RISE(ON_OFF_BUTTON));
	// hal_gpio_ResetIrq(ON_OFF_BUTTON >> 8);
  DRV_GpioSetDirection(ON_OFF_BUTTON, 0);
  DRV_GpioSetEdgeIndication(ON_OFF_BUTTON,2, 50);
	// Normally, the vector should be a generic GPIO handler which should parse
	// the status and call the appropriate function, but for now, use
	// Util_Powerdown_Handler() as the only handler
	// hal_SetUsrVector(HAL_GPIO_IRQ, DM_PowerdownHandler);
  // DM_RegisterHandler(DM_DEVID_GPIO_9, DM_PowerdownHandler);
#else
  // Handled by the Keypad for Granite based chip
#endif
	PD_Trig = FALSE;
#ifdef HAVE_TIME_TEST
  long_time = 0;
#endif 

  return TRUE;
}


UINT32 DRV_GetRtcClock(VOID)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
   return hal_timRTC_GetTime();
#else
    // TODO Implement
    return 0;
#endif
}

UINT32 DRV_SetRtcClock(UINT32 ulClock)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    hal_timRTC_SetTime(ulClock);
#else
    // TODO Implement
#endif
    return 0;
}

BOOL DM_Delay(UINT32 nSec)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
    UINT32 Time = 0;
    Time = hal_timRTC_GetTime()/256;
    while (hal_timRTC_GetTime()/256 < (Time + nSec));
#else
    hal_TimDelay(nSec * HAL_TICK1S);
#endif
    return TRUE;
}


// Very basic delay for non-critical timing
// This will wait n/256 sec +/- 1/256 sec, so
// for a delay of at _least_ m/256, use (m+1)

BOOL DM_Wait(UINT32 nTick)
{
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	UINT32 RTC_Start;
	UINT32 RTC_End;
	RTC_Start = hal_timRTC_GetTime();
	if ((RTC_Start & 0x80000000) != 0)
	{	
       // In upper half of the range - readjust...
       RTC_Start &= ~(0x80000000);
       RTC_End = RTC_Start + nTick;
       while ((hal_timRTC_GetTime() + 0x80000000) < RTC_End)
            ;
	}
	else
	{
      RTC_End = RTC_Start + nTick;
      while (hal_timRTC_GetTime() < RTC_End)
            ;
	}
#else
  hal_TimDelay(nTick);
#endif
    return TRUE;
}
BOOL DM_SendDBGEvent()
{
  // hal_HST_SendEvent(0xc0);
  return TRUE;

}



#if 0
PRIVATE VOID DM_PowerdownHandler(UINT16 cause)
{
	// wait a bit to debounce (especially in the case of power on)
	UINT8 nLevel;
	COS_EVENT ev;
	
//	CSW_TRACE(100, "DM_PowerdownHandler!!!\n");
	//return;
	DRV_GpioGetLevel(ON_OFF_BUTTON,&nLevel );
	if (nLevel)
	{
//	hal_pwl1_Threshold(0);
	ev.nEventId =  EV_KEY_DOWN;
	ev.nParam1	= SK_POWER_OFF;
	ev.nParam2	= 0;
	ev.nParam3	= 0;
	dm_SendKeyMessage(&ev);
	}
	else
	{
//	hal_pwl1_Threshold(0xff);	

	ev.nEventId =  EV_KEY_UP;
	ev.nParam1	= SK_POWER_OFF;
	ev.nParam2	= 0;
	ev.nParam3	= 0;
	dm_SendKeyMessage(&ev);
	
	}
	#if 0
    #ifdef HAVE_TIME_TEST
	UINT32 Cur_Time = 0;
    #endif 
    
	COS_EVENT ev;

	if (PD_Trig == TRUE)
	{
        #ifdef HAVE_TIME_TEST
	    Cur_Time = hal_timRTC_GetTime()/256;
        #endif 
        
      //  hal_SetUsrVectorMsk(HAL_GPIO_IRQ, HAL_GPIO_RISE(ON_OFF_BUTTON)); 
        PD_Trig = FALSE; 
        
        #if 0 
        //TS_Print(" power down: %d, %d \n", Cur_Time, long_time);
        
	    if( Cur_Time > long_time + 2)
        {
            ev.nEventId = EV_KEY_PRESS; //EV_DM_POWER_OFF_NORMAL_IND;
            ev.nParam2  = 1; // long press 2 s
            //PD_Trig = FALSE;
        }
        else
        { 
            ev.nEventId = EV_KEY_UP; //EV_DM_POWER_OFF_NORMAL_IND;
            ev.nParam2  = 0; // normal press
        }
        #endif        

        #if 0 
        ev.nParam1  = SK_POWER_OFF;
        ev.nParam3  = 0;
        dm_SendKeyMessage(&ev);
        #else
        ev.nEventId = EV_CSW_PRV_KEY;
        ev.nParam1  = MAKEUINT32(0, SK_POWER_OFF);
        ev.nParam3  = 0;
        BAL_SendTaskEvent(&ev, CSW_TASK_ID_SYS);
        #endif 
	}
	else
	{        
        #ifdef HAVE_TIME_TEST
		Cur_Time = hal_timRTC_GetTime();        
		while (hal_timRTC_GetTime() < (Cur_Time + 1))
            ;
        #endif
        
        DM_Wait(1);

        #ifdef HAVE_TIME_TEST
        long_time = hal_timRTC_GetTime()/256;
        #endif 
        
		if (hal_gpio_GetVal(ON_OFF_BUTTON) != 0)
		{
			PD_Trig = TRUE;
		//	hal_SetUsrVectorMsk(HAL_GPIO_IRQ, HAL_GPIO_FALL(ON_OFF_BUTTON));            

            #if 0 
        	ev.nEventId = EV_KEY_DOWN;
            ev.nParam1  = SK_POWER_OFF;
            ev.nParam2  = 0;
            ev.nParam3  = 0;   
            dm_SendKeyMessage(&ev);
            #else
        	ev.nEventId = EV_CSW_PRV_KEY;
            ev.nParam1  = MAKEUINT32(1, SK_POWER_OFF);
            ev.nParam2  = 0;
            ev.nParam3  = 0;
            BAL_SendTaskEvent(&ev, CSW_TASK_ID_SYS);
            #endif 
		} 
	}
	#endif
}
#endif



