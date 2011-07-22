#include "fat_base.h"
#include "fat_local.h"
#include "fs.h"
#include "fs_base.h"

#include <string.h>

#include "fss_debug.h"

#include "sxr_ops.h"

extern struct task_struct g_fs_current;
extern struct file* g_fs_file_list;
/*
首字符不能是.
Windows 中的文件名不能含有下列字符：\ / : * ? “ < > |
  (1) \/用于分割component
  (2):用于盘符；包含":"的component name只能是形如"C:", ..., "Z:"或
  "c:", ..., "z:"，并且只能是pathname中的首个component。
  (3)*用于通配符
  (4)?用于匹配任意一个字符
  (5)<>|用于重定向和管道
  (6)"用于shell，例如"My document"
*/
BOOL is_file_name_valid( PCWSTR pathname, BOOL bIgnoreBaseName )
{
    WCHAR *p = pathname;
    WCHAR *q;
    WCHAR *component = NULL;
    int   comp_len;
    int   index = 0;

    if( NULL == pathname ) // pathname is emtpy
    {
        return FALSE;
    }
    component = NAME_BUF_ALLOC();
    if (!component)
    {
        return FALSE;
    }
    
    while ( *p )
    {
        while ( *p && IS_COMPONENT_SEPATATOR( *p ) )
            p++;

        if ( !*p )
        {
            NAME_BUF_FREE(component);
            return TRUE;
        }
                
        index++;

        /* p points to a NON-COMPONENT_SEPATATOR-CHAR now */
        q = p;

        while ( *q && !IS_COMPONENT_SEPATATOR( *q )  )
            q++;

        // 如果名字里面包含有字母则可能导致转成unicode后超出510字节。
        // 所以有必要在转OEM之前调is_unicode_name_too_long()直接对UNICODE进行长度检查。
        if ( ( comp_len = q -p ) > FS_FILE_NAME_LEN ) 
        {
            NAME_BUF_FREE(component);
            SVC_FSS_TRACE(DL_VFSERROR, "in is_file_name_valid, pathname error! comp_len = %d\n",comp_len);
            return FALSE;
        }

        if (0 == *q && bIgnoreBaseName)
        {
            NAME_BUF_FREE(component);
            return TRUE;
        }

         strncpy(component, p, comp_len );
         component[ comp_len ] = 0;

         if ( DSM_TcStrChr( component, _T(':') ) )
         {
            // invalid driver name
            if ( 1 != index ||2 != DSM_TcStrLen( component ) ||!IS_VALID_DRIVER_NAME(component[ 0 ])  )
            {
                NAME_BUF_FREE(component);
                SVC_FSS_TRACE(DL_VFSERROR, "in is_file_name_valid, pathname error! index= %d,component[ 0 ] = %c\n",index,component[ 0 ]);
                return FALSE;
            }
            // Driver specified not found
            else if ( _T('c') != component[ 0 ] && _T('C') != component[ 0 ] ) 
            {
                NAME_BUF_FREE(component);
                SVC_FSS_TRACE(DL_VFSERROR, "in is_file_name_valid, pathname error! icomponent[ 0 ] = %c\n",component[ 0 ]);
                return FALSE;
            }
         }
         // characters * ? > < | \" are found in component name
         else if ( DSM_TcStrChr( component, _T('*') ) || DSM_TcStrChr( component, _T('?') ) 
                   || DSM_TcStrChr( component, _T('>') ) || DSM_TcStrChr( component, _T('<') )
                   || DSM_TcStrChr( component, _T('|') ) || DSM_TcStrChr( component, _T('"') ) )
         {
             NAME_BUF_FREE(component);
             SVC_FSS_TRACE(DL_VFSERROR, "in is_file_name_valid, pathname error!characters * ? > < | \"are found in component name");   
             return FALSE;
         }

         p = q;
        }
    
        NAME_BUF_FREE(component);
        return TRUE;
        
}


/*
Assume that 
    1. pathname contains one character at least.
    2. pathname specifies a DIRECTORY file, rather than REGULAR file.
    
如果pathname以以"/"结尾，则自动追加一个"."字符。
*/
WCHAR* format_dir_name( PCWSTR pathname )
{
    WCHAR *path = NULL;
    int len;

    len = DSM_TcStrLen( pathname );
    if( len > FS_PATH_LEN )
    {
        return NULL;
    }
    
    //path = (UINT8 *)sxr_Malloc(len);
    //path = g_NameBuf2;
    path = NAME_BUF_ALLOC();
    if(NULL == path)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "in format_dir_name,path is NULL");   
        return NULL;
    }

    len--;
    if ( _T('/') == pathname[ len ] || _T('\\') == pathname[ len ])
    {
        DSM_TcStrCpy( path, pathname );
        // 由于NAME_BUF为结束符保留了两个字节，
        // 所以这里追加一个字节不会溢出。
        DSM_TcStrCat( path, (PCWSTR)_T(".") );
        return path;
    }
    else
    {
        DSM_TcStrCpy( path, pathname );
        return path;
    }        
}


