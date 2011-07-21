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

/*******************************************************************************
* Filename:
* ---------
*  FileSystemSrc.c
*
* Project:
* --------
 
*
* Description:
* ------------
*  
*
* Author:
* -------
*  
*
*==============================================================================
* 				HISTORY
 
*------------------------------------------------------------------------------
*
*------------------------------------------------------------------------------
 
*==============================================================================
*******************************************************************************/

/**************************************************************

  FILENAME	: FileSystemSrc.h
  
    PURPOSE		: Source File for the File System 
    
 
      
 
        
          DATE		: August-01-2003
          
**************************************************************/

#ifndef FILESYSTEMSRC_C_
    #define FILESYSTEMSRC_C_
#endif

#include "../../DebugLevels/include/debuginitdef.h"

#ifdef _MS_VC_VER_
#include <stdio.h>
#include "dsm_stdtype.h"
#include "dsm_cf.h"

#include "fs.h"
#include "filesystem.h"
#include "ucs2prot.h"
#include "unicodedcl.h"
#include "fs_errcode.h"
#include "tm.h"


BOOL g_tflashCardExist = FALSE;
BOOL g_hasMountedFlashFS = FALSE;
BOOL g_hasMountedTFlashFS = FALSE;
BOOL fmt_need_abort = FALSE;

extern INT16 fat_UnicodeLen(UINT8* pszUnicode);
extern INT32 fat_Unicode2OEM(UINT8* pszUnicode,UINT16 iULen,UINT8* pszOEM,UINT16* piOLen);

BOOL TM_FileTimeToSystemTime (
                CONST TM_FILETIME nFileTime, TM_SYSTEMTIME *pSystemTime)
{
    struct tm* pTm;
    time_t FileTime;
    UINT16 iTime;

    FileTime = nFileTime.DateTime; 
    pTm = localtime(&FileTime);
    pSystemTime->uDay = pTm->tm_mday;
    pSystemTime->uDayOfWeek = pTm->tm_wday;
    pSystemTime->uHour = pTm->tm_hour;
    pSystemTime->uMilliseconds = 0;
    pSystemTime->uMinute = pTm->tm_min;
    pSystemTime->uMonth = pTm->tm_mon;
    pSystemTime->uSecond = pTm->tm_sec;
    pSystemTime->uYear = pTm->tm_year;

    return TRUE;
    
}

#else // _MS_VC_VER_

#define __NEWSIMULATOR 
#include "filesystemdef.h"
#include "ucs2prot.h"
#undef __NEWSIMULATOR
#include "mmi_features.h"
#include "fat_fs.h"
#include "mmi_trace.h"
#include "ctype.h"
#include "fs_func.h"
#include "unicodedcl.h"
#include "oslmemory.h"
#if defined(DUAL_TFLASH_SUPPORT)          
extern BOOL g_hasMountedTFlash2FS;
extern BOOL g_tflashCard2Exist;
#endif
extern BOOL g_hasMountedFlashFS;
extern BOOL g_hasMountedTFlashFS;
extern BOOL g_tflashCardExist;
extern kal_bool fmt_need_abort;
#if 1 //add gdm 080401
extern BOOL g_hasMountedNandFlashFS;
extern BOOL g_nandflashCardExist;
#endif
#endif // _MS_VC_VER_

#define MMI_FS_TFLASH_POINT  "/tflash" // The T-Flash device mount point.
#define MMI_FS_TFLASH_POINT_LEN  14 // The T-Flash device mount point length.

#define MAX_FILE_NAME_LEN		FS_FILE_NAME_LEN          //255
#define MAX_FILE_PATH_LEN		FS_PATH_LEN                  //260
#define MAX_FILE_NAME_UNICODE_LEN   FS_FILE_NAME_UNICODE_LEN
#define MAX_FILE_PATH_UNICODE_LEN   FS_PATH_UNICODE_LEN
#define MAX_DEVICE_NAME_LEN              16
#define CVTFUN_CATCHAR_LEN                 2

#define SZ_MMI__FILE_COPY_BUF   (1024)
#define MT_MAX_FILE_NUM              1024
#define FS_FREE_SPACE_SIZE          0
#define NR_MAX_DEPTH                    60
#define INVALID_FD                          -1

#define MMI_FS_PHONE_AUDIO_DIR         "Audio"
#define MMI_FS_PHONE_VIDEO_DIR         "Video"
#define MMI_FS_PHONE_PHOTO_DIR         "Photos"
#define MMI_FS_PHONE_MUSIC_DIR         "My Music"

#define MMI_FS_TFLASH_AUDIO_DIR        "D:/Audio"
#define MMI_FS_TFLASH_VIDEO_DIR         "D:/Video"
#define MMI_FS_TFLASH_PHOTO_DIR        "D:/Photos"
#define MMI_FS_TFLASH_MUSIC_DIR         "D:/My Music"
#define MMI_FS_TFLASH_EBOOK_DIR         "D:/Ebook"
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
#define MMI_FS_SEC_TFLASH_AUDIO_DIR        "F:/Audio"
#define MMI_FS_SEC_TFLASH_VIDEO_DIR         "F:/Video"
#define MMI_FS_SEC_TFLASH_PHOTO_DIR        "F:/Photos"
#define MMI_FS_SEC_TFLASH_MUSIC_DIR         "F:/My Music"
#define MMI_FS_SEC_TFLASH_EBOOK_DIR         "F:/Ebook"
#endif
/*- zhouqin modify for dual t-flash card 20110503*/

#define __FS_SUPPORT_UNICODE__
#define UPCASE(c) 				((c)>='a' && (c)<='z' ? ((c)-('a'-'A')):(c))

typedef struct
{
    U8 mount_state;
    U8 busy_status;
}Stu_DriveMapStatus;

typedef struct FSSIM_SORT_STRUCT
{
    U8 *filename;
    FS_DOSDirEntry fileinfo;
} FSSIM_SORT_T;

struct _FSSIM_FILE_INFO_NODE
{
    U8 *filename;
    FS_DOSDirEntry fileinfo;
    struct _FSSIM_FILE_INFO_NODE *next;
};
typedef struct _FSSIM_FILE_INFO_NODE FSSIM_FILE_INFO_NODE;

FSSIM_FILE_INFO_NODE *g_fssim_sort_file_list = NULL;

INT32 fssim_sort_cnt;





#if defined(DUAL_TFLASH_SUPPORT)
U8 DriveMap[5] = {'C','D','E','F','\0'};
Stu_DriveMapStatus DriveMapSatus[4] = {{0x00, 0x00},{0xff, 0x00},{0xff, 0x00},{0xff, 0x00}};
#else
U8 DriveMap[4] = {'C','D','E','\0'};
Stu_DriveMapStatus DriveMapSatus[3] = {{0x00, 0x00},{0xff, 0x00},{0xff, 0x00}};
#endif
U8 g_vCurDir[MAX_FILE_PATH_UNICODE_LEN + MMI_FS_TFLASH_POINT_LEN + LEN_FOR_UNICODE_NULL_CHAR] = {0x2F,0x00,0x00,0x00};


extern INT32 FS_GetFileSize(INT32 fd);
static INT32  FormatFlashFS(UINT8 iFlag); 
static INT32  FormatTFlash(UINT8 iFlag); 
static INT32 MakePhoneDir();
static VOID MakeTFlashDir();
void FmgrSort(INT32 len, INT32 flag);
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
static INT32  FormatSecTFlash(UINT8 iFlag);
static VOID MakeSecTFlashDir();
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
#ifdef _MS_VC_VER_
#define LEN_FOR_OEM_UNI_BUFFER      (FS_FILE_NAME_UNICODE_LEN + 2)

extern UINT8 g_Oem_Uni_Buf[LEN_FOR_OEM_UNI_BUFFER];
extern UINT16 g_ULen;

INT16 fat_UnicodeLen(UINT8* pszUnicode);

VOID ShowFileInfoList(VOID)
{
    extern FSSIM_FILE_INFO_NODE *g_fssim_sort_file_list;
    FSSIM_FILE_INFO_NODE *psCurrentNode = g_fssim_sort_file_list;

    while ( psCurrentNode )
    {        
        if (psCurrentNode->filename != NULL)
        {
            DSM_MemZero(g_Oem_Uni_Buf, LEN_FOR_OEM_UNI_BUFFER);
            fat_Unicode2OEM(psCurrentNode->filename, (UINT16)fat_UnicodeLen(psCurrentNode->filename), g_Oem_Uni_Buf, &g_ULen);
            dsm_Printf( BASE_FAT_TS_ID, "\t%s", g_Oem_Uni_Buf);
            if (FS_ATTR_DIR == psCurrentNode->fileinfo.Attributes)
            {
                dsm_Printf( BASE_FAT_TS_ID, "/\n");
            }
            else
            {
                dsm_Printf( BASE_FAT_TS_ID, "\n");
            }
        }
        psCurrentNode = psCurrentNode->next;
    }
   
    return ;
}
#endif // #ifdef _MS_VC_VER_
extern void fmgr_init_context(void);
extern void DisplayIdleScreen(void);
void enableSeeMMSFile(void)
{
	DriveMap[2] = 'C';
	fmgr_init_context();
	DisplayIdleScreen();
}
void disableSeeMMSFile(void)
{
	DriveMap[2] = 'E';
	fmgr_init_context();
	DisplayIdleScreen();
}

// Modify by suwg at 2007/6/18 for MMI_FS.
#define     _UNICODE_
#ifdef      _UNICODE_
    #define pathlen(str)                   (2 * UCS2Strlen((UINT8*)str))
    #define pathcpy(dst, src)                UCS2Strcpy((S8*)dst, (S8*)src)
    #define pathcmp(str1, str2)         UCS2Strcmp((const S8*)str1, (const S8*)str2)
    #define pathupr(str)                     UniPathUpr((UINT8*)str)
#else
    #define pathlen(str)                    strlen((char*)str)
    #define pathcpy(dst, src)                strcpy((char*)dst, (char*)src)
    #define pathcmp(str1, str2)         strcmp((char*)str1, (char*)str2)
    #define pathupr(str)               SUL_CharUpper((char*)str)
#endif

struct _MMI_FS_FileOpened
{
    char *pcFilePath;
    INT32 iFh;
    struct _MMI_FS_FileOpened *next;
};
typedef struct _MMI_FS_FileOpened MMI_FS_FileOpened;

// move to FileSystemDef.h
//#define OpenedList_SemInit() do { g_OpenedList_Sem = COS_CreateSemaphore(1); } while(0)
//#define OpenedList_Down()    do { COS_WaitForSemaphore(g_OpenedList_Sem, COS_WAIT_FOREVER); } while(0)
//#define OpenedList_Up()      do { COS_ReleaseSemaphore(g_OpenedList_Sem); } while(0)

INT16 DSM_UnicodeLen(UINT8* pszUnicode);
UINT8* UniPathUpr(UINT8* pUniPath);
INT8 UniStrCmp_IgnoreHAndL(const UINT16 *str1, const UINT16 *str2);
BOOL IsFileOpened(char *pcFilePath);
INT32 AddOpenedFileToList(char *pcFilePath, FS_HANDLE iFh);
INT32 RemoveOpenedFile(FS_HANDLE iFh);
HANDLE g_OpenedList_Sem;

MMI_FS_FileOpened *g_OpenedFileList = NULL; // Add by suwg at 2007/6/18 for FS file protect.

