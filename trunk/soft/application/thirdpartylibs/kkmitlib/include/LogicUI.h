#ifndef _LOGICUI_H
#define _LOGICUI_H

#include "extdecl.h"

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// !!! public declare start !!!

// UIP parse return value
#define LUI_ERROR								-1 // error
#define LUI_NULL								0  // null 
#define LUI_WIN_ONLY						1  // only UI win
#define LUI_MSG_ONLY						2  // only msg
#define LUI_WIN_AND_MSG					3  // both UI win and msg
#define LUI_WIN_CACHE						4  // catch UI

#ifdef VIDEO_CACHE
#define  MAX_VIDEO_CACHED_NUM	30
#endif

/////////////////////////////////////////////////////////////////////////
// UI Package length
#if ((CLIENT_MAJOR_VERSION == 1) && (CLIENT_MINOR_VERSION == 0))
	#define UIP_TOTAL_LENGTH(pPackBuf) \
		(int32_t)(UIP_OFFSET_VL \
		+ (pPackBuf[UIP_OFFSET_MESSAGE_SIZE] & 0xFF)  \
		+ (pPackBuf[UIP_OFFSET_PROPERTY_SIZE] & 0xFF) \
		+ (int32_t)((pPackBuf[UIP_OFFSET_SCREEN_SIZE_HIGH] & 0xff) <<8 | \
							pPackBuf[UIP_OFFSET_SCREEN_SIZE_LOW] & 0xff)  \
		+ (int32_t)((pPackBuf[UIP_OFFSET_COMMAND_SIZE_HIGH] & 0xff)<<8 | \
							pPackBuf[UIP_OFFSET_COMMAND_SIZE_LOW] & 0xff) \
		+ (int32_t)((pPackBuf[UIP_OFFSET_PICTURE_SIZE_HIGH] & 0xff)<<8 | \
							pPackBuf[UIP_OFFSET_PICTURE_SIZE_LOW] & 0xff) \
		+ (int32_t)((pPackBuf[UIP_OFFSET_SOUND_SIZE_HIGH] & 0xff)  <<8 | \
							pPackBuf[UIP_OFFSET_SOUND_SIZE_LOW] & 0xff) )
#else
	#define UIP_TOTAL_LENGTH(pPackBuf) \
		(int32_t)(UIP_OFFSET_VL \
		+ (pPackBuf[UIP_OFFSET_MESSAGE_SIZE] & 0xFF)  \
		+ (int32_t)((pPackBuf[UIP_OFFSET_PROPERTY_SIZE_H] & 0xff) <<8 | \
							pPackBuf[UIP_OFFSET_PROPERTY_SIZE_L] & 0xff)  \
		+ (int32_t)((pPackBuf[UIP_OFFSET_SCREEN_SIZE_HIGH] & 0xff) <<8 | \
							pPackBuf[UIP_OFFSET_SCREEN_SIZE_LOW] & 0xff)  \
		+ (int32_t)((pPackBuf[UIP_OFFSET_COMMAND_SIZE_HIGH] & 0xff)<<8 | \
							pPackBuf[UIP_OFFSET_COMMAND_SIZE_LOW] & 0xff) \
		+ (int32_t)((pPackBuf[UIP_OFFSET_PICTURE_SIZE_HIGH] & 0xff)<<8 | \
							pPackBuf[UIP_OFFSET_PICTURE_SIZE_LOW] & 0xff) \
		+ (int32_t)((pPackBuf[UIP_OFFSET_SOUND_SIZE_HIGH] & 0xff)  <<8 | \
							pPackBuf[UIP_OFFSET_SOUND_SIZE_LOW] & 0xff) )
#endif

// !!! public declare end !!!
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#ifdef _LOGIC_UI_
// !!! private declare start !!!

typedef struct _LUI_Item
{
	int32_t  type;
//	int32_t  id;
	int32_t  sizeLimit;
	int32_t  titleSize;
	byte_t * title;
	int32_t  valueSize;
	byte_t * value;
} LUI_Item;

typedef struct _LUI_SubItem
{
	int32_t subItemID;
	int32_t whichChoiceGroup;
	int32_t index;
	int32_t nPropertyLen;
	byte_t  property[MAX_SUBITEM_PROPERTY_LEN];
	//bruce0912010
#ifdef __KK_UI_V2__
	byte_t strLabel[MAX_SUBITEM_CAPTION_LEN];
#endif				
} LUI_SubItem;

