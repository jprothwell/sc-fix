// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__831E2592_865A_4B61_AC06_8CB28F8D0880__INCLUDED_)
#define AFX_STDAFX_H__831E2592_865A_4B61_AC06_8CB28F8D0880__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

//////////////////////////////////////////////////////////////////////////
// zhuoxz
#include "Simulator\AMLog.h"
#include "Simulator\CoolModule.h"

#define _EXPORT_NETWORK_SIMULATOR
#define MMI_ON_WIN32

#define WM_CHILD_LOOSE_FOCUS      WM_USER+101
#define WM_SEND_MSG_SIM           WM_USER+102
#define WM_WRITE_MSG_SIM          WM_USER+103
#define WM_SEND_POP_MSG           WM_USER+104
#define WM_SEND_ASYNC_EVENT_MSG   WM_USER+105
#define WM_SETTINGFOCUS           WM_USER+107
#define WM_SENDVALUE              WM_USER+106
#define WM_CHECKUNCHECK           WM_USER+108
#define WM_SET_VALUES_IN_DIALOG   WM_USER+109
#define HANDLE_DOWN_ARROW_KEY     WM_USER+110
#define HANDLE_UP_ARROW_KEY       WM_USER+111
#define HANDLE_SPACEBAR           WM_USER+112
#define WM_SEND_MSG_FROM_ADDEDIT  WM_USER+113
#define WM_ENABLE_SAVE_BUTTON     WM_USER+114
#define HANDLE_RIGHT_ARROW_KEY    WM_USER+115
#define HANDLE_LEFT_ARROW_KEY     WM_USER+116
#define MAX_LANG_RES_STRING_LEN   255	
#define MAX_STR_RES_LEN           200

#define LAYRER0 0
#define LAYRER1 1
#define LAYRER2 2
#define LAYRER3 3
#define LAYRER4 4

#define SIM1    1<<16
#define SIM2    2<<16

#define GETLAYER(param) (param & 0xFFFF)
#define GETSIMNUM(param) (param & 0xFFFF0000)

//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__831E2592_865A_4B61_AC06_8CB28F8D0880__INCLUDED_)