// 带通配符的串的匹配，不区分字母的大小写。
int pattern_match( PCWSTR name, PCWSTR pattern )
{

#define IS_LETTER(c)        ((c >= _T('A') && c <= _T('Z')) || (c >= _T('a') && c <= _T('z')))
#define IS_MATCH_LETTER_I(c1, c2)   (IS_LETTER(c1) && IS_LETTER(c2)&& (c1 - c2 == 32 || c2 - c1 == 32 || c1 -c2 == 0))

typedef enum
{ 
    START, MATCH_CHAR, MATCH_STAR, SEARCH_STRING, MATCH_STRING, DONE, ERROR
}STATE_T;

    WCHAR *p = name, *q = pattern;
    UINT16 i = 0;
    STATE_T state = START;

    // 检查目标串，包含有'*'或者'?'则出错。
    if (DSM_TcStrChr(name, _T('*')) || DSM_TcStrChr(name, _T('?')))
    {
        return PATTERN_CMP_UNMATCH;
    }
    
    while (state != DONE && state != ERROR)
    {
        switch (state)
        {
            case START: // 匹配开始。
                if (NULL == p || NULL == q)
                {
                    state = ERROR;
                }
                else
                {
                    state = MATCH_CHAR;
                }
                break;

            // 还没遇到'*'之前，按一个个字符进行匹配。
            case MATCH_CHAR: 
                if ((*p == *q || IS_MATCH_LETTER_I(*p, *q)) || (_T('?') == *q && *p != _T('\0')))
                {
                    if (_T('\0') == *q && _T('\0') == *p)
                    {
                        state = DONE;
                    }
                    else
                    {
                        p++, q++;
                    }
                }
                else if (_T('*') == *q)
                {
                    state = MATCH_STAR;
                }
                else
                {
                    state = ERROR;
                }
                break;

            // 遇到'*'后，提前查找'*'后面的字符串进行匹配。
            // 这个字符串遇到'\0'或者又一个'*'时结束，'?'被当成字符
            // 串里面的一个字符。
            case MATCH_STAR: 
                if (_T('\0') == *(q + 1))
                {
                    state = DONE;
                }
                else if (_T('*') == *(q + 1))
                {
                    ++q;
                }
                else
                {// 开始提前搜索这个'*'后面的字符串进行匹配。
                    i = 1;
                    state = SEARCH_STRING;
                }
                break;

            // 搜索相匹配的字符串。
            case SEARCH_STRING: 
                if (_T('\0') == *p)
                {
                    state = ERROR;
                }
                else if (*p != *(q + i) && !IS_MATCH_LETTER_I(*p, *(q + i)) && *(q + i) != _T('?'))
                {
                    ++p;
                }
                else // *(p + j) == *(q + i) || *(q + i) == _T('?')
                {
                    // 找到了第一个匹配的字符，
                    // 转到两个字符串的匹配状态。
                    ++i;
                    state = MATCH_STRING;
                }
                break;

            // 开始以字符为单位对两个字符串进行比较。
            case MATCH_STRING:
                if (_T('*') == *(q + i))
                {// 当前字符串匹配时，又遇到一个'*'。必须先处理这种情况。
                    q += i;
                    p += (i - 1);
                    state = MATCH_STAR;
                }
                else if (_T('\0') == *(p + i - 1) && *(q + i) != _T('\0'))
                {// 目标串提前结束了,匹配失败。
                    state = ERROR;
                }
                else if (*(p + i - 1) == *(q + i) || IS_MATCH_LETTER_I(*(p + i - 1), *(q + i)) || _T('?') == *(q + i))
                {// 字符串匹配中...
                    if (_T('\0') == *(p + i - 1) && _T('\0') == *(q + i))
                    {
                        state = DONE;
                    }
                    else
                    {
                        ++i;
                    }
                }
                else// (*(p + i - 1) != *(q + i))
                {
                   i = 1, ++p;
                   state = SEARCH_STRING;
                }
                break;

            case DONE:
                
            case ERROR:
                
            default:
                break;
        }
    }

    return (DONE == state ? PATTERN_CMP_MATCH : PATTERN_CMP_UNMATCH);
    
}


