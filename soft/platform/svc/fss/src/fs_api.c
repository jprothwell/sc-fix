#include "fat_base.h"
#include "fs.h"
#include "fs_base.h"
#include "fat_local.h"
#include "storage.h"

#include <string.h>

#include "fss_debug.h"

#include "cs_types.h"

#include "sxr_ops.h"

#ifdef _MS_VC_VER_
#include <assert.h>
extern INT32 g_dsm_errno;
extern BOOL wfcheat;
#endif

HANDLE g_fs_sem = 0xFF;

extern struct super_block* g_fs_super_block_table;
extern struct task_struct g_fs_current;
extern struct file* g_fs_file_list;
//extern struct file g_fs_file_table[ FS_NR_OPEN_FILES_MAX ];
//extern struct inode g_fs_inode_table[ FS_NR_INODES_MAX ];
extern UINT8 *g_FSRootDevName;

extern INT32 DRV_DevNr2Name( UINT32 iDevNr, PSTR pszDevName );

extern UINT32 DRV_MMCPowerUp (VOID);
extern UINT32 DRV_MMCPowerDown (VOID);
/********************************************************************************/
// Function:
//     Detect the device if is formated.
// Parameter:
//     pszDevName[in]:  the device name.
//     iFsType[in]: the file system type.
// Return value:
//     if is formated return ERR_FS_HAS_FORMATED,else return ERR_FS_NOT_FORMAT;
//*******************************************************************************


INT32 FS_HasFormatted(PCSTR pszDevName, UINT8 iFsType)
{
    INT32 iDevNo;
    BOOL bIsMMC = FALSE;
    INT32 iResult;
    INT32 iRet;    
    
    if ( !pszDevName )
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    switch(iFsType)
    {
    case FS_TYPE_FAT:
    	iDevNo = DRV_DevName2DevNr(pszDevName);
    	if(INVALID_DEVICE_NUMBER == iDevNo)
    	{		
            SVC_FSS_TRACE( DL_VFSERROR, "in fs_hasformatted DevNo is invalid, pszDevName = %s\n", pszDevName);
            SVC_FSS_TRACE( DL_VFSERROR, "in fs_hasformatted DevNo is invalid,iDevNo = %x0x\n", iDevNo );
            return ERR_FS_DEVICE_NOT_REGISTER;
        }
        
        if ( ( 0 == DSM_StrNCaselessCmp( "mmc", pszDevName, 3) ) )
        {
            bIsMMC = TRUE;
            iRet = DRV_OpenMMC( NULL );
            if( ERR_SUCCESS != iRet )
            {
                SVC_FSS_TRACE( DL_VFSERROR, "check formatted or not[%s]: Open MMC device failed.\n", pszDevName );
                return ERR_FS_OPEN_DEV_FAILED;
            }
        }
        
        FS_Down();
    	iResult = fat_HasFormated(iDevNo, iFsType);
        FS_Up();        
    	iRet = fs_SetErrorCode(iResult);
    	break;
    case FS_TYPE_FFS:
    default:
    	iRet = ERR_FS_UNKNOWN_FILESYSTEM;
    	break;
    }
    
    if ( bIsMMC )
    {
        //DRV_CloseMMC( NULL );
    }
    
    return iRet;
}

//********************************************************************************
// Function:
//   This function is format the virtual disk.
// Parameter:
//   pszDevType[in]: the name of device.
//   iFsType[in]: the type of file system.
//   iFlag[in]: The option of format, 0:Quickly;1:Normal.
// Return value:
//      ERR_SUCCESS indicates a successful operation. 
//         And unsuccessful operation is denoted by the err code as following.
//      ERR_FAT_READ_SEC_FAILED:    Read the sector failed.
//      ERR_FAT_WRITE_SEC_FAILED:   Write the sector failed.
//      ERR_FAT_MALLOC_FAILED       allocte memory failed.
//*******************************************************************************

INT32 FS_Format(PCSTR pszDevName, UINT8 iFsType, UINT8 iFlag)
{
    INT32 iDevNo;
    UINT32 iBlockSize;
    UINT32 iBlockCount;
    UINT32 iDiskTotSec;
    UINT8 szVolLab[FBR_VOL_LAB_SIZE + 1];
    UINT8 szOEMName[FBR_OEM_NAME_SIZE + 1];
    UINT8 iOpt;
    INT32 iResult;
    INT32 iRet;
    BOOL bIsMMC = FALSE;
    struct super_block *sb = NULL;
    if ( !pszDevName )
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    // Not allow format the vds2,because it storage the reg and the sms data
    // allow formatvds0 and vds1
#ifndef _MS_VC_VER_
   
    if ( pszDevName && 0 == DSM_StrCaselessCmp( pszDevName, "vds2" ) )
    {
        SVC_FSS_TRACE( DL_ERROR, "vds2 is not permitted to access for FS" );
        return ERR_FS_INVALID_PARAMETER;
    }
#endif
    
    iRet = ERR_SUCCESS;
    SVC_FSS_TRACE( DL_FATDETAIL, "FS_Format devname = %s, FsType = %d, Flag = %d.\n",
            		pszDevName, iFsType,iFlag );  

    FS_Down();
    
    switch(iFsType)
    {
    case FS_TYPE_FAT:
        iDevNo = DRV_DevName2DevNr(pszDevName);
        if(INVALID_DEVICE_NUMBER == iDevNo)
        {
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_Format DevNo is invalid, pszDevName = %s\n", pszDevName);
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_Format DevNo is invalid,iDevNo = %x0x\n", iDevNo );
            iRet = ERR_FS_DEVICE_NOT_REGISTER;
            goto step0_failed;
        }

        if ( ( 0 == DSM_StrNCaselessCmp( "mmc", pszDevName, 3) ) )
        {
            bIsMMC = TRUE;
            iRet = DRV_OpenMMC( NULL );
            if( ERR_SUCCESS != iRet )
            {
                SVC_FSS_TRACE( DL_VFSERROR, "Format device[%s]: Open MMC failed.\n", pszDevName );
                iRet = ERR_FS_OPEN_DEV_FAILED;
                goto step0_failed;
            }
        }

        
        // If the s_root and s_mountpoint are equal, the superblock will be for ROOT FS. 
        
        sb = get_super( iDevNo );
        if (sb)
        {
            SVC_FSS_TRACE( DL_FATERROR,"Format a device which is mounting is NOT permitted" ); 
            iRet = ERR_FS_FORMAT_MOUNTING_DEVICE;				
            goto step1_failed;
        }
 
        iBlockSize = 0;
        iBlockCount = 0;
        SVC_FSS_TRACE( DL_FATDETAIL, " Call DRV_GET_DEV_INFO  iDevNo = %d.\n", iDevNo);  
        iResult = DRV_GET_DEV_INFO(iDevNo,&iBlockCount,&iBlockSize);
        if(ERR_SUCCESS != iResult)
        {
            SVC_FSS_TRACE( DL_FATERROR,
            		" DRV_GET_DEV_INFO  failed!,iResult = %d.\n",
            		iResult); 
			iRet = ERR_FS_GET_DEV_INFO_FAILED;				
            goto step1_failed;
	    }
	    else
        {			
            SVC_FSS_TRACE( DL_FATDETAIL,
            		" DRV_GET_DEV_INFO  success!,iBlockCount = %d,iBlockSize = %d.\n",
            		iBlockCount,
            		iBlockSize); 
            iDiskTotSec = (iBlockSize / 512) * iBlockCount;

            strcpy((INT8*)szVolLab,(INT8*)FAT_VOLLABLE);

            strcpy((INT8*)szOEMName,(INT8*)FAT_OEMNAME);

            iOpt = iFlag;
            SVC_FSS_TRACE( DL_FATDETAIL,
            		"Call fat_format with iDevNo = %d, iDiskTotSec = %d,szVolLab = %s,szOEMName = %s,iOpt = %d.\n",
            		iDevNo,
            		iDiskTotSec,
            		(UINT8*)szVolLab,
            		(UINT8*)szOEMName,
            		iOpt);
                       
            iResult = fat_format(iDevNo, iDiskTotSec,(UINT8*)szVolLab,(UINT8*)szOEMName,iOpt);

            SVC_FSS_TRACE( DL_FATDETAIL, "fat_format return, iResult = %d.\n", iResult);  
            if(_ERR_FAT_SUCCESS == iResult)
            {
            	iRet = ERR_SUCCESS;
            }
            else
            {
            	iRet = fs_SetErrorCode(iResult);
            }
        }
    	break;    
    case FS_TYPE_FFS:        
    default:
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
        
step1_failed:    
step0_failed:
    FS_Up();
    return iRet;
}

/*
We custom the structure file{} for FS_Find*** APIs:
1. Use f_pos field to save the next dir-entry to be found next time
2. Use f_mode and  f_flags fields to save the pointer to the pattern for FindNextFile().
    The memory will be freed at FindClose().
*/
INT32 FS_FindFirstFile( PCWSTR pszFileName, PFS_FIND_DATA  pFindData )
{
    WCHAR *path = NULL;
    WCHAR *pattern = NULL;
    INT32 patternlen;
    struct inode *dir_i;
    struct file * f = NULL;
    struct dentry *psDe = NULL;
    INT32 fd;
    INT32 iRet = ERR_SUCCESS;
    UINT16 iULen = 0;
    
    if(NULL == pFindData) // invalid pathname or pFindData is NULL
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindFirstFile, parameter error" );
        return  ERR_FS_INVALID_PARAMETER;
    }
    
    iULen = (UINT16)DSM_TcStrLen(pszFileName);    
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    // path name character validate, Ignore base name.
    if( !is_file_name_valid( pszFileName, TRUE ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindFirstFile , pathname is invalid");
        return  ERR_FS_INVALID_PARAMETER;
    }
    
    FS_Down();
    
    for ( fd = 0; fd < (INT32)g_fs_cfg.uNrOpenFileMax; fd++ )
    {
        if ( NULL == g_fs_current.filp[ fd ] )                 /*unused fd found*/
        {
            break;
        }
    }
    if((INT32)g_fs_cfg.uNrOpenFileMax == fd) // No fd is available
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindFirstFile, fd overflow, fd = %d",fd );
        iRet = ERR_FS_FDS_MAX;
        goto step0_failed;
    }

    f = fs_alloc_fd();
    if (NULL == f)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindFirstFile, fs_alloc_fd() return NULL");
        iRet = ERR_FS_NO_MORE_MEMORY;
        goto step0_failed;
    }
    f = fs_fd_list_add(&g_fs_file_list, f);
    
    path = format_dir_name( pszFileName );
    if(NULL == path) // malloc failed
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindFirstFile, path is NULL" );
        iRet = ERR_FS_NO_MORE_MEMORY;
        goto step0_failed;
    }

    dir_i = dir_namei( path, &patternlen, (CONST WCHAR**)&pattern );
    if(NULL == dir_i ) // path is invalid 
    {
        SVC_FSS_TRACE(DL_FATERROR, "++++ Call dir_namei() return NULL! ++++\n");
        iRet = ERR_FS_PATHNAME_PARSE_FAILED;
        goto step0_failed;
    }
    if(0 == patternlen) // patternlen is 0
    {
        SVC_FSS_TRACE(DL_FATERROR, "++++ Call dir_namei() return. ERR_FS_NO_BASENAME! ++++\n");
        iRet = ERR_FS_NO_BASENAME;
        goto step1_failed;
    }

    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE( DL_FATERROR, "In FS_FindFirstFile malloc dentry failed.\n");
        iRet = ERR_FS_NO_MORE_MEMORY;
        goto step1_failed;
    }

    // 这里拷贝字符串的长度应该不会超过文件名的最大长度，
    // 因为前面已经做了检查。
    memset( psDe, '\0', sizeof( *psDe ) );
    DSM_TcStrCpy( psDe->name, pattern);

    psDe->append_data = FAT_CSO2INO(0, 0, 0); 
    psDe->append_data2 = FS_FLAG_FIND_FILE;
    
    iRet = do_find_entry(&dir_i, psDe, FAT_LOOK_GET_NAME);
    if ( DIR_ENTRY_NOT_FOUND == iRet )
    {
        SVC_FSS_TRACE(DL_FATWARNING, "+++ do_find_entry() return failed\n");
        iRet = ERR_FS_NO_MORE_FILES;
        goto step1_failed;
    }

    // Modify by suwg at 2007/7/6
    f->f_pattern = sxr_Malloc((DSM_TcStrLen(pattern) + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR));
    if(NULL == f->f_pattern)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindFirstFile, f_pattern is NULL" );
        iRet = ERR_FS_NO_MORE_MEMORY;
        goto step2_failed;
    }

    g_fs_current.filp[ fd ] = f;
    f->f_count = 1;
    f->f_inode = dir_i;
    f->f_flags = FS_O_FIND;
    f->f_pos = psDe->append_data;

    DSM_TcStrCpy( f->f_pattern, pattern );      // Modify by suwg at 2007/7/6

    pFindData->st_mode = psDe->inode->i_mode;  // directory or regular file
    pFindData->st_size = psDe->inode->i_size;
    pFindData->st_atime = psDe->inode->i_atime;
    pFindData->st_mtime = psDe->inode->i_mtime;
    pFindData->st_ctime = psDe->inode->i_ctime;

    iULen = (UINT16)DSM_TcStrLen(psDe->name);
    if(iULen > FS_FILE_NAME_UNICODE_LEN)
    {
        iRet =  ERR_FS_READ_DIR_FAILED;
        goto step2_failed;
    }

    DSM_TcStrCpy(pFindData->st_name, psDe->name);
    iRet = ERR_SUCCESS;

