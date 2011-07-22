#include "fat_base.h"
#include "fs.h"
#include "fs_base.h"

#include <string.h>

#include "fss_debug.h"

#include "sxr_ops.h"


struct task_struct g_fs_current = {NULL, NULL, NULL, NULL};

#if 0
struct super_block g_fs_super_block_table[ FS_NR_SUPER ] = 
{
    /*Must initialize dev_nr as INVALID_DEVICE_NUMBER*/    
    { INVALID_DEVICE_NUMBER,}, 
    { INVALID_DEVICE_NUMBER,},
    { INVALID_DEVICE_NUMBER,}, 
    { INVALID_DEVICE_NUMBER,}, 
};
#endif

struct super_block* g_fs_super_block_table = NULL;

struct file* g_fs_file_list = NULL;

//struct inode g_fs_inode_table[ FS_NR_INODES_MAX ] = {{{{0,},},},};


struct fs_cfg_info g_fs_cfg = {FS_NR_SUPER, FS_NR_TASK_OPEN_FILE_MAX};

INT32 fs_GlobalVarInit()
{
    g_fs_super_block_table = sxr_Malloc((SIZEOF(*g_fs_super_block_table)) * g_fs_cfg.uNrSuperBlock);
    if (NULL == g_fs_super_block_table)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fs_GlobalVarInit: malloc g_fs_super_block_table failed!");
        return ERR_FS_NO_MORE_MEMORY;
    }
    memset(g_fs_super_block_table, 0, (SIZEOF(*g_fs_super_block_table)) * g_fs_cfg.uNrSuperBlock);

    g_fs_current.filp = sxr_Malloc((sizeof( struct file * ) ) * g_fs_cfg.uNrOpenFileMax);
    if (NULL == g_fs_current.filp)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fs_GlobalVarInit: malloc g_fs_current.filp failed!");
        goto failed;
    }
    g_fs_current.wdPath = sxr_Malloc((FS_PATH_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR));
    if (NULL == g_fs_current.wdPath)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fs_GlobalVarInit: malloc g_fs_current.wdPath failed!");
        goto failed;
    }
    g_fs_current.pwd = NULL;
    g_fs_current.root = NULL;
    memset( g_fs_current.filp, 0x00, (UINT16)((sizeof( struct file * ) ) * g_fs_cfg.uNrOpenFileMax));
    memset( g_fs_current.wdPath, 0x00, (FS_PATH_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR));

    g_FATBuffer.FATSectorBuf = sxr_Malloc(DEFAULT_SECSIZE * 2);
    if (NULL == g_FATBuffer.FATSectorBuf)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fs_GlobalVarInit: malloc g_FATBuffer.FATSectorBuf failed!");
        sxr_Free(g_fs_super_block_table);
        goto failed;
    }

    return ERR_SUCCESS;

failed:
    if (g_fs_super_block_table != NULL)
    {
        sxr_Free(g_fs_super_block_table);
        g_fs_super_block_table = NULL;
    }
    
    if (g_fs_current.filp != NULL)
    {
        sxr_Free(g_fs_current.filp);
        g_fs_current.filp = NULL;
    }
    if (g_fs_current.wdPath != NULL)
    {
        sxr_Free(g_fs_current.wdPath);
        g_fs_current.wdPath = NULL;
    }
    g_fs_current.pwd = NULL;
    g_fs_current.root = NULL;

    if (g_FATBuffer.FATSectorBuf != NULL)
    {
        sxr_Free(g_fs_super_block_table);
        g_FATBuffer.FATSectorBuf = NULL;
    }

    return ERR_FS_NO_MORE_MEMORY;
}


INT32 fs_GlobalVarFree()
{
    if (g_fs_super_block_table != NULL)
    {
        sxr_Free(g_fs_super_block_table);
        g_fs_super_block_table = NULL;
    }
    
    if (g_fs_current.filp != NULL)
    {
        sxr_Free(g_fs_current.filp);
        g_fs_current.filp = NULL;
    }
    if (g_fs_current.wdPath != NULL)
    {
        sxr_Free(g_fs_current.wdPath);
        g_fs_current.wdPath = NULL;
    }
    g_fs_current.pwd = NULL;
    g_fs_current.root = NULL;

    if (g_FATBuffer.FATSectorBuf != NULL)
    {
        sxr_Free(g_fs_super_block_table);
        g_FATBuffer.FATSectorBuf = NULL;
    }

    return ERR_SUCCESS;
}


