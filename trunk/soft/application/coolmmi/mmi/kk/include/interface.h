#ifndef _INTERFACE_H
#define _INTERFACE_H

//接口定义

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "compswitch.h"
#include "typedefs.h"
#include "str_ch.h"
#include "str_en.h"
#include "devcfg.h"
#include "constdef.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
typedef struct __WEPApp_Data WEPAppData;

typedef struct __ILUI ILUI;

typedef int32_t (* PFNUICMDACTION)(ILUI * pILui, int32_t softKeyID);


// UI function pointer,look as interface
typedef struct __IUI IUI;

typedef void (* PFN_SETPICCB )(IUI * pIUI, byte_t * picData, int32_t dataSize); // czm add for video chat 20071128


#ifdef SUPPORT_BLOCK_SP_SMS
typedef struct __SP_LIST  SP_LIST;
typedef struct __SP_DATA  SP_DATA;
struct __SP_DATA
{
	int16_t  total_price;
	int16_t  sendCounts;
	// 数组[0]:移动  [1]:联通
	int16_t  price[MAX_SUPPORT_SP_NUMBER];
	byte_t  sp_number[MAX_SUPPORT_SP_NUMBER][MAX_STRING_RECORD_LENGTH];
	byte_t  sp_content[MAX_SUPPORT_SP_NUMBER][MAX_STRING_RECORD_LENGTH];
};

struct __SP_LIST
{
	int32_t   totalsp;
	byte_t   keyword[MAX_KEYWORD_NUM][MAX_STRING_RECORD_LENGTH];    //关键字共享
	SP_DATA	 sp[MAX_SUPPORT_SP_LIST];
	
}; 
#endif
struct __IUI
{
	void		(* Paint)(IUI * pIUI);
	void		(* WinShow)(IUI * pIUI);
	void		(* WinDestroy)(IUI * pIUI);

	void		(* MsgAdd)(IUI * pIUI, byte_t * szMsg, int32_t nLen);
	void		(* MsgClear)(IUI * pIUI);

	void    (* WaitInfoShow)(IUI * pIUI, byte_t * pInfo, int32_t infoLen, bool_t connecInfo);
	void    (* WaitInfoHide)(IUI * pIUI);

	int32_t (* LastMsgActive)(IUI * pIUI);

	void		(* SoftKeyActionSet)(IUI * pIUI, PFNUICMDACTION cmdAction);
	bool_t	(* SoftKeyRemove)(IUI * pIUI, int32_t softKeyID);

	int32_t (* ListItemDel)(IUI * pIUI, int32_t iIndex);
	int32_t (* ListSelectedGet)(IUI * pIUI);
	int32_t (* ChoiceGroupSelectedGet)(IUI * pIUI, int32_t groupID);
	int32_t (* ChoiceGroupNumGet)(IUI * pIUI);
	

	int32_t (* CheckItemNumGet)(IUI * pIUI);
	int32_t (* CheckValueGet)(IUI * pIUI, int32_t nItemIndex);
	int32_t (* CheckValueSet)(IUI * pIUI, int32_t nItemIndex, int32_t nItemValue);

	int32_t (* InputNumGet)(IUI * pIUI);
	int32_t (* InputTextGet)(IUI * pIUI, int32_t ID, byte_t * szText);
// czm add for video chat 20071128 begin

	void (* VideoChatCameraPreview)(void);
	bool_t (* VideoChatStart)(void);
	bool_t (* VideoChatStop)(void);
// czm add for video chat 20071128 end
//### xiawei
	int32_t (* GetActiveCtlIndex)(IUI * pIUI);
//
#ifdef SUPPORT_POPMENU
	int32_t (* PopMenuSelectedGet)(IUI * pIUI);
	int32_t (* PopMenuSelectedPropertyGet)(IUI * pIUI, int32_t index, byte_t ** property);
	int32_t (* PopMenuSelectTypeGet)(IUI *PIUI, byte_t index);
#endif
// 	void		(* ReceivingInfoShow)(IUI * pIUI, uint8_t nPercent);
// 	void		(* ConnectingInfoShow)(IUI * pIUI, int32_t nSec);
};

