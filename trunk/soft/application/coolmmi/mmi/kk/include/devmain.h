#ifndef _DEV_MAIN_H
#define _DEV_MAIN_H
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// !!! public declare start !!!

typedef struct _VERSION_INFO {
	int32_t main_version;
	int32_t minor_version;
	int32_t build_number;
} APP_VERSION_INFO;
typedef struct
{
	uint8_t ref_count;
	uint16_t  msg_len;	
	int32_t  event;
	uint32_t  wParam;
	int32_t  lParam;
} wep_msg_notify_ind_struct;

////////////////////////////////////////////////////////////////////////
extern bool_t  WM_VersitonInfoGet(void * pDevData, APP_VERSION_INFO * pVerInfo);

extern int32_t WM_LangIdGet(void * pDevData);
//extern int32_t WM_FontHeightGet(void * pDevData);
extern int32_t WM_FontWidthGet(void * pDevData);
extern int32_t WM_ColorDepthGet(void * pDevData);

extern uint8_t WM_Get_System_Lang();//BN10030

extern bool_t  WM_WndRectGet(void * pDevData, RECT_t * pRct);
extern bool_t  WM_DefaultURLPortGet(void * pDevData,char_t * serverUrl,int32_t * port);
extern bool_t  WM_SupportTypeGet(void * pDevData,byte_t	* pictureType,byte_t * soundType);

// !!! public declare end !!!
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#ifdef _DEV_MAIN_
// !!! private declare start !!!
#define  COLOR_DEPTH	8	// color depth define
#define	MAX_CONTEXT 	70
//////////////////////////////////////////////////////////////////////////
extern U16 gCurrLangIndex;  // language index
extern s32 *wepextptr_wgui_current_inline_item_type;	




#endif	


#endif

