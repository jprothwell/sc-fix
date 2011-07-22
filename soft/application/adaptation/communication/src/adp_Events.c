/******************************************************************************/
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/******************************************************************************/
/* Filename:    adp_Events.c                                                    */
/*                                                                            */
/* Description:                                                               */
/*     change MYK message to coolsand message .                                  */
/*     @sharpen 2006-05-20.                                                       */
/******************************************************************************/

#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#ifdef MMI_ON_HARDWARE_P
#include "chip_id.h"
#endif
 
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
#include "unit_test.h"
#include "dual_sim.h"
#include "adp_events.h"
#include "adp_nw_event.h"
#include "adp_eventsredef.h"
#include "cfw.h"   
#include "phonebooktypes.h"  
#ifdef MMI_ON_HARDWARE_P
#include "pmd_m.h"
#endif

 
#ifdef __MMI_THEMES_V2_SUPPORT__
#include "thememanager.h"
#endif
#include "messagesexdcl.h"
#include "adp_newearphoneplan.h"
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

//dyj add start 20060616
#include "device.h"
#include "stack_msgs.h"
#include <cswtype.h>
#include "mmi_msg_struct.h"
//dyj add end
#include "phonebooktypes.h"

#include "mmi_trace.h"

#include "tm.h"
#include "datetimegprot.h"
 

#include "settingsgdcl.h"

#include "nvram_data_items.h"

#include "di.h"
#include "filesystemdef.h"

#ifdef MMI_ON_HARDWARE_P
#include "key_defs.h"
#else
extern const KeyPadMap nKeyPadMap[];
#endif
#include "adp_fs.h"

#ifdef __MMI_RUNNING_LED__
#include "led.h"
#endif
#include "gsm7bitdefaultalphabet.h"
#ifdef __MMI_NOKIA_STYLE_N800__
#define __KEYPADBACKLIGHT_LIGHT__ 0x40
#else
#define __KEYPADBACKLIGHT_LIGHT__ 0xE0
#endif
#define __KEYPADBACKLIGHT_DARK__ 0x00

#ifdef MMI_ON_WIN32
#define KP_NB        'Z' // no button
#endif

extern void VibratePlayRepeat();
extern void VibratePlayOnce();
extern void VibratePlaydur();
extern void StartRingTone(U16);
static BOOL ADP_DispatchDmIndMsg(COS_EVENT* pCosEvent);
//static BOOL ADP_DispatchDmReqMsg(COS_EVENT* pCosEvent);
extern BOOL ADP_DispatchCalllogMsg(COS_EVENT *pCosEvent);
extern void  Msg_Adaption_PHB_Init_Ready(void);
extern void Msg_Adaption_PHB_startup_begin(void);
extern void  Msg_Adaption_PHB_Init(void);

#ifdef __ADP_MULTI_SIM__
extern void Adp_Calllog_Send_Start_Ind(U8 nSimId);
#else  /* No Defined __ADP_MULTI_SIM__ */
extern void Adp_Calllog_Send_Start_Ind(void);
#endif /* __ADP_MULTI_SIM__ */

#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__)
extern BOOL mmi_phb_init_check_sim_change(void);
#endif


#if defined(__MMI_SCREEN_ROTATE__)
extern MMI_BOOL mmi_frm_is_screen_width_height_swapped(void);
#endif

BOOL g_hasMountedFlashFS = FALSE;
BOOL g_hasMountedTFlashFS = FALSE;
BOOL g_tflashCardExist = FALSE;

#if defined(DUAL_TFLASH_SUPPORT)
BOOL g_hasMountedTFlash2FS = FALSE;
BOOL g_tflashCard2Exist = FALSE;
#endif

//add gdm 080401
BOOL g_hasMountedNandFlashFS = FALSE;
BOOL g_nandflashCardExist = FALSE;

BOOL g_FixedInit[MMI_SIM_NUMBER] = {FALSE,};

BOOL g_IsVibrate = FALSE; //Added by jinzh:20070716
UINT16 Earphone_vol = 0;   //记录耳机按钮的电平值 Added by jinzh:20070623
UINT16 Earphone_flag = 0xffff;
BOOL KeyDown = FALSE;		//Added by jinzh:20070623
static INT32 keyProtectCnt_Dm = 0; //to count key dealing counts
//HANDLE protectKeyCnt = 0;
// add by chenqiang 20070808 for bug 5889 5800 b
extern BOOL g_phb_copyordelete_error_flag;
// add by chenqiang 20070808 for bug 5889 5800 e
extern BOOL flag_init_ok;
extern void (*PHBListCopyOrDeleteTimerOutErrorFUN)(void);

//[221] Added by ZXB 2009/02/06								[BEGIN]
// Add module context data structure and global var for dual-sim.
#ifdef __ADP_MULTI_SIM__
ADP_COMMON_CONTEXT_T g_CommonContext[4] =
{
	{ADP_SIM_1, },
	{ADP_SIM_2, },
	{ADP_SIM_3, },
	{ADP_SIM_4, },
};

ADP_COMMON_CONTEXT_T *g_pCommonContext = &g_CommonContext[0];
#endif
//[221] Added by ZXB 2009/02/06								[END]

#ifdef __ADP_MULTI_SIM__
U8 nSIMBadOrNotInsert =0; // indicate the number of bad sim and not inserted sim
BOOL gSIMIsNotExist[ADP_SIM_NUMBER] = {0,};// indicate the sim is not inserted
extern BOOL g_PHBSIMStartInd[];
extern BOOL g_PHBSIMStartFinish[];
extern adp_phb_context_t adp_gPHBContex[];
extern U8 gCurrentSMSSimID;
extern U8 gPHBCurrentSimID;
#else
extern adp_phb_context_t adp_gPHBContex;
#endif

#ifdef __ADP_MULTI_SIM__
extern adp_sim_context_t g_SIMContext[];
extern adp_sim_context_t *g_pSIMContext;
#endif


extern adp_phb_context_t *adp_pPHBContext;

#ifdef __ADP_MULTI_SIM__
extern ADP_CC_CONTEXT_T *g_pCCContext; //CC
extern ADP_CC_CONTEXT_T g_CCContext[];

extern ADP_SS_CONTEXT_T *g_pSSContext; //SS
extern ADP_SS_CONTEXT_T g_SSContext[];

extern ADP_CHIS_CONTEXT_T *g_pCHISContext; //call history
extern ADP_CHIS_CONTEXT_T g_CHISContext[];

#endif

#ifdef __ADP_MULTI_SIM__
extern adp_sat_context_t g_SATContext[];
#else
extern adp_sat_context_t g_SATContext;
#endif
extern adp_sat_context_t *g_pSATContext;
extern adp_wap_context_t    *adp_pWAPContext;

extern BOOL ADP_DispatchPhbMsg(COS_EVENT *p);


BOOL bIsChargerConnected = FALSE;
/*GUOJ Add End  For 6011 Ver: TBM780  on 2007-8-2 14:13 */
	



 
/**************************************************************/
#ifdef MMI_ON_HARDWARE_P	
#ifdef TGT_GALLITE_G800
#if (NUMBER_OF_SIM > 2)    
static const KeyPadMapSlim nKeyPadMap[] = {
{KP_0,				KEY_0},
{KP_1,				KEY_1},
{KP_2,				KEY_2},
{KP_3,				KEY_3},
{KP_4,				KEY_4},
{KP_5,				KEY_5},
{KP_6,				KEY_6},
{KP_7,				KEY_7},
{KP_8,				KEY_8},
{KP_9,				KEY_9},
{KP_STAR,			       KEY_STAR},
{KP_POUND,				KEY_POUND}, //12
{KP_SC,			       KEY_ENTER},
{KP_SL,			KEY_LSK},
{KP_SR,			KEY_RSK},
{KP_UP,	                            KEY_UP_ARROW},  //25
{KP_DW,                        KEY_DOWN_ARROW},
{KP_LT,	                     KEY_LEFT_ARROW},
{KP_RT,                        KEY_RIGHT_ARROW},
{KP_OK,			       KEY_ENTER},
{KP_SND,		KEY_SEND},
{KP_END,			KEY_END},
{KP_VD,		KEY_VOL_DOWN},
{KP_VU,		KEY_VOL_UP},
{KP_VU,			KEY_VOL_UP},
{KP_VD,	   	KEY_VOL_DOWN},
{KP_VD,	KEY_VOL_DOWN},
{KP_VU,		KEY_VOL_UP},
{KP_VU,		KEY_VOL_UP},
{KP_VD,		KEY_VOL_DOWN},

{KP_PWR,KEY_POWER},
#ifndef __SENDKEY2_SUPPORT__
{KP_BACK,			KEY_INVALID},
{KP_DEL,			       KEY_CLEAR},
#else
{KP_BACK,			KEY_SEND2},
{KP_DEL,			       KEY_CAMERA},
#endif

{KP_A,					KEY_A,					},
{KP_H,					KEY_H,					},
{KP_I,					KEY_I,					},
{KP_J,					KEY_J,					},
{KP_K,					KEY_K,					},
{KP_L,					KEY_L,					},
{KP_N,					KEY_N,					},
{KP_M,					KEY_M,					},
{KP_O,					KEY_O,					},
{KP_P,					KEY_P,					},
{KP_Q,					KEY_Q,					},
{KP_S,					KEY_S,					},
{KP_U,					KEY_U,					},
{KP_W,					KEY_W,					},
{KP_Y,					KEY_Y,					},
{KP_Z,					KEY_Z,					},
{KP_BKSPACE,			KEY_BACKSPACE1,		},//' ~ '
{KP_SMS,				KEY_SMS,				},
{KP_PBOOK,				KEY_CLOG,				},
{KP_COMMA,			     	KEY_COMMA,				}, //input :,
{KP_AND,					KEY_AND,				}, //input &
{KP_STOP,					KEY_FULLSTOP,			},//input ; .
{KP_NUMLOCK ,					KEY_SHIFT1,				},//comon shift to input upper char
{KP_SHIFT,					KEY_SHIFT_INPUTM,		},//input to switch
{KP_SPACE,			  	     	KEY_SPACE1,				},//input space
{KP_QUES,					KEY_INTEGRR,			},//input ?
{KP_CTRL,					KEY_CALENDER,				},//entry symble list
{KP_ENTER,					KEY_NEWLINE,			},//input enter key next line
{KP_F0,					KEY_SEND1,			},//input enter key next line


{KP_NB,		KEY_INVALID}
};
#else
static const KeyPadMapSlim nKeyPadMap[] = {
{KP_0,				KEY_0},
{KP_1,				KEY_1},
{KP_2,				KEY_2},
{KP_3,				KEY_3},
{KP_4,				KEY_4},
{KP_5,				KEY_5},
{KP_6,				KEY_6},
{KP_7,				KEY_7},
{KP_8,				KEY_8},
{KP_9,				KEY_9},
{KP_STAR,			       KEY_STAR},
{KP_POUND,				KEY_POUND}, //12
{KP_SC,			       KEY_ENTER},
{KP_SL,			KEY_LSK},
{KP_SR,			KEY_RSK},
{KP_UP,	                            KEY_UP_ARROW},  //25
{KP_DW,                        KEY_DOWN_ARROW},
{KP_LT,	                     KEY_LEFT_ARROW},
{KP_RT,                        KEY_RIGHT_ARROW},
{KP_OK,			       KEY_ENTER},
{KP_SND,		KEY_SEND},
#ifndef TGT_USING_POWER_KEY
{KP_PWR,			KEY_END},
#else
{KP_END,			KEY_END},
#endif
{KP_VD,		KEY_VOL_DOWN},
{KP_VU,		KEY_VOL_UP},
{KP_VU,			KEY_VOL_UP},
{KP_VD,	   	KEY_VOL_DOWN},
{KP_VD,	KEY_VOL_DOWN},
{KP_VU,		KEY_VOL_UP},
{KP_VU,		KEY_VOL_UP},
{KP_VD,		KEY_VOL_DOWN},

{KP_PWR,KEY_POWER},
#ifndef __SENDKEY2_SUPPORT__
{KP_BACK,			KEY_INVALID},
{KP_DEL,			       KEY_CLEAR},
#else
{KP_BACK,			KEY_SEND1},
{KP_DEL,			       KEY_SEND2 },
#endif

{KP_A,					KEY_A,					},
{KP_H,					KEY_H,					},
{KP_I,					KEY_I,					},
{KP_J,					KEY_J,					},
{KP_K,					KEY_K,					},
{KP_L,					KEY_L,					},
{KP_N,					KEY_N,					},
{KP_M,					KEY_M,					},
{KP_O,					KEY_O,					},
{KP_P,					KEY_P,					},
{KP_Q,					KEY_Q,					},
{KP_S,					KEY_S,					},
{KP_U,					KEY_U,					},
{KP_W,					KEY_W,					},
{KP_Y,					KEY_Y,					},
{KP_Z,					KEY_Z,					},
{KP_BKSPACE,			KEY_BACKSPACE1,		},//' ~ '
{KP_SMS,				KEY_SMS,				},
{KP_PBOOK,				KEY_CLOG,				},
{KP_COMMA,			     	KEY_COMMA,				}, //input :,
{KP_AND,					KEY_AND,				}, //input &
{KP_STOP,					KEY_FULLSTOP,			},//input ; .
{KP_NUMLOCK ,					KEY_SHIFT1,				},//comon shift to input upper char
{KP_SHIFT,					KEY_SHIFT_INPUTM,		},//input to switch
{KP_SPACE,			  	     	KEY_SPACE1,				},//input space
{KP_QUES,					KEY_INTEGRR,			},//input ?
{KP_CTRL,					KEY_CTRL1,				},//entry symble list
{KP_ENTER,					KEY_NEWLINE,			},//input enter key next line
{KP_F0,					KEY_CAMERA ,			},//input enter key next line


{KP_NB,		KEY_INVALID}
};
#endif
#elif TGT_TD_4SIM
static const KeyPadMapSlim nKeyPadMap[] = {
{KP_0,				KEY_0},
{KP_1,				KEY_1},
{KP_2,				KEY_2},
{KP_3,				KEY_3},
{KP_4,				KEY_4},
{KP_5,				KEY_5},
{KP_6,				KEY_6},
{KP_7,				KEY_7},
{KP_8,				KEY_8},
{KP_9,				KEY_9},
{KP_STAR,			       KEY_STAR},
{KP_POUND,				KEY_POUND}, //12
{KP_SC,			       KEY_ENTER},
{KP_SL,			KEY_LSK},
{KP_SR,			KEY_RSK},
{KP_UP,	                            KEY_UP_ARROW},  //25
{KP_DW,                        KEY_DOWN_ARROW},
{KP_LT,	                     KEY_LEFT_ARROW},
{KP_RT,                        KEY_RIGHT_ARROW},
{KP_OK,			       KEY_ENTER},
{KP_SND,		KEY_SEND},
{KP_END,			KEY_END},
{KP_VD,		KEY_VOL_DOWN},
{KP_VU,		KEY_VOL_UP},
{KP_VU,			KEY_VOL_UP},
{KP_VD,	   	KEY_VOL_DOWN},
{KP_VD,	KEY_VOL_DOWN},
{KP_VU,		KEY_VOL_UP},
{KP_VU,		KEY_VOL_UP},
{KP_VD,		KEY_VOL_DOWN},

{KP_PWR,				       KEY_END},
{KP_BACK,			KEY_SEND2},
{KP_DEL,			       KEY_CAMERA},

{KP_A,					KEY_A,					},
{KP_H,					KEY_H,					},
{KP_I,					KEY_I,					},
{KP_J,					KEY_J,					},
{KP_K,					KEY_K,					},
{KP_L,					KEY_L,					},
{KP_N,					KEY_N,					},
{KP_M,					KEY_M,					},
{KP_O,					KEY_O,					},
{KP_P,					KEY_P,					},
{KP_Q,					KEY_Q,					},
{KP_S,					KEY_S,					},
{KP_U,					KEY_U,					},
{KP_W,					KEY_W,					},
{KP_Y,					KEY_Y,					},
{KP_Z,					KEY_Z,					},
{KP_BKSPACE,			KEY_BACKSPACE1,		},//' ~ '
{KP_SMS,				KEY_SMS,				},
{KP_PBOOK,				KEY_CLOG,				},
{KP_COMMA,			     	KEY_COMMA,				}, //input :,
{KP_AND,					KEY_AND,				}, //input &
{KP_STOP,					KEY_FULLSTOP,			},//input ; .
{KP_NUMLOCK ,					KEY_SHIFT1,				},//comon shift to input upper char
{KP_SHIFT,					KEY_SHIFT_INPUTM,		},//input to switch
{KP_SPACE,			  	     	KEY_SPACE1,				},//input space
{KP_QUES,					KEY_INTEGRR,			},//input ?
{KP_CTRL,					KEY_CTRL1,				},//entry symble list
{KP_ENTER,					KEY_NEWLINE,			},//input enter key next line
{KP_FM,					KEY_FM,			},//input enter key next line


{KP_NB,		KEY_INVALID}
};
#elif defined(__MMI_BLACKBERRY_QWERTY__)
static const KeyPadMapSlim nKeyPadMap[] = {
{KP_0,				KEY_0},
{KP_1,				KEY_1},
{KP_2,				KEY_2},
{KP_3,				KEY_3},
{KP_4,				KEY_4},
{KP_5,				KEY_5},
{KP_6,				KEY_6},
{KP_7,				KEY_7},
{KP_8,				KEY_8},
{KP_9,				KEY_9},
{KP_STAR,	        KEY_STAR},
{KP_POUND,	        KEY_POUND}, //12
{KP_SC,		        KEY_ENTER},
{KP_SL,		        KEY_LSK},
{KP_SR,		        KEY_RSK},
{KP_UP,	            KEY_UP_ARROW},  //25
{KP_DW,             KEY_DOWN_ARROW},
{KP_LT,	            KEY_LEFT_ARROW},
{KP_RT,             KEY_RIGHT_ARROW},
{KP_OK,		        KEY_ENTER},
{KP_SND,	        KEY_SEND1},
{KP_END,	        KEY_END},
{KP_VD,		        KEY_VOL_DOWN},
{KP_VU,		        KEY_VOL_UP},
{KP_VU,		        KEY_VOL_UP},
{KP_VD,	   	        KEY_VOL_DOWN},
{KP_VD,	            KEY_VOL_DOWN},
{KP_VU,		        KEY_VOL_UP},
{KP_VU,		        KEY_VOL_UP},
{KP_VD,		        KEY_VOL_DOWN},
{KP_PWR,	        KEY_END},

{KP_BACK,			KEY_RSK},
{KP_DEL,		    KEY_CLEAR},

{KP_A,			    KEY_A,					},
{KP_B,		        KEY_B,					},
{KP_C,				KEY_C,					},
{KP_D,				KEY_D,					},
{KP_E,				KEY_E,					},
{KP_F,				KEY_F,					},
{KP_G,				KEY_G,					},
{KP_H,				KEY_H,					},
{KP_I,				KEY_I,					},
{KP_J,				KEY_J,					},
{KP_K,				KEY_K,					},
{KP_L,				KEY_L,					},
{KP_M,				KEY_M,					},
{KP_N,				KEY_N,					},
{KP_O,				KEY_O,					},
{KP_P,				KEY_P,					},
{KP_Q,				KEY_Q,					},
{KP_R,				KEY_R,					},
{KP_S,				KEY_S,					},
{KP_T,				KEY_T,					},
{KP_U,				KEY_U,					},
{KP_V,				KEY_V,					},
{KP_W,				KEY_W,					},
{KP_X,				KEY_X,					},
{KP_Y,				KEY_Y,					},
{KP_Z,				KEY_Z,					},
{KP_BKSPACE,		KEY_BACKSPACE1,		},//' ~ '
{KP_SMS,			KEY_SMS,				},
{KP_PBOOK,			KEY_CLOG,				},
{KP_COMMA,			KEY_COMMA,				}, //input :,
{KP_AND,			KEY_AND,				}, //input &
{KP_STOP,			KEY_FULLSTOP,			},//input ; .
{KP_NUMLOCK ,		KEY_SHIFT_INPUTM,			},//comon shift to input upper char
{KP_SHIFT,			KEY_SHIFT1,		},//input to switch
{KP_SPACE,			KEY_SPACE1,		},//input space
{KP_QUES,			KEY_INTEGRR,		},//input ?
{KP_CTRL,			KEY_CTRL1,			},//entry symble list
{KP_ENTER,			KEY_NEWLINE,		},//input enter key next line
{KP_F0,				KEY_CAMERA,			},
{KP_F1, 			KEY_SYM, 			},
{KP_F2, 			KEY_DOLLAR, 		},
{KP_F3, 			KEY_STOP,			},
{KP_F4, 			KEY_MUSIC, 			},
{KP_F5, 			KEY_SEND2,			},
{KP_F6, 			KEY_INVALID, 		},
{KP_FM,				KEY_FM,			    },//input enter key next line
{KP_NB,		        KEY_INVALID         }
};
#else
static const KeyPadMapSlim nKeyPadMap[] = {
{KP_0,				KEY_0},
{KP_1,				KEY_1},
{KP_2,				KEY_2},
{KP_3,				KEY_3},
{KP_4,				KEY_4},
{KP_5,				KEY_5},
{KP_6,				KEY_6},
{KP_7,				KEY_7},
{KP_8,				KEY_8},
{KP_9,				KEY_9},
{KP_STAR,			       KEY_STAR},
{KP_POUND,				KEY_POUND}, //12
{KP_SC,			       KEY_ENTER},
{KP_SL,			KEY_LSK},
{KP_SR,			KEY_RSK},
{KP_UP,	                            KEY_UP_ARROW},  //25
{KP_DW,                        KEY_DOWN_ARROW},
{KP_LT,	                     KEY_LEFT_ARROW},
{KP_RT,                        KEY_RIGHT_ARROW},
{KP_OK,			       KEY_ENTER},
{KP_SND,		KEY_SEND},
{KP_END,			KEY_END},
{KP_VD,		KEY_VOL_DOWN},
{KP_VU,		KEY_VOL_UP},
{KP_VU,			KEY_VOL_UP},
{KP_VD,	   	KEY_VOL_DOWN},
{KP_VD,	KEY_VOL_DOWN},
{KP_VU,		KEY_VOL_UP},
{KP_VU,		KEY_VOL_UP},
{KP_VD,		KEY_VOL_DOWN},

{KP_PWR,				       KEY_END},
#ifndef __SENDKEY2_SUPPORT__
{KP_BACK,			KEY_SEND},
{KP_DEL,			       KEY_CLEAR},
#else
{KP_BACK,			KEY_SEND1},
{KP_DEL,			       KEY_SEND2},
#endif

{KP_A,					KEY_A,					},
{KP_H,					KEY_H,					},
{KP_I,					KEY_I,					},
{KP_J,					KEY_J,					},
{KP_K,					KEY_K,					},
{KP_L,					KEY_L,					},
{KP_N,					KEY_N,					},
{KP_M,					KEY_M,					},
{KP_O,					KEY_O,					},
{KP_P,					KEY_P,					},
{KP_Q,					KEY_Q,					},
{KP_S,					KEY_S,					},
{KP_U,					KEY_U,					},
{KP_W,					KEY_W,					},
{KP_Y,					KEY_Y,					},
{KP_Z,					KEY_Z,					},
{KP_BKSPACE,			KEY_BACKSPACE1,			},//' ~ '
{KP_SMS,				KEY_SMS,				},
{KP_PBOOK,				KEY_CLOG,				},
{KP_COMMA,			    KEY_COMMA,				}, //input :,
{KP_AND,				KEY_AND,				}, //input &
{KP_STOP,				KEY_FULLSTOP,			},//input ; .
{KP_NUMLOCK ,			KEY_SHIFT1,				},//comon shift to input upper char
{KP_SHIFT,				KEY_SHIFT_INPUTM,		},//input to switch
{KP_SPACE,			  	KEY_SPACE1,				},//input space
{KP_QUES,				KEY_INTEGRR,			},//input ?
{KP_CTRL,				KEY_CTRL1,				},//entry symble list
{KP_ENTER,				KEY_NEWLINE,			},//input enter key next line
{KP_FM,					KEY_FM,					},//input enter key next line
#if defined(__PROJECT_GALLITE_C01__) 
{KP_QQ,					KEY_QQ,					},
#endif
{KP_MUSIC,				KEY_MUSIC,				},
{KP_MUSIC_PLAY,			KEY_MUSIC_PLAY,			},
{KP_MUSIC_STOP,			KEY_MUSIC_STOP,			},
{KP_MUSIC_PREV,			KEY_MUSIC_PREV,			},
{KP_MUSIC_NEXT,			KEY_MUSIC_NEXT,			},
{KP_NB,					KEY_INVALID}
};
#endif
#endif