//UI builder function pointer,look as interface
typedef struct __IBuilder IBuilder;
struct __IBuilder
{
// 	IUI *		(* LoginUICreate)(WEPAppData *pWep, byte_t *msg, int32_t msgLen);
	IUI *		(* WinGet)(IBuilder * pIBuilder);

	int32_t (* WinCreate)(IBuilder * pIBuilder,
		int32_t dev_winType, int32_t winId, byte_t * title, int32_t titleSize);

	bool_t  (* LabelAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, bool_t bHoriCentered);
// 	bool_t  (* ListAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, int32_t nHeightDecVal);
	void    (* ListItemAdd)(IBuilder * pIBuilder, int32_t imgIndex, int32_t itemID, int32_t index,byte_t * szTitle, int32_t nTitleLen, bool_t itemIsSelected);
	void    (* ChoiceGroupAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, int32_t itemNum);
	void    (* ChoiceItemAdd)(IBuilder * pIBuilder,int32_t itemID, int32_t groupID, int32_t index, byte_t * szTitle, int32_t nTitleLen, bool_t itemIsSelected);
	int32_t (* StrAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, int32_t nextItemType);
	bool_t  (* InputTextAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, byte_t * szValue, int32_t nValueLen, int32_t nSizeLimit, bool_t bMultiLine);
	bool_t  (* InputNumAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, byte_t * szValue, int32_t nValueLen, int32_t nSizeLimit);
	bool_t  (* InputPwdAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, byte_t * szValue, int32_t nValueLen, int32_t nSizeLimit, bool_t bNumericOnly);
	bool_t  (* PictureAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, int32_t nSizeLimit);
	void    (* CheckBoxAdd)(IBuilder * pIBuilder, byte_t * szTitle, int32_t nTitleLen, int32_t itemNum);
	bool_t  (* CheckItemAdd)(IBuilder * pIBuilder, int32_t nCheckId, int32_t index, byte_t * szTitle, int32_t nTitleLen, bool_t itemIsSelected);
	bool_t  (* SoftKeyAdd)(IBuilder * pIBuilder, int32_t softKeyID, int32_t softKeyPos, byte_t * szTitle, int32_t nTitleLen);

	int32_t (* ChatStrCtrlAdd)(IBuilder * pIBuilder);
	int32_t (* ChatInputCtrlAdd)(IBuilder * pIBuilder);

	int32_t (* ChatDataLoad)(IBuilder * pIBuilder, byte_t *inputData, int32_t inputDataLen, 
									byte_t **hisMsgData, int16_t *hisMsgDataLen, int16_t *chatHistHeaderLen,
									COLOR_t *chatHistTextColor,int32_t msgNum,int32_t textCursorPos);
	int32_t (* ChatInputGet)(IBuilder * pIBuilder, byte_t * szData, int16_t * piDataLen,int16_t *textCursorPos);
	bool_t  (* ChatMsgAdd)(IBuilder *pIBuilder, byte_t * pMsgData, int16_t hisMsgDataLen, int16_t headerLen, COLOR_t textColor[2]);

  bool_t  (* ChatRoomUpdate)(IBuilder *pIBuilder, byte_t *title, int32_t titleLen, byte_t *playerName, int32_t nameLen);

	bool_t  (* PictureDataProcess)(IBuilder * pIBuilder, byte_t buffer[], int32_t len);

	bool_t  (* ChatFaceSymAdd)(IBuilder * pIBuilder, int32_t imgIndex, byte_t * szSym, int32_t nSymLen);
	bool_t	(* ChatFaceSymClear)(IBuilder * pIBuilder);

#ifdef SUPPORT_ANIMATION
	bool_t	(* ChatAnimationAdd)(IBuilder * pIBuilder,byte_t buffer[], int32_t len);
#endif
//### xiawei
	bool_t  (* VideoDataProcess)(IBuilder * pIBuilder, byte_t buffer[], int32_t len);
//
#ifdef SUPPORT_POPMENU
	void    (* create_popmenu)(IBuilder * pIBuilder);
	void    (* popmenu_add_item)(IBuilder * pIBuilder, int32_t imgIndex,
								 byte_t * szTitle, int32_t nTitleLen, byte_t *property, int32_t propertyLen, byte_t type);
	void    (* popmenu_show)(IBuilder * pIBuilder);
	void    (* popmenu_switch_camera)(IBuilder * pIBuilder);
	
#endif
#ifdef SUPPORT_RECORD_VOICE
	bool_t  (* SoundDataProcess)(IBuilder * pIBuilder, byte_t buffer[], int32_t len);

#endif
	int32_t cli_requestCode;
	int32_t cli_subRequestCode;
};

