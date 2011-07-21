#ifndef _ExtDecl_H
#define _ExtDecl_H
//Íâ²¿ÉùÃ÷

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// constants define (must be identical to the value in ProtHandle.h)
#define WEP_WUI_WAIT_SCREEN		60		// WUI_WAIT_SCREEN
#define WEP_WUI_MAIN_SCREEN		99		// WUI_MAIN_SCREEN
#define WEP_WUI_CHAT_SCREEN		253		// WUI_CHAT_SCREEN
#define WEP_WUI_EXIT_SCREEN		255		// WUI_EXIT_SCREEN

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define CHAT_FACE_ID_BASE	180	// base of chat face picture id

#define CHAT_ICON_ID_BASE	220	// base of chat icon picture id
#define MAX_CHAT_ICON_NUM	(16+1+3)	 
															// +1: private chat icon, icon index: 236
															// +3: MSN: 237, ICQ: 238, QQ: 239

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// softkey id define
#define UI_SOFTKEY_ID_BASE				0x100
#define GAME_SOFTKEY_ID_BASE			0x200

#define SOFTKEY_ID_HAS_MSG		0x300
#define SOFTKEY_ID_FTP				0x301

#ifdef SUPPORT_POPMENU
#define SOFTKEY_ID_POPMENU				0x302
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// soft key position define
#define SKEY_POS_OK				0
#define SKEY_POS_BACK			1

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// external Function Prototypes
extern ILNT * LNT_Create(WEPAppData * pWep);
extern void   LNT_Destroy(ILNT * pILnt);

extern INET * NET_Create(WEPAppData * pWep);
extern void   NET_Destroy(INET * pINet);

extern ILUI * LUI_Create(WEPAppData * pWep);
extern void   LUI_Destroy(ILUI * pILui);

extern IBuilder * UI_BuilderCreate(WEPAppData * pWep);
extern void   UI_BuilderDestroy(IBuilder * pIBuilder);

extern IPDP * PDP_Create(WEPAppData * pWep);
extern void   PDP_Destroy(IPDP * pIPdp);

extern ILD *  LD_Create(WEPAppData * pWep);
extern void	  LD_Destroy(ILD * pLocalStore);

extern int32_t UTL_Byte2NetUniByte(byte_t * pDest, const byte_t * pSrc, int32_t nLen);
extern int32_t UTL_Ascii2NetUniByte(byte_t * pDest, const char_t * pSrc);
extern int32_t UTL_NetUniByte2Ascii(char_t * pDest, const byte_t * pSrc, int32_t srcLen);

extern void WEP_AppException(WEPAppData *pWep, int32_t iExcNum);

#endif

