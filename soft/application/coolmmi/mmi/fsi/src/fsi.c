/*create by chenhe,20080109.to adapted to diffirent FS of flash and Tflash*/
#include "cswtype.h"
#include "mmi_trace.h"
#include <ts.h>

#define FS_FILE_NAME_LEN       255 // Folder or File name size in byts, to support fat long file name.
#define FS_PATH_LEN            260  
#define LEN_FOR_NULL_CHAR               1         // Length of terminated character('\0' for OEM).
#define FSI_TRACE 0
// Storage information.
typedef struct _FS_INFO
{
	UINT32 iTotalSize;    // Total size
    UINT32 iUsedSize;     // Has used  size 
}FS_INFO;
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
INT32 SRVAPI FS_Create(
    PCSTR pszFileName, 
    UINT32 iMode
);
BOOL SRVAPI FS_MakeDir( 
    PCSTR pszDirName, 
    INT32 nMode 
);
BOOL SRVAPI FS_RemoveDir( 
    PCSTR pszDirName
);
HANDLE SRVAPI FS_FindFirstFile( 
    PCSTR pszFileName, 
    PFS_FIND_DATA  pFindData
);
INT32 SRVAPI FS_FindNextFile(
    INT32 iFd, 
    PFS_FIND_DATA pFindData
);

INT32 SRVAPI FS_FindClose(
    INT32 iFd
);
INT32 SRVAPI FS_Open(
    PCSTR pszFileName, 
    UINT32 iFlag, 
    UINT32 iMode
);
INT32 SRVAPI FS_Close(
    INT32 iFd
);
INT32 SRVAPI FS_Read(
    INT32 iFd, 
    UINT8 *pBuf, 
    UINT32 iLen
);
INT32 SRVAPI FS_Write(
    INT32 iFd, 
    UINT8 *pBuf, 
    UINT32 iLen
);
INT32 SRVAPI FS_Delete(
PCSTR pszFileName
);
INT32 SRVAPI FS_Seek(
    INT32 iFd, 
    INT32 iOffset, 
    UINT8 iOrigin
);
INT32 SRVAPI FS_Rename( 
    PCSTR pszOldname, 
    PCSTR pszNewname 
);
INT32 SRVAPI FS_GetFileAttr(
    PCSTR     pszFileName,
    FS_FILE_ATTR* pFileAttr
);
INT32 SRVAPI FS_GetFSInfo(
    PCSTR pszDevName,
    FS_INFO* psFSInfo
);
INT32 SRVAPI FS_IsEndOfFile(
    INT32 iFd
);
//add gdm 080306
INT32 SRVAPI FS_GetFileSize(
    INT32 iFd
);
INT32 FS_GetFileName(INT32 iFd, INT32 iNameBufferLen, UINT8 *pcFileName);
INT32 FS_Unmount( 
    PCSTR pszMountPoint,
    BOOL  bForce
);
INT32 FS_Mount
( 
    PCSTR pszDevName, 
    PCSTR pszMountPoint, 
    INT32 iFlag, 
    UINT8 iType 
);
INT32 SRVAPI FS_Format(
    PCSTR pszDevName, 
    UINT8 iFsType, 
    UINT8 iFlag
);
INT32 SRVAPI FS_HasFormatted(
    PCSTR pszDevName, 
    UINT8 iFsType
);
INT32 SRVAPI FS_MountRoot(
    PCSTR pszRootDevName 
);
//public marco or variable+
#define FHD_OFFSET (1000)
//public marco or variable-

