/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_KeyHandler.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*     MMI key handler based on CSW platform.                                  */
 
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
#ifdef __MMI_TOUCH_SCREEN__
#include "touchscreengprot.h"
#endif 
 
#include <csw.h>

#include "mmi.h"
#include "uim.h"
#include "adp_events.h"
#include "adp_eventsredef.h"
#include "adp_timer.h"   
#include "idleappdef.h"   //added by jinzh:20070721
 
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
#include "callsetupenum.h"
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
#include "gdi_image.h"    
#include "gdi_layer.h"   
#include "timerevents.h"
#include "mmi_trace.h"
#include "fs_type.h"
#include "globalmenuitems.h"
#include "nvram_user_defs.h"
#include "phonebookdef.h"
typedef struct
{
  mmi_keypads_enum    KeyCode;
  MMI_TIMER_IDS TimerIdentifier;  
} TimerId2Key;

/*Guojian Modify Start For Keybrd Ver: Jessamine  on 2008-6-28 16:13 */
const TimerId2Key TimerId2KeyTable[]=
{			
	{ KEY_0            		, KEY_TIMER_ID0},
	{ KEY_1            		, KEY_TIMER_ID1},
	{ KEY_2            		, KEY_TIMER_ID2},
	{ KEY_3            		, KEY_TIMER_ID3},
	{ KEY_4            		, KEY_TIMER_ID4},
	{ KEY_5            		, KEY_TIMER_ID5},
	{ KEY_6            		, KEY_TIMER_ID6},
	{ KEY_7            		, KEY_TIMER_ID7},
	{ KEY_8            		, KEY_TIMER_ID8},
	{ KEY_9            		, KEY_TIMER_ID9},
	{ KEY_LSK        		, KEY_TIMER_ID10},
	{ KEY_RSK        		, KEY_TIMER_ID11}, 
	{ KEY_CSK	   		, KEY_TIMER_ID_NONE},
	{ KEY_UP_ARROW  	,KEY_TIMER_ID12},	
	{ KEY_DOWN_ARROW	,KEY_TIMER_ID13},
	{ KEY_LEFT_ARROW   	, KEY_TIMER_ID14}, 
	{ KEY_RIGHT_ARROW  	, KEY_TIMER_ID15}, 
	{ KEY_SEND			,KEY_TIMER_ID16},		
	{ KEY_END          		, KEYPAD_LONG_PRESS},
	{ KEY_CLEAR			,KEY_TIMER_ID17},		
	{ KEY_STAR         		, KEY_TIMER_ID18},
	{ KEY_POUND        	, KEY_TIMER_ID19},
	{ KEY_VOL_UP		,KEY_TIMER_ID20},				
	{ KEY_VOL_DOWN		,KEY_TIMER_ID21},		
	{ KEY_QUICK_ACS		,KEY_TIMER_ID22},			
	{ KEY_CAMERA		,KEY_TIMER_ID23},		
	{ KEY_ENTER			,KEY_TIMER_ID24},			
	{ KEY_WAP			,KEY_TIMER_ID25},	
	{ KEY_IP				,KEY_TIMER_ID26},	
	{ KEY_EXTRA_1		,KEY_TIMER_ID27},	
	{ KEY_EXTRA_2		,KEY_TIMER_ID28},
#ifdef __SENDKEY2_SUPPORT__
	{ KEY_SEND1		,KEY_TIMER_ID29},	
	{ KEY_SEND2		,KEY_TIMER_ID30},
#endif
#if defined(TGT_GALLITE_G800)
	{ KEY_POWER,		KEY_TIMER_ID31},
#endif

	{KEY_SMS,0},
	{KEY_CLOG,0},	
	{KEY_A,0},
	{KEY_B,0},
	{KEY_C,0},
	{KEY_D,0},
	{KEY_E,0},
	{KEY_F,0},
	{KEY_G,0},
	{KEY_H,0},
	{KEY_I,0},
	{KEY_J,0},
	{KEY_K,0},
	{KEY_L,	0},
	{KEY_M,0},
	{KEY_N,0},
	{KEY_O,0},
	{KEY_P,0},
	{KEY_Q,0},
	{KEY_R,0},
	{KEY_S,0},
	{KEY_T,0},

	{KEY_U,0},
	{KEY_V,0},
	{KEY_W,0},
	{KEY_X,0},
	{KEY_Y,0},
	{KEY_Z,0},
							
	{KEY_BACKSPACE1,	0},//qiff add for full keypad
	{KEY_COMMA,	0},
	{KEY_FULLSTOP,0},	
	{KEY_SHIFT1,0},
	{KEY_SHIFT_INPUTM,0},
	{KEY_SPACE1,0},
	{KEY_AND,0},
	{KEY_INTEGRR,0},
	{KEY_CTRL1,0},
	#ifndef __MMI_NOKIA_STYLE_N800__
	{KEY_FM, 0},
	#endif


	//insert new key before here
	{ MAX_KEYS		,KEY_TIMER_ID_NONE},
};
/*Guojian Modify End  For Keybrd Ver: Jessamine  on 2008-6-28 16:13 */
#define IsKeyLongPress(Key_Code)  ( (IsMyTimerExist(KEYPAD_LONG_PRESS)||IsMyTimerExist(KEY_TIMER_ID0))&&(gKeyCodeOld == Key_Code))

