#include "fat_local.h"
#include "fat_base.h"
#include "fs.h"
#include "fs_base.h"
#include "storage.h"

#include <string.h>

#include "sxr_ops.h"

#include "fss_debug.h"

INT32 fat_trunc_fatentry(FAT_SB_INFO *sb_info, UINT32 lFstCluLO)
{
    UINT32 curclu, nextClu;
    BOOL bSynchFAT = FALSE;
    INT32 lErrCode = _ERR_FAT_SUCCESS;
     
    if (lFstCluLO > ((sb_info->iDataSec / sb_info->iSecPerClus) + 2) || lFstCluLO < 2)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fat_trunc_fatentry: lFstCluLO = 0x%x, _ERR_FAT_PARAM_ERROR!\n", lFstCluLO);
        return _ERR_FAT_ERROR;
    }
    curclu = lFstCluLO;
    do
    {
        lErrCode = fat_get_next_cluster(sb_info, curclu, &nextClu);
        if (lErrCode != _ERR_FAT_SUCCESS)
        {
            SVC_FSS_TRACE(DL_FATERROR, "fat_trunc_fatentry: fat_get_next_cluster() return %d", lErrCode);
            return lErrCode;
        }
        if (fat_is_last_cluster(sb_info, nextClu))
        {
            // 到了最后一个簇时才强制将FAT表的缓存写回磁盘，
            // 不必每改一个FAT表项就写回磁盘，这样可以提高效率。
            bSynchFAT = TRUE;
        }
        lErrCode = fat_set_fat_entry_value(sb_info, curclu, CLUSTER_FREE, bSynchFAT, NULL);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE( DL_FATERROR,"in fat_trunc_fatentry,fat_set_fat_entry_value error_0, ErrorCode = %d \n",lErrCode); 
            return lErrCode;
        }
        if (sb_info->iFreeCnterInitFlag)
        {
            sb_info->iFreeCluCnt++;
        }
        curclu = nextClu;
    }while (!(fat_is_last_cluster(sb_info, curclu)));

    return _ERR_FAT_SUCCESS;
}


/********************************************************************************/
// Function:
//   Remove the dir entry on the pointed position.
// Parameter:
//      sb_info[in]: super block, just for getting essential information.
//      de [in], the direcotry entry to be remove.
//      bsec[in], the sector on which the 'de' stays.
//      eoff[in], the offsec in the 'bsec' where the 'de' stays.
// Return value:
//      _ERR_FAT_SUCCESS indicates a successful operation. 
//     
//      And unsuccessful operation is denoted by the err code as following:
//
//      _ERR_FAT_WRITE_SEC_FAILED:  Write the sector failed. 
//      _ERR_FAT_READ_SEC_FAILED:   Read the sector failed.
//      _FAT_DIR_NOT_NULL:          The operated dir is not null.
//*******************************************************************************
//
// Operation steps:
//
// 1) Test the type of the entry, regular file or directory
// 2) Remove the content which the 'de' point to according the test result
//      a) regular file, remove it anyway;
//      b) directory, remove it if the directory is NULL(has only '.' and '..' 
//          directory), or do not remove it.
// 3) Remove the 'de' itselft
//
//
// bsec和eoff参数指向第二个参数de，即要remove的entry所在的位置
// (扇区号和扇区号指定扇区上的偏移位置)
//
INT32 fat_remove_entry(FAT_SB_INFO *sb_info, struct fat_dir_entry *de, 
                UINT32 bclu, UINT16 bsec, UINT16 eoff, BOOL bRename)
{
    INT32 lErrCode = _ERR_FAT_SUCCESS;
    UINT8* secbuf;
    INT32 i;
    
    FAT_DIR_ENTRY dent;
    UINT32 cluster;
    UINT8  cursec;
    UINT8  ecount;
    
    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        SVC_FSS_TRACE( DL_FATERROR,"in fat_remove_fatentry, NULL == secbuf  \n"); 
        return _ERR_FAT_MALLOC_FAILED;        
    }

    if(bRename)
    {
        goto delete_entry;
    }

    //不可能删除根目录本身，这意味着要删除的目录文件必然都在Data Region中，
    //从而cluster必然不小于2；注意:目录文件对应的目录项可能在Root Dir Region中
    cluster = (UINT32)(de->DIR_FstClusLO | (UINT32)(de->DIR_FstClusHI<<16));

    //
    // Step 1) Test the type of the entry, regular file or directory
    //
    if(de->DIR_Attr & ATTR_DIR)
    {
        // If the entry pointed to a not null directory, we do not delete it.
        do{
            for(cursec=0; cursec<sb_info->iSecPerClus; cursec++)
            {
                memset(secbuf, 0x00, DEFAULT_SECSIZE);
                lErrCode = fat_read_cluster(sb_info, cluster, cursec, secbuf);
                if(_ERR_FAT_SUCCESS == lErrCode)
                {                
                    for(i=0; i<DEFAULT_SECSIZE; i+=DEFAULT_DIRENTRYSIZE)
                    {
                        Buf2FDE((UINT8*)(secbuf+i), &dent);
                        if(0x00 == dent.DIR_Name[0])
                        {
                            goto delete_entry;
                        }
                        else if(0xE5 == dent.DIR_Name[0]) // ignore deleted dir
                        {
                            continue;
                        }
                        else if('.' == dent.DIR_Name[0]) // igore '.' and '..'
                        {
                            continue;
                        }
                        else  // found one, that is, the directory is not null.
                        {
                            SVC_FSS_TRACE( DL_FATWARNING, "In  fat_remove_entry, _ERR_FAT_DIR_NOT_NULL.\n"); 
                            lErrCode = _ERR_FAT_DIR_NOT_NULL;
                            goto end;
                        }
                    }
                }
                else
                {
                    lErrCode = _ERR_FAT_READ_SEC_FAILED;
                    goto end;
                }
            }
            
            lErrCode = fat_get_next_cluster(sb_info, cluster, &cluster);
            if (lErrCode != _ERR_FAT_SUCCESS)
            {
                SVC_FSS_TRACE(DL_FATERROR, "fat_remove_entry: fat_get_next_cluster() return %d", lErrCode);
                goto end;
            }
        }while(!fat_is_last_cluster(sb_info, cluster));        
    }

    //delete_entry:
    //
    // Coming here, the entry pointed to eithe a not-null directory or a regular file
    // and we can delete it safely.
    //
    
    // Step 2) Remove the 'de' itselft
    //
    // Delete the de by setting the DIR_Name[0] to 0xE5, that is, just mark it.
    //
  