/*
 *	do_find_entry()
 *
 * finds an entry in the specified directory with the wanted name. 
 *
 * when the entry is found, we INCREASE REFERENCE COUNT of the inode
 * associated with the entry.
 * 
 * if entry's name to be found is ".", DOT_FOUND is returned; for "..", DOTDOT_FOUND is returned;
 * for other value, if found, DIR_ENTRY_FOUND is found and make *ppFound  point to the entry found, 
 * else DIR_ENTRY_NOT_FOUND is returned.
 *
 * If @dents is NULL, we search from dir_i->dents; else from @dents.
 *
 * struct ffs_dentry_data
 *  {
 *       struct ffs_full_entry *dents;          //IN parameter
 *      struct ffs_full_entry *pFound;       //OUT parameter
 *   };
 * 中的dents域指明了从i_info->dents下的哪个struct ffs_full_entry开始搜索
 * 匹配的entry; pFound域则指向i_info->dents中匹配的entry.
 * 在Find系列的API中,上次搜索的pFound作为下次搜索的dents,从而可以
 * 遍历链表i_info->dents.
 */
int do_find_entry(struct inode ** dir_i, struct dentry *de, UINT32 iFlag)
{
    struct super_block *sb;
    UINT32 ino = 0;
    INT32 lErrCode = ERR_SUCCESS;

    if( NULL != de->inode )
    {
        SVC_FSS_TRACE(DL_VFSERROR, "do_find_entry: parameter error, de->inode != NULL.\n");
        return -1;
    }
    
    if ( _T('.') == de->name[ 0 ] )
    {
        switch ( DSM_TcStrLen( de->name ) )
        {
        default:
            break;
        case 1:
            de->inode = *dir_i;
            de->inode->i_count++;
            return DOT_FOUND;
        case 2:
            if ( _T('.') != de->name[ 1 ] )
            {
                break;
            }
            else /* deal with ".." */
            {
                if ( *dir_i == g_fs_current.root )/*根文件系统的root directory*/
                {
                    de->inode = *dir_i;
                    de->inode->i_count++;
                    return DOTDOT_FOUND;
                }
                else if ( (*dir_i)->i_ino == FS_ROOT_INO )/*非根文件系统的root directory*/
                {
                    /* '..' over a mount-point results in 'dir_i' being exchanged for the mounted
                       directory-inode. NOTE! We set mounted, so that we can iput the new dir_i */
                    sb = get_super( (*dir_i)->i_dev );
                    if ( sb->s_mountpoint )
                    { // 释放掉外面传进来的工作目录，并切换到挂载点，
                       // 这里也是唯一的地方要求@dir_i须传进指向指针的指针。
                        iput( *dir_i );
                        *dir_i = sb->s_mountpoint;
                        (*dir_i)->i_count++;
                    }
                }
            }
        }
    }

    if ( FS_TYPE_FFS == (*dir_i)->i_fstype )
    {
        SVC_FSS_TRACE(DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (*dir_i)->i_fstype);
        return DIR_ENTRY_NOT_FOUND;
    }
    else if ( FS_TYPE_FAT == (*dir_i)->i_fstype )
    {
        lErrCode = do_fat_find_entry(*dir_i, de, &ino,iFlag);
        if(ERR_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE(DL_VFSBRIEF, "do_find_entry: do_fat_find_entry() return = %d.\n", lErrCode);
            return DIR_ENTRY_NOT_FOUND;
        }
    }
    else // unknown filesystem type
    {
        SVC_FSS_TRACE(DL_VFSERROR, "do_find_entry: DIR_ENTRY_NOT_FOUND, fstype = %d\n", (*dir_i)->i_fstype);
        return DIR_ENTRY_NOT_FOUND;
    }
    
    de->inode = iget( (*dir_i)->i_sb, ino );
    if( NULL == de->inode ) // iget failed
    {
        SVC_FSS_TRACE(DL_VFSERROR, "do_find_entry: iget de->inode is NULL.ino = %d\n",ino);
        return DIR_ENTRY_NOT_FOUND;
    }
    /*we have increased the refcnt for de->inode in function iget()*/

    //
    // Deal with the member 'pfsdata'  of 'de'.
    // If 'pfsdata' is unused, just do nothing
    //
    if ( de->inode )
    {
        if ( FS_TYPE_FFS == (*dir_i)->i_fstype )
        {
            // DO NOTHING.
            return DIR_ENTRY_NOT_FOUND;
        }
        else if ( FS_TYPE_FAT == (*dir_i)->i_fstype )
        {
            // DO NOTHING.
        }
        else // unknown filesystem type
        {
            return DIR_ENTRY_NOT_FOUND;
        }
        
        return DIR_ENTRY_FOUND;
    }
    return DIR_ENTRY_NOT_FOUND;
}