step2_failed:
    iput( psDe->inode );

step1_failed:
    /*
    当FS_FindFirstFile成功时,不释放dir_i, 因为在随后的
    FS_FindNextFile()中会使用该inode, 在FS_FindClose()中会释放该inode.
    */
    if ( ERR_SUCCESS != iRet )
    {
        iput( dir_i );
    }

    if (psDe != NULL)
    {
        DENTRY_FREE(psDe);
    }
    
step0_failed:

    if (iRet != ERR_SUCCESS && f != NULL)
    {
        if(f->f_pattern != NULL)
        {
            sxr_Free(f->f_pattern);
        }
        fs_fd_list_del(&g_fs_file_list, f);
    }
    
    if(path != NULL)
    {
        NAME_BUF_FREE(path);
    }

    FS_Up();
     
    return ( ERR_SUCCESS == iRet ) ? fd : iRet;
}


INT32 FS_FindNextFile( INT32 fd, PFS_FIND_DATA pFindData)
{
    struct file * f;
    struct inode * dir_i;
    struct dentry *psDe = NULL;
    INT32 iRet;
    UINT16 iULen = 0;

    if(NULL ==pFindData)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindNextFile, parameter error, pFindData is NULL" );
        return ERR_FS_INVALID_PARAMETER;
    }

    if(!FD_IS_VALID( fd ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindNextFile, parameter error, fd is invalid,fd = %d",fd );
        return ERR_FS_INVALID_PARAMETER;
    }

    f = g_fs_current.filp[fd];
    if(!f || !fs_is_file_opened(&g_fs_file_list, f))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindNextFile, f is NULL" );
        return ERR_FS_BAD_FD;
    }

    if(0 == (f->f_flags & FS_O_FIND)) // handle is not for finding
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_FindNextFile, handle is not for finding, f->flag = %d",f->f_flags);
        return ERR_FS_BAD_FD;
    }
    if ( 0 == f->f_pos )
    {
        return ERR_FS_NO_MORE_FILES;
    }

    FS_Down();
    
    dir_i = f->f_inode;
    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        FS_Up();
        return ERR_FS_NO_MORE_MEMORY;
    }
    memset( psDe, '\0', sizeof( *psDe ) );
    DSM_TcStrCpy( psDe->name, f->f_pattern);

    psDe->append_data = f->f_pos;
    psDe->append_data2 = FS_FLAG_FIND_FILE;
    
    iRet = do_find_entry( &dir_i, psDe, FAT_LOOK_GET_NAME );
    if(DIR_ENTRY_FOUND == iRet)
    {
        f->f_pos = psDe->append_data;

    	pFindData->st_mode = psDe->inode->i_mode;  // directory or regular file
    	pFindData->st_size = psDe->inode->i_size;
        pFindData->st_atime = psDe->inode->i_atime;
        pFindData->st_mtime = psDe->inode->i_mtime;
        pFindData->st_ctime = psDe->inode->i_ctime;
        // NameFat2User(psDe.inode->u.fat_i.entry.DIR_Name, pFindData->st_name);
        // strcpy(pFindData->st_name, psDe.name);
        iULen = (UINT16)DSM_TcStrLen(psDe->name);
        if(iULen > FS_FILE_NAME_UNICODE_LEN)
        {
            DENTRY_FREE(psDe);
            FS_Up();
            return ERR_FS_READ_DIR_FAILED;
        }
        DSM_TcStrCpy(pFindData->st_name, psDe->name);
        iput( psDe->inode );
        iRet = ERR_SUCCESS;
    }
    else if ( DIR_ENTRY_NOT_FOUND == iRet )
    {       
        iRet = ERR_FS_NO_MORE_FILES;
    }
    else
    {
        iRet = fs_SetErrorCode(iRet);
    }
    DENTRY_FREE(psDe);
    FS_Up();
    return iRet;
}


INT32 FS_FindClose(INT32 fd)
{
    INT32 iRet; 
    
    FS_Down();
    iRet = fs_findclose(fd);
    FS_Up();
    
    return iRet;    
}


INT32 FS_GetFileAttr( PCWSTR pszFileName, FS_FILE_ATTR* pFileAttr)
{
    BOOL bResult;
    INT32 iRet = ERR_SUCCESS;    
    struct inode *inode = NULL;    
    UINT16 iULen = 0;
    
    if( !pszFileName || !pFileAttr ) // hFile is invalid
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    iULen = (UINT16)DSM_TcStrLen(pszFileName);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN )
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    bResult = is_file_name_valid( pszFileName, FALSE );
    if(!bResult)
    {        
        SVC_FSS_TRACE(DL_FATWARNING, "Invalid file name");
        return ERR_FS_INVALID_PARAMETER;
    }
    
    FS_Down();
    inode = namei( pszFileName );
    if(!inode)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "directory entry does not exist" );        
        iRet = ERR_FS_PATHNAME_PARSE_FAILED;
    }
    else
    {
        pFileAttr->i_no = inode->i_ino;
    	pFileAttr->i_mode = inode->i_mode;
    	pFileAttr->i_size = inode->i_size;
        pFileAttr->i_count = (UINT16)(inode->i_count - 1);
    	pFileAttr->i_atime = inode->i_atime;
    	pFileAttr->i_mtime = inode->i_mtime;
    	pFileAttr->i_ctime = inode->i_ctime;
        iput( inode );
    }
    FS_Up();    
    return iRet;
}


INT32 FS_GetFSInfo(PCSTR pszDevName,FS_INFO* psFSInfo)
{
    INT32 iDevNo;
    UINT32 iFatEntry;
    INT32 iOldFatEntry;
    UINT32 iSizePerClus;
    UINT32 iFreeSize;
    struct super_block* pSB = NULL;
    INT32 iResult;
    INT32 iRet = ERR_SUCCESS;

    if ( !pszDevName )
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    FS_Down();
    
    iDevNo = DRV_DevName2DevNr(pszDevName);
    if(INVALID_DEVICE_NUMBER == iDevNo)
    {
        FS_Up();
    	return ERR_FS_DEVICE_NOT_REGISTER;
    }
    
    iResult = fs_FindSuperBlock(iDevNo,&pSB);
    if(_ERR_FS_SUCCESS == iResult)
    {
        switch(pSB->s_fstype)
        {
            case FS_TYPE_FAT: 
               
                psFSInfo->iTotalSize = (pSB->u.fat_sb.iDataSec / pSB->u.fat_sb.iSecPerClus) * pSB->u.fat_sb.iSecPerClus * pSB->u.fat_sb.iBytesPerSec;
                
                iFreeSize = 0;
                iFatEntry = 0;
                iOldFatEntry = 0;
                iSizePerClus = (pSB->u.fat_sb.iBytesPerSec) * (pSB->u.fat_sb.iSecPerClus);
                
                iResult = fat_stat_free_cluster(&(pSB->u.fat_sb), &iFatEntry);
                if(ERR_SUCCESS != iResult)
                {
                    iRet = fs_SetErrorCode(iResult);
                }
                
                iFreeSize = iFatEntry*iSizePerClus;
                psFSInfo->iUsedSize = (psFSInfo->iTotalSize - iFreeSize);
                break;

            case FS_TYPE_FFS:
            default:
                iRet = ERR_FS_UNKNOWN_FILESYSTEM;
                break;
        }
    }
    else
    {
        iRet = fs_SetErrorCode(iResult);
    }

    FS_Up();
    
    return iRet;
}


INT32 FS_GetCurDir(UINT32 uSize, PWSTR pCurDir)
{
    UINT32 uErrCode = ERR_SUCCESS;
    UINT16 uLen;

    FS_Down();
    if (NULL == pCurDir)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "FS_GetCurDir: pCurDir is null");
        uErrCode = ERR_FS_INVALID_PARAMETER;
        goto _get_cur_dir_end;
    }

    if (NULL == g_fs_current.wdPath)
    {
        uErrCode = ERR_FS_OPERATION_NOT_GRANTED;
        goto _get_cur_dir_end;
    }
    
    uLen = (UINT16)DSM_TcStrLen(g_fs_current.wdPath);
    if ((UINT32)uLen + LEN_FOR_NULL_CHAR > uSize / SIZEOF(WCHAR))
    {
        SVC_FSS_TRACE(DL_VFSERROR, "FS_GetCurDir: uLen(%d), uSize(%d), ERR_DSS_FS_NAME_BUFFER_TOO_SHORT", uLen, uSize);
        uErrCode = ERR_FS_NAME_BUFFER_TOO_SHORT;
        goto _get_cur_dir_end;
    }

    DSM_TcStrCpy(pCurDir, g_fs_current.wdPath);
    uErrCode = ERR_SUCCESS;
    
_get_cur_dir_end:
    
    FS_Up();
    return uErrCode;
}


