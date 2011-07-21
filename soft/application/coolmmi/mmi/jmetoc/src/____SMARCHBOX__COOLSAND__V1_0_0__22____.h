/*
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 文件模块 /////////////////////////////////////////////////////////////

extern int FS_GetDrive(u32,u32,u32);
extern int FS_SetCurrentDir(u16*);
extern void kal_prompt_trace(int mod_id, const char *fmt,...);
extern int mod_mmi;
extern int mod_trace;

/**
 * 打开文件。
 * @param path 文件名(绝对路径)
 * @return 文件句柄
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 openFile(u16* path) {
  s32 handle = MMI_FS_Open(path, FS_READ_WRITE);
  s32 i;
  if (handle >= 0) {
    for (i = 0; i < 10; i++) {
      if (jmetoc_file_handles[i] == -1) {
        jmetoc_file_handles[i] = handle;
        //kal_prompt_trace(mod_trace, "fs open %d", i);
        break;
      }
    }
  }
  return handle;
}

/**
 * 关闭文件。
 * @param handle 文件句柄
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void closeFile(s32 handle) {
  s32 i;
  for (i = 0; i < 10; i++) {
    if (jmetoc_file_handles[i] == handle) {
      jmetoc_file_handles[i] = -1;
      //kal_prompt_trace(mod_trace, "fs cls %d", i);
      break;
    }
  }
  MMI_FS_Close(handle);
}

/**
 * 获得 SD卡盘符。
 * @return SD卡盘符
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI u16 getSDDriverName() {
  //return MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
  return 'D';
}

/**
 * 获得内部盘符。
 * @return 盘符
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI u16 getInterDriverName() {
  return 'E';
}

/**
 * 读取文件。
 * @param handle 文件句柄
 * @param data 文件数据
 * @param len 读取长度
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void readFile(s32 handle, void* data, u32 len) {
  u32 l;
  MMI_FS_Read(handle, data, len, &l);
}

/**
 * 写入文件。
 * @param handle 文件句柄
 * @param data 文件数据
 * @param len 读取长度
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void writeFile(s32 handle, void* data, u32 len) {
  u32 l;
  MMI_FS_Write(handle, data, len, &l);
}

/**
 * 文件指针定位(当前位置基准)。
 * @param handle 文件句柄
 * @param offset 偏移位置
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void seekFile(s32 handle, s32 offset) {
  MMI_FS_Seek(handle, offset, FS_FILE_CURRENT);
}

/**
 * 文件指针定位(从头为基准)。
 * @param handle 文件句柄
 * @param offset 偏移位置
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void seekFile2(s32 handle, s32 offset) {
  MMI_FS_Seek(handle, offset, FS_FILE_BEGIN);
}

/**
 * 获得文件大小。
 * @param handle 文件句柄
 * @return 文件大小
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI u32 getFileSize(s32 handle) {
  u32 size;
  MMI_FS_GetFileSize(handle, &size);
  return size;
}

/**
 * 创建文件。
 * @param path 文件名(绝对路径)
 * @return 文件句柄
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 createFile(u16* path) {
  return MMI_FS_Open(path, FS_CREATE_ALWAYS);
}

/**
 * 重新命名文件。
 * @param old 原文件名(绝对路径)
 * @param path 新文件名(绝对路径)
 * 操作返回值
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 renameFile(u16* old, u16* path) {
  return MMI_FS_Rename(old, path);
}

/**
 * 删除文件。
 * @param path 文件名
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void deleteFile(u16* path) {
  MMI_FS_Delete(path);
}

/**
 * 获得文件属性。
 * <p>
 * 文件属性列表：<br/>
 * #define FS_ATTR_READ_ONLY        0x01<br/>
 * #define FS_ATTR_HIDDEN           0x02<br/>
 * #define FS_ATTR_SYSTEM           0x04<br/>
 * #define FS_ATTR_VOLUME           0x08<br/>
 * #define FS_ATTR_DIR              0x10<br/>
 * #define FS_ATTR_ARCHIVE          0x20<br/>
 * #define FS_LONGNAME_ATTR         0x0F<br/>
 * </p>
 * @param path 文件名
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 getFileAttr(u16* path) {
  return MMI_FS_GetAttributes(path);
}

int kal_wstrlen(u16 *wstr);
/**
 * 获得目录里面的文件名列表。
 * <p>
 * 获得目录里面包含目录和文件在内的所有文件列表。
 * </p>
 * @param dirPath 目录路径(绝对路径)
 * @return 文件名列表
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI java_util_Vector* getDirList(u16* dirPath) {
  s32 handle;
  java_util_Vector* vec;
  java_lang_String* s;
  u16* buf = (u16*)java_lang_Runtime_malloc(256);
  FS_DOSDirEntry info;
  int len;
  handle = MMI_FS_SetCurrentDir(dirPath);
  if (handle < 0) {
    return null;
  } else {
    handle = MMI_FS_FindFirst((u16*)L"*.*", 0, 0, &info, buf, 128);
    if (handle < 0) return null;
    vec = java_util_Vector_new();
    java_util_Vector_Vector_alias2(vec);
    do {
      len = kal_wstrlen(buf);
      s = java_lang_Runtime_new_string(buf, len);
      java_util_Vector_addElement(vec, (java_lang_Object*)s);
      buf = (u16*)java_lang_Runtime_malloc(256);
    } while(MMI_FS_FindNext(handle, &info, buf, 128) == 0);
    MMI_FS_FindClose(handle);
    return vec;
  }
}

/**
 * 创建目录。
 * @param dirPath 目录名(绝对路径)
 * @return 目录句柄
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 createDir(u16* dirPath) {
  return MMI_FS_CreateDir(dirPath);
}

/**
 * 删除目录。
 * @param dirPath 目录名(绝对路径)
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void deleteDir(u16* dirPath) {
  FS_RemoveDir(dirPath);
}

#define FS_DI_BASIC_INFO         0x00000001L
#define FS_DI_FREE_SPACE         0x00000002L
#define FS_DI_FAT_STATISTICS     0x00000004L

typedef struct 
{
   char            Label[24];
   char            DriveLetter;
   u8            WriteProtect;
   char            Reserved[2];
   u32            SerialNumber;
   u32            FirstPhysicalSector;
   u32            FATType;
   u32            FATCount;
   u32            MaxDirEntries;
   u32            BytesPerSector;
   u32            SectorsPerCluster;
   u64        TotalClusters;
   u32            BadClusters;
   u64        FreeClusters;
   u32            Files;
   u32            FileChains;
   u32            FreeChains;
   u32            LargestFreeChain;
}FS_DiskInfo;

extern int dbg_TraceOutputText(u16 nIndex, const char* fmt, ...);
#define kal_prompt_trace(trace_level, fmt, ...) \
			dbg_TraceOutputText(10, (fmt), ##__VA_ARGS__);
extern int MMI_FS_GetDiskInfo(u8 * DriveName, FS_DiskInfo * DiskInfo, int Flags);

/**
 * 获得某个驱动的可用空间大小。
 * @param path 绝对路径
 * @return 驱动可用空间大小，字节计
 * @author runasea
 * @version 1.0.0.1, 2010-01-10
 */
s32 getDriverFreeSize(u16* path) {
  FS_DiskInfo disk_info;
  s32 size;
  u16 drv[4];
  for (size = 0; size < 4; size++) {
    drv[size] = path[size];
  }
  drv[3] = 0;
  MMI_FS_GetDiskInfo((u8*)drv, &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
  size = disk_info.FreeClusters * disk_info.SectorsPerCluster * disk_info.BytesPerSector;
  kal_prompt_trace(0, "drv size %d", size);
  return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
