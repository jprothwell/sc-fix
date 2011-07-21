#include "mmi_features.h"
#include "wgui_categories.h"
#include "wgui_status_icons.h"
#include "globalconstants.h"    /* need MAX_SUB_MENUS */
#include "historygprot.h"
#include "javaprot.h"
#include "javadef.h"
#include "internal.h"
#include "custom_mmi_default_value.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "dual_sim.h"
#include "simdetectiongprot.h"
#include "simdetectiondef.h"
#include "jam_msg_handler.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif
#include "mmi_features_camera.h"
 #include "cameraresdefs.h"
 
#include "sxr_tim.h"
U8 g_mmi_java_aud_volume_level = 0xff;  /* 0~6 */
U8 g_mmi_java_is_vib_on = MMI_FALSE;        /* on-off */
U8 g_mmi_java_wap_prof_id = 0;
#ifdef __MMI_MULTI_SIM__
U8 g_mmi_java_sim_id = 0;
#endif
U8 g_mmi_java_storagepath = 0;

/*****************************************************************************
 * FUNCTION
 *  mmi_java_is_no_network_service
 * DESCRIPTION
 *  This function is to detect if network is available or not
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL mmi_java_is_no_network_service(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

#ifdef  __MMI_MULTI_SIM__
	if(MTPNP_AD_Get_UsableSide_Number() > 0)
    {
        return MMI_FALSE;
    }
    else
    {
        return MMI_TRUE;
    }
#else
    if (mmi_bootup_is_sim_valid() == MMI_TRUE &&
        mmi_bootup_get_active_flight_mode() == MMI_FALSE)
    {
        return MMI_FALSE;
    }
    else
    {
        return MMI_TRUE;
    }
#endif
}


/*****************************************************************************
 * FUNCTION
 *  mmi_java_store_setting
 * DESCRIPTION
 *  store java setting back to NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_store_setting(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    WriteValue(NVRAM_JAVA_SETTING_SOUND, &g_mmi_java_aud_volume_level, DS_BYTE, &error);
    WriteValue(NVRAM_JAVA_SETTING_VIB, &g_mmi_java_is_vib_on, DS_BYTE, &error);
#if 0//JiaShuo Delete for J2ME
#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    WriteValue(NVRAM_JAVA_SETTING_VK, &g_mmi_java_vk_permission, DS_BYTE, &error);
#endif 
    
#endif
#ifdef __MMI_MULTI_SIM__
    WriteValue(NVRAM_JAVA_SETTING_SIM_ID, &g_mmi_java_sim_id, DS_BYTE, &error);
#endif
#ifdef __NATIVE_JAVA__
    WriteValue(NVRAM_JAVA_SETTING_STORAGEPATH, &g_mmi_java_storagepath, DS_BYTE, &error);
  #endif
    WriteValue(NVRAM_JAVA_SETTING_PROF_ID, &g_mmi_java_wap_prof_id, DS_BYTE, &error);

}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_load_setting
 * DESCRIPTION
 *  load java setting from NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_load_setting(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ReadValue(NVRAM_JAVA_SETTING_SOUND, &g_mmi_java_aud_volume_level, DS_BYTE, &error);

    /* if not init, init as 4 */
    if (g_mmi_java_aud_volume_level == 0xff)
    {
        g_mmi_java_aud_volume_level = 4;
        WriteValue(NVRAM_JAVA_SETTING_SOUND, &g_mmi_java_aud_volume_level, DS_BYTE, &error);
    }

    ReadValue(NVRAM_JAVA_SETTING_VIB, &g_mmi_java_is_vib_on, DS_BYTE, &error);

    /* if not init, init as ON */
    if (g_mmi_java_is_vib_on == 0xff)
    {
        g_mmi_java_is_vib_on = TRUE;
        WriteValue(NVRAM_JAVA_SETTING_VIB, &g_mmi_java_is_vib_on, DS_BYTE, &error);
    }

#if 0//JiaShuo Delete for J2ME
#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    ReadValue(NVRAM_JAVA_SETTING_VK, &g_mmi_java_vk_permission, DS_BYTE, &error);

    /* if not init, init as first wap profile */
    if (g_mmi_java_vk_permission == 0xff)
    {
        g_mmi_java_vk_permission = 0;
        WriteValue(NVRAM_JAVA_SETTING_VK, &g_mmi_java_vk_permission, DS_BYTE, &error);
    }
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 

#endif
    ReadValue(NVRAM_JAVA_SETTING_PROF_ID, &g_mmi_java_wap_prof_id, DS_BYTE, &error);
    if (g_mmi_java_wap_prof_id == 0xff)
    {
        g_mmi_java_wap_prof_id = 0;
        WriteValue(NVRAM_JAVA_SETTING_PROF_ID, &g_mmi_java_wap_prof_id, DS_BYTE, &error);
    }
#ifdef __MMI_MULTI_SIM__
    ReadValue(NVRAM_JAVA_SETTING_SIM_ID, &g_mmi_java_sim_id, DS_BYTE, &error);

    if (g_mmi_java_sim_id == 0xff)
    {
        g_mmi_java_sim_id = MTPNP_AD_GET_UsableSide_Index();
    }
    else
    {
        if(!MTPNP_PFAL_Is_Card_Usable(g_mmi_java_sim_id))
        {
            g_mmi_java_sim_id = MTPNP_AD_GET_UsableSide_Index();
        }
    }
    
    WriteValue(NVRAM_JAVA_SETTING_SIM_ID, &g_mmi_java_sim_id, DS_BYTE, &error);
   gcj_TraceOut(0,"mmi_java_load_setting g_mmi_java_sim_id=%d\n",g_mmi_java_sim_id);
   	
#endif

#ifdef __NATIVE_JAVA__

    ReadValue(NVRAM_JAVA_SETTING_STORAGEPATH, &g_mmi_java_storagepath, DS_BYTE, &error);
  if(g_mmi_java_storagepath == 0xff)
 {
	g_mmi_java_storagepath = 0;
	WriteValue(NVRAM_JAVA_SETTING_STORAGEPATH, &g_mmi_java_storagepath, DS_BYTE, &error);
  }
#endif

}

kal_bool jam_usb_mode_check(void)
{
    return gcj_IsJavaOn();
}

