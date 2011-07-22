
//FFS
/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2005, CII Technologies, Inc.               */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* This source code is property of CII-tech. The information contained in*/
/* this file is confidential. Distribution, reproduction, as well as          */
/* exploitation,or transmisison of any content of this file is not            */
/* allowed except if expressly permitted.                                     */
/* Infringements result in damage claims!                                     */
/*                                                                            */
/* FILE NAME                                                                  */
/*      fs.h                                                                  */
/*                                                                            */
/* Support Fs type                                                            */
/*        FFS                                                                 */
/* Support device                                                             */
/*        Flash                                                               */
/* DESCRIPTION                                                                */
/*    FS service declarations, constant definitions and macros.               */
/*    The compile switch is                                                   */
/*         #if !(defined(_CSW_VER_1_)|| defined(_MS_VC_VER_))                 */
/******************************************************************************/

//FS
/******************************************************************************/
/*                                                                            */
/*              Copyright (C) 2006~2007, Coolsand Technologies, Inc.          */
/*                            All Rights Reserved                             */
/*                                                                            */
/******************************************************************************/
/* This source code is property of Coolsand-tech.                             */
/* modified the api define and supported file system type.                    */
/* Support Fs type                                                            */
/*        FAT                                                                 */     
/* Support device                                                             */
/*        Flash                                                               */
/*        T-Flash                                                             */
/* DESCRIPTION                                                                */
/*    The compile switch is                                                   */
/*           #if (defined(_CSW_VER_1_)|| defined(_MS_VC_VER_))                */ 
/*                                                                            */
/******************************************************************************/

#ifndef __FS_H__
#define __FS_H__

#include "cs_types.h"

// The FS API define begin.
#if (defined(_CSW_VER_1_)|| defined(_MS_VC_VER_))

// Open flag define
#define FS_O_RDONLY	            0       // Read only.
#define FS_O_WRONLY	            1       // Write only.
#define FS_O_RDWR	            2       // Read and Write. 
#define FS_O_ACCMODE	            3       // Access. 

#define FS_O_CREAT		    00100   // If the file exists, this flag has no effect except as noted under FS_O_EXCL below. Otherwise, the file shall be created.
#define FS_O_EXCL		    00200   // If FS_O_CREAT and FS_O_EXCL are set, the function shall fail if the file exists.
#define FS_O_TRUNC		    01000   // If the file exists, and is a regular file, and the file is successfully opened FS_O_WRONLY or FS_O_RDWR, its length shall be truncated to 0.
#define FS_O_APPEND	            02000   // If set, the file offset shall be set to the end of the file prior to each write.

#define FS_SEEK_SET                 0       // Seek from beginning of file.
#define FS_SEEK_CUR                 1       // Seek from current position.
#define FS_SEEK_END                 2       // Set file pointer to EOF plus "offset"

 
// 用16bit来记录inode的mode。
// 其中15-12bit用于记录inode的type。
//
#define INODE_TYPE_BITMASK     0xF000     
#define INODE_TYPE_UNKNOWN	   0x0000      
#define INODE_TYPE_REGULAR     0x8000     
#define INODE_TYPE_DIRECTORY   0x4000     
#define FS_INODE_IS_REG(m)	   (((m) & INODE_TYPE_BITMASK) == INODE_TYPE_REGULAR)
#define FS_INODE_IS_DIR(m)	   (((m) & INODE_TYPE_BITMASK) == INODE_TYPE_DIRECTORY)
#define FS_INODE_GET_TYPE(m)	((m) & INODE_TYPE_BITMASK)

#define FS_FILE_NAME_LEN       255 // Folder or File name size in byts, to support fat long file name.
#define FS_PATH_LEN            260 // max path length,to support fat long file name.

#define FS_FILE_NAME_UNICODE_LEN   (2*FS_FILE_NAME_LEN) // max folder or File name size in byts for uincode.
#define FS_PATH_UNICODE_LEN        (2*FS_PATH_LEN) // max path size for unicode.



#define LEN_FOR_NULL_CHAR               1         // Length of terminated character('\0' for OEM).
#define LEN_FOR_UNICODE_NULL_CHAR       2         // Size of terminated character('\0' for Unicode).

#define FS_TYPE_FAT             0x01      // File system type -- FAT.
#define FS_TYPE_FFS             0x02      // File system type -- FFS.

#define FS_FORMAT_NORMAL        0X01      // format type -- normal(full). 
#define FS_FORMAT_QUICK         0X00      // format type -- quick.

