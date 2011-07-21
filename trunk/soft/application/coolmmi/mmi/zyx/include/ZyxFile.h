#ifndef __ZYX_FILE_H__
#define __ZYX_FILE_H__

#include "zyxtypes.h"

typedef int ZFILE;

typedef enum
{
	ZFS_MODE_READ           =   1,	  //�ɶ�
	ZFS_MODE_WRITE          =   2,	  //��д
	ZFS_MODE_CREATE         =   4,  //�ļ�������ʱ�½�
	ZFS_MODE_CREATE_ALWAYS  =   8, //�����½�
        ZFS_MODE_SHARE          = 0x10    //�����
}ZYX_FS_OPEN_MODE;

//�ļ�ָ���ƶ���ʽ
typedef enum
{
   ZFS_BEGIN=0,					//���ļ�ͷ
   ZFS_CURRENT,					//�ӵ�ǰλ��
   ZFS_END						//���ļ�β
}ZYX_FS_SEEK_POS;

typedef enum
{
        DEVICE_SYSTEM,     //ϵͳ��
	DEVICE_TCARD,       //T���洢��
	DEVICE_NORMAL    //�ֻ��ڴ���
}ZYX_DEVICE_DRIVE;

typedef enum
{
   ZFS_REDNONY = 0x1,	
   ZFS_HIDDEN    = 0x2,
   ZFS_SYSTEM    =0x4 ,
   ZFS_ARCHIVE   = 0x8
}ZYX_FS_ATTRIBUTE;

extern ZFILE ZyxFileOpen(char* file_name,int flag);
extern int ZyxFileClose(ZFILE hfile);
extern int ZyxFileRead(ZFILE hfile, char* data,int length);
extern int ZyxFileWrite(ZFILE hfile, char *data, int length);
extern int ZyxFileCommit(ZFILE hfile);
extern int ZyxFileSeek(ZFILE hfile,int offset,ZYX_FS_SEEK_POS pos);
extern int ZyxFileTell(ZFILE hfile);
extern BOOL ZyxFileExist(char* file_name);
extern int ZyxFileGetSize(ZFILE hfile);
extern int ZyxFileDelete(char* file_name);
extern int ZyxFileRename(char* old_name,char* new_name);
extern int ZyxFileSetAtrributes(char* file_name,ZYX_FS_ATTRIBUTE attrib);
extern int ZyxCreateDir(char* dir_name);
extern int ZyxRemoveDir(char* dir_name);

extern int ZyxGetDrive(ZYX_DEVICE_DRIVE drive);
extern int ZyxGetDriveInfo(ZYX_DEVICE_DRIVE drive, U64 *totalspace, U64 *freespace);

#endif
