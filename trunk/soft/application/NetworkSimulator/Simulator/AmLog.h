/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
#pragma once

#include <stdio.h>
#include <windows.h>

#define AMLOG_SETFILENAME(filename) g_AmLog.SetFileName(filename)
#define AMLOG_SETLOGLEVEL_NOTHING g_AmLog.SetLogLevel(CAmLoglevelNothing)

#define AMLOG_SETLOGLEVEL_ERROR g_AmLog.SetLogLevel(CAmLoglevelError)
#define AMLOG_SETLOGLEVEL_INFO g_AmLog.SetLogLevel(CAmLoglevelInfo)
#define AMLOG_SETLOGLEVEL_DEVELOPERINFO g_AmLog.SetLogLevel(CAmLoglevelDeveloperInfo)

/*#define AMLOGINFO g_AmLog.Lock();\*/
#define AMLOGINFO g_AmLog.SetLineNumber(__LINE__);\
	g_AmLog.SetSourceFileName(__FILE__);\
	g_AmLog.LogInfo

/*#define AMLOGERROR g_AmLog.Lock();\*/
#define AMLOGERROR g_AmLog.SetLineNumber(__LINE__);\
	g_AmLog.SetSourceFileName(__FILE__);\
	g_AmLog.LogError

/*#define AMLOGDEVINFO g_AmLog.Lock();\*/
#define AMLOGDEVINFO g_AmLog.SetLineNumber(__LINE__);\
	g_AmLog.SetSourceFileName(__FILE__);\
	g_AmLog.LogDevInfo

typedef enum CAmLogLevels
{
	CAmLoglevelNothing,
	CAmLoglevelError,
	CAmLoglevelInfo,
	CAmLoglevelDeveloperInfo
}CAmLogLevels;

class CAmLog
{
protected:
	CRITICAL_SECTION m_cs;
	FILE *m_File;
	TCHAR m_FileName[MAX_PATH];
	CAmLogLevels m_LogLevel;
	char m_SourceFile[MAX_PATH];
	int m_LineNumber;
	void LogNow(TCHAR *LoglevelName, TCHAR *LogString);
	void ReplaceCRLF(TCHAR *s);
public:
	CAmLog();
	~CAmLog();
	void Lock() {EnterCriticalSection(&m_cs);};
	void SetLineNumber(int line) {m_LineNumber = line;};
	void SetSourceFileName(char *filename);
	bool SetFileName(TCHAR *FileName);
	void SetLogLevel(CAmLogLevels LogLevel);
	void LogError(TCHAR *format, ...);
	void LogInfo(TCHAR *format, ...);
	void LogDevInfo(TCHAR *format, ...);
};

extern CAmLog g_AmLog;










