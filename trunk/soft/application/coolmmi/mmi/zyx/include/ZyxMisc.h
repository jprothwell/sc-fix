#ifndef __ZYX_MISC_H__
#define __ZYX_MISC_H__

#include "zyxtypes.h"

typedef struct
{
    char UpdateSerial[25]; //升级序列号
    char MidVer[10];    //中间件版本
    char factory[8];      //厂商名，最大7个字符，空字节填\0
    char modelid[8];      //mobile id，最大7个字符，空字节填\0
    char scnumber[24];      // sim卡短信中心号码
    char *Reserved[10];       //备用
}ZyxUserInfo,*ZyxUserInfoPtr;

typedef struct
{			
	U8	mcc[3];	//MCC
	U8	mnc[3];	//MNC
	U8	lac[2];	//LAC
	U16	cell_id;	 //cell ID
	U8	nc_info_index; // index in rlc array to acquire the corresponding arfcn, bsic, rxlevel...
} ZyxCellInfo,*ZyxCellInfoPtr;

typedef void (*stack_func_ptr)(void);
typedef int (*ZyxEditorCb)(BOOL bComfirm);
typedef int (*ZyxCellInfoCb)(ZyxCellInfoPtr cellinfo);

extern void ZyxGetUserinfo(ZyxUserInfoPtr userinfo);
extern char* ZyxGetImei(void);
extern char* ZyxGetImsi(void);
extern void ZyxEnableBacklight(void);
extern void ZyxDisableBacklight(void);
extern void ZyxSetWallPaper(char* file_name);
extern void ZyxSetPhoneTone(char* file_name);

extern void ZyxEnterEditor(U16 *content,int size,ZyxEditorCb cb);
extern void ZyxOpenSMS(U16 *sms_content);
extern void ZyxOpenMMS(U16 *phone_number, U16 *mms_subject, U16 *mms_content, U16 *file_path);
extern void ZyxSendMMS(U16 *phone_number, U16 *mms_subject, U16 *mms_content);

extern void ZyxGetCellInfo(ZyxCellInfoCb cb);
extern void ZyxStopGetCell ();
extern U32 INT_SwitchStackToRun(void *stack_start, U32 stack_size, stack_func_ptr func, U32 argc, ...);
extern void ZyxTcmAddrAndSize(PU8 *addr,U32 *size);
extern void ZyxActiveDone(void);

extern int ZyxUnzip( char *src_file, char *dst_path);

#endif
