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
 *	fs_type.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *    This file defines the type/structure for export
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _FS_TYPE_H
#define _FS_TYPE_H
/*del by wuzc at 20061218 for fs*/
#if 0
#include "csw.h"   // zrx add 20060524
#endif
#include "mmi_data_types.h"
#include "kal_release.h"
/*del by wuzc at 20061218 for fs*/

/*------------------------- SPECIAL DEFINE -----------------------*/
#ifndef MAX_TLS_INDEX
#define MAX_TLS_INDEX            2
#endif

#define FS_TRACE_INDEX           2
#define FS_MAX_PATH              520             
#define FS_BUFFER_SIZE           512

//Attributes in SPEC
#define FS_ATTR_READ_ONLY        0x01
#define FS_LONGNAME_ATTR         0x0F

//Customized Device List
#define FS_DEVICE_REMOVABLE      0x0008
#define FS_DEVICE_NOR_FLASH      0x0040
#define FS_DEVICE_NAND_FLASH     0x0080

/*-------------------- Parameter for APIs ---------------------*/
//FS_Open Parameter

#define FS_READ_ONLY             0x00000100L
#define FS_OPEN_SHARED           0x00000200L
#define FS_OPEN_NO_DIR           0x00000400L
#define FS_OPEN_DIR              0x00000800L
#define FS_READ_WRITE            0x00001000L
#define FS_CREATE                0x00010000L
#define FS_CREATE_ALWAYS         0x00020000L 
#define FS_COMMITTED		     0x01000000L
#define FS_CACHE_DATA	         0x02000000L
#define FS_LAZY_DATA		     0x04000000L
#define FS_NONBLOCK_MODE	     0x10000000L
#define FS_PROTECTION_MODE		 0x20000000L


//FS_GetDiskInfo Parameter
#define FS_DI_BASIC_INFO         0x00000001L
#define FS_DI_FREE_SPACE         0x00000002L
#define FS_DI_FAT_STATISTICS     0x00000004L

//FS_GetDrive Parameter
#define FS_NO_ALT_DRIVE          0x00000001
#define FS_ONLY_ALT_SERIAL       0x00000002
#define FS_DRIVE_I_SYSTEM        0x00000004
#define FS_DRIVE_V_NORMAL        0x00000008
#define FS_DRIVE_V_REMOVABLE     0x00000010

#if defined(DUAL_TFLASH_SUPPORT)
#define FS_DRIVE_TFCARD_SECOND   0x00000020
#endif

#if 1 //add gdm 080401
#define FS_DRIVE_V_NANDFLASH     0x00000020
#endif

//FS_GeneralFormat Parameter
#define FS_FORMAT_HIGH           0x00000001
#define FS_FORMAT_LOW            0x00000002

#define FS_FMT_SINGLE_FAT        0x00000100L
#define FS_FMT_FORCE_LOW_LEVEL   0x00000200L
#define FS_FMT_NO_LOW_LEVEL      0x00000400L
#define FS_FMT_GET_DATA_SECTOR   0x00000800L
#define FS_FMT_FAT_12            0x00001000L
#define FS_FMT_FAT_16            0x00002000L
#define FS_FMT_FAT_32            0x00004000L
#define FS_FMT_NO_FAT_32         0x00008000L

#define FS_FLASH_NO_HIGH_FMT     0x20000000L
#define FS_FLASH_NO_LOW_FMT      0x40000000L

//FS_Move Parameter
#define FS_MOVE_COPY             0x00000001
#define FS_MOVE_KILL             0x00000002
#define FS_MOVE_OVERWRITE        0x00000004

