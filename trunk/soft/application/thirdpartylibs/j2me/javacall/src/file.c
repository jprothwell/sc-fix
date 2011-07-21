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

#include "javacall_file.h"
#include "mmi_trace.h"
#include "fs_func.h"
#include "fs.h"
/**
 * PLEASE NOTE:
 * API descriptions in the header file SUPERCEDE the descriptions
 * herein.
 */

/**
 * Initializes the File System
 * @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> or negative value on error
 */
javacall_result javacall_file_init(void){mmi_trace(1,"current function : %s\n",__FUNCTION__);
    return JAVACALL_OK;
}
/**
 * Cleans up resources used by file system
 * @return <tt>JAVACALL_OK</tt> on success, <tt>JAVACALL_FAIL</tt> or negative value on error
 */ javacall_result javacall_file_finalize(void){mmi_trace(1,"current function : %s\n",__FUNCTION__);
    return JAVACALL_OK;
}

/**
 * Open a file
 * @param unicodeFileName fully-qualified name (including path) of file to be 
 *        opened, in UNICODE (utf16)
 * @param fileNameLen length of the file name or
 *        JAVACALL_UNKNOWN_LENGTH, which may be used for null terminated string 
 * @param flags open control flags
 *        Applications must specify exactly one of the first three
 *        values (file access modes) below in the value of "flags"
 *        JAVACALL_FILE_O_RDONLY, 
 *        JAVACALL_FILE_O_WRONLY, 
 *        JAVACALL_FILE_O_RDWR
 *
 *        And any combination (bitwise-inclusive-OR) of the following:
 *        JAVACALL_FILE_O_CREAT, 
 *        JAVACALL_FILE_O_TRUNC, 
 *        JAVACALL_FILE_O_APPEND,
 *
 * @param handle address of pointer to file identifier
 *        on successful completion, file identifier is returned in this 
 *        argument. This identifier is platform specific and is opaque
 *        to the caller.  
 * @return <tt>JAVACALL_OK</tt> on success, 
 *         <tt>JAVACALL_FAIL</tt> or negative value on error
 * 
 */
unsigned char sxs_SerialFlush (void);

javacall_result javacall_file_open(const javacall_utf16 * unicodeFileName, int fileNameLen, int flags, /*OUT*/ javacall_handle * handle){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	int fhandle;
	S8 namepath[100];
	int newflags=0;
	//asm("break 1");
	UnicodeToAnsii(namepath, unicodeFileName);
	//mmi_trace(1,"file name is %s,FLAGS = 0X%x \n",namepath,flags);
/*	
	if (strstr(namepath,"FFFFFFFF"))
	{
		sxs_SerialFlush ();
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");asm("nop");
		asm("nop");
		
		asm("break 1");
	}
	*/
	if ( flags == JAVACALL_FILE_O_RDONLY )
	{
	    newflags |= FS_READ_ONLY;
	}
	else if (( flags & JAVACALL_FILE_O_RDWR ) && ( flags & JAVACALL_FILE_O_CREAT )) 
	{
	    newflags |= (FS_READ_WRITE | FS_CREATE );
	}
	else if ( flags & JAVACALL_FILE_O_TRUNC )
	{
	    newflags |= FS_CREATE_ALWAYS;
	}
	 else if ( flags & JAVACALL_FILE_O_RDWR )
	{
	    newflags |= FS_READ_WRITE;
	}
	else if ( flags & JAVACALL_FILE_O_CREAT )
	{
	    newflags |= FS_CREATE;
	}
	else if ( flags & JAVACALL_FILE_O_WRONLY )
	{
	    newflags |= FS_READ_WRITE;
	}
	else if ( flags & JAVACALL_FILE_O_APPEND )
	{
	    newflags |= FS_READ_WRITE;
	}
	fhandle = MMI_FS_Open(unicodeFileName, newflags);
	mmi_trace(1,"open filehandle=%d, filename=%s, FLAGS = 0x%x newflags=0x%x\n",fhandle,namepath,flags,newflags);
	if (fhandle < 0)
    {
        *handle = NULL;
    	return JAVACALL_FAIL;
    }

	*handle = fhandle; 
	return JAVACALL_OK;
}