extern BOOL isSetKeypadVolume; //add by panxu 20070227
extern idle_context_struct  g_idle_context;   //Added by Jinzh:20070721
extern U16   gKeyPadLockFlag;		//Added by Jinzh:20070721
extern const CUSTOM_STRING StrRes_2[];
extern const CUSTOM_STRING StrRes_1[];
#define MILLISEC(x)	((x)*(16))
static S16  gKeyCodeOld;

#define LONG_PRESS_TIME 1500

//extern UINT32 g_Time_Measure; //added by licheng to test time measure
#ifdef __MMI_ENGINEER_MODE__
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
extern void RecordCurrKeyHandler(S16 keyCode, S16 keyType);
#endif
#endif
void KeyLongPress(void)
{
	U8 i = 0;
	for(i=0; i<sizeof(TimerId2KeyTable)/sizeof(TimerId2KeyTable[0]);i++)
	{
		if(IsKeyLongPress(TimerId2KeyTable[i].KeyCode))
		{
#ifdef __MMI_ENGINEER_MODE__
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
			RecordCurrKeyHandler(gKeyCodeOld, KEY_LONG_PRESS);
#endif
#endif
			ExecuteCurrKeyHandler(gKeyCodeOld, KEY_LONG_PRESS);
			return;
		}
	}
}
//dyj modify start 20060609

extern void UpdateScr1002ActiveCall(void);

#ifdef __MMI_BLACKBERRY_QWERTY__
S16 gKeyCode = 0;
#endif

void COS_KeyEventHandler( ilm_struct* Message) 
{
	S16 Coskeycode=0,Coskeytype=0;
	msg_kbd_data * msg_key = (msg_kbd_data *) Message->local_para_ptr;
	int i , total_key;
	KEYBRD_MESSAGE keyMessage;
	Coskeycode = (S16)msg_key->Keycode;

	//play keypad tone add by panxu 200961224
	memset(&keyMessage, 0, sizeof(KEYBRD_MESSAGE));
	keyMessage.nMsgType = msg_key->Keyevent;
	keyMessage.nKeyCode = msg_key->Keycode;

#ifdef __MMI_BLACKBERRY_QWERTY__
	gKeyCode = msg_key->Keycode;
#endif

	/*Wangzl Modify Start  Ver: TBM_780  on 2007-7-31 16:14 */
	if((!mmi_is_lcd_backlight_on())&&( KEY_EVENT_DOWN   == msg_key->Keyevent))
	{
        	UpdateScr1002ActiveCall();
		TurnOnBacklight(1);
		return;
	}
	else
	{
		TurnOnBacklight(1);
	}
	/*Wangzl Modify End  For 5860 Ver: TBM_780  on 2007-7-31 16:14 */

  
	/*Guojian Modify Start For Keybrd Ver: Jessamine  on 2008-6-28 16:58 */
	total_key = sizeof(TimerId2KeyTable)/sizeof(TimerId2KeyTable[0]);
	for(i=0; i<total_key;i++)
	{
		if(TimerId2KeyTable[i].KeyCode == Coskeycode)
			break;
	}
	/*Guojian Modify End  For Keybrd Ver: Jessamine  on 2008-6-28 16:58 */

	if (msg_key->Keyevent==KEY_EVENT_DOWN)
	{
		/*Guojian Modify Start For Keybrd Ver: Jessamine  on 2008-6-28 16:57 */
		if(Coskeycode == KEY_END)
		{
			StopTimer(KEY_TIMER_ID0);
			StartTimer(KEYPAD_LONG_PRESS, 3000/*MILLISEC(5000)*/, KeyLongPress);
		}
		
#if defined(TGT_USING_POWER_KEY)
		if(Coskeycode == KEY_POWER)
			StartTimer(KEY_TIMER_ID0, 3000/*MILLISEC(5000)*/, KeyLongPress);

#endif
		else if(total_key != i) //存在这个键值
		{
			StopTimer(KEYPAD_LONG_PRESS);
			StartTimer(KEY_TIMER_ID0, LONG_PRESS_TIME, KeyLongPress);
		}
		
#ifdef MMI_MEMLEAK_CHK	
		if(KEY_STAR==Coskeycode)
		{
			mmi_memleakCHK();
		}
#endif		
		/*Guojian Modify End  For Keybrd Ver: Jessamine  on 2008-6-28 16:57 */
		
		if( KEY_VOL_UP == Coskeycode ||KEY_VOL_DOWN == Coskeycode)//chenhe,070726,after a key started a timer,don't change keycode to avoid of sending the longpress with the side key as its key code
		{
			if(g_idle_context.IsOnIdleScreen == 1)   //Added by jinzh:20070728
			isSetKeypadVolume=TRUE;
			gKeyCodeOld = total_key+1;
		}  
		else
		{
			gKeyCodeOld = Coskeycode;					
		}
		Coskeytype = KEY_EVENT_DOWN;
	}
	else if(msg_key->Keyevent==KEY_EVENT_UP)
	{

		if(Coskeycode == KEY_END)
		{
			TRACE_SYS_FUNC("Coskeycode == KEY_END");
			isSetKeypadVolume=FALSE;				//Added by jinzh:20070622 解决设置按键音音量时直接返回idle按键音消失问题
		#if defined(TGT_USING_POWER_KEY)	
                        StopTimer(KEY_TIMER_ID0);
                #endif
			StopTimer(KEYPAD_LONG_PRESS);
		}
		else if(total_key != i) //存在这个键值
		{
			StopTimer(KEY_TIMER_ID0);
#if defined(TGT_USING_POWER_KEY)
                        StopTimer(KEYPAD_LONG_PRESS);
#endif
		}
		Coskeytype = KEY_EVENT_UP;
	}
	else if(msg_key->Keyevent==KEY_EVENT_LONG_PRESS )
	{
		TRACE_SYS_FUNC("msg_key->Keyevent == KEY_EVENT_LONG_PRESS");
		Coskeytype = KEY_EVENT_LONG_PRESS;
	}


	if (!(isSetKeypadVolume) ||    
		(g_idle_context.IsOnIdleScreen && 
		!gKeyPadLockFlag &&
		(Coskeycode != KEY_VOL_UP) &&
		(Coskeycode != KEY_VOL_DOWN) ) ||
		(isSetKeypadVolume &&!g_idle_context.IsOnIdleScreen &&
		(Coskeycode != KEY_VOL_UP&&Coskeycode != KEY_VOL_DOWN))
		) //add by panxu 20070227 to avoid double key pad tone when set the key pad volume
	{//在设置音量的时候或idle下按音量调节键时不从这里播放键盘音
		extern U8 mmi_kbn_key_tone_hdlr(KEYBRD_MESSAGE *eventKey);		
#if defined(TGT_USING_POWER_KEY)
		if(KEY_POWER != Coskeycode)
#endif
		mmi_kbn_key_tone_hdlr(&keyMessage);
	}
	

	if(g_mmi_frm_cntx.kbd_pre_func)
		g_mmi_frm_cntx.kbd_pre_func(&keyMessage); 

#ifdef __MMI_ENGINEER_MODE__	
#if defined(AUTO_TEST_KEYPRESS_RECORD_SUPPORT)
	RecordCurrKeyHandler(Coskeycode, Coskeytype);
#endif
#endif
	ExecuteCurrKeyHandler(Coskeycode, Coskeytype);	
}

