#ifndef _CII_DSM_CF_H_
#define _CII_DSM_CF_H_

#ifdef _MS_VC_VER_
#include <stdio.h>
#include <stdlib.h>
#include "dsm_stdtype.h" //this stdtype.h is in local directory
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <io.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <crtdbg.h>
#include <assert.h>
#include <stdio.h>
#include "dsm_stdtype.h" //this stdtype.h is in local directory
#include "drv_mmc.h"
#include "dsm_dev_driver.h"
#include "dsm_config.h"
#include "flash_simu.h"
#else 
#include <cswtype.h>
#include <sul.h>
#include <ts.h>
#include <ds.h>/*for DS_ASSERT*/
//#include <rfm.h>
#include <base_prv.h>
#include <tm.h>
#include <csw.h>
#include <csw_ver.h>
#include "drv_mmc.h"
#include "dsm_dev_driver.h"
#include "dsm_config.h"
#include "tgt_dsm_cfg.h"
#include "dbg.h"
#endif
#include "vds_api.h"

#ifdef _MS_VC_VER_

#ifdef __MMI_MULTI_SIM__  //modified by wjj 20110321
#define NUMBER_OF_SIM 3
#else
#define NUMBER_OF_SIM 1
#endif
#ifndef HKEY
typedef HANDLE HKEY;
#endif

#ifndef PHKEY
typedef HKEY* PHKEY;
#endif
#define MAKEUINT16 MAKEINT16
#define MAKEUINT32 MAKEINT32

#define BASE_DSM_TS_ID    0                 //must NOT bigger than 256
#define BASE_VDS_TS_ID    1                 //must NOT bigger than 256
#define BASE_FAT_TS_ID    2                 //must NOT bigger than 256
#define BASE_DB_TS_ID     3                 //must NOT bigger than 256
#define BASE_REG_TS_ID    4
#define BASE_FFS_TS_ID    13
#define MMI_TS_ID              6

#ifndef LEN_FOR_NULL_CHAR
#define LEN_FOR_NULL_CHAR 1
#endif

#ifndef NULL_CHAR
#define NULL_CHAR '\0'
#endif

INT32 CSW_TRACE(UINT32 id, CONST INT8 *fmt, ...);
//#define DSM_ASSERT(ex) assert(ex)
VOID DSM_Assert(BOOL ex,CONST CHAR* format, ...);
#define DSM_ASSERT DSM_Assert
#define DSM_Sleep(obj) 
#define ERR_SUCCESS 0

#define DSM_MAlloc                          malloc
#define DSM_Free                            free

#define SIZEOF                              sizeof

#define DSM_MemSet                          memset
#define DSM_MemCpy                          memcpy
#define DSM_MemCmp                          memcmp
#define DSM_MemZero(str,len)                memset((char*)(str),0,(len));
#define DSM_MemChr                             memchr

#define DSM_StrLen(str)                     strlen((char*)str)
#define DSM_StrCpy(dst,src)                 strcpy((char*)dst,(char*)src)
#define DSM_StrNCpy(dst,src,size)           strncpy((char*)dst,(char*)src,(size_t)size)
#define DSM_StrCmp(str1,str2)               strcmp((char*)str1,(char*)str2)
#define DSM_StrNCmp(str1,str2,size)         strncmp((char*)str1,(char*)str2,(size_t)size)
#define DSM_StrCaselessCmp(str1,str2)       stricmp((char*)str1,(char*)str2)
#define DSM_StrNCaselessCmp(str1,str2,size) strnicmp((char*)str1,(char*)str2,(size_t)size)
#define DSM_StrChr(str,ch)                  strchr((char*)str,(int)ch)
#define DSM_StrRChr(str,ch)                 strrchr((char*)str,(int)ch)
#define DSM_StrUpr(str)                     strupr((char*)str)
#define DSM_StrLwr(str)                     strlwr((char*)str)
#define DSM_StrCat(dst, src)                strcat((char*)dst,(char*)src)
#define DSM_StrNCat(dst,src,count)          strncat((char*)dst,(char*)src,(size_t)count)
#define DSM_StrAToI                         atoi
#define DSM_StrIToA                         itoa
#define DSM_U64ToA                         ui64toa
#define DSM_StrPrint                         sprintf
#define DSM_StrVPrint                       _vsnprintf
#define DSM_GetRandNum                      rand
#define DSM_Time(ptr)                      time(ptr)
#define DSM_StrUpper(pstr)                  _strupr((char*)pstr)
#define DSM_StrStr                          strstr
#define TSTXT(obj)                          (obj)

