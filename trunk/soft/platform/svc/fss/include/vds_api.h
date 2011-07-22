#ifndef _VDS_API_H_
#define _VDS_API_H_

#define VDS_PARTITION_NAME_LEN  7
typedef struct _MODULE_INFO
{
    UINT16 iBeginBlockNO;
    UINT32 ulTotalBlock;
    UINT32 ulBlockSize;
}
MODULE_INFO;

typedef struct vds_partition_info
{
    UINT8  acPartName[ VDS_PARTITION_NAME_LEN + LEN_FOR_NULL_CHAR ];
    UINT32 iFirstSecAddr;     // 分区的flash起始地址
    UINT32 iFlashSectorSize;    // 分区上flash物理扇区大小
    UINT32 iFlashTotalSize;     // 分区大小
    UINT32 iVBSize;             // 分区的VB大小
    UINT32 iMode;	              // 打开模式，目前用来区分数据回读的级别
}
VDS_PARTITION_INFO;


/*
!!!WARING!!!  
WE ASSUME:
1. 由VDS管理的诸sector的大小一致
2. 由VDS管理的诸sector在空间上连续
*/
typedef  struct vds_flash_info
{
    INT32  bHasInit;
    UINT32 iFlashStartAddr;     // 由VDS管理的flash的起始地址
    UINT32 iFlashSectorSize;    // 由VDS管理的flash的sector大小(以byte为单位)
    UINT32 iNrSector;           // 由VDS管理的flash的sector的个数
}
VDS_FLASH_INFO;

#define VDS_MODULE_REG     0x10
#define VDS_MODULE_WDB    0x11
#define VDS_MODULE_FS        0x12
#define VDS_MODULE_SMS    0x13
#define VDS_MODULE_PBK     0x14

INT32 VDS_PartitionHasFormatted( PCSTR pszPartName );
INT32 VDS_Init(VOID);
INT32 VDS_PartitionInit( PCSTR pszPartName, UINT32 iMode, UINT32 *pPartId );
INT32 VDS_PartitionFormat
(
    PCSTR pszPartName,  BOOL bForce,
    UINT32 iFirstSecIndex, UINT32  iTotalSecor, UINT32 iBlockSize
);
INT32 VDS_Open(PCSTR pszPartName, INT32 *pDevHandle );
INT32 VDS_Close( PCSTR pszPartName );
INT32 VDS_WriteBlock( UINT32 iPartId, UINT32 iVBNum, UINT8*pBuff );
INT32 VDS_ReadBlock( UINT32 iPartId, UINT32 iVBNum, UINT8* pBuff );
INT32 VDS_ClearBlock( UINT32 iPartId, UINT32 iVBNum );
INT32 VDS_GetPartitionInfo( UINT32 iPartId, UINT32 *piNrBlock, UINT32 *piBlockSize );

// 获取模块使用VDS的信息
INT32 VDS_GetModuleInfo( UINT16 iModuleID, MODULE_INFO *psInfo );

INT32 VDS_GetFlashInfo( VDS_FLASH_INFO *psInfo );

// 按字节线性读VDS，成功返回ERR_SUCCESS，
// 读取成功的长度通过pBytesRead返回。
INT32 VDS_Read( UINT16 iModuleID, UINT32 ulAddrOffset, BYTE* pBuffer,
                UINT32 ulBytesToRead, UINT32* pBytesRead );

// 按字节线性写VDS，成功返回ERR_SUCCESS，
// 写成功的长度通过pBytesWritten返回。
INT32 VDS_Write( UINT16 iModuleID, UINT32 ulAddrOffset, CONST BYTE* pBuffer,
                 UINT32 ulBytesToWrite, UINT32* pBytesWritten );

#define REG_Read(ulAddrOffset, pBuffer, ulBytesToRead, pBytesRead) \
            VDS_Read(VDS_MODULE_REG, ulAddrOffset, pBuffer, ulBytesToRead, pBytesRead )

#define REG_Write(ulAddrOffset, pBuffer, ulBytesToWrite, pBytesWritten) \
            VDS_Write(VDS_MODULE_REG, ulAddrOffset, pBuffer, ulBytesToWrite, pBytesWritten )

#define DM_Read(ulAddrOffset, pBuffer, ulBytesToRead, pBytesRead) \
            VDS_Read(VDS_MODULE_SMS, ulAddrOffset, pBuffer, ulBytesToRead, pBytesRead )

#define DM_Write(ulAddrOffset, pBuffer, ulBytesToWrite, pBytesWritten) \
            VDS_Write(VDS_MODULE_SMS, ulAddrOffset, pBuffer, ulBytesToWrite, pBytesWritten )

#if 0  // 目前只有REG跟DB使用了"VDS3"。
#define PBK_Read(ulAddrOffset, pBuffer, ulBytesToRead, pBytesRead) \
            VDS_Read(VDS_MODULE_PBK, ulAddrOffset, pBuffer, ulBytesToRead, pBytesRead )

#define PBK_Write(ulAddrOffset, pBuffer, ulBytesToWrite, pBytesWritten) \
            VDS_Write(VDS_MODULE_PBK, ulAddrOffset, pBuffer, ulBytesToWrite, pBytesWritten )
#endif

VOID DSM_GetVersion( INT32 iModId, CHAR* pVersion );


#endif // _VDS_API_H_