//-----------------------------------------------------------------------------
//功能：比较两个UNICODE串，字母不区分大小写;
//返回值：0：两个串相等；1：str1 > str2；-1：str1 < str2
//Suwg
#if 1
INT8 UniStrCmp_IgnoreHAndL(const UINT16 *str1, const UINT16 *str2)
{
    WORD ch1,ch2;
    WORD len,len1,len2;

    if(str1 == 0 || str2 == 0)
    {
        return 0;
    }
    while(str1)
    {
        if(*str1 <= 0x20)
        {
            str1 ++;
        }
        else
        {
            break;
        }
    }

    while(str2)
    {
        if(*str2 <= 0x20)
        {
            str2 ++;
        }
        else
        {
            break;
        }
    }
    
    len1 = DSM_UnicodeLen((UINT8*)str1);
    len2 = DSM_UnicodeLen((UINT8*)str2);
    len = len1 > len2 ? len2:len1;
    
    while(len > 0)
    {       
        ch1 = *str1;
        ch2 = *str2;

        if (ch1 != ch2)
        {
            // change lower  to upper.
            if((ch1 & 0xff) == ch1 && (ch1 >= 'a') && (ch1  <= 'z'))
            {
                ch1 = ch1  - ('a' - 'A');                           				
            }
	     if((ch2 & 0xff) == ch2 && (ch2 >= 'a') && (ch2 <= 'z'))
            {
                ch2 = ch2  - ('a' - 'A');                           				
            }
            return ((ch1 - ch2) > 0 ? 1 : -1);
        }
        str1++;
        str2++;
        len -=2;
    } 

    if(len1 > len2)
    {
        return -1;
    }
    else if(len1 < len2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#else
INT8 UniStrCmp_IgnoreHAndL(const UINT16 *str1, const UINT16 *str2)
{
    //U8 ch1,ch2;
    UINT32 len1 = 0,len2 = 0;
    U8* pname1 = NULL;
    U8* pname2 = NULL;
    INT32 result;

    if(str1 == 0 || str2 == 0)
    {
        return 0;
    }
    
    len1 = DSM_UnicodeLen((UINT8*)str1);
    len2 = DSM_UnicodeLen((UINT8*)str2);

        
    result = ML_Unicode2LocalLanguage((UINT8*)str1,DSM_UnicodeLen((UINT8*)str1),&pname1,&len1,NULL);
    if(result != ERR_SUCCESS)
    {        
        return 0;
    }

    result = ML_Unicode2LocalLanguage((UINT8*)str2,DSM_UnicodeLen((UINT8*)str2),&pname2,&len2,NULL);
    if(result != ERR_SUCCESS)
    {        
        return 0;
    }
  //  len = len1 > len2 ? len2:len1;
    
    result = strcmp(pname1,pname2);     
    if(pname1)
    {
       CSW_Free(pname1);
    }

    if(pname2)
    {
       CSW_Free(pname2);
    }

    return result;
}
#endif

/*WeiD Add Start For 6360 Ver: TBM780  on 2007-8-21 14:12 */
U16* UniStr_Strrchr(UINT16 *pInputStr,UINT16 c)
{
  UINT16 *p1 = pInputStr;
  UINT16* pOutStr = NULL;
  
  while ( 0 != *p1)
  {
    if (*p1 == c)
    {
     	 pOutStr = p1;
    }
    p1++;
  }
  return pOutStr;
}
/*WeiD Add End  For 6360 Ver: TBM780  on 2007-8-21 14:12 */

UINT8* UniPathUpr(UINT8* pUniPath)
{
    UINT8* p = pUniPath;

    if ( !pUniPath )
    {
        return pUniPath;
    }

    while ( *p || *(p + 1) )
    {
        if ( *p >= 'a' && *p <= 'z' && 0 == *(p + 1) )
        {
            (*p) -= 32;
        }
        p += 2;
    }

    return pUniPath;
}


BOOL IsFileOpened(char *pcFilePath)
{
    MMI_FS_FileOpened *psCurrentNode = g_OpenedFileList;
    char *pcFilePath2 = NULL;
    BOOL bRet;

    pcFilePath2 = (char*)OslMalloc(pathlen(pcFilePath) + LEN_FOR_UNICODE_NULL_CHAR);
    if ( !pcFilePath2 )
    {
        return FALSE;
    }
    pathcpy(pcFilePath2, pcFilePath);
    pathupr(pcFilePath2);

    OpenedList_Down();
    while ( psCurrentNode )
    {
        if ( 0 == pathcmp(psCurrentNode->pcFilePath, pcFilePath2) )
        {
            bRet = TRUE;
            goto end;
        }
        else
        {
            psCurrentNode = psCurrentNode->next;
        }
    }
    bRet = FALSE;

    end:
    if ( pcFilePath2 )
    {
        OslMfree(pcFilePath2);
    }
    OpenedList_Up();
    return bRet;

}


INT32 AddOpenedFileToList(char *pcFilePath, FS_HANDLE iFh)
{
    MMI_FS_FileOpened * psFileNode = NULL;
    char *pcPathBuf = NULL;
    INT32 iRet = FS_NO_ERROR;

    if ( !pcFilePath || iFh < 0 )
    {
        return FS_PARAM_ERROR;
    }

    OpenedList_Down();
    psFileNode = (MMI_FS_FileOpened*)OslMalloc(sizeof(MMI_FS_FileOpened));
    if ( !psFileNode )
    {
        iRet = FS_FAT_ALLOC_ERROR;
        goto step1_failed;
    }
    pcPathBuf = (char*)OslMalloc(pathlen(pcFilePath) + LEN_FOR_UNICODE_NULL_CHAR);
    if ( !pcPathBuf )
    {
        iRet = FS_FAT_ALLOC_ERROR;
        goto step2_failed;
    }

    pathcpy(pcPathBuf, pcFilePath);
    pathupr(pcPathBuf);
    psFileNode->pcFilePath = pcPathBuf;
    psFileNode->iFh = iFh;
    psFileNode->next = g_OpenedFileList;
    g_OpenedFileList = psFileNode;

    OpenedList_Up();
    return FS_NO_ERROR;

    step2_failed:
    OslMfree(psFileNode);
    step1_failed:
    OpenedList_Up();

    return iRet;
}


INT32 RemoveOpenedFile(FS_HANDLE iFh)
{
    MMI_FS_FileOpened *psCurrentFileNode = g_OpenedFileList;
    MMI_FS_FileOpened *psPreFileNode = NULL;
    INT32 iRet = FS_NO_ERROR;

    if ( iFh < 0 || !g_OpenedFileList )
    {
        return FS_PARAM_ERROR;
    }

    OpenedList_Down();

    if ( iFh == g_OpenedFileList->iFh )
    {
        psCurrentFileNode = g_OpenedFileList;
        g_OpenedFileList = psCurrentFileNode->next;

        if ( psCurrentFileNode->pcFilePath )
        {
            OslMfree(psCurrentFileNode->pcFilePath);
        }
        OslMfree(psCurrentFileNode);
        goto success;
    }

    psPreFileNode = psCurrentFileNode;
    while ( (psCurrentFileNode = psCurrentFileNode->next) != NULL )
    {
        if ( iFh == psCurrentFileNode->iFh )
        {
            psPreFileNode->next = psCurrentFileNode->next;
            if ( psCurrentFileNode->pcFilePath )
            {
                OslMfree(psCurrentFileNode->pcFilePath);
            }
            OslMfree(psCurrentFileNode);
            goto success;
        }
        psPreFileNode = psCurrentFileNode;
    }

    // node not found.
    OpenedList_Up();
    return ERR_FS_BAD_FD;

    success:   
    OpenedList_Up();
    return iRet;

}
// End modify.


UINT32 checkFileListDistinctness(FSSIM_FILE_INFO_NODE *psFileNode)
{
#if 0
	FSSIM_FILE_INFO_NODE *psCurrentNode = g_fssim_sort_file_list;
	if(NULL == psCurrentNode)
	{
			mmi_trace(g_sw_FMGR,"chenhe,checkFileListDistinctness,111");

		return FALSE;
	}

	for(;psCurrentNode!=NULL;psCurrentNode=psCurrentNode->next)
	{
		if(UCS2Strcmp((const S8 *) psCurrentNode->filename, (const S8 * )psFileNode->filename) == (S32)NULL )
		{
					mmi_trace(g_sw_FMGR,"chenhe,checkFileListDistinctness,222");

			return TRUE;
		}
	}
					mmi_trace(g_sw_FMGR,"chenhe,checkFileListDistinctness,333");
#endif
	return FALSE;
		

}
FSSIM_FILE_INFO_NODE* AddToFileInfoList(FSSIM_FILE_INFO_NODE *psFileNode)
{
	if ( NULL == psFileNode)
	{
		return NULL;
	}
	if(checkFileListDistinctness(psFileNode))
	{
		mmi_trace(g_sw_FMGR,"chenhe,AddToFileInfoList,fail");
		return NULL;
	}
	psFileNode->next = g_fssim_sort_file_list;
	g_fssim_sort_file_list = psFileNode;

	return g_fssim_sort_file_list;
}


FSSIM_FILE_INFO_NODE* GetFileInfoByIndex(UINT16 iIndex)
{
    FSSIM_FILE_INFO_NODE *psCurrentNode = g_fssim_sort_file_list;
    UINT16 i;

    for (i = 0; i < iIndex; ++i)
    {    
        psCurrentNode = psCurrentNode->next;
        if (NULL == psCurrentNode)
        {
            break;
        }
    }
    
    return psCurrentNode;
}

FSSIM_FILE_INFO_NODE* FreeFileInfoList(VOID)
{
    FSSIM_FILE_INFO_NODE *psCurrentNode = NULL;

    while ( g_fssim_sort_file_list )
    {
        psCurrentNode = g_fssim_sort_file_list;
        g_fssim_sort_file_list = g_fssim_sort_file_list->next;
     
        if (psCurrentNode->filename != NULL)
        {
            OslMfree(psCurrentNode->filename);
        }
        OslMfree(psCurrentNode);
    }
   
    fssim_sort_cnt = 0;
    return g_fssim_sort_file_list;
}

#ifdef MMI_ON_HARDWARE_P
/*add by wuzc at 20061218 for fs*/
INT MMI_FS_GetFirstCluster(FS_HANDLE FileHandle, U32 * Cluster)
{
   return -1;
}
#endif /* #ifdef MMI_ON_HARDWARE_P */

S32 GetFSFileSize(FS_HANDLE handle)
{
    return(0);
}
INT32 pfError(INT32 err)
{
    return(0);
}

INT32 pfOpenError(FS_HANDLE err)
{
    return(0);
}

#ifdef MMI_ON_HARDWARE_P
INT MMI_FS_Commit(FS_HANDLE FileHandle)
{
    return FS_Flush( FileHandle);
}
#endif /* #ifdef MMI_ON_HARDWARE_P */

S32 StartFileRead(FS_HANDLE handle, U32 nIndex)
{
    return(0);
}

S32 pfgetc(FS_HANDLE handle)
{
    return(0);
}
BOOL IsEndOfCurrentFile(void)
{
    return(0);
}
S32 pCacheFileRead(FS_HANDLE handle, S32 nBufferSize, S32* nRead, U8*   pBuffer)
{
    return(0);
}
S32 pCacheFileSeek(FS_HANDLE handle, S32 nBytes, S32 nType)
{
    return(0); 
}

PUBLIC S32 pfgetfilesize(FS_HANDLE hFile, S32 *pSize)
{
    return(*pSize = FS_GetFileSize(hFile)) ? FS_ERROR_RESERVED : FS_NO_ERROR;
}

/* wufasong added 2007.05.09 for Old File System APIs. */


BOOL FS_Stat (PCSTR pszPathName, FS_STAT * pStatData)
{
    INT32 iRet = FS_ERROR_RESERVED;
    FS_FILE_ATTR fileAttr;

    MMI_ASSERT((pszPathName != NULL) || (pStatData != NULL));

    memset(&fileAttr, 0x00, sizeof(FS_FILE_ATTR));

    iRet = FS_GetFileAttr(pszPathName, &fileAttr);

    if ( iRet != ERR_SUCCESS )
    {
        TRACE_EF(g_sw_FMGR, "[FS_Stat():] FS_GetFileAttr ErrCode = %d.", iRet);
        return(FALSE);
    }

    memset(pStatData, 0x00, sizeof(FS_STAT));

    pStatData->st_atime = fileAttr.i_atime;
    pStatData->st_ctime = fileAttr.i_ctime;
    pStatData->st_mode  = fileAttr.i_mode;
    pStatData->st_mtime = fileAttr.i_mtime;
    pStatData->st_size  = fileAttr.i_size;

    return(TRUE);
}

BOOL FS_Query( INT8 iQueryIndex, PVOID pBuf)
{
    INT32 iRet = FS_NO_ERROR;
    FS_INFO fsInfo;

    memset(&fsInfo, 0x00, sizeof(FS_INFO));

    iRet = FS_GetFSInfo("mmc0", &fsInfo);

    if ( iRet != ERR_SUCCESS )
    {
        TRACE_EF(g_sw_FMGR, "[FS_Query():] FS_GetFileAttr ErrCode = %d.", iRet);
        return(FALSE);
    }

    return(TRUE);
}

BOOL FS_IsDirFile(UINT8 *pszFileName)
{
    INT32 iRet = FS_ERROR_RESERVED;
    FS_FILE_ATTR fileAttr;
    INT32 len =  0;
    
    MMI_ASSERT(pszFileName != NULL);
    
    memset(&fileAttr, 0x00, sizeof(FS_FILE_ATTR));
    len = UCS2Strlen(pszFileName);
    if ( (*(pszFileName + len*2 - 2)  == '/') && (*(pszFileName + len*2 - 1)  == 0)  )
    {
        *(pszFileName + len*2 -1) = 0;
        *(pszFileName + len*2 -2) = 0;
    }

    iRet = FS_GetFileAttr(pszFileName, &fileAttr);

    TRACE_EF(g_sw_FMGR, "############# FS_IsDirFile() the file's mode is %x", fileAttr.i_mode);

    if ( iRet != ERR_SUCCESS )
    {
        // 1:Parse some component of the pathname failed.
        // 2:Parameter is invalid.
        TRACE_EF(g_sw_FMGR, "[FS_IsDirFile():] FS_GetFileAttr ErrCode = %d.", iRet);
        return(FALSE);
    }

    return(FS_IS_DIR(fileAttr.i_mode));


}


//======================================================
// strcmpi   2007-6-16       shizh create
//======================================================
INT32 strcmpi(char *str1, char *str2)
{
    INT32 ret = 0;
    INT32 i = 0;
    
    MMI_ASSERT(str1);
    MMI_ASSERT(str2);

    ret = strlen(str1) - strlen(str2);
    
    if ( ret == 0 )
    {
        ret = strlen(str1);
        for ( i = 0; i< ret; i++ )
        {
            if ( str1[i] == str2[i] )
            {
                continue;
            }
            else
            {
                return -1;
            }

        }
        return 0;
    }
    else
    {
        return -1;
    }
}

#if 1

//=======================================================
// ConvertDosC2Unix  //   2007-6-17  panxu create
//======================================================
INT ConvertDosC2Unix(U8 *pUnixName, CONST U8 *pDosName)
{
    U8 buffer[14]= {0};
    
    TBM_ENTRY(0x2840);

    if ( (*pDosName == '/') && (*(pDosName+1) == 0 ) )
    {
        pfnUnicodeStrcpy(pUnixName, pDosName);

        TBM_EXIT(0x2840);
        return pfnUnicodeStrlen(pUnixName);
    }

    if ( ((*pDosName == 'c' || *pDosName == 'C')) && (*(pDosName +1) == 0) &&
          (*(pDosName +2) == ':') &&  (*(pDosName +3) == 0) &&
          ((*(pDosName + 4) == '/') || (*(pDosName + 4)== '\\')) &&
          (*(pDosName +5) == 0))
    {
        AnsiiToUnicodeString(buffer, "/");//chenhe for jasper

        pfnUnicodeStrcpy( pUnixName, buffer);
        pfnUnicodeStrcat(pUnixName, pDosName + 6);
        TBM_EXIT(0x2840);
        return pfnUnicodeStrlen(pUnixName);
    }
    else
    {
        TBM_EXIT(0x2840);
        return FS_NO_ERROR;
    }
}
//======================================================
// ConvertDosD2Unix  //            2007-6-17       panxu           create
//======================================================

INT ConvertDosD2Unix(U8 *pUnixName, CONST U8 *pDosName)
{
    U8 buffer[20] = {0};
    
    TBM_ENTRY(0x2841);

    if ( ((*pDosName == 'd' || *pDosName == 'D')) && (*(pDosName +1) == 0) &&
          (*(pDosName +2) == ':')  && (*(pDosName +3) == 0) &&
          ((*(pDosName + 4) == '/') || (*(pDosName + 4)== '\\')) && (*(pDosName +5) == 0))
    {
        /*WeiD Modify Start For 6208 Ver: TBM780  on 2007-8-17 15:5 */
        AnsiiToUnicodeString(buffer, TFLASH_ROOT_PATH); 
        /*WeiD Modify End  For 6208 Ver: TBM780  on 2007-8-17 15:5 */
        pfnUnicodeStrcpy( pUnixName, buffer);
        pfnUnicodeStrcat(pUnixName, pDosName + 6);

        TBM_EXIT(0x2841);
        return pfnUnicodeStrlen(pUnixName);

    }
    else if ( (*pDosName == '/') && (*(pDosName+1) ==0) )
    {
        pfnUnicodeStrcpy(pUnixName, pDosName);

        TBM_EXIT(0x2841);
        return pfnUnicodeStrlen(pUnixName);
    }
    #if 1 //gdm 080402
    else if ( ((*pDosName == 'e' || *pDosName == 'E')) && (*(pDosName +1) == 0) &&
          (*(pDosName +2) == ':')  && (*(pDosName +3) == 0) &&
          ((*(pDosName + 4) == '/') || (*(pDosName + 4)== '\\')) && (*(pDosName +5) == 0))
    {
        AnsiiToUnicodeString(buffer, "/home/");//gdm 081224 vds1->E
        pfnUnicodeStrcpy( pUnixName, buffer);
        pfnUnicodeStrcat(pUnixName, pDosName + 6);

        TBM_EXIT(0x2841);
        return pfnUnicodeStrlen(pUnixName);
    }
    #endif
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    else if ( ((*pDosName == 'f' || *pDosName == 'F')) && (*(pDosName +1) == 0) &&
          (*(pDosName +2) == ':')  && (*(pDosName +3) == 0) &&
          ((*(pDosName + 4) == '/') || (*(pDosName + 4)== '\\')) && (*(pDosName +5) == 0))
    {
        AnsiiToUnicodeString(buffer, TFLASH_SECOND_PATH_NAME);
        pfnUnicodeStrcpy( pUnixName, buffer);
        pfnUnicodeStrcat(pUnixName, pDosName + 6);

        TBM_EXIT(0x2841);
        return pfnUnicodeStrlen(pUnixName);
    }
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
    else
    {
        TRACE_FMGR_FUNC("ConvertDosD2Unix Convert Failed!!!!!!!!!!!!!!!!!!!!");
        TBM_EXIT(0x2841);
        return FS_ERROR_RESERVED;
    }
}
//======================================================
// ConvertDosName2Unix  //   2007-6-17  panxu create
//======================================================

INT ConvertDosName2Unix(U8 *pUnixName, CONST U8 *pDosName)
{
    INT count = 0;
    
    TBM_ENTRY(0x2842);
    
    TRACE_FMGR_FUNCTION();

    count = ConvertDosC2Unix(pUnixName, pDosName);

    if ( count == 0 )
    {
        count = ConvertDosD2Unix(pUnixName, pDosName);
    }

    TBM_EXIT(0x2842);
    return count;    
}
//======================================================
// IsRelativeName    //   2007-6-13  panxu create
//======================================================

BOOL IsRelativeName(U8 *pName)
{
    TBM_ENTRY(0x2843);
    //dos style
    #if 0 //gdm 080402
    if ( (*pName == 'C' || *pName == 'c' || *pName == 'D' || *pName == 'd' ) && (*(pName +1) == 0))
    #else
     /*+ zhouqin modify for dual t-flash card 20110503*/
     #if defined(DUAL_TFLASH_SUPPORT)
     if ( (*pName == 'C' || *pName == 'c' || *pName == 'D' || *pName == 'd' || *pName == 'E' || *pName == 'e'  || *pName == 'F' || *pName == 'f' ) && (*(pName +1) == 0))
     #else
     if ( (*pName == 'C' || *pName == 'c' || *pName == 'D' || *pName == 'd' || *pName == 'E' || *pName == 'e' ) && (*(pName +1) == 0))
     #endif
     /*- zhouqin modify for dual t-flash card 20110503*/
    #endif
    {
        if ( (*(pName +2) == ':') && (*(pName + 3) == 0) && (*(pName + 4) == '/' || *(pName + 4) == '\\') && (*(pName + 5) == 0) )
        {
            TBM_EXIT(0x2843);
            return FALSE;
        }
        TBM_EXIT(0x2843);
        return TRUE; 
    }
    //linux style
    else if ( (*pName == '/')  && (*(pName+1) == 0) )
    {
        TBM_EXIT(0x2843);
        return FALSE;
    }
    else
    {
        TBM_EXIT(0x2843);
        return TRUE;
    }
}


//======================================================
// ConvertRelative2Full   //   2007-6-13  panxu create
//======================================================
void ConvertRelative2Full(U8 *pFullName, U8 *pRelativeName)
{
    U8 buffer[4] = "\0";
    
    TBM_ENTRY(0x2844);
    TRACE_FMGR_FUNCTION();
    MMI_ASSERT(pRelativeName != NULL);

    if ( IsRelativeName(pRelativeName) )
    {
        pfnUnicodeStrcpy(pFullName, g_vCurDir);

        if ((*(pFullName + pfnUnicodeStrlen(pFullName)*2 -2) != '/') &&
             (*(pFullName + pfnUnicodeStrlen(pFullName)*2 -1) != 0 ) )
        {
            AnsiiToUnicodeString(buffer, "/");
            pfnUnicodeStrcat(pFullName, buffer);
        }

        pfnUnicodeStrcat(pFullName, pRelativeName);

        TBM_EXIT(0x2844);
        return ;

    }
    pfnUnicodeStrcpy(pFullName, pRelativeName);

    TBM_EXIT(0x2844);
}


#else   //__FS_SUPPORT_UNICODE__

//=======================================================
// ConvertDosName2Unix  //   2007-6-13  shizh create
//=======================================================

void ConvertDosName2Unix(U8 *pUnixName, CONST U8 *pDosName)
{
    TRACE_EF(g_sw_FMGR,"############## ConvertDosName2Unix() pDosName = %s", pDosName);

    INT32 len = 0;
    INT32 i   = 0;

    MMI_ASSERT(pDosName != NULL && pUnixName != NULL);

    len = strlen(pDosName);

    // 如果该文件名是 / 开头，则直接返回 
    if ( pDosName[0] == '/' )
    {
        strcpy(pUnixName, pDosName);
        pUnixName[len] = '\0';
    }

    // 如果以C:开始，C: 盘对应 / 目录 
    if ( (pDosName[0] == 'c' || pDosName[0] == 'C') && pDosName[1] == ':' )
    {
#if  0
        pUnixName[0] = '/';

        if ( len == 3 )
        {
            pUnixName[1 ] = '\0';
        }
        else
        {
            for ( i = 2; i < len; i++ )
            {
                if ( pDosName[i] == '\\' )
                {
                    pUnixName[i - 2] = '/' ;   //unix风格比win风格少了"C:"这样的两个字符
                }
                else
                {
                    pUnixName[i - 2] = pDosName[i];
                }
            }

            pUnixName[len - 2] = '\0';

        }
#else 
        // for new CSW FS
        // C: 盘对应 /home 目录 


        if ( len == 3 )
        {
            strcpy ( pUnixName, "/home/" );
            pUnixName[ len + 3 ] = '\0';
        }
        else
        {
            strcpy ( pUnixName, "/home" );
            for ( i = 2; i < len; i++ )
            {
                if ( pDosName[i] == '\\' )
                {
                    pUnixName[ i + 3 ] = '/' ;
                }
                else
                {
                    pUnixName[i + 3 ] = pDosName[i];
                }
            }
            pUnixName[ i + 3  ] = '\0';

        }
#endif
    }
    // 如果以C:开始，D: 盘对应/tflash

    else if ( (pDosName[0] == 'd' || pDosName[0] == 'D') && pDosName[1] == ':' )
    {
        strcpy(pUnixName, "/tflash");

        if ( len == 3 )
        {
            strcat(pUnixName, '\0');
        }
        else
        {
            for ( i = 2; i < len; i++ )
            {
                if ( pDosName[i] == '\\' )
                {
                    pUnixName[i + 5] = '/';  // 因为 "/tflash" 比 "d:" 多五个字符
                }
                else
                {
                    pUnixName[ i + 5 ] = pDosName[i];
                }
            }
        }

        pUnixName[ i + 5 ] = '\0';

    }

    else           // relative name case
    {
        for ( i = 0; i < len; i++ )
        {
            if ( pDosName[i] == '\\' )
            {
                pUnixName[i] = '/' ;
            }
            else
            {
                pUnixName[i] = pDosName[i];
            }
        }
        pUnixName[len] = '\0';

    }
    TRACE_EF(g_sw_FMGR,"############## ConvertDosName2Unix() pUnixName = %s", pUnixName);
}


//==========================================================
// ConvertUnixName2Dos  //   2007-6-13  shizh create
//==========================================================

U16 ConvertUnixName2Dos(U8 *pDosName, CONST U8 *pUnixName)
{
    U16 count = 0;
    INT32 len = 0;
    INT32 i   = 0;
    INT32 posHomeStr = 0;
    INT32 posFlashStr = 0;


    MMI_ASSERT(pDosName != NULL && pUnixName != NULL);

    len = strlen(pUnixName);
    //MMI_ASSERT(len >= 5);          

    // 如果该文件名是 c，C，d, D 开头，则直接返回 
    if ( ( (pUnixName[0] == 'c') || 
    (pUnixName[0] == 'C') ||
    (pUnixName[0] == 'd') || 
    (pUnixName[0] == 'D') )  && pUnixName[1] == ':' )
    {
        strcpy(pDosName, pUnixName);
        pDosName[len] = '\0' ;
        return len;
    }

    // C: 盘对应 /home目录 

    //posHomeStr = strspn(pUnixName, "/home");
    posHomeStr = strspn(pUnixName, "/");
    posFlashStr = strspn(pUnixName, "/tflash");


    if ( (posHomeStr != 1) || (posFlashStr != 6) )
    {
        return count; // filename error.
    }

    TRACE_EF(g_sw_FMGR,"################## \
                    ConvertUnixName2Dos()  posHomeStr : %d", posHomeStr);
    //以/开头

    if ( posHomeStr == 1 )
    {
        pDosName[0] = 'C';
        pDosName[1] = ':';

        if ( len == 2 )
        {
            pDosName[2] = '\0';

            count = 3;
        }
        else
        {
            for ( i = 0; i < len; i++ )
            {
                if ( pUnixName[i] == '/' )
                {
                    pDosName[i + 2] = '\\';    // i-3 因为 “/” 比 "c:/“ 少两个字符
                }
                else
                {
                    pDosName[i + 2] = pUnixName[i];
                }
            }
            pDosName[i + 2] = '\0';
            count = i + 2 ;
        }
    }

    //以/flash开头
    else if ( posFlashStr == 7 )
    {

        pDosName[0] = 'D';
        pDosName[1] = ':';

        if ( len == 7 )
        {
            pDosName[2] = '\0';
            count = 2 ;
        }
        else
        {
            for ( i = 7; i < len; i++ )
            {
                if ( pUnixName[i] == '/' )
                {
                    pDosName[i - 5] = '\\';
                }
                else
                {
                    pDosName[i - 5 ]= pUnixName[i];
                }
            }
            pDosName[i - 5 ] = '\0';
            count = i - 5;
        }
    }

    else
    {
        for ( i = 0; i < len; i++ )
        {
            if ( pUnixName[i] == '/' )
            {
                pDosName[i] = '\\' ;
            }
            else
            {
                pDosName[i] = pUnixName[i];
            }
        }
        pDosName[len] = '\0';

        count = len ;

    }

#ifdef __FS_VDS1_IS_ROOTDIR__
    //以 /home 开头

    if ( posHomeStr == 5 )
    {
        pDosName[0] = 'C';
        pDosName[1] = ':';

        if ( len == 5 )
        {
            pDosName[2] = '\0';

            count = 2;
        }
        else
        {
            for ( i = 5; i < len; i++ )
            {
                if ( pUnixName[i] == '/' )
                {
                    pDosName[i - 3] = '\\';    // i-3 因为 “/home” 比 "c:"多三个字符
                }
                else
                {
                    pDosName[i - 3] = pUnixName[i];
                }
            }
            pDosName[i - 3] = '\0';
            count = i -3 ;
        }
    }
#endif

    return count;
}

void GetExtension(UINT8 *string, UINT8 *ext)
{
    UINT16 i = 0;
    UINT16 len = strlen(string);

    for ( i= len - 1; i >= 0 ; --i )
    {
        if ( string[i] == '.' )
        {
            strncpy((string + i + 1), ext, 3);
        }
    }
}
//=========================================================
// IsRelativeName    //   2007-6-13  shizh create
//==========================================================

BOOL IsRelativeName(U8 *pName)
{
    if ( pName[0] != 0x2f )
    {
        return TRUE;
    }
    return FALSE;
}


//=========================================================
// ConvertRelative2Full     2007-6-13  shizh create
//=========================================================
void ConvertRelative2Full(U8 *pFullName, U8 *pRelativeName)
{
    MMI_ASSERT(pRelativeName);

    U8  g_UnixCurDir[MAX_FILE_PATH_LEN] = "\0";

    if ( IsRelativeName(pRelativeName) )
    {
        ConvertDosName2Unix(g_UnixCurDir, g_vCurDir);

        TRACE_EF(g_sw_FMGR,"##############  ConvertRelative2Full() g_CurDir( win 2 Unix) = %s", g_UnixCurDir);

        strcpy(pFullName, g_UnixCurDir);
        strcat(pFullName, pRelativeName);
    }
    else
    {
        strcpy(pFullName, pRelativeName);
    }


    TRACE_EF(g_sw_FMGR,"##############  ConvertRelative2Full() pFullName = %s", pFullName);

}

#endif

void GetExtension(UINT8 *string, UINT8 *ext)
{
    INT16 i = 0;
    UINT16 len = strlen(string);

    for ( i= len - 1; i >= 0 ; --i )
    {
        if ( string[i] == '.' )
        {
            strncpy((string + i + 1), ext, 3);
        }
    }
}

void Convert2DosDateTime(FS_DOSDateTime *pDosDateTime, UINT32 time)
{
    TM_FILETIME fileTime;
    TM_SYSTEMTIME sysTime;
    
    memset(&fileTime, 0, sizeof(TM_FILETIME));
    fileTime.DateTime = time;
    memset(&sysTime, 0, sizeof(TM_SYSTEMTIME));

    TM_FileTimeToSystemTime(fileTime, &sysTime);

    pDosDateTime->Day = sysTime.uDay;
    pDosDateTime->Hour = sysTime.uHour;
    pDosDateTime->Minute = sysTime.uMinute;
    pDosDateTime->Month = sysTime.uMonth;
    pDosDateTime->Second2 = sysTime.uSecond;
    pDosDateTime->Year1980 = sysTime.uYear - 1980;
}

#ifdef MMI_ON_HARDWARE_P
//====================================================
// MMI_FS_IsExist    2011-03-01   Caoxh    create
//====================================================
BOOL MMI_FS_IsExist(const UINT8 * FileName)
{
    INT32 fd = 0;
    BOOL bRet = FALSE;
    U32 iLen = 0;
    U8* pFileName = NULL;
    U8* pUnixFileName = NULL;
	
    FS_FIND_DATA  sFindData = {0,};
    iLen = pfnUnicodeStrlen(FileName)*2 + pfnUnicodeStrlen(g_vCurDir)*2 ;

    pFileName = OslMalloc(iLen + CVTFUN_CATCHAR_LEN +  LEN_FOR_UNICODE_NULL_CHAR);  
    if(NULL == pFileName)
    {
	    MMI_ASSERT(0);
    }
    pUnixFileName = OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);
    if(NULL == pUnixFileName)
    {
    	    MMI_ASSERT(0);
    }
	
    ConvertRelative2Full((U8*)pFileName,(U8*)FileName);
    ConvertDosName2Unix(pUnixFileName, pFileName);
	
    fd = FS_FindFirstFile(FileName,&sFindData);
    if (fd >= 0 )
    {
        bRet =  TRUE;
        FS_FindClose(fd);
    }
    else
    {
        bRet = FALSE;
    }
	
    if(NULL != pFileName)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }
    if(NULL != pUnixFileName)
    {
        OslMfree(pUnixFileName);
        pUnixFileName = NULL;
    }
	
    return bRet;

}
/*WUZC Add Start For MMIFS Ver:    on 2007-1-8 18:28 */

/*
 * Function:       MMI_FS_Open
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:             FileName  Flag
 * Output:           File handle
 * Return:           File handle
 * Others:         
 */
 extern BOOL is_file_name_valid( UINT16* pathname, BOOL bIgnoreBaseName );
FS_HANDLE MMI_FS_Open(const UINT8 * FileName, INT32 Flag)
{
    INT32 fhd = FS_ERROR_RESERVED;
    INT32 NewFlag = 0;
    INT32 mode    = 0;  //It is not used nowadays
    INT32 iRet = FS_NO_ERROR;
    U8* pFileName = NULL;
    U8* pUnixFileName = NULL;
    U32 iLen = 0;
    //U32 iULen = 0; 
	U8 tmpName[FS_PATH_LEN];
	if ( (pfnUnicodeStrlen(FileName)) <  FS_FILE_NAME_LEN )
	{
	    UnicodeToAnsii((S8 *)tmpName, (S8 *)FileName);
	    mmi_trace(g_sw_FMGR,"%s\n", tmpName);
	}
	
    TBM_ENTRY(0x2830);
    TRACE_FMGR_FUNCTION();  
    MMI_ASSERT(FileName != NULL);
    
#if 1 //gdm 090804  CSW update fs_attribute
    if ( Flag & FS_ATTR_HIDDEN )
    {
        mode |= FS_ATTR_HIDDEN;
    }
#endif

    if(NULL == FileName)
    {
       TBM_EXIT(0x2830);
        return FS_PARAM_ERROR;
    }
    
    iLen = pfnUnicodeStrlen(FileName)*2 + pfnUnicodeStrlen(g_vCurDir)*2 ;

    
    pFileName = OslMalloc(iLen + CVTFUN_CATCHAR_LEN +  LEN_FOR_UNICODE_NULL_CHAR);  
    if(NULL == pFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    pUnixFileName = OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);
    if(NULL == pUnixFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    ConvertRelative2Full((U8*)pFileName, (U8*)FileName);
    ConvertDosName2Unix(pUnixFileName, pFileName);

    if( pfnUnicodeStrlen(pUnixFileName)*2 > MAX_FILE_PATH_UNICODE_LEN)
    {
        TRACE_EF(g_sw_FMGR, "MMI_FS_Open  iLen >= MAX_FILE_PATH_UNICODE_LEN iLen=%d.",iLen);
        iRet = FS_FILENAME_TOO_LONG;
        goto _func_end;
    }

    if ( Flag & FS_OPEN_DIR )
    {
        if ( FS_IsDirFile(pUnixFileName) )
        {
           iRet = FS_FOLDER_HANDLE;
           goto _func_end;
        }
        else
        {            
            iRet = FS_ERROR_RESERVED;
            TRACE_EF(g_sw_FMGR, "[MMI_FS_Open : FS_OPEN_DIR] : FS_IsDirFile Err.");
            goto _func_end;
        }
    }

    if ( Flag & FS_READ_ONLY )
    {
        NewFlag |= FS_O_RDONLY;
    }
    else if (( Flag & FS_READ_WRITE ) && ( Flag & FS_CREATE )) 
    {
        NewFlag |= (FS_O_RDWR | FS_O_CREAT );
    }
    else if ( Flag & FS_CREATE_ALWAYS )
    {
        NewFlag |= (FS_O_RDWR | FS_O_CREAT | FS_O_TRUNC);
    }
     else if ( Flag & FS_READ_WRITE )
    {
        NewFlag |= FS_O_RDWR;
    }
    else if ( Flag & FS_CREATE )
    {
        NewFlag |= (FS_O_RDWR | FS_O_CREAT | FS_O_EXCL);
    }
    else
    {
        iRet = FS_PARAM_ERROR;
        TRACE_EF(g_sw_FMGR, "[MMI_FS_Open : ] Param Err.");
        goto _func_end;
    }


    TRACE_EF(g_sw_FMGR, "NewFlag = %u", NewFlag);
#if 0
    if ( IsFileOpened(pUnixFileName) )
    {
        TRACE_EF(g_sw_FMGR, "[OpenFile] The file has been opened! ErrCode = FS_FILE_OPENED");
        iRet = FS_FILE_OPENED;
        goto _func_end;
    }
#endif
	if( !is_file_name_valid((UINT16 *) pUnixFileName, FALSE ))
	{  
		fhd = ERR_FS_INVALID_PARAMETER;
	}
	else
	{
		fhd = FS_Open(pUnixFileName, NewFlag, mode);
	}

    if ( fhd >= 0 )
    {
        TRACE_EF(g_sw_FMGR, "[OpenFile] OK! handle = %d mode=%d", fhd, mode);
      //  AddOpenedFileToList(pUnixFileName, fhd);
        iRet = fhd;
    }
    else
    {
        if( ERR_FS_FILE_NOT_EXIST == fhd)
        {
            iRet = FS_FILE_NOT_FOUND;
        }
	else if(ERR_FS_DISK_FULL == fhd)
	{
		iRet = FS_DISK_FULL;
	}
	else
	{
		iRet = FS_ERROR_RESERVED;
	}
        TRACE_EF(g_sw_FMGR, "[OpenFile]: MMI_FS_Open() open file failed! ErrCode = %d",fhd);
		if ( pfnUnicodeStrlen(pUnixFileName) <  FS_FILE_NAME_LEN )
		{
			UnicodeToAnsii((S8 *)tmpName, (S8 *)pUnixFileName);
			mmi_trace(g_sw_FMGR,"%s\n",tmpName);
		}
  	}

_func_end:
    if(NULL != pFileName)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }
    if(NULL != pUnixFileName)
    {
        OslMfree(pUnixFileName);
        pUnixFileName = NULL;
    }
    TBM_EXIT(0x2830);
#if 1 // zxb: 有了下面这行代码就可以"避免"导致mp4无法识别文件类型的问题. 原因待查...
	TRACE_EF(g_sw_FMGR, "Just a test trace information...");
#endif
    return iRet;
}
/*
 * Function:       MMI_FS_Close
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:            File handle 
 * Output:          Nal
 * Return:         FS_NO_ERROR FS_ERROR_RESERVED
 * Others:         
 */
