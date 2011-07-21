/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 * NVRAMINTERFACE.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for nvram read write data interface.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
//add for debug
#include "mmi_trace.h"

#ifdef MMI_ON_HARDWARE_P
#include "kal_release.h"      	/* basic data type */
#include "stack_common.h"  
#include "stack_msgs.h"
#include "app_ltlcom.h"       	/* task message communiction */
#include "stacklib.h"        	/* basic type for dll, evshed, stacktimer */
#include "event_shed.h"       	/* event scheduler */    
#include "stack_timer.h"      	/* stack timer */   
#include "l4dr.h"
#include "lcddr.h"
#include  "nvram_data_items.h"
#include "mmi_data_types.h"
#include "oslmemory.h"
#include "nvram_defs.h"
#include "nvram_user_defs.h"
#endif

#include "stdc.h"
#include "queuegprot.h"
#include "nvramprot.h"
#include "nvramgdcl.h"
#include "nvramhwmapfile.h"
#include "nvramtype.h"
#include "debuginitdef.h"
//#include "taskinit.h"  /* for mmitaskprot.h */
#include "mmitaskprot.h"  /* for protocoleventhandler() prototype */
#ifdef MMI_ON_WIN32
#include "windows.h"
#include "kal_release.h"
#include "nvram_data_items.h"
#endif

 
#include "cswtype.h"
//#include "fs.h" //del by wuzc for fs at 20061220
 
#include "filesystemdef.h"
#include "fs_func.h"

#include "nvramenum.h"
#include "timerevents.h"
#include "simdetectiongexdcl.h" /* for g_pwr_context */
 
#include "sul.h"
 
 

#include "phonebooktypes.h"

void initNvramFolder(void)
{
	U8 folderAnsiiName[128],folderUnicodeName[256];
	INT32 result;
	sprintf(folderAnsiiName,"/%s",NVRAM_DEFAULT_FOLDER);
	AnsiiToUnicodeString(folderUnicodeName,folderAnsiiName);
	result=FS_IsDirFile(folderUnicodeName);
	mmi_trace(1,"chenhe,initNvramFolder,result is %d",result);
	if(FALSE == result)
	{
		result=FS_MakeDir(folderUnicodeName, 0);
		mmi_trace(1,"chenhe,initNvramFolder,FS_MakeDir,result is %d",result);
	}
	
}

 
#ifdef MMI_ON_HARDWARE_P //luanyy add
extern const kal_uint8 NVRAM_CACHE_BYTE_DEFAULT[256];

void displayNvramSize(void)
{
	U8 folderAnsiiName[128],folderUnicodeName[256];
	INT32 nvramTotalSize;
	sprintf(folderAnsiiName,"/%s/*",NVRAM_DEFAULT_FOLDER);
	AnsiiToUnicodeString(folderUnicodeName,folderAnsiiName);
	nvramTotalSize = MMI_FS_GetDirSize(folderUnicodeName);
	mmi_trace(1,"chenhe,displayNvramSize,nvramTotalSize is %dK",nvramTotalSize/1024);

}

