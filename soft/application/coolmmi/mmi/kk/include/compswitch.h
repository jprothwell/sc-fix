#ifndef _COMPSWITCH_H
#define _COMPSWITCH_H
// compile switch

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// !!! public declare start !!!

#define WEP_DEBUG			// defined to support debug version

#define VIDEO_ROOM        // defined to chat video room

#define VIDEO_CACHE      // show cache

#define WEP_NET_SEND_OPT
#define SUPPORT_SERVER_CHANGE			// defined to support server/port change when login
#define SUPPORT_PORT_CHANGE				// defined to support port change when login

#define SUPPORT_ICON_TITLE				// defined to support icon title of screen
#define GAME_MY_TURN_PROMPT				// defined to prompt per 30s when my turn

#define GAME_BOARD_CENTERING			// defined to support game board centering

//#define SUPPORT_FTP								// defined to support WEP ftp protocol

//#define NOT_DRAW_IMAGE_OUT_CLIP		// defined to don't draw images out of current clip area

//#define UPDATE_WHOLE_SCREEN				// defined to  update whole game screen when got Playing Package

//#define UI_CTL_TEST								// defined to test UI screen

// #define GAME_STROKE_TEST						// defined to test draw strokes in game screen
// #ifdef GAME_STROKE_TEST
// 	#define GAME_STROKE_TEST_NO_NET		// must used with GAME_STROKE_TEST, test stroke when NO network
// #endif

// #define SUPPORT_TICKER							// defined to support ticker
// #define SUPPORT_ANIMATION           // defined to support Animation
// #define SUPPORT_FACE_ANIMATION			// defined to support Face Animation
//#define NEED_DO_NULL_REQUEST				// send NULL request to server when read socker timeout

// #define SEND_WHEN_TIME_OUT					// send package to server when time out

#define SUPPORT_PROXY								// defined to support proxy ,only for phone to use CMWAP
// #define SUPPORT_SOCKETPROXY							// defined to support socket proxy ,must define SUPPORT_PROXY//james 070523

#define SUPPORT_POPMENU
#if defined(CFW_DUAL_SIM)
#define SUPPORT_DUAL_SIM
#endif
#define SUPPORT_RECORD_VOICE

#define SUPPORT_BLOCK_SP_SMS
#define SUPPORT_BACK_SCREEN
#define KK_CAMERA_REFRESH_SOFTWARE

//#define _KK_DEBUG_SELECT_
#ifdef _KK_DUAL_CAM_SUPPORT_
#define SUPPORT_DUAL_CAMERA
#endif

	#define KK_CSDXXXX_V07A	71
	#define KK_CSDXXXX_V07B	72
	#define KK_CSDXXXX_V08A	81
	#define KK_CSDXXXX_V08B	82
	#define KK_CSDXXXX_V09A	91
	#define KK_CSDXXXX_V09B	92
	
	#define KK_CSDXXXX_SWVER   KK_CSDXXXX_V07B
	#define KK_CSDSWVER_MACRO  KK_CSDXXXX_V07B
	
#if (KK_MTKXXXX_SWVER < KK_MTKXXXX_V08A)
#ifdef WIN32
#undef _KK_CSDCODEC_
#endif
#endif

 //BN10065
#define SUPPORT_AUTO_LINES

//BN10117
//#define SUPPORT_DISABLE_PREVIEW

#define _KK_CREATE_ACCOUNT_
//#define _KK_SELECT_ACCOUNT_

#ifdef _KK_SELECT_ACCOUNT_
#undef _KK_CREATE_ACCOUNT_
#endif

//#define KK_SUPPORT_OVERSEASP

#ifndef __KK_UI_V2__
#define __KK_UI_V2__
#endif

#endif 