INT MMI_FS_Close(FS_HANDLE FileHandle)
{
    TBM_ENTRY(0x2831);
    if ( FileHandle == FS_FOLDER_HANDLE )
    {
        TBM_EXIT(0x2831);
        return FS_NO_ERROR;
    }

    if ( ERR_SUCCESS == FS_Close(FileHandle) )
    {
       // RemoveOpenedFile(FileHandle);
        TRACE_EF(g_sw_FMGR, "[MMI_FS_Close] close handle = %d", FileHandle);
        TBM_EXIT(0x2831);
        return FS_NO_ERROR;
    }
    TBM_EXIT(0x2831);
    return FS_ERROR_RESERVED;

}

/*
 * Function:       MMI_FS_Write
 * Description:     For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:             FileHandle, DataPtr, Length, Written
 * Output:           Writen
 * Return:           FS_NO_ERROR FS_ERROR_RESERVED
 * Others:         
 */
//#define MMI_FS_READWRITE_BY_UNIT
#ifdef MMI_FS_READWRITE_BY_UNIT
extern BOOL mmi_audply_is_playing();
#define MMI_FS_READWRITE_UNIT_SIZE 4096
BOOL mmi_fs_isHigherPriorityTask_inUse(void)
{
	return mmi_audply_is_playing();
}
INT MMI_FS_Write_ByUnit(FS_HANDLE FileHandle, void * DataPtr, INT32 Length)
{
	INT32 fsUnit_times = Length/MMI_FS_READWRITE_UNIT_SIZE;// 4K
	INT32 rest=Length%MMI_FS_READWRITE_UNIT_SIZE;
	INT32 vi=0,lwriten=0,result=0;
	INT8 *tmpDataPtr=(UINT8 *)DataPtr;
	mmi_trace(1,"chenhe,MMI_FS_Write_ByUnit,FileHandle is %d,Length is %d,fsUnit_times is %d",FileHandle,Length,fsUnit_times);
	while(vi<fsUnit_times)
	{
		
		lwriten=FS_Write(FileHandle,tmpDataPtr,MMI_FS_READWRITE_UNIT_SIZE);
		mmi_trace(1,"chenhe,MMI_FS_Write_ByUnit,lwriten is %d",lwriten);

		if(lwriten!=MMI_FS_READWRITE_UNIT_SIZE)
		{
			return lwriten;
		}
		tmpDataPtr=tmpDataPtr+MMI_FS_READWRITE_UNIT_SIZE;
		result+=lwriten;
		vi++;
		COS_Sleep(2);
	}
	
	lwriten=FS_Write(FileHandle,tmpDataPtr,rest);
	result+=lwriten;
	mmi_trace(1,"chenhe,MMI_FS_Write_ByUnit,result is %d,Length is %d",result,Length);

	return result;

	
}

INT MMI_FS_Read_ByUnit(FS_HANDLE FileHandle, void * DataPtr, INT32 Length)
{

	INT32 fsUnit_times = Length/MMI_FS_READWRITE_UNIT_SIZE;// 4K
	INT32 rest=Length%MMI_FS_READWRITE_UNIT_SIZE;
	INT32 vi=0,lreadLen=0,result=0;
	INT8 *tmpDataPtr=(UINT8 *)DataPtr;
	mmi_trace(1,"chenhe,MMI_FS_Read_ByUnit,FileHandle is %d,Length is %d,fsUnit_times is %d,rest is %d",FileHandle,Length,fsUnit_times,rest);
	while(vi<fsUnit_times)
	{
		
		lreadLen=FS_Read(FileHandle,tmpDataPtr,MMI_FS_READWRITE_UNIT_SIZE);
			mmi_trace(1,"chenhe,MMI_FS_Read_ByUnit,lreadLen is %d",lreadLen);

		if(lreadLen!=MMI_FS_READWRITE_UNIT_SIZE)
		{
			return FS_ERROR_RESERVED;
		}
		tmpDataPtr=tmpDataPtr+MMI_FS_READWRITE_UNIT_SIZE;
		vi++;
		result+=lreadLen;
		COS_Sleep(2);
	}
	
	lreadLen=FS_Read(FileHandle,tmpDataPtr,rest);
	
	result+=lreadLen;
	mmi_trace(1,"chenhe,MMI_FS_Read_ByUnit,result is %d,Length is %d,lreadLen is %d",result,Length,lreadLen);

	return result;
	
}
#endif
INT MMI_FS_Write(FS_HANDLE FileHandle, void * DataPtr, INT32 Length, INT32 * Written)
{
    INT32 len = 0;
    
    TBM_ENTRY(0x2832);
    MMI_ASSERT(DataPtr != NULL);
    MMI_ASSERT(Written != NULL);

    if ( Length == 0 )
    {
        TBM_EXIT(0x2832);
        return FS_NO_ERROR;
    }
#ifdef MMI_FS_READWRITE_BY_UNIT	
	if(mmi_fs_isHigherPriorityTask_inUse())
	{
		len= MMI_FS_Write_ByUnit(FileHandle, DataPtr, Length);
	}
	else
#endif		
	{
		len = FS_Write(FileHandle, DataPtr, Length);
	}
    if (len >= 0)
    {
        *Written = len;
        TRACE_EF(g_sw_FMGR, "[MMI_FS_Write] has writen to fh(%d) %d bytes", FileHandle, len);
        TBM_EXIT(0x2832);
        return FS_NO_ERROR;
    }
    else
    {
        *Written = FS_ERROR_RESERVED;
        if ( ERR_FS_DISK_FULL == len )
        {
            TBM_EXIT(0x2832);
            return FS_DISK_FULL;
        }

        if ( ERR_FS_BAD_FD == len )
        {
            TBM_EXIT(0x2832);
            return FS_INVALID_FILE_HANDLE;
        }

        TRACE_EF(g_sw_FMGR, "[MMI_FS_Write]WriteError! FileHandle = %d, ErrorCode = %d", FileHandle, len);    
        TBM_EXIT(0x2832);
        return FS_ERROR_RESERVED;  //Error code
    }
}

//add 2010-0205 for QQ
INT MMI_FS_Flush(FS_HANDLE FileHandle)
{
      return FS_Flush( FileHandle);
	
}
//add end


/*
 * Function:       MMI_FS_Read
 * Description:     For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          FileHandle, DataPtr, Length, Read
 * Output:        Read 
 * Return:         FS_NO_ERROR FS_ERROR_RESERVED
 * Others:         
 */


INT MMI_FS_Read(FS_HANDLE FileHandle, void *DataPtr, INT32 Lenth, INT32*ReadLen)
{
    INT32 len = 0;
    
    TBM_ENTRY(0x2833);
    MMI_ASSERT(DataPtr != NULL);
    MMI_ASSERT(ReadLen != NULL);
	
#ifdef MMI_FS_READWRITE_BY_UNIT	
	if(mmi_fs_isHigherPriorityTask_inUse())
	{
		len= MMI_FS_Read_ByUnit(FileHandle, DataPtr, Lenth);
	}
	else
#endif		
	{
		len = FS_Read(FileHandle, DataPtr, Lenth);
	}
    
    if (len >= 0)
    {
        *ReadLen = len;
        TRACE_EF(g_sw_FMGR, "[MMI_FS_Read]   has read to fh(%d) %d bytes", FileHandle, len);
        TBM_EXIT(0x2833);
        return FS_NO_ERROR;
    }
    else
    {
        *ReadLen = FS_ERROR_RESERVED;
        if ( ERR_FS_BAD_FD == len )
        {

            TBM_EXIT(0x2833);
            return FS_INVALID_FILE_HANDLE;
        }
        TRACE_EF(g_sw_FMGR, "[MMI_FS_Read] FileHandle = %d, ErrorCode = %d", FileHandle, len);
        TBM_EXIT(0x2833);
        return FS_ERROR_RESERVED;

    }

}

/*
 * Function:         MMI_FS_Seek
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:            FileHandle, Offset, Whence
 * Output:         nal
 * Return:         The current postion of file Or fs_error_resvered
 * Others:         
 */
INT MMI_FS_Seek(FS_HANDLE FileHandle, INT32 Offset, INT32 Whence)
{
    U8   where = 0;
    INT32  pos = 0; 
    
    TBM_ENTRY(0x2834);

    switch ( Whence )
    {
        case FS_FILE_BEGIN:      // FS_FILE_BEGIN:FS_SEEK_POS_ENUM
            where = FS_SEEK_SET;
            break;
        case FS_FILE_CURRENT:
            where = FS_SEEK_CUR;
            break;
        case FS_FILE_END:
            where = FS_SEEK_END;
            break;
        default:
            TBM_EXIT(0x2834);
            return FS_ERROR_RESERVED;       
    }

    pos = FS_Seek(FileHandle,Offset,where);

    if ( pos < 0 )
    {
        TRACE_EF(g_sw_FMGR, "[SeekError!] FileHandle = %d, ErrorCode = %d", FileHandle, pos);
        TBM_EXIT(0x2834);
        return FS_ERROR_RESERVED;
    }
    TBM_EXIT(0x2834);
    return pos;
}

int MMI_FS_GetFilePosition(FS_HANDLE FileHandle, UINT *Position)
{
    INT32  pos = 0; 
    

    pos = FS_Seek(FileHandle,0,FS_SEEK_CUR);

    if ( pos < 0 )
    {
        TRACE_EF(g_sw_FMGR, "[SeekError!] FileHandle = %d, ErrorCode = %d", FileHandle, pos);
        return FS_ERROR_RESERVED;
    }
    else 
		*Position = pos;
    return pos;

}
/*
 * Function:       MMI_FS_GetFileSize
 * Description:   For FMGR & FS 
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          FileHandle, Size
 * Output:        Size 
 * Return:        FS_NO_ERROR or FS_ERROR_RESERVED
 * Others:         
 */
INT MMI_FS_GetFileSize(FS_HANDLE FileHandle, INT32 * Size)
{
    INT32 rst = 0;
    
    mmi_trace(g_sw_FMGR, "-----+++++Func: %s;File: %s;Line: %d+++++-----FileHandle=%d", __FUNCTION__, __FILE__, __LINE__, FileHandle);
    MMI_ASSERT(Size != NULL);

    rst = FS_GetFileSize(FileHandle);       
	mmi_trace(g_sw_FMGR, "-----+++++Func: rst is %d", __FUNCTION__,rst);

    if ( rst < 0 )
    {
        return  FS_ERROR_RESERVED;
    }

    *Size = rst;
    return FS_NO_ERROR;    
}

#if 1//add gdm to support ftruncate 080522
INT MMI_FS_Truncate(FS_HANDLE FileHandle)
{
	INT32 ret = 0;
	TRACE_EF(g_sw_FMGR, "[MMI_FS_Truncate:] ");

	ret = FS_ChangeSize(FileHandle, 0);
	if(ERR_SUCCESS != ret )
	{
	    TRACE_EF(g_sw_FMGR, "[MMI_FS_Truncate] call FS_ChangeSize() ret = %d", ret);
	    return FS_ERROR_RESERVED;
	}
	else
	{
	      ret = MMI_FS_Close(FileHandle);
	}

	return ret;
}
#endif

/*
 * Function:       MMI_FS_CreateDir
 * Description:   For FMGR & FS 
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          DirName
 * Output:         nal
 * Return:        FS_NO_ERROR or FS_ERROR_RESERVED 
 * Others:         
 */
INT MMI_FS_CreateDir(const UINT8 * DirName)
{
    INT32 iRet = FS_NO_ERROR;
    U8* pFileName = NULL;
    U8* pUnixFileName = NULL;
    U32 iLen = 0;
    
    TBM_ENTRY(0x2835);
    TRACE_FMGR_FUNCTION();
    MMI_ASSERT(DirName != NULL);    
    
    if(NULL == DirName)
    {
        TRACE_EF(g_sw_FMGR, "[MMI_FS_CreateDir:] (param)DirName == NULL");
        TBM_EXIT(0x2835);
        return FS_PARAM_ERROR;
    }


     iLen = pfnUnicodeStrlen(DirName)*2 + pfnUnicodeStrlen(g_vCurDir)*2 + LEN_FOR_UNICODE_NULL_CHAR;
    pFileName = OslMalloc(iLen  + CVTFUN_CATCHAR_LEN);  
    if(NULL == pFileName)
    {
        TRACE_EF(g_sw_FMGR, "[MMI_FS_CreateDir:] (Malloc)pFileName == NULL");
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    pUnixFileName = OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);

    if(NULL == pUnixFileName)
    {
        iRet = FS_ERROR_RESERVED;
        TRACE_EF(g_sw_FMGR, "[MMI_FS_CreateDir:] (Malloc)pUnixFileName == NULL");
        goto _func_end;
    }
    
    ConvertRelative2Full((U8*)pFileName,(U8*)DirName);
    ConvertDosName2Unix(pUnixFileName, pFileName);

    if(pfnUnicodeStrlen(pUnixFileName)*2 > MAX_FILE_PATH_UNICODE_LEN)
    {
        iRet = FS_FILENAME_TOO_LONG;
        TRACE_EF(g_sw_FMGR, "[MMI_FS_CreateDir:] (pUnixFileName)*2>MAX_FILE_PATH_UNICODE_LEN");       
        goto _func_end;
    }
    iLen = pfnUnicodeStrlen((S8*)pUnixFileName)*2;
    if((*(pUnixFileName + iLen -2) == '/') && (*(pUnixFileName + iLen -1) == 0))
    {
        *(pUnixFileName + iLen -2) = 0;
        *(pUnixFileName + iLen -1) = 0;
    }
    
    iRet = FS_MakeDir(pUnixFileName, 0);

    if ( iRet != ERR_SUCCESS )
    {
        TRACE_EF(g_sw_FMGR, "[MMI_FS_CreateDir:] ErrCode = %d", iRet);       
        if ( ERR_FS_FILE_EXIST == iRet )
        {
            iRet = FS_FILE_EXISTS;
            TRACE_EF(g_sw_FMGR, "[MMI_FS_CreateDir:] (ERR_FS_FILE_EXIST)");
            goto _func_end;
        }
        else if (( ERR_FS_PATHNAME_PARSE_FAILED == iRet ) || (ERR_FS_INVALID_PARAMETER == iRet) )
        {
            iRet = FS_INVALID_FILENAME;
            TRACE_EF(g_sw_FMGR, "[MMI_FS_CreateDir:] (ERR_FS_PATHNAME_PARSE_FAILED)");
            goto _func_end;
        }
        else
        {
            TRACE_EF(g_sw_FMGR, "[MMI_FS_CreateDir:] ErrCode = (%d)", iRet);
            iRet = FS_ERROR_RESERVED;
            goto _func_end;
        }
    }

    iRet = FS_NO_ERROR;
    
_func_end:
    if(NULL != pFileName)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }
    if(NULL != pUnixFileName)
    {
        OslMfree(pUnixFileName);
        pUnixFileName = NULL;
    }
    
    TBM_EXIT(0x2835);
    return iRet;
}

/*
 * Function:       MMI_FS_GetDrive
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:             Type,  Serial,  AltMask
 * Output:          nal
 * Return:         dirver index
 * Others:         
 */
INT MMI_FS_GetDrive(INT32 Type, INT32 Serial, INT32 AltMask)
{
    INT32  Result = FS_ERROR_RESERVED;
    
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    if ( (Type & (FS_DRIVE_V_NORMAL | FS_DRIVE_V_REMOVABLE|FS_DRIVE_V_NANDFLASH | FS_DRIVE_TFCARD_SECOND)) == 0 )
#else
    if ( (Type & (FS_DRIVE_V_NORMAL | FS_DRIVE_V_REMOVABLE|FS_DRIVE_V_NANDFLASH)) == 0 )
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
        return FS_PARAM_ERROR;


    /* match type first */
    switch ( Type )
    {
        case FS_DRIVE_V_NORMAL:

            if ( g_hasMountedFlashFS )
            {
                Result = DriveMap[2];//modify gdm 081224 vds1->E
            }

            break;
        case FS_DRIVE_V_REMOVABLE:
            if ( g_hasMountedTFlashFS)            
            {
                Result = DriveMap[1];
            }

            break;
        /*case FS_DRIVE_V_NANDFLASH:
            if ( g_hasMountedNandFlashFS)            
            {
                Result = DriveMap[2];
            }

            break;*/
        case FS_DRIVE_I_SYSTEM:
          if ( g_hasMountedFlashFS )
            {
                Result = DriveMap[0];//add wangrui
            }
/*+ zhouqin modify for dual t-flash card 20110503*/          
#if defined(DUAL_TFLASH_SUPPORT)          
        case FS_DRIVE_TFCARD_SECOND:
          if ( g_hasMountedTFlash2FS )
            {
                Result = DriveMap[3];//add wangrui
            }
          break;
#endif	
/*- zhouqin modify for dual t-flash card 20110503*/
        default:
            return FS_PARAM_ERROR;
    }

    TRACE_EF(g_sw_FMGR, "Result = %d", Result);

    if ( Result > 0 )
    {
        return Result;
    }

    return FS_DRIVE_NOT_FOUND;
}

/*
 * Function:       MMI_FS_GetFileInfo
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:             FilePath, FileInfo
 * Output:          FileInfo
 * Return:         0
 * Others:         
 */
INT MMI_FS_GetFileInfo(S8 * FilePath, FS_FileInfo * FileInfo)
{
#if 0
    MMI_ASSERT(FilePath);
    MMI_ASSERT(FileInfo);

    BOOL ftag = FALSE;
    FS_STAT  f_stat;
    U8 szFileName[MAX_FILE_PATH_LEN*ENCODING_LENGTH] = "\0";
    U8 szUnixFileName[MAX_FILE_PATH_LEN*ENCODING_LENGTH] = "\0";
    FS_DOSDirEntry DirEntry;

    ConvertRelative2Full(szFileName, FilePath);
    ConvertDosName2Unix(szUnixFileName, szFileName);

    memset(&f_stat, 0x00, sizeof(FS_STAT));
    memset(&DirEntry, 0x00, sizeof(DirEntry));

    ftag = FS_Stat (szFileName , &f_stat);


    if ( (ftag == TRUE) && (f_stat.st_mode != INODE_TYPE_DIRECTORY) )
    {
        U8 *pdot = strrchr(f_stat.st_name, '.' );

        if ( pdot != NULL )
        {
            U8 namelen = (pdot - (U8 *)(f_stat.st_name));

            strncpy(DirEntry.FileName, f_stat.st_name, namelen>sizeof(DirEntry.FileName)? 8:sizeof(DirEntry.FileName));
            strncpy(DirEntry.Extension, ++pdot, sizeof(DirEntry.Extension));
            DirEntry.FileSize = f_stat.st_size;
        }
        else
        {
            strcpy(DirEntry.FileName, f_stat.st_name);
            DirEntry.FileSize = f_stat.st_size;
        }
        FileInfo->DirEntry = &DirEntry;
    }
#endif    
    return 0;
}

/*
 * Function:        MMI_FS_GetDevStatus
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          DriveIdx, Flag
 * Output:         nal
 * Return:         FS_PARAM_ERROR or fs_no_error
 * Others:         
 */
INT MMI_FS_GetDevStatus(INT32 DriveIdx, INT32 Flag)
{
    INT32 offset;

#if defined(DUAL_TFLASH_SUPPORT)
    if ( (toupper( DriveIdx) != 'C') && (toupper(DriveIdx) != 'D')&& (toupper(DriveIdx) != 'F') )
#else
    if ( (toupper( DriveIdx) != 'C') && (toupper(DriveIdx) != 'D'))
#endif
    {
        return FS_PARAM_ERROR;
    }

    offset = toupper( DriveIdx) - 'C';
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    if ( offset > FS_TOTAL_STORAGE )
#else
    if ( offset >= FS_TOTAL_STORAGE )
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
    {
        /* drive number exceeds the maximum */

        return FS_PARAM_ERROR;
    }

    /* reterive fssim_device[] */

    if ( g_hasMountedFlashFS )
    {
        DriveMapSatus[2].mount_state = 0;//modify gdm 081224
    }
    else
    {
        DriveMapSatus[2].mount_state = 1;///modify gdm 081224
    }

    if (g_hasMountedTFlashFS)
    {
        DriveMapSatus[1].mount_state = 0;
    }
    else
    {
        DriveMapSatus[1].mount_state = 1;
    }
#if defined(DUAL_TFLASH_SUPPORT)
    if (g_hasMountedTFlash2FS)
    {
        DriveMapSatus[3].mount_state = 0;
    }
    else
    {
        DriveMapSatus[3].mount_state = 1;
    }
#endif    
    /*if (g_hasMountedNandFlashFS)
    {
        DriveMapSatus[2].mount_state = 0;
    }
    else
    {
        DriveMapSatus[2].mount_state = 1;
    }*/


    switch ( Flag )
    {
        case FS_MOUNT_STATE_ENUM:
            if ( DriveMapSatus[offset].mount_state > 0 )
            {
                return FS_FLASH_MOUNT_ERROR;
            }
            break;
        case FS_FEATURE_STATE_ENUM:
            if ( DriveMapSatus[offset].busy_status > 0 )
            {
                return FS_DEVICE_BUSY;
            }
            break;
        default:
            break;
    }
    return FS_NO_ERROR;
}

/*
 * Function:       MMI_FS_GetDiskInfo
 * Description:   For FMGR & FS 
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:             DriveName, DiskInfo,  Flags
 * Output:           nal
 * Return:          fs_error_reserved
 * Others:         
 */
