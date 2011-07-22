#ifndef _CII_FS_BASE_H
#define _CII_FS_BASE_H


//系统初始化时进行磁盘扫描支持的最大目录层数。
#define FS_NR_SCANDISK_MAX_LAYERS  16

//系统中能打开file数,一个open操作对应一个struct file. 
#define FS_NR_OPEN_FILES_MAX    32//(2 * FS_NR_INODES_MAX ) // 32

// task_struct里面定义每个task能同时打开文件的最大数。
#define FS_NR_TASK_OPEN_FILE_MAX           FS_NR_OPEN_FILES_MAX 


#define FS_FLAG_NOT_FIND_FILE   0
#define FS_FLAG_FIND_FILE       1

#define FS_NR_SUPER 4

#define PATTERN_CMP_MATCH           1
#define PATTERN_CMP_UNMATCH         2

#define DOT_FOUND                   1
#define DOTDOT_FOUND                2
#define DIR_ENTRY_FOUND             4
#define DIR_ENTRY_NOT_FOUND         5

// Minix文件系统中根目录使用1号i节点，但在FAT12及FAT16中，
// 1会被转换成根目录下的第二个目录项的物理位置导致出错。
#define FS_ROOT_INO                 (~0) 

#define LEN_FOR_NULL_CHAR 1

#define FS_Down()   sxr_TakeSemaphore(g_fs_sem)
#define FS_Up()     sxr_ReleaseSemaphore(g_fs_sem) 

// FS的配置信息。
struct fs_cfg_info
{
    UINT32 uNrSuperBlock; // super-block的最大数。
    UINT32 uNrOpenFileMax; // 每个task可以同时打开文件的最大数。
};

struct super_block
{
    UINT32  s_dev; /*MUST be long, for we will convert it to pointer*/
    UINT32  s_magic;
    struct inode * s_root;
    struct inode *s_mountpoint;
    UINT32 s_time;
    //struct task_struct * s_wait;
    UINT8 s_lock;
    UINT8 s_rd_only;
    UINT8 padding[2];
    UINT8 s_fstype;
    struct inode *inode_chain;
    union 
    {
        struct fat_sb_info  fat_sb;
        void       *generic_sbp;
    } u;
};

struct inode
{
    union  // 放在前面是为了调试方便，没有其他用意
    {
        struct fat_inode_info		fat_i;
        //struct ffs_inode_info		ffs_i;
        void						*generic_ip;
    } u;
    
    UINT32 i_dev;				/*MUST be long, for we will convert it to pointer*/
    UINT32	i_ino;				/*inode number*/
    UINT32 i_size;		    /*the length of the file*/
    UINT16 i_mode;
    UINT16 i_count;
    UINT8  i_nlinks;
    UINT8  i_is_mountpoint;
    UINT8  i_lock;
    UINT8  pad;
    UINT32  i_fstype;
    UINT32 i_atime;       /*Time of last access to the file*/
    UINT32 i_mtime;      /*Time of last data modification*/
    UINT32 i_ctime;       /*Time of last status(or inode) change*/
    struct super_block	*i_sb;
    struct inode *next;
};

struct dentry
{
    struct inode * inode;
    WCHAR name [FS_FILE_NAME_LEN + LEN_FOR_NULL_CHAR];
    UINT32 append_data;		// append data
    UINT32 append_data2;
};

struct file
{
    UINT16 f_mode;
    UINT16 f_flags;
    UINT16 f_count;
    UINT16 padding;
    UINT32 f_pos;
    PVOID  f_pattern;
//    PVOID  f_prvdata;
    struct inode * f_inode;
    struct file *next;
};


/*
 * 目前这个tast_struct为FS内部定义并使用，系统的所有task共享根目录root及工作目录pwd；
 * 如果在多task的系统中每个task需要拥有各自的根目录及工作目录的话，
 * 则FS须使用kernel所定义的task_struct并做相应改动。
 */
struct task_struct
{
    struct inode * pwd;
    struct inode * root;
    struct file  ** filp;
    WCHAR *wdPath; // 当前工作目录的路径。
};