#define FS_POWER_ON             0x01      // fs status is power on.
#define FS_POWER_OFF            0x00      // fs status is power off.

#ifndef PCWSTR
#define PCWSTR UINT16*
#endif

#ifndef PWSTR
#define PWSTR UINT16*
#endif


// FIXME WCHAR is a cswtype and MUST NOT
// be redefined
//#ifndef WCHAR
//#define WCHAR UINT16
//#endif

// The file attribute struct.
typedef struct _FS_FILE_ATTR 
{
    UINT32  i_no;         // inode number 
    UINT32  i_size;       // For regular files, the file size in bytes
    UINT16  i_mode;       // Mode of file 
    UINT16  i_count;      // has opened count. 
    UINT32  i_atime;      // Time of last access to the file
    UINT32  i_mtime;      // Time of last data modification
    UINT32  i_ctime;      // Time of last status(or inode) change    
}FS_FILE_ATTR;

// Storage information.
typedef struct _FS_INFO
{
	UINT32 iTotalSize;    // Total size
    UINT32 iUsedSize;     // Has used  size 
}FS_INFO;

// Find file information.
typedef struct _FS_FIND_DATA
{
    UINT16  st_mode;       // Mode of file 
    UINT32  st_size;       // For regular files, the file size in bytes
    UINT32  st_atime;      // Time of last access to the file
    UINT32  st_mtime;      // Time of last data modification
    UINT32  st_ctime;      // Time of last status(or inode) change
    WCHAR st_name[ FS_FILE_NAME_LEN + LEN_FOR_NULL_CHAR ]; // The name of file. 
}FS_FIND_DATA,*PFS_FIND_DATA;

INT32 FS_HasFormatted(
    PCSTR pszDevName, 
    UINT8 iFsType
);

INT32 FS_Format(
    PCSTR pszDevName, 
    UINT8 iFsType, 
    UINT8 iFlag
);

INT32 FS_MountRoot(
    PCSTR pszRootDevName 
);

INT32 FS_Mount
( 
    PCSTR pszDevName, 
    PCWSTR pszMountPoint, 
    INT32 iFlag, 
    UINT8 iType 
);

INT32 FS_Unmount( 
    PCWSTR pszMountPoint,
    BOOL  bForce
);

INT32 FS_Open(
    PCWSTR pszFileName, 
    UINT32 iFlag, 
    UINT32 iMode
);
    
INT32 FS_Create(
    PCWSTR pszFileName, 
    UINT32 iMode
);

INT32 FS_Close(
    INT32 iFd
);

INT32 FS_Read(
    INT32 iFd, 
    UINT8 *pBuf, 
    UINT32 iLen
);

INT32 FS_Write(
    INT32 iFd, 
    UINT8 *pBuf, 
    UINT32 iLen
);

INT32 FS_Delete(
    PCWSTR pszFileName
);

INT32 FS_Seek(
    INT32 iFd, 
    INT32 iOffset, 
    UINT8 iOrigin
);

INT32 FS_MakeDir(
    PCWSTR pszDirName, 
    UINT32 iMode
);

INT32 FS_RemoveDir(
    PCWSTR pszDirName
);

INT32 FS_FindFirstFile( 
    PCWSTR pszFileName, 
    PFS_FIND_DATA  pFindData
);

INT32 FS_FindNextFile(
    INT32 iFd, 
    PFS_FIND_DATA pFindData
);

INT32 FS_FindClose(
    INT32 iFd
);

INT32 FS_GetFileAttr(
    PCWSTR     pszFileName,
    FS_FILE_ATTR* pFileAttr
);

INT32 FS_GetFSInfo(
    PCSTR pszDevName,
    FS_INFO* psFSInfo
);

INT32 FS_IsEndOfFile(
    INT32 iFd
);

INT32 FS_Rename( 
    PCWSTR pszOldname, 
    PCWSTR pszNewname 
);

INT32 FS_ChangeDir( PCWSTR pszDirName );

INT32 FS_GetCurDir(UINT32 iSize, PWSTR pszCurDir);

INT32 FS_ChangeSize(INT32 fd, UINT32 nFileSize);

INT32 FS_Flush(INT32 fd);

INT32 FS_GetFileName(INT32 iFd, INT32 iNameBufferSize, PWSTR pcFileName);

INT32 FS_PowerOn(VOID);

INT32 FS_PowerOff(VOID);