INT MMI_FS_GetDiskInfo(const UINT8 * DriveName, FS_DiskInfo * DiskInfo, INT32 Flags)
{
    INT32 Result = FS_ERROR_RESERVED;
    FS_INFO psFSInfo;
    U8 Drive = 0;
    
    TRACE_FMGR_FUNCTION();
    MMI_ASSERT(DriveName != NULL);
    MMI_ASSERT(DiskInfo != NULL);
    
   if ((NULL == DriveName) || (NULL == DiskInfo) )
   {
        return FS_PARAM_ERROR;
   }
   
    Drive = *DriveName;
    Drive = UPCASE(Drive);
#if defined(DUAL_TFLASH_SUPPORT)    
    if( (Drive!= 'C') && ( Drive != 'D' )&& ( Drive != 'E' )&& ( Drive != 'F' ))
#else        
    if( (Drive!= 'C') && ( Drive != 'D' )&& ( Drive != 'E' ))
#endif        
    {
        return  FS_PARAM_ERROR;
    }

    DiskInfo->WriteProtect = FALSE;

    switch (Drive)
    {
        case 'C':
            Result = FS_GetFSInfo("vds0", &psFSInfo);// dingjian 20080326 modify for new fs
            break;
        case 'D':
            Result = FS_GetFSInfo("mmc0", &psFSInfo);
            break;
        case 'E':
            Result = FS_GetFSInfo("vds1", &psFSInfo);//modify gdm 081224 vds1->E
            break;
#if defined(DUAL_TFLASH_SUPPORT)    //MMC1
        case 'F':
            Result = FS_GetFSInfo("mmc1", &psFSInfo);
            break;
#endif            
        default:
            break;
    }

    if ( ERR_SUCCESS  == Result )
    {
        DiskInfo->FreeClusters = psFSInfo.iTotalSize - psFSInfo.iUsedSize;
        DiskInfo->TotalClusters = psFSInfo.iTotalSize;
        DiskInfo->SectorsPerCluster = 1;
        DiskInfo->BytesPerSector = 1;
        Result = Flags;
		#if 1
		U32 tempUseCluL, tempUseCluH, tempTotCluL, tempTotCluH;
		tempUseCluL = (U32)psFSInfo.iUsedSize;
		tempUseCluH = (U32)(psFSInfo.iUsedSize>>32);
		tempTotCluL = (U32)(psFSInfo.iTotalSize );
		tempTotCluH = (U32)(psFSInfo.iTotalSize>>32);
		TRACE_EF(g_sw_FMGR, "UseCluH=0x%x UseClu=0x%x TotCluH=0x%x TotCluL=0x%x",tempUseCluH, tempUseCluL,  tempTotCluH, tempTotCluL);
		#endif		
    }
    else
    {
		mmi_trace(g_sw_FMR, "call FS_GetFSInfo() return=%d", Result);
		/*if( Result != ERR_FS_NOT_MOUNT)
			MMI_ASSERT(0);*/
		Result = FS_ERROR_RESERVED;
    }

    return Result;

#if 0  //shizh delete 2007-6-24
/*

    if ( TRUE == FS_Query(FS_Q_FREE_SIZE, &FreeBytes) )
    {
        TRACE_EF(g_sw_FMGR, "FreeBytes = %d", FreeBytes);
        DiskInfo->FreeClusters = FreeBytes;
        DiskInfo->SectorsPerCluster = 1;
        DiskInfo->BytesPerSector = 1;
        Result = Flags; 
    }
    else
    {
        Result = -1;
    }

    if ( TRUE == FS_Query(FS_Q_FLASH_SIZE, &TotalBytes) )
    {
        TRACE_EF(g_sw_FMGR, "TotalBytes = %d", TotalBytes);
        DiskInfo->TotalClusters = TotalBytes;
        DiskInfo->SectorsPerCluster = 1;
        DiskInfo->BytesPerSector = 1;
        Result = Flags; 
    }
    else
    {
        Result = -1;
    }   
    return Result;
*/
#endif
}

/*
 * Function:       MMI_FS_TestMSDC
 * Description:  For FMGR & FS  
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          MSDCIndex
 * Output:         nal
 * Return:         FS_MSDC_MOUNT_ERROR
 * Others:         
 */
INT MMI_FS_TestMSDC(INT32 MSDCIndex)
{
/*0: ms card
    1: sd card
    2: mmc card
    3: unkonwcard
 */
    return FS_MSDC_MOUNT_ERROR;
}

/*
 * Function:       MMI_FS_XDelete
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          FullPath, Flag
 * Output:         nal
 * Return:    FS_GENERAL_FAILURE     
 * Others:         
 */
 extern kal_bool fmt_get_abort_copy (void);