UINT8 adp_KeyMapGetKey(mmi_keypads_enum key)
{
	int i=0;
	int size=(sizeof(nKeyPadMap))/(sizeof (KeyPadMapSlim));
	for (i=0;i<size;i++)
	{
		if (nKeyPadMap[i].nMMIKeyCode == key)
			return nKeyPadMap[i].nKeyCode;
	}
	return KP_NB;
}


/**************************************************************
	FUNCTION NAME		: adp__SendMessageToMMI
	PURPOSE			    : Send message about CC to CSD MMI task.
	INPUT PARAMETERS	: local_para_struct *msg_ptr
						: msg_type msg_id
	OUTPUT PARAMETERS   : NULL
	RETURN				: NULL
**************************************************************/
void Adp_SendMessageToMMI(local_para_struct *msg_ptr, msg_type_t msg_id, module_type src_mod_id)
{

	ilm_struct  ilm_ptr;

	ilm_ptr.src_mod_id = src_mod_id;
	ilm_ptr.dest_mod_id = MOD_MMI;
	ilm_ptr.local_para_ptr = (local_para_struct*)msg_ptr;
	ilm_ptr.peer_buff_ptr = NULL;
	ilm_ptr.msg_id = msg_id;
	OslMsgSendExtQueue(&ilm_ptr);

}


UINT8 ADP_GetFreeUti()
{
	UINT8 uti, ret;
	ret = CFW_GetFreeUTI(0, &uti);
//	mmi_trace(g_sw_ADP_SIM,"UTI: Func: %s uti=%d, ret=0x%x", __FUNCTION__, uti, ret);
	return uti;
}
/**************************************************************

	FUNCTION NAME		: ADP_DispatchDmMsg

  	PURPOSE				: Message Adaption for Device Manager  

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

	REMARKS				: 

**************************************************************/
static BOOL ADP_DispatchDmMsg(COS_EVENT* pCosEvent)
{
	//TRACE_SYS_FUNCTION();
	ASSERT(pCosEvent != NULL);
	
//	if (pCosEvent->nEventId >= MSG_ID_L4A_CODE_BEGIN && pCosEvent->nEventId <= (MSG_ID_L4A_CODE_BEGIN + 1300))
//		return ADP_DispatchDmReqMsg(pCosEvent);
	
//	else if (pCosEvent->nEventId >= EV_DM_BASE && pCosEvent->nEventId <= EV_DM_END_)
	if (pCosEvent->nEventId >= EV_DM_BASE && pCosEvent->nEventId <= EV_DM_END_)
		return ADP_DispatchDmIndMsg(pCosEvent); 
	else
		return FALSE;
}

U32 CSW2CSDPowerOn(U32 poweron)
{
	U32 poweronmode = 0xffffffff;
	INT32 handle = -1,reason = FALSE;
	UINT8 tmpFilename[256];

#ifdef __MMI_RUNNING_LED__
    stopKeyPadLED();
#endif
	switch (poweron)
	{
		case DM_POWRN_ON_CAUSE_KEY:
			poweronmode = POWER_ON_KEYPAD;
			break;
		case DM_POWRN_ON_CAUSE_CHARGE:
			poweronmode = POWER_ON_CHARGER_IN;
			break;
		case DM_POWRN_ON_CAUSE_ALARM:
			poweronmode = POWER_ON_ALARM;
			break;
		case DM_POWRN_ON_CAUSE_EXCEPTION:
			poweronmode = POWER_ON_EXCEPTION;
			break;
		case DM_POWRN_ON_CAUSE_RESET:		
        	AnsiiToUnicodeString(tmpFilename,PHONE_RESTORE_FLAG_PATH);
			handle=FS_Open(tmpFilename,FS_O_RDONLY,0);
			if(handle>=0)
			{
				FS_Read(handle,(UINT8*)&reason,sizeof(reason));
				mmi_trace(1,"CSW2CSDPowerOn,reason is %d",reason);
				if(reason)
				{
					poweronmode = POWER_ON_KEYPAD;
				}
				else
				{
					poweronmode = POWER_ON_EXCEPTION;
				}
				FS_Close(handle);
                FS_Delete(tmpFilename);
            }
			else
			{
				poweronmode = POWER_ON_EXCEPTION;
			}
			break;
		case DM_POWRN_ON_CAUSE_UNKOWN:			
		default:
			poweronmode = POWER_ON_EXCEPTION;
			break;
	}	
	mmi_trace(1,"CSW2CSDPowerOn,poweronmode is %d",poweronmode);

	return	poweronmode;
}


/**************************************************************

	FUNCTION NAME		: ADP_DispatchDmIndMsg 

  	PURPOSE				: Message Adaption for Device Manager  IND

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

	REMARKS				: 

**************************************************************/


void dm_dealedKey(void)
{
	keyProtectCnt_Dm--;
	if (keyProtectCnt_Dm < 0)
		mmi_trace(1, "warning invalid value keyProtectCnt_Dm = %d\n", keyProtectCnt_Dm);
}

#ifdef __ADP_MULTI_SIM__
U8 ADP_GetCfwDmSetting(void)
{
	CFW_COMM_MODE mode[ADP_SIM_NUMBER];
	U8 set = 0;
	U16 i = 0;
	memset(mode, 0, sizeof(mode));
	
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_GetComm(&mode1, CFW_SIM_0);
	UT_CFW_GetComm(&mode2, CFW_SIM_1);
#else
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		CFW_GetComm(&(mode[i]), i);
		mmi_trace(g_sw_ADP_NW,"ADP_GetCfwDmSetting, mode[%d] is %d",i, mode[i]);
		set |= (mode[i]<<i);
	}
#endif

#if defined(__UT_SIMU_DM_DUAL_MODE__)
	set = DM_DUAL_MODE;

#elif defined(__UT_SIMU_DM_ONLY_SIM1__)
	set = DM_ONLY_CARD1;

#elif defined(__UT_SIMU_DM_ONLY_SIM2__)
	set = DM_ONLY_CARD2;

#elif defined(__UT_SIMU_FLIGHT_MODE__)
	set = DM_FLIGHT_MODE;

#endif
	#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
	set = 0x0F;
	#endif
	mmi_trace(g_sw_ADP_NW, "ADP_GetCfwDmSetting, set is 0x%x", set);
	return set;
}

#endif