INT32 g_tst_err_code = 0;
S32 ReadRecordInt(U16 nFileId,U16 nRecordId, void *pBuffer, U16 nBufferSize,S16 *pError, S8* filename, S32 lineNumber)
{
	S8 fileName[100];
	S8 name_uncode[100];
	INT32 handle= -1;
	INT32 iBytes = 0;
	INT32 retValue = -1;

	INT32 ret = 0;

	*pError  = 0;
	mmi_trace(g_sw_NVRAM, TSTXT("read record from NVRAM \n"));
	
	memset(fileName, 0, sizeof(fileName));
	memset(name_uncode, 0, sizeof(name_uncode));

	SUL_StrPrint(fileName, "/%s/NVRAM_%d_%d_FILE.dat",NVRAM_DEFAULT_FOLDER, nFileId, nRecordId);

	AnsiiToUnicodeString(name_uncode, fileName);
	handle = FS_Open(name_uncode, FS_O_RDONLY, 0);
	/* based on CSW mamual V3.00.02, when FS_O_RDONLY is set and the named file does not exist, return ERR_FS_NO_DIR_ENTRY. gdm 090609*/
	if(( handle <0) && ( handle != ERR_FS_FILE_NOT_EXIST) && ( handle != ERR_FS_NO_DIR_ENTRY) )
	{
		mmi_trace(g_sw_NVRAM, "call FS_Open() failed!handle = %d, filename =%s\n", handle, fileName);
              g_tst_err_code = handle;
		MMI_ASSERT(0);
	}

	if(handle >= 0)
	{

		if (nFileId == NVRAM_EF_PHB_SOMEFIELDS_LID)
		{
			ret = FS_Seek(handle, nBufferSize, FS_SEEK_SET);
			if ( ret < 0)
			{
				mmi_trace(g_sw_NVRAM, "Call FS_Seek() failed! return=%d, filehandle=%d, iOffset =%d\n", ret, handle, nBufferSize);
				g_tst_err_code = ret;
			}
				
			else
			{
				iBytes = FS_Read(handle, pBuffer, 229);

				if(iBytes >= 0)
				{
					*pError = NVRAM_READ_SUCCESS;
					retValue = iBytes;
				}
				else if( iBytes!= ERR_FS_BAD_FD)
				{
					mmi_trace(g_sw_NVRAM, "read NVRAM_EF_PHB_SOMEFIELDS_LID file failed! %d, filehandle=%d\n", iBytes, handle);
                                   g_tst_err_code = iBytes;
					MMI_ASSERT(0);
				}
				else
				{
					mmi_trace(g_sw_NVRAM, "read NVRAM_EF_PHB_SOMEFIELDS_LID file failed! %d\n", iBytes);
				}			
			}

		}
		else
		{
			ret = FS_Seek(handle, 0, FS_SEEK_SET);
			if ( ret < 0)
			{
				mmi_trace(g_sw_NVRAM, "Call FS_Seek() failed! return=%d, filehandle=%d, iOffset =%d\n", ret, handle, nBufferSize);
			}
			
			else
			{
				iBytes = FS_Read(handle, pBuffer, nBufferSize);

				if(iBytes >= 0)
				{
					*pError = NVRAM_READ_SUCCESS;
					retValue = iBytes;
				}
				else if( iBytes!= ERR_FS_BAD_FD)
				{
					mmi_trace(g_sw_NVRAM, "read file failed! %d, filehandle=%d\n", iBytes, handle);
                                   g_tst_err_code = iBytes;
					MMI_ASSERT(0);
				}
				else
				{
					mmi_trace(g_sw_NVRAM, "read file failed! %d\n", iBytes);
				}
				
			}

		}
	}
	else
	{
		if(nFileId < NVRAM_EF_LAST_LID_VENDOR)
		{
			*pError =  NVRAM_READ_SUCCESS;
			retValue = nBufferSize;
		}
		else if(nFileId>=NVRAM_EF_LAST_LID_VENDOR&&nFileId<NVRAM_EF_LAST_LID_CUST)
		{
		    //here: copy from NULL
		    if(logical_data_item_table_cust[nFileId - NVRAM_EF_LAST_LID_VENDOR].default_value != NULL)
		    {

				SUL_MemCopy8 (pBuffer, logical_data_item_table_cust[nFileId - NVRAM_EF_LAST_LID_VENDOR].default_value, nBufferSize);
				*pError =  NVRAM_READ_SUCCESS;
				retValue = nBufferSize;
		    }
		    else
		    {
				*pError =  NVRAM_READ_FAIL;
				retValue = 0;
		    }

		}
		else if (nFileId>NVRAM_EF_CUSTPACK_BASE&&nFileId<NVRAM_EF_LAST_LID_CUSTPACK)   
		{
		    if(logical_data_item_table_custpack[nFileId - NVRAM_EF_CUSTPACK_BASE].default_value != NULL)
                 {

			SUL_MemCopy8 (pBuffer, logical_data_item_table_custpack[nFileId - NVRAM_EF_CUSTPACK_BASE].default_value, nBufferSize);
			*pError =  NVRAM_READ_SUCCESS;
			retValue = nBufferSize;
		    }
		    else
		    {
			*pError =  NVRAM_READ_FAIL;
			retValue = 0;
		    }
	

		}
		else if (NVRAM_PHB_NVRAM_LID == nFileId)
		{

			handle = FS_Open(name_uncode, FS_O_CREAT|FS_O_RDWR, 0);
			if (handle >= 0)
			{
				ret = FS_Seek(handle, 0, FS_SEEK_SET);
				if ( ret < 0)
				{
					mmi_trace(g_sw_NVRAM, "Call FS_Seek() failed! return=%d, filehandle=%d\n", ret, handle);
				}	
				else
				{
					ret = FS_Read(handle, pBuffer, nBufferSize);
					if (ret < 0)
					{
						mmi_trace(g_sw_NVRAM, "FS_read error = %d, filehandle=%d!\n",ret, handle);
						if (FS_Write(handle, pBuffer, nBufferSize) != nBufferSize)
							mmi_trace(g_sw_NVRAM, "FS_Write error!\n");
					}
				}
				retValue = ret;

			}
			else if(ERR_FS_DISK_FULL!=handle)
			{
				mmi_trace(g_sw_NVRAM, "FS_creat***failed: %d, fileName %s!\n", handle, fileName);
                            g_tst_err_code = handle;
				MMI_ASSERT(0);	
			}        	
			else
				mmi_trace(g_sw_NVRAM, "FS_creat***failed: %d, fileName %s!\n", handle, fileName);

		}
		else if (nFileId >= NVRAM_DIALEDCALL_NVRAM_LID && nFileId <= NVRAM_VOICEMAIL2_NVRAM_LID)
		{
			handle = FS_Open(name_uncode, FS_O_RDWR , 0);
			if(handle >= 0)
			{
				ret = FS_Seek(handle, 0, FS_SEEK_SET);
				if ( ret < 0)
				{
					mmi_trace(g_sw_NVRAM, "Call FS_Seek() failed! return=%d, filehandle=%d\n", ret, handle);
				}	
				else
				{
					ret = FS_Read(handle, pBuffer, nBufferSize);
					if(ret<0)
					 	mmi_trace(g_sw_NVRAM, "FS_Read***failed =%d,filehandle=%d", ret, handle);
					if((ret!=ERR_FS_BAD_FD)&&(ret<0))
					{
                                        g_tst_err_code = ret;
                                        MMI_ASSERT(0);		
					}
					
				}
				retValue = ret;
			}
			else
			{
				mmi_trace(g_sw_NVRAM, TSTXT("FS_creat***failed: %d, fileName %s!\n"), handle, fileName);
			}
		}
		
		else if (nFileId == NVRAM_EF_PHB_SOMEFIELDS_LID)
		{  
			SUL_MemCopy8 (pBuffer, logical_data_item_table_cust[14].default_value, 229);
			*pError =  NVRAM_READ_SUCCESS;
			retValue = nBufferSize;
		}
	}
	if(handle >= 0)
	{
		ret = FS_Close(handle);	
		if (ret != 0)
		{
		 	mmi_trace(g_sw_NVRAM, "FS_Close() failed ret =%d, filehandle=%d", ret, handle);
                     g_tst_err_code = ret;
			MMI_ASSERT(0);
		}
	}
	return retValue;

}

S32 ReadAllPHBFiled(void *pBuffer, U32 nSeekPos, U32 nReadBufferSize,S16 *pError)
{
	S8 fileName[100];
	S8 name_uncode[100];
	INT32 handle= -1;
	INT32 iBytes = 0;
	INT32 retValue = -1;
	INT32 ret = 0;


	memset(fileName, 0, sizeof(fileName));
	memset(name_uncode, 0, sizeof(name_uncode));

	SUL_StrPrint(fileName, "/%s/NVRAM_%d_%d_FILE.dat",NVRAM_DEFAULT_FOLDER, NVRAM_EF_PHB_SOMEFIELDS_LID, 1);
  
	AnsiiToUnicodeString(name_uncode, fileName);
	handle = FS_Open(name_uncode, FS_O_RDONLY, 0);
	if(( handle <0) && ( handle != ERR_FS_FILE_NOT_EXIST) && ( handle != ERR_FS_NO_DIR_ENTRY) ) //add gdm 090609
	{
		mmi_trace(g_sw_NVRAM, "call FS_Open() failed!handle = %d, filename =%s\n", handle, fileName);
              g_tst_err_code = handle;
		MMI_ASSERT(0);
	}
  
	if(handle >= 0)
	{
		ret = FS_Seek(handle, nSeekPos, FS_SEEK_SET);

		if ( ret < 0)
		{
			mmi_trace(g_sw_NVRAM, "Call FS_Seek() failed! return=%d, filehandle=%d\n", ret, handle);
		}
			
		else
		{			
			iBytes = FS_Read(handle, pBuffer, nReadBufferSize);

			if(iBytes >= 0)
			{
				*pError = NVRAM_READ_SUCCESS;
				retValue = iBytes;
			}
			else if( iBytes != ERR_FS_BAD_FD)
			{
				mmi_trace(g_sw_NVRAM, "ReadAllPHBFiled FS_Read () NVRAM_EF_PHB_SOMEFIELDS_LID file failed! ret=%d,filehandle=%d\n", iBytes, handle);
                            g_tst_err_code = iBytes;
				MMI_ASSERT(0);
			}
			else
			{
				mmi_trace(g_sw_NVRAM, "read file failed! %d\n", iBytes);
			}
		}
	}

	if(handle >= 0)
	{
		ret = FS_Close(handle);	
		if (ret != 0)
		{
		 	mmi_trace(g_sw_NVRAM, "FS_Close() failed ret =%d, filehandle=%d", ret, handle);
                     g_tst_err_code = ret;
			MMI_ASSERT(0);
		}
	}
	return retValue;

}


