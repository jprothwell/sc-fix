#ifndef _PRODATAUNPACK_H_
#define _PRODATAUNPACK_H_

//////////////////////////////////////////////////////////////////////////
//package type define
typedef enum
{
	TYPE_PKG_CP = 0,			//0	// CP
	TYPE_PKG_RDP,				//1	// RDP
	TYPE_PKG_UIP,				//2	// UIP
	TYPE_PKG_MULTI_UIP,			//3 // MULTI UIP
	TYPE_PKG_FP,				//4 // 
	TYPE_PKG_CHP,               //5 // CHARGE PACKAGE
}TYPE_PKG;

#define TYPE_RESPONSE_NORMAL 0
#define TYPE_RESPONSE_CACHE  1

#define TYPE_CP_TEXT					0 
#define TYPE_CP_PIC						1
#define TYPE_CP_SOUND					2
#define TYPE_CP_VIDEO_INVITE			3
#define TYPE_CP_VIDEO_END				4
#define TYPE_CP_VIDEO_ACCEPT			5
#define TYPE_CP_VIDEO_DENY				6

#define TYPE_CP_CHAT_INVITE				7
#define TYPE_CP_CHAT_END				8
#define TYPE_CP_CHAT_ACCEPT				9
#define TYPE_CP_CHAT_DENY				10
#define TYPE_CP_CHAT_ROOM_A_ENTRY		11 // admin entry video room
#define TYPE_CP_CHAT_ROOM_V_ENTRY		12 // visitor entry video room

#define TYPE_CP_ROOM_TEXT				13 //czm 090415 for 退出演播室后收到消息
/*#define PH_OFFSET_PACKAGE_LEVEL: 1 UIP; 3 CHAT ACTION; 3 CHAT TEXT; 5 CHAT SOUND; 5 CHAT PIC */
#define TYPE_PACKAGE_LEVEL_UIP			1
#define TYPE_PACKAGE_LEVEL_CP_ACTION	3
#define TYPE_PACKAGE_LEVEL_CP_TEXT		3
#define TYPE_PACKAGE_LEVEL_CP_SOUND		4
#define TYPE_PACKAGE_LEVEL_CP_PIC		5
 

// offset index of VC Package Header(toal is 0 ~ 30)
#define PH_OFFSET_HEADER_0						0		// header char 0
#define PH_OFFSET_HEADER_1						1		// header char 1
#define PH_OFFSET_SIZE_HIGH						2		// high byte of whole package size
#define PH_OFFSET_SIZE_LOW						3		// low byte of whole package size

#define PH_OFFSET_PACKAGE_TYPE					4		// package type

#define PH_OFFSET_REQUEST_CODE_HIGH				5		// high byte of the request code
#define PH_OFFSET_REQUEST_CODE_LOW				6		// low byte of the request code

#define PH_OFFSET_PLAYER_ID						7	// playerId(MSB in 7)
#define PH_OFFSET_OPP_PLAYER_ID					11		// current opp player ID

#define PH_OFFSET_SESSION_ID					15	// SessionId(MSB in 11)

#define PH_OFFSET_PACKAGE_LEVEL					19  // 3 UIP; 5 SOUND CHAT; 7 PIC CHAT

#define PH_OFFSET_REQUEST_SN					20	// serial number of request

#define PH_OFFSET_CONNECT_TYPE				22   // 0 CMNET ; 1 CMWAP

#define PH_OFFSET_LANGUAGE_TYPE				23

#define PH_OFFSET_VL						25
// offset index of Chat Package 
#define CP_OFFSET_TYPE							25
//#define CP_OFFSET_RECEIVER_ID					26
#define CP_OFFSET_RECEIVER_SERVER_ID			26
#define CP_OFFSET_RECEIVER_NICKNAME_SIZE		27
#define CP_OFFSET_SENDER_SERVER_ID				28
#define CP_OFFSET_SENDER_NICKNAME_SIZE			29
#define CP_OFFSET_DATA_SIZE_H					30
#define CP_OFFSET_DATA_SIZE_L					31
#define CP_OFFSET_VL							40

// offset index of Redirect Package
#define RDP_OFFSET_SAVE_FLAG					25
#define RDP_OFFSET_PROPERTY_SIZE_H				26
#define RDP_OFFSET_PROPERTY_SIZE_L				27
#define RDP_OFFSET_REQUEST_SIZE_H				28
#define RDP_OFFSET_REQUEST_SIZE_L				29
#define RDP_OFFSET_VL							30

// offset index of Forward Package
#define FP_OFFSET_REQUEST_SIZE_H				25
#define FP_OFFSET_REQUEST_SIZE_L				26
#define FP_OFFSET_VL							30


// offset index of WEP User Interface Package
#define UIP_OFFSET_RESPONSE_CODE				25
#define UIP_OFFSET_PRIPORITY					26
#define UIP_OFFSET_TRANSFER_TYPE				27
#define UIP_OFFSET_SOUND_ID						28

