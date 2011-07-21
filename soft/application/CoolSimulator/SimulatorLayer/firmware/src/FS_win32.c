//FFS WIN32
#define _WIN32_WINNT 0x0500
#define UNICODE

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "cswtype.h"
#include "fs.h"
#include "errorcode.h"

#include "fs_type.h"
#include "fat_fs.h"
#include "FSSim_def.h"

INT32 SRVAPI FS_HasFormatted(
    PCSTR pszDevName, 
    UINT8 iFsType
)
{
  return ERR_FS_HAS_FORMATED;
}

INT32 SRVAPI FS_Format(
    PCSTR pszDevName, 
    UINT8 iFsType, 
    UINT8 iFlag
){}

INT32 FS_Mount
( 
    PCSTR pszDevName, 
    PCSTR pszMountPoint, 
    INT32 iFlag, 
    UINT8 iType 
)
{
  return ERR_SUCCESS;
}

INT32 SRVAPI FS_Open(
    PCSTR pszFileName, 
    UINT32 iFlag, 
    UINT32 iMode
){}
    
INT32 SRVAPI FS_Close(
    INT32 iFd
){}

INT32 SRVAPI FS_Read(
    INT32 iFd, 
    UINT8 *pBuf, 
    UINT32 iLen
){}

INT32 SRVAPI FS_Write(
    INT32 iFd, 
    UINT8 *pBuf, 
    UINT32 iLen
){}

INT32 SRVAPI FS_Delete(
    PCSTR pszFileName
){}

INT32 SRVAPI FS_Seek(
    INT32 iFd, 
    INT32 iOffset, 
    UINT8 iOrigin
){}

INT32 SRVAPI FS_MakeDir(
    PCSTR pszDirName, 
    UINT32 iMode
)
{
  INT32 err = MMI_FS_CreateDir(pszDirName);
  if (err == FS_NO_ERROR)
  {
    err = ERR_SUCCESS;
  }
  else if (err == FS_FILE_EXISTS)
  {
    err = ERR_FS_FILE_EXIST;
  }
  else
  {
    err = ERR_FS_FAILURE;
  }
  return err;
}

INT32 SRVAPI FS_RemoveDir(
    PCSTR pszDirName
){}

INT32 SRVAPI FS_FindFirstFile( 
    PCSTR pszFileName, 
    PFS_FIND_DATA  pFindData
){}

INT32 SRVAPI FS_FindNextFile(
    INT32 iFd, 
    PFS_FIND_DATA pFindData
){}

INT32 SRVAPI FS_FindClose(
    INT32 iFd
){}

INT32 SRVAPI FS_GetFileAttr(
    PCSTR     pszFileName,
    FS_FILE_ATTR* pFileAttr
){}

INT32 SRVAPI FS_GetFSInfo(
    PCSTR pszDevName,
    FS_INFO* psFSInfo
){}

INT32 SRVAPI FS_IsEndOfFile(
    INT32 iFd
){}

INT32 SRVAPI FS_Rename( 
    PCSTR pszOldname, 
    PCSTR pszNewname 
){}

INT32 FS_GetFileName(INT32 iFd, INT32 iNameBufferLen, UINT8 *pcFileName){}

//jiashuo add start  
/*************************************************************************/
/*																	   */
/* FUNCTION															   */
/*																	   */
/*	  FS_GetFileSize												   */
/*																	   */
/* DESCRIPTION														   */
/*																	   */
/*	  This function is used to the file size.						   */
/*																	   */
/* INPUTS 															   */
/*																	   */
/*								                                       */
/*																	   */
/* OUTPUTS															   */
/*																	   */
/*	  Error code													   */
/*																	   */
/*************************************************************************/
INT32 SRVAPI FS_GetFileSize( 
    HANDLE hFile
)
{

  #if 0
  DWORD size_high;

  /* verify file handle */

  if (FileHandle >= (FSSIM_MAX_FILE + 1))

	  return FS_INVALID_FILE_HANDLE;

  else if (fssim_file[FileHandle].state != STATE_INUSE)

	  return FS_INVALID_FILE_HANDLE;

  else if (fssim_file[FileHandle].handle == INVALID_HANDLE_VALUE)

	  return FS_INVALID_FILE_HANDLE;


  size_high = 0;

  *Size = GetFileSize(fssim_file[FileHandle].handle, &size_high);

  if (size_high > 0) {
	  fssim_printf(("==================================================================\n"));
	  fssim_printf(("Fatal Error!! The file size of file handle %d exceeds 0xFFFFFFFF!!\n", FileHandle));
	  fssim_printf(("file size = 0x%08X%08X\n", size_high, *Size));
	  fssim_printf(("==================================================================\n"));
	  ASSERT(0);
  }

  return FS_NO_ERROR;
  #endif
}
//jiashuo add end
INT32 FS_ChangeSize(INT32 fd, UINT32 nFileSize)
{
  return (-1);
}
 INT32 FS_Truncate(HANDLE FileHandle)
 {
  return (-1);
 }

INT32 FS_Unmount( 
    PCSTR pszMountPoint,
    BOOL  bForce
){}

#if 0 /* delete by zhuoxz,2009-3-24 */
INT32 SRVAPI FS_MountRoot(
    PCSTR pszRootDevName 
){}


INT32 SRVAPI FS_Create(
    PCSTR pszFileName, 
    UINT32 iMode
){}
INT32 SRVAPI FS_FindFirstFile( 
    PCSTR pszFileName, 
    PFS_FIND_DATA  pFindData
){}

INT32 SRVAPI FS_FindNextFile(
    INT32 iFd, 
    PFS_FIND_DATA pFindData
){}

INT32 SRVAPI FS_FindClose(
    INT32 iFd
){}
INT32 SRVAPI FS_Rename( 
    PCSTR pszOldname, 
    PCSTR pszNewname 
){}
INT32 SRVAPI FS_PowerOn(
    PCSTR pszDevName
){}

INT32 SRVAPI FS_PowerOff(
    PCSTR pszDevName
){}
PCSTR FS_GetNameFromHandle(
                          HANDLE hFile
                          )
{
    return(-1);
}

INT16 fat_UnicodeLen(UINT8* pszUnicode){}

INT16 fat_ConvertGBToUni( UINT16 c ){}

INT32 fat_Unicode2OEM(UINT8* pszUnicode,UINT16 iULen,UINT8* pszOEM,UINT16* piOLen){}

#endif /* #if 0 */ /* delete by zhuoxz,2009-3-24 */

/* Add by zhuoxz,2009-4-29 */
/* start */
INT32 SRVAPI FS_Flush(INT32 fd)
{
  return 0;
}

INT32 SRVAPI FS_ChangeDir(PCSTR pszDirName)
{
  return 0;
}
/* end */

#undef UNICODE
//////

