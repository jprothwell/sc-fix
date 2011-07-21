#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "zyxfile.h"
#include "zyxsys.h"
#include "zyxstring.h"
#include "zyxstd.h"

int Zyx_get_target_fileflag(ZYX_FS_OPEN_MODE mflag)
{
    int targetFlag = 0;

    //targetFlag |= FS_READ_ONLY;

    if (mflag & ZFS_MODE_CREATE)
    targetFlag |= FS_CREATE;

    if (mflag & ZFS_MODE_READ)
    targetFlag |= FS_READ_ONLY;

    if (mflag & ZFS_MODE_SHARE)
    targetFlag |= FS_OPEN_SHARED;
    
    if (mflag & ZFS_MODE_WRITE)
    {
    targetFlag &= (~FS_READ_ONLY);
    targetFlag |= FS_READ_WRITE;
    }

    if (mflag & ZFS_MODE_CREATE_ALWAYS)
    {
    targetFlag &= (~FS_CREATE);
    targetFlag |= FS_CREATE_ALWAYS;
    } 
    
    return targetFlag;
}

ZFILE ZyxFileOpen(char* file_name,int flag)
{
    U16 UFileName[128] = {0};
    int ret;

    if (file_name[1]) //ascii
    {
        ZyxUTF8ToUCS(file_name,UFileName,128);
        ret = MMI_FS_Open((char*)UFileName, Zyx_get_target_fileflag(flag));
     }
    else
    {
        ret = MMI_FS_Open((char*)file_name, Zyx_get_target_fileflag(flag));
    }

    //ZyxTrace("zyx file open %s,flag=%d,ret=%d",file_name,flag,ret);
    return ret >= 0? (ret + 1) : -1;
}

int ZyxFileClose(ZFILE hfile)
{
    int ret = MMI_FS_Close(hfile - 1);

    return ret == 0? 0:-1;
}

int ZyxFileRead(ZFILE hfile, char* data,int length)
{
    INT32 read_num = -1;
    int ret = MMI_FS_Read(hfile - 1,data,length,&read_num);

    //ZyxTrace("zyx file read %d,%p,%d,ret=%d",hfile,data,length,ret );
    return ret == 0? read_num:-1;
}

int ZyxFileWrite(ZFILE hfile, char *data, int length)
{
    INT32 write_num = -1;
    int ret = MMI_FS_Write(hfile - 1,data,length,&write_num);

    return ret == 0? write_num:-1;
}

int ZyxFileCommit(ZFILE hfile)
{
    int ret = MMI_FS_Commit(hfile - 1);

     return ret == 0? 0:-1;
}

int ZyxFileSeek(ZFILE hfile,int offset,ZYX_FS_SEEK_POS pos)
{
    int ret = MMI_FS_Seek(hfile - 1, offset, pos);

    return ret >=0? ret:-1;
}

int ZyxFileTell(ZFILE hfile)
{
    unsigned int Position;
    int ret;
    
    ret=MMI_FS_GetFilePosition(hfile - 1,&Position);

    return ret >=0? Position:-1;
}

int ZyxFileGetSize(ZFILE hfile)
{
    INT32 size;
    int ret = MMI_FS_GetFileSize(hfile - 1, &size);

    return ret == 0? size:-1;
}

 BOOL ZyxFileExist(char* file_name)
{
    ZFILE hFile;

    hFile = ZyxFileOpen( file_name, ZFS_MODE_READ);
    if (hFile >= 0)
    {
        ZyxFileClose(hFile);
        return TRUE;
    }
    else
        return FALSE;
}

int ZyxFileDelete(char* file_name)
{
    U16 UFileName[128] = {0};
    int ret;

    if (file_name[1]) //ascii
    {
        ZyxUTF8ToUCS(file_name,UFileName,128);
        ret = MMI_FS_Delete((char*)UFileName);
     }
    else
    {
        ret = MMI_FS_Delete((char*)file_name);
    }

    ZyxTrace("zyx file del %s,ret=%d",file_name,ret);
    return ret == 0? 0:-1;
}

int ZyxFileRename(char* old_name,char* new_name)
{
    U16 UFileName[128] = {0},UFileName2[128] = {0};
    int ret;

    if (old_name[1] && new_name[1]) //ascii
    {
        ZyxUTF8ToUCS(old_name,UFileName,128);
        ZyxUTF8ToUCS(new_name,UFileName2,128);
        ret = MMI_FS_Rename((char*)UFileName, (char*)UFileName2);
     }
    else
    {
        ret = MMI_FS_Rename((char*)old_name, (char*)new_name);
    }
    
    if (ret != 0) //fail
    {
        ZFILE fOld,fNew;
        int len;
        char *buff = NULL;
        
        fOld = ZyxFileOpen(old_name, ZFS_MODE_READ);
        ZyxFileDelete(new_name);
        if (fOld >= 0)
        {
            fNew = ZyxFileOpen(new_name, ZFS_MODE_CREATE);
            if (fNew >= 0)
            {
                buff = ZyxAlloc(1024);
                if (buff)
                {
                    do
                   {
                        len = ZyxFileRead(fOld,buff,1024);
                        ZyxFileWrite(fNew,buff,len);
                   }while (len == 1024);
                    ZyxFree(buff);
                }
                ZyxFileClose(fNew);
                ret = 0;
            }
            ZyxFileClose(fOld);
        }
    }
    
    ZyxTrace("zyx file rename %s to %s,ret=%d",old_name,new_name,ret);
    return ret == 0? 0:-1;
}

