#ifndef _CII_FAT_BASE_H
#define _CII_FAT_BASE_H

#include "cs_types.h"

#include "sxr_ops.h"


struct super_block;
struct file;

// Support fat file system type.
#define FAT12 12                        
#define FAT16 16
#define FAT32 32

// FAT卷之前的隐藏扇区最大数
#define FAT_NR_MAX_HIDE_SECTOR   256

// Size defined.
#define DEFAULT_SECSIZE         512             // Default secorters size.
#define DEFAULT_DIRENTRYSIZE    32         // Default directory entry size
#define SIZE_MB                (1024*1024)    

#define FAT_BOOT_RECORD_SIZE    62     // Boot record size.
#define FAT_DIR_ENTRY_SIZE      32     // Directory entry size.
#define FAT_MAX_NAME_LEN        11     // FAT 8-3 Directory name or file name max size.
#define FAT_MAX_USER_NAME_LEN   12 // Directory name or file name for user max size.
//#define FAT_MAX_PATH_LEN        80     // The total path length of a short name cannot exceed 80 characters (64 char path + 3 drive
                                       // letter + 12 for 8.3 name + NUL) including the trailing NUL.
#define UINT32_SIZE             0x04
#define UINT16_SIZE             0x02
#define UINT8_SIZE              0x01

#define CLUSTER_NULL           ((UINT32)(~0))
#define CLUSTER_FREE            0
#define CLUSTER_BAD             0xFFFFFFF7


// Directory attr defined.
#define ATTR_RO      0x01             // read-only 
#define ATTR_HIDDEN  0x02             // hidden 
#define ATTR_SYS     0x04             // system 
#define ATTR_VOLUME  0x08             // volume label 
#define ATTR_DIR     0x10             // directory 
#define ATTR_ARCH    0x20             // archived 

// long file name defined.
#define FAT_MAX_LONG_NAME_LEN       255 // Long dir name or file name max lengnt.
#define FAT_MAX_LONG_PATH_LEN       (260 - LEN_FOR_NULL_CHAR)   // Long file name path max lenght.

#define FAT_ATTR_LONG_NAME              (ATTR_RO|ATTR_HIDDEN|ATTR_SYS|ATTR_VOLUME)
#define FAT_ATTR_LONG_NAME_MASK         (ATTR_RO|ATTR_HIDDEN|ATTR_SYS|ATTR_VOLUME|ATTR_DIR|ATTR_ARCH)
#define FAT_LONG_ENTRY_LAST_FLAG    0x40 // long entry last entry flag.
#define FAT_LONG_PER_ENTRY_NAME_LEN 26   // Name length per long entry.

// lossy conversion flag.
#define FAT_LOSSY_CONVERSION        1   //lossy conversion,from long to short. 
#define FAT_INTACT_CONVERSION       0   //intact conversion from long to short.

#define FAT_NOT_SUPPORT_LONG_NAME   0   // not support long file name
#define FAT_SUPPORT_LONG_NAME       1   // support long file name.