/**
 * Closes the file with the specified handle
 * @param handle handle of file to be closed
 * @return <tt>JAVACALL_OK</tt> on success, 
 *         <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
javacall_result javacall_file_close(javacall_handle handle){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	int ret;
	ret = MMI_FS_Close(handle);
	if (ret < 0)
    {   mmi_trace(1,"!!!current function fail: %s\n",__FUNCTION__);
    		return JAVACALL_FAIL;
     }
	return JAVACALL_OK;
}


/**
 * Reads a specified number of bytes from a file.
 * @param handle handle of file 
 * @param buf buffer to which data is read
 * @param size number of bytes to be read. Actual number of bytes
 *              read may be less, if an end-of-file is encountered
 * @return the number of bytes actually read
 */
long javacall_file_read(javacall_handle handle, unsigned char *buf, long size){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	long count;
	int ret;
	ret = MMI_FS_Read(handle, buf, size, &count);
	//mmi_trace(1," %s , count = %d,return ret is %d\n",__FUNCTION__,count,ret);

    if (ret < 0)
    {   mmi_trace(1,"!!!current function fail: %s\n",__FUNCTION__);
    		return JAVACALL_FAIL;
    }
    return count;
}

/**
 * Writes bytes to file.
 * @param handle handle of file 
 * @param buf buffer to be written
 * @param size number of bytes to write
 * @return the number of bytes actually written. This is normally the same 
 *         as size, but might be less (for example, if the persistent storage being 
 *         written to fills up).
 */
long javacall_file_write(javacall_handle handle, const unsigned char *buf, long size){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	long count;
	int ret;
	ret = MMI_FS_Write(handle, buf, size, &count);
	//mmi_trace(1," %s ,return ret is %d\n",__FUNCTION__,ret);
    if (ret < 0)
    {   mmi_trace(1,"!!!current function fail: %s\n",__FUNCTION__);
    		return JAVACALL_FAIL;
    }
    return count;
}

/**
 * Deletes a file from the persistent storage.
 * @param unicodeFileName fully-qualified name (including path) of file to be 
 *         deleted, in UNICODE (utf16)
 * @param fileNameLen length of the file name or
 *        JAVACALL_UNKNOWN_LENGTH, which may be used for null terminated string 
 * @return <tt>JAVACALL_OK</tt> on success, 
 *         <tt>JAVACALL_FAIL</tt> or negative value on error
 */
javacall_result javacall_file_delete(const javacall_utf16 * unicodeFileName, int fileNameLen){mmi_trace(1,"current function : %s\n",__FUNCTION__);
	int ret;
	ret = MMI_FS_Delete(unicodeFileName);
	if (ret < 0)
    {   mmi_trace(1,"!!!current function fail: %s ret=%d\n",__FUNCTION__,ret);
    		return JAVACALL_FAIL;
    }
	return JAVACALL_OK;
}

/**
 * The  truncate function is used to truncate the size of an open file in 
 * the filesystem storage.
 * For CDC-HI - based implementations, it is required that this function also
 * be capable of enlarging a file (standard unix truncate() functionality).
 * For CLDC-HI - based implementations, this function need not have the 
 * capability to enlarge a file.
 * @param handle identifier of file to be truncated
 *         This is the identifier returned by javacall_file_open()
 * @param size size to truncate to
 * @return <tt>JAVACALL_OK</tt> on success, 
 *         <tt>JAVACALL_FAIL</tt> or negative value on error
 */