INT32 fs_SetWorkDirPath(WCHAR *WorkDirPath, WCHAR *ChangeDirPath)
{
    WCHAR *p = NULL;
    WCHAR *tmp = NULL;
    WCHAR *p2 = NULL;
    WCHAR *dirName;
    INT32 nameLen;
    WCHAR c;
    WCHAR c2;
    UINT8 cheat = 1;
    
    if (!WorkDirPath || !ChangeDirPath)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "in FS_SetWorkDirPath, parameter error");
        return ERR_FS_INVALID_PARAMETER;
    }

    tmp = (WCHAR*)sxr_Malloc((FS_PATH_LEN + LEN_FOR_NULL_CHAR) * SIZEOF(WCHAR));
    if (NULL == tmp)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "fs_SetWorkDirPath: Malloc failed!");
        return ERR_FS_NO_MORE_MEMORY;
    }

    DSM_TcStrCpy(tmp, WorkDirPath);
    p = tmp;
    
    if (_T('\\') == *ChangeDirPath || _T('/') == *ChangeDirPath)
    {
        // ChangeDirPath为绝对路径。
        *p = 0;
    }
    else
    {
        // ChangeDirPath为相对路径，初始化p指向当前工作路径的字符串末尾。
        while (*p)
        {
            p++;
        }
        while (p > tmp && (_T('/') == (c = *(p - 1)) || _T('\\') == c))
        {
            p--;
            *p = 0;
        }
    }

    while (_T('\\') == *ChangeDirPath || _T('/') == *ChangeDirPath)
    {
        ChangeDirPath++;
    }
    while (cheat)
    {
        while (_T(' ') == *ChangeDirPath)
        {// 去掉名字前面的空格。
            ChangeDirPath++;
        }
        dirName = ChangeDirPath;
        while ( (c = *(ChangeDirPath++)) != 0 && c != _T('\\') && c != _T('/'))
        {
            ;
        }
        p2 = ChangeDirPath - 1;
        while (p2 > dirName && (_T(' ') == *p2))
        {// 去掉名字后面的空格。
            p2--;
        }
        if ((nameLen = p2 - dirName) > 0)
        {
            if (1 == nameLen && 0 == DSM_TcStrNCmp(dirName, (PCWSTR)_T("."), 1))
            {
               ;
            }
            else if (2 == nameLen && 0 == DSM_TcStrNCmp(dirName, (PCWSTR)_T(".."), 2))
            {
                while (p > tmp && (c2 = *(--p)) != _T('\\') && c2 != _T('/'))
                {
                    ;//
                }
                *p = 0;
            }
            else
            {
                if (p + (nameLen + 1) > tmp + FS_PATH_LEN)
                {
                    sxr_Free(tmp);
                    return ERR_FS_PATHNAME_PARSE_FAILED;
                }
                DSM_TcStrNCpy(p, (PCWSTR)_T("/"), 1);
                p++;
                DSM_TcStrNCpy(p, dirName, nameLen);
                p += nameLen;
            }
        }
        if (!c)
        {
            break;
        }
    }
    
    if (p == tmp)
    {
        DSM_TcStrCpy(p, (PCWSTR)_T("/"));
    }
    else
    {
        *p = _T('\0');
    }
    DSM_TcStrCpy(WorkDirPath, tmp);
    sxr_Free(tmp);
    return ERR_SUCCESS;

}


INT32 fs_GetSuperBlockFree(struct super_block** psSuperBlock)
{
    INT32 iRet = _ERR_FS_NO_MORE_SB_ITEM;
    INT32 i;

    for ( i = 0; i < (INT32)g_fs_cfg.uNrSuperBlock; i++ ) // FS_NR_SUPER is 1 here.
    {
        if( NULL == g_fs_super_block_table[ i ].s_root)
        {
            *psSuperBlock = g_fs_super_block_table + i;
            iRet = _ERR_FS_SUCCESS;
            break;
        }
    }
    if(iRet != _ERR_FS_SUCCESS)
    {
        SVC_FSS_TRACE(DL_VFSWARNING, "in fs_GetSuperBlockFree  super_block_table is full\n");
    }
    return iRet;
}


INT32 fs_FindSuperBlock(UINT32 iDev,struct super_block** psSuperBlock) 
{
    INT32 iRet = _ERR_FS_NOT_MOUNT;
    INT32 i;

    for ( i = 0; i < (INT32)g_fs_cfg.uNrSuperBlock; i++ ) // FS_NR_SUPER is 1 here.
    {
        if( iDev == g_fs_super_block_table[ i ].s_dev)
        {
            *psSuperBlock = g_fs_super_block_table + i ;
            iRet = _ERR_FS_SUCCESS;
            break;
        }
    }
    if(_ERR_FS_SUCCESS != iRet)
    {
        SVC_FSS_TRACE(DL_VFSWARNING, "in fs_FindSuperBlock  super_block is not found,iDev =  \n",iDev);        
    }
    return iRet;
}