/**************************************************************

	FUNCTION NAME		: WriteRecord

  	PURPOSE				: User level API for writing record in the file

	INPUT PARAMETERS	: U16 nFileId		: file LID
						  U16 nRecordId		: Record index in file
						  U16 nBufferSize	: Size of record to be write
											  it should be same as file 
											  record size
						  void *pBuffer		: Buffer in which data need to 
											  be retrieved,caller will allocate
											  memory for this.	

	OUTPUT PARAMETERS	: 
						  U16 *pError		: Error returned from PS	

	RETURNS				: BOOL

 

**************************************************************/
S32 WriteRecordInt(U16 nFileId,U16 nRecordId,void *pBuffer,U16 nBufferSize,S16 *pError, S8* filename, S32 lineNumber)
{   
	S8 fileName[100];
	S8 name_uncode[100];
	INT32   handle = -1;
	//INT32 iBytes = 0;
	S32 retValue = -1;
	S32 ret = 0;

	memset(fileName, 0, sizeof(fileName));
	memset(name_uncode, 0, sizeof(name_uncode));
	
	SUL_StrPrint(fileName, "/%s/NVRAM_%d_%d_FILE.dat",NVRAM_DEFAULT_FOLDER, nFileId, nRecordId);
	AnsiiToUnicodeString(name_uncode, fileName);
	handle = FS_Open(name_uncode, FS_O_WRONLY | FS_O_CREAT, 0);

	if(( handle < 0) && (handle != ERR_FS_DISK_FULL))
	{
		Trace( "WriteRecordInt: FS_Open() failed. The fileName = %s hasn't been opened, return value = %d.\n", fileName,handle);
		*pError = NVRAM_WRITE_FAIL;
			
//		MMI_ASSERT(0);
	}

	if( handle < 0)
	{
		Trace( "WriteRecordInt: FS_Open() failed. The fileName = %s hasn't been opened, return value = %d.\n", fileName,handle);
		*pError = NVRAM_WRITE_FAIL;
	}
	else
	{

	    if (NVRAM_PHB_NVRAM_LID == nFileId )
	    {
	      //COS_Sleep(200);
	       // index = nBufferSize/sizeof(CFW_PBK_ENTRY_INFO);
			ret = FS_Seek(handle, nBufferSize, FS_SEEK_SET);
			if ( ret < 0)
			{
				Trace( "Call FS_Seek() failed! return=%d, filehandle=%d, offset=%d\n", ret, handle, nBufferSize);
			}
			else
			{
				ret = FS_Write(handle, pBuffer, sizeof(CFW_PBK_ENTRY_INFO));

				if (ret != sizeof(CFW_PBK_ENTRY_INFO))
				{
				  Trace( " write phonebook data fail %d,handle=%d\n", ret, handle);
				}
				if ((ret != sizeof(CFW_PBK_ENTRY_INFO)) && (ret !=ERR_FS_DISK_FULL)&&( ret !=ERR_FS_BAD_FD))
				{
				  g_tst_err_code = ret;
				  MMI_ASSERT(0);
				}
			}
		
	    }
			
	    else if ( nFileId >= NVRAM_DIALEDCALL_NVRAM_LID && nFileId <= NVRAM_SMSPROSC_NVRAM_LID)
	    {
	        mmi_trace(g_sw_NVRAM, " write calllog nvram data\n");
	        ret = FS_Seek(handle, 0, FS_SEEK_SET);
		if ( ret < 0)
		{
			mmi_trace(g_sw_NVRAM, "Call FS_Seek() failed! return=%d, filehandle=%d%d\n", ret, handle);
		}
		else
		{
		        ret = FS_Write(handle, pBuffer, nBufferSize);

		        if (ret != nBufferSize)
		        {
		            Trace( " write calllog or sim or sms or phone data fail = %d \n", ret);
		        }
			 if ((ret != nBufferSize) && (ret !=ERR_FS_DISK_FULL))
			 {
                               g_tst_err_code = ret;
                               MMI_ASSERT(0);
			 }
		}
	    }
		
	    else if (nFileId == NVRAM_EF_PHB_SOMEFIELDS_LID)
	    {
	     // COS_Sleep(200);
	      //  index = nBufferSize/229;
				ret = FS_Seek(handle, nBufferSize, FS_SEEK_SET);
		if ( ret < 0)
		{
			mmi_trace(g_sw_NVRAM, "Call FS_Seek() failed! return=%d, filehandle=%d, offset=%d\n", ret, handle, nBufferSize);
		}
		else
		{
		        ret = FS_Write(handle, pBuffer, 229);

		        if (ret != 229)
		        {
		          Trace( " write phonebook data fail %d\n", ret);
		        }
			if ((ret !=229) && (ret !=ERR_FS_DISK_FULL)&&(ret !=ERR_FS_BAD_FD))
			{
			     g_tst_err_code = ret;
			     MMI_ASSERT(0);
			}
		}
	    }
		
	    else
	    {
    		ret = FS_Write(handle, pBuffer, nBufferSize);

	        if (ret != nBufferSize)
	        {
        	          Trace( " write small data fail = %d,handle = %d \n", ret,handle);
	        }
			if ((ret !=nBufferSize) && (ret !=ERR_FS_DISK_FULL))
			{
			     g_tst_err_code = ret;
			     MMI_ASSERT(0);
			}
	    }
				
		if(ret >= 0)
		{
			mmi_trace(g_sw_NVRAM, " ret = %d, sizeof(buffer) = %d\n", ret, nBufferSize);
			mmi_trace(g_sw_NVRAM, "  has been write into the file! TURE\n");
			*pError =  NVRAM_WRITE_SUCCESS;
			 retValue = ret;
		  }
	 	else
		  {
	  		mmi_trace(g_sw_NVRAM, "Write file failed!\n");
		  	*pError = NVRAM_WRITE_FAIL;
			 retValue = ret;
		  }		
	}
	if(handle >= 0)
	{
		ret = FS_Close(handle);
		if (ret != 0)
		{
		 	Trace( "FS_Close() failed ret =%d, filehandle=%d", ret, handle);
                     g_tst_err_code = ret;
			MMI_ASSERT(0);
		}
	}
 	return retValue;
	//end: add by panxu 2006-8-23
 
}