// Offset and size defined.
#define FBR_JMP_BOOT_POS        0x00
#define FBR_JMP_BOOT_SIZE       (UINT8_SIZE*3)
#define FBR_OEM_NAME_POS        (FBR_JMP_BOOT_POS+FBR_JMP_BOOT_SIZE)
#define FBR_OEM_NAME_SIZE       (UINT8_SIZE*8)
#define FBR_BYTES_PER_SEC_POS   (FBR_OEM_NAME_POS+FBR_OEM_NAME_SIZE)
#define FBR_BYTES_PER_SEC_SIZE  UINT16_SIZE
#define FBR_SEC_PER_CLUS_POS    (FBR_BYTES_PER_SEC_POS+FBR_BYTES_PER_SEC_SIZE)
#define FBR_SEC_PER_CLUS_SIZE   UINT8_SIZE
#define FBR_RSVD_SEC_CNT_POS    (FBR_SEC_PER_CLUS_POS+FBR_SEC_PER_CLUS_SIZE)
#define FBR_RSVD_SEC_CNT_SIZE   UINT16_SIZE
#define FBR_NUM_FATS_POS        (FBR_RSVD_SEC_CNT_POS+FBR_RSVD_SEC_CNT_SIZE)
#define FBR_NUM_FATS_SIZE       UINT8_SIZE
#define FBR_ROOT_ENT_CNT_POS    (FBR_NUM_FATS_POS+FBR_NUM_FATS_SIZE)
#define FBR_ROOT_ENT_CNT_SIZE   UINT16_SIZE
#define FBR_TOT_SEC16_POS       (FBR_ROOT_ENT_CNT_POS+FBR_ROOT_ENT_CNT_SIZE)
#define FBR_TOT_SEC16_SIZE      UINT16_SIZE
#define FBR_MEDIA_POS           (FBR_TOT_SEC16_POS+FBR_TOT_SEC16_SIZE)
#define FBR_MEDIA_SIZE          UINT8_SIZE
#define FBR_FAT_SZ16_POS        (FBR_MEDIA_POS+FBR_MEDIA_SIZE)
#define FBR_FAT_SZ16_SIZE       UINT16_SIZE
#define FBR_SEC_PER_TRK_POS     (FBR_FAT_SZ16_POS+FBR_FAT_SZ16_SIZE)
#define FBR_SEC_PER_TRK_SIZE    UINT16_SIZE
#define FBR_NUM_HEADS_POS       (FBR_SEC_PER_TRK_POS+FBR_SEC_PER_TRK_SIZE)
#define FBR_NUM_HEADS_SIZE      UINT16_SIZE
#define FBR_HIDD_SEC_POS        (FBR_NUM_HEADS_POS+FBR_NUM_HEADS_SIZE)
#define FBR_HIDD_SEC_SIZE       UINT32_SIZE
#define FBR_TOL_SEC32_POS       (FBR_HIDD_SEC_POS+FBR_HIDD_SEC_SIZE)
#define FBR_TOL_SEC32_SIZE      UINT32_SIZE
// FAT16/12
#define FBR_DRV_NUM_POS         (FBR_TOL_SEC32_POS+FBR_TOL_SEC32_SIZE)
#define FBR_DRV_NUM_SIZE        UINT8_SIZE
#define FBR_RESERVED1_POS       (FBR_DRV_NUM_POS+FBR_DRV_NUM_SIZE)
#define FBR_RESERVED1_SIZE      UINT8_SIZE
#define FBR_BOOT_SIG_POS        (FBR_RESERVED1_POS+FBR_RESERVED1_SIZE)
#define FBR_BOOT_SIG_SIZE       UINT8_SIZE
#define FBR_VOL_ID_POS          (FBR_BOOT_SIG_POS+FBR_BOOT_SIG_SIZE)
#define FBR_VOL_ID_SIZE         UINT32_SIZE
#define FBR_VOL_LAB_POS         (FBR_VOL_ID_POS+FBR_VOL_ID_SIZE)
#define FBR_VOL_LAB_SIZE        (UINT8_SIZE*11)
#define FBR_FIL_SYS_TYPE_POS    (FBR_VOL_LAB_POS+FBR_VOL_LAB_SIZE)
#define FBR_FIL_SYS_TYPE_SIZE   (UINT8_SIZE*8)
// FAT32
#define FBR_FATSZ32_POS         (FBR_TOL_SEC32_POS+FBR_TOL_SEC32_SIZE)
#define BPB_FATSZ32_SIZE         UINT32_SIZE

#define BPB_EXTFLAGS_POS        (FBR_FATSZ32_POS + BPB_FATSZ32_SIZE)
#define BPB_EXTFLAGS_SIZE        UINT16_SIZE

#define BPB_FSVER_POS           (BPB_EXTFLAGS_POS + BPB_EXTFLAGS_SIZE)
#define BPB_FSVER_SIZE           UINT16_SIZE

#define BPB_ROOTCLUS_POS        (BPB_FSVER_POS+BPB_FSVER_SIZE)
#define BPB_ROOTCLUS_SIZE        UINT32_SIZE

#define BPB_FSINFO_POS          (BPB_ROOTCLUS_POS + BPB_ROOTCLUS_SIZE)
#define BPB_FSINFO_SIZE          UINT16_SIZE

#define BPB_BKBOOTSEC_POS       (BPB_FSINFO_POS + BPB_FSINFO_SIZE)
#define BPB_BKBOOTSEC_SIZE       UINT16_SIZE

#define BPB_RESERVED_POS        (BPB_BKBOOTSEC_POS+ BPB_BKBOOTSEC_SIZE)
#define BPB_RESERVED_SIZE       (UINT8_SIZE*12)

#define BS_DRVNUM_POS           (BPB_RESERVED_POS + BPB_RESERVED_SIZE)
#define BS_DRVNUM_SIZE           UINT8_SIZE

#define BS_RESERVED1_POS        (BS_DRVNUM_POS + BS_DRVNUM_SIZE)
#define BS_RESERVED1_SIZE        UINT8_SIZE

#define BS_BOOTSIG_POS          (BS_RESERVED1_POS + BS_RESERVED1_SIZE)
#define BS_BOOTSIG_SIZE          UINT8_SIZE

#define BS_VOLID_POS            (BS_BOOTSIG_POS + BS_BOOTSIG_SIZE)
#define BS_VOLID_SIZE            UINT32_SIZE