INT32 FS_Rename( PCWSTR pszOldname, PCWSTR pszNewname )
{
    BOOL bRet;
    INT32 iResult;
    INT32 iRet = ERR_SUCCESS;
    WCHAR *basename;
    INT32 namelen;
    struct inode *dir_old, *dir_new;
    struct dentry *psOld_de = NULL, *psDe = NULL;
    UINT16 iULen = 0;
    
    if (NULL == pszOldname || NULL ==pszNewname)
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    iULen = (UINT16)DSM_TcStrLen(pszOldname);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN )
    {
        return ERR_FS_INVALID_PARAMETER;
    }
        
    bRet = is_file_name_valid( pszOldname, FALSE );
    if(!bRet)
    {        
        SVC_FSS_TRACE(DL_FATWARNING, "invalid old pathname");
        return ERR_FS_INVALID_PARAMETER;
    }

    iULen = (UINT16)DSM_TcStrLen(pszNewname);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    bRet = is_file_name_valid( pszNewname, FALSE );
    if(!bRet)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "invalid new pathname");
        return ERR_FS_INVALID_PARAMETER;
    }

    FS_Down();

    dir_old = dir_namei( pszOldname, &namelen, (CONST WCHAR**)&basename );
    if(!dir_old)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "old path is invalid");
        iRet = ERR_FS_PATHNAME_PARSE_FAILED;
        goto step0_failed;
    }
    if(!namelen)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "file name not specified");
        iRet = ERR_FS_NO_BASENAME;
        goto step1_failed;
    }

    psOld_de = DENTRY_ALLOC();
    if (NULL == psOld_de)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "FS_Rename: DENTRY_ALLOC psOld_de is NULL.\n");
        iRet = ERR_FS_NO_MORE_MEMORY;
        goto step1_failed;
    }

    memset( psOld_de, 0, sizeof( *psOld_de ) );
    DSM_TcStrCpy(psOld_de->name, basename);

    iResult = find_entry( &dir_old, psOld_de );
    if(-1 == iResult)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "file not found");
        iRet = ERR_FS_NO_DIR_ENTRY;
        goto step1_failed;
    }
    
    // Some process is dealing with the file. It will not be removed.
    if( /*FS_INODE_IS_REG( psOld_de->inode->i_mode ) && */psOld_de->inode->i_count > 1)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "Some process is dealing with the file. It will not be renamed.");
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto step2_failed;
    }
    
    dir_new = dir_namei( pszNewname, &namelen, (CONST WCHAR**)&basename );
    if(!dir_new)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "old path is invalid");
        iRet = ERR_FS_PATHNAME_PARSE_FAILED;
        goto step2_failed;
    }
    if(!namelen)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "file name not specified");
        iRet = ERR_FS_NO_BASENAME;
        goto step3_failed;
    }

    if ( FS_INODE_IS_DIR( psOld_de->inode->i_mode ) && dir_new != dir_old )
    {
        SVC_FSS_TRACE(DL_FATWARNING, "Move directory is not permitted");
        iRet = ERR_FS_REMOVE_DIRECTORY_BY_RENAME;
        goto step3_failed;
    }

    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "FS_Rename: DENTRY_ALLOC psDe is NULL.\n");
        iRet = ERR_FS_NO_MORE_MEMORY;
        goto step3_failed;
    }
    
    memset( psDe, 0, sizeof( *psDe ) );
    DSM_TcStrCpy( psDe->name, basename);
    
    iResult = find_entry( &dir_new, psDe );
    if(!iResult)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "file has existed");
        iput( psDe->inode );
        iRet = ERR_FS_FILE_EXIST;
        goto step3_failed;
    }
    
    iResult = fat_do_link( psOld_de, dir_new, psDe, FS_INODE_GET_TYPE( psOld_de->inode->i_mode ) );
    if(iResult)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "do link failed");
        iRet = fs_SetErrorCode(iResult);
        goto step3_failed;
    }
    
    iResult = fat_do_unlink(dir_old, psOld_de, TRUE );
    if(iResult)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "do unlink failed");
        // iput( psDe->inode ); // 这里的@psDe->inode本来就为空。suwg delete at 2007/7/3
        iRet = fs_SetErrorCode(iResult);
        goto step3_failed;
    }

    iRet = ERR_SUCCESS;

step3_failed:
    iput( dir_new );

    if (psDe != NULL)
    {
        DENTRY_FREE(psDe);
    }

step2_failed:
    iput( psOld_de->inode );
step1_failed:
    iput( dir_old );
    
    if (psOld_de != NULL)
    {
        DENTRY_FREE(psOld_de);
    }
    
step0_failed:
    FS_Up();
    return iRet;
}


/* NO semaphore protection */
INT32 FS_MountRoot( PCSTR pszRootDevName )
{    
    INT32 lErrCode = ERR_SUCCESS;
    INT32 iDevNo;
    UINT32 i;
    struct super_block * sb = NULL;
    struct inode *root_i;
    BOOL bMountFromMMC = FALSE;
   
    
    if ( g_fs_current.root )
    {
        SVC_FSS_TRACE( DL_FATWARNING, "Root file system has mounted" );
        return ERR_SUCCESS;
    }
    
    if (g_fs_sem == 0xFF) 
    { 
        g_fs_sem = sxr_NewSemaphore(1);
    }
    
    g_fs_current.pwd = NULL;
    g_fs_current.root = NULL;
    memset( g_fs_current.filp, 0x00, (sizeof( struct file * ) ) * g_fs_cfg.uNrOpenFileMax);

    // Clean super block table
    for ( i = 0; i < g_fs_cfg.uNrSuperBlock; i++ ) // FS_NR_SUPER is 1 here.
    {
        g_fs_super_block_table[ i ].s_dev = INVALID_DEVICE_NUMBER;
        //g_fs_super_block_table[ i ].s_lock = 0;   
        g_fs_super_block_table[ i ].s_root = NULL;
        g_fs_super_block_table[ i ].s_mountpoint = NULL;
    }
    
    fs_GetSuperBlockFree(&sb);

    iDevNo = DRV_DevName2DevNr( pszRootDevName ? pszRootDevName : g_FSRootDevName );
    if (INVALID_DEVICE_NUMBER == iDevNo)
    {
        return ERR_FS_DEVICE_NOT_REGISTER;
    }

    if ( ( pszRootDevName && 0 == DSM_StrNCaselessCmp( "mmc", pszRootDevName, 3) ) ||
         ( !pszRootDevName && 0 == DSM_StrNCaselessCmp( "mmc", g_FSRootDevName, 3) ) )
    {
        bMountFromMMC = TRUE;
        lErrCode = DRV_OpenMMC( NULL );
        if( ERR_SUCCESS != lErrCode )
        {
            SVC_FSS_TRACE( DL_VFSERROR, "Mount root filesystem: Open MMC device failed" );
            return ERR_FS_OPEN_DEV_FAILED;
        }
    }
    
    /* We ASSUME that root filesystem is FAT */
    lErrCode = fat_read_super(sb, iDevNo);
    if( ERR_SUCCESS != lErrCode )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Mount root filesystem: failed to read superblock" );        
        goto step0_failed;
    }

    root_i = iget(sb, (UINT32)FS_ROOT_INO);
    if( NULL == root_i)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Mount root filesystem: failed to read root inode" );
        lErrCode = ERR_FS_MOUNT_READ_ROOT_INODE_FAILED;
        goto step0_failed;
    }
    
    root_i->i_is_mountpoint = FALSE; // 不能设为TRUE，不然会导  致iget(FS_ROOT_INO)时会死循环。
    sb->s_mountpoint = sb->s_root = root_i;
    
    g_fs_current.pwd = root_i;   
    g_fs_current.root = root_i;  

    root_i->i_count += 3;

    DSM_TcStrCpy(g_fs_current.wdPath, (PCWSTR)_T("/"));    
    return ERR_SUCCESS;
    
step0_failed:    
    if ( bMountFromMMC )
    {
        //DRV_CloseMMC( NULL );
    }
    sb->s_dev = INVALID_DEVICE_NUMBER;
    return fs_SetErrorCode(lErrCode);    
}


INT32 FS_Mount( PCSTR pszDevName, PCWSTR pszMountPoint, INT32 iFlag, UINT8 iType )
{
    struct inode *mount_point_i, *root_i;
    int iDevNr;
    struct super_block * sb;
    INT32 iRet = ERR_SUCCESS;
    INT32 iResult = ERR_SUCCESS;
    BOOL bMountFromMMC = FALSE;
    UINT16 iULen = 0;
    
    //cheat compiler
    iFlag = iFlag; // WARNING: maybe need do it.
    
    if ( !pszDevName )
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    iULen = (UINT16)DSM_TcStrLen(pszMountPoint);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }    
    
    iDevNr = DRV_DevName2DevNr( pszDevName );
    if (INVALID_DEVICE_NUMBER == iDevNr)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Unknown device name[%s]", pszDevName );
        return ERR_FS_DEVICE_NOT_REGISTER;
    }

    // file name character validate.
    if( !is_file_name_valid( pszMountPoint, FALSE ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Mount , pathname is invalid");
        return  ERR_FS_INVALID_PARAMETER;
    }
    
    FS_Down();
    
    mount_point_i = namei( pszMountPoint );  

    if(!mount_point_i) // mount point not found
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Mountpoint does not exist!\n" );
        iRet = ERR_FS_INVALID_DIR_ENTRY;
        goto step0_failed;
    }

    if(!FS_INODE_IS_DIR( mount_point_i->i_mode ))  // mount point is not directory
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Mountpoint MUST be a directory" );
        iRet = ERR_FS_NOT_DIRECTORY;
        goto step1_failed;
    }

    if( mount_point_i->i_count != 1 ) // mount point is busy
    {
        SVC_FSS_TRACE( DL_VFSERROR, "[%s] is under reference, i_count = %d.\n", pszMountPoint, mount_point_i->i_count);
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto step1_failed;
    }

    if( mount_point_i->i_ino == FS_ROOT_INO )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "some fs has mounted on the mountpoint[%s]", pszMountPoint );
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto step1_failed;
    }
    
    sb = get_super( iDevNr );
    if(sb)  // file system has been mounted
    {
        SVC_FSS_TRACE( DL_VFSERROR, "file system on the device[%s] HAS mounted", pszDevName );
        iRet = ERR_FS_HAS_MOUNTED;
        goto step1_failed;
    }

    for ( sb = g_fs_super_block_table; sb < g_fs_cfg.uNrSuperBlock + g_fs_super_block_table; sb++ )
    {
        if ( INVALID_DEVICE_NUMBER == sb->s_dev )
            break;
    }
    if(sb == g_fs_cfg.uNrSuperBlock + g_fs_super_block_table) // g_fs_super_block_table is full
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Too many file systems has mounted" );
        iRet = ERR_FS_MOUNTED_FS_MAX;
        goto step1_failed;
    }
        
    switch( iType )
    {
        case FS_TYPE_FAT:
            if ( 0 == DSM_StrNCaselessCmp( "mmc", pszDevName, 3) )
            {
                bMountFromMMC = TRUE;
                iRet = DRV_OpenMMC( NULL );
                if( ERR_SUCCESS != iRet )
                {
                    SVC_FSS_TRACE( DL_VFSERROR, "Mount filesystem: Open MMC device failed" );
                    iRet =  ERR_FS_OPEN_DEV_FAILED;
                    goto step1_failed;
                }
            }
            
            iResult = fat_read_super( sb, iDevNr );
            if(iResult != ERR_SUCCESS) // read fat super block from device failed
            {
                SVC_FSS_TRACE( DL_VFSERROR, "Read the super block for the file system to mount failed" );
                iRet = fs_SetErrorCode(iResult);
                goto step2_failed;
            }
            
            break;
        default:  // unknown file system iType
            iRet = ERR_FS_UNKNOWN_FILESYSTEM;
            SVC_FSS_TRACE(DL_VFSERROR, "unknown file system type");
            goto step1_failed;
            
            break;
    }

    root_i = iget( sb, (UINT32)FS_ROOT_INO );
    if(!root_i)  // Unable to read root i-node
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Read the root inode for the file system to mount failed" );
        iRet = ERR_FS_MOUNT_READ_ROOT_INODE_FAILED;
        goto step2_failed;
    }
    
    mount_point_i->i_is_mountpoint = TRUE;

    /*DO NOT increase the reference for the inode, for mount_point_i is a emporary pointer*/
    sb->s_mountpoint = mount_point_i;

    /*DO NOT increase the reference for the inode, for root_i is a emporary pointer*/
    sb->s_root = root_i;

    /* NOTE! we don't iput( mount_point_i ), we do that in umount */
    /* NOTE! we don't close mmc, we do that in umount */

    FS_Up();
    
    // 这里的@mount_point_i并不会释放，直到该设备被御载。
    return ERR_SUCCESS;
    