delete_entry:
    
    memset(&dent, 0x00, sizeof(FAT_DIR_ENTRY));
    
    if(bclu > 0)  // Operation not on the root directory
    {
        lErrCode = fat_read_entries(sb_info, bclu, bsec, eoff, secbuf, &ecount);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_remove_entry,fat_read_entries error, ErrorCode = %d \n",lErrCode); 
            goto end;
        }
        
        for(i=0; i< ecount; i++)
        {
            *(secbuf+i*DEFAULT_DIRENTRYSIZE) = 0xe5;
        }

        Buf2FDE((secbuf+(ecount-1)*DEFAULT_DIRENTRYSIZE), &dent);
        
        lErrCode = fat_write_entries(sb_info, bclu, bsec, eoff, secbuf, ecount);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_remove_entry,fat_write_entries error, ErrorCode = %d \n",lErrCode);
            goto end;
        }
    }
    else // Operation on the root directory
    {
        lErrCode = fat_read_root_entries(sb_info, bsec, eoff, secbuf, &ecount);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_remove_entry,fat_read_root_entries error, ErrorCode = %d \n",lErrCode);
            goto end;
        }
        
        for(i=0; i< ecount; i++)
        {
            *(secbuf+i*DEFAULT_DIRENTRYSIZE) = 0xe5;
        }

        Buf2FDE((secbuf+(ecount-1)*DEFAULT_DIRENTRYSIZE), &dent);
        
        lErrCode = fat_write_root_entries(sb_info, bsec, eoff, secbuf, ecount);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_remove_entry,fat_write_root_entries error, ErrorCode = %d \n",lErrCode);
            goto end;
        }       
    }

    // Step 3) Remove the content which the 'de' point to according the test result
    // if the directory is not null, we can not get here.
    if(!bRename && (de->DIR_FstClusLO != CLUSTER_FREE || de->DIR_FstClusHI != CLUSTER_FREE))
    {
        UINT32 FstCluster;

        FstCluster = (UINT32)((UINT32)(de->DIR_FstClusHI << 16) |de->DIR_FstClusLO);
        lErrCode = fat_trunc_fatentry(sb_info, FstCluster);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_remove_entry,fat_trunc_fatentry error, ErrorCode = %d \n",lErrCode);
            goto end;
        }
        // 在上面的fat_trunc_fatentry()中已经将包括第一个的cluster的FAT表项释放。
        /* su_wenguang 2007/5/30
        cluster = dent.DIR_FstClusLO;
        lErrCode = fat_set_fat_entry_value(sb_info, cluster, CLUSTER_FREE);
        if(_ERR_FAT_SUCCESS != lErrCode)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_remove_entry,fat_set_fat_entry_value error, ErrorCode = %d \n",lErrCode);
            goto end;
        }*/
    }
end:    
    if(NULL != secbuf)
    {
        FAT_SECT_BUF_FREE(secbuf);
    }    
    return lErrCode;    
}


//*******************************************************************************
// Function:
//   create a directory or file entry in the pointed directory and alloc cluster
//   for the newly created entry.
// Parameter:
//   diri[in], the pointed directory on which to create entry.
//   de [in][out], point to the entry to add, output the DIR_FstClusLO filed.   
// Return value:
//    _ERR_FAT_SUCCESS indicates a successful operation. 
//         And unsuccessful operation is denoted by the err code as following.
//      _ERR_FAT_WRITE_SEC_FAILED:   Write the sector failed. 
//      _ERR_FAT_READ_SEC_FAILED:    Read the sector failed.
//      _ERR_FAT_DISK_FULL:          The logical disk is full.
//      _ERR_FAT_MALLOC_FAILED       allocte memory failed.
//*******************************************************************************