/**************************************************************

	FUNCTION NAME		: ADP_DispatchDmIndMsg 

  	PURPOSE				: Message Adaption for Device Manager  IND

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

	REMARKS				: 

**************************************************************/
#ifdef __TFLASH_MOUNT_DYNAMIC__            
extern void ADP_FlashDetect(UINT16 is_on);
#endif
static BOOL ADP_DispatchDmIndMsg(COS_EVENT* pCosEvent)
{
	TBM_ENTRY(0x29b8);
	ASSERT(pCosEvent != NULL);
	
	switch(pCosEvent->nEventId)
	{	
		case EV_TIMER:
		{			
			//add by chenq for bug 5889 and 5800, 20070803 B
	              if(pCosEvent->nParam1 == PHBLIST_COPY_OR_DELETE_TIMEROUTID)
	              {
	                g_phb_copyordelete_error_flag = TRUE;
	                PHBListCopyOrDeleteTimerOutErrorFUN();            
	              }
			break;
		}
		case EV_DM_GPADC_IND:   //Added by jinzh:20070620
		{
#ifdef __FF_AUDIO_SET__
                  extern void SendMessageForEarphoneKey();
                  extern void SendMessageForEarphonePlugIn();
                  extern void SendMessageForEarphonePlugOut();
                  
                 Earphone_vol=(UINT16)((pCosEvent->nParam1)&0xFFFF);
                 mmi_trace(g_sw_ADP_AUD, "Earphone detect p1=%d,p2=%d,p3=%d\n", Earphone_vol,((pCosEvent->nParam2)&0xFFFF),((pCosEvent->nParam3)&0xFFFF));
                
            // check if MIC_DET press down before check earphone detect
                if(pmd_GetEarModeStatus() != 1 && GetHandsetInPhone() == 1)
                    {
                        if(MAXVOLT_FOR_EARPHONE_PRESSKEY >= Earphone_vol)	
                		{
                			KeyDown = TRUE;
                		} 
                        else if((KeyDown == TRUE) 
                                &&(MAXVOLT_EARPHONE_IN_PHONE >= Earphone_vol 
                			    && MINVOLT_EARPHONE_IN_PHONE <= Earphone_vol))
                		{
                			KeyDown = FALSE;
                			SendMessageForEarphoneKey();
                		}
                    }
                
                    if(((pCosEvent->nParam3)&0x0F)==3)  // pCosEvent->nParam3 == 0 using interupt detect, 3 using GPADC detect
                    {                        
                        if((MAXVOLT_EARPHONE_IN_PHONE >= Earphone_vol 
                    			&& MINVOLT_EARPHONE_IN_PHONE <= Earphone_vol) 
                    			&& GetHandsetInPhone() == 0)//低电平为耳机插入
                    		if(1 == Earphone_flag)
                    		{
                    		    Earphone_flag = 0;
                    			SetHandsetInPhone(1);
                    			SendMessageForEarphonePlugIn();
                                mmi_trace(g_sw_ADP_AUD, "Earphone detect send message plugin");
                    		}
                            else
                            {
                                Earphone_flag = 1;
                            }
                        else if(MINVOLT_EARPHONE_NOT_IN_PHONE <= Earphone_vol 
                    			&& GetHandsetInPhone() == 1)  //拔下耳机
                    		if(1 == Earphone_flag)
                    		{
                    		    Earphone_flag = 0;
                                KeyDown = FALSE;
                    			SetHandsetInPhone(0);
                    			SendMessageForEarphonePlugOut();
                                mmi_trace(g_sw_ADP_AUD, "Earphone detect send message plugout");
                    		}
                            else
                            {
                                Earphone_flag = 1;
                            }
                        else
                                Earphone_flag = 0;
                            
                    }
                    else
                    {
                        UINT16 Earphone_level;
                        Earphone_level  = pCosEvent->nParam2 & 0x0f;
                        
                        if (Earphone_flag != Earphone_level)
                        {
                            Earphone_flag = Earphone_level;
                            if(Earphone_level==1)
                            {   
                                 SetHandsetInPhone(1);
                        		 SendMessageForEarphonePlugIn();
                                 mmi_trace(g_sw_ADP_AUD, "Earphone detect send message plugin");                                
                            }
                            else
                            {
                                KeyDown = FALSE;
                        		SetHandsetInPhone(0);
                        		SendMessageForEarphonePlugOut();
                                mmi_trace(g_sw_ADP_AUD, "Earphone detect send message plugOut");
                            }
                        }
                    }
                    
#endif			

	    	break;
		}
		
       	case EV_DM_POWER_ON_IND:
        {
			U32 nPoweOnMode = pCosEvent->nParam1;
//			U8 Bcs = 0;	//Battery Charging  of status
//			U8 Bcl = 0;	//Battery Charging  of level
//			U16 Mpc = 0;	//Power consumption 
			TM_SYSTEMTIME pSystemTime;
            ilm_struct  ilm_ptr; 

			mmi_eq_power_on_ind_struct *pPwrOn=OslMalloc(sizeof(mmi_eq_power_on_ind_struct));
			ASSERT(pPwrOn != NULL);
//			memset(pPwrOn, 0, sizeof(mmi_eq_power_on_ind_struct));
#if 0
            mmi_eq_battery_status_ind_struct *pbattery_status=OslMalloc(sizeof(mmi_eq_battery_status_ind_struct));
			ASSERT(pbattery_status != NULL);
			memset(pbattery_status, 0, sizeof(mmi_eq_battery_status_ind_struct));
#endif //panxu del 20071006			
              	
//			memset(&pSystemTime, 0, sizeof(TM_SYSTEMTIME));	
//			memset(&ilm_ptr, 0, sizeof(ilm_struct));
			
 

//            PM_GetBatteryInfo(&Bcs, &Bcl, &Mpc);
            TM_GetSystemTime(&pSystemTime);
//            pPwrOn->rtc_time.rtc_year = pSystemTime.uYear ;
            pPwrOn->rtc_time.rtc_mon  = pSystemTime.uMonth;
            pPwrOn->rtc_time.rtc_day  = pSystemTime.uDay;
            pPwrOn->rtc_time.rtc_hour = pSystemTime.uHour;
            pPwrOn->rtc_time.rtc_min  = pSystemTime.uMinute;
            pPwrOn->rtc_time.rtc_sec  = pSystemTime.uSecond;
            pPwrOn->rtc_time.rtc_wday = pSystemTime.uDayOfWeek;

			if (pSystemTime.uYear > 2030)
			{
				pSystemTime.uYear = 2030;
				pPwrOn->rtc_time.rtc_year = 30;
				TM_SetSystemTime(&pSystemTime);
			}
			else if (pSystemTime.uYear < 2000)
			{
				pSystemTime.uYear = 2000;
				pPwrOn->rtc_time.rtc_year = 0;
				TM_SetSystemTime(&pSystemTime);
			}
			else 
			{
				pPwrOn->rtc_time.rtc_year = pSystemTime.uYear - 2000;
			}
		
          	pPwrOn->poweron_mode = CSW2CSDPowerOn(nPoweOnMode);

#if defined (GREENSTONE_2308A_DEV)
		if(pPwrOn->poweron_mode == POWER_ON_CHARGER_IN)
		{
			pPwrOn->poweron_mode = POWER_ON_KEYPAD; //
		}
#endif	
		
		/*GUOJ Add Start For 6011 Ver: TBM780  on 2007-8-2 14:16 */

		
		if(pPwrOn->poweron_mode == POWER_ON_CHARGER_IN)
		{
			bIsChargerConnected= TRUE;
		}/*GUOJ Add End  For 6011 Ver: TBM780  on 2007-8-2 14:16 */
		else  if(pPwrOn->poweron_mode == POWER_ON_KEYPAD || pPwrOn->poweron_mode == POWER_ON_EXCEPTION) //changde by panxu 2007-8-10 for定时开机
		{
			SetBandFirstPowerOn();
			CFW_ShellControl(CFW_CONTROL_CMD_POWER_ON);
		}
		else //alarm power on, we don't use CFW_ShellControl to start stack. add by panxu 2007-8-10
		{
			//do nothing for alarm power on
		}

		TRACE_EF(1, "pPwrOn->poweron_mode = %d", pPwrOn->poweron_mode);
		
#ifdef __MMI_MULTI_SIM__
		pPwrOn->dm_setting = ADP_GetCfwDmSetting();
#else
	#ifdef __ADP_MULTI_SIM__
		if(ADP_GetCfwDmSetting() == 0)
			pPwrOn->flightmode_state = 1;
		else
			pPwrOn->flightmode_state = 0;
	#endif
#endif
		ilm_ptr.local_para_ptr = (local_para_struct *)pPwrOn;
		ilm_ptr.dest_mod_id = MOD_MMI;
		ilm_ptr.src_mod_id = MOD_L4C;
		ilm_ptr.msg_id = MSG_ID_MMI_EQ_POWER_ON_IND;
		OslMsgSendExtQueue(&ilm_ptr);

		break;
		}
		
    	//zough add for alarm 20061109 begin
    	case EV_TIM_ALRAM_IND:
       	{
       		ilm_struct  ilm_almind;
			ilm_almind.dest_mod_id = MOD_MMI;
			ilm_almind.src_mod_id = MOD_L4C;    
			ilm_almind.local_para_ptr = NULL;
			ilm_almind.peer_buff_ptr = NULL;
			ilm_almind.msg_id = MSG_ID_MMI_EQ_ALARM_IND;               
			OslMsgSendExtQueue(&ilm_almind);  	

			break;
		}
		
		//zough add for alarm 20061109 end
       	case EV_KEY_DOWN:
    	case EV_KEY_PRESS:
    	case EV_KEY_UP:
       	{
       		S16 KeyMapIndex;  // zrx add 20060621
       		ilm_struct  ilm_ptr;
			static U8 flag_up = 0;
			static U8 max_keys = ( sizeof(nKeyPadMap)/sizeof(nKeyPadMap[0]));
			msg_kbd_data  *msg_key;
			memset(&ilm_ptr,0,sizeof(ilm_ptr));

			if(keyProtectCnt_Dm>=10 )
			{

				if ((flag_up != 1) && (pCosEvent->nEventId == EV_KEY_UP)) /*flag_up is add by panxu*/
				{
					flag_up = 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				flag_up = 0;
			}

			/*to asure the key queue to mmi is less than 10+++*/
       		msg_key = OslMalloc(sizeof(msg_kbd_data));
			
			//COS_WaitForSemaphore(protectKeyCnt,COS_WAIT_FOREVER); 
			//COS_ReleaseSemaphore(protectKeyCnt);
			/*to asure the key queue to mmi is less than 10---*/			
			
       		ilm_ptr.local_para_ptr = (local_para_struct *)msg_key ;
       		ilm_ptr.dest_mod_id = (module_type)MOD_MMI;
			ilm_ptr.src_mod_id = MOD_L4C;
			ilm_ptr.msg_id = MSG_ID_MMI_EQ_KEYPAD_DETECT_IND;
			msg_key->Keycode = (S16)pCosEvent->nParam3;  	
			mmi_trace(1,"msg_key->Keycode is %c",msg_key->Keycode);
       		// zrx add beign 20060621
       		for(KeyMapIndex=0;KeyMapIndex<max_keys;KeyMapIndex++)
       		{
       			if (msg_key->Keycode== nKeyPadMap[KeyMapIndex].nKeyCode)
                {	
					msg_key->Keycode =nKeyPadMap[KeyMapIndex].nMMIKeyCode;
#if defined(__MMI_SCREEN_ROTATE__)
					if(mmi_frm_is_screen_width_height_swapped()){
						switch(msg_key->Keycode){
							case KEY_DOWN_ARROW:
								msg_key->Keycode = KEY_RIGHT_ARROW;
								break;
							case KEY_UP_ARROW:
								msg_key->Keycode = KEY_LEFT_ARROW;
								break;
							case KEY_LEFT_ARROW:
								msg_key->Keycode = KEY_DOWN_ARROW;
								break;
							case KEY_RIGHT_ARROW:
								msg_key->Keycode = KEY_UP_ARROW;
								break;
						}
					}
#endif
					break;
       			}
    		}
			mmi_trace(1,"KeyMapIndex is %d",KeyMapIndex);
			mmi_trace(1,"msg_key->Keycode is %d",msg_key->Keycode);

       		if (KeyMapIndex>= max_keys)
       			msg_key->Keycode =KEY_INVALID;

			// zrx add end
			if (pCosEvent->nEventId==EV_KEY_DOWN)
			{
				/*WeiD Add Start For 6068 Ver: TBM780  on 2007-8-3 19:38 */
				if( (msg_key->Keycode ==KEY_SEND) ||
					(msg_key->Keycode ==KEY_CAMERA)
					/*||(msg_key->Keycode ==KEY_ENTER) delete for bug 17613*/)
				{
					U32 		oldtime_down;
					static U32 	key_filter_down_time = 0;
					oldtime_down = key_filter_down_time;
					key_filter_down_time =csw_TMGetTick();

					if((key_filter_down_time - oldtime_down) < (MMI_TICK1S >> 1))  //if near key is shorter than 0.5sec,we dispose it
					{
						key_filter_down_time = 0;
				            	if(msg_key != NULL)
				            	{
				             	 	OslMfree(msg_key);
				              		msg_key = NULL;
				            	}
						break;
					}
				}
				/*WeiD Add End  For 6068 Ver: TBM780  on 2007-8-3 19:38 */
				msg_key->Keyevent = KEY_EVENT_DOWN;
			}
			else if (pCosEvent->nEventId==EV_KEY_UP)
			{
				
				/*WeiD Add Start For 6068 Ver: TBM780  on 2007-8-3 19:38 */
				if((msg_key->Keycode ==KEY_SEND) ||
					(msg_key->Keycode ==KEY_CAMERA)
					/*||(msg_key->Keycode ==KEY_ENTER) delete for bug 17613*/)
				{
					U32 		oldtime_up;
					static U32 	key_filter_up_time = 0;
					oldtime_up = key_filter_up_time;
					key_filter_up_time =csw_TMGetTick();

					if((key_filter_up_time - oldtime_up) < (MMI_TICK1S >> 1))
					{
						key_filter_up_time = 0;
					            if(msg_key != NULL)
					            {
					              OslMfree(msg_key);
					              msg_key = NULL;
					            }
						break;
					}
				}
				/*WeiD Add End  For 6068 Ver: TBM780  on 2007-8-3 19:38 */
			
				msg_key->Keyevent = KEY_EVENT_UP;
			
			}
			else if (pCosEvent->nEventId==EV_KEY_PRESS && pCosEvent->nParam2==1)
			{
// hal_key does NOT give KEY_PRESS
                MMI_ASSERT(0);
//				TRACE_SYS_FUNC("(pCosEvent->nEventId == EV_KEY_PRESS && pCosEvent->nParam2 == 1)");
//				msg_key->Keyevent = KEY_EVENT_LONG_PRESS;
			}
			keyProtectCnt_Dm++;
       		OslMsgSendExtQueue(&ilm_ptr); 
       		break;	
       	}
#ifdef __TFLASH_MOUNT_DYNAMIC__            
		case EV_DM_GPIO_IND:   
                    if(pCosEvent->nParam2 == 0xff)
                    {
                        if(pCosEvent->nParam1 == 1)
                        {
                           ADP_FlashDetect(1);
                        }
                        else
                        {
                            ADP_FlashDetect(0);
                        }
                    }
                break;     
#endif
		default:
			// TODO: 单纯关闭下面的代码可能会有问题!
			//ProtocolEventHandler(Message.oslMsgId,(void*)Message.oslDataPtr,(int)Message.oslSrcId, (void*) Message.oslPeerBuffPtr );
			TBM_EXIT(0x29b8);
			return FALSE;
	}
		TBM_EXIT(0x29b8);
	return TRUE;
}

#if 0
/**************************************************************

	FUNCTION NAME		: ADP_DispatchDmReqMsg 

  	PURPOSE				: Message Adaption for Device Manager  REQ

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

	REMARKS				: 

**************************************************************/
static BOOL ADP_DispatchDmReqMsg(COS_EVENT* pCosEvent)
{
	ASSERT(pCosEvent != NULL);

	return TRUE;
}
#endif

extern void ToSetSpeakerMode(void);
extern void ToSetEarpieceMode();
extern void ToSetHandSetMode();
extern void GetAudioParam(void);
extern void GetAudioProfile(void);
extern void ToSetBluetoothMode();

static BOOL ADP_DispatchUemReqMsg(COS_EVENT* pCosEvent)
{
	TBM_ENTRY(0x29b5);
	ASSERT(pCosEvent != NULL);
	
	switch (pCosEvent->nEventId)
	{
	   	//zough add for alarm 20061109 begin
	   	case MSG_ID_MMI_EQ_EXE_RTC_TIMER_REQ:
	   	{
	   		ilm_struct *Message = NULL;
	   		mmi_eq_set_rtc_time_req_struct *dataPtr = NULL;
	   		UINT8 AlarmNum;
	   		TM_ALARM  Alarm1;
	   		TM_SYSTEMTIME almsystemtime;
	   		TM_FILETIME  FileTime;
			ilm_struct  ilm_ptr;
			mmi_eq_set_rtc_time_rsp_struct *alarmRsp;

	   		Message = (ilm_struct *)pCosEvent->nParam1;
	   		dataPtr = (mmi_eq_set_rtc_time_req_struct *)Message->local_para_ptr;

	   		almsystemtime.uDay = dataPtr->info.data_time.rtc_day;
	   		almsystemtime.uDayOfWeek = dataPtr->info.data_time.rtc_wday;
	   		almsystemtime.uHour = dataPtr->info.data_time.rtc_hour;
	   		almsystemtime.uMinute = dataPtr->info.data_time.rtc_min;
	   		almsystemtime.uMonth = dataPtr->info.data_time.rtc_mon;
	   		almsystemtime.uYear = dataPtr->info.data_time.rtc_year + YEARFORMATE;
	   		almsystemtime.uSecond = dataPtr->info.data_time.rtc_sec;
	   		almsystemtime.uMilliseconds = 0;

	   		TM_SystemTimeToFileTime (&almsystemtime,&FileTime);

			Alarm1.nTextLength = 13;
			Alarm1.nType = 0;
			Alarm1.pText = "I love you!!";
			Alarm1.nFileTime = FileTime.DateTime;
			Alarm1.nIndex = 1;
			Alarm1.nRecurrent = 0;
				
			alarmRsp = (mmi_eq_set_rtc_time_rsp_struct *)
				OslMalloc(sizeof(mmi_eq_set_rtc_time_rsp_struct));
			ASSERT(alarmRsp != NULL);

			ilm_ptr.dest_mod_id = MOD_MMI;
			ilm_ptr.src_mod_id = MOD_L4C;    
			ilm_ptr.local_para_ptr = (local_para_struct *)alarmRsp;
			ilm_ptr.peer_buff_ptr = NULL;
			ilm_ptr.msg_id = MSG_ID_MMI_EQ_EXE_RTC_TIMER_RSP;     

			if (TM_SetAlarm( &Alarm1, &AlarmNum)==TRUE)
			{
				alarmRsp->result = MMI_TRUE;
				OslMsgSendExtQueue(&ilm_ptr);  	
			}
			else
			{
				alarmRsp->result = MMI_FALSE;
				OslMsgSendExtQueue(&ilm_ptr);  	
			}
		}		
			break;

		case MSG_ID_MMI_EQ_EXE_DEL_RTC_TIMER_REQ:
		{
			ilm_struct ilm_delalm;

		ilm_delalm.src_mod_id = MOD_L4C;
		ilm_delalm.dest_mod_id = MOD_MMI;
		ilm_delalm.local_para_ptr = NULL;
		ilm_delalm.peer_buff_ptr = NULL;
		ilm_delalm.msg_id =  MSG_ID_MMI_EQ_EXE_DEL_RTC_TIMER_RSP;

		if (TM_ClearAllAlarm() == TRUE)
			OslMsgSendExtQueue(&ilm_delalm);
	}

	break;

	//zough add for alarm 20061109 end

	case MSG_ID_MMI_EQ_POWER_OFF_REQ:
	{
		BOOL PowerOff;
		BOOL bFPO = FALSE;
		mmi_eq_power_off_rsp_struct *p = NULL;
		ilm_struct Message;
		memset(&Message, 0, sizeof(Message));


		p = (mmi_eq_power_off_rsp_struct*) OslMalloc(sizeof(mmi_eq_power_off_rsp_struct));
		ASSERT( p != NULL);

		if (pCosEvent->nEventId == MSG_ID_MMI_EQ_POWER_OFF_REQ)
			TRACE_SYS_FUNCTION();
		
		CFW_CfgCheckFirstPowerOnFlag(&bFPO);
		mmi_trace(TRUE," Func: %s bFPO:%d", __FUNCTION__, bFPO);
		if (bFPO)
			CFW_CfgSetFirstPowerOnFlag(FALSE);

		PowerOff = TRUE;

		if (PowerOff == TRUE)
			p->result = TRUE;
		else
			p->result = FALSE;

		Message.src_mod_id = MOD_L4C;

		Message.dest_mod_id = MOD_MMI;

		Message.msg_id = MSG_ID_MMI_EQ_POWER_OFF_RSP;

		Message.local_para_ptr = (local_para_struct *)p;

		Message.peer_buff_ptr = NULL;

			OslMsgSendExtQueue(&Message);
		}
			break;
			
		case MSG_ID_MMI_EQ_POWER_ON_REQ:
		{
			/*dongwg add for alarm power on 20070726 start*/
			ilm_struct *Message = NULL;
	   		mmi_eq_power_on_req_struct *dataPtr = NULL;
			U32 nPoweOnMode;
			U8 Bcs;
			U8 Bcl;
			U16 Mpc;
			mmi_eq_power_on_ind_struct *p;

		    TM_SYSTEMTIME pSystemTime;
		    ilm_struct  ilm_ptr; 
	   		Message = (ilm_struct *)pCosEvent->nParam1;
	   		dataPtr = (mmi_eq_power_on_req_struct *)Message->local_para_ptr;

/*panxu Del Start For 定时开机 Ver:    on 2007-8-10 16:22 */
			#if 0
			
			if (dataPtr->fun == 2)
			{
				DM_Reset();
				TBM_EXIT(0x29b5);
				return;
			}
			#endif
/*panxu Del End  For 定时开机 Ver:    on 2007-8-10 16:22 */
			/*dongwg add for alarm power on 20070726 end*/
			nPoweOnMode = DM_POWRN_ON_CAUSE_KEY;
			Bcs = 0;
			Bcl = 0;
			Mpc = 0;
			
		p = OslMalloc(sizeof(mmi_eq_power_on_ind_struct));
			ASSERT(p != NULL);
			memset(p, 0, sizeof(mmi_eq_power_on_ind_struct));


			/*guoj Add Start For 6047 Ver: TBM780  on 2007-8-3 14:55 */
			SetBandFirstPowerOn();
			CFW_ShellControl(CFW_CONTROL_CMD_POWER_ON);
			/*guoj Add End  For 6047 Ver: TBM780  on 2007-8-3 14:55 */
			
			TM_GetSystemTime(&pSystemTime);
			PM_GetBatteryInfo(&Bcs, &Bcl,&Mpc);
			
			switch (Bcl)
			{
				case 0 :
					p->battery_level = BATTERY_LOW_POWEROFF;
					break;
				case 20:
					p->battery_level = BATTERY_LEVEL_0;
					break;        	      	            
				case 40:
					p->battery_level = BATTERY_LEVEL_1;
					break; 	      	      
				case 60:
					p->battery_level= BATTERY_LEVEL_1;
					break;
				case 80:
					p->battery_level= BATTERY_LEVEL_2;
					break;
				case 100:
					p->battery_level = BATTERY_LEVEL_3;
					break;

			}
	      	p->rtc_time.rtc_year = pSystemTime.uYear ;
	      	p->rtc_time.rtc_mon =pSystemTime.uMonth;
	      	p->rtc_time.rtc_day =pSystemTime.uDay;
	      	p->rtc_time.rtc_hour =pSystemTime.uHour;
	      	p->rtc_time.rtc_min =pSystemTime.uMinute;
	      	p->rtc_time.rtc_sec =pSystemTime.uSecond;
	      	p->rtc_time.rtc_wday =pSystemTime.uDayOfWeek;
			if (pSystemTime.uYear > 2030)
			{
				pSystemTime.uYear = 2030;
				p->rtc_time.rtc_year = 30;
				TM_SetSystemTime(&pSystemTime);
			}
			else if (pSystemTime.uYear < 2000)
			{
				pSystemTime.uYear = 2000;
				p->rtc_time.rtc_year = 0;
				TM_SetSystemTime(&pSystemTime);
			}
			else 
			{
				p->rtc_time.rtc_year = pSystemTime.uYear - 2000;
			}
/*Add the process to get the initial value of p->dm_setting by liuxl on 20090611 */
#ifdef __MMI_MULTI_SIM__
		p->dm_setting = ADP_GetCfwDmSetting();
#else
	#ifdef __ADP_MULTI_SIM__
		if(ADP_GetCfwDmSetting() == 0)
			p->flightmode_state = 1;
		else
			p->flightmode_state = 0;
	#endif
#endif

	      	p->poweron_mode = CSW2CSDPowerOn(nPoweOnMode);
			ilm_ptr.local_para_ptr = (local_para_struct *)p;    
	   		ilm_ptr.dest_mod_id = MOD_MMI;
	    	ilm_ptr.src_mod_id = MOD_L4C;
	    	ilm_ptr.msg_id = MSG_ID_MMI_EQ_POWER_ON_IND;
			//MSG_ID_MMI_EQ_POWER_ON_REQ
	    	OslMsgSendExtQueue(&ilm_ptr); 
		}	
			break;
			
		case    MSG_ID_MMI_EQ_EXE_GPIO_LEVEL_REQ:
		{
			ilm_struct *Message = NULL;
			mmi_eq_exe_gpio_level_req_struct *pReq = NULL;

			Message = (ilm_struct *)pCosEvent->nParam1;
			pReq = (mmi_eq_exe_gpio_level_req_struct *)Message->local_para_ptr;
			
			//UINT8  level = 0;//wangzl:now this is only for LED_KEY

			//TRACE_EF(g_sw_SYSTEM, "MSG_ID_MMI_EQ_EXE_GPIO_LEVEL_REQ:type=%d;level=%d", pReq->gpio_dev_type, pReq->gpio_dev_level);
			
			if (pReq->gpio_dev_level < LED_LIGHT_LEVEL_MAX)
			{
				//level = pReq->gpio_dev_level * 25; // TODO: ...需要进一步确认这个改动
				switch (pReq->gpio_dev_type)
				{
					case GPIO_DEV_LED_MAINLCD:
						//TRACE_SYS_FUNC("GPIO_DEV_LED_MAINLCD");
						//DRV_SetPwl0(level);//remove by guoh
						/*Wangzl Modify Start For 5860 Ver: TBM_780  on 2007-7-31 16:13 */
						#ifdef MMI_ON_HARDWARE_P
						if(pReq->gpio_dev_level == LED_LIGHT_LEVEL5)
						{
							lcd_bright_level(MAIN_LCD,7);
						}
						else
						{
                            lcd_bright_level(MAIN_LCD,pReq->gpio_dev_level);
						}
						#endif
						/*Wangzl Modify End  For 5860 Ver: TBM_780  on 2007-7-31 16:13 */
						break;
					case GPIO_DEV_LED_KEY:
              //TRACE_SYS_FUNC("GPIO_DEGPIO_DEV_LED_KEY");
              // old delete by dingjian 20071010 for CS621
              //DRV_SetPwl1(pReq->gpio_dev_level * 25);
              if(!pReq->gpio_dev_level)
              {
                  DRV_SetKeyBacklight(__KEYPADBACKLIGHT_DARK__); //chenhe change for greenstone
              }
              else
              {
                  DRV_SetKeyBacklight(__KEYPADBACKLIGHT_LIGHT__);
              }
						break;
				}		
			}
			//wangzl:add 函数中的8 代表virbator,使用它是因为当前代表virbator的宏的值不能正确代表其gpio口的值
			if(GPIO_DEV_VIBRATOR==pReq->gpio_dev_type)
			{
		/*	wangrui modify 2008.10.19 begin for fixbug 9693*/
	//                        mmi_trace(1,"chenhe,vibrator test,pReq->gpio_dev_type is %d",pReq->gpio_dev_type);

			#ifdef MMI_ON_HARDWARE_P
                 pmd_SetLevel(PMD_LEVEL_VIBRATOR, pReq->gpio_dev_level);
			#endif


		/*	wangrui modify 2008.10.19 end*/
				}
			//wangzl:add E
		}
			break;
			 
		case   MSG_ID_MMI_EQ_SET_VOLUME_REQ:
			break;

		case    MSG_ID_MMI_EQ_SET_MUTE_REQ:    //Modified by jinzh:20070617
		{
			ilm_struct *Message = NULL;
	   		mmi_eq_set_mute_req_struct *setMuteReq=NULL;

	   		Message = (ilm_struct *)pCosEvent->nParam1;
			ASSERT(Message != NULL);
				
		setMuteReq = (mmi_eq_set_mute_req_struct *)Message->local_para_ptr;
			ASSERT(Message != NULL);

			if(setMuteReq->audio_device == DM_DEVICE_MICROPHONE)
			{
				if(setMuteReq->mute_onoff)
				{
					DM_SetAudioControl(DM_AUD_IOCTL_MUTE_MIC,0);//静音
				}
				else
				{
					DM_SetAudioControl(DM_AUD_IOCTL_UNMUTE_MIC,0);//非静音
				}
			}
			else if(setMuteReq->audio_device == DM_DEVICE_SPEAKER)
			{
				if(setMuteReq->mute_onoff)
				{
					DM_SetAudioControl(DM_AUD_IOCTL_MUTE_SPK,0);//静音
				}
				else
				{
					DM_SetAudioControl(DM_AUD_IOCTL_UNMUTE_SPK,0);//非静音
				}
			}
			else
			{
				mmi_trace(g_sw_CC,"Func: %s MSG_ID_MMI_EQ_SET_MUTE_REQ FAIL", __FUNCTION__);		
			}
		}

			break;

		case    MSG_ID_MMI_EQ_SET_SILENT_MODE_REQ:
			break;

		case    MSG_ID_MMI_EQ_SET_GPIO_LEVEL_REQ:
			break;

		case    MSG_ID_MMI_EQ_GET_GPIO_LEVEL_REQ:
			break;

		case    MSG_ID_MMI_EQ_PLAY_AUDIO_REQ:
			break;
			 
		case   MSG_ID_MMI_EQ_STOP_AUDIO_REQ:
			break;
			
		case   MSG_ID_MMI_EQ_PLAY_AUDIO_STREAM_REQ:
			break;

		case   MSG_ID_MMI_EQ_STOP_AUDIO_STREAM_REQ:
			break;

		case   MSG_ID_MMI_EQ_PLAY_EXT_IMELODY_REQ:
			break;

		case   MSG_ID_MMI_EQ_SET_SPEECH_MODE_REQ:
			break;

		case   MSG_ID_MMI_EQ_PLAY_AUDIO_BY_NAME_REQ:
			break;

		case   MSG_ID_MMI_EQ_STOP_AUDIO_BY_NAME_REQ:
			break;

		case   MSG_ID_MMI_EQ_SET_AUDIO_PROFILE_REQ:
			break;

		case   MSG_ID_MMI_EQ_GET_AUDIO_PROFILE_REQ:
			/*Wangzl Add Start For 6348 Ver: TBM780  on 2007-8-22 9:11 */
			{
				GetAudioProfile();
			}
			/*Wangzl Add End  For 6348 Ver: TBM780  on 2007-8-22 9:11 */
			break;

		case   MSG_ID_MMI_EQ_SET_AUDIO_PARAM_REQ:
			break;

		case   MSG_ID_MMI_EQ_GET_AUDIO_PARAM_REQ:
			/*Wangzl Add Start For 6348 Ver: TBM780  on 2007-8-22 9:12 */
			{
				GetAudioParam();
			}
			/*Wangzl Add End  For 6348 Ver: TBM780  on 2007-8-22 9:12 */
			break;

		case   MSG_ID_MMI_EQ_START_ADC_ALL_CHANNEL_REQ:
			break;

		case   MSG_ID_MMI_EQ_STOP_ADC_ALL_CHANNEL_REQ:
			break;

		case   MSG_ID_MMI_EQ_SET_AUDIO_MODE_REQ:   //jinzh 20070713
#ifdef __FF_AUDIO_SET__  
		{
			ilm_struct *Message = NULL;
	   		mmi_eq_set_audio_mode_req_struct *myMsgPtr=NULL;

	   		Message = (ilm_struct *)pCosEvent->nParam1;
			ASSERT(Message != NULL);
				
	   		myMsgPtr = (mmi_eq_set_audio_mode_req_struct*)Message->local_para_ptr;
			ASSERT(myMsgPtr != NULL);
						
			if(myMsgPtr->mode == AUD_MODE_LOUDSPK)
			{

				ToSetSpeakerMode();//免提模式
		
			}
			
			else if(myMsgPtr->mode == AUD_MODE_NORMAL)			//听筒模式
			{
				ToSetEarpieceMode();
			}
			else if(myMsgPtr->mode == AUD_MODE_HEADSET)			//耳机模式
			{
				ToSetHandSetMode();
			}
#ifdef __MMI_BT_SUPPORT__					
			else if(myMsgPtr->mode == AUD_MODE_BLUETOOTH)			//Bluetooth mode
			{
				ToSetBluetoothMode();
			}	
#endif	


		}
#endif 
			break;
			 
		case   MSG_ID_MMI_EQ_PLAY_PATTERN_REQ:
		{
			ilm_struct *oslMessage = (ilm_struct *) pCosEvent->nParam1;
			mmi_eq_play_pattern_req_struct *REQ_dataPtr = (mmi_eq_play_pattern_req_struct *)oslMessage->local_para_ptr;
			TRACE_EF(g_sw_AUDIO, "REQ_dataPtr->action = %d", REQ_dataPtr->action);
			 
			switch (REQ_dataPtr->action)
			{
// dingjian 20071224 update mmi
#ifdef __VIBRATOR__
				case PLAY_ONCE:
				{
					g_IsVibrate=TRUE;
    				vibrator_open();//modify by dingj 20070926
					StartTimer(VIBRATE_TIME_ONCE_ID, VIBRATE_TIME_DURATION, VibratePlayOnce);
				}
					break;

				case PLAY_REPEAT:
				{
					TRACE_AUD_FUNC("PLAY_REPEAT");
					g_IsVibrate=TRUE;
    				vibrator_open();//modify by dingj 20070926
					StartTimer(VIBRATE_TIME_REPEAT_ID, VIBRATE_TIME_DURATION, VibratePlayRepeat);
				}
					break;

				case PLAY_STOP:
				{	
					TRACE_AUD_FUNC("zhendong    ---------------STOP");
					g_IsVibrate=FALSE;
					StopTimer(VIBRATE_TIME_REPEAT_ID);
					vibrator_close();//modify by dingj 20070926
				}
#endif // __VIBRATOR__		
					break;
                case PLAY_DUR:
                {
					TRACE_AUD_FUNC("PLAY_DUR");
					g_IsVibrate=TRUE;
					vibrator_open();//modify by dingj 20070926
					StartTimer(VIBRATE_TIME_DUR_ID, REQ_dataPtr->dur, VibratePlaydur);
                }
                    break;
				default:
					break;
		   	}
		}
	   		break;
			 
		case   MSG_ID_MMI_EQ_SET_HW_LEVEL_REQ:
			/*Wangzl Add Start For 6367 Ver: TBM780  on 2007-8-22 10:25 */
			{
				//添加实际设置函数

				//添加实际设置函数
				{
					mmi_eq_set_hw_level_rsp_struct *set_hw_level_rsp;
					ilm_struct  ilm_ptr;
					
    					set_hw_level_rsp = (mmi_eq_set_hw_level_rsp_struct *)OslMalloc(sizeof(mmi_eq_set_hw_level_rsp_struct));
					ASSERT(set_hw_level_rsp != NULL);
					
					memset(set_hw_level_rsp, 0, sizeof(mmi_eq_set_hw_level_rsp_struct));
					set_hw_level_rsp->result = TRUE;
					//修改result值为真正的设置结果
					//添加cause值的设置
					
					ilm_ptr.local_para_ptr = (local_para_struct *)set_hw_level_rsp;
					ilm_ptr.dest_mod_id = MOD_MMI;
					ilm_ptr.src_mod_id = MOD_L4C;
					ilm_ptr.msg_id = MSG_ID_MMI_EQ_SET_HW_LEVEL_RSP;
					OslMsgSendExtQueue(&ilm_ptr);
				}
			}
			/*Wangzl Add End  For 6367 Ver: TBM780  on 2007-8-22 10:25 */
			break;

		case   MSG_ID_MMI_EQ_GET_HW_LEVEL_REQ:
			break;

		case   MSG_ID_MMI_EQ_SET_SLEEP_MODE_REQ:
			{
				/*Wangzl Add Start For 6373 Ver: TBM780  on 2007-9-10 11:49 */
				ilm_struct *oslMessage=(ilm_struct *) pCosEvent->nParam1;
				mmi_eq_set_sleep_mode_req_ind_struct * Req_dataPtr=(mmi_eq_set_sleep_mode_req_ind_struct *)oslMessage->local_para_ptr;
				SetBacklightMode(Req_dataPtr);
			
				{
					mmi_eq_set_sleep_mode_rsp_struct *set_sleep_mode_rsp;
					ilm_struct  ilm_ptr;
					
    					set_sleep_mode_rsp = (mmi_eq_set_sleep_mode_rsp_struct *)OslMalloc(sizeof(mmi_eq_set_sleep_mode_rsp_struct));
					ASSERT(set_sleep_mode_rsp != NULL);
					
					memset(set_sleep_mode_rsp, 0, sizeof(mmi_eq_set_sleep_mode_rsp_struct));
					set_sleep_mode_rsp->result = (kal_bool)TRUE;
					ilm_ptr.local_para_ptr = (local_para_struct *)set_sleep_mode_rsp;
					ilm_ptr.dest_mod_id = MOD_MMI;
					ilm_ptr.src_mod_id = MOD_L4C;
					ilm_ptr.msg_id = MSG_ID_MMI_EQ_SET_SLEEP_MODE_RSP;
					OslMsgSendExtQueue(&ilm_ptr);
				}
				/*Wangzl Add End  For 6373 Ver: TBM780  on 2007-9-10 11:49 */
			}
			break;
			
		case   MSG_ID_MMI_EQ_GET_MS_IMEI_REQ:
			break;

		case   MSG_ID_MMI_EQ_SET_MS_IMEI_REQ:
			break;

		case   MSG_ID_MMI_EQ_RTC_POWERON_STATE_REQ:
			break;
			 
	     
		case    MSG_ID_MMI_EQ_SET_RTC_TIME_REQ:
		{
			ilm_struct *oslMessage=(ilm_struct *) pCosEvent->nParam1;
			mmi_eq_set_rtc_time_req_struct * REQ_dataPtr=(mmi_eq_set_rtc_time_req_struct *)oslMessage->local_para_ptr;
			ilm_struct  ilm_ptr;
			TM_SYSTEMTIME pSystemTime;
			mmi_cc_set_max_acm_rsp_struct  *RSP_localPtr= OslMalloc(sizeof(mmi_cc_set_max_acm_rsp_struct));				   
			pSystemTime.uYear=REQ_dataPtr->info.data_time.rtc_year + 2000;//+1970;
			pSystemTime.uMonth=REQ_dataPtr->info.data_time.rtc_mon;  
			pSystemTime.uDay=REQ_dataPtr->info.data_time.rtc_day;
			pSystemTime.uHour=REQ_dataPtr->info.data_time.rtc_hour;
			pSystemTime.uMinute=REQ_dataPtr->info.data_time.rtc_min;
			pSystemTime.uSecond=REQ_dataPtr->info.data_time.rtc_sec;                 
			pSystemTime.uDayOfWeek=REQ_dataPtr->info.data_time.rtc_wday;                
			ilm_ptr.dest_mod_id = MOD_MMI;                
			ilm_ptr.src_mod_id = (module_type)MOD_L4C;    
			ilm_ptr.local_para_ptr = (local_para_struct *)RSP_localPtr;   

			if (TM_SetSystemTime(&pSystemTime) == TRUE)  /*与文挡说明不一致*/
				RSP_localPtr->result=1;   
			else
				RSP_localPtr->result=0;   

			ilm_ptr.msg_id = MSG_ID_MMI_EQ_SET_RTC_TIME_RSP;  
			OslMsgSendExtQueue(&ilm_ptr);      
		}
	   		break;
	   
		case  MSG_ID_MMI_EQ_GET_RTC_TIME_REQ:
		{
			ilm_struct *pMsg = NULL;
			mmi_eq_get_rtc_time_req_struct *temp;
			TM_SYSTEMTIME pSystemTime;
			mmi_eq_get_rtc_time_rsp_struct *msg_p;
			ilm_struct ilm_ptr;
			BOOL return_flag = FALSE;

      pMsg = (ilm_struct*) pCosEvent->nParam1;
			temp = (mmi_eq_get_rtc_time_req_struct *)pMsg->local_para_ptr;

			if (temp->rtc_type== 0)    //RTC_TIME_CLOCK_IND
				return_flag = TM_GetSystemTime(&pSystemTime);
			else if (temp->rtc_type == 1)      //RTC_ALARM_IND
				;

			if (return_flag == TRUE)
			{
				msg_p = OslMalloc(sizeof(mmi_eq_get_rtc_time_rsp_struct));
				ilm_ptr.dest_mod_id = MOD_MMI;
				ilm_ptr.src_mod_id = MOD_L4C;
				ilm_ptr.peer_buff_ptr =NULL;
				ilm_ptr.local_para_ptr = (local_para_struct*)msg_p;
				ilm_ptr.msg_id = MSG_ID_MMI_EQ_GET_RTC_TIME_RSP;
				msg_p->result = 0;
				msg_p->num = 0x80;
				msg_p->rtctime[0].alarm_format = 0;
				msg_p->rtctime[0].alarm_index = 0;
				msg_p->rtctime[0].type =0;
				msg_p->rtctime[0].recurr = 0;
				msg_p->rtctime[0].data_time.rtc_day =pSystemTime.uDay;
				msg_p->rtctime[0].data_time.rtc_hour = pSystemTime.uHour;
				msg_p->rtctime[0].data_time.rtc_min = pSystemTime.uMinute;
				msg_p->rtctime[0].data_time.rtc_sec = pSystemTime.uSecond;
				msg_p->rtctime[0].data_time.rtc_wday = pSystemTime.uDayOfWeek;
				msg_p->rtctime[0].data_time.rtc_mon = pSystemTime.uMonth;
				msg_p->rtctime[0].data_time.rtc_year = pSystemTime.uYear - 2000; 
				OslMsgSendExtQueue(&ilm_ptr);               			
			}
		}
			break;
			
		default :
			TBM_EXIT(0x29b5);
			return FALSE;
	}
	TBM_EXIT(0x29b5);
	return TRUE;
}

static BOOL ADP_DispatchUemMsg(COS_EVENT* pCosEvent)
{
	ASSERT(pCosEvent != NULL);

	if (pCosEvent->nEventId >= MSG_ID_MMI_EQ_SET_VOLUME_REQ && pCosEvent->nEventId <= MSG_ID_MMI_EQ_POWER_OFF_REQ)
		return ADP_DispatchUemReqMsg(pCosEvent);

	 #if 0
 	 else if (pCosEvent->nEventId >= EV_DM_BASE && pCosEvent->nEventId <= EV_DM_END_)
	 {
		ADP_DispatchUemRspMsg(pCosEvent);
		return TRUE;
	 } 
	 #endif
	 /*Wangzl Add Start For 6345 Ver: TBM780  on 2007-11-20 13:7 */
	 else if(pCosEvent->nEventId == MSG_ID_EM_START_REQ) /* MSG_ID_MMI_EM_START_REQ => MSG_ID_EM_START_REQ,zhuoxz,2009-6-18*/
	 {	
		ilm_struct *oslMessage=(ilm_struct *) pCosEvent->nParam1;
		mmi_em_start_req_struct * Req_data=(mmi_em_start_req_struct *)oslMessage->local_para_ptr;
		
		mmi_em_start_rsp_struct *em_start_rsp = NULL;
		ilm_struct  ilm_ptr;
		//添加实际功能
		//添加实际功能
		em_start_rsp = (mmi_em_start_rsp_struct *)OslMalloc(sizeof(mmi_em_start_rsp_struct));
		ASSERT(em_start_rsp != NULL);
		
		memset(em_start_rsp, 0, sizeof(mmi_em_start_rsp_struct));
		em_start_rsp->result = TRUE;
		em_start_rsp->info_request = Req_data->info_request;
		
		ilm_ptr.local_para_ptr = (local_para_struct *)em_start_rsp;
		ilm_ptr.dest_mod_id = (module_type)MOD_MMI;
		ilm_ptr.src_mod_id = MOD_L4C;
		ilm_ptr.msg_id = MSG_ID_MMI_EM_START_RSP;
		OslMsgSendExtQueue(&ilm_ptr);
		
	 	return TRUE;
	 }
	 /*Wangzl Add End  For 6345 Ver: TBM780  on 2007-11-20 13:7 */
	 /*Wangzl Add Start For 6371 Ver: TBM780  on 2007-11-20 13:7 */
	 else if(pCosEvent->nEventId == MSG_ID_MMI_EQ_GET_UART_REQ)
	 {
	 	//添加实际获得函数
	 	//添加实际获得函数
	 	mmi_eq_get_uart_rsp_struct *get_uart_rsp = NULL;
		ilm_struct ilm_ptr;
	 	get_uart_rsp = (mmi_eq_get_uart_rsp_struct *)OslMalloc(sizeof(mmi_eq_get_uart_rsp_struct));
		ASSERT(get_uart_rsp != NULL);

		memset(get_uart_rsp,0,sizeof(mmi_eq_get_uart_rsp_struct));
		//修改下列值为获得的实际返回值
		get_uart_rsp->result = TRUE;
		get_uart_rsp->ps_baud_rate = UART_BAUD_AUTO;
		get_uart_rsp->ps_uart_port = 0;
		get_uart_rsp->tst_baud_rate = UART_BAUD_AUTO;
		get_uart_rsp->tst_uart_port = 0;

		ilm_ptr.local_para_ptr = (local_para_struct *)get_uart_rsp;
		ilm_ptr.dest_mod_id = MOD_MMI;
		ilm_ptr.src_mod_id = MOD_L4C;
		ilm_ptr.msg_id = MSG_ID_MMI_EQ_GET_UART_RSP;
		OslMsgSendExtQueue(&ilm_ptr);
		
	 	return TRUE;
	 }
	 else if(pCosEvent ->nEventId == MSG_ID_MMI_EQ_SET_UART_REQ)
	 {
	 	//添加实际设置函数
	 	//添加实际设置函数
	 	mmi_eq_set_uart_rsp_struct *set_uart_rsp = NULL;
		ilm_struct ilm_ptr;
	 	set_uart_rsp = (mmi_eq_set_uart_rsp_struct *)OslMalloc(sizeof(mmi_eq_set_uart_rsp_struct));
		ASSERT(set_uart_rsp != NULL);

		memset(set_uart_rsp,0,sizeof(mmi_eq_set_uart_rsp_struct));
		//修改下列值为获得的实际返回值
		set_uart_rsp->result = TRUE;
		
		ilm_ptr.local_para_ptr = (local_para_struct *)set_uart_rsp;
		ilm_ptr.dest_mod_id = MOD_MMI;
		ilm_ptr.src_mod_id = MOD_L4C;
		ilm_ptr.msg_id = MSG_ID_MMI_EQ_SET_UART_RSP;
		OslMsgSendExtQueue(&ilm_ptr);
		
	 	return TRUE;
	 }
	 /*Wangzl Add End  For 6371 Ver: TBM780  on 2007-11-20 13:7 */
	else 
		return FALSE;
}

BOOL ADP_DispatchUemIndMsg(COS_EVENT* pCosEvent)
{
	return FALSE;
}

BOOL ADP_DispatchUemRspMsg(COS_EVENT* pCosEvent)
{
	return FALSE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchDmRspMsg

  	PURPOSE				: Message Adaption for Call Control  RSP

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

	REMARKS				: 

**************************************************************/
BOOL ADP_DispatchDmRspMsg(COS_EVENT* pCosEvent)
{
	return FALSE;
}
//BOOL ADP_DispatchNwMsg(COS_EVENT* pCosEvent)
//{
//	if (pCosEvent->nEventId >= MSG_ID_MMI_NW_GET_IMEI_REQ && pCosEvent->nEventId <= MSG_ID_MMI_NW_CFUN_STATE_REQ)
//	{
//	       return ADP_DispatchNwReqMsg(pCosEvent);
//	}
//	else if (pCosEvent->nEventId >= EV_CFW_NW_RSP_BASE && pCosEvent->nEventId <= EV_CFW_NW_RSP_END_)
//	{
//		if (ADP_DispatchNwIndMsg(pCosEvent)) 
//			return TRUE;
//		ADP_DispatchNwRspMsg(pCosEvent);
//			return TRUE;
//	}
//	else
//		return FALSE;	
//}
//
//BOOL ADP_DispatchNwIndMsg(COS_EVENT* pCosEvent)
//{
//     return FALSE;
//}
//
//BOOL ADP_DispatchNwReqMsg(COS_EVENT* pCosEvent)
//{
//     return FALSE;
//}
//
//BOOL ADP_DispatchNwRspMsg(COS_EVENT* pCosEvent)
//{
//     return FALSE;
//}
//

U8 g_MyPrevSimStatus = 0;

/*wangrui add 2008.11.3 begin for fixbug10232    */
static void SendPhnLockReq()
{
	mmi_smu_password_required_ind_struct *p = NULL;
	ilm_struct  ilm = {0};
	mmi_trace(g_sw_ADP_SMU,"MMI_SIM: Func: %s PHONELOCK", __FUNCTION__);
	
	ilm.dest_mod_id = MOD_MMI;
	ilm.src_mod_id = MOD_L4C;
	ilm.msg_id = MSG_ID_MMI_SMU_PASSWORD_REQUIRED_IND;

	MMI_MALLOC(p, mmi_smu_password_required_ind_struct);
	p->type = MMI_TYPE_PHONELOCK;
	ilm.local_para_ptr = (local_para_struct *)p;
	
	OslMsgSendExtQueue(&ilm);
}
/*wangrui add 2008.11.3 end*/

extern void InitGbInputMethod(void);

extern BOOL bNeedInit[];
#ifdef __ADP_MULTI_SIM__
extern UINT32 CFW_GetSimSPN(CFW_SIM_ID nSimID, CFW_SPN_INFO *pSPNInfo);
#else
extern UINT32 CFW_GetSimSPN(CFW_SPN_INFO *pSPNInfo);
#endif

static BOOL do_cswmsg_EV_CFW_NW_REG_STATUS_IND(COS_EVENT *pev)
{
	UINT32 param1 = pev->nParam1;
	UINT8 type = adp_GetCswEvType(pev);
	ilm_struct	ilm = {0};
	mmi_nw_attach_ind_struct *p = NULL;

	mmi_trace(g_sw_ADP_NW, "MMI_NW: do_cswmsg_EV_CFW_NW_REG_STATUS_IND. type=%d", type);
#ifdef __ADP_MULTI_SIM__
	mmi_trace(g_sw_ADP_NW, "MMI_NW: sim_id=%d", g_pCommonContext->sim_id);
#endif /* __ADP_MULTI_SIM__ */
#ifdef __UT_SIMU_FLIGHT_MODE__
	return TRUE;
#endif

	switch (type)
	{

	case 0:

		if (param1 == 0)
		{
			MMI_MALLOC(p, mmi_nw_attach_ind_struct);

			// status
			p->status = MMI_RAC_NO_CELL;

			// Fill ILM data structure.
			ilm.dest_mod_id = MOD_MMI;
		#ifdef __ADP_MULTI_SIM__
			ilm.src_mod_id = adp_SimId2IlmMod(g_pCommonContext->sim_id);
		#else
			ilm.src_mod_id = MOD_L4C;
		#endif
			ilm.msg_id = MSG_ID_MMI_NW_ATTACH_IND;
			ilm.local_para_ptr = (local_para_struct *)p;

			// Send message to L4C or L4C_2.
			OslMsgSendExtQueue(&ilm);
		}

		// TODO: need to process the case: param1==1.

		break;

	case 1:
		// TODO: confirm this case with CSW team.
		break;

	case 2:
		////////////////////////////////////////
		// 1,5
		// CFW_NW_STATUS_REGISTERED_HOME or
		// CFW_NW_STATUS_REGISTERED_ROAMING
		if (param1 == 1 || param1 == 5)
		{
			UINT32 i, cfw_return;
			UINT8 mode;
			UINT8 OperatorId[6];
			CFW_SPN_INFO SPNInfo;

			MMI_MALLOC(p, mmi_nw_attach_ind_struct);

			// status
			p->status = MMI_RAC_OK;
			if(param1 == 5)
			{
				p->gsm_state = 1;
			}

			// plmn
		#ifdef __ADP_MULTI_SIM__
			#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
			U8 nSimId = g_pCommonContext->sim_id;
			if(g_pCommonContext->sim_id > ADP_SIM_2)
				nSimId -= 2;
			cfw_return = CFW_NwGetCurrentOperator(OperatorId, &mode, nSimId); // automatic mode
			#else
			cfw_return = CFW_NwGetCurrentOperator(OperatorId, &mode, g_pCommonContext->sim_id); // automatic mode
			#endif
		#else
			cfw_return = CFW_NwGetCurrentOperator(OperatorId, &mode); // automatic mode
		#endif



			// Convert ASCII string form.

			for (i = 0; i < ARRAY_SIZE(OperatorId); i++)
			{
				if(OperatorId[i] == 0xF)
				{
				    p->plmn[i] = '\0';
				}
				else
				{
				    p->plmn[i] = OperatorId[i] + '0';
				}
			}

			// Convert to C string.
			p->plmn[ARRAY_SIZE(OperatorId)] = '\0';
                    memset(&p->spn_info, 0x00, sizeof(MMI_SPN_INFO));
                    if(param1 == 1)
                    {
                    #ifdef __ADP_MULTI_SIM__
                        CFW_GetSimSPN(g_pCommonContext->sim_id, &SPNInfo);
                    #else
                        CFW_GetSimSPN(&SPNInfo);
                    #endif

                        p->spn_info.nSPNameLen = SPNInfo.nSPNameLen;
                        if(p->spn_info.nSPNameLen > 16)
                        {
                            p->spn_info.nSPNameLen = 16;
                        }
                        
                        p->spn_info.nSPNDisplayFlag = SPNInfo.nSPNDisplayFlag & 0x01;
                        memcpy(p->spn_info.nSpnName, SPNInfo.nSpnName, p->spn_info.nSPNameLen);
                    }

			mmi_trace(g_sw_ADP_NW,"MMI_NW: Reg Func: %s plmn:%s param1 %d spnlen %d spnflag %d name %x", __FUNCTION__, p->plmn, param1, SPNInfo.nSPNameLen, SPNInfo.nSPNDisplayFlag,SPNInfo.nSpnName[0]);

			// Fill ILM data structure.
		#ifdef __ADP_MULTI_SIM__
			ilm.src_mod_id = adp_SimId2IlmMod(g_pCommonContext->sim_id);
		#else
			ilm.src_mod_id = MOD_L4C;
		#endif
			ilm.dest_mod_id = MOD_MMI;

			ilm.msg_id = MSG_ID_MMI_NW_ATTACH_IND;

			ilm.local_para_ptr = (local_para_struct *)p;

			// Send message to L4C or L4C_2.
			OslMsgSendExtQueue(&ilm);
			#if 0 //2010.11.03 now we do this after PHB init done
			/*Guojian Add Start For Call Forward Ver: GreenStone  on 2008-11-2 14:4 */
			{
				static BOOL first_in[4] = {TRUE,TRUE,TRUE,TRUE};
			#ifdef __ADP_MULTI_SIM__
				mmi_trace(1,"SS Callforward Func: %s SIM%d_first_in=%d", __FUNCTION__, g_pCommonContext->sim_id + 1, first_in[g_pCommonContext->sim_id]);
				if (first_in[g_pCommonContext->sim_id] == TRUE)
				{
					AdpInitCallFwowardingStart(g_pCommonContext->sim_id);//add used to query all call forwarding
					first_in[g_pCommonContext->sim_id] = FALSE;
				}
			#else  /* No Defined __ADP_MULTI_SIM__ */
				if (first_in[0] == TRUE)
				{
					AdpInitCallFwowardingStart();//add used to query all call forwarding
					first_in[0] = FALSE;
				}
			#endif /* __ADP_MULTI_SIM__ */
			}
			/*Guojian Add End  For Call Forward Ver: GreenStone  on 2008-11-2 14:4 */
			#else
			{
				static BOOL bFirst_in[ADP_SIM_NUMBER] = {FALSE,};
				#ifdef __ADP_MULTI_SIM__
				if(!bFirst_in[g_pCommonContext->sim_id])
				{
					bNeedInit[g_pCommonContext->sim_id] = TRUE;
					bFirst_in[g_pCommonContext->sim_id] = TRUE;
				}
				#else
				if(!bFirst_in[0])
				{
					bNeedInit[0] = TRUE;
					bFirst_in[0] = TRUE;
				}
				#endif
				
			}
			#endif
		}

		///////////////////////////////////////////
		// 0,2,3,4
		// CFW_NW_STATUS_NOTREGISTERED_NOTSEARCHING or
		// CFW_NW_STATUS_NOTREGISTERED_SEARCHING or
		// CFW_NW_STATUS_REGISTRATION_DENIED or
		// CFW_NW_STATUS_UNKNOW
		else
		{
			MMI_MALLOC(p, mmi_nw_attach_ind_struct);

			// status
			switch (param1)
			{

			case 0:

			case 4:
				p->status = MMI_RAC_NO_CELL;
				break;

			case 2:
				p->status = MMI_RAC_SEARCHING;
				break;

			case 3:
				p->status = MMI_RAC_LIMITED_SERVICE;
				break;

			default:
				MMI_ASSERT(0);
				break;
			}

			// Fill ILM data structure.
		#ifdef __ADP_MULTI_SIM__
			ilm.src_mod_id = adp_SimId2IlmMod(g_pCommonContext->sim_id);
		#else
			ilm.src_mod_id = MOD_L4C;
		#endif
			ilm.dest_mod_id = MOD_MMI;

			ilm.msg_id = MSG_ID_MMI_NW_ATTACH_IND;

			ilm.local_para_ptr = (local_para_struct *)p;

			// Send message to L4C or L4C_2.
			OslMsgSendExtQueue(&ilm);
		}
		mmi_trace(g_sw_ADP_NW,"MMI_NW: Func: %s p->status=%d", __FUNCTION__, p->status);
		
		break;

	case 3:
		// TODO: add code if GPRS supported.
		MMI_ASSERT(0);
		break;

	default:
		MMI_ASSERT(0);
		break;
	}

	return TRUE;
}

static void SimuPhnLockInd(void)
{
	static BOOL done = FALSE;

	////////////////////////////////////////
	// Just do once.
	if (!done)
	{
		SendPhnLockReq();
		done = TRUE;
	}		
}

#ifdef ADP_DUALSIM_MODE_CLOSE_NOSIM
//open_close: FALSE close
//			TRUE  open
BOOL bNoSIMHandled[ADP_SIM_NUMBER] = {FALSE,};

void ADP_InitCloseNoSIM(void)
{
	memset(bNoSIMHandled, 0, sizeof(bNoSIMHandled)/sizeof(bNoSIMHandled[0]));
}
void ADP_SimulateNoSignal(U8 nSimID)
{
	ilm_struct	ilm = {0};		
	mmi_nw_rx_level_ind_struct *p = OslMalloc(sizeof(mmi_nw_rx_level_ind_struct));
	MMI_ASSERT(p != NULL);

	ilm.local_para_ptr = (local_para_struct *)p;
	p->level = 0;
	ilm.dest_mod_id = MOD_MMI;

	ilm.src_mod_id = MOD_L4C + nSimID;
	ilm.msg_id = MSG_ID_MMI_NW_RX_LEVEL_IND;

	OslMsgSendExtQueue(&ilm);
}
void ADP_CloseNoSIM(void)
{
	U32 ret = 0;
	U8 dmSet = 0;
	U8 i;
	U8 nSIM = 0;
	CFW_COMM_MODE mod = CFW_DISABLE_COMM;
	
	dmSet = ADP_GetCfwDmSetting();

	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		if(!gSIMIsNotExist[i]&&(dmSet&(0x01<<i)))
		{
			nSIM++;
		}
	}
	mmi_trace(g_sw_ADP_NW, "ADP_CloseNoSIM, nSIM is %d", nSIM);
	if(nSIM)
	{//close all nosim
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			if(dmSet&(0x01<<i) && gSIMIsNotExist[i] && !bNoSIMHandled[i])
			{
				ret = CFW_SetComm(mod, 1, FREE_UTI, i);
				bNoSIMHandled[i] = TRUE;
				mmi_trace(g_sw_ADP_NW, "ADP_CloseNoSIM, sim%d, ret is 0x%x",i,ret);
				//move to respone handler
				//if(ret == ERR_SUCCESS)
				//{
				//	ADP_SimulateNoSignal(i);
				//}
				break;
			}
		}
	}
	else
	{
		//search first open sim
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			if(dmSet & (1<<i))
				break;
		}
		//close all other
		i++;
		for(; i < ADP_SIM_NUMBER; i++)
		{
			if((dmSet & (1 << i))&&gSIMIsNotExist[i]&& !bNoSIMHandled[i])
			{
				ret= CFW_SetComm(mod, 1, FREE_UTI, i);
				bNoSIMHandled[i] = TRUE;
				mmi_trace(g_sw_ADP_NW, "ADP_CloseNoSIM, sim%d, ret is 0x%x",i, ret);
				if(ret == ERR_SUCCESS)
				{
					ADP_SimulateNoSignal(i);
				}
				break;
			}
		}
	}
}

