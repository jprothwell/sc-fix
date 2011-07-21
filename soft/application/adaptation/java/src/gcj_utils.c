#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"
#include "internal.h"

#include "sxr_tksd.hp"
#include "debuginitdef.h"
 
char *checkAddres[6];
 static int count=0;
 static int num=0;
 static int havaSpace=0;
 void gcj_pageSpay(char * startAdd,char *endAdd)
{
#if 0
	if (havaSpace && num>5)
	{
	/*
		return;
		num = num%6;
		hal_DbgPageProtectDisable(num);
		*/
		int i;
		int slot= -1;
		for (i=0;i<6;i++)
		{
			if (checkAddres[i] == startAdd)
				return;
			else if (checkAddres[i] == NULL )
				slot = i;
		}
		if (slot >=0)
		{	
			checkAddres[slot] = startAdd;
			hal_DbgPageProtectSetup( slot, 2, (unsigned int)(startAdd), (unsigned int)(endAdd) );
			hal_DbgPageProtectEnable(slot);
			gcj_TraceOut(0,"gcj_pageSpay %d :0x%x ~ 0x%x",slot,startAdd,endAdd);
			return;
		}
	}
	else if (num <6)
	{
		hal_DbgPageProtectSetup( num, 2, (unsigned int)(startAdd), (unsigned int)(endAdd) );
		hal_DbgPageProtectEnable(num);
		gcj_TraceOut(0,"gcj_pageSpay %d :0x%x ~ 0x%x",num,startAdd,endAdd);
		checkAddres[num] = startAdd;
		num++;
		count ++;
	}
#endif
	if (num<6)
	{
		hal_DbgPageProtectSetup( num, 2, (unsigned int)(startAdd), (unsigned int)(endAdd) );
		hal_DbgPageProtectEnable(num);
		//gcj_TraceOut(0,"gcj_pageSpay %d :0x%x ~ 0x%x",num,startAdd,endAdd);
		checkAddres[num] = startAdd;
		num++;
	}
}

  void gcj_pageSpay0(char * startAdd,char *endAdd)
 {
	  gcj_TraceOut(0,"gcj_pageSpay0 :0x%x ~ 0x%x",startAdd,endAdd);
	  hal_DbgPageProtectSetup( 0, 2, (unsigned int)(startAdd), (unsigned int)(endAdd) );
	  hal_DbgPageProtectEnable(0);
 }

 void gcj_pageSpay4(char * startAdd,char *endAdd)
{
	 gcj_TraceOut(0,"gcj_pageSpay4 :0x%x ~ 0x%x",startAdd,endAdd);
	 hal_DbgPageProtectSetup( 4, 2, (unsigned int)(startAdd), (unsigned int)(endAdd) );
	 hal_DbgPageProtectEnable(4);
}

 
  void gcj_pageSpay5(char * startAdd,char *endAdd)
 {
	  gcj_TraceOut(0,"gcj_pageSpay5 :0x%x ~ 0x%x",startAdd,endAdd);
	  hal_DbgPageProtectSetup( 5, 2, (unsigned int)(startAdd), (unsigned int)(endAdd) );
	  hal_DbgPageProtectEnable(5);
 }

void gcj_pageSpayClose0()
{
	  hal_DbgPageProtectDisable(0);
}

  
void gcj_pageSpayClose4()
{
	  hal_DbgPageProtectDisable(4);
}
void gcj_pageSpayClose5()
{
	hal_DbgPageProtectDisable(5);
}
void gcj_pageSpayClose()
{
	int i;
	for ( i=0;i<6;i++)
	{
		//if (checkAddres[i] == startAdd)
		{	
			checkAddres[i] = NULL;
			hal_DbgPageProtectDisable(i);
			havaSpace = 1;
			//gcj_TraceOut(0,"gcj_pageSpayClose %d :0x%x",i,startAdd,startAdd);
		}
	}
	num=0;
}

u8 sxs_SerialFlush (void);

void abort(void)
{
	sxs_CheckPendingTk();
	sxs_SerialFlush();
	MMI_ASSERT(0);
}

typedef  FS_HANDLE FILE;
void *_impure_ptr;

FILE *stderr;
int __errno=2;
//int printf (const char *ict, ...);
int fprintf (FILE *f, const char *fmt, ...);
int fputs(FILE *f, const char *fmt, ...)
{

}

int fprintf (FILE *f, const char * fmt, ...)
{
	f=f;
	va_list ap;
	int len;
	char *buf[256];
	va_start(ap, fmt);
	len = vsnprintf(buf, -(size_t)buf, fmt, ap);
	va_end(ap);
	sxs_fprintf(1<<21, buf); 
	return (len);
}

#ifndef gcj_TraceOut
void gcj_TraceOut(int index,char * fmt,...)
{
#if 1
	char uartBuf[256];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(uartBuf, sizeof(uartBuf), fmt, ap);
	va_end(ap);
		// Forced trace
	sxs_fprintf(1<<21, uartBuf); 
	sxs_SerialFlush();
#endif
}
#endif
void _Jv_diag_write (char *buff, unsigned int len)
{
	gcj_TraceOut(1,"%s",buff);
}




void fflush(FILE * stream)
{
	sxs_SerialFlush();
}

void __sync_synchronize()
{
	//asm ("break 1");

}

void exit(int n)
{
	n=n;
}
void * getenv(void * name)
{
	name=name;
	return 0;
}

int fwrite(void *fd,const unsigned char *buff, unsigned long size)
{
	_Jv_diag_write(buff,size);
	return size;
}

int
atexit(void (*f)(void))
{
  /* If the system doesn't provide a definition for atexit, use on_exit
     if the system provides that.  */
  f=f;
  return 0;
}


#undef atol
long atol(const char *s)
{
	return atoi(s);
}
void diag_write_char(unsigned char * c)
{
	gcj_TraceOut(1,"%c",c);
}
