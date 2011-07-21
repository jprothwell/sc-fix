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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *  FileSystemDef.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  This file defines type, data structure, constant of File System Simulation.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_FILESYSTEMDEF_H
#define _MMI_FILESYSTEMDEF_H


#include "l4dr.h"
#include "fmt_def.h"    
/*add start by wuzc at 20061218 for fs*/
#include "fat_fs.h"  
#include "fs.h"
/*add end by wuzc at 20061218 for fs*/
/***************************************************************************** 
* Define
*****************************************************************************/
/*WeiD Add Start For 6208 Ver: TBM780  on 2007-8-17 15:4 */
#ifdef MMI_ON_HARDWARE_P
#define   TFLASH_ROOT                  "/t"
#define   TFLASH_ROOT_PATH        "/t/"
#else
#define   TFLASH_ROOT                  "E:"
#define   TFLASH_ROOT_PATH        "E:/"
#endif

/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
#ifdef MMI_ON_HARDWARE_P
#define TFLASH_SECOND_ROOT_NAME           "/t1"
#define TFLASH_SECOND_PATH_NAME  "/t1/"

#else
#define   TFLASH_SECOND_ROOT_NAME                  "F:"
#define   TFLASH_SECOND_PATH_NAME        "F:/"
#endif
#endif
/*- zhouqin modify for dual t-flash card 20110503*/

/*WeiD Add End  For 6208 Ver: TBM780  on 2007-8-17 15:4 */
#define	MAX_BUFF_SIZE			512
#define	MAX_FILE_NAME			200
#define 	FS_MAX_DRIVE			2
#define OpenedList_SemInit() do { g_OpenedList_Sem = COS_CreateSemaphore(1); } while(0)
#define OpenedList_Down()    do { COS_WaitForSemaphore(g_OpenedList_Sem, COS_WAIT_FOREVER); } while(0)
#define OpenedList_Up()        do { COS_ReleaseSemaphore(g_OpenedList_Sem); } while(0)
extern HANDLE g_OpenedList_Sem;
typedef enum
{

   FS_NOR_STORAGE,
   FS_REMOVABLE_STORAGE,
#if defined(DUAL_TFLASH_SUPPORT)
   FS_TFCARD_SECOND,
#endif   
   FS_TOTAL_STORAGE
} fs_drive_enum;

/* wufasong added 2007.05.04 from here for the Old file system APIs. */
typedef struct _FS_STAT 
{
    UINT32  st_dev;        //Device ID of device containing file
    UINT32  st_ino;        //File serial number
    UINT16  st_mode;       //Mode of file 
    UINT16  st_nlink;      //Number of hard links to the file
    UINT32  st_size;       //For regular files, the file size in bytes
    UINT32  st_atime;      //Time of last access to the file
    UINT32  st_mtime;      //Time of last data modification
    UINT32  st_ctime;      //Time of last status(or inode) change
    UINT8   st_name[ FS_FILE_NAME_LEN + LEN_FOR_NULL_CHAR ];
}FS_STAT;

//-------------------------------------------------------------------

BOOL SRVAPI FS_Stat ( 
    PCSTR pszPathName, 
    FS_STAT * pStatData
);

BOOL SRVAPI FS_Query( 
    INT8 iQueryIndex, 
    PVOID pBuf 
);

//-------------------------------------------------------------------


INT   MMI_FS_GetFirstCluster(FS_HANDLE FileHandle, U32 * Cluster);

#ifdef MMI_ON_HARDWARE_P
	
	#define FILE_HANDLE				FS_HANDLE 
	FILE_HANDLE						OpenCache(U8*	pFileName, S32 nType);
	
	#define PFS_READ				FS_READ_ONLY
	#define PFS_READ_BINARY		PFS_READ
	
	//Opens an empty file for writing. If the given file exists, its contents are destroyed. 
	#define PFS_WRITE				FS_READ_WRITE|FS_CREATE_ALWAYS
	//Opens for writing and Reading; creates the file first if it doesn’t exist. 
	#define PFS_WRITE_READ			FS_READ_WRITE|FS_CREATE

	#define pfopen(x,y,...)			FS_Open((x),(y),0)
	#define pfclose(x)			FS_Close((x))
									
	#define pfwrite(x1,x2,x3, ...)  FS_Write((x1),(x2),(x3))
	#define pfread(x1,x2,x3,...)   FS_Read((x1),(x2),(x3))
									
	#define pfseek(x1,x2,x3)		FS_Seek((x1),(x2),(x3))
	#define pfdelete(x)	      		FS_Delete((x))
	
	#define pfrename(x,y)	   	FS_Rename((U16 *)(x),(U16 *)(y))
	#define pfcreateDir(x)	   	FS_MakeDir ((U8*)(x),0)
	#define pfdirexit(x,y)		((x),(y))
	
	#define pfindfirst(x1, x2)		FS_FindFirstFile((U8*)(x1),(PFS_FIND_DATA*)(x2))  
	#define pfindnext(x1, x2) 		FS_FindNextFile((x1),(PFS_FIND_DATA*)(x2))
	#define pffindclose(h) FS_FindClose((h))

	#define pfhasformatted FS_HasFormatted
	#define pfformat(name) FS_Format((name))
	#define pfmountroot FS_MountRoot

	PUBLIC S32 pfgetfilesize(FS_HANDLE hFile, S32 *pSize);
	