// 在diri目录中创建名字为pszFileName的目录项(entry)
INT32 fat_add_entry(struct inode *diri, FAT_DIR_ENTRY *de,
            UINT32* bclu, UINT16 *bsec, UINT16 *eoff, WCHAR* pszFileName, BOOL bRename)
{
    FAT_DIR_ENTRY *d_entry = &(diri->u.fat_i.entry);
    FAT_SB_INFO *sb = &(diri->i_sb->u.fat_sb);
    UINT8* secbuf = NULL;    
    UINT32 curclu = 0, preclu = 0;
    UINT32 curclu2 = 0, preclu2 = 0;
    UINT16 secoff = 0;
    UINT8 cursec;
    FAT_DIR_ENTRY dent;
    
    INT32 iRet = _ERR_FAT_SUCCESS;
    INT32 iResult = _ERR_FAT_SUCCESS;

    UINT32 ecount = 0;
    FAT_DIR_ENTRY snent;  // short name entry
    UINT8* ebuf;
    UINT16 freenr = 0;
    UINT16 freebegoff =0;
    UINT16 freebegsec =0;
    UINT32 freebegclu = 0;
    UINT16 leftspace = 0;
    BOOL   hasset = FALSE;
    
    ebuf = FAT_SECT_BUF_ALLOC();
    if(NULL == ebuf)
    {
        return _ERR_FAT_MALLOC_FAILED;        
    }   
    
    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        iRet = _ERR_FAT_MALLOC_FAILED;
        SVC_FSS_TRACE(DL_FATERROR, "fat_add_entry(): No more memory\n");
        goto end;
    }    

    curclu = (UINT32)((UINT32)(d_entry->DIR_FstClusHI<< 16) | d_entry->DIR_FstClusLO);        //当前目录下的第一个cluster
    iRet = fat_get_long_entry(sb, curclu, pszFileName, (FAT_LONG_DIR_ENTRY*)ebuf, &ecount);
    if(_ERR_FAT_SUCCESS != iRet)
    {
        SVC_FSS_TRACE(DL_FATERROR, "fat_add_entry: fat_get_long_entry() return %d\n", iRet);
        goto end;
    }
    
    Buf2FDE((ebuf + (ecount-1) * sizeof(FAT_DIR_ENTRY)), &snent);

    // Set the member value of FAT_DIR_ENTRY
    snent.DIR_Attr = de->DIR_Attr;
    snent.DIR_CrtDate = de->DIR_CrtDate;
    snent.DIR_CrtTime = de->DIR_CrtTime;
    snent.DIR_CrtTimeTenth = de->DIR_CrtTimeTenth;  // need handle it
    snent.DIR_FileSize = de->DIR_FileSize;
    snent.DIR_FstClusHI = de->DIR_FstClusHI; // 如创建的是目录，下面会重新设置first-cluster
    snent.DIR_FstClusLO = de->DIR_FstClusLO;
    snent.DIR_LstAccDate = de->DIR_LstAccDate;
    // de->DIR_Name, has set before.
    snent.DIR_NTRes = de->DIR_NTRes;
    snent.DIR_WrtDate = de->DIR_WrtDate;
    snent.DIR_WrtTime = de->DIR_WrtTime;
    
    //
    // 1) find free DIR entry in the existed cluster 
    // 2) if all of the cluster is full, alloc a new cluster
    // 3) add the entry gotten from 1) or 2) into parent DIR entry
    //
    
    // Find the free entry in already allocate cluster.
    do
    {
        //Find the free entry in current cluser.
        for(cursec = 0; cursec < sb->iSecPerClus; cursec++)
        {
            // Find the free entry in current sector.
            memset(secbuf, 0x00, DEFAULT_SECSIZE);
            iResult = fat_read_cluster(sb, curclu, cursec, secbuf);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                iRet = _ERR_FAT_READ_SEC_FAILED;
                SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_read_cluster error, ErrorCode = %d \n",iRet);
                goto end;
            }
            
            for(secoff = 0; secoff < DEFAULT_SECSIZE; secoff += DEFAULT_DIRENTRYSIZE)
            {
                Buf2FDE((UINT8*)(secbuf+secoff), &dent);
                
                // Deleted entry or free entry.
                if((0x00 == dent.DIR_Name[0]) || (0xE5 == dent.DIR_Name[0]))
                {
                    if(!hasset)               // 遇到符合条件的，只设置一次
                    {
                        freebegsec = cursec;  // 只记住开始的位置
                        freebegoff = secoff;
                        freebegclu = curclu;
                        hasset = TRUE;
                    }
                    
                    freenr++;               // 记录空fat项的个数
                    
                    if(freenr == ecount)
                    {
                        goto free_entry_found;
                    }
                }
                else
                {
                    freebegsec = 0;
                    freebegoff = 0;
                    freebegclu = 0;
                    freenr = 0;
                    hasset = FALSE;
                }
            }
        }

        preclu = curclu;
        iRet = fat_get_next_cluster(sb, preclu, &curclu);
        if (iRet != _ERR_FAT_SUCCESS)
        {
            SVC_FSS_TRACE(DL_FATERROR, "fat_add_entry: fat_get_next_cluster() return %d", iRet);
            goto end;
        }
    }while((!fat_is_last_cluster(sb, curclu)));

    //
    // If NOT found the free entry in allocted cluster,
    // allocte one or two new clusters for it.
    //
    preclu2 = preclu;
    iRet = fat_get_free_cluster(sb, preclu2, TRUE, &curclu2);
    if(iRet < 0)
    {
        SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_get_free_cluster()1 failed! ErrorCode = %d\n",iRet);
        goto end;
    }

    if(0 == freebegsec && 0 == freebegoff && 0 == freebegclu)
    {
        freebegclu = curclu2;
    }

    // Set the FAT table to 0xFFFF for the new-cluster.
    iRet = fat_set_fat_entry_value(sb, curclu2, CLUSTER_NULL, TRUE, NULL);
    if(_ERR_FAT_SUCCESS != iRet)
    {
        SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_set_fat_entry_value_0, ErrorCode = %d\n",iRet);
        goto end;
    }

    if (sb->iFreeCnterInitFlag)
    {
        sb->iFreeCluCnt--;
    }
    
    // Set the FAT table to new-cluster for the pre-cluster.
    iRet = fat_set_fat_entry_value(sb, preclu2, curclu2, TRUE, NULL);
    if(_ERR_FAT_SUCCESS != iRet)
    {
        SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_set_fat_entry_value_1, ErrorCode = %d\n",iRet);
        goto end;
    }
    if(TRUE == hasset)
    {
        leftspace = (UINT16)(sb->iSecPerClus*sb->iBytesPerSec - (freebegoff + freebegsec*sb->iBytesPerSec));
    }
    else
    {
        leftspace = 0;
    }
    // Allocate the second cluster
    if((UINT16)(ecount*sizeof(FAT_DIR_ENTRY)) > sb->iSecPerClus*sb->iBytesPerSec+leftspace)
    {
        preclu2 = curclu2;
        iRet = fat_get_free_cluster(sb, preclu2, TRUE, &curclu2);
        if (iRet < 0)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_get_free_cluster()2 failed! ErrorCode = %d\n",iRet);
            goto end;
        }

        // Set the FAT table to 0xFFFF for the new-cluster.
        iRet = fat_set_fat_entry_value(sb, curclu2, CLUSTER_NULL, TRUE, NULL);
        if(_ERR_FAT_SUCCESS != iRet)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_set_fat_entry_value_2, ErrorCode = %d\n",iRet);
            goto end;
        }
        
        if (sb->iFreeCnterInitFlag)
        {
            sb->iFreeCluCnt--;
        }
        
        // Set the FAT table to new-cluster for the pre-cluster.
        iRet = fat_set_fat_entry_value(sb, preclu2, curclu2, TRUE, NULL);
        if(_ERR_FAT_SUCCESS != iRet)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_set_fat_entry_value_3, ErrorCode = %d\n",iRet);
            goto end;
        }
    
    }

    
