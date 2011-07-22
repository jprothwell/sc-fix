#include "fs.h"
#include "fat_local.h"
#include "fat_base.h"
#include "fs_base.h"
#include "storage.h"

#include <string.h>

#include "fss_debug.h"

#include "cs_types.h"

#include "sxr_ops.h"


extern struct task_struct g_fs_current;
extern struct super_block* g_fs_super_block_table;
extern FAT_CACHE g_FATBuffer;


INT32 fat_read_super( struct super_block *sb, INT32 iDevNo)
{
    INT32 lErrCode;
    
    sb->s_dev = iDevNo;
    sb->s_magic = FAT_MAGIC;
    sb->s_fstype = FS_TYPE_FAT;   
    
    lErrCode = fat_read_sb_info(sb);
    if( _ERR_FAT_SUCCESS != lErrCode)
    {
        SVC_FSS_TRACE(DL_FATERROR,"in fat_read_super, fat_read_sb_info failed, ErrorCode = %d\n",lErrCode);    
    }
    return lErrCode;
}


/*
供VFS使用，VFS在设置好inode{}中不依赖于FAT的消息后，再调用本函数
让FAT将自己的私有消息填好再把inode{}回传给VFS
*/
INT32 fat_read_inode ( struct inode *inode )
{
    FAT_SB_INFO* fsbi = &(inode->i_sb->u.fat_sb);
    INT32 lErrCode = _ERR_FAT_SUCCESS;
    
    //bsec是sector在cluster中的index；而bsec2是sector在volume中的index
    UINT32 bclu;
    UINT16 bsec, soff, bsec2;
    FAT_DIR_ENTRY entry;    
    UINT8* ebuf;
    UINT8  ecount = 0;
    
    //struct inode* curi;
    //struct inode **prei;
    
    if( NULL == inode)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_read_inode. inode is null \n");
        return _ERR_FAT_PARAM_ERROR;
    }

    memset(&entry, 0x00, sizeof(FAT_DIR_ENTRY));
    
    ebuf = FAT_SECT_BUF_ALLOC();
    if(NULL == ebuf)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_read_inode. ebuf is null \n");
        return _ERR_FAT_MALLOC_FAILED;        
    }

    if(FS_ROOT_INO == inode->i_ino) // Read root inode 
    {
        entry.DIR_Attr |= ATTR_DIR;
        entry.DIR_FileSize = fsbi->iRootEntCnt*FAT_DIR_ENTRY_SIZE;
        // FAT12及FAT16不存在物理的根目录，FAT32根目录使用2号簇。
        if (FAT32 == fsbi->iFATn)
        {
            entry.DIR_FstClusLO = 2;
            entry.DIR_FstClusHI = (UINT16)((fsbi->iFat32RootClus >> 16) & 0x0000FFFF);
        }
        else
        {
            entry.DIR_FstClusLO = 0;
            entry.DIR_FstClusHI = 0;
        }
        inode->u.fat_i.entry = entry;
        inode->i_mode = INODE_TYPE_DIRECTORY;
        inode->u.fat_i.BeginSec    = 0;
        inode->u.fat_i.EntryOffset = 0;
        inode->u.fat_i.EntryCount  = 0;
        inode->u.fat_i.Dirty = DATA_CLEAN;
    }
    else
    {
        bclu = FAT_INO2CLUS(inode->i_ino);
        bsec = FAT_INO2SEC(inode->i_ino);
        soff = FAT_INO2OFF(inode->i_ino);

        if(bclu > 0)
        {
            bsec2 = (UINT16)(fsbi->iDataStartSec + ((bclu-2) * fsbi->iSecPerClus)+ bsec);  
            lErrCode = fat_read_entries(fsbi, bclu, bsec, soff, ebuf, &ecount);
        }
        else
        {
            bsec2 = (UINT16)(fsbi->iRootDirStartSec + bsec); 
            lErrCode = fat_read_root_entries(fsbi, bsec, soff, ebuf, &ecount);
        }
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_read_inode. read entries error ErrorCode = %d\n",lErrCode);
            goto end;
        }
        
        Buf2FDE(&ebuf[(ecount-1)*DEFAULT_DIRENTRYSIZE], &entry);

        if(entry.DIR_Attr & ATTR_DIR)
        {
            inode->i_mode = INODE_TYPE_DIRECTORY;
        }
        else
        {
            inode->i_mode = INODE_TYPE_REGULAR;
        }
        inode->u.fat_i.BeginSec = bsec2;
        inode->u.fat_i.EntryOffset = soff;
        inode->u.fat_i.EntryCount = ecount;
        inode->u.fat_i.Dirty = DATA_CLEAN;
    
        Buf2FDE(&ebuf[(ecount-1)*DEFAULT_DIRENTRYSIZE], &entry);

        inode->u.fat_i.entry = entry;

        if (!(entry.DIR_Attr & ATTR_DIR) && (((UINT32)entry.DIR_FstClusLO | (UINT32)(entry.DIR_FstClusHI << 16)) != 0))
        {
            // 如果是常规文件并且该文件非空则将其占用的所有cluster读出并压缩
            // 放在fat_i.CluChain中，这样可以提高文件的读写速度；如果是目录则不必。
            lErrCode = fat_read_fatentries(fsbi, (UINT32)((UINT32)entry.DIR_FstClusLO | (UINT32)(entry.DIR_FstClusHI << 16)), &(inode->u.fat_i.CluChain)); 
            if(_ERR_FAT_SUCCESS != lErrCode)
            {
                SVC_FSS_TRACE(DL_FATERROR, "in fat_read_inode. fat_read_fatentries error ErrorCode =%d \n",lErrCode);
            	goto end;
            }
        }
    }

    inode->i_size = entry.DIR_FileSize;
    inode->i_ctime = DSM_UniteFileTime(entry.DIR_CrtDate,entry.DIR_CrtTime,entry.DIR_CrtTimeTenth);
    inode->i_atime = DSM_UniteFileTime(entry.DIR_LstAccDate,0,0);
    inode->i_mtime = DSM_UniteFileTime(entry.DIR_WrtDate,entry.DIR_WrtTime,0);

#if 0
    //
    // CHAIN TABLE OPERATION: 
    // Added open file(inode) to inode chain
    //
    //curi = fsbi->inode_chain;
    //prei = &(fsbi->inode_chain);

    while(curi)
    {
        prei = &(curi->u.fat_i.next);
        curi = curi->u.fat_i.next;        
    }

    *prei = inode;
#endif

end:
    if(NULL != ebuf) 
    {
        FAT_SECT_BUF_FREE(ebuf);
    }
    
    return lErrCode;
}