#ifdef _DSM_DEBUG_
typedef struct _FS_DENTRY_BUF
{
    struct dentry data;
    UINT32  refcnt;
}FS_DENTRY_BUF;

struct dentry* dentry_alloc();
VOID dentry_free(struct dentry* psDe);

#define DENTRY_ALLOC()  dentry_alloc()
#define DENTRY_FREE(psDe)     dentry_free(psDe)

#else
#define DENTRY_ALLOC()  (struct dentry*)sxr_Malloc(sizeof(struct dentry))
#define DENTRY_FREE(psDe)     sxr_Free(psDe)
#endif // #ifdef _DSM_DEBUG_


#define IS_COMPONENT_SEPATATOR( c ) ( _T('\\') == (c) || _T('/') == (c) )
#define IS_VALID_DRIVER_NAME( c ) ( (_T('c') <= (c) && _T('z') >= (c)) ||(_T('C') <= (c) && _T('Z') >= (c)) )


// local function 
void iput( struct inode * inode );
struct inode * iget( struct super_block *sb, UINT32 inr );

extern struct fs_cfg_info g_fs_cfg;

#define FD_IS_VALID( fd ) ((fd) >= 0 && (INT32)( fd ) < (INT32)g_fs_cfg.uNrOpenFileMax)

/*
在FS_FindXXX序列API中, 考虑到一致性, handle-for-finding和
handle-for-regular-file都是process的file{}数组的下标, 只不过
对前者, file的f_pos域被转义了: 它记录的是
directroy-inode中的dents链表中下一个待匹配的entry.

由于这个转义, 在FS_FindXXX序列API中, 必须检查
handle是否是用于finding的.
*/
#define FS_O_FIND		        00400

//BOOL is_unicode_name_too_long(CONST WCHAR *UniPathname );
BOOL is_file_name_valid( PCWSTR pathname, BOOL bIgnoreBaseName );
struct inode * dir_namei( PCWSTR pathname, INT32* namelen, CONST WCHAR** basename );
int find_entry( struct inode ** dir_i, struct dentry *de );
WCHAR* format_dir_name( PCWSTR pathname );
int pattern_match( PCWSTR name, PCWSTR pattern );
int do_find_entry( struct inode ** dir_i, struct dentry *de, UINT32 iFlag);
struct inode * get_empty_inode( void );
struct inode * namei( PCWSTR pathname );
struct super_block * get_super( UINT32 devnr );
struct inode* inode_list_add(struct inode **ppsInodeList, struct inode *psInode);
INT32 inode_list_del(struct inode **ppsInodeList, struct inode *psInode);
INT32 fs_GlobalVarInit();
INT32 fs_GlobalVarFree();

INT32 fs_GetSuperBlockFree(struct super_block** psSuperBlock);
INT32 fs_FindSuperBlock(UINT32 iDev,struct super_block** psSuperBlock);
INT32 fs_SetWorkDirPath(WCHAR* WorkDirPath, WCHAR* ChangeDirPath);

INT32 FS_ScanDisk(PCSTR pszDevName,UINT8 iFsType);
INT32 fs_SetErrorCode(INT32 iErrCode);

struct file* fs_alloc_fd( void );
struct file* fs_fd_list_add(struct file **ppsFileList, struct file *psFD);
INT32 fs_fd_list_del(struct file **ppsFileList, struct file *psFD);
BOOL fs_is_file_opened(struct file **ppsFileList, struct file *psFD);

INT32 fs_close(INT32 fd);
INT32 fs_findclose(INT32 fd);


#ifdef FS_PRIVATE_MEM_MANAGE_WITHOUT_GROUP
void get_mega_kilo_byte( int n, int *mega, int *kilo, int *byte );
void* private_malloc( UINT32 size );
void private_free( void *ptr );
#endif // #ifdef FS_PRIVATE_MEM_MANAGE_WITHOUT_GROUP

#define _ERR_FS_SUCCESS             ERR_SUCCESS    

#define _ERR_FS_NO_MORE_SB_ITEM       -101001 // no more super block item in the table.
#define _ERR_FS_NOT_MOUNT             -101002 // the device unmount.
#endif //_CII_FS_BASE_H