void jam_enter_usb_mode(void)
{
    javanotify_stop();
    COS_Sleep(1000);
	HideStatusIcon(STATUS_ICON_JAVA_PAUSE);
	UpdateStatusIcons();
}


extern void javanotify_install_midlet_wparams(const char* httpUrl,
                                       int silentInstall, int forceUpdate);

unsigned char EventInputMbox=0;
unsigned char QueueEventInputMbox=0;
unsigned char QueueEventInputTaskId=0;
unsigned char QueueEventTaskInEventApi=0;

unsigned char getTaskMboxId(unsigned char taskId);
void CreatGcjEventUtilsTask();
void DestoryGcjEventUtilsTask();


EVNET_ACTION_T gcjEventQueue;
void event_action_init()
{
	EventInputMbox = 0;
	memset(&gcjEventQueue,0,sizeof(gcjEventQueue));
}

void flash_keyQueue()
{
	if (EventInputMbox)
	{	
		if ( !sxr_SbxHot(EventInputMbox))
			gcj_StubCondNotify(EventInputMbox);
	}
}

void gcj_StubNativeEventHandleInit()
{
	unsigned char currentTaskId= sxr_GetCurrentTaskId();
	EventInputMbox = getTaskMboxId(currentTaskId);
	CreatGcjEventUtilsTask();
	gcj_TraceOut(0,"gcj_StubNativeEventHandleInit currentTaskId=%d,EventInputMbox=%d",currentTaskId,EventInputMbox);
}


void gcj_StubNativeQueueEventHandleInit()
{
	unsigned char currentTaskId= sxr_GetCurrentTaskId();
	QueueEventInputTaskId = currentTaskId;
	QueueEventInputMbox = getTaskMboxId(currentTaskId);
	gcj_TraceOut(0,"gcj_StubNativeQueueEventHandleInit currentTaskId=%d,QueueEventInputMbox=%d,",currentTaskId,EventInputMbox);
}
unsigned char gcj_IsEventQueueEmpty()
{
	unsigned char ret=0;
	int i;
	int status = sxr_EnterSc();
	int pushIndex = gcjEventQueue.pushIndex;
	int popIndex = gcjEventQueue.popIndex;
	if (pushIndex == popIndex && gcjEventQueue.isFull == 0 )
	{
		sxr_ExitSc(status);
		return 1;
	}
	else
	{
		sxr_ExitSc(status);
		return 0;
	}
	sxr_ExitSc(status);
	return ret;
}

void gcj_StubFlushEventQueue()
{
	if (EventInputMbox)
	{	
		if (!gcj_IsEventQueueEmpty()  && !sxr_SbxHot(EventInputMbox))
			gcj_StubCondNotify(EventInputMbox);
	}
}

unsigned char event_isJavaInProcessing=0;

void gcj_StubBlockEventHandle(unsigned char isBlock)
{
	event_isJavaInProcessing = isBlock;
}

unsigned int event_action_numItem=0;
unsigned int event_action_count=0;
#define RETRY	-1
#define PENDING	-2


static char event_action_push_internal(EVENT_HANDLER_T data)
{
	if (EventInputMbox == 0)
	{
		return 0;
	}
	if (gcj_StubIsForceToQuit()  || !gcj_IsJavaOn() || gcj_StubIsPrepareToQuit())
		return 0;
	int status = sxr_EnterSc();
	if (gcjEventQueue.inProcessing)// || event_isJavaInProcessing)
	{
		sxr_ExitSc(status);
		return RETRY;
	}
	if (gcjEventQueue.isFull)
	{
		/*
		if (event_isJavaInProcessing)
		{
			sxr_ExitSc(status);
			return RETRY;
		}
		else
		*/
		{
			sxr_ExitSc(status);
			//flash_keyQueue();
			if ( !sxr_SbxHot(EventInputMbox))
				gcj_StubCondNotify(EventInputMbox);
			return RETRY;
		}
	}
	int pushIndex = gcjEventQueue.pushIndex;
	int popIndex = gcjEventQueue.popIndex;
	gcjEventQueue.data[pushIndex++] = data;
	pushIndex %=MAX_ACTION_STONE;
	if (pushIndex == popIndex)
		gcjEventQueue.isFull = 1;
	gcjEventQueue.pushIndex = pushIndex;
	sxr_ExitSc(status);
	//gcj_TraceOut(0,"event_action_push push index =%d\n",pushIndex);
	if ( !sxr_SbxHot(EventInputMbox))
		gcj_StubCondNotify(EventInputMbox);
	return 0;
}
int DEBUG_VALUE=0;
int isTraceEvent=0;
int EVENT_DEBUG_VALUE=0;
int VM_EVENT_DEBUG_VALUE=0;

#define MAX_PENDING_COUNT 20
EVENT_HANDLER_T pendingData[MAX_PENDING_COUNT];
unsigned char pendingCount=0;
unsigned char InRemovePendingEvent=0;
unsigned char CheckAndPendingEvent(EVENT_HANDLER_T data)
{
	if (QueueEventTaskInEventApi && !InRemovePendingEvent)
	{
	/*
		int count = pendingCount-1;
		while (count  >1 )
		{
			for(int i=0;i<count-1;i++)
			{
				if (pendingData[i].java_event_type == KEY_EVENT && pendingData[i+1].java_event_data == KEY_EVENT && 
					pendingData[i].java_event_data.keyEventData.keycode == pendingData[i+1].java_event_data.keyEventData.keycode &&
					pendingData[i].java_event_data.keyEventData.keytype == PRESSED && pendingData[i+1].java_event_data.keyEventData.keytype == RELEASED)
				{
					for (int j=i;j<count-2;j++)
						pendingData[j] = pendingData[j+2];
					pendingCount --;
					pendingCount--;
				}
				 count = pendingCount-1;
				 break;
			}
		}
		*/
		if (data.java_event_type ==KEY_EVENT || data.java_event_type == PEN_EVENT)
			return TRUE;
		pendingData[pendingCount++] = data;
		if (pendingCount>MAX_PENDING_COUNT)
			abort();
		return TRUE;
	}	
	return FALSE;
}
unsigned char event_action_push(EVENT_HANDLER_T data);

