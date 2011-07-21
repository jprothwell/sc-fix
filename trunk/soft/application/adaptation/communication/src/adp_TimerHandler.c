/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_TimerHandler.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*     MMI Timer Handler on CSW platform.                                  */
 
/******************************************************************************/
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"

 
#include "wgui_categories_inputs.h"
#include "imeres.h"
 
#include "globalconstants.h"
#include "taskinit.h"
#include "mmitaskprot.h"
#include "thrdosinterface.h"
#include "globalstruct.h"
#include "debuginitdef.h"
#include "protocolevents.h"
#include "asynceventsgprot.h"
#include "alarmframeworkprot.h"    
#include "callmanagementgprot.h"
#include "callmanagementstruct.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "service.h"
#include "satgprots.h"
#include "cphsprotocolhandlers.h"
#include "engineermodeprot.h"
#include "factorymodeprot.h"
#include "gpioinc.h"
#include "phonebookgprot.h"
#include "callhistorymain.h"
#include "funandgamesprots.h"
#include "messagesresourcedata.h"
#include "organizergprot.h"
#include "alarmgprot.h"
#include "scheduledpoweronoffgprot.h"
#include "unicodedcl.h"
#include "unicodedef.h"
#include "organizergprot.h"
#include "idleappprot.h"
#include "conversions.h"
#include "nvramprot.h"
 
#include <csw.h>
#include "mmi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "adp_timer.h"
#include "mmitask.h"
 
#ifdef __MMI_THEMES_V2_SUPPORT__
#include "thememanager.h"
#endif
#include "messagesexdcl.h"

#ifdef __MMI_USB_SUPPORT__
#include "usbdevicegprot.h"
   #ifdef MMI_ON_HARDWARE_P
   extern kal_bool INT_USBBoot(void);
   #endif
#endif

#ifdef __MMI_IRDA_SUPPORT__
#include "irdammigprots.h"
#endif


#ifdef __MMI_BT_SUPPORT__
#include "btmmiscrgprots.h"
#endif

#ifdef __MMI_FILE_MANAGER__
#include "filemanagergprot.h"
#include "msdc_def.h"
#endif

#ifdef __MMI_WEBCAM__
#include "mdi_datatype.h"
#include "mdi_webcam.h"
#endif /* __MMI_WEBCAM__ */

#include "soundeffect.h"

#include "shortcutsprots.h"
#include "sublcdhistorygprot.h"
#include "todolistdef.h"

#include "wpssprotos.h"
#include "calleridgprot.h"
#if defined(__MMI_STOPWATCH__)
#include "stopwatchgprot.h"
#endif
#if defined(__MMI_EMAIL__)		
#include "emailappgprot.h"
#endif // def __MMI_EMAIL__	
#include "osthreadinterfacegprot.h"


#include "eventsgprot.h"
#include "allappgprot.h"
#include "wrappergprot.h"
#include "queuegprot.h"

#include "keybrd.h"
#include "nvramexdcl.h"
#include "simdetectiongprot.h"
#include "wgui_status_icons.h"
#include "poweronchargerprot.h"
#include "settinggprots.h"
#include "simdetectiongexdcl.h"
#include "phonesetupgprots.h" /* phnsetreadnvramcalibrationdata */
#if defined(__MMI_SMART_MESSAGE_MO__) && !defined(__MMI_MESSAGES_EMS__)
/* under construction !*/
#endif /*__MMI_SMART_MESSAGE_MO__ && !__MMI_MESSAGES_EMS__*/

 
#ifdef __MMI_POC__
#include "pocgprot.h"
#endif


#undef __NEWSIMULATOR


#ifdef MMI_ON_HARDWARE_P
#include "lcd_sw_inc.h"
#include "sublcd.h"
#else
#include "lcd_if.h"
#endif
#include "composeringtoneprots.h"

#ifdef __MMI_IMPS__
#include "mmi_imps_gprot.h"
#endif

/* 
**   Extern Varibales 
*/
FuncPtr currTimerFuncPtrs[MAX_TIMERS];	


/* 
**   Local Variables 
*/
typedef struct _TIMERTABLE_T
{
	/* store the timer_id. MSB(Most Significant Bit) is align_timer_mask */
	U16					timer_id;
	/* store the timer duration */
	U32				TimeDuration;
	/* store the timer_expiry_func */
	FuncPtr	callback_func;
        
	/* point to the next TIMERTABLE data */
	struct _TIMERTABLE_T	*next;
} TIMERTABLE_T;

typedef struct _TIMER_SCHED_T
{
    /* timer table */
    TIMERTABLE_T *pTable;
    /* scheduler's suspend flag */
     BOOL       is_suspend;   

}TIMER_SCHED_T;