U8 ADP_ReopNoSIM(void)
{
	U32 ret = 0;
	U8 dmSet = 0;
	U8 i;
	U8 nSIM = 0;
	CFW_COMM_MODE mod = CFW_ENABLE_COMM;
	U8 nReopSIM = 0;
	
	dmSet = ADP_GetCfwDmSetting();


	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		if(!gSIMIsNotExist[i]&&(dmSet&(0x01<<i)))
		{
			nSIM++;
		}
	}
	mmi_trace(g_sw_ADP_NW, "ADP_ReopNoSIM, nSIM is %d", nSIM);
	if(nSIM)
	{//open all nosim
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			if(dmSet&(0x01<<i) && gSIMIsNotExist[i])
			{
				ret = CFW_SetComm(mod, 1, FREE_UTI, i);
				nReopSIM++;
				mmi_trace(g_sw_ADP_NW, "ADP_ReopNoSIM, sim%d, ret is 0x%x",i ,ret);
			}
		}
	}
	else
	{
		//search first open sim
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			if(dmSet & (1<<i))
				break;
		}
		//open all other
		i++;
		for(; i < ADP_SIM_NUMBER; i++)
		{
			if((dmSet & (1 << i))&&gSIMIsNotExist[i])
			{
				ret= CFW_SetComm(mod, 1, FREE_UTI, i);
				nReopSIM++;
				mmi_trace(g_sw_ADP_NW, "ADP_ReopNoSIM, sim%d, ret is 0x%x",i, ret);
			}
		}
	}
	
	return nReopSIM;
}
#endif
/*****************************************************************************
 * FUNCTION
 *  do_cswmsg_EV_CFW_INIT_IND
 * DESCRIPTION
 *  Process message EV_CFW_INIT_IND comes from CSW layer.
 * PARAMETERS
 *  pCosEvent: CSW message data structure.
 * RETURNS
 *  Returns true when it process this message sucessfully, else return FALSE.
 *  You need to process this message in other place when it returns FALSE.
 *****************************************************************************/
 extern void adp_SAT_SETUPMENU_ind(void);
