#ifndef _CONSTDEF_H
#define _CONSTDEF_H

//常量及宏定义

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Exception Number
typedef enum
{
	EXC_DATA_PACKET_INVALID= 1,				// 1
	EXC_MEMORY_ALLOC_FAILED,				// 2
	EXC_UIP_SIZE_INVALID,					// 3
	EXC_UIP_FLAG_INVALID,					// 4
	EXC_UIP_SIZE_TOO_LARGE,					// 5
	EXC_UIP_PARSE_FAILED,					// 6
	EXC_UIP_ITEM_NULL,						// 7

	EXC_VIDEOCHAT_UNEXCEPCTED_ID,				// 8
	EXC_UIP_ITEM_INVALID = 100,				// UIP ITEM invalid

} EXCEPTION_NUM_DEFINE;

///////////////////////////////////////////////////////////////////////////////
// language id define
//#define ENGLISH						0
//#define CHINESE						1

typedef enum
{
	KK_LANG_ENGLISH = 0,
	KK_LANG_SCHINESE,
	KK_LANG_TCHINESE,
	
 	KK_LANG_SPANISH,
	KK_LANG_DANISH,
	KK_LANG_POLISH , 
	KK_LANG_THAI  ,
	KK_LANG_FRENCH,      
	KK_LANG_GERMAN,   
	KK_LANG_ITALIAN , 
	KK_LANG_RUSSIAN  ,
	KK_LANG_BULGARIAN ,  
	KK_LANG_UKRAINIAN ,  
	KK_LANG_PORTUGUESE ,
	KK_LANG_TURKISH  ,
	KK_LANG_VIETNAMESE ,    
	KK_LANG_INDONESIAN,  
	KK_LANG_CZECH ,
	KK_LANG_MALAY ,
	KK_LANG_NORWEGIAN , 
	KK_LANG_ARABIC ,
	KK_LANG_PERSIAN,
	KK_LANG_URDU ,
	KK_LANG_HINDI ,
	KK_LANG_TAMIL ,
	KK_LANG_BENGALI,

	KK_LANG_MAX,
} KK_LANG_ID;



///////////////////////////////////////////////////////////////////////////////
#define WEP_ON						1
#define WEP_OFF						0

///////////////////////////////////////////////////////////////////////////////
#define MAX_ICON_NUM							256

#define MAX_LOCAL_LIST_ICON_INDEX	4096	 


#define SMS_SEND_UNSUPPORTED			0		// NOT supported by device
#define SMS_SEND_OK								1		
#define SMS_SEND_FAILED						2		
#define SMS_SEND_ERROR_DATA				3		// mobileno or content error

// UI item parameters define
#define UI_MAX_CTL_NUM						32	
#define UI_MAX_CHOICEGROUP_NUM		4		 
#define UI_MAX_CHECKBOX_NUM				4		 
#define UI_MAX_TEXT_NUM						16	 
#define UI_MAX_STATIC_NUM					16	 
#define UI_MAX_PICTUIRE_NUM				20	 
#define UI_MAX_SOUND_NUM					10	 
#define UI_MAX_CHECK_ITEM_NUM			16	 

#define UI_MAX_SUBITEM_NUM					(32+1)	 
																			// +1: client add "cancel" item in buddy list
#define MAX_SUBITEM_PROPERTY_LEN	256	 

#define UI_MAX_SINGLE_STRING_LEN	256	 

#define UI_MAX_SOFTKEY_NUM				2		 
#define MAX_SOFTKEY_TITLE_LEN			20	 

#define MAX_CHAT_INPUT_CHARS			70	 

#define MAX_SCREEN_TITLE_LEN			40	 

///////////////////////////////////////////////////////////////////////////////
#define CLI_SOFTKEY_ID_BASE				60000	// client softkey id base

	
//BN10012 b:	compatible version definition 

#define KK_CSDXXXX_MT6223             11
#define KK_CSDXXXX_MT6225             12
#define KK_CSDXXXX_MT6226             13
#define KK_CSDXXXX_MT6235             14
#define KK_CSDXXXX_MT6253             15

#define  KK_CSDXXXX_BBCHIP   KK_CSDXXXX_MT6225
//base band chip info
#define KK_CSD6223         KK_CSDXXXX_MT6223
#define KK_CSD6225         KK_CSDXXXX_MT6225
#define KK_CSD6226         KK_CSDXXXX_MT6226
#define KK_CSD6235         KK_CSDXXXX_MT6235
#define KK_CSD6253         KK_CSDXXXX_MT6253

#define KK_CSDBB_CHIP         KK_CSDXXXX_BBCHIP	 

//sw base version
#define KK_CSDSW_VER07A         KK_CSDXXXX_V07A
#define KK_CSDSW_VER07B         KK_CSDXXXX_V07B
#define KK_CSDSW_VER08A         KK_CSDXXXX_V08A
#define KK_CSDSW_VER08B         KK_CSDXXXX_V08B
#define KK_CSDSW_VER09A         KK_CSDXXXX_V09A
#define KK_CSDSW_VER09B         KK_CSDXXXX_V09B