struct file* fs_alloc_fd( void )
{
    struct file * fd = NULL;

    fd = (struct file *)sxr_Malloc(SIZEOF(*fd));
    if( NULL == fd)
    {
        return NULL;
    }
    
    memset( fd, 0x00, sizeof(*fd) );
    return fd;
}


struct file* fs_fd_list_add(struct file **ppsFileList, struct file *psFD)
{
    if (NULL == ppsFileList || NULL == psFD)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fs_fd_list_add: _ERR_FAT_PARAM_ERROR, ppsFileList(0x%x), psFD(0x%x)", ppsFileList, psFD);
        return (struct file *)NULL;
    }
    
    psFD->next = *ppsFileList;
    *ppsFileList = psFD;
    return psFD;
}


INT32 fs_fd_list_del(struct file **ppsFileList, struct file *psFD)
{
    struct file **ppPrei;
    struct file *pCuri;
    
    if (NULL == ppsFileList || NULL == psFD)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fs_fd_list_del: _ERR_FAT_PARAM_ERROR, ppsFileList(0x%x), psFD(0x%x)", ppsFileList, psFD);
        return ERR_FS_INVALID_PARAMETER;
    }

    ppPrei = ppsFileList;
    pCuri = *ppsFileList;
    while (pCuri != NULL)
    {
        if (pCuri == psFD)
        {
            *ppPrei = pCuri->next;
            sxr_Free(pCuri);
            return ERR_SUCCESS;
        }
        else
        {
            ppPrei = &(pCuri->next);
            pCuri = pCuri->next;
        }
    }
    return ERR_FS_INVALID_PARAMETER;
    
}


BOOL fs_is_file_opened(struct file **ppsFileList, struct file *psFD)
{
    struct file **ppPrei;
    struct file *pCuri;
    
    if (NULL == ppsFileList || NULL == psFD)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fs_fd_list_del: _ERR_FAT_PARAM_ERROR, ppsFileList(0x%x), psFD(0x%x)", ppsFileList, psFD);
        return FALSE;
    }

    ppPrei = ppsFileList;
    pCuri = *ppsFileList;
    while (pCuri != NULL)
    {
        if (pCuri == psFD)
        {
            return TRUE;
        }
        else
        {
            ppPrei = &(pCuri->next);
            pCuri = pCuri->next;
        }
    }
    return FALSE;
}


#ifdef FS_PRIVATE_MEM_MANAGE_WITHOUT_GROUP

#define FORMAT_STRING "%-36s: %-8d[0x%-8x] ( %-4dM %-4dK %-4dB)"

struct mem_free_blk
{
    int       nunits;
    struct mem_free_blk* next;
};

struct ext_mem_free_blk
{
    int       nunits;
    struct mem_free_blk* next;
    int      max_alloced;
    int      cur_alloced;
    int      max_counter;
    int      cur_counter;
};

#define MEM_UNIT_SIZE sizeof( struct mem_free_blk )


struct mem_free_blk * g_fs_mm_free_list = NULL;
struct mem_free_blk g_fs_mm_blocks[ 512 * 4] = { { 0, NULL }, }; // 16k space
struct ext_mem_free_blk g_fs_mm_base;

// TODO: HANDLE g_fs_mm_sem;  // 必要时需要加上信号量

void get_mega_kilo_byte( int n, int *mega, int *kilo, int *byte )
{
    *mega = *kilo = *byte = 0;

    if ( n > 1024 * 1024 )
        *mega = n / (1024 * 1024);
    n -= (*mega) * ( 1024 * 1024 );
    if ( n > 1024 )
        *kilo = n / 1024;
    n -= (*kilo) * 1024;
    *byte = n;
}

