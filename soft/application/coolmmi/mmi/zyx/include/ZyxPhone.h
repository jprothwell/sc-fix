#ifndef __ZYX_PHONE_H__
#define __ZYX_PHONE_H__


#include "zyxtypes.h"

typedef void (*ZyxSMSCB) (char* num,int result);
typedef BOOL (*ZyxContentCB) (S8* smsNumber,U16 *pContent);

extern int ZyxSendSms(char* num, char * msg, int nTimes, ZyxSMSCB cb);
//extern int ZyxSendSms(char* num,U16* msg, int nTimes,ZyxSMSCB cb);
extern void ZyxSmsAddHoldKey(U16* key,U16 *replykey,U16 *replycontent,ZyxContentCB cb);
void ZyxSmsAddChannelInfo(int chid,char* key,char *replykey,char *replycontent,ZyxContentCB cb);

extern void ZyxMakeCall(char* num);


#endif