#define BS_VOLLAB_POS           (BS_VOLID_POS + BS_VOLID_SIZE)
#define BS_VOLLAB_SIZE          (UINT8_SIZE*11)   

#define BS_FILSYSTYPE_POS       (BS_VOLLAB_POS + BS_VOLLAB_SIZE)
#define BS_FILSYSTYPE_SIZE       (UINT8_SIZE*8)

// The sub fat boot record info for fat12 and fat16
typedef struct _FAT_BOOT_SUB_RECORD_1216
{
      UINT32 BS_VolID;
      UINT8  BS_DrvNum; 
      UINT8  BS_Reserved1;
      UINT8  BS_BootSig;      
      UINT8  BS_VolLab[FBR_VOL_LAB_SIZE];
      UINT8  BS_FilSysType[FBR_FIL_SYS_TYPE_SIZE];
      UINT8  pad[3];
}FAT_BOOT_SUB_RECORD_1216;

// The sub fat boot record info for fat32
typedef struct _FAT_BOOT_SUB_RECORD_32
{
      UINT32 BPB_FATSz32;
      UINT32 BPB_RootClus;  
      UINT32 BS_VolID;
      UINT16 BPB_ExtFlags; 
      UINT16 BPB_FSVer;      
      UINT16 BPB_FSInfo;
      UINT16 BPB_BkBootSec;      
      UINT8  BS_DrvNum;
      UINT8  BS_Reserved1;
      UINT8  BS_BootSig;
      UINT8  BPB_Reserved[BPB_RESERVED_SIZE];
      UINT8  BS_VolLab[BS_VOLLAB_SIZE];
      UINT8  BS_FilSysType[BS_FILSYSTYPE_SIZE];
      UINT8  pad[2];
}FAT_BOOT_SUB_RECORD_32;

//The fat boot record
typedef struct _FAT_BOOT_RECORD
{
    UINT32 BPB_HiddSec;
    UINT32 BPB_TotSec32;    
    UINT16 BPB_BytesPerSec;
    UINT16 BPB_RsvdSecCnt;
    UINT16 BPB_RootEntCnt;
    UINT16 BPB_TotSec16;
    UINT16 BPB_FATSz16;
    UINT16 BPB_SecPerTrk;
    UINT16 BPB_NumHeads;
    UINT8  BPB_SecPerClus;
    UINT8  BPB_NumFATs;
    UINT8  BPB_Media;    
    UINT8  BS_OEMName[FBR_OEM_NAME_SIZE];
    UINT8  BS_jmpBoot[FBR_JMP_BOOT_SIZE];
    union  
    {
        FAT_BOOT_SUB_RECORD_1216 sub_rec1216;
        FAT_BOOT_SUB_RECORD_32 sub_rec32;
    }u ;
    UINT8  pad;
}FAT_BOOT_RECORD;
// Position and size define for FAT dir(short directroy entry).
#define FDE_DIR_NAME_POS            0x00
#define FDE_DIR_NAME_SIZE           (UINT8_SIZE*11)
#define FDE_DIR_ATTR_POS            (FDE_DIR_NAME_POS+FDE_DIR_NAME_SIZE)
#define FDE_DIR_ATTR_SIZE           UINT8_SIZE
#define FDE_DIR_NTRES_POS           (FDE_DIR_ATTR_POS+FDE_DIR_ATTR_SIZE)
#define FDE_DIR_NTRES_SIZE          UINT8_SIZE
#define FDE_DIR_CRT_TIME_TENTH_POS  (FDE_DIR_NTRES_POS+FDE_DIR_NTRES_SIZE)
#define FDE_DIR_CRT_TIME_TENTH_SIZE UINT8_SIZE
#define FDE_DIR_CRT_TIME_POS        (FDE_DIR_CRT_TIME_TENTH_POS+FDE_DIR_CRT_TIME_TENTH_SIZE)
#define FDE_DIR_CRT_TIME_SIZE       UINT16_SIZE
#define FDE_DIR_CRT_DATA_POS        (FDE_DIR_CRT_TIME_POS+FDE_DIR_CRT_TIME_SIZE)
#define FDE_DIR_CRT_DATA_SIZE       UINT16_SIZE
#define FDE_DIR_LST_ACC_DATA_POS    (FDE_DIR_CRT_DATA_POS+FDE_DIR_CRT_DATA_SIZE)
#define FDE_DIR_LST_ACC_DATA_SIZE   UINT16_SIZE
#define FDE_DIR_FST_CLUS_HI_POS     (FDE_DIR_LST_ACC_DATA_POS+FDE_DIR_LST_ACC_DATA_SIZE)
#define FDE_DIR_FST_CLUS_HI_SIZE    UINT16_SIZE
#define FDE_DIR_WRT_TIME_POS        (FDE_DIR_FST_CLUS_HI_POS+FDE_DIR_FST_CLUS_HI_SIZE)
#define FDE_DIR_WRT_TIME_SIZE       UINT16_SIZE
#define FDE_DIR_WRT_DATA_POS        (FDE_DIR_WRT_TIME_POS+FDE_DIR_WRT_TIME_SIZE)
#define FDE_DIR_WRT_DATA_SIZE       UINT16_SIZE
#define FDE_DIR_FST_CLUS_LO_POS     (FDE_DIR_WRT_DATA_POS+FDE_DIR_WRT_DATA_SIZE)
#define FDE_DIR_FST_CLUS_LO_SIZE    UINT16_SIZE
#define FDE_DIR_FILE_SIZE_POS       (FDE_DIR_FST_CLUS_LO_POS+FDE_DIR_FST_CLUS_LO_SIZE)
#define FDE_DIR_FILE_SIZE_SIZE      UINT32_SIZE