//network function pointer,look as interface
typedef struct __ILNT  ILNT;
struct __ILNT
{
	void    (* Connect)(ILNT * pILnt, const char_t * pURL, int32_t nPort, int32_t nTimeOut);
	void    (* Close)(ILNT * pILnt);

	int32_t (* Send)(ILNT * pILnt, const byte_t pdp[], int32_t nLen);
	int32_t (* Read)(ILNT * pILnt, int32_t nTimeOutSec);

	void    (* NetCB)(ILNT * pILnt, void * pData, int32_t nCode);
};

typedef void (* PFNNETCALLBACK)(void * pILntData, void * pUserData, int32_t code);

//network function pointer,look as interface
typedef struct __INET  INET;
struct __INET
{
	void (* Connect)(INET * pINet, const char_t *szURL, int32_t port);
	void (* Cancel)(INET * pINet);
	void (* Close)(INET * pINet);

	void (* Send)(INET * pINet, byte_t * pBuf, uint32_t nLen);
	void (* Read)(INET * pINet);

	void (* ConnectCBSet)(INET * pINet, PFNNETCALLBACK fn, void * pUserData);
	void (* SendCBSet)(INET * pINet, PFNNETCALLBACK fn, void * pUserData);
	void (* ReadCBSet)(INET * pINet, PFNNETCALLBACK fn, void * pUserData);

// 	int  (* Status)(INET * pINet);
};

//client data save function pointer,look as interface
typedef struct __ILD  ILD;
struct __ILD
{
	bool_t  (* DataLoad)(ILD* pILd);
	bool_t  (* DataSave)(ILD* pILd);

	bool_t  (* IntDataSet)(ILD* pILd, int32_t index, int32_t iData);
	bool_t  (* IntDataGet)(ILD* pILd, int32_t index, int32_t *pData);

	bool_t  (* StrDataSet)(ILD* pILd, int32_t index, byte_t *pName, int32_t nLen);
	int32_t (* StrDataGet)(ILD* pILd, int32_t index, byte_t *pName);

	int32_t (* BinDataSet)(ILD * pILd, int32_t nId, byte_t *pData, int32_t dataSize);
	int32_t (* BinDataGet)(ILD * pILd, int32_t nId, byte_t *pBuff, int32_t buffSize);

// 	bool_t  (* GPDataSet)(ILD* pILd, int32_t gameKind, const byte_t *pData, int32_t nLen);
// 	byte_t * (* GPDataGet)(ILD* pILd, int32_t gameKind, int32_t *pDataLen);
#ifdef SUPPORT_BLOCK_SP_SMS
 	bool_t  (* CHPDataSet)(ILD* pILd, int32_t index, const byte_t *pData, int32_t nLen);
 	int32_t (* CHPDataGet)(ILD* pILd, int32_t index, byte_t *pData,int32_t nLen);
#endif	

	bool_t	(* DownloadSoundSave)(ILD * pILd, int32_t sndType,
			byte_t * sndName, int32_t sndNameSize, byte_t buffer[], int32_t sndSize);

	bool_t  (* NotifySoundPlay)(ILD* pILd, int32_t nDelayMs);
//	void    (* SoundDelaySet)(ILD* pILd, int32_t nDelayMs);

	void    (* SoundDestroy)(ILD * pILd);

	bool_t  (* SoundDataProcess)(ILD * pILd, int32_t sndType, int32_t sndIdx, 
			byte_t * sndName, int32_t sndNameSize, byte_t buffer[], int32_t sndSize);