/*
 *	find_entry()
 *
 * finds an entry in the specified directory with the wanted name. 
 * WHEN THE ENTRY IS FOUND, WE INCREASE THE REFERENCE COUNT of the INODE 
 * ASSOCIATED WITH IT.
 * 
 * 0 is returned when found else -1 returned.
 */
int find_entry( struct inode ** dir_i, struct dentry *de )
{
    de->append_data2 = FS_FLAG_NOT_FIND_FILE;
    return  ( DIR_ENTRY_NOT_FOUND == do_find_entry( dir_i, de,FAT_LOOK_EXISTED) ) ? -1 : 0;
}



// We ASSUME that the pathname contains the basename. i.e, the pathname is 
// not terminated with COMPONENT SEPATATOR.
//
// Convert directory name( the part of the pathname that excludes the basename) 
// to the corresponding inode.
//
// NOTE: THE PATH MUST BE NOT ENDED WITH '/', ELSE WE CONSIDER 
// THAT THE LAST COMPONENT NAME IS MISSED.
//
// dir_namei() returns the inode of the directory, and the basename.
//
// when the directory name is converted to inode SUCCESSFULLY, 
// we INCREASE THE REFERENCE COUNT of the inode associated with the directory name.
struct inode * dir_namei(PCWSTR pathname, INT32* namelen, CONST WCHAR** basename)
{
    WCHAR * p = pathname;
    struct inode * working_i;
    WCHAR * thisname = NULL;
    int len;
    struct dentry *psDe = NULL;
    long lRet;
    UINT32 cheat = 1;

    if( NULL == g_fs_current.root || 0 == g_fs_current.root->i_count)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "dir_namei: No root inode, or refcount of root inode is 0");
        return (struct inode *)NULL;
    }

    if( NULL == g_fs_current.pwd || 0 == g_fs_current.pwd->i_count)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "dir_namei: No cwd inode, or refcount of cwd inode is 0");
        return (struct inode *)NULL;
    }
      
    if ( IS_COMPONENT_SEPATATOR(*p) )
    {
        working_i = g_fs_current.root;
        p++;

        while ( IS_COMPONENT_SEPATATOR(*p) )          /*skip  one or more continuous '/' */
            p++;
    }    
    else if ( *p )
    {
        working_i = g_fs_current.pwd;
    }
    else
    {
        SVC_FSS_TRACE(DL_VFSWARNING, "dir_namei: pathname is empty");
        return (struct inode *)NULL;
    }
    
    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "In dir_namei malloc dentry failed.\n");
        return (struct inode *)NULL;
    }
    working_i->i_count++;

    while ( cheat ) // cheat = 1, to avoid warning in level 4
    {
        thisname = p;
        if ( !FS_INODE_IS_DIR( working_i->i_mode ) )
        {
            SVC_FSS_TRACE(DL_VFSWARNING, "dir_namei: inode is not dir.\n");
            iput( working_i );
            DENTRY_FREE(psDe);
            return (struct inode *)NULL;
        }
        
        for ( len = 0; *p && ( !IS_COMPONENT_SEPATATOR(*p ) ); p++, len++ )
            /*NULL*/;

        if ( !*p )
        {
            break;
        }
        while (  IS_COMPONENT_SEPATATOR(*p ) )          /*skip  one or more continuous '/' */
            p++;

        /* We ASSUME that the length of @thisname is not more than FS_FILE_NAME_LEN */
        if (len > FS_FILE_NAME_LEN)
        {
            SVC_FSS_TRACE(DL_VFSERROR, "dir_namei: name-len too long, len = %d.\n", len);
            iput(working_i);
            DENTRY_FREE(psDe);
            return (struct inode *)NULL;
        }
        memset( psDe, 0, sizeof(struct dentry) );
        DSM_TcStrNCpy( psDe->name, thisname, len );
        
        lRet = find_entry( &working_i, psDe );
        if ( -1 == lRet ) // find_entry failed
        {
            SVC_FSS_TRACE(DL_VFSWARNING, "dir_namei: find_entry() failed: lRet = %d.\n", lRet);
            iput( working_i );
            DENTRY_FREE(psDe);
            return (struct inode *)NULL;
        }
        iput( working_i );
        working_i = psDe->inode;
    }

    /* NULL character is not counted */
    *namelen = p - thisname;
    *basename = thisname;
    DENTRY_FREE(psDe);
    return working_i;
}

