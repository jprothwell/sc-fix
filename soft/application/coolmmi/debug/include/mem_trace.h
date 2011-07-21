#ifndef MEM_TRACE_H__
#define MEM_TRACE_H__


#define MEMTRC_SUCCESS              0
#define MEMTRC_FAILURE             -1

#define MAX_DYNAMIC_MEM_RECORD      1024
#define DYN_MEM_FILE_NAME_LEN       32
#define MEMORY_LEAK_FILE            "c:/mem_trace.log"

struct t_record
{
    unsigned long h_base;                          /* pointer for base address */
    unsigned long h_len;                           /* memory length */
    char          filename[DYN_MEM_FILE_NAME_LEN]; /* file name */
    int           linenum;                         /* line number */
    UINT8         used;
    UINT8         fill_bytes[3];
};


extern void  InitRecordList(void);
extern BOOL IfMemoryLeak(void);
extern int   WriteMemLeakInfo(void);
extern void* OslMallocTrace(unsigned long size, char *filename, int line);
extern void  OslFreeTrace(void *base, char *filename, int line);

#define OslMalloc_Trace(size)      OslMallocTrace(size, __FILE__, __LINE__)
#define OslFree_Trace(base)        OslFreeTrace(base, __FILE__, __LINE__)


#endif
