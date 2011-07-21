#ifndef _DEVCFG_H
#define _DEVCFG_H

//设备相关常量
#include "compswitch.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// client VER num
#define CLIENT_MAJOR_VERSION		2
#define CLIENT_MINOR_VERSION		6001//3491
//#define CLIENT_CORP_VERSION			101 // 视频聊天室
#define CLIENT_BUILD_NUMBER			12

#ifdef WIN32
//#define DEV_TEST_PUBLIC	
//#define DEV_TEST_PRIVATE	
#endif

#ifdef DEV_TEST_PUBLIC

#ifdef WIN32
#define WEP_SERVER_URL				"61.129.14.23"
#else
#define WEP_SERVER_URL				"61.129.14.22"
#endif

#define	WEP_SERVER_PORT				8585

#elif defined(DEV_TEST_PRIVATE)

#ifdef WIN32
#define WEP_SERVER_URL				"192.168.1.18"
#else
#define WEP_SERVER_URL				"58.38.6.25"
#endif

#define	WEP_SERVER_PORT				19595

#else /* release */ 

#ifdef WIN32
#define WEP_SERVER_URL				"www.shoujikk.com"
#else
#define WEP_SERVER_URL				"www.shoujikk.com"
#endif

#define	WEP_SERVER_PORT				8585
#endif

//////////////////////////////////////////////////////////////////////////

// define memory malloc module id 
typedef enum
{
	MALLOC_MODULE_NULL          = 0,
	MALLOC_MODULE_LOGIC_MAIN,//main entry of logic code 
	MALLOC_MODULE_LOGIC_RESPONSEBUFFER,
	MALLOC_MODULE_LOGIC_UI,
	MALLOC_MODULE_LOGIC_NET,
	MALLOC_MODULE_LOCAL_DATA,
	MALLOC_MODULE_DATA_PACKED,
	MALLOC_MODULE_BUILDER,
	MALLOC_MODULE_NET,
	MALLOC_MODULE_DATA_UNPACKED,//save prestore UI
	MALLOC_MODULE_CHATDATA,
	MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA,
	MALLOC_MODULE_IMAGE_DATA,//for game image 
	MALLOC_MODULE_PICTURE_DATA = MALLOC_MODULE_IMAGE_DATA+10,//for UI pic,assume 10 game images
#ifdef VIDEO_CACHE
	MALLOC_MODULE_VIDEO_CACHE_DATA,
#endif
#ifdef SUPPORT_BACK_SCREEN
	MALLOC_MODULE_BACK_SCREEN,
#endif
	MALLOC_MODULE_WEPCAM,
} MEMORY_MALLOC_MODULE_ID;


// wait message rectangle define
#define WAIT_MSG_RECT_LEFT		10
#define WAIT_MSG_RECT_TOP		60
#define WAIT_MSG_RECT_WIDTH		(UI_device_width- WAIT_MSG_RECT_LEFT * 2)
#define WAIT_MSG_RECT_HEIGHT	32


//////////////////////////////////////////////////////////////////////////
// module memory size define
#define MALLOC_MODULE_LOGIC_MAIN_INIT_SIZE    	(16*1024) //(11 * 1024)	// 11K // czm add for video chat 20071128 
#define MALLOC_MODULE_LOGIC_RESPONSEBUFFER_INIT_SIZE    	(12 * 1024)	// 12K
#define MALLOC_MODULE_LOGIC_UI_INIT_SIZE   			( 8 * 1024)	//  8K
#define MALLOC_MODULE_LOGIC_NET_INIT_SIZE				(64)	// 64B
#define MALLOC_MODULE_LOCAL_DATA_INIT_SIZE    	(2 * 1024)	// 2K
#define MALLOC_MODULE_DATA_PACKED_INIT_SIZE   	( 3 * 1024)	//  3K
#define MALLOC_MODULE_BUILDER_INIT_SIZE					(14 * 1024)	// 14K
#define MALLOC_MODULE_NET_INIT_SIZE			( 256)	//  256B
#define MALLOC_MODULE_DATA_UNPACKED_INIT_SIZE   	(5 * 1024)	// 5K
#define MALLOC_MODULE_CHATDATA_INIT_SIZE   	(10 * 1024)	// 10K// czm 070205 for chat history
#define MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA_SIZE	(10 * 8) 
#define MALLOC_MODULE_IMAGE_DATA_SIZE (12 *1024)//12k
#define MALLOC_MODULE_PICTURE_DATA_SIZE (25 *1024)//12k