static BOOL do_cswmsg_EV_CFW_INIT_IND(COS_EVENT *pev)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	UINT8 type = adp_GetCswEvType(pev);
	UINT32 p1 = pev->nParam1;
	UINT32 p2 = pev->nParam2;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	////////////////////////////////////////
	// Simulate phone lock IND message to MMI when got SIM status or NO-SIM status message.
	// Here we do not check whether phone lock is set, instead of doing this in MMI layer.
	if (type == CFW_INIT_STATUS_SIM || type == CFW_INIT_STATUS_NO_SIM)
		SimuPhnLockInd();
	
	switch (type)
	{

	case CFW_INIT_STATUS_SAT:
	{
		if ((p1 & CFW_INIT_SIM_SAT) == CFW_INIT_SIM_SAT)
		{
			////////////////////////////////////////
			// Simulate EV_CFW_SAT_CMDTYPE_IND message because of it is absent from CFW layer.
			pev->nEventId = EV_CFW_SAT_CMDTYPE_IND;
			pev->nParam1 = 0x25;
			pev->nParam2 = 0x00;
		#ifdef __ADP_MULTI_SIM__
			pev->nParam3 |= 0x10000;
		#else
			pev->nParam3 = 0x10000;
		#endif
		#ifdef __ADP_MULTI_SIM__
			adp_ConfigureContext(ADP_MODULE_SAT,adp_GetCSWSimID(ADP_GET_CSW_FLAG(pev->nParam3)));
		#endif
			adp_SAT_SETUPMENU_ind();
			// Returns FALSE to continue to process the simulated message in other place.
			return TRUE;
		}
		else
			mmi_trace(g_sw_ADP_SAT, "[221]SAT initialization failed! ((p1&CFW_INIT_SIM_SAT)==0)");
	}

		break;

	case CFW_INIT_STATUS_SIM:
	{
		ilm_struct  ilm = {0};
		mmi_smu_password_required_ind_struct *p = NULL;

		MMI_MALLOC(p, mmi_smu_password_required_ind_struct);
		p->chv_info.chv1_count = (LOUINT16(p2) & 0x0F);
		p->chv_info.chv2_count = ((LOUINT16(p2) & 0xF00) >> 8);
		p->chv_info.ubchv1_count = ((LOUINT16(p2) & 0xF0) >> 4);
		p->chv_info.ubchv2_count = ((LOUINT16(p2) & 0xF000) >> 12);
		p->cphs_retry_count = (LOUINT16(p2) & 0xF0) >> 4;

		if ((p1&CFW_INIT_SIM_PIN1_READY) == CFW_INIT_SIM_PIN1_READY && !((p1&CFW_INIT_SIM_WAIT_PUK1) == CFW_INIT_SIM_WAIT_PUK1))
			p->type = MMI_TYPE_NO_REQUIRED;
		else if ((p1&CFW_INIT_SIM_WAIT_PIN1) == CFW_INIT_SIM_WAIT_PIN1)
			p->type = MMI_TYPE_CHV1;
		else if ((p1&CFW_INIT_SIM_WAIT_PUK1) == CFW_INIT_SIM_WAIT_PUK1)
			p->type = MMI_TYPE_UBCHV1;
		else if ((p1&CFW_INIT_SIM_CARD_BLOCK) == CFW_INIT_SIM_CARD_BLOCK)
			p->type = MMI_TYPE_SIM_CARD_BLOCKED;
		else
			MMI_FAULT_FORK;

		ilm.msg_id = MSG_ID_MMI_SMU_PASSWORD_REQUIRED_IND;
		ilm.dest_mod_id = MOD_MMI;
		ilm.src_mod_id = ADP_COMM_MSG_SRC_MOD;
		ilm.local_para_ptr = (local_para_struct *)p;
	#ifdef __ADP_MULTI_SIM__	
		mmi_trace(g_sw_ADP_SIM, "MMI_SIM: send MSG_ID_MMI_SMU_PASSWORD_REQUIRED_IND to MMI. ->type=%d,SimId=%d", p->type, g_pCommonContext->sim_id);
	#endif
		OslMsgSendExtQueue(&ilm);

	#ifdef __ADP_MULTI_SIM__
		Adp_Calllog_Send_Start_Ind(g_pCommonContext->sim_id);	 //initialize calllog
	#else  /* No Defined __ADP_MULTI_SIM__ */
		Adp_Calllog_Send_Start_Ind();	 //initialize calllog
	#endif /* __ADP_MULTI_SIM__ */
	#ifdef __ADP_MULTI_SIM__
		Adp_Get_FDN_State(g_pCommonContext->sim_id);
	#endif	

	}

	break;

	case CFW_INIT_STATUS_AUTODAIL:
	{
		ilm_struct  ilm_ptr;

		memset(&ilm_ptr, 0, sizeof(ilm_struct));

		ilm_ptr.dest_mod_id = MOD_MMI;
	#ifdef __ADP_MULTI_SIM__	
		ilm_ptr.src_mod_id = adp_SimId2IlmMod(g_pCommonContext->sim_id);
	#else
		ilm_ptr.src_mod_id = MOD_L4C;
	#endif
		ilm_ptr.msg_id = MSG_ID_MMI_CC_FACTORY_AUTO_DAIL_IND;

		OslMsgSendExtQueue(&ilm_ptr);

	}

	break;

	case CFW_INIT_STATUS_NO_SIM:
	{
		ilm_struct  ilm_ptr;
	  	mmi_smu_fail_ind_struct *p = OslMalloc(sizeof(mmi_smu_fail_ind_struct));

		ASSERT(p != NULL);

#ifdef __ADP_MULTI_SIM__
		adp_ConfigureContext(ADP_MODULE_SMS, g_pCommonContext->sim_id);
		DummyInitSMSForNoSim(p1, p2, type);
#endif

		p->cause = MMI_SIM_NO_INSERTED;

		memset(&ilm_ptr, 0, sizeof(ilm_struct));
		ilm_ptr.dest_mod_id = MOD_MMI;
#ifdef __ADP_MULTI_SIM__
		ilm_ptr.src_mod_id = adp_SimId2IlmMod(g_pCommonContext->sim_id);
#else
		ilm_ptr.src_mod_id = MOD_L4C;
#endif
		ilm_ptr.msg_id = MSG_ID_MMI_SMU_FAIL_IND;  
		ilm_ptr.local_para_ptr = (local_para_struct *)p;

#ifdef __ADP_MULTI_SIM__	
		mmi_trace(g_sw_ADP_SIM, "MMI_SIM: send MSG_ID_MMI_SMU_FAIL_IND[MMI_SIM_NO_INSERTED] to MMI. SimId=%d", g_pCommonContext->sim_id);
#endif
		OslMsgSendExtQueue(&ilm_ptr);

			// init ME PBK  add by chenq B

#ifdef __ADP_MULTI_SIM__
		gSIMIsNotExist[g_pCommonContext->sim_id] = TRUE;
#endif

#ifdef __ADP_MULTI_SIM__
		if(flag_init_ok == FALSE)
		{
			BOOL bPHBAllInitDone = TRUE;
			U8 i;
			
			for(i = 0; i < ADP_SIM_NUMBER; i++)
			{
				if(i != g_pCommonContext->sim_id && !g_PHBSIMStartFinish[i])
				{
					bPHBAllInitDone = FALSE;
					break;
				}
			}
			if(bPHBAllInitDone)
			{
				Msg_Adaption_PHB_Init();
			}
		}
		
		
		nSIMBadOrNotInsert++;
		if(ADP_SIM_NUMBER == nSIMBadOrNotInsert)
#endif	
		{
			Msg_Adaption_PHB_Init_Ready();

			Msg_Adaption_PHB_startup_begin();

			Msg_Adaption_PHB_Init();
		}
	}
	break;

	case CFW_INIT_STATUS_SIMCARD:
	{
		mmi_trace(g_sw_ADP_SIM, "MMI_SIM: CFW_INIT_STATUS_SIMCARD +++Func: %s----p1=0x%x", __FUNCTION__,p1);

		if (p1& CFW_INIT_SIM_CARD_BAD)     //Bad SIM card or SIM card with bad PBK info.
		{
			ilm_struct  ilm_ptr;
			mmi_smu_fail_ind_struct  *p = OslMalloc(sizeof(mmi_smu_fail_ind_struct));

			ASSERT(p != NULL);
			mmi_trace(g_sw_ADP_SIM, TSTXT("MMI_SIM: CFW_INIT_STATUS_SIM: CFW_INIT_SIM_CARD_BAD \n"));

			ilm_ptr.local_para_ptr = (local_para_struct *)p;
			p->cause = MMI_SIM_ACCESS_ERROR;
			ilm_ptr.dest_mod_id = MOD_MMI;
		#ifdef __ADP_MULTI_SIM__
			ilm_ptr.src_mod_id = adp_SimId2IlmMod(g_pCommonContext->sim_id);
		#else
			ilm_ptr.src_mod_id = MOD_L4C;
		#endif
			ilm_ptr.msg_id = MSG_ID_MMI_SMU_FAIL_IND;

			OslMsgSendExtQueue(&ilm_ptr);

			#ifdef __ADP_MULTI_SIM__
			nSIMBadOrNotInsert++;
			#endif
			
			if (!flag_init_ok)
			{
				// init ME PBK
			#ifdef __ADP_MULTI_SIM__
				if(ADP_SIM_NUMBER == nSIMBadOrNotInsert)
			#endif	
				{
					Msg_Adaption_PHB_Init_Ready();

					Msg_Adaption_PHB_startup_begin();

					Msg_Adaption_PHB_Init();
				}
				// init ME PBK
			}

		}
		else if (p1& CFW_INIT_SIM_CARD_CHANGED)
		{

			mmi_trace(g_sw_ADP_SIM, TSTXT("MMI_SIM: Normal card,CFW_INIT_SIM_CARD_CHANGED %s,%s \n"),
					  (p2&0x1) == 0 ? "FDN" : "ADN",
					  ((p2&0x02) >> 1) == 0 ? "BDN disable" : "BDN enable");

#if defined(__MMI_PHB_CALLERGROUP_IN_SIM__)
			mmi_phb_init_check_sim_change();
#endif
		}
		else if (p1 & CFW_INIT_SIM_NORMAL_CARD)
		{
			mmi_trace(g_sw_ADP_SIM, TSTXT("MMI_SIM: Normal card,CFW_INIT_SIM_NORMAL_CARD %s,%s \n"),
					  (p2&0x1) == 0 ? "FDN" : "ADN",
					  ((p2&0x02) >> 1) == 0 ? "BDN disable" : "BDN enable");
		}
		else if (p1&CFW_INIT_SIM_TEST_CARD)// Test card
		{
			mmi_trace(g_sw_ADP_SIM, TSTXT("MMI_SIM: Test card\n"));
		}

	}

	break;

	case CFW_INIT_STATUS_SMS:
	{
#ifdef __ADP_MULTI_SIM__
		adp_ConfigureContext(ADP_MODULE_SMS, g_pCommonContext->sim_id);
#endif
		InitSMS(p1, p2, type);
	}

	break;

	case CFW_INIT_STATUS_PBK:
	{
		 
		UINT32 nCFWRtn = ERR_SUCCESS;
		#ifdef __ADP_MULTI_SIM__
		U8 i;
		U8 nIndReceived = 0;
		BOOL bInitOnGoing = MMI_FALSE;
		#endif
		
		#ifdef __ADP_MULTI_SIM__
		mmi_trace(g_sw_ADP_PHB, TSTXT("PHB Init CFW_INIT_STATUS_PBK SIM:%d"), g_pCommonContext->sim_id);
		adp_ConfigureContext(ADP_MODULE_PHB, g_pCommonContext->sim_id);
		#endif

	#ifdef __ADP_MULTI_SIM__
		for(i = 0; i < ADP_SIM_NUMBER; i++)
		{
			if(g_PHBSIMStartInd[i])
			{
				nIndReceived++;
				break;
			}
		}
		if(ADP_SIM_NUMBER == nIndReceived)
		{
			return TRUE;
		}
		
		if (0 == nIndReceived)
		{
	#endif
			if (p1 == 0)
			{
				Msg_Adaption_PHB_Init_Ready();
			}
	#ifdef __ADP_MULTI_SIM__
		}
	#endif
		if (p1 == 0)
		{
			mmi_trace(g_sw_ADP_PHB, TSTXT("PHB Init CFW_INIT_STATUS_PBK p1 == 0"));

		#ifdef __ADP_MULTI_SIM__
			if(0 == nIndReceived)
			{
				Msg_Adaption_PHB_startup_begin();
			}
			
			for(i = 0; i < ADP_SIM_NUMBER; i++)
			{
				if(g_PHBSIMStartInd[i] && !g_PHBSIMStartFinish[i])
				{
					bInitOnGoing = TRUE;
					mmi_trace(g_sw_ADP_PHB, "PHB card%d init on going, pending card%d init", i, g_pCommonContext->sim_id);
				}
			}

			if(!bInitOnGoing)
			{
			#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
				nCFWRtn = UT_CFW_SimGetPbkStorage(CFW_PBK_SIM, FREE_UTI, adp_pPHBContext->nSimId);
			#else
				U8 Uti = FREE_UTI;
				#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
				Simu_Rsp_Marker_Struct simuRspMarker;
				simuRspMarker.msgID = EV_CFW_SIM_GET_PBK_STRORAGE_INFO_RSP;
				simuRspMarker.nSimID = adp_pPHBContext->nSimId;
				simuRspMarker.Uti = Uti;
				simuRspMarker.pNext = NULL;
				adp_simu_add_Rsp_Marker(&simuRspMarker);
				#endif
				nCFWRtn = CFW_SimGetPbkStrorageInfo(CFW_PBK_SIM, Uti, gPHBCurrentSimID);
			#endif
			}
			
			g_PHBSIMStartInd[adp_pPHBContext->nSimId] = TRUE;
			
			if(nCFWRtn != ERR_SUCCESS)
			{
				ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);
			}
		#else
			nCFWRtn = CFW_SimGetPbkStorage(CFW_PBK_SIM, FREE_UTI);
			Msg_Adaption_PHB_startup_begin();
			if(nCFWRtn != ERR_SUCCESS)
			{
				Msg_Adaption_PHB_Init();
			}
		#endif
			mmi_trace(g_sw_ADP_PHB, TSTXT("PHB Init CFW_INIT_STATUS_PBK CFW_SimGetPbkStorage = 0x%x"), nCFWRtn);
		}
		else
		{
		#ifdef __ADP_MULTI_SIM__
			g_PHBSIMStartInd[adp_pPHBContext->nSimId] = TRUE;
			ADP_PhbInitFinishOneCardStartAnother(adp_pPHBContext->nSimId);		
		#endif				
		}
	}

	break;

	default:
		break;

	}

	return TRUE;
}