step2_failed:    
    if ( bMountFromMMC )
    {
        //DRV_CloseMMC( NULL );
    }

    sb->s_dev = INVALID_DEVICE_NUMBER;
    
step1_failed:
    iput( mount_point_i );
step0_failed:
    FS_Up();
    return iRet;
}


INT32 FS_Unmount( PCWSTR pszMountPoint, BOOL bForce )
{
    struct inode *mount_point_i, *inode;
    struct super_block * sb;
    INT32 iRet;
    INT32 i;
    UINT8 DevName[ 8 ] = { 0, };
    UINT32 fd;
    UINT16 iULen = 0;

    iULen = (UINT16)DSM_TcStrLen(pszMountPoint);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    // file name character validate.
    if( !is_file_name_valid( pszMountPoint, FALSE ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Unmount , pathname is invalid");
        return  ERR_FS_INVALID_PARAMETER;
    }
       
    FS_Down();

    // 参数传进来的是挂载点的路径，namei()里面调iget()时，
    // 挂载点的i节点实际上会被换成被挂载设备的根目录i节点。
    mount_point_i = namei( pszMountPoint );  

    if( !mount_point_i )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Mountpoint does not exist" );
        iRet = ERR_FS_INVALID_DIR_ENTRY;
        goto step0_failed;
    }
    
    if(!FS_INODE_IS_DIR( mount_point_i->i_mode ))  // mount point is not directory
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Mountpoint MUST be a directory file" );
        iRet = ERR_FS_NOT_DIRECTORY;
        goto step1_failed;
    }
    
    /*
    不能根据mount_point_i->i_is_mountpoint是否为TRUE来判断pszOEM下
    是否已经mount了一个文件系统,但可以根据mount_point_i的i_ino是否是FS_ROOT_INO
    来判断,在FS_Mount()中采用了该方法
    */
    for ( i = 0, sb = g_fs_super_block_table; i < (INT32)g_fs_cfg.uNrSuperBlock; i++, sb++ )
    {
        if ( INVALID_DEVICE_NUMBER != sb->s_dev && sb->s_root == mount_point_i )
        {
            _ASSERT_( FS_ROOT_INO == mount_point_i->i_ino );
            break;
        }
    }
    if( (INT32)g_fs_cfg.uNrSuperBlock == i ) // super block not found
    {
        SVC_FSS_TRACE( DL_VFSERROR, "NO file system mounts on [%s]", pszMountPoint );
        iRet = ERR_FS_UNMOUNT_NOT_FIND_SB;
        goto step1_failed;
    }

    iRet = DRV_DevNr2Name( sb->s_dev, DevName );
    if ( ERR_SUCCESS != iRet )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "Device not found for mountpoint [%s]", pszMountPoint );
        iRet = ERR_FS_UNMOUNT_INVALID_DEV_NUMBER;
        goto step1_failed;
    }

    if ( !bForce )
    {
        for ( inode = sb->inode_chain; inode != NULL; inode = inode->next )
        {
            /*why we skip mount_point_i? because it is surely in inode table:-)*/
            if ( inode->i_count > 0 && inode->i_dev == sb->s_dev && inode != mount_point_i )
            {
                SVC_FSS_TRACE( DL_VFSERROR, "File system to unmount is being used" );
                iRet = ERR_FS_UNMOUNT_FS_BUSY;
                goto step1_failed;
            }
        }
    }
    else
    {
        struct file *f;
        for ( fd = 0; fd < g_fs_cfg.uNrOpenFileMax; fd++ )
        {
            f = g_fs_current.filp[ fd ];
            if (!f)
            {
                continue;
            }
            
            inode = f->f_inode;
            if ( inode->i_dev == sb->s_dev )
            {
                if(f->f_flags & FS_O_FIND) // fd is for finding
                {
                    iRet = fs_findclose( fd );
                    if( ERR_SUCCESS != iRet )
                    {
                        SVC_FSS_TRACE( DL_VFSERROR, "FindClose(%d) failed[%d]", fd, iRet );
                        iRet = fs_SetErrorCode(iRet);
                        goto step1_failed;
                    }
                }
                else
                {
                    iRet = fs_close(fd);
                    if( ERR_SUCCESS != iRet )
                    {
                        SVC_FSS_TRACE( DL_VFSERROR, "Close(%d) failed[%d]", fd, iRet );
                        iRet = fs_SetErrorCode(iRet);
                        goto step1_failed;
                    }
                }
            }
        }

        // 不管当前的工作目录是否在挂载设备上，都将被换成根设备的根目录？
        // TODO...
        iput( g_fs_current.pwd );
        g_fs_current.pwd = g_fs_current.root;
        g_fs_current.root->i_count++;
    }

    /*How to check mount_point_i is root inode or not?*/
    if( g_fs_current.root != mount_point_i && mount_point_i->i_count != 2 ) // mount point  is busy
    {
        SVC_FSS_TRACE( DL_VFSERROR, "[%s] is under reference", pszMountPoint );
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto step1_failed;
    }
    else if ( g_fs_current.root != mount_point_i )
    {/* Unmount non-root-FS */
        sb->s_mountpoint->i_is_mountpoint = FALSE;
        iput( sb->s_root ); //i.e, iput( mount_point_i );
        iput( sb->s_mountpoint );
    }
    else
    {
        /* Unmount root-FS from "/." is permitted */
        sb->s_mountpoint->i_is_mountpoint = FALSE;
        iput( sb->s_root ); //i.e, iput( mount_point_i );
        iput( sb->s_mountpoint );
        iput(g_fs_current.pwd);
        iput(g_fs_current.root);
        
        g_fs_current.pwd = NULL;
        g_fs_current.root = NULL;
        memset( g_fs_current.filp, 0x00, (sizeof( struct file * ) ) * g_fs_cfg.uNrOpenFileMax);
    }
    
    sb->s_dev = INVALID_DEVICE_NUMBER;
    memset( sb, 0, sizeof(*sb) );
    
    if ( 0 == DSM_StrNCaselessCmp( "mmc", DevName, 3 ) )
    {
        //iRet = DRV_CloseMMC( NULL );
    }
 
    iRet = ERR_SUCCESS;
    
step1_failed:
    iput( mount_point_i );
step0_failed:    
    FS_Up();
    return iRet;
}


/*
Upon successful completion, TRUE is returned, else FALSE is returned. 
*/
INT32 FS_ChangeDir( PCWSTR pszDirName )
{
    WCHAR *path;
    struct inode * dir_i, * inode;
    struct dentry *psDe = NULL;
    WCHAR *basename;
    INT32 namelen;
    long lErrCode;
    UINT16 iULen = 0;
    
    iULen = (UINT16)DSM_TcStrLen(pszDirName);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    if( !is_file_name_valid( pszDirName, FALSE ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_ChangeDir, pszDirName is invalid" );
        return ERR_FS_INVALID_PARAMETER;
    }

    path = format_dir_name( pszDirName );
    if(!path)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_ChangeDir, path is invalid" );
        return ERR_FS_NO_MORE_MEMORY;
    }

    FS_Down();
    /*namei()--begin*/
    dir_i = dir_namei( path, &namelen, (CONST WCHAR**)&basename );
    if ( !dir_i )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_ChangeDir, dir_i is invalid" );
        lErrCode = ERR_FS_PATHNAME_PARSE_FAILED;
        goto step0_failed;
    }

    if ( !namelen ) // path is invalid
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_ChangeDir, namelen = %d",namelen);
        lErrCode = ERR_FS_NO_BASENAME;
        goto step1_failed;
    }

    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_ChangeDir, malloc psDe failed!.\n");
        lErrCode = ERR_FS_NO_MORE_MEMORY;
        goto step1_failed;
    }
    memset( psDe, 0, sizeof( *psDe ));
    DSM_TcStrCpy( psDe->name, basename);

    lErrCode = find_entry( &dir_i, psDe );
    if(lErrCode)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_ChangeDir, find_entry error lErrCode = %d",lErrCode);
      //  lErrCode = ERR_FS_NO_DIR_ENTRY;
        goto step1_failed;
    }

    inode = psDe->inode;
    /*namei()--end*/

    if ( !FS_INODE_IS_DIR( inode->i_mode ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_ChangeDir, inode is not dir");
        iput( inode );
        lErrCode = ERR_FS_NOT_DIRECTORY;
        goto step1_failed;
    }

    lErrCode = fs_SetWorkDirPath(g_fs_current.wdPath, pszDirName);
    if (lErrCode != ERR_SUCCESS)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "FS_ChangeDir: fs_SetWorkDirPath return %d", lErrCode);
        iput(inode);
        goto step1_failed;
    }
    
    iput( g_fs_current.pwd );
    g_fs_current.pwd = inode;   /*we have add the refcnt for inode in find_entry()*/

step1_failed:
    iput( dir_i );
    
    if (psDe != NULL)
    {
        DENTRY_FREE(psDe);
    }
    
step0_failed:
    if(path != NULL)
    {
        NAME_BUF_FREE(path);
    }

    FS_Up();
    return fs_SetErrorCode(lErrCode);
}