void* private_malloc( UINT32 size )
{
    int nunits;
    struct mem_free_blk **pp;
    struct mem_free_blk *p;
    int mega = 0, kilo = 0, byte = 0;
    int remained = 0;
    
    if ( 0 == size )
        return NULL;

    // TODO: down( &g_fs_mm_sem );
    
    if ( !g_fs_mm_free_list )
    {
        g_fs_mm_free_list = (struct mem_free_blk*)&g_fs_mm_base;
        g_fs_mm_base.nunits = 0;
        g_fs_mm_base.next = g_fs_mm_blocks;
        g_fs_mm_base.max_alloced = g_fs_mm_base.cur_alloced = 0;
        g_fs_mm_base.cur_counter = 0;
        g_fs_mm_base.max_counter = 0;
        g_fs_mm_blocks[ 0 ].next = (struct mem_free_blk*)&g_fs_mm_base;
        g_fs_mm_blocks[ 0 ].nunits = sizeof(g_fs_mm_blocks) / sizeof(*g_fs_mm_blocks);
    }
    
    nunits =( ( size - 1 ) / MEM_UNIT_SIZE + 1 ) + 1/* for header itself */;
    
    for ( pp = &(g_fs_mm_free_list->next); *pp != g_fs_mm_free_list; pp = &( (*pp)->next)  )
    {
        if ( (*pp)->next != g_fs_mm_free_list && *pp > (*pp)->next )
        {
            SVC_FSS_TRACE(DL_VFSWARNING, "disordered list!!!!!!!!!!!!!!\n");
            goto failed;
        }

        if( (*pp)->nunits >= nunits )
        {
            if ( (*pp)->nunits == nunits )
            {
                p = *pp;
                *pp = p->next;
            }
            else
            {
                (*pp)->nunits -= nunits;
                p = *pp + (*pp)->nunits;
                p->nunits = nunits;
            }
            
            p->next = (struct mem_free_blk *)0xDEADBEEF;
            g_fs_mm_base.cur_counter++;
            if ( g_fs_mm_base.cur_counter > g_fs_mm_base.max_counter )
                g_fs_mm_base.max_counter = g_fs_mm_base.cur_counter;

            g_fs_mm_base.cur_alloced += (nunits - 1 ) * MEM_UNIT_SIZE;
            if ( g_fs_mm_base.cur_alloced > g_fs_mm_base.max_alloced )
                g_fs_mm_base.max_alloced = g_fs_mm_base.cur_alloced;
            
            // TODO: up( &g_fs_mm_sem );
            return ( void* )( p + 1 );
        }
    }
failed:    

    SVC_FSS_TRACE(DL_VFSWARNING, "memory: 0x%p --> 0x%p\n",
        g_fs_mm_blocks, (char*)g_fs_mm_blocks + sizeof( g_fs_mm_blocks ));

    for ( p = g_fs_mm_free_list->next; p != g_fs_mm_free_list; p = p->next  )
    {
        remained += p->nunits * MEM_UNIT_SIZE;
        SVC_FSS_TRACE(DL_VFSWARNING,"freed block(offset: %08p,  size: %04d)\n", 
            p, p->nunits * MEM_UNIT_SIZE);
    }
    SVC_FSS_TRACE(DL_VFSWARNING,"remained memory: %d\n", remained);

    for ( p = g_fs_mm_free_list->next; p != g_fs_mm_free_list; p = p->next  )
    {
        if ( p->next != g_fs_mm_free_list )
        {
            SVC_FSS_TRACE(DL_VFSWARNING,"fragment   (offset: %08p,  size: %04d)\n", 
                p + p->nunits, ( p->next - p ) * MEM_UNIT_SIZE);
        }
    }
    
    get_mega_kilo_byte( g_fs_mm_base.cur_alloced, &mega, &kilo, &byte );
    SVC_FSS_TRACE(DL_VFSWARNING, FORMAT_STRING"\n", "static memory allocated",
                     g_fs_mm_base.cur_alloced, g_fs_mm_base.cur_alloced, mega, kilo, byte);

    get_mega_kilo_byte( g_fs_mm_base.max_alloced, &mega, &kilo, &byte );
    SVC_FSS_TRACE(DL_VFSWARNING, FORMAT_STRING"\n", "maximum static memory allocated",
                     g_fs_mm_base.max_alloced, g_fs_mm_base.max_alloced, mega, kilo, byte );

    SVC_FSS_TRACE(DL_VFSWARNING, "current counter [%d]\n", g_fs_mm_base.cur_counter );
    SVC_FSS_TRACE(DL_VFSWARNING, "maximum counter [%d]\n", g_fs_mm_base.max_counter );

    get_mega_kilo_byte(  MEM_UNIT_SIZE * g_fs_mm_base.cur_counter, &mega, &kilo, &byte );
    SVC_FSS_TRACE(DL_VFSWARNING, FORMAT_STRING"\n", "current control info allocated",
        MEM_UNIT_SIZE * g_fs_mm_base.cur_counter, MEM_UNIT_SIZE * g_fs_mm_base.cur_counter, mega, kilo, byte );

    get_mega_kilo_byte(  MEM_UNIT_SIZE * g_fs_mm_base.max_counter, &mega, &kilo, &byte );
    SVC_FSS_TRACE(DL_VFSWARNING, FORMAT_STRING"\n", "maximum control infoallocated",
        MEM_UNIT_SIZE * g_fs_mm_base.max_counter, MEM_UNIT_SIZE * g_fs_mm_base.max_counter, mega, kilo, byte );

    // TODO: up( &g_fs_mm_sem );
    return NULL;
}