/**************************************************************

	FUNCTION NAME		: DeleateRecord

  	PURPOSE				: User level API for Deleting record from the file

	INPUT PARAMETERS	: U16 nFileId		: file LID
						  U16 nRecordId		: Record index in file
						  
	OUTPUT PARAMETERS	: U16 *pError		: Error returned from PS	

	RETURNS				: BOOL

 

**************************************************************/
BOOL DeleteRecord( U16 nFileId, U16 nRecordId,S16 *pError)
{
     #if 0
	S8 fileName[100];
	S32	retValue = -1;

	sprintf(fileName,"%s%d_%d",NVRM_FILES_PATH,nFileId,nRecordId);
	
	/*Remove file*/
	retValue = remove(fileName);
	if(retValue == 0)
	{
		return MMI_TRUE;
	}
	else
	{
		return MMI_FALSE;
	}
  #endif
  
   

       S8     fileName[100];
	S32	  retValue = -1;

	SUL_StrPrint(fileName,"%s%d_%d",NVRM_FILES_PATH,nFileId,nRecordId);
	
	/*Remove file*/
	retValue = pfdelete(fileName);
	if(retValue == 0)
	{
		return MMI_TRUE;
	}
	else
	{
		return MMI_FALSE;
	}

	 
	
}
 
#else //MMI_ON_HARDWARE_P
/**************************************************************

	FUNCTION NAME		: ReadRecordInt()

  	PURPOSE				: User level API for reading record from the file

	INPUT PARAMETERS	: U16 nFileId		: file LID
						  U16 nRecordId		: Record index in file
						  U16 nBufferSize	: Size of record to be read
											  it should be same as file 
											  record size

	OUTPUT PARAMETERS	: void *pBuffer		: Buffer in which data need to 
											  be retrieved,caller will allocate
											  memory for this.
						  U16 *pError		: Error returned from PS	

	RETURNS				: BOOL

 

**************************************************************/
S32 ReadRecordInt(U16 nFileId,U16 nRecordId, void *pBuffer, U16 nBufferSize,S16 *pError, S8* filename, S32 lineNumber)
{
  S8 fileName[_MAX_PATH];
  U32 iBytesRead;
  HANDLE handle;
  S32	retValue	=	-1;
  U32 temp = 0;
  CHAR buf[2*_MAX_PATH] = "\0";
  
  TCHAR szFileName[_MAX_PATH] = {0};
  TCHAR szDir[_MAX_PATH] = {0};
  TCHAR szDriver[_MAX_PATH] = {0};

  DWORD dwAttr;

  GetModuleFileName(NULL, szFileName, _MAX_PATH);
  _splitpath(szFileName, szDriver, szDir, NULL, NULL);
  
  sprintf(buf,"%s%s%s",szDriver, szDir,"nvramdata");

  dwAttr = GetFileAttributes(buf);
  if(!((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)))
  {
    CreateDirectory(buf,NULL);
  }
  sprintf(fileName,"%s\\nvram%d_%d",buf,nFileId,nRecordId);

	handle = CreateFile(
						fileName ,
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL,
						0);

	if(handle == INVALID_HANDLE_VALUE)
	{
           char temp_write[255];
                U32 iBytes=1;
                //Create new file
	        handle = CreateFile(
						   fileName ,
						   GENERIC_READ|GENERIC_WRITE,
						   FILE_SHARE_READ,
						   NULL,
						   OPEN_ALWAYS,
						   FILE_ATTRIBUTE_NORMAL,
						   0);
		memset(temp_write, 0xFF, sizeof(temp_write));
		WriteFile(handle, temp_write, sizeof(temp_write), &iBytes, NULL);
		CloseHandle(handle);

		/*Do get Last error*/
		// if((temp = GetLastError())	 == ERROR_FILE_NOT_FOUND )		 
		 {

			 memset(pBuffer,0xFF,nBufferSize);
		 }
		*pError = NVRAM_READ_FAIL;
	}
	else
	{
		if(ReadFile(handle,pBuffer,nBufferSize,&iBytesRead,NULL))
		{
			retValue = iBytesRead;
			*pError = NVRAM_READ_SUCCESS;
		}
		
		CloseHandle(handle);

	}
	return retValue;
}

/**************************************************************

	FUNCTION NAME		: WriteRecord

  	PURPOSE				: User level API for writing record in the file

	INPUT PARAMETERS	: U16 nFileId		: file LID
						  U16 nRecordId		: Record index in file
						  U16 nBufferSize	: Size of record to be write
											  it should be same as file 
											  record size
						  void *pBuffer		: Buffer in which data need to 
											  be retrieved,caller will allocate
											  memory for this.	

	OUTPUT PARAMETERS	: 
						  U16 *pError		: Error returned from PS	

	RETURNS				: BOOL

 

**************************************************************/
S32 WriteRecordInt(U16 nFileId,U16 nRecordId,void *pBuffer,U16 nBufferSize,S16 *pError, S8* filename, S32 lineNumber)
{
  S8 fileName[_MAX_PATH];
  U32 iBytes;
  HANDLE handle;
  S32	retValue	=	-1;
  CHAR buf[_MAX_PATH] = "\0";
  
  TCHAR szFileName[_MAX_PATH] = {0};
  TCHAR szDir[_MAX_PATH] = {0};
  TCHAR szDriver[_MAX_PATH] = {0};

  DWORD dwAttr;

  GetModuleFileName(NULL, szFileName, _MAX_PATH);
  _splitpath(szFileName, szDriver, szDir, NULL, NULL);
  
  sprintf(buf,"%s%s%s",szDriver, szDir,"nvramdata");
  dwAttr = GetFileAttributes(buf);
  if(!((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)))
  {
    CreateDirectory(buf,NULL);
  }
  sprintf(fileName,"%s\\nvram%d_%d",buf,nFileId,nRecordId);

	handle = CreateFile(
						fileName ,
						GENERIC_READ|GENERIC_WRITE,
						FILE_SHARE_READ,
						NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						0);
	if(handle == INVALID_HANDLE_VALUE)
	{
		*pError = NVRAM_WRITE_FAIL;
	}
	else
	{
		if(WriteFile(handle,pBuffer,nBufferSize,&iBytes,NULL))
		{
			retValue = iBytes;
			*pError =  NVRAM_WRITE_SUCCESS;
		}
		
		CloseHandle(handle);

	}
	return retValue;

}