#define KK_CSDSW_VER         KK_CSDXXXX_SWVER		 
//BN10012 e

// client command id define
typedef enum
{
	CLI_CMD_BASE	= CLI_SOFTKEY_ID_BASE,

	CLI_CMD_START,				// start login
	CLI_CMD_EXIT,					// exit app
	CLI_CMD_HELP,					// help

	CLI_CMD_EXIT_GAME,		// exit game command

	CLI_CMD_HAS_MSG,			// has message

	CLI_CMD_GAME_MENU,		// game, menu command
	CLI_CMD_GAME_GIVEUP,	// game, give up command

	CLI_CMD_GIVEUP_OK,		// game give up OK
	CLI_CMD_GIVEUP_CANCEL,// game give up cancel
	
	CLI_CMD_GO_CHAT,			// client command: go chat
	CLI_CMD_DENY_CHAT,		// client command: deny chat

	CLI_CMD_CHAT_SEND,		// chat or BUDDY_LIST, send message
	CLI_CMD_CHAT_OPTION,	// chat, option

	CLI_CMD_BUDDY_OK,			// "OK" button in BUDDY_LIST screen
	CLI_CMD_BUDDY_DEL,		// "delete" button in BUDDY_LIST screen(IM)
	CLI_CMD_BUDDY_BACK,		// "back" button in BUDDY_LIST screen(chat room)

	CLI_CMD_SETTINGS_OK,	// "OK" button in MY_SETTINGS screen

	CLI_CMD_EXIT_CONFIRM_OK,//"OK" means exit WEP

	CLI_CMD_GO_VIDEO_CHAT,			// client command: go video chat // czm add for video chat 20071128
	CLI_CMD_DENY_VIDEO_CHAT,		// client command: deny video chat // czm add for video chat 20071128

	CLI_CMD_DENY_VIDEO_CHAT_RETURN, // client command: RETURN video chat // czm add for video chat 

//	CLI_CMD_VIDEOCHAT_MODE_SELECT_OK, // czm add for video chat 20071128

	CLI_CMD_VIDEOCHAT_MODE_SELECT_RETURN, // czm add for video chat 20071128 // SETTING OK

	CLI_CMD_VIDEOCHAT_CAMERA_INTERVAL_OK, // czm add for video chat 20071128 // SETTING OK

} CLIENT_SOFTKEY_IDS;

// next UI item define (used for "StrAdd" function)
#define NEXT_ITEM_NULL							0		// current is the last item
#define NEXT_ITEM_STRING_LF					1		// the next item is line feed STRING
#define NEXT_ITEM_STRING_CONT				2		// the next item is continuous STRING
#define NEXT_ITEM_NOT_OR_NEW_STRING	3		// the next item is not STRING or new STRING

// if connect to server fail this count, restore the default server
#define MAX_CONNECT_FAIL_COUNT			3
#define	MAX_DEFAULT_SERVER_URL_LEN	32

///////////////////////////////////////////////////////////////////////////////
#define DATA_FORMAT_MASK				0x80
#define DATA_FORMAT_GENERAL			0x00
#define DATA_FORMAT_CUSTOM			0x80

///////////////////////////////////////////////////////////////////////////////
#define PICTURE_FORMAT_PNG			0x01	// .png
#define PICTURE_FORMAT_GIF			0x02	// .gif
#define PICTURE_FORMAT_JPG			0x04	// .jpg
#define PICTURE_FORMAT_BMP			0x08	// .bmp
#define PICTURE_FORMAT_BREW_PNG	0x10	// .png
#define PICTURE_FORMAT_ICO			0x20	// .ico

///////////////////////////////////////////////////////////////////////////////
// device supported format of sound
#define SOUND_FORMAT_WAV				0x01	// .wav
#define SOUND_FORMAT_MIDI				0x02	// .mid
#define SOUND_FORMAT_MMF				0x04	// .mmf
#define SOUND_FORMAT_QCELP			0x08	// .qcp
#define SOUND_FORMAT_AMR			0x10     //.amr

// sound id base define
#define UI_SOUND_BASE						0x1000
#define GAME_SOUND_BASE					0x2000
#define XUI_SOUND_BASE					0x3000

///////////////////////////////////////////////////////////////////////////////
// net status define
typedef enum __NET_STATUS
{
	NET_STATUS_IDLE,
	NET_STATUS_CONNECTING,
	NET_STATUS_READING,
	NET_STATUS_WRITING
} NET_STATUS;

///////////////////////////////////////////////////////////////////////////////
#define NET_ERR_SUCCESS		 0	// net operate success
#define NET_ERR_CONNECT		-1	// net connect error
#define NET_ERR_READ			-2	// net read error
#define NET_ERR_SEND			-3	// net send error
#define NET_ERR_TIMEOUT		-4	// net recv/send time out
#define NET_ERR_OVERFLOW	-5	// net recv buffer overflow
#define NET_ERR_DATA	    -6	// net recv error data : data have not wep's prohhandle header.//james 070521
#define NET_ERR_SOCKETPROXY	    -7	// connect wep server via 10.0.0.172:80//james 070523
#define NET_ERR_CONNECTTEST	    -8	// connect test//james 070523