void private_free( void *ptr )
{
    struct mem_free_blk *bp = ( struct mem_free_blk *)ptr - 1;
    struct mem_free_blk *p, *prev;
    int     size = ( bp->nunits - 1 ) * MEM_UNIT_SIZE;

    // TODO: down( &g_fs_mm_sem );
    if (g_fs_mm_free_list == g_fs_mm_free_list->next)
    {
        SVC_FSS_TRACE(DL_VFSWARNING, "free list is empty\n");
        g_fs_mm_free_list->next = bp;
        bp->next = g_fs_mm_free_list;
    }
    else
    {
        for ( prev = g_fs_mm_free_list, p = prev->next; p != g_fs_mm_free_list && p + p->nunits <= bp; prev = p, p =  p->next )
        {
            if(p == bp)
            {
                SVC_FSS_TRACE(DL_VFSWARNING, "===========ASSERT===========\n" );
                SVC_FSS_TRACE(DL_VFSWARNING, "ASSERT: [%s][%d]\n", __FILE__, __LINE__ );
                SVC_FSS_TRACE(DL_VFSWARNING, "===========ASSERT===========\n" );
                
                return;
            }
            //FS_ASSERT_RWV( p != bp );
        }

        if ( p != g_fs_mm_free_list )
        {
            if ( bp + bp->nunits == p )
            {
                bp->nunits += p->nunits;
                bp->next = p->next;
            }
            else
            {
                bp->next = p;
            }
            
            if ( bp == prev + prev->nunits )
            {
                prev->nunits += bp->nunits;
                prev->next = bp->next;
            }
            else 
            {
                prev->next = bp;
            }
        }
        else
        {
            if ( bp == prev + prev->nunits )
            {
                prev->nunits += bp->nunits;
            }
            else
            {
                bp->next = prev->next;
                prev->next = bp;
            }
        }
    }

    g_fs_mm_base.cur_counter--;
    g_fs_mm_base.cur_alloced -= size;
    
    // TODO: up( &g_fs_mm_sem );

    if(!(g_fs_mm_base.cur_alloced < g_fs_mm_base.max_alloced))
    {
        SVC_FSS_TRACE(DL_VFSWARNING, "===========ASSERT===========\n");
        SVC_FSS_TRACE(DL_VFSWARNING, "ASSERT: [%s][%d]\n", __FILE__, __LINE__);
        SVC_FSS_TRACE(DL_VFSWARNING, "===========ASSERT===========\n");
        
        return;
    }
    
    // FS_ASSERT_RWV( g_fs_mm_base.cur_alloced < g_fs_mm_base.max_alloced );
}

#endif // #ifdef  FS_PRIVATE_MEM_MANAGE_WITHOUT_GROUP


#ifdef _DSM_DEBUG_
#define NR_FS_DENTRY_BUF 10

FS_DENTRY_BUF g_Dentry_Table[NR_FS_DENTRY_BUF] = { {{0, {0,},0, 0}, 0}, };

struct dentry* dentry_alloc()
{
    INT32 i;
    FS_DENTRY_BUF *cur;
    
    for(i=0; i < NR_FS_DENTRY_BUF; i++)
    {
        cur = g_Dentry_Table + i;

        if ( 0 == cur->refcnt )
        {
            cur->refcnt = 1;
            memset( &(cur->data), 0x00, sizeof( cur->data ) );
            return (struct dentry*)cur;
        }        
    }

    return (struct dentry*)NULL;
}

VOID dentry_free(struct dentry* pDe)
{
    int i;
    FS_DENTRY_BUF *cur;

    for ( i = 0; i < NR_FS_DENTRY_BUF; i++ )
    {
        cur = g_Dentry_Table + i;
        if ( 1 == cur->refcnt && pDe == (struct dentry*)cur )
        {
            cur->refcnt = 0;
            return ;
        }
    }    
}
#endif // #if defined(_DSM_DEBUG_)


