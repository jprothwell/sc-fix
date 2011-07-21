#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"
#include "internal.h"
#include "javaprot.h"

#include "hal_mem_map.h"
const char *sxr_GetTaskName (void);

int _taskid;
char * sxr_get_stack_bottom()
{
	int i=0;
	char *name= sxr_GetTaskName();
	_taskid = sxr_GetCurrentTaskId();
	//for (i=0;i<10;i++)
	//	gcj_TraceOut(1,"heap  %d status : %d\n",i,sxr_GetAbsoluteHeapRemainingSize(i));
	gcj_TraceOut(1,"current Taskid is %d, name is %s\n",_taskid,name);
	return sxr_GetStatckBottom(_taskid);
}
char * sxr_get_data_start()
{
	return &_ram_start;
}

char * sxr_get_data_end()
{
	return &_ram_bss_end;
}

extern UINT32 _gcj_flash_start_location;
extern UINT32 _gcj_ram_start;
extern UINT32 _gcj_ram_end;

extern UINT32 _gcj_ram_bss_start;
extern UINT32 _gcj_ram_bss_end;
/*
#define GC_DATASTARTSYM _ram_start
#define GC_DATAENDSYM _ram_end

#define GC_DATASTARTSYM2 _ram_bss_start
#define GC_DATAENDSYM2 _ram_bss_end
*/

void page_spy();

void gcj_reset_data()
{
    UINT32* src;
    UINT32* dst;
   gcj_TraceOut(1,"gcj_reset_data start ticks is %d\n",csw_TMGetTick());
    //page_spy();
    // Copy code & data from ROM space to RAM.
    //UINT32 status = sxr_EnterSc();
    for (src = (UINT32*) &_gcj_flash_start_location,
             dst = (UINT32*) &_gcj_ram_start;
         dst < (UINT32*) &_gcj_ram_end ;
         src++, dst++)
    {
        *dst = *src;
    }
    //sxr_ExitSc(status);	 
    //sxr_Sleep(10);
    //status = sxr_EnterSc();
    for ( dst = (UINT32*) &_gcj_ram_bss_start; dst < (UINT32*) &_gcj_ram_bss_end ;dst++)
    {
     *dst = 0;
    }
    //memset(&_gcj_ram_bss_start,0,&_gcj_ram_bss_start - &_gcj_ram_bss_end);
   // sxr_ExitSc(status);   	
   gcj_TraceOut(0,"gcj_reset_data done ticks is %d\n",csw_TMGetTick());
}

extern unsigned char _JCR_START;
extern unsigned char _JCR_SIZE;
void gcj_init_jcr()
{
/*
	gcj_TraceOut(1,"_ram_start = 0x%x",&_ram_start);
	gcj_TraceOut(1,"_ram_end = 0x%x",&_ram_end);
	gcj_TraceOut(1,"_ram_bss_start = 0x%x",&_ram_bss_start);
	gcj_TraceOut(1,"_ram_bss_end = 0x%x",&_ram_bss_end);
	gcj_TraceOut(1,"_gcj_ram_start = 0x%x",&_gcj_ram_start);
	gcj_TraceOut(1,"_gcj_ram_end = 0x%x",&_gcj_ram_end);
	gcj_TraceOut(1,"_gcj_ram_bss_start = 0x%x",&_gcj_ram_bss_start);
	gcj_TraceOut(1,"_gcj_ram_bss_end = 0x%x",&_gcj_ram_bss_end);
*/	
	int i;
	unsigned char * address = &_JCR_START;
	//for (i=0; i< &_JCR_SIZE; i+=sizeof(void *) )
	_Jv_RegisterClasses(address);
	//COS_PageProtectSetup(COS_PAGE_NUM_4,COS_PAGE_READWRITE_TRIGGER,0x813b1b2c,0x813b1b2e);
	//COS_PageProtectEnable(COS_PAGE_NUM_4);
	//COS_PageProtectSetup(COS_PAGE_NUM_5,COS_PAGE_READWRITE_TRIGGER,0x813b1b2c,0x813b1b2e);
	//COS_PageProtectEnable(COS_PAGE_NUM_5);
}
void gcj_dinit_jcr()
{
	int i;
	unsigned char * address = &_JCR_START;
	//for (i=0; i< &_JCR_SIZE; i+=sizeof(void *) )
	_Jv_UnregisterClass(address);
}