INT32 FS_Open(PCWSTR pszFileName, UINT32 iFlag, UINT32 iMode)
{
    INT32 fhd;
    struct inode * dir_i = NULL;
    struct file * f = NULL;
    WCHAR *basename = NULL;
    INT32 namelen;
    struct dentry *psDe = NULL;
    INT32 lErrCode = ERR_SUCCESS;
    UINT16 iULen = 0;
    
    //cheat compiler
    iMode = iMode;

    iULen = (UINT16)DSM_TcStrLen(pszFileName);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    // file name character validate.
    if( !is_file_name_valid( pszFileName, FALSE ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open ,filename is invalid");
        return  ERR_FS_INVALID_PARAMETER;
    }

    // To truncate the file, we should have the write access to the file
    if ( ( iFlag & FS_O_TRUNC ) && ( ( iFlag & FS_O_ACCMODE ) == FS_O_RDONLY ) )
    {
        SVC_FSS_TRACE(DL_FATERROR, "FS_Open: ERR_FS_INVALID_PARAMETER1.\n");
        return ERR_FS_INVALID_PARAMETER;
    }

    // To append the file, we should have the write access to the file
    if ( ( iFlag & FS_O_APPEND ) && ( ( iFlag & FS_O_ACCMODE ) == FS_O_RDONLY ) )
    {
        SVC_FSS_TRACE(DL_FATERROR, "FS_Open: ERR_FS_INVALID_PARAMETER2.\n");
        return ERR_FS_INVALID_PARAMETER;
    }

    FS_Down();
    
    // Get a valid file descriptor
    for ( fhd = 0; fhd < (INT32)g_fs_cfg.uNrOpenFileMax; fhd++ )
    {
        if ( NULL == g_fs_current.filp[ fhd ] )                 /*unused fhd found*/
            break;
    }

    if(fhd == (INT32)g_fs_cfg.uNrOpenFileMax)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "FS_Open: FS_NR_OPEN_FILES_MAX.\n");
        lErrCode = ERR_FS_FDS_MAX;
        goto step0_failed;
    }

    f = fs_alloc_fd(); // 向内存动态分配一个新的文件描述符。
    if (NULL == f)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open, fs_alloc_fd() return NULL");
        lErrCode = ERR_FS_NO_MORE_MEMORY;
        goto step0_failed;
    }

    f = fs_fd_list_add(&g_fs_file_list, f); // 添加到文件描述符链表。
    
    // 得到最顶层目录的节点。
    dir_i = dir_namei( pszFileName, &namelen, (CONST WCHAR**)&basename );
    if ( !dir_i )
    {
        SVC_FSS_TRACE(DL_FATWARNING, "FS_Open: ERR_FS_PATHNAME_PARSE_FAILED.\n");
        lErrCode = ERR_FS_PATHNAME_PARSE_FAILED;
        goto step0_failed;
    }
    if(0 == namelen)
    {
        lErrCode = ERR_FS_NO_BASENAME;
        goto step1_failed;
    }

    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open, malloc psDe failed!.\n");
        lErrCode = ERR_FS_NO_MORE_MEMORY;
        goto step1_failed;
    }

    memset( psDe, 0, sizeof( *psDe ) );
    DSM_TcStrCpy( psDe->name, basename );

    // 在上面得到的目录中查找是否存在要打开的文件。
    // 如找到，@psDe里面将得到i节点。
    if ( 0 == find_entry( &dir_i, psDe ) )
    {
        SVC_FSS_TRACE( DL_FATDETAIL, "in FS_Open, ------------------------------------->psDe.inode.i_ino = %d\n",psDe->inode->i_ino);
        // If the file is existd, creating action must be done
        if(( iFlag & FS_O_CREAT ) && ( iFlag &FS_O_EXCL )) 
        {
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open the file already existed, and flag is FS_O_CREAT");
            lErrCode = ERR_FS_FILE_EXIST;
            goto step2_failed;
        }

        // 找到是一个目录而不是文件则出错。
        if ( FS_INODE_IS_DIR( psDe->inode->i_mode ) ) // Attempt to open a directroy file
        {
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open, the inode is not a file,but a dir\n");
            lErrCode = ERR_FS_IS_DIRECTORY;
            goto step2_failed;
        }

        goto check_if_trunc;
    }

    if ( !( iFlag & FS_O_CREAT ) ) // entry not found
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open the file not existed!");
        lErrCode = ERR_FS_NO_DIR_ENTRY;
        goto step1_failed;
    }
    
    /*
    *  如果要打开的文件不存在
    *  而创建标志已置位，则创建文件。
    */

    // FS_O_CREAT flag is set
    switch ( dir_i->i_fstype )
    {
    case FS_TYPE_FFS:
        break;

    case FS_TYPE_FAT:       
        // 如果前面没找到该文件，并且创建标志已置位，则创建。
        lErrCode = fat_create_file( dir_i, psDe, INODE_TYPE_REGULAR );        
        if(_ERR_FAT_SUCCESS != lErrCode)
        {            
            SVC_FSS_TRACE( DL_FATERROR,
          		"in FS_Open() fat_create_file() failed,lErrCode = %d.\n",
          		lErrCode
        	  ); 
            goto step1_failed;
        }
        break;

    default: // unknown fs type
        lErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open, unknown file system type\n");
        goto step1_failed;
        break;
    }

    goto exit_success;

    // 如果前面找到了该文件，则检查是否要将文件截短。
check_if_trunc:
    if ( iFlag & FS_O_TRUNC )
    {
       switch ( dir_i->i_fstype )
       {
        case FS_TYPE_FAT:
            lErrCode = fat_trunc_file( dir_i, psDe, iMode );
            if(ERR_SUCCESS != lErrCode) // create file failed
            {
                SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open, trunc file error, ErrorCode = %d\n",lErrCode);
                //lErrCode = ERR_FS_TRUNC_FILE_FAILED;
                goto step2_failed;
            }
            break;

        default:
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_Open, unknown file system type_1\n");
            lErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
            goto step2_failed;
            break;
        }
    }    
exit_success:
    // TODO: 此时才关联，是否会让其他process抢了该file{}
    g_fs_current.filp[ fhd ] = f;
    f->f_mode = psDe->inode->i_mode;
    f->f_flags = ( UINT16 ) iFlag;
    f->f_count = 1;
    f->f_inode = psDe->inode;
    /* Whether FS_O_APPEND flag is specified or not, we always set file pointer to 0,
         because we may read the file before the first write. */
    f->f_pos = 0;

    iput( dir_i );
    if (psDe != NULL)
    {
        DENTRY_FREE(psDe);
    }
    FS_Up();
    return fhd;
    
step2_failed:
    iput( psDe->inode );
    
step1_failed:
    iput( dir_i );

    if (psDe != NULL)
    {
        DENTRY_FREE(psDe);
    }
    
step0_failed:
    if (f != NULL)
    {
        fs_fd_list_del(&g_fs_file_list, f);
    }
    FS_Up();
    return fs_SetErrorCode(lErrCode);
}


INT32 FS_Create(PCWSTR pszFileName, UINT32 iMode)
{
    return FS_Open( pszFileName, FS_O_RDWR | FS_O_CREAT | FS_O_TRUNC, iMode );
}

INT32 FS_Close(INT32 fd)
{
    INT32 iRet; 
    
    FS_Down();
    iRet = fs_close(fd);
    FS_Up();
    
    return iRet;    
}

INT32 FS_Delete(PCWSTR pszFileName)
{
    WCHAR* basename;
    INT32 namelen;
    struct inode *dir_i;
    struct dentry *psDe = NULL;
    INT32 iRet;
    INT32 lErrCode = ERR_SUCCESS;
    UINT16 iULen = 0;
   
    iULen = (UINT16)DSM_TcStrLen(pszFileName);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }

    if(!is_file_name_valid( pszFileName, FALSE )) // invalid pathname
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, FileName is invalid\n");
        return ERR_FS_INVALID_PARAMETER;
    }

    FS_Down();
    
    dir_i = dir_namei( pszFileName, &namelen, (CONST WCHAR**)&basename );
    if( !dir_i ) // path is invalid
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, dir is invalid\n");
        lErrCode = ERR_FS_PATHNAME_PARSE_FAILED;
        goto step0_failed;
    }
    
    if(!namelen) // file name not specified
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, namelen = %d\n",namelen);
        lErrCode = ERR_FS_NO_BASENAME;
        goto step1_failed;
    }

    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, malloc psDe failed!.\n");
        lErrCode = ERR_FS_NO_MORE_MEMORY;
        goto step1_failed;
    }
    
    memset( psDe, 0, sizeof( *psDe ) );
    DSM_TcStrCpy( psDe->name, basename );

    iRet = find_entry( &dir_i, psDe );
    if(-1 == iRet)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, find_entry return -1, find no entry\n");
        lErrCode = ERR_FS_NO_DIR_ENTRY;
        goto step1_failed;
    }

    if(FS_INODE_IS_DIR( psDe->inode->i_mode )) // entry is directory
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, entry is directoryy\n");
        lErrCode = ERR_FS_IS_DIRECTORY;
        goto step2_failed;
    }

    if(psDe->inode->i_dev != dir_i->i_dev) // Not permition
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, psDe.inode->i_dev = %d, dir_i->i_dev = %d\n",psDe->inode->i_dev,dir_i->i_dev);
        lErrCode = ERR_FS_OPERATION_NOT_GRANTED;
        goto step2_failed;
    }

    // Some process is dealing with the file. It will not be removed.
    if(psDe->inode->i_count > 1)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, psDe.inode->i_count = %d\n",psDe->inode->i_count);
        lErrCode = ERR_FS_OPERATION_NOT_GRANTED;
        goto step2_failed;
    }
        
    if ( FS_TYPE_FAT == dir_i->i_fstype )
    {
        iRet = fat_do_unlink( dir_i, psDe, FALSE );
        if(ERR_SUCCESS != iRet) // unlink failed.
        {
            lErrCode = fs_SetErrorCode(iRet);
            goto step2_failed;
        }
    }
    else  // unknown filesystem type
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Delete, unknown file system type\n");
        lErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
        goto step2_failed;
    }
        
    dir_i->i_mtime = DSM_GetFileTime();
    
    lErrCode = ERR_SUCCESS;

step2_failed:
    iput( psDe->inode );
step1_failed:
    iput( dir_i );

    if (psDe != NULL)
    {
        DENTRY_FREE(psDe);
    }
step0_failed:
    FS_Up();
    return lErrCode;
}