#define UIP_OFFSET_MESSAGE_SIZE					29
#define UIP_OFFSET_PROPERTY_SIZE_H				30
#define UIP_OFFSET_PROPERTY_SIZE_L				31
#define UIP_OFFSET_SCREEN_SIZE_HIGH				32
#define UIP_OFFSET_SCREEN_SIZE_LOW				33
#define UIP_OFFSET_COMMAND_SIZE_HIGH			34
#define UIP_OFFSET_COMMAND_SIZE_LOW				35
#define UIP_OFFSET_PICTURE_SIZE_HIGH			36
#define UIP_OFFSET_PICTURE_SIZE_LOW				37
#define UIP_OFFSET_SOUND_SIZE_HIGH				38
#define UIP_OFFSET_SOUND_SIZE_LOW				39

//offset paired field
#define UIP_OFFSET_VL                 40

#define MULTI_UIP_HEADER_LENGTH		20	// header length while multiple UIP in one package


// offset index of CHP Package
#define CHP_OFFSET_CHARGE						25
#define CHP_OFFSET_KEYWORD_SIZE					26
#define CHP_OFFSET_SP_TOTAL_PRICE				27
#define CHP_OFFSET_SP_PRICE						28
#define CHP_OFFSET_SP_NUMBER_SIZE				29
#define CHP_OFFSET_SP_CONTENT_SIZE				30
#define CHP_OFFSET_KEYWORD_DATA					35

// offset UIP request package
#define UIRP_OFFSET_LANGUAGE_ID					25
#define UIRP_OFFSET_MAJOR_VERSION				26 
#define UIRP_OFFSET_MINOR_VERSION				27   // 2bytes
//#define UIRP_OFFSET_CORP_VERSION				28 
#define UIRP_OFFSET_BUILD_NUMBER				29
#define UIRP_OFFSET_SERVER_ID					30 

//add for scn 0715 begin
#define UIRP_OFFSET_PROPERTY_SIZE_HIGH			31 
#define UIRP_OFFSET_PROPERTY_SIZE_LOW			32

// #define UIRP_OFFSET_CANVAS_WIDTH_HIGH			31 
// #define UIRP_OFFSET_CANVAS_WIDTH_LOW			32 
// #define UIRP_OFFSET_CANVAS_HEIGHT_HIGH			33 
// #define UIRP_OFFSET_CANVAS_HEIGHT_LOW			34 
// #define UIRP_OFFSET_COLOR_DEPTH					35 
// #define UIRP_OFFSET_SUPPORT_PIC_TYPE			36 
// #define UIRP_OFFSET_SUPPORT_SOUND_TYPE			37 
//add for scn 0715 end

#define UIRP_OFFSET_INPUT_NUM					38 
#define UIRP_OFFSET_CHOICEGROUP_NUM				39
#define UIRP_OFFSET_CHECKITEM_NUM				40
#define UIRP_OFFSET_LIST_PROPERTY_NUM			41
#define UIRP_OFFSET_HIDDEN_SIZE_HIGH			42
#define UIRP_OFFSET_HIDDEN_SIZE_LOW				43
#define UIRP_OFFSET_VL							45

//////////////////////////////////////////////////////////////////////////
// server package type define
#define SVR_PKG_UIP						0	// UIP
#define SVR_PKG_GMP						1	// GMP
#define SVR_PKG_PP						2	// PP
#define SVR_PKG_MTP						3	// MTP
#define SVR_PKG_CP						4	// CP
#define SVR_PKG_TP						5	// TP
#define SVR_PKG_RDP						6	// RDP
#define SVR_PKG_DFP						7	// DFP
#define SVR_PKG_UFP						8	// UFP
#define SVR_PKG_XUIP					9	// XUIP
#define SVR_PKG_XUCP					10	// XUCP


///////////////////////////////////////////////////////////////////////////////
// client package type define
#define CLI_PKG_RP						0		// RP
#define CLI_PKG_3GRP					1		// 3G RP
#define CLI_PKG_DFRP					2		// DFRP
#define CLI_PKG_UFRP					3		// UFRP
#define CLI_PKG_PICRP					4		// PICRP

#ifdef SEND_WHEN_TIME_OUT
#define CLI_PKG_TIMEOUT				255	// TIME OUT
#endif

///////////////////////////////////////////////////////////////////////////////
// package transfer type define
#define TRF_TYPE_RESPONSE			0	// server response to request of client
#define TRF_TYPE_PUSHED				1	// server pushed to client


///////////////////////////////////////////////////////////////////////////////
// UIP screen important mask define
#define UIP_MASK_NONCACHABLE		0x80	// MASK: non-cachable screen //for back screen
#define UIP_MASK_ICON_LIST			0x40	// MASK: use icon in list
#define UIP_MASK_CLI_SAVABLE		0x20	// MASK: savable in client  // save to file
#define UIP_MASK_SCREEN_TYPE		0x0F	// MASK: UI screen type
#ifdef SUPPORT_BACK_SCREEN
#define UIP_MASK_CACHE_FOR_BACK_ONLY 0x10   // MASK: cache screen,but only for back. If not back,should delete and send package.
#endif

