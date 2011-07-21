////////////////////////////////////////////////////////////////////////////////
//        Copyright (C) 2002-2006, Coolsand Technologies, Inc.
//                       All Rights Reserved
// 
// This source code is property of Coolsand. The information contained in this
// file is confidential. Distribution, reproduction, as well as exploitation,
// or transmisison of any content of this file is not allowed except if
// expressly permitted.Infringements result in damage claims!
//
// FILENAME: LED.c
// 
// DESCRIPTION:
//   TODO... 
// 
// REVISION HISTORY:
//   NAME            DATE            REMAKS
//   TODO... 
//   dingjian        20070913      build
//
////////////////////////////////////////////////////////////////////////////////
#if defined (__MMI_RUNNING_LED__)
#include "led.h"
#include "cs_types.h"
#include "timerevents.h"
#include "mmi_trace.h"
#include "phonesetup.h"
#include "eventsgprot.h"

#include "pmd_m.h"

#define led_open(a)  pmd_SetLevel(a, 1)
#define led_close(a) pmd_SetLevel(a, 0)

PRIVATE CONST PMD_LEVEL_ID_T g_LED_RED = PMD_LEVEL_LED0;
PRIVATE CONST PMD_LEVEL_ID_T g_LED_GREEN = PMD_LEVEL_LED1;
PRIVATE CONST PMD_LEVEL_ID_T g_LED_BLUE = PMD_LEVEL_LED2;

BOOL g_sw_DI_LED_FLAG=FALSE;
#define LED_ALL_LIGHT_DELAY 500
#define LED_POWER_ON_DELAY 300
#define LED_POWER_OFF_DELAY 300

UINT8 g_Led_Cnt=0;
static UINT8 g_led_status=0;

//BOOL  g_phnset_cntx_p->curEnableRollLedStatus=TRUE;        // Using g_phnset_cntx_p->curEnableRollLedStatus to set the LED status

VOID set_led_alllight(VOID)
{
    led_open(g_LED_RED);    
    led_open(g_LED_GREEN);
    led_open(g_LED_BLUE);
}

VOID set_led_alldark(VOID)
{
    led_close(g_LED_RED);
    led_close(g_LED_GREEN);
    led_close(g_LED_BLUE);
}

void showRunningLED(void)
{
    if(g_phnset_cntx_p->curEnableRollLedStatus)
    {
        /*if(g_Led_Cnt==2)*/
        /*{*/
            /*g_Led_Cnt=0;*/
            /*mmi_trace(1,"KEN : 0");*/
        /*}*/
        /*else            */
        {
            switch(g_led_status)
            {
                case 0:
                    set_led_alldark();
                    g_led_status++;
                    mmi_trace(1,"KEN : 1");
                    break;
                case 1:
                    led_open(g_LED_BLUE);
                    g_led_status++;
                    mmi_trace(1,"KEN : 2");
                    break;
                case 2:
                    led_close(g_LED_BLUE);
                    led_open(g_LED_RED);
                    mmi_trace(1,"KEN : 3");
                    g_led_status++;
                    break;

                case 3:
                    led_close(g_LED_RED);
                    led_open(g_LED_GREEN);
                    g_led_status++;
                    mmi_trace(1,"KEN : 4");
                    break;

                case 4:
                    led_close(g_LED_GREEN);
                    led_open(g_LED_RED);
                    led_open(g_LED_BLUE);
                    g_led_status++;
                    mmi_trace(1,"KEN : 5");
                    break;
                case 5:
                    led_close(g_LED_RED);
                    led_open(g_LED_GREEN);
                    led_open(g_LED_BLUE);
                    mmi_trace(1,"KEN : 6");
                    g_led_status++;
                    break;
                case 6:
                    led_close(g_LED_BLUE);
                    led_close(g_LED_GREEN);
                    led_open(g_LED_RED);
                    mmi_trace(1,"KEN : 7");
                    g_led_status++;
                    break;
                case 7:
                    led_open(g_LED_BLUE);
                    led_open(g_LED_GREEN);
                    g_led_status=0;
                    mmi_trace(1,"KEN : 8");
                    g_Led_Cnt++;
                    break;                
            }
        }
    }
}

void showPowerOnLED(void)
{
     if(g_phnset_cntx_p->curEnableRollLedStatus)
     {
        if(g_Led_Cnt==2)
        {
            StopTimer(POWER_ON_OFF_LED_TIMER);
            g_Led_Cnt=0;
            mmi_trace(1,"KEN : 29");
        }
        else
        {
            StartTimer(POWER_ON_OFF_LED_TIMER, LED_POWER_ON_DELAY, showPowerOnLED );
            showRunningLED();
            mmi_trace(1,"KEN : 20");
        }
     }
}
void showPowerOffLED(void)
{
     if(g_phnset_cntx_p->curEnableRollLedStatus)
     {
        if(g_Led_Cnt==2)
        {
            StopTimer(POWER_ON_OFF_LED_TIMER);
            g_Led_Cnt = 0;
            mmi_trace(1,"KEN : 39");
        }
        else
        {
            StartTimer(POWER_ON_OFF_LED_TIMER, LED_POWER_OFF_DELAY, showPowerOffLED );
            showRunningLED();
            mmi_trace(1,"KEN : 30");
        }
     }
}
void StopPowerOnOffLED(void)
{
    StopTimer(POWER_ON_OFF_LED_TIMER);
    g_Led_Cnt = 0;
}

void showMusicRunningLED(void)
{
     if(g_phnset_cntx_p->curEnableRollLedStatus)
     {
        StartTimer(POWER_ON_OFF_LED_TIMER, LED_POWER_OFF_DELAY, showMusicRunningLED);        
        showRunningLED();
     }
}

void stopMusicRunningLED(void)
{
    g_Led_Cnt = 0;
    StopTimer(POWER_ON_OFF_LED_TIMER);
}

void showKeyPadLED(void)
{
    static UINT8 status=0;
      if(g_phnset_cntx_p->curEnableRollLedStatus)
     {
            if(status==0)
            {
                led_close(g_LED_RED);
                led_close(g_LED_GREEN);
                led_open(g_LED_BLUE);
                status=1;
                mmi_trace(1,"KEN: status=0");
            }
            else if (status==1)
            {
                led_open(g_LED_RED);
                led_close(g_LED_GREEN);
                led_close(g_LED_BLUE);
                status=2;
                mmi_trace(1,"KEN: status=1");
            }
            else
            {   
                led_close(g_LED_RED);
                led_open(g_LED_GREEN);
                led_close(g_LED_BLUE);
                status=0;
                mmi_trace(1,"KEN: status=2");
            }
      }
}

void stopKeyPadLED(void)
{
    set_led_alldark();
}

#endif

