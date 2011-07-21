#define MODULE_NAME "JDD_FILE"
#define FILE_NAME "jdd_fileapi.c"
 /***************************************************************************
 *
 * File Name jdd_fileapi.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
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
 ***************************************************************************/
 
 /** 
 * @file	jdd_fileapi.c
 * @ingroup	jdd_fileapi
 * @brief	Describes the file related API's.
 **/
 
#ifdef JATAAYU_SUPPORT 
 /***************************************************************************
 * System Include Files
 **************************************************************************/
#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#else
#include "cswtype.h"
#endif
#include "fs.h"
#include "errorcode.h"
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/
#include <ddl.h>
#include <jcal.h>
#include "jdi_cutils_tcsstr.h"
#include "jwap_mms_common.h"
#include "fat_fs.h"


 /***************************************************************************
 * Macros
 **************************************************************************/
#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					FILE_MODULE
#else
#define __MODULE_ID__					0
#endif
 
/* 
  * By default, logs are mapped to JDD Logs. If file is used for logging,
  * then JDD_LOG SHOULD NOT be used. 
  */
#if 1
 #define PLATFORM_LOG0(x)
 #define PLATFORM_LOG1(x, y)
#endif//0
#define MAX_FILE_DISCRIPTOR		FS_NR_OPEN_FILES_MAX //Maximum value of ifd 
 /***************************************************************************
 * Global Variable declarations and definitions
 **************************************************************************/
 
 /***************************************************************************
 * Local Function Prototypes
 **************************************************************************/
 
 
 /***************************************************************************
 * All Local Function Definitions
 **************************************************************************/

static JC_CHAR* MapFileNameToDrive(const JC_CHAR *pmFilePath)
{
 	JC_CHAR *pmFileName = JC_NULL;

	 if ( (*pmFilePath == 'c' || *pmFilePath == 'C') &&
		   (*(pmFilePath +1) == ':') &&
		   ((*(pmFilePath + 2) == '/') || (*(pmFilePath + 2)== '\\')))
	 {
	 	pmFileName = jdd_MemAlloc(jc_wstrlen(pmFilePath)+1 , sizeof(JC_CHAR));
	 	jc_wstrcpy(pmFileName, pmFilePath+2);
	 }
	 else if ( (*pmFilePath == 'D' || *pmFilePath == 'd') &&
			   (*(pmFilePath +1) == ':') &&
			   ((*(pmFilePath + 2) == '/') || (*(pmFilePath + 2)== '\\')))	 
	 {
		pmFileName = jdd_MemAlloc(jc_wstrlen(pmFilePath)+1, sizeof(JC_CHAR));
	 	jc_wstrcpy(pmFileName, pmFilePath);
		*pmFileName = '/' ;
		 *(pmFileName+1) = 't' ;
		 *(pmFileName+2) = '/' ;
		
	 }
	 else if ( (*pmFilePath == 'e' || *pmFilePath == 'E') &&
		   (*(pmFilePath +1) == ':') &&
		   ((*(pmFilePath + 2) == '/') || (*(pmFilePath + 2)== '\\')))
	 {
	 	pmFileName = jdd_MemAlloc(jc_wstrlen(pmFilePath) + strlen("/home")+ 1, sizeof(JC_CHAR));
	 	jdi_CUtilsTcsStrCpy("/home", pmFileName);
		jc_wstrcat(pmFileName, pmFilePath+2);
	 }
	  return pmFileName;
}

 /**
 * @brief 			This is a utility function. This function is used to 
 *					convert all forward slashes to back slashes.
 * @param [in, out] *pFilePath Specifies the name/path of the file
 */
#if 0
static void convertFwdSlashToBackSlash (JC_INT8 *pFilePath)
{
	 
	jdd_AssertOnFail ((JC_NULL != pFilePath), \
				"jdd_FSOpen: pmFileName is NULL") ;

		 while ('\0' != *pFilePath)
		 {
			 if ('/' == *pFilePath)
			 {	 
				 *pFilePath = '\\' ;				 
			 }
			 pFilePath++ ;
		 }
	
	return ;
 }

/**
 * @brief 			This is a utility function. This function is used to 
 *					convert all back slashes to forward slashes.
 * @param [in, out] *pFilePath Specifies the name/path of the file 
 */
static void convertBackSlashToFwdSlash (JC_INT8 *pFilePath)
{
	jdd_AssertOnFail ((JC_NULL != pFilePath), \
				"jdd_FSOpen: pmFileName is NULL") ;
	 
		 while ('\0' != *pFilePath)
		 {
			 if ('\\' == *pFilePath)
			 {	
				 *pFilePath = '/' ;				 
			 }
			 pFilePath++ ;
		 }
}
#endif //0 
 

/**
 * @brief 		This function converts platform return to JC_RETCODE
 * @param[in]	uiErrorStatus PlateForm specific Error code
 * @retval		Error code of type JC_RETCODE
 *
 * Converts PlatForm specific Error code to corresponding JC_RETCODE error
 */
#if 0
static JC_RETCODE getJCFError (JC_INT32	iErrorStatus)
{
	JC_RETCODE rCode ;
	
	switch(uiErrorStatus)
	{
		/* Replace <platformErrVal> with Platform specific error values */
		/*
		case <platformErrVal>:
			rCode = JC_ERR_FILE_CREATEDIR ;
			break ;

		case <platformErrVal>:
			rCode  = JC_ERR_FILE_DIR_ALREADY_EXIST ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_CHANGEDIR ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_RENAME ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_WRITE ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_FLUSH ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_OPEN ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_CLOSE ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_SIZE ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_DELETE ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_SEEK ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_READ ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_SYS_INIT ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_SYS_DEINIT ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_DIR_DELETE ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_DIR_RENAME ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_NOT_FOUND ;
			break ;

		case <platformErrVal>:
			rCode = JC_ERR_FILE_EOF ;
			break ;		
		*/
		default :
			rCode = JC_ERR_UNKNOWN ;
	}	

	return (rCode) ;
}
#endif

 /***************************************************************************
 * All Global Function Definitions
 **************************************************************************/

/** 
 * @ingroup		jdd_fileapi
 * @brief 		This function initializes the file system. 
 * @param[out]	pvFileSysHandle Specifies the handle for the File API interface.
 *				This shall be used by the other file interface API calls.
 * @retval		JC_OK If File System layer is initialized successfully
 * @retval		JC_ERR_FILE_SYS_INIT If the file system cannot be initialized
 *
 * This function needs to be invoked before calling any of the file API calls 
 * specified in this document. If the platform does not require any 
 * initialization this call can return the success value. 
 * @see 		#jdd_FSDeinitialize 
 */