//[221] Added by ZXB 2009/02/23								[END]


static BOOL do_cswmsg_EV_CFW_NW_SIGNAL_QUALITY_IND(COS_EVENT *pev)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

#if 0
	//wangzl:add B don't treat with this when the phone is in sleep mode
	if (gIsInSleepMode)
		return TRUE;
#endif /* Guoj delete. It's not used now at 2010-3-26 */

#ifdef __UT_SIMU_FLIGHT_MODE__
	return TRUE;

#endif

	if (adp_GetCswEvType(pev) == 0)
	{
		ilm_struct	ilm = {0};		
		mmi_nw_rx_level_ind_struct *p = OslMalloc(sizeof(mmi_nw_rx_level_ind_struct));
		MMI_ASSERT(p != NULL);

		ilm.local_para_ptr = (local_para_struct *)p;
		p->level = pev->nParam1;
		ilm.dest_mod_id = MOD_MMI;
//[221] Added by ZXB 2009/02/13 							[BEGIN]
#ifdef __ADP_MULTI_SIM__
		ilm.src_mod_id = adp_SimId2IlmMod(g_pCommonContext->sim_id);
#else
		ilm.src_mod_id = MOD_L4C;
#endif
//[221] Added by ZXB 2009/02/13 							[END]
		ilm.msg_id = MSG_ID_MMI_NW_RX_LEVEL_IND;

		OslMsgSendExtQueue(&ilm);
	}
	else
		mmi_trace(g_sw_ADP_NW, TSTXT("EV_CFW_NW_SIGNAL_QUALITY_IND: ERROR!!! \n"));

	return TRUE;
}


static BOOL do_cswmsg_EV_CFW_SIM_STATUS_IND(COS_EVENT *pev)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	
	ilm_struct	ilm = {0};
	mmi_smu_fail_ind_struct  *p = OslMalloc(sizeof(mmi_smu_fail_ind_struct));

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	ASSERT(p != NULL);

	ilm.local_para_ptr = (local_para_struct *)p;

	if (pev->nParam1 == 0)
		p->cause = MMI_SIM_CMD_SUCCESS;
	else if (pev->nParam1 == 1)
		p->cause = MMI_SIM_CARD_REMOVED;

	ilm.dest_mod_id = MOD_MMI;
#ifdef __ADP_MULTI_SIM__
	ilm.src_mod_id = adp_SimId2IlmMod(g_pCommonContext->sim_id);
#else
	ilm.src_mod_id = MOD_L4C;
#endif
	ilm.msg_id = MSG_ID_MMI_SMU_FAIL_IND;

	OslMsgSendExtQueue(&ilm);

	mmi_trace(g_sw_ADP_COMMON, TSTXT("EV_CFW_SIM_STATUS_IND: %s \n"), pev->nParam1 == 0 ? "CMD_SUCCESS" : "CARD_REMOVED");

	return TRUE;
}


static BOOL ADP_DispatchMiscCfwIndMsg(COS_EVENT* pCosEvent)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	
	BOOL r = FALSE;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	
	TBM_ENTRY(0x29b0);

	ASSERT(pCosEvent != NULL);

#ifdef __ADP_MULTI_SIM__
	adp_ConfigureContext(ADP_MODULE_COMMON, adp_GetCSWSimID(ADP_GET_CSW_FLAG(pCosEvent->nParam3)));
#endif

	r = ADP_EventHandler(pCosEvent);

	#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
	adp_simu_add_new_msg(pCosEvent);
	#endif

	TBM_EXIT(0x29b0);

	return r;
}

static BOOL ADP_DispatchMiscCfwMsg(COS_EVENT* pCosEvent)
{
  	if (pCosEvent->nEventId >= EV_CFW_IND_BASE && pCosEvent->nEventId <= EV_CFW_IND_END_)
	{
		return ADP_DispatchMiscCfwIndMsg(pCosEvent);
	}
  	else
  		return FALSE;
}

 
//dyj add start 20060615
BOOL ADP_DispatchSmuReqMsg(COS_EVENT* pCosEvent)
{
	switch(pCosEvent->nEventId)
	{

	case MSG_ID_MMI_SMU_GET_IMSI_REQ:
#ifdef __ADP_MULTI_SIM__		
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
		UT_CFW_SimGetProviderId(FREE_UTI, ADP_SIM_1);
#else
		CFW_SimGetProviderId(FREE_UTI, ADP_SIM_1);
#endif
#else
		CFW_SimGetProviderId(FREE_UTI);
#endif
		break;

	case MSG_ID_MMI_SMU_LOCK_REQ:
		break;

	case MSG_ID_MMI_SMU_RESET_DATA_REQ:
		break;

	case MSG_ID_MMI_SMU_GET_DIAL_MODE_REQ:
		break;

	case MSG_ID_MMI_SMU_VERIFY_PIN_REQ:
		break;

	case MSG_ID_MMI_SMU_GET_PIN_TYPE_REQ:
		break;

	case MSG_ID_MMI_SMU_SET_PREFERRED_OPER_LIST_REQ:
		break;

	case MSG_ID_MMI_SMU_GET_PREFERRED_OPER_LIST_REQ:
		break;

	case MSG_ID_MMI_SMU_SET_PUC_REQ:
		break;

	case MSG_ID_MMI_SMU_GET_PUC_REQ:
		break;

	case MSG_ID_MMI_SMU_CHANGE_PASSWORD_REQ:
		break;

	case MSG_ID_MMI_SMU_RESTRICTED_ACCESS_REQ:
		break;

	case MSG_ID_MMI_SMU_SET_DIAL_MODE_REQ:
		break;

	case MSG_ID_MMI_SMU_WRITE_SIM_REQ:
		break;

	case MSG_ID_MMI_SMU_READ_SIM_REQ:
		break;

	case MSG_ID_MMI_SMU_POWER_OFF_REQ:
		break;

	case MSG_ID_MMI_SMU_REMOVE_REQ:
		break;

	case MSG_ID_MMI_SMU_READ_FILE_INFO_REQ:
		break;

	case MSG_ID_MMI_SMU_CHECK_PIN_STATUS_REQ:
		break;
	}

	return TRUE;
}

BOOL ADP_DispatchSmuMsg(COS_EVENT* pCosEvent)
{
	if (pCosEvent->nEventId >= MSG_ID_MMI_SMU_GET_IMSI_REQ && pCosEvent->nEventId <= MSG_ID_MMI_SMU_CHECK_PIN_STATUS_REQ)
	{
		ADP_DispatchSmuReqMsg(pCosEvent);
		return TRUE;
	}
		return FALSE;
}

extern PUBLIC UINT32 hal_TimGetUpTime(VOID);

// In idle, report low bat warning to MMI every 60s
// In call, report it every second.
// Please refer LBAT_IDLE_DURCNT_NORMAL and LBAT_TALK_DURCNT_NORMAL
#define LOW_BAT_WARNING_TIME_OUT_IDLE  (60 *16384 ) // 60s Don't change!
#define LOW_BAT_WARNING_TIME_OUT_CALL  (1 *16384 )  // 1s  Don't change!

UINT32 nPMParam1 = 0xFFFFFFFF;
UINT32 startTime = 0xFFFFFFFF;
static BOOL ADP_DispatchPmIndMsg(COS_EVENT* pCosEvent)
{
	mmi_eq_battery_status_ind_struct *battery_status;
	ilm_struct ilm_ptr;
	UINT32 nParam1;
	UINT32 nParam2;

	ASSERT(pCosEvent != NULL);
	

	
	nParam1 = pCosEvent->nParam1;
	nParam2 = pCosEvent->nParam2;
	battery_status = OslMalloc(sizeof(mmi_eq_battery_status_ind_struct));

	TBM_ENTRY(0x29b1);
	ASSERT(battery_status != NULL);
	
	ilm_ptr.local_para_ptr = (local_para_struct *)battery_status;
	ilm_ptr.dest_mod_id    = MOD_MMI;
	ilm_ptr.src_mod_id     = MOD_L4C;    
	ilm_ptr.msg_id         = MSG_ID_MMI_EQ_BATTERY_STATUS_IND;
	ChgrUpdateScrCallback(LOUINT16(nParam1));
	switch(HIUINT16(nParam1))
	{
		case 0:   
			if (bIsChargerConnected)
			{
				battery_status->battery_status = PMIC_CHARGER_OUT;
				bIsChargerConnected = FALSE;
			}
			else
			{
				battery_status->battery_status = PMIC_VBAT_STATUS;
			}
			break;
		case 1:
			battery_status->battery_status = PMIC_CHARGER_IN;
			bIsChargerConnected = TRUE;
			break;
		case 2:
			OslMfree(battery_status);
			battery_status = NULL;
			TBM_EXIT(0x29b1);
			return TRUE;
			break;
		case 3:
			battery_status->battery_status = PMIC_CHARGE_COMPLETE;
			break;
		case 4:// 4 means time out
			battery_status->battery_status = PMIC_CHARGE_COMPLETE;
			break;
		case 5:
			battery_status->battery_status = PMIC_OVERBATTEMP;
			break;
		default:
			mmi_trace(g_sw_PM, "Unknown charger connecting status! status=%d", HIUINT16(nParam1));
			break;
	}

	switch(LOUINT16(nParam2))
	{
		case PM_BATTERY_STATE_NORMAL:
      #ifdef MMI_ON_WIN32
  			  if(LOUINT16(nParam1) == 0) 
					battery_status->battery_voltage = BATTERY_LEVEL_2;
				else if((LOUINT16(nParam1) > 0) && (LOUINT16(nParam1) <21))
					battery_status->battery_voltage = BATTERY_LOW_WARNING;
				else if((LOUINT16(nParam1) > 20) && (LOUINT16(nParam1) < 41))
					battery_status->battery_voltage = BATTERY_LEVEL_0;
				else if((LOUINT16(nParam1) > 40) && (LOUINT16(nParam1) < 61))
					battery_status->battery_voltage = BATTERY_LEVEL_1;
				else if((LOUINT16(nParam1) > 60) && (LOUINT16(nParam1) < 81))
					battery_status->battery_voltage = BATTERY_LEVEL_2;
				else if((LOUINT16(nParam1) > 80) && (LOUINT16(nParam1) < 101))
					battery_status->battery_voltage = BATTERY_LEVEL_3;
#else
#if defined (__MMI_NOKIA_STYLE_N800__) && !defined (TGT_GALLITE_R400)
			switch(LOUINT16(nParam1)) //generally 0% = 3.2V , 100% = 4.2V
			{
				case  0 ... 15: // 3.35 power off   
					battery_status->battery_voltage = BATTERY_LOW_POWEROFF;
					break;
				case 16 ... 20: // 3.40 extreme low warning; Can NOT make calls, but SMS is OK
					battery_status->battery_voltage = BATTERY_LOW_POWEROFF;
					break;        	      	            
				case 21 ... 25: // 3.45 warning
					battery_status->battery_voltage = BATTERY_LOW_POWEROFF;
					break;        	      	            
				case 26 ... 30://3.50
					battery_status->battery_voltage = BATTERY_LOW_POWEROFF;
					break; 	      	      
				case 31 ... 35://3.55
					battery_status->battery_voltage = BATTERY_LOW_POWEROFF;
					break;
				case 36 ... 40://3.60
					battery_status->battery_voltage = BATTERY_LOW_WARNING;
					break;
				case 41 ... 45:
					battery_status->battery_voltage = BATTERY_LEVEL_1;
					break;
				case 46 ... 50:
					battery_status->battery_voltage = BATTERY_LEVEL_2;
					break;
				case 51 ... 60:
					battery_status->battery_voltage = BATTERY_LEVEL_3;
					break;
				case 61 ... 70:
					battery_status->battery_voltage = BATTERY_LEVEL_4;
					break;
				case 71 ... 100:
					battery_status->battery_voltage = BATTERY_LEVEL_5;
					break;
			}
#else
			switch(LOUINT16(nParam1)) //generally 0% = 3.2V , 100% = 4.2V
			{
				case  0 ... 15: // 3.35 power off   
					battery_status->battery_voltage = BATTERY_LOW_POWEROFF;
					break;
				case 16 ... 20: // 3.40 extreme low warning; Can NOT make calls, but SMS is OK
					battery_status->battery_voltage = BATTERY_LOW_TX_PROHIBIT;
					break;        	      	            
				case 21 ... 25: // 3.45 warning
					battery_status->battery_voltage = BATTERY_LOW_WARNING;
					break;        	      	            
				case 26 ... 30:
					battery_status->battery_voltage = BATTERY_LEVEL_0;
					break; 	      	      
				case 31 ... 40:
					battery_status->battery_voltage = BATTERY_LEVEL_1;
					break;
				case 41 ... 50:
					battery_status->battery_voltage = BATTERY_LEVEL_2;
					break;
				case 51 ... 60:
					battery_status->battery_voltage = BATTERY_LEVEL_3;
					break;
				case 61 ... 70:
					battery_status->battery_voltage = BATTERY_LEVEL_4;
					break;
				case 71 ... 100:
					battery_status->battery_voltage = BATTERY_LEVEL_5;
					break;
			}
 #endif     
 #endif     
			break;
// CSW will send "PM_BATTERY_STATE_NORMAL" ONLY, so it is no need to handle other cases.
#if 0
		case PM_BATTERY_STATE_LOW      :
			battery_status->battery_voltage = BATTERY_LOW_WARNING;
			break;
		case PM_BATTERY_STATE_CRITICAL :
			battery_status->battery_voltage = BATTERY_LOW_WARNING;
			break;
		case PM_BATTERY_STATE_SHUTDOWN :
			#ifndef DEV_BOARD_NO_SHUTDOWN_ON_LOW_BATTERY
			battery_status->battery_voltage = BATTERY_LOW_POWEROFF;
			#else
			battery_status->battery_voltage = BATTERY_LEVEL_2;
			#endif
			break;
#endif
	}
	
	mmi_trace(g_sw_ADP_COMMON, TSTXT("PM IND: nParam1[0x%x] battery_status->battery_voltage[0x%x]\n"), nParam1,battery_status->battery_voltage);
	mmi_trace(g_sw_ADP_COMMON, TSTXT("PM IND: nPMParam1[0x%x]\n"), nPMParam1);

// If bal_volt is low poweroff, send it to MMI anyway.
    if((nPMParam1 == (battery_status->battery_voltage | (nParam1 & 0xFFFF0000))) && (battery_status->battery_voltage != BATTERY_LOW_POWEROFF))
    { // battery_voltage is the same as the one of last time
        if(BATTERY_LOW_WARNING < battery_status->battery_voltage)
        {
            OslMfree(battery_status);
            TBM_EXIT(0x29b1);
            return TRUE;
        }
        else
        {
            if(hal_TimGetUpTime() - startTime < (GetTotalCallCount() ? LOW_BAT_WARNING_TIME_OUT_CALL : LOW_BAT_WARNING_TIME_OUT_IDLE))
            {
                OslMfree(battery_status);
                TBM_EXIT(0x29b1);
                return TRUE;
            }
            else
            { // Pop low battary warning, and reset startTime
                startTime = hal_TimGetUpTime();
            }
        }
    }
    else
    { // new battery_voltage
        nPMParam1 = battery_status->battery_voltage | (nParam1 & 0xFFFF0000);
        if(BATTERY_LOW_WARNING >= battery_status->battery_voltage)
        {
            startTime = hal_TimGetUpTime();
        }
    }

	OslMsgSendExtQueue(&ilm_ptr); 
	TBM_EXIT(0x29b1);
	return TRUE;
}