// FAT directory entry.
typedef struct fat_dir_entry
{
    UINT32 DIR_FileSize;
    UINT16 DIR_WrtTime;
    UINT16 DIR_WrtDate;
    UINT16 DIR_FstClusLO;
    UINT16 DIR_CrtTime;
    UINT16 DIR_CrtDate;
    UINT16 DIR_LstAccDate;
    UINT16 DIR_FstClusHI;
    UINT8  DIR_NTRes;
    UINT8  DIR_CrtTimeTenth;
    UINT8  DIR_Name[FAT_MAX_NAME_LEN];
    UINT8  DIR_Attr;
}FAT_DIR_ENTRY;


// Position and size define for FAT long directory entry.
#define FLDE_LDIR_ORD_POS 0
#define FLDE_LDIR_ORD_SIZE 1

#define FLDE_LDIR_NAME1_POS (FLDE_LDIR_ORD_POS + FLDE_LDIR_ORD_SIZE)
#define FLDE_LDIR_NAME1_SIZE 10

#define FLDE_LDIR_ATTR_POS  (FLDE_LDIR_NAME1_POS + FLDE_LDIR_NAME1_SIZE)
#define FLDE_LDIR_ATTR_SIZE 1

#define FLDE_LDIR_TYPE_POS  (FLDE_LDIR_ATTR_POS + FLDE_LDIR_ATTR_SIZE)
#define FLDE_LDIR_TYPE_SIZE 1

#define FLDE_LDIR_CHKSUM_POS  (FLDE_LDIR_TYPE_POS + FLDE_LDIR_TYPE_SIZE)
#define FLDE_LDIR_CHKSUM_SIZE 1

#define FLDE_LDIR_NAME2_POS  (FLDE_LDIR_CHKSUM_POS + FLDE_LDIR_CHKSUM_SIZE)
#define FLDE_LDIR_NAME2_SIZE 12

#define FLDE_LDIR_FSTCLUSLO_POS  (FLDE_LDIR_NAME2_POS + FLDE_LDIR_NAME2_SIZE)
#define FLDE_LDIR_FSTCLUSLO_SIZE 2

#define FLDE_LDIR_NAME3_POS  (FLDE_LDIR_FSTCLUSLO_POS + FLDE_LDIR_FSTCLUSLO_SIZE)
#define FLDE_LDIR_NAME3_SIZE 4

#define FLDE_LONG_ENTRY_SIZE (FLDE_LDIR_ORD_SIZE +\
                              FLDE_LDIR_NAME1_SIZE +\
                              FLDE_LDIR_ATTR_SIZE +\
                              FLDE_LDIR_TYPE_SIZE +\
                              FLDE_LDIR_CHKSUM_SIZE +\
                              FLDE_LDIR_NAME2_SIZE +\
                              FLDE_LDIR_FSTCLUSLO_SIZE+\
                              FLDE_LDIR_NAME3_SIZE)
// FAT long directory entry.
typedef struct _fat_long_dir_entry
{
    UINT8 LDIR_Ord;         // First is 1,last is (FAT_LONG_ENTRY_LAST_FLAG | N).
    UINT8 LDIR_Name1[FLDE_LDIR_NAME1_SIZE];   // Characters 1-5 of the long-name sub-component in this dir entry.
    UINT8 LDIR_Attr;        // Attributes - must be FAT_ATTR_LONG_NAME.
    UINT8 LDIR_Type;        // If zero, indicates a directory entry that is a sub-component of a
                            // long name. NOTE: Other values reserved for future extensions.
    UINT8 LDIR_Chksum;      // Checksum of name in the short dir entry at the end of the long dir set.
    UINT8 LDIR_Name2[FLDE_LDIR_NAME2_SIZE];   // Characters 6-11 of the long-name sub-component in this dir entry. 
    UINT16 LDIR_FstClusLO;  // Must be ZERO. 
    UINT8 LDIR_Name3[FLDE_LDIR_NAME3_SIZE];    // Characters 12-13 of the long-name sub-component in this dir entry.
}FAT_LONG_DIR_ENTRY;