// Error code
/******************************************************************************/
// The named file is a directory. 
// The functions that use it include: FS_Open();FS_Delete().
#define ERR_FS_IS_DIRECTORY                              -4200001
/******************************************************************************/

/******************************************************************************/
// The named file is not a directory. 
// The functions that use it include: FS_Mount();FS_Unmount();FS_ChangeDir();
// FS_RemoveDir().
#define ERR_FS_NOT_DIRECTORY                             -4200002
/******************************************************************************/

/******************************************************************************/
// A component of the path prefix does not name an existing directory or path is 
// an empty string.
// The functions that use it include: FS_Rename();FS_Open();FS_Delete();
// FS_RemoveDir().
#define ERR_FS_NO_DIR_ENTRY                              -4200003
/******************************************************************************/

/******************************************************************************/
// Operation is not granted.This error code Generally return when the count field of 
// inode not equal the prescriptive value or the  flag field of file is FS_O_FIND.
// The function that use it inlude:FS_Rename();FS_Mount();FS_Unmount();FS_Delete();
// FS_Seek();FS_Seek();FS_IsEndOfFile();FS_Read();FS_Write();FS_RemoveDir();
// FS_ChangeSize();FS_Flush();FS_GetFileSize();FS_GetFileName().
// 
#define ERR_FS_OPERATION_NOT_GRANTED                     -4200005
/******************************************************************************/

/******************************************************************************/
// Directory is not empty.
// The function that use it inlude:FS_Rename();FS_RemoveDir().
#define ERR_FS_DIR_NOT_EMPTY                             -4200006
/******************************************************************************/

/******************************************************************************/ 

// No file descriptor is available.
// The function that use it inlude:FS_FindFirstFile();FS_Open().
#define ERR_FS_FDS_MAX                                   -4200007
/******************************************************************************/

/******************************************************************************/
//open files too many for a process
#define ERR_FS_PROCESS_FILE_MAX                          -4200008
/******************************************************************************/

/******************************************************************************/
// File has existed.
// The function that use it inlude:FS_Rename();FS_Open();FS_MakeDir().
#define ERR_FS_FILE_EXIST                                -4200009
/******************************************************************************/

/******************************************************************************/
// The base file name(or directory name) length is zero in the path string. 
// The function that use it inlude:FS_FindFirstFile();FS_Rename();FS_Open();
// FS_Delete();FS_MakeDir();FS_RemoveDir().
#define ERR_FS_NO_BASENAME                               -4200011
/******************************************************************************/

/******************************************************************************/
// Bad file descriptor or find descriptor.
// The function that use it inlude:FS_FindNextFile();FS_IsEndOfFile();
// FS_ChangSize();FS_Flush();FS_Close();FS_FindClose();FS_Seek();FS_Write();
// FS_Read();FS_GetFileName().
#define ERR_FS_BAD_FD                                    -4200012
/******************************************************************************/

/******************************************************************************/
// No more files in the specifiyed directory.
// The function that use it inlude:FS_FindFirstFile();FS_FindNextFile();
#define ERR_FS_NO_MORE_FILES                            -4200013
/******************************************************************************/

/******************************************************************************/
// The device has mounted.
// The function that use it inlude:FS_Mount().
#define ERR_FS_HAS_MOUNTED                               -4200014
/******************************************************************************/

/******************************************************************************/
// The count of mounted device more than the max mount count.
// The function that use it inlude:FS_Mount().
#define ERR_FS_MOUNTED_FS_MAX                           -4200015
/******************************************************************************/

/******************************************************************************/
// The FS type is not support.when the file type not FAT type,return this error
// code.
// The function that use it inlude:FS_HasFormated();FS_GetFsInfo();FS_Mount();
// FS_Open();FS_Delete();FS_Read();FS_Write();FS_MakeDir();FS_RemoveDir();
// FS_ScanDisk();FS_ChangeSize();FS_Flush();FS_GetFileName().
#define ERR_FS_UNKNOWN_FILESYSTEM                       -4200016
/******************************************************************************/

/******************************************************************************/
// The dirctory is invalid.when the mount point not found,this error code will
// return.
// The function that use it inlude:FS_Mount();FS_Unmount().
#define ERR_FS_INVALID_DIR_ENTRY                       -4200018
/******************************************************************************/