free_entry_found:
    if((de->DIR_Attr&ATTR_DIR) && !bRename)
    {
        // Alloc a cluster for the creating dir-entry.
        // Set the output parameter.
        iRet = fat_get_free_cluster(sb, 2, TRUE, &curclu2);
        if(iRet < 0)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_get_free_cluster error, ErrorCode = %d\n", iRet);
            goto end;
        }
        snent.DIR_FstClusLO = (UINT16)(curclu2 & 0x0000FFFF);
        snent.DIR_FstClusHI = (UINT16)((curclu2 >> 16) & 0x0000FFFF);
            
        // set the FAT of new cluster .
        iRet = fat_set_fat_entry_value(sb, curclu2, CLUSTER_NULL, TRUE, NULL); 
        if(_ERR_FAT_SUCCESS !=  iRet)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_set_fat_entry_value_4 error, ErrorCode = %d\n",iRet);
            goto end;
        }
        if (sb->iFreeCnterInitFlag)
        {
            sb->iFreeCluCnt--;
        }

    }
    
    // Write the creating dir-entry into current directory

    // Set the output parameters
    *bclu = freebegclu;
    *bsec = freebegsec;
    *eoff = freebegoff;

    FDE2Buf(&snent, (ebuf + (ecount-1) * sizeof(FAT_DIR_ENTRY)));
    iRet = fat_write_entries(sb, *bclu, *bsec, *eoff, ebuf, (UINT8)ecount);
    if(_ERR_FAT_SUCCESS != iRet)
    {
        SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_write_entries error, ErrorCode = %d\n",iRet);
        goto end;
    }

    //
    // If we are creating a directory, we make '.' and '..' diretory 
    // into the newly creating diretory
    //
    if((de->DIR_Attr&ATTR_DIR) && !bRename)
    {
        memset(&dent, 0x00, sizeof(dent));
        memset(secbuf, 0x00, DEFAULT_SECSIZE);
        
        memcpy(dent.DIR_Name, ".          ",11);
        dent.DIR_FstClusLO = snent.DIR_FstClusLO;
        dent.DIR_FstClusHI = snent.DIR_FstClusHI;
        dent.DIR_Attr |= ATTR_DIR;
        FDE2Buf(&dent, secbuf);
        
        memcpy(dent.DIR_Name, "..         ",11);
        dent.DIR_Attr |= ATTR_DIR;
        if (FS_ROOT_INO == diri->i_ino)
        {// 用簇号0表示上级目录为根目录。
            dent.DIR_FstClusLO = 0;
            dent.DIR_FstClusHI = 0;
        }
        else
        {
            dent.DIR_FstClusLO = d_entry->DIR_FstClusLO;
            dent.DIR_FstClusHI = d_entry->DIR_FstClusHI;
        }
        FDE2Buf(&dent, (UINT8*)(secbuf+DEFAULT_DIRENTRYSIZE));
        
        iRet = fat_write_cluster(sb, curclu2, 0, secbuf);
        if(_ERR_FAT_SUCCESS != iRet)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_entry,fat_write_cluster error, ErrorCode = %d\n",iRet);
            goto end;
        }
    }
    
end:
    
    if(NULL != secbuf) 
    {
        FAT_SECT_BUF_FREE(secbuf);
    }
    if(NULL != ebuf) 
    {
        FAT_SECT_BUF_FREE(ebuf);
    }

    return iRet;
}

/********************************************************************************/
// Function:
//   create a directory or file entry in the root directory and alloc cluster
//   for the newly created entry.
//
// Parameter:
//   rdiri[in], Point to the inode of root node.
//   de [in][out], point to the entry to add, output the DIR_FstClusLO filed.   
// Return value:
//    _ERR_FAT_SUCCESS indicates a successful operation. 
//         And unsuccessful operation is denoted by the err code as following.
//      _ERR_FAT_WRITE_SEC_FAILED:   Write the sector failed. 
//      _ERR_FAT_READ_SEC_FAILED:    Read the sector failed.
//      _ERR_FAT_ROOT_FULL:          The root dir is full.
//      _ERR_FAT_MALLOC_FAILED       allocte memory failed.
//*******************************************************************************
INT32 fat_add_root_entry(struct inode *rdiri, FAT_DIR_ENTRY *de, 
            UINT16 *bsec, UINT16 *eoff, WCHAR* pszFileName, BOOL bRename)
{
    FAT_SB_INFO* sb_info = &(rdiri->i_sb->u.fat_sb);
    UINT8* secbuf;    
    UINT32  secoff = 0;
    FAT_DIR_ENTRY dent;
    UINT8 cheat = 1;
    UINT32 cursec;
    INT32 iRet = _ERR_FAT_SUCCESS;
    
    UINT32 ecount = 0;
    FAT_DIR_ENTRY snent;  // short name entry
    UINT8* ebuf;
    UINT16 freenr = 0;
    UINT16 freebegoff =0;
    UINT16 freebegsec =0;
    BOOL   hasset = FALSE;
    
    ebuf = FAT_SECT_BUF_ALLOC();
    if(NULL == ebuf)
    {
        SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_root_entry,malloc failed_0\n");
        return _ERR_FAT_MALLOC_FAILED;        
    }    

    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_root_entry,malloc failed_1\n",iRet);
        iRet = _ERR_FAT_MALLOC_FAILED;
        goto end;
    }

    iRet = fat_get_long_entry(sb_info, 0, pszFileName, (FAT_LONG_DIR_ENTRY*)ebuf, &ecount);
    if(_ERR_FAT_SUCCESS != iRet)
    {
        SVC_FSS_TRACE(DL_FATWARNING, "call fat_get_long_entry , iRet = %d\n",iRet);
        goto end;
    }

    Buf2FDE((ebuf + (ecount-1) * sizeof(FAT_DIR_ENTRY)), &snent);

    // Set the member value of FAT_DIR_ENTRY
    snent.DIR_Attr = de->DIR_Attr;
    snent.DIR_CrtDate = de->DIR_CrtDate;
    snent.DIR_CrtTime = de->DIR_CrtTime;
    snent.DIR_CrtTimeTenth = de->DIR_CrtTimeTenth;  // need handle it
    snent.DIR_FileSize = de->DIR_FileSize;
    snent.DIR_FstClusHI = de->DIR_FstClusHI; // 在下面会重新设置first-cluster
    snent.DIR_FstClusLO = de->DIR_FstClusLO;
    snent.DIR_LstAccDate = de->DIR_LstAccDate;
    // de->DIR_Name, has set before.
    snent.DIR_NTRes = de->DIR_NTRes;
    snent.DIR_WrtDate = de->DIR_WrtDate;
    snent.DIR_WrtTime = de->DIR_WrtTime;
    
    cursec = sb_info->iRootDirStartSec;
    do 
    {
        iRet = DRV_BLOCK_READ( sb_info->sb->s_dev, cursec + sb_info->iHiddSec, secbuf );
        if(_ERR_FAT_SUCCESS != iRet)
        {
            SVC_FSS_TRACE(DL_FATERROR, "DRV_BLOCK_READ error, ErrorCode = %d\n",iRet);
            goto end;

        }

        for(secoff = 0; secoff < DEFAULT_SECSIZE; secoff += DEFAULT_DIRENTRYSIZE)
        {
            Buf2FDE((UINT8*)(secbuf+secoff), &dent);

            // Deleted entry or free entry.
            if(0x00 == dent.DIR_Name[0] || 0xE5 == dent.DIR_Name[0])
            {
                if(!hasset)               // 遇到符合条件的，只设置一次
                {
                    freebegsec = (UINT16)cursec;  // 只记住开始的位置
                    freebegoff = (UINT16)secoff;

                    hasset = TRUE;
                }
                
                freenr++;
                
                if(freenr == ecount)
                {
                    goto free_entry_found;
                }
            }
            else
            {
                freebegsec = 0;
                freebegoff = 0;
                freenr = 0;
                hasset = FALSE;
            }
        }

        cursec++;      
        if(cursec - sb_info->iRootDirStartSec >= sb_info->iRootDirSecCnt)
        {
           iRet = _ERR_FAT_ROOT_FULL;   
           SVC_FSS_TRACE(DL_DETAIL, " in fat_add_root_entry,fat_root is full\n");
           goto end;
        }
    } while(1 == cheat); // dead loop