/**************************************************************

	FUNCTION NAME		: DeleateRecord

  	PURPOSE				: User level API for Deleting record from the file

	INPUT PARAMETERS	: U16 nFileId		: file LID
						  U16 nRecordId		: Record index in file
						  
	OUTPUT PARAMETERS	: U16 *pError		: Error returned from PS	

	RETURNS				: BOOL

 

**************************************************************/
pBOOL DeleteRecord( U16 nFileId, U16 nRecordId,S16 *pError)
{
  S8 fileName[_MAX_PATH];
  S32	retValue = -1;
  CHAR buf[_MAX_PATH] = "\0";
  
  TCHAR szFileName[_MAX_PATH] = {0};
  TCHAR szDir[_MAX_PATH] = {0};
  TCHAR szDriver[_MAX_PATH] = {0};
  
  DWORD dwAttr;

  GetModuleFileName(NULL, szFileName, _MAX_PATH);
  _splitpath(szFileName, szDriver, szDir, NULL, NULL);
  
  sprintf(buf,"%s%s%s",szDriver, szDir,"nvramdata");
  dwAttr = GetFileAttributes(buf);
  if(!((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)))
  {
    CreateDirectory(buf,NULL);
  }
  sprintf(fileName,"%s\\nvram%d_%d",buf,nFileId,nRecordId);
	
	/*Remove file*/
	retValue = remove(fileName);
	if(retValue == 0)
	{
		return MMI_TRUE;
	}
	else
	{
		return MMI_FALSE;
	}
}

S32 ReadAllPHBFiled(void *pBuffer, U32 nSeekPos, U32 nReadBufferSize,S16 *pError)
{
    char fileName[_MAX_PATH];
    S32	retValue = -1;
    char buf[_MAX_PATH] = "\0";

    char szFileName[_MAX_PATH] = {0};
    char szDir[_MAX_PATH] = {0};
    char szDriver[_MAX_PATH] = {0};

    DWORD dwAttr;

    FILE* handle = NULL;
    size_t iBytes;

    GetModuleFileNameA(NULL, szFileName, _MAX_PATH);
    _splitpath(szFileName, szDriver, szDir, NULL, NULL);

    sprintf(buf,"%s%s%s",szDriver, szDir,"nvramdata");
    dwAttr = GetFileAttributes(buf);
    if(!((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)))
    {
        CreateDirectoryA(buf,NULL);
    }
    sprintf(fileName,"%s\\nvram%d_%d",buf,NVRAM_EF_PHB_SOMEFIELDS_LID,1);

    handle = fopen(fileName, "rb");
    mmi_trace(g_sw_NVRAM, "ReadAllPHBFiled openfile return value = %d\n", handle);
  
    if(handle)
    {
        mmi_trace(g_sw_NVRAM, "ReadAllPHBFiled open %s OK!!!\n", fileName);

    	fseek(handle, nSeekPos, FS_SEEK_SET);
    	iBytes = fread(pBuffer,1,nReadBufferSize,handle);

    	if(iBytes >= 0)
    	{
    		*pError = NVRAM_READ_SUCCESS;
    		retValue = iBytes;
    	}
    	else
    	{
    		mmi_trace(g_sw_NVRAM, "ReadAllPHBFiled NVRAM_EF_PHB_SOMEFIELDS_LID file failed! %d\n", iBytes);
    	}
    }
    else
    {
        memset(pBuffer,0xFF,nReadBufferSize);
        handle = fopen(fileName, "wb");
        if (handle)
        {
            fwrite(pBuffer,1,nReadBufferSize,handle);
        }
        *pError =  NVRAM_READ_SUCCESS;
        retValue = nReadBufferSize;
    }
    
    if(handle)
    {
        fclose(handle);	
    }
    
    return retValue;

}

#endif //MMI_ON_HARDWARE_P
/**************************************************************

	FUNCTION NAME		: ReadValueInt()

  	PURPOSE				: User level API for 1 byte , 2 byte and 8 byte 

	INPUT PARAMETERS	: nDataItemId		: Data Item ID
						  void *pBuffer		: Buffer in which data need to 
											  be retrieved,caller will allocate
											  memory for this.	

	OUTPUT PARAMETERS	: 
						  U16 *pError		: Error returned from PS	

	RETURNS				: BOOL

 

**************************************************************/


S32 ReadValueInt( U8 nDataItemId, void *pBuffer, U8 nDataType , S16 *pError,S8* fileName, S32 lineNumber)
{
 
	S32 status = -1;
	S16 error = -1;
	
    // MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_READ_VAL_INT_HDLR, nDataItemId, nDataType));

	switch(nDataType)
	{
		
		case DS_BYTE:
		{
			/* second time reading, it alwasy return one request item */
			if(byteDataReadFlag)
			{
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_READ_VAL_INT_INFO_HDLR, nDataType));
				/* Data is cached no need to read from NVRAM*/
				SUL_MemCopy8(pBuffer,&byte_data[nDataItemId*nDataType],nDataType);
				status = DS_BYTE;
				error = NVRAM_READ_SUCCESS;
			}
			/* process first time reading all data and return first request item data */
			else
			{
				/* Read data from the NVRAM file */
				/*NVRAM_CACHED_FILE_LID is defined in Pluto NVRAM files, first time to read all out */
				U8 tempBuffer[256];
				status = ReadRecordInt(NVRAM_EF_CACHE_BYTE_LID, 1, tempBuffer, sizeof(tempBuffer), &error,fileName,lineNumber);
				if(status == -1)
				{
					//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_READ_VAL_INT_HDLR));
					*pError = -1;
					return status;
				}
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_READ_VAL_INT_INFO2_HDLR, nDataType));
				/* copy out all total reading data */
				SUL_MemCopy8(byte_data, tempBuffer, sizeof(tempBuffer));
				byteDataReadFlag = 1;
				/* return first request item data */
				SUL_MemCopy8(pBuffer,&byte_data[nDataItemId*nDataType],nDataType);
				status = DS_BYTE;
				error = NVRAM_READ_SUCCESS;

			}
			break;
		}/* End of case DS_BYTE*/

		case DS_SHORT:
		{
			if(uintDataReadFlag)
			{
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_READ_VAL_INT_INFO_HDLR, nDataType));
				/* Data is cached no need to read from NVRAM*/
				SUL_MemCopy8(pBuffer,&uint_data[nDataItemId*nDataType],nDataType);
				status = DS_SHORT;
				error = NVRAM_READ_SUCCESS;
			}
			else
			{
				/* Read data from the NVRAM file */
				/*NVRAM_CACHED_FILE_LID is defined in Pluto NVRAM files*/
				U8 tempBuffer[256];
				status = ReadRecordInt( NVRAM_EF_CACHE_SHORT_LID, 1, tempBuffer, sizeof(tempBuffer), &error, fileName, lineNumber);
				if(status == -1)
				{
					//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_READ_VAL_INT_HDLR));
					*pError = -1;
					return status;
				}
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_READ_VAL_INT_INFO2_HDLR, nDataType));
				SUL_MemCopy8(uint_data,tempBuffer,256);
				uintDataReadFlag = 1;
				SUL_MemCopy8(pBuffer,&uint_data[nDataItemId*nDataType],nDataType);
				status = DS_SHORT;
				error = NVRAM_READ_SUCCESS;

			}

			break;
		}/* End of case DS_SHORT*/

		case DS_DOUBLE:
		{
			if(doubleDataReadFlag)
			{
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_READ_VAL_INT_INFO_HDLR, nDataType));
				/* Data is cached no need to read from NVRAM*/
				SUL_MemCopy8(pBuffer,&double_data[nDataItemId*nDataType],nDataType);
				status = DS_DOUBLE;
				error = NVRAM_READ_SUCCESS;
			}
			else
			{
				/* Read data from the NVRAM file */
				/*NVRAM_CACHED_FILE_LID is defined in Pluto NVRAM files*/
				U8 tempBuffer[256];
				status = ReadRecordInt( NVRAM_EF_CACHE_DOUBLE_LID,1,tempBuffer,256, &error,fileName,lineNumber);
				if(status == -1)
				{
					//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_READ_VAL_INT_HDLR));
					*pError = -1;
					return status;
				}
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_READ_VAL_INT_INFO2_HDLR, nDataType));
				SUL_MemCopy8(double_data,tempBuffer,256);
				doubleDataReadFlag = 1;
				SUL_MemCopy8(pBuffer,&double_data[nDataItemId*nDataType],nDataType);
				status = DS_DOUBLE;
				error = NVRAM_READ_SUCCESS;

			}
			break;
		}/* End of case DS_DOUBLE*/
	}

		*pError = error;
 return status;
}