//FS_Count/FS_XDelete/Sort Parameter for RecFolderAction
#define FS_FILE_TYPE             0x00000004     //0x00000001 & 2 is reserved for internal
#define FS_DIR_TYPE              0x00000008
#define FS_RECURSIVE_TYPE        0x00000010
#define FS_NO_SORT               0x00000020     //Sort Parameter
#define FS_SORT_NAME             0x00000040     //Sort Parameter
#define FS_SORT_SIZE             0x00000080     //Sort Parameter
//#define FS_SORT_ATTR             0x00000100     //Sort Parameter
#define FS_SORT_TYPE             0x00000200     //Sort Parameter
#define FS_SORT_TIME             0x00000400     //Sort Parameter
#if 1//support sort time by descending 
#define FS_SORT_DTIME             0x00000100     //Sort Parameter
#endif
#define FS_COUNT_SIZE            0x00000800     //QM internal for RecFolderAction
#define FS_REMOVE_CHECK          0x00001000     //QM internal for RecFolderAction
#define FS_FILTER_SYSTEM_ATTR    0x00002000     //FS_Count only
#define FS_REC_COPYRIGHT_DEL     0x00004000     //FS_SweepCopyrightFile only
#define FS_REC_COPYRIGHT_LIST    0x00008000     //FS_GetCopyrightList only

//FSSetTrace Parameter
#define FS_TRACE_API             0x00000001
#define FS_TRACE_ERROR           0x00000002
#define FS_TRACE_DISABLE         0x00000004
#define FS_TRACE_ALL_TASK        0x00000008

//Quota Management
#define FS_QMAX_NO_LIMIT         0xf1f2f3f4 //~3.8GB
#define FS_COUNT_IN_BYTE         0x00000001
#define FS_COUNT_IN_CLUSTER      0x00000002

//FS_Abort Parameter
#define FS_ABORT_CHECK_DRIVE     0x12345678  
#define FS_ABORT_XDELETE         0x87654321
 
/*-------------------- Type Define --------------------*/
#if defined(_WIN32) || defined(WIN32) || defined(GEN_FOR_PC)
#define ONE_BYTE_ALIGN_ADS
#else
//#define ONE_BYTE_ALIGN_ADS	__packed    // zrx del 20060522
#define ONE_BYTE_ALIGN_ADS
#endif 
 
#if !defined(__AMNT__) && !defined(MMI_ON_WIN32)
//typedef unsigned int	DWORD;   // zrx del 20060524
#endif  /* !__AMNT__ && !MMI_ON_WIN32 */
    
 //typedef unsigned short WORD;     // zrx del 20060524
 //typedef unsigned int   UINT;    // zrx del 20060522
//typedef unsigned char  BYTE;    // zrx del 20060524
//typedef void * HANDLE;              // zrx del 20060524

typedef long FS_HANDLE;
typedef long FS_INT32;
typedef void (* FS_FormatCallback)(const char * DeviceName, int Action, UINT Total, UINT Completed);
typedef int FS_ProgressCallback(int Action, UINT Total, UINT Completed, UINT Info);

 
typedef  struct 

{
   unsigned int Second2:5;
   unsigned int Minute:6;
   unsigned int Hour:5;
   unsigned int Day:5;
   unsigned int Month:4;
   unsigned int Year1980:7;
} FS_DOSDateTime;

typedef  struct 
{
   char           FileName[8];   // It is unused nowadays.
   char           Extension[3];  // It is unused nowadays.
   BYTE           CreateTimeTenthSecond;
   FS_DOSDateTime CreateDateTime;
   WORD           LastAccessDate;
   WORD           FirstClusterHi;
   FS_DOSDateTime DateTime;
   WORD           FirstCluster;
   UINT          FileSize;
   UINT           Attributes;
   BYTE           NTReserved;
}FS_DOSDirEntry;

typedef struct 
{
   UINT Cluster;
   UINT Index;
}FS_FileLocationHint;

typedef struct 
{
   const FS_DOSDirEntry * DirEntry;
   UINT                  FilePos;
   UINT                  AllocatedSize;
   UINT                  ClusterChains;
   UINT                  VolumeSerialNumber;
   const char           * FullName;
}FS_FileInfo;