free_entry_found:
    if((de->DIR_Attr&ATTR_DIR) && !bRename)
    {
        UINT32 iFreeClu = 0;
        
        //    
        // Alloc a cluster for the newly created directory.
        //        
        iRet = fat_get_free_cluster(sb_info, 2, TRUE, &iFreeClu);
        if(iRet < 0)
        {
            SVC_FSS_TRACE(DL_FATWARNING, " in fat_add_root_entry,fat_get_free_cluster error ErrorCode = %d\n",iRet);
            goto end;
        }

        snent.DIR_FstClusLO = (UINT16)(iFreeClu & 0xffff);
        snent.DIR_FstClusHI= (UINT16)((iFreeClu >> 16) & 0xffff);
        
        iRet = fat_set_fat_entry_value(sb_info, iFreeClu, CLUSTER_NULL, TRUE, NULL);    
        if(_ERR_FAT_SUCCESS != iRet)
        {
            SVC_FSS_TRACE(DL_FATWARNING, " in fat_add_root_entry,fat_set_fat_entry_value_0 error ErrorCode = %d\n",iRet);
            goto end;
        }
        if (sb_info->iFreeCnterInitFlag)
        {
            sb_info->iFreeCluCnt--;
        }
    }
    
    // Set the out parameters.
    *bsec = (UINT16)(freebegsec - sb_info->iRootDirStartSec);
    *eoff = (UINT16)freebegoff;

    FDE2Buf(&snent, (ebuf + (ecount-1) * sizeof(FAT_DIR_ENTRY)));
    iRet = fat_write_root_entries(sb_info, *bsec, *eoff, ebuf, (UINT8)ecount);
    if(_ERR_FAT_SUCCESS != iRet)
    {
        SVC_FSS_TRACE( DL_FATDETAIL,"in fat_add_root_entry(), fat_write_root_entries iRet = %d.\n",iRet);  
        goto end;
    }
    
    // If we are creating a directory, we make '.' and '..' diretory 
    // into the newly creating diretory
    if((de->DIR_Attr&ATTR_DIR) && !bRename)
    {
        memset(&dent, 0x00, sizeof(dent));
        memset(secbuf, 0x00, DEFAULT_SECSIZE);
        
        memcpy(dent.DIR_Name, ".          ", 11);
        dent.DIR_Attr |= ATTR_DIR;
        dent.DIR_FstClusLO = snent.DIR_FstClusLO;
        dent.DIR_FstClusHI= snent.DIR_FstClusHI;
        FDE2Buf(&dent, secbuf);
        
        memcpy(dent.DIR_Name, "..         ", 11);
        dent.DIR_Attr |= ATTR_DIR;
        dent.DIR_FstClusLO = 0; // 用0表示根目录。
        dent.DIR_FstClusHI = 0;
        FDE2Buf(&dent, (UINT8*)(secbuf+DEFAULT_DIRENTRYSIZE));
        
        iRet = fat_write_cluster(sb_info, (UINT32)((UINT16)snent.DIR_FstClusLO |(UINT32)(snent.DIR_FstClusHI<<16)), 0, secbuf);
        if(_ERR_FAT_SUCCESS != iRet)
        {
            SVC_FSS_TRACE( DL_FATERROR,"In  fat_add_root_entry,fat_write_cluster error, ErrorCode = %d\n",iRet);
            goto end;
        }

        //* TODO: 循环写入cluster中剩余剩余sector.
    }

end:    
    if(NULL != secbuf) 
    {
        FAT_SECT_BUF_FREE(secbuf);
    }
    if(NULL != ebuf) 
    {
        FAT_SECT_BUF_FREE(ebuf);
    }

    return iRet;
}

