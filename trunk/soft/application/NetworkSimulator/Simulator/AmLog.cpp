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

/**************************************************************

	FILENAME	: AlLog.cpp

  	PURPOSE		: Logger for Simulator

	REMARKS		: nil

	AUTHOR		: Kuldeep

	DATE		: Aug 5,03

**************************************************************/

#include "stdafx.h"
#include "amlog.h"
#include <time.h>
#include <crtdbg.h>

CAmLog g_AmLog;


CAmLog::CAmLog()
{
	m_FileName[0] = '\0';
	m_LogLevel = CAmLoglevelNothing;
	m_File = NULL;
	//InitializeCriticalSection(&m_cs);
}

CAmLog::~CAmLog()
{
	if (m_File != NULL)
	{
		fclose(m_File);
	}
	//DeleteCriticalSection(&m_cs);
}
/**************************************************************

	FUNCTION NAME		: SetFileName()

  	PURPOSE				: Sets the filename for logging

	INPUT PARAMETERS	: 

	OUTPUT PARAMETERS	: 

	RETURNS				: bool

	REMARKS				: It sets the filename for logging and opens the file for logging

**************************************************************/

bool CAmLog::SetFileName(TCHAR *FileName)
{
	if (m_File != NULL)
	{
		fclose(m_File);
	}
	//we wanna save the logfile the same place as this process is...
	TCHAR file[MAX_PATH];
	int r = GetModuleFileName(NULL, file, MAX_PATH);
	_ASSERTE(r != 0);
	if (r == 0)
	{
		return false;
	}
	//now we have the filename and path, let's remove the filename so we only have the path...
	TCHAR *mid = file + r;
	while (*(--mid) != '\\' && mid > file);
	if (mid <= file)
	{
		return false;
	}
	*(++mid) = '\0';
	_tcscpy(m_FileName, file);
	_tcscat(m_FileName, FileName);
	//open the logfile...
	m_File = _tfopen(m_FileName,_T("wt"));
	_ASSERTE(m_File != NULL);
	if (m_File == NULL)
	{
		return false;
	}

	return true;
}
/**************************************************************

	FUNCTION NAME		: SetSourceFileName()

  	PURPOSE				: Sets the filename for logging

	INPUT PARAMETERS	: filename

	OUTPUT PARAMETERS	: 

	RETURNS				: void

	REMARKS				: It sets the filename for logging Filename appears with the log message.

**************************************************************/


void CAmLog::SetSourceFileName(char *filename)
{
	//strip the path from the filename...
	char *mid = filename + strlen(filename);
	while (mid > filename)
	{
		if (*(--mid) == '\\')
		{
			mid++;
			break;
		}
	}
	//store the filename...
	strcpy(m_SourceFile, mid);
}
/**************************************************************

	FUNCTION NAME		: SetLogLevel()

  	PURPOSE				: Sets the logging level to log.

	INPUT PARAMETERS	: Log levels

	OUTPUT PARAMETERS	: 

	RETURNS				: void

	REMARKS				: It sets the logging level. All levels lower then this logging level are logged

**************************************************************/



void CAmLog::SetLogLevel(CAmLogLevels LogLevel)
{
	m_LogLevel = LogLevel;
}
/**************************************************************

	FUNCTION NAME		: LogNow

  	PURPOSE				: Logs the message

	INPUT PARAMETERS	: TCHAR *LoglevelName, TCHAR *LogString

	OUTPUT PARAMETERS	: 

	RETURNS				: void

	REMARKS				: It logs the string passed with the filename.

**************************************************************/




void CAmLog::LogNow(TCHAR *LoglevelName, TCHAR *LogString)
{
	if (m_File == NULL)
	{
		_ASSERTE(!"Filename is not set...");
		return;
	}

	//get the current date and time, and format it to the format we wanna use...
	time_t now;
	time(&now);
	struct tm *tmnow = localtime(&now);
	char strnow[25];
	strftime(strnow, 24, "%Y-%m-%d %H:%M:%S", tmnow);

#ifdef _UNICODE
	if (m_LogLevel == CAmLoglevelDeveloperInfo)
	{
		fprintf(m_File, "%s\t%S\t%s, %d\t%S\r\n", strnow, LoglevelName, m_SourceFile, m_LineNumber, LogString);
	}
	else
	{
		fprintf(m_File, "%s\t%S\t%S\r\n", strnow, LoglevelName, LogString);
	}
#else
	if (m_LogLevel == CAmLoglevelDeveloperInfo)
	{
		fprintf(m_File, "%s\t%s\t%s, %d\t%s\r\n", strnow, LoglevelName, m_SourceFile, m_LineNumber, LogString);
	}
	else
	{
		fprintf(m_File, "%s\t%s\t%s\r\n", strnow, LoglevelName, LogString);
	}
#endif
#ifdef AMLOG_TRACE
	TCHAR mid[1025] = {0};
	wsprintf(mid, _T("%s\r\n"), LogString);
	OutputDebugString(mid);
#endif
#ifdef AMLOG_STDOUT
	TCHAR mid2[1025] = {0};
	wsprintf(mid2, _T("%s\r\n"), LogString);
	wprintf(mid2);
#endif
}

void CAmLog::ReplaceCRLF(TCHAR *s)
{
	TCHAR *mid = s;
	while (*mid != '\0')
	{
		switch (*mid)
		{
		case '\r':
			*mid = '|';
			break;
		case '\n':
			*mid = '|';
			break;
		}
		mid++;
	}
}

/**************************************************************

	FUNCTION NAME		: LogInfo

  	PURPOSE				: Interface for logging.information

	INPUT PARAMETERS	: TCHAR *format, ...

	OUTPUT PARAMETERS	: 

	RETURNS				: void

	REMARKS				: Exported for logging with variable arguments

**************************************************************/

void CAmLog::LogInfo(TCHAR *format, ...)
{
	if (m_LogLevel == CAmLoglevelDeveloperInfo || m_LogLevel == CAmLoglevelInfo)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("Info"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	//LeaveCriticalSection(&m_cs);
}
/**************************************************************

	FUNCTION NAME		: LogError

  	PURPOSE				: Interface for logging.errors

	INPUT PARAMETERS	: TCHAR *format, ...

	OUTPUT PARAMETERS	: 

	RETURNS				: void

	REMARKS				: Exported for logging with variable arguments

**************************************************************/
void CAmLog::LogError(TCHAR *format, ...)
{
	if (m_LogLevel == CAmLoglevelDeveloperInfo || m_LogLevel == CAmLoglevelInfo || m_LogLevel == CAmLoglevelError)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("Error"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	//LeaveCriticalSection(&m_cs);
}
/**************************************************************

	FUNCTION NAME		: LogDevInfo

  	PURPOSE				: Interface for logging.information for developers

	INPUT PARAMETERS	: TCHAR *format, ...

	OUTPUT PARAMETERS	: 

	RETURNS				: void

	REMARKS				: Exported for logging with variable arguments

**************************************************************/

void CAmLog::LogDevInfo(TCHAR *format, ...)
{
	if (m_LogLevel == CAmLoglevelDeveloperInfo)
	{
		//never corrupt the last error value...
		DWORD LastError = GetLastError();
		//do the actual logging...
		TCHAR mid[1025] = {0}; //the wvsprintf function never puts more than 1024 bytes in a string...
		va_list args;
		va_start(args, format);
		wvsprintf(mid, format, args);
		ReplaceCRLF(mid);
		LogNow(_T("DevInfo"), mid);
		va_end(args);
		SetLastError(LastError);
	}
	//LeaveCriticalSection(&m_cs);
}



















