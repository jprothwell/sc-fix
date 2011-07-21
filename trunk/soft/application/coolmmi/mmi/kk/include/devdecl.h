#ifndef _DEVDECL_H
#define _DEVDECL_H
//设备相关函数声明

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#include "typedefs.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//  event define
typedef enum
{
	WEP_DATA_ARRIVAL = 1,	// data package arrival
#ifdef SUPPORT_FTP
	WEP_FILE_TRANSFER,		// file transfer
	WEP_FILE_SELECT,		// file select
#endif
	WEP_AUTOLOGON,//auto logon WEP server
	WEP_REGISTER,    
	WEP_EXIT,
	WEP_CAMERA_GET_CB,
#ifdef WEP_NET_SEND_OPT
	WEP_DATA_SEND,
#endif
	WEP_SHOW_CHAT_TEXT,
#ifdef SUPPORT_BLOCK_SP_SMS	
	WEP_SMS_SEND,
#endif
} APPLICATION_EVENTS;

// debug output function
void WM_DebugPrint(const char * fmt, ...);
void WEP_DebugPrint(const char * fmt, ...);
extern int32_t kk_is_support_dual_cam(void);
extern int32_t kk_is_support_horizon_cam(void); //BN10122
extern void    kk_init_cam_sensor(void);
///////////////////////////////////////////////////////////////////////////////
extern void *  DEV_Malloc(int32_t moduleId, int32_t sizeInBytes);
extern void		 DEV_Free(void * p);
extern void *  DEV_MemCpy(void *dest, const void *src, int32_t count);
extern void *  DEV_MemSet(void *dest, int32_t c, int32_t count);
extern int32_t DEV_MemCmp(const void * p1, const void * p2, int32_t count);
extern int32_t DEV_MemFind(const byte_t * pBuf, int32_t nBufLen,const byte_t * pFind, int32_t nFindLen);

///////////////////////////////////////////////////////////////////////////////
extern int32_t DEV_TimerStart(void * pDevData, int32_t timerId,
				int32_t nElapseMs, PFNTIMERCALLBACK pfn, void * pUserData);
extern int32_t DEV_TimerStop(void * pDevData, int32_t timerId);

///////////////////////////////////////////////////////////////////////////////
extern int32_t DEV_RandNumGet();

///////////////////////////////////////////////////////////////////////////////
extern int32_t DEV_LocalIconInfoGet(void * pDd, byte_t buf[], int32_t bufLen);

///////////////////////////////////////////////////////////////////////////////
extern void WM_MsgNotify(void * pDevData, int32_t event, uint32_t wParam, int32_t lParam);
extern int32_t WM_SmsSend(void * pDevData, byte_t mobileNo[], int32_t mobileNoLen,
								byte_t smsMsg[], int32_t smsMsgLen);
extern int32_t WM_LoadWAPBrowser(void * pDevData, byte_t URL[], int32_t URLLen);
//extern int32_t WM_Call(void * pDevData, byte_t mobileNo[], int32_t mobileNoLen);//james 070711
extern void WM_BackLightSet(void * pDevData, bool_t bLightOn);

//
extern uint32_t DEV_GetPlatSWVer( void );
extern uint32_t DEV_GetPlatBBVer( void );
extern uint8_t DEV_IsMtkCodec( void );


///////////////////////////////////////////////////////////////////////////////
//export unicode func //BN10047
extern void DevInitUnicodeSupport(void);

extern int32_t (*pfnKKUnicodeStrlen) (const char *arrOut);
extern char* (*pfnKKUnicodeStrcpy) (char *strDestination, const char *strSource);
extern char* (*pfnKKUnicodeStrncpy) (char *strDestination, const char *strSource, uint32_t size);
extern int32_t (*pfnKKUnicodeStrcmp) (const char *string1, const char *string2);
extern int32_t (*pfnKKUnicodeStrncmp) (const char *string1, const char *string2, uint32_t size); 
extern char* (*pfnKKUnicodeStrcat) (char *strDestination, const char *strSource);
extern char* (*pfnKKUnicodeStrncat) (char *strDestination, const char *strSource, uint32_t size);

extern uint16_t (*pfnKKAnsiiToUnicodeString)(char *pOutBuffer, char *pInBuffer);
extern uint16_t (*pfnKKUnicodeToAnsii)(char *pOutBuffer, char *pInBuffer);

extern uint8_t DEV_IsKKDec( void );
extern int Dev_UJDecodeFrame(unsigned char  * inputData, int inputDataSize,int image_width,int image_height,unsigned char *p_Out,int bufsize);
/*extern int Dev_UJEncodeFrame(unsigned char  * RgbData, int RgbDataSize,int image_width,int image_height,int out_jpeg_width, int out_jpeg_height, \
	unsigned char *p_Out,int bufsize,int q);*/
extern int Dev_DecodeFrame(unsigned char  * inputData, int inputDataSize,int image_width,int image_height,unsigned char *p_Out,int bufsize);
extern int Dev_EncodeFrame(unsigned char  * RgbData, int RgbDataSize,int image_width,int image_height,int out_jpeg_width, int out_jpeg_height, \
	unsigned char *p_Out,int bufsize,int q);
extern uint16_t DEV_GetImgId( uint16_t kk_img_id );
//bruce091218
#ifdef __KK_UI_V2__
#ifdef __MMI_UI_LIST_HIGHLIGHT_EFFECTS__
extern void gui_block_list_effect(void);
extern void gui_unblock_list_effect(void);
#endif

#if(KK_MTKXXXX_SWVER >= KK_MTKXXXX_V08B)
typedef uint32_t (*wgui_cat_get_list_menu_flags_funcptr_type)(void);
extern void wgui_cat_register_get_flags_callback(wgui_cat_get_list_menu_flags_funcptr_type get_flags_callback);
#endif

extern void UI_GetSystemThemes( void );
extern void UI_RestoreSystemThemes( void );

//bruce100105 for BN10070
extern uint32_t g_wep_start_time;
extern void DEV_SetVideoStartTime( void );

//bruce for adjust horizontal x offset
extern uint8_t DEV_SetVideoOffsetX( void );

extern void DEV_GeVideoResolution( video_resol_id *local, video_resol_id *remote );
extern void DEV_CameraGetResolution(uint16_t *cap_width, uint16_t *cap_height, uint16_t *transf_width, uint16_t *transf_height);
extern int32_t WM_FontHeightGet(void );


#endif /* __KK_UI_V2__ */


//bruce100113 b
#if (KK_MTKXXXX_BBCHIP == KK_MTKXXXX_MT6223)
#ifdef _KK_DSP_GS500_SUPPORT_
#define KK_DSP_CAPTURE_WIDTH 		80
#define KK_DSP_CAPTURE_HEIGHT 	64
#define KK_DSP_TRANSFER_WIDTH 	80
#define KK_DSP_TRANSFER_HEIGHT 	64

extern bool_t gs500_kk_camera_pwron(void);
extern void gs500_kk_camera_preview(void);
extern void gs500_kk_camera_capture(unsigned  char *jpge_buffer, unsigned  int *jpge_size);
extern void gs500_kk_camera_pwroff(void);
#endif
#endif
//bruce100113 e

extern void DEV_BitBlt(int32_t x1, int32_t y1, int32_t x2, int32_t y2);   ////BN10112 add

extern int DEV_IsNoneCamera( void );

#endif