#define FAT_IS_LAST_LONG_ENTRY(buff)   ((((buff)[FLDE_LDIR_ATTR_POS] & FAT_ATTR_LONG_NAME_MASK) == FAT_ATTR_LONG_NAME) &&\
                                        ((buff)[FLDE_LDIR_ORD_POS] != 0xe5) &&\
                                        (((buff)[FLDE_LDIR_ORD_POS] & 0x40) != 0))
                                        
#define FAT_IS_MIDDLE_LONG_ENTRY(buff) ((((buff)[FLDE_LDIR_ATTR_POS] & FAT_ATTR_LONG_NAME_MASK) == FAT_ATTR_LONG_NAME) &&\
                                        ((buff)[FLDE_LDIR_ORD_POS] != 0xe5) &&\
                                        (((buff)[FLDE_LDIR_ORD_POS] & 0x3F) > 1))
                                        
#define FAT_IS_FIRST_LONG_ENTRY(buff)  ((((buff)[FLDE_LDIR_ATTR_POS] & FAT_ATTR_LONG_NAME_MASK) == FAT_ATTR_LONG_NAME) &&\
                                        ((buff)[FLDE_LDIR_ORD_POS] != 0xe5) &&\
                                        (((buff)[FLDE_LDIR_ORD_POS] & 0x3F) == 1)) 
                                        
#define FAT_IS_SHORT_ENTRY(buff)       ((((buff)[FLDE_LDIR_ATTR_POS] & FAT_ATTR_LONG_NAME_MASK) != FAT_ATTR_LONG_NAME) &&\
                                        ((buff)[FLDE_LDIR_ORD_POS] != 0xe5) &&\
                                        ((buff)[FLDE_LDIR_ORD_POS] != 0x0) &&\
                                        (((buff)[FDE_DIR_ATTR_POS] & ATTR_VOLUME) == 0x0))
#define FAT_IS_FREE_ENTRY(buff)        ((buff)[FLDE_LDIR_ORD_POS] == 0x0)
#define FAT_IS_DELETE_ENTRY(buff)      ((buff)[FLDE_LDIR_ORD_POS] == 0xE5)
#define FAT_IS_JAP_DELETE_ENTRY(buff)  ((buff)[FLDE_LDIR_ORD_POS] == 0x05)
// For short dir entry 
#define FAT_IS_DIR_ENTRY(buff)        (((buff)[11] & ATTR_DIR) != 0x0)
#define FAT_GET_DIR_FSTCLUSLO(buff)   (MAKEFATUINT16((buff)[26],(buff)[27]))



typedef struct cluster_frag
{
    UINT32 iBegClus;
    UINT32 iNum;
    struct cluster_frag *next;
}CLUSTER_FRAG;

typedef struct cluster_chain
{
    struct cluster_frag *pFragChain;
    struct cluster_frag *pLastFrag;
    UINT32 iFragNum;
}CLUSTER_CHAIN;

typedef struct fat_inode_info
{
    //struct inode *next; // delete by suwg at 2007/9/10
    UINT32 BeginSec;    //entry所在的sector在volume中的index
    UINT32 EntryOffset;
    UINT32 EntryCount;
    UINT32 Dirty;
    CLUSTER_CHAIN CluChain;
    FAT_DIR_ENTRY entry;
}FAT_INODE_INFO;


typedef struct fat_sb_info
{
    UINT32 iHiddSec;
    UINT32 iFATStartSec;
    UINT32 iFAT2StartSec;
    UINT32 iFATSize;
    UINT32 iRootDirStartSec;
    UINT32 iDataStartSec;
    UINT32 iRootDirSecCnt;
    UINT32 iFATn;

    UINT16 iRsvdSecCnt;
    UINT16 iBytesPerSec;

    UINT16 iRootEntCnt;
    UINT8  iSecPerClus;
    UINT8  iSuppLName;    // if support long file name.
                          // FAT_NOT_SUPPORT_LONG_NAME: not support    
                          // FAT_SUPPORT_LONG_NAME: is support
                         
    UINT32 iDataSec;      //FAT的File and Directory Data Region所占的sector数
    UINT32 iFat32RootClus; // 32格式的根目录的起始簇。

    UINT32 iFreeCluCnt;             // 空闲簇的计数器。
    UINT32 iFreeCnterInitFlag;  // 空闲簇计数器是否初始化的标志，0未初始化，1已初始化。
    
    //struct inode *inode_chain; // delete by suwg at 2007/9/10
    struct super_block *sb;
}FAT_SB_INFO;