// UIP screen type define (masked by UIP_MASK_SCREEN_TYPE)
#define UI_TYPE_LIST						0	// UIP type: list
#define UI_TYPE_FORM						1	// UIP type: form

// UIP item type define
#define ITM_CHOICE_GROUP					0	// UIP item: choice group
#define ITM_LIST_ITEM							1	// UIP item: item of list
#define ITM_CHOICE_ITEM						2	// UIP item: item of choice group
#define ITM_STRING								3	// UIP item: string
#define ITM_INPUT_TEXT						4	// UIP item: text input
#define ITM_INPUT_NUMBER					5	// UIP item: number input
#define ITM_INPUT_NUM_PASSWORD		6	// UIP item: numeric password input
#define ITM_PICTURE								7	// UIP item: picture
#define ITM_HIDDEN								8	// UIP item: hidden
#define ITM_MULTI_LINE_INPUT 			9	// UIP item: multi-line input text
#define ITM_INPUT_PASSWORD			 10 // UIP item: password input
#define ITM_CHECK_BOX						 11 // UIP item: password input
#define ITM_CHECK_ITEM					 12 // UIP item: password input

#ifdef SUPPORT_POPMENU
#define ITM_POPMENU							13	// UIP item: popmenu
#define ITM_POPMENU_ITEM					14  // UIP item: popmenu item
#endif

// UIP softkey type define
#define SKEY_TYPE_FTP						 -1	// FTP: special usage for FTP
#define SKEY_TYPE_COMMON					0	// common: normal process
#define SKEY_TYPE_BACK						1	// back: back to last screen
#define SKEY_TYPE_LINK						2	// link: get new requestCode and subrequestCode
#ifdef SUPPORT_POPMENU
#define SKEY_TYPE_OPTION					3
#endif
///////////////////////////////////////////////////////////////////////////////
// LGM stroke type define
#define STK_FILL_RECT							0	// filled rectangle
#define STK_DRAW_RECT							1	// rectangle
#define STK_DRAW_LINE							2	// line
#define STK_DRAW_STRING						3	// string
#define STK_COMMAND_BUTTON				4	// command button
#define STK_FILL_ELLIPSE					5	// filled Ellipse
#define STK_DRAW_ELLIPSE					6	// Ellipse
#define STK_FILL_ROUND_RECT				7	// filled rounded rectangle
#define STK_DRAW_ROUND_RECT				8	// rounded rectangle
#define STK_DRAW_IMAGE						9	// image
#define STK_DRAW_NOTHING				255	// nothing, for refresh usage

///////////////////////////////////////////////////////////////////////////////
// CHAT opponent type define
#define CHAT_OPPONENT_TYPE_WEP		0
#define CHAT_OPPONENT_TYPE_MSN		1
#define CHAT_OPPONENT_TYPE_QQ			2
#define CHAT_OPPONENT_TYPE_VIDEO_INVITE  4 // czm add for video chat invite 1220
#define CHAT_OPPONENT_TYPE_VIDEO_STOP  5 // czm add for video chat stop 20071220
#define CHAT_OPPONENT_TYPE_VIDEO_ACCEPT  6 // czm add for video chat accept 20071220
#define CHAT_OPPONENT_TYPE_VIDEO_DENY  7 // czm add for video chat deny 20071220

#define CHAT_ROOM_CMD_SYSTEM			100
#define CHAT_ROOM_CMD_USER_ENTRY	101
#define CHAT_ROOM_CMD_USER_LEAVE	102
#define CHAT_ROOM_CMD_SET_COLOR		103


///////////////////////////////////////////////////////////////////////////////
// FTP UFRP done flag define
#define UFRP_DONE_UPLOADING				0	// uploading
#define UFRP_DONE_FINISH					1	// finished
#define UFRP_DONE_EXCEPTION				2	// exception


///////////////////////////////////////////////////////////////////////////////
// WEP Package Header Chars
#define WEP_HEADER_CHAR_0					0x57	// 'W'
#define WEP_HEADER_CHAR_1					0x45	// 'E'

///////////////////////////////////////////////////////////////////////////////
// UI screen id define
// #define WUI_GAME_SCREEN						0
// 
// #define WUI_GAME_LOBBY						2
// 
#define WUI_WAIT_SCREEN						60	// waiting screen
// 
// #define WUI_CHAT_ICON_SEL					62	// chat room, icon select
// 
// #define WUI_CHAT_ROOM_OPTION			64	// chat room option
// #define WUI_CHAT_OPTION						65	// chat option
// 
// #define WUI_CHAT_STOP_VIDEO_OPTION		69  // option with video chat stop // czm add for video chat 1220
// 
// #define WUI_CHAT_FACE_SEL1				67	// chat face select 1
// #define WUI_CHAT_FACE_SEL2				68	// chat face select 2
// 
// #define WUI_MAIN_SCREEN						99  // main screen
// 
// #define WUI_ANIMATION_SELECT			236	// animation select in CHAT or CHATROOM

//#define WUI_NEW_VIDEO_CHAT_SCREEN       238 // new video chat screen // czm add for video chat 20071128
//#define WUI_VIDEO_CHAT_SCREEN			239 // video chat screen // czm add for video chat 20071128