INT32 fat_write_entries(FAT_SB_INFO* sb, 
            UINT32 bclu, UINT16 bsec, UINT16 eoff, UINT8* ebuf, UINT8 ecount)
{
    UINT32 len;
    UINT8* bufp = ebuf;
    UINT8  secnum = (UINT8)bsec;
    UINT32 clunum = bclu;
    UINT32 secoff = eoff;
    UINT8* secbuf;
    
    UINT8  cheat = 1;
    
    INT32 iResult;
    INT32 iRet = _ERR_FAT_SUCCESS;

    len = sizeof(FAT_DIR_ENTRY)*ecount;

    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        SVC_FSS_TRACE( DL_FATERROR,"In  fat_write_entries,malloc error\n");
        return _ERR_FAT_MALLOC_FAILED;        
    }

    while(cheat)
    {
        memset(secbuf, 0x00, DEFAULT_SECSIZE);
        
        if(secoff == 0) // 对secoff区分等于0和小于0(见下)的情况，
        {               // 是为了在等于0时减少一次读操作
            if(len > DEFAULT_SECSIZE) 
            {
                iResult = fat_write_cluster(sb, clunum, secnum, bufp);
                if(_ERR_FAT_SUCCESS != iResult)
                {   
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_entries,fat_write_cluster_0 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                bufp += DEFAULT_SECSIZE;
                secnum++;
                secoff = 0;
                len -= DEFAULT_SECSIZE;
            }
            else if(len == DEFAULT_SECSIZE) 
            {
                iResult = fat_write_cluster(sb, clunum, secnum, bufp);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_entries,fat_write_cluster_1 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                break;
            }
            else // secoff + len < DEFAULT_SECSIZE (1)
            {
                
                iResult = fat_read_cluster(sb, clunum, secnum, secbuf);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_entries,fat_read_cluster_0 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                memcpy((UINT8*)(secbuf+secoff), bufp, len);
                iResult = fat_write_cluster(sb, clunum, secnum, secbuf);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_entries,fat_write_cluster_2 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                break;
            }
        }
        else if(secoff > 0)
        {
            if(secoff + len > DEFAULT_SECSIZE) 
            {
                iResult = fat_read_cluster(sb, clunum, secnum, secbuf);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_entries,fat_read_cluster_1 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                memcpy((UINT8*)(secbuf+secoff), bufp, DEFAULT_SECSIZE-secoff);
                
                iResult = fat_write_cluster(sb, clunum, secnum, secbuf);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_entries,fat_write_cluster_3 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                bufp += (DEFAULT_SECSIZE-secoff);
                
                len -= (DEFAULT_SECSIZE-secoff);
                secnum++;
                secoff = 0;
            }
            else //secoff + len <= DEFAULT_SECSIZE (1)
            {
                iResult = fat_read_cluster(sb, clunum, secnum, secbuf);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_entries,fat_read_cluster_2 error, ErrorCode = %d\n",iRet);
                    break;
                }

                memcpy((UINT8*)(secbuf+secoff), bufp, len);

                iResult = fat_write_cluster(sb, clunum, secnum, secbuf);
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_entries,fat_write_cluster_4 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                break;
            }
        }        
        
        if(secnum == sb->iSecPerClus) // Get to the end of the cluster
        {
            secnum = 0;
            iRet = fat_get_next_cluster(sb, clunum, &clunum);
            if (iRet != _ERR_FAT_SUCCESS)
            {
                SVC_FSS_TRACE(DL_FATERROR, "fat_write_entries: fat_get_next_cluster() return %d", iRet);
                break;
            }
        }
    }// while(cheat)

    if(NULL != secbuf) 
    {
        FAT_SECT_BUF_FREE(secbuf);
    }
    
    return iRet;
}


