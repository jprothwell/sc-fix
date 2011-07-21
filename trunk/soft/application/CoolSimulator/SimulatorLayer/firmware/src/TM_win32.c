//TM WIN32
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "cswtype.h"
#include "tm.h"

BOOL SRVAPI TM_SystemTimeToFileTime(
    CONST TM_SYSTEMTIME * pSystemTime,
    TM_FILETIME * pFileTime
){}

BOOL SRVAPI TM_FileTimeToSystemTime (
    CONST TM_FILETIME nFileTime,
    TM_SYSTEMTIME * pSystemTime
){}

BOOL SRVAPI TM_FormatSystemTime(
    CONST TM_SYSTEMTIME * pSystemTime,
    UINT16 uFormat,
    RESID nResFormatID,
    PSTR pDateString,
    UINT8 uDateStringSize,
    PSTR pTimeString,
    UINT8 uTimeStringSize
){}



BOOL SRVAPI TM_FormatFileTime(
    CONST TM_FILETIME nFileTime,
    UINT16 uFormat, 
    RESID nResFormatID, 
    PSTR pDateString,
    UINT8 uDateStringSize,
    PSTR pTimeString, 
    UINT8 uTimeStringSize
){}

UINT32 SRVAPI TM_GetTime (
    VOID
){}

BOOL SRVAPI TM_SetSystemTime(
    TM_SYSTEMTIME * pSystemTime
){}

BOOL SRVAPI TM_GetSystemTime(
    TM_SYSTEMTIME * pSystemTime
){}

BOOL SRVAPI TM_SetTimeFormat(
  UINT16 nTimeFormat	  //The time format parameter to be set.
){}
UINT16  SRVAPI TM_GetTimeFormat(void){}

INT8 SRVAPI TM_GetTimeZone(
    VOID
){}

BOOL SRVAPI TM_SetTimeZone(
    INT8 TimeZone
){}

BOOL SRVAPI  TM_GetSystemFileTime(
    TM_FILETIME *pFileTime
){}

BOOL SRVAPI TM_FormatDateTime(
    TM_FILETIME *pFileTime, 
    UINT16 uFormat, 
    RESID nResFormatID,
    PSTR pDateString,
    UINT8 uDateStringSize,
    PSTR pTimeString,
    UINT8 uTimeStringSize
){}

BOOL SRVAPI TM_SetLocalTime(
                           TM_SYSTEMTIME* pSystemTime
                           ){}
BOOL SRVAPI TM_GetLocalTime (
                            TM_SYSTEMTIME* pSystemTime
                            ){}
BOOL SRVAPI TM_FormatDateTimeEx(
    TM_FILETIME *pFileTime,
    PSTR pString
){}

BOOL SRVAPI TM_FormatFileTimeToStringEx(
    TM_FILETIME FileTime,
    UINT8 String[18]
){}


UINT8 SRVAPI TM_SmsTimeStampToFileTime(
    TM_SMS_TIME_STAMP ts,
    TM_FILETIME* pFileTime
){}

BOOL SRVAPI TM_ListAlarm (
    UINT8* pArrayCnt,
    TM_ALARM** pAlarmArray
){}

BOOL SRVAPI TM_SetAlarm(
    TM_ALARM* pAlarm,
    UINT8* pAlarmNum
){}

BOOL SRVAPI TM_KillAlarm (
    UINT8 nIndex
){}

BOOL SRVAPI TM_GetAlarm (
    UINT8 nIndex,
    TM_ALARM* pAlarm
){}
BOOL SRVAPI TM_ClearAllAlarm (
    VOID
){}
void SRVAPI TM_GetLastErr(UINT32* pErrorCode){}
 UINT32 csw_TMGetTick(VOID)
{
  return GetTickCount();
}
 