int ZyxFileSetAtrributes(char* file_name,ZYX_FS_ATTRIBUTE attrib)
{
    U8 targetFlag = 0;
    U16 UFileName[128] = {0};
    int ret;
    
    if (attrib & ZFS_REDNONY)
        targetFlag |= FS_ATTR_READ_ONLY;

    if (attrib & ZFS_HIDDEN)
        targetFlag |= FS_ATTR_HIDDEN;

     if (attrib & ZFS_SYSTEM)
        targetFlag |= FS_ATTR_SYSTEM;

     if (attrib & ZFS_ARCHIVE)
        targetFlag |= FS_ATTR_ARCHIVE;

    if (file_name[1]) //ascii
    {
        ZyxUTF8ToUCS(file_name,UFileName,128);
        ret = MMI_FS_SetAttributes((char*)UFileName, targetFlag);
     }
    else
    {
        ret = MMI_FS_SetAttributes((char*)file_name, targetFlag);
    }

    ZyxTrace("zyx file set file %s,attrb=%d,ret=%d",file_name,targetFlag,ret);
    return ret >= 0?ret:-1;
}

int ZyxCreateDir(char* dir_name)
{
    U16 UFileName[128] = {0};
    int ret;

    if(dir_name[Zyx_strlen(dir_name)-1]=='\\')
    {
         dir_name[Zyx_strlen(dir_name)-1] = 0;
    }
    if (dir_name[1]) //ascii
    {
        ZyxUTF8ToUCS(dir_name,UFileName,128);
        ret = MMI_FS_CreateDir((char*)UFileName);
     }
    else
    {
        ret = MMI_FS_CreateDir((char*)dir_name);
    }
    ZyxTrace("zyx file create dir %s,ret=%d",dir_name,ret);
    return ret == 0? 0:-1;
}

int ZyxRemoveDir(char* dir_name)
{
    U16 UFileName[128] = {0};
    int ret;

    if (dir_name[1]) //ascii
    {
        ZyxUTF8ToUCS(dir_name,UFileName,128);
        ret = MMI_FS_RemoveDir((char* )UFileName);//MMI_FS_XDelete((char*)UFileName, FS_FILE_TYPE | FS_DIR_TYPE | FS_RECURSIVE_TYPE);
     }
    else
    {
        ret = MMI_FS_RemoveDir((char*)dir_name);
    }
    ZyxTrace("zyx file del dir %s,ret=%d",dir_name,ret);
    return ret >= 0? 0:-1;
}


int ZyxGetDrive(ZYX_DEVICE_DRIVE drive)
{
    S32 ret_drv = 0;
    
#ifdef APP_RUN_AT_INTERNAL_FLASH
    if (drive == DEVICE_TCARD)
        drive = DEVICE_NORMAL;
#endif
    
    switch(drive)
    {
    case DEVICE_SYSTEM:
      	 ret_drv=MMI_FS_GetDrive(FS_DRIVE_I_SYSTEM, 1, FS_NO_ALT_DRIVE );
         break;
    case DEVICE_NORMAL:
      	 ret_drv=MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL|FS_DRIVE_I_SYSTEM );
         break;
    case DEVICE_TCARD:
    default:
        ret_drv=MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
        break;
    }
    
    return ret_drv;
}

int ZyxGetDriveInfo(ZYX_DEVICE_DRIVE drive, U64 *totalspace, U64 *freespace)
{
    int fs_ret;
    FS_DiskInfo disk_info;
    char file_path_asc[5] = {0};
    U16 file_path[10] = {0};
    
    sprintf(file_path_asc,"%c:\\",ZyxGetDrive(drive));
    ZyxUTF8ToUCS(file_path_asc,file_path,10);
    fs_ret = MMI_FS_GetDiskInfo((char*) file_path, &disk_info, FS_DI_BASIC_INFO | FS_DI_FREE_SPACE);
    
    if (fs_ret >= 0)
    {
        *totalspace = disk_info.TotalClusters * disk_info.SectorsPerCluster * disk_info.BytesPerSector;
        *freespace = disk_info.FreeClusters * disk_info.SectorsPerCluster * disk_info.BytesPerSector;
    }

    ZyxTrace("zyx drive %c total=%d",ZyxGetDrive(drive),*totalspace);
    ZyxTrace("zyx drive %c free=%d",ZyxGetDrive(drive),*freespace);
    return fs_ret >= 0? 0:-1;
}

#endif