unsigned char CheckAndRemoveAllPending()
{
	if (!QueueEventTaskInEventApi && pendingCount>0)
	{
		InRemovePendingEvent =1;
		unsigned int count=pendingCount-1;
		while (pendingCount--)
		{
			event_action_push(pendingData[count-pendingCount]);
			//pendingData[count-pendingCount] = 0;
		}
		InRemovePendingEvent=0;
	}
}

unsigned int debug_retryCount=0;

unsigned char event_action_push(EVENT_HANDLER_T data)
{
	unsigned char isCanLoss =0;
	unsigned int retryCount=0;
	UINT32 nCallerAdd;
	COS_GET_RA(&nCallerAdd);
	int ret=0;
	debug_retryCount = 0;
	//if ((data.java_event_type==KEY_EVENT&& data.java_event_data.keyEventData.keytype!= IME) || data.java_event_type == PEN_EVENT )
	//	isCanLoss=1;
	do  {
		
		debug_retryCount=retryCount;
		if (ret == RETRY)		
		{
			gcj_TraceOut(0,"event_action_push CALLER=0X%x,retryCount=%d,new event=%d,inProcessing=%d,pop event=%d,GC_isNeedToSuspend=%d,DEBUG_VALUE=%d,EVENT_VALUE=%d,VM_EVENT_VALUE=%d",nCallerAdd,retryCount,data.java_event_type,gcjEventQueue.inProcessing,gcjEventQueue.data[gcjEventQueue.popIndex].java_event_type,GC_isNeedToSuspend(),DEBUG_VALUE,EVENT_DEBUG_VALUE,VM_EVENT_DEBUG_VALUE);
			if ( !sxr_SbxHot(EventInputMbox))
				gcj_StubCondNotify(EventInputMbox);
			ret = event_action_push_internal(data);// &&( !isCanLoss || (isCanLoss && retryCount++ <3)))
			if (ret == RETRY)
				COS_Sleep(100);
			else
				return ret;
		}
		ret = event_action_push_internal(data);// &&( !isCanLoss || (isCanLoss && retryCount++ <3)))
		retryCount++;
		if (ret == RETRY && CheckAndPendingEvent(data))
		{
			gcj_TraceOut(1,"event_action_push pending pendingCount =%d\n",pendingCount);
			debug_retryCount = 0;
			return PENDING;
		}
		if (retryCount > 1000 && QueueEventInputTaskId!=0)
		{
			gcj_page_spy_task_stack(QueueEventInputTaskId);
		}
		if (retryCount>100)
		{
			if ( !sxr_SbxHot(QueueEventInputTaskId))
				gcj_StubCondNotify(QueueEventInputTaskId);
		}
		else if (retryCount>150)
		{			
			isTraceEvent=1;
		}
	}while (ret==RETRY);// && !(isCanLoss && retryCount>5));
	debug_retryCount=0;
	return ret;
}

int event_action_pop(void)
{
	int status = sxr_EnterSc();
	int pushIndex = gcjEventQueue.pushIndex;
	int popIndex = gcjEventQueue.popIndex;
	if (pushIndex == popIndex && gcjEventQueue.isFull == 0 )
	{
		sxr_ExitSc(status);
		return 0;
	}
	int ret;
	int *p=0;
	if (event_action_count == 0)	
		gcjEventQueue.inProcessing = 1;
	if (gcjEventQueue.data[popIndex].java_event_type == COMMAND_EVENT || gcjEventQueue.data[popIndex].java_event_type == SYSTEM_EVENT)
		event_action_numItem = 2; 
	else if (gcjEventQueue.data[popIndex].java_event_type == KEY_EVENT)
	{
		event_action_numItem = 3;
		if (gcjEventQueue.data[popIndex].java_event_data.keyEventData.keytype == IME)
			event_action_numItem = 2;
	}
	else if (gcjEventQueue.data[popIndex].java_event_type == PEN_EVENT)
	        event_action_numItem = 4;
	else if (gcjEventQueue.data[popIndex].java_event_type == MM_EOM_EVENT)
        	event_action_numItem = 3;
	else
		abort();
	p = &gcjEventQueue.data[popIndex];
	ret = p[event_action_count];
	if (event_action_count>0)
		p[event_action_count]=0;
	event_action_count++;
	if (event_action_count >=event_action_numItem)
	{
		++popIndex;
		gcjEventQueue.inProcessing = 0;
		event_action_count = 0;
		p[event_action_count]=0;
		popIndex %=MAX_ACTION_STONE;
		gcjEventQueue.isFull = 0;
		gcjEventQueue.popIndex = popIndex;
	}
	if (popIndex == pushIndex && gcjEventQueue.inProcessing &&gcjEventQueue.isFull == 0)
		abort();
	sxr_ExitSc(status);
	//gcj_TraceOut(0,"event_action_pop pop index =%d,ret =%d\n",popIndex,ret);
	return ret;
}

int gcj_StubEventsReadInt(void)
{
	return event_action_pop();
}

void command_press(void)
{
	gcj_TraceOut(0,"command_press\n");
	EVENT_HANDLER_T eventData;
	eventData.java_event_type = COMMAND_EVENT;
	eventData.java_event_data.commandEventData.commandtype = MENU_REQUESTED;
	event_action_push(eventData);
}