/******************************************************************************/
// The input parameter is invalid.
// The function that use it inlude:FS_Format();FS_FindFirstFile();FS_GetFileAttr();
// FS_GetCurDir();FS_Rename();FS_Rename();FS_MountRoot();FS_Mount();FS_Unmount();
// FS_ChangeDir();FS_Open();FS_Delete();FS_Seek();FS_Read();FS_Write();FS_MakeDir();
// FS_RemoveDir().
#define ERR_FS_INVALID_PARAMETER                       -4200019
/******************************************************************************/

/******************************************************************************/
// The FS type not support.
// The function that use it inlude:FS_Flush();FS_HasFormated().
#define ERR_FS_NOT_SUPPORT							   -4200020
/******************************************************************************/

/******************************************************************************/
// Malloc failed.
// The function that use it inlude all function.
#define ERR_FS_NO_MORE_MEMORY				    	   -4200025
/******************************************************************************/

/******************************************************************************/
// The device is unregister.
// The function that use it inlude:FS_HasFormated();FS_Format();FS_GetFSInfo();
// FS_MountRoot();FS_Mount();FS_ScanDisk().
#define ERR_FS_DEVICE_NOT_REGISTER	     			   -4200027

/******************************************************************************/
// No more inode to use.
// The function that use it inlude:FS_Create();FS_Open();FS_MakeDir();
// FS_ScanDisk().
#define ERR_FS_NO_MORE_INODE                           -4200029
/******************************************************************************/

/******************************************************************************/
// The disk is full,no more space to use.
// The function that use it include: FS_Write();FS_Create();FS_Open();
// FS_MakeDir();FS_ChangeSize().
#define ERR_FS_DISK_FULL                               -4200030
/******************************************************************************/

/******************************************************************************/
// The device is not format.
// The function that use it include: FS_HasFormated().
#define ERR_FS_NOT_FORMAT							   -4200032
/******************************************************************************/

/******************************************************************************/
// The device has formated.
// The function that use it include: FS_HasFormated().
#define ERR_FS_HAS_FORMATED							   -4200033
/******************************************************************************/

/******************************************************************************/
// Can't find the supper block.
// The function that use it include: FS_Unmount().
#define ERR_FS_UNMOUNT_NOT_FIND_SB     				   -4200035
/******************************************************************************/

/******************************************************************************/
// The device is busy,can't unmount.
// The function that use it include: FS_Unmount().
#define ERR_FS_UNMOUNT_FS_BUSY     					   -4200037
/******************************************************************************/

/******************************************************************************/
// The device is wrong,can't open.
// The function that use it include: FS_HasFormated();FS_Format();FS_Unmount();
// FS_Mount();FS_ScanDisk().
#define ERR_FS_OPEN_DEV_FAILED     					   -4200038
/******************************************************************************/

/******************************************************************************/
// The count more than the max count of root entry count,that dir entry in the
// root dirctory.
// The function that use it include: FS_Open();FS_Create();FS_MakeDir();FS_Rename().
#define ERR_FS_ROOT_FULL       					       -4200039
/******************************************************************************/

/******************************************************************************/
// Access the reg failed.
// The function that use it include: FS_PowerOn();FS_PowerOff().
#define ERR_FS_ACCESS_REG_FAILED       				   -4200040
/******************************************************************************/

/******************************************************************************/
// Parse the path name error.
// The function that use it include: FS_FindFirstFile();FS_GetFileAttr();
// FS_GetCurDir();FS_Rename();FS_ChangeDir();FS_Open();FS_Delete();FS_MakeDir();
// FS_RemoveDir().
#define ERR_FS_PATHNAME_PARSE_FAILED                    -4200041
/******************************************************************************/

/******************************************************************************/
// Get the dir entry failed in the specifiy dirctory.
// The function that use it include: FS_FindFirstFile().
#define ERR_FS_READ_DIR_FAILED                          -4200048
/******************************************************************************/

/******************************************************************************/
// Get root inode failed.
// The function that use it include: FS_MountRoot();FS_Mount().
#define ERR_FS_MOUNT_READ_ROOT_INODE_FAILED				-4200050
/******************************************************************************/

/******************************************************************************/
// The device number is invalid.
// The function that use it include: FS_MountRoot().
#define ERR_FS_UNMOUNT_INVALID_DEV_NUMBER               -4200051
/******************************************************************************/

/******************************************************************************/
// Move directory is not permitted
// The function that use it include: FS_Rename().
#define ERR_FS_REMOVE_DIRECTORY_BY_RENAME               -4200052
/******************************************************************************/