//dyj modify end

//void KeyOneHandler(void)
//{
//
//    	s32	DOW_list[]={	'S','M','T','W','T','F','S'	};
//	s32 i,x,y,character_width,character_height;
//
//	s32 x1,y1,x2,y2;
//	UI_font_type	saved_font={	1,0,1,MEDIUM_FONT,0,0	};
//	color text_color = {255,255,255,100};
//	
//	coolsand_UI_push_clip();
//	coolsand_UI_push_text_clip();
//	coolsand_UI_reset_clip();
//
//	coolsand_UI_set_font(saved_font);
//	character_height=coolsand_UI_get_character_height();
//	x1=40;
//	y1=0;
//	x2=x1+60;
//	y2=y1+60;
//	coolsand_UI_set_clip(x1,y1,x2,y2);
//	coolsand_UI_set_text_clip(x1,y1,x2,y2);        		
//
//	coolsand_UI_set_text_color(text_color);
//	coolsand_UI_move_text_cursor(x1+10,y1+10);
//	//coolsand_UI_print_character((UI_character_type)DOW_list[1]);		
//	coolsand_UI_print_text((UI_string_type)GetString(STR_GLOBAL_BACK)); 
//	
//	coolsand_UI_pop_text_clip();
//	coolsand_UI_pop_clip();
//	//gdi_layer_blt_base_layer(x1,y1,x2,y2);	
//	coolsand_UI_BLT_double_buffer(x1,y1,x2,y2);
//	
//       //gdi_image_draw(0, 0, (U8 *) LOGO_BMP);   
//    	//gdi_layer_blt_base_layer(0,0,128,160);	 
//	StartTimer(COS_TEST_TIMER, 5000, Timer_Test);
//}
//void KeyTwoHandler(void)
//{
//	 
//	//coolsand_UI_BLT_double_buffer(0,0,128,160);
//	//TM_FILETIME* pFileTime;
//	//TM_GetRTC (& pFileTime);
//	
//	
//	StartTimer(COS_TEST_TIMER_1, 60000, Timer_Test);
//	
//}

