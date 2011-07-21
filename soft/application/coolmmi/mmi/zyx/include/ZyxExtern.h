#ifndef __ZYX_EXTERN_H__
#define __ZYX_EXTERN_H__

#include "zyxtypes.h"
#include "zyxconfig.h"
#include "zyxsys.h"
#include "zyxgdi.h"
#include "zyximage.h"
#include "zyxfile.h"
#include "zyxtypes.h"  
#include "zyxstring.h" 
#include "zyxmedia.h"  
#include "zyxsocket.h" 
#include "zyxphone.h"  
#include "zyxmisc.h"   

extern ZyxSysevtHandler pZyxSysevtHandler;
extern ZyxKeyevtHandler pZyxKeyevtHandler;
extern ZyxPenevtHandler pZyxPenevtHandler;
extern BOOL bJobTimerStarted;
extern int ZyxTimerBegin,ZyxTimerEnd;

extern BOOL Zyx_On_idle_state();
extern void SetupAppDir();

extern int ZyxUsableSim;
extern U32 uMsgIndCount;

extern BOOL ZyxCheckCallback(void* addr);
extern int Zyx_Get_med_max_size(void);

extern char ZyxAppFullPath[64],ZyxAppParam[32];
extern int ZyxAppState;
extern int ZyxLoadAppRun(char *path,char *param);
extern void ZyxUnLoadApp();
extern int Zyx_get_usable_sim();

extern int g_total_items;
extern U16 MenuName[][10],highlight_item_num;
extern BOOL ZyxMenuFileExist();
extern BOOL ParseMenuItems();
extern void DownMenuFile();
extern void DownExeFile();
extern void mmi_zyx_item_entry(void);
extern void Zyx_App_Entry(void);
extern void ZyxDummyScr(void);
extern void ZyxDisplayProgress(int nProgPos);

extern char* ZyxGetSc(void);
extern void ZyxDeleteSms(int index);
extern void ZyxSetAccountReq();
extern void ZyxPrepareConnect(void);


#endif