void menuEventCommand(commandStruct menu[],int index)
{
	EVENT_HANDLER_T menu_keyData;
	gcj_TraceOut(0,"menuEventCommand id=%d,index=%d\n",menu[index].id,index);
	//menu_keyData.java_event_type = COMMAND_EVENT;
	//menu_keyData.java_event_data.commandEventData.commandtype = MENU_DISMISSED ;
	//event_action_push(menu_keyData);
	menu_keyData.java_event_type = COMMAND_EVENT;
	menu_keyData.java_event_data.commandEventData.commandtype = menu[index].id ;
	event_action_push(menu_keyData);
}
/*****************************************************************************
 * FUNCTION
 *  jvm_keypad_collector
 * DESCRIPTION
 *  collect all key event, insert key event into pool buffer except
 *  END key
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 int testgcjkey=0;
extern unsigned char	inMenu;

 #if 1
void jvm_keypad_collector(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint16 Keycode, Keytype;
	EVENT_HANDLER_T eventData;
	eventData.java_event_type = KEY_EVENT;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GetkeyInfo(&Keycode, &Keytype);
/*
	switch (Keytype)
	{
		case KEY_EVENT_DOWN:
			eventData.java_event_data.keyEventData.keytype = PRESSED;
			break;
		case KEY_EVENT_UP:
			eventData.java_event_data.keyEventData.keytype = RELEASED;
			break;
		case KEY_REPEAT:
			eventData.java_event_data.keyEventData.keytype = REPEATED;
			break;
		case KEY_LONG_PRESS:
			eventData.java_event_data.keyEventData.keytype = PRESSED;
			break;
		case KEY_FULL_PRESS:
			eventData.java_event_data.keyEventData.keytype = PRESSED;
			break;
		default :
			eventData.java_event_data.keyEventData.keytype = PRESSED;
			break;
	}
	*/
	gcj_TraceOut(0,"jvm_keypad_collector keycode=%d,keytype=%d,buttonCommands[LEFT]=%d,buttonCommands[RIGHT]=%d",Keycode,Keytype,buttonCommands[BUTTON_LEFT],buttonCommands[BUTTON_RIGHT]);
	switch (Keycode)
	{
		case KEY_0:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM0;
			break;
		case KEY_1:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM1;
			break;
		case KEY_2:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM2;
			break;
		case KEY_3:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM3;
			break;
		case KEY_4:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM4;
			break;
		case KEY_5:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM5;
			break;
		case KEY_6:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM6;
			break;
		case KEY_7:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM7;
			break;
		case KEY_8:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM8;
			break;
		case KEY_9:
			eventData.java_event_data.keyEventData.keycode = KEY_NUM9;
			break;
		case KEY_LSK:
/*			if (Keytype == KEY_EVENT_UP && !inFullScreenMode && !popInMenu && !testgcjkey)
			{
				eventData.java_event_type = COMMAND_EVENT;
				eventData.java_event_data.commandEventData.commandtype = buttonCommands[BUTTON_LEFT];
			}
			else if (inFullScreenMode || popInMenu)
			{
			    eventData.java_event_data.keyEventData.keycode = KEY_LEFT_SOFT;
			    testgcjkey=1;
			}
			else if (Keytype == KEY_EVENT_UP && testgcjkey && !inFullScreenMode)
			{
			    eventData.java_event_data.keyEventData.keycode = KEY_LEFT_SOFT;
			    testgcjkey=0;
			}
			else
			{
			    return;
			}
*/			
			if (buttonCommands[BUTTON_LEFT] != BUTTON_NOT_SET)
			{
				eventData.java_event_type = COMMAND_EVENT;
				eventData.java_event_data.commandEventData.commandtype = buttonCommands[BUTTON_LEFT];
			}
			else
			    eventData.java_event_data.keyEventData.keycode = KEY_LEFT_SOFT;
			break;
		case KEY_RSK:
/*			if (Keytype == KEY_EVENT_UP && !inFullScreenMode && !popInMenu && !testgcjkey)
			{
				eventData.java_event_type = COMMAND_EVENT;
				eventData.java_event_data.commandEventData.commandtype = buttonCommands[BUTTON_RIGHT];
			}
			else if (inFullScreenMode || popInMenu)
			{
			   eventData.java_event_data.keyEventData.keycode = KEY_RIGHT_SOFT;
			   testgcjkey=1;
			}
			else if (Keytype == KEY_EVENT_UP && testgcjkey && !inFullScreenMode)
			{
			   eventData.java_event_data.keyEventData.keycode = KEY_RIGHT_SOFT;
			   testgcjkey=0;
			}
			else
			{
			return;
			}
*/			
			if (buttonCommands[BUTTON_RIGHT] != BUTTON_NOT_SET)
			{
				eventData.java_event_type = COMMAND_EVENT;
				eventData.java_event_data.commandEventData.commandtype = buttonCommands[BUTTON_RIGHT];
			}
			else
				eventData.java_event_data.keyEventData.keycode = KEY_RIGHT_SOFT;

			break;
		case KEY_CSK:
			if (set_InputDone &&  !inMenu && gcj_IsHavaCenterKey())
			{
				if (buttonCommands[BUTTON_CENTER] != BUTTON_NOT_SET)
				{
					eventData.java_event_type = COMMAND_EVENT;
					eventData.java_event_data.commandEventData.commandtype = buttonCommands[BUTTON_CENTER];
				}
				else
					eventData.java_event_data.keyEventData.keycode = FIRE;
			}
			else
				eventData.java_event_data.keyEventData.keycode = FIRE;
			break;
		case KEY_UP_ARROW:
			eventData.java_event_data.keyEventData.keycode = UP;
			break;
		case KEY_DOWN_ARROW:
			eventData.java_event_data.keyEventData.keycode = DOWN;
			break;
		case KEY_LEFT_ARROW:
			eventData.java_event_data.keyEventData.keycode = LEFT;
			break;
		case KEY_RIGHT_ARROW:
			eventData.java_event_data.keyEventData.keycode = RIGHT;
			break;
#ifdef __SENDKEY2_SUPPORT__
		case KEY_SEND1:
#else			
		case KEY_SEND:
#endif			
			if (set_InputDone && !gcj_IsHavaDelKey())
				eventData.java_event_data.keyEventData.keycode = KEY_CLEARJ;
			else
				eventData.java_event_data.keyEventData.keycode = SYSTEM_KEY_SEND;
			break;
		case KEY_END:
			//eventData.java_event_data.keyEventData.keycode = SYSTEM_KEY_END;
			//if (Keytype == KEY_EVENT_DOWN)
				javanotify_pause_confirm();
			return;
		case KEY_CLEAR:
			eventData.java_event_data.keyEventData.keycode = KEY_CLEARJ;
			break;
		case KEY_STAR:
			eventData.java_event_data.keyEventData.keycode = KEY_STARJ;
			break;
		case KEY_POUND:
			eventData.java_event_data.keyEventData.keycode = KEY_POUNDJ;
			break;
		case KEY_ENTER:
			if (set_InputDone &&  !inMenu && !gcj_IsHavaCenterKey())
			{
				if (buttonCommands[BUTTON_CENTER] != BUTTON_NOT_SET)
				{
					eventData.java_event_type = COMMAND_EVENT;
					eventData.java_event_data.commandEventData.commandtype = buttonCommands[BUTTON_CENTER];
				}
				else
					eventData.java_event_data.keyEventData.keycode = FIRE;
			}
			else
				eventData.java_event_data.keyEventData.keycode = FIRE;
			break;
		default :
			mmi_trace(1,"current Coskeycode = %d\n",Keycode);
			return;
			break;
	}
	int ret=0;
	if (eventData.java_event_type == KEY_EVENT)
	{
		if (popInMenu)
		{
			ret = popMenuKeyHandle(&eventData);
		}
		{
			eventData.java_event_data.keyEventData.keytype = PRESSED;
			event_action_push(eventData);
			eventData.java_event_data.keyEventData.keytype = RELEASED;
			event_action_push(eventData);
		}
	}
	else
	{
		event_action_push(eventData);
	}
}
 #endif

/*****************************************************************************
 * FUNCTION
 *  jvm_register_keypad
 * DESCRIPTION
 *  this function will register keypad handler to JVM_redirect_keyevent() function.
 * CALLS
 *  
 * PARAMETERS
 *  is_resume       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void jvm_register_keypad(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 keyCode = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearAllKeyHandler();
    clear_left_softkey();
    clear_right_softkey();
    register_default_hide_softkeys();

    for (keyCode = 0; keyCode < MAX_KEYS; keyCode++)
    {
    
        SetKeyHandler((FuncPtr) jvm_keypad_collector, keyCode, KEY_EVENT_UP);
        //SetKeyHandler((FuncPtr) jvm_keypad_collector, keyCode, KEY_EVENT_DOWN);
        //SetKeyHandler((FuncPtr) jvm_keypad_collector, keyCode, KEY_EVENT_REPEAT);
       // SetKeyHandler((FuncPtr) jvm_keypad_collector, keyCode, KEY_EVENT_LONG_PRESS);
        
    }
}

void jvm_unregister_keypad(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 keyCode = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearAllKeyHandler();
    clear_left_softkey();
    clear_right_softkey();
    register_default_hide_softkeys();
}

 
extern  FuncPtr currKeyFuncPtrs[MAX_KEYS][MAX_KEY_TYPE];		 /* available key func ptrs to current screen  */

void gcj_StopAllKeyHandle()
{
	 U16 keyCode = 0;
	 U16 keyType = 0;
	
	 for(keyCode = 0; keyCode < MAX_KEYS; keyCode++)
	 {
		 for(keyType = 0; keyType < MAX_KEY_TYPE; keyType++)
		 {
			 currKeyFuncPtrs[keyCode][keyType] = NULL;
		 }
	 }
}
 
#ifdef __MMI_TOUCH_SCREEN__
static void jvm_pen_down_hdlr(mmi_pen_point_struct pos)
{
    mmi_trace(1,"jvm_pen_down_hdlr x=%d y=%d",pos.x,pos.y);
    EVENT_HANDLER_T eventData;
    if(inFullScreenMode)
    {
    	eventData.java_event_type = PEN_EVENT;
        eventData.java_event_data.penEventData.keytype = PRESSED;
        eventData.java_event_data.penEventData.x = pos.x;
        eventData.java_event_data.penEventData.y = pos.y;
    }
    else
    {
        if (wgui_test_lsk_position(pos) || wgui_test_rsk_position(pos))
        {
            return;
        }
        else
        {
        	eventData.java_event_type = PEN_EVENT;
            eventData.java_event_data.penEventData.keytype = PRESSED;
            eventData.java_event_data.penEventData.x = pos.x;
            eventData.java_event_data.penEventData.y = pos.y;
        }
    }
    event_action_push(eventData);
}

static void jvm_pen_up_hdlr(mmi_pen_point_struct pos)
{
    mmi_trace(1,"jvm_pen_up_hdlr x=%d y=%d",pos.x,pos.y);
    EVENT_HANDLER_T eventData;
    if(inFullScreenMode)
    {
        eventData.java_event_type = PEN_EVENT;
        eventData.java_event_data.penEventData.keytype = RELEASED;
        eventData.java_event_data.penEventData.x = pos.x;
        eventData.java_event_data.penEventData.y = pos.y;
    }
    else
    {
        if(wgui_test_lsk_position(pos))
        {
            eventData.java_event_type = COMMAND_EVENT;
            eventData.java_event_data.commandEventData.commandtype = buttonCommands[BUTTON_LEFT];
        }
        else if(wgui_test_rsk_position(pos))
        {
            eventData.java_event_type = COMMAND_EVENT;
            eventData.java_event_data.commandEventData.commandtype = buttonCommands[BUTTON_RIGHT];
        }
        else
        {
        	eventData.java_event_type = PEN_EVENT;
            eventData.java_event_data.penEventData.keytype = RELEASED;
            eventData.java_event_data.penEventData.x = pos.x;
            eventData.java_event_data.penEventData.y = pos.y;
        }
    }
    event_action_push(eventData);
}

static void jvm_pen_move_hdlr(mmi_pen_point_struct pos)
{
    mmi_trace(1,"jvm_pen_move_hdlr x=%d y=%d",pos.x,pos.y);

    EVENT_HANDLER_T eventData;
    eventData.java_event_type = PEN_EVENT;
    eventData.java_event_data.penEventData.keytype = DRAGGED;
    eventData.java_event_data.penEventData.x = pos.x;
    eventData.java_event_data.penEventData.y = pos.y;
    event_action_push(eventData);
}


void jvm_register_pen_events(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //jvm_tp_flush();
    wgui_register_pen_down_handler(jvm_pen_down_hdlr);
    wgui_register_pen_up_handler(jvm_pen_up_hdlr);
    wgui_register_pen_move_handler(jvm_pen_move_hdlr);
    //wgui_register_pen_long_tap_handler(jvm_pen_long_tap_hdlr);
    //wgui_register_pen_repeat_handler(jvm_pen_repeat_hdlr);
    //wgui_register_pen_abort_handler(jvm_pen_abort_hdlr);
    //mmi_pen_register_stroke_down_handler(jvm_pen_stroke_down_hdlr);
    //mmi_pen_register_stroke_move_handler(jvm_pen_stroke_move_hdlr);
    //mmi_pen_register_stroke_up_handler(jvm_pen_stroke_up_hdlr);
}

void jvm_unregister_pen_events(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_register_general_pen_handler();
    /* wgui_register_general_pen_stroke_handler(); */
}

#else /* __MMI_TOUCH_SCREEN__ */ 
void jvm_register_pen_events(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

void jvm_unregister_pen_events(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}
#endif /* __MMI_TOUCH_SCREEN__ */ 

unsigned char *Current_Java_App_Name=NULL;

extern void gcj_init_java_dir();
void InitJAVA()
{
    SetProtocolEventHandler((PsFuncPtr) mmi_java_recv_show_alert_user_req, MSG_ID_MMI_JAVA_SHOW_ALERT_USER_DIALOG_REQ);
    mmi_java_load_setting();
    gcj_init_java_dir();
    mmi_java_store_setting();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_highlight_menu
 * DESCRIPTION
 *  Highlight handler routine for Java menu item in FunAndGames menu.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
/* void Highlight_JAVA_MENU(void) */
void mmi_native_java_qq_highlight_menu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Current_Java_App_Name ="qq";
    SetLeftSoftkeyFunction(EntryScrJavaMenu, KEY_EVENT_UP);
    SetKeyHandler(EntryScrJavaMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

void mmi_native_java_msn_highlight_menu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Current_Java_App_Name = "msn";	
    SetLeftSoftkeyFunction(EntryScrJavaMenu, KEY_EVENT_UP);
    SetKeyHandler(EntryScrJavaMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

/*****************************************************************************
 * FUNCTION
 *  EntryScrJavaMenu
 * DESCRIPTION
 *  Enter the Java menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
 
unsigned char javaIsInBackground=0;
extern void lanch_j2me(unsigned char *appName);
void javanotify_start(void)
{
    TASK_HANDLE * hTask;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_J2ME);
    gcj_TraceOut(0,"javanotify_start---nMailBoxId=%d------------------------------\n",hTask->nMailBoxId);
    gcj_StubCondNotify(hTask->nMailBoxId);
}
unsigned char gcj_paused_ok=0;
unsigned char gcj_resume_ok=0;
void javanotify_pause(void)
{
	hal_HstSendEvent(0xffffbbbb);

	gcj_TraceOut(0,"javanotify_pause::EventInputMbox=%d", EventInputMbox);
	if (!javaIsInBackground)
	{
		hal_HstSendEvent(0xffffbbcc);
		EVENT_HANDLER_T eventData;
		eventData.java_event_type = SYSTEM_EVENT;
		eventData.java_event_data.systemEventData.systemtype = SUSPEND_ALL;
		if (event_action_push(eventData) == 0)
		{
			while (!gcj_paused_ok && !(gcj_StubIsForceToQuit()  || !gcj_IsJavaOn()))
			{
				if ( !sxr_SbxHot(EventInputMbox))
					gcj_StubCondNotify(EventInputMbox);
				COS_Sleep(200);
			}
			gcj_paused_ok = 0;
			javaIsInBackground=1;
		}
	}
}

void javanotify_pause_confirm()
{
	gcj_TraceOut(0,"javanotify_pause_confirm::EventInputMbox=%d", EventInputMbox);
	if (EventInputMbox !=0)
		GoBackHistory();
}

extern int checkforinputwindow;
void javanotify_resume(void)
{
	gcj_TraceOut(0,"javanotify_resume::EventInputMbox=%d", EventInputMbox);
	if (javaIsInBackground)
	{
		gcj_image_reset();
		EVENT_HANDLER_T eventData;
		eventData.java_event_type = SYSTEM_EVENT;
		eventData.java_event_data.systemEventData.systemtype = RESUME_ALL;		
		if (event_action_push(eventData) == 0)
		{	
			javaIsInBackground=0;
			while (!gcj_resume_ok && !(gcj_StubIsForceToQuit()  || !gcj_IsJavaOn()))
			{
				if ( !sxr_SbxHot(EventInputMbox))
					gcj_StubCondNotify(EventInputMbox);
				COS_Sleep(200);
			}
			gcj_resume_ok = 0;
		}
	}
	//checkforinputwindow = 0;
}

void javanotify_stop(void)
{
	gcj_TraceOut(0,"javanotify_stop");

    EVENT_HANDLER_T eventData;
    eventData.java_event_type = SYSTEM_EVENT;
    eventData.java_event_data.systemEventData.systemtype = SHUTDOWN;
    event_action_push(eventData);
    COS_Sleep(100);
    javaIsInBackground=0;
}

unsigned char gcj_IsJavaPause()
{
	return javaIsInBackground;
}

void gcj_StubExitJava()
{
	COS_EVENT evt;
	evt.nEventId = (unsigned int)MSG_ID_MMI_J2ME_EXIT_SCREEN_REQ;
	gcj_StubSendEvent(&evt,MOD_MMI);
}

HANDLE GcjEventUtilsTaskHandle=NULL;

void GcjEventUtilsTask(void *data)
{
	while (1)
	{
		gcj_StubFlushEventQueue();
		COS_Sleep(100);
	}
}


void CreatGcjEventUtilsTask()
{
/*
	GcjEventUtilsTaskHandle = COS_CreateTask(GcjEventUtilsTask, 
            NULL, NULL,
            512, 
            240, 
            COS_CREATE_DEFAULT, 0, "GcjEventUtils Task"); 
*/            
}

void DestoryGcjEventUtilsTask()
{
	//COS_DeleteTask(GcjEventUtilsTaskHandle);
}
extern unsigned char gcj_IsForceToQuit;

void JavaMainTask(void)
{
	
    TASK_HANDLE * hTask;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_J2ME);
    while (1)
    {
	gcj_TraceOut(0,"JavaMainTask---nMailBoxId=%d------------------------------\n",hTask->nMailBoxId);
	gcj_StubCondWait(hTask->nMailBoxId,0);
	InitJAVA();
//        sxr_GcjInitSignalHandle();	
//        sxr_GcjSetBaseSbx(hTask->nMailBoxId);
	lanch_j2me(Current_Java_App_Name);
	mdi_audio_resume_background_play();
	gcj_StubExitJava();
	gcj_TraceOut(0,"sxr_CheckStackUsage task is =%d, usage is %d\n",hTask->nTaskId,sxr_CheckStackUsage(hTask->nTaskId));
    }
}

gdi_handle	java_baseLayer;
gdi_handle 	java_secondLayer;
extern unsigned char gcj_IsReturnFormError;
#ifdef JATAAYU_SUPPORT
#ifdef JATAAYU_SUPPORT_MMS 
	extern INT8 jMMS_IsMMSAppOrDownloadActive(void);
#endif
#ifdef JATAAYU_SUPPORT_WAP
extern  INT8 jMMS_IsWapConnected (void);
#endif
#endif

unsigned char entryScrJavaEnd=0;
void EntryScrJavaMenu(void)
{
	U8 *guiBuffer;
	U16 activeScrId,tmp;
	if (gcj_IsForceToQuit)
	{
		gcj_IsForceToQuit = 0;
		return;
	}
	if (Current_Java_App_Name == NULL)
		return;
	activeScrId = GetActiveScreenId();
	tmp = SCR_JAVA;
	gcj_TraceOut(0,"EntryScrJavaMenu---activeScrId=%d,tmp=%d,javaIsInBackground=%d-------------------------------\n",activeScrId,tmp,javaIsInBackground);
	if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}
	mmi_java_load_setting();
	if (gcj_CheckDir() == -1)
		return;
	mdi_audio_suspend_background_play();
#ifdef __FLIGHT_MODE_SUPPORT__
	if (mmi_bootup_get_active_flight_mode() != 0)
	{
		/* prevent from entry BT main menu from shortcut */
		DisplayPopup(
			(U8*) GetString(STR_MENU_FLIGHT_MODE),
			IMG_GLOBAL_ERROR,
			0,
			UI_POPUP_NOTIFYDURATION_TIME,
			(U8) WARNING_TONE);
		return;
	}
#endif /* __FLIGHT_MODE_SUPPORT__ */

	if(!HasValidSIM())
	{
		DisplayPopup((U8*)GetString(STR_SIM_ACCESS_ERROR_MSG),IMG_GLOBAL_WARNING,0,2000,(U8)ERROR_TONE);
		return;
	}

 #ifdef JATAAYU_SUPPORT
	if(
	#ifdef JATAAYU_SUPPORT_MMS
		jMMS_IsMMSAppOrDownloadActive()
	#endif	
	#ifdef JATAAYU_SUPPORT_WAP
	#ifdef JATAAYU_SUPPORT_MMS
		||
	#endif
		jMMS_IsWapConnected()
	#endif
		)
	{
		DisplayPopup((PU8)GetString(STR_ID_CAMERA_NOTIFY_GPRS_ACTIVE) ,
			IMG_GLOBAL_PROGRESS, UI_POPUP_NOTIFYDURATION_TIME,
			UI_POPUP_NOTIFYDURATION_TIME,
			WARNING_TONE);
				return;
	}
     #endif
	
	if(gcj_InitMemHeap() == FALSE)
	{
		DisplayPopup(
		(U8*) GetString(STR_JAVA_ERROR_NO_MORE_MEMORY),
		IMG_GLOBAL_ERROR,
		0,
		UI_POPUP_NOTIFYDURATION_TIME,
		(U8) WARNING_TONE);
	    
	    return;
	}

	EntryNewScreen(SCR_JAVA, ExitScrJavaMenu, EntryScrJavaMenu, NULL);
	guiBuffer = GetCurrGuiBuffer(SCR_JAVA);
/*
	if(guiBuffer != NULL)
	{
	   	if ( !gcj_IsJavaOn())
   		{
	   		gcj_DeInitMemHeap();
		      	GoBackHistory();
			mdi_audio_resume_background_play();	
			return;  
   		}
	}
	*/
	gcj_CheckDelKey();
	gcj_CheckCenterKey();
	entry_full_screen();
	HideStatusIcon(STATUS_ICON_JAVA_PAUSE);
	ShowStatusIcon(STATUS_ICON_JAVA_PAUSE);
	UpdateStatusIcons();
	gdi_layer_reset_clip();
	gdi_layer_reset_text_clip();
	gdi_layer_multi_layer_enable();
	gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	gdi_layer_get_base_handle(&java_baseLayer);
	gdi_layer_create(0, 0, LCD_WIDTH, LCD_HEIGHT, &java_secondLayer);
	if (java_secondLayer == NULL)
		abort();
	jvm_register_keypad();
        jvm_register_pen_events();
	gcj_TraceOut(0,"EntryScrJavaMen2u---activeScrId=%d,SCR_JAVA=%d,javaIsInBackground=%d-------------------------------\n",activeScrId,SCR_JAVA,javaIsInBackground);
	//if ( activeScrId != (SCR_JAVA+1) && activeScrId != (SCR_JAVA+2))
	if (!gcj_StubIsPrepareToQuit())
	{
		if (!javaIsInBackground)
		{
			javanotify_start();
			while(!EventInputMbox)
			{
				COS_Sleep(50);
			}
			
//			COS_Sleep(500);
		}
		else
		{
			javanotify_resume();
		}
	}
	entryScrJavaEnd =1;
	gcj_TraceOut(0,"EntryScrJavaMenu end");
}


backGroundAction gcj_backGroundAction=NULL;


void gcj_RegisterBackGroundAction(backGroundAction f)
{
	gcj_backGroundAction = f;
}
void gcj_ResetBackGroundAction()
{
	gcj_backGroundAction=NULL;
}

void gcj_StubJavaEntryBackground(void)
{
	gcj_paused_ok = 1;
	gcj_TraceOut(0,"j2me:current task id is %d, file %s,function %s\n",sxr_GetCurrentTaskId(),__FILE__,__FUNCTION__);	
	if (gcj_backGroundAction)
	{
		gcj_backGroundAction();
		gcj_ResetBackGroundAction();
	}
	else
	{
		//GoBackHistory();
	}
}

void gcj_StubJavaExitBackground(void)
{
	gcj_resume_ok = 1;
	gcj_TraceOut(0,"j2me:current task id is %d, file %s,function %s,set_InputDone=%d\n",sxr_GetCurrentTaskId(),__FILE__,__FUNCTION__,set_InputDone);	
}




/*****************************************************************************
 * FUNCTION
 *  ExitScrJavaMenu
 * DESCRIPTION
 *  Main Menu->FunAndGames->Java Exit Function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitScrJavaMenu(void)
{
	U16 activeScrId;
	activeScrId = GetActiveScreenId();
	hal_HstSendEvent(0xffffaaaa);
	gcj_TraceOut(0,"ExitScrJavaMenu::javaentry=%d\n", gcj_IsJavaOn());
	jvm_unregister_keypad();
    	jvm_unregister_pen_events();
	//javanotify_shutdown();
	if ( gcj_IsJavaOn() && !gcj_StubIsPrepareToQuit())// && activeScrId != (SCR_JAVA+1) && activeScrId != (SCR_JAVA+2))
		javanotify_pause();
	if ( java_secondLayer)
	{
		gdi_layer_free(java_secondLayer);
		java_secondLayer=0;
	}	
	entryScrJavaEnd = 0;
}

void j2me_enter_idle_screen_notify(void)
{
}

BOOL java_is_launch_after_install(void)
{
	return FALSE;
}

void mmi_java_send_mid_start_req(void)
{
	EntryNewScreen(SCR_JAVA_APP, NULL, NULL, NULL);
	entry_full_screen();

}

void j2me_enter_incoming_call_notify(void)
{
}

void EntryScrJavaEM(void)
{
}


void mmi_java_local_install_content_routing(kal_uint8 *file_path)
{
/*
	S8 unamepath[512];
	S8 namepath[300]="file:///";
	U8 *guiBuffer;
	
	guiBuffer = GetCurrGuiBuffer(SCR_JAVA);
	
	EntryNewScreen(SCR_JAVA, ExitScrJavaMenu, NULL, NULL);
	entry_full_screen();
	
	jvm_register_keypad();
	//asm("break 1");
	UnicodeToAnsii(unamepath, file_path);
	strcat(namepath,unamepath);
	mmi_trace(1,"mmi_java_local_install_content_routing file_path = %s \n",namepath);
	javanotify_install_midlet_wparams(namepath,1,0);
	*/
}

void gcj_interface_reset()
{
	event_action_init();
	DestoryGcjEventUtilsTask();
	gcj_ResetBackGroundAction();
	//sxr_StopFunctionTimer(flash_keyQueue);
	javaIsInBackground = 0;
	if (java_secondLayer)
		gdi_layer_free(java_secondLayer);
	java_secondLayer=0;
	
}
int AvoidFlashingWhiteFlag = 0;

void gcj_StubQuitButtonPressed()
{
    	AvoidFlashingWhiteFlag = 1;
	gcj_StopAllKeyHandle();
}
volatile unsigned char GotalertToQuit=0;

void j2me_send_alertuser(char *message, int len)
{
    if (gcj_StubIsForceToQuit()  || !gcj_IsJavaOn())
    {
     return;
    }

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ilm_struct *ilm_ptr;
    mmi_java_show_alert_user_dialog_req_struct *msg;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg = (mmi_java_show_alert_user_dialog_req_struct*)
        construct_local_para((kal_uint16) (sizeof(mmi_java_show_alert_user_dialog_req_struct) + len * 2), 0);
    if (msg ==NULL)
    {
	    if (gcj_StubIsPrepareToQuit())
	    {
	    	GotalertToQuit = 1;
		entryScrJavaEnd = 1;	
	    }
	    return ;
    }
    UCS2Strcpy((S8*) msg->message, (S8*) message);
    msg->message[(len << 1)] = 0;
    msg->message[(len << 1) + 1] = 0;

    ilm_ptr = allocate_ilm(MOD_J2ME);
    ilm_ptr->msg_id = MSG_ID_MMI_JAVA_SHOW_ALERT_USER_DIALOG_REQ;  /* Set the message id */
    ilm_ptr->peer_buff_ptr = NULL;  /* there are no peer message */
    ilm_ptr->local_para_ptr = (local_para_struct*) msg;

    SEND_ILM(MOD_J2ME, MOD_MMI, J2ME_MMI_SAP, ilm_ptr);
}

