
#include "cswtype.h"
#include "string.h"
#include "stdio.h"
#include "cos.h"
#include "fs.h"
#include "tm.h"
#include "mem_trace.h"

extern void *MMI_Malloc(char *file,char *function,int line,UINT32 size);
extern void MMI_Free(void *addr);
static struct t_record record[MAX_DYNAMIC_MEM_RECORD];

void InitMemRecordList(void)
{
    memset(record, 0x00, MAX_DYNAMIC_MEM_RECORD * sizeof(struct t_record));
}

void AddMemRecord(unsigned long base, unsigned long clicks, char *filename, int line)
{
    int idx;
    struct t_record *ptr;
    
    ptr = record;
    for(idx = 0; idx < MAX_DYNAMIC_MEM_RECORD; idx++)
    {
        if(!ptr->used)
        {
            memset(ptr->filename, 0, DYN_MEM_FILE_NAME_LEN);
            strncpy(ptr->filename, filename, DYN_MEM_FILE_NAME_LEN - 1);
            ptr->linenum = line;
            ptr->h_base  = base;
            ptr->h_len   = clicks;
            ptr->used    = 1;
            break;
        }
        ptr++;
    }
}

BOOL DelMemRecord(unsigned long base)
{
    int idx;
    struct t_record *ptr;
    
    ptr = record;
    for(idx = 0; idx < MAX_DYNAMIC_MEM_RECORD; idx++)
    {
        if(ptr->used && (ptr->h_base == base))
        {
            memset(ptr, 0, sizeof(struct t_record));
            return TRUE;
        }
        ptr++;
    }
    return FALSE;
}

int GetLeakBlockNum(void)
{
    struct t_record *ptr;
    int num = 0;
    int idx;
    
    ptr = record;
    for(idx = 0; idx < MAX_DYNAMIC_MEM_RECORD; idx++)
    {
        if(ptr->used)
        {
            num++;
        }
        ptr++;
    }
    return num;
}

UINT8 GetOneMemRecord(char *pContents, UINT16 BlockId)
{
    struct t_record *ptr;
    
    *pContents = 0;
    
    if( BlockId >= MAX_DYNAMIC_MEM_RECORD )
    {
        *pContents=0;
        return 0;
    }
    
    ptr = record + BlockId;
    if( ptr->h_len )
    {
        sprintf(pContents, "Memory Leak in file:%s, line=%d, size=%ld\n", ptr->filename, ptr->linenum, ptr->h_len);
    }
    else
    {
        strcpy( pContents, "unused block" );
    }
    
    return ptr->used;
}

int WriteMemLeakInfo(void)
{
    char    buff[128]  = "\0";
    UINT16  MemLeakId  = 0;
	BOOL    bRet       = FALSE;
    HANDLE  pFile      = 0;
	TM_SYSTEMTIME   systemTime;
	
    pFile = FS_Open(MEMORY_LEAK_FILE, FS_O_WRONLY | FS_O_CREAT, 0);
	if (pFile == 0)
	{
		return MEMTRC_FAILURE;
	}
    
	bRet = TM_GetSystemTime(&systemTime);
	if (bRet == FALSE)
	{
		return MEMTRC_FAILURE;
	}
	
    sprintf(buff, "Memory Leak Begin : %04d-%02d-%02d %02d:%02d\n", systemTime.uYear, systemTime.uMonth, systemTime.uDay, systemTime.uHour, systemTime.uMinute);
    FS_Write(pFile, buff, strlen(buff));
	
    while(1)
    {
        while (GetOneMemRecord(buff, MemLeakId) != 0)
        {
            if (!buff[0])
            {
                sprintf(buff, "The End\n\n");
                goto LeaveOut;
            }
			
            ++MemLeakId;
        }
		
        FS_Write(pFile, buff, strlen(buff));
		
        ++MemLeakId;
    }
	
LeaveOut:
    FS_Write(pFile, buff, strlen(buff));
    FS_Close(pFile);
	
    return MEMTRC_SUCCESS;
}

BOOL IfMemoryLeak(void)
{
    struct t_record *ptr = record;
    int idx = 0;

    while (idx < MAX_DYNAMIC_MEM_RECORD)
    {
        if (ptr->used)
        {
            return TRUE;
        }
        idx++;
        ptr++;
    }
    return FALSE;
}

void* OslMallocTrace(unsigned long size, char *filename, int line)
{
    void *pRet = NULL;
    
    pRet = COS_MALLOC(size);
    if (pRet == NULL)
    {
        return NULL;
    }
    
    AddMemRecord((unsigned long)pRet, size, filename, line);
    
    return pRet;
}

void OslFreeTrace(void *base, char *filename, int line)
{
    if (DelMemRecord((unsigned long)base) == FALSE)
    {
        return;
    }
    
    MMI_Free(base);
}