static BOOL ADP_DispatchPmMsg(COS_EVENT* pCosEvent)
{
    if (pCosEvent->nEventId == EV_PM_BC_IND)
        return ADP_DispatchPmIndMsg(pCosEvent);
    return FALSE;
}

//dyj add end

//zhoumn added for setting Message @2007/03/07<<<start
mmi_cc_set_cug_rsp_struct g_SetCUG;
/**************************************************************

	FUNCTION NAME		: Call_Setting_CUG_Rsp

  	PURPOSE				: send RSP to MMI layer
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL Call_Setting_CUG_Rsp(COS_EVENT *pCosEvent)
{
	ilm_struct SendMes;
	TBM_ENTRY(0x29b4);
	ASSERT(pCosEvent != NULL);

	// Send RSP message to MMI layer.
	memset(&g_SetCUG, 0, sizeof(g_SetCUG));
	g_SetCUG.result = TRUE;
	SendMes.dest_mod_id = MOD_MMI;
	SendMes.src_mod_id = MOD_L4C;
	SendMes.msg_id = PRT_MMI_CC_SET_CUG_RSP;
	SendMes.local_para_ptr = (local_para_struct *)&g_SetCUG;
	SendMes.peer_buff_ptr = NULL;
	
	OslMsgSendExtQueue(&SendMes);
	TBM_EXIT(0x29b4);
	return TRUE;
}

/**************************************************************

	FUNCTION NAME		: ADP_DispatchSettingMsg

  	PURPOSE				: Recevied adaption Message
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL ADP_DispatchSettingMsg(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	
	if (pCosEvent->nEventId == PRT_MMI_CC_SET_CUG_REQ)
		return Call_Setting_CUG_Rsp(pCosEvent);
	else 
		return FALSE;
}


/**************************************************************

	FUNCTION NAME		: ADP_DispatchRMCMsg

  	PURPOSE				: Recevied remote PC Message
	INPUT PARAMETERS	: COS_EVENT *pCosEvent

	OUTPUT PARAMETERS	:

	RETURNS				: void

	REMARKS			: 

**************************************************************/
static BOOL ADP_DispatchRMCMsg(COS_EVENT *pCosEvent)
{
#ifdef MMI_ON_HARDWARE_P
	ilm_struct SendMes;
       mmi_rmc_pc_ind_struct *p;
	ASSERT(pCosEvent != NULL);
       if(pCosEvent->nEventId == EV_RMT_APP_REQ){
        	// Send RSP message to MMI layer.
               p = (mmi_rmc_pc_ind_struct *)OslMalloc(sizeof(mmi_rmc_pc_ind_struct));
        	ASSERT(p != NULL);     
               p->command = pCosEvent->nParam1;
               p->para = pCosEvent->nParam2;
        	SendMes.dest_mod_id = MOD_MMI;
        	SendMes.src_mod_id = MOD_L4C;
        	SendMes.msg_id = MSG_ID_RMC_REQ_ID;
        	SendMes.local_para_ptr = (local_para_struct *)p;
        	SendMes.peer_buff_ptr = NULL;
        	mmi_trace(1, "--> <MSG_ID_RMC_REQ_ID 0x%x>",MSG_ID_RMC_REQ_ID);
        	OslMsgSendExtQueue(&SendMes);

        	return TRUE;
        }
       else{
            return FALSE;
        }
#else
       return FALSE;
#endif
    
}


//zhoumn added for setting Message @2007/03/07>>>end

//dyj add start 20060609
/**************************************************************

	FUNCTION NAME		: ADP_DispatchMessage 

  	PURPOSE				: Message Adaption for all messages

	INPUT PARAMETERS	: COS_EVENT* pCosEvent

	OUTPUT PARAMETERS	: nil

	RETURNS				: void 

	REMARKS			: Very modue may have free function(req,ind,rsp),
	                                           
                            If the value of msg is in the range of  one modue, it will return value of TURE.
**************************************************************/

//[221] Added by ZXB 2009/02/04								[BEGIN]
// <Code modification/append/erasement reason...>
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
static BOOL adp_IsIndMsg(COS_EVENT *pev)
{
	ASSERT(pev != NULL);

	return
		pev->nEventId >= EV_CFW_IND_BASE && pev->nEventId <= EV_CFW_IND_END_ ?
		TRUE :
		FALSE;
}

#endif
//[221] Added by ZXB 2009/02/04								[END]

//[221] Added by ZXB 2009/02/04								[END]
extern BOOL ADP_DispatchSatMsg(COS_EVENT *);
extern BOOL ADP_DispatchSsMsg(COS_EVENT *pCosEvent);
void ADP_DispatchMessage(COS_EVENT *pCosEvent)
{
	ASSERT(pCosEvent != NULL);
	if (ADP_DispatchDmMsg(pCosEvent))              //device manigment  Related Messages 
       	return;

#ifdef __GPRS_MODE__	
	if (ADP_DispatchGPRSMsg(pCosEvent))              //JIASHUO add for GPRS message,20090212
   		return;
#endif

//[221] Added by ZXB 2009/02/04								[BEGIN]
// Double the IND message in table.
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT

	if (adp_IsIndMsg(pCosEvent))
	{
		if (UT_AdpDoubleInd(pCosEvent))
		{
			mmi_trace(g_sw_ADP_COMMON, "[221]Simulated SIM-2 IND. ID=%u,P1=%u,P2=%u,P3=%u,type=%u,simID=%u",
					  pCosEvent->nEventId, pCosEvent->nParam1, pCosEvent->nParam2, pCosEvent->nParam3,
					  adp_GetCswEvType(pCosEvent), adp_GetCSWSimID(adp_GetCswEvFlag(pCosEvent)));
		}
	}

#endif
//[221] Added by ZXB 2009/02/04								[END]

	if (ADP_DispatchMiscCfwMsg(pCosEvent))           //cfw  Related Messages 
       	return;

	if (ADP_DispatchSatMsg(pCosEvent))             //Sat Control Related Messages hefeng 07.6.15
		return;

	if (ADP_DispatchPmMsg(pCosEvent))                //power manage  Related Messages
		return;
	
	if (ADP_DispatchCcMsg(pCosEvent))                //Call Control Related Messages 
		return;

	if (ADP_DispatchSmsMsg(pCosEvent))               //sms Related Messages 
		return;

	
	if (ADP_DispatchPhbMsg(pCosEvent))               //phonebook Related Messages 
		return;
	
	if (ADP_DispatchAudMsg(pCosEvent))				//Audio Related Messages
		return;
	
	if (ADP_DispatchSimMsg(pCosEvent))            	 //sim Related Messages 
		return;


	if (ADP_DispatchSsMsg(pCosEvent))               //SS Related Messages
		return;

	if (ADP_DispatchCalllogMsg(pCosEvent))          //calllog Related Messages 
		return;
	
	if (ADP_DispatchSettingMsg(pCosEvent))          //setting Related Messages 
		return;
	
	if (ADP_DispatchUemMsg(pCosEvent))              //UEM Related Messages 
		return; 
	
    if (ADP_DispatchNwMsg(pCosEvent))               // Network Related Messages 
       	return;

    if(ADP_DispatchRMCMsg(pCosEvent))
        return;
	   
	
	
	//if (ADP_DispatchSatMsg(pCosEvent))             //Sat Control Related Messages 
	//	return;

	return;
	
}
extern local_para_struct *first_push_ind_ptr;

void MMI_Free_msg_buf(ilm_struct *ilm_ptr)
{
	if (ilm_ptr == NULL)
		return;

#ifdef __MMI_SUPPORT_BLUETOOTH__
	if (ilm_ptr->local_para_ptr != NULL && ilm_ptr->local_para_ptr != first_push_ind_ptr)
#else
	if (ilm_ptr->local_para_ptr != NULL)
#endif
	{
		OslMfree(ilm_ptr->local_para_ptr);
		ilm_ptr->local_para_ptr = NULL;
	}

	OslMfree(ilm_ptr);
	ilm_ptr = NULL;
}

extern void MMI_InitCswFS(void);
 
BOOL MMI_InitCfw(VOID)
{
	#ifdef __ADP_MULTI_SIM__
	U8 i;
	#endif
	//
	// Configure CFW...
	//
	MMI_InitCswFS(); //wufasong added 2006.11.01
	
#ifdef __ADP_MULTI_SIM__
#ifdef ADP_CFW_DUALSIM_SIMULATOR_SUPPORT
	UT_CFW_CfgSetSmsFormat(1, ADP_SIM_1);
	UT_CFW_CfgSetSmsFormat(1, ADP_SIM_2);
	UT_CFW_CfgSetSmsShowTextModeParam(1, ADP_SIM_1);
	UT_CFW_CfgSetSmsShowTextModeParam(1, ADP_SIM_2);
	UT_CFW_CfgSetSmsFormat(1, ADP_SIM_1);
	UT_CFW_CfgSetSmsFormat(1, ADP_SIM_2);
	UT_CFW_CfgSetClip(1, ADP_SIM_1);
	UT_CFW_CfgSetClip(1, ADP_SIM_2);	
	UT_CFW_CfgSetNwStatus(1, ADP_SIM_1);
	UT_CFW_CfgSetNwStatus(1, ADP_SIM_2);	
#else
	for(i = 0; i < ADP_SIM_NUMBER; i++)
	{
		//
		// SMS Configure
		//
		CFW_CfgSetSmsFormat(1, i);

		CFW_CfgSetSmsShowTextModeParam(1, i);

		CFW_CfgSetSmsFormat(1, i);
		//CFW_CfgSetSmsStorage(CFW_SMS_STORAGE_SM, CFW_SMS_STORAGE_SM);

		//Save New SMS to ME, CB and SR don't store.

		//CFW_CfgSetNewSmsOption( CFW_SMS_MULTIPART_OPTION_ALL|CFW_SMS_ROUT_DETAIL_INFO,
		// CFW_CFG_RECEIVE_STORAGE(CFW_SMS_STORAGE_SM, 0, 0));

		//
		// CC Configure
		//
		CFW_CfgSetClip(1, i);
		//
		// NW Configure
		//
		CFW_CfgSetNwStatus(1, i);
		//
		// Pbk Configure
		//
		// ...
	}
#endif	
#else
	CFW_CfgSetSmsFormat(1);
	CFW_CfgSetSmsShowTextModeParam(1);
	CFW_CfgSetSmsFormat(1);
	CFW_CfgSetClip(1);
	CFW_CfgSetNwStatus(1);	
#endif
	//
	// Don't hook the CFW Events.
	// so the CFW RSP/IND events will inform to MMI Default Task(BAL_MmiTask).
	//
	CFW_BalSetEventPorc(NULL, NULL);

    return TRUE;
}

//==============================================================
// MMI_InitFlashFS           2007-6-14       shizh         create
//==============================================================
void MMI_InitFlashFS()
{
    INT32 iRet = 0;
    UINT8 home[32] = {0};

    AnsiiToUnicodeString(home, "/home");
    OpenedList_SemInit();
    g_hasMountedFlashFS = FALSE;
    iRet = FS_MakeDir(home, 0);
    if ( iRet != ERR_SUCCESS )
    {      

        TRACE_EF(1, "[MMI_InitFlash] FS_MakeDir(home) Exist!"
        " Fg_hasMountedFlashFS = %d", g_hasMountedFlashFS);
    }    
    //return;
#if 1 //changed by gdm to display flash menu  ERR_FS_HAS_FORMATED
    iRet = FS_HasFormatted("vds1", FS_TYPE_FAT) ;
    if (ERR_FS_HAS_FORMATED != iRet)
    {
         TRACE_EF(1, "[MMI_InitFlash]  FS_HasFormatted return value = %d. ",iRet);
        iRet = FS_Format("vds1", FS_TYPE_FAT, FS_FORMAT_QUICK);
        if ( iRet != ERR_SUCCESS )
        {
            g_hasMountedFlashFS = FALSE;  // shizh add 2007-6-14           
            TRACE_EF(1, "[MMI_InitFlash] format vds1 failed! "
            "g_hasMountedFlashFS = %d", g_hasMountedFlashFS);
            return;
        }
        else
        {           
            TRACE_EF(1, "[MMI_InitFlash] format vds1 success! ");
        }
    }
    else
    {
        TRACE_EF(1, "[MMI_InitFlash] vds1 has formated. ");
    }

    
    // mount flash fs to /home dir
    TRACE_EF(1, "[MMI_InitFlash] mount flash fs >>>>>><<<<<<");

    iRet = FS_Mount( "vds1", home, 0, FS_TYPE_FAT );
    if ( iRet == ERR_SUCCESS )
    {
        g_hasMountedFlashFS = TRUE;	 // shizh add 2007-6-14 //add by panxu not to display flash menu//changed by gdm to display flash menu081208            
        TRACE_EF(1, "[MMI_InitFlash] /home dir has been mounted successfully !"
        " g_hasMountedFlashFS= %d", g_hasMountedFlashFS);    
    }
    else
    {
        TRACE_EF(1, "[MMI_InitFlash] /home dir has been mounted fail !"
        " g_hasMountedFlashFS= %d", g_hasMountedFlashFS);    
    }
#endif
    return;

}

//==============================================================
// MMI_InitTFlash      2007-6-14       shizh         create
//==============================================================

void MMI_InitTFlash()
{
	INT32 iRet = 0;
	UINT8 tflash[20] = {0};
	INT i = 0;

	/*WeiD Modify Start For 6208 Ver: TBM780  on 2007-8-17 15:0 */
	AnsiiToUnicodeString(tflash, TFLASH_ROOT);
	/*WeiD Modify End  For 6208 Ver: TBM780  on 2007-8-17 15:0 */

	iRet = FS_MakeDir(tflash, 0);

	if ( iRet != ERR_SUCCESS )
	{
		if ( iRet != ERR_FS_FILE_EXIST )
		{
			TRACE_EF(1, "[MMI_InitTFlash]tfalsh dir isn't exist ! create dir & mount failed! ");    
			return;
		}
        TRACE_EF(1, "[MMI_InitTFlash] tfalsh dir has exist ! ");
	}

	iRet = FS_HasFormatted("mmc0", FS_TYPE_FAT);
        
	switch ( iRet )
	{
		case ERR_FS_OPEN_DEV_FAILED: //打不开设备（卡没插上）
			g_tflashCardExist = FALSE;
			TRACE_EF(1, "[MMI_InitTFlash] Can't open device(Tflash card)!. ");
			return;

		case ERR_FS_UNKNOWN_FILESYSTEM://不支持的文件系统，需要格式化
			g_tflashCardExist = TRUE;
			TRACE_EF(1, "[MMI_InitTFlash] ERR_FS_UNKNOWN_FILESYSTEM : File system is not supported.");
			return;

		case ERR_FS_NOT_SUPPORT://不支持的文件系统，需要格式化
			g_tflashCardExist = TRUE;
			TRACE_EF(1, "[MMI_InitTFlash] ERR_FS_NOT_SUPPORT  : not supported fat32.");
			return;

		case ERR_FS_NOT_FORMAT:	//卡没格式化，需要格式化
			g_tflashCardExist = TRUE;
			TRACE_EF(1, "[MMI_InitTFlash] ERR_FS_NOT_FORMAT : No format!");
			return;

		case ERR_FS_HAS_FORMATED:

			g_tflashCardExist = TRUE;

			TRACE_EF(1, "[MMI_InitTFlash] tfalsh FS_HAS_FORMATED,"
						" g_tflashCardExist =%d", g_tflashCardExist);

			for ( i = 0; i < 3; i++ )
			{
				TRACE_EF(1, "[MMI_InitTFlash] mount tflash card >>>>>>%d<<<<<<", i+1);

				iRet = FS_Mount( "mmc0", tflash, 0, FS_TYPE_FAT );

				if ( iRet == ERR_SUCCESS )
				{
					g_hasMountedTFlashFS = TRUE;  

					TRACE_EF(1, "[MMI_InitTFlash] tflash dir has mounted successfully !"
							 " g_hasMountedTFlashFS= %d", g_hasMountedTFlashFS);

					break;
				}
			}
            
			return;

		default:
			TRACE_EF(1, "mount tfalsh : unknown Error!");
			break;
	}
}
BOOL checkTCardExist(void)
{
    return g_hasMountedTFlashFS;
}
#if 1 //add gdm 080401
BOOL checkNandFlashExist(void)
{
    return g_hasMountedNandFlashFS;
}
#endif 

//==============================================================
#ifdef JATAAYU_SUPPORT
extern void WAP_MMS_initFlash(void);
#endif
#if defined(DUAL_TFLASH_SUPPORT)
 extern void   MMI_initTFlashSecond(void);
#endif

void MMI_InitCswFS(void)
{

	mmi_trace(g_sw_SYSTEM, "Excute func : MMI_InitCswFS()");

	MMI_InitFlashFS();     //add by panxu for camera 2007-8-10
#ifdef JATAAYU_SUPPORT
#ifndef MMI_ON_WIN32
	WAP_MMS_initFlash();
#endif
#endif

    MMI_InitTFlash(); 
#if defined(DUAL_TFLASH_SUPPORT)
     MMI_initTFlashSecond();
#endif
}

BOOL MMI_FreeEvent(COS_EVENT* pEvent)
{
	BOOL ret=TRUE;;

	//MMI msg id range
	if ((pEvent->nEventId >= MSG_ID_L4A_CODE_BEGIN) &&	(pEvent->nEventId  <= MSG_ID_L4A_CODE_BEGIN + MSG_ID_L4A_CODE_RANGE))
	{
		ilm_struct  *ilm_ptr = (ilm_struct  *)pEvent->nParam1;

		if (ilm_ptr != NULL)
		{
			if (ilm_ptr->local_para_ptr != NULL)
			{
				OslMfree(ilm_ptr->local_para_ptr);
				ilm_ptr->local_para_ptr = NULL;
			}

			OslMfree((void *)ilm_ptr);
			ilm_ptr = NULL;
			return ret;
		}
	}

	//multimedia msg id range
	if ((pEvent->nEventId >= MSG_ID_MED_CODE_BEGIN) && (pEvent->nEventId  <= MSG_ID_MED_CODE_BEGIN + MSG_ID_MED_CODE_RANGE))
	{
		ilm_struct  *ilm_ptr = (ilm_struct  *)pEvent->nParam1;

		if (ilm_ptr != NULL)
		{
			if (ilm_ptr->local_para_ptr != NULL)
			{
				OslMfree(ilm_ptr->local_para_ptr);
				ilm_ptr->local_para_ptr = NULL;
			}

			 OslMfree(ilm_ptr);
			ilm_ptr = NULL;
			return ret;
		}
	}

	//adv ansynch fs msg id range
	if ((pEvent->nEventId >= MSG_ID_FMT_MSG_CODE_BEGIN) && (pEvent->nEventId  <= MSG_ID_FMT_MSG_CODE_BEGIN + MSG_ID_FMT_MSG_CODE_RANGE))
	{
		ilm_struct  *ilm_ptr = (ilm_struct  *)pEvent->nParam1;

		if (ilm_ptr != NULL)
		{
			if (ilm_ptr->local_para_ptr != NULL)
			{
				OslMfree(ilm_ptr->local_para_ptr);
				ilm_ptr->local_para_ptr = NULL;
			}

			 OslMfree(ilm_ptr);
			ilm_ptr = NULL;
			return ret;
		}
	}
	
	//TP task to MMI msg id range
	if ((pEvent->nEventId >= 	DRIVER_MSG_CODE_BEGIN) && (pEvent->nEventId  <= 	DRIVER_MSG_CODE_BEGIN + DRIVER_MSG_CODE_RANGE))
	{
		ilm_struct  *ilm_ptr = (ilm_struct  *)pEvent->nParam1;

		if (ilm_ptr != NULL)
		{
			if (ilm_ptr->local_para_ptr != NULL)
			{
				OslMfree(ilm_ptr->local_para_ptr);
				ilm_ptr->local_para_ptr = NULL;
			}

			OslMfree(ilm_ptr);
			ilm_ptr = NULL;
			return ret;
		}
	}

	// csw msg id range
	else
	{
		 OslMfree((void*)pEvent->nParam1);
		pEvent->nParam1 = 0;
		return ret;
	}

	return TRUE;
}

#ifdef __ADP_MULTI_SIM__

UINT8 adp_GetCSWSimID(UINT8 cswFlag)
{
	if (cswFlag  <= ADP_SIM_4)
	{
		return ADP_SIM_1 + cswFlag;
	}

	return ADP_SIM_ERROR;
}

UINT8 adp_GetMMISimID(UINT8 destMod)
{
	if (destMod >= MOD_L4C && destMod <= MOD_L4C_4)
	{
		return ADP_SIM_1 + destMod - MOD_L4C;
	}
	return ADP_SIM_ERROR;
}