//
// find 'de' on the directory 'dir_i', output ino.
// mainly to get the ino
//
INT32 do_fat_find_entry( struct inode * dir_i, struct dentry *de, UINT32 *ino,UINT32 iFlag)
{
    FAT_SB_INFO *sb_info = &(dir_i->i_sb->u.fat_sb);
    FAT_DIR_ENTRY fentry;
    UINT32 bclu;
    UINT16 bsec, eoff;
    INT32 lErrCode = _ERR_FAT_SUCCESS;
    UINT16 ecount;
    INT32 i;
    
    //
    // The 'append_data' is used to save the postion information for finding function
    // and it is made of three members: 1)begining cluster, 2)begining sector(based on cluster)
    // and 3) offset on the begining sector
    // 
    if(FS_FLAG_FIND_FILE == de->append_data2)
    {
        bclu = FAT_INO2CLUS(de->append_data);
        bsec = FAT_INO2SEC(de->append_data);
        eoff = FAT_INO2OFF(de->append_data);
    }
    else
    {
        bclu = 0;
        bsec = 0;
        eoff = 0;
    }
    
    if((FS_ROOT_INO == dir_i->i_ino 
         || 0 == (UINT32)(dir_i->u.fat_i.entry.DIR_FstClusLO | (UINT32)(dir_i->u.fat_i.entry.DIR_FstClusHI<<16)))
         && sb_info->iFATn != FAT32)
    { // FAT12或FAT16的根目录。FAT12与FAT16的根目录没有起始簇须特殊处理，
       //而FAT32的根目录则有起始簇。
        lErrCode = fat_lookup_entry(&(dir_i->i_sb->u.fat_sb), \
                                    0, de->name, &fentry, &bclu, &bsec, &eoff, &ecount,iFlag);
        if(lErrCode != _ERR_FAT_SUCCESS)
        {
            goto end;
        }

        // Make up inode number for the VFS
        
        if ((fentry.DIR_Attr & ATTR_DIR) && 0 == ((UINT32)(fentry.DIR_FstClusHI << 16) | (UINT32)fentry.DIR_FstClusLO))
        {
            // 一级子目录中对应根目录的目录项".."其"DIR-FstClu"为0，
            // 所以可以根据"DIR-FstClu"为0这个特点判断一个目录项对
            // 应的是否为根目录而作特殊处理。
            *ino = (UINT32)FS_ROOT_INO;
        }
        else
        {
            // bsec = (UINT16)(bsec - sb_info->iRootDirStartSec);  20070128. 不要减去iRootDirStartSec了。
            // 因为了传出的就是相对于根目录开始扇区的偏移扇区数
            *ino = FAT_CSO2INO(bclu, bsec, eoff);
        }
        
        // The following code just for find function
        for(i=0; i<ecount; i++)
        {
            eoff += DEFAULT_DIRENTRYSIZE;
            if(eoff == sb_info->iBytesPerSec)
            {
                eoff = 0;
                bsec++;
            }
        }
        
        de->append_data = FAT_CSO2INO(bclu, bsec, eoff);
    }
    else
    {
        UINT32 startclu;
        if(0 == bclu)
        {
            startclu = dir_i->u.fat_i.entry.DIR_FstClusLO;
            startclu |= (UINT32)(dir_i->u.fat_i.entry.DIR_FstClusHI << 16);
        }
        else
        {
            startclu = bclu;
        }

        if(fat_is_last_cluster(sb_info, startclu))
        {
            lErrCode = _ERR_FAT_READ_EXCEED;
            goto end;
        }
        
        lErrCode = fat_lookup_entry(&(dir_i->i_sb->u.fat_sb), \
            startclu, de->name, &fentry, &bclu, &bsec, &eoff, &ecount, iFlag);
        if(lErrCode != _ERR_FAT_SUCCESS)
        {
            goto end;
        }

        if ((fentry.DIR_Attr & ATTR_DIR) && 0 == ((UINT32)(fentry.DIR_FstClusHI << 16) | (UINT32)fentry.DIR_FstClusLO))
        {
            // 一级子目录中对应根目录的目录项".."其"DIR-FstClu"为0，
            // 所以可以根据"DIR-FstClu"为0这个特点判断一个目录项对
            // 应的是否为根目录而作特殊处理。
            *ino = (UINT32)FS_ROOT_INO;
        }
        else
        {
            // bsec = (UINT16)(bsec - sb_info->iRootDirStartSec);  20070128. 不要减去iRootDirStartSec了。
            // 因为了传出的就是相对于根目录开始扇区的偏移扇区数
            *ino = FAT_CSO2INO(bclu, bsec, eoff);
        }

        if (FS_FLAG_FIND_FILE == de->append_data2)
        {
            // The following code just for find function
            for(i=0; i<ecount; i++)
            {
                eoff += DEFAULT_DIRENTRYSIZE;

                if(eoff == sb_info->iBytesPerSec)
                {
                    bsec++;
                    eoff = 0;
                }

                if(bsec == sb_info->iSecPerClus)
                {
                    bsec = 0;
                    lErrCode = fat_get_next_cluster(sb_info, bclu, &bclu);
                    if (lErrCode != _ERR_FAT_SUCCESS)
                    {
                        SVC_FSS_TRACE(DL_FATERROR, "do_fat_find_entry: fat_get_next_cluster() return %d", lErrCode);
                        goto end;
                    }
                }
            }
            
            de->append_data = FAT_CSO2INO(bclu, bsec, eoff);
        }
    }

end:
    if(_ERR_FAT_SUCCESS != lErrCode)
    {
        SVC_FSS_TRACE(DL_FATDETAIL, "in do_fat_find_entry.fat_lookup_entry error ErrorCode = %d\n",lErrCode);
        return lErrCode;
    }
    else
    {
        return _ERR_FAT_SUCCESS;
    }
}

INT32 fat_write_inode( struct inode * inode, BOOL bFreeFlag )
{
    FAT_CACHE *psFATCache = NULL;    
    FAT_SB_INFO* fsbi;
    //struct inode* curi;
    //struct inode **prei;
    UINT8* secbuf;
    INT32 iRet;

    iRet = _ERR_FAT_SUCCESS;
    fsbi = &(inode->i_sb->u.fat_sb);
    //curi = fsbi->inode_chain;
    //prei = &(fsbi->inode_chain);
    secbuf = NULL;
    
    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_write_inode, malloc failed\n");
        return _ERR_FAT_MALLOC_FAILED;        
    }

    //while(curi)
    {
        //if(curi->i_ino== inode->i_ino)    
        {
            //(*prei) = curi->u.fat_i.next;

            //
            // Write the entry of this inode into disk 
            // if the file size is change.
            // It changes file size or others.
            //
            if (fat_is_inode_relating_with_fatcache(inode, &psFATCache))
            {
                iRet = fat_synch_fat_cache(fsbi, psFATCache);
                if(_ERR_FAT_SUCCESS != iRet)
                {
                    SVC_FSS_TRACE(DL_FATERROR, "in fat_write_inode, fat_synch_fat_cache failed,iRet = %d\n",iRet);
                    FAT_SECT_BUF_FREE(secbuf);
                    return iRet;
                }
            }
            iRet = fat_update_dentry(inode, secbuf);
            if(_ERR_FAT_SUCCESS != iRet)
            {
                SVC_FSS_TRACE(DL_FATERROR, "in fat_write_inode, fat_update_dentry failed,iRet = %d\n",iRet);
                FAT_SECT_BUF_FREE(secbuf);
                return iRet;
            }

            if (bFreeFlag)
            {
                fat_free_fatentries(&(inode->u.fat_i.CluChain));			
                //memset( inode, 0, sizeof( struct inode );
            }
            //break;
        }

        //prei = &(curi->u.fat_i.next);
        //curi = curi->u.fat_i.next;
    }
    
    if(NULL != secbuf)
    {
        FAT_SECT_BUF_FREE(secbuf);
    }
    
    return iRet;
    
}