//
//	namei()
//
// is used by most simple commands to get the inode of a specified name.
// Open, link etc use their own routines, but this is enough for things
// like 'chmod' etc.
// WHEN THE ENTRY IS FOUND, WE INCREASE THE REFCNT OF the INODE 
// ASSOCIATED WITH IT.
//
struct inode * namei( PCWSTR pathname )
{
    WCHAR *basename;
    INT32 namelen;
    struct inode *dir_i;
    struct inode *psInode = NULL;
    struct dentry *psDe = NULL;
    long lRet;

    dir_i = dir_namei( pathname, &namelen, (CONST WCHAR**)&basename );
    if( !dir_i ) // path is invalid
    {
        SVC_FSS_TRACE(DL_VFSWARNING, "namei: dir_namei() return NULL.\n");
        return (struct inode*)NULL;
    }
    
    if ( !namelen )     			/* special case: '/home/gauss/' etc */
        return dir_i;

    if (namelen > FS_FILE_NAME_LEN)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "namei: basename too long, namelen = %d.\n", namelen);
        iput(dir_i);
        return (struct inode*)NULL;
    }
    psDe = DENTRY_ALLOC();
    if (NULL == psDe)
    {
        SVC_FSS_TRACE(DL_VFSERROR, "In namei malloc dentry failed.\n");
        iput(dir_i);
        return (struct inode*)NULL;
    }
    memset( psDe, 0, sizeof( *psDe ) );
    DSM_TcStrCpy( psDe->name, basename );
    lRet = find_entry( &dir_i, psDe );
    if ( -1 == lRet )
    {
        SVC_FSS_TRACE(DL_VFSWARNING, "namei: find_entry() return lRet = %d.\n", lRet);
        DENTRY_FREE(psDe);
        iput( dir_i );
        return (struct inode*)NULL;
    }
    psInode = psDe->inode;
    DENTRY_FREE(psDe);
    iput( dir_i );
    return psInode;
}

INT32 fs_close(INT32 fd)
{
    struct file * filp;
    INT32 iRet;

    if( !FD_IS_VALID( fd ) ) // fd is invalid
    {
        return ERR_FS_BAD_FD; 
    } 
    
    filp = g_fs_current.filp[ fd ];
    if(!fs_is_file_opened(&g_fs_file_list, filp))
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if(filp->f_flags & FS_O_FIND) // handle is for finding
    {
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }
    
    if(!filp->f_count) // f_count should not be ZERO
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if ( --filp->f_count )
    {// 一个文件描述符只能对应一个fd，这里不应该被执行。
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }
    
    iput( filp->f_inode );
    iRet = fs_fd_list_del(&g_fs_file_list, filp);
    g_fs_current.filp[ fd ] = NULL;

    // 由于使用了新的提高读文件性能的算法，
    // 以下几行代码已经没用。
    //if(filp->f_prvdata && ((FAT_READ*)(filp->f_prvdata))->pFatEntryBuf)
    //{
    //    private_free(((FAT_READ*)(filp->f_prvdata))->pFatEntryBuf);
    //    memset(filp->f_prvdata, 0x00, sizeof(struct fat_read));
    //}
    
    //memset(filp, 0x00, sizeof(struct file));

    //iRet = ERR_SUCCESS;
    
label_exit:    
    return iRet;    
}

INT32 fs_findclose(INT32 fd)
{
    struct file * filp;
    INT32 iRet;

    if(!FD_IS_VALID( fd )) // hFile is invalid
    {
        return ERR_FS_BAD_FD;
    }
    filp = g_fs_current.filp[ fd ];
    if(!fs_is_file_opened(&g_fs_file_list, filp))
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }
    
    if(0 == (filp->f_flags & FS_O_FIND)) // handle is not for finding
    {
        iRet = ERR_FS_OPERATION_NOT_GRANTED;
        goto label_exit;
    }

    if(!filp->f_count)
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }
    
    if ( --filp->f_count )                /*more fd redirects to the same file{}*/
    {
        iRet = ERR_FS_BAD_FD;
        goto label_exit;
    }

    if(filp->f_pattern != NULL)
    {
        sxr_Free(filp->f_pattern);
        filp->f_pattern = NULL;
    }

    iput( filp->f_inode );
    fs_fd_list_del(&g_fs_file_list, filp);
    g_fs_current.filp[ fd ] = NULL;

    iRet = ERR_SUCCESS;

label_exit:
    return iRet;
}


