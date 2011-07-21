#ifndef _WEP_MAIN_H
#define _WEP_MAIN_H

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// !!! public declare start !!!

////////////////////////////////////////////////////////////////////////
extern void *  WEP_AppInit(void * pDevData);

extern bool_t  WEP_AppStart(void * pWepData);
extern bool_t  WEP_AppExit(void * pWepData);

extern bool_t  WEP_AppSuspend(void * pWepData);
extern bool_t  WEP_AppResume(void * pWepData);
 
extern bool_t  WEP_NetCB(void * pWepData, int32_t nCode);




#endif 


