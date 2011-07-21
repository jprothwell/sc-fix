
#include "Fs_func.h"
#include "Cswtype.h"
#include "Mmi_trace.h"
#include "Fs_errcode.h"

//#define DEBUG

#ifdef __cplusplus
extern "C" {
#endif

//extern int MMI_FS_CheckFile(const WCHAR * FileName);

static int jtTTS_itoa(char *buf, int i, int base)
{
#define LEN	20

	char *s;
	int rem;
	int count;
	char rev[LEN+1];

	count = 0;

	rev[LEN] = 0;
	if (i == 0)
		{
		buf[0] = '0';
		++buf;
		return 1;
		}
	s = &rev[LEN];
	while (i)
	{
		rem = i % base;
		if (rem < 10)
			*--s = rem + '0';
		else if (base == 16)
			*--s = "ABCDEF"[rem - 10];
		i /= base;
	}
	while (*s)
	{
		buf[0] = *s++;
		++buf;
		count++;
	}

	return count;
}


//*
int jt_Printf(const char *fmt, ...) 
{

    DBG_ERR_T returnStatus;
    va_list ap;

    va_start(ap, fmt);
    returnStatus = dbg_TraceVariadicOutputText(1, fmt, ap);
    va_end(ap);
	
    return returnStatus;

	
	
}
//*/

void* jt_Fopen(const char *file_name, const char *mode)
{
	FS_HANDLE fp = -1;
	//char path[128];

	if(NULL != file_name)
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fopen path: %s", file_name);
	#endif
	}

	if('r' == mode[0])
	{
		fp = MMI_FS_Open((const UINT8*)file_name, FS_READ_ONLY);
	}
	else if('w' == mode[0])
	{
		fp = MMI_FS_Open((const UINT8 *)file_name, FS_READ_WRITE);
	}

	//jt_Printf("tts test, fopen fp: %d", fp);

	if(fp > 0)
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fopen successful, fp = %d .", fp);
	#endif
		//return (fp++);
		//fp++;
		return fp;
	}
	else if(fp == 0)
	{ 
		fp =-1;
		return fp;
	}
	else
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fopen error,  error = %d .", fp);
	#endif
		return NULL;
	}

	//return &fp;
}

int jt_Fclose(void* f_handle)
{
	if(f_handle == -1)
		f_handle = 0;
	FS_HANDLE handle = (FS_HANDLE)f_handle;//*((FS_HANDLE*)f_handle);
	//FS_HANDLE handle = *((FS_HANDLE*)f_handle);

	//mmi_trace(1, "jt_Fclose the fp is %d", handle);
	//jt_Printf("tts test, fseek fp: %d", handle);
	BOOL code = MMI_FS_Close(handle);
	
	if(code < 0)
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fclose err, the code is: %d", SUL_GetLastError());
	#endif
	}
	else
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fclose successful.");
	#endif
	}

	return code;
}

size_t jt_Fread(void* buffer, size_t size, size_t count, void* f_handle)
{
	UINT32 read;
	int code;
	if(f_handle == -1)
		f_handle = 0;
	FS_HANDLE handle = (FS_HANDLE)f_handle;//*((FS_HANDLE*)f_handle);

	//mmi_trace(1, "jt_Fread the fp is %d", handle);
	//jt_Printf("tts test, fread fp: %d", handle);
	code = MMI_FS_Read(handle, buffer, (INT32)(size*count), &read);
	
	if(code < 0)
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fread error, the code is: %d", SUL_GetLastError());
	#endif
		return -1;
	}
	else
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fread read successful, read count : %d .", read);
	#endif
		return (size_t)read;
	}
}

int jt_Fseek(void* f_handle, long offset, int origin)
{
	INT32 result;
	if(f_handle == -1)
		f_handle = 0;
	FS_HANDLE handle = (FS_HANDLE)f_handle;//*((FS_HANDLE*)f_handle);
	//FS_HANDLE handle = *((FS_HANDLE*)f_handle);

	//mmi_trace(1, "jt_Fseek the fp is %d", handle);
	//jt_Printf("tts test, fseek fp: %d", handle);
	if(origin == 0)
	{
		result = MMI_FS_Seek(handle, offset, FS_FILE_BEGIN);
	}
	else if(origin == 1)
	{
		result = MMI_FS_Seek(handle, offset, FS_FILE_CURRENT);
	}
	else
	{
		result = -1;
	}
	
	if(result < 0)
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fseek error, the code is : %d .", SUL_GetLastError());
	#endif
	}
	else
	{
	#ifdef DEBUG
		mmi_trace(1, "jt_Fseek successful. now the offset is : %d", offset);
		mmi_trace(1, "jt_Fseek successful. now the origin is : %d", origin);
	#endif
	}

	return result;
}

#ifdef __cplusplus
}
#endif