typedef struct 
{
   char            Label[24];
   char            DriveLetter;
   BYTE            WriteProtect;
   char            Reserved[2];
   UINT            SerialNumber;
   UINT            FirstPhysicalSector;
   UINT            FATType;
   UINT            FATCount;
   UINT            MaxDirEntries;
   UINT            BytesPerSector;
   UINT            SectorsPerCluster;
   UINT64        TotalClusters;
   UINT            BadClusters;
   UINT64        FreeClusters;
   UINT            Files;
   UINT            FileChains;
   UINT            FreeChains;
   UINT            LargestFreeChain;
}FS_DiskInfo;

//For Driver START
typedef ONE_BYTE_ALIGN_ADS struct 
{
   BYTE  BootIndicator,
         StartHead,
         StartSector,
         StartTrack,
         OSType,
         EndHead,
         EndSector,
         EndTrack;
   UINT  RelativeSector,
         Sectors;
} FS_PartitionRecord;

typedef ONE_BYTE_ALIGN_ADS struct 
{
   BYTE BootCode[512-4*sizeof(FS_PartitionRecord)-sizeof(WORD)];
   FS_PartitionRecord PTable[4];
   WORD Signature;
} FS_MasterBootRecord;

typedef ONE_BYTE_ALIGN_ADS struct 
{
   BYTE     PhysicalDiskNumber;
   BYTE     CurrentHead;
   BYTE     Signature;
   UINT     SerialNumber;
   BYTE     Label[11];
   char     SystemID[8];
}FS_ExtendedBIOSParameter;

typedef ONE_BYTE_ALIGN_ADS struct 
{
   char     OEMName[8];
   WORD     BytesPerSector;
   BYTE     SectorsPerCluster;
   WORD     ReservedSectors;
   BYTE     NumberOfFATs;
   WORD     DirEntries;
   WORD     SectorsOnDisk;
   BYTE     MediaDescriptor;
   WORD     SectorsPerFAT;
   WORD     SectorsPerTrack;
   WORD     NumberOfHeads;
   UINT     NumberOfHiddenSectors;
   UINT     TotalSectors;
   ONE_BYTE_ALIGN_ADS union 
   {
      ONE_BYTE_ALIGN_ADS struct 
      {
         FS_ExtendedBIOSParameter BPB;
      } _16;
      ONE_BYTE_ALIGN_ADS struct 
      {
         UINT     SectorsPerFAT;
         WORD     Flags;
         WORD     Version;
         UINT     RootDirCluster;
         WORD     FSInfoSector;
         WORD     BackupBootSector;
         BYTE     Reserved[12];
         FS_ExtendedBIOSParameter BPB;
      } _32;
   } E;
}FS_BIOSParameter;

typedef ONE_BYTE_ALIGN_ADS struct 
{
   BYTE NearJump[3];
   FS_BIOSParameter BP;
   BYTE BootCode[512-3-sizeof(FS_BIOSParameter)-sizeof(WORD)];
   WORD Signature;
}FS_BootRecord;

typedef struct fsdriver 
{
   int  (* MountDevice)    (void * DriveData, int DeviceNumber, int DeviceType, UINT Flags);
   int  (* ShutDown)       (void * DriveData);
   int  (* ReadSectors)    (void * DriveData, UINT Sector, UINT Sectors, void * Buffer);
   int  (* WriteSectors)   (void * DriveData, UINT Sector, UINT Sectors, void * Buffer);
   int  (* MediaChanged)   (void * DriveData);
   int  (* DiscardSectors) (void * DriveData, UINT Sector, UINT Sectors);
   int  (* GetDiskGeometry)(void * DriveData, FS_PartitionRecord * DiskGeometry, BYTE * MediaDescriptor);
   int  (* LowLevelFormat) (void * DriveData, const char * DeviceName, FS_FormatCallback Progress, UINT Flags);
   int  (* NonBlockWriteSectors) (void * DriveData, UINT Sector, UINT Sectors, void * Buffer);
   int  (* RecoverableWriteSectors) (void * DriveData, UINT Sector, UINT Sectors, void * Buffer);
   int  (* ResumeSectorStates) (void  * DriveData);
   int  (* HighLevelFormat) (void);
   int  (* FlushData) (void * DriveData);
}FS_Driver;

 
#ifdef MMI_ON_WIN32    
typedef struct _fsMutex
{
   kal_semid   mt_sem;
   kal_taskid	mt_sem_owner;
   kal_uint32	mt_lock_count;
   int         DeviceNum_1;
   int         DeviceNum_2;
}FS_Mutex;

