#ifndef __JAVA__H__
#define __JAVA__H__
#include "cfw.h"


typedef void (*gcj_callback)(unsigned char semp,int *ret,int result); 

typedef struct net_event_t {
	unsigned char semp;
	gcj_callback finish;
	unsigned int *result;
}_NET_EVENT_T;

typedef struct net_event_set_opt_t {
	unsigned char semp;
	SOCKET socketId;
	unsigned short option; 
	unsigned char *value;
	unsigned char value_size;
	gcj_callback finish;	
	unsigned int *result;
}_NET_EVENT_SET_OPT_T;

typedef struct net_event_get_host_t {
	unsigned char semp;
	unsigned char* domain;
	unsigned char* address; 
	unsigned char* adress_len;
	gcj_callback finish;	
	unsigned int *result;
}_NET_EVENT_GET_HOST_T;

typedef struct net_event_soc_con_t {
	unsigned char semp;
	SOCKET nSocket;
	CFW_TCPIP_SOCKET_ADDR* pName; 
	UINT8 nNameLen;
	gcj_callback finish;
	unsigned int *result;	
}_NET_EVENT_SOC_CON_T;

typedef struct net_event_soc_read_t {
	unsigned char semp;
	SOCKET nSocket;
	char* inBuf; 
	int maxLen;
	gcj_callback finish;
	unsigned int *result;	
}_NET_EVENT_SOC_READ_T;

typedef struct net_event_soc_write_t {
	unsigned char semp;
	SOCKET nSocket;
	char* outBuf; 
	int writeLen;
	gcj_callback finish;
	unsigned int *result;	
}_NET_EVENT_SOC_WRITE_T;

typedef struct net_event_soc_close_t {
	unsigned char semp;
	SOCKET nSocket;
	gcj_callback finish;
	unsigned int *result;	
}_NET_EVENT_SOC_CLOSE_T;


typedef struct file_event_open_t {
	unsigned char semp;
	char * unicodeName;
	int flag;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_OPEN_T;

typedef struct file_event_close_t {
	unsigned char semp;
	FS_HANDLE file;
	gcj_callback finish;	
	unsigned int *result;
}_FILE_EVENT_CLOSE_T;

typedef struct file_event_read_t {
	unsigned char semp;
	FS_HANDLE FileHandle;
	void *DataPtr;
	INT32 Lenth;
	INT32*ReadLen;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_READ_T;

typedef struct file_event_write_t {
	unsigned char semp;
	FS_HANDLE FileHandle;
	void *DataPtr;
	INT32 Lenth;
	INT32*Written;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_WRITE_T;

typedef struct file_event_seek_t {
	unsigned char semp;
	FS_HANDLE FileHandle;
	INT32 Offset;
	INT32 Whence;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_SEEK_T;

typedef struct file_event_changesize_t {
	unsigned char semp;
	FS_HANDLE FileHandle;
	UINT32 nFileSize;
	gcj_callback finish;
	unsigned int *result;
}_FILE_EVENT_CHANGE_SIZE_T;

typedef struct file_event_getfilesize_t {
	unsigned char semp;
	FS_HANDLE FileHandle;
	INT32 *nFileSize;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_GET_FILE_SIZE_T;

typedef struct file_event_getfilename_t {
	unsigned char semp;
	FS_HANDLE FileHandle;
	INT32 iNameBufferLen;
	UINT8 * FileName;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_GET_FILE_NAME_T;

typedef struct file_event_diskinfo_t {
	unsigned char semp;
	UINT8 * DriveName;
	FS_DiskInfo * DiskInfo;
	int Flags;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_DISK_INFO_T;

typedef struct file_event_findfirst_t {
	unsigned char semp;
	UINT8 * NamePattern;
	BYTE Attr;
	BYTE AttrMask;
	FS_DOSDirEntry * FileInfo;
	UINT8 * FileName;
	INT32 MaxLength;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_FIND_FIRST_T;

typedef struct file_event_findnext_t {
	unsigned char semp;
	FS_HANDLE FileHandle;
	FS_DOSDirEntry * FileInfo;
	UINT8 * FileName;
	INT32 MaxLength;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_FIND_NEXT_T;

typedef struct file_event_findclose_t {
	unsigned char semp;
	FS_HANDLE FileHandle;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_FIND_CLOSE_T;


typedef struct file_event_delete_t {
	unsigned char semp;
	UINT8 * unicodeName;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_DELETE_T;

typedef struct file_event_rename_t {
	unsigned char semp;
	UINT8 * unicodeOName;
	UINT8 * unicodeNName;
	gcj_callback finish;
	unsigned int *result;	
}_FILE_EVENT_RENAME_T;

typedef struct _native_editer {
	int maxLen;
}_NATIVE_EDITER_T;

#endif