JC_RETCODE	jdd_FSInitialize (JDD_FSHANDLE *pvFileSysHandle) 
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	PLATFORM_LOG0 ("jdd_FSInitialize: Entry") ;

	/*--- Platform Specific Implementation Start ---*/
	rCode = JC_OK ;

	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = getJCFError (<platformErrValue>) ;
		PLATFORM_LOG1 ("jdd_FSInitialize: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0 ("jdd_FSInitialize: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_fileapi
 * @brief 		This function De-Initializes the File System Layer
 * @param[in]	pvFileSysHandle Specifies the handle for the file system,
 *				which is returned by #jdd_FSInitialize
 * @retval		JC_OK If File System layer is de-initialized successfully
 * @retval		JC_ERR_FILE_SYS_DEINIT If the file system cannot 
 *				be de-initialized
 *
 * The File system layer shall be de-initialized using this call. 
 * After this the pvFileSysHandle shall not be used by the other 
 * file interface API calls. The file system layer must be re-initialized
 * before commencing any further operations on file system.
 * @see 		#jdd_FSInitialize
 */

JC_RETCODE	jdd_FSDeinitialize (JDD_FSHANDLE pvFileSysHandle)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PLATFORM_LOG0 ("jdd_FSDeInitialize: Entry") ;

	/*--- Platform Specific Implementation Start ---*/
	rCode = JC_OK ;

	/*
	if ((Platform API succeeds))
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode = getJCFError (<platformErrValue>) ;
		PLATFORM_LOG1 ("jdd_FSDeInitialize: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/
	
	PLATFORM_LOG0 ("jdd_FSDeInitialize: Exit") ;
	return (rCode) ;
}

/**
 * @ingroup		jdd_fileapi
 * @brief 		This function is used to open the file.
 * @param[in]	vFileSysHandle Specifies the handle for the file system, 
 *				which is returned by #jdd_FSInitialize.
 * @param[in]	pmFileName Specifies the name of the file, which needs
 *				to be opened
 * @param[in]	eOpenMode Specifies the mode in which the file should be opened
 * @retval		On success file reference for the opened file is returned, 
 *				otherwise JC_NULL.
 *
 *	This function is responsible for opening the specified file name for 
 *	the specified mode. 
 * @see 		#jdd_FSRead
 * @see 		#jdd_FSWrite
 * @see 		#jdd_FSClose
 */

JDD_FILE  jdd_FSOpen  (JDD_FSHANDLE vFileSysHandle, \
					   const JC_CHAR *pmFileName, \
					   EFileOpenMode eOpenMode) 
{   
	JC_INT32 iFlag = 0;
	JC_INT32 iFd =-1;
	JC_CHAR* pmFName = JC_NULL;
	/*--- Platform Specific Variables ---*/
	
	JDD_LOG_INFO2("jdd_FSOpen: File Name %s Mode %d",pmFileName,eOpenMode);	
	jdd_AssertOnFail ((JC_NULL != pmFileName), "jdd_FSOpen: pmFileName is NULL") ;
	
	/*--- Platform Specific Implementation Start ---*/
	if (JC_NULL == pmFileName)
	{
		mmi_trace (1, "JDD_LOG: File Open for NULL file") ;
		return JC_NULL ;
	}
	pmFName = MapFileNameToDrive(pmFileName) ;
	
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;
	/*Please check if platform Function requires File System Handle */
	
	switch (eOpenMode)
	{
	case E_OPEN_READ_MODE :
		/* Set File Open Mode to read only */
		iFlag = FS_O_RDONLY ;
		break ;
		
	case E_OPEN_READ_WRITE_MODE :
		/* Set File Open Mode to read-write */
		iFlag = FS_O_RDWR ;
		break ;
		
	case E_CREATE_WRITE_MODE :
		/* Set File Open Mode to write mode */
		iFlag = FS_O_CREAT | FS_O_WRONLY | FS_O_TRUNC ;
		break ;
		
	case E_CREATE_WRITE_READ_MODE :
		/* Set File Open Mode to write-read mode */
		iFlag = FS_O_CREAT | FS_O_RDWR | FS_O_TRUNC ;				
		break ;
		
	case E_CREATE_APPEND_WRITE_MODE :
		/* Set File Open Mode to write mode */
		iFlag = FS_O_APPEND | FS_O_WRONLY | FS_O_CREAT ;				
		break ;
		
	case E_CREATE_APPEND_WRITE_READ_MODE :
		/* Set File Open Mode to write-read mode */
		iFlag = FS_O_APPEND | FS_O_RDWR | FS_O_CREAT ;
		break ;
		
	default :
		jdd_AssertOnFail (0, "jdd_FSOpen: EFileOpenMode is default") ;
		break ;
	}
	if(pmFName)
	{
		iFd = FS_Open ((PCSTR)pmFName, iFlag, 0);/*Last Parameter is not used*/
	}
	else
	{
		iFd = FS_Open ((PCSTR)pmFileName, iFlag, 0);/*Last Parameter is not used*/
	}
	{
		JC_INT8	pcFName [52] = {0, } ;
		INT32		Ret ;
		FS_INFO	psFSInfo ;

		Ret = FS_GetFSInfo(CS_FS_MMS_IDENTIFIER,&psFSInfo) ;
		if (Ret != ERR_SUCCESS)
		{
			psFSInfo.iTotalSize = psFSInfo.iUsedSize = 0 ;
		}			
		jdi_CUtilsStrTcsNCpy (pmFileName, 50, pcFName) ;
		mmi_trace (1, "JDD_LOG: File Open for [%s] returned [%d], WAP_MMS Total and Available FS Mem are [%d] and [%d]",
			pcFName, (UINT32)psFSInfo.iTotalSize, (UINT32)psFSInfo.iTotalSize - (UINT32)psFSInfo.iUsedSize,
			iFd == 0?(MAX_FILE_DISCRIPTOR + 1):iFd) ;
	}
	if(iFd < 0)
	{
		mmi_trace (1, "JDD_LOG: File Open Error[%d] @@@",iFd) ;
		PLATFORM_LOG1 ("jdd_FSOpen: Error:[%d]",iFd) ;

		iFd = 0 ;
	}
	else if(0 == iFd)
	{
		
		iFd = MAX_FILE_DISCRIPTOR +1 ;
	}
	mmi_trace (1, "JDD_LOG: File Open iFd is %d @@@",iFd) ;		
	/*vFileHandle = PlatForm Specific Function for File open */
	
	/*--- Platform Specific Implementation End ---*/
	
	PLATFORM_LOG1 ("jdd_FSOpen: Exit:[%d]",iFd) ;
	jdd_MemFree(pmFName);
	return (JDD_FILE)(iFd) ;
}

 
/**
 * @ingroup		jdd_fileapi
 * @brief 		This API is used to close the opened file.
 * @param[in]	vFileHandle Specifies the reference to the opened file, 
 *				which is returned by #jdd_FSOpen
 * @retval		JC_OK If Success
 * @retval		JC_ERR_FILE_CLOSE If file cannot be closed
 */

JC_RETCODE jdd_FSClose (JDD_FILE vFileHandle) 
{
	JC_RETCODE rCode ;
	JC_INT32 rVal ;
	/*--- Platform Specific Variables ---*/
	/*--------------------------------------*/

	PLATFORM_LOG0 ("jdd_FSClose: Entry") ;
	jdd_AssertOnFail ((0 <= vFileHandle), \
		                  "jdd_FSClose: vFileHandle is NULL") ;

	/*--- Platform Specific Implementation Start ---*/

	/* PlatForm Specific Function for File Close */
	if(MAX_FILE_DISCRIPTOR +1 == (JC_INT32)vFileHandle)
	{
		vFileHandle = 0;		
	}
	rVal = FS_Close ((JC_INT32)vFileHandle);
	
	if (ERR_SUCCESS == rVal)
	{
			rCode = JC_OK ;
	}
	else
	{
		 rCode = JC_ERR_FILE_CLOSE ;
	      // rCode = getJCFError (rVal) ; //Log error 
		   PLATFORM_LOG1 ("jdd_FSClose: Returns [%d]", rCode) ;
	}
	
	/*--- Platform Specific Implementation End ---*/
	PLATFORM_LOG0 ("jdd_FSClose: Exit") ;
	mmi_trace (1, "JDD_LOG: jdd_FSClose for file handle [%d] returned [%X],rVal is %d", (JC_INT32)vFileHandle, -rCode,rVal) ;
	return (rCode) ;
}


/**
 * @ingroup		jdd_fileapi
 * @brief 		This API is used to write the data to a opened file stream.
 * @param[in] 	pvWriteBuf Specifies the data which needs to be written
 * @param[in] 	uiItemSize Specifies the size of the item in bytes
 * @param[in] 	uiCount Specifies the number of items which needs to be written
 * @param[in] 	vFileHandle Specifies the reference to the opened file, 
 *				which is returned by #jdd_FSOpen
 * @return		Returns the number of full items actually written to the file
 * @return		returns -1 In case of failure
 *
 * 	This function writes the given number of items to the file and 
 *	returns the numberof items successfully written to file. 
 *	If platform write api expects size in bytes rather than items, 
 *	item count mulitplied with itemsize can be used. 
 */

JC_INT32 jdd_FSWrite (const void * pvWriteBuf, \
					  const JC_UINT32 uiItemSize, \
					  const JC_UINT32 uiCount, \
					  JDD_FILE vFileHandle) 
{
	JC_INT32	iWrittenSize ;
	JC_INT32	iWriteSize ;
	JC_INT32	iBytesWritten = 0;
	/*--- Platform Specific Variables ---*/
	/*--------------------------------------*/
	INT32		Ret ;
	FS_INFO	psFSInfo ;

	Ret = FS_GetFSInfo(CS_FS_MMS_IDENTIFIER,&psFSInfo) ;
	if (Ret != ERR_SUCCESS)
	{
		psFSInfo.iTotalSize = psFSInfo.iUsedSize = 0 ;
	}			
	mmi_trace (1, "JDD_LOG: jdd_FSWrite >> WAP_MMS Total and Available FS Mem are [%d] and [%d]",
			(UINT32)psFSInfo.iTotalSize, (UINT32)psFSInfo.iTotalSize - (UINT32)psFSInfo.iUsedSize) ;
	PLATFORM_LOG0 ("jdd_FSWrite: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pvWriteBuf), \
		                  "jdd_FSWrite: pvWriteBuf is NULL") ;
	jdd_AssertOnFail ((0 != uiCount && 0 != uiItemSize), \
		                  "jdd_FSWrite: Write with 0 size") ;
	
	if(MAX_FILE_DISCRIPTOR +1 == (JC_INT32)vFileHandle)
	{
		vFileHandle = 0;		
	}


	/* If platform write () expects bytes */
	iWriteSize = uiItemSize * uiCount ;

	iBytesWritten = FS_Write (((JC_INT32)vFileHandle), (JC_UINT8 *)pvWriteBuf, iWriteSize);
	if(iBytesWritten == iWriteSize)
	{
		iWrittenSize = uiCount ;
	}
	else
	{
		iWrittenSize = -1 ;
		mmi_trace(1,"jdd_FSWrite returnval[%d]writesize[%d]",iBytesWritten,iWriteSize);
		//getJCFError (iBytesWritten) ; //Log error 
	}
	
	/* 
	 * If platform write () can handle items, uiItemSize and uiCount 
	 * can be used directly
	 */

	/* Platform API to write iWriteSize and returns iWrittenSize */ ;
	/*
	if (Platform API fails)
	{
		iWrittenSize = -1 ;
	}
	*/
	
	/* 
	 * If platform write () returns bytes written to file successfully, 
	 * iWrittenSize is calculated as mentioned in else statement below. 
	 * If Platform can directly write items to the file, comment "else" part
	 */
	 
	/*
	else
	{
		iWrittenSize = iWrittenSize / uiItemSize ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0 ("jdd_FSWrite: Exit") ;
	return (iWrittenSize) ;
}


/**
 * @ingroup		jdd_fileapi
 * @brief 		This function is used to flush the open file stream.
 * @param[in]	vFileHandle Specifies the reference to the opened file,
 *				which is returned by #jdd_FSOpen
 * @retval		JC_OK If File Flush was successful 
 * @retval		JC_ERR_FILE_FLUSH  If File Flush was unsuccessful
 */

JC_RETCODE jdd_FSFlush (JDD_FILE vFileHandle)
{
/*	JC_RETCODE rCode ;
	JC_INT32 rVal ; 
	jdd_AssertOnFail ((JC_NULL != vFileHandle), \
		                 "jdd_FSFlush: File Handle is NULL") ;

	if(MAX_FILE_DISCRIPTOR +1 == (JC_INT32)vFileHandle)
	{
		vFileHandle = 0;		
	}
	rVal = FS_Flush ((JC_INT32)vFileHandle) ;			
	if (ERR_SUCCESS == rVal)
	 {
			rCode = JC_OK ;
	 }
	 else
	 {
		rCode = JC_ERR_FILE_FLUSH ;
	   PLATFORM_LOG1 ("jdd_FSFlush: Returns [%d]", rCode) ;
	 }
	return (rCode) ;*/
	return JC_OK ;
}


/**
 * @ingroup		jdd_fileapi
 * @brief 		This function reads contents of the specified file.
 * @param[out]	pvReadBuf Specifies the buffer into which the contents of the
 *				file are read into.
 * @param[in]	uiItemSize Specifies the size of each item that is to be read.
 * @param[in]	uiItems is the number of items of size uiItemSize that is 
 *				required to be read.
 * @param[in]	vFileHandle is the reference to the file on which file read is 
 *				to be read.
 * @retval		On success this call returns the number of successfully read 
 *              items from the file.
 * @retval		Returns -1 On failure.
 *
 * This function reads the required number of items from the file and returns 
 * the number of items successfully read from the file.The contents of the file
 * are read into the buffer pReadBuf. 
 */

JC_INT32 jdd_FSRead(void *pvReadBuf, \
					const JC_UINT32 uiItemSize, \
					const JC_UINT32 uiItems, \
					JDD_FILE vFileHandle)
{
	JC_INT32	iReadSize = -1 ;
	JC_INT32	iBytesToRead ;
	JC_INT32	iBytesRead ;
	/*--- Platform Specific Variables ---*/
	/*--------------------------------------*/
	INT32		Ret ;
	FS_INFO	psFSInfo ;

	Ret = FS_GetFSInfo(CS_FS_MMS_IDENTIFIER,&psFSInfo) ;
	if (Ret != ERR_SUCCESS)
	{
		psFSInfo.iTotalSize = psFSInfo.iUsedSize = 0 ;
	}
	PLATFORM_LOG0 ("jdd_FSRead: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pvReadBuf), \
						"jdd_FSRead: pvReadBuf is NULL") ;
	jdd_AssertOnFail ((0 != uiItems && 0 != uiItemSize), \
						"jdd_FSRead: Read with 0 size") ;

	/* If platform read () expects bytes */
	iBytesToRead = uiItemSize * uiItems ;
	/* 
	 * If platform read () can handle items, uiItemSize and uiItems
	 * can be used directly 
	 */

	/* Platform API to read iBytesToRead and returns iReadSize */
	if(MAX_FILE_DISCRIPTOR +1 == (JC_INT32)vFileHandle)
	{
		vFileHandle = 0;		
	}
	iBytesRead = FS_Read ( (JC_INT32)vFileHandle, (JC_UINT8 *)pvReadBuf, iBytesToRead);
	mmi_trace (1, "JDD_LOG: jdd_FSRead WAP_MMS Available FS Mem is [%d] and Bytes Read [%d]",
			(UINT32)psFSInfo.iTotalSize - (UINT32)psFSInfo.iUsedSize, iBytesRead) ;
	if (iBytesRead <= 0)
	{
		iReadSize = -1 ;
		//getJCFError (iBytesRead) ;		
	}
	else
	{
		iReadSize = iBytesToRead / uiItemSize ;
	}
	
	
	/* 
	 * If platform read () returns bytes read from file successfully, iReadSize
	 * is calculated as mentioned in else statement below. 
	 * If Platform can directly	read items from the file, comment "else" part 
	 */
	/*
	else
	{
		iReadSize = iReadSize / uiItemSize ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG1 ("jdd_FSRead: Exit[%d]",iReadSize) ;
	return (iReadSize) ;
}


/** 
 * @ingroup		jdd_fileapi
 * @brief 		This function moves the file pointer to a specified position 
 * 				within the specified file.
 * @param[in]	vFileHandle  Specifies the reference to the opened file,
 *				which is returned by #jdd_FSOpen
 * @param[in] 	iPos Specifies the number of bytes from the origin.
 * @param[in] 	eSeekMode Specifies the seek mode.
 * @retval		JC_OK If file pointer is successfully moved.
 * @retval		JC_ERR_FILE_SEEK If there is an error when moving 
 *				the file pointer.
 *
 * This function sets the file position indicator of the specified file to the
 * specified position. The new position is calculated from the offset bytes to 
 * the position specified by eSeekMode. 
 */

JC_RETCODE jdd_FSSeek (JDD_FILE vFileHandle, \
					   const JC_INT32 iPos, \
					   EFileSeekMode eSeekMode)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	JC_UINT8	iOrigin =0 ;
	JC_INT32	iOffset ;
	/*-----------------------------------*/

	PLATFORM_LOG0 ("jdd_FSSeek: Entry") ;	
	jdd_AssertOnFail (0 <= vFileHandle, \
						"jdd_FSSeek: vFileHandle is NULL") ;
	
	if ( (E_FILE_SEEK_END == eSeekMode) && (0 < iPos) )
	{
		rCode = JC_ERR_FILE_EOF ;
		PLATFORM_LOG1 ("jdd_FSSeek: Returns [%d]", rCode) ;
	}
	else if ( (E_FILE_SEEK_SET == eSeekMode) && (0 > iPos) )
	{
		rCode = JC_ERR_FILE_SEEK ; 
		PLATFORM_LOG1 ("jdd_FSSeek: Returns [%d]", rCode) ;
	}
	/*--- Platform Specific Implementation Start ---*/

	/* 
	 * If platform cannot seek backwards, workaround is to convert 
	 * to forward seek 
	 */
	 
	/*
	else if (0 > iPos)	
	{
		[Implementation to Convert Backward Seek to Forward Seek ]
	}
	*/	

	else
	{
//		rCode = JC_OK ;

		switch (eSeekMode)
		{
			case E_FILE_SEEK_SET:
				/* Set platform specific seek mode */
				iOrigin = FS_SEEK_SET ;
				break ;
			case E_FILE_SEEK_CUR:
				/* Set platform specific seek mode */
				iOrigin = FS_SEEK_CUR ;
				break ;
			case E_FILE_SEEK_END:
				/* Set platform specific seek mode */
				iOrigin = FS_SEEK_END ;
				break ;
			default:
				rCode = JC_ERR_FILE_SEEK ;
				jdd_AssertOnFail (0 , "jdd_FSSeek: EFileSeekMode is default") ;
				PLATFORM_LOG1 ("jdd_FSSeek: Returns [%d]", rCode) ;
		}		
	}

		/* PlatForm Specific Function for File Seek */
	if(MAX_FILE_DISCRIPTOR +1 == (JC_INT32)vFileHandle)
	{
		vFileHandle = 0;		
	}
		iOffset = FS_Seek ( (JC_INT32)vFileHandle, iPos, (JC_UINT8)iOrigin) ;				
		if (iOffset >= 0)
		{
			rCode = JC_OK ;
		}
		else
		{
			rCode = JC_ERR_FILE_SEEK ;
			//rCode = getJCFError (iOffset) ;
			PLATFORM_LOG1 ("jdd_FSSeek: Returns [%d]", rCode) ;
		}
	/*	*/


	/*--- Platform Specific Implementation End ---*/		
	mmi_trace (1, "jdd_FSSeek: filehandle [%d], iPos [%d], eSeekMode [%d] and iOffset [%d]",
	(JC_UINT32)vFileHandle, iPos, eSeekMode, iOffset) ;	
	return (rCode) ;
}


/**
 * @ingroup		jdd_fileapi
 * @brief 		This function is used to obtain the current file size from 
 *				the opened file stream.
 * @param[in]	vFileHandle Specifies the reference to the opened file, 
 * 				which is returned by #jdd_FSOpen
 * @param[out]	puiFileSize Returns the current size of the specified file
 * @retval		JC_OK on sucess 
 * @retval		JC_ERR_FILE_SIZE on failure
 */

JC_RETCODE jdd_FSGetSize (JDD_FILE vFileHandle, \
						  JC_UINT32 *puiFileSize) 
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	JC_INT32 iFileSize ;
	/*--------------------------------------*/
	PLATFORM_LOG0 ("jdd_FSGetSize: Entry") ;	
	jdd_AssertOnFail ((0 <= vFileHandle && \
						JC_NULL != puiFileSize), \
						"jdd_FSGetSize: vFileHandle/puiFileSize is NULL") ;

	/*--- Platform Specific Implementation Start ---*/

	/* Platform API to get file size and assign to puiFileSize */
	if(MAX_FILE_DISCRIPTOR +1 == (JC_INT32)vFileHandle)
	{
		vFileHandle = 0;		
	}
	iFileSize = FS_GetFileSize( (JC_INT32)vFileHandle); //In file fsi.c 	
	if (iFileSize >= 0)
	{
		*puiFileSize = iFileSize ;
		rCode = JC_OK ;
	}
	else
	{
		rCode = JC_ERR_FILE_SIZE ;
		//rCode = getJCFError(iFileSize) ; //Error Logs
		PLATFORM_LOG1 ("jdd_FSGetSize: Returns [%d]", rCode) ;
	}
	/**/
	/* 
	 * If file size cannot be obtained directly from any of the platform APIs,
	 * fseek () or ftell () might be used to get the file size 
	*/
	/*--- Platform Specific Implementation End ---*/
	mmi_trace (1, "JDD_LOG: jdd_FSGetSize for handle [%d] returned [%d]", (JC_INT32)vFileHandle, *puiFileSize) ;
	PLATFORM_LOG0 ("jdd_FSGetSize : Exit") ;	
	return (rCode) ;
}


/**
* @ingroup		jdd_fileapi
* @brief 		This function is used to reposition the file pointer to the 
*				beginning of the opened stream.
* @param[in]	vFileHandle specifies the reference to the opened file, 
*				which is returned by #jdd_FSOpen
* @retval		JC_OK If Success
* @retval		JC_ERR_FILE_SEEK if file rewind fails
* @see			#jdd_FSOpen 
*/

JC_RETCODE jdd_FSRewind (JDD_FILE vFileHandle)
{	
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	/*--------------------------------------*/

	PLATFORM_LOG0 ("jdd_FSRewind: Entry") ;
	jdd_AssertOnFail ((0 <= vFileHandle), \
		                "jdd_FSRewind: vFileHandle is NULL") ;

	/*--- Platform Specific Implementation Start ---*/
		
	/* PlatForm Specific Function for File rewind */
	/*		
	if (Platform API Succeeds)
	 {
			rCode = JC_OK ;
	 }
	 else
	 {
	       rCode = getJCFError (<platformErrValue>) ;
		   PLATFORM_LOG1 ("jdd_FSRewind: Returns [%d]", rCode) ;
	 }
	*/
	
	/*
	 * Workaround maybe required for the platforms that dont 
	 * support Rewind function
	 */
	 if(MAX_FILE_DISCRIPTOR +1 == (JC_INT32)vFileHandle)
	{
		vFileHandle = 0;		
	}
	rCode = jdd_FSSeek (vFileHandle, 0, E_FILE_SEEK_SET) ;

	/*--- Platform Specific Implementation End ---*/
	
	mmi_trace (1, "jdd_FSRewind : filehandle [%d]", (JC_UINT32)vFileHandle) ;	
	return (rCode) ;
}


/**
 * @ingroup	:jdd_fileapi
 * @brief This function specifies whether the given file name exists.
 * @param[in] vFileSysHandle Specifies the handle for the file system, which 
						is returned by #jdd_FSInitialize.
 * @param[in] pmFileName Specifies the name of the file
 * @retval E_TRUE If file exists
 * @retval E_FALSE If file does not exists
 */

JC_BOOLEAN jdd_FSIsFileExist (JDD_FSHANDLE vFileSysHandle, \
							  JC_CHAR *pmFileName )
{
	JC_BOOLEAN	bRetval = E_FALSE ;
	JC_CHAR* pmFName = JC_NULL;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/
	JDD_FILE		vFileHandle = JC_NULL ;/**/ /* Required for workaround*/

	PLATFORM_LOG0 ("jdd_FSIsFileExist: Entry") ;
	pmFName = MapFileNameToDrive(pmFileName) ;
	jdd_AssertOnFail ((JC_NULL != pmFileName), "jdd_FSIsFileExist: File name is NULL") ;

	/*--- Platform Specific Implementation Start ---*/
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;
	
	/*If Application wants to convert all forward slashes to back slashes in 
		Destination File name  */
		
		/*	
			convertFwdSlashToBackSlash (pcFileName) ;
		*/
	
	/* Platform API for IsFileExist */

		/* 
		 * Workaround for the platform that does not support IsFileExist
		 */
	if(pmFName)
	{
		vFileHandle = jdd_FSOpen (vFileSysHandle, pmFName, E_OPEN_READ_MODE) ;
	}
	else
	{
		vFileHandle = jdd_FSOpen (vFileSysHandle, pmFileName, E_OPEN_READ_MODE) ;
	}	
	if(vFileHandle > 0)
	{
		bRetval = E_TRUE ;
		jdd_FSClose (vFileHandle) ;
	}
	jdd_MemFree(pmFName);
	/*--- Platform Specific Implementation End ---*/
	mmi_trace (1, "JDD_LOG: jdd_FSIsFileExist () returned [%d]", bRetval) ;
	return (bRetval) ;
}




/** 
 * @ingroup		jdd_fileapi
 * @brief 		This API is used to obtain the current file size.
 * @param[in]	vFileSysHandle Specifies the handle for the file system, which
 *				is returned by the initialize function.
 * @param[in]	pmFileName Specifies the name of the file.
 * @param[out]	puiFileSize Returns the current size of the specified file name
 * @retval		JC_OK on sucess.
 * @retval		JC_ERR_FILE_SIZE if size cannot be obtained.
 * @note 		The file name should contain full path.
 */

JC_RETCODE jdd_FSGetFileSize (JDD_FSHANDLE vFileSysHandle, \
							  JC_CHAR *pmFileName, \
							  JC_UINT32 *puiFileSize) 
{
	JC_RETCODE rCode ;
	/* 
	 * If platform cannot tell size without opening it, following variable
	 * need to be used 
	 */
	/* JDD_FILE	vFileHandle	= JC_NULL ; */
	
	/*--- Platform Specific Variables ---*/
	JC_INT32 rVal ;
	FS_FILE_ATTR FileAttr ={0,} ;
	JC_CHAR* pmFName = JC_NULL;
	/*--------------------------------------*/

	PLATFORM_LOG0 ("jdd_FSGetFileSize: Entry") ;
	jdd_AssertOnFail (JC_NULL != pmFileName && JC_NULL != puiFileSize, \
				"jdd_FSGetFileSize: pmFileName/puiFileSize is NULL") ;
	
	/*--- Platform Specific Implementation Start ---*/
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;
	/* PlatForm Specific Function for getting file size */
	pmFName = MapFileNameToDrive(pmFileName) ;

	if(pmFName)
	{
		rVal = FS_GetFileAttr ((PCSTR)pmFName,  &FileAttr);			
	}
	else
	{
		rVal = FS_GetFileAttr ((PCSTR)pmFileName,  &FileAttr);
	}		
	if (ERR_SUCCESS == rVal)
	 {
		*puiFileSize = FileAttr.i_size ;//For regular files, the file size in bytes.
		rCode = JC_OK ;
	 }
	 else
	 {
		 rCode = JC_ERR_FILE_SIZE ;
	       //rCode = getJCFError (rVal) ;
	 }
	/**/
		/* 
		 * If platform API to get file size using file path, following 
		 * workaround can be used 
		 */
		/*
		vFileHandle = jdd_FSOpen (vFileSysHandle, pmFileName, \
									E_OPEN_READ_MODE) ;
	if (JC_NULL != vFileHandle)
	{
		rCode = jdd_FSGetSize (vFileHandle, puiFileSize) ;
		if (JC_OK != rCode)
		{
			rCode = jdd_FSClose (fileHandle) ;
		}
	}
	else
	{
		rCode = JC_ERR_FILE_SIZE ;
		PLATFORM_LOG1 ("jdd_FSGetFileSize: Returns [%d]", rCode) ;
	}
	*/
	/*--- Platform Specific Implementation End ---*/
	jdd_MemFree(pmFName);
	mmi_trace (1, "JDD_LOG: jdd_FSGetFileSize () returned [%d]", *puiFileSize) ;
   	PLATFORM_LOG1 ("jdd_FSGetFileSize: Returns [%d]", rCode) ;
	return (rCode) ;
}


/**
 * @ingroup		jdd_fileapi
 * @brief 		This function renames old file name to new file name. 
 * @param[in]	vFileSysHandle Specifies the handle for the file system, 
 *				which is returned by #jdd_FSInitialize.
 * @param[in]	pmSrcFileName Specifies the existing file name 
 * 				which needs to be renamed
 * @param[in]	pmDestFileName Specifies the new name for the existing file.
 * @retval		JC_OK If file was successfully renamed
 * @retval		JC_ERR_FILE_RENAME If Error in Renaming the file.
 * @retval		JC_ERR_FILE_NOT_FOUND If Source file is not present
 */

JC_RETCODE jdd_FSRename (JDD_FSHANDLE vFileSysHandle, \
						 const JC_CHAR *pmSrcFileName, \
						 const JC_CHAR *pmDestFileName)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	JC_INT32 rVal = ERR_SUCCESS ;
	/*-----------------------------------*/

	PLATFORM_LOG0 ("jdd_FSRename: Entry") ;
	
	jdd_AssertOnFail ((JC_NULL != pmSrcFileName) && \
						(JC_NULL != pmDestFileName), \
						"jdd_FSRename: Invalid Parameter") ;	

	if ( E_TRUE == jdd_FSIsFileExist (JC_NULL, (JC_CHAR*)pmDestFileName ) ) 
	{
		rCode = JC_ERR_FILE_DIR_ALREADY_EXIST ; 
		PLATFORM_LOG1 ("jdd_FSRename: Returns [%d]", rCode) ;
	}	
	else if (E_FALSE == jdd_FSIsFileExist (JC_NULL, (JC_CHAR*)pmSrcFileName ) ) 
	{
		rCode = JC_ERR_FILE_NOT_FOUND ; 
		PLATFORM_LOG1 ("jdd_FSRename: Returns [%d]", rCode) ;
	}
	else 
	{

		/*--- Platform Specific Implementation Start ---*/
		PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;

			/* 
			 * If Application wants to convert all forward slashes to back
			 * slashes in Destination File name  
			 */
			/*	
				convertFwdSlashToBackSlash (pcSrcFileName) ;
				convertFwdSlashToBackSlash (pcDestFileName) ;
		*/

		/* PlatForm Specific Function to Rename File  */
		rVal = FS_Rename ((PCSTR)pmSrcFileName, (PCSTR)pmDestFileName) ;
				
		if (ERR_SUCCESS == rVal)
		 {
				rCode = JC_OK ;
		 }
		 else
		 {
			 rCode = JC_ERR_FILE_RENAME ;
			   //rCode = getJCFError (rVal) ;
			   PLATFORM_LOG1 ("jdd_FSRename: Returns [%d]", rCode) ;
		 }
		/**/
		
		/*--- Platform Specific Implementation End ---*/
	}
	mmi_trace (1, "JDD_LOG: jdd_FSRename () returned [%d]", rVal) ;
	PLATFORM_LOG0 ("jdd_FSRename: Exit") ;	
	return (rCode) ;
}


/**
* @ingroup		jdd_fileapi
* @brief 		This function deletes the specified file
* @param[in]	vFileSysHandle Specifies the handle for the file system, which 
*				is returned by the initialize function.
* @param[in]	pmFileName Specifies the file name which needs to be deleted
* @retval		JC_ERR_FILE_DELETE Specifies the error in deleting the file.
* @retval		JC_OK on success.
*/

JC_RETCODE jdd_FSDelete (JDD_FSHANDLE vFileSysHandle, \
						 const JC_CHAR *pmFileName)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	JC_INT32 rVal ;
	JC_CHAR* pmFName = JC_NULL;
	/*--------------------------------------*/
	
	PLATFORM_LOG0 ("jdd_FSDelete: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pmFileName), "jdd_FSDelete: pmFileName is NULL") ;

	{
		JC_INT8  pcFName [52] = {0, } ;
			
		jdi_CUtilsStrTcsNCpy (pmFileName, 50, pcFName) ;
		mmi_trace (1, "JDD_LOG: jdd_FSDelete file name: [%s]", pcFName) ;
	}
	
	
	/*--- Platform Specific Implementation Start ---*/
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;
	/* PlatForm Specific Function for File Delete */
	pmFName = MapFileNameToDrive(pmFileName) ;

	if(pmFName)
	{
		rVal = FS_Delete ((PCSTR)pmFName);	
	}
	else
	{
		rVal = FS_Delete ((PCSTR)pmFileName);
	}
	if (ERR_SUCCESS == rVal)
	{
		rCode = JC_OK ;
	}
	else
	{
		rCode =  JC_ERR_FILE_DELETE ;
		PLATFORM_LOG1 ("jdd_FSDelete: Returns [%d]", rCode) ;
	}
	jdd_MemFree(pmFName);	
	/*--- Platform Specific Implementation End ---*/
	
	mmi_trace (1,"JDD_LOG: jdd_FSDelete : Exit rcode %d ", rVal) ;	
	
	return (rCode) ;
}


/**
 * @ingroup		jdd_fileapi
 * @brief 		This function is used to create new directory.
 * @param[in]	vFileSysHandle Specifies the handle for the file system, 
 *				which is returned by #jdd_FSInitialize
 * @param[in] 	pmDirName Specifies the directory name which needs to be created
 * @retval		JC_OK If directory was successfully created.
 * @retval		JC_ERR_FILE_CREATEDIR if directory cannot be created.
 * @retval		JC_ERR_FILE_DIR_ALREADY_EXIST If Directory already exists.
 *
 * This function creates a directory with the name given and sets 
 * the default permissions. This function shall not set the current directory 
 * path to the one  created.
 */

JC_RETCODE jdd_FSMakeDir (JDD_FSHANDLE vFileSysHandle, \
						  const JC_CHAR *pmDirName)
{
	JC_RETCODE	rCode ;
	
	/*--- Platform Specific Variables ---*/
	JC_INT32 rVal ;
	/*-----------------------------------*/	

	PLATFORM_LOG0 ("jdd_FSMakeDir: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pmDirName), \
		                      "jdd_FSMakeDir: pmDirName is NULL") ;
	
	if (E_TRUE == jdd_FSIsDirExist (vFileSysHandle, pmDirName))
	{
		rCode = JC_ERR_FILE_DIR_ALREADY_EXIST ;
		PLATFORM_LOG1 ("jdd_FSMakeDir: Returns [%d]", rCode) ;
	}
	else
	{
		/*--- Platform Specific Implementation Start ---*/
		PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;

			/*
			 * If Application wants to convert all forward slashes to back 
			 * slashes in Destination File name  
			 */
			/*	
				convertFwdSlashToBackSlash (pcDirName) ;		
		*/		

		/* PlatForm Specific Function for Make Directory  */
		rVal = FS_MakeDir ((PCSTR)pmDirName, 0);		
		{
			JC_INT8  pcFName [52] = {0, } ;
				
			jdi_CUtilsStrTcsNCpy (pmDirName, 50, pcFName) ;
			mmi_trace (1, "JDD_LOG: Make Dir: %s returned %d", pcFName, rVal) ;
		}
		if (ERR_SUCCESS == rVal)
		 {
				rCode = JC_OK ;
		 }
		 else
		 {
			 rCode = JC_ERR_FILE_DIR_ALREADY_EXIST ;
			   //rCode = getJCFError (rVal) ;
			   PLATFORM_LOG1 ("jdd_FSMakeDir: Returns [%d]", rCode) ;
		 }
		
		/*--- Platform Specific Implementation End ---*/
	}

	PLATFORM_LOG0 ("jdd_FSMakeDir: Exit") ;
	return (rCode) ;
}

/** 
* @ingroup		jdd_fileapi
* @brief 		This function sets the current directory path to the 
*				specified directory. 
* @param[in]	vFileSysHandle Specifies the handle for the file system, which
*				is returned by #jdd_FSInitialize
* @param[in]	pmDirName Specifies the name of the directory which has to 
*				be made the current directory
* @retval		JC_OK on success
* @retval		JC_ERR_FILE_CHANGEDIR Specifies the error in changing 
*				the Directory.
*/

JC_RETCODE jdd_FSChDir (JDD_FSHANDLE vFileSysHandle, \
						const JC_CHAR *pmDirName)
{
	JC_RETCODE rCode ;
	/*--- Platform Specific Variables ---*/
	JC_INT32 rVal ;
	/*--------------------------------------*/

	PLATFORM_LOG0 ("jdd_FSChDir : Entry") ;
	jdd_AssertOnFail (JC_NULL != pmDirName, \
						"jdd_FSChDir: pmDirName is NULL") ;

	/*--- Platform Specific Implementation Start ---*/
	PARAM_INTENTIONALLY_NOT_USED(vFileSysHandle) ;
	
	/* PlatForm Specific Function for Change Directory */

	rVal = FS_ChangeDir  ((PCSTR)pmDirName) ;
	if (ERR_SUCCESS == rVal)
	 {
			rCode = JC_OK ;
	 }
	 else
	 {
		 rCode = JC_ERR_FILE_CHANGEDIR ;

	       //rCode = getJCFError (rVal) ;
		   PLATFORM_LOG1 ("jdd_FSChDir: Returns [%d]", rCode) ;
	 }
/*	*/
	/*--- Platform Specific Implementation End ---*/
	PLATFORM_LOG0 ("jdd_FSChDir : Exit") ;	
	return (rCode) ;
}

//TBD
#if 0
static JC_RETCODE DirDeleteAll (const JC_CHAR * psargDirName)
{
	INT32 rVal ;
	INT32 iFd ;
	JC_RETCODE rCode = JC_OK ;
	FS_FIND_DATA sFindDatanext = {0,};
	
	iFd =  FS_FindFirstFile((PCSTR)psargDirName,  &sFindDatanext) ;
	while(iFd >= 0)
	{
		if(FS_ATTR_DIR & sFindDatanext.st_mode)
		{
			rVal =  FS_RemoveDir (sFindDatanext.st_name) ;
			if(ERR_SUCCESS == rVal)
			{
				rCode = JC_OK ;
			}
			else
			{
				if(ERR_FS_DIR_NOT_EMPTY == rVal)
				{
					rCode = DirDeleteAll ((JC_CHAR *)sFindDatanext.st_name) ;
				}
			}
			
		}
		else if(!(FS_ATTR_DIR & sFindDatanext.st_mode))
		{
			rVal = FS_Delete (sFindDatanext.st_name);
		}
		//else if(INODE_TYPE_UNKNOWN & sFindDatanext.st_mode)
		//{
			//Error case do nothing  
		//	break;
		//}
		jc_memset(&sFindDatanext, 0, sizeof(FS_FIND_DATA)) ;
		
		if(ERR_SUCCESS != FS_FindNextFile(iFd, &sFindDatanext))
		{
			FS_FindClose(iFd);
			break;
		}		
	}	
return rCode ;	
}
#endif
/**
 * @ingroup		jdd_fileapi
 * @brief 		This function deletes the given directory.
 * @param[in]	vFileSysHandle Specifies the handle for the file system, 
 *				which is returned by #jdd_FSInitialize.
 * @param[in]	pmDirName Specifies the name of the directory which 
 *				needs to be deleted.
 * @retval		JC_OK If Directory is deleted successfully.
 * @retval		JC_ERR_DIR_DELETE If directory could not be deleted.
 *
 * This function should delete all the files and subdirectory inside the 
 * given directory, if it exists.
 */
 
JC_RETCODE jdd_FSDelDir (JDD_FSHANDLE vFileSysHandle,
						 const JC_CHAR *pmDirName) 
{
	JC_RETCODE	rCode ;
	JC_INT32		rVal  = 0;
	JC_INT8		pcFName [256] = {0, } ;
       FS_FIND_DATA sFindData;
       JC_INT32 rFd = -1;
       JC_UINT32 iLen = 0;
       JC_INT16 *pFindName = NULL;
       JC_INT8 rConCh[4] = {'/',0,0,0,};

	jdd_AssertOnFail ((JC_NULL != pmDirName), "jdd_FSDelDir: pmDirName is NULL") ;
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;
     
	jdi_CUtilsStrTcsNCpy (pmDirName,50,pcFName) ;
	
       iLen = pfnUnicodeStrlen((const S8*)pmDirName) ;
	pFindName = jdd_MemAlloc(iLen + 512,sizeof(JC_INT16));
	if(!pFindName)
       {
           return JC_ERR_DIR_DELETE;
	}
       pfnUnicodeStrcpy(pFindName,(const S8 *)pmDirName);
	pFindName[iLen] = '/';
	pFindName[iLen + 1] = '*';
	pFindName[iLen+  2] = 0;

	
       rFd = FS_FindFirstFile(pFindName, &sFindData);
       while(rFd >= 0 && rVal == 0)
       {
            if(sFindData.st_name[0] == '.' && 
               sFindData.st_name[1] == 0)
            {
                
            }
            else
            {
                pfnUnicodeStrcpy(pFindName,pmDirName);                    
                pfnUnicodeStrcat(pFindName,rConCh);
                pfnUnicodeStrcat(pFindName,sFindData.st_name);
                if(FS_IS_DIR(sFindData.st_mode))
                {
                    
                    rVal = FS_RemoveDir (pFindName) ;
                    if(rVal != 0)
                    {
                        break;
                    }
                }
                else
                {
                    rVal = FS_Delete(pFindName) ;
                    if(rVal != 0)
                    {
                        break;
                    }
                }
               
            }
           rVal = FS_FindNextFile(rFd,&sFindData);
       }
	if(rFd >= 0)
       {
           FS_FindClose(rFd);
        }
	rVal = FS_RemoveDir (pmDirName) ;
	jdi_CUtilsStrTcsNCpy (pmDirName,50,pcFName) ;
	mmi_trace (1, "JDD_LOG: FS_RemoveDir: %s returned %d", pcFName, rVal) ;
	if (FS_NO_ERROR == rVal)
	{
		rCode = JC_OK ;
	}
	else 
	{	
		rCode = JC_ERR_DIR_DELETE ;
	}
	if(pFindName)
	{
		jdd_MemFree(pFindName);
	}
	return (rCode) ;
}


/**
 * @ingroup		jdd_fileapi
 * @brief 		This function is used to rename the directory.
 * @param[in]	vFileSysHandle Specifies the handle for the file system, 
 *				which is returned by #jdd_FSInitialize.
 * @param[in]	pmSrcDirName Specifies the existing directory name which 
 *				needs to be renamed
 * @param[in]	pmDestDirName Specifies the new name for the 
 *				existing directory. 
 * @retval		JC_OK If directory was successfully renamed
 * @retval		JC_ERR_FILE_DIR_ALREADY_EXIST If destination Directory 
 *				name already exists.
 */

JC_RETCODE jdd_FSDirRename (JDD_FSHANDLE vFileSysHandle, \
							const JC_CHAR *pmSrcDirName, \
							const JC_CHAR *pmDestDirName)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	JC_INT32 rVal ;
	/*-----------------------------------*/

	PLATFORM_LOG0 ("jdd_FSDirRename: Entry") ;

	jdd_AssertOnFail ((JC_NULL != pmSrcDirName) && \
						(JC_NULL != pmDestDirName), \
							"jdd_FSDirRename: Invalid Parameter") ;
	
	if (E_TRUE == jdd_FSIsDirExist (vFileSysHandle, pmDestDirName))
	{
		rCode = JC_ERR_FILE_DIR_ALREADY_EXIST ;
		PLATFORM_LOG1 ("jdd_FSDirRename: Returns [%d]", rCode) ;
	}
	else
	if (E_FALSE == jdd_FSIsDirExist (vFileSysHandle, pmSrcDirName))
	{
		rCode = JC_ERR_DIR_RENAME ;
		PLATFORM_LOG1 ("jdd_FSDirRename: Returns [%d]", rCode) ;
	}
	else
	{
		/*--- Platform Specific Implementation Start ---*/
		PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;
		
			/* 
			 * If Application wants to convert all forward slashes to 
			 * back slashes in Directory name  
			 */
			/*	
			convertFwdSlashToBackSlash ( (JC_INT8 *) pmDestDirName) ;
			convertFwdSlashToBackSlash ( (JC_INT8 *) pmSrcDirName) ;
			*/
		
		/* PlatForm Specific Function for Rename Directory  */
		rVal = FS_Rename ((PCSTR)pmSrcDirName, (PCSTR)pmDestDirName) ;				
		if (ERR_SUCCESS == rVal)
		 {
				rCode = JC_OK ;
		 }
		 else
		 {
			 rCode = JC_ERR_FILE_RENAME ;
			   //rCode = getJCFError (rVal) ;
			   PLATFORM_LOG1 ("jdd_FSDirRename: Returns [%d]", rCode) ;
		 }
		/**/

		/*--- Platform Specific Implementation End ---*/
	}

	PLATFORM_LOG0 ("jdd_FSDirRename: Exit") ;	
	return (rCode);
}


/**
* @ingroup	:jdd_fileapi
* @brief This Function specifies whether the given directory exists.
* @param [in]	vFileSysHandle Specifies the handle for the file system, 
						which is returned by #jdd_FSInitialize.
* @param[in]	pmDirName Specifies the name of the directory.
* @retval		E_TRUE Specifies the directory exists.
* @retval		E_FALSE Specifies the directory does not exist.
*/

JC_BOOLEAN jdd_FSIsDirExist (JDD_FSHANDLE vFileSysHandle, \
							  const JC_CHAR *pmDirName)
{
	JC_BOOLEAN	bRetVal ;
	/*--- Platform Specific Variables ---*/
	JC_INT32 rVal ;
	FS_FILE_ATTR FileAttr = {0,};
	/*--------------------------------------*/
	
	PLATFORM_LOG0 ("jdd_FSIsDirExist: Entry") ;
	jdd_AssertOnFail ((JC_NULL != pmDirName), \
		                   "jdd_FSIsDirExist: pmDirName is NULL") ;
	
	/*--- Platform Specific Implementation Start ---*/
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;

	/* PlatForm Specific Function for IsDirExist */
	rVal = FS_GetFileAttr ((PCSTR)pmDirName, &FileAttr) ;
	if(ERR_SUCCESS == rVal && FS_ATTR_DIR == FileAttr.i_mode)
	{
		bRetVal = E_TRUE ;
	}
	else
	{
		bRetVal = E_FALSE ;
		PLATFORM_LOG1 ("jdd_FSIsDirExist: Returns [%d]", bRetVal) ;
	}
	

		/* 
		 * Workaround for the platforms that dont support IsDirExist function: 
		 * Call jdd_FSOpenDir, if its implemented. If jdd_FSOpenDir 
		 * returns Success, call jdd_FSCloseDir and set bRetVal to E_TRUE 
		 * Else, set bRetVal to E_FALSE
		 */
		
	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0 ("jdd_FSIsDirExist : Exit") ;	
	return (bRetVal) ;
}

/**
* @ingroup		jdd_fileapi
* @brief 		This function will return the root directory
* @param[out]	pmRootDirectory Returns the root directory 
* @param [in, out]puiDirSize	Specifies the root directory length expected '/'
*							Returns the string length of the root directory	
* @retval		JC_OK on success
* @retval		JC_ERR_CFG_INSUFFICIENT_MEMORY if no sufficient memory
*
* This function copies the root directory to pmRootDirectory.If the param  
* pmRootDirectory is JC_NULL or puiDirSize is less than the reqired size then
* it fills the puiDirSize with the  reqired size and returns the error code 
* JC_ERR_CFG_INSUFFICIENT_MEMORY.
*/

JC_RETCODE jdd_FSGetRootDirectory (JC_CHAR *pmRootDirectory, \
								   JC_UINT32 *puiDirSize)
{
	JC_RETCODE rCode = JC_OK ;
	JC_UINT32 uiRootDirSize = 0 ;
	/*--- Platform Specific Variables ---*/
	/*--------------------------------------*/

	PLATFORM_LOG0 ("jdd_FSGetRootDirectory : Entry") ;

	/*--- Platform Specific Implementation Start ---*/
	/*
	 * Get the length of the root directory. 
	 * If "/" is considered as root directory
	 * then, uiRootDirSize = jc_strlen ("/") ;
	 */
	
	if ((JC_NULL == pmRootDirectory) || (*puiDirSize < uiRootDirSize))
	{
		*puiDirSize = uiRootDirSize ;
		rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
		PLATFORM_LOG1 ("jdd_FSGetRootDirectory: Returns [%d]", rCode) ;
	}
	else
	{
		/* Copy Root Directory Path to pmRootDirectory .
		 * If "/" is considered as root, then, 
		 * jdi_CUtilsTcsStrCpy ("/", pmRootDirectory) ;
  		 */			
		 pmRootDirectory = (JC_CHAR *)jdd_MemAlloc (sizeof (JC_CHAR), jc_wstrlen (L"/")) ;
		 jc_memcpy (pmRootDirectory, (L"/"),3) ;
			/*		
			if (Path is Copied successfully)
			 {
					rCode = JC_OK ;
			 }
			 else
			 {
				   rCode = getJCFError (<platformErrValue>) ;
				   PLATFORM_LOG1 ("jdd_FSGetRootDirectory: Returns [%d]", rCode) ;
			 }
			*/
	}
	
	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0 ("jdd_FSGetRootDirectory : Exit") ;	
	return (rCode) ;
}



/** 
 * @ingroup		jdd_fileapi
 * @brief 		This function opens the specified directory and returns 
 *				the number of sub-directories  and number of file(s) present.
 * @param[in]	vFileSysHandle Specifies the handle for the file system, 
 * 				which is returned #jdd_FSInitialize.
 * @param[in]	pmDirName Specifies the name of the directory, 
 *				which needs to be opened.
 * @param[out]	puiOpenDirID Returns the identifier for opening this 
 *				directory path
 * @param[out]	puiDirCount Returns the number of directory available in the 
 *				specified directory.
 * @param[out]	puiFileCount Returns the number of file available 
 *				in the specified directory.
 * @retval		JC_OK If Directory was successfully opened 
 * @retval		Platform error should return if Open Directory fails
 */

JC_RETCODE jdd_FSOpenDir (JDD_FSHANDLE vFileSysHandle, \
						  JC_CHAR	*pmDirName, \
						  JC_UINT32	*puiOpenDirID, \
						  JC_UINT32	*puiDirCount, \
						  JC_UINT32	   *puiFileCount)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PLATFORM_LOG0 ("jdd_FSOpenDir: Entry") ;

	jdd_AssertOnFail ((JC_NULL != pmDirName), \
						"jdd_FSOpenDir: pmDirName is NULL") ;
	jdd_AssertOnFail ((JC_NULL != puiOpenDirID), \
						"jdd_FSOpenDir: puiOpenDirID is NULL") ;
	jdd_AssertOnFail ((JC_NULL != puiDirCount), \
						"jdd_FSOpenDir: puiDirCount is NULL") ;
	jdd_AssertOnFail ((JC_NULL != puiFileCount), \
						"jdd_FSOpenDir: puiFileCount is NULL") ;
	
	/*--- Platform Specific Implementation Start ---*/
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;
	/*
	 *	Open "pmDirName" directory with platform API
	 *	Ex:PlatformOpenDir (psDirName, puiOpenDirID, puiDirCount, puiFileCount) ;
	 *	Assign value for this variable "puiOpenDirID"
	 *	If platform doesnot return the sub-directory count and file count 
	 *	within the directory, loop through the directory and get the 
	 *	sub-directories and file count.
	 *	Assign value to "puiFileCount"
	 *	Assign value to "puiDirCount"
	*/

	/* PlatForm Specific Function for Dir Open */
	/*		
	if (Platform API Succeeds)
	 {
			rCode = JC_OK ;
	 }
	 else
	 {
	       rCode = getJCFError (<platformErrValue>) ;
		   PLATFORM_LOG1 ("jdd_FSOpenDir: Returns [%d]", rCode) ;
	 }
	*/

	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0 ("jdd_FSOpenDir: Exit") ;
	return (rCode) ;	
}

/** 
 * @ingroup		jdd_fileapi
 * @brief 		This function frees the reference to the open dir.
 * @param[in]	vFileSysHandle Specifies the handle for the file system, 
 *				which is returned by the initialize function.
 * @param[in]	uiOpenDirID Specifies the directory identifier that is 
 *				returned by #jdd_FSOpenDir function.
 * @retval		JC_OK If Directory was successfully closed
 */

JC_RETCODE jdd_FSCloseDir (JDD_FSHANDLE	vFileSysHandle, \
						   JC_UINT32 uiOpenDirID)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PLATFORM_LOG0 ("jdd_FSCloseDir: Entry") ;

	jdd_AssertOnFail ((0 != uiOpenDirID), \
						"jdd_FSCloseDir: uiOpenDirID is NULL") ;

	/*--- Platform Specific Implementation Start ---*/
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;

	/*PlatForm Specific Function for Dir Close */
	/*		
	if (Platform API Succeeds)
	 {
			rCode = JC_OK ;
	 }
	 else
	 {
	       rCode = getJCFError (<platformErrValue>) ;
		   PLATFORM_LOG1 ("jdd_FSCloseDir: Returns [%d]", rCode) ;
	 }
	*/

	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0 ("jdd_FSCloseDir: Exit") ;
	return (rCode) ;
}



