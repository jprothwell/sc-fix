#ifndef _DEVDEF_H
#define _DEVDEF_H
//设备相关

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Mandatory includes
/*  Include: MMI header file */
#define __NEWSIMULATOR
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#ifdef MMI_ON_WIN32
//#include "windows.h"
#endif
//#include "PixtelDataTypes.h"

#include "gui_themes.h"
#include "gui.h"
#include "mmithemes.h"
#include "custthemesres.h"
#include "thememanager.h"

#include "gdi_include.h"
#include "gdi_lcd_config.h"
#include "gdi_const.h"
#include "lcd_sw_rnd.h"
#include "gui_data_types.h"  
//#include "themecomponents.h" 
#include "wgui_categories_defs.h"

//... Add More MMI header
#include "stack_msgs.h"
#include "queuegprot.h"
#include "eventsgprot.h"
#include "historygprot.h"
#include "globalmenuitems.h"
#include "globalconstants.h"
#include "globaldefs.h"
#include "wgui_categories.h"
#include "wgui_categories_inputs.h"
#include "custmenures.h"
#include "commonscreens.h"
#include "unicodexdcl.h"
#include "timerevents.h"
//#include "DebugInitDef.h"

//...Add MSG header
#include "customer_ps_inc.h"
#include "messagesl4def.h"
#include "messagesresourcedata.h"
#include "messagesmiscell.h"
#include "messagesexdcl.h"
#include "smsguiinterfaceprot.h"
#include "smsguiinterfacetype.h"
#include "smspshandler.h"
#include "cellbroadcasttype.h"

#include "gui_ems.h"

//#include "wgui_ems.h"
#include "gsm7bitdefaultalphabet.h"
//
#include "smsal_l4c_enum.h"
//
#include "nvramenum.h"
#include "nvramprot.h"
#include "nvramtype.h"
//
#include "csp.h"
#include "frameworkstruct.h"
#include "mainmenudef.h"
#if (KK_MTKXXXX_SWVER < KK_MTKXXXX_V09A)
#include "callhistorygprots.h"
#endif
#include "callmanagementstruct.h"
#include "callmanagementgprot.h"
#include "callhistoryenum.h"
#include "miscfunctions.h"
#include "settingprofile.h"
#include "profilegprots.h"
#include "phonebookgprot.h"
#include "idleappprot.h"
#include "alarmframeworkprot.h"

#include "wgui_status_icons.h"
#include "gpioinc.h"
#include "keybrd.h"
#include "sublcdhistorygprot.h"
#include "mdi_datatype.h"
#include "mdi_audio.h"
#include "wgui_inline_edit.h"

#include "protocolevents.h"

#include "custdataprots.h"

#include "gui_setting.h"

#include "wap.h"
//#include "PoCGProt.h"
#include "satdefs.h"
//#include "FunAndGamesResDef.h"
#include "stack_msgs.h"
#include "gameframework.h"
//#include "DataAccountResDef.h"
#if defined(__MMI_TOUCH_SCREEN__)
#include "gui_virtual_keyboard.h"

#if (KK_MTKXXXX_SWVER < KK_MTKXXXX_V08A)
//#include "EditorPenGprot.h"
#include "editorpen.h"
#endif

#include "wgui_touch_screen.h"
#include "touchscreengprot.h"
#endif
#include "smsapi.h" //bn10046

#ifdef SUPPORT_FTP
#ifdef __DRM_SUPPORT__
#include "drm_def.h"
#endif
#include "filemgr.h"
#include "profilemgr.h"
#include "profilemgrgprot.h"
#include "filemanagerdef.h"
#include "filemanagergprot.h"
#include "filemgrstruct.h"
#include "wgui_categories_fmgr.h"