INT32 fat_create_file( struct inode *dir_i, struct dentry *de, int mode )
{
    struct fat_dir_entry fde;
    UINT32 bclu = 0;
    UINT16 bsec = 0, eoff = 0;
    UINT32 ino;
    struct inode *inode;
    struct inode *empty;
    INT32 lErrCode =  _ERR_FAT_SUCCESS;
    struct super_block* sb = dir_i->i_sb;
    UINT32 dev_nr = sb->s_dev;
    UINT32 fileTime;
    
    
    memset(&fde, 0x00, sizeof(struct fat_dir_entry));

    empty = get_empty_inode();
    if( NULL == empty ) // inode table is full
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_create_file,get_empty_inode error,\n");
        return _ERR_FAT_NO_MORE_INODE;
    }
    
    ino = FAT_CSO2INO(bclu, bsec, eoff);
    inode = empty;
    inode->i_count = 1;
    inode->i_dev = dev_nr;
    //inode->i_ino, set it below.
    inode->i_sb = sb;
    inode->i_fstype = sb->s_fstype;
    inode = inode_list_add(&sb->inode_chain, inode);

    
    if(INODE_TYPE_DIRECTORY == mode)
    {
        fde.DIR_Attr |= ATTR_DIR;
    }
    else
    {
        fde.DIR_Attr |= ATTR_ARCH; // 此属性用于让一些备份程序知道文件有更改过。
    }

    fileTime = DSM_GetFileTime();
    inode->i_mtime = inode->i_ctime = fileTime;
    fde.DIR_CrtDate = DSM_GetFileTime2Date(fileTime);
    fde.DIR_CrtTime = DSM_GetFileTime2Time(fileTime);
    inode->i_atime = DSM_UniteFileTime(fde.DIR_CrtDate, 0,0);
    SVC_FSS_TRACE(DL_FATDETAIL,"in fat_create_file DIR_CtrDate = %d,DIR_CrtTime = %d.\n",
        fde.DIR_CrtDate,
        fde.DIR_CrtTime
        );
    fde.DIR_CrtTimeTenth = 0;  // need handle it
    fde.DIR_FileSize = 0;
    fde.DIR_FstClusHI = 0;
    fde.DIR_FstClusLO = 0;
    fde.DIR_LstAccDate = fde.DIR_CrtDate;
    // de->DIR_Name, need set in fat_add_root_entry() or fat_add_entry() function.
    fde.DIR_NTRes = 0;
    fde.DIR_WrtDate = fde.DIR_CrtDate;
    fde.DIR_WrtTime = fde.DIR_CrtTime;

    if(FS_ROOT_INO == dir_i->i_ino && dir_i->i_sb->u.fat_sb.iFATn != FAT32)
    {
        lErrCode = fat_add_root_entry(dir_i, &fde, &bsec, &eoff, de->name, FALSE);
        // bsec = (UINT16)(bsec - sb_info->iRootDirStartSec);  20070128. 不要减去iRootDirStartSec了。
        // 因为了传出的就是相对于根目录开始扇区的偏移扇区数
        SVC_FSS_TRACE(DL_FATDETAIL, "in fat_create_file, fat_add_root_entry return: bsec = %d, eoff = %d.\n", bsec, eoff);
       
    }
    else
    {
        lErrCode = fat_add_entry(dir_i, &fde, &bclu, &bsec, &eoff, de->name, FALSE);   
        SVC_FSS_TRACE(DL_FATDETAIL, "in fat_create_file, fat_add_entry return:\nbclu = %d, bsec = %d, eoff = %d.\n", bclu, bsec, eoff);
    }

    if(_ERR_FAT_SUCCESS == lErrCode)
    {	
        ino = FAT_CSO2INO(bclu, bsec, eoff);
        inode->i_ino = ino;
        SVC_FSS_TRACE(DL_FATDETAIL, "in fat_create_file,call fat_read_inode() with: inode_no = %d.\n", ino);
        lErrCode = fat_read_inode( inode );
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            iput(inode);
            SVC_FSS_TRACE(DL_FATERROR, "in fat_create_file,fat_read_inode error.\n");
            lErrCode = lErrCode;
        }
        else
        {
            de->inode = inode;
            lErrCode = lErrCode;
        }        
    }
    else
    {
        SVC_FSS_TRACE( DL_FATDETAIL,	"in fat_create_file() after add entry,lErrCode = %d.\n", lErrCode);  
        iput(inode);		        
    }

    return lErrCode;   
}

//
// Operation steps:
//
// 1) set all of the FAT items to 0x00, except the first one.
// 2) set the fat_dir_entry.DIR_FileSize to ZERO.
// 
INT32 fat_trunc_file( struct inode *dir_i, struct dentry *de, int mode )
{
    FAT_SB_INFO* sb_info = &(dir_i->i_sb->u.fat_sb);
    FAT_DIR_ENTRY* fde = &(de->inode->u.fat_i.entry);    
    INT32 lErrCode =  _ERR_FAT_SUCCESS;
    UINT16 bsec = (UINT16)(de->inode->u.fat_i.BeginSec);
    UINT16 eoff = (UINT16)(de->inode->u.fat_i.EntryOffset);
    UINT32 bclu;
    UINT8 entbuf[DEFAULT_DIRENTRYSIZE];
    UINT8 ecount;
    UINT8 *secbuf;
    UINT32 firstCluster;
    UINT32 fileTime;
    
    mode = mode;
    firstCluster = (UINT32)((UINT32)(fde->DIR_FstClusLO) | (UINT32)(fde->DIR_FstClusHI << 16));
    
    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_trunc_file,buf malloc error\n");
        return _ERR_FAT_MALLOC_FAILED;        
    }
    
    // set the fat_dir_entry.DIR_FileSize to ZERO.
    // Update the size in inode for the opened file.
    bclu = FAT_INO2CLUS(de->inode->i_ino);
    bsec = FAT_INO2SEC(de->inode->i_ino);
    eoff = FAT_INO2OFF(de->inode->i_ino);

    fileTime = DSM_GetFileTime();
    de->inode->i_ctime = de->inode->i_mtime = fileTime;
    fde->DIR_WrtDate = DSM_GetFileTime2Date(fileTime);
    fde->DIR_WrtTime = DSM_GetFileTime2Time(fileTime);
    // 创建覆盖一个文件时,也将创建日期及时间更新?
    fde->DIR_CrtDate = fde->DIR_WrtDate;
    fde->DIR_CrtTime = fde->DIR_WrtTime;
    
    de->inode->i_atime = DSM_UniteFileTime(fde->DIR_WrtDate, 0,0);
    fde->DIR_LstAccDate = fde->DIR_WrtDate;
    fde->DIR_FstClusLO = 0; // 空文件不占用cluster. su_wenguang 2007/5/30
    fde->DIR_FstClusHI = 0;
    fde->DIR_FileSize = 0;
    de->inode->i_size = 0;
   
    if(bclu > 0)
    {
        lErrCode = fat_read_entries(sb_info, bclu, bsec, eoff, secbuf, &ecount);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_trunc_file,fat_read_entries error, ErrorCode = %d\n",lErrCode);
            goto end;
        }

        FDE2Buf(fde, entbuf);
        memcpy((UINT8*)(secbuf+(ecount-1)*DEFAULT_DIRENTRYSIZE), entbuf, DEFAULT_DIRENTRYSIZE);

        lErrCode = fat_write_entries(sb_info, bclu, bsec, eoff, secbuf, ecount);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_trunc_file,fat_write_entries error, ErrorCode = %d\n",lErrCode);
            goto end;
        }
    }
    else
    {
        lErrCode= fat_read_root_entries(sb_info, bsec, eoff, secbuf, &ecount);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_trunc_file,fat_read_root_entries error, ErrorCode = %d\n",lErrCode);
            goto end;
        }
        
        FDE2Buf(fde, entbuf);
        memcpy((UINT8*)(secbuf+(ecount-1)*DEFAULT_DIRENTRYSIZE), entbuf, DEFAULT_DIRENTRYSIZE);

        lErrCode= fat_write_root_entries(sb_info, bsec, eoff, secbuf, ecount);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_trunc_file,fat_write_root_entries error, ErrorCode = %d\n",lErrCode);
            goto end;
        }
    }

    // 如果该文件非空： set all of the FAT items to 0x00, include the first one.
    if (firstCluster != 0)
    {
        lErrCode = fat_trunc_fatentry(sb_info, firstCluster);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_trunc_file,fat_trunc_fatentry error, ErrorCode = %d\n",lErrCode);
            goto end;
        }
        fat_free_fatentries(&(de->inode->u.fat_i.CluChain));
    }
/* 空文件不占用cluster，因此删去此段代码。su_wenguang 2007/5/30
    lErrCode = fat_read_fatentries(sb_info, de->inode->u.fat_i.entry.DIR_FstClusLO, 
        &(de->inode->u.fat_i.CluChain));
    if(_ERR_FAT_SUCCESS != lErrCode)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_trunc_file,fat_read_fatentries error, ErrorCode = %d\n",lErrCode);
        goto end;
    }*/
    
end:
    if(NULL != secbuf) 
    {
        FAT_SECT_BUF_FREE(secbuf);
    }
    
    return lErrCode;
}

/*
Upon successful completion, 0 is returned, else errno is returned. 

在FS_UnlinkByHandle()中调用本函数时，如果要unlink的direntry关联的是
directory file，则应该将direntry关联的inode的nlinks置为0，以保证
随后的iput()中会释放该inode。

在FS_Rename()中调用本函数时，如果要unlink的direntry关联的是
directory file，则应该将direntry关联的inode的nlinks减1而不是置为0，
以保证随后的iput()中不会释放该inode。

BOOL bRename: call me from FS_Rename() or not.
*/
INT32 fat_do_unlink( struct inode *dir_i, struct dentry *de, BOOL bRename )
{
    INT32 lErrCode = _ERR_FAT_SUCCESS;
    UINT32 bclu;
    UINT16 bsec, soff;

    bclu = FAT_INO2CLUS(de->inode->i_ino);
    bsec = FAT_INO2SEC(de->inode->i_ino);  // 如果是根目录，得到的是根目录区中扇区的偏移扇区。
    soff = FAT_INO2OFF(de->inode->i_ino);

    if(bclu > 0)
    {
        lErrCode = fat_remove_entry(&(dir_i->i_sb->u.fat_sb), 
                    &(de->inode->u.fat_i.entry), bclu, bsec, soff, bRename);
    }
    else
    {
        lErrCode = fat_remove_entry(&(dir_i->i_sb->u.fat_sb), 
                    &(de->inode->u.fat_i.entry), 0, bsec, soff, bRename);
    }

    return lErrCode;
}