/** 
 * @ingroup		jdd_fileapi
 * @brief 		This function returns the next entry in the directory list. 
 *				It also specifies whether the retrieved entry is file 
 *				or directory.
 * @param[in]	vFileSysHandle Specifies the handle for the file system, 
 *				which is returned by the initialize function.
 * @param[in]	uiOpenDirID Specifies the directory identifier that is returned
				by #jdd_FSOpenDir function.
 * @param[in]	bFullPath Specifies whether the return value should be 
 *				full path or only
				the name of next entry
 * @param[out]	pmName Returns the name of the next entry.
 * @param[out]	pbIsDirectory Returns whether the entry that is retrieved 
 *				is file or directory.
 * @retval		JC_OK on success
 */

JC_RETCODE jdd_FSGetNextEntry (JDD_FSHANDLE vFileSysHandle, \
							   JC_UINT32 uiOpenDirID, \
							   JC_BOOLEAN bFullPath, \
							   JC_CHAR **pmName, \
							   JC_BOOLEAN *pbIsDirectory)
{
	JC_RETCODE rCode = JC_ERR_NOT_IMPLEMENTED ;
	/*--- Platform Specific Variables ---*/
	/*-----------------------------------*/

	PLATFORM_LOG0 ("jdd_FSGetNextEntry: Entry") ;
	jdd_AssertOnFail ((JC_NULL != *pmName), \
		                       "jdd_FSGetNextEntry: *pmName is NULL") ;
	
	/*--- Platform Specific Implementation Start ---*/
	PARAM_INTENTIONALLY_NOT_USED (vFileSysHandle) ;

	/* PlatForm Specific Function for Get Next Entry  */
	/*		
	if (Platform API Succeeds)
	 {
			rCode = JC_OK ;
	 }
	 else
	 {
	       rCode = getJCFError (<platformErrValue>) ;
		   PLATFORM_LOG1 ("jdd_FSGetNextEntry: Returns [%d]", rCode) ;
	 }
	*/

	/*--- Platform Specific Implementation End ---*/

	PLATFORM_LOG0 ("jdd_FSGetNextEntry: Exit") ;
	return (rCode) ;
}
# if 0
int _snprintf(char * buf, size_t len, const char *fmt, ...)
{
	return 1;
}
#endif
/* End of File */
#endif