#else   /* MMI_ON_HARDWARE_P */

	/*
	 * NoteXX: i have change some definitions and declarations here
	 *         for integration of File System Simulation.
	 *         (Originally most functions are connected to C Run-time
	 *         library.)
	 */
	
	#define FILE_HANDLE int
	
	FILE_HANDLE OpenCache(U8 *pFileName, U8 *pType);
	FILE_HANDLE OpenFileFS(U8 *pFileName, U8 *pType);
	FILE_HANDLE pFindFirstEx(const PS8 x1, U8 x2, U8 x3,  void* x4,  PS8 x5,  U32 x6 );
	U8 pFindNextEx( FILE_HANDLE x1,  void* x2,  PS8 x3,  U32 x4 );
	void CloseFileFS(FILE_HANDLE f);
	size_t ReadFileFS(void *buffer, size_t size, size_t count, FILE_HANDLE f, size_t *read);
	size_t WriteFileFS(const void *buffer, size_t size, size_t count, FILE_HANDLE f);
		
	//Opens for reading. If the file does not exist or cannot be found.Fails Returned To Call 
	#define PFS_READ _T("r")
	//Opens an empty file for writing. If the given file exists, its contents are destroyed. 
	#define PFS_WRITE _T("w")
	//Opens for writing and Reading; File Pointer at EOF.creates the file first if it doesn’t exist. 
	#define PFS_WRITE_READ _T("a+")
	#define PFS_READ_BINARY	_T("rb")
	
	#define pfopen(x,y)	fopen(x,y)
	#define pfclose fclose
	#define pfwrite(x1,x2,x3,x4,x5) fwrite(x1, x2, x3, x4)
	#define pfread(x1,x2,x3,x4,x5) fread(x1, x2, x3,x4,x5)
	#define pfseek fseek
	#define pfdelete remove
	#define pfrename rename
	#define pcreateDir(x) CreateDir(x)/*no use*/
	#define pfindfirst(x1, x2, x3, x4, x5, x6) ((const PS8)x1, (U8)x2, (U8)x3, (void*)x4, (PS8)x5, (U32)x6 ) /*no use*/
	#define pfindnext(x1, x2, x3, x4) ((FILE_HANDLE)x1, (void*)x2, (PS8)x3, (U32)x4 )/*no use*/
	#define pffindclose(h) ((void *)h)

#endif  /* MMI_ON_HARDWARE_P */
/***************************************************************************** 
* Typedef 
*****************************************************************************/

/***************************************************************************** 
* Extern Global Variable
*****************************************************************************/

/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
 
/*del by wuzc at 20061218 for fs */
#if 0
S32 Searchfile(FILE_HANDLE);
 

S32 GetFSFileSize(FILE_HANDLE handle);
S32 StartFileRead(FILE_HANDLE handle, U32 nIndex);
S32 pfgetc(FILE_HANDLE handle);
pBOOL IsEndOfCurrentFile(void);
S32 pCacheFileRead(FILE_HANDLE handle, S32 nBufferSize, S32* nRead, U8*	pBuffer);
S32 pCacheFileSeek(FILE_HANDLE handle, S32 nBytes, S32 nType);
S32 pCopyFile(PU8 pSourceFile, PU8 pDestFile, pBOOL bFailIfExists);
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
S32 pCopyFileFromOffset(FILE_HANDLE fSourceFile, PU8 pDestFile, U32 offset, U32 size);
#endif								
#endif
/*del by wuzc at 20061218 for fs */

#endif /*_MMI_FILESYSTEMDEF_H */

