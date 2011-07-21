/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__5____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// ���Դ��� /////////////////////////////////////////////////////////////
#if 0
#define ____RUN_TEST____

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

int MMI_FS_GetDiskInfo(u8 * DriveName, FS_DiskInfo * DiskInfo, int Flags);

/**
 * ���в��Դ��롣
 * <p>
 * �ڶ����� ____RUN_TEST____ ���Ժ󣬱����Դ���λᱻ���á�
 * </p>
 * <p>
 * ���Դ���ε��õ�ʱ��Ϊ����������ո�������Ӧ�û�û�б����ص�ʱ��
 * ���ʱ������ϵͳģ��������ɵ��ò��ԡ�
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void runTestCode() {
  FS_DiskInfo disk_info;
  //javax_wireless_messaging_NativeMessage_sendSms("Hello Smarch", "18989491652"); //���Ͷ��Ų���
  MMI_FS_GetDiskInfo((u8*)L"D:/", &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
  kal_prompt_trace(0, "sd size %d", disk_info.FreeClusters * disk_info.SectorsPerCluster * disk_info.BytesPerSector);
  MMI_FS_GetDiskInfo((u8*)L"E:/", &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
  kal_prompt_trace(0, "ram size %d", disk_info.FreeClusters * disk_info.SectorsPerCluster * disk_info.BytesPerSector);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