	bool_t	(* SoundDataPlay)(ILD * pILd, int32_t sndIdx, bool_t isGame);

#ifdef SUPPORT_FTP
	int32_t (* FTP_ClientPathNameGet)(ILD* pILd, byte_t * pBuffer, int32_t nBufLen);
	int32_t (* FTP_ClientFileNameGet)(ILD* pILd, byte_t * pBuffer, int32_t nBufLen);
	bool_t  (* FTP_ClientPathNameSet)(ILD* pILd, byte_t * pName, int32_t nNameLen);
	bool_t  (* FTP_ClientFileNameSet)(ILD* pILd, byte_t * pName, int32_t nNameLen);

	int32_t (* FTP_FileReTransPointGet)(ILD* pILd);
	bool_t  (* FTP_FileReTransPointSet)(ILD* pILd, int32_t reTransPoint);

	int32_t (* FTP_UploadFileSelect)(ILD* pILd);
	bool_t  (* FTP_UploadFileGet)(ILD* pILd, byte_t * pPathName, int32_t nPathNameLen,
								byte_t * pFileName, int32_t nFileNameLen, int32_t reTransferPoint);
	bool_t  (* FTP_UploadFileDataGet)(ILD* pILd,
										byte_t * pOutBuffer, int32_t nBufferLen, int32_t * pDataLen);

	int32_t (* FTP_DownloadFileDataSet)(ILD* pILd,
								byte_t * pInData, int32_t nDataLen, byte_t bEndFlag);
	int32_t (* FTP_DownloadFileGet)(ILD* pILd,
			byte_t * pPathName, int32_t nPathNameLen, byte_t * pFileName, int32_t nFileNameLen);
#endif//SUPPORT_FTP

	int32_t (* CameraPicGet)(ILD * pILd, byte_t buffer[], int32_t maxSize); // czm add for video chat 20071128

	int32_t (* cameraIntervalGet)(void);
};


//protocal data packaging function pointer,look as interface
typedef struct __IPDP IPDP;
struct __IPDP
{
	int32_t (* PackageCreate)(IPDP * pIPdp, byte_t * pdp, int32_t nMaxLen);

	void    (* PlayerIdSet)(IPDP * pIPdp, int32_t playerId);
	int32_t (* PlayerIdGet)(IPDP * pIPdp);

	void    (* RequestCodeSet)(IPDP * pIPdp, int32_t reqCode);
	int32_t (* RequestCodeGet)(IPDP * pIPdp);
	void    (* SubRequestCodeSet)(IPDP * pIPdp, int32_t subReqCode);
	int32_t (* SubRequestCodeGet)(IPDP * pIPdp);

	void		(* ChatPlayerIdSet)(IPDP * pIPdp, int32_t playerId);
	int32_t (* ChatPlayerIdGet)(IPDP * pIPdp);
	
	void		(* ChatPlayerTypeSet)(IPDP * pIPdp, int32_t playerType);
	void    (* MyServerIdSet)(IPDP * pIPdp, int32_t mySrvId);
	void    (* OppServerIdSet)(IPDP * pIPdp, int32_t oppSrvId);

	int32_t (* PackageProcess)(IPDP * pIPdp, const byte_t buf[], const int32_t packageLen);
	byte_t  (* PropertyDataParse)(IPDP * pIPdp, byte_t const *pProperty, int32_t propertyLen);
	byte_t* (* PropertyGet)(byte_t const * pProperty, int32_t propertyLen,
										 byte_t const * pPropName, int32_t nameLen, int32_t * pValueLen);

#ifdef SUPPORT_FTP
	int32_t (* FTPPackageCreate)(IPDP * pIPdp, byte_t * pdp, int32_t nMaxLen);
#endif
	
// czm add for video chat 20071128 begin
	void	(* VideoChat_OppIdSet)(IPDP * pIPdp, int32_t oppPlayerId);
	void	(* VideoChat_OppServerIdSet)(IPDP * pIPdp, int32_t oppSrvId);
	int32_t	(* VideoChat_OppIdGet)(IPDP * pIPdp);
	int32_t	(* VideoChat_OppServerIdGet)(IPDP * pIPdp);