PRIVATE TIMER_SCHED_T timer_sched = 
    {
#ifdef MMI_ON_HARDWARE_P
    .pTable =NULL,
    .is_suspend = FALSE
#else
    /* Modify by zhuoxz, 2009-7-29 */
    NULL,
    FALSE
#endif /*MMI_ON_HARDWARE_P*/
    };
    
void SuspendAllNoAlignedCosTimer()
{
    timer_sched.is_suspend = TRUE;
}


void ResumeAllNoAlignedCosTimer()
{
    TIMERTABLE_T *pTable = timer_sched.pTable;
    TIMERTABLE_T *pTablePrev;    
    timer_sched.is_suspend = FALSE;
    
    while(pTable != NULL){

 	 SetTimerHandler(pTable->timer_id, pTable->callback_func);	
	 COS_SetTimerEX(GetMmiTaskHandle(MOD_MMI), pTable->timer_id, COS_TIMER_MODE_SINGLE, pTable->TimeDuration);       
//        mmi_trace(0, "ResumeAllNoAlignedCosTimer:nTimerId=%d", pTable->timer_id);             
        pTablePrev = pTable;
        pTable = pTable->next;
        OslMfree(pTablePrev);
  
     }
    
    timer_sched.pTable = NULL;
    
}

void new_timersched(U16 nTimerId,U32 nTimeDuration,FuncPtr TimerExpiryFunction)
{

    TIMERTABLE_T *pTable = timer_sched.pTable;

    if(pTable == NULL){
        
        timer_sched.pTable = OslMalloc(sizeof(TIMERTABLE_T));
        pTable = timer_sched.pTable;
        
    }else{
    
            do{
                if(pTable->next == NULL){
                    pTable->next = OslMalloc(sizeof(TIMERTABLE_T));
                    pTable = pTable->next;
                    break;
                }
                pTable = pTable->next;
            
            }while(pTable != NULL);
            
    } 
    
    pTable->timer_id = nTimerId;
    pTable->TimeDuration = nTimeDuration;
    pTable->callback_func = TimerExpiryFunction;
    pTable->next = NULL;
        
   // mmi_trace(0, "new_timersched:nTimerId=%d", nTimerId);    
    
}

void cancel_timersched(U16 nTimerId)
{
    TIMERTABLE_T *pTable = timer_sched.pTable;
    TIMERTABLE_T *pTablePrev = timer_sched.pTable;

	if(pTable == NULL)
		return;

    while((pTable->timer_id != nTimerId) && (pTable->next != NULL)){
		pTablePrev = pTable;
		pTable = pTable->next;
	}

	if(pTable ->timer_id != nTimerId){
		return;
	}
    
	if(pTablePrev == pTable){ //first node
		timer_sched.pTable = timer_sched.pTable->next;
	}else{
		pTablePrev->next = pTable->next;
	}
	
	OslMfree(pTable);

}


void StartCosTimerInt(U16 nTimerId,U32 nTimeDuration,oslTimerFuncPtr TimerExpiryFunction, U8 alignment)
{
	ASSERT(nTimeDuration != 0);
	nTimeDuration = nTimeDuration * 16;//1ms = 16.384tick
//	mmi_trace(0, "StartCosTimerInt:nTimerId=%d,alignment=%d", nTimerId, alignment);    

       if((timer_sched.is_suspend == TRUE) && (alignment == TIMER_IS_NO_ALIGNMENT)){
           new_timersched(nTimerId,nTimeDuration,(FuncPtr)TimerExpiryFunction);
           return;  
        }
       
	 SetTimerHandler(nTimerId,(FuncPtr)TimerExpiryFunction);	
	 COS_SetTimerEX(GetMmiTaskHandle(MOD_MMI), nTimerId, COS_TIMER_MODE_SINGLE, nTimeDuration);

}


void StopCosTimerInt(U16 nTimerId)
{
//	mmi_trace(0, "StopCosTimerInt:nTimerId=%d", nTimerId);    
       if(timer_sched.is_suspend == TRUE){
            cancel_timersched(nTimerId);
        }
	SetTimerHandler(nTimerId, (FuncPtr)NULL);	
	COS_KillTimerEX(GetMmiTaskHandle(MOD_MMI), nTimerId);
}



void SetTimerHandler(U16 TimerID,FuncPtr funcPtr)
{
	MMI_ASSERT(TimerID<MAX_TIMERS) ;
	currTimerFuncPtrs[TimerID] = funcPtr;
}

// TODO: 为何要删除以下代码?
//void ExecCurrTimerHandler(U16 TimerId)               //tangjie del 20060621
//qiff add timer id sync with current timer
U16 currTimerID = 0;
void ExecCurrTimerHandler(U16 TimerId)       //tangjie add 20060621
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	FuncPtr currFuncPtr = NULL;
	currTimerID = TimerId;
	currFuncPtr = currTimerFuncPtrs[TimerId];
	if(currFuncPtr)
		(*currFuncPtr)();
}


void Timer_Test(void)
{
	return;
}