/**************************************************************

	FUNCTION NAME		: WriteValue

  	PURPOSE				: User level API to write 1 byte , 2 byte and 8 byte 

	INPUT PARAMETERS	: nDataItemId		: Data Item ID
						  void *pBuffer		: Buffer in which data need to 
											  be retrieved,caller will allocate
											  memory for this.	

	OUTPUT PARAMETERS	: 
						  U16 *pError		: Error returned from PS	

	RETURNS				: BOOL

 

**************************************************************/

S32 WriteValueInt( U8 nDataItemId, void *pBuffer, U8 nDataType , S16 *pError,S8* fileName, S32 lineNumber)
{
	
	S16 error = -1;
	S32 status = -1;

	//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_WRITE_VAL_INT_HDLR, nDataItemId, nDataType));

	switch(nDataType)
	{
		case DS_BYTE:
		{
			/* second time access, it always sets the request item */
			if(byteDataReadFlag)
			{
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_WRITE_VAL_INT_INFO_HDLR, nDataType));
				/* Data is cached no need to read from NVRAM*/
				SUL_MemCopy8(&byte_data[nDataItemId*nDataType],pBuffer,nDataType);
				/* write into NVRAM module, if not wait for falshing directly */ 
				if (!byteDataWriteFlashFlag)
				{
					status = WriteRecordInt(NVRAM_EF_CACHE_BYTE_LID,1,byte_data,256, &error,fileName,lineNumber);
					if(status == -1)
					{
						//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
						*pError = -1;
						return status;
					}
				}
				status = DS_BYTE;
				error = NVRAM_WRITE_SUCCESS;
			}
			/* process first time access all data and return first write item data */
			else
			{
				/* Read all data from the NVRAM file */
				/*NVRAM_CACHED_FILE_LID is defined in Pluto NVRAM files*/
				status = ReadRecordInt(NVRAM_EF_CACHE_BYTE_LID,1,byte_data,256, &error,fileName,lineNumber);
				if(status == -1)
				{
					//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_READ_VAL_INT_HDLR));
					*pError = -1;
					return status;
				}
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_WRITE_VAL_INT_INFO2_HDLR, nDataType));
				SUL_MemCopy8(&byte_data[nDataItemId*nDataType],pBuffer,nDataType);
				/* write into NVRAM module, if not wait for falshing directly */ 
				if (!byteDataWriteFlashFlag)
				{
					status = WriteRecordInt(NVRAM_EF_CACHE_BYTE_LID, 1, byte_data, 256, &error, fileName,lineNumber);
				}
				byteDataReadFlag = 1;
				status = DS_BYTE;
				error = NVRAM_WRITE_SUCCESS;
			}
			break;
		}/* End of case DS_BYTE*/


		case DS_SHORT:
		{
			if(uintDataReadFlag)
			{
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_WRITE_VAL_INT_INFO_HDLR, nDataType));
				/* Data is cached no need to read from NVRAM*/
				SUL_MemCopy8(&uint_data[nDataItemId*nDataType],pBuffer,nDataType);
				/* write into NVRAM module, if not wait for falshing directly */ 
				if (!uintDataWriteFlashFlag)
				{
					status = WriteRecordInt(NVRAM_EF_CACHE_SHORT_LID,1,uint_data,256, &error,fileName,lineNumber);
					if(status == -1)
					{
						//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
						*pError = -1;
						return status;
					}
				}	
				status = DS_SHORT;
				error = NVRAM_WRITE_SUCCESS;

			}
			else
			{
				/* Read data from the NVRAM file */
				/*NVRAM_CACHED_FILE_LID is defined in Pluto NVRAM files*/
				status = ReadRecordInt(NVRAM_EF_CACHE_SHORT_LID,1,uint_data,256, &error,fileName,lineNumber);
				if(status == -1)
				{
					//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_READ_VAL_INT_HDLR));
					*pError = -1;
					return status;
				}
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_WRITE_VAL_INT_INFO2_HDLR, nDataType));
				SUL_MemCopy8(&uint_data[nDataItemId*nDataType],pBuffer,nDataType);
				/* write into NVRAM module, if not wait for falshing directly */ 
				if (!uintDataWriteFlashFlag)
				{
					WriteRecordInt(NVRAM_EF_CACHE_SHORT_LID,1,uint_data,256, &error,fileName,lineNumber);
				}
				uintDataReadFlag = 1;
				status = DS_SHORT;
				error = NVRAM_WRITE_SUCCESS;
			}
			break;
		}/* End of case DS_SHORT*/

		case DS_DOUBLE:
		{
			if(doubleDataReadFlag)
			{
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_WRITE_VAL_INT_INFO_HDLR, nDataType));
				/* Data is cached no need to read from NVRAM*/
				SUL_MemCopy8(&double_data[nDataItemId*nDataType],pBuffer,nDataType);
				/* write into NVRAM module, if not wait for falshing directly */ 
				if (!doubleDataWriteFlashFlag)
				{
					status = WriteRecordInt(NVRAM_EF_CACHE_DOUBLE_LID,1,double_data,256, &error,fileName,lineNumber);
					if(status == -1)
					{
						//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
						*pError = -1;
						return status;
					}
				}
				status = DS_DOUBLE;
				error = NVRAM_WRITE_SUCCESS;
			}
			else
			{
				//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_WRITE_VAL_INT_INFO_HDLR, nDataType));
				/* Read data from the NVRAM file */
				/*NVRAM_CACHED_FILE_LID is defined in Pluto NVRAM files*/
				status = ReadRecordInt(NVRAM_EF_CACHE_DOUBLE_LID,1,double_data,256, &error,fileName,lineNumber);
				if(status == -1)
				{
					//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_READ_VAL_INT_HDLR));
					*pError = -1;
					return status;
				}
				SUL_MemCopy8(&double_data[nDataItemId*nDataType],pBuffer,nDataType);
				/* write into NVRAM module, if not wait for falshing directly */ 
				if (!doubleDataWriteFlashFlag)
				{
					status = WriteRecordInt(NVRAM_EF_CACHE_DOUBLE_LID,1,double_data,256, &error,fileName,lineNumber);
					if(status == -1)
					{
						//MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
						*pError = -1;
						return status;
					}
				}
				status = DS_DOUBLE;
				error = NVRAM_WRITE_SUCCESS;
				doubleDataReadFlag = 1;
			}
			break;
		}/* End of case DS_DOUBLE*/

	}/* End of switch */

	*pError = error;
	return status;
}
#ifndef MMI_ON_WIN32
/**************************************************************

	FUNCTION NAME		: Mmi_WriteNvramRecord
  PURPOSE				: for mmi write nvaram data 
	INPUT PARAMETERS	: nFileId		: nFileId Item ID
						          void *pBuffer		: Buffer in which data need to 
											  be retrieved,caller will allocate
											  memory for this.	
  auther :  hansl
  date: 2007/6/15
	OUTPUT PARAMETERS	: 
	U16 *pError		: Error returned from PS	
	RETURNS				: S32
 

**************************************************************/