javacall_result javacall_file_truncate(javacall_handle handle, javacall_int64 size){mmi_trace(1,"current function : %s\n",__FUNCTION__);
	int ret;
	ret = FS_ChangeSize(handle,size);
	if (ret<0)
    {   
        mmi_trace(1,"!!!current function fail: %s ret=%d\n",__FUNCTION__,ret);
    	return JAVACALL_FAIL;
    }
	return JAVACALL_OK;
}

/**
 * Sets the file pointer associated with a file identifier 
 * @param handle identifier of file
 *               This is the identifier returned by javacall_file_open()
 * @param offset number of bytes to offset file position by
 * @param flag controls from where offset is applied, from 
 *                 the beginning, current position or the end
 *                 Can be one of JAVACALL_FILE_SEEK_CUR, JAVACALL_FILE_SEEK_SET 
 *                 or JAVACALL_FILE_SEEK_END
 * @return on success the actual resulting offset from beginning of file
 *         is returned, otherwise -1 is returned
 */
javacall_int64 javacall_file_seek(javacall_handle handle, javacall_int64 offset, javacall_file_seek_flags flag){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	int ret;
	ret = MMI_FS_Seek(handle, offset, flag);
	//mmi_trace(1,"file %s ,return ret is %d\n",__FUNCTION__,ret);
	if(ret < 0)
    {
        mmi_trace(1,"javacall_file_seek fail");
    	return JAVACALL_FAIL;
    }

    if(flag == JAVACALL_FILE_SEEK_SET && ret < offset)
    {
        mmi_trace(1,"javacall_file_seek FS_ChangeSize");
        ret = FS_ChangeSize(handle, offset);
        
        if(ret < 0)
        {
            mmi_trace(1,"javacall_file_seek FS_ChangeSize fail");
            return JAVACALL_FAIL;
        }
        
        ret = MMI_FS_Seek(handle, offset, JAVACALL_FILE_SEEK_SET);

        if(ret < 0)
        {
            mmi_trace(1,"javacall_file_seek FS_ChangeSize seek fail");
            return JAVACALL_FAIL;
        }

        return ret;
    }
    else
    {
        //TODO:impl change size when not on JAVACALL_FILE_SEEK_SET condition
        return ret;
    }

}


/**
 * Get file size 
 * @param handle identifier of file
 *               This is the identifier returned by pcsl_file_open()
 * @return size of file in bytes if successful, -1 otherwise
 */
javacall_int64 javacall_file_sizeofopenfile(javacall_handle handle){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	int ret;
	unsigned int size;
	ret = MMI_FS_GetFileSize(handle, &size);
	//mmi_trace(1,"file %s ,size %d,return ret is %d\n",__FUNCTION__,size,ret);
	if (ret <0)
    {   mmi_trace(1,"!!!current function fail: %s\n",__FUNCTION__);
            return JAVACALL_FAIL;
    }
	return size;
}

/**
 * Get file size
 * @param fileName fully-qualified name (including path) of file
 *         in UNICODE (utf16)
 * @param fileNameLen length of the file name or
 *        JAVACALL_UNKNOWN_LENGTH, which may be used for null terminated string 
 * @return size of file in bytes if successful, -1 otherwise 
 */
javacall_int64 javacall_file_sizeof(const javacall_utf16 * fileName, int fileNameLen){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
   FS_HANDLE ret;
   int ret2;
   int filesize;
   S8 namepath[100];
   ret = MMI_FS_Open(fileName, FS_READ_ONLY);
   if (ret >=0)
   {
	ret2 = MMI_FS_GetFileSize(ret, &filesize);
	UnicodeToAnsii(namepath, fileName);
	mmi_trace(1,"file %s ,file name %s, size %d,return ret is %d\n",__FUNCTION__,namepath,filesize,ret2);
    MMI_FS_Close(ret);
	if (ret2 <0)
    {
        mmi_trace(1,"!!!current function fail: %s getsizefail\n",__FUNCTION__);
    	return JAVACALL_FAIL;
    }
	else
    {   
	    return filesize;
    }
   }
   else
    {
    mmi_trace(1,"!!!current function fail: %s openfail\n",__FUNCTION__);
    return JAVACALL_FAIL;
    }
}

