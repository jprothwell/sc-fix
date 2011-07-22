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
    UINT32 iFirstSecAddr;     // ������flash��ʼ��ַ
    UINT32 iFlashSectorSize;    // ������flash����������С
    UINT32 iFlashTotalSize;     // ������С
    UINT32 iVBSize;             // ������VB��С
    UINT32 iMode;	              // ��ģʽ��Ŀǰ�����������ݻض��ļ���
}
VDS_PARTITION_INFO;


/*
!!!WARING!!!  
WE ASSUME:
1. ��VDS�������sector�Ĵ�Сһ��
2. ��VDS�������sector�ڿռ�������
*/
typedef  struct vds_flash_info
{
    INT32  bHasInit;
    UINT32 iFlashStartAddr;     // ��VDS�����flash����ʼ��ַ
    UINT32 iFlashSectorSize;    // ��VDS�����flash��sector��С(��byteΪ��λ)
    UINT32 iNrSector;           // ��VDS�����flash��sector�ĸ���
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

// ��ȡģ��ʹ��VDS����Ϣ
INT32 VDS_GetModuleInfo( UINT16 iModuleID, MODULE_INFO *psInfo );

INT32 VDS_GetFlashInfo( VDS_FLASH_INFO *psInfo );

// ���ֽ����Զ�VDS���ɹ�����ERR_SUCCESS��
// ��ȡ�ɹ��ĳ���ͨ��pBytesRead���ء�
INT32 VDS_Read( UINT16 iModuleID, UINT32 ulAddrOffset, BYTE* pBuffer,
                UINT32 ulBytesToRead, UINT32* pBytesRead );

// ���ֽ�����дVDS���ɹ�����ERR_SUCCESS��
// д�ɹ��ĳ���ͨ��pBytesWritten���ء�
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

#if 0  // Ŀǰֻ��REG��DBʹ����"VDS3"��
#define PBK_Read(ulAddrOffset, pBuffer, ulBytesToRead, pBytesRead) \
            VDS_Read(VDS_MODULE_PBK, ulAddrOffset, pBuffer, ulBytesToRead, pBytesRead )

#define PBK_Write(ulAddrOffset, pBuffer, ulBytesToWrite, pBytesWritten) \
            VDS_Write(VDS_MODULE_PBK, ulAddrOffset, pBuffer, ulBytesToWrite, pBytesWritten )
#endif

VOID DSM_GetVersion( INT32 iModId, CHAR* pVersion );


#endif // _VDS_API_H_





