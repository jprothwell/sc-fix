#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"

#include "fs_type.h"
#include "frameworkstruct.h"
#include "filemgr.h"
#include "fs.h"
#include "fs_func.h"
#include "internal.h"
#include "settingprofile.h"
#include "imageviewerresdefs.h"
#include "filemanagerdef.h"
#include "java.h"
extern U8 g_mmi_java_storagepath;
const char * FlashStorageRoot = "E:/java/";
const char *TCardStorageRoot = "D:/java/";
#define MAX_FILE_HANDLE 10

typedef struct gcj_file_cache_t {
	unsigned char isOpened;
	unsigned char isCached;
	unsigned char isDirty;
	unsigned char *cache;
}GCJ_FILE_CACHE_T;

static GCJ_FILE_CACHE_T javaFileHandleList[MAX_FILE_HANDLE];
_GCJ_MUTEX_T fileMutex;

char *javaStorageRoot;
char *javaLibRoot = "E:/jlib/";
extern unsigned char fileResultSem;
extern BOOL g_hasMountedTFlashFS;

void gcj_creat_java_dir()
{
	int ret=-1;
	S8 unicodeName[150];
	if(g_mmi_java_storagepath == 1)javaStorageRoot = TCardStorageRoot;
	else if(g_mmi_java_storagepath == 0)javaStorageRoot = FlashStorageRoot; 
	else javaStorageRoot = FlashStorageRoot; 
	AnsiiToUnicodeString(unicodeName,javaStorageRoot);
	ret = MMI_FS_CreateDir(unicodeName);
	gcj_TraceOut(1,"gcj_creat_java_dir  %s,ret is %d\n",javaStorageRoot,ret);
}
void gcj_init_java_dir()
{
	int ret=-1;
	S8 unicodeName[150];
	if(g_mmi_java_storagepath == 1)javaStorageRoot = TCardStorageRoot;
	else if(g_mmi_java_storagepath == 0)javaStorageRoot = FlashStorageRoot; 
	else javaStorageRoot = FlashStorageRoot; 
	gcj_StubInitMutex(&fileMutex);
	AnsiiToUnicodeString(unicodeName,javaStorageRoot);
	ret = MMI_FS_CreateDir(unicodeName);
	gcj_TraceOut(1,"gcj_init_java_dir  %s,ret is %d\n",javaStorageRoot,ret);
	/*
	if (fileResultSem ==0)
		fileResultSem = sxr_NewSemaphore(1);
	gcj_TraceOut(1,"gcj_init_java_dir fileResultSem=%d\n",fileResultSem);
	*/
	if((g_mmi_java_storagepath == 1) &&(ret < 0 && ret != FS_FILE_EXISTS))
	{
    		j2me_send_alertuser((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD),UCS2Strlen((const S8*)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD)));
		javaStorageRoot = FlashStorageRoot;
		AnsiiToUnicodeString(unicodeName,javaStorageRoot);
		ret = MMI_FS_CreateDir(unicodeName);
      }
       
	
}