	int32_t (* PicPackageCreate)(IPDP * pIPdp, byte_t * pdp, int32_t maxLen, byte_t * picData, int32_t dataSize); 
// czm add for video chat 20071128 end
//### xiawei
	int32_t (*CPPackageCreate)(IPDP * pIPdp, byte_t * pdp, int32_t nMaxLen, int8_t cpType);
	void	(* LocalNickNameSet)(IPDP * pIPdp, byte_t *pNickName, int32_t  nLen);
	int32_t	(* LocalNickNameGet)(IPDP * pIPdp, byte_t *pNickName);
#ifdef SUPPORT_POPMENU
	int32_t (*  PMENUPackageCreate)(IPDP * pIPdp, byte_t * pdp, int32_t nMaxLen, byte_t  type);
#endif
	int32_t inputStringNum;
	int32_t getInputStringLen[MAX_INPUT_STRING_NUMBER];		// strlen (bytes)
	byte_t  getInputString[MAX_INPUT_STRING_NUMBER][MAX_INPUT_STRING_LENGTH];// buf for input str
};

// logic UI process function pointer,look as interface
struct __ILUI
{
	int32_t (* UIPackageProcess)(ILUI * pILui, byte_t buffer[], uint32_t nBufferLen, IUI ** pNewIUI);
	int32_t (* ChatFaceProcess)(ILUI * pILui, byte_t buffer[], uint32_t nBufferLen);
	int32_t (* ChatIconAdd)(ILUI * pILui);

	void    (* CachedIdClear)(ILUI * pILui);
	bool_t  (* GameExitUICreate)(ILUI * pILui, IUI **pNewIUI);
	bool_t  (* ChatInviteUICreate)(ILUI * pILui, byte_t *pBuf, uint32_t nLength, IUI **pNewIUI);
	bool_t  (* LoginUICreate)(ILUI * pILui, byte_t *msg, int32_t msgLen);

	int32_t (* ChatRoomIdGet)(ILUI * pILui);

	int32_t (* ChatBuddyInfoAdd)(ILUI * pILui, int32_t iPlayerID, const byte_t * szPlayerName, int32_t iNameSize, int16_t iPlayerType, int16_t iServerId);
	int32_t (* ChatBuddyInfoDel)(ILUI * pILui, int32_t iIndex);
//bruce0912010
#ifdef __KK_UI_V2__
	int32_t (* ChatBuddyInfoGet)(ILUI * pILui,
		int32_t iIndex, int32_t * piPlayerID, byte_t * pszPlayerName, int32_t * piNameSize, int16_t * piPlayerType, int16_t * piServerId);

	int16_t (*GetChatBuddyName)(ILUI * pILui,int32_t PlayerID,  byte_t * pszPlayerName, int32_t * piNameSize);
#endif

	int32_t (* ChatDataSave)(ILUI * pILui);
	void    (* ChatDataClear)(ILUI * pILui);

	void    (* SubItemInfoGet)(ILUI * pILui,	int32_t idx,	int32_t *ID,
							int32_t *whichChoiceGroup,	int32_t *index,byte_t  *property,int32_t *nPropertyLen);

	bool_t  (* HiddenItemAdd)(ILUI * pILui,
							byte_t * szHiddenValue, int32_t nHiddenValueSize);
	void    (* HiddenItemClear)(ILUI * pILui);
	int32_t (* HiddenItemGet)(ILUI * pILui, int32_t ID, byte_t * pBuf, int32_t bufLen);
	int32_t (* HiddenItemNumGet)(ILUI * pILui);

	int32_t (* UIShow)(ILUI * pILui, bool_t bNeedSow);

	int32_t (* ChoiceGroupNumGet)(ILUI * pILui);
	int32_t (* InputNumGet)(ILUI * pILui);

	void		(* SoftKeyRestore)(ILUI * pILui);

	int32_t (* WinIDGet)(ILUI * pILui);
	int32_t (* WinTypeGet)(ILUI * pILui);

	void    (* WaitingInfoShow)(ILUI * pILui, int32_t msgId, int32_t param);
// czm add for video chat 1220 begin
	bool_t (* VideoChat_Start)(ILUI * pILui);
	bool_t (* VideoChat_Stop)(ILUI * pILui);
// czm add for video chat 1220 end

	int32_t(* ListSelectedPropertyGet)(ILUI * pILui, int32_t selectedIndex, byte_t ** propertyValue);

//### xiawei
	int32_t (* CPackageProcess)(ILUI * pILui, byte_t buffer[], uint32_t nBufferLen, IUI ** pNewIUI);

//###

};