#include "smsguiInterfacetype.h"
#include "smspshandler.h"
#include "messagesexdcl.h"
#include "pixteldatatypes.h" 
#include "smsapi.h"
#include "messagesmiscell.h"
#include "stack_config.h"
#include "gui_ems.h"
#include "ImageViewer.h"
#include "ImageViewerresdefs.h"
#include "vdoplystrenum.h"
#include "composeringtonedefs.h"
#include "soundrecorderdef.h"
#include "abrepeaterdef.h"
#include "fs_type.h"
#include "fmt_def.h"
#endif

#ifdef MMI_ON_HARDWARE_P
#include "app_ltlcom.h"
#include "wap_ps_struct.h"
//#include "wapadp.h"
#include "stack_config.h"
#include "stack_ltlcom.h"
#include "stack_common.h"
#endif

#include "wgui_categories_popup.h"
#include "commonscreensenum.h"
#undef __NEWSIMULATOR

#include "typedefs.h"
#include "compswitch.h"
//#include "wepzone.h"
#include "app_datetime.h"
#include "datetimetype.h"
//#include "stack_common.h"
//#include "custom_sap.h"
#if (KK_MTKXXXX_SWVER < KK_MTKXXXX_V08A) || (KK_MTKXXXX_SWVER == KK_MTKXXXX_V09A)
#include "eventsgprot.h"
#endif

#include "stack_msgs.h"
#include "protocolevents.h"
//#include "custom_sap.h"
#if(KK_MTKSWVER_MACRO == KK_MTKSW_VER08A) || (KK_MTKSWVER_MACRO == KK_MTKSW_VER08B)
#include "med_utility.h"
#endif

#if(KK_MTKSWVER_MACRO == KK_MTKSW_VER08A) //BN10012
//#include "MTPNP_AD_master_header.h"
#include "mtpnp_ad_master_if.h"
#include "mtpnp_pfal_master_if.h"
#include "mtpnp_pfal_master_def.h"

#include "mtpnp_sdk_master_if.h"
#include "mtpnp_ad_master_common_def.h"
#endif
#include "mmi_data_types.h"
#include "wgui_categories_multimedia.h"
#include "kk_res_iddef.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#define PLAT_LITTLE_ENDIAN


// user defined message
typedef enum {
	WM_DATA_ARRIVAL = 0x8000,//WM_APP,	// data package arrival
	WM_FILE_TRANSFER,					// file transfer
	WM_RE_SETFOCUS,						// re-set focus
	WM_AUTO_LOGIN,						// automatic login
} USER_MSG;

 
#define MAX_ICON_NUM			256		 


//LISTVIEW 的图标状态
#define ICON_NOT_EXIST		-2
#define ICON_NOT_LOAD			-3

#define UI_ICON_WIDTH			40
#define UI_ICON_HEIGHT		40
#define UI_ICON_WIDTH_SMALL			32
#define UI_ICON_HEIGHT_SMALL		32
#define UI_LITTLE_ICON_WIDTH		16
#define UI_LITTLE_ICON_HEIGHT		16

#define UI_LITTLE_ICON_WIDTH_SMALL		12
#define UI_LITTLE_ICON_HEIGHT_SMALL		12

#define LEFT_SOFTKEY_ID   0xaa
#define RIGHT_SOFTKEY_ID  0xcc

#define CSD_IMAGE_HEADER_LEN 8

typedef enum {
	RECT_ALL = 0,		//Update all
	RECT_PORTRAIT,	//Update portrait
	RECT_TITLE,			//Update title
	RECT_WEPBAR,		//Update Wep Bar
	RECT_AD,				//Update AD
	RECT_SOFTKEY,		//Update Softkey
} UPDATE_RECT;

////////////////////////////////////////////////////////////////////////////////

#define LD_MAX_SOUND_NUM			10	 

typedef struct _LD_SoundData
{
	int32_t		sndIdx;
	int32_t		lenOfSndData;
	byte_t		sndType;
	bool_t		isCycle;
	UI_character_type		sndName[MAX_FILENAME_LEN];
} LDSoundData;