S32 Mmi_WriteNvramRecord(U16 nFileId,U16 nRecordId,void *pBuffer,U16 nBufferSize,S16 *pError, S8* filename, S32 lineNumber)
{
  S8   fileName[100];
	INT32   handle = -1;
	INT32 iBytes = 0;
	S32 retValue = -1;

	memset(fileName, 0, sizeof(fileName));
	
	SUL_StrPrint(fileName, "/NVRAM_%d_%d_FILE.dat", nFileId, nRecordId);	
	handle = FS_Open(fileName, FS_O_WRONLY | FS_O_CREAT, 0);

	if(handle < 0)
	{
			mmi_trace(g_sw_NVRAM, "Write_mmi_nvramRecord: The fileName = %s hasn't been opened, Failed%0x, %d \n", fileName, SUL_GetLastError(),handle);
			*pError = NVRAM_WRITE_FAIL;
	
	}
	else
	{
		iBytes = FS_Write(handle, pBuffer, nBufferSize);
    
		if(iBytes == nBufferSize)
	  {
	  	mmi_trace(g_sw_NVRAM, "Write mmi nvramfile failed! %d\n", iBytes);
			*pError =  NVRAM_WRITE_SUCCESS;
			retValue = iBytes;
	  }
	 	else
	  {
	  		mmi_trace(g_sw_NVRAM, "Write mmi nvramfile failed!\n");
		  	*pError = NVRAM_WRITE_FAIL;
	  }
    
	}
	
	FS_Close(handle);
 	return retValue;
}

/**************************************************************

	FUNCTION NAME		: Mmi_ReadNvramRecord
  PURPOSE				: for mmi read nvaram data 
	INPUT PARAMETERS	: nFileId		: nFileId Item ID
						          void *pBuffer		: Buffer in which data need to 
											  be retrieved,caller will allocate
											  memory for this.	
  auther :  hansl
  date: 2007/6/15
	OUTPUT PARAMETERS	: 
	U16 *pError		: Error returned from PS	
	RETURNS				: S32
 

**************************************************************/

S32 Mmi_ReadNvramRecord(U16 nFileId,U16 nRecordId, void *pBuffer, U16 nBufferSize,S16 *pError, S8* filename, S32 lineNumber)
{
  S8 fileName[100];
	INT32 handle= -1;
	INT32 iBytes = 0;
	INT32 retValue = -1;
	
	memset(fileName, 0, sizeof(fileName));

	SUL_StrPrint(fileName, "/NVRAM_%d_%d_FILE.dat", nFileId, nRecordId);
	
	 handle = FS_Open(fileName, FS_O_RDONLY, 0);

	if(handle > 0)
	{		
		iBytes = FS_Read(handle, pBuffer, nBufferSize);
		if(iBytes == nBufferSize)
		{
			mmi_trace(g_sw_NVRAM, "mmi_read file datafailed! %d\n", iBytes);
			*pError = NVRAM_READ_SUCCESS;
			retValue = iBytes;
		}
		else
		{
			mmi_trace(g_sw_NVRAM, "mmi_read file datafailed!\n");
		}
	}
	else
	{
			mmi_trace(g_sw_NVRAM, "mmi_read file default data !\n");
		 if(nFileId < NVRAM_EF_LAST_LID_VENDOR)
     {
        
       *pError =  NVRAM_WRITE_SUCCESS;
		 		retValue = nBufferSize;
     }
     else if(nFileId>=NVRAM_EF_LAST_LID_VENDOR&&nFileId<NVRAM_EF_LAST_LID_CUST)
     {

        SUL_MemCopy8 (pBuffer, logical_data_item_table_cust[nFileId - NVRAM_EF_LAST_LID_VENDOR].default_value, nBufferSize);
       *pError =  NVRAM_WRITE_SUCCESS;
       retValue = nBufferSize;

     }
     else if (nFileId>NVRAM_EF_CUSTPACK_BASE&&nFileId<NVRAM_EF_LAST_LID_CUSTPACK)   
     {

       SUL_MemCopy8 (pBuffer, logical_data_item_table_custpack[nFileId - NVRAM_EF_CUSTPACK_BASE].default_value, nBufferSize);
       *pError =  NVRAM_WRITE_SUCCESS;
       retValue = nBufferSize;
      
     }
	}

	FS_Close(handle);
	
	return retValue;
}
#endif