/******************************************************************************/
// The device is mounted,when format option is begin.
// The function that use it include: FS_Format().
#define ERR_FS_FORMAT_MOUNTING_DEVICE                   -4200053
/******************************************************************************/

/******************************************************************************/
#define ERR_FS_FORMAT_ROOT_DEV_HAS_MOUNTPOINT           -4200054
/******************************************************************************/


/******************************************************************************/
// The boot information is destroy on the specifiy device.
// The function that use it include: FS_Format() FS_HasFormated();FS_Mount();
// FS_MountRoot().
#define ERR_FS_DATA_DESTROY                                -4200056 
/******************************************************************************/

/******************************************************************************/
// Read the FAT entry failed.
// The function that use it include: FS_Open(); FS_Create();FS_MakeDir();
// FS_Write(); FS_Read();FS_ChangeSize();FS_RemoveDir();FS_Delete();FS_Rename();
// FS_ScanDisk();FS_HasFormate();FS_MountRoot().
#define ERR_FS_READ_SECTOR_FAILED                          -4200057
/******************************************************************************/


/******************************************************************************/
// Write the FAT entry failed.
// The function that use it include: FS_Open(); FS_Create();FS_MakeDir();
// FS_Write(); FS_ChangeSize();FS_RemoveDir();FS_Delete();FS_Rename();
// FS_ScanDisk();.
#define ERR_FS_WRITE_SECTOR_FAILED                         -4200058  
/******************************************************************************/

/******************************************************************************/
// Read the file exceed the end of file.
// The function that use it include: FS_Read(); 
#define ERR_FS_READ_FILE_EXCEED                            -4200059
/******************************************************************************/

/******************************************************************************/
// The pos exceed the end of file when write a file.
// The function that use it include: FS_Write(); 
#define ERR_FS_WRITE_FILE_EXCEED                           -4200060
/******************************************************************************/

/******************************************************************************/
// The sample long name too more in a dirctory,so can't make a short file as 
// "fo~xxx ext".the max count is 999 in a dirctory.
// The function that use it include: FS_MakeDir();FS_Create();FS_Open(). 
#define ERR_FS_FILE_TOO_MORE                               -4200061 
/******************************************************************************/

/******************************************************************************/
// The file not exist.
// The function that use it include: FS_Open();FS_Delete();FS_GetFileAttr();
// FS_Rename(). 
#define ERR_FS_FILE_NOT_EXIST                              -4200062
/******************************************************************************/

/******************************************************************************/
// Try to rename a file to another device.
// The function that use it include: FS_Rename().
#define ERR_FS_DEVICE_DIFF                                 -4200063
/******************************************************************************/

/******************************************************************************/
// Get the device information  failed.
// The function that use it include: FS_Format().
#define ERR_FS_GET_DEV_INFO_FAILED                         -4200064
/******************************************************************************/

/******************************************************************************/
// No more supper block item.
// The function that use it include: FS_MountRoot();FS_Mount().
#define ERR_FS_NO_MORE_SB_ITEM                             -4200065 
/******************************************************************************/


/******************************************************************************/
// The device not mount.
// The function that use it include:FS_GetFSInfo().
#define ERR_FS_NOT_MOUNT                                   -4200066 
/******************************************************************************/

#define ERR_FS_NAME_TOO_LONG                               -4200067 

/******************************************************************************/
// The buffer too short to save a file name.
// The function that use it include:FS_GetCurDir();FS_GetFileName().
#define ERR_FS_NAME_BUFFER_TOO_SHORT                       -4200068
/******************************************************************************/

/******************************************************************************/
// The inode not reguler.
// The function that use it include:FS_Read();FS_Write();FS_GetFileSize().
#define ERR_FS_NOT_REGULAR       		    			   -42000100
/******************************************************************************/
// The fs-api define end.
// The ffs-api define begin.
#else  

#define FS_O_RDONLY	            0
#define FS_O_WRONLY	            1
#define FS_O_RDWR	            2
#define FS_O_ACCMODE	        3

#define FS_O_CREAT		        00100
#define FS_O_EXCL		        00200
#define FS_O_TRUNC		        01000
#define FS_O_APPEND	            02000

#define FS_SEEK_SET             0       // Seek from beginning of file.
#define FS_SEEK_CUR             1       // Seek from current position.
#define FS_SEEK_END             2       // Set file pointer to EOF plus "offset"