#define FAT_SZ_FR_BUF   12 // (DEFAULT_SECSIZE*2),必须为4的整数倍
#define FAT_NR_SECT_BUF 16 // 在优化读操作之前是6 -> 10 ->16
#define FAT_NR_NAME_BUF 10 // 8->10


struct _FILE_INODE_LIST
{
    struct inode *psInode;
    struct _FILE_INODE_LIST *next;
};
typedef struct _FILE_INODE_LIST* FILE_INODE_LIST;

typedef struct _FAT_CACHE
{
    UINT32 dev;
    UINT32 FATSecNum;    //specifies which sector is cached
    UINT8* FATSectorBuf; //[DEFAULT_SECSIZE * 2];
    FILE_INODE_LIST psFileInodeList;
    BOOL bDirty;
}FAT_CACHE;    // FAT表缓存

extern FAT_CACHE g_FATBuffer;


// 当前FAT卷可用簇的总数，用于提高查询空间使用情况时的效率。
typedef struct _FREE_CLU_COUNTER
{
    UINT32 dev;
    UINT32 nFreeClusterCount;
}FREE_CLU_COUNTER;

typedef struct _SECT_BUF
{
    UINT8 data[DEFAULT_SECSIZE*2];
    UINT32  refcnt;
}SECT_BUF;

#define MAKEFATINT32(a, b, c, d) MAKEINT32(MAKEINT16((a), (b)), MAKEINT16((c), (d)))
#define MAKEFATINT16(a, b) MAKEINT16((a), (b))
#define MAKEFATUINT32(a, b, c, d) MAKEUINT32(MAKEUINT16((a), (b)), MAKEUINT16((c), (d)))
#define MAKEFATUINT16(a, b) MAKEUINT16((a), (b))

// return value defined
#define _ERR_FAT_SUCCESS                0					// successed 
#define _ERR_FAT_PARAM_ERROR            -1001				// parameter error.
#define _ERR_FAT_WRITE_SEC_FAILED       -1002				// write sector failed.
#define _ERR_FAT_READ_SEC_FAILED        -1003				// read sector failed.
#define _ERR_FAT_MALLOC_FAILED          -1004				// allocte memory failed. 
#define _ERR_FAT_DISK_FULL              -1005				// the logical disk is full.
#define _ERR_FAT_ROOT_FULL              -1006				// the root dir is full.
#define _ERR_FAT_READ_EXCEED            -1007				// read file exceed. 
#define _ERR_FAT_WRITE_EXCEED           -1008				// write file exceed. 
#define _ERR_FAT_NOT_SUPPORT            -1009				// not supported
#define _ERR_FAT_BPB_ERROR              -1010				// PBP struct error.
#define _ERR_FAT_DIR_NOT_NULL           -1016               // Dir not null
#define _ERR_FAT_NO_MORE_INODE          -1017               // No more inode to alloc
#define _ERR_FAT_FILE_TOO_MORE          -1018
#define _ERR_FAT_FILE_NOT_EXISTED       -1020               // File or dir not existed
#define _ERR_FAT_DIFF_DEV               -1021               // Rename on different device
#define _ERR_FAT_NOT_MATCH_CLUSTER      -1022               // Position to cluster not found.

#define _ERR_FAT_HAS_FORMATED           -1023               // have formated.
#define _ERR_FAT_NOT_FORMAT             -1024               // not formated.
#define _ERR_FAT_ERROR                  -1025               // Fat unknow error
#define _ERR_FAT_STR_FORMAT_ERROR       -1027               // The input string format is error, 
                                                            // used the unicode2oem or oem2unicode 

#define FAT_VOLLABLE				"CSD19730821"           // voluem lable 
#define FAT_OEMNAME 				"MSWIN4.1"               // oem name
#define FAT_MAGIC					0xa1b2c3d4              // migic number 
#define FAT_FSI_TRAILSIG			0xAA55   				// boot sector end symbol.
#define FAT_ROOT_ENTRY_COUNT	    	512                 // root directory entry count.
#define FAT_RESERVED_SECTOR_COUNT_16    1					// FAT16 reserved sector count .
#define FAT_RESERVED_SECTOR_COUNT_32    32					// FAT32 reserved sector count.
#define FAT_HIDE_SECTOR_COUNT           0                   // hide sector count.
#define FAT_FAT_COUNT				    2                   // fat count.