// module memory base define
#define MALLOC_MODULE_LOGIC_MAIN_BASE    	0
#define MALLOC_MODULE_LOGIC_RESPONSEBUFFER_BASE    	(MALLOC_MODULE_LOGIC_MAIN_BASE     + MALLOC_MODULE_LOGIC_MAIN_INIT_SIZE)
#define MALLOC_MODULE_LOGIC_UI_BASE   	(MALLOC_MODULE_LOGIC_RESPONSEBUFFER_BASE + MALLOC_MODULE_LOGIC_RESPONSEBUFFER_INIT_SIZE)
#define MALLOC_MODULE_LOGIC_NET_BASE	(MALLOC_MODULE_LOGIC_UI_BASE    + MALLOC_MODULE_LOGIC_UI_INIT_SIZE)

#define MALLOC_MODULE_LOCAL_DATA_BASE   	(MALLOC_MODULE_LOGIC_NET_BASE     + MALLOC_MODULE_LOGIC_NET_INIT_SIZE)
#define MALLOC_MODULE_DATA_PACKED_BASE   	(MALLOC_MODULE_LOCAL_DATA_BASE + MALLOC_MODULE_LOCAL_DATA_INIT_SIZE)
#define MALLOC_MODULE_BUILDER_BASE	(MALLOC_MODULE_DATA_PACKED_BASE    + MALLOC_MODULE_DATA_PACKED_INIT_SIZE)
#define MALLOC_MODULE_NET_BASE	(MALLOC_MODULE_BUILDER_BASE + MALLOC_MODULE_BUILDER_INIT_SIZE)
#define MALLOC_MODULE_DATA_UNPACKED_BASE   	(MALLOC_MODULE_NET_BASE   + MALLOC_MODULE_NET_INIT_SIZE)
#define MALLOC_MODULE_CHATDATA_BASE   	(MALLOC_MODULE_DATA_UNPACKED_BASE   + MALLOC_MODULE_DATA_UNPACKED_INIT_SIZE)
#define MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA_BASE   	(MALLOC_MODULE_CHATDATA_BASE   + MALLOC_MODULE_CHATDATA_INIT_SIZE)
#define MALLOC_MODULE_IMAGE_DATA_BASE   	(MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA_BASE   + MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA_SIZE)
#define MALLOC_MODULE_PICTURE_DATA_BASE   	(MALLOC_MODULE_IMAGE_DATA_BASE   + MALLOC_MODULE_IMAGE_DATA_SIZE)

#define MALLOC_MODULE_TOTAL_SIZE         \
					MALLOC_MODULE_LOGIC_MAIN_INIT_SIZE     \
				+ MALLOC_MODULE_LOGIC_RESPONSEBUFFER_INIT_SIZE    \
				+ MALLOC_MODULE_LOGIC_UI_INIT_SIZE    \
				+ MALLOC_MODULE_LOGIC_NET_INIT_SIZE \
				+ MALLOC_MODULE_LOCAL_DATA_INIT_SIZE \
				+ MALLOC_MODULE_DATA_PACKED_INIT_SIZE\
				+ MALLOC_MODULE_BUILDER_INIT_SIZE	\
				+ MALLOC_MODULE_NET_INIT_SIZE\
				+ MALLOC_MODULE_DATA_UNPACKED_INIT_SIZE\
				+ MALLOC_MODULE_CHATDATA_INIT_SIZE\
				+ MALLOC_MODULE_IMAGE_ATTRIBUTE_DATA_SIZE\
				+ MALLOC_MODULE_IMAGE_DATA_SIZE\
				+ MALLOC_MODULE_PICTURE_DATA_SIZE

// soft key rectangle define
// #define SK_RECT_LEFT_LSK			20
// #define SK_RECT_LEFT_RSK			150
// #define SK_RECT_TOP						240
// #define SK_RECT_WIDTH					66
// #define SK_RECT_HEIGHT				24

 
#define MAX_RECEIVE_BUFFER_LEN		(25 * 1024)	// ! the value should be divided exactly by 4
 
//#define MAX_REQUEST_BUFFER_LEN		(2 * 1024)  // ! the value should be divided exactly by 4
#define MAX_REQUEST_BUFFER_LEN		(12 * 1024)// ! the value should be divided exactly by 4 // czm add for video chat

#ifdef WEP_NET_SEND_OPT
#define MAX_SEND_BUFFER_LEN          (12*1024)
#endif

 
//#define MAX_PACKAGE_LEN_WARNING		(12 * 1024)	// warning if package larger than this value

// vital message prompt interval (seconds)
#define VITAL_MESSAGE_PROMPT_INT	30
// my turn prompt interval (seconds)
#define MY_TURN_PROMPT_INT				30