typedef struct _LUI_ChatPlayerData
{
	int32_t iPlayerID;	
	byte_t  playerName[MAX_CHAT_PLAYER_NAME_LEN+2];	// nick name of chat player
	int16_t nPlayerType;	// type of chat: WEP/MSN/QQ
	int16_t nServerId;		// id of the server which chat player connected
} LUI_ChatPlayerData;

typedef struct _LUI_ChatData
{
	byte_t chatHistoryBuffer[MAX_CHAT_ITEM_NUMBER * MAX_CHAT_MESSAGE_SIZE * 2];
	byte_t chatUserInput[MAX_CHAT_MESSAGE_SIZE * 2];
	int16_t  chatInputLen;			// chat user input message length

	byte_t * pChatHistMessage[MAX_CHAT_ITEM_NUMBER];	// pointers to history message
	int16_t  chatHistMsgLen[MAX_CHAT_ITEM_NUMBER];			// chat history message length
	int16_t  chatHistHeaderLen[MAX_CHAT_ITEM_NUMBER];		// chat history header length
	int16_t  chatRecordTypeIndex[MAX_CHAT_ITEM_NUMBER];	// such as WEP/MSN/QQ
	COLOR_t  chatHistTextColor[MAX_CHAT_ITEM_NUMBER][2];	// chat history text color(0:header;1:content)
	
	int16_t  numberOfHistMsg;
	int16_t  textCursorPos;	// current cursor position

} LUI_ChatData;

typedef struct _LUI_SoftKey
{
	int32_t softKeyID;
	int16_t type;
	int16_t softKeyPos;
	int32_t requestCode;
	int32_t subRequestCode;
	int32_t softKeyTitleLen;
	byte_t  softKeyTitle[MAX_SOFTKEY_TITLE_LEN];
} LUI_SoftKey;


typedef struct __LUI_Data LUI;
struct __LUI_Data
{
	ILUI Vtbl;	// must be the first element!

	WEPAppData * pWep;

	int32_t winID;
 	int32_t winType;
 	int32_t winLevel;
 	byte_t transferType;	
 	bool_t isWinCachable;
	
	COLOR_t chatTextColorType[MAX_CHAT_TEXT_COLOR_TYPE][2];	// color type of chat message text
	int32_t  nChatRoomId;
	int32_t titleLen;
	byte_t chatRoomTitle[MAX_SCREEN_TITLE_LEN/sizeof(byte_t)];

	// chatter data
	LUI_ChatPlayerData chatPlayerData[MAX_CHAT_PLAYER_NUMBER];
	int32_t nBuddyNum;	
	LUI_ChatData * pChatData; // added by czm 070115

//	LUI_ChatData * pAllChatData; //point to chat data, for malloc&free memory 
	// 2* sizeof(LUI_ChatData)
	// pAllChatData im ; pAllChatData+1 chatroom
	// added by czm 070115

	int32_t subItemNum;
	LUI_SubItem subItem[UI_MAX_SUBITEM_NUM];

	int32_t choiceGroupNum;
	int32_t indexInGroup[UI_MAX_CHOICEGROUP_NUM]; // item num in choice group

	int32_t checkBoxNum;//check box ctl num
	int32_t indexIncheckBox[UI_MAX_CHOICEGROUP_NUM];//the num of item in one check box

	int32_t itemID;

	int32_t hiddenItemNum;
	
	int32_t inputStrNum;

	int32_t numOfSoftKey;	
	LUI_SoftKey softKey[UI_MAX_SOFTKEY_NUM];	

	int32_t		idOfCachedUIP[MAX_CACHED_UIP_NUM];	// id of cached UIP
	byte_t		priOfCachedUIP[MAX_CACHED_UIP_NUM];	// priority of cached UIP

#ifdef VIDEO_CACHE
	int32_t videoCacheMutex;
	int32_t videoCacheNum;
	byte_t videoCacheBuff[MAX_VIDEO_CACHED_NUM][WEP_MAX_PIC_SIZE];
	int32_t videoCacheBuffLen[MAX_VIDEO_CACHED_NUM];
#endif

	//bruce0912010
#ifdef __KK_UI_V2__
	byte_t strLabel[UI_MAX_SUBITEM_NUM][MAX_SUBITEM_CAPTION_LEN];
#endif			

};


// !!! private declare end !!!
#endif	// _LOGIC_UI_
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#endif//_LOGICUI_H

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// end of file 