extern adp_sms_context_t *adp_pSMSContext;
extern adp_sms_context_t adp_gSMSContext[];
#ifdef __GPRS_MODE__
extern adp_wap_context_t    adp_gWAPContext[];
extern adp_wap_context_t    *adp_pWAPContext;
#endif

void adp_ConfigureContext(ADP_MODULE module, UINT8 simID)
{
	UINT8 index = 0;

	if (simID >= ADP_SIM_NUMBER)
	{
		return;
	}
	else
	{
		index = simID - ADP_SIM_1;
	}

	switch (module)
	{

	case ADP_MODULE_COMMON:
		g_pCommonContext = &g_CommonContext[index];
		g_pCommonContext->sim_id = index;
		break;

	case ADP_MODULE_SMS:
		adp_pSMSContext = &adp_gSMSContext[index];
		adp_pSMSContext->nSimId = index;
		gCurrentSMSSimID = adp_pSMSContext->nSimId;
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		if(gCurrentSMSSimID > ADP_SIM_2)
			gCurrentSMSSimID -= 2;
		#endif
		break;

	case ADP_MODULE_CC:
		g_pCCContext = &g_CCContext[index];
		g_pCCContext->sim_id = index;
		break;

	case ADP_MODULE_PHB:
		adp_pPHBContext = &adp_gPHBContex[index];
		adp_pPHBContext->nSimId = index;
		gPHBCurrentSimID = index;
		#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
		if(gPHBCurrentSimID > ADP_SIM_2)
			gPHBCurrentSimID -= 2;
		#endif
		break;

	case ADP_MODULE_NW:
		g_pNwContext = &g_NwContext[index];
		g_pNwContext->sim_id = index;
		break;

	case ADP_MODULE_SIM:
		g_pSIMContext = &g_SIMContext[index];
		g_pSIMContext->sim_id = index;
		break;

	case ADP_MODULE_SS:
		g_pSSContext = &g_SSContext[index];
		g_pSSContext->sim_id = index;
		break;

	case ADP_MODULE_CALL_LOG:
		g_pCHISContext = &g_CHISContext[index];
		g_pCHISContext->sim_id = index;
		break;

	case ADP_MODULE_SAT:
		g_pSATContext = &g_SATContext[index];
		g_pSATContext->sim_id = index;
		break;
#ifdef __GPRS_MODE__		
	case ADP_MODULE_WAP:
		adp_pWAPContext = &adp_gWAPContext[index];
		adp_pWAPContext->sim_id = index;
		break;
#endif		
		//Add for different modules

	default:
		ASSERT(0);
		break;
	}
}

#endif

static BOOL DefaultEventHandler(COS_EVENT *MsgStruct)
{
	mmi_trace(g_sw_ADP_COMMON, "[221]Unprocessed message: id=%d,p1=%d,p2=%d,p3=%d", 
		MsgStruct->nEventId, MsgStruct->nParam1, MsgStruct->nParam2, MsgStruct->nParam3);

	return FALSE;
}

static BOOL ExecuteCurrEventHandler(U32 EventID, COS_EVENT *MsgStruct)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
    switch(EventID)
    {
    	case EV_CFW_INIT_IND:
			Trace("CFW Init MSG EV_CFW_INIT_IND ");
			Trace("CFW Init p1:0x%x, p2:0x%x, p3:0x%x ", MsgStruct->nParam1, MsgStruct->nParam2, MsgStruct->nParam3);
			do_cswmsg_EV_CFW_INIT_IND(MsgStruct);
			break;
		case EV_CFW_NW_SIGNAL_QUALITY_IND:
			Trace("MMI_NW: MSG EV_CFW_NW_SIGNAL_QUALITY_IND ");
			Trace("MMI_NW: p1:0x%x, p2:0x%x, p3:0x%x ", MsgStruct->nParam1, MsgStruct->nParam2, MsgStruct->nParam3);
			do_cswmsg_EV_CFW_NW_SIGNAL_QUALITY_IND(MsgStruct);
			break;
		case EV_CFW_SIM_STATUS_IND:
			Trace("SIM Status MSG EV_CFW_SIM_STATUS_IND ");
			Trace("SIM Status p1:0x%x, p2:0x%x, p3:0x%x ", MsgStruct->nParam1, MsgStruct->nParam2, MsgStruct->nParam3);
			do_cswmsg_EV_CFW_SIM_STATUS_IND(MsgStruct);
			break;
		case EV_CFW_NW_REG_STATUS_IND:
			Trace("MMI_NW: MSG EV_CFW_NW_REG_STATUS_IND ");
			Trace("MMI_NW: p1:0x%x, p2:0x%x, p3:0x%x ", MsgStruct->nParam1, MsgStruct->nParam2, MsgStruct->nParam3);
			do_cswmsg_EV_CFW_NW_REG_STATUS_IND(MsgStruct);
			break;
		default:
			DefaultEventHandler(MsgStruct);
			return FALSE;
	}
	return TRUE;
}

BOOL ADP_EventHandler(COS_EVENT *MsgStruct)
{
	return ExecuteCurrEventHandler(MsgStruct->nEventId, MsgStruct);
}



//Parse TP-DCS , more in "3G TS 23.038 V3.3.0"
void ADP_USSDParseDCS(UINT8 nInputData, UINT8* pAlphabet)
{
    //Parse TP-DCS , more in "3G TS 23.038 V3.3.0"
    *pAlphabet =  ADP_MSG_RESERVED;
    
    if((nInputData & 0xf0) == 0x00)// bit(7-0): 0000 xxxx
    {
        *pAlphabet  = ADP_MSG_GSM_7;
    }
    else if((nInputData & 0xf0) == 0x10)// bit(7-0): 0001 xxxx
    {
        if((nInputData & 0x0f) == 0x00) // bit(7-0): 0001 0000
            *pAlphabet  = ADP_MSG_GSM_7;
        else if ((nInputData & 0x0f) == 0x01)// bit(7-0): 0001 0001
            *pAlphabet  = ADP_MSG_UCS2;
		else
            *pAlphabet  = ADP_MSG_RESERVED;

    }
    else if((nInputData & 0xc0) == 0x40)  //bit(7-0): 01xx xxxx
    {
        //Set pAlphabet
        if((nInputData & 0x0c) == 0x00)// bit(7-0): 01xx 00xx 
        {
            *pAlphabet = ADP_MSG_GSM_7;
        }
        else if((nInputData & 0x0c) == 0x04)// bit(7-0): 01xx 01xx 
        {
            *pAlphabet = ADP_MSG_8_BIT_DATA;
        }
        else if((nInputData & 0x0c) == 0x08)// bit(7-0): 01xx 10xx 
        {
            *pAlphabet = ADP_MSG_UCS2;
        }
        else if((nInputData & 0x0c) == 0x0c)// bit(7-0): 01xx 11xx 
        {
            *pAlphabet = ADP_MSG_RESERVED;
        }
    }    
    else if((nInputData & 0xf0) == 0x90)  //bit(7-0): 1001 xxxx
    {
        //Set pAlphabet
        if((nInputData & 0x0c) == 0x00)// bit(7-0): 1001 00xx 
        {
            *pAlphabet = ADP_MSG_GSM_7;
        }
        else if((nInputData & 0x0c) == 0x04)// bit(7-0): 1001 01xx 
        {
            *pAlphabet = ADP_MSG_8_BIT_DATA;
        }
        else if((nInputData & 0x0c) == 0x08)// bit(7-0): 1001 10xx 
        {
            *pAlphabet = ADP_MSG_UCS2;
        }
        else if((nInputData & 0x0c) == 0x0c)// bit(7-0): 1001 11xx 
        {
            *pAlphabet = ADP_MSG_RESERVED;
        }
    }
    else if((nInputData & 0xf0) == 0xf0)  //bit(7-0): 1111 0xxx
    {
        if((nInputData & 0x04) == 0x04) // bit(7-0): 1111 01xx
            *pAlphabet  = ADP_MSG_8_BIT_DATA;
        else// bit(7-0): 1111 00xx
            *pAlphabet  = ADP_MSG_GSM_7;
    }
    
    mmi_trace(TRUE,"USSD Func: %s nInputData= %d, *pAlphabet=%d", __FUNCTION__, nInputData, *pAlphabet);

    if(*pAlphabet == ADP_MSG_RESERVED)
    {
        *pAlphabet =  ADP_MSG_GSM_7;
    }
	    
}


void ADP_SATParseDCS(UINT8 nInputData, UINT8 *pAlphabet)
{
    *pAlphabet =  ADP_MSG_RESERVED;

    if((nInputData & 0xc0) == 0x00) //bit(7-0) :00xx xxxx
    {
        if((nInputData & 0x0c) == 0x00)//bit(7-0) :00xx 00xx
        {
            *pAlphabet  = ADP_MSG_GSM_7;            
        }
        else if((nInputData & 0x0c) == 0x04)//bit(7-0) :00xx 01xx
        {
            *pAlphabet  = ADP_MSG_8_BIT_DATA;            
        }
        else if((nInputData & 0x0c) == 0x08)//bit(7-0) :00xx 10xx
        {
            *pAlphabet  = ADP_MSG_UCS2;            
        }        
    }
    else if((nInputData & 0xf0) == 0xe0) //bit(7-0) :1110 xxxx
    {
        *pAlphabet  = ADP_MSG_UCS2;            
    }
    else if((nInputData & 0xf0) == 0xf0) //bit(7-0) :1111 xxxx
    {
        if((nInputData & 0x04) == 0x00)//bit(7-0) :1111 x0xx
        {
            *pAlphabet  = ADP_MSG_GSM_7;            
        }
        else if((nInputData & 0x04) == 0x04)//bit(7-0) :00xx 01xx
        {
            *pAlphabet  = ADP_MSG_8_BIT_DATA;            
        }
    }
    
    mmi_trace(TRUE,"USSD Func: %s nInputData= %d, *pAlphabet=%d", __FUNCTION__, nInputData, *pAlphabet);

    if(*pAlphabet == ADP_MSG_RESERVED)
    {
        *pAlphabet =  ADP_MSG_GSM_7;
    }
}

INT32  ADP_SAT_Text_Data_Decode(UINT8 *pDest, UINT8*pDestDec, UINT8 *pSrc,  UINT8 SrcDec, UINT16 nSrcLen)
{
    UINT8 adp_dec = 0;
    INT32 nDestlen = 0;

    if(pDest == NULL || pDestDec == NULL || pSrc == NULL)
    {
        return nDestlen;    
    }
    
    ADP_SATParseDCS(SrcDec, &adp_dec);

    if (adp_dec == ADP_MSG_GSM_7 || adp_dec == ADP_MSG_RESERVED)  //7 bit code
    {
    	nDestlen = SUL_Decode7Bit(pSrc, pDest, nSrcLen);
    	*pDestDec = MMI_8BIT_DCS;  //ascii coding
    }
    else if (adp_dec == ADP_MSG_8_BIT_DATA)
    {
    	*pDestDec = MMI_8BIT_DCS;
        memcpy(pDest, pSrc, nSrcLen);	
        nDestlen = nSrcLen;
    }
    else if (adp_dec == ADP_MSG_UCS2)
    {
    	*pDestDec = MMI_UCS2_DCS;
    	
    	if(pSrc[0] == 0x81)
    		*pDestDec = MMI_UCS2_81;
    	else if(pSrc[0] == 0x82)
    		*pDestDec = MMI_UCS2_82;
    	
        memcpy(pDest, pSrc, nSrcLen);	
        nDestlen = nSrcLen;
    }

    return nDestlen;
}

#if 0
void ADP_InitEvents(void)
{
	UINT16 count = 0;

	g_AdpMaxEvent = 0;

	for (count = 0; count < ADP_MAX_EVENT; count++)
	{
		g_AdpEventHandler[count].EventID = 0;
		g_AdpEventHandler[count].EntryFuncPtr = NULL;
	}
}

void ADP_SetEventHandler(AdpEvHandlerPtr pfnEvHandler, UINT32 EventID)
{
	UINT16 count = 0;
	BOOL IsNewEvent = TRUE;

	for (count = 0; count < g_AdpMaxEvent; count++)
	{
		if (g_AdpEventHandler[count].EventID == EventID)
		{
			IsNewEvent = FALSE;
			break;
		}
	}

	if (IsNewEvent)
	{
		g_AdpMaxEvent++;
		MMI_ASSERT(g_AdpMaxEvent < ADP_MAX_EVENT);
	}

	g_AdpEventHandler[count].EventID = EventID;
	g_AdpEventHandler[count].EntryFuncPtr = pfnEvHandler;
}

void ClearAllEventHandler(void)
{
	UINT16 count = 0;

	for (count = 0; count < g_AdpMaxEvent; count++)
	{
		g_AdpEventHandler[count].EventID = 0;
		g_AdpEventHandler[count].EntryFuncPtr = NULL;
	}
}

void ClearEventHandler(UINT32 EventID)
{
	ADP_SetEventHandler(NULL, EventID);
}
#endif
#ifdef ADP_SIMU_FOUR_CARDS_WITH_TWO_CARDS
#define ADP_SIMU_DURATION (20)
#define ADP_SIMU_EVENT_DURATION (10)
Simu_Msg *pSimuMsg = NULL;

void adp_simu_timer_handler(void)
{
	COS_EVENT ev = {0};
	
	StopTimer(ADP_SIMULATE_TIMER);
	if(NULL != pSimuMsg)
	{
		Simu_Msg *pTempMsg;
		mmi_trace(1, "TEST: %s, simu msg sim id is %d", __FUNCTION__, ADP_GET_CSW_FLAG(pSimuMsg->pCosEvent->nParam3));

		ev.nEventId =  pSimuMsg->pCosEvent->nEventId;
		ev.nParam1 = pSimuMsg->pCosEvent->nParam1;
		ev.nParam2 = pSimuMsg->pCosEvent->nParam2;
		ev.nParam3 = pSimuMsg->pCosEvent->nParam3;
	
		COS_SendEvent(GetMmiTaskHandle(MOD_L4C), &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL);

		pTempMsg = pSimuMsg->pNext;

		OslMfree(pSimuMsg->pCosEvent);
		OslMfree(pSimuMsg);
		
		pSimuMsg = pTempMsg;
		if(NULL != pSimuMsg)
		{
			StartTimer(ADP_SIMULATE_TIMER, ADP_SIMU_EVENT_DURATION, adp_simu_timer_handler);
		}
	}
}
void adp_simu_start_timer(void)
{
	mmi_trace(1, "TEST: %s", __FUNCTION__);
	if(IsMyTimerExist(ADP_SIMULATE_TIMER))
		return;
	StartTimer(ADP_SIMULATE_TIMER, ADP_SIMU_DURATION, adp_simu_timer_handler);
}
typedef struct 
{
	U32 msgId;
	U8 *dcs;
}SimuMsgStr;

static const SimuMsgStr NeedDoubleMsgID[]=
{
	{EV_CFW_INIT_IND, "EV_CFW_INIT_IND"},
	{EV_CFW_NW_SIGNAL_QUALITY_IND, "EV_CFW_NW_SIGNAL_QUALITY_IND"},
	//{EV_CFW_SIM_STATUS_IND, "EV_CFW_SIM_STATUS_IND"},
	{EV_CFW_NW_REG_STATUS_IND, "EV_CFW_NW_REG_STATUS_IND"},
};
BOOL ifEvtNeedSimu(const U32 evtID)
{
	S16 i;
	for(i = 0; i < sizeof(NeedDoubleMsgID)/sizeof(NeedDoubleMsgID[0]); i++)
	{
		if(evtID == NeedDoubleMsgID[i].msgId)
		{
			mmi_trace(1, "TEST: Simu Add Msg %s", NeedDoubleMsgID[i].dcs);
			return TRUE;
		}
	}
	return FALSE;
}
void adp_simu_add_new_msg(COS_EVENT *pCosEvent)
{
	Simu_Msg *pTempMsg;
	Simu_Msg *pLastMsg;
	
	U8 nSimID = ADP_GET_CSW_FLAG(pCosEvent->nParam3);
	
	if((nSimID == ADP_SIM_1 ||nSimID == ADP_SIM_2) && ifEvtNeedSimu(pCosEvent->nEventId))
	{
		if(pSimuMsg == NULL)
		{
			pSimuMsg = (Simu_Msg *)OslMalloc(sizeof(Simu_Msg));
			memset(pSimuMsg, 0, sizeof(Simu_Msg));
			pSimuMsg->pCosEvent = (COS_EVENT*)OslMalloc(sizeof(COS_EVENT));
			memcpy(pSimuMsg->pCosEvent, pCosEvent, sizeof(COS_EVENT));
			pSimuMsg->pCosEvent->nParam3 += 2;
			pSimuMsg->pNext = NULL;
			mmi_trace(1, "TEST: %s, newadd msg simid is %d", __FUNCTION__, ADP_GET_CSW_FLAG(pSimuMsg->pCosEvent->nParam3));
		}
		else
		{
			pTempMsg = (Simu_Msg *)OslMalloc(sizeof(Simu_Msg));
			memset(pTempMsg, 0, sizeof(Simu_Msg));
			pTempMsg->pCosEvent = (COS_EVENT*)OslMalloc(sizeof(COS_EVENT));
			memcpy(pTempMsg->pCosEvent, pCosEvent, sizeof(COS_EVENT));
			pTempMsg->pCosEvent->nParam3 += 2;
			pTempMsg->pNext = NULL;
			
			pLastMsg = pSimuMsg;
			while(NULL != pLastMsg->pNext)
				pLastMsg = pLastMsg->pNext;
			pLastMsg->pNext = pTempMsg;
			
			mmi_trace(1, "TEST: %s, newadd msg simid is %d", __FUNCTION__, ADP_GET_CSW_FLAG(pTempMsg->pCosEvent->nParam3));
		}
		
		adp_simu_start_timer();
	}
}

Simu_Rsp_Marker_Struct *pRspMarker = NULL;
void adp_simu_add_Rsp_Marker(Simu_Rsp_Marker_Struct *pMarker)
{
	Simu_Rsp_Marker_Struct *pTempMarker;
	Simu_Rsp_Marker_Struct *pLastMarker;
	
	mmi_trace(1, "TEST: %s, msgid is %d, uti is %d, simid is %d", __FUNCTION__, pMarker->msgID, pMarker->Uti, pMarker->nSimID);
	
	if(NULL == pRspMarker)
	{
		pRspMarker = (Simu_Rsp_Marker_Struct*)OslMalloc(sizeof(Simu_Rsp_Marker_Struct));
		memset(pRspMarker, 0, sizeof(Simu_Rsp_Marker_Struct));
		memcpy(pRspMarker, pMarker, sizeof(Simu_Rsp_Marker_Struct));
		pRspMarker->pNext = NULL;
	}
	else
	{
		pTempMarker = (Simu_Rsp_Marker_Struct*)OslMalloc(sizeof(Simu_Rsp_Marker_Struct));
		memset(pTempMarker, 0, sizeof(Simu_Rsp_Marker_Struct));
		memcpy(pTempMarker, pMarker, sizeof(Simu_Rsp_Marker_Struct));

		pLastMarker = pRspMarker;
		while(NULL != pLastMarker->pNext)
		{
			pLastMarker = pLastMarker->pNext;
		}
		pLastMarker->pNext = pTempMarker;
	}
}
void adp_sim_remove_rsp_marker_node(Simu_Rsp_Marker_Struct *pMarker)
{
	Simu_Rsp_Marker_Struct *pTempMarker = pRspMarker;
	if(pTempMarker == pMarker)
	{
		pRspMarker = pTempMarker->pNext;
		OslMfree(pTempMarker);
		mmi_trace(1, "TEST: %s, line %d", __FUNCTION__, __LINE__);
		return;
	}
	while(NULL != pTempMarker)
	{
		if(pTempMarker->pNext == pMarker)
		{
			pTempMarker->pNext = pMarker->pNext;
			OslMfree(pMarker);
			mmi_trace(1, "TEST: %s, line %d", __FUNCTION__, __LINE__);
			break;
		}
		pTempMarker = pTempMarker->pNext;
	}
}
U8 adp_simu_get_sim_id_by_uti_for_rsp_msg(U32 nEventId, U8 uti, U8 Old_simID)
{
	U8 nSimID = Old_simID;
	Simu_Rsp_Marker_Struct *pTempMarker = pRspMarker;
	while(NULL != pTempMarker)
	{
		if(pTempMarker->msgID == nEventId && pTempMarker->Uti == uti)
		{
			nSimID = pTempMarker->nSimID;
			adp_sim_remove_rsp_marker_node(pTempMarker);
			break;
		}
		pTempMarker = pTempMarker->pNext;
	}
	mmi_trace(1, "TEST: %s, event is %d, uti is %d,return nSimID is %d", __FUNCTION__, nEventId, uti, nSimID);
	return nSimID;
}
#endif