// WEPAppData is the main struct for this application. WEPAppData  hold
// all the data which need to be used throughout the life of application.
struct __WEPApp_Data
{
	void * pDeviceData;	// special device data

	ILUI				* pILUI;	// logic UI
	IPDP				* pIPdp;	// protocol handle
	ILNT				* pILnt;	// logic network

	INET				*	pINet;
	IBuilder		*	pIBuilder;
	IUI					*	pIUI;
	ILD					* pILd;
	
	byte_t		*responseBuffer;	// ! the size should be divided exactly by 4
	int32_t		numOfBytesRead;

	byte_t		pdp[MAX_REQUEST_BUFFER_LEN];	//  ! the size should be divided exactly by 4
	int32_t   pdpLen;						// length of last RP

	int32_t   numOfBytesSent;
#ifdef WEP_NET_SEND_OPT
	int32_t   numOfBygesRemain;
	byte_t      *sendBuffer;   
#endif
//	int32_t   numOfScreen;		// num of prestore UI
//	byte_t  *	pScreenBuf[MAX_PRESTORE_UIP_NUM];		// pointer to prestore UI buffer

#ifdef SUPPORT_BACK_SCREEN
	int32_t   numOfBackupScreen; // 缓存页面个数
	int16_t   winIdOfBackUpScreen[MAX_BACKUP_SCREEN_NUM]; // 缓存页面的ID
	int32_t   sizeOfBackUpscreen[MAX_BACKUP_SCREEN_NUM];
	bool_t    bCacheForBackOnly[MAX_BACKUP_SCREEN_NUM];
	byte_t  *	pBackupScreenBuf[MAX_BACKUP_SCREEN_NUM]; // 缓存页面数据
#endif

	byte_t		backScreenBuf[MAX_BACK_UIP_LEN];
	byte_t		mruScreenBuf[MAX_BACK_UIP_LEN];

	byte_t    MsgQueueBuf[MAX_MESSAGE_QUEUE_NUM][MAX_UIP_LEN];
	int32_t   numOfMsgInQueue;

 
	byte_t		chatMessageBuf[MAX_UIP_LEN];   
//	byte_t		chatMessageBuf[MAX_CP_LENGTH];        

	int32_t		lastScreenToGo;		// last screen to go (back to)

	RECT_t    rcChildWnd;				// UI/GAME window size(relative to client rect of main window)

	int32_t		NormalFontWidth;
	int32_t		NormalFontHeight;
	int32_t		NormalFontAscent;
	int32_t		NormalFontDescent;

	int16_t		UI_lineHeight;		// UI line height
	int16_t		softkeyHeight;		// softkey height

	int32_t   languageId;				// current language ID

	byte_t    colorDepth;				// color depth of device
	byte_t		supportedPictureType;
	byte_t		supportedSoundType;
	byte_t    dummy;

	int32_t		serverId;					// recently connect(ed) server
// 	int32_t		exitStatus;				// last time exist status

	uint32_t	dwVitalMsgID;

	int32_t   dwConnectSeconds;	// connect count-down counter (seconds)

	int32_t		WEP_major_version;
	int32_t		WEP_minor_version;

	int32_t   WEP_build_number;

	int32_t   chatWhisperOpponentId;	// opponent id of whispering player in CHAT ROOM

	int32_t		currentServerType;		// the server type currently connected,
							// 0: main server; >0: app server; -1: not login; -2: disconnected

	bool_t dataPackageArrival;
	bool_t serverReDirecting;

	bool_t appSoundOn;
	bool_t appAutoSave;
	bool_t appAutoLogon;
#ifdef SUPPORT_PROXY
	bool_t appUseProxy;
 	bool_t nullPackage;//will send a null package
	bool_t httpConnectting;//a http connectting
	bool_t httpConnectSuccess;
#endif	
	
	bool_t inMsgBox;
	bool_t bShowWaitMsg;

	bool_t bRequesting;				// TRUE indicate requesting data from server