// #define WUI_NEW_CHAT_ROOM					240	// new CHAT ROOM screen
// #define WUI_CHAT_ROOM							241	// CHAT ROOM screen
// 
// #define WUI_CHAT_ROOM_FROM_OPTION	242	// go chat room screen from option screen
// 
// #define WUI_GET_CHAT_FACE_SYM			250	// get chat face symbol
// 
// #define WUI_SOUND_CONTROL					251	// sound control
// 
// #define WUI_NEW_CHAT_SCREEN				252	// new CHAT screen
#define WUI_CHAT_SCREEN						253	// CHAT screen
//#define WUI_GET_CHAT_PHRASE				254	// get CHAT phrase

#define WUI_EXIT_SCREEN						255	// exit screen

///////////////////////////////////////////////////////////////////////////////
// request code & sub request code
#define RQ_NULL_REQUEST						0

#define RQ_SIGN_IN								10

#define RQ_GAMELOBBY							29

#define RQ_FILE_UPLOAD						40		// file upload request
#ifdef RQ_FILE_UPLOAD
  #define SRQ_FILE_UPLOAD_REQUEST		201		// file upload sub request
#endif

#define RQ_FILE_TRANSFER					41		// file transfer request
#ifdef RQ_FILE_TRANSFER
  #define SFQ_SELECT_UPLOAD_FILE		200		// select file to upload
  #define SFQ_FILE_UPLOAD_REQUEST		202		// file upload request
  #define SFQ_FILE_DOWNLOAD_REQUEST	203		// file download request
  #define SFQ_FILE_UPLOAD_RETRY			204		// file download request
  #define SFQ_FILE_DOWNLOAD_RETRY		205		// file download request
#endif

#define RQ_SMS_SEND								72		// sms send
#define RQ_WAP_DOWNLOAD						80		// need to load WAP browser
#define RQ_CALL								81		//call ,//james 070711

#define RQ_CHAT_OPTION						183		// chat option menu
#define RQ_CHATROOM_OPTION				67		// chatroom option menu
#define SRQ_CHAT_ANIMATION				235		// chat animation

 // czm add for video chat start
#define RQ_VIDEOCHAT				191 // video chat 
#define SRQ_VIDEOCHAT_MODE_SELECT		   100
#define SRQ_VIDEOCHAT_LOCAL_PIC_SELECT	   101
#define SRQ_VIDEOCHAT_INTERVAL_SET		   102
#define SRQ_VIDEOCHAT_START				   1
#define SRQ_VIDEOCHAT_STOP				   2
#define SRQ_VIDEOCHAT_ACCEPT			   3
#define SRQ_VIDEOCHAT_DENY			       4
// czm add for video chat end 

#define RQ_REQ_AND_GO							199		// do request and goto specified screen

#define RQ_GIVE_UP								205		// gave up game

#define RQ_DENY_GAME							208		// deny game invitation

#define RQ_BUDDY_INFO							221		// buddy info

#define RQ_GO_CHAT								225		// agree to CHAT
#define RQ_DENY_CHAT							226		// deny CHAT request
#define RQ_SEND_MSG								227		// send CHAT message

//#define RQ_GO_VIDEO_CHAT						228		// agree to VIDEO CHAT // czm add for video chat 20071128
//#define RQ_DENY_VIDEO_CHAT						229		// deny VIDEO CHAT request // czm add for video chat 20071128

#define RQ_DO_MY_TURN    					245

#define RQ_SEND_VIDEO_CHAT_PIC					237     // send video chat pic // czm add for video chat 20071128

#define RQ_VIEW_REMARK						240		// new added
#define RQ_FORWARD_STEP  					248
#define RQ_BACKWARD_STEP 					249
#define RQ_HOME_STEP     					250
#define RQ_LAST_STEP     					251

//#define RQ_MAX_VALUE							59999	// maximum valid request value

//### by xiawei
#define WUI_ENTER_LOGIN_SCREEN				1901
#define CLI_WEP_LOGIN						1902
#define CLI_WEP_REGISTER					1903
#define CLI_SMS_REGISTER					1905

#define CLI_CHAT_SWITCH_CAMERA				999  //切换镜头


#define CLI_WEP_EXIT						1920
#define WUI_ENTER_CHAT_SCREEN				1921
//#define CLI_CHAT_SCREEN_OPTION			1922
//#define CLI_CHAT_SCREEN_OPTION_BACK		8888
#define CLI_WEP_SWITCH_USER					1924
//#define CLI_VIDEO_CHAT_SCREEN_OPTION		1925
#define CLI_EXIT_VIDEO_CHAT					1926	
#define CLI_VIDEO_CHAT_INVITE				1932
//#define CLI_VIDEO_CHAT_INTERVAL_SET		1933
//#define CLI_VIDEO_CHAT_OPTION_INVITE		1934
#define CLI_CHAT_ADD_BUDDY					1943  //加为好友
#define CLI_VIDEOCHAT_SCREEN_SEND			9005  //视频聊天图片发送
#define CLI_CHAT_SCREEN_SEND				9006  //视频聊天文字发送