#else

typedef UINT FS_Mutex;

#endif  /* MMI_ON_WIN32 */

struct _fsDrive;
struct _fsBuffer;

typedef struct _fsDeviceData{
   FS_Mutex         * Lock;
   struct _fsDrive  * FirstDrive;
   BYTE               MediaPresent;
   BYTE               MediaRemovedReported;
   BYTE               Reserved;
   BYTE               MountState;
   int                SectorSize;
   FS_PartitionRecord Geometry;
   UINT               SectorShift;
   UINT               AccessCount;
   struct _fsBuffer * B; 
   struct _fsBuffer * AltBuffer;
   int                ErrorCondition; 
   int                PhysicalDiskIndex;
   char               FriendlyName[7];
   BYTE               MediaDescriptor;
}FS_DeviceData;

typedef struct _fsDevice 
{
   int                DeviceType;
   int                DeviceNumber;
   UINT               DeviceFlags;
   FS_Driver        * Driver;
   void             * DriverData;
   FS_DeviceData      DevData;
}FS_Device;

typedef struct _fsDirEntry{
   UINT                 DirCluster;
   FS_FileLocationHint  LongPos;
   FS_FileLocationHint  ShortPos;
   FS_DOSDirEntry       Dir;
}FS_DirEntry;

typedef struct _fsDrive 
{
   struct _fsDevice *   Dev;
   struct _fsDrive  *   NextDrive;
   int                  MountState;
   FS_PartitionRecord   Geometry;
   UINT                 Clusters;
   UINT                 SectorsPerCluster;
   UINT                 ClusterSize;
   UINT                 ClusterShift;
   UINT                 SPerCShift;
   UINT                 FATType;
   UINT                 FATCount;
   UINT                 SectorsPerFAT;
   UINT                 ClusterWatermark;
   UINT                 FreeClusterCount;
   UINT                 InfoSector;
   UINT                 SerialNumber;
   UINT                 FirstSector;
   UINT                 FirstFATSector;
   UINT                 FirstDirSector;
   UINT                 FirstDataSector;
   UINT                 RootDirEntries;
   FS_DirEntry          CurrDirEntry;
   char                 CurrDir[FS_MAX_PATH];
#ifdef __FS_QM_SUPPORT__   
   BYTE                 QuotaMgt;
   BYTE                 Reserved[3];
#endif   
}FS_Drive;

typedef struct _fsBuffer 
{
   struct _fsBuffer * Next;
   struct _fsBuffer * Prev;
   FS_Device        * Dev;
   BYTE             * Data;
   UINT              Sector;
   struct _fsDrive  * FATSectorInfo;
   UINT              Flags;
   UINT              FirstDirtyTime, 
                      LastDirtyTime;
   int                Num;   
   BYTE               recoverable_flag;   
#define    FS_RECOVERABLE_WRITE     1
#define    FS_NORMAL_WRITE          0
}FS_Buffer;
//For Driver END

typedef struct 
{
   WCHAR Pattern[8];
}FS_Pattern_Struct;

typedef struct __fsQuotaStruct
{
   BYTE        Path[FS_MAX_PATH/2];
   UINT        Priority;
   UINT        Qmin;
   UINT        Qmax;
   UINT        Uint;
}FS_QuotaStruct;