#define NET_TIMEOUT								30	// request timeout interval(seconds)
#define WAIT_NOTICE_INTERVAL					(6*NET_TIMEOUT)		// wait time before notice the opposite (120s)
#define WAIT_TIMEOUT_INTERVAL					(9*NET_TIMEOUT)		// wait time before timeout (180s)
#define NET_READ_TIMEOUT					50	// idle read timeout interval (seconds)
#define MAX_NET_LINGER_INTERVAL					(NET_READ_TIMEOUT+20)	// maximum network linger interval (seconds)

#define SPIDER										2
#define FONT_HEIGHT								18
#define FONT_WIDTH								12
#define SOFTKEY_HEIGHT						(24 - SPIDER)

typedef enum
{
	UI_ID_CHAT_INVITE			=	1001,	// "chat invite" screen ID
	UI_ID_CHAT_BUDDY_LIST,				// "buddy list" screen ID
	UI_ID_MY_SETTINGS,						// "my settings" screen ID
	UI_ID_EXIT_CONFIRM,						//	exit WEP confirm screen ID
	UI_ID_VIDEOCHAT_PICMODESEL,             // select get pic from camera or local // czm add for video chat 20071128
	UI_ID_VIDEOCHAT_CAMERA_INTERVAL_SET,    // camera interval setting // czm add for video chat 20071128
} CLIENT_UI_SCREEN_ID;

#define MAX_CHAT_PLAYER_NUMBER		20		 
#define MAX_CHAT_ITEM_NUMBER			20		 
#define MAX_CHAT_PLAYER_NAME_LEN	40		 
#define MAX_CHAT_MESSAGE_SIZE			100		 

#define MAX_CHAT_FACE_NUMBER			(30+MAX_CHAT_ICON_NUM)		 
#define MAX_CHAT_FACE_SYM_SIZE		6			 

#define MAX_CHAT_TEXT_COLOR_TYPE	20		 

#define MAX_UI_MESSAGE_SIZE				256		 

#define MAX_FILE_NAME_SIZE				100		 

#define MAX_SUBITEM_CAPTION_LEN		80	 

#define UI_MAX_LABEL_NUM					24	 
#define MAX_LABEL_CAPTION_LEN			60	 
#define MAX_VIDEO_CHAT_PIC_SIZE         (3*1024)	// czm add for video chat 20071128 

// wait msg id
#define MSG_CONNECT				1
#define MSG_WAIT					2
#define MSG_RECEIVE				3

// void function pointer type define
typedef void (* PFNTIMERCALLBACK)(void * pData);

typedef enum
{
	TIMER_CONNECT_INFO = 0,
	TIMER_VITAL_MSG,
	TIMER_MYTURN_PROMPT,
	TIMER_NET_TIMEOUT,

#ifdef SUPPORT_ANIMATION
	TIMER_ANIMATION,
#endif
	
#ifdef SUPPORT_PROXY
	TIMER_HTTP_SEND,
#endif
	TIMER_VIDEOCHAT_CAMERA_GET, // czm add for video chat 20071128

#ifdef VIDEO_CACHE
	TIMER_VIDEOCHAT_VIDEO_SHOW,
#endif
#ifdef SUPPORT_RECORD_VOICE
	TIMER_RECORD_PROMPT,
#endif
	/* Please add before this line only */
	MAX_TIMER_NUM /* Imp: Please do not modify this */
} CSD_TIMER_IDS;

#define MAX_STRING_RECORD_LENGTH	128

// string record
typedef enum
{
	LOCAL_DATA_RECORD_USER_NAME = 0,	// string, user name
	LOCAL_DATA_RECORD_PASS_WORD,			// string, password
	LOCAL_DATA_RECORD_SERVER_URL,			// string, server URL

	LOCAL_DATA_RECORD_USER_HEAD,	  	// string, user head picture name
	LOCAL_DATA_RECORD_UPD_SVR_IP,			// string, update server IP

	/* Please add before this line only */
	MAX_STRING_RECORD /* Imp: Please do not modify this */
} LOCAL_DATA_RECORD_STRIDS;


// integer record
typedef enum
{
	LOCAL_DATA_RECORD_SERVER_PORT = 0,	// int, server port
	LOCAL_DATA_RECORD_SETTING_SOUND,		// int, setting
#ifdef SUPPORT_PROXY
	LOCAL_DATA_RECORD_SETTING_USEPROXY,	// int, setting
#endif
	LOCAL_DATA_RECORD_SETTING_AUTOSAVE,	// int, setting
	LOCAL_DATA_RECORD_SETTING_AUTOLOGON,// int, setting
	LOCAL_DATA_RECORD_EXIST_STATUS,			// int, exit status
	LOCAL_DATA_RECORD_CONNECT_FAIL,			// int, connect fail times
	LOCAL_DATA_RECORD_CACHED_UIP_VER,		// int, version of cached UIP

#if 1 //#ifdef SUPPORT_DUAL_SIM	//wyl20100408
	LOCAL_DATA_RECORD_WHICH_SIM,
#endif
#if 1 //#ifdef _KK_SELECT_ACCOUNT_
	LOCAL_DATA_RECORD_WHICH_ACCOUNT,
#endif
	/* Please add before this line only */
	MAX_INTEGER_RECORD /* Imp: Please do not modify this */
} LOCAL_DATA_RECORD_INTIDS;