typedef void (*func_ptr) (void);

extern func_ptr _CTORS_START[];
extern func_ptr _CTORS_END[];
void gcj_init_ctor()
{
	func_ptr f;
	int i;
	int size=(_CTORS_END-_CTORS_START);
	for (i=0; i< size; i++ )
	{
		f = (func_ptr)(_CTORS_START[i]);
		f();
		gcj_TraceOut(1,"gcj_init_ctor function is 0x%x\n",f);
	}
}
_GCJ_MUTEX_T memMutex;

#define GCJ_USE_DLMALLOC
#define     GCJ_HEAP_INDEX             4
#if (LCD_WIDTH == 176)
#define 	  GCJ_HEAP_MAX_SIZE	1000*1024
#else
#define 	  GCJ_HEAP_MAX_SIZE	1100*1024
#endif
#define GCJ_HEAP_MIN_SIZE	400*1024

 unsigned char *gcj_HeapAdd=0;
 unsigned char *brk=0;
 void GC_set_max_heap_size(int size);
 void *_sxr_HMallocNoLock (unsigned int Size, unsigned char Idx);
 void Jv_InitImgMutex();
 void Jv_thread_mutex_init();
 void gcj_StubAlterAndQuit();


unsigned char gcj_check_memsize(unsigned char heapId,unsigned int neededSize)
{
	void *ret=NULL;
	int remainSIze = sxr_GetAbsoluteRemainingSize(neededSize,heapId);
	if (remainSIze> neededSize)
	{
		ret =_sxr_HMalloc(neededSize,heapId);
		gcj_TraceOut(0,"gcj_check_memsize heapId =%d,neededSize=%d,remainSIze=%d,ret=0x%x\n",heapId,neededSize,remainSIze,ret);
		if (!ret)
			return 0;
		else
			sxr_Free(ret);
		return 1;
	}
	return 0;
}

static unsigned int CurrentJavaHeapSIze=0;
void *gcj_getMem()
{
    CurrentJavaHeapSIze = GCJ_HEAP_MAX_SIZE;
    while (CurrentJavaHeapSIze > GCJ_HEAP_MIN_SIZE)
    {
    	if (!gcj_check_memsize(3,CurrentJavaHeapSIze))
	{
		CurrentJavaHeapSIze -= 10*1024;
    	}
	else
	{
		    //CurrentJavaHeapSIze -= 50*1024; // for cos malloc
		return COS_Malloc(CurrentJavaHeapSIze,0);
	}
    }
    return NULL;
}
 BOOL gcj_InitMemHeap()
 {
    //avoid multi times malloc
    if(gcj_HeapAdd != NULL)
    {
        return TRUE;
    }
    brk = gcj_HeapAdd = gcj_getMem();
   gcj_TraceOut(0, "gcj_InitMemHeap gcj_HeapAdd = 0x%x,CurrentJavaHeapSIze=%d\n", gcj_HeapAdd,CurrentJavaHeapSIze);
    if (brk == NULL)
    {  
        return FALSE;
    }
#ifndef GCJ_USE_DLMALLOC
    _sxr_NewHeap (gcj_HeapAdd, CurrentJavaHeapSIze, GCJ_HEAP_INDEX);
#endif
    gcj_StubInitMutex(&memMutex);
    Jv_InitImgMutex();
    Jv_thread_mutex_init();
    GC_set_max_heap_size(CurrentJavaHeapSIze);
    return TRUE;
 }
 void dl_mem_reset();

 void gcj_DeInitMemHeap()
 {
	dl_mem_reset();
	COS_Free(gcj_HeapAdd);
	gcj_HeapAdd = brk = NULL;
 }
#ifdef GCJ_USE_DLMALLOC
 typedef int ptrdiff_t;
 void*	   sbrk(ptrdiff_t increment)
 {
	 void *p = brk;
	 brk += increment;
	if (increment !=0)
	 	gcj_TraceOut(0,"----------------------sbrk increment =%d, p =0x%x---------------------------------------\n",increment,p);
	 if (brk >	gcj_HeapAdd + CurrentJavaHeapSIze)
	   {
		 brk -= increment;
		 return ~0;
	   }
	 return p;	 
 }
#else
#endif
#ifdef GCJ_USE_DLMALLOC

