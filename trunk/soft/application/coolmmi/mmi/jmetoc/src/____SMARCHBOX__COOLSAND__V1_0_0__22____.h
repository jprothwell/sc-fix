/*
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// �ļ�ģ�� /////////////////////////////////////////////////////////////

extern int FS_GetDrive(u32,u32,u32);
extern int FS_SetCurrentDir(u16*);
extern void kal_prompt_trace(int mod_id, const char *fmt,...);
extern int mod_mmi;
extern int mod_trace;

/**
 * ���ļ���
 * @param path �ļ���(����·��)
 * @return �ļ����
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
 * �ر��ļ���
 * @param handle �ļ����
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
 * ��� SD���̷���
 * @return SD���̷�
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI u16 getSDDriverName() {
  //return MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
  return 'D';
}

/**
 * ����ڲ��̷���
 * @return �̷�
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI u16 getInterDriverName() {
  return 'E';
}

/**
 * ��ȡ�ļ���
 * @param handle �ļ����
 * @param data �ļ�����
 * @param len ��ȡ����
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void readFile(s32 handle, void* data, u32 len) {
  u32 l;
  MMI_FS_Read(handle, data, len, &l);
}

/**
 * д���ļ���
 * @param handle �ļ����
 * @param data �ļ�����
 * @param len ��ȡ����
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void writeFile(s32 handle, void* data, u32 len) {
  u32 l;
  MMI_FS_Write(handle, data, len, &l);
}

/**
 * �ļ�ָ�붨λ(��ǰλ�û�׼)��
 * @param handle �ļ����
 * @param offset ƫ��λ��
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void seekFile(s32 handle, s32 offset) {
  MMI_FS_Seek(handle, offset, FS_FILE_CURRENT);
}

/**
 * �ļ�ָ�붨λ(��ͷΪ��׼)��
 * @param handle �ļ����
 * @param offset ƫ��λ��
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void seekFile2(s32 handle, s32 offset) {
  MMI_FS_Seek(handle, offset, FS_FILE_BEGIN);
}

/**
 * ����ļ���С��
 * @param handle �ļ����
 * @return �ļ���С
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI u32 getFileSize(s32 handle) {
  u32 size;
  MMI_FS_GetFileSize(handle, &size);
  return size;
}

/**
 * �����ļ���
 * @param path �ļ���(����·��)
 * @return �ļ����
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 createFile(u16* path) {
  return MMI_FS_Open(path, FS_CREATE_ALWAYS);
}

/**
 * ���������ļ���
 * @param old ԭ�ļ���(����·��)
 * @param path ���ļ���(����·��)
 * ��������ֵ
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 renameFile(u16* old, u16* path) {
  return MMI_FS_Rename(old, path);
}

/**
 * ɾ���ļ���
 * @param path �ļ���
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI void deleteFile(u16* path) {
  MMI_FS_Delete(path);
}

/**
 * ����ļ����ԡ�
 * <p>
 * �ļ������б�<br/>
 * #define FS_ATTR_READ_ONLY        0x01<br/>
 * #define FS_ATTR_HIDDEN           0x02<br/>
 * #define FS_ATTR_SYSTEM           0x04<br/>
 * #define FS_ATTR_VOLUME           0x08<br/>
 * #define FS_ATTR_DIR              0x10<br/>
 * #define FS_ATTR_ARCHIVE          0x20<br/>
 * #define FS_LONGNAME_ATTR         0x0F<br/>
 * </p>
 * @param path �ļ���
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 getFileAttr(u16* path) {
  return MMI_FS_GetAttributes(path);
}

int kal_wstrlen(u16 *wstr);
/**
 * ���Ŀ¼������ļ����б�
 * <p>
 * ���Ŀ¼�������Ŀ¼���ļ����ڵ������ļ��б�
 * </p>
 * @param dirPath Ŀ¼·��(����·��)
 * @return �ļ����б�
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
 * ����Ŀ¼��
 * @param dirPath Ŀ¼��(����·��)
 * @return Ŀ¼���
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
JMETOCAPI s32 createDir(u16* dirPath) {
  return MMI_FS_CreateDir(dirPath);
}

/**
 * ɾ��Ŀ¼��
 * @param dirPath Ŀ¼��(����·��)
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
 * ���ĳ�������Ŀ��ÿռ��С��
 * @param path ����·��
 * @return �������ÿռ��С���ֽڼ�
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