#define NET_IS_SUCCESS(x) ((x) >= 0)

///////////////////////////////////////////////////////////////////////////////
#define CNN_SVR_TYPE_MAIN					0		// current connected to main server
#define CNN_SVR_TYPE_NOT_LOGIN		-1	// client not login
#define CNN_SVR_TYPE_DISCONNECTED	-2	// disconnected from server

///////////////////////////////////////////////////////////////////////////////
#define MAX_INPUT_STRING_NUMBER		9			 
#define MAX_INPUT_STRING_LENGTH		254		 
#define MAX_INPUT_STRING_CONT			255		// this length is 254 and continuous with the next input

typedef enum __EWEPKEY {
	GAME_UP,		// game key up
	GAME_DOWN,	// game key down
	GAME_LEFT,	// game key left
	GAME_RIGHT,	// game key right
	GAME_OK,		// game key OK
} WEPKEY;

// client command id define
typedef enum
{
	DEV_UI_TYPE_FORM,		// form
	DEV_UI_TYPE_LIST,		// normal list
	DEV_UI_TYPE_LIST_LITTLE_ICON,	// little icon matrix list
	DEV_UI_TYPE_LIST_LARGE_ICON,	// large icon matrix list
} UI_SCREEN_TYPE;

#define MAX_UIP_LEN						1000	 
#define MAX_BACK_UIP_LEN			1500	 
#define MAX_PRESTORE_UIP_NUM	10		 
#define MAX_MESSAGE_QUEUE_NUM				3		 
#define MAX_CP_LENGTH					576		 
#define MAX_TICKER_TITLE_LEN	200		 

#define MAX_DYNAMIC_PROP_NUM	10		 
#define MAX_DYNAMIC_PROP_LEN	200		 

#ifdef SUPPORT_BACK_SCREEN
#define MAX_BACKUP_SCREEN_NUM  16
#endif
///////////////////////////////////////////////////////////////////////////////
// macro to get string pointer (according to LANGUAGE ID)
//#define GETSTRRES(str, langId) ((langId == CHINESE) ? GET_STR_CH(STR_CH_##str) : GET_STR_EN(STR_EN_##str))
//#define GETSTRRES(str, langId) GET_STR_CH(STR_CH_##str)

#define GETSTRRES(str, langId) \
	( \
		(langId == KK_LANG_ENGLISH) ? GET_STR_EN(STR_EN_##str): \
		(langId == KK_LANG_SCHINESE) ? GET_STR_CH(STR_CH_##str) : \
		GET_STR_EN(STR_EN_##str) \
	)

//hight byte
#define HBYTE(w)   ((byte_t) (((uint16_t) (w) >> 8) & 0xFF))

//low byte
#define LBYTE(w)   ((byte_t) ((w) & 0xFF))

#define _MAKE_WORD(byteH, byteL) \
				((uint16_t)((((uint16_t) ((byte_t) (byteH))) << 8) | ((byte_t) (byteL))))

// RGB_MAKE macro define
#define RGB_MAKE(r,g,b)		((COLOR_t)(((byte_t)(r)|((uint16_t)((byte_t)(g))<<8))|(((uint32_t)(byte_t)(b))<<16)))
// color operation
#define RGB_R(c)	((byte_t)((c) & 0xff))
#define RGB_G(c)	((byte_t)(((c) >> 8) & 0xff))
#define RGB_B(c)	((byte_t)(((c) >> 16) & 0xff))

#ifndef MAX
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#endif

#ifndef MIN
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )
#endif

// swap two value (note: a and b must be the SAME type!)
#define SWAP(a, b)  {(a) ^= (b); (b) ^= (a); (a) ^= (b);}

// get number of elements in an array
#define TOTAL_ELEMENTS(array)  (int32_t)( sizeof(array) / sizeof(array[0]) )

#define ISDIGIT(c)  ( (c >= '0') && (c <= '9') )

#define ISALPHA(c)  ( (uint32_t) ( ((c)|32) - 'a') < 26 )

#define ISALNUM(c)  ( ISDIGIT(c) || ISALPHA(c) )

#ifdef SUPPORT_RECORD_VOICE

#define  RECORD_STATUS_IDLE						0
#define  RECORD_STATUS_PLAYING					1
#define  RECORD_STATUS_RECORDING				2			
#define  RECORD_STATUS_RECORD_FINISH			3
#define  RECORD_STATUS_PLAY_FINISH				4
#define  RECORD_STATUS_PLAY_PAUSE				5
#define  RECORD_STATUS_RECORD_PAUSE				6
#define  RECORD_STATUS_SENDING				    7
#define  RECORD_STATUS_SEND_FINISH				8
#endif
#endif