INT32 FS_Seek(INT32 fd, INT32 iOffset, UINT8 iOrigin)
{
    struct file * file;
    INT32 tmp;
    INT32 iRet = ERR_SUCCESS;

    
    if ( !FD_IS_VALID( fd ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Seek, invalide fd = %d\n",fd);
        return ERR_FS_BAD_FD;
    }

 //   FS_Down();
    
    file = g_fs_current.filp[ fd ];

    if ( !( file ) || !fs_is_file_opened(&g_fs_file_list, file) || !( file->f_inode ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Seek, file is invalid\n");
 //       FS_Up();
        return ERR_FS_INVALID_PARAMETER;
    }

    if ( file->f_flags & FS_O_FIND ) // handle is for finding
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Seek, file->f_flags is for finding, = %d\n",file->f_flags);
  //      FS_Up();
        return ERR_FS_OPERATION_NOT_GRANTED;
    }

    switch ( iOrigin )
    {
        case FS_SEEK_SET:
            tmp = 0 + iOffset;
            file->f_pos = tmp < 0 ? 0 : (tmp > (INT32)file->f_inode->i_size ? file->f_inode->i_size: tmp);
            iRet = file->f_pos;
            break;
            
        case FS_SEEK_CUR:
            tmp = file->f_pos + iOffset;
            file->f_pos = tmp < 0 ? 0 : (tmp > (INT32)file->f_inode->i_size ? file->f_inode->i_size: tmp);
            iRet = file->f_pos;
            break;
            
        case FS_SEEK_END:
            tmp = file->f_inode->i_size + iOffset;
            file->f_pos = tmp < 0 ? 0 : (tmp > (INT32)file->f_inode->i_size ? file->f_inode->i_size: tmp);
            iRet = file->f_pos;
            break;
            
        default:
            iRet = ERR_FS_INVALID_PARAMETER;
    }

 //   FS_Up();
    return iRet;    
}


INT32 FS_IsEndOfFile(INT32 fd)
{
    struct file * filp;
    INT32 iRet;

    if(!FD_IS_VALID( fd ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_IsEndOfFile, fd = %d, is invalid\n",fd);
        return ERR_FS_BAD_FD;
    }
    
    FS_Down();
    
    filp = g_fs_current.filp[ fd ];
    if(!filp || !fs_is_file_opened(&g_fs_file_list, filp)) // fd has closed
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }
    if ( filp->f_flags & FS_O_FIND ) // handle is for finding
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_IsEndOfFile, filp->f_flags = %d\n",filp->f_flags);
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }

    iRet = (( filp->f_inode->i_size == (UINT32)filp->f_pos ) ? 1 : 0);

label_exit:

    FS_Up();
    return iRet;    
}


/*WARNING:假设错误码为负数*/
INT32 FS_Read(INT32 fd, UINT8 *pBuf, UINT32 iLen)
{
    struct file * file;
    struct inode * inode;
    int iLocalLen;
    INT32 iResult = 0;
    INT32 iRet;

    if( NULL == pBuf || iLen < 0)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Read, parameter error, iLen = %d\n",iLen);
        return ERR_FS_INVALID_PARAMETER;
    }
    
    if ( !FD_IS_VALID( fd ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Read, parameter error, fd = %d\n",fd);
        return ERR_FS_BAD_FD;
    }
    
    FS_Down();
    
    file = g_fs_current.filp[ fd ];

    if ( iLen < 0 || !file || !fs_is_file_opened(&g_fs_file_list, file))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Read, iLen = %d\n",iLen);
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }
    
    iLocalLen = iLen;

    if(file->f_flags & FS_O_FIND) // handle is for finding
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Read, 0_file->f_flags = %d\n",file->f_flags);
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }
    if (FS_O_WRONLY == ( file->f_flags & FS_O_ACCMODE ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Read, 1_file->f_flags = %d\n",file->f_flags);
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }

    if ( 0 == iLocalLen )
    {
        iRet = 0;
        goto label_exit;
    }
    
    inode = file->f_inode;

    if ( !FS_INODE_IS_REG( inode->i_mode ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Read, not regular file\n");
        iRet = ERR_FS_NOT_REGULAR;
        goto label_exit;
    }

    // 如果要读的长度加上当前pos已经
    // 超出文件的大小，就把要读的长度截短。
    if ( iLocalLen + file->f_pos > inode->i_size )
    {
        iLocalLen = inode->i_size - file->f_pos;
    }
    
    if ( iLocalLen <= 0 )/*after seek opteration, len may be negative*/
    {
        iRet = 0;
        goto label_exit;
    }

    if ( FS_TYPE_FAT == inode->i_fstype )
    {
        // 根据取到的节点跟file读文件。
        iResult = fat_file_read( inode, file, (INT8*)pBuf, iLocalLen);
        if(0 <= iResult)
        {
            iRet = iResult;
        }
        else
        {
           SVC_FSS_TRACE( DL_FATERROR, "Call fat_file_read() failed. Local error code:%d.\n", iResult);
           iRet = fs_SetErrorCode(iResult);	           
           goto label_exit;
        }
        
    }
    else
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Read, unknown file system\n");
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        goto label_exit;
    }     
    
label_exit:
    FS_Up();
    return iRet;
}


/*WARNING:假设错误码为负数*/
INT32 FS_Write(INT32 fd, UINT8 *pBuf, UINT32 iLen)
{
    struct file * file;
    struct inode * inode;
    INT32 iResult;
    INT32 iRet;
	
    if( NULL == pBuf || iLen < 0)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Write, invalid parameter\n");
        return ERR_FS_INVALID_PARAMETER;
    }
    
    if( !FD_IS_VALID( fd ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Write, invalid fd = %d\n",fd);
    	return ERR_FS_BAD_FD;
    }
    
    FS_Down();
    
    file = g_fs_current.filp[ fd ];
	
    if (NULL == file || !fs_is_file_opened(&g_fs_file_list, file))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_Write, file is NULL.\n");
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }
	
    if(file->f_flags & FS_O_FIND) // handle is for finding
    {
        SVC_FSS_TRACE( DL_VFSERROR, "0_in FS_Write,file->f_flags  = %d\n",file->f_flags);
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }
	
    if (FS_O_RDONLY == ( file->f_flags & FS_O_ACCMODE ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "1_in FS_Write,file->f_flags  = %d\n",file->f_flags);
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }
	
    if ( 0 == iLen )
    {
        iRet = 0;
        goto label_exit;
    }

    inode = file->f_inode;
    
    if ( !FS_INODE_IS_REG( inode->i_mode ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "1_in FS_Write,file is not regular\n");
        iRet = ERR_FS_NOT_REGULAR;
        goto label_exit;
    }
    
    if ( FS_TYPE_FAT == inode->i_fstype )
    {
        // 根据前面取得的节点及file写该文件。
        iResult = fat_file_write( inode, file, (INT8*)pBuf, iLen );
        if(_ERR_FAT_SUCCESS == iResult)		
        {
            iRet = iLen;
        }
        else
        {
            SVC_FSS_TRACE( DL_VFSERROR, "1_in FS_Write,fat_file_write error\n");
            iRet = fs_SetErrorCode(iResult);			
        }
    }
    else
    {
        SVC_FSS_TRACE( DL_VFSERROR, "1_in FS_Write,unknow filesystem\n");
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
    }      

label_exit:
    FS_Up();
    return iRet;
}


INT32 FS_MakeDir(PCWSTR pszDirName, UINT32 iMode)
{
    WCHAR *path = NULL;
    WCHAR *basename;
    INT32 namelen;
    struct inode *dir_i;
    struct dentry *psDe = NULL;
    long iRet;
    INT32 lErrCode = ERR_SUCCESS;
    UINT16 iULen = 0;
    
    iULen = (UINT16)DSM_TcStrLen(pszDirName);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }

    // file name character validate.
    if(!is_file_name_valid( pszDirName, FALSE ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir,DirName is invalid\n");
        return ERR_FS_INVALID_PARAMETER;
    }
    iMode = INODE_TYPE_DIRECTORY;
    path = format_dir_name( pszDirName );
    if(!path)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir, format_dir_name is NULL\n");
        return ERR_FS_NO_MORE_MEMORY;
    }

    FS_Down();
    
    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir(), malloc psDe failed!.\n");
        lErrCode = ERR_FS_NO_MORE_MEMORY;
        goto step0_failed;
    }
    
    memset(psDe, 0x00, sizeof(struct dentry));
    dir_i = dir_namei( path, &namelen, (CONST WCHAR**)&basename );
    if ( !dir_i ) // path is invalid
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir,dir_i is NULL\n");
        lErrCode = ERR_FS_PATHNAME_PARSE_FAILED;
        goto step0_failed;
    }
    if(0 == namelen) // namelen is 0
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir,namelen = %d\n",namelen);
        lErrCode = ERR_FS_NO_BASENAME;
        goto step1_failed;
    }

    DSM_TcStrCpy( psDe->name, basename);

    // TODO 
    // 需要改造find_entry,应该返回具体的错误码！
    // 在此处把内部错误码转换成对应的外部错误码
    iRet = find_entry( &dir_i, psDe );
    if(!iRet)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir,find_entry error, iRet = %d\n",iRet);
        lErrCode = ERR_FS_FILE_EXIST;
        goto step1_failed;
    }
    
    if ( FS_TYPE_FFS == dir_i->i_fstype )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir,find_entry error, iRet = %d\n",iRet);
        lErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
        goto step1_failed;
    }
    else if ( FS_TYPE_FAT == dir_i->i_fstype )
    {
        iRet = fat_create_file( dir_i, psDe, INODE_TYPE_DIRECTORY );
        if(iRet)
        {	
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir,fat_create_file error, iRet = %d\n",iRet);
	     lErrCode = fs_SetErrorCode(iRet);			
            goto step1_failed;
        }
    }
    else
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_MakeDir,unknown file system\n");
        lErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
        goto step1_failed;
    }

    lErrCode = ERR_SUCCESS;

step1_failed:
    if ( psDe->inode )
    {
        iput( psDe->inode );
    }
    iput( dir_i );

    if (psDe != NULL)
    {
        DENTRY_FREE(psDe);
    }
    
step0_failed:
    if(path != NULL)
    {
        NAME_BUF_FREE(path);
    }

    FS_Up();
    return lErrCode;    
}