#define CLI_EXIT_CHAT_SCREEN				9010

#ifdef VIDEO_ROOM
#define RQ_ENTER_VIDEO_ROOM_ADMIN			7901   //管理员从菜单进入演播室
#define RQ_ENTER_VIDEO_ROOM_VISITOR			7902   //观众从菜单进入演播室
#define RQ_VIDEO_ROOM_PIC_SEND				7903   //演播室发送图片,admin权限
#define RQ_VIDEO_ROOM_MSG_SEND				7904   //演播室发送消息,visitor权限
#define RQ_VIDEO_ROOM_EXIT					7910   //演播室退出
#endif

#ifdef SUPPORT_POPMENU
#define CLI_SCREEN_OPTION					3   //弹出菜单选项按钮							
#endif
///////////////////////////////////////////////////////////////////////////////
// response code
#define RS_NONE										0		// null
#define RS_GET_SCREEN							10	// get prestore screen
#define RS_END_CHAT								40	// end CHAT status
#define RS_INVALID_PLAY						45	// invalid play
#define RS_IS_MY_TURN							47	// is my turn
#define RS_END_GAME								48	// game end
#define RS_VIEW_GAME							49	// start view game
#define RS_SERVER_DISCONNECT			255	// network disconnected by server
#define RS_END_XUI								60	//XUI app end
// the following only used in TP
#define RS_TP_REFRESH_SCREEN			1		// TP, refresh screen

///////////////////////////////////////////////////////////////////////////////
// File Transfer Request Package . DFRP
#define DFRP_OFFSET_RETRY_POINT				16	// file download retry start point(MSB in 16)
#define DFRP_OFFSET_CACHE_SIZE				20	// client receive cache size (KB)

// UFRP
#define UFRP_OFFSET_DONE_FLAG					16	// file upload finish flag(1:finished)

#define UFRP_OFFSET_FILE_NAME_SIZE		21	// file name size
#define UFRP_OFFSET_CLIENT_PATH_SIZE	22	// size of file path in client device
#define UFRP_OFFSET_FILE_DATA_SIZE		23	// uploading data size

#define FTRP_OFFSET_VL								30	// first variable-length field

// czm add for video chat 20071128 begin
///////////////////////////////////////////////////////////////////////////////
// PICRP
#define PICRP_OFFSET_OPP_ID				16	// OPP ID (4 bytes, MSB in 16)
#define PICRP_OFFSET_DATA_SIZE_HIGH     20
#define PICRP_OFFSET_DATA_SIZE_LOW      21

#define PICRP_OFFSET_VL					30	// first variable-length field

#define PICRP_OFFSET_PIC_ID				31  // picture id
#define PICRP_OFFSET_PIC_FRAMES			32  // picture total frames
#define PICRP_OFFSET_PIC_INDEX			33  // picture current frame index

// czm add for video chat 20071128 end

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// offset index of WEP Server Package Header(toal is 0 ~ 30)
// #define PH_OFFSET_HEADER_0						0		// header char 0
// #define PH_OFFSET_HEADER_1						1		// header char 1
// #define PH_OFFSET_SIZE_HIGH						2		// high byte of whole package size
// #define PH_OFFSET_SIZE_LOW						3		// low byte of whole package size
// #define PH_OFFSET_PACKAGE_TYPE				4		// package type
// #define PH_OFFSET_RESPONSE_CODE				5		// response code
// #define PH_OFFSET_TRANSFER_TYPE				6		// transfer type, 0: response; 1: pushed
// #define PH_OFFSET_PACKAGE_LEVEL				7		// Important level of package

///////////////////////////////////////////////////////////////////////////////

// 25~29 reserve

// #define RP_OFFSET_GAME_ID							16	// gameId(MSB in 16)
// #define RP_OFFSET_GAME_KIND						20	// current game kind(=0 if NOT in game)
// #define RP_OFFSET_CACHED_GP_VER				21	// cached GP version(0: not buffered)
// #define RP_OFFSET_SCREEN_ID						22	// id of the current screen(0 - game screen)
// #define RP_OFFSET_SELECTED_ITEM_0			23	// index of selected item 0
// #define RP_OFFSET_SELECTED_ITEM_1			24	// index of selected item 1
// #define RP_OFFSET_SELECTED_ITEM_2			25	// index of selected item 2
// #define RP_OFFSET_SELECTED_ITEM_3			26	// index of selected item 3
// #define RP_OFFSET_CHECKBOX_VALUE_HIGH	27	// high byte of check box value
// #define RP_OFFSET_CHECKBOX_VALUE_LOW	28	// low byte of check box value
// #define RP_OFFSET_CANVAS_WIDTH_HIGH		29	// high byte of canvas width
// #define RP_OFFSET_CANVAS_WIDTH_LOW		30	// low byte of canvas width
// #define RP_OFFSET_CANVAS_HEIGHT_HIGH	31	// high byte of canvas height
// #define RP_OFFSET_CANVAS_HEIGHT_LOW		32	// low byte of canvas height
// #define RP_OFFSET_COLOR_DEPTH					33	// color depth of client screen
// #define RP_OFFSET_SUPPORT_PIC_TYPE		34	// supported picture types of client device
// #define RP_OFFSET_SUPPORT_SOUND_TYPE	35	// supported sound types of client device
// #define RP_OFFSET_MY_SERVER_ID				36	// id of my server
// #define RP_OFFSET_OPP_SERVER_ID				37	// id of the opponent's server
//#define RP_OFFSET_REQUEST_SN					38	// serial number of request