INT MMI_FS_XDelete(const UINT8 * pcDirName, INT32 Flag)
{
    INT32 iFindFd = -1;
    UINT16 *p = NULL;
    INT32 iRet = FS_ERROR_RESERVED;
    FS_FIND_DATA sFindData;
    UINT8 strUni[] = { '*', 0, 0, 0};
    U8 *pcPath = NULL;
    U8 *pcDosPath = NULL;

    MMI_ASSERT(pcDirName != NULL);
    Flag = Flag; //not use..
    pcPath = (UINT8*)OslMalloc(MAX_FILE_PATH_UNICODE_LEN + LEN_FOR_UNICODE_NULL_CHAR);
    if( NULL == pcPath )
    {
        return FS_ERROR_RESERVED;
    }
    pcDosPath = (UINT8*)OslMalloc(MAX_FILE_PATH_UNICODE_LEN + LEN_FOR_UNICODE_NULL_CHAR);
    if( NULL == pcDosPath )
    {
        OslMfree(pcPath);
        pcPath  = NULL;
        return FS_ERROR_RESERVED;
     }

    p = (UINT16*)pcPath;
    
    ConvertRelative2Full((U8*)pcDosPath, (U8*)pcDirName);
    ConvertDosName2Unix((U8*)pcPath,(U8*)pcDosPath);

    TRACE_FMGR_FUNC("[MMI_FS_RemoveDir:] pcPath");

    while (*p)
    {
        p++;
    }

    if ((UINT32)(p + pfnUnicodeStrlen(strUni)) > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
    {
        iRet = FS_ERROR_RESERVED;
        goto failed;
    }
    pfnUnicodeStrcpy((S8*)p, (S8*)strUni);

    iFindFd = FS_FindFirstFile(pcPath, &sFindData);
    
    mmi_trace(g_sw_FMGR,"MMI_FS_XDelete,iFindFd is %d",iFindFd);
    
    if (ERR_FS_NO_MORE_FILES == iFindFd)
    {
        iRet = FS_NO_ERROR; // 不可能的情况。
        goto SUCCESS;
    }
    else if (iFindFd < 0)
    {
        iRet = FS_ERROR_RESERVED;
        goto failed;
    }
    
    if ((UINT32)(p + pfnUnicodeStrlen(sFindData.st_name)) > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
    {
        iRet = FS_ERROR_RESERVED;
        goto failed;
    }
    pfnUnicodeStrcpy((S8*)p, (S8*)(sFindData.st_name));

    if (!FS_IS_DIR( sFindData.st_mode ))
    {
        if ( ERR_SUCCESS != FS_Delete(pcPath) )
        {
            TRACE_EF(g_sw_FMGR, "[#error!!]: FS_Delete111 return = %d.\n", iRet);
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }
        COS_Sleep(1);
    }
    else if (FS_IS_DIR( sFindData.st_mode ) 
                 && '.'== sFindData.st_name[0] && 0 == sFindData.st_name[1]) 
    {
        ;
    }
    else
    {
#if 0  //不删除本目录
        iRet = MMI_FS_RemoveDir(pcPath);
        if (FS_NO_ERROR== iRet)
        {
            ;
        }
        else
        {
            TRACE_EF(g_sw_FMGR,  "[#error!!]: FS_RemoveDir111 return = %d.\n", iRet);
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }
#endif
    }

    while ( ERR_SUCCESS == ( iRet = FS_FindNextFile( iFindFd, &sFindData ) ) ) 
    {
        if ((UINT32)(p + pfnUnicodeStrlen(sFindData.st_name)) > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
        {
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }
        pfnUnicodeStrcpy((S8*)p, (S8*)sFindData.st_name);
      
        if (!FS_IS_DIR( sFindData.st_mode ))
        {
            if ( ERR_SUCCESS != FS_Delete(pcPath) )
            {
                TRACE_EF(g_sw_FMGR, "[#error!!]: FS_Delete222 return = %d.\n", iRet);
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            COS_Sleep(1);
			
		if(fmt_get_abort_copy())
		{
		 iRet = FS_NO_ERROR;
		 goto failed;
		}
        }
        else if (FS_IS_DIR( sFindData.st_mode ) 
                     && '.'== sFindData.st_name[0] && 0 == sFindData.st_name[1])
        {
            ;
        }
        else
        {/*
            iRet = MMI_FS_RemoveDir(pcPath);
            if (FS_NO_ERROR == iRet)
            {
                ;
            }
            else
            {
                TRACE_EF(g_sw_FMGR,  "[#error!!]: FS_RemoveDir222 return = %d.\n", iRet);
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }*/
        }
    }

    if (iRet != ERR_FS_NO_MORE_FILES)
    {
        iRet = FS_ERROR_RESERVED;
        goto failed;
    }
    else
    {
        iRet = FS_NO_ERROR;
    }

SUCCESS:
    
failed:

    OslMfree(pcPath);
    OslMfree(pcDosPath);
    if(iFindFd >= 0)
    {
      FS_FindClose(iFindFd);
    }

    TRACE_EF(g_sw_FMGR, "+++++failed !!!+++++.\n", iRet);
    fmt_need_abort = FALSE;

    return iRet;
        
}

/*
 * Function:       MMI_FS_Delete
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:           FileName
 * Output:         nal
 * Return:         FS_NO_ERROR FS_ERROR_RESERVED
 * Others:         
 */
INT MMI_FS_Delete(const UINT8 * FileName)
{
    INT32 iRet = FS_NO_ERROR;
    U8* pFileName = NULL;
    U8* pUnixFileName = NULL;
    U32 iLen = 0;

	U8 tmpName[FS_PATH_LEN];
	if ( (pfnUnicodeStrlen(FileName)) <  FS_FILE_NAME_LEN )
	{
	    UnicodeToAnsii((S8 *)tmpName, (S8 *)FileName);
	    mmi_trace(g_sw_FMGR,"%s\n", tmpName);
	}
    mmi_trace(g_sw_FMGR, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
    
    TBM_ENTRY(0x2836);
    MMI_ASSERT(FileName != NULL);

    if(NULL == FileName)
    {
        iRet = FS_PARAM_ERROR;
        goto _func_end;
    }
    iLen = pfnUnicodeStrlen(FileName)*2 + pfnUnicodeStrlen(g_vCurDir)*2 + LEN_FOR_UNICODE_NULL_CHAR;
    pFileName = OslMalloc(iLen + CVTFUN_CATCHAR_LEN);  
    if(NULL == pFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    pUnixFileName = OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);
    if(NULL == pUnixFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    
    ConvertRelative2Full((U8*)pFileName,(U8*)FileName);
    ConvertDosName2Unix(pUnixFileName, pFileName);

    if( !is_file_name_valid((UINT16 *) pUnixFileName, FALSE ))
    {  
        iRet = FS_INVALID_FILENAME;
        goto _func_end; 
    }

    iRet = FS_Delete(pUnixFileName); // return ERR_SUCCESS

    if ( iRet != ERR_SUCCESS )
    {
        if ( ERR_FS_PATHNAME_PARSE_FAILED == iRet )
        {
            TRACE_EF(g_sw_FMGR, "[MMI_FS_Delete ]DeleteError! ErrorCode = %d", iRet);
            iRet = FS_INVALID_FILENAME;
            goto _func_end; 
        }
	 else if (ERR_FS_OPERATION_NOT_GRANTED == iRet)
	 {
            TRACE_EF(g_sw_FMGR, "[MMI_FS_Delete ]DeleteError! ErrorCode = %d", iRet);
            iRet = FS_ACCESS_DENIED;
            goto _func_end; 
	 }
        else
        {
            TRACE_EF(g_sw_FMGR, "[MMI_FS_Delete ]DeleteError! ErrorCode = %d", iRet);

            iRet = FS_ERROR_RESERVED;
            goto _func_end; 
        }
        TRACE_EF(g_sw_FMGR, "[MMI_FS_Delete ]DeleteError! ErrorCode = %d", iRet);
    }
    iRet = FS_NO_ERROR;
_func_end:
    if(NULL != pFileName)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }
    if(NULL != pUnixFileName)
    {
        OslMfree(pUnixFileName);
        pUnixFileName = NULL;
    }
    TBM_EXIT(0x2836);
    return iRet;
}

/*
 * Function:       MMI_FS_FindFirst
 * Description:  For FMGR & FS  
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          NamePattern Attr AttrMask FileInfo FileName MaxLength
 * Output:         FileInfo FileName
 * Return:         FS_ERROR_RESERVED  fhd
 * Others:         
 */
INT MMI_FS_FindFirst(const UINT8 * NamePattern, BYTE Attr, BYTE AttrMask, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength)
{
    UINT8 pointPoint[6] = {'.', 0, '.', 0, 0, 0};
    UINT8 point[4] = { '.', 0, 0, 0};
    FS_HANDLE fhd = 0;
    FS_FIND_DATA* psFindData = NULL;
    INT32 iRet = FS_NO_ERROR;
    U8* pFileName     = NULL;
    U8* pUnixFileName = NULL;
    U32 iLen = 0;

    
    TBM_ENTRY(0x2837);
    TRACE_FMGR_FUNCTION();
    MMI_ASSERT( NamePattern != NULL);
    MMI_ASSERT(FileInfo != NULL);
    
    if(NULL == FileName)
    {
        TBM_EXIT(0x2837);
        return FS_PARAM_ERROR;
    }
    
    iLen = pfnUnicodeStrlen(NamePattern)*2 + pfnUnicodeStrlen(g_vCurDir)*2 + LEN_FOR_UNICODE_NULL_CHAR;
    pFileName = OslMalloc(iLen + CVTFUN_CATCHAR_LEN);  
    if(NULL == pFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    
    pUnixFileName = OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);
    if(NULL == pUnixFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    
    psFindData = OslMalloc(sizeof(FS_FIND_DATA));
    if(NULL == psFindData)
    {
        iRet = FS_FAIL_GET_MEM;
        goto _func_end;
    }
    
    ConvertRelative2Full((U8*)pFileName,(U8*)NamePattern);
    ConvertDosName2Unix(pUnixFileName, pFileName);
    
    memset(psFindData, 0x00, sizeof(FS_FIND_DATA));
    fhd = FS_FindFirstFile(pUnixFileName, psFindData);
    mmi_trace(g_sw_FMGR,"MMI_FS_FindFirst,fhd is %d",fhd);
    if ( fhd >= 0 )
    {
        if( ( pfnUnicodeStrcmp(psFindData->st_name, pointPoint) != 0 ) &&
             ( pfnUnicodeStrcmp(psFindData->st_name, point) != 0 ) )

        {
            if ( psFindData->st_mode & FS_ATTR_DIR )
            {
                FileInfo->Attributes = FS_ATTR_DIR;
            }
            else if (! (psFindData->st_mode & FS_ATTR_DIR ))
            {
                FileInfo->Attributes = 0;   
                FileInfo->FileSize = psFindData->st_size;  
            }
            else
            {
                FileInfo->Attributes = 0x00;            
            }
            Convert2DosDateTime(&(FileInfo->DateTime), psFindData->st_ctime);
            pfnUnicodeStrcpy(FileName, psFindData->st_name);
        }
	else
	{
		FileInfo->Attributes = FS_ATTR_DIR | FS_ATTR_SYSTEM;
		pfnUnicodeStrcpy(FileName, psFindData->st_name);
	}
        iRet = fhd;
        goto _func_end;
    }
    else
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    
_func_end:
    
    if(NULL != pFileName)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }
    if(NULL != pUnixFileName)
    {
        OslMfree(pUnixFileName);
        pUnixFileName = NULL;
    }
    if(NULL != psFindData)
    {
        OslMfree(psFindData);
        psFindData = NULL;
    }
    
    TBM_EXIT(0x2837);
    return iRet;
}

/*
 * Function:       MMI_FS_FindNext
 * Description:   For FMGR & FS 
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:            FileHandle,   * FileInfo,   * FileName,   MaxLength
 * Output:         FileName
 * Return:         FS_NO_ERROR  or FS_ERROR_RESERVED
 * Others:         
 */
INT MMI_FS_FindNext(FS_HANDLE FileHandle, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength)
{
    INT32 fhd = -1;
    INT32 iRet = 0;
    FS_FIND_DATA  *psFindData = NULL;
    UINT8 pointPoint[6] = {'.', 0, '.', 0, 0, 0};
    UINT8 point[4] = { '.', 0, 0, 0};
    mmi_trace(g_sw_FMGR,"MMI_FS_FindNext");
    TBM_ENTRY(0x2838);
   // TRACE_FMGR_FUNCTION();

    MMI_ASSERT( FileHandle >= 0);
    MMI_ASSERT(FileInfo != NULL);
    MMI_ASSERT(FileName != NULL);

    psFindData = OslMalloc(sizeof(FS_FIND_DATA));
    if (NULL == psFindData)
    {
        iRet = FS_ERROR_RESERVED;
        goto _fun_end;
    }
    
    memset(psFindData, 0x00, sizeof(FS_FIND_DATA));

     fhd = FS_FindNextFile(FileHandle, psFindData);
	//fhd = MCI_FindNextFile(psFindData);

    TRACE_EF(g_sw_FMGR, "=========>findNextFile ret = %d", fhd);

    if ( fhd == ERR_SUCCESS )
    {
        if ( ( pfnUnicodeStrcmp(psFindData->st_name, pointPoint) != 0 )&&
            ( pfnUnicodeStrcmp(psFindData->st_name, point) != 0 ) )
        {
            if ( psFindData->st_mode & FS_ATTR_DIR )
            {
                FileInfo->Attributes = FS_ATTR_DIR;
            }
            else if ( !(psFindData->st_mode & FS_ATTR_DIR ))
            {
                FileInfo->Attributes = 0;   
                FileInfo->FileSize = psFindData->st_size;      
            }
            else
            {
                FileInfo->Attributes=  0x00;            
            }                   
            Convert2DosDateTime(&(FileInfo->DateTime), psFindData->st_ctime);
            pfnUnicodeStrcpy(FileName, psFindData->st_name);
        }
	else
	{
		 FileInfo->Attributes = FS_ATTR_DIR | FS_ATTR_SYSTEM;
		 pfnUnicodeStrcpy(FileName, psFindData->st_name);
	}
        iRet = FS_NO_ERROR;
    }
    else
    {
        iRet = FS_ERROR_RESERVED;
    }
    _fun_end:
        
        if (psFindData != NULL )
        {
            OslMfree(psFindData);
            psFindData = NULL;
        }
        
        TBM_EXIT(0x2838);
        return iRet;

}


/*
 * Function:       MMI_FS_GetDirSize
 * Description:   For FMGR & FS 
 * Created By:    chenhe
 * Created Date: 2009年10月12日
 * Input:            dirName
 * Output:         NIL
 * Return:         total size of all files under this directory
 * Others:         
 */
UINT32 MMI_FS_GetDirSize( CONST UINT8 * dirName)
{
	INT32 fhd;
	FS_FIND_DATA fsInfo;
	UINT8 fullName[FS_FILE_NAME_UNICODE_LEN],unixName[FS_FILE_NAME_UNICODE_LEN];
	UINT32 totalsize=0;
	UINT8 pointPoint[6] = {'.', 0, '.', 0, 0, 0};
	UINT8 point[4] = { '.', 0, 0, 0};
	UINT32 sizeMode = 0;//one file will occupy at least 512 bytes

	ConvertRelative2Full((U8*)fullName,(U8*)dirName);
	ConvertDosName2Unix(unixName, fullName);
	
	fhd=FS_FindFirstFile(unixName,&fsInfo);
	if(fhd>=0)
	{
		if( ( pfnUnicodeStrcmp(fsInfo.st_name, pointPoint) != 0 )&&( pfnUnicodeStrcmp(fsInfo.st_name, point) != 0 ) )
		{
			sizeMode= fsInfo.st_size%512;
			fsInfo.st_size = (sizeMode == 0)?fsInfo.st_size:(fsInfo.st_size+512-sizeMode);
			totalsize = fsInfo.st_size;
		}
		mmi_trace(g_sw_FMGR,"chenhe,MMI_FS_GetDirSize,000,totalsize is %d",totalsize);

		while(FS_FindNextFile(fhd, &fsInfo)==ERR_SUCCESS)
		{
			mmi_trace(g_sw_FMGR,"chenhe,MMI_FS_GetDirSize,111,fsInfo.st_size is %d",fsInfo.st_size);
			if( ( pfnUnicodeStrcmp(fsInfo.st_name, pointPoint) != 0 )&&( pfnUnicodeStrcmp(fsInfo.st_name, point) != 0 ) )
			{
				sizeMode= fsInfo.st_size%512;
				fsInfo.st_size = (sizeMode == 0)?fsInfo.st_size:(fsInfo.st_size+512-sizeMode);
				totalsize += fsInfo.st_size;
			}
		}
		FS_FindClose(fhd);
	}
	mmi_trace(g_sw_FMGR,"chenhe,MMI_FS_GetDirSize,totalsize is %d",totalsize);
	return totalsize;	
}


/*
 * Function:       MMI_FS_FindClose
 * Description:   For FMGR & FS 
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          FileHandle
 * Output:         nal
 * Return:         FS_NO_ERROR  or FS_ERROR_RESERVED
 * Others:         
 */
INT MMI_FS_FindClose(FS_HANDLE FileHandle)
{
    TBM_ENTRY(0x2839);
    MMI_ASSERT(FileHandle >= 0);

    TRACE_EF(g_sw_FMGR, "MMI_FS_FindClose(): FileHandle = %d", FileHandle);

    if ( ERR_SUCCESS == (FS_FindClose(FileHandle)) )
    {
        TBM_EXIT(0x2839);
        return FS_NO_ERROR;
    }
    TBM_EXIT(0x2839);
    return FS_ERROR_RESERVED;
}

/*
 * Function:       MMI_FS_SetCurrentDir
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日 
 * Input:          DirName
 * Output:         nal
 * Return:          FS_NO_ERROR  or FS_ERROR_RESERVED
 * Others:         
 */
INT MMI_FS_SetCurrentDir(const UINT8 * DirName)
{
    MMI_ASSERT(DirName != NULL);
    pfnUnicodeStrcpy(g_vCurDir, DirName);
    return 0;


/*   2007-6-12 shizh delete 
    UnicodeToAnsii(szDirName, DirName);
    if ( szDirName[strlen(szDirName) - 1] != '/' )
        strcat(szDirName, "/");

    TRACE_EF(g_sw_FMGR, "MMI_FS_SetCurrentDir(): DirName is %s", szDirName);

    szDirName[0] = UPCASE(szDirName[0]);
    ptr = strstr(szDirName, "C:");
    if ( ptr != NULL )
        ptr = ptr + 2;
    else
        ptr = szDirName;

    TRACE_EF(g_sw_FMGR, "MMI_FS_SetCurrentDir(): DirName change into %s",ptr);

    if ( TRUE == FS_ChangeDir(ptr) )
    {
        return FS_NO_ERROR;
    }
    else
    {
        TRACE_EF(g_sw_FMGR, "FS_ChangeDir: SUL_GetLastError() is %d", SUL_GetLastError());
        return FS_ERROR_RESERVED;
    }
*/
}

#if 0
/*
 * Function:       MMI_FS_RemoveDir
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          FullPath
 * Output:         nal
 * Return:         FS_PARAM_ERROR or FS_NO_ERROR
 * Others:         
 */
INT MMI_FS_RemoveDir(const UINT8 * FullPath)
{
    TBM_ENTRY(0x283A);
    TRACE_FMGR_FUNCTION();

    UINT8 *szFileName = NULL;
    UINT8 *szUnixFileName = NULL;

    UINT8 szTempStr1[10] = {0};
    UINT8 szTempStr2[10] = {0};

    FS_HANDLE fh = NULL;
    FS_FIND_DATA fd;
    UINT filecnt = 0;
    UINT dircnt = 0;
    UINT len = 0;
    UINT lenPath = 0, lenCurDir = 0;
    INT Ret = 0, retError = 0;

    lenPath = pfnUnicodeStrlen(FullPath) *2;
    lenCurDir = pfnUnicodeStrlen(g_vCurDir)*2;

    szFileName = (UINT8*)OslMalloc( lenPath + lenCurDir + 30);
    szUnixFileName = (UINT8*)OslMalloc( lenPath + lenCurDir + 30);

    if ( (szFileName == NULL) && (szUnixFileName == NULL) )
    {
        TRACE_FMGR_FUNC("OslMalloc Error!");
        //COS_Sleep(100);
        return FS_ERROR_RESERVED;
    }

    memset(&fd, 0x00, sizeof(FS_FIND_DATA));

    ConvertRelative2Full(szFileName, FullPath);
    ConvertDosName2Unix(szUnixFileName, szFileName);

    memset(szFileName, 0, sizeof(szFileName));
    UCS2Strcpy(szFileName, szUnixFileName);

    AnsiiToUnicodeString(szTempStr1, "/*");
    AnsiiToUnicodeString(szTempStr2, "*");

    len = pathlen(szFileName);

    TRACE_FMGR_FUNCTION();
     //COS_Sleep(100);

    if ( ( *(szFileName + len - 2) != '/' ) && ( *(szFileName + len) == 0   ) &&  ( *(szFileName + len + 1) == 0   ) )//倒数(fpath+len-1)第三个字节不一定为零
    {
        UCS2Strcat(szFileName, szTempStr1);
    }
    else if ( ( *(szFileName + len - 2) == '/' ) &&  ( *(szFileName + len - 1) == 0 ) && ( *(szFileName + len) == 0 ) &&  ( *(szFileName + len + 1) == 0 ) )
    {
        UCS2Strcat(szFileName, szTempStr2);
    }
    else
    {
        retError = FS_PARAM_ERROR;
        goto end;
    }

    memset(szTempStr1, 0, sizeof(szTempStr1));
    memset(szTempStr2, 0, sizeof(szTempStr2));
    AnsiiToUnicodeString(szTempStr1, ".");
    AnsiiToUnicodeString(szTempStr2, "..");

    fh = FS_FindFirstFile(szFileName, &fd);

    len =  pathlen(szUnixFileName);

    TRACE_FMGR_FUNCTION();
    //COS_Sleep(100);

    while ( fh >= 0 )
    {
        if ( (fd.st_mode & INODE_TYPE_DIRECTORY) && ( UCS2Strcmp(fd.st_name, szTempStr1) != 0 ) &&
        ( UCS2Strcmp(fd.st_name, szTempStr2) != 0 ) )
        {
            memset(szFileName, 0x00, sizeof(szFileName));

            if ( ( *(szUnixFileName + len - 2) == '/' ) && ( *(szUnixFileName + len - 1) == 0   ) &&
            ( *(szUnixFileName + len) == 0   ) && ( *(szUnixFileName + len + 1) == 0   ) )
            {
                UCS2Strcpy(szFileName, szUnixFileName);
                UCS2Strcat(szFileName, fd.st_name);
            }
            else
            {
                UCS2Strcpy(szFileName, szUnixFileName);
                memset(szTempStr2, 0, sizeof(szTempStr2));
                AnsiiToUnicodeString(szTempStr2, "/");
                UCS2Strcat(szFileName, szTempStr2);
                UCS2Strcat(szFileName, fd.st_name);
            }

            Ret = FS_RemoveDir(szFileName);
            TRACE_EF(g_sw_FMGR,"[MMI_FS_RemoveDir:] FS_RemoveDir Ret = %d", Ret);
            //COS_Sleep(100);

    
            if ( ERR_SUCCESS != Ret )
            {
                if ( ERR_FS_DIR_NOT_EMPTY == Ret )
                {
                    Ret = MMI_FS_RemoveDir(szFileName);
                }
                else
                {
                    retError = FS_ERROR_RESERVED;
                }
            }
            retError = FS_NO_ERROR;

        }
        else if ( fd.st_mode & INODE_TYPE_REGULAR )
        {
            memset(szFileName, 0x00, sizeof(szFileName));

            if ( ( *(szUnixFileName + len - 2) == '/' ) && ( *(szUnixFileName + len - 1) == 0   ) &&
            ( *(szUnixFileName + len) == 0   ) && ( *(szUnixFileName + len + 1) == 0   ) )
            {
                UCS2Strcpy(szFileName, szUnixFileName);
                UCS2Strcat(szFileName, fd.st_name);
            }
            else
            {
                UCS2Strcpy(szFileName, szUnixFileName);
                memset(szTempStr2, 0, sizeof(szTempStr2));
                AnsiiToUnicodeString(szTempStr2, "/");
                UCS2Strcat(szFileName, szTempStr2);
                UCS2Strcat(szFileName, fd.st_name);
            }

            TRACE_FMGR_FUNCTION();

            if ( ERR_SUCCESS != FS_Delete(szFileName) )
            {
                retError = FS_ERROR_RESERVED;
                goto end;

            }

        }
        else if ( fd.st_mode & INODE_TYPE_UNKNOWN )
        {
            MMI_ASSERT(0);
        }

        if ( ERR_SUCCESS != FS_FindNextFile(fh, &fd) )
        {
            FS_FindClose(fh);
            break;
        }
    }

    //del folder itself 

    if ( Ret == FS_NO_ERROR )
    {
        if ( ( *(szUnixFileName + len - 2) == '/' ) && ( *(szUnixFileName + len - 1) == 0   ) &&
        ( *(szUnixFileName + len) == 0   ) && ( *(szUnixFileName + len + 1) == 0   ) )
        {
            szUnixFileName[len - 2] = 0;
            szUnixFileName[len - 1] = 0;
        }

        Ret = FS_RemoveDir(szUnixFileName);

        TRACE_EF(g_sw_FMGR, "[MMI_FS_RemoveDir:] FS_RemoveDir ret = %d", Ret);

        if ( ERR_SUCCESS !=  Ret )
        {
            // /home, /tflash目录不能删除
            if ( ERR_FS_OPERATION_NOT_GRANTED == Ret )
            {
                TRACE_EF(g_sw_FMGR, "[MMI_FS_RemoveDir:] FS_RemoveDir ret = ERR_FS_OPERATION_NOT_GRANTED");

                retError = FS_NO_ERROR;
                goto end;

            }
            TRACE_EF(g_sw_FMGR, "[MMI_FS_RemoveDir:] FS_RemoveDir ret = %d", Ret);

            retError = FS_ERROR_RESERVED;
            goto end;

        }

        retError = FS_NO_ERROR;
        goto end;

    }
    retError = FS_ERROR_RESERVED;

    end:

    if ( szFileName != NULL )
    {
        OslMfree(szFileName);
        szFileName = NULL;
    }

    if ( szUnixFileName != NULL )
    {
        OslMfree(szUnixFileName);
        szUnixFileName = NULL;
    }

    TBM_EXIT(0x283A);
    return retError;
}

#endif

extern void sxr_Sleep (u32 Period);

INT MMI_FS_RemoveDir(const UINT8* pcDirName)
{
    INT32 i, iDepth = 0;
    INT32 iFindFd[NR_MAX_DEPTH];
    UINT16 *pOriginal, *p = NULL;
    INT32 iRet = FS_ERROR_RESERVED;
    UINT8 c, c2;
    UINT16 UniChar;
    UINT32 iNameLen;
    FS_FIND_DATA sFindData;
    UINT8 strUni[] = {'/', 0, '*', 0, 0, 0};
    U8 *pcPath = NULL;
    U8 *pcDosPath = NULL;
    INT removedSize=0,tmpfileSize=0;
    MMI_ASSERT(pcDirName != NULL);
    
    pcPath = (UINT8*)OslMalloc(MAX_FILE_PATH_UNICODE_LEN + LEN_FOR_UNICODE_NULL_CHAR);
    if( NULL == pcPath )
    {
        return FS_ERROR_RESERVED;
    }
    pcDosPath = (UINT8*)OslMalloc(MAX_FILE_PATH_UNICODE_LEN + LEN_FOR_UNICODE_NULL_CHAR);
    if( NULL == pcDosPath )
    {
        OslMfree(pcPath);
        pcPath  = NULL;
        return FS_ERROR_RESERVED;
     }
  
    ConvertRelative2Full((U8*)pcDosPath,(U8*)pcDirName);
    ConvertDosName2Unix(pcPath, pcDosPath);
    
    // can't remove root-dir
    if (('/' == pcPath[0] || '\\' == pcPath[0]) 
        && (0 == pcPath[1] && 0 == pcPath[2] && 0 == pcPath[3]))
    {
        iRet = FS_PARAM_ERROR;
        goto failed;
    }
    p = (UINT16*)pcPath;

    TRACE_FMGR_FUNC("[MMI_FS_RemoveDir:] pcPath");

    while (*p)
    {
        p++;
    }
    while (TRUE)
    {// 去掉字符串后面多余的'/'。
        UniChar = *(p - 1);
        c = *((UINT8*)(&UniChar));
        c2 = *((UINT8*)(&UniChar) + 1);
        if (('/' == c || '\\' == c) && c2 == 0)
        {
            p--;
        }
        else
        {
            break;
        }
    }
    *p = (UINT16)0;
    pOriginal = p;

    for (i = 0; i < NR_MAX_DEPTH; ++i)
    {
        iFindFd[i] = INVALID_FD;
    }

    while (TRUE)
    {
        if (INVALID_FD == iFindFd[iDepth])
        {
            if ((UINT32)(p + pfnUnicodeStrlen(strUni)) > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            pfnUnicodeStrcpy((S8*)p, (S8*)strUni);
            p++;

            iFindFd[iDepth] = FS_FindFirstFile(pcPath, &sFindData);
            if (ERR_FS_NO_MORE_FILES == iFindFd[iDepth])
            {
                ; // 不可能的情况。
            }
            else if (iFindFd[iDepth] < 0)
            {
                iDepth--;
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            
            if ((UINT32)(p + pfnUnicodeStrlen(sFindData.st_name)) > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            pfnUnicodeStrcpy((S8*)p,(S8*)sFindData.st_name);
               
            if (! FS_IS_DIR( sFindData.st_mode ))
            {
    		tmpfileSize=MMI_FS_GetFileSizeByName(pcPath);
                if ( ERR_SUCCESS != FS_Delete(pcPath) )
                {
                    TRACE_EF(g_sw_FMGR, "[#error!!]: FS_Delete111 return = %d.\n", iRet);
                    iRet = FS_ERROR_RESERVED;
                    goto failed;
                }
		removedSize+=tmpfileSize;
                //COS_Sleep(1);
            }
            else if (FS_IS_DIR( sFindData.st_mode ) 
                         && '.'== sFindData.st_name[0] && 0 == sFindData.st_name[1]) 
            {
                ;
            }
            else
            {                
                iRet = FS_RemoveDir(pcPath);
                if (ERR_SUCCESS == iRet)
                {
                    ;
                }
                else if (iRet != ERR_FS_DIR_NOT_EMPTY)
                {
                    TRACE_EF(g_sw_FMGR,  "[#error!!]: FS_RemoveDir111 return = %d.\n", iRet);
                    iRet = FS_ERROR_RESERVED;
                    goto failed;
                }
                else
                {
                    goto dir_not_empty;
                }
            }
        }

        while ( ERR_SUCCESS == ( iRet = FS_FindNextFile( iFindFd[iDepth], &sFindData ) ) ) 
        {
            if ((UINT32)(p + pfnUnicodeStrlen(sFindData.st_name)) > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            pfnUnicodeStrcpy((S8*)p, (S8*)sFindData.st_name);
          
            if (!FS_IS_DIR( sFindData.st_mode ))
            {
    		tmpfileSize=MMI_FS_GetFileSizeByName(pcPath);
                if ( ERR_SUCCESS != FS_Delete(pcPath) )
                {
                    TRACE_EF(g_sw_FMGR, "[#error!!]: FS_Delete222 return = %d.\n", iRet);
                    iRet = FS_ERROR_RESERVED;
                    goto failed;
                }
		removedSize+=tmpfileSize;
                sxr_Sleep(1);
            }
            else if (FS_IS_DIR( sFindData.st_mode ) 
                         && '.'== sFindData.st_name[0] && 0 == sFindData.st_name[1])
            {
                ;
            }
            else
            {
                iRet = FS_RemoveDir(pcPath);
                if (ERR_SUCCESS == iRet)
                {
                    ;
                }
                else if (iRet != ERR_FS_DIR_NOT_EMPTY)
                {
                    TRACE_EF(g_sw_FMGR,  "[#error!!]: FS_RemoveDir222 return = %d.\n", iRet);
                    iRet = FS_ERROR_RESERVED;
                    goto failed;
                }
                else
                {
                    goto dir_not_empty;
                }
            }
        }

        if (iRet != ERR_FS_NO_MORE_FILES)
        {
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }

        // 当前目录已空，将当前目录删除。
        iRet = FS_FindClose(iFindFd[iDepth]);
        iFindFd[iDepth] = INVALID_FD;
        iDepth--;
        p--;
        *p = (UINT16)0;
        
        if ((iRet = FS_RemoveDir(pcPath)) != ERR_SUCCESS)
        {
            TRACE_EF(g_sw_FMGR,  "[#error!!]: FS_RemoveDir333 return = %d.\n", iRet);
            //COS_Sleep(100);
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }
        if (p == pOriginal)
        {
            if (iDepth != -1)
            {
                TRACE_EF(g_sw_FMGR,  "[#error!!]: p == pOriginal && iDepth != 0.\n");
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            break;
        }

        while (TRUE)
        {// 返回上级目录继续搜索。
            UniChar = *(p - 1);
            c = *((UINT8*)(&UniChar));
            c2 = *((UINT8*)(&UniChar) + 1);
            if (!(('/' == c || '\\' == c) && c2 == 0))
            {
                p--;
            }
            else
            {
                break;
            }
        }
                    
        *p = (UINT16)0;
        continue;
        
dir_not_empty:    
        if ((NR_MAX_DEPTH - 1) == iDepth)
        {// 如果栈满则退出。
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }      
        iNameLen = pfnUnicodeStrlen(sFindData.st_name);
        p += iNameLen;
        iDepth++;
    }
    
    OslMfree(pcPath);
    OslMfree(pcDosPath);
    return FS_NO_ERROR;

failed:

    OslMfree(pcPath);
    OslMfree(pcDosPath);

    TRACE_EF(g_sw_FMGR, "+++++failed !!!+++++.\n", iRet);
    for (i = iDepth; i >= 0; --i)
    {
        TRACE_EF(g_sw_FMGR, "+++++FS_FindClose: iFindFd[%d] = %d +++++.\n",i, iFindFd[i]);
        FS_FindClose(iFindFd[i]);
    }
	if(iRet<0)
	{
		return iRet;
	}
	else
	{
		return removedSize;
	}
        
}

/*
 * Function:       MMI_FS_CloseMSDC
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日    
 * Input:          MSDCIndex,  Mode 
 * Output:         nal
 * Return:         FS_NO_ERROR
 * Others:         
 */
INT MMI_FS_CloseMSDC(INT32 MSDCIndex, INT32 Mode)
{
    return FS_NO_ERROR;
}

/*
 * Function:       MMI_FS_Move
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日      
 * Input:              * SrcFullPath,     * DstFullPath,   Flag,   Progress 
 *Output:         nal
 * Return:         FS_PARAM_ERROR or FS_NO_ERROR  
 * Others:         
 */

INT MMI_FS_Move(const UINT8 * SrcFullPath, const UINT8 * DstFullPath, INT32 Flag, 
                                  FS_ProgressCallback Progress,
                                  INT32 IfExist)
{
    INT32 iFdSrc = -1;
    INT32 iFdDst = -1;
    INT32 iLenReaded, iLenWriten, iTotalWriten = 0;
    UINT8 *pBuffer = NULL;
    INT32 iCpyRet = FS_ERROR_RESERVED;
    INT64  size = 0;
    
    FS_DiskInfo diskInfo;
    
    TBM_ENTRY(0x283B);
    TRACE_FMGR_FUNC("MMI_FS_Move:enter");
    MMI_ASSERT(SrcFullPath != NULL);
    MMI_ASSERT(DstFullPath != NULL);
    
    memset(&diskInfo, 0, sizeof(FS_DiskInfo));

    if( 0 != IfExist)
    {
      iCpyRet = MMI_FS_Open(DstFullPath, FS_READ_ONLY);
      if (iCpyRet >= 0)
      {
        MMI_FS_Close(iCpyRet);
        TBM_EXIT(0x283B);       
        return FS_FILE_EXISTS;
      }
    }
    /*
    if( FS_MOVE_KILL == Flag )
    {
        iCpyRet = MMI_FS_Rename(SrcFullPath, DstFullPath);
        if (FS_NO_ERROR == iCpyRet)
        {   
            TRACE_FMGR_FUNC("[MMI_FS_Move ]:  Same Device,  Rename file ok .");
            TBM_EXIT(0x283B);
             return  FS_NO_ERROR;
        }
    }
    */
    iCpyRet = MMI_FS_GetDiskInfo(DstFullPath, &diskInfo, FS_DI_FREE_SPACE);

    if (iCpyRet < 0)
    {
        TBM_EXIT(0x283B);
        return FS_ERROR_RESERVED;
    }
    
		mmi_trace(g_sw_FMGR,"gdm:MMI_FS_Move, free=0x%x", (U32)diskInfo.FreeClusters);
    size = diskInfo.FreeClusters -MMI_FS_GetFileSizeByName(SrcFullPath);
    if (size < FS_FREE_SPACE_SIZE)
    {
        TRACE_FMGR_FUNC("[#trace ]: MMI_FS_Move no enough space.");
        TBM_EXIT(0x283B);
        return FS_DISK_FULL;
    }

    TRACE_FMGR_FUNC("[#trace ]: MMI_FS_Move output SrcFullPath, DstFullPath");

    iFdSrc = MMI_FS_Open(SrcFullPath, FS_READ_ONLY);
    if ( iFdSrc < 0 )
    {
        TRACE_EF(g_sw_FMGR,"[#error!!!]: open SourFile Failed\n");
        iCpyRet = iFdSrc;
        goto failed;
    }

    TRACE_EF(g_sw_FMGR,"open SourFile Succeed\n");
    iFdDst = MMI_FS_Open(DstFullPath, FS_CREATE_ALWAYS);
    if ( iFdDst <0 )
    {
        TRACE_EF(g_sw_FMGR,"[#error!!!]: open DestFile error\n,iFdDst =%d.\n", iFdDst);
        if (ERR_FS_DISK_FULL == iCpyRet)
        {
            iCpyRet = FS_DISK_FULL;
        }
        else
        {
            iCpyRet = FS_ERROR_RESERVED;
        }
        goto failed;
    }
    pBuffer = (UINT8*)OslMalloc(SZ_MMI__FILE_COPY_BUF);
    if ( !pBuffer )
    {
        iCpyRet = FS_ERROR_RESERVED;
        TRACE_EF(g_sw_FMGR,"[#error!!!]: OslMalloc error\n");
        goto failed;
    }

    //iTime1 = TM_GetTime();

    while ( !FS_IsEndOfFile(iFdSrc) )
    {
        if ( fmt_need_abort )
        {
            iCpyRet = FS_NO_ERROR;            
            goto failed;
        }
        iLenReaded = FS_Read(iFdSrc, pBuffer, SZ_MMI__FILE_COPY_BUF);
        if ( iLenReaded < 0 )
        {
            TRACE_EF(g_sw_FMGR,"[#error!!!]: read error\n");
            iCpyRet = FS_ERROR_RESERVED;
            goto failed;
        }
        iLenWriten = FS_Write(iFdDst, pBuffer, iLenReaded);
        if ( iLenWriten != iLenReaded )
        {
            TRACE_EF(g_sw_FMGR,"[#error!!!]: write error\n");
            if (iLenWriten == ERR_FS_DISK_FULL)
            {
                TRACE_EF(g_sw_FMGR,"[#error!!!]: write error\n");
                //COS_Sleep(100);
                iCpyRet = FS_DISK_FULL;
                goto failed;
            }
            TRACE_EF(g_sw_FMGR,"[#error!!!]: ERR_FS_DISK_FULL\n");
            iCpyRet = FS_ERROR_RESERVED;
            goto failed;
        }


        iTotalWriten += iLenWriten;

        TRACE_EF(g_sw_FMGR, "Copying file, size copied is %d.\n", iTotalWriten);

#if 0
        iTime2 = TM_GetTime();
        if ( (iTime2 - iTime1) > MMI_BG_REPORT_PERIOD )
        {
            ev.nParam1 = (iTotalWriten/fileAttr.i_size)*100;  
            ev.nEventId = MMI_FILE_COPY_PROCESSING;
            COS_SendEvent(g_hMmiBgTask, &ev, COS_WAIT_FOREVER, COS_EVENT_PRI_NORMAL) ;
            iTime1 = iTime2;
        }
#endif
        COS_Sleep(1);
    }
    iCpyRet = FS_NO_ERROR;
    TRACE_EF(g_sw_FMGR,"Copy file complete, size copied is %d.\n", iTotalWriten);

    failed:

    if ( iFdSrc >= 0 )
    {
        MMI_FS_Close(iFdSrc);
    }
    if ( iFdDst >= 0 )
    {
        MMI_FS_Close(iFdDst);
    }
    if ( pBuffer )
    {
        OslMfree(pBuffer);
    }

    if ( (FS_NO_ERROR == iCpyRet) && (FS_MOVE_KILL == Flag)  && !fmt_need_abort) //FMT_MOVE
    {
        if ( MMI_FS_Delete(SrcFullPath) != 0 )
        {
            iCpyRet = FS_ERROR_RESERVED;
        }
    }

    if( fmt_need_abort )
    {
        fmt_need_abort = FALSE;
    }
    TBM_EXIT(0x283B);
    return iCpyRet;

}

/*
 * Function:       MMI_FS_Abort
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          ActionHandle
 * Output:       nal  
 * Return:   FS_NO_ERROR      
 * Others:         
 */
INT MMI_FS_Abort(INT32 ActionHandle)
{
    return FS_NO_ERROR;
}

/*
 * Function:     MMI_FS_GeneralFormat  
 * Description:  For FMGR & FS  
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:            * DriveName,  Level,  Progress
 *Output:         nal
 * Return:         FS_PARAM_ERROR or FS_NO_ERROR  
 * Others:         
 */
 extern INT32 DSM_UserDataClear(VOID);

INT MMI_FS_GeneralFormat(const UINT8 * DriveName, INT32 Level, FS_FormatCallback Progress)
{
    UINT8 flag = 0;
    INT32 iRet = 0;
    
    MMI_ASSERT(DriveName != NULL);

    if ( FS_FORMAT_HIGH == Level )
    {
        flag = FS_FORMAT_QUICK;
    }
    else
    {
        flag = FS_FORMAT_NORMAL;
    }

    TRACE_EF(g_sw_FMGR, "MMI_FS_GeneralFormat DriveName = %c", *DriveName);

    switch ( *DriveName )
    {
	case 'a':
	case 'A':
		iRet =DSM_UserDataClear();
		break;
		
        case 'c':
        case 'C':
            iRet = FormatFlashFS(flag);
            break;

        case 'd':
        case 'D':
    	      iRet = FormatTFlash(flag);//MCI_FsmFormat(*DriveName);
            break;

        case 'e':
        case 'E':
            #if 1 // gdm 081225 Open in greenstone.vds1->E
            iRet = FormatFlashFS(flag);
            #endif 
            break;
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
      case 'f':
      case 'F':
    	      iRet = FormatSecTFlash(flag);
            break;
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
        default:
		mmi_trace(g_sw_FMGR,"format invalid driver,name is %c",*DriveName);
            break;
    }

    if ( iRet == FS_NO_ERROR )
    {
        return FS_NO_ERROR;
    }

    return FS_ERROR_RESERVED;
}

/*
 * Function:       MMI_FS_GetCheckBufferSize
 * Description:   For FMGR & FS 
 * Created By:    WUZC
 * Created Date: 2007年1月23日    
 * Input:          DriveName
 * Output:        nal 
 * Return:    FS_ERROR_RESERVED     
 * Others:         
 */
INT MMI_FS_GetCheckBufferSize(const UINT8 * DriveName)
{
    return FS_ERROR_RESERVED;   
}

/*
 * Function:       MMI_FS_CheckDrive
 * Description:   For FMGR & FS 
 * Created By:    WUZC
 * Created Date: 2007年1月23日
  * Input:            * DriveName,  * Checkbuffer
 * Output:         nal 
 * Return:         FS_ERROR_RESERVED      
 * Others:         
 */
INT MMI_FS_CheckDrive(const UINT8 * DriveName, BYTE * Checkbuffer)
{
    return FS_ERROR_RESERVED;   
}
#endif /*#ifdef MMI_ON_HARDWARE_P*/
/*
 * Function:       MMI_FS_ConfigExtDevice
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:            Action,  *DrvFuncs,  Slots,  **SlotIdArray,  *SlotsConfiged
 * Output:          nal 
 * Return:          FS_ERROR_RESERVED           
 * Others:         
 */
INT MMI_FS_ConfigExtDevice(INT32 Action, FS_Driver *DrvFuncs, INT32 Slots, void **SlotIdArray, INT32 *SlotsConfiged)
{
    return FS_ERROR_RESERVED;   
}

#ifdef MMI_ON_HARDWARE_P

/*
 * Function:       MMI_FS_XFindClose
 * Description:   For FMGR & FS 
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          Position
 * Output:        nal 
 * Return:    FS_ERROR_RESERVED            
 * Others:         
 */
INT MMI_FS_XFindClose(INT32 * Position)
{
    FreeFileInfoList();
    fssim_sort_cnt = 0;
    return FS_NO_ERROR;   
}
#endif /*#ifdef MMI_ON_HARDWARE_P*/

#if 0 
void Sort(INT32 *items, INT32 count)
{
    INT32 a = 0;
    INT32 b = 0;
    INT32 t = 0;

    for ( a = 1; a < count; ++a )
    {
        for ( b = count - 1; b >= a; --b )
        {
            if ( items[b-1] > items[b] )
            {
                t = items[b - 1];
                items[b-1] = items[b];
                items[b] = t;
            }
        }
    }
}
#endif

void SortByName(INT32 len)
{
    FSSIM_FILE_INFO_NODE *node1 = NULL, *node2 = NULL, *dir_list = NULL,*prenode = NULL;
    FSSIM_FILE_INFO_NODE *file_list = NULL;
    int i,j;
    int dir_count = 0;
    int file_count = 0;
    int exchange = 0;

    // classify de.
    // de_chain = dir_chain + file_chain
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < len; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 1; j < len - i; j++)
        {
            
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }
           
            if((node1->fileinfo.Attributes & FS_ATTR_DIR) < (node2->fileinfo.Attributes & FS_ATTR_DIR))
            {	
                if(node1 == g_fssim_sort_file_list)
                {					
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;				
                    node1 = prenode->next;
                                        	
                }
                else
                {					
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;				
                    node1 = prenode->next;
                }				
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }

    }

    // find the file_list header.
    // accumulative total of directory number.
    dir_list = g_fssim_sort_file_list;   
    while(dir_list) 
    {
        if((dir_list->fileinfo.Attributes & FS_ATTR_DIR))
        {
            dir_count ++;
        }
        else
        {
            file_list = dir_list;
            break;
        }
        if(dir_list->next == 0)
        {
            break;
        }
        if((dir_list->next->fileinfo.Attributes & FS_ATTR_DIR) == 0)
        {
            file_list = dir_list->next;
            dir_list->next = NULL;
            break;
        }        
        
        dir_list = dir_list->next;
    }
    
    file_count = len - dir_count;
    // sort directory list by name. 
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < dir_count; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 1; j < dir_count - i; j++)
        {
            node2 = node1->next;
             if(node2 == 0)
            {
                break;
            }
           
            if (UniStrCmp_IgnoreHAndL((UINT16*)node1->filename, (UINT16*)node2->filename) > 0)
            {	
                if(node1 == g_fssim_sort_file_list)
                {
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;	
                    node1 = prenode->next;
                }
                else
                {
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;		
                    node1 = prenode->next;
                }	
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }

    // sort file list by name. 
    prenode = file_list;
    for(i = 0; i < file_count && file_list != NULL; i++)
    {
        exchange = 0;
        node1 = file_list;
        for(j = 1; j < file_count - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }

            if(UniStrCmp_IgnoreHAndL((UINT16*)node1->filename, (UINT16*)node2->filename) > 0)
            {	
                if(node1 == file_list)
                {
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    file_list = node2;
                    node1 = prenode->next;
                }
                else
                {
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;		
                    node1 = prenode->next;
                }	
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }

    // joint dir chain and file chain.
    if(dir_count == 0)
    {
        g_fssim_sort_file_list = file_list;
    }
    else if(file_count > 0)
    {
        dir_list = g_fssim_sort_file_list;   
        prenode = dir_list;
        while(dir_list)
        {
            prenode = dir_list;
            dir_list = dir_list->next;
        };
        if(prenode)
        {
            prenode->next = file_list;
        }    
     }
     else
     {
        // donoting. 
     }
    return ;
}



void SortBySize(INT32 len)
{
    FSSIM_FILE_INFO_NODE *node1 = NULL, *node2 = NULL, *dir_list = NULL,*prenode = NULL;
    FSSIM_FILE_INFO_NODE *file_list = NULL;
    int i,j;
    int dir_count = 0;
    int file_count = 0;
    int exchange = 0;

    // classify de.
    // de_chain = dir_chain + file_chain
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < len; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 1; j < len - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }
            if((node1->fileinfo.Attributes & FS_ATTR_DIR) < (node2->fileinfo.Attributes & FS_ATTR_DIR))
            {	
                if(node1 == g_fssim_sort_file_list)
                {					
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;				
                    node1 = prenode->next;
                                        	
                }
                else
                {					
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;				
                    node1 = prenode->next;
                }				
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }

    }

    // find the file_list header.
    // accumulative total of directory number.
    dir_list = g_fssim_sort_file_list;	
    while(dir_list) 
    {
        if((dir_list->fileinfo.Attributes & FS_ATTR_DIR))
        {
            dir_count ++;
        }
        else
        {
            file_list = dir_list;
            break;
        }
        if(dir_list->next == 0)
        {
            break;
        }
        if((dir_list->next->fileinfo.Attributes & FS_ATTR_DIR) == 0)
        {
            file_list = dir_list->next;
            dir_list->next = NULL;
            break;
        }        
        
        dir_list = dir_list->next;
    }
    
    file_count = len - dir_count;

    // sort directory list by name. 
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < dir_count; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 1; j < dir_count - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }
            if (UniStrCmp_IgnoreHAndL((UINT16*)node1->filename, (UINT16*)node2->filename) > 0)
            {	
                if(node1 == g_fssim_sort_file_list)
                {
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;	
                    node1 = prenode->next;

                }
                else
                {
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;		
                    node1 = prenode->next;
                }	
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }

    // sort file list by name. 
    prenode = file_list;
    for(i = 0; i < file_count && file_list != NULL; i++)
    {
        exchange = 0;
        node1 = file_list;
        for(j = 1; j < file_count - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }
            if(node1->fileinfo.FileSize > node2->fileinfo.FileSize)
            {	
                if(node1 == file_list)
                {
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    file_list = node2;
                    node1 = prenode->next;

                }
                else
                {
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;		
                    node1 = prenode->next;
                }	
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }

     // joint dir chain and file chain.
    if(dir_count == 0)
    {
        g_fssim_sort_file_list = file_list;
    }
    else if(file_count > 0)
    {
        dir_list = g_fssim_sort_file_list;   
        prenode = dir_list;
        while(dir_list)
        {
            prenode = dir_list;
            dir_list = dir_list->next;
        };
        if(prenode)
        {
            prenode->next = file_list;
        }    
     }
     else
     {
        // donoting. 
     }
    return ;
}



INT16 TimeCmp(FS_DOSDateTime DateTime1, FS_DOSDateTime DateTime2)
{
    if (DateTime1.Year1980 > DateTime2.Year1980)
    {
        return 1;
    }
    else if (DateTime1.Year1980 < DateTime2.Year1980)
    {
        return -1;
    }
    else
    {
        if (DateTime1.Month > DateTime2.Month)
        {
            return 1;
        }
        else if (DateTime1.Month < DateTime2.Month)
        {
            return -1;
        }
        else
        {
            if (DateTime1.Day > DateTime2.Day)
            {
                return 1;
            }
            else if (DateTime1.Day < DateTime2.Day)
            {
                return -1;
            }
            else
            {
                if (DateTime1.Hour > DateTime2.Hour)
                {
                    return 1;
                }
                else if (DateTime1.Hour < DateTime2.Hour)
                {
                    return -1;
                }
                else
                {
                    if (DateTime1.Minute > DateTime2.Minute)
                    {
                        return 1;
                    }
                    else if (DateTime1.Minute < DateTime2.Minute)
                    {
                        return -1;
                    }
                    else
                    {
                        if (DateTime1.Second2 > DateTime2.Second2)
                        {
                            return 1;
                        }
                        else if (DateTime1.Second2 < DateTime2.Second2)
                        {
                            return -1;
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }
}


void SortByTime(INT32 len,  INT8 Flag)
{
    FSSIM_FILE_INFO_NODE *node1 = NULL, *node2 = NULL, *dir_list = NULL,*prenode = NULL;
    FSSIM_FILE_INFO_NODE *file_list = NULL;
    int i,j;
    int dir_count = 0;
    int file_count = 0;
    int exchange = 0;
    int cmp_result;
    
    // classify de.
    // de_chain = dir_chain + file_chain
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < len; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 1; j < len - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }
            if((node1->fileinfo.Attributes & FS_ATTR_DIR) < (node2->fileinfo.Attributes & FS_ATTR_DIR))
            {	
                if(node1 == g_fssim_sort_file_list)
                {					
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;				
                    node1 = prenode->next;
                                        	
                }
                else
                {					
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;				
                    node1 = prenode->next;
                }				
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }

    }

    // find the file_list header.
    // accumulative total of directory number.
    dir_list = g_fssim_sort_file_list;	
    while(dir_list) 
    {
        if((dir_list->fileinfo.Attributes & FS_ATTR_DIR))
        {
            dir_count ++;
        }
        else
        {
            file_list = dir_list;
            break;
        }
        if(dir_list->next == 0)
        {
            break;
        }
        if((dir_list->next->fileinfo.Attributes & FS_ATTR_DIR) == 0)
        {
            file_list = dir_list->next;
            dir_list->next = NULL;
            break;
        }  
        dir_list = dir_list->next;
    }
    
    file_count = len - dir_count;

    // sort directory list by name. 
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < dir_count; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 1; j < dir_count - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }

            cmp_result = \
                (Flag == TRUE) ? \
            TimeCmp(node1->fileinfo.DateTime, node2->fileinfo.DateTime) : \
            TimeCmp(node2->fileinfo.DateTime, node1->fileinfo.DateTime);
            if(cmp_result > 0)
            {	
                if(node1 == g_fssim_sort_file_list)
                {
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;	
                    node1 = prenode->next;

                }
                else
                {
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;		
                    node1 = prenode->next;
                }	
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }

    // sort file list by name. 
    prenode = file_list;
    for(i = 0; i < file_count && file_list != NULL; i++)
    {
        exchange = 0;
        node1 = file_list;
        for(j = 1; j < file_count - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }

           cmp_result = \
                (Flag == TRUE) ? \
            TimeCmp(node1->fileinfo.DateTime, node2->fileinfo.DateTime) : \
            TimeCmp(node2->fileinfo.DateTime, node1->fileinfo.DateTime);
            if(cmp_result > 0)
            {	
                if(node1 == file_list)
                {
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    file_list = node2;
                    node1 = prenode->next;

                }
                else
                {
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;		
                    node1 = prenode->next;
                }	
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }
     // joint dir chain and file chain.
    if(dir_count == 0)
    {
        g_fssim_sort_file_list = file_list;
    }
    else if(file_count > 0)
    {
        dir_list = g_fssim_sort_file_list;   
        prenode = dir_list;
        while(dir_list)
        {
            prenode = dir_list;
            dir_list = dir_list->next;
        };
        if(prenode)
        {
            prenode->next = file_list;
        }    
     }
     else
     {
        // donoting. 
     }
    return ;
}


void SortByType(INT32 len)
{
    FSSIM_FILE_INFO_NODE *node1 = NULL, *node2 = NULL, *dir_list = NULL,*prenode = NULL;
    FSSIM_FILE_INFO_NODE *file_list = NULL;
    int i,j;
    int dir_count = 0;
    int file_count = 0;
    int exchange = 0;

    // classify de.
    // de_chain = dir_chain + file_chain
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < len; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 1; j < len - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }
            if((node1->fileinfo.Attributes & FS_ATTR_DIR) < (node2->fileinfo.Attributes & FS_ATTR_DIR))
            {	
                if(node1 == g_fssim_sort_file_list)
                {					
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;				
                    node1 = prenode->next;
                                        	
                }
                else
                {					
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;				
                    node1 = prenode->next;
                }				
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }

    }

    // find the file_list header.
    // accumulative total of directory number.
    dir_list = g_fssim_sort_file_list;	
    while(dir_list) 
    {
        if((dir_list->fileinfo.Attributes & FS_ATTR_DIR))
        {
            dir_count ++;
        }
        else
        {
            file_list = dir_list;
            break;
        }
        if(dir_list->next == 0)
        {
            break;
        }
        if((dir_list->next->fileinfo.Attributes & FS_ATTR_DIR) == 0)
        {
            file_list = dir_list->next;
            dir_list->next = NULL;
            break;
        }  
        dir_list = dir_list->next;
    }    
    file_count = len - dir_count;
    
    // sort directory list by name. 
    prenode = g_fssim_sort_file_list;
    for(i = 0; i < dir_count; i++)
    {
        exchange = 0;
        node1 = g_fssim_sort_file_list;
        for(j = 1; j < dir_count - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }
            if (UniStrCmp_IgnoreHAndL((UINT16*)node1->filename, (UINT16*)node2->filename) > 0)
            {	
                if(node1 == g_fssim_sort_file_list)
                {
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    g_fssim_sort_file_list = node2;	
                    node1 = prenode->next;

                }
                else
                {
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;		
                    node1 = prenode->next;
                }	
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }

    // sort file list by name. 
    prenode = file_list;
    for(i = 0; i < file_count && file_list != NULL; i++)
    {
        exchange = 0;
        node1 = file_list;
        for(j = 1; j < file_count - i; j++)
        {
            node2 = node1->next;
            if(node2 == 0)
            {
                break;
            }
            if(UniStrCmp_IgnoreHAndL(UniStr_Strrchr((UINT16*)node1->filename,'.'), UniStr_Strrchr((UINT16*)node2->filename,'.')) > 0)
            {	
                if(node1 == file_list)
                {
                    prenode = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    file_list = node2;
                    node1 = prenode->next;

                }
                else
                {
                    prenode->next = node2;					
                    node1->next = node2->next;
                    node2->next = node1;
                    prenode = node2;		
                    node1 = prenode->next;
                }	
                exchange = 1;
            }
            else
            {
                prenode = node1;				
                node1 = node1->next;
            }
        }
        if(exchange == 0)
        {
            break;
        }        
    }

    // joint dir chain and file chain.
    if(dir_count == 0)
    {
        g_fssim_sort_file_list = file_list;
    }
    else if(file_count > 0)
    {
        dir_list = g_fssim_sort_file_list;   
        prenode = dir_list;
        while(dir_list)
        {
            prenode = dir_list;
            dir_list = dir_list->next;
        };
        if(prenode)
        {
            prenode->next = file_list;
        }    
     }
     else
     {
        // donoting. 
     }
    return ;
}

void FmgrSort(INT32 len, INT32 flag)
{

    TRACE_EF(g_sw_FMGR, "FmgrSort(): flag = %x", flag);

    if ( flag & FS_NO_SORT )
    {        
        return ;
    }
    
    if ( flag & FS_SORT_NAME )
    {       
        SortByName(len);
    }
    else if ( flag & FS_SORT_SIZE )
    {
        SortBySize(len);
    }
    else if ( flag & FS_SORT_TIME )
    {        
        SortByTime(len, TRUE);//sort by ascending 	
    }   

    else if ( flag & FS_SORT_TYPE )
    {
        SortByType(len);
    }
    else if ( flag & FS_SORT_DTIME )
    {
        SortByTime(len, FALSE);//sort by descending  
    }
    else
    {
       //do nothing.
    }
    return ;
}

#ifdef MMI_ON_HARDWARE_P

INT MMI_FS_XFindReset(const UINT8 * Pattern, FS_Pattern_Struct * PatternArray, INT32 PatternNum, BYTE ArrayMask, BYTE AttrMask, INT32 Flag, INT32 Timeout, INT32 * Position)
{
    UINT8 pointPoint[6] = {'.', 0, '.', 0, 0, 0};
    UINT8 point[4] = {'.', 0, 0, 0};
    U8 *pFileName = NULL;
    U8 * pUnixFileName = NULL;
	  
    FS_FIND_DATA   *psFindData = NULL;

    FSSIM_FILE_INFO_NODE *psCurrentNode = NULL,*addlistResult=NULL;    
	
    FS_DOSDirEntry FileInfo;
    FS_HANDLE fhd = 0;
    U32 iLen = 0;
    INT32 iRet = 0;
    INT32 cnt = 0;
    
	U8 tmpName[FS_PATH_LEN];
	
    TBM_ENTRY(0x283C);

    MMI_ASSERT( Pattern != NULL);
    MMI_ASSERT(PatternArray != NULL);

//
//     if ( (Flag & FS_NO_SORT) == 0 && (Flag & FS_SORT_NAME) == 0 &&
//          (Flag & FS_SORT_SIZE) == 0 && (Flag & FS_SORT_ATTR) == 0 &&
//          (Flag & FS_SORT_TIME) == 0 && (Flag & FS_SORT_TYPE) == 0 )
//
//         return FS_PARAM_ERROR;
//
//     if ( Flag & ~(FS_FILE_TYPE | FS_DIR_TYPE | FS_NO_SORT | FS_SORT_NAME | FS_SORT_SIZE | FS_SORT_ATTR | FS_SORT_TIME | FS_SORT_TYPE) )
//
//         return FS_PARAM_ERROR;
//

    if((NULL == Pattern) || (NULL == PatternArray))
    {
        TBM_EXIT(0x283C);
        return FS_PARAM_ERROR;
    }
    
    iLen = pfnUnicodeStrlen(Pattern)*2 + pfnUnicodeStrlen(g_vCurDir)*2 + LEN_FOR_UNICODE_NULL_CHAR;
    
    pFileName = OslMalloc(iLen + CVTFUN_CATCHAR_LEN);  
    if(NULL == pFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    
    pUnixFileName = OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);
    if(NULL == pUnixFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }

    psFindData = OslMalloc(sizeof(FS_FIND_DATA));
    if(NULL == psFindData)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }

    memset(psFindData, 0x00, sizeof(FS_FIND_DATA));
    memset(&FileInfo, 0, sizeof(FS_DOSDirEntry));
#ifdef _MS_VC_VER_
    pfnUnicodeStrcpy(pUnixFileName, Pattern);
#else
    ConvertRelative2Full((U8*)pFileName,(U8*)Pattern);
    ConvertDosName2Unix(pUnixFileName, pFileName);
#endif // #ifdef _MS_VC_VER_

    FreeFileInfoList();
    fssim_sort_cnt = 0;

	if ( (pfnUnicodeStrlen(pUnixFileName)) <  FS_FILE_NAME_LEN )
	{
	    UnicodeToAnsii((S8 *)tmpName, (S8 *)pUnixFileName);
	    mmi_trace(g_sw_FMGR,"%s\n", tmpName);
	}
    fhd = FS_FindFirstFile(pUnixFileName, psFindData);


    if (fhd < 0)  //chenhe for jasperII
    //if (fhd == 0)
    {
            TRACE_EF(g_sw_FMGR, " MMI_FS_XFindReset() : FS_FindFirst() ErrCode = %d",fhd);
            iRet = FS_ERROR_RESERVED;
            goto _func_end;
    }

    if ( ( pfnUnicodeStrcmp(psFindData->st_name, pointPoint) != 0 ) &&
    (pfnUnicodeStrcmp(psFindData->st_name, point) != 0) )
    {
        psCurrentNode = OslMalloc(sizeof(FSSIM_FILE_INFO_NODE));
        if (NULL == psCurrentNode)
        {
            iRet = FS_ERROR_RESERVED;
            goto _func_end;
        }
        
        memset(psCurrentNode, 0x00, sizeof(FSSIM_FILE_INFO_NODE));

        iLen = pfnUnicodeStrlen(psFindData->st_name);
        psCurrentNode->filename = OslMalloc(2 * (pfnUnicodeStrlen(psFindData->st_name) + LEN_FOR_NULL_CHAR));
        if (NULL == psCurrentNode->filename)
        {
            OslMfree(psCurrentNode);
            iRet = FS_ERROR_RESERVED;
            goto _func_end;
        }

        pfnUnicodeStrcpy(psCurrentNode->filename, psFindData->st_name);
        
		mmi_trace(g_sw_FMGR, "gdm: MMI_FS_XFindReset() psFindData->st_mode=%x",psFindData->st_mode);
        if ( psFindData->st_mode & FS_ATTR_DIR )
        {
			FileInfo.Attributes = psFindData->st_mode;  
        }
        else if (!( psFindData->st_mode & FS_ATTR_DIR ))
        {
		#if 0//gdm 090804 CSW update fs_attribute
            FileInfo.Attributes = FS_ATTR_ARCHIVE;  
		#else
            FileInfo.Attributes = psFindData->st_mode;  
		#endif
            FileInfo.FileSize = psFindData->st_size;
        }
		mmi_trace(g_sw_FMGR, "gdm: MMI_FS_XFindReset() FileInfo.Attributes=%x",FileInfo.Attributes);

        Convert2DosDateTime(&(FileInfo.DateTime), psFindData->st_ctime);

        memcpy(&psCurrentNode->fileinfo, &FileInfo, sizeof(FS_DOSDirEntry));

        addlistResult=AddToFileInfoList(psCurrentNode);
	if(addlistResult == NULL)
	{
		OslMfree(psCurrentNode->filename );
		OslMfree(psCurrentNode);
		iRet = FS_ERROR_RESERVED;
		goto _func_end;
	}
        psCurrentNode = NULL;
        cnt++;
	
    }

    do 
    {
        memset(psFindData, 0, sizeof(FS_FIND_DATA));
        memset(&FileInfo, 0, sizeof(FS_DOSDirEntry));
   //chenhe for jasperII+
           iRet = FS_FindNextFile(fhd, psFindData);

         // iRet = MCI_FindNextFile(psFindData);
	mmi_trace(g_sw_FMGR,"MMI_FS_XFindReset,fine next fileHandle is %ld",iRet);
	 if ( ERR_SUCCESS == iRet )
        {
            if ( ( pfnUnicodeStrcmp(psFindData->st_name, pointPoint) != 0 ) &&
            (pfnUnicodeStrcmp(psFindData->st_name, point) != 0) )
            {
                psCurrentNode = OslMalloc(sizeof(FSSIM_FILE_INFO_NODE));
                if (NULL == psCurrentNode)
                {
                    fssim_sort_cnt = cnt;
                    iRet = cnt; //iRet = FS_ERROR_RESERVED;
                    goto _func_end;
                }

                memset(psCurrentNode, 0x00, sizeof(FSSIM_FILE_INFO_NODE));

                psCurrentNode->filename = OslMalloc(2 * (pfnUnicodeStrlen(psFindData->st_name) + LEN_FOR_NULL_CHAR));
                if (NULL == psCurrentNode->filename)
                {
                    OslMfree(psCurrentNode);
                    fssim_sort_cnt = cnt;
                    iRet = cnt; //iRet = FS_ERROR_RESERVED;
                    goto _func_end;
                }

                pfnUnicodeStrcpy(psCurrentNode->filename, psFindData->st_name);

				mmi_trace(g_sw_FMGR, "gdm: MMI_FS_XFindReset() psFindData->st_mode=%x",psFindData->st_mode);
                if ( psFindData->st_mode & FS_ATTR_DIR )
                {
		            FileInfo.Attributes = psFindData->st_mode;  
                }
                else if ( !(psFindData->st_mode & FS_ATTR_DIR ))
                {
				#if 0//gdm 090804 CSW update fs_attribute
		            FileInfo.Attributes = FS_ATTR_ARCHIVE;  
				#else
		            FileInfo.Attributes = psFindData->st_mode;  
				#endif
                    FileInfo.FileSize = psFindData->st_size;
                }
				mmi_trace(g_sw_FMGR, "gdm: MMI_FS_XFindReset() FileInfo.Attributes=%x",FileInfo.Attributes);

                Convert2DosDateTime(&(FileInfo.DateTime), psFindData->st_ctime);
                memcpy(&psCurrentNode->fileinfo, &FileInfo, sizeof(FS_DOSDirEntry));
                addlistResult=AddToFileInfoList(psCurrentNode);
		if(addlistResult == NULL)
		{
			OslMfree(psCurrentNode->filename );
		    OslMfree(psCurrentNode);
		    iRet = FS_ERROR_RESERVED;
		    goto _func_end;
		}			
		
                psCurrentNode = NULL;
                cnt++;
	        if (cnt >= MT_MAX_FILE_NUM) 
		{
		          /* too many files */
		          
		          iRet = ERR_FS_NO_MORE_FILES;
			   fssim_sort_cnt = cnt;		          
		          break;
	        }
		
            }
        }
    }while (ERR_SUCCESS == iRet);

    if ( iRet == ERR_FS_NO_MORE_FILES )
    {
        iRet = cnt;
    }
    else
    {
        TRACE_EF(g_sw_FMGR, " MMI_FS_XFindReset() :FS_FindNextFile ErrCode = %d. ", iRet);
        iRet = FS_ERROR_RESERVED;
    }

    fssim_sort_cnt = cnt;
    FmgrSort(cnt, Flag);

    //*Position = g_fssim_sort_file_list;

_func_end:
    if( fhd>=0)
    {
	    if ( ERR_SUCCESS == (FS_FindClose(fhd)) )
	    {
	        ;
	    }
	    else
	    {
	        TRACE_EF(g_sw_FMGR, " MMI_FS_XFindReset() : FS_FindClose(%d) Close failed!",fhd);
	        iRet = FS_ERROR_RESERVED;
	    }
    }



    if(NULL != pFileName)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }
    if(NULL != pUnixFileName)
    {
        OslMfree(pUnixFileName);
        pUnixFileName = NULL;
    }
    if(NULL != psFindData)
    {
        OslMfree(psFindData);
        psFindData = NULL;
    }
    
    TBM_EXIT(0x283C);
    return iRet;
    
}

/*
 * Function:       MMI_FS_XFindStart
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Calls:          
 * Called By:      
 * Input:          
 * Output:         
 * Return:         
 * Others:         
 */
INT MMI_FS_XFindStart(const UINT8 * Pattern, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength, INT32 Index, INT32 * Position, INT32 Flag)
{
    
    FSSIM_FILE_INFO_NODE *psFileInfoNode = NULL;
    
    TBM_ENTRY(0x283D);
    MMI_ASSERT(FileName != NULL);
    MMI_ASSERT(Position != NULL);
    /*MMI_ASSERT(*Position == (DWORD)g_fssim_sort_file_list);*/ //gdm 090610


    if ( fssim_sort_cnt <= 0 ) 
    {
        TBM_EXIT(0x283D);
        return FS_ACCESS_DENIED;
    }

    if ( Index >= (DWORD)fssim_sort_cnt )
    {
        TBM_EXIT(0x283D);
        return FS_NO_MORE_FILES;
    }
    /* copy FileInfo */
    psFileInfoNode = GetFileInfoByIndex(Index);
    if (NULL == psFileInfoNode)
    {
        TBM_EXIT(0x283D);
        return FS_NO_MORE_FILES;
    }
    
    memcpy(FileInfo, &(psFileInfoNode->fileinfo), sizeof(FS_DOSDirEntry));

    /* copy file name */
    pfnUnicodeStrcpy(FileName, psFileInfoNode->filename);
#if 0 
    if ( (MaxLength / sizeof(TCHAR)) < pfnUnicodeStrlen(g_fssim_sort_file_list[Index].filename) )
    {
        /* user buffer is not sufficient */

        if ( Flag == FS_FIND_LFN_TRUNC )
        {
            pfnUnicodeStrcpy(FileName, g_fssim_sort_file_list[Index].filename);
        }
        else
        {
#ifdef NAME_TRUNC_T0_83
            /* use short file name */
            pch = FileInfo->FileName;
            pchdst = (char *)FileName;

            for ( i = 0; i < MaxLength; i += sizeof(TCHAR) )
            {
                if ( pch == FileInfo->FileName + 8 )
                {
                    pch = FileInfo->Extension;
                }

                memcpy(pchdst, pch, sizeof(TCHAR));

                pch++;

                pchdst += sizeof(UINT8);
            }
#endif
        }
    }
    else
    {
        pfnUnicodeStrcpy(FileName, g_fssim_sort_file_list[Index].filename);

    }
#endif

    TBM_EXIT(0x283D);
    return Index;
}

/*
 * Function:       MMI_FS_FindFirstN
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:        * NamePattern,  * PatternArray,  PatternNum,  ArrayMask,  Attr,  AttrMask,  * FileInfo,  * FileName,  MaxLength,  EntryIndex, INT32 Flag)  
 * Output:  nal        
 * Return:      FS_ERROR_RESERVED   
 * Others:         
 */
INT MMI_FS_FindFirstN(const UINT8 * NamePattern, FS_Pattern_Struct * PatternArray, INT32 PatternNum, BYTE ArrayMask, BYTE Attr, BYTE AttrMask, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength, INT32 EntryIndex, INT32 Flag)
{
#if 0
    TRACE_FMGR_FUNCTION();

    MMI_ASSERT(NamePattern != NULL);
    MMI_ASSERT(FileInfo != NULL);
    MMI_ASSERT(FileName ! = NULL);
    MMI_ASSERT(PatternArray != NULL);

    S8 szFileName[MAX_FILE_PATH_LEN*ENCODING_LENGTH] = "\0";
    U8 buf[MAX_FILE_NAME_LEN] = "\0";
    FS_HANDLE fhd = NULL;
    FS_FIND_DATA   FindData;
    UINT8 st_name[ MAX_FILE_NAME_LEN ] = "\0";
    UINT8   Ret  =0;
    INT32     i = 0;
    BOOL    tag = FALSE;
    INT32 beginindex = 0;
    U8  *ptr= NULL;

    memset(&FindData, 0x00, sizeof(FS_FIND_DATA));

    UnicodeToAnsii(szFileName, NamePattern);

    ptr = strstr(szFileName, "C:");
    if ( ptr != NULL )
        ptr = ptr + 2;
    else
        ptr = szFileName;


    szFileName[0] = UPCASE(szFileName[0]);

    for ( i=0; i< PatternNum; i++ )
    {
        UnicodeToAnsii(buf, PatternArray[i].Pattern);
        TRACE_EF(g_sw_FMGR, "MMI_FS_FindFirstN() PatternArray[%d] is %s. ", i, buf);
    }

    fhd = FS_FindFirstFile(ptr, &FindData);
    TRACE_EF(g_sw_FMGR, "MMI_FS_FindFirstN() NamePattern is %s(%s); fhd = %d; ArrayMask = 0x%0x; EntryIndex=%d ", szFileName, ptr,fhd, ArrayMask,EntryIndex);


    if ( fhd >= 0 )
    {
        if ( FindData.st_mode & INODE_TYPE_DIRECTORY )
        {
            FileInfo->Attributes = FS_ATTR_DIR;
        }
        else if ( FindData.st_mode & INODE_TYPE_REGULAR )
        {
            FileInfo->Attributes = 0x00;            
            FileInfo->FileSize = FindData.st_size;  
        }
        else
        {
            FileInfo->Attributes = 0x00;            
        }
        //TRACE_EF(g_sw_FMGR, "FindData.st_name = %s; st_mode = 0x%0x; st_size = %d;", FindData.st_name, FindData.st_mode, FindData.st_size);

        do
        {
            if ( ((ArrayMask & FS_ATTR_DIR) && (FindData.st_mode & INODE_TYPE_DIRECTORY)) ||
            ((ArrayMask & FS_FILE_TYPE) && (FindData.st_mode & INODE_TYPE_REGULAR))
            )
            {
                for ( i = 0; i < PatternNum; i++ )
                {
                    memset(buf, 0x00, sizeof(buf));
                    UnicodeToAnsii(buf, PatternArray[i].Pattern);

                    if ( strstr(buf, FindData.st_name) )
                    {
                        tag = TRUE;         
                        break;
                    }
                }                   
            }

            if ( (EntryIndex > beginindex++) || (tag == TRUE) )
            {
                Ret  = FS_FindNextFile(fhd, &FindData);
                if ( Ret == FALSE )
                    return FS_ERROR_RESERVED;
            }
            else
            {
                if ( (MaxLength / 2) > strlen(FindData.st_name) )
                    memcpy(st_name, FindData.st_name, strlen(FindData.st_name));
                else
                    memcpy(st_name, FindData.st_name, MaxLength / 2);

                AnsiiToUnicodeString(FileName, st_name);
                return fhd;             
            }           
            tag = FALSE;
        }while ( Ret == TRUE );   
    }
    else
    {
        return FS_ERROR_RESERVED;
    }
#endif
    return FS_ERROR_RESERVED;
}

#ifdef MMI_ON_HARDWARE_P
/*
 * Function:    MMI_FS_FindNextN   
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日    
 * Input:             Handle,  * PatternArray,  PatternNum,  ArrayMask,  * FileInfo,  * FileName,  MaxLength,  Flag
 * Output:         FileName FileInfo
 * Return:      FS_ERROR_RESERVED    FS_NO_ERROR
 * Others:         
 */
INT MMI_FS_FindNextN(FS_HANDLE Handle, FS_Pattern_Struct * PatternArray, INT32 PatternNum, BYTE ArrayMask, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength, INT32 Flag)
{
#if 0
    FS_FIND_DATA   FindData;
    UINT8   st_name[MAX_FILE_PATH_LEN*ENCODING_LENGTH ] = "\0";
    U8      buf[MAX_FILE_NAME_LEN*2] = "\0";
    BOOL    Ret = FALSE;
    BOOL    tag = FALSE;
    INT32     i = 0;
    MMI_ASSERT( Handle != NULL);
    MMI_ASSERT(PatternArray != NULL);
    MMI_ASSERT(FileInfo != NULL);
    MMI_ASSERT(FileName ! =  NULL);

    memset(&FindData, 0x00, sizeof(FS_FIND_DATA));

    Ret = FS_FindNextFile(Handle, &FindData);
    //TRACE_EF(g_sw_FMGR, "MMI_FS_FindNextN()  fhd = %d ;ArrayMask = 0x%0x", Handle,ArrayMask);
    for ( i = 0; i < PatternNum; i++ )
    {
        UnicodeToAnsii(buf, PatternArray[i].Pattern);
        TRACE_EF(g_sw_FMGR, "MMI_FS_FindFirstN() PatternArray[%d] is %s. ", i, buf);
    }

    if ( Ret == ERR_SUCCESS )
    {
        if ( FindData.st_mode & INODE_TYPE_DIRECTORY )
        {
            FileInfo->Attributes = FS_ATTR_DIR;
        }
        else if ( FindData.st_mode & INODE_TYPE_REGULAR )
        {
            FileInfo->Attributes = 0x00;
            FileInfo->FileSize = FindData.st_size;      
        }
        else
        {
            FileInfo->Attributes = 0x00;            
        }

        //TRACE_EF(g_sw_FMGR, "FindData.st_name= %s; st_mode=0x%0x ;st_size = %d ", 	FindData.st_name, FindData.st_mode, FindData.st_size);

        do
        {
            if ( ((ArrayMask & FS_ATTR_DIR) && (FindData.st_mode & INODE_TYPE_DIRECTORY)) ||
            ((ArrayMask & FS_FILE_TYPE) && (FindData.st_mode & INODE_TYPE_REGULAR))
            )
            {
                for ( i = 0; i < PatternNum; i++ )
                {
                    memset(buf, 0x00, sizeof(buf));
                    UnicodeToAnsii(buf, PatternArray[i].Pattern);

                    if ( strstr(buf, FindData.st_name) )
                    {
                        tag = TRUE;
                        break;
                    }
                }                   
            }

            if ( tag == TRUE )
            {
                Ret  = FS_FindNextFile(Handle, &FindData);
                if ( Ret == FALSE )
                    return FS_ERROR_RESERVED;
            }
            else
            {
                if ( (MaxLength / 2) > strlen(FindData.st_name) )
                    memcpy(st_name, FindData.st_name, strlen(FindData.st_name));
                else
                    memcpy(st_name, FindData.st_name, MaxLength / 2);

                AnsiiToUnicodeString(FileName, st_name);

                return FS_NO_ERROR;
            }
            tag = FALSE;
        }while ( Ret == TRUE );
    }
    else
    {
        return FS_ERROR_RESERVED;
    }
#endif
    return FS_ERROR_RESERVED;
}
#endif /*#ifdef MMI_ON_HARDWARE_P*/

/*
 * Function:       MMI_FS_Rename
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:      * FileName,   * NewName
 * Output:        nal 
 * Return:      FS_ERROR_RESERVED    FS_NO_ERROR
 * Others:         
 */
INT MMI_FS_Rename(const UINT8 * FileName, const UINT8 * NewName)
{
    INT32 iRet = 0;
    U32 iLenOld = 0;
    U32 iLenNew = 0;
    U32 iLen = 0;
    U8 *pFileName = NULL;
    U8 *pFOldName = NULL;
    U8 *pFNewName = NULL;

	U8 tmpName[FS_PATH_LEN];
	if ( (pfnUnicodeStrlen(FileName)) <  FS_FILE_NAME_LEN )
	{
	    UnicodeToAnsii((S8 *)tmpName, (S8 *)FileName);
	    mmi_trace(g_sw_FMGR,"%s\n", tmpName);
	}
	if ( (pfnUnicodeStrlen(NewName)) <  FS_FILE_NAME_LEN )
	{
	    UnicodeToAnsii(tmpName, (S8 *)NewName);
	    mmi_trace(g_sw_FMGR,"%s\n",tmpName);
	}
    TBM_ENTRY(0x283E);
    TRACE_FMGR_FUNCTION();

    MMI_ASSERT(FileName != NULL);
    MMI_ASSERT(NewName != NULL);

    if((NULL == FileName) || (NULL == NewName))
    {
         TBM_EXIT(0x283E);
         return FS_PARAM_ERROR;
    }

    iLenOld = pfnUnicodeStrlen(FileName)*2+ pfnUnicodeStrlen(g_vCurDir)*2 + LEN_FOR_UNICODE_NULL_CHAR;
    
    pFOldName = OslMalloc(iLenOld + MMI_FS_TFLASH_POINT_LEN);  
    if(NULL == pFOldName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
   
   memset(pFOldName,0x0,iLenOld + MMI_FS_TFLASH_POINT_LEN);
    iLenNew = pfnUnicodeStrlen(NewName)*2 + pfnUnicodeStrlen(g_vCurDir)*2 + LEN_FOR_UNICODE_NULL_CHAR;
    pFNewName = OslMalloc(iLenNew + MMI_FS_TFLASH_POINT_LEN);
    if(NULL == pFNewName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }

	memset(pFNewName,0x0,iLenNew + MMI_FS_TFLASH_POINT_LEN);
    iLen = iLenNew > iLenOld ? iLenNew : iLenOld;
    pFileName = OslMalloc( iLen + MMI_FS_TFLASH_POINT_LEN + LEN_FOR_UNICODE_NULL_CHAR);
    
    if(NULL == pFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }

    ConvertRelative2Full((U8*)pFileName,(U8*)FileName);
    ConvertDosName2Unix(pFOldName, pFileName);
    
    memset(pFileName, 0, iLen + MMI_FS_TFLASH_POINT_LEN + LEN_FOR_UNICODE_NULL_CHAR);
    ConvertRelative2Full((U8*)pFileName,(U8*)NewName);
    ConvertDosName2Unix(pFNewName, pFileName);
    

    iRet = FS_Rename(pFOldName, pFNewName);
    if ( ERR_SUCCESS  != iRet )
    {
        TRACE_EF(g_sw_FMGR, " [MMI_FS_Rename :], FS_Rename ErrCode = %d", iRet);
        if (ERR_FS_FILE_EXIST == iRet)
        {
            if (0 == UniStrCmp_IgnoreHAndL((UINT16*)pFOldName, (UINT16*)pFNewName))
            {// 新名字跟旧名字一致，直接返回成功。
                iRet = FS_NO_ERROR;
                TRACE_EF(g_sw_FMGR, "[MMI_FS_Rename :] UniStrCmp_IgnoreHAndL iRet=%d;File: %s;Line: %d", iRet);
                goto _func_end;
            }

            iRet = FS_FILE_EXISTS;
            goto _func_end;
        }
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }

    iRet = FS_NO_ERROR;
    
_func_end:

    if (pFileName != NULL)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }

    if (pFOldName != NULL)
    {
        OslMfree(pFOldName);
        pFOldName = NULL;
    }
    
    if (pFNewName != NULL)
    {
        OslMfree(pFNewName);
        pFNewName = NULL;
    }
        
    TBM_EXIT(0x283E);
    return iRet;
}

/*
 * Function:       MMI_FS_Count
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:           * FullPath,  Flag  
 * Output:         nal
 * Return:      FS_ERROR_RESERVED    FS_NO_ERROR
 * Others:         
 */
 INT MMI_FS_Count(const UINT8* pcDirName, INT32 Flag)
{
    INT32 i, iDepth = 0;
    INT32 iFindFd[NR_MAX_DEPTH];
    UINT16 *pOriginal, *p = NULL;
    INT32 iRet;
    UINT8 c, c2;
    UINT16 UniChar;
    UINT32 iNameLen;
    FS_FIND_DATA sFindData;
    UINT8 strUni[] = {'/', 0, '*', 0, 0, 0};
    U8 *pcPath = NULL;
    U8 *pcDosPath = NULL;
    INT iFileNum = 0;

    TBM_ENTRY(0x283F);
    
    pcPath = (UINT8*)OslMalloc(MAX_FILE_PATH_UNICODE_LEN + LEN_FOR_UNICODE_NULL_CHAR);
    if( NULL == pcPath )
    {
        TBM_EXIT(0x283F);
        return FS_ERROR_RESERVED;
    }
    pcDosPath = (UINT8*)OslMalloc(MAX_FILE_PATH_UNICODE_LEN + LEN_FOR_UNICODE_NULL_CHAR);
    if( NULL == pcDosPath )
    {
        OslMfree(pcPath);
        TBM_EXIT(0x283F);
        return FS_ERROR_RESERVED;
     }

    p = (UINT16*)pcPath;
    
    ConvertRelative2Full((U8*)pcDosPath,(U8*)pcDirName);
    ConvertDosName2Unix(pcPath, pcDosPath);

    TRACE_EF(g_sw_FMGR,"Enter in [MMI_FS_Count] pcPath:");
    
    while (*p)
    {
        p++;
    }
    while (TRUE)
    {// 去掉字符串后面多余的'/'。
        if ((UINT32)(p - 1) == (UINT32)pcPath)
        {// 如果要统计的是根目录则保留。
            break;
        }
        UniChar = *(p - 1);
        c = *((UINT8*)(&UniChar));
        c2 = *((UINT8*)(&UniChar) + 1);
        if (('/' == c || '\\' == c) && c2 == 0)
        {
            p--;
        }
        else
        {
            break;
        }
    }
    *p = (UINT16)0;
    pOriginal = p;

    for (i = 0; i < NR_MAX_DEPTH; ++i)
    {
        iFindFd[i] = INVALID_FD;
    }

    while (TRUE)
    {
        if (INVALID_FD == iFindFd[iDepth])
        {
            if ((UINT32)(p + pfnUnicodeStrlen(strUni)) > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
            {
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            pfnUnicodeStrcpy((S8*)p,(S8*)strUni);
            p++;

            iFindFd[iDepth] = FS_FindFirstFile(pcPath, &sFindData);
            if (ERR_FS_NO_MORE_FILES == iFindFd[iDepth])
            {
                goto current_dir_empty;
            }
            else if (iFindFd[iDepth] < 0)
            {
                iDepth--;
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
                         
            if (!FS_IS_DIR( sFindData.st_mode ))
            {
                if ( Flag & FS_FILE_TYPE )
                {
                    iFileNum++;
                }
            }
            else if (FS_IS_DIR( sFindData.st_mode )) 
            {
                if ('.'== sFindData.st_name[0] && 0 == sFindData.st_name[1])
                {
                    ;
                }
                else
                {
                    if (Flag & FS_DIR_TYPE)
                    {
                        iFileNum++;
                    }
                    if (Flag & FS_RECURSIVE_TYPE)
                    {
                        if ((UINT32)(p + pfnUnicodeStrlen(sFindData.st_name)) 
                            > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
                        {
                            iRet = FS_ERROR_RESERVED;
                            goto failed;
                        }
                        pfnUnicodeStrcpy((S8*)p,(S8*)sFindData.st_name);
                        goto recursive_dir;
                    }
                }
            }
        }

        while ( ERR_SUCCESS == ( iRet = FS_FindNextFile( iFindFd[iDepth], &sFindData ) ) ) 
        {
            if (!FS_IS_DIR( sFindData.st_mode ))
            {
                if ( Flag & FS_FILE_TYPE )
                {
                    iFileNum++;
                }
            }
            else if (FS_IS_DIR( sFindData.st_mode )) 
            {
                if ('.'== sFindData.st_name[0] && 0 == sFindData.st_name[1])
                {
                    ;
                }
                else
                {
                    if (Flag & FS_DIR_TYPE)
                    {
                        iFileNum++;
                    }
                    if (Flag & FS_RECURSIVE_TYPE)
                    {
                        if ((UINT32)(p + pfnUnicodeStrlen(sFindData.st_name)) 
                            > (UINT32)(pcPath + MAX_FILE_PATH_UNICODE_LEN))
                        {
                            iRet = FS_ERROR_RESERVED;
                            goto failed;
                        }
                        pfnUnicodeStrcpy((S8*)p,(S8*)sFindData.st_name);
                        goto recursive_dir;
                    }
                }
            }
        }

        if (iRet != ERR_FS_NO_MORE_FILES)
        {
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }

        // 已统计完当前目录。
        iRet = FS_FindClose(iFindFd[iDepth]);

current_dir_empty:
        iFindFd[iDepth] = INVALID_FD;
        iDepth--;
        p--;
        *p = (UINT16)0;
        
        if (p == pOriginal)
        {
            if (iDepth != -1)
            {
                TRACE_EF(g_sw_FMGR,  "[#error!!]: p == pOriginal && iDepth != 0.\n");
                iRet = FS_ERROR_RESERVED;
                goto failed;
            }
            break;
        }

        while (TRUE)
        {// 返回上级目录继续统计。
            UniChar = *(p - 1);
            c = *((UINT8*)(&UniChar));
            c2 = *((UINT8*)(&UniChar) + 1);
            if (!(('/' == c || '\\' == c) && c2 == 0))
            {
                p--;
            }
            else
            {
                break;
            }
        }
                    
        *p = (UINT16)0;
        continue;
        
recursive_dir:    // 压栈，进行统计子目录。
        if ((NR_MAX_DEPTH - 1) == iDepth)
        {// 如果栈满则退出。
            iRet = FS_ERROR_RESERVED;
            goto failed;
        }      
        iNameLen = pfnUnicodeStrlen(sFindData.st_name);
        p += iNameLen;
        iDepth++;
    }
    
    OslMfree(pcPath);
    OslMfree(pcDosPath);
    mmi_trace(g_sw_FMGR,"iFileNum=%d.\n",iFileNum);
    TBM_EXIT(0x283F);
    return iFileNum;

failed:

    OslMfree(pcPath);
    OslMfree(pcDosPath);

    TRACE_EF(g_sw_FMGR, "+++++failed !!!+++++.\n", iRet);
    for (i = iDepth; i >= 0; --i)
    {
        TRACE_EF(g_sw_FMGR, "+++++FS_FindClose: iFindFd[%d] = %d +++++.\n",i, iFindFd[i]);
        FS_FindClose(iFindFd[i]);
    }
    TBM_EXIT(0x283F);
    return (INT)iRet;
        
}

#if 0
INT MMI_FS_Count(const UINT8 * FullPath, INT32 Flag)
{
    TBM_ENTRY(0x283F);

    UINT8 *pFileName = NULL;
    UINT8 *pUnixFileName = NULL;
    UINT8 *pfullpath = NULL;
    UINT8 str1[10] = {0};
    UINT8 str2[10] = {0};

    FS_HANDLE fh = NULL;
    FS_FIND_DATA *fd = NULL;
    U32 filecount = 0;
    U32 dircount = 0;
    U32 iLen = 0;
    INT32 iRet = 0, retError = 0;


    iLen = MAX_FILE_PATH_UNICODE_LEN + MMI_FS_TFLASH_POINT_LEN  + LEN_FOR_UNICODE_NULL_CHAR;
    
    pFileName = OslMalloc(iLen);  
    if(NULL == pFileName)
    {
        retError = FS_ERROR_RESERVED;
        goto _func_end;
    }
    
    pUnixFileName = OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);
    if (NULL == pUnixFileName)
    {
        retError = FS_ERROR_RESERVED;
        goto _func_end;
    }

    pfullpath =(UINT8*)OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);

    if ( NULL == pfullpath )
    {
        retError = FS_ERROR_RESERVED;
        goto _func_end;
    }

    fd = OslMalloc(sizeof(FS_FIND_DATA));
    if(NULL == fd)
    {
        retError = FS_ERROR_RESERVED;
        goto _func_end;
    }
    
    memset(fd, 0x00, sizeof(FS_FIND_DATA));

    ConvertRelative2Full(pFileName, FullPath);
    ConvertDosName2Unix(pUnixFileName, pFileName);

    pfnUnicodeStrcpy(pfullpath, pUnixFileName);

    AnsiiToUnicodeString(str1, "/*");
    AnsiiToUnicodeString(str2, "*");

    iLen = pathlen(pfullpath);

    if ( ( *(pfullpath + iLen - 2) != '/' ) && ( *(pfullpath + iLen + 0) == 0   ) && ( *(pfullpath + iLen + 1) == 0   ) )
    {
        UCS2Strcat(pfullpath, str1);
    }
    else if ( ( *(pfullpath + iLen - 2) == '/' ) && ( *(pfullpath + iLen - 1) == 0   ) &&
    ( *(pfullpath + iLen + 0) == 0   ) &&  ( *(pfullpath + iLen + 1) == 0   ) )
    {
        UCS2Strcat(pfullpath, str2);
    }
    else
    {
        retError = FS_PARAM_ERROR;
        goto _func_end;
    }

    if ( Flag & FS_DIR_TYPE )
    {
        memset(str1, 0, sizeof(str1));
        memset(str2, 0, sizeof(str2));
        AnsiiToUnicodeString(str1, ".");
        AnsiiToUnicodeString(str2, "..");

        fh = FS_FindFirstFile(pfullpath, fd);

        while ( fh >= 0 )
        {
            if ( (fd->st_mode & INODE_TYPE_DIRECTORY) && // filter "." ,  ".."  Dir
                   ( UCS2Strcmp(fd->st_name, str1) != 0 ) &&
                   ( UCS2Strcmp(fd->st_name, str2) != 0 ) )
            {
                dircount++;

                memset(pFileName, 0x00, iLen);
                if ( ( *(pUnixFileName + iLen - 2) == '/' ) && ( *(pUnixFileName + iLen - 1) == 0   ) &&
                ( *(pUnixFileName + iLen) == 0 ) && ( *(pUnixFileName + iLen + 1) == 0   ) )
                {
                    UCS2Strcpy(pFileName, pUnixFileName);
                    UCS2Strcat(pFileName, fd->st_name);
                }
                else
                {
                    UCS2Strcpy(pFileName, pUnixFileName);
                    memset(str2, 0, sizeof(str2));
                    AnsiiToUnicodeString(str2, "/");
                    UCS2Strcat(pFileName, str2);
                    UCS2Strcat(pFileName, fd->st_name);
                }

                if ( Flag & FS_RECURSIVE_TYPE )
                    iRet = MMI_FS_Count(pFileName, FS_DIR_TYPE|FS_RECURSIVE_TYPE);
                //Ret: 如果为目录为空，则递归到里面的时候FS_FindFirstFile()返回为零。

                if ( iRet != FS_ERROR_RESERVED )
                {
                    dircount += iRet;
                }
            }
            else if ( fd->st_mode & INODE_TYPE_UNKNOWN )
            {
                MMI_ASSERT(0);
            }

            if ( ERR_SUCCESS  != FS_FindNextFile(fh, fd) )
            {
                break;
            }
        }
        FS_FindClose(fh);
    }

    if ( Flag & FS_FILE_TYPE )
    {
        memset(str1, 0, sizeof(str1));
        memset(str2, 0, sizeof(str2));
        AnsiiToUnicodeString(str1, ".");
        AnsiiToUnicodeString(str2, "..");

        fh = FS_FindFirstFile(pfullpath, fd);

        while ( fh >= 0 )
        {
            if ( fd->st_mode & INODE_TYPE_REGULAR )
                filecount++;

            if ( ( fd->st_mode & INODE_TYPE_DIRECTORY ) && // filter "." ,  ".."  Dir
                  ( UCS2Strcmp(fd->st_name, str1) != 0 ) &&
                  ( UCS2Strcmp(fd->st_name, str2) != 0 ) )
            {
                memset(pFileName, 0x00, iLen);

                if ( ( *(pUnixFileName + iLen - 2) == '/' )  && ( *(pUnixFileName + iLen - 1) == 0   ) &&
                ( *(pUnixFileName + iLen) == 0 ) && ( *(pUnixFileName + iLen + 1) == 0   ) )
                {
                    UCS2Strcpy(pFileName, pUnixFileName);
                    UCS2Strcat(pFileName, fd->st_name);
                }
                else
                {
                    UCS2Strcpy(pFileName, pUnixFileName);
                    memset(str2, 0, sizeof(str2));
                    AnsiiToUnicodeString(str2, "/");
                    UCS2Strcat(pFileName, str2);
                    UCS2Strcat(pFileName, fd->st_name);
                }
                TRACE_FMGR_FUNCTION();
                if ( Flag & FS_RECURSIVE_TYPE )
                {
                    iRet = MMI_FS_Count(pFileName, FS_RECURSIVE_TYPE|FS_FILE_TYPE);
                }

                if ( iRet != FS_ERROR_RESERVED )
                {
                    filecount += iRet;
                }
            }
            else if ( fd->st_mode & INODE_TYPE_UNKNOWN )
            {
                MMI_ASSERT(0);
            }
            if ( ERR_SUCCESS  != FS_FindNextFile(fh, fd) )
                break;
        }
        FS_FindClose(fh);
    }


    if ( (Flag & FS_DIR_TYPE) && !(Flag & FS_FILE_TYPE) )
    {
        retError = dircount;
        goto _func_end;
    }

    if ( (Flag & FS_FILE_TYPE) && !(Flag & FS_DIR_TYPE) )
    {
        TRACE_EF(g_sw_FMGR,"MMI_FS_Count() return filecount= %d", filecount);
        retError = filecount;
        goto _func_end;
    }

    if ( (Flag & FS_DIR_TYPE) || (Flag & FS_FILE_TYPE) )
    {
        TRACE_EF(g_sw_FMGR," MMI_FS_Count() return filecount +dircount= %d", filecount +dircount);
        retError = filecount +dircount;
        goto _func_end;
    }
    else
    {
        TRACE_EF(g_sw_FMGR,"MMI_FS_Count() return filecount= %d", filecount);
        retError = FS_GENERAL_FAILURE;
        goto _func_end;
    }

    retError = FS_NO_ERROR;

    _func_end:

    if ( pFileName != NULL )
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }

    if ( pUnixFileName != NULL )
    {
        OslMfree(pUnixFileName);
        pUnixFileName = NULL;
    }

    if ( pfullpath != NULL )
    {
        OslMfree(pfullpath);
        pfullpath = NULL;
    }

    if(fd != NULL)
    {
        OslMfree(fd);
        fd = NULL;
    }
    
    TBM_EXIT(0x283F);
    return retError;
}
#endif

/*
 * Function:     MMI_FS_GetAttributes  
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
 * Input:          FileName
 * Output:         nal
 * Return:      FS_ERROR_RESERVED    FS_NO_ERROR
 * Others:         
 */
#if 1 //unmask gdm to support MMI_FS_GetAttributes() 080522
INT MMI_FS_GetAttributes(const UINT8 * FileName)
{
    
    U8 szFileName[MAX_FILE_PATH_LEN*ENCODING_LENGTH] = "\0";
    U8 szUnixFileName[MAX_FILE_PATH_LEN*ENCODING_LENGTH] = "\0";
    INT32  attr = 0;
    FS_STAT fs_stat;

    ConvertRelative2Full((U8*)szFileName,(U8*)FileName);
    ConvertDosName2Unix(szUnixFileName, szFileName);

    memset(&fs_stat, 0x00, sizeof(FS_STAT));

    if ( FALSE == FS_Stat(szUnixFileName, &fs_stat) )
    {
        return FS_ERROR_RESERVED;
    }

#if 0//gdm 080522
    //目前只支持FS_ATTR_DIR

    if ( fs_stat.st_mode & INODE_TYPE_DIRECTORY )
    {
        attr |= FS_ATTR_DIR;
    }
#else
    attr = fs_stat.st_mode;
#endif
    return attr;
}
#endif /*#ifdef MMI_ON_HARDWARE_P*/

/*
 * Function:       MMI_FS_SetAttributes
 * Description:    For FMGR & FS
 * Created By:    WUZC
 * Created Date: 2007年1月23日
  * Input:           FileName,  Attributes
 * Output:         nal
 * Return:        FS_NO_ERROR 
 * Others:         
 */
INT MMI_FS_SetAttributes(const UINT8 * FileName, BYTE Attributes)
{
    // U8 szFileName[MAX_FILE_PATH_LEN*ENCODING_LENGTH] = "\0";
    //目前只支持FS_ATTR_DIR
     U8 szFileName[MAX_FILE_PATH_LEN*ENCODING_LENGTH] = "\0";
    U8 szUnixFileName[MAX_FILE_PATH_LEN*ENCODING_LENGTH] = "\0";
    INT32 NewFlag = 0;
    INT32 iRet = FS_NO_ERROR;
	
    ConvertRelative2Full((U8*)szFileName,(U8*)FileName);
    ConvertDosName2Unix(szUnixFileName, szFileName);


   iRet =  FS_SetFileAttr(szUnixFileName, NewFlag);
    	
    return iRet;
}
#endif

//====================================================
// MMI_FS_IsEndofFIle    2007-6-27   shizh    create
//====================================================
BOOL MMI_FS_IsEndOfFile(FS_HANDLE FileHandle)
{
    INT32 ret = 0;

    ret = FS_IsEndOfFile(FileHandle);

    if ( ret )
    {
        return TRUE;
    }

    TRACE_EF(g_sw_FMGR, "[Error!]MMI_FS_IsEndOfFile 0x%x", ret);

    return FALSE;

}

#ifdef MMI_ON_HARDWARE_P
//====================================================
// MMI_FS_GetFileSizeByName          2007-6-27   shizh    create
//====================================================

INT32 MMI_FS_GetFileSizeByName(const UINT8 *FileName)
{
    
    U8 *pFileName = NULL;
    U8 *pUnixFileName = NULL;
    INT32  iRet = 0;
    U32 iLen = 0;
    FS_FILE_ATTR attr;

    MMI_ASSERT(FileName != NULL);

    if(NULL == FileName)
    {
        return FS_PARAM_ERROR;
    }
    
    iLen = pfnUnicodeStrlen(FileName)*2 + pfnUnicodeStrlen(g_vCurDir)*2 + LEN_FOR_UNICODE_NULL_CHAR;
    pFileName = OslMalloc(iLen + CVTFUN_CATCHAR_LEN);  
    if(NULL == pFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    pUnixFileName = OslMalloc(iLen + MMI_FS_TFLASH_POINT_LEN);
    if(NULL == pUnixFileName)
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }
    ConvertRelative2Full((U8*)pFileName,(U8*)FileName);
    ConvertDosName2Unix(pUnixFileName, pFileName);

    iRet = FS_GetFileAttr(pUnixFileName, &attr);

    if ( ERR_SUCCESS == iRet )
    {
        iRet =  attr.i_size;
        goto _func_end;
    }
    else
    {
        iRet = FS_ERROR_RESERVED;
        goto _func_end;
    }

_func_end:
    if(NULL != pFileName)
    {
        OslMfree(pFileName);
        pFileName = NULL;
    }
    if(NULL != pUnixFileName)
    {
        OslMfree(pUnixFileName);
        pUnixFileName = NULL;
    }
    return iRet;

}
#endif /*#ifdef MMI_ON_HARDWARE_P*/

//====================================================
// MMI_FS_GetFileName          2007-6-27   shizh    create
//====================================================
INT MMI_FS_GetFileName(FS_HANDLE FileHandle, INT32 iNameBufferLen, UINT8 *FileName )
{
    
    INT32 iRet = 0;

    MMI_ASSERT(FileHandle >= 0);
    MMI_ASSERT(FileName != NULL);
  
    iRet = FS_GetFileName(FileHandle, iNameBufferLen, FileName);

    if ( ERR_SUCCESS != iRet )
    {
        return FS_ERROR_RESERVED;
    }

    return FS_NO_ERROR;
    
}
//====================================================
// Format vds1          2007-6-23   shizh    create
//====================================================
INT32 FormatFlashFS(UINT8 iFlag)
{
    INT32 iRet = 0;
    UINT8 home[32] = {0};    

    AnsiiToUnicodeString(home, "/home");

    if ( g_hasMountedFlashFS )
    {
        iRet = FS_Unmount(home, TRUE);

        if ( iRet != ERR_SUCCESS )
        {
            return FS_ERROR_RESERVED;
        }

        g_hasMountedFlashFS = FALSE;
    }

    
#if 1//gdm 081208 to display flash menu
    iRet = FS_Format("vds1", FS_TYPE_FAT, iFlag);

    if ( iRet != ERR_SUCCESS )
    {
        g_hasMountedFlashFS = FALSE; 
        TRACE_EF(g_sw_FMGR, "Format vds1 failed! Errcode = %d", iRet);
        return FS_ERROR_RESERVED;
    }
	
    iRet = FS_MakeDir(home, 0);

    if ( iRet != ERR_SUCCESS )
    {
        if ( iRet != ERR_FS_FILE_EXIST )
        {
            g_hasMountedFlashFS = FALSE;  
            return FS_ERROR_RESERVED;
        }

        TRACE_EF(g_sw_FMGR, "FS_MakeDir(home):exist! ");
    }
    iRet = FS_Mount( "vds1", home, 0, FS_TYPE_FAT );

    if ( iRet == ERR_SUCCESS )
    {
        g_hasMountedFlashFS = TRUE;
        if(MakePhoneDir())
	    return FS_ERROR_RESERVED;

        TRACE_EF(g_sw_FMGR, "in FormatFlashFS vds1 has been mounted successfully ! g_hasMountedFlashFS = %d", g_hasMountedFlashFS);
        return FS_NO_ERROR;
    }
#endif
    g_hasMountedFlashFS = FALSE;
    return FS_ERROR_RESERVED;

}
//====================================================
// Format mmc0           2007-6-23   shizh    create
//====================================================
INT32 FormatTFlash(UINT8 iFlag)
{
    INT32 iRet = 0;
    UINT8 tflash[20] = {0};

    /*WeiD Modify Start For 6208 Ver: TBM780  on 2007-8-17 15:5 */
    AnsiiToUnicodeString(tflash, TFLASH_ROOT);
    /*WeiD Modify End  For 6208 Ver: TBM780  on 2007-8-17 15:5 */

    if ( g_hasMountedTFlashFS )
    {
        iRet = FS_Unmount(tflash, TRUE);
        if ( iRet != ERR_SUCCESS )
        {
            return FS_ERROR_RESERVED;
        }
        g_hasMountedTFlashFS = FALSE;
    }

    iRet = FS_MakeDir(tflash, 0);

    if ( iRet != ERR_SUCCESS )
    {
        if ( iRet != ERR_FS_FILE_EXIST )
        {
            return FS_ERROR_RESERVED;
        }
    }

    iRet = FS_Format("mmc0", FS_TYPE_FAT, iFlag);

    if ( iRet != ERR_SUCCESS )
    {
        g_hasMountedTFlashFS = FALSE;
        TRACE_EF(g_sw_FMGR, " Format mmc0 failed! ErrCode = %d", iRet);
        return FS_ERROR_RESERVED;
    }

    iRet = FS_Mount( "mmc0", tflash, 0, FS_TYPE_FAT );

    if ( iRet == ERR_SUCCESS )
    {
        g_hasMountedTFlashFS = TRUE;  
        MakeTFlashDir();
        TRACE_EF(g_sw_FMGR, "tfalsh has been mounted successfully ! "
        "g_hasMountedTFlashFS= %d", g_hasMountedTFlashFS);
        return FS_NO_ERROR;
    }
    TRACE_EF(g_sw_FMGR, " mount mmc0 failed! ErrCode = %d", iRet);

    return FS_ERROR_RESERVED;
}

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
INT32 FormatSecTFlash(UINT8 iFlag)
{
    INT32 iRet = 0;
    UINT8 sectflash[20] = {0};

    /*WeiD Modify Start For 6208 Ver: TBM780  on 2007-8-17 15:5 */
    AnsiiToUnicodeString(sectflash, TFLASH_SECOND_ROOT_NAME);
    /*WeiD Modify End  For 6208 Ver: TBM780  on 2007-8-17 15:5 */

    if ( g_hasMountedTFlash2FS )
    {
        iRet = FS_Unmount(sectflash, TRUE);
        if ( iRet != ERR_SUCCESS )
        {
            return FS_ERROR_RESERVED;
        }
        g_hasMountedTFlash2FS = FALSE;
    }

    iRet = FS_MakeDir(sectflash, 0);

    if ( iRet != ERR_SUCCESS )
    {
        if ( iRet != ERR_FS_FILE_EXIST )
        {
            return FS_ERROR_RESERVED;
        }
    }

    iRet = FS_Format("mmc1", FS_TYPE_FAT, iFlag);

    if ( iRet != ERR_SUCCESS )
    {
        g_hasMountedTFlash2FS = FALSE;
        TRACE_EF(g_sw_FMGR, " Format mmc1 failed! ErrCode = %d", iRet);
        return FS_ERROR_RESERVED;
    }

    iRet = FS_Mount( "mmc1", sectflash, 0, FS_TYPE_FAT );

    if ( iRet == ERR_SUCCESS )
    {
        g_hasMountedTFlash2FS = TRUE;  
        MakeSecTFlashDir();
        TRACE_EF(g_sw_FMGR, "sec tfalsh has been mounted successfully ! "
        "g_hasMountedTFlash2FS= %d", g_hasMountedTFlash2FS);
        return FS_NO_ERROR;
    }
    TRACE_EF(g_sw_FMGR, " mount mmc1 failed! ErrCode = %d", iRet);

    return FS_ERROR_RESERVED;
}

VOID MakeSecTFlashDir()
{
    U8 szDirName[40] = {0};
    INT iRet = FS_ERROR_RESERVED;
    
    //make "PHOTOS" dir
    AnsiiToUnicodeString(szDirName, MMI_FS_SEC_TFLASH_PHOTO_DIR);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"photos\" dir failed!");
    }
    //make "VIDEO" dir
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_SEC_TFLASH_VIDEO_DIR);
    iRet = MMI_FS_CreateDir(szDirName);    
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"Video\" dir failed!");
    }
    
    //make "MYMUSIC " dir
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_SEC_TFLASH_MUSIC_DIR);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"My Music\" dir failed!");
    }

    //make "AUDIO" dir
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_SEC_TFLASH_AUDIO_DIR);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"Audio\" dir failed!");
    }
	
