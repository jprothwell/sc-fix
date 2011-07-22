#ifndef _CII_FS_BASE_H
#define _CII_FS_BASE_H


//ϵͳ��ʼ��ʱ���д���ɨ��֧�ֵ����Ŀ¼������
#define FS_NR_SCANDISK_MAX_LAYERS  16

//ϵͳ���ܴ�file��,һ��open������Ӧһ��struct file. 
#define FS_NR_OPEN_FILES_MAX    32//(2 * FS_NR_INODES_MAX ) // 32

// task_struct���涨��ÿ��task��ͬʱ���ļ����������
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

// Minix�ļ�ϵͳ�и�Ŀ¼ʹ��1��i�ڵ㣬����FAT12��FAT16�У�
// 1�ᱻת���ɸ�Ŀ¼�µĵڶ���Ŀ¼�������λ�õ��³���
#define FS_ROOT_INO                 (~0) 

#define LEN_FOR_NULL_CHAR 1

#define FS_Down()   sxr_TakeSemaphore(g_fs_sem)
#define FS_Up()     sxr_ReleaseSemaphore(g_fs_sem) 

// FS��������Ϣ��
struct fs_cfg_info
{
    UINT32 uNrSuperBlock; // super-block���������
    UINT32 uNrOpenFileMax; // ÿ��task����ͬʱ���ļ����������
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
    union  // ����ǰ����Ϊ�˵��Է��㣬û����������
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
 * Ŀǰ���tast_structΪFS�ڲ����岢ʹ�ã�ϵͳ������task�����Ŀ¼root������Ŀ¼pwd��
 * ����ڶ�task��ϵͳ��ÿ��task��Ҫӵ�и��Եĸ�Ŀ¼������Ŀ¼�Ļ���
 * ��FS��ʹ��kernel�������task_struct������Ӧ�Ķ���
 */
struct task_struct
{
    struct inode * pwd;
    struct inode * root;
    struct file  ** filp;
    WCHAR *wdPath; // ��ǰ����Ŀ¼��·����
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
��FS_FindXXX����API��, ���ǵ�һ����, handle-for-finding��
handle-for-regular-file����process��file{}������±�, ֻ����
��ǰ��, file��f_pos��ת����: ����¼����
directroy-inode�е�dents��������һ����ƥ���entry.

�������ת��, ��FS_FindXXX����API��, ������
handle�Ƿ�������finding��.
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