// offset of "size" part of the paired
// #define RP_OFFSET_INPUT_SIZE_0				40	// size of input string 0
// #define RP_OFFSET_INPUT_SIZE_1				41	// size of input string 1
// #define RP_OFFSET_INPUT_SIZE_2				42	// size of input string 2
// #define RP_OFFSET_INPUT_SIZE_3				43	// size of input string 3
// #define RP_OFFSET_INPUT_SIZE_4				44	// size of input string 4
// #define RP_OFFSET_INPUT_SIZE_5				45	// size of input string 5
// #define RP_OFFSET_INPUT_SIZE_6				46	// size of input string 6
// #define RP_OFFSET_INPUT_SIZE_7				47	// size of input string 7
// #define RP_OFFSET_INPUT_SIZE_8				48	// size of input string 8
// 
// #define RP_OFFSET_USER_NAME_SIZE		RP_OFFSET_INPUT_SIZE_0	// size of user name(for UITP)
// #define RP_OFFSET_PASSWORD_SIZE			RP_OFFSET_INPUT_SIZE_1	// size of password (for UITP)
// #define RP_OFFSET_MOVE_SIZE					RP_OFFSET_INPUT_SIZE_2	// size of MY_TURN data(for GITP)

#define RP_OFFSET_VL									30	// first variable-length field

/////////////////////////////
#define RP_INPUT_STRING_DUMMY_BYTE	0x00	// dummy byte in long input string

/////////////////////////////////////////////////////////////////////////////
// offset index of WEP User Interface Package
// #define UIP_OFFSET_SOUND_ID						20
// #define UIP_OFFSET_PRIPORITY					21
// #define UIP_OFFSET_TICKER_ID					22
// 
// #if ((CLIENT_MAJOR_VERSION == 1) && (CLIENT_MINOR_VERSION == 0))
// 	#define UIP_OFFSET_MESSAGE_SIZE				25
// 	#define UIP_OFFSET_PROPERTY_SIZE			26
// #else
// 	#define UIP_OFFSET_MESSAGE_SIZE				24
// 	#define UIP_OFFSET_PROPERTY_SIZE_H		25
// 	#define UIP_OFFSET_PROPERTY_SIZE_L		26
// #endif
// #define UIP_OFFSET_SCREEN_SIZE_HIGH		27
// #define UIP_OFFSET_SCREEN_SIZE_LOW    28
// #define UIP_OFFSET_COMMAND_SIZE_HIGH  29
// #define UIP_OFFSET_COMMAND_SIZE_LOW   30
// #define UIP_OFFSET_PICTURE_SIZE_HIGH  31
// #define UIP_OFFSET_PICTURE_SIZE_LOW   32
// #define UIP_OFFSET_SOUND_SIZE_HIGH		33
// #define UIP_OFFSET_SOUND_SIZE_LOW			34
// 
// offset paired field
 
// 
// #define MULTI_UIP_HEADER_LENGTH		16	// header length while multiple UIP in one package

///////////////////////////////////////////////////////////////////////////////
// offset index of WEP Game Package
#define GP_OFFSET_SIZE  				16	// offset of GP Package Size (double bytes)
#define GP_OFFSET_GP_VERSION		18	// offset of GP version (double bytes)
#define GP_OFFSET_PGO_X					22	// pixelGameOriginX (double bytes)
#define GP_OFFSET_PGO_Y					24	// pixelGameOriginY (double bytes)
#define GP_OFFSET_MAX_X					26	// offset of Board MaxX
#define GP_OFFSET_MAX_Y 				27	// offset of Board maxY
#define GP_OFFSET_PGW 					28	// offset of pixelGridWidth
#define GP_OFFSET_PGH 					29	// offset of pixelGridHeight
#define GP_OFFSET_CURSOR_WIDTH	30	// offset of cursor Width
#define GP_OFFSET_CURSOR_HEIGHT	31	// offset of cursor Height
#define GP_OFFSET_CURSOR_COLOR	32	// offset of cursor Color
#define GP_OFFSET_PIECE_WIDTH		33	// offset of piece width
#define GP_OFFSET_PIECE_HEIGHT	34	// offset of piece height
#define GP_OFFSET_GAME_TYPE			35	// offset of game Type
#define GP_OFFSET_MONEY_HIGH	 	36	// offset of betHigh
#define GP_OFFSET_MONEY_LOW			37	// offset of betLow
#define GP_OFFSET_MIN_BET_MONEY	38	// offset of minBet
#define GP_OFFSET_X_OFFSET			39	// offset of xOffset(Chinese Checker)
#define GP_OFFSET_SCC						40	// offset of selectedCursorColor
#define GP_OFFSET_BOARD_WIDTH		41	// offset of game board width(double bytes)
#define GP_OFFSET_BOARD_HEIGHT	43	// offset of game board height(double bytes)
#define GP_OFFSET_BOARD_COLOR		45	// offset of game board background color
#define GP_OFFSET_GAME_ID				46	// offset of game id(MSB in 46 and LSB in 49)