#ifdef __MMI_EBOOK_READER__
    //make "EBOOK" dir
	memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_SEC_TFLASH_EBOOK_DIR);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"ebook\" dir failed!");
    }
#endif	
    
}
#endif
/*- zhouqin modify for dual t-flash card 20110503*/

//====================================================
// MakePhoneDir        2007-7-12   shizh    create
//====================================================
INT32  MakePhoneDir()
{
    U8 szDirName[40] = {0};
    INT iRet = FS_ERROR_RESERVED;
	
#if 1//modify gdm 081225, dynamic select drive
    U8 drive;
    U8 buf[10];
    U8 Ubuf[40];
    drive = MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 1, FS_NO_ALT_DRIVE);
    if ( (drive  >= 'A') && (drive <= 'Z') )
    {
        sprintf ((PS8)buf, "%c:/", drive);
    }	
    else
    {
    	 return FS_ERROR_RESERVED;
    }

    //make "PHOTOS" dir
    AnsiiToUnicodeString(szDirName, buf);
    AnsiiToUnicodeString(Ubuf, MMI_FS_PHONE_PHOTO_DIR);
    pfnUnicodeStrcat((PS8)szDirName, Ubuf);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"photos\" dir failed!");
    	 return FS_ERROR_RESERVED;
    }
	
    //make "MYMUSIC " dir
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, buf);
    AnsiiToUnicodeString(Ubuf, MMI_FS_PHONE_MUSIC_DIR);
    pfnUnicodeStrcat((PS8)szDirName, Ubuf);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"My Music\" dir failed!");
    	 return FS_ERROR_RESERVED;
    }

    //make "VUDIO" dir
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, buf);
    AnsiiToUnicodeString(Ubuf, MMI_FS_PHONE_AUDIO_DIR);
    pfnUnicodeStrcat((PS8)szDirName, Ubuf);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"Audio\" dir failed!");
    	 return FS_ERROR_RESERVED;
    }
	
    //make "VIDEO" dir
