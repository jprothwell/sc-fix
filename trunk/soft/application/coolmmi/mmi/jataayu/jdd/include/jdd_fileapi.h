/***************************************************************************
 * $Id: jdd_fileapi.h,v 1.5 2007/12/14 06:16:02 browserdevjtbase Exp $
 * $Revision: 1.5 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu") 
 * or Jataayu's suppliers as the case may be. The Software is protected by 
 * copyright, including without limitation by applicable copyright laws, 
 * international treaty provisions, other intellectual property laws and 
 * applicable laws in the country in which the Software is being used. 
 * You shall not modify, adapt or translate the Software, without prior 
 * express written consent from Jataayu. You shall not reverse engineer, 
 * decompile, disassemble or otherwise alter the Software, except and 
 * only to the extent that such activity is expressly permitted by 
 * applicable law notwithstanding this limitation. Unauthorized reproduction 
 * or redistribution of this program or any portion of it may result in severe 
 * civil and criminal penalties and will be prosecuted to the maximum extent 
 * possible under the law. Jataayu reserves all rights not expressly granted. 
 * 
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdd_fileapi.h,v $
 * Revision 1.5  2007/12/14 06:16:02  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.4  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.3  2006/06/20 13:28:37  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.6  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.4  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.3  2005/06/24 13:36:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.2  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/

#ifndef _JDD_FILEAPI_H
#define _JDD_FILEAPI_H

/** 
 * @file jdd_fileapi.h
 * @ingroup File
 * @brief Defines file abstraction interfaces. 
 *
 * All the file API's are assumed to be synchronous, when the call returns it is assumed 
 * that the operation attempted is completed and the result of the operation is available 
 * at that moment.
 */

/**
 * @brief This enumeration contains the different ways of opening the file mode.
 */
typedef enum e_File_Open_Mode
{
	E_OPEN_READ_MODE = 0x00, /**< Opens for reading. If file not exists then returns failure. */
	E_OPEN_READ_WRITE_MODE, /**< Opens for both reading and writing. If file not exists then returns failure */

	E_CREATE_WRITE_MODE, /**< Opens an empty file for writing and if the file exists, its contents are destroyed.  */
	E_CREATE_WRITE_READ_MODE, /**< Opens an empty file for both reading and writing. 
							   * If the file exists, its contents are destroyed */
	E_CREATE_APPEND_WRITE_MODE, /**< Opens for writing at the end of the file (appending).
								 * If the file does not exists, the file needs to be created */ 

	E_CREATE_APPEND_WRITE_READ_MODE /**< Opens for appending and reading. 
									 * If the file does not exists, the file needs to be created */
} EFileOpenMode ;
 
/**
 * @brief This enumeration contains the different seek positions.
 */
typedef enum e_File_Seek_Pos
{
	E_FILE_SEEK_SET = 0x00, /**< Seeks the file pointer to the position calculated using the offset and relative to the beginning position of file. */
	E_FILE_SEEK_CUR, /**< Seeks the file pointer to the position calculated using the offset and relative to the current position of file. */
	E_FILE_SEEK_END /**< Seeks the file pointer to the position calculated using the offset and relative to the end position of file. */
} EFileSeekMode ;

/**
 * @brief Some platforms require that the file system needs to be initialized before using 
 * and this initialized handle to be used in subsequent calls to the file system API's. 
 * So this handle specifies the file system in initialized/ready to use state.
 */
typedef void *		JDD_FSHANDLE ;

/** 
 * @brief This is the handle associated with the opened file
 */
typedef void *		JDD_FILE ;