void gcj_TrimJavaHeap()
{
	int released=0;
	gcj_StubTakeMutex(&memMutex);
	_Jv_RunGC();
	released = dlmalloc_trim(0);
	_Jv_GCSetMaximumHeapSize(CurrentJavaHeapSIze-released);
	CurrentJavaHeapSIze -= released;
	gcj_StubReleaseMutex(&memMutex);
}
#endif
extern unsigned char *logoAddress;

void gc_free (void *ptr)
{
	void *check = ptr;
	int ret=0;
	if (ptr <gcj_HeapAdd || ptr>gcj_HeapAdd+CurrentJavaHeapSIze || ptr == 0 )
		abort();
	gcj_StubTakeMutex(&memMutex);
#ifdef GCJ_USE_DLMALLOC	
	dlfree(ptr);
	//ret = dlmalloc_trim(0);
	//gcj_TraceOut(1,"stub:task id=%d,current function=%s,check=0x%x,prt=0x%x,ret=%d\n",sxr_GetCurrentTaskId(),__FUNCTION__,check,ptr,ret);
#else	
	sxr_Free(ptr);
#endif
	gcj_StubReleaseMutex(&memMutex);
}

int unalloced_size=0;
int unalloced_count=0;
void * gc_malloc(unsigned int size)
{
	static int firstin=1;
	char * ret=NULL;
	if (size > CurrentJavaHeapSIze)
		abort();
	gcj_StubTakeMutex(&memMutex);
#ifdef GCJ_USE_DLMALLOC	
	ret =dlmalloc(size);
#else
	ret = _sxr_HMallocNoLock(size, GCJ_HEAP_INDEX);
#endif
	gcj_StubReleaseMutex(&memMutex);
	if (unalloced_size == size)
		gcj_TraceOut(1,"stub1:task id=%d,current function=%s,prt=0x%x,size=%d,unalloced_count=%d\n",sxr_GetCurrentTaskId(),__FUNCTION__,ret,size,unalloced_count);
	if (ret == 0)
	{
#ifdef GCJ_USE_DLMALLOC	
		//dlmalloc_stats();
		//dlmalloc_trim(0);
		//ret =dlmalloc(size);
#endif
		if (unalloced_size == size)
		{
			unalloced_count++;
		}
		else
			unalloced_count=0;
		unalloced_size = size;
		gcj_TraceOut(1,"stub2:task id=%d,current function=%s,prt=0x%x,size=%d,unalloced_count=%d\n",sxr_GetCurrentTaskId(),__FUNCTION__,ret,size,unalloced_count);
	}
	else
	{
		if (unalloced_size == size) 
		{
			unalloced_size = 0;
			unalloced_count=0;
		}
	}
	if (ret != NULL)
		memset(ret,0,size);
	return ret;
}

unsigned int total_static_mem=0;
void *malloc(unsigned int size)
{
	char * ret;
	total_static_mem+=size;
	ret = COS_Malloc(size, 0);
	//ret = gc_malloc(size);
	if (ret != NULL)
		memset(ret,0,size);
	if (ret == NULL)
	{
		gcj_TraceOut(1,"stub:task id=%d,current function=%s,prt=0x%x,size=%d,total_static_mem =%d\n",sxr_GetCurrentTaskId(),__FUNCTION__,ret,size,total_static_mem);
		//gcj_StubAlterAndQuit();
	}
	return ret;
}
void *calloc (unsigned int size, unsigned int mult)
{
	char * ret=NULL;
	ret = malloc(size);
	gcj_TraceOut(1,"stub:task id=%d,current function=%s,prt=0x%x,size=%d\n",sxr_GetCurrentTaskId(),__FUNCTION__,ret,size);
	return ret;
}

void free(void *ptr)
{
	//gcj_TraceOut(1,"stub:task id=%d,current function=%s,prt=0x%x\n",sxr_GetCurrentTaskId(),__FUNCTION__,ptr);
	if (ptr == NULL)
		abort();
	COS_Free(ptr);
	//gc_free(ptr);
}

void * realloc(void *ptr,unsigned int size)
{
	char *ret=NULL;
	if (ptr == NULL)
		ret = (unsigned char *)malloc(size);
	else if (size == 0)
		free(ptr);
	else
	{
		ret = (unsigned char *)malloc(size);
		if (ret == NULL)
			abort();
		memcpy(ret,ptr,size);
		free(ptr);
	}
	if (ret == NULL)
		abort();
	gcj_TraceOut(1,"stub:task id=%d,current function=%s,prt=0x%x,size=%d\n",sxr_GetCurrentTaskId(),__FUNCTION__,ret,size);
	
	return ret;
}



