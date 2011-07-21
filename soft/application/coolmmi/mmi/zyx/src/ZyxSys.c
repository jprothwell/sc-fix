#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "zyxsys.h"
#include "zyxstd.h"
#include "zyxextern.h"

int ZyxTimerBegin = ZYX_TIMER_BEGIN,ZyxTimerEnd = ZYX_TIMER_END;

void ZyxRegSysevtCallback (ZyxSysevtHandler handle_sysevt)
{
    pZyxSysevtHandler = handle_sysevt;
}

void ZyxRegKeyboardCallback (ZyxKeyevtHandler handle_keyevt)
{
    pZyxKeyevtHandler = handle_keyevt;
}

void ZyxRegPenevtCallback (ZyxPenevtHandler handle_penevt)
{
    pZyxPenevtHandler = handle_penevt;
}

static void GobackTimer(int timer_id)
{
    GoBackHistory();
}

void ZyxAppQuit(void)
{
    ZyxTrace("zyx app quit");
    ZyxStartTimer(20,GobackTimer);
}

static int zyx_timer_use[100] = {0};
static ZyxTimerHandler zyx_timer_cb[100] ={0};
void volatile ZyxCallFunc1( U32 parg, void* pentry);

void zyx_timer_handler(void *param)
{
    int i;
    int timer_id = (int)param;
    int ZyxMaxTimerNum = ZyxTimerEnd - ZyxTimerBegin;   
	
 	ZyxSysLockEnter();   
    for (i = 0;i<ZyxMaxTimerNum;i++)
    {
        if (zyx_timer_use[i] == timer_id)
        {
            zyx_timer_use[i] = 0;
            //ZyxTrace("zyx timer cb id=%d",timer_id);
            if (ZyxCheckCallback((void*)zyx_timer_cb[i]))
            {
            	#if 1 //ndef __ZYX_CS_LOAD__
                	zyx_timer_cb[i](timer_id);
				#else
					ZyxCallFunc1((U32)timer_id,  (void*)&zyx_timer_cb[i]);
				#endif
            }
            break;
        }
    }
 	ZyxSysLockLeave();   
}

#if 0
static U32 zyx_timer_data[100][4];
void ZyxStartSetData(int timer_id, U32 data1, U32 data2, U32 data3, U32 data4)
{
	zyx_timer_data[timer_id][0] = data1;
	zyx_timer_data[timer_id][1] = data2;
	zyx_timer_data[timer_id][2] = data3;
	zyx_timer_data[timer_id][3] = data4;
}

void ZyxStartGetData(int timer_id, U32* data1, U32* data2, U32* data3, U32* data4)
{
	if (data1) *data1 = zyx_timer_data[timer_id][0];
	if (data2) *data2 = zyx_timer_data[timer_id][1];
	if (data3) *data3 = zyx_timer_data[timer_id][2];
	if (data4) *data4 = zyx_timer_data[timer_id][3];
}
#endif

int ZyxStartTimer(unsigned int ms,ZyxTimerHandler cb)
{
    int i;
    int timer_id = -1;
    int ZyxMaxTimerNum = ZyxTimerEnd - ZyxTimerBegin;    

        for (i = 0;i<ZyxMaxTimerNum;i++)
        {
            if (zyx_timer_use[i]==0)
            {
                timer_id = i+ZyxTimerBegin;
                zyx_timer_use[i] = timer_id;
                zyx_timer_cb[i] = cb;
                break;
            }
        }
    //ZyxTrace("zyx start timer id=%d",timer_id);
    StartTimer((U16)timer_id,ms,(FuncPtr)zyx_timer_handler);
    return timer_id;    
}

int ZyxStopTimer(int timer_id)
{
    //ZyxTrace("zyx stop timer id=%d",timer_id);
    
    if (timer_id >= ZyxTimerBegin && timer_id < ZyxTimerEnd)
    {
        StopTimer((U16)timer_id);
        zyx_timer_use[timer_id-ZyxTimerBegin] = 0;
        zyx_timer_cb[timer_id-ZyxTimerBegin] = 0;
        
		return 1;      
    }
    
    return 0;
}
extern int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);
extern int snprintf(char *buf, size_t size, char const *fmt, ...);

#define ZYX_TRACE_BUFSIZE	(240)
//U8 test_trace_flag = 0;
void ZyxTrace(char const *fmt,...)
{
    va_list args;
    char buf[ZYX_TRACE_BUFSIZE]={0};  
    ZyxTime time;
    int ret;

    ZyxGetSysTime(&time);
    ret = snprintf(buf, ZYX_TRACE_BUFSIZE, "[%d:%d:%d]     ", time.hour,time.min,time.sec);    

    va_start(args, fmt);
    ret += vsnprintf(buf+ret, ZYX_TRACE_BUFSIZE-ret, fmt, args);
    va_end(args);

#if 0
    if (ret < ZYX_TRACE_BUFSIZE - 3){
	buf[ret ++] = '\r';
	buf[ret ++] = '\n';
	buf[ret ++] = 0;
    }else{
	buf[ZYX_TRACE_BUFSIZE - 3] = '\r';
	buf[ZYX_TRACE_BUFSIZE - 2] = '\n';
	buf[ZYX_TRACE_BUFSIZE - 1] = 0;    	
    }
#endif

#if 0
	{
		char filepath[128]={0};
		U16 UFileName[128] = {0};
		int fret;

		sprintf(filepath,"%c:\\ZYXAPP\\%s",ZyxGetDrive(DEVICE_TCARD),"zyx.log");
		ZyxUTF8ToUCS(filepath,UFileName,128);
		fret = MMI_FS_Open((char*)UFileName, FS_CREATE|FS_READ_WRITE);
		if (fret >= 0){
			INT32 wsize;
			MMI_FS_Write(fret,buf,ret,&wsize);
			MMI_FS_Close(fret);
		}
	}
#endif

    mmi_trace(g_sw_DEBUG, buf);
}    