/**
 * Check if a regular file exists in file system storage.
 *
 * @param fileName fully-qualified name (including path) of file
 *         in UNICODE (utf16)
 * @param fileNameLen length of the file name or
 *        JAVACALL_UNKNOWN_LENGTH, which may be used for null terminated string 
 * @return <tt>JAVACALL_OK </tt> if it exists and is a regular file, 
 *         <tt>JAVACALL_FAIL</tt> otherwise (e.g., if the file is a directory, or does not exist)
 */
javacall_result javacall_file_exist(const javacall_utf16 * fileName, int fileNameLen){//mmi_trace(1,"current function : %s\n",__FUNCTION__);
	S8 namepath[100];
	int newflags=0;
	FS_HANDLE ret;
	//asm("break 1");
	UnicodeToAnsii(namepath, fileName);
	mmi_trace(1,"javacall_file_exist fileName=%s\n",namepath);
	ret = MMI_FS_Open(fileName, FS_READ_ONLY);
	if ( ret >=0)
	{
		MMI_FS_Close(ret);
		return JAVACALL_OK;
	}
	else 
	{
		mmi_trace(1,"javacall_file_exist retrun hand = %d\n",ret);
		return JAVACALL_FAIL;
	}
}


/** Force the data to be written into the file system storage
 * @param handle identifier of file
 *               This is the identifier returned by javacall_file_open()
 * @return JAVACALL_OK  on success, <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
javacall_result javacall_file_flush(javacall_handle handle){mmi_trace(1,"current function : %s\n",__FUNCTION__);
	int ret;
	ret = MMI_FS_Flush(handle);
	if (ret == 0)
		return JAVACALL_OK;
	else
	{
		mmi_trace(1,"javacall_file_flush return ret=%d\n",ret);
		return JAVACALL_FAIL;
	}
}

/**
 * Renames the filename. 
 * If the underlying operating system API can "rename" from one arbitrary 
 * path to another, this behavior is preferable. If not, and if the filename 
 * parameters have different paths, a value of JAVACALL_FAIL, should be
 * returned.
 * @param unicodeOldFilename current fully-qualified name (including path) of 
 *         file in UNICODE (utf16)
 * @param oldNameLen current name length or
 *        JAVACALL_UNKNOWN_LENGTH, which may be used for null terminated string
 * @param unicodeNewFilename new fully-qualified name (including path) of 
 *         file in UNICODE (utf16)
 * @param newNameLen length of new name or
 *        JAVACALL_UNKNOWN_LENGTH, which may be used for null terminated string 
 * @return <tt>JAVACALL_OK</tt>  on success, 
 *         <tt>JAVACALL_FAIL</tt> or negative value otherwise
 */
javacall_result javacall_file_rename(const javacall_utf16 * unicodeOldFilename, int oldNameLen, const javacall_utf16 * unicodeNewFilename, int newNameLen){mmi_trace(1,"current function : %s\n",__FUNCTION__);
	int ret;
	S8 namepath[100];
	//asm("break 1");
	UnicodeToAnsii(namepath, unicodeOldFilename);
	mmi_trace(1,"javacall_file_rename old name is %s,oldNameLen = %d \n",namepath,oldNameLen);
	UnicodeToAnsii(namepath, unicodeNewFilename);
	mmi_trace(1,"javacall_file_rename new name is %s,newNameLen = %d \n",namepath,newNameLen);
	ret = MMI_FS_Rename(unicodeOldFilename, unicodeNewFilename);
	if (ret <0)
	{
	mmi_trace(1,"javacall_file_rename ret = %d \n",ret);
		return JAVACALL_FAIL;
	}
	return JAVACALL_OK;
}


#ifdef __cplusplus
}
#endif