long clock()
{
	//abort();
	return csw_TMGetTick();
}

volatile unsigned long long currentTImeMillis=0;
volatile unsigned long long prevTImeMillis=0;
const unsigned long long timeAfter2000 = (946684800)*1000LL;

int gcj_StubGetLocalTimeZoneOffset()
{
	int izone=0;
	
	//izone = TM_GetTimeZone();
	izone = PhnsetGetHomeCity()+1;
	//gcj_TraceOut(0,"gcj_StubGetLocalTimeZoneOffset izone=%d\n",izone);	
	return izone;
}

static unsigned long long getCurrentMillis()
{
	TM_SYSTEMTIME SystemTime = {0,} ;

	static unsigned int record_millis;
	static unsigned int record_time;
	volatile unsigned int ticks= hal_TimGetUpTime();
	ticks &=0x3fff;
	volatile unsigned int millis= (ticks*1000)>>14;// (ticks%16384 *1000)/16384;//(ticks*1000)>>14;
	volatile unsigned int time = TM_GetTimeZoneTime(0,&SystemTime);
	while (!time)
	{
		time = TM_GetTimeZoneTime(0,&SystemTime);
		gcj_TraceOut(0,"getCurrentMillis time return null\n");
		COS_Sleep(50);
	}
	time -= gcj_StubGetLocalTimeZoneOffset()*60*60; //get UTC Time
	//millis = (millis+500)%1000;
	if (millis < record_millis && time == record_time)
	{
		millis = 1000+millis;
	}
	record_millis = millis;
	record_time = time;
	//gcj_TraceOut(0,"getCurrentMillis current ticks=%d,millis=%d,time=%d",ticks,millis,time);
	return time*1000LL+millis;
}
unsigned long long gcj_StubGetTimeMillis()
{
	unsigned long long ret=0;
	currentTImeMillis = ret =  getCurrentMillis();
	if (currentTImeMillis <  prevTImeMillis)
	{
		//COS_Sleep(10);
		gcj_TraceOut(0,"currentTImeMillis current sec=%d,%d,millis=%d,ticks=%d",currentTImeMillis/1000,(unsigned int)currentTImeMillis%1000,hal_TimGetUpTime());
		gcj_TraceOut(0,"prevTImeMillis current sec=%d,%d,millis=%d,ticks=%d",prevTImeMillis/1000,(unsigned int)prevTImeMillis%1000,hal_TimGetUpTime());
		currentTImeMillis = prevTImeMillis;//getCurrentMillis();
	}
	prevTImeMillis = ret;
	return ret+timeAfter2000;


}

long time()
{
	gcj_StubGetTimeMillis();
}
	

unsigned char getMainThreadMbox();
unsigned char gcj_IsJavaOn();
unsigned char gcj_IsJavaPause();

static unsigned char gcj_IsNeedToQuit=0;
extern unsigned char gcj_IsForceToQuit;

void javanotify_resume(void);

void gcj_NativeJavaQuit(unsigned char isQuit)
{
	gcj_IsNeedToQuit=isQuit;
	gcj_IsForceToQuit=1;
	gcj_TraceOut(0,"taskId=%d,gcj_NativeJavaQuit gcj_IsNeedToQuit=%d",sxr_GetCurrentTaskId(),gcj_IsNeedToQuit);
	if (gcj_IsNeedToQuit && gcj_IsJavaOn())
	{	
		gcj_StubCondNotify(getMainThreadMbox());
		if (gcj_IsJavaPause())
			javanotify_resume();
	}
}
unsigned char gcj_StubIsForceToQuit()
{
	return gcj_IsNeedToQuit;
}



unsigned char netResultSem=0;
unsigned char fileResultSem=0;
void gcj_check_fileHandle();
void gcj_reset_native()
{
	gcj_NativeDestoryThread(getMainThreadMbox());
	gcj_DeInitMemHeap();
	gcj_interface_reset();
	gcj_net_reset();
	gcj_image_reset();
	gcj_menu_reset();
	gcj_popupmenu_reset();
	gcj_check_fileHandle();
/*	
	sxr_FreeSemaphore(netResultSem);
	sxr_FreeSemaphore(fileResultSem);
*/	
}