// Look up flag define.
#define FAT_LOOK_EXISTED              0x1                  // 
#define FAT_LOOK_GET_NAME             0x2                  // if found the entry,get the filename  
// #define FAT_LOOK_IN_SE                0x4                  // look up in short entry
// #define FAT_LOOK_IN_LE                0x4                  // look up in long entry


VOID FBR2Buf(FAT_BOOT_RECORD *fbr, UINT8 *buf);
VOID Buf2FBR(UINT8 *buf, FAT_BOOT_RECORD *fbr);
VOID FDE2Buf(FAT_DIR_ENTRY *fde, UINT8 *buf);
VOID Buf2FDE(UINT8 *buf, FAT_DIR_ENTRY *fde);
VOID Buf2FLDE(UINT8 *buf, FAT_LONG_DIR_ENTRY *flde);
VOID FLDE2Buf(FAT_LONG_DIR_ENTRY *flde,UINT8 *buf);
UINT8 fat_GetSecNumPerClus_1216(UINT32 iDiskTotSec);
UINT8 fat_GetSecNumPerClus_32(UINT32 iDiskTotSec);
VOID fat_GetFormatInfo(
			UINT32 iDiskTotSec,
			UINT8* vollab,
			UINT8 vollab_len,
			UINT8* oemname,
			UINT8 oemname_len,
			FAT_BOOT_RECORD *pFbr,
			UINT32* pType,
			UINT32* pDataClus,
			UINT32* pRootDirSects
			);


INT32 fat_read_cluster(FAT_SB_INFO *sb_info, UINT32 clunum, 
                        UINT32 offset, UINT8 *secbuf);
INT32 fat_write_cluster(FAT_SB_INFO *sb_info, UINT32 clunum, 
                         UINT32 offset, UINT8 *secbuf);

INT32 fat_get_next_cluster(FAT_SB_INFO *sb_info, UINT32 iPreClu, UINT32 *pNexClu);
BOOL  fat_is_last_cluster(FAT_SB_INFO *sb_info, UINT32 fatent);
INT32 fat_get_free_cluster(FAT_SB_INFO *sb_info, UINT32 iPreClu, BOOL fCleanClu, UINT32 *pFreeClu);
BOOL  fat_is_inode_relating_with_fatcache(CONST struct inode *psInode, FAT_CACHE **ppsFATCache);
INT32 fat_relate_inode_with_fatcache(FAT_CACHE *psFATCache, struct inode *psInode);
INT32 fat_synch_relating_file(FAT_SB_INFO *sb_info, FAT_CACHE *psFATCache);
INT32 fat_synch_fat_cache(FAT_SB_INFO *sb_info, FAT_CACHE *psFATCache);
INT32 fat_set_fat_entry_value(FAT_SB_INFO *sb_info, 
                              UINT32 fatent,
                              UINT32 value, 
                              BOOL bSynchFlag,
                              struct inode *psInode
                              );

INT32 fat_lookup_entry(
        FAT_SB_INFO *sb_info,
        UINT32 startclu, 
        WCHAR *filename, 
        FAT_DIR_ENTRY *d_entry, 
        UINT32 *bclu, 
        UINT16 *bsec, 
        UINT16 *eoff,
        UINT16 *ldecount,
        UINT32 iFlag
        );

INT32 NameUser2Fat(PCSTR userName, PSTR fatName);
INT32 NameFat2User(PCSTR fatName, PSTR userName);


// FAT API Begin
INT32 fat_read_sb_info(struct super_block * sb);

INT32 fat_add_entry(struct inode *diri, FAT_DIR_ENTRY *de,
            UINT32* bclu, UINT16 *bsec, UINT16 *eoff, WCHAR* pszFileName, BOOL bRename);
INT32 fat_add_root_entry(struct inode *rdiri, FAT_DIR_ENTRY *de, 
            UINT16 *bsec, UINT16 *eoff, WCHAR* pszFileName, BOOL bRename);
INT32 fat_remove_entry(FAT_SB_INFO *sb_info, struct fat_dir_entry *de, 
                UINT32 bclu, UINT16 bsec, UINT16 eoff, BOOL bRename);

UINT8 fat_GetFatType(UINT32 iCountOfClus);
UINT8 fat_GetFatTypeForFormat(UINT32 iDiskTotSec);
INT32 fat_GetRandNum();
INT32 fat_GetFATSz(UINT32 iMatedType,
                   UINT32 iTotalSec,
                   UINT32 iSecPerClus,
                   UINT32 iRootDirSec
                   );

INT32 fat_write_entries(FAT_SB_INFO* sb, 
            UINT32 bclu, UINT16 bsec, UINT16 eoff, UINT8* ebuf, UINT8 ecount);