typedef struct _history_ext
{
	U16 scrnID;
	FuncPtr entryFuncPtr; 
	U8 inputBuffer[MAX_INPUT_BUFFER];			/* running text data 比系统设定值多了一倍,为了实现大批量文字输入*/
	U8 guiBuffer[MAX_GUI_BUFFER];				/* this includes hilite item, input mode & etc..*/
}history_ext;
////////////////////////////////////////////////////////////////////////////////

typedef struct _DEVICE_DATA
{
	void * pAppData;	// pointer to application data

	// memory pool
//	byte_t memoryPool[MALLOC_MODULE_TOTAL_SIZE];

	// timer
	UINT nTimerId[MAX_TIMER_NUM];
	PFNTIMERCALLBACK pfnTimerCB[MAX_TIMER_NUM];
	void * pTimerData[MAX_TIMER_NUM];

	int32_t imageIndex[MAX_ICON_NUM];

	int32_t msgSize;
	UI_character_type bufMessage[MAX_UI_MESSAGE_SIZE * 2/sizeof(UI_character_type)];
	UI_character_type titleMessage[30];

	uint16_t screenId;

	bool_t bScreenDataGot;
// 	bool_t bShowWelcomeAll;//logon UI use image

////////////////////////////////////////////////////////////////////////////////

#ifdef SUPPORT_TICKER
	byte_t szWepTikerTitle[200];//MAX_TICKER_TITLE_LEN
#endif

	LDSoundData SoundData[LD_MAX_SOUND_NUM];

	bool_t hideWin;//hide win ,NOT show 

	int32_t imgUsedBufLen;//used buffer length of game image 
	int32_t picUsedBufLen;//used buffer length of ui picture
	color_t currThemeColor;//current theme color
#ifdef SUPPORT_FTP  
	bool_t b_EnterFileManagerScreen;   //BLOG
	bool_t b_ImageInformationHaveGot;
#endif
/* by xw 20090324
	byte_t    MsgQueueBuf[MAX_MESSAGE_QUEUE_NUM][MAX_RECEIVE_BUFFER_LEN];
	int32_t msgLen[MAX_MESSAGE_QUEUE_NUM];
	int32_t   numOfMsgInQueue;
*/
	UI_character_type pWaitingMsgData[50];
	int32_t pWaitingMsgLen;
	bool_t   isWaitScreen;
	bool_t   isWaitScreenRecordFlag;
} DeviceData; 

//#define WEP_PATH					"c:\\WEP\\"							// WEP path
//#define WEP_RES_PATH			"c:\\WEP\\res\\"				// resource path
//#define WEP_DOWNLOAD_PATH "c:\\WEP\\download\\"		// download path
//#define WEP_LDF_PATH			"c:\\WEP\\ldf\\"				// local data file path
//#define WEP_TMP_PATH			"c:\\WEP\\tmp\\"				// temporary path
//#define WEP_LD_PATH				"C:\\WEP\\ld\\"					//local data path

#define WEP_PATH					"e:/WEP"							// WEP path
#define WEP_RES_PATH			"e:/WEP/res/"				// resource path
#define WEP_DOWNLOAD_PATH "e:/WEP/download/"		// download path
#define WEP_LDF_PATH			"e:/WEP/ldf/"				// local data file path
#define WEP_TMP_PATH			"e:/WEP/tmp/"				// temporary path
#define WEP_LD_PATH				"e:/WEP/ld/"					//local data path
#define WEP_LD_FILENAME		"SW_CSD.dat"						//filename of local Data

// newline string
#define STR_LINE_FEED			"\x000A"	
// newline char for search
#define CHR_LINE_FEED			'\x000A'

#define CHR_SPACE					0x20	// space char
#define CHR_NUL						0x00	// nul char

#define WEP_NORMAL_WIDTH 176
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
extern int32_t DEV_NetUniByte2LocStr(UI_character_type * pDest, const byte_t * pSrc, int32_t nSrcLen);
extern int32_t DEV_LocStr2NetUniByte(byte_t * pDest, const UI_character_type * pSrc, int32_t nSrcLen);

#endif 
/* _DEVDEF_H */