// 新文件名在目录项de中，目录项的其他信息可以从old_de中得到。
INT32 fat_do_link( struct dentry *old_de, struct inode *dir_i, struct dentry *de, UINT32 type )
{
    FAT_DIR_ENTRY old_entry;
    INT32 lErrCode = _ERR_FAT_SUCCESS;
    UINT32 bclu = 0;
    UINT16 bsec = 0, eoff = 0;
    UINT32 fileTime;

    type = type; // WARNING: maybe need to do it.
    
    if(dir_i->i_sb->s_dev != old_de->inode->i_sb->s_dev)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fat_do_link(), link difference device!\n");
        return _ERR_FAT_DIFF_DEV;
    }

    old_entry = old_de->inode->u.fat_i.entry;
    fileTime = DSM_GetFileTime();
    old_entry.DIR_CrtDate = DSM_GetFileTime2Date(fileTime);
    old_entry.DIR_CrtTime = DSM_GetFileTime2Time(fileTime);
    old_entry.DIR_LstAccDate = old_entry.DIR_CrtDate;
    old_entry.DIR_WrtDate = old_entry.DIR_CrtDate;
    old_entry.DIR_WrtTime = old_entry.DIR_CrtTime;

    if(FS_ROOT_INO == dir_i->i_ino && dir_i->i_sb->u.fat_sb.iFATn != FAT32)
    {
        lErrCode = fat_add_root_entry(dir_i, &old_entry, &bsec, &eoff, de->name, TRUE);
    }
    else
    {
        lErrCode = fat_add_entry(dir_i, &old_entry, &bclu, &bsec, &eoff, de->name, TRUE);
    }
    
    if(_ERR_FAT_SUCCESS != lErrCode)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fat_do_link(), add entry or add_root_entry error!\n");
        return lErrCode;
    }
	
    return lErrCode;
}

INT32 fat_HasFormated(INT32 iDevNo, UINT8 iType)
{
    UINT32 iSecOff = 0;
    UINT32 iFatSz = 0;
    UINT32 iFatIndex = 0;
    UINT32 i;
    struct super_block sSB;
    struct super_block* pSB = NULL;
    struct fat_sb_info* pFatSB;
    UINT8* pBuff = NULL;
    UINT32 iFatSymbol = 0;
    INT32 iResult = 0;
    INT32 iRet = _ERR_FAT_HAS_FORMATED;

    iType = iType;
    pSB = &sSB;

    memset(&sSB,0,SIZEOF(sSB));
     
    // malloc the buffer to stored the fbr content.
    pBuff = FAT_SECT_BUF_ALLOC();
    if(NULL == pBuff)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fat_HasFormated(), malloc error! \n");
        return _ERR_FAT_MALLOC_FAILED;
    }      
    
    pSB->s_dev = iDevNo;
    // Read the boot sector Region
    iResult = fat_read_sb_info(pSB);
    if(_ERR_FAT_SUCCESS == iResult)
    {		
        pFatSB = &(pSB->u.fat_sb);        
	
        if(FAT32 == pFatSB->iFATn || FAT16 == pFatSB->iFATn || FAT12 == pFatSB->iFATn)
	{			
            // Read the FAT Region
            iSecOff = pFatSB->iFATStartSec;//sFbr.BPB_RsvdSecCnt + sFbr.BPB_HiddSec;
            iFatIndex = 0;

            iFatSz = pFatSB->iFATSize;
            while((iFatIndex < FAT_FAT_COUNT) && (_ERR_FAT_HAS_FORMATED == iRet))
            { 	
                //memset(pBuff,0,DEFAULT_SECSIZE);
                for(i = 0; i < 1; i++)
                {
                    iResult = DRV_BLOCK_READ( iDevNo, iSecOff + pSB->u.fat_sb.iHiddSec, pBuff );
                    if(_ERR_FAT_SUCCESS != iResult)
                    {
                        iRet = _ERR_FAT_READ_SEC_FAILED;
                        SVC_FSS_TRACE(DL_FATERROR, "in fat_HasFormated(), DRV_BLOCK_READ! ErrorCode = %d \n",iResult);
                        break;
                    }
                    iFatSymbol = 0x0;
                    // FAT表第0项的值为0xFFF8(FAT16)标识着介质为磁盘，当在PC格式化部分SD卡时，
                    // FAT表0项的值并不一定是0xFFF8，因此我们判断其值只要不为0即可。
                    if(FAT32 == pFatSB->iFATn)
                    {                        
                        memcpy(&iFatSymbol,pBuff,4);
                        if (iFatSymbol != 0)//(0x0ffffff8 == iFatSymbol)
                        {
                            memcpy(&iFatSymbol,pBuff + 4,4);
                            if (iFatSymbol != 0)
                            {
                                continue;
                            }
                        }
                    }
                    else if(FAT16 == pFatSB->iFATn)
                    {                     
                        memcpy(&iFatSymbol,pBuff,2);
                        if (iFatSymbol != 0)//(0xfff8 == iFatSymbol)                           
                        {
                            memcpy(&iFatSymbol,pBuff + 2,2);
                            if (iFatSymbol != 0)
                            {
                                continue;
                            }
                        }
                    }
                    else if(FAT12 == pFatSB->iFATn)
                    {            			
                        memcpy(&iFatSymbol,pBuff,2);
                        if (iFatSymbol != 0)//(0xfff8 == iFatSymbol)
                        {
                            continue;
                        }                    
                    }

                    SVC_FSS_TRACE(DL_FATERROR, "in fat_HasFormated(), iFatSymbol(0x%x) error \n", iFatSymbol);
                    iRet = _ERR_FAT_NOT_FORMAT;
                    break;
                }    

                iSecOff += iFatSz;			
                iFatIndex ++;
            }
        }
        else
        {
            SVC_FSS_TRACE(DL_FATWARNING, "in fat_HasFormated The fat type not support!iFATn = %d.\n",pFatSB->iFATn);
            iRet = _ERR_FAT_NOT_SUPPORT;
        }
    } 
    else    
    {
        if(_ERR_FAT_BPB_ERROR == iResult)
        {
            iRet = _ERR_FAT_NOT_FORMAT;
        }
        else
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_HasFormated fat_read_sb_info failed!iResult = %d.\n",iResult);
            iRet = iResult;
        }
    }  
	
    if(NULL != pBuff)
    {        
        FAT_SECT_BUF_FREE(pBuff);
    }
    return iRet;
}



//********************************************************************************
// Function:
//   This function is format the logistic disk.
// Parameter:
//   size[in], the disk max size for byte.
//   vollab[in],point to the string of the voluem lable.
//   oemname[in],Point to the string of OEM name.
//   opt[in], The option of format, 0:Quickly;1:Normal.
// Return value:
//      _ERR_FAT_SUCCESS indicates a successful operation. 
//         And unsuccessful operation is denoted by the err code as following.
//      _ERR_FAT_READ_SEC_FAILED:    Read the sector failed.
//      _ERR_FAT_WRITE_SEC_FAILED:   Write the sector failed.
//      _ERR_FAT_MALLOC_FAILED       allocte memory failed.
//*******************************************************************************