INT32 fat_write_root_entries(FAT_SB_INFO* sb,
                UINT16 bsec, UINT16 eoff, UINT8* ebuf, UINT8 ecount)
{
    UINT32 len;
    UINT8* bufp = ebuf;
    UINT32  secnum = bsec + sb->iRootDirStartSec;
    UINT32 secoff = eoff;
    UINT8* secbuf;
    UINT8  cheat = 1;
    INT32 iResult;
    INT32 iRet = _ERR_FAT_SUCCESS;

    len = sizeof(FAT_DIR_ENTRY)*ecount;

    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries, malloc failed_0\n");
        return _ERR_FAT_MALLOC_FAILED;        
    }

    while(cheat)
    {
        memset(secbuf, 0x00, DEFAULT_SECSIZE);
        
        if(secoff == 0) // 对secoff区分等于0和小于0(见下)的情况，
        {               // 是为了在等于0时减少一次读操作
            if(len > DEFAULT_SECSIZE) 
            {
                 SVC_FSS_TRACE( DL_FATDETAIL,
            		"in fat_write_root_entries() call DRV_BLOCK_WRITE(),with secnum = %d.\n",
            		secnum);  
                iResult = DRV_BLOCK_WRITE( sb->sb->s_dev, secnum + sb->iHiddSec, bufp );
                SVC_FSS_TRACE( DL_FATDETAIL,
            		"in fat_write_root_entries() after DRV_BLOCK_WRITE(),iResul = %d.\n",
            		iResult); 
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries,DRV_BLOCK_WRITE_0 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                bufp += DEFAULT_SECSIZE;
                
                secnum++;
                secoff = 0;
                len -= DEFAULT_SECSIZE;
            }
            else if(len == DEFAULT_SECSIZE) 
            {
                SVC_FSS_TRACE( DL_FATDETAIL,"secoff == 0----------------in write =,secnum = %d, sb->iRootDirStartSec=%d\n",secnum,sb->iRootDirStartSec);
                iResult = DRV_BLOCK_WRITE( sb->sb->s_dev, secnum + sb->iHiddSec, bufp );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries,DRV_BLOCK_WRITE_1 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                break;
            }
            else // secoff + len < DEFAULT_SECSIZE (1)
            {
                iResult = DRV_BLOCK_READ( sb->sb->s_dev, secnum + sb->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries,DRV_BLOCK_READ_0 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                memcpy((UINT8*)(secbuf+secoff), bufp, len);
                SVC_FSS_TRACE( DL_FATDETAIL,"secoff == 0----------------in write <,secnum = %d, sb->iRootDirStartSec=%d\n",secnum,sb->iRootDirStartSec);
                iResult = DRV_BLOCK_WRITE( sb->sb->s_dev, secnum + sb->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries,DRV_BLOCK_WRITE_2 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                break;
            }
        }
        else if(secoff > 0)
        {
            if(secoff + len > DEFAULT_SECSIZE) 
            {
                iResult = DRV_BLOCK_READ( sb->sb->s_dev, secnum + sb->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries,DRV_BLOCK_READ_1 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                memcpy((UINT8*)(secbuf+secoff), bufp, DEFAULT_SECSIZE-secoff);
                SVC_FSS_TRACE( DL_FATDETAIL,"secoff>0----------------in write >,secnum = %d,sb->iRootDirStartSec=%d\n",secnum,sb->iRootDirStartSec);
                iResult = DRV_BLOCK_WRITE( sb->sb->s_dev, secnum + sb->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries,DRV_BLOCK_WRITE_3 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                bufp += (DEFAULT_SECSIZE-secoff);
                
                len -= (DEFAULT_SECSIZE-secoff);
                secnum++;
                secoff = 0;
            }
            else //secoff + len <= DEFAULT_SECSIZE (1)
            {
				iResult = DRV_BLOCK_READ( sb->sb->s_dev, secnum + sb->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_READ_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries,DRV_BLOCK_READ_2 error, ErrorCode = %d\n",iRet);
                    break;
                }

                memcpy((UINT8*)(secbuf+secoff), bufp, len);
                SVC_FSS_TRACE( DL_FATDETAIL,"secoff>0----------------in write <=,secnum = %d,sb->iRootDirStartSec=%d\n",secnum,sb->iRootDirStartSec);
                iResult = DRV_BLOCK_WRITE( sb->sb->s_dev, secnum + sb->iHiddSec, secbuf );
                if(_ERR_FAT_SUCCESS != iResult)
                {
                    iRet = _ERR_FAT_WRITE_SEC_FAILED;
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_write_root_entries,DRV_BLOCK_WRITE_4 error, ErrorCode = %d\n",iRet);
                    break;
                }
                
                break;
            }
        }        
    }// while(cheat)

    if(NULL != secbuf) 
    {
        FAT_SECT_BUF_FREE(secbuf);
    }
    
    return iRet;
}


INT32 fat_read_root_entries(FAT_SB_INFO* sb,
                UINT16 bsec, UINT16 eoff, UINT8* ebuf, UINT8* ecount)
{
    UINT16 cursec;
    UINT32 secoff;
    UINT8* secbuf;
    UINT8* bufp;
    INT32 iResult = _ERR_FAT_SUCCESS;
    UINT8 cheat = 1;
    UINT8 status = 0;
    UINT8 count = 0;
    
    bufp = ebuf;    
    cursec = (UINT16)(bsec+sb->iRootDirStartSec);
    
    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        SVC_FSS_TRACE( DL_FATERROR,"in fat_read_root_entries,malloc failed!\n");
        return _ERR_FAT_MALLOC_FAILED;        
    }
    SVC_FSS_TRACE( DL_FATDETAIL, "in fat_read_root_entries, bsec = %d, eoff = %d.\n", bsec, eoff);
    while(cheat)
    {
        memset(secbuf, 0x00, DEFAULT_SECSIZE);
        iResult = DRV_BLOCK_READ(sb->sb->s_dev, cursec + sb->iHiddSec, secbuf);
        if(_ERR_FAT_SUCCESS != iResult)
        {
            iResult = _ERR_FAT_READ_SEC_FAILED;
            SVC_FSS_TRACE( DL_FATERROR,"in fat_read_root_entries,DRV_BLOCK_READ error, ErrorCode = %d\n",iResult);
            goto end;
        }
        
        for(secoff = eoff; secoff < DEFAULT_SECSIZE; secoff += DEFAULT_DIRENTRYSIZE)
        {
            UINT8* np = (UINT8*)(secbuf+secoff);
            SVC_FSS_TRACE( DL_FATDETAIL, "------------in read, cursec = %d, secoff = %d\n",cursec, secoff);        
            switch(status)
            {
            case 0: // start status
                if(FAT_IS_SHORT_ENTRY(np))   // short 
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------0 in short entry, eoff = %d\n",eoff);
                    memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                    bufp += DEFAULT_DIRENTRYSIZE;
                    count++;
                    goto end;
                }
                else if(FAT_IS_LAST_LONG_ENTRY(np) && FAT_IS_FIRST_LONG_ENTRY(np)) // last & first
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------0 in long entry_1, eoff = %d\n",eoff);
                    memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                    bufp += DEFAULT_DIRENTRYSIZE;
                    count++;
                    status = 1;
                }
                else if(FAT_IS_LAST_LONG_ENTRY(np) && !FAT_IS_FIRST_LONG_ENTRY(np)) // last & not first
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------0 in long entry_2, eoff = %d\n",eoff);
                    memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                    bufp += DEFAULT_DIRENTRYSIZE;
                    count++;
                    status = 2;
                }
                else
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------0 in else, eoff = %d\n",eoff);
                    SVC_FSS_TRACE( DL_FATERROR,"in fat_read_root_entries,_ERR_FAT_ERROR\n");
                    iResult = _ERR_FAT_ERROR;
                    goto end;
                    /*
                    bufp = ebuf;
                    status = 0;
                    count = 0;*/
                }
                break;
                
            case 1:  // last & first status
                if(FAT_IS_SHORT_ENTRY(np)) // short
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------1 in short, eoff = %d\n",eoff);
                    memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                    bufp += DEFAULT_DIRENTRYSIZE;
                    count++;
                    goto end;
                }
                else
                {
                    SVC_FSS_TRACE( DL_FATDETAIL, "------------1 in else, eoff = %d\n",eoff);
                    bufp = ebuf;
                    status = 0;
                    count = 0;
                }
                break;
                
            case 2: // middle long name status
                if(FAT_IS_FIRST_LONG_ENTRY(np))
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------2 in first long, eoff = %d\n",eoff);
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------long entry\n");
                    memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                    bufp += DEFAULT_DIRENTRYSIZE;
                    count++;
                    status = 1;
                }
                else if(FAT_IS_MIDDLE_LONG_ENTRY(np))
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------2 in middle long, eoff = %d\n",eoff);
                    memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                    bufp += DEFAULT_DIRENTRYSIZE;
                    count++;
                    status = 3;
                }
                else
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------2 in else long, eoff = %d\n",eoff);
                    bufp = ebuf;
                    status = 0; 
                    count = 0;
                }
                break;
                
            case 3: // last long name status
                if(FAT_IS_MIDDLE_LONG_ENTRY(np))
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------3 in middle long, eoff = %d\n",eoff);
                    memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                    bufp += DEFAULT_DIRENTRYSIZE;
                    count++;
                    status = 3;
                }
                else if(FAT_IS_FIRST_LONG_ENTRY(np))
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------3 in first long, eoff = %d\n",eoff);
                    memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                    bufp += DEFAULT_DIRENTRYSIZE;
                    count++;
                    status = 1;
                }
                else
                {
                    SVC_FSS_TRACE( DL_FATDETAIL,"------------3 in else, eoff = %d\n",eoff);
                    bufp = ebuf;
                    status = 0;
                    count = 0;
                }
                break;
            }
        }
        
        cursec++;      
        eoff = 0;  // 切换到新的扇区上查找时，需要从扇区开始进行，所以重新设置在扇区中的偏移位置。
        
        if(cursec - sb->iRootDirStartSec >= sb->iRootDirSecCnt)
        {
            iResult = _ERR_FAT_ERROR;
            SVC_FSS_TRACE( DL_FATERROR,"in fat_read_root_entries,FAT_ROOT_FULL:\n");
            SVC_FSS_TRACE( DL_FATERROR,"cursec = %d, sb->iRootDirStartSec = %d, sb->iRootDirSecCnt = %d.\n", 
                    cursec, sb->iRootDirStartSec, sb->iRootDirSecCnt);
            goto end;
        }
    }
    