int gcj_CheckDir()
{
/*
	int ret=-1;
	S8 unicodeName[150];
	if(g_mmi_java_storagepath == 1)javaStorageRoot = TCardStorageRoot;
	else if(g_mmi_java_storagepath == 0)javaStorageRoot = FlashStorageRoot; 
	else javaStorageRoot = FlashStorageRoot; 
	AnsiiToUnicodeString(unicodeName,javaStorageRoot);
	ret = MMI_FS_Open(unicodeName,FS_OPEN_DIR);
	gcj_TraceOut(1,"gcj_CheckDir  %s,ret is %d,g_hasMountedTFlashFS=%d\n",javaStorageRoot,ret,g_hasMountedTFlashFS);
	if (ret != 0xff )//&& ret != FS_FILE_EXISTS)
*/	
	{
		if (g_mmi_java_storagepath == 1 && !g_hasMountedTFlashFS)
		{
			if (!gcj_StubIsForceToQuit())
			{
	    			//j2me_send_alertuser((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD),UCS2Strlen((const S8*)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD)));
				DisplayPopup((PU8)GetString(STR_ID_IMGVIEW_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, UI_POPUP_NOTIFYDURATION_TIME, (U8) WARNING_TONE);
				if (gcj_IsJavaOn())
					gcj_NativeJavaQuit(1);
				return -1;
			}
		}
		/*
		else
		{
			j2me_send_alertuser((PU8)GetString(STR_GLOBAL_NOT_AVAILABLE),UCS2Strlen((const S8*)GetString(STR_GLOBAL_NOT_AVAILABLE)));
		}
		return -1;
		*/
	}
	return 0;
}

gcjStubFileInitStorageRoot(char ** root)
{
	*root = javaStorageRoot;
	gcj_TraceOut(1,"gcjStubFileInitStorageRoot  %s,fileResultSem=%d\n",*root,fileResultSem);
}
#define USE_EVENT_API
#ifdef USE_EVENT_API
#define DEBUG_SIGN   -12345678
int gcjEventApi_FileRet= DEBUG_SIGN;

volatile unsigned int eventApiFileInProcess=0;
void EventFileFinish(unsigned char semp,int *ret,int result)
{
/*
	sxr_TakeSemaphore(fileResultSem);
	if (gcjEventApi_FileRet !=DEBUG_SIGN )
		abort();
	gcjEventApi_FileRet = result;
*/	
	if (gcj_StubIsForceToQuit() || !gcj_IsJavaOn())
	{
		//gcjEventApi_FileRet = DEBUG_SIGN;
		//sxr_ReleaseSemaphore(fileResultSem);
		return;
	}
	if (*ret != DEBUG_SIGN)
		{ *ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	else
		*ret = result;
	gcj_StubReleaseSemphone(semp);
}


int gcjEventAPI_FileOpen(const char *unicodeName,int flag)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_OPEN_T *data = (_FILE_EVENT_OPEN_T *)OslMalloc(sizeof (_FILE_EVENT_OPEN_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->unicodeName = unicodeName;
	data->flag = flag;
	data->finish = EventFileFinish;
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_OPEN_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
/*	
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 

	return ret;;
}


int gcjEventAPI_FileClose(FS_HANDLE file)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_CLOSE_T *data = (_FILE_EVENT_CLOSE_T *)OslMalloc(sizeof (_FILE_EVENT_CLOSE_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->file = file ;
	data->finish = EventFileFinish;	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_CLOSE_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_FileRead(FS_HANDLE FileHandle, void *DataPtr, INT32 Lenth, INT32*ReadLen)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_READ_T *data = (_FILE_EVENT_READ_T *)OslMalloc(sizeof (_FILE_EVENT_READ_T));
	if (!data)
		abort();
	data->semp = tmpSem;
	data->FileHandle = FileHandle;
	data->DataPtr = DataPtr ;
	data->Lenth = Lenth ;
	data->ReadLen = ReadLen ;
	data->finish = EventFileFinish;	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_READ_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_FileWrite(FS_HANDLE FileHandle, void * DataPtr, INT32 Length, INT32 * Written)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_WRITE_T *data = (_FILE_EVENT_WRITE_T *)OslMalloc(sizeof (_FILE_EVENT_WRITE_T));	
	if (!data)
		abort();
	data->semp = tmpSem;
	data->FileHandle = FileHandle;
	data->DataPtr = DataPtr ;
	data->Lenth = Length ;
	data->Written = Written ;
	data->finish = EventFileFinish;
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_WRITE_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_FileSeek(FS_HANDLE FileHandle, INT32 Offset, INT32 Whence)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_SEEK_T *data = (_FILE_EVENT_SEEK_T *)OslMalloc(sizeof (_FILE_EVENT_SEEK_T));
	if (!data)
		abort();	
	data->semp = tmpSem;
	data->FileHandle = FileHandle;
	data->Offset = Offset ;
	data->Whence = Whence ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_SEEK_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_FileChangeSize(FS_HANDLE FileHandle, UINT32 nFileSize)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_CHANGE_SIZE_T *data = (_FILE_EVENT_CHANGE_SIZE_T *)OslMalloc(sizeof (_FILE_EVENT_CHANGE_SIZE_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->FileHandle = FileHandle;
	data->nFileSize = nFileSize ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_CHANGE_SZIE_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_FileGetFileSize(FS_HANDLE FileHandle, INT32 *nFileSize)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_GET_FILE_SIZE_T *data = (_FILE_EVENT_GET_FILE_SIZE_T *)OslMalloc(sizeof (_FILE_EVENT_GET_FILE_SIZE_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->FileHandle = FileHandle;
	data->nFileSize = nFileSize ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_GET_FILE_SZIE_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_FileGetFileName(FS_HANDLE FileHandle, INT32 iNameBufferLen, UINT8 *FileName )
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_GET_FILE_NAME_T *data = (_FILE_EVENT_GET_FILE_NAME_T *)OslMalloc(sizeof (_FILE_EVENT_GET_FILE_NAME_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->FileHandle = FileHandle;
	data->iNameBufferLen = iNameBufferLen ;
	data->FileName = FileName ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_GET_FILE_NAME_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}



int gcjEventAPI_GetDiskInfo(const UINT8 * DriveName, FS_DiskInfo * DiskInfo, INT32 Flags)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_DISK_INFO_T *data = (_FILE_EVENT_DISK_INFO_T *)OslMalloc(sizeof (_FILE_EVENT_DISK_INFO_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->DriveName = DriveName ;
	data->DiskInfo = DiskInfo ;
	data->Flags = Flags ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_DISK_INFO_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_FindFirst(const UINT8 * NamePattern, BYTE Attr, BYTE AttrMask, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_FIND_FIRST_T *data = (_FILE_EVENT_FIND_FIRST_T *)OslMalloc(sizeof (_FILE_EVENT_FIND_FIRST_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->NamePattern = NamePattern ;
	data->Attr = Attr ;
	data->AttrMask = AttrMask ;
	data->FileInfo = FileInfo ;
	data->FileName = FileName ;
	data->MaxLength = MaxLength ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_GET_FIRST_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_FindNext(FS_HANDLE FileHandle, FS_DOSDirEntry * FileInfo, UINT8 * FileName, INT32 MaxLength)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_FIND_NEXT_T *data = (_FILE_EVENT_FIND_NEXT_T *)OslMalloc(sizeof (_FILE_EVENT_FIND_NEXT_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->FileHandle = FileHandle ;
	data->FileInfo = FileInfo ;
	data->FileName = FileName ;
	data->MaxLength = MaxLength ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_GET_NEXT_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}

int gcjEventAPI_FindClose(FS_HANDLE FileHandle)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_FIND_CLOSE_T *data = (_FILE_EVENT_FIND_CLOSE_T *)OslMalloc(sizeof (_FILE_EVENT_FIND_CLOSE_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->FileHandle = FileHandle ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_GET_CLOSE_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_Delete(const UINT8 * unicodeName)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_DELETE_T *data = (_FILE_EVENT_DELETE_T *)OslMalloc(sizeof (_FILE_EVENT_DELETE_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->unicodeName = unicodeName ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_DELETE_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}


int gcjEventAPI_Rename(const UINT8 *unicodeOName,const UINT8 *unicodeNName)
{
	int ret=DEBUG_SIGN;
	int tmpSem = gcj_StubNewSemphone();
	COS_EVENT evt;
	_FILE_EVENT_RENAME_T *data = (_FILE_EVENT_RENAME_T *)OslMalloc(sizeof (_FILE_EVENT_RENAME_T));
	if (!data)
		abort();
	
	data->semp = tmpSem;
	data->unicodeOName = unicodeOName ;
	data->unicodeNName = unicodeNName ;
	data->finish = EventFileFinish;
	
	data->result = &ret;
	evt.nEventId = (unsigned int)MSG_ID_GCJ_FILE_RENAME_REQ;
	evt.nParam1 = data;
	gcj_StubSendEvent(&evt,MOD_FMT);	
	eventApiFileInProcess++;
	gcj_StubTakeSemphone(tmpSem);
	/*
	ret = gcjEventApi_FileRet;
	if (gcjEventApi_FileRet == DEBUG_SIGN)
		abort();
	gcjEventApi_FileRet = DEBUG_SIGN;
	*/
	//sxr_ReleaseSemaphore(fileResultSem);
	eventApiFileInProcess--;
	
	if (ret == DEBUG_SIGN)
		{ ret = -1;gcj_TraceOut(1,"function %s,error",__FUNCTION__);}//abort(); 
	return ret;;
}
#else
#define gcjEventAPI_FileOpen MMI_FS_Open
#define gcjEventAPI_FileClose MMI_FS_Close
#define gcjEventAPI_FileRead MMI_FS_Read
#define gcjEventAPI_FileWrite MMI_FS_Write
#define gcjEventAPI_FileSeek MMI_FS_Seek
#define gcjEventAPI_FileChangeSize FS_ChangeSize
#define gcjEventAPI_FileGetFileSize MMI_FS_GetFileSize
#define gcjEventAPI_FileGetFileName MMI_FS_GetFileName
#define gcjEventAPI_GetDiskInfo MMI_FS_GetDiskInfo
#define gcjEventAPI_FindFirst MMI_FS_FindFirst
#define gcjEventAPI_FindNext	MMI_FS_FindNext
#define gcjEventAPI_FindClose MMI_FS_FindClose
#define gcjEventAPI_Delete MMI_FS_Delete
#define gcjEventAPI_Rename MMI_FS_Rename
#endif
int gcj_StubReadFile(char *filename,unsigned char *readBuf,int readLen)
{
	FS_HANDLE handle;
	int ret=-1;
	int count=0;
	S8 unicodeName[150];
	S8 unicodefName[100];
	AnsiiToUnicodeString(unicodeName,javaLibRoot);
	AnsiiToUnicodeString(unicodefName,filename);
	UCS2Strcat(unicodeName,unicodefName);
	handle = gcjEventAPI_FileOpen(unicodeName,FS_READ_ONLY);
	if (handle >=0)
	{
		ret =gcjEventAPI_FileRead(handle,readBuf,readLen,&count);
		gcjEventAPI_FileClose(handle);
		if (ret >=0)
			ret =count;
		return ret;
	}
	else
		return ret;
}


unsigned char gcj_StubFileStorageExists(char *filePath)
{
	FS_HANDLE ret;
	S8 unicodeName[260];
	AnsiiToUnicodeString(unicodeName,filePath);
	ret = gcjEventAPI_FileOpen(unicodeName,FS_READ_ONLY);
	gcj_TraceOut(0,"gcj_StubFileStorageExists file is %s,ret is %d\n",filePath,ret);
	if (ret >=0)
	{
		gcjEventAPI_FileClose(ret);
		return TRUE;
	}
	return FALSE;
}

unsigned int currentAvailable=0;

unsigned int gcj_StubFileAvailableStorage()
{
	int ret;
	unsigned int ret_2=0;
	FS_DiskInfo disc_info;
	char adir[3];
	kal_wchar dir[3];
	memset(dir,0,6);
	UnicodeNToAnsii(adir,javaStorageRoot,2);
	AnsiiToUnicodeString(dir,adir);
	ret = gcjEventAPI_GetDiskInfo((UINT8 *)dir,&disc_info,FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
	if(ret>0 && ret&(FS_DI_BASIC_INFO|FS_DI_FREE_SPACE))
	{
		ret_2 = disc_info.FreeClusters*disc_info.SectorsPerCluster*disc_info.BytesPerSector;
	        if(ret_2 < 10 * 1024)
	        {
	            j2me_send_alertuser((PU8)GetString(FMGR_FS_DISK_FULL_TEXT),UCS2Strlen((const S8*)GetString(FMGR_FS_DISK_FULL_TEXT)));
		    gcj_NativeJavaQuit(1);
	        }
		currentAvailable = ret_2;
		if (ret_2>2147483647)
			ret_2=2147483647;
	}
	gcj_TraceOut(0,"gcj_StubFileAvailableStorage ret is %d,ret_2=%d\n",ret,ret_2);
	if (ret <0)
		ret = 0;
	else
		ret = ret_2;
	return ret;
}

void gcj_StubFileDeleteStorage(char *filePath)
{
	FS_HANDLE ret;
	S8 unicodeName[260];
	AnsiiToUnicodeString(unicodeName,filePath);
	ret = gcjEventAPI_Delete(unicodeName);
	gcj_TraceOut(0,"gcj_StubFileDeleteStorage file is %s,ret is %d\n",filePath,ret);
}

void gcj_StubFileRenameStorage(char *oldName,char *newName)
{
	FS_HANDLE ret;
	S8 unicodeOName[260];
	S8 unicodeNName[260];
	AnsiiToUnicodeString(unicodeOName,oldName);
	AnsiiToUnicodeString(unicodeNName,newName);
	ret = gcjEventAPI_Rename(unicodeOName,unicodeNName);
	gcj_TraceOut(0,"gcj_StubFileRenameStorage oldfile is %s,newfile is %s,ret is %d\n",oldName,newName,ret);
}

char fileName[260];
char rootName[260];
FS_HANDLE findHandle;
void gcj_StubFileGetFirstFileThatStartsWith(char *root,char ** retname)
{
	S8 unicodeName[260];
	FS_DOSDirEntry fs_file_info;
	strcpy(rootName,root);
	AnsiiToUnicodeString(unicodeName,root);
	UCS2Strcat(unicodeName,(const S8*)L"/*");
	findHandle = gcjEventAPI_FindFirst(unicodeName,0,0,&fs_file_info,fileName,(FMGR_MAX_FILE_LEN +1)*ENCODING_LENGTH);
	if (findHandle >=0)
	{
		UnicodeToAnsii(unicodeName,fileName);
		strcpy(fileName,root);
		strcat(fileName,unicodeName);
		*retname = fileName;
	}
	else
		*retname=NULL;
	gcj_TraceOut(0,"gcj_StubFilegetFirstFileThatStartsWith root is %s,fileName is %s,findHandle is %d\n",root,fileName,findHandle);
}

void gcj_StubFileGetNextFileThatStartsWith(char *root,char ** retname)
{
	if (strcmp(root,rootName))
		abort();
	S8 unicodeName[260];
	FS_DOSDirEntry fs_file_info;
	if (gcjEventAPI_FindNext(findHandle,&fs_file_info,fileName,(FMGR_MAX_FILE_LEN +1)*ENCODING_LENGTH) !=0)
	{
		*retname = NULL;
		gcjEventAPI_FindClose(findHandle);
	}
	else
	{
		UnicodeToAnsii(unicodeName,fileName);
		strcpy(fileName,root);
		strcat(fileName,unicodeName);
		*retname = fileName;
	}
	gcj_TraceOut(0,"gcj_StubFileGetNextFileThatStartsWith root is %s,fileName is %s,findHandle is %d\n",root,fileName,findHandle);
}


typedef enum _access_mode{
	READ  = 1,
 	WRITE = 2,
	READ_WRITE = 3,
}ACCESS_MODE_T;

unsigned int fileHandleCount=0;
int gcj_StubRandomAccessStreamOpen(char *filePath, ACCESS_MODE_T mode)
{
	FS_HANDLE ret;
	int flags;
	S8 unicodeName[260];
	
	AnsiiToUnicodeString(unicodeName,filePath);
	switch (mode)
	{
		case READ:
			flags = FS_READ_ONLY;
			break;
		case WRITE:
			flags = FS_READ_WRITE;
			break;
		case READ_WRITE:
			flags = FS_READ_WRITE | FS_CREATE;
			break;
		default :
			abort();
	}
	gcj_StubTakeMutex(&fileMutex);
	ret = gcjEventAPI_FileOpen(unicodeName,flags);
	if (!javaFileHandleList[ret%MAX_FILE_HANDLE].isOpened)
	{	
		javaFileHandleList[ret%MAX_FILE_HANDLE].isOpened=1;
	}
	else
		{ gcj_TraceOut(1,"function %s,error handle=%d",__FUNCTION__,ret);return -1;}//abort(); 
	fileHandleCount++;
	gcj_StubReleaseMutex(&fileMutex);
	gcj_TraceOut(0,"task id =%d, gcj_StubRandomAccessStreamOpenfile is %s,mode=%d,ret is %d,at %d ticks\n",sxr_GetCurrentTaskId(),filePath,mode,ret,clock());
	if (ret <0)
		ret = -1;
	return ret;
}

int gcj_StubRandomAccessStreamClose(int handle)
{
	int ret;
	gcj_StubTakeMutex(&fileMutex);
	ret = gcjEventAPI_FileClose(handle);	
	if (javaFileHandleList[handle%MAX_FILE_HANDLE].isOpened)
	{	
		javaFileHandleList[handle%MAX_FILE_HANDLE].isOpened=0;
	}
	else
		{ gcj_TraceOut(1,"function %s,error handle=%d",__FUNCTION__,handle);return -1;}//abort(); 
	fileHandleCount--;
	gcj_StubReleaseMutex(&fileMutex);
	gcj_TraceOut(0,"task id =%d, gcj_StubRandomAccessStreamClose handle=%d,ret is %d,at %d ticks\n",sxr_GetCurrentTaskId(),handle,ret,clock());
	return ret;
}

int gcj_StubRandomAccessStreamRead(int handle,char *outBuff,int length)
{
	int count;
	int ret;
	int index = handle%MAX_FILE_HANDLE;
	gcj_StubTakeMutex(&fileMutex);
	if (!javaFileHandleList[index].isOpened)
	{	
		{ gcj_TraceOut(1,"function %s,error handle=%d",__FUNCTION__,handle);return -1;}//abort(); 
	}
	gcj_StubReleaseMutex(&fileMutex);
	ret = gcjEventAPI_FileRead(handle,outBuff,length,&count);
	if (ret < 0)
	{
		gcj_TraceOut(0,"gcj_StubRandomAccessStreamRead handle is %d,length is %d,count is %d,ret is %d\n",handle,length,count,ret);
		return -1;
	}
	return count;
}

int gcj_StubRandomAccessStreamWrite(int handle,char *inBuff,int length)
{
	int count;
	int ret;
	//gcjEventAPI_FileFlush(handle);
	gcj_StubTakeMutex(&fileMutex);
	if (!javaFileHandleList[handle%MAX_FILE_HANDLE].isOpened)
	{	
		{ gcj_TraceOut(1,"function %s,error handle=%d",__FUNCTION__,handle);return -1;}//abort(); 
	}
	gcj_StubReleaseMutex(&fileMutex);
	ret = gcjEventAPI_FileWrite(handle,inBuff,length,&count);
	if (ret < 0)
	{
		gcj_TraceOut(0,"gcj_StubRandomAccessStreamWrite handle is %d,length is %d,count is %d,ret is %d\n",handle,length,count,ret);
		ret = -1;
	}
	return ret;
}

int gcj_StubRandomAccessStreamPosition(int handle,int absolutePosition)
{
	gcj_TraceOut(0,"gcj_StubRandomAccessStreamPosition 0 handle is %d,absolutePosition is %d,task id=%d\n",handle,absolutePosition,sxr_GetCurrentTaskId());
	int ret;
	gcj_StubTakeMutex(&fileMutex);
	if (!javaFileHandleList[handle%MAX_FILE_HANDLE].isOpened)
	{	
		{ gcj_TraceOut(1,"function %s,error handle=%d",__FUNCTION__,handle);return -1;}//abort(); 
	}
	gcj_StubReleaseMutex(&fileMutex);
	if (absolutePosition>50*1024)
	{
		gcj_TraceOut(0,"gcj_StubRandomAccessStreamPosition e handle is %d,absolutePosition is %d,task id=%d\n",handle,absolutePosition,sxr_GetCurrentTaskId());
		return -1;
	}
	if (currentAvailable!=0 && absolutePosition >= currentAvailable)
		return -1;
	ret = gcjEventAPI_FileSeek(handle,absolutePosition,FS_FILE_BEGIN);
	if (ret < 0)
	{
		gcj_TraceOut(0,"gcj_StubRandomAccessStreamPosition 1 handle is %d,absolutePosition is %d,ret is %d\n",handle,absolutePosition,ret);
		return -1;
	}
	if (ret < absolutePosition )
		ret = gcjEventAPI_FileChangeSize(handle,absolutePosition);
	if (ret < 0)
	{
		gcj_TraceOut(0,"gcj_StubRandomAccessStreamPosition 2 handle is %d,absolutePosition is %d,ret is %d\n",handle,absolutePosition,ret);
		return -1;
	}
	ret = gcjEventAPI_FileSeek(handle,absolutePosition,FS_FILE_BEGIN);
	if (ret < 0)
	{
		gcj_TraceOut(0,"gcj_StubRandomAccessStreamPosition 3 handle is %d,absolutePosition is %d,ret is %d\n",handle,absolutePosition,ret);
		ret = -1;
	}
	return ret;
}
int gcj_StubRandomAccessStreamSizeOf(int handle)
{
	int ret;
	int filesize;
	gcj_StubTakeMutex(&fileMutex);
	if (!javaFileHandleList[handle%MAX_FILE_HANDLE].isOpened)
	{	
		{ gcj_TraceOut(1,"function %s,error handle=%d",__FUNCTION__,handle);return -1;}//abort(); 
	}
	gcj_StubReleaseMutex(&fileMutex);
	ret = gcjEventAPI_FileGetFileSize(handle,&filesize);
	if (ret < 0)
	{
		gcj_TraceOut(0,"gcj_StubRandomAccessStreamSizeOf handle is %d,filesize is %d,ret is %d\n",handle,filesize,ret);
		filesize=-1;
	}
	return filesize;
}

int gcj_StubRandomAccessStreamTruncateStream(int handle,int size)
{
	int ret;
	gcj_StubTakeMutex(&fileMutex);
	if (!javaFileHandleList[handle%MAX_FILE_HANDLE].isOpened)
	{	
		{ gcj_TraceOut(1,"function %s,error handle=%d",__FUNCTION__,handle);return -1;}//abort(); 
	}
	gcj_StubReleaseMutex(&fileMutex);
	ret = gcjEventAPI_FileChangeSize(handle,size);
	if (ret < 0)
	{
		gcj_TraceOut(0,"gcj_StubRandomAccessStreamTruncateStream handle is %d,size is %d,ret is %d\n",handle,size,ret);
		ret = -1;
	}
	return ret;
}

void gcj_check_fileHandle()
{
	int i=0;
	char name[256];
	char uname[256];
	gcj_TraceOut(0,"gcj_check_fileHandle fileHandleCount is %d\n",fileHandleCount);
	//gcj_StubTakeMutex(&fileMutex);
	int ret = gcj_CheckDir();
	if (ret == -1)
		return;
	for (i=0;i<MAX_FILE_HANDLE;i++)
	{
		
		if (javaFileHandleList[i%MAX_FILE_HANDLE].isOpened)
		{	
			if (gcj_StubIsForceToQuit() || !gcj_IsJavaOn())
			{
				if (!MMI_FS_GetFileName(i,128,uname))
				{
					UnicodeToAnsii(name,uname);
					gcj_TraceOut(0,"gcj_check_fileHandle handle is %d,name is %s\n",i,name);
					MMI_FS_Close(i);
					javaFileHandleList[i%MAX_FILE_HANDLE].isOpened=0;
				}
			}
			else
			{
				if (!gcjEventAPI_FileGetFileName(i,128,uname))
				{
					UnicodeToAnsii(name,uname);
					gcj_TraceOut(0,"gcj_check_fileHandle handle is %d,name is %s\n",i,name);
					gcjEventAPI_FileClose(i);
					javaFileHandleList[i%MAX_FILE_HANDLE].isOpened=0;
				}
			}
		}
	}
	//gcj_StubReleaseMutex(&fileMutex);
}

extern unsigned char *Current_Java_App_Name;

// TODO: getAPPProperty form jad file
unsigned char *gcj_StubGetAppProperty(unsigned char *key)
{
	gcj_TraceOut(1,"gcj_StubGetAppProperty %s need to get \"%s\" from jad file",Current_Java_App_Name,key);
	if (!strcmp(Current_Java_App_Name,"mxit"))
	{
		if (!strcmp(key,"p"))
			return "Nokia/C3-00";
		if (!strcmp(key,"r"))
			return "http://www.mxit.com/res/";
		if (!strcmp(key,"rstr"))
			return "Installing";
		if (!strcmp(key,"seq"))
			return "731531309145971";
		if (!strcmp(key,"capd"))
			return "4718592";
		if (!strcmp(key,"MIDlet-Version"))
			return "5.9.8";
	}
	return NULL;
}

