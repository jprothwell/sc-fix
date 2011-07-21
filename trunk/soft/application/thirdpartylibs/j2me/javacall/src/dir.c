/*
 *
 * Copyright  1990-2009 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */


#ifdef __cplusplus
extern "C" {
#endif
#include "mmi_data_types.h"
#include "globaldefs.h"
#include "fat_fs.h"             /* mmi_public_drv */
#include "javacall_dir.h"
#include "mmi_trace.h"
#include "ucs2prot.h"


static UINT16 filename[JAVACALL_MAX_FILE_NAME_LENGTH];
static INT32 fhand;

/**
 * returns a handle to a file list. This handle can be used in
 * subsequent calls to javacall_dir_get_next() to iterate through
 * the file list and get the names of files that match the given string.
 * 
 * @param path the name of a directory, but it can be a
 *             partial file name
 * @param pathLen length of directory name or
 *        JAVACALL_UNKNOWN_LENGTH, which may be used for null terminated string 
 * @return pointer to an opaque filelist structure, that can be used in
 *         javacall_dir_get_next() and javacall_dir_close
 *         NULL returned on error, for example if root directory of the
 *         input 'string' cannot be found.
 */


javacall_handle javacall_dir_open(const javacall_utf16* path, int pathLen){
    FS_DOSDirEntry file_info;
	UINT16 namepath[JAVACALL_MAX_FILE_NAME_LENGTH];
    UINT8 ansiinamepath[JAVACALL_MAX_FILE_NAME_LENGTH];
    
    if( pathLen > JAVACALL_MAX_FILE_NAME_LENGTH ) {
 	    mmi_trace (1,"javacall_dir_open: Path length too long\n");
        return NULL;
    }

    memset(namepath, 0, JAVACALL_MAX_FILE_NAME_LENGTH * 2);
	UCS2Strcpy((S8*)namepath, (const S8*)path);
	UCS2Strcat((S8*)namepath, (const S8*)L"/*");
	UnicodeToAnsii((S8*)ansiinamepath, (S8*)namepath);
	mmi_trace(1,"javacall_dir_open PATH is %s,pathLen = %d \n",ansiinamepath,pathLen);
    fhand = MMI_FS_FindFirst((const UINT8*)namepath, 0, 0, &file_info,(UINT8*)filename,JAVACALL_MAX_FILE_NAME_LENGTH*2);
	mmi_trace(1,"javacall_dir_open fhd = %d \n",fhand);
    if(fhand < 0){
        return NULL;
    }
    return (javacall_handle)(&fhand);
}
    
/**
 * closes the specified file list. This handle will no longer be
 * associated with this file list.
 * @param handle pointer to opaque filelist struct returned by
 *               javacall_dir_open 
 */
void javacall_dir_close(javacall_handle handle){
    fhand = *((INT32 *)handle);
    INT32 ret;
    mmi_trace(1,"javacall_dir_close handle=%d\n",fhand);
    if(fhand >= 0){
        ret = MMI_FS_FindClose(fhand);
        mmi_trace(1,"javacall_dir_close ret=%d\n",ret);
    }
}
    
/**
 * return the next filename in directory path (UNICODE format)
 * The order is defined by the underlying file system.
 * 
 * On success, the resulting file will be copied to user supplied buffer.
 * The filename returned will omit the file's path
 * 
 * @param handle pointer to filelist struct returned by javacall_dir_open
 * @param outFilenameLength will be filled with number of chars written 
 * 
 * 
 * @return pointer to UNICODE string for next file on success, 0 otherwise
 * returned param is a pointer to platform specific memory block
 * platform MUST BE responsible for allocating and freeing it.
 */
javacall_utf16* javacall_dir_get_next(javacall_handle handle, int* /*OUT*/ outFilenameLength){
    FS_DOSDirEntry file_info;
    INT32 fhand = *((INT32 *)handle);
    INT32 ret;
    mmi_trace(1,"javacall_dir_get_next handle=%d\n",fhand);
    if(fhand < 0){
        return NULL;
    }

    ret = MMI_FS_FindNext(fhand, &file_info, (UINT8*)filename,JAVACALL_MAX_FILE_NAME_LENGTH*2);

    if(ret != FS_NO_ERROR){
        if (outFilenameLength) {
            *outFilenameLength = 0;
        }
        return NULL;
    }
    
    if (outFilenameLength) {
        *outFilenameLength = UCS2Strlen((const S8*)filename);
    }
    
    mmi_trace(1,"javacall_dir_get_next ret=%d len=%d\n",ret,*outFilenameLength);
    
    return ((javacall_utf16*)filename);
}
    
/**
 * Checks the size of free space in storage. 
 * @return size of free space
 */
javacall_int64 javacall_dir_get_free_space_for_java(void){
    return 0;
}
    
    
/**
 * Returns the root path of java's application directory.
 * 
 * @param rootPath returned value: pointer to unicode buffer, allocated 
 *        by the VM, to be filled with the root path.
 
 *                    OUT : lenght of set rootPath
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully
 *         <tt>JAVACALL_FAIL</tt> if an error occured
 */
javacall_result javacall_dir_get_root_path(javacall_utf16* /* OUT */ rootPath,
                                           int* /* IN | OUT */ rootPathLen){mmi_trace(1,"current function : %s\n",__FUNCTION__);
    int ret;
	ret = AnsiiToUnicodeString(rootPath, "D:/CSD_JVM/appdb");
	mmi_trace(1,"ret = %d\n",ret);
	*rootPathLen = ret;
    return JAVACALL_OK;
}  

/**
 * Returns the root path of java's configuration directory.
 * 
 * @param configPath returned value: pointer to unicode buffer, allocated 
 *        by the VM, to be filled with the root path.
 
 *                    OUT : lenght of set rootPath
 * @return <tt>JAVACALL_OK</tt> if operation completed successfully
 *         <tt>JAVACALL_FAIL</tt> if an error occured
 */
javacall_result javacall_dir_get_config_path(javacall_utf16* /* OUT */ configPath,
                                           int* /*IN | OUT*/ configPathLen){mmi_trace(1,"current function : %s\n",__FUNCTION__);
    int ret;
	ret = AnsiiToUnicodeString(configPath, "D:/CSD_JVM/lib");
	mmi_trace(1,"ret = %d\n",ret);
	*configPathLen = ret;
    return JAVACALL_OK;
}

/**
 *  Returns file separator character used by the underlying file system
 * (usually this function will return '\\';)
 * @return 16-bit Unicode encoded file separator
 */
javacall_utf16 javacall_get_file_separator(void){mmi_trace(1,"current function : %s\n",__FUNCTION__);
	javacall_utf16 tmp[2];
	int ret;
	//ret = AnsiiNToUnicodeString(tmp,"/",1);
	//mmi_trace(1,"ret = %d\n",ret);
    return '/';//tmp[0];
}
    
    
/**
 * Check if the given path is located on secure storage
 * The function should return JAVACALL_TRUE only in the given path
 * is located on non-removable storage, and cannot be accessed by the 
 * user or overwritten by unsecure applications.
 * @return <tt>JAVACALL_TRUE</tt> if the given path is guaranteed to be on 
 *         secure storage
 *         <tt>JAVACALL_FALSE</tt> otherwise
 */
javacall_bool javacall_dir_is_secure_storage(javacall_utf16* classPath, int pathLen){mmi_trace(1,"current function : %s\n",__FUNCTION__);
    return JAVACALL_FALSE;
}

#ifdef __cplusplus
}
#endif