//offset of paletteSize
#define GP_OFFSET_PALETTE_SIZE						50
//offset of boardStrokeNum field(51~69, totally 19 board slices)
#define GP_OFFSET_BOARD_STROKE_NUMBER_0		51
//offset of the first piece stoke number field(70~133, totally 4x16 pieces)
#define GP_OFFSET_PIECE_STROKE_NUMBER_0		70
//offset of the first cursor stoke number field(134~135, totally 2 cursors)
#define GP_OFFSET_CURSOR_STROKE_NUMBER_0	134
// offset of Picture Data Size (double bytes)
#define GP_OFFSET_PICTURE_SIZE						136
// offset of Sound Data Size (double bytes)
#define GP_OFFSET_SOUND_SIZE							138

//offset of the first variable-length field
#define GP_OFFSET_VL											150

///////////////////////////////////////////////////////////////////////////////
// offset index of WEP Playing Package
#define PP_OFFSET_BLACK_OR_WHITE    		16
#define PP_OFFSET_LAST_MOVED_X 					17
#define PP_OFFSET_LAST_MOVED_Y 					18
#define PP_OFFSET_RESTR_START_X     		19
#define PP_OFFSET_RESTR_START_Y     		20
#define PP_OFFSET_RESTR_END_X       		21
#define PP_OFFSET_RESTR_END_Y       		22
#define PP_OFFSET_SOUND_ID				   		23

#if ((CLIENT_MAJOR_VERSION == 1) && (CLIENT_MINOR_VERSION == 0))
	#define PP_OFFSET_BOARD_SIZE        		35
	#define PP_OFFSET_MESSAGE_SIZE      		36
	#define PP_OFFSET_PROPERTY_SIZE     		37
#else
	#define PP_OFFSET_BOARD_SIZE        		34
	#define PP_OFFSET_MESSAGE_SIZE      		35
	#define PP_OFFSET_PROPERTY_SIZE_H    		36
	#define PP_OFFSET_PROPERTY_SIZE_L    		37
#endif
#define PP_OFFSET_STRING_SIZE_0     		38
#define PP_OFFSET_STRING_SIZE_1     		39
#define PP_OFFSET_STRING_SIZE_2     		40
#define PP_OFFSET_STRING_SIZE_3     		41
#define PP_OFFSET_STRING_SIZE_4     		42
#define PP_OFFSET_STRING_SIZE_5     		43
#define PP_OFFSET_STRING_SIZE_6     		44
#define PP_OFFSET_STRING_SIZE_7     		45

// offset of Picture Data Size (double bytes)
#define PP_OFFSET_PICTURE_SIZE		  		46
// offset of Sound Data Size (double bytes)
#define PP_OFFSET_SOUND_SIZE  					48


//offset of the first variable-length field
#define PP_OFFSET_VL                		50


///////////////////////////////////////////////////////////////////////////////
// offset index of WEP Chatting Package
// #define CP_OFFSET_OPPONENT_ID	     			16	// OpponentId(MSB in 16 and LSB in 19)
// 
// #define CP_OFFSET_OPPONENT_TYPE					20	// 0-WEP, 1-MSN, 2-ICQ 4-video invite 5-video stop
// #define CP_OFFSET_MY_SERVER_ID					21	// id of my server
// #define CP_OFFSET_OPP_SERVER_ID					22	// id of the opponent's server
// 
// #define CP_OFFSET_NICKNAME_SIZE					25	// length of nickname
// #define CP_OFFSET_MESSAGE_SIZE					26	// length of chat message
// #if ((CLIENT_MAJOR_VERSION == 1) && (CLIENT_MINOR_VERSION == 0))
// 	#define CP_OFFSET_PROPERTY_SIZE     		27	// length of property
// #else
// 	#define CP_OFFSET_PROPERTY_SIZE_H    		27
// 	#define CP_OFFSET_PROPERTY_SIZE_L    		28
// #endif
// 
// #define CP_OFFSET_HEADER_COLOR_R    		30	// header color, R
// #define CP_OFFSET_HEADER_COLOR_G    		31  // header color, G
// #define CP_OFFSET_HEADER_COLOR_B    		32  // header color, B
// #define CP_OFFSET_CONTENT_COLOR_R    		33  // content color, R
// #define CP_OFFSET_CONTENT_COLOR_G    		34  // content color, G
// #define CP_OFFSET_CONTENT_COLOR_B    		35  // content color, B
// 
// // offset of Animation Data Size (double bytes)
// #define CP_OFFSET_ANIMATION_SIZE				36
// // offset of Sound Data Size (double bytes)
// #define CP_OFFSET_SOUND_SIZE		    		38
// 
// //offset of the first variable-length field
// #define CP_OFFSET_VL                		40