#define  FS_TIME(ptr)                       time(ptr)
#define sxr_Sleep                          
#define sxr_NewSemaphore(obj)       1  
#define sxr_TakeSemaphore(a)
#define sxr_ReleaseSemaphore(a)
extern UINT32 ML_Unicode2LocalLanguage(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset[12]);
extern UINT32 ML_LocalLanguage2UnicodeBigEnding(const UINT8* in, UINT32 in_len, UINT8** out, UINT32* out_len,UINT8 nCharset1[12]);
#define DM_Reset() //exits(1)
#define hal_HstSendEvent(x) printf("event:0x%x\n",x)
#define DSM_HstSendEvent hal_HstSendEvent

#else  //_MS_VC_VER_

#ifdef FS_PRIVATE_MEM_MANAGE
#define DSM_MAlloc private_malloc
#define DSM_Free private_free
void get_mega_kilo_byte( int n, int *mega, int *kilo, int *mybyte );
void* private_malloc( UINT32 size );
void private_free( void *ptr );
#else
#define DSM_MAlloc(obj)                     CSW_MALLOC(BASE_DSM_TS_ID, (obj))
#define DSM_Free(obj)                       CSW_FREE(BASE_DSM_TS_ID, (PSTR)(obj))
#endif
#define DSM_MemSet                          SUL_MemSet8
#define DSM_MemCpy                          SUL_MemCopy8
#define DSM_MemCmp                          SUL_MemCompare
#define DSM_MemZero                         SUL_ZeroMemory8
#define DSM_MemChr                             SUL_MemChr
                                            
#define DSM_StrLen(str)                     SUL_Strlen((PCSTR)(str))
#define DSM_StrCpy(dst,src)                 SUL_StrCopy((PSTR)(dst),(PCSTR)(src))
#define DSM_StrNCpy(dst,src,size)           SUL_StrNCopy((PSTR)(dst),(PCSTR)(src), (size))
#define DSM_StrCmp(str1,str2)               SUL_StrCompare((PCSTR)(str1),(PCSTR)(str2))
#define DSM_StrNCmp(str1,str2,size)         SUL_StrNCompare((PCSTR)(str1),(PCSTR)(str2), (size))
#define DSM_StrCaselessCmp(str1,str2)       SUL_StrCaselessCompare((PCSTR)str1,(PCSTR)str2)
#define DSM_StrNCaselessCmp(str1,str2,size) SUL_StrNCaselessCompare((PCSTR)str1,(PCSTR)str2,(UINT32)size)
#define DSM_StrChr(str,ch)                  SUL_StrChr((PSTR)str,(INT32)ch)
#define DSM_StrRChr(str,ch)                 strrchr2((PSTR)str,(INT32)ch)
#define DSM_StrUpr(str)                     SUL_CharUpper((PSTR)str)
#define DSM_StrCat(dst, src)                SUL_StrCat((PSTR)(dst),(PCSTR)(src))
#define DSM_StrNCat(dst,src,count)          SUL_StrNCat((PSTR)dst,(PCSTR)src,(UINT32)count)
#define DSM_StrIToA                         itoa 
#define DSM_U64ToA                         ui64toa
#define DSM_StrAToI                         SUL_StrAToI
#define DSM_StrPrint                        SUL_StrPrint
#define DSM_StrVPrint                       SUL_StrVPrint
#define  DSM_Time(ptr)                      TM_GetTime()
#define  FS_TIME(ptr)                       TM_GetTime()