// for Flash File System Query 
#define FS_Q_FREE_SECTORS       1    //2 bytes
#define FS_Q_FLASH_SIZE         2    //4 bytes
#define FS_Q_FREE_SIZE          3    //4 bytes
#define FS_Q_USED_SIZE          4    //4 bytes
#define FS_Q_LOST_SIZE          5    //4 bytes
#define FS_Q_PATH_DEPTH_MAX     6    //2 bytes        
#define FS_Q_INODES_MAX         7    //2 bytes
#define FS_Q_DEV_MANUFACTURER   8    //2 bytes
#define FS_Q_FS_SIZE_IN_FALSH   9    // 4 bytes
#define FS_Q_FS_OFFSET_IN_FALSH 10   // 4 bytes
// for Flash File System Query 

#define FS_FILE_NAME_LEN        31 // Folder or File name size in byts.
#define LEN_FOR_NULL_CHAR       1

// 
// 用16bit来记录inode的mode。
// 其中15-12bit用于记录inode的type。
//
#define INODE_TYPE_BITMASK     0xF000     //0170000
#define INODE_TYPE_UNKNOWN	   0x0000     // 
#define INODE_TYPE_REGULAR     0x8000     //0100000
#define INODE_TYPE_DIRECTORY   0x4000     //040000
#define FS_INODE_IS_REG(m)	   (((m) & INODE_TYPE_BITMASK) == INODE_TYPE_REGULAR)
#define FS_INODE_IS_DIR(m)	   (((m) & INODE_TYPE_BITMASK) == INODE_TYPE_DIRECTORY)

typedef BOOL (*PFN_CALLBACK_FIND_FILE)( PCSTR pszFileName, PVOID pUserData );

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
}FS_STAT, FS_FIND_DATA, *PFS_FIND_DATA;

BOOL FS_Format( 
    PCSTR pszName
); // C: D: ,,,, 
    
BOOL FS_FormatWithoutEraseFlash( 
    PCSTR pszName
); // C: D: ,,,, 

HANDLE FS_Open( 
    PCSTR pszFileName,
    INT32 nFlag, 
    INT32 nMode 
);

HANDLE FS_Create(
    PCSTR pszFileName,
    INT32 nMode 
);

BOOL  FS_Close( 
    HANDLE hFile
);

INT32 FS_Read( 
    HANDLE hFile, 
    PSTR pBuf, 
    INT32 nLen
);

INT32 FS_Write( 
    HANDLE hFile, 
    PSTR pBuf, 
    INT32 nLen
);

BOOL FS_Link(
    PCSTR pszOldName, 
    PCSTR pszNewName
);

BOOL FS_UnLink( 
    PCSTR pszFileName
);

BOOL FS_Rename( 
    PCSTR pszOldName,
    PCSTR pszNewName
);

INT32 FS_Seek( 
    HANDLE hFile, 
    INT32 nOffset, 
    INT32 nOrigin
);

INT32 FS_GetFileSize( 
    HANDLE hFile
);

BOOL FS_MakeDir( 
    PCSTR pszDirName, 
    INT32 nMode 
);
BOOL FS_RemoveDir( 
    PCSTR pszDirName
);

HANDLE FS_OpenDir( 
    PCSTR pszDirName,
    INT32 nFlag, 
    INT32 nMode 
);

BOOL  FS_CloseDir( 
    HANDLE hFile
);

BOOL FS_ReadDir( 
    PCSTR pszDirName,
    PSTR* pszOutBuf 
);

BOOL FS_ChangeDir(
    PCSTR pszDirName
);

HANDLE FS_FindFirstFile( 
    PCSTR pszFileName, 
    PFS_FIND_DATA  pFindData
);

BOOL FS_FindNextFile( 
    HANDLE hFindFile,
    PFS_FIND_DATA pFindData
);

BOOL FS_FindClose( 
    HANDLE hFindFile 
);

BOOL FS_FindEx (
    PCSTR pszFileName, // c:/*.*, c:/*.txt 
    PVOID DUserata,
    PFN_CALLBACK_FIND_FILE pcbFindFile
);

BOOL FS_Stat ( 
    PCSTR pszPathName, 
    FS_STAT * pStatData
);

BOOL FS_Query( 
    INT8 iQueryIndex, 
    PVOID pBuf 
);
BOOL FS_IsEndOfFile(
    HANDLE hFile );


VOID FS_DEBUG(
    INT32 debug_switch,
    INT32 debug_level
);
#endif
// The ffs-api define end.

#endif // _H_