INT32 fat_format(INT32 iDevNo, UINT32 iDiskTotSec,UINT8* vollab,UINT8* oemname,UINT8 opt)
{   
    UINT8 vollab_len;
    UINT8 oemname_len;
    UINT32 iDataSecCount;
    UINT32 iSecOff = 0;
    UINT32 iFatSz = 0;
    UINT32 iFatIndex = 0;
    UINT32 i;
    FAT_BOOT_RECORD sFbr;
    UINT8* pBuff;
    UINT32 iFatType;
    UINT32 iDataClus;
    UINT16 iTrailSig;
    INT32 iRemainsSize;
    UINT32 iRemainsSec;
    UINT32 iRootDirSec;
    UINT32 iOtherRsvdSec;
    INT32 iResult;    
    INT32 iRet = _ERR_FAT_SUCCESS;
    UINT32 iBadClu = 0;

    // input parameter check.
    if((0 == iDiskTotSec) || ((0 != opt) &&(1 != opt)))
    {
        return _ERR_FAT_PARAM_ERROR;
    }   
    if(NULL == vollab || NULL == oemname)
    {
        return _ERR_FAT_PARAM_ERROR;
    }
	
    // malloc the buffer to stored the fbr content.
    pBuff = FAT_SECT_BUF_ALLOC();
    if(NULL == pBuff)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_format buf malloc error\n");
        return _ERR_FAT_MALLOC_FAILED;
    }

    if ((UINT32)iDevNo == g_FATBuffer.dev)
    {// 撤消内存中对该设备的FAT表的缓存。
        g_FATBuffer.dev = 0;
        g_FATBuffer.FATSecNum = 0;
    }
    
    vollab_len = (UINT8)strlen(vollab);
    oemname_len = (UINT8)strlen(oemname);    
    iFatType = 0;
    iDataClus = 0;
    // Get FBR and fat type.
    fat_GetFormatInfo(iDiskTotSec,
                vollab,
                vollab_len,
                oemname,
                oemname_len,
                &sFbr,
                &iFatType,
                &iDataClus,
                &iRootDirSec
		);

    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the Reserved Region end,iSecOff = %d.\n", iSecOff);
    // Write the Hide Region
    memset(pBuff,0,DEFAULT_SECSIZE);
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the Hide Region begin,iSecOff = %d.\n", iSecOff);
    for(i = 0; i < sFbr.BPB_HiddSec; i++)
    {        
        iResult = DRV_BLOCK_WRITE( iDevNo, iSecOff, pBuff );        
        if(_ERR_FAT_SUCCESS != iResult)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_1 error\n");
            iRet = _ERR_FAT_WRITE_SEC_FAILED;
            goto end;
        }
        else
        {
        	iSecOff ++;
        }
    }  
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the Hide Region end,iSecOff = %d.\n", iSecOff);

    iSecOff = 1; // 写引导扇区的操作放在最后面。

    // Write other reserved Region
    iOtherRsvdSec = sFbr.BPB_RsvdSecCnt - 1;
    // memset(pBuff,0,DEFAULT_SECSIZE);
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write other reserved Region begin,iSecOff = %d.\n", iSecOff);
    for(i = 0; i < iOtherRsvdSec; i++)
    {        
        iResult = DRV_BLOCK_WRITE( iDevNo, iSecOff + sFbr.BPB_HiddSec, pBuff );            
        if(_ERR_FAT_SUCCESS != iResult)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_1 error\n");
            iRet = _ERR_FAT_WRITE_SEC_FAILED;
            goto end;
        }
        else
        {
        	iSecOff ++;
        }
    }
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write other reserved Region end,iSecOff = %d.\n", iSecOff);
    
    // Write the FAT Region
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the FAT Region begin,iSecOff = %d.\n", iSecOff);
                
    iFatSz = iFatType == FAT32? sFbr.u.sub_rec32.BPB_FATSz32 : sFbr.BPB_FATSz16;
    if(FAT32 == iFatType)
    {
        iRemainsSize = ((iFatSz * sFbr.BPB_BytesPerSec) - 8) - (iDataClus*4);
    }
    else if(FAT16 == iFatType)
    {
        iRemainsSize = ((iFatSz * sFbr.BPB_BytesPerSec) - 4) - (iDataClus*2);
    }
    else // FAT12
    {
        iRemainsSize = ((iFatSz * sFbr.BPB_BytesPerSec) - 3) - ((iDataClus*3)/2);
    }

    iRemainsSize = iRemainsSize >= 0 ? iRemainsSize : 0;
    
    iRemainsSec = iRemainsSize/sFbr.BPB_BytesPerSec;

    iRemainsSize = (iRemainsSize % sFbr.BPB_BytesPerSec);
    
    while(iFatIndex < sFbr.BPB_NumFATs)
    {
        memset(pBuff,0,DEFAULT_SECSIZE);
        if(iFatType == FAT32)
        {
            memset(pBuff, 0xff, 12);
            pBuff[0] = 0xf8;
            pBuff[3] = 0x0f;
            pBuff[11] = 0x0f;
        }
        else if(iFatType == FAT16)
        {
            memset(pBuff,0xff,4);
            pBuff[0]= 0xf8;  
        }
        else
        {
            memset(pBuff,0xff,3);
            pBuff[0]= 0xf8;                 
        }			
        if(1 == iFatSz)            
        {
            memset(pBuff + (DEFAULT_SECSIZE - iRemainsSize),0xff, (UINT16)iRemainsSize);
        }   
        
        // SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() before DRV_BLOCK_WRITE, iDevNo = %d, iSecOff = %d, pBuff = 0x%08x\n",
        //     iDevNo, iSecOff, pBuff);
        iResult = DRV_BLOCK_WRITE( iDevNo, iSecOff + sFbr.BPB_HiddSec, pBuff );
        if(_ERR_FAT_SUCCESS != iResult)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_2 error\n");
            iRet = _ERR_FAT_WRITE_SEC_FAILED;
            goto end;
        }
        else
        {
        	iSecOff++;
        }

        for(i = 1; i < (iFatSz - iRemainsSec) && _ERR_FAT_SUCCESS == iResult; i++)
        {   
            memset(pBuff,0,DEFAULT_SECSIZE);
            if(i == iFatSz - iRemainsSec - 1)
            {
                memset(pBuff + (DEFAULT_SECSIZE - iRemainsSize),0xff, (UINT16)iRemainsSize);
            }
		
            iResult = DRV_BLOCK_WRITE( iDevNo, iSecOff + sFbr.BPB_HiddSec, pBuff );            
            if(_ERR_FAT_SUCCESS != iResult)
            {
                SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_3 error\n");
                iRet = _ERR_FAT_WRITE_SEC_FAILED;
                goto end;
            }
            else
            {
            	iSecOff ++;
            }
        }
        for(i = 0; i < iRemainsSec && _ERR_FAT_SUCCESS == iResult; i++)
        {         
            memset(pBuff,0xff,DEFAULT_SECSIZE);
            
            iResult = DRV_BLOCK_WRITE( iDevNo, iSecOff + sFbr.BPB_HiddSec, pBuff );            
            if(_ERR_FAT_SUCCESS != iResult)
            {
                SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_4 error\n");
                iRet = _ERR_FAT_WRITE_SEC_FAILED;
                goto end;
            }
            else
            {
            	iSecOff ++;
            }
        }
        iFatIndex ++;
    } 
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the FAT Region end,iSecOff = %d.\n", iSecOff);

    // Write the Root Directory Region
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the Root DIR Region begin,iSecOff = %d.\n", iSecOff);
    memset(pBuff,0,DEFAULT_SECSIZE);
    
    for(i = 0; i < iRootDirSec; i++)
    {
        iResult = DRV_BLOCK_WRITE( iDevNo, iSecOff + sFbr.BPB_HiddSec, pBuff );      
        if(_ERR_FAT_SUCCESS != iResult)
        {
            SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_5 error\n");
            iRet = _ERR_FAT_WRITE_SEC_FAILED;
            goto end;    
        }
        else 
        {
        	iSecOff++;					
        }
    }
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the Root DIR Region end,iSecOff = %d.\n", iSecOff);

    // Write the Data Region
    if(FS_FORMAT_NORMAL == opt)
    {        
        memset(pBuff,0,DEFAULT_SECSIZE);
        SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the Data Region begin,iSecOff = %d.\n", iSecOff);
        // SecCount include iDataSecCount and iRootSecCount.
        if(0 < sFbr.BPB_TotSec32)
        {				
            iDataSecCount = sFbr.BPB_TotSec32 
                        - (sFbr.BPB_RsvdSecCnt + (sFbr.u.sub_rec32.BPB_FATSz32*sFbr.BPB_NumFATs))
                        - iRootDirSec; 
        }
        else
        {
            iDataSecCount = sFbr.BPB_TotSec16 
                        - (sFbr.BPB_RsvdSecCnt + (sFbr.BPB_FATSz16*sFbr.BPB_NumFATs))
                        - iRootDirSec; 
        }
        
        for(i = 0; i < iDataSecCount; i++)
        {
            SVC_FSS_TRACE(DL_FATDETAIL, "iSecOff = %d.\n", iSecOff);
            iResult = DRV_BLOCK_WRITE( iDevNo, iSecOff + sFbr.BPB_HiddSec, pBuff );
            if(_ERR_FAT_SUCCESS != iResult)
            {
                if ((i / sFbr.BPB_SecPerClus) + 2 == iBadClu)
                {
                    continue;
                }
                else
                {
                    iBadClu = (i / sFbr.BPB_SecPerClus) + 2;
                }
                SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_6 error(%d), we will set cluster(%d) bad\n", iResult, iBadClu);
                iResult = fat_format_set_fat_entry(iDevNo, iFatType, &sFbr, iBadClu, CLUSTER_BAD, pBuff);
                if (iResult != _ERR_FAT_SUCCESS)
                {
                    iRet = iResult;
                    goto end;
                }
                if (FAT32 == iFatType && iBadClu == sFbr.u.sub_rec32.BPB_RootClus)
                {
                    // 如果是32格式并且坏的是根目录所在的簇，那么根目录必须更改到别的位置。
                    sFbr.u.sub_rec32.BPB_RootClus++;
                    if (sFbr.u.sub_rec32.BPB_RootClus >= iDataClus + 2)
                    {
                        SVC_FSS_TRACE(DL_FATERROR, "in fat_format can not change BPB_RootClus\n");
                        iRet = _ERR_FAT_WRITE_SEC_FAILED;
                        goto end;
                    }
                }
                memset(pBuff, 0x00, DEFAULT_SECSIZE);
            }

            iSecOff++;
        }
        SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the Data Region end,iSecOff = %d.\n", iSecOff);
    }
    else if (iFatType == FAT32)
    { // 如果是快速格式化并且文件系统类型为FAT32，清空根目录缺省使用的2号簇。
        memset(pBuff,0,DEFAULT_SECSIZE);
        for(i = 0; i < sFbr.BPB_SecPerClus; i++)
        {
            SVC_FSS_TRACE(DL_FATDETAIL, "iSecOff = %d.\n", iSecOff);
            iResult = DRV_BLOCK_WRITE( iDevNo, iSecOff + sFbr.BPB_HiddSec, pBuff );
            //SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() after DRV_BLOCK_WRITE, iResult = %d\n", iResult);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_7 error\n");
                iRet = _ERR_FAT_WRITE_SEC_FAILED;
                goto end;    
            }
            else 
            {
            	iSecOff++;					
            }
        }
    }

    SVC_FSS_TRACE(DL_FATDETAIL,"in fat_format() fat_GetFormatInfo return,iFatType = %d,iDataClus = %d,iRootDirSec = %d.:\n",
        iFatType,
        iDataClus,
        iRootDirSec);
  
    SVC_FSS_TRACE(DL_FATDETAIL,"sFbr:BPB_HiddSec = %d,BPB_TotSec32 = %d,BPB_BytesPerSec = %d,BPB_RsvdSecCnt = %d,BPB_RootEntCnt = %d.\n",
        sFbr.BPB_HiddSec,
        sFbr.BPB_TotSec32,
        sFbr.BPB_BytesPerSec,
        sFbr.BPB_RsvdSecCnt,
        sFbr.BPB_RootEntCnt
        );
    SVC_FSS_TRACE(DL_FATDETAIL,"BPB_TotSec16 = %d,BPB_FATSz16 = %d,BPB_SecPerTrk = %d,BPB_NumHeads = %d,BPB_SecPerClus = %d,BPB_NumFATs = %d.\n",
        sFbr.BPB_TotSec16,
        sFbr.BPB_FATSz16,
        sFbr.BPB_SecPerTrk,
        sFbr.BPB_NumHeads,
        sFbr.BPB_SecPerClus,
        sFbr.BPB_NumFATs
    );
    SVC_FSS_TRACE(DL_FATDETAIL,"BPB_Media = %d.\n",
        sFbr.BPB_Media
        );

    if (FAT32 == iFatType)
    {
        iResult = fat_format_set_fat_entry(iDevNo, iFatType, &sFbr, sFbr.u.sub_rec32.BPB_RootClus, CLUSTER_NULL, pBuff);
        if (iResult != _ERR_FAT_SUCCESS)
        {
            iRet = iResult;
            goto end;
        }
    }
    
    FBR2Buf(&sFbr,pBuff);
    
    iTrailSig = FAT_FSI_TRAILSIG;
    memcpy(pBuff + (DEFAULT_SECSIZE - SIZEOF(UINT16)),&iTrailSig,SIZEOF(UINT16));
    
    // Write the Reserved Region.
    // Write the boot sector. 
    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() Write the Reserved Region begin,iSecOff = %d.\n", iSecOff);
    iResult = DRV_BLOCK_WRITE( iDevNo, 0 + sFbr.BPB_HiddSec, pBuff );
    if(_ERR_FAT_SUCCESS != iResult)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_format DRV_BLOCK_WRITE_0 error\n");
        iRet = _ERR_FAT_WRITE_SEC_FAILED;
    }