typedef struct __fsInternQStruct
{
   UINT        Qmin;
   UINT        Qmax;
   UINT        Qnow;
}FS_InternQStruct;

typedef struct __fsFile
{
   FS_Drive             *  Drive; 
   FS_Device            *  Dev;
   int                     SpecialKind;
   UINT                    Unique;
   UINT                    Flags;
   FS_DirEntry             DirEntry;
   UINT                    FilePointer;
   UINT                    LastCluster;
   UINT                    Cluster;
   UINT                    Offset;
   char                    FullName[FS_MAX_PATH];
   void                 *  Task; 
   UINT                    HintNum;
   FS_FileLocationHint  *  Seek_Hint; 
   BYTE                    Lock;
   BYTE                    Valid;
   BYTE                    Reserved[sizeof(int) - 2*sizeof(BYTE)];
}FS_File;

typedef struct __fsTables
{
   FS_Drive  * DriveTable;
   FS_File   * FileTable;
   FS_Buffer * BufferTable;
   BYTE      * BufferData;
}FS_Tables;

typedef struct __fsExceptionData
{
   UINT      * Next;
   int         Data[11];
   int         XValue;
   int         State;
   int         IsHandled;   
}FS_ExceptionData;

/*------------------------- ENUM TYPE -----------------------*/
//For Driver only
typedef enum
{
   FS_DEVICE_UNKNOWN,
   FS_DEVICE_FLOPPY,
   FS_DEVICE_FDISK
}FS_DRIVER_DEVICE_ENUM;

typedef enum
{	
   FS_NO_WAIT,
   FS_INFINITE
}FS_TIMEOUT_ENUM;

//FS_Seek Parameter
typedef enum
{
   FS_FILE_BEGIN,
   FS_FILE_CURRENT,
   FS_FILE_END
}FS_SEEK_POS_ENUM;

//FS_GeneralFormat Callback Return Value
typedef enum
{	
   FS_FMT_PGS_DONE = -1,
   FS_FMT_PGS_FAIL,
   FS_FMT_PGS_LOW_FMT,
   FS_FMT_PGS_HIGH_FMT
}FS_FORMAT_PGS_ENUM;

//FS_Move Callback Return Value
typedef enum
{	
   FS_MOVE_PGS_FAIL = -1,
   FS_MOVE_PGS_PREPARE,
   FS_MOVE_PGS_START,
   FS_MOVE_PGS_ING,
   FS_MOVE_PGS_DONE
}FS_MOVE_PGS_ENUM;

//Find Series Return Value
typedef enum
{	
   FS_NOT_MATCH,
   FS_LFN_MATCH,
   FS_SFN_MATCH
}FS_FIND_ENUM;

//FS_GetDevType Return Value
typedef enum
{	
  FS_DEVICE_TYPE_NOR = 1,
  FS_DEVICE_TYPE_NAND,
  FS_DEVICE_TYPE_CARD
}FS_DEVICE_TYPE_ENUM;

//FS_XFindStart, FS_FindFirstN, FS_FindNextN Parameter
typedef enum
{
   FS_FIND_DEFAULT,
   FS_FIND_LFN_TRUNC
}FS_GET_NAME_ENUM;

//Quota management
typedef enum
{
   FS_QP_HIG_ENUM,
   FS_QP_MID_ENUM,
   FS_QP_LOW_ENUM,
   FS_QP_DEL_ENUM
}FS_QP_ENUM;

//FS_GetDevStatus Parameter
typedef enum
{ 
   FS_MOUNT_STATE_ENUM,
   FS_FEATURE_STATE_ENUM
}FS_GET_DEV_STATUS_ENUM;

//FS_LockFAT Parameter
typedef enum
{
   FS_LOCK_USB_ENUM,
   FS_LOCK_BLOCK_ENUM,
   FS_LOCK_NONBLOCK_ENUM
}FS_LOCK_TYPE_ENUM;

#endif //_FS_TYPE_H