	bool_t bInChat;						// TRUE indicate in CHAT
	bool_t bInChatScreen;			// TRUE indicate CHAT win

#ifdef VIDEO_ROOM
	bool_t bInVideoRoom;				// TRUE indicate in video ROOM 
	bool_t bIsAdmin;					// TRUE indicate administrator
	int32_t videoRoomId;
#endif
	
// czm add for video chat 20071128 begin
	bool_t bInVideoChat;  // TRUE indicate in VEDIO CHAT 
//	int32_t cameraPicInterval;   // the time between pics, ms

	int32_t videoSpeed;
	int32_t videoRoomSpeed;

	byte_t  videoChatPicBuff[MAX_VIDEO_CHAT_PIC_SIZE];
	int32_t videoChatPicSize;

	uint8_t videoChatPicId;
	uint8_t videoChatPicFrams;
	uint8_t videoChatPicIndex;	
// czm add for video chat 20071128 end

	bool_t bInXUI;						// TRUE indicate in XUI application

	bool_t bMsgButtonAdded;

	bool_t smsIsSent;
	int8_t smsSendStatus;
//### xiawei
	byte_t		spid1[64];			// sp number for chinamobile
	byte_t		spid2[64];			// sp number for chinaunicom
	byte_t		id[140];			// content of sp
	byte_t      keyword[16];			// keyword for sms
	int8_t		smsSendCount;			//  counts of sending
//
#ifdef SUPPORT_FTP
	bool_t FTP_InProcess;			// file transfer in process
	byte_t FTP_UFRP_Done;			// ftp UFRP done flag
	int32_t FTP_reqCode;				// ftp request code
	int32_t FTP_subReqCode;		// ftp sub-request code

	bool_t FTP_SelectingFile;
	
	bool_t FTP_UserActioned;			// user actioned when file transfering
	byte_t FTP_UserCmdType;				// user command type
	int32_t FTP_UserCmdReqCode;		// user command request code
	int32_t FTP_UserCmdSubReqCode;	// user command sub-request code

#endif//SUPPORT_FTP

	// dynamic property
	int32_t nDynamicPropertyId[MAX_DYNAMIC_PROP_NUM];
	byte_t  szDynamicProperty[MAX_DYNAMIC_PROP_NUM][MAX_DYNAMIC_PROP_LEN];
	byte_t	nDynamicPropertyLen[MAX_DYNAMIC_PROP_NUM];

	// ticker data
	int32_t tickerId;
	int32_t tickerType;
	COLOR_t tickerColor;
	int32_t tickerTitleLen;
	byte_t  szTickerTitle[MAX_TICKER_TITLE_LEN];

	PFNUICMDACTION pXUCPCmdAction;

	char_t defaultServerUrl[MAX_DEFAULT_SERVER_URL_LEN];
	int32_t defaultServerPort;

	bool_t userConnMethodSelected;//user don't select connect method ,so need to try the connect method//james 070523

	bool_t bSOC_WOULDBLOCK;  //  SOC_WOULDBLOCK

#ifdef SUPPORT_DUAL_SIM
	byte_t simCardIndex;
	int32_t simStatus;
#endif
	bool_t bOpenFullEditor;
#ifdef SUPPORT_RECORD_VOICE
	byte_t    recordBuf[MAX_RECORD_BUFFER_LEN];
	byte_t    playSoundBuf[MAX_RECORD_BUFFER_LEN];
	uint16_t  recordBufLen;
	uint16_t  playSoundBufLen;
	uint16_t  recordfilefullName[128];
	uint16_t  sendingLen;
	uint16_t  sendingoffsetLen;
	bool_t    bRecordSound;
	bool_t    bPlaySound;
	byte_t    secondCounts;
	byte_t    recordStatus;      
#endif
	bool_t    bNeedShowChatText;
	bool_t    bDeActiveWin;            //窗口是否是激活状态，来确定是否刷新
#ifdef SUPPORT_BLOCK_SP_SMS	
	SP_LIST   splist;
//add for scn 0715 begin
	byte_t    scn[100]; 
	byte_t    scn_length; 
////add for scn 0715 end
#endif

//#ifdef KK_SUPPORT_OVERSEASP
	byte_t   imsi[64];
	byte_t   imsi_length;
//#endif
//bruce0912010
#ifdef __KK_UI_V2__
	byte_t itemHilightOfCurrList;
#endif

};

#endif