/*****************************************************************************
* FUNCTION
*  SetFlashNVRAMCatchData
* DESCRIPTION
*   This function is to set the flag if want to flash the BYTE, SHORT, or DOUBLE of catch datas into NVRAM.
*
* PARAMETERS
*  a IN  is to enable to flash into NVRAM or not, TRUE is to flash into NVRAM, otherwise is false.
* RETURNS
*  Data set success or not.
* GLOBALS AFFECTED
*   VOID
*****************************************************************************/
 
#if 0
U8 SetFlashNVRAMCatchData(U8 nDataType, U8 l_DataWriteFlashFlag)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_FLASH_CATCH_DATA_HDLR, nDataType, l_DataWriteFlashFlag));
   
	switch(nDataType)
	{
	case DS_BYTE:
		byteDataWriteFlashFlag = l_DataWriteFlashFlag;
		break;
	case DS_SHORT:
		uintDataWriteFlashFlag = l_DataWriteFlashFlag;
		break;
	case DS_DOUBLE:
		doubleDataWriteFlashFlag = l_DataWriteFlashFlag;
		break;
	default:
		MMI_ASSERT(0);
		break;
	}
	return MMI_TRUE;
}

/*****************************************************************************
* FUNCTION
*  SetFlashAllNVRAMCatchData
* DESCRIPTION
*   This function is to set the flag if want to flash all the BYTE, SHORT, and DOUBLE of catch datas into NVRAM.
*
* PARAMETERS
*  a IN  is to enable to flash into NVRAM or not, TRUE is to flash into NVRAM, otherwise is false.
* RETURNS
*  Data set success or not.
* GLOBALS AFFECTED
*   VOID
*****************************************************************************/
U8 SetFlashAllNVRAMCatchData(U8 l_DataWriteFlashFlag)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_FLASH_CATCH_ALL_DATA_HDLR, l_DataWriteFlashFlag));
   
	byteDataWriteFlashFlag = l_DataWriteFlashFlag;
	uintDataWriteFlashFlag = l_DataWriteFlashFlag;
	doubleDataWriteFlashFlag = l_DataWriteFlashFlag;
	return MMI_TRUE;
}

/*****************************************************************************
* FUNCTION
*  WriteFlashNVRAMCatchData
* DESCRIPTION
*   This function is to write the BYTE, SHORT, DOUBLE of catch datas into NVRAM.
*
* PARAMETERS
*  a IN  is BYTE, SHORT, DOUBLE data type.
* RETURNS
*  Data write success or not.
* GLOBALS AFFECTED
*   VOID
*****************************************************************************/
S32 WriteFlashNVRAMCatchData(U8 nDataType)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 status = -1;
	S16 error = -1;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_WRITE_NVM_CATCH_DATA_HDLR, nDataType));

	switch(nDataType)
	{
	case DS_BYTE:
		if (byteDataWriteFlashFlag)
		{
			byteDataWriteFlashFlag = MMI_FALSE;
			status = WriteRecordInt(NVRAM_EF_CACHE_BYTE_LID, 1, byte_data, sizeof(byte_data), &error, __FILE__, __LINE__);
			if(status == -1)
			{
				MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
				MMI_ASSERT(0);
				return status;
			}
		}
		break;
	case DS_SHORT:
		if (uintDataWriteFlashFlag)
		{
			uintDataWriteFlashFlag = MMI_FALSE;
			status = WriteRecordInt(NVRAM_EF_CACHE_SHORT_LID, 1, uint_data, sizeof(uint_data), &error, __FILE__, __LINE__);
			if(status == -1)
			{
				MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
				MMI_ASSERT(0);
				return status;
			}
		}
		break;
	case DS_DOUBLE:
		if (doubleDataWriteFlashFlag)
		{
			doubleDataWriteFlashFlag = MMI_FALSE;
			status = WriteRecordInt(NVRAM_EF_CACHE_DOUBLE_LID, 1, double_data, sizeof(double_data), &error, __FILE__, __LINE__);
			if(status == -1)
			{
				MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
				MMI_ASSERT(0);
				return status;
			}
		}
		break;
	default:
		MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_WARN_NVM_WRITE_HDLR));
		MMI_ASSERT(0);
		break;
	}
	return status;

}

/*****************************************************************************
* FUNCTION
*  WriteFlashAllNVRAMCatchData
* DESCRIPTION
*   This function is to write all catch data into NVRAM.
*
* PARAMETERS
*  VOID
* RETURNS
*  Data write success or not.
* GLOBALS AFFECTED
*   VOID
*****************************************************************************/
S32 WriteFlashAllNVRAMCatchData(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 status = -1;
	S16 error = -1;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_INFO_NVM_FLASH_ALL_CATCH_DATA_HDLR));

	if (byteDataWriteFlashFlag)
	{
		byteDataWriteFlashFlag = MMI_FALSE;
		status = WriteRecordInt(NVRAM_EF_CACHE_BYTE_LID, 1, byte_data, 256, &error, __FILE__, __LINE__);
		if(status == -1)
		{
			MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
			MMI_ASSERT(0);
		return status;
		}
	}
	if (uintDataWriteFlashFlag)
	{
		uintDataWriteFlashFlag = MMI_FALSE;
		status = WriteRecordInt(NVRAM_EF_CACHE_SHORT_LID, 1, uint_data, 256, &error, __FILE__, __LINE__);
		if(status == -1)
		{
			MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
			MMI_ASSERT(0);
		return status;
		}
	}
	if (doubleDataWriteFlashFlag)
	{
		doubleDataWriteFlashFlag = MMI_FALSE;
		status = WriteRecordInt(NVRAM_EF_CACHE_DOUBLE_LID, 1, double_data, 256, &error, __FILE__, __LINE__);
		if(status == -1)
		{
			MMI_TRACE((MMI_TRACE_G1_FRM, MMI_FRM_ERROR_NVM_WRITE_VAL_INT_HDLR));
			MMI_ASSERT(0);
		return status;
		}
	}
	return status;
}



/*****************************************************************************
* FUNCTION
*  IsInNVRAMProcedure
* DESCRIPTION
*   This function is to check if in NVRAM procedure while loop
*
* PARAMETERS
*  VOID
* RETURNS
*  MMI_TRUE - in NVRAM procedure
*  MMI_FALSE - not in NVRAM procedure
* GLOBALS AFFECTED
*   VOID
*****************************************************************************/
U8 IsInNVRAMProcedure(void)
{
	PRINT_INFORMATION((" IsInNVRAMProcedure(%d)", in_nvram_procedure));
   return (in_nvram_procedure>0)?(U8)MMI_TRUE:(U8)MMI_FALSE;
}
#endif
 