#define BASE_FS_TS_ID  CFW_SMS_TS_ID
#define BASE_VDS_TS_ID CSW_TS_ID + 1
#define BASE_DSM_TS_ID BASE_FFS_TS_ID
extern void sxr_Sleep( UINT32 );
extern BOOL hal_HstSendEvent(UINT32 ch);
#define DSM_Sleep(obj) sxr_Sleep(obj)
UINT8* itoa( INT32 value, UINT8* string, UINT32 radix );

#ifdef _FS_DEBUG
extern BOOL hal_HstSendEvent(UINT32 ch);
//extern BOOL g_SendEventActive; 

extern  VOID hal_DbgAssert(CONST CHAR* format, ...);
#define DSM_HstSendEvent(hex) hal_HstSendEvent(hex)
#define DSM_ASSERT(BOOL, format, ...)                   \
{if (!(BOOL)) {                                                            \
    hal_DbgAssert(format, ##__VA_ARGS__);           \
}}
#else
#define DSM_HstSendEvent(hex) //{do{;} while(0);}
#define DSM_ASSERT(BOOL, format, ...)  //do{;}while(0)
#endif

#endif //_MS_VC_VER_
 
#ifndef _T_DSM
#define  _T_DSM(x)         (x)
#endif

#ifndef _L_DSM
#define  _L_DSM(x)         L##x 
#endif

#ifndef PCWSTR
#define PCWSTR UINT16*
#endif

#ifndef PWSTR
#define PWSTR UINT16*
#endif

#ifndef WCHAR
#define WCHAR UINT16
#endif

UINT32 tcslen(PCWSTR str);
int tcscmp(PCWSTR s1, PCWSTR s2);
int tcsncmp(PCWSTR s1, PCWSTR s2, UINT32 n);
WCHAR* tcscpy(PWSTR to, PCWSTR from);
WCHAR* tcsncpy(PWSTR dst, PCWSTR src, UINT32 n);
WCHAR* tcscat(PWSTR s, PCWSTR append);
WCHAR* tcsncat(PWSTR dst, PCWSTR src, UINT32 len);
WCHAR* tcschr(PCWSTR p, WCHAR ch);
WCHAR* tcsrchr(PCWSTR p, WCHAR ch);
WCHAR* tcsupr(PWSTR p);


#define DSM_TcStrCpy                    tcscpy
#define DSM_TcStrNCpy                   tcsncpy
#define DSM_TcStrChr                    tcschr
#define DSM_TcStrCmp                    tcscmp
#define DSM_TcStrNCmp                   tcsncmp
#define DSM_TcStrLen                    tcslen
#define DSM_TcStrCat                    tcscat
#define DSM_TcStrUpr                    tcsupr
#define DSM_Unicode2OEM ML_Unicode2LocalLanguage
#define DSM_OEM2Uincode ML_LocalLanguage2UnicodeBigEnding


UINT16 DSM_GetTime();
UINT16 DSM_GetData();
UINT32 DSM_UniteFileTime(UINT16 iDate,UINT16 iTime,UINT8 iTimeTeenth);
UINT32 DSM_GetFileTime();
UINT16 DSM_GetFileTime2Date(UINT32 iFileTime);
UINT16 DSM_GetFileTime2Time(UINT32 iFileTime);
INT16 DSM_UnicodeLen(UINT8* pszUnicode);
UINT8* ui64toa(UINT64  value,UINT8* string,UINT32 radix);
VOID DSM_Dump(UINT8* pBuff,UINT32 iTotalSize,INT32 iLineSize);
void DSM_Hex2Str(UINT8* hex,INT32 len,UINT8* bufout);
// Error Code Base for VC
#endif // _CII_VDS_CF_H_