end:
    if(NULL != secbuf) 
    {
        FAT_SECT_BUF_FREE(secbuf);
    }

    *ecount = count;
    
    return iResult;
}


INT32 fat_read_entries(FAT_SB_INFO* sb, UINT32 bclu,
                UINT16 bsec, UINT16 eoff, UINT8* ebuf, UINT8* ecount)
{
    UINT8  cursec, secnum;
    UINT32 clunum, secoff;
    UINT8* secbuf;
    UINT8* bufp;
    INT32 iResult = _ERR_FAT_SUCCESS;
    UINT8 status = 0;
    UINT8 count = 0;

    bufp = ebuf;    
    clunum = bclu;
    secnum = (UINT8)bsec;
    
    secbuf = FAT_SECT_BUF_ALLOC();
    if(NULL == secbuf)
    {
        SVC_FSS_TRACE( DL_FATERROR,"in fat_read_entries,malloc failed!\n");
        return _ERR_FAT_MALLOC_FAILED;        
    }
    
    while(!fat_is_last_cluster(sb, clunum))
    {
        for(cursec = secnum; cursec < sb->iSecPerClus; cursec++)
        {
            memset(secbuf, 0x00, DEFAULT_SECSIZE);
            
            iResult = fat_read_cluster(sb, clunum, cursec, secbuf);
            if(_ERR_FAT_SUCCESS != iResult)
            {
                iResult = _ERR_FAT_READ_SEC_FAILED;
                SVC_FSS_TRACE( DL_FATERROR,"in fat_read_entries,fat_read_cluster error, ErrorCode = %d\n",iResult);
                goto end;
                
            }
            
            for(secoff = eoff; secoff < DEFAULT_SECSIZE; secoff += DEFAULT_DIRENTRYSIZE)
            {
                UINT8* np = (UINT8*)(secbuf+secoff);
                switch(status)
                {
                case 0: // start status
                    if(FAT_IS_SHORT_ENTRY(np))   // short 
                    {
                        memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                        bufp += DEFAULT_DIRENTRYSIZE;
                        count++;
                        goto end;
                    }
                    else if(FAT_IS_LAST_LONG_ENTRY(np) && FAT_IS_FIRST_LONG_ENTRY(np)) // last & first
                    {
                        memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                        bufp += DEFAULT_DIRENTRYSIZE;
                        count++;
                        status = 1;
                    }
                    else if(FAT_IS_LAST_LONG_ENTRY(np) && !FAT_IS_FIRST_LONG_ENTRY(np)) // last & not first
                    {
                        memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                        bufp += DEFAULT_DIRENTRYSIZE;
                        count++;
                        status = 2;
                    }
                    else
                    {
                        SVC_FSS_TRACE( DL_FATERROR,"in fat_read_entries, _ERR_FAT_ERROR\n");
                        iResult = _ERR_FAT_ERROR;
                        goto end;
                        /*
                        bufp = ebuf;
                        status = 0;
                        count = 0;
                        */
                    }
                    break;
                    
                case 1:  // last & first status
                    if(FAT_IS_SHORT_ENTRY(np)) // short
                    {
                        memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                        bufp += DEFAULT_DIRENTRYSIZE;
                        count++;
                        goto end;
                    }
                    else
                    {
                        bufp = ebuf;
                        status = 0;
                        count = 0;
                    }
                    break;
                    
                case 2: // middle long name status
                    if(FAT_IS_FIRST_LONG_ENTRY(np))
                    {
                        memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                        bufp += DEFAULT_DIRENTRYSIZE;
                        count++;
                        status = 1;
                    }
                    else if(FAT_IS_MIDDLE_LONG_ENTRY(np))
                    {
                        memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                        bufp += DEFAULT_DIRENTRYSIZE;
                        count++;
                        status = 3;
                    }
                    else
                    {
                        bufp = ebuf;
                        status = 0; 
                        count = 0;
                    }
                    break;
                    
                case 3: // last long name status
                    if(FAT_IS_MIDDLE_LONG_ENTRY(np))
                    {
                        memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                        bufp += DEFAULT_DIRENTRYSIZE;
                        count++;
                        status = 3;
                    }
                    else if(FAT_IS_FIRST_LONG_ENTRY(np))
                    {
                        memcpy(bufp, np, DEFAULT_DIRENTRYSIZE);
                        bufp += DEFAULT_DIRENTRYSIZE;
                        count++;
                        status = 1;
                    }
                    else
                    {
                        bufp = ebuf;
                        status = 0;
                        count = 0;
                    }
                    break;
                }
            }
            eoff = 0;
        }
        
        iResult = fat_get_next_cluster(sb, clunum, &clunum);
        if (iResult != _ERR_FAT_SUCCESS)
        {
            SVC_FSS_TRACE(DL_FATERROR, "fat_read_entries: fat_get_next_cluster() return %d", iResult);
            goto end;
        }
        secnum = 0;
    }
    
end:
    if(NULL != secbuf) 
    {
        FAT_SECT_BUF_FREE(secbuf);
    }

    *ecount = count;
    return iResult;
    
}
