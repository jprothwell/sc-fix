/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, Coolsand Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of Coolsand. The information contained in     */
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      ts.h                                                                  */
/*                                                                            */
/* DESCRIPTION                                                                */
/*    Trace Service service declarations, constant definitions and macros     */
/*                                                                            */
/******************************************************************************/

#ifndef _TS_H_
#define _TS_H_
#include "cswtype.h" //simon add
#define CFW_DRV_TS_ID         1
#define DM_TS_ID              2
#define CFW_DM_TS_ID          2
#define CFW_PM_TS_ID          3
#define BASE_BAL_TS_ID        10
#define BASE_COS_TS_ID        11
#define BASE_DB_TS_ID         12
#define BASE_REG_TS_ID        13
#define BASE_FFS_TS_ID        14
#define BASE_FAT_TS_ID        15
#define BASE_SUL_TS_ID        16
#define BASE_TM_TS_ID         17
#define BASE_VFS_TS_ID        18
#define TMS_TS_ID             19

#define CFW_AOM_TS_ID         30
#define CFW_CFG_TS_ID         31
#define CFW_NW_TS_ID          32
#define CFW_SIM_TS_ID         33
#define CFW_CC_TS_ID          34
#define CFW_SMS_TS_ID         35
#define CFW_SS_TS_ID          36
#define CFW_PBK_TS_ID         37
#define CFW_GPRS_TS_ID        38
#define CFW_CSD_TS_ID         39
#define CFW_TCPIP_TS_ID       40
#define CFW_PPP_TS_ID         41
#define CFW_SHELL_TS_ID       42
#define AT_TS_ID              50
#define MMI_TS_ID_BASE        100

#define MMI_TS_ID             (MMI_TS_ID_BASE)

#define TS_PORT_SERIAL      0x01

#define TS_PORT_USB           0x08
#define TS_PORT_FILE          0x10

#define TS_OUTPUT_FILE        0x01
#define TS_OUTPUT_VC          0x02 // VC Debug output window.
#define TS_OUTPUT_CONSOLE     0x04 // Console window.
#define TS_OUTPUT_MF32        0x08 // MF32 tools

#define TS_ERROR_LEVEL        0x01
#define TS_WARNING_LEVEL      0x02
#define TS_INFO_LEVEL         0x04
#define TS_ASSERT_LEVEL       0x08

#ifndef HAVE_NO_TS_IF
UINT32 SRVAPI TS_SetOutputMask(UINT16 nIndex, BOOL bMode);






UINT32 SRVAPI TS_OutputText(UINT16 nModuleIdx, PCSTR pFormat, ...);
VOID TS_TraceEnable(BOOL bEnable) ;
PCSTR SRVAPI TS_GetEventName(UINT32 nEvId);

//////////////////////////////////////////////////////////////////////////

//
// Redefine the compatible debug API to trace.
// 
#if defined(DEBUG)
#define TRACE0(fmt)                               TS_OutputText(0, (PCSTR)fmt) 
#define TRACE1(fmt, arg1)                         TS_OutputText(0,(PCSTR)fmt,arg1)
#define TRACE2(fmt, arg1, arg2)                   TS_OutputText(0,(PCSTR)fmt, arg1, arg2)
#define TRACE3(fmt, arg1, arg2, arg3)             TS_OutputText(0,(PCSTR)fmt, arg1, arg2, arg3)
#define TRACE4(fmt, arg1, arg2, arg3, arg4)       TS_OutputText(0,(PCSTR)fmt, arg1, arg2, arg3, arg4)
#define TRACE5(fmt, arg1, arg2, arg3, arg4, arg5) TS_OutputText(0,(PCSTR)fmt, arg1, arg2, arg3, arg4, arg5)
#define ASSERT(x)                                 DS_ASSERT(x)
#define CHECKHANDLE(handle)                       DS_CHECKHANDLE(handle)
//luanyy modify start
#ifndef MMI_ON_WIN32
#define printf                       {   }
#endif
//luanyy modify end
#else // _DEBUG
//#ifdef MMI_ON_WIN32
//#else

#define TRACE0(f)					 {	 }
#define TRACE1(f, e1)				 {	 }
#define TRACE2(f, e1, e2)			 {	 }
#define TRACE3(f, e1, e2, e3)		 {	 }
#define TRACE4(f, e1, e2, e3, e4)	 {	 }
#define TRACE5(f, e1, e2, e3, e4, e5){	 }
#define CHECKHANDLE(handle)          {	 }
#define ASSERT(x)                    {	 }
//#endif
//luanyy modify start
#ifndef MMI_ON_WIN32
#define printf                       {   }
#endif
//luanyy modify end
#endif // _DEBUG

#endif  //HAVE_NO_TS_IF
//
//add by lxp
//
#define TS_START_FUNC()  TS_OutputText(0, "%s() Start \n", __FUNCTION__); 
#define TS_END_FUNC()    TS_OutputText(0, "%s() Start \n", __FUNCTION__); 
#endif // _H_

