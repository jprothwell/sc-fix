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
 *	fs_func.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    This file declares the exported APIs
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _FS_FUNC_H
#define _FS_FUNC_H

#include "kal_release.h"
#include "fs_type.h"

#ifndef _FS_TYPE_H
#error "Please include fs_type.h first..."
#endif

#include "csw.h"
#include "mmi_data_types.h"


// General I/O
#define __FS_SORT_SUPPORT__
#define __FS_MOVE_SUPPORT__

#ifdef FILESYSTEMSRC_C_
#define FSAPI_EXTERN
#else
#define FSAPI_EXTERN extern 
#endif

FSAPI_EXTERN FS_HANDLE MMI_FS_Open(const UINT8 * FileName, INT32 Flag);     
FSAPI_EXTERN INT MMI_FS_OpenHint(const UINT8 * FileName, UINT Flag, FS_FileLocationHint * DSR_Hint);
FSAPI_EXTERN INT MMI_FS_Close(FS_HANDLE FileHandle);         
FSAPI_EXTERN INT MMI_FS_CloseAll(void);
FSAPI_EXTERN INT MMI_FS_Read(FS_HANDLE FileHandle, void *DataPtr, INT32 Lenth, INT32*ReadLen);         
FSAPI_EXTERN INT MMI_FS_Write(FS_HANDLE FileHandle, void * DataPtr, INT32 Length, INT32 * Written);    
//add 2010-0205 for QQ
FSAPI_EXTERN INT MMI_FS_Flush(FS_HANDLE FileHandle);  
//add end
FSAPI_EXTERN INT MMI_FS_SetSeekHint(FS_HANDLE FileHandle, UINT HintNum, FS_FileLocationHint * Hint);
FSAPI_EXTERN INT MMI_FS_Seek(FS_HANDLE FileHandle, INT32 Offset, INT32 Whence);             
FSAPI_EXTERN INT MMI_FS_Commit(FS_HANDLE FileHandle);
FSAPI_EXTERN INT MMI_FS_ReleaseFH(void * TaskId);
FSAPI_EXTERN INT MMI_FS_Abort(INT32 ActionHandle);
FSAPI_EXTERN INT MMI_FS_ParseFH(FS_HANDLE FileHandle);

//Information
FSAPI_EXTERN INT MMI_FS_GetFileInfo(S8* FileHandle, FS_FileInfo * FileInfo);
FSAPI_EXTERN INT MMI_FS_GetFileSize(FS_HANDLE FileHandle, INT32 * Size);
FSAPI_EXTERN INT MMI_FS_GetFilePosition(FS_HANDLE FileHandle, UINT * Position);
FSAPI_EXTERN INT MMI_FS_SetFileTime(FS_HANDLE FileHandle, const FS_DOSDateTime * Time);
FSAPI_EXTERN INT MMI_FS_GetAttributes(const UINT8 * FileName);
FSAPI_EXTERN INT MMI_FS_GetFirstCluster(FS_HANDLE FileHandle, UINT * Cluster);
extern INT32 MMI_FS_GetFileSizeByName( const UINT8 * FileName);
extern INT MMI_FS_GetFileName(FS_HANDLE FileHandle, INT32 iNameBufferLen, UINT8 *FileName );

//File Only Operation
FSAPI_EXTERN INT MMI_FS_SetAttributes(const UINT8 * FileName, BYTE Attributes);
FSAPI_EXTERN INT MMI_FS_Delete(const UINT8 * FileName);
FSAPI_EXTERN INT MMI_FS_PseudoMerge(const UINT8 * FileName1, const UINT8 * FileName2);
FSAPI_EXTERN INT MMI_FS_CheckFile(const UINT8 * FileName);

//Folder Only Operation
FSAPI_EXTERN INT MMI_FS_GetCurrentDir(UINT8 * DirName, UINT MaxLength);
FSAPI_EXTERN INT MMI_FS_SetCurrentDir(const UINT8 * DirName);
FSAPI_EXTERN INT MMI_FS_CreateDir(const UINT8 * DirName);
FSAPI_EXTERN INT MMI_FS_RemoveDir(const UINT8 * DirName);
FSAPI_EXTERN INT MMI_FS_GetFolderSize(const UINT8 *DirName, UINT Flag);

//File and Folder Operations
FSAPI_EXTERN INT MMI_FS_Extend(FS_HANDLE FileHandle, UINT Length);
FSAPI_EXTERN INT MMI_FS_Truncate(FS_HANDLE FileHandle);
FSAPI_EXTERN INT MMI_FS_MakeFileName(FS_HANDLE FileHandle, UINT Flag, UINT8 * FileName, UINT MaxLength);
FSAPI_EXTERN INT MMI_FS_Rename(const UINT8 * FileName, const UINT8 * NewName);
BOOL FS_IsDirFile(UINT8 *pszFileName);

#ifdef __FS_MOVE_SUPPORT__
FSAPI_EXTERN INT MMI_FS_Move(const UINT8 * SrcFullPath, const UINT8 * DstFullPath, INT32 Flag, FS_ProgressCallback Progress,INT32 IfExist );
#endif

FSAPI_EXTERN INT MMI_FS_Count(const UINT8 * FullPath, INT32 Flag);
FSAPI_EXTERN INT MMI_FS_XDelete(const UINT8 * FullPath, INT32 Flag);

