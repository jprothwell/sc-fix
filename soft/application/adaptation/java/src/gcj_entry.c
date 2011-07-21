extern void entry_main(unsigned char *appName);
extern void Jv_RuntimeInit();
extern void Jv_RuntimeDeinit();
extern void gcj_init_ctor();
extern void gcj_init_jcr();
extern void gcj_dinit_jcr();
extern void sleep(unsigned int misec);
extern void gcj_reset_native();
extern void gcj_reset_data();
extern void gcj_TraceOut(int index,char * fmt,...);
extern unsigned int csw_TMGetTick( void);

unsigned int checkQuitTime=0;
unsigned char trace_class_loading;
unsigned char javaentry=0;
unsigned char javaLIfe=0;
 extern void lanch_j2me(unsigned char *appName)
{
	trace_class_loading = 0;
	javaLIfe = 1;
	gcj_init_ctor();
	gcj_init_jcr();
	Jv_RuntimeInit();
	javaentry = 1;
	if (!strcmp(appName,"qq"))
	{
#if defined (LCDSIZE_176_220) && defined(__MMI_LARGE_FONTS__)
		gcj_SetJavaColor(0x8EA9DE,0x6986B4,0x00000000);
		gcj_qq2007_MainMIDlet();
#else
		gcj_com_tencent_kqq2006_ui_MainMIDlet();
#endif
	}
	else	
		abort();
	javaentry=0;
	gcj_TraceOut(1,"lanch_j2me quit start at %d ticks\n",csw_TMGetTick());
	gcj_dinit_jcr();
	Jv_RuntimeDeinit();
	gcj_reset_native();
	gcj_reset_data();
	gcj_TraceOut(1,"lanch_j2me quit end at %d ticks\n",csw_TMGetTick());
}

 
 
unsigned char gcj_IsJavaOn()
{
 return javaentry;
}

unsigned char gcj_isJavaLife()
{
 return javaLIfe;
}

unsigned char IsJavaOn()
{
    return gcj_IsJavaOn();
}