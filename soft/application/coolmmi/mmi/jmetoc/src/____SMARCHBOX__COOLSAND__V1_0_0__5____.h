/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__5____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 测试代码 /////////////////////////////////////////////////////////////
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
 * 运行测试代码。
 * <p>
 * 在定义了 ____RUN_TEST____ 宏以后，本测试代码段会被调用。
 * </p>
 * <p>
 * 测试代码段调用的时机为整个虚拟机刚刚启动后，应用还没有被加载的时候。
 * 这个时候，所有系统模块可以自由调用测试。
 * </p>
 * @author runasea
 * @version 1.0.0.1, 2009-06-02
 */
void runTestCode() {
  FS_DiskInfo disk_info;
  //javax_wireless_messaging_NativeMessage_sendSms("Hello Smarch", "18989491652"); //发送短信测试
  MMI_FS_GetDiskInfo((u8*)L"D:/", &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
  kal_prompt_trace(0, "sd size %d", disk_info.FreeClusters * disk_info.SectorsPerCluster * disk_info.BytesPerSector);
  MMI_FS_GetDiskInfo((u8*)L"E:/", &disk_info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
  kal_prompt_trace(0, "ram size %d", disk_info.FreeClusters * disk_info.SectorsPerCluster * disk_info.BytesPerSector);
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