INT32 FS_RemoveDir(PCWSTR pszDirName)
{
    WCHAR *path;
    WCHAR * basename;
    INT32 namelen;
    struct inode *dir_i;
    struct dentry *psDe = NULL;
    INT32 iRet;
    INT32 lErrCode = ERR_SUCCESS;
    UINT16 iULen = 0;

    iULen = (UINT16)DSM_TcStrLen(pszDirName);
    if(0 == iULen || iULen > FS_PATH_UNICODE_LEN)
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    if(!is_file_name_valid( pszDirName, FALSE ))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, DirName is invalid\n");
        return ERR_FS_INVALID_PARAMETER;
    }

    path = format_dir_name( pszDirName );
    if(!path)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, path is invalid\n");
        return ERR_FS_NO_MORE_MEMORY;
    }
    
    FS_Down();
    
    dir_i = dir_namei( path, &namelen, (CONST WCHAR**)&basename );
    if ( !dir_i ) // path is invalid
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, dir_i is invalid\n");
        lErrCode = ERR_FS_PATHNAME_PARSE_FAILED;
        goto step0_failed;
    }
    if(0 == namelen) // namelen is 0
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, namelen = %d\n",namelen);
        lErrCode = ERR_FS_NO_BASENAME;
        goto step1_failed;
    }

    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir(), malloc psDe failed!.\n");
        lErrCode = ERR_FS_NO_MORE_MEMORY;
        goto step1_failed;
    }

    // 这里拷贝字符串的长度应该不会超过文件名的最大长度，
    // 因为前面已经做了检查。
    memset( psDe, 0, sizeof( *psDe ) );
    DSM_TcStrCpy( psDe->name, basename);

    iRet = find_entry( &dir_i, psDe ); // directory not found
    if(-1 == iRet)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, find_entry error, iRet = %d\n",iRet);
        lErrCode = ERR_FS_NO_DIR_ENTRY;
        goto step1_failed;
    }

    if(!FS_INODE_IS_DIR( psDe->inode->i_mode )) // Not directory
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, psDe->inode->i_mode = %d\n",psDe->inode->i_mode);
        lErrCode = ERR_FS_NOT_DIRECTORY;
        goto step2_failed;
    }
        
    if(psDe->inode->i_dev != dir_i->i_dev)
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, psDe->inode->i_dev = %d, dir_i->i_dev = %d\n",psDe->inode->i_dev,dir_i->i_dev);
        lErrCode = ERR_FS_OPERATION_NOT_GRANTED;
        goto step2_failed;
    }
    /*
    After calling find_entry(), its refcnt is increased by one. Its refcnt will be greater than 1 when 
    SOME process is accessing the directory. For example, try to delete itself under the directory.
    NOTE: if we run the command "rd ..", the refcnt of the .. inode may be 1, but it will not be 
    empty!!!
    */
    if(psDe->inode->i_count > 1) // Not permition
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, psDe->inode->i_count = %d\n",psDe->inode->i_count);
        lErrCode = ERR_FS_OPERATION_NOT_GRANTED;
        goto step2_failed;
    }

    // i_nlinks字段实际上没有被使用,fat_remove_entry()中会判断目录是否非空。
    /*if(psDe->inode->i_nlinks > 2) // Directory is not empty
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, psDe->inode->i_nlinks = %d\n",psDe->inode->i_nlinks);
        lErrCode = ERR_FS_DIR_NOT_EMPTY;
        goto step2_failed;
    }*/

    if ( FS_TYPE_FFS == dir_i->i_fstype )
    {
        // iRet = ffs_do_unlink( dir_i, psDe, FALSE );
        // if(iRet)
        // {
        //     lErrCode = ERR_FS_FAILURE;
        //     goto step2_failed;
        // }
        SVC_FSS_TRACE( DL_VFSERROR, "1_in FS_RemoveDir, unknown file system\n");
        lErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
        goto step2_failed;
    }
    else if ( FS_TYPE_FAT == dir_i->i_fstype )
    {
        iRet = fat_do_unlink( dir_i, psDe, FALSE );
        if(iRet)
        {
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_RemoveDir, unlink failed iRet = %d\n",iRet);
            lErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
            lErrCode = fs_SetErrorCode(iRet);;
            goto step2_failed;
        }
    }
    else
    {
        SVC_FSS_TRACE( DL_VFSERROR, "0_in FS_RemoveDir, unknown file system\n",iRet);
        lErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
        goto step2_failed;
    }
    
    dir_i->i_mtime = DSM_GetFileTime();
    lErrCode = ERR_SUCCESS;

step2_failed:
    iput( psDe->inode );
    
step1_failed:
    iput( dir_i );
    
    if (psDe != NULL)
    {
        DENTRY_FREE(psDe);
    }
    
step0_failed:
    if(path != NULL)
    {
        NAME_BUF_FREE(path);
    }    
    
    FS_Up();
    return lErrCode;    
}


//Return:
// ERR_SUCCESS
// ERR_FS_INVALID_PARAMETER
// ERR_FS_DEVICE_NOT_REGISTER
// ERR_FS_OPEN_DEV_FAILED
// ERR_FS_NOT_FORMAT
// ERR_FS_NO_MORE_MEMORY
// ERR_FS_UNKNOWN_FILESYSTEM
INT32 FS_ScanDisk(PCSTR pszDevName,UINT8 iFsType)
{    
    INT32 iDevNo;	
    INT32 iResult;
    INT32 iRet;
    BOOL bIsMMC = FALSE;
    

    if ( !pszDevName )
    {
        return ERR_FS_INVALID_PARAMETER;
    }
    
    iRet = ERR_SUCCESS;
    SVC_FSS_TRACE( DL_FATWARNING, "FS_ScanDisk begin,pszDevName = %s, iFsType = %d.\n",
            		pszDevName, iFsType);  
    switch(iFsType)
    {
    case FS_TYPE_FAT:
        iDevNo = DRV_DevName2DevNr(pszDevName);
        if(INVALID_DEVICE_NUMBER == iDevNo)
        {
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_ScanDisk,iDevNo = %d\n",iDevNo);
            SVC_FSS_TRACE( DL_VFSERROR, "in FS_ScanDisk, DevName = %s\n",pszDevName);
            return ERR_FS_DEVICE_NOT_REGISTER;
        }

        if ( ( 0 == DSM_StrNCaselessCmp( "mmc", pszDevName, 3) ) )
        {
            bIsMMC = TRUE;
            iRet = DRV_OpenMMC( NULL );
            if( ERR_SUCCESS != iRet )
            {
                SVC_FSS_TRACE( DL_VFSERROR, "Device[%s]: Open MMC device failed.\n", pszDevName );
                return ERR_FS_OPEN_DEV_FAILED;
            }
        }  

        FS_Down();
        SVC_FSS_TRACE( DL_FATDETAIL, "Call fat_scan_disk() with iDevNo = %d.\n", iDevNo);  
        iResult = fat_scan_disk(iDevNo);
        SVC_FSS_TRACE( DL_FATDETAIL, "fat_scan_disk() return, iResult = %d.\n", iResult);  
        FS_Up();
        
        if(_ERR_FAT_SUCCESS == iResult)
        {
            iRet = ERR_SUCCESS;
        }
        else
        {
            iRet = fs_SetErrorCode(iResult);
        }
    	break;
        
    case FS_TYPE_FFS:
        
    default:
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_ScanDisk, unknown file system\n");
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        break;
    }
        
    if ( bIsMMC )
    {
        //DRV_CloseMMC( NULL );
    }
    
    return iRet;
    
}


// 扩充或缩小文件，文件缩小时如果文件指针file->pos超出文件的范围则
// 将被移到缩小后的文件的末尾。
INT32 FS_ChangeSize(INT32 fd, UINT32 nFileSize)
{
    struct file *file = NULL;
    struct inode *psInode;
    INT32 iRet = ERR_SUCCESS;
    
    if( !FD_IS_VALID( fd ) )
    {
        SVC_FSS_TRACE( DL_FATERROR, "in FS_ChangeSize,fd = %d, ERR_FS_INVALID_PARAMETER",fd);
        return ERR_FS_BAD_FD;
    }

    FS_Down();
    file = g_fs_current.filp[fd];
    if(NULL == file || !fs_is_file_opened(&g_fs_file_list, file))
    {
        SVC_FSS_TRACE( DL_FATERROR, "in FS_ChangeSize, file is NULL\n");
        iRet = ERR_FS_BAD_FD;
        goto end;
    }

    if(file->f_flags & FS_O_FIND) // handle is for finding
    {
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto end;
    }

    if (FS_O_RDONLY == ( file->f_flags & FS_O_ACCMODE ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "FS_ChangeSize, file->f_flags  = %d\n",file->f_flags);
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto end;
    }
    
    psInode = file->f_inode;
    if (nFileSize == psInode->i_size)
    {
        SVC_FSS_TRACE( DL_WARNING, "in FS_ChangeSize, SizeToChange = %d, fileSize = %d", nFileSize, psInode->i_size);
        iRet = ERR_SUCCESS;
        goto end;
    }
    
    switch ( psInode->i_fstype )
    {
    case FS_TYPE_FFS:
        break;

    case FS_TYPE_FAT:
        if (nFileSize > psInode->i_size)
        {
            iRet = fat_file_extend(psInode, file, nFileSize - psInode->i_size);
            if(iRet != _ERR_FAT_SUCCESS)
            {            
                SVC_FSS_TRACE( DL_FATERROR, "FS_ChangeSize: fat_file_extend() failed, iRet = %d.\n", iRet); 
                goto end;
            }
        }
        else
        {
            iRet = fat_file_truncate( psInode, nFileSize );
            if(iRet != _ERR_FAT_SUCCESS)
            {            
                SVC_FSS_TRACE( DL_FATERROR, "FS_ChangeSize: fat_file_truncate() failed, iRet = %d.\n", iRet); 
                goto end;
            }
            if (file->f_pos > psInode->i_size)
            {
                file->f_pos = psInode->i_size;
            }
        }
        iRet = ERR_SUCCESS;
        break;

    default: // unknown fs type
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        SVC_FSS_TRACE( DL_VFSERROR, "FS_ChangeSize: unknown file system type\n");
        goto end;
    }
    
end:
    if(iRet < 0)
    {
        iRet = fs_SetErrorCode(iRet);
    }
    FS_Up();
    return iRet; 

}


INT32 FS_Flush(INT32 fd)
{
    struct file * filp;
    INT32 iRet;

    if( !FD_IS_VALID( fd ) ) // fd is invalid
    {
        return ERR_FS_BAD_FD; 
    }

    FS_Down();
    filp = g_fs_current.filp[ fd ];

    if(!filp || !fs_is_file_opened(&g_fs_file_list, filp)) // fd has closed
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if(filp->f_flags & FS_O_FIND) // handle is for finding
    {
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }
    
    if(!filp->f_count) // f_count should not be ZERO
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    switch ( filp->f_inode->i_fstype )
    {
    case FS_TYPE_FFS:
        iRet = ERR_FS_NOT_SUPPORT;
        goto label_exit;        
        break;

    case FS_TYPE_FAT:       
        iRet = fat_write_inode( filp->f_inode, FALSE ); // 将节点写回磁盘但不释放。
        if(iRet != _ERR_FAT_SUCCESS)
        {            
            SVC_FSS_TRACE( DL_FATERROR, "FS_Flush: fat_write_inode() failed, iRet = %d.\n", iRet); 
            goto label_exit;
        }
        else
        {
            iRet = ERR_SUCCESS;
        }
        break;

    default: // unknown fs type
        iRet = ERR_FS_UNKNOWN_FILESYSTEM;
        SVC_FSS_TRACE( DL_VFSERROR, "FS_Flush: unknown file system type\n");
        goto label_exit;
    }
    
label_exit:
    FS_Up();
    return iRet;
}


INT32 FS_GetFileSize(INT32 fd)
{
    struct file *file;
    struct inode *inode;
    
    if( !FD_IS_VALID( fd ) )
    {
        SVC_FSS_TRACE( DL_FATERROR, "in FS_GetFileSize,fd is invalid ,= % d\n",fd);
        return ERR_FS_BAD_FD;
    }
    FS_Down();
    file = g_fs_current.filp[fd];
    if(NULL == file || !fs_is_file_opened(&g_fs_file_list, file))
    {
        FS_Up();
        SVC_FSS_TRACE( DL_FATERROR, "in FS_GetFileSize,file is NULL\n");
        return ERR_FS_BAD_FD;
    }
    
    if(file->f_flags & FS_O_FIND) // handle is for finding
    {
        FS_Up();
        SVC_FSS_TRACE( DL_FATERROR, "in FS_GetFileSize, handle is for finding\n");
        return ERR_FS_OPERATION_NOT_GRANTED;
    }
     inode = file->f_inode;
     if ( !FS_INODE_IS_REG( inode->i_mode ) )
    {
        FS_Up();
        SVC_FSS_TRACE( DL_FATERROR, "in FS_GetFileSize,file is not regular \n");
        return ERR_FS_NOT_REGULAR;
        
    }
    else
    {
        SVC_FSS_TRACE( DL_FATDETAIL, "iSize = %d\n",inode->i_size);
        FS_Up();
        return inode->i_size;
    }
}