BOOL checkTflashPath(PCSTR filename)
{
    #if 0 //gdm 080402
	if(( (*filename == '/'&&*(filename+1)== 0) &&(*(filename+2) == 't'&&*(filename+3) == 0)&&(*(filename+4) == '/'&&*(filename+5) == 0) ) ||
		( (*filename == 'd'&&*(filename+1) == 0) &&(*(filename+2) == ':'&&*(filename+3) == 0)&&(*(filename+4) == '/'&&*(filename+5) == 0) ) ||
		( (*filename == 'D'&&*(filename+1) == 0) &&(*(filename+2) == ':'&&*(filename+3) == 0)&&(*(filename+4) == '/'&&*(filename+5) == 0) ) )
    #else
	if(( (*filename == '/'&&*(filename+1)== 0) &&(*(filename+2) == 't'&&*(filename+3) == 0)&&(*(filename+4) == '/'&&*(filename+5) == 0) ) ||
		( (*filename == 'd'&&*(filename+1) == 0) &&(*(filename+2) == ':'&&*(filename+3) == 0)&&(*(filename+4) == '/'&&*(filename+5) == 0) ) ||
		( (*filename == 'D'&&*(filename+1) == 0) &&(*(filename+2) == ':'&&*(filename+3) == 0)&&(*(filename+4) == '/'&&*(filename+5) == 0) ) ||
		( (*filename == 'e'&&*(filename+1) == 0) &&(*(filename+2) == ':'&&*(filename+3) == 0)&&(*(filename+4) == '/'&&*(filename+5) == 0) ) ||
		( (*filename == 'E'&&*(filename+1) == 0) &&(*(filename+2) == ':'&&*(filename+3) == 0)&&(*(filename+4) == '/'&&*(filename+5) == 0) ) )
    #endif
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
INT32  FSI_Create(PCSTR pszFileName,  UINT32 iMode)
{
	mmi_trace(FSI_TRACE,"FSI_Create");
	if(checkTflashPath(pszFileName))
	{
		//return MCI_FsmMakeDir(pszFileName);
	}
	else
	{
		return FS_Create(pszFileName,iMode);
	}
}


BOOL FSI_MakeDir( PCSTR pszDirName, INT32 nMode )
{
	mmi_trace(FSI_TRACE,"FSI_MakeDir");
	if(checkTflashPath(pszDirName))
	{
		return MCI_FsmMakeDir(pszDirName);
	}
	else
	{
		mmi_trace(FSI_TRACE,"FSI_MakeDir,flash");
		return FS_MakeDir(pszDirName,nMode);
	}
}

//modify the type of return value by gaodm 080226
INT32 FSI_RemoveDir(     PCSTR pszDirName)
{
	mmi_trace(FSI_TRACE,"FSI_RemoveDir");
	INT32 ret;
	if(checkTflashPath(pszDirName))
	{
		ret = MCI_FsmRemoveDir(pszDirName);
		mmi_trace(FSI_TRACE,"FSI_RemoveDir ret=%d\n",ret);

		return ret;
	}
	else
	{
		return FS_RemoveDir(pszDirName);
	}
}

HANDLE  FSI_FindFirstFile( PCSTR pszFileName, PFS_FIND_DATA  pFindData)
{
    mmi_trace(g_sw_FMGR, "-----+++++Func: %s;File: %s;Line: %d+++++-----", __FUNCTION__, __FILE__, __LINE__);
	mmi_trace(FSI_TRACE,"FSI_FindFirstFile");
	if(checkTflashPath(pszFileName))
	{
		return MCI_FindFirstFile(pszFileName,pFindData)+FHD_OFFSET;
	}
	else
	{
		return FS_FindFirstFile(pszFileName,pFindData);
	}
}
INT32  FSI_FindNextFile(     INT32 hFindFile,   PFS_FIND_DATA pFindData)
{
	mmi_trace(FSI_TRACE,"FSI_FindNextFile,hFindFile is %d",hFindFile);
	if(hFindFile >= FHD_OFFSET)
	{

		return MCI_FindNextFile(pFindData);
	}
	else
	{
		return FS_FindNextFile(hFindFile,pFindData);
	}
#if 1
#else
	if(checkTflashPath(pszFileName))
	{
		return MCI_FindNextFile(pszFileName,pFindData);
	}
	else
	{
		return FS_FindNextFile(hFindFile,pFindData);
	}
#endif	
}
INT32  FSI_FindClose( INT32 iFd)
{
	mmi_trace(FSI_TRACE,"FSI_FindClose,iFd is %d",iFd);
	if(iFd >= FHD_OFFSET)
	{
		return MCI_FindClose();
	}
	else
	{
		return FS_FindClose(iFd);
	}
}


INT32 SRVAPI FSI_Open(    PCSTR pszFileName,     UINT32 iFlag,     UINT32 iMode)
{
		mmi_trace(FSI_TRACE,"FSI_Open");
	if(checkTflashPath(pszFileName))
	{
		return MCI_FsOpen(pszFileName,iFlag,iMode)+FHD_OFFSET;
	}
	else
	{
	mmi_trace(FSI_TRACE,"FSI_Open,flash");
		return FS_Open(pszFileName,iFlag,iMode);
	}
}

INT32 SRVAPI FSI_Close(    INT32 iFd)
{
	mmi_trace(FSI_TRACE,"FSI_Close,iFd is %d",iFd);
	if(iFd >= FHD_OFFSET)
	{
		return MCI_FsClose(iFd-FHD_OFFSET);
	}
	else
	{
		return FS_Close(iFd);
	}
}


INT32 SRVAPI FSI_Read( INT32 iFd,     UINT8 *pBuf,     UINT32 iLen)
{
		mmi_trace(FSI_TRACE,"FSI_Read,iFd is %d, iLen=%d",iFd,iLen);
	if(iFd >= FHD_OFFSET)
	{
		return MCI_FsRead(iFd-FHD_OFFSET,pBuf,iLen);
	}
	else
	{
		return FS_Read(iFd,pBuf,iLen);
	}
}

INT32 SRVAPI FSI_Write(    INT32 iFd,     UINT8 *pBuf,     UINT32 iLen)
{
	mmi_trace(FSI_TRACE,"FSI_Write,iFd is %d",iFd);
	if(iFd >= FHD_OFFSET)
	{
		return MCI_FsWrite(iFd-FHD_OFFSET, pBuf, iLen);
	}
	else
	{
		return FS_Write(iFd,pBuf,iLen);
	}
}
INT32 SRVAPI FSI_Delete(    PCSTR pszFileName)
{
	mmi_trace(FSI_TRACE,"FSI_Delete");
	if(checkTflashPath(pszFileName))
	{
		return MCI_FsmDelete(pszFileName, 0x00000004 );//FS_FILE_TYPE = 0x00000004
	}
	else
	{
		return FS_Delete(pszFileName);
	}
}

INT32 SRVAPI FSI_Seek(    INT32 iFd,     INT32 iOffset,     UINT8 iOrigin)
{
	mmi_trace(FSI_TRACE,"FSI_Seek,iFd is %d",iFd);
	if(iFd >= FHD_OFFSET)
	{
		return MCI_FsSeek(iFd-FHD_OFFSET, iOffset, iOrigin);
	}
	else
	{
		return FS_Seek(iFd,iOffset,iOrigin);
	}
}
INT32 SRVAPI FSI_Rename(     PCSTR pszOldname,     PCSTR pszNewname )
{
	mmi_trace(FSI_TRACE,"FSI_Rename");

	if(checkTflashPath(pszOldname))
	{
		return MCI_FsmRename(pszOldname,pszNewname);
	}
	else
	{
		return FS_Rename(pszOldname,pszNewname);
	}
}


INT32 SRVAPI FSI_GetFileAttr(    PCSTR     pszFileName,    FS_FILE_ATTR* pFileAttr)
{
	mmi_trace(FSI_TRACE,"FSI_GetFileAttr");
	if(checkTflashPath(pszFileName))
	{
		return MCI_GetFileAttr(pszFileName,pFileAttr);
	}
	else
	{
		return FS_GetFileAttr(pszFileName,pFileAttr);
	}
}

INT32 SRVAPI FSI_GetFSInfo(    PCSTR pszDevName,    FS_INFO* psFSInfo)
{
	mmi_trace(1,"FSI_GetFSInfo,%s",pszDevName);
	if((strcmp(pszDevName,"mmc0") == 0) ||(strcmp(pszDevName,"nand0") == 0))
	{
		return MCI_GetFSInfo(pszDevName, psFSInfo);
	}
	else
	{
		INT32 ret=FS_GetFSInfo(pszDevName,psFSInfo);
		mmi_trace(1,"FSI_GetFSInfo,flash, totle=%d,used=%d", psFSInfo->iTotalSize, psFSInfo->iUsedSize);
   return ret;
	}
}
INT32 SRVAPI FSI_IsEndOfFile(    INT32 iFd)
{
	mmi_trace(FSI_TRACE,"FSI_IsEndOfFile,iFd is %d",iFd);

	if(iFd >= FHD_OFFSET)
	{
		return MCI_IsEndOfFile(iFd-FHD_OFFSET);
	}
	else
	{
		return FS_IsEndOfFile(iFd);
	}
}

//add gdm 080306
INT32 SRVAPI FSI_GetFileSize(    INT32 iFd)
{
	mmi_trace(FSI_TRACE,"FSI_GetFileSize,iFd is %d",iFd);

	if(iFd >= FHD_OFFSET)
	{
		return MCI_GetFileSize(iFd-FHD_OFFSET);
	}
	else
	{
		return FS_GetFileSize(iFd);
	}
}


//add gdm 080522
INT32 SRVAPI FSI_ChangeSize( INT32 iFd, UINT32 nFileSize)
{
	mmi_trace(FSI_TRACE,"FSI_ChangeSize,iFd is %d",iFd);

	if(iFd >= FHD_OFFSET)
	{
		return MCI_ChangeSize(iFd-FHD_OFFSET, nFileSize);
	}
	else
	{
		mmi_trace(FSI_TRACE,"FSI_ChangeSize Flash err");
		return -1;;
	}
}
	
INT32 FSI_GetFileName(INT32 iFd, INT32 iNameBufferLen, UINT8 *pcFileName)
{
	mmi_trace(FSI_TRACE,"FSI_GetFileName,iFd is %d",iFd);

	if(iFd >= FHD_OFFSET)
	{
		//return MCI_FindClose();
	}
	else
	{
		return FS_GetFileName(iFd,iNameBufferLen,pcFileName);
	}
}
INT32 FSI_Unmount(     PCSTR pszMountPoint,    BOOL  bForce)
{
	mmi_trace(FSI_TRACE,"FSI_Unmount,bForce is %d",bForce);

	if(0)
	{
		//return MCI_FsmRename(pszOldname,pszNewname);
	}
	else
	{
		return FS_Unmount(pszMountPoint,bForce);
	}
}
INT32 FSI_Mount(     PCSTR pszDevName,     PCSTR pszMountPoint,     INT32 iFlag,     UINT8 iType )
{
	mmi_trace(FSI_TRACE,"FSI_Mount,iType is %d",iType);

	if(0)
	{
		//return MCI_FsmRename(pszOldname,pszNewname);
	}
	else
	{
		return FS_Mount(pszDevName,pszMountPoint,iFlag,iType);
	}
}
INT32 SRVAPI FSI_Format(    PCSTR pszDevName,     UINT8 iFsType,     UINT8 iFlag)
{
	mmi_trace(FSI_TRACE,"FSI_Format,iFsType is %d",iFsType);

	if(0)
	{
		//return MCI_FsmRename(pszOldname,pszNewname);
	}
	else
	{
		return FS_Format(pszDevName,iFsType,iFlag);
	}
}
INT32 SRVAPI FSI_HasFormatted(    PCSTR pszDevName,     UINT8 iFsType)
{
	mmi_trace(FSI_TRACE,"FSI_HasFormatted,iFsType is %d",iFsType);

	if(0)//(strcmp(pszDevName,"mmc0")==0)
	{
		//return MCI_FsmRename(pszOldname,pszNewname);
	}
	else
	{
		return FS_HasFormatted(pszDevName,iFsType);
	}
}
INT32 SRVAPI FSI_MountRoot(    PCSTR pszRootDevName )
{
	mmi_trace(FSI_TRACE,"FSI_MountRoot");

	if(0)
	{
		//return MCI_FsmRename(pszOldname,pszNewname);
	}
	else
	{
		return FS_MountRoot(pszRootDevName);
	}
}