end:
    if(NULL != pBuff)
    {
        FAT_SECT_BUF_FREE(pBuff);
    }

    SVC_FSS_TRACE(DL_FATDETAIL, "In fat_format() befor return, iSecOff = %d\n", iSecOff);
    
    return iRet;
}


INT32 fat_scan_disk(INT32 iDevNo)
{
    typedef struct _search_dir
    {
        UINT32 iClus;        
        UINT16 iEOff;
        UINT8 iSec;
        UINT8 iRemain;
    }SEARCH_DIR;
    
    INT32  iRet = _ERR_FAT_SUCCESS;
    INT32 iResult;
    SEARCH_DIR pFatDir[FS_NR_SCANDISK_MAX_LAYERS + 1];   
    UINT8* pFat = NULL;

    
    UINT32 iMaxSecCount;              // max sector count to read.
    UINT8* pSecBuf = NULL;
    struct super_block sb;
    struct fat_sb_info* sb_info;
    UINT32 iClusCount;
      
    UINT32 i;       // entry offset in the sector
    INT32 d = 0;    // dir level
    UINT32 n = 0;   // fat item array  index. 
     
    sb.s_dev = iDevNo;
    iResult = fat_read_sb_info(&sb);
    if(_ERR_FAT_SUCCESS != iResult)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_scan_disk fat_read_sb_info error, ErrorCode = %d\n",iResult);
        return iResult;
    }
    sb_info = &(sb.u.fat_sb);
    pSecBuf = FAT_SECT_BUF_ALLOC();
    if(NULL == pSecBuf)
    {
        return _ERR_FAT_MALLOC_FAILED;        
    }
    iClusCount = sb_info->iDataSec/sb_info->iSecPerClus + 2;
    
    pFat = sxr_Malloc(iClusCount);
    if(NULL == pFat)
    {
        SVC_FSS_TRACE(DL_FATERROR, "in fat_scan_disk sxr_Malloc error, malloc failed\n");
        sxr_Free(pSecBuf);
        return _ERR_FAT_MALLOC_FAILED; 
    }
    memset(pFat,0,(UINT16)iClusCount);
    d  = 0;
    memset(pFatDir,0,SIZEOF(SEARCH_DIR)*FS_NR_SCANDISK_MAX_LAYERS);
 
    while(0 <= d)
    {         
        if(FS_NR_SCANDISK_MAX_LAYERS  <= d )
        {
            iRet = _ERR_FAT_NO_MORE_INODE;
            SVC_FSS_TRACE(DL_FATWARNING, "in fat_scan_disk FS_NR_SCANDISK_MAX_LAYERS  <= d \n");
            goto _SCAN_DISK_ERROR;
        }
          // look sub dir iMaxSector
        if(0 < d)
        {
            iMaxSecCount = sb_info->iSecPerClus;             
        }
        else
        {
            
            if(sb_info->iBytesPerSec == 0)
            {
                 SVC_FSS_TRACE(DL_FATERROR,
                		"in fat_scan_disk() sb_info->iBytesPerSec = %d!return.\n",
                		sb_info->iBytesPerSec); 
                
                 iRet = _ERR_FAT_BPB_ERROR;
                 goto _SCAN_DISK_ERROR;
            }
            iMaxSecCount = sb_info->iRootDirStartSec + ((sb_info->iRootEntCnt * DEFAULT_DIRENTRYSIZE)/sb_info->iBytesPerSec);          
        }
        do
        {   
        
            for(; pFatDir[d].iSec < iMaxSecCount; pFatDir[d].iSec++)
            {
                memset(pSecBuf, 0x00, DEFAULT_SECSIZE);
                if(0 < d)
                {
                    iResult = fat_read_cluster(sb_info,
                                               pFatDir[d].iClus,
                                               (UINT8)(pFatDir[d].iSec), 
                                               pSecBuf);
                }
                else
                {                    
                    iResult = DRV_BLOCK_READ( sb_info->sb->s_dev,
                                              sb_info->iRootDirStartSec + pFatDir[d].iSec + sb_info->iHiddSec,
                                              pSecBuf);
                }
                if(_ERR_FAT_SUCCESS != iResult)
                {  
                    SVC_FSS_TRACE(DL_FATERROR, "in fat_scan_disk DRV_BLOCK_READ_0 error, iResult = %d\n",iResult);
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto _SCAN_DISK_ERROR;
                }
              
          
                for(pFatDir[d].iEOff = pFatDir[d].iEOff; pFatDir[d].iEOff < DEFAULT_SECSIZE; pFatDir[d].iEOff += DEFAULT_DIRENTRYSIZE)
                {                 
                    i = pFatDir[d].iEOff;
                    if(FAT_IS_FREE_ENTRY(pSecBuf+i))
                    {          
                        pFatDir[d].iEOff = 0;
                        goto _ENTRY_IS_FREE;
                    }
                    else if(FAT_IS_DELETE_ENTRY(pSecBuf+i))        
                    { 
                        continue;
                    }   
                    else if(FAT_IS_SHORT_ENTRY(pSecBuf+i))
                    {
                        if('.' == pSecBuf[i])
                        {
                            continue;
                        }
                        if(FAT_IS_JAP_DELETE_ENTRY(pSecBuf + i))
                        {                        
                            continue;
                        }
                        n = FAT_GET_DIR_FSTCLUSLO(pSecBuf + i);
                        if(n < iClusCount && n > 1)
                        {
                            pFat[n]  = 1;                       
                        }
                        while((!fat_is_last_cluster(sb_info, n)) && (0 < n))
                        {
                            if(n < iClusCount)
                            {
                                pFat[n]  = 1;    
                            }
                            else
                            {
                                goto _SCAN_DISK_ERROR;
                                // break;
                            }
                            iRet = fat_get_next_cluster(sb_info, n, &n);
                            if (iRet != _ERR_FAT_SUCCESS)
                            {
                                SVC_FSS_TRACE(DL_FATERROR, "fat_scan_disk: fat_get_next_cluster() return %d", iRet);
                                goto _SCAN_DISK_ERROR;
                            }
                        }
                        
                        if(FAT_IS_DIR_ENTRY(pSecBuf + i))
                        {
                            pFatDir[d].iEOff += DEFAULT_DIRENTRYSIZE;
                            n = FAT_GET_DIR_FSTCLUSLO(pSecBuf + i);
                            //pFat[n]  = 1;
                            goto _ENTRY_IS_DIR;
                        } 
                    }                    
                    
                }
                pFatDir[d].iEOff = 0;
            }  
            if(0 < pFatDir[d].iClus)
            {
                iRet = fat_get_next_cluster(sb_info, pFatDir[d].iClus, &(pFatDir[d].iClus));
                if (iRet != _ERR_FAT_SUCCESS)
                {
                    SVC_FSS_TRACE(DL_FATERROR, "fat_scan_disk: fat_get_next_cluster2() return %d", iRet);
                    goto _SCAN_DISK_ERROR;
                }
                pFatDir[d].iSec = 0;
            }
                        
        }while((!fat_is_last_cluster(sb_info, pFatDir[d].iClus)) && (0 < pFatDir[d].iClus));
_ENTRY_IS_FREE:
        d--;
        continue;
_ENTRY_IS_DIR:
        d++;        
        pFatDir[d].iClus = n;
        
        continue;
    }       
    
    if(NULL != pSecBuf)
    {
        FAT_SECT_BUF_FREE(pSecBuf);
        pSecBuf = NULL;
    }
    
    iResult = fat_set_fat_entry_all(sb_info,pFat,iClusCount);
    iRet = iResult;            
    if(NULL != pFat)
    {
        sxr_Free(pFat);
        pFat = NULL;
    }
    return iRet;