INT32 FS_GetFileName(INT32 hFile, INT32 iNameBufferSize, PWSTR pFileName)
{
    struct file * psFile;
    struct inode * psInode;
    WCHAR *pUniName = NULL;
    UINT16 uUniLen;
    UINT32 uErrCode;
    
    if( !FD_IS_VALID( hFile ) )
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_GetFileName, invalid fd = %d\n",hFile);
        uErrCode = ERR_FS_BAD_FD;
        return uErrCode;
    }
    pUniName = NAME_BUF_ALLOC();
    if (!pUniName)
    {
        uErrCode = ERR_FS_NO_MORE_MEMORY;
        return uErrCode;
    }
    
    FS_Down();
    psFile = g_fs_current.filp[ hFile ];	
    if (NULL == psFile || !fs_is_file_opened(&g_fs_file_list, psFile))
    {
        SVC_FSS_TRACE( DL_VFSERROR, "in FS_GetFileName, psFile is NULL.\n");
        uErrCode = ERR_FS_BAD_FD;
        goto failed;
    }
    if(psFile->f_flags & FS_O_FIND) // handle is for finding
    {
        FS_Up();
        SVC_FSS_TRACE( DL_FATERROR, "in FS_GetFileName, handle is for finding\n");
        uErrCode = ERR_FS_OPERATION_NOT_GRANTED;
        goto failed;
    }
    psInode = psFile->f_inode;
    
    if ( FS_TYPE_FAT == psInode->i_fstype )
    {
        // 根据前面取得的节点取该文件的文件名。
        uErrCode = (UINT32)fat_get_file_name( psInode,  pUniName);
        if(_ERR_FAT_SUCCESS == uErrCode)		
        {
            //DSM_MemZero(pUniName, FAT_MAX_LONG_PATH_LEN + 2);
            uUniLen = (UINT16)DSM_TcStrLen(pUniName);
            if ((UINT32)(uUniLen + LEN_FOR_NULL_CHAR) > iNameBufferSize / SIZEOF(WCHAR))
            {
                uErrCode = ERR_FS_NAME_BUFFER_TOO_SHORT;
                goto failed;
            }
            DSM_TcStrCpy(pFileName, pUniName);
            uErrCode = ERR_SUCCESS;
        }
        else
        {
            SVC_FSS_TRACE( DL_VFSERROR, "fat_get_file_name error, errorCode = %d.\n", uErrCode);
            uErrCode = fs_SetErrorCode(uErrCode);			
        }
    }
    else
    {
        SVC_FSS_TRACE( DL_VFSERROR, "FS_GetFileName: unknow filesystem\n");
        uErrCode = ERR_FS_UNKNOWN_FILESYSTEM;
    }      

failed:
    if (pUniName)
    {
        NAME_BUF_FREE(pUniName);
    }
    FS_Up();
    return uErrCode;
}


INT32 FS_PowerOn(VOID)
{
/*     UINT8 iFsType = FS_TYPE_FAT; */
/*     HKEY hRKey = REG_HKEY_SYSTEM_ROOT; */
/*     const UINT8* pszSKName= (UINT8*)"FS"; */
/*     const UINT8* pszVName = (UINT8*)"PSTATUS"; */
/*     HKEY hKey = 0; */
/*     UINT8 iType = 0; */
/*     UINT8 iStatus = 0; */
/*     PVOID pData = NULL; */
/*     UINT8 iDataSize = SIZEOF(UINT8); */
    INT32 iResult;
    INT32 iRet = ERR_SUCCESS;


    iResult = DRV_MMCPowerUp(); // MMC设备信号量初始化。
    if (iResult != ERR_SUCCESS)
    {
        SVC_FSS_TRACE(DL_FATERROR, "FS_PowerOn: DRV_MMCPowerUp() failed, errorCode = %d.", iResult);    
    }

    iResult = fs_GlobalVarInit();
    if (iResult != ERR_SUCCESS)
    {
        SVC_FSS_TRACE(DL_FATERROR, "FS_PowerOn: fs_GlobalVarInit() failed, errorCode = %d.", iResult);
        return iResult;
    }
    
/*     iResult = REG_OpenKey(hRKey,pszSKName,&hKey); */
/*     if(ERR_SUCCESS == iResult) */
/*     { */
/*         pData = (PVOID)&iStatus; */
/*         iResult = REG_GetValue(hKey,pszVName,&iType,pData,&iDataSize); */
/*         if(ERR_SUCCESS == iResult) */
/*         { */
/*             if(FS_POWER_ON == iStatus) */
/*             {                 */
/*                 iResult = FS_ScanDisk((PCSTR)"vds0", iFsType);  */
/*                 if (ERR_SUCCESS == iResult) */
/*                 { */
/*                     SVC_FSS_TRACE( DL_FATDETAIL, "scan vds0 success.\n",iResult); */
/*                 } */
/*                 else */
/*                 { */
/*                     SVC_FSS_TRACE( DL_FATERROR, "scan vds0 failing. error code = %d.\n",iResult); */
/*                     iRet = iResult;                                     */
/*                 } */
/*                 iResult = FS_ScanDisk((PCSTR)"vds1", iFsType);                 */
/*                 if (ERR_SUCCESS == iResult) */
/*                 { */
/*                     SVC_FSS_TRACE( DL_FATDETAIL, "scan vds1 success.\n",iResult);                                         */
/*                 } */
/*                 else */
/*                 { */
/*                     SVC_FSS_TRACE( DL_FATERROR, "scan vds1 failed. error code = %d.\n",iResult); */
/*                     iRet = iResult; */
/*                 } */
/*             } */
/*             else */
/*             { */
/*                 iStatus = FS_POWER_ON; */
/*                 iResult = REG_SetValue(hKey,pszVName,iType,pData,iDataSize); */
/*                 if(ERR_SUCCESS != iResult) */
/*                 { */
/*                     iRet = ERR_FS_ACCESS_REG_FAILED; */
/*                 }                 */
/*             } */
/*         } */
/*         else */
/*         { */
/*             SVC_FSS_TRACE( DL_VFSERROR, "in FS_PowerOn, REG_GetValue Error, iResult = %d\n",iResult); */
/*             iRet = ERR_FS_ACCESS_REG_FAILED; */
/*         } */
/*         REG_CloseKey(hKey); */
/*     } */
/*     else */
/*     { */
/*         SVC_FSS_TRACE( DL_VFSERROR, "in FS_PowerOn, REG_OpenKey Error, iResult = %d\n",iResult); */
/*         iRet = ERR_FS_ACCESS_REG_FAILED; */
/*     } */
    
    return iRet;
}


INT32 FS_PowerOff(VOID)
{
/*     HKEY hRKey = REG_HKEY_SYSTEM_ROOT; */
/*     const UINT8* pszSKName= (UINT8*)"FS"; */
/*     const UINT8* pszVName = (UINT8*)"PSTATUS"; */
/*     HKEY hKey = 0; */
/*     UINT8 iType; */
/*     UINT8 iStatus; */
/*     PVOID pData = (PVOID)&iStatus; */
/*     UINT8 iDataSize; */
/*     INT32 iResult; */
    INT32 iRet = ERR_SUCCESS;

    DRV_MMCPowerDown();
    fs_GlobalVarFree();
/*     iResult = REG_OpenKey(hRKey,pszSKName,&hKey); */
/*     if(ERR_SUCCESS == iResult) */
/*     { */
        
/*         iStatus = FS_POWER_OFF; */
/*         iType = REG_TYPE_INT8; */
/*         iDataSize = SIZEOF(UINT8); */
/*         iResult = REG_SetValue(hKey,pszVName,iType,pData,iDataSize); */
/*         if(ERR_SUCCESS != iResult) */
/*         { */
/*             SVC_FSS_TRACE( DL_VFSERROR, "in FS_PowerOff, REG_SetValue Error, iResult = %d\n",iResult); */
/*             iRet = ERR_FS_ACCESS_REG_FAILED;          */
/*         } */
/*         REG_CloseKey(hKey); */
/*     } */
/*     else */
/*     { */
/*         SVC_FSS_TRACE( DL_VFSERROR, "in FS_PowerOff, REG_OpenKey Error, iResult = %d\n",iResult); */
/*         iRet = ERR_FS_ACCESS_REG_FAILED; */
/*     } */
    
    return iRet;        
}



INT32 fs_SetErrorCode(INT32 iErrCode)
{    
    INT32 iRet = ERR_SUCCESS;
    
    switch(iErrCode)
    {
        case _ERR_FAT_SUCCESS:          iRet = ERR_SUCCESS                 ;break;
        case _ERR_FAT_PARAM_ERROR:      iRet = ERR_FS_INVALID_PARAMETER    ;break;
        case _ERR_FAT_WRITE_SEC_FAILED: iRet = ERR_FS_WRITE_SECTOR_FAILED  ;break;
        case _ERR_FAT_READ_SEC_FAILED:  iRet = ERR_FS_READ_SECTOR_FAILED   ;break;
        case _ERR_FAT_MALLOC_FAILED:    iRet = ERR_FS_NO_MORE_MEMORY       ;break;
        case _ERR_FAT_DISK_FULL:        iRet = ERR_FS_DISK_FULL            ;break;
        case _ERR_FAT_ROOT_FULL:        iRet = ERR_FS_ROOT_FULL            ;break;
        case _ERR_FAT_READ_EXCEED:      iRet = ERR_FS_READ_FILE_EXCEED     ;break;
        case _ERR_FAT_WRITE_EXCEED:     iRet = ERR_FS_WRITE_FILE_EXCEED    ;break;
        case _ERR_FAT_NOT_SUPPORT:      iRet = ERR_FS_NOT_SUPPORT          ;break;
        case _ERR_FAT_BPB_ERROR:        iRet = ERR_FS_DATA_DESTROY         ;break;
        case _ERR_FAT_FILE_TOO_MORE:    iRet = ERR_FS_FILE_TOO_MORE        ;break;
        case _ERR_FAT_FILE_NOT_EXISTED: iRet = ERR_FS_FILE_NOT_EXIST       ;break;
        case _ERR_FAT_DIFF_DEV:         iRet = ERR_FS_DEVICE_DIFF          ;break;
        case _ERR_FAT_HAS_FORMATED:     iRet = ERR_FS_HAS_FORMATED         ;break;
        case _ERR_FAT_NOT_FORMAT:       iRet = ERR_FS_NOT_FORMAT           ;break;
        case _ERR_FAT_NO_MORE_INODE:    iRet = ERR_FS_NO_MORE_INODE        ;break;
        case _ERR_FS_NO_MORE_SB_ITEM:   iRet = ERR_FS_NO_MORE_SB_ITEM      ;break;
        case _ERR_FS_NOT_MOUNT:         iRet = ERR_FS_NOT_MOUNT            ;break; 
        case _ERR_FAT_DIR_NOT_NULL:     iRet = ERR_FS_DIR_NOT_EMPTY        ;break; 
        case _ERR_FAT_ERROR:            iRet = ERR_FS_DATA_DESTROY         ;break; 
        default:                       
            iRet = iErrCode;
            break;
    }
    return iRet;
}