#define MAX_CACHED_UIP_NUM		16

// binary record
typedef enum
{
	LOCAL_DATA_RECORD_UIP_MIN =0,		// binary, min UIP id
	LOCAL_DATA_RECORD_UIP_MAX = LOCAL_DATA_RECORD_UIP_MIN + MAX_CACHED_UIP_NUM,	 

	LOCAL_DATA_RECORD_CHATDATA,  // czm 070205 for chat history
	/* Please add before this line only */
	MAX_BINARY_RECORD	/* Imp: Please do not modify this */
} LOCAL_DATA_RECORD_BINIDS;

#define  DEVICE_FONT_HEIGHT 16
#define  DEVICE_FONT_WIDTH 16

#define MAX_MESSAGE_QUEUE_NUM	3			 

#ifdef SUPPORT_DUAL_SIM
#define	DUALSIM_USABLE_STATUS				   (35)//MTPNP_AD_DUALSIM_USABLE
#endif

#define WEP_MAX_PIC_SIZE				(3*1024)
#ifdef SUPPORT_RECORD_VOICE
#define MAX_RECORD_BUFFER_LEN			(10*1024)	
#define MAX_RECORD_VOICE_SECONDS		(6)
#endif
#ifdef SUPPORT_BLOCK_SP_SMS
#define MAX_SUPPORT_SP_NUMBER			2   
#define MAX_SUPPORT_SP_LIST				20
#define MAX_KEYWORD_NUM					10
#endif

//bruce0912010
//#ifdef __KK_UI_V2__
typedef enum
{
	VIDEO_RESOL00_48X48 = 0,
	VIDEO_RESOL01_64X64,
	VIDEO_RESOL02_80X80,
	VIDEO_RESOL03_96X96,
	VIDEO_RESOL04_112X112,
	VIDEO_RESOL05_128X96,
	VIDEO_RESOL06_128X128,
	VIDEO_RESOL_VER1_MAX = VIDEO_RESOL06_128X128,
	VIDEO_RESOL07_160X120,
	VIDEO_RESOL08_176X144,
	VIDEO_RESOL09_320X240,
	VIDEO_RESOL10_352X288,
	VIDEO_RESOL_TOTAL
} video_resol_id;

typedef enum
{
	KK_IMG_START = 0,
	KK_IMG_VIEWCHAT_000 = KK_IMG_START,
	KK_IMG_VIEWCHAT_001,
	KK_IMG_VIEWCHAT_002,
	KK_IMG_VIEWCHAT_CHECKBOX1,
	KK_IMG_VIEWCHAT_CHECKBOX2,
	KK_IMG_VIEWCHAT_WAITING,
	KK_IMG_VIEWCHAT_SLOGO,
	KK_IMG_VIEWCHAT_LOGO,
	KK_IMG_VIEWCHAT_UP_ARROW,
	KK_IMG_VIEWCHAT_DOWN_ARROW,

	KK_IMG_VIEWCHAT_V2_KK_TITLE,
	KK_IMG_VIEWCHAT_V2_KK_TITLE_MIDDLE,
	KK_IMG_VIEWCHAT_V2_KK_TITLE_MINI,
	KK_IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_CH,
	KK_IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_EN,
	KK_IMG_VIEWCHAT_V2_CHAT_WAITING,
	KK_IMG_VIEWCHAT_V2_CHAT_WAITING_MINI,
	KK_IMG_VIEWCHAT_V2_BUDDY_NAME,
	KK_IMG_VIEWCHAT_V2_DURING_TIME,
	KK_IMG_VIEWCHAT_V2_GPRS_SIG00,
	KK_IMG_VIEWCHAT_V2_GPRS_SIG11,
	KK_IMG_VIEWCHAT_V2_GPRS_SIG22,
	KK_IMG_VIEWCHAT_V2_GPRS_SIG33,
	KK_IMG_VIEWCHAT_V2_GPRS_SIG44,
	KK_IMG_MAX
} kk_img;

#define SIG_LEVEL_MAX 	4

//#endif 

/* __KK_UI_V2__ */

#endif
