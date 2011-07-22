#include <cswtype.h>
#include <base.h>
#include "chip_id.h"
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
#include <hal.h>
#else
#include "hal_timers.h"
// Modem2G's class RTC handler don't have any parameter
#define alarm_isr(a)    alarm_isr()
#endif
#include <cos.h>
#include <base_prv.h>
#include <dm_prv.h>

PRIVATE void alarm_isr(UINT16 status);

#define DM_DEVID_KEYPAD 0 
#define DM_DEVID_ALRAM  1
#define DM_DEVID_XX
BOOL g_KeyBoard_Init_Status = FALSE ;
extern BOOL COS_SendSysEvent(
  COS_EVENT* pEvent,
  UINT16 nOption
);
extern VOID DM_InitToneOperationDoneSem(VOID);

BOOL DM_KeyboardInit()
{
    if(g_KeyBoard_Init_Status)
        return TRUE ;
    g_KeyBoard_Init_Status = TRUE ;
    dm_PowerdownInit();
    
    //
    // Key Init 
    //
    dm_KeyInit();
    return TRUE ;
}

BOOL dm_Init() 
{
    // 
    // Alarm Init
    // 
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	hal_SetUsrVectorMsk( HAL_RTCTIMER_IRQ, HAL_TIM_IRQ_ENABLE );
	hal_SetUsrVector( HAL_RTCTIMER_IRQ, alarm_isr );
#else
    hal_TimRtcIrqSetMask(TRUE);
    hal_TimRtcAlarmIrqSetHandler(alarm_isr);
#endif
    DM_KeyboardInit() ;
    DM_InitToneOperationDoneSem();

    return TRUE;
}

PRIVATE void alarm_isr(UINT16 status)
{
    COS_EVENT ev;
    
#if (CHIP_ASIC_ID == CHIP_ASIC_ID_JADE)
	hal_timRTC_AlarmClearStatus();
#else
    // Cleared by the IRQ handler in Modem2G
#endif
    
    ev.nEventId = EV_TIM_ALRAM_IND; 
    ev.nParam1  = 0; //hal_timRTC_GetTime();
    ev.nParam2  = 0;
    ev.nParam3  = 0;
    COS_SendSysEvent(&ev, (UINT16)0);
}

BOOL dm_SendToDefaultMMITask(COS_EVENT* pev)
{
    return COS_SendEvent(COS_GetDefaultMmiTaskHandle(), pev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);
}