INT32 fat_write_root_entries(FAT_SB_INFO* sb,
                UINT16 bsec, UINT16 eoff, UINT8* ebuf, UINT8 ecount);
INT32 fat_read_root_entries(FAT_SB_INFO* sb,
                UINT16 bsec, UINT16 eoff, UINT8* ebuf, UINT8* ecount);
INT32 fat_read_entries(FAT_SB_INFO* sb, UINT32 bclu,
                UINT16 bsec, UINT16 eoff, UINT8* ebuf, UINT8* ecount);  
UINT16 fat_get_long_name(FAT_LONG_DIR_ENTRY* pLDe,UINT8* pszName);

VOID fat_name_change(UINT8* pBuff,UINT32 iLen);

#ifdef FS_PRIVATE_MEM_MANAGE_WITHOUT_GROUP
UINT8* fat_sect_buf_alloc();
VOID fat_sect_buf_free(SECT_BUF* secbuf);
#define FAT_SECT_BUF_ALLOC()   fat_sect_buf_alloc()
#define FAT_SECT_BUF_FREE(secbuf)   fat_sect_buf_free((SECT_BUF*)secbuf)
#else
#define FAT_SECT_BUF_ALLOC()   sxr_Malloc(DEFAULT_SECSIZE*2)
#define FAT_SECT_BUF_FREE(secbuf) sxr_Free(secbuf)
#endif

//#define _DSM_DEBUG_
#if defined(_DSM_DEBUG_)
typedef struct _NAME_BUF
{
    WCHAR data[FAT_MAX_LONG_PATH_LEN + LEN_FOR_NULL_CHAR];
    UINT32  refcnt;
}NAME_BUF;

UINT8* name_buf_alloc();
VOID name_buf_free(UINT8* pNameBuf);

#define NAME_BUF_ALLOC()  name_buf_alloc()
#define NAME_BUF_FREE(pNameBuf)     name_buf_free(pNameBuf)

#else // _DSM_DEBUG_
#define NAME_BUF_ALLOC()  (WCHAR*)sxr_Malloc((FS_PATH_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR))
#define NAME_BUF_FREE(pNameBuf)     sxr_Free(pNameBuf)
#endif // _DSM_DEBUG_

INT32 fat_long_to_short(WCHAR* pszLongName,
                        PSTR pszPName,
                        UINT32* piPLen,
                        PSTR pszEName,
                        UINT32* piELen,
                        UINT32* pLossy);

INT32 fat_get_long_entry(FAT_SB_INFO *psSBInfo,
                         UINT32 iStartClu,
                         WCHAR* pszFileName,
                         FAT_LONG_DIR_ENTRY* psLDe,
                         UINT32* pCount);
VOID fat_Trim(WCHAR* pBuff);
UINT8 fat_ChkSum (UINT8 *pFcbName);
BOOL fat_is_long_name_valid(WCHAR* pszFileName);
BOOL fat_is_short_name_valid(UINT8* pszFileName);
BOOL fat_is_long_name(WCHAR* pszFileName);


// FAT API End.

INT32 fat_update_dentry(struct inode *inode, UINT8 *secbuf);
INT32 fat_trunc_fatentry(FAT_SB_INFO *sb_info, UINT32 lFstCluLO);
INT32 fat_stat_free_cluster(FAT_SB_INFO *sb_info, UINT32 *piFreeCluNum);
INT32 fat_set_fat_entry_all(FAT_SB_INFO *sb_info, UINT8* pFat,UINT32 iClusCount);
INT32 fat_read_fatentries(FAT_SB_INFO *sb_info, UINT32 iStartClu, CLUSTER_CHAIN *pChain);
INT32 fat_fpos2CSO(struct inode *inode, UINT32 iPos, UINT32 *piClu, UINT32 *piSec, UINT32 *piOff, UINT32 *pLastCluster);
INT32 fat_append_fatentries_chain(CLUSTER_CHAIN *pChain, UINT32 iClu);
INT32 fat_free_fatentries(CLUSTER_CHAIN *pChain);
INT32 fat_get_name_for_entrys(FAT_SB_INFO* sb_info, WCHAR* filename, UINT32 bclu, UINT16 bsec, UINT16 eoff);
UINT32 fat_fpos2cluster(struct inode *inode, CLUSTER_CHAIN *pChain, UINT32 iPos, UINT32 *pPreClu);
INT32 fat_format_set_fat_entry(INT32 iDevNo, UINT32 iFatType, FAT_BOOT_RECORD *psFbr, UINT32 fatent, UINT32 value, UINT8* secBuf);

#endif //_CII_FAT_BASE_H