///////////////////////////////////////////////////////////////////////////////
// offset index of re-direct Package
// #define RDP_OFFSET_SAVE_FLAG						16	// if save server URL and port
// #define RDP_OFFSET_REDIRECT_SERVER_TYPE 17	// the type of the server to be re-directed to
//                                         // 0: main logon server; else: other app server
// #if ((CLIENT_MAJOR_VERSION == 1) && (CLIENT_MINOR_VERSION == 0))
// 	#define RDP_OFFSET_MESSAGE_SIZE      		25
// 	#define RDP_OFFSET_PROPERTY_SIZE     		26
// #else
// 	#define RDP_OFFSET_MESSAGE_SIZE      		24
// 	#define RDP_OFFSET_PROPERTY_SIZE_H   		25
// 	#define RDP_OFFSET_PROPERTY_SIZE_L   		26
// #endif
// #define RDP_OFFSET_REQUEST_SIZE_HIGH 		27
// #define RDP_OFFSET_REQUEST_SIZE_LOW  		28
// 
// //offset of the first variable-length field
// #define RDP_OFFSET_VL                		40

///////////////////////////////////////////////////////////////////////////////
// File Transfer Response Package . DFP
#define DFP_OFFSET_DONE_FLAG						16	// file download done flag(0:false, 1:true, 2: giveup)
#define DFP_OFFSET_FILE_DATA_SIZE				20	// client receive cache size (bytes)(MSB in 20)

#define DFP_OFFSET_VL										30	// first variable-length field

// UFP
#define UFP_OFFSET_DONE_FLAG						16	// file upload done flag(0:false, 1:true, 2: giveup)
#define UFP_OFFSET_RETRY_POINT					17	// file upload retry point(bytes)(MSB in 17)


///////////////////////////////////////////////////////////////////////////////
// offset index of WEP eXtra UI Content Package
#define XUCP_OFFSET_BLACK_OR_WHITE    		16
#define XUCP_OFFSET_LAST_MOVED_X 					17
#define XUCP_OFFSET_LAST_MOVED_Y 					18
#define XUCP_OFFSET_RESTR_START_X     		19
#define XUCP_OFFSET_RESTR_START_Y     		20
#define XUCP_OFFSET_RESTR_END_X       		21
#define XUCP_OFFSET_RESTR_END_Y       		22
#define XUCP_OFFSET_SOUND_ID				   		23

#if ((CLIENT_MAJOR_VERSION == 1) && (CLIENT_MINOR_VERSION == 0))
	#define XUCP_OFFSET_BOARD_SIZE        		35
	#define XUCP_OFFSET_MESSAGE_SIZE      		36
	#define XUCP_OFFSET_PROPERTY_SIZE     		37
#else
	#define XUCP_OFFSET_BOARD_SIZE        		34
	#define XUCP_OFFSET_MESSAGE_SIZE      		35
	#define XUCP_OFFSET_PROPERTY_SIZE_H     	36
	#define XUCP_OFFSET_PROPERTY_SIZE_L     	37
#endif

#define XUCP_OFFSET_STRING_SIZE_0     		38
#define XUCP_OFFSET_STRING_SIZE_1     		39
#define XUCP_OFFSET_STRING_SIZE_2     		40
#define XUCP_OFFSET_STRING_SIZE_3     		41
#define XUCP_OFFSET_STRING_SIZE_4     		42
#define XUCP_OFFSET_STRING_SIZE_5     		43
#define XUCP_OFFSET_STRING_SIZE_6     		44
#define XUCP_OFFSET_STRING_SIZE_7     		45
//......
#define XUCP_OFFSET_STRING_SIZE_23     		61

#define XUCP_OFFSET_HIDDEN_STRING_SIZE  	62

// offset of Softkey Data Size (double bytes)
#define XUCP_OFFSET_SOFTKEY_SIZE_HIGH  		66
#define XUCP_OFFSET_SOFTKEY_SIZE_LOW  		67

// offset of Refresh Data Size (double bytes)
#define XUCP_OFFSET_REFRESH_SIZE_HIGH			68
#define XUCP_OFFSET_REFRESH_SIZE_LOW			69

// offset of Picture Data Size (double bytes)
#define XUCP_OFFSET_PICTURE_SIZE_HIGH  		70
#define XUCP_OFFSET_PICTURE_SIZE_LOW  		71

// offset of Sound Data Size (double bytes)
#define XUCP_OFFSET_SOUND_SIZE_HIGH				72
#define XUCP_OFFSET_SOUND_SIZE_LOW				73


//offset of the first variable-length field
#define XUCP_OFFSET_VL                		80


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define CHAT_ROOM_PLAYER_ID_ALL		999999	// id of "All" in CHAT ROOM

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define NULL_REQUEST_PACKAGE_LEN	UIRP_OFFSET_VL//(RP_OFFSET_PLAYER_ID+4)		// null request package length define

#define NICKNAME_MAX_LENGTH					256
//### by xiawei
#endif//_PRODATAUNPACK_H_

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// end of file