//Find File
FSAPI_EXTERN INT MMI_FS_FindFirst(const UINT8 * NamePattern, BYTE Attr, BYTE AttrMask, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength);
FSAPI_EXTERN INT MMI_FS_FindNext(FS_HANDLE FileHandle, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength);
FSAPI_EXTERN INT MMI_FS_FindFirstN(const UINT8 * NamePattern, FS_Pattern_Struct * PatternArray, INT32 PatternNum, BYTE ArrayMask, BYTE Attr, BYTE AttrMask, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength, INT32 EntryIndex, INT32 Flag);
FSAPI_EXTERN INT MMI_FS_FindNextN(FS_HANDLE Handle, FS_Pattern_Struct * PatternArray, INT32 PatternNum, BYTE ArrayMask, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength, INT32 Flag);
FSAPI_EXTERN INT MMI_FS_FindClose(FS_HANDLE FileHandle);

#ifdef __FS_SORT_SUPPORT__
FSAPI_EXTERN INT MMI_FS_XFindReset(const UINT8 * Pattern, FS_Pattern_Struct * PatternArray, INT32 PatternNum, BYTE ArrayMask, BYTE AttrMask, INT32 Flag, INT32 Timeout, INT32 * Position);
FSAPI_EXTERN INT MMI_FS_XFindStart(const UINT8 * Pattern, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength, INT32 Index, INT32 * Position, INT32 Flag);
FSAPI_EXTERN INT MMI_FS_XFindClose(INT32 * Position);
#endif

//Drive Management
FSAPI_EXTERN INT MMI_FS_CreateBootSector(void * BootSector, const FS_PartitionRecord * Partition, BYTE MediaDescriptor, UINT MinSectorsPerCluster, UINT Flags);
FSAPI_EXTERN INT MMI_FS_GetPartitionInfo(const UINT8 * DriveName, FS_PartitionRecord * PartitionInfo);
FSAPI_EXTERN INT MMI_FS_CommitAll(const UINT8 * DriveName);
FSAPI_EXTERN INT MMI_FS_GetDrive(INT32 Type, INT32 Serial, INT32 AltMask);
FSAPI_EXTERN INT MMI_FS_MappingDrive(UINT UpperOldDrv, UINT UpperNewDrv);
FSAPI_EXTERN INT MMI_FS_GeneralFormat(const UINT8 * DriveName, INT32 Level, FS_FormatCallback Progress);
FSAPI_EXTERN UINT32 MMI_FS_GetDirSize( CONST UINT8 * dirName) ;

#ifdef __FS_QM_SUPPORT__
FSAPI_EXTERN INT MMI_FS_QmGetFree(const BYTE * Path);
#endif

FSAPI_EXTERN INT MMI_FS_GetClusterSize(UINT DriveIdx);

#if (defined __FS_CHECKDRIVE_SUPPORT__) || (defined _NAND_FLASH_BOOTING_) || (defined __FS_SYSDRV_ON_NAND__)
FSAPI_EXTERN INT MMI_FS_GetCheckBufferSize(const UINT8 * DriveName);
FSAPI_EXTERN INT MMI_FS_CheckDrive(const UINT8 * DriveName, BYTE * Checkbuffer);
#endif

//Disk Management
FSAPI_EXTERN INT MMI_FS_CreateMasterBootRecord(void * SectorBuffer, const FS_PartitionRecord * DiskGeometry);
FSAPI_EXTERN INT MMI_FS_SplitPartition(void * MasterBootRecord, UINT Sectors);
FSAPI_EXTERN INT MMI_FS_GetDiskInfo(const UINT8 * DriveName, FS_DiskInfo * DiskInfo, INT32 Flags);
FSAPI_EXTERN void MMI_FS_ShutDown(void);
FSAPI_EXTERN INT MMI_FS_UnlockAll(void);
FSAPI_EXTERN INT MMI_FS_SanityCheck(void);
FSAPI_EXTERN INT MMI_FS_SetDiskFlag(void);
FSAPI_EXTERN INT MMI_FS_CheckDiskFlag(void);
FSAPI_EXTERN INT MMI_FS_ClearDiskFlag(void);
FSAPI_EXTERN INT MMI_FS_GetDevType(const UINT8 * Path);
FSAPI_EXTERN INT MMI_FS_GetDevStatus(INT32 DriveIdx, INT32 Flag);
FSAPI_EXTERN INT MMI_FS_LockFAT(UINT Type);

//Card management
FSAPI_EXTERN INT MMI_FS_TestMSDC(INT32 MSDCIndex);
FSAPI_EXTERN INT MMI_FS_CloseMSDC(INT32 MSDCIndex, INT32 Mode);
//OTG Card Reader Management 
FSAPI_EXTERN INT MMI_FS_ConfigExtDevice(INT32 Action, FS_Driver *DrvFuncs, INT32 Slots, void **SlotIdArray, INT32 *SlotsConfiged);

//Debug
#ifdef __FS_TRACE_SUPPORT__
FSAPI_EXTERN INT MMI_FS_SetTrace(UINT Flag, UINT Timeout);
FSAPI_EXTERN INT MMI_FS_DumpFHTable(void);
#endif

//Copyright
#ifdef __P_PROPRIETARY_COPYRIGHT__
FSAPI_EXTERN INT MMI_FS_SweepCopyrightFile(void);
FSAPI_EXTERN INT MMI_FS_GetCopyrightList(void);
FSAPI_EXTERN INT MMI_FS_GetCopyrightConfig(UINT *Protect, UINT *NonProtect, const UINT8 **Folder,
                          const FS_Pattern_Struct **Pattern, UINT **List1, UINT **List2);
#endif

#endif //_FS_FUNC_H