#ifdef __cplusplus
extern "C" {
#endif

/** 
 * @brief The file system layer shall be initialized using this API call. 
 * @param[out] pvFileSysHandle Specifies the handle for the File API interface. This shall be used by the other file interface API calls.
 *
 * This function needs to be invoked before calling any of the file API calls 
 * specified in this document. If the platform does not require any initialization this 
 * call can return the success value. 
 * @see jdd_FSDeinitialize 
 */
JC_RETCODE	jdd_FSInitialize (JDD_FSHANDLE *pvFileSysHandle) ;

/**
 * @brief This API is used to deinitialize the file system handle. 
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * 
 * The File system layer shall be de-initialized using this call. After this the 
 * vfileSysHandle shall not be used by the other file interface API calls. 
 * The file system layer must be re-initialized before commencing any further operations
 * on file system.
 * @see jdd_FSInitialize
 */
JC_RETCODE  jdd_FSDeinitialize (JDD_FSHANDLE vFileSysHandle) ;

/**
 * @brief This API is used to open the file.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmFileName Specifies the name of the file, which needs to be opened
 * @param[in] eOpenMode Specifies the mode in which the file should be opened.
 * @return On success file reference for the opened file is returned, otherwise NULL.
 * This function is responsible for opening the specified file name for the specified mode. 
 * @see jdd_FSRead
 * @see jdd_FSWrite
 * @see jdd_FSClose
 */
JDD_FILE  jdd_FSOpen  (JDD_FSHANDLE vFileSysHandle, const JC_CHAR *pmFileName,
					   EFileOpenMode eOpenMode) ;
/**
 * @brief This API is used to close the opened file.
 * @param[in] vFileHandle Specifies the reference to the opened file, which is returned by #jdd_FSOpen
 */
JC_RETCODE jdd_FSClose (JDD_FILE vFileHandle) ;

/**
 * @brief This API is used to write the data to a opened file stream.
 * @param[in] pvWriteBuf Specifies the data which needs to be written
 * @param[in] uiItemSize Specifies the size of the item in bytes
 * @param[in] uiCount Specifies the number of items which needs to be written
 * @param[in] vFileHandle Specifies the reference to the opened file, which is returned by #jdd_FSOpen
 * @return Returns the number of full items actually written to the file. In case of error 
 * it should return -1.
 * This function writes the given number of items to the file and returns the number of items successfully written to file. On failure it returns -1.
 */
JC_INT32 jdd_FSWrite (const void * pvWriteBuf, const JC_UINT32 uiItemSize,
					  const JC_UINT32 uiCount, JDD_FILE vFileHandle) ;
/**
 * @brief This API is used to flushes the open file stream.
 * @param[in] vFileHandle Specifies the reference to the opened file, which is returned by #jdd_FSOpen
 */
JC_RETCODE jdd_FSFlush (JDD_FILE vFileHandle) ;
/**
 * @brief This API is used to read the data from the opened file stream.
 * @param[in] pvReadBuf Specifies the reference where the data needs to be read and stored.
 * @param[in] uiItemSize Specifies the size of the item in bytes
 * @param[in] uiCount Specifies the number of items which needs to be read
 * @param[in] vFileHandle Specifies the reference to the opened file, which is returned by #jdd_FSOpen
 * @return Returns the number of full items actually read from the file. In case of error 
 * it should return -1.
 * This function reads the required number of items from the file and returns the number of items successfully read from the file. 
 */
JC_INT32 jdd_FSRead (void *pvReadBuf, const JC_UINT32 uiItemSize,
					 const JC_UINT32 uiCount, JDD_FILE vFileHandle) ;
/**
 * @brief This API is used to move the file pointer to a specified location.
 * @param[in] vFileHandle Specifies the reference to the opened file, which is returned by #jdd_FSOpen
 * @param[in] iPos	Specifies the number of bytes from the origin.
 * @param[in] eSeekMode Specifies the seek mode.
 * @retval JC_ERR_FILE_SEEK Returns when there is an error in moving the file pointer.
 * This function sets the file position indicator of the specified file. 
 * The new position is calculated from the offset bytes to the position specified by eSeekMode. 
 */
JC_RETCODE jdd_FSSeek (JDD_FILE vFileHandle, const JC_INT32 iPos,
					   EFileSeekMode eSeekMode) ;
/**
 * @brief This API is used to obtain the current file size from the opened file stream.
 * @param[in] vFileHandle Specifies the reference to the opened file, which is returned by #jdd_FSOpen
 * @param[out] puiFileSize Returns the current size of the specified file
 * @note The #jdd_FSGetFileSize function is used to obtain the current file size without 
 *		 opening the file.
 */
JC_RETCODE jdd_FSGetSize (JDD_FILE vFileHandle, JC_UINT32 *puiFileSize) ;

/**
 * @brief This API is used to reposition the file pointer to the beginning of the file for the opened stream
 * @param[in] vFileHandle Specifies the reference to the opened file, which is returned by #jdd_FSOpen
 */
JC_RETCODE jdd_FSRewind (JDD_FILE vFileHandle) ;

/**
 * @brief This API specifies whether the given file name is exists.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmFileName Specifies the name of the file
 * @retval E_TRUE Specifies the file exists
 * @retval E_FALSE Specifies the file does not exists
 * @note The file name should cotain the full path.
 */
JC_BOOLEAN jdd_FSIsFileExist (JDD_FSHANDLE vFileSysHandle, JC_CHAR *pmFileName) ;

/** 
 * @brief This API is used to obtain the current file size.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmFileName Specifies the name of the file.
 * @param[out] puiFileSize Returns the current size of the specified file name.
 * @note The file name should cotain the full path. 
 * @note The #jdd_FSGetSize is used to get the size of the file for the opened stream, 
 *		  where as this function is used to get without opening the file.		  
 */
JC_RETCODE jdd_FSGetFileSize (JDD_FSHANDLE vFileSysHandle, JC_CHAR *pmFileName,
							  JC_UINT32 *puiFileSize) ;
/**
 * @brief This API is used to rename the file.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmSrcFileName Specifies the existing file name which needs to be renamed
 * @param[in] pmDestFileName Specifies the new name for the existing file. 
 * @retval JC_ERR_FILE_RENAME Specifies the error in renaming the file name.
 * 
 * This function renames the old file name specified to the new file name. 
 */
JC_RETCODE jdd_FSRename (JDD_FSHANDLE vFileSysHandle, const JC_CHAR *pmSrcFileName,
						 const JC_CHAR *pmDestFileName) ;
/**
 * @brief This API is used to delete the file.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmFileName Specifies the file name which needs to be deleted
 * @retval JC_ERR_FILE_DELETE Specifies the error in deleting the file.
 * 
 */
JC_RETCODE jdd_FSDelete (JDD_FSHANDLE vFileSysHandle, const JC_CHAR *pmFileName) ;
/**
 * @brief This API is used to create the new directory.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmDirName Specifies the directory name which needs to be created.
 *
 * This function creates a directory with the name given and sets the default permissions. 
 * This function shall not set the current directory path to the one  created.
 */
JC_RETCODE jdd_FSMakeDir (JDD_FSHANDLE vFileSysHandle, const JC_CHAR *pmDirName) ;
/** 
 * @brief This function sets the current directory path to the one specified. 
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmDirName Specifies the name of the directory which has to be made the current directory
 */
JC_RETCODE jdd_FSChDir (JDD_FSHANDLE vFileSysHandle, const JC_CHAR *pmDirName) ;

/**
 * @brief This function deletes the given directory.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmDirName Specifies the name of the directory which needs to be deleted.
 *
 * This function should delete all the files and subdirectory inside the given directory, if it exists.
 */
JC_RETCODE jdd_FSDelDir (JDD_FSHANDLE vFileSysHandle, const JC_CHAR *pmDirName) ;

/**
 * @brief This API is used to rename the directory.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmSrcDirName Specifies the existing directory name which needs to be renamed
 * @param[in] pmDestDirName Specifies the new name for the existing directory. 
 * @retval JC_ERR_DIR_RENAME Specifies the error in renaming the directory name.
 * 
 */
JC_RETCODE jdd_FSDirRename (JDD_FSHANDLE vFileSysHandle, const JC_CHAR *pmSrcDirName, 
							const JC_CHAR *pmDestDirName) ;
/**
 * @brief This API specifies whether the given directory name is exists.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmDirName Specifies the name of the directory
 * @retval E_TRUE Specifies the directory exists
 * @retval E_FALSE Specifies the directory does not exists
 */
JC_BOOLEAN jdd_FSIsDirExist (JDD_FSHANDLE vFileSysHandle, const JC_CHAR *pmDirName) ;

/* @fn jdd_FSGetRootDirectory 
 * @brief
 *    This function copies the root directory to pmRootDirectory.If the param pmRootDirectory 
 * is NULL or puiDirSize is lessthan the reqired size then it fills the puiDirSize with the 
 * reqired size and returns the error code JC_ERR_CFG_INSUFFICIENT_MEMORY.
 */
JC_RETCODE jdd_FSGetRootDirectory (JC_CHAR *pmRootDirectory, JC_UINT32 *puiDirSize) ;
/** 
 * @brief This function opens the specified directory and returns the number of directories and number of file(s) available. 
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] pmDirName Specifies the name of the directory, which needs to be opened.
 * @param[out] puiOpenDirID Returns the identifier for opening this directory path
 * @param[out] puiDirCount Returns the number of directory available in the specified directory.
 * @param[out] puiFileCount Returns the number of file available in the specified directory.
 */
JC_RETCODE jdd_FSOpenDir (JDD_FSHANDLE vFileSysHandle, JC_CHAR	*pmDirName, 
						  JC_UINT32	   *puiOpenDirID, JC_UINT32	*puiDirCount,
						  JC_UINT32	   *puiFileCount) ;

/** 
 * @brief This function returns the next entry in the directory list. It also specifies whether the retrieved entry is file or directory.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] uiOpenDirID Specifies the directory identifier that is returned by #jdd_FSOpenDir function.
 * @param[in] bFullPath Specifies whether the return value should be full path or only the name of next entry
 * @param[out] pmName Returns the name of the next entry.
 * @param[out] pbIsDirectory Returns whether the entry that is retrieved is file or directory.
 */
JC_RETCODE jdd_FSGetNextEntry (JDD_FSHANDLE vFileSysHandle, JC_UINT32 uiOpenDirID,
							   JC_BOOLEAN	bFullPath, JC_CHAR **pmName, 
							   JC_BOOLEAN	*pbIsDirectory) ;

/** 
 * @brief This function frees the reference to the open dir.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which is returned by the initialize function.
 * @param[in] uiOpenDirID Specifies the directory identifier that is returned by #jdd_FSOpenDir function.
 */
JC_RETCODE jdd_FSCloseDir (JDD_FSHANDLE	vFileSysHandle, JC_UINT32 uiOpenDirID) ; 

#ifdef __cplusplus
}
#endif

#endif

/* END OF FILE */