void* ZyxAlloc(int size)
{
    void*buffer_p=NULL;

    buffer_p = OslMalloc(size);

     if (NULL == buffer_p)
        ZyxTrace("zyx alloc mem size=%d fail",size);
     return (void*)buffer_p;
}

void ZyxFree(void* mem)
{
    //ZyxTrace("zyx ext free =%x",mem);
    if(mem != NULL) 
        OslMfree(mem);
}
#if 0
void* ZyxRealloc(void* mem, int osize, int nsize)
{
	void* nmem;

	nmem = ZyxAlloc(nsize);
	if  (nmem && mem){
		Zyx_memcpy(nmem, mem, osize);
	}

	if (mem){
		ZyxFree(mem);
	}

	return nmem;
}
#endif
void* ZyxCreateMutex(char *name)
{
	HANDLE ret;
	ret= COS_CreateSemaphore(1); 
	
	ZyxTrace("zyx mutex create %p",ret);
	return (void*)ret;
}

void ZyxTakeMutex(void *handle)
{
	COS_WaitForSemaphore((HANDLE)handle, COS_WAIT_FOREVER); 
}
void ZyxGiveMutex(void *handle)
{
	COS_ReleaseSemaphore((HANDLE)handle); 
}

void ZyxDeleteMutex(void *handle)
{
	ZyxTrace("zyx mutex delete %d",handle);
	COS_DeleteSemaphore((HANDLE)handle);
}

static void* g_zyx_mutex = 0;	

//xxxxxxxxxv for stop bug
void ZyxSysLockInit(){
	if (g_zyx_mutex == 0){
		g_zyx_mutex = ZyxCreateMutex(0);
	}
}
void ZyxSysLockExit(){
	if (g_zyx_mutex){
		ZyxDeleteMutex(g_zyx_mutex);
		g_zyx_mutex = 0;
	}
}

void ZyxSysLockEnter(){
	if (g_zyx_mutex){
		ZyxTakeMutex(g_zyx_mutex);
	}
}

void ZyxSysLockLeave(){
	if (g_zyx_mutex){
		ZyxGiveMutex(g_zyx_mutex);
	}
}

void ZyxSRand(int seed)
{
    srand(seed);
}

int ZyxRand(void)
{
    S32 randNum;

    randNum = rand();
    if (randNum < 0)
        randNum = -randNum;

    return randNum;
}

void ZyxSleep(ULONG mil)
{
    COS_Sleep(mil);
}

U32 ZyxGetSysTicks(void)
{
	// 16384 * 1tick = 1s (16384=2^14)
	U32 t = csw_TMGetTick();	

	return (U32)(((U64)t*1000)>>14);
  //  return (U32 )csw_TMGetTick();
}

int ZyxGetSysTime(ZyxTimePtr time)
{
	TM_SYSTEMTIME pSystemTime;

	if(TM_GetSystemTime(&pSystemTime))
	{

		time->year = pSystemTime.uYear ;
		time->month =  pSystemTime.uMonth;
		time->day=  pSystemTime.uDay;
		time->hour =  pSystemTime.uHour;
		time->min = pSystemTime.uMinute;
		time->sec = pSystemTime.uSecond;
		time->weekday =  pSystemTime.uDayOfWeek;

		return 1;
	}
	else 
		return 0;
}

int ZyxGetLCDWidth(void)
{
    return UI_device_width;
}

int ZyxGetLCDHeight(void)
{
    return UI_device_height;
}

int ZyxGetLCDDepth(void)
{
#if GDI_MAIN_BASE_LAYER_FORMAT==GDI_COLOR_FORMAT_32
      return 32;
#elif GDI_MAIN_BASE_LAYER_FORMAT==GDI_COLOR_FORMAT_24
	return 24;
#elif GDI_MAIN_BASE_LAYER_FORMAT==GDI_COLOR_FORMAT_16
	return 16;
#else
       return 16;
#endif
}

extern kal_uint32 get_lcd_frame_buffer_address(void);
char* ZyxGetDisplayBuf()
{
#if 0	
	return (void *)get_lcd_frame_buffer_address();
#else
	U8* ptr;
	gdi_layer_get_buffer_ptr(&ptr);	
	return (char*)ptr;
#endif	
}

#if 0
void ZyxFlush()
{
    //lcd_power_ctrl(KAL_TRUE);
    //START_LCD_TRANSFER;
    gdi_layer_blt_previous(0, 0,UI_device_width - 1, UI_device_height - 1);
    ZyxTrace("zyx flush");
}
#endif

void ZyxFlush(int x, int y, int w, int h)
{
    //lcd_power_ctrl(KAL_TRUE);
    //START_LCD_TRANSFER;
    gdi_layer_blt_previous(x, y, w, h);
    //ZyxTrace("zyx flush");
}

BOOL ZyxCheckCallback(void* addr)
{
    if (addr == NULL)
        return FALSE;
    else
        return TRUE;
}


#endif