void mmi_java_recv_show_alert_user_req(void *MsgStruct)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    mmi_java_show_alert_user_dialog_req_struct *msg;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    msg = (mmi_java_show_alert_user_dialog_req_struct*) MsgStruct;
	gcj_TraceOut(0,"mmi_java_recv_show_alert_user_req\n");
    if (gcj_StubIsPrepareToQuit())
		GotalertToQuit=1;
    DisplayPopup((PU8) msg->message, IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8) WARNING_TONE);
}

void JVMEntrySleep(void)
{
    if(gcj_IsJavaOn() == 1)
		javanotify_pause();
}

void JVMEntryActive(void)
{
    if(gcj_IsJavaOn() == 1 && GetActiveScreenId() == SCR_JAVA )
		javanotify_resume();
}
void  GC_disable(void);
static unsigned char gcj_PerpareToQuit=0;
unsigned char gcj_StubIsPrepareToQuit()
{
	return gcj_PerpareToQuit;
}

void gcj_StubAlterAndQuit()
{
    unsigned char quit_from_background=0;
    gcj_PerpareToQuit = 1;
    JVMEntryActive();
    GC_disable();	
    if (gcj_StubIsForceToQuit())
    {
    	if (sxr_GetCurrentTaskId() !=getMainThreadTskId() && sxr_GetCurrentTaskId() != QueueEventInputTaskId)
    		gcj_StubThreadSleep(10000);//wait to quit
	return;
    }

    if (GetActiveScreenId() != SCR_JAVA && GetActiveScreenId() != POPUP_SCREENID)
		quit_from_background=1;
    j2me_send_alertuser((PU8)GetString(STR_JAVA_ERROR_NO_MORE_MEMORY),UCS2Strlen((const S8*)GetString(STR_JAVA_ERROR_NO_MORE_MEMORY)));
    while (!GotalertToQuit )
	gcj_StubThreadSleep(500);	
   if (!quit_from_background)
    {
    	while (!entryScrJavaEnd)
		gcj_StubThreadSleep(500); //wait the pop end
    }
 
   /*
    DisplayPopup((PU8)GetString(STR_JAVA_ERROR_NO_MORE_MEMORY), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8) WARNING_TONE);
    COS_Sleep(1000);
    */
    gcj_NativeJavaQuit(1);
    if (sxr_GetCurrentTaskId() !=getMainThreadTskId() && sxr_GetCurrentTaskId() != QueueEventInputTaskId)
    		gcj_StubThreadSleep(10000);//wait to quit
}
void gcj_StubAlterNoMemory()
{
    //j2me_send_alertuser((PU8)GetString(STR_JAVA_ERROR_NO_MORE_MEMORY),UCS2Strlen((const S8*)GetString(STR_JAVA_ERROR_NO_MORE_MEMORY)));
}