#if 0//modify gdm 081225,greenstone, dont need create "Video" folder on vds1
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_PHONE_VIDEO_DIR);
    iRet = MMI_FS_CreateDir(szDirName);    
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"video\" dir failed!");
    }
#endif

#endif
    return FS_NO_ERROR;
}

//====================================================
// MakeTFlashDir     2007-7-12   shizh    create
//====================================================
VOID MakeTFlashDir()
{
    U8 szDirName[40] = {0};
    INT iRet = FS_ERROR_RESERVED;
    
    //make "PHOTOS" dir
    AnsiiToUnicodeString(szDirName, MMI_FS_TFLASH_PHOTO_DIR);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"photos\" dir failed!");
    }
    //make "VIDEO" dir
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_TFLASH_VIDEO_DIR);
    iRet = MMI_FS_CreateDir(szDirName);    
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"Video\" dir failed!");
    }
    
    //make "MYMUSIC " dir
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_TFLASH_MUSIC_DIR);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"My Music\" dir failed!");
    }

    //make "AUDIO" dir
    memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_TFLASH_AUDIO_DIR);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"Audio\" dir failed!");
    }
	
#ifdef __MMI_EBOOK_READER__
    //make "EBOOK" dir
	memset(szDirName, 0, sizeof(szDirName));
    AnsiiToUnicodeString(szDirName, MMI_FS_TFLASH_EBOOK_DIR);
    iRet = MMI_FS_CreateDir(szDirName);
    if ( iRet != FS_NO_ERROR)
    {
        TRACE_EF(g_sw_FMGR, "Create \"ebook\" dir failed!");
    }
#endif	
    
}
//====================================================


#undef FILESYSTEMSRC_C_