_SCAN_DISK_ERROR:
    if(NULL != pSecBuf)
    {
        FAT_SECT_BUF_FREE(pSecBuf);
        pSecBuf = NULL;
    }
    if(NULL != pFat)
    {
        sxr_Free(pFat);
        pFat = NULL;
    }
    return iRet;
    
}


INT32 fat_set_fat_entry_all(FAT_SB_INFO *sb_info, UINT8* pFat,UINT32 iClusCount)
{
    UINT32 FATOffset;
    UINT32 ThisFATSecNum = 0;
    UINT32 ThisFATEntOffset = 0, OldThisFATSecNum = 0;
    UINT16 iFatValue16;
    UINT32 iFatValue32;
  //  UINT16 iFatValue1 = 0xffff;
    UINT8*  secbuf = NULL;
    UINT32 iCurClu;
    INT32 iResult = _ERR_FAT_SUCCESS;
    INT32 iRet = _ERR_FAT_SUCCESS;      
    UINT8 cheat = 1;
    BOOL bWrite = FALSE;

    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        return _ERR_FAT_MALLOC_FAILED;        
    }

    iCurClu = 2;
        
    if(sb_info->iFATn == FAT12)
    {
        do 
        {
            cheat = 0;
            for(; iCurClu < iClusCount ; iCurClu++)
            {
                if(0 == pFat[iCurClu])
                {
                    cheat = 1;
                    break;
                }                
            }
            if(iCurClu >= iClusCount)
            {
                cheat = 0;
            }
            if(0 == cheat)
            {
                if(TRUE == bWrite)
                {   
                    iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, OldThisFATSecNum + sb_info->iHiddSec, secbuf );
                    if(_ERR_FAT_SUCCESS != iResult)
                    {
                        SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_WRITE_0 error, iResult = %d\n",iResult);
                        iRet = _ERR_FAT_WRITE_SEC_FAILED;
                        goto end;
                    }
                    
                    iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, OldThisFATSecNum + 1 + sb_info->iHiddSec, 
                                                                        secbuf+DEFAULT_SECSIZE );
                    if(_ERR_FAT_SUCCESS != iResult)
                    {
                         SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_WRITE_1 error, iResult = %d\n",iResult);
                        iRet = _ERR_FAT_WRITE_SEC_FAILED;
                        goto end;
                    }                   
                }
                iRet = _ERR_FAT_SUCCESS;
                goto end;
            }
            FATOffset = iCurClu + (iCurClu / 2);// 根据cluster number算出它在FAT中的字节偏移量
            ThisFATSecNum = sb_info->iRsvdSecCnt + (FATOffset / sb_info->iBytesPerSec);
            ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;
   
            if(ThisFATSecNum >= sb_info->iRsvdSecCnt + sb_info->iFATSize) 
            {
                 SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all ThisFATSecNum error, ThisFATSecNum =%d, sb_info->iRsvdSecCnt  = %d,sb_info->iFATSize = %d\n",ThisFATSecNum,sb_info->iRsvdSecCnt ,sb_info->iFATSize);
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto end;
            }
            
            if(OldThisFATSecNum < ThisFATSecNum) 
            {
                if(0 < OldThisFATSecNum)
                {
                    if(TRUE == bWrite)
                    {                        
                        iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, OldThisFATSecNum + sb_info->iHiddSec, secbuf );
                        if(_ERR_FAT_SUCCESS != iResult)
                        {
                            SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_WRITE_2 error, iResult = %d\n",iResult);
                            iRet = _ERR_FAT_WRITE_SEC_FAILED;
                            goto end;
                        }
                        
                        iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, OldThisFATSecNum + 1 + sb_info->iHiddSec, 
                                                                                secbuf+DEFAULT_SECSIZE );
                        if(_ERR_FAT_SUCCESS != iResult)
                        {
                            SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_WRITE_3 error, iResult = %d\n",iResult);
                            iRet = _ERR_FAT_WRITE_SEC_FAILED;
                            goto end;
                        }
                    }
                }
                memset(secbuf, 0x00, DEFAULT_SECSIZE*2);                                
                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum + sb_info->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_READ_0 error, iResult = %d\n",iResult);
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }
                
                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum+1 + sb_info->iHiddSec, 
                                                                    secbuf+DEFAULT_SECSIZE );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_READ_1 error, iResult = %d\n",iResult);
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }
                bWrite = FALSE;
                OldThisFATSecNum = ThisFATSecNum;
            }
            iFatValue16 = MAKEFATUINT16(secbuf[ThisFATEntOffset], secbuf[ThisFATEntOffset+1]);
       //     iFatValue1 = 0xffff;
            if(iCurClu & 0x0001) // cluster number is ODD
            {   
                iFatValue16 = (UINT16)((iFatValue16>>4) & 0x0fff);
                if(0 != iFatValue16 && 0x0FF7 != iFatValue16)
                {
                    secbuf[ThisFATEntOffset] = (UINT8)(0x0F & secbuf[ThisFATEntOffset]);
                    secbuf[ThisFATEntOffset + 1] = (UINT8)(0x00 & secbuf[ThisFATEntOffset + 1]);
                    bWrite = TRUE;
                }
            }
            else // cluster number is EVEN
            {                
                iFatValue16 = (UINT16)(iFatValue16 & 0x0fff);;
                if(0 != iFatValue16 && 0x0FF7 != iFatValue16)
                {
                    secbuf[ThisFATEntOffset] = (UINT8)(0x00 & secbuf[ThisFATEntOffset]);
                    secbuf[ThisFATEntOffset + 1] = (UINT8)(0xF0 & secbuf[ThisFATEntOffset + 1]);
                    bWrite = TRUE;
                }
            }           

            iCurClu++; // 连续扫描后面所有的cluster
        }while(cheat);
    }
    else if(sb_info->iFATn == FAT16)
    {
        do 
        {
            cheat = 0;
            for(; iCurClu < iClusCount ; iCurClu ++)
            {
                if(0 == pFat[iCurClu])
                {
                    cheat = 1;
                    break;
                }                
            }
            if(iCurClu >= iClusCount)
            {
                cheat = 0;
            }
            if(0 == cheat)
            {
                if(TRUE == bWrite)
                {   
                    iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, OldThisFATSecNum + sb_info->iHiddSec, secbuf );
                    if(_ERR_FAT_SUCCESS != iResult)
                    {
                        SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_WRITE_0 error, iResult = %d\n",iResult);
                        iRet = _ERR_FAT_WRITE_SEC_FAILED;
                        goto end;
                    }
                }
                iRet = _ERR_FAT_SUCCESS;
                goto end;
            }
            FATOffset = iCurClu*2;// 根据cluster number算出它在FAT中的字节偏移量
            ThisFATSecNum = sb_info->iRsvdSecCnt + (FATOffset / sb_info->iBytesPerSec);
            ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;
   
            if(ThisFATSecNum >= sb_info->iRsvdSecCnt + sb_info->iFATSize) 
            {
                 SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all ThisFATSecNum error, ThisFATSecNum =%d, sb_info->iRsvdSecCnt  = %d,sb_info->iFATSize = %d\n",ThisFATSecNum,sb_info->iRsvdSecCnt ,sb_info->iFATSize);
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto end;
            }
            
            if(OldThisFATSecNum < ThisFATSecNum) 
            {
                if(0 < OldThisFATSecNum)
                {
                    if(TRUE == bWrite)
                    {                        
                        iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, OldThisFATSecNum + sb_info->iHiddSec, secbuf );
                        if(_ERR_FAT_SUCCESS != iResult)
                        {
                            SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_WRITE_2 error, iResult = %d\n",iResult);
                            iRet = _ERR_FAT_WRITE_SEC_FAILED;
                            goto end;
                        }   
                    }
                }
                memset(secbuf, 0x00, DEFAULT_SECSIZE);        
                
                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum + sb_info->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_READ_0 error, iResult = %d\n",iResult);
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }  
                
                bWrite = FALSE;
                OldThisFATSecNum = ThisFATSecNum;
            }
            iFatValue16 = MAKEFATUINT16(secbuf[ThisFATEntOffset], secbuf[ThisFATEntOffset+1]);
            if(0 != iFatValue16 && 0xFFF7 != iFatValue16)
            {
                secbuf[ThisFATEntOffset]     = (UINT8)0x0;
                secbuf[ThisFATEntOffset + 1] = (UINT8)0x0;               
                bWrite = TRUE;
            }
            iCurClu++; // 连续扫描后面所有的cluster
        }while(cheat);
    }
    else // FAT32
    {
        do 
        {
            cheat = 0;
            for(; iCurClu < iClusCount ; iCurClu ++)
            {
                if(0 == pFat[iCurClu])
                {
                    cheat = 1;
                    break;
                }                
            }
            
            if(iCurClu >= iClusCount)
            {
                cheat = 0;
            }
            
            if(0 == cheat)
            {
                if(TRUE == bWrite)
                {   
                    iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, OldThisFATSecNum + sb_info->iHiddSec, secbuf );
                    if(_ERR_FAT_SUCCESS != iResult)
                    {
                        SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_WRITE_0 error, iResult = %d\n",iResult);
                        iRet = _ERR_FAT_WRITE_SEC_FAILED;
                        goto end;
                    }                    
                    
                    
                }
                iRet = _ERR_FAT_SUCCESS;
                goto end;
            }
            FATOffset = iCurClu*4;// 根据cluster number算出它在FAT中的字节偏移量
            ThisFATSecNum = sb_info->iRsvdSecCnt + (FATOffset / sb_info->iBytesPerSec);
            ThisFATEntOffset = FATOffset % sb_info->iBytesPerSec;
   
            if(ThisFATSecNum >= sb_info->iRsvdSecCnt + sb_info->iFATSize) 
            {
                 SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all ThisFATSecNum error, ThisFATSecNum =%d, sb_info->iRsvdSecCnt  = %d,sb_info->iFATSize = %d\n",ThisFATSecNum,sb_info->iRsvdSecCnt ,sb_info->iFATSize);
                iRet = _ERR_FAT_READ_SEC_FAILED;
                goto end;
            }
            
            if(OldThisFATSecNum < ThisFATSecNum) 
            {
                if(0 < OldThisFATSecNum)
                {
                    if(TRUE == bWrite)
                    {                        
                        iResult = DRV_BLOCK_WRITE( sb_info->sb->s_dev, OldThisFATSecNum + sb_info->iHiddSec, secbuf );
                        if(_ERR_FAT_SUCCESS != iResult)
                        {
                            SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_WRITE_2 error, iResult = %d\n",iResult);
                            iRet = _ERR_FAT_WRITE_SEC_FAILED;
                            goto end;
                        }   
                    }
                }
                memset(secbuf, 0x00, DEFAULT_SECSIZE);        
                
                iResult = DRV_BLOCK_READ( sb_info->sb->s_dev, ThisFATSecNum + sb_info->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    SVC_FSS_TRACE(DL_FATERROR, "in fat_set_fat_entry_all DRV_BLOCK_READ_0 error, iResult = %d\n",iResult);
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }  
                
                bWrite = FALSE;
                OldThisFATSecNum = ThisFATSecNum;
            }
            iFatValue32 = MAKEFATUINT32(secbuf[ThisFATEntOffset], secbuf[ThisFATEntOffset+1],secbuf[ThisFATEntOffset+2],secbuf[ThisFATEntOffset+3]);
            if(0 != iFatValue32 && 0x0FFFFFF7 != iFatValue32)
            {
                secbuf[ThisFATEntOffset]     = (UINT8)0x0;
                secbuf[ThisFATEntOffset + 1] = (UINT8)0x0;
                secbuf[ThisFATEntOffset + 2] = (UINT8)0x0;
                secbuf[ThisFATEntOffset + 3] = (UINT8)0x0;
                bWrite = TRUE;
            }
            iCurClu++; // 连续扫描后面所有的cluster
        }while(cheat);
    }
    
end:    
    if(NULL != secbuf)
    {
        FAT_SECT_BUF_FREE(secbuf);
    }

    return iRet;
}


INT32 fat_get_file_name(struct inode * inode, WCHAR *fileName)
{
    UINT32 bclu;
    UINT16 bsec, eoff;
    INT32 iRet;

    if (!inode || !fileName)
    {
        return _ERR_FAT_PARAM_ERROR;
    }
    
    bclu = FAT_INO2CLUS(inode->i_ino);
    bsec = FAT_INO2SEC(inode->i_ino);
    eoff = FAT_INO2OFF(inode->i_ino);
    
    iRet = fat_get_name_for_entrys(&(inode->i_sb->u.fat_sb), fileName, bclu,  bsec, eoff);
    return iRet;
}


