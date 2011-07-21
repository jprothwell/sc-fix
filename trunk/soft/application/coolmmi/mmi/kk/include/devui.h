#ifndef _DEVUI_H
#define _DEVUI_H
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// !!! public declare start !!!


// !!! public declare end !!!
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#ifdef _DEV_UI_
// !!! private declare start !!!
#define USE_NEW_SIM_CONTROL

#define UI_MAX_LINE_NUM   (6+2)/*pangpeng 090206 for Chat Edit add 2 for prolong the history box*/
//#define UI_MAX_LINE_NUM   6

// EDIT input mode define
#define IME_TXT			0x01	// 文本模式
#define IME_NUM			0x02	// 数字模式
#define IME_PIN			0x80	// 密码模式

typedef struct _UI_SoftKey
{
	int32_t softkeyId;
	int32_t softkeyPos;
	UI_character_type softkeyTitle[MAX_SOFTKEY_TITLE_LEN / sizeof(UI_character_type)];
} UISoftKey;
typedef enum _UI_CtlType
{
	CTL_EDIT = 1,
	CTL_EDIT_ML,
	CTL_STRING,
	CTL_CHOICE,
	CTL_LIST,
	CTL_LABEL,
	CTL_SPACE,
	CTL_CHECKBOX,
	CTL_PICTURE,
	CTL_SOUND,
	CTL_SOFTKEY,
} UICtlType;

typedef struct _inline_text_edit_history
{	word	history_ID;
	s16		text_length;
	s16		allocated_length;
	s16		available_length;
	s16		current_position;
	s16		text_offset_x;
	s16		input_type;
	s16		dummy;	/*	Added for history issue on hardware	*/
	dword	flags;
#if defined(__MMI_TOUCH_SCREEN__)
	s16 vk_lang_type;
#endif

#if (KK_MTKXXXX_SWVER == KK_MTKXXXX_V08A) || (KK_MTKXXXX_SWVER == KK_MTKXXXX_V08B)
    mmi_imc_history ime_history;
#endif
} inline_text_edit_history;
#define MAX_BUFFER_SIZE 5000   

typedef struct _UI_Data
{
	IUI  Vtbl;	// MUST be first element

	WEPAppData * pWep;

 	PFNUICMDACTION pCmdAction;

	int32_t winID;
	int32_t dev_winType;

	RECT_t cUIRc;	// Client UI Rect

	UI_character_type buffer[MAX_BUFFER_SIZE/ sizeof(UI_character_type)];  // 所有UI控件文本信息buffer
	int32_t nCurBufLen; // 当前文本长度

	UI_character_type screenTitle[MAX_SCREEN_TITLE_LEN / sizeof(UI_character_type)]; // screen title,NOTICE: screen title can be changed

	int32_t nLabelNum;
	int32_t ctrlLabelIndex[UI_MAX_LABEL_NUM];	// indice of labels
	UI_character_type *pLabelText[UI_MAX_LABEL_NUM];//label 文本指针，注意：最好自带文本结束符
	int32_t labelTextLen[UI_MAX_LABEL_NUM]; //label 文本长度

	int32_t nEditNum;	// number of edit fields
	int32_t ctrlEditIndex[UI_MAX_TEXT_NUM];	// indice of edit fields
	int32_t editType[UI_MAX_TEXT_NUM];		// edit type
	int32_t nEditMaxLen[UI_MAX_TEXT_NUM];		 
	UI_character_type *pEditText[UI_MAX_TEXT_NUM];// 编辑框已有文本指针，注意：最好自带文本结束符
	int32_t editTextLen[UI_MAX_TEXT_NUM]; //编辑框文本长度

	int32_t nChoiceGroupNum;	// number of choice groups (or lists)
	int32_t ctrlChoiceIndex[UI_MAX_CHOICEGROUP_NUM];	// indice of choice groups
	int32_t nSubItemNumOfChoiceGroup[UI_MAX_CHOICEGROUP_NUM];	// number of subitems of choice groups
	int32_t nChoiceGroupSelectedIndex[UI_MAX_CHOICEGROUP_NUM]; // currently selected index of choice groups

	int32_t nSubItemNum;	// number of sub items (for choice groups(checkbox) or list)
	UI_character_type * pSubItemText[UI_MAX_SUBITEM_NUM]; // 子项文本指针，注意：最好自带文本结束符
	int32_t subItemTextLen[UI_MAX_SUBITEM_NUM]; //子项文本长度
	int32_t subItemImgIndex[UI_MAX_SUBITEM_NUM];//子项对应的图标索引
	UI_image_type subItemImage[UI_MAX_SUBITEM_NUM];
	

	int32_t nStringNum;					// number of string ctrl 
	int32_t ctrlStringIndex[UI_MAX_STATIC_NUM];	// indice of string control
	int32_t nStringLineNum[UI_MAX_STATIC_NUM];	// line nubmer of string
	UI_character_type * pStringText[UI_MAX_STATIC_NUM]; // 只读字串文本指针，注意：最好自带文本结束符
	int32_t stringTextLen[UI_MAX_STATIC_NUM]; //只读字串文本长度

	int32_t indexIncheckBox[UI_MAX_CHOICEGROUP_NUM];//the num of item in one check box
	int32_t nCheckboxNum;	// number of checkbox ctrl
	int32_t nCheckboxItem;	// number of check items
	int32_t ctrlCheckItemIndex[UI_MAX_CHECK_ITEM_NUM]; //indice of checkbox item in all ctrls
	int32_t checkBoxValue[UI_MAX_CHECK_ITEM_NUM];
	UI_character_type * pCheckText[UI_MAX_CHECK_ITEM_NUM]; // Check box文本指针，注意：最好自带文本结束符
	int32_t checkTextLen[UI_MAX_CHECK_ITEM_NUM]; // Check box文本长度，但在buffer中占用长度为MAX_SUBITEM_CAPTION_LEN，因为其内容可以改变

	int32_t numOfPictureItem;	
	int32_t imgInxOfPictureItem[UI_MAX_PICTUIRE_NUM];
	int32_t ctrlPictureIndex[UI_MAX_PICTUIRE_NUM];	// indice of picture control

	int32_t numOfSoftKey;	
	UISoftKey uiSK[UI_MAX_SOFTKEY_NUM];

#ifdef SUPPORT_FTP
	int32_t preSoftkeyID;
#endif	

	UI_character_type szFullEditLSKCaption[MAX_SOFTKEY_TITLE_LEN / sizeof(UI_character_type)];

	// save all controls' handle
	int32_t nControls;
	int32_t iActiveCtl;
	bool_t bCanActive[UI_MAX_CTL_NUM];
	UICtlType CtrlType[UI_MAX_CTL_NUM];

	int32_t chatStrCtrlInx;	
	int32_t chatInputCtrlInx;	

	UI_character_type *pWaitingMsgText;//提示文本指针，注意：最好自带文本结束符
	int32_t waitingMsgLen; //提示文本长度

	int32_t welcomeImageIndex;
	bool_t  welcomeShowHelpInfo;

	int32_t nHScrollWidth;	

	bool_t showConnectingMsg;		// TRUE mean show connecting message and FALSE mean waiting message
	
	int16_t imgWidth;//imagelist Icon width
	int16_t imgHeight;//imagelist Icon height

	int32_t formCtlTotalHeight;
	int32_t scrollOffset;

	bool_t bOpenFullEditor;
	int32_t totalLineNum;
	bool_t MultiLineRdOnlyCategory;
	int32_t numOfMsg;
#ifdef SUPPORT_ANIMATION
	int16_t aniFrameWidth;
	int16_t aniFrameHeight;
	int16_t aniFrameTotalNum;
	byte_t	aniFrameSeq[128];
#endif
	int32_t numOfPicture;
	int32_t picUsedBufLen;
	
	bool_t  bKeepSoftkey;
} UI;

typedef struct _UI_ChatPlayerData
{
	int32_t iPlayerID;	
	byte_t  playerName[MAX_CHAT_PLAYER_NAME_LEN+2];	// nick name of chat player
	int16_t nPlayerType;	// type of chat: WEP/MSN/QQ
	int16_t nServerId;		// id of the server which chat player connected
} UIChatPlayerData;

typedef struct _UI_PictureData
{
	int32_t		lenOfPicData;
	byte_t *	pszPicData;
	byte_t		pictureType;
} UIPictureData;

#define MAX_STRING_DATA_SIZE	1000	 

typedef struct _UI_Builder
{
	IBuilder Vtbl;

	WEPAppData * pWep;

	UI ui;
	UI * pUI;

	int32_t sx,sy,dx,dy;
	
	UI_character_type strBuf[MAX_STRING_DATA_SIZE * 2 / sizeof(UI_character_type)];	// STRING content buffer

	UI_character_type  chatFaceSymbol[MAX_CHAT_FACE_NUMBER][MAX_CHAT_FACE_SYM_SIZE];
	int32_t chatFaceImageId[MAX_CHAT_FACE_NUMBER];
	int32_t chatSymbolNum;

	int32_t numOfPicture;	
	UIPictureData stPicture[UI_MAX_PICTUIRE_NUM];

} Builder;

#ifdef		SUPPORT_POPMENU

#define		MAX_POPMENU_ITEM				10		//最大容量10项，超过6项显示上下指示图标
#define		MAX_POPMENU_ITEM_STRING			30 //14		//最多显示6个汉字
#define		MAX_POPMENU_DISPLAY_ITEM		7      	//popmenu可显示区域最多显示6项
#define		POPMENU_ARROW_AREA_HEIGHT		16
//bruce091210    
#ifdef __KK_UI_V2__
#define		POPMENU_BG_COLOR					gdi_act_color_from_rgb(255,15,51, 79)
#define 		POPMENU_BG_OUTLINE_COLOR          	gui_color(0, 12, 34)
#define 		POPMENU_BG_BORDER_COLOR          	gui_color(33, 89, 125)
//#define		POPMENU_BG_BORDER_COLOR			gdi_act_color_from_rgb(255,0,12, 34)

#define		POPMENU_ITEM_START_COLOR		gdi_act_color_from_rgb(255, 24,37, 48)
#define		POPMENU_ITEM_END_COLOR			gdi_act_color_from_rgb(255,80, 133, 187)

#define		POPMENU_ITEM_TEXT_COLOR			gui_color(255,255,255)
#define		POPMENU_ITEM_TEXT_ACTIVE_COLOR	gui_color(255,255,255)

#else
#define		POPMENU_BG_COLOR				gdi_act_color_from_rgb(255,176,191, 200)
#define		POPMENU_BG_BORDER_COLOR			gdi_act_color_from_rgb(255,177,192, 201)
#define		POPMENU_ITEM_COLOR				gdi_act_color_from_rgb(255, 25, 122, 169)
#define		POPMENU_ITEM_TEXT_COLOR			gui_color(0,0,0)
#define		POPMENU_ITEM_TEXT_ACTIVE_COLOR	gui_color(255,255,255)
#endif /* __KK_UI_V2__ */
#define		POPMENU_OPACITY_VALUE			255
#define		POPMENU_ITEM_TAP_X				2
#define		POPMENU_ITEM_TAP_Y				2
#define     POPMENU_ITEM_TEXT_TAP_X			4
#define		POPMENU_ITEM_TEXT_TAP_Y			3
//#define		POPMENU_ITEM_HIEGHT				22
//#define		POPMENU_WIDTH		120//96
//#define		POPMENU_X			(LCD_WIDTH - POPMENU_WIDTH - 2)


uint8_t           popmenu_item_height = 0;  //BN10030
uint16_t  		popmenu_width = 0;
uint16_t 		popmenu_x = 0;

#define		POPMENU_UP_ARROW				-1
#define		POPMENU_DOWN_ARROW				-2

//bruce100111 for src bg color    
#ifdef __KK_UI_V2__
#define KK_SRC_BG_TITLE_START_COLOR 		gdi_act_color_from_rgb(255, 24,37, 48);
#define KK_SRC_BG_TITLE_END_COLOR			gdi_act_color_from_rgb(255,80, 133, 187); 

#define KK_SRC_BG_CONTENT_START_COLOR	gdi_act_color_from_rgb(255,24, 37, 48);
#define KK_SRC_BG_CONTENT_END_COLOR		gdi_act_color_from_rgb(255,61, 95, 124);

#define KK_SRC_BG_SOFTKEY_START_COLOR	gdi_act_color_from_rgb(255, 24,37, 48);
#define KK_SRC_BG_SOFTKEY_END_COLOR		gdi_act_color_from_rgb(255,80, 133, 187);
#endif /* __KK_UI_V2__ */

typedef struct _tag_pmenu
{

	U16			x, y;
	U16			width, height;
	uint8_t     n_items;
	uint8_t     displayed_items;
   	uint8_t     first_item;
    uint8_t 	last_item;
	uint8_t     focusItem;		      
	uint8_t		imgId[MAX_POPMENU_ITEM];
	uint8_t		textLen[MAX_POPMENU_ITEM];
	UI_character_type text[MAX_POPMENU_ITEM][MAX_POPMENU_ITEM_STRING];
	int32_t		nPropertyLen[MAX_POPMENU_ITEM];
	byte_t		property[MAX_POPMENU_ITEM][MAX_SUBITEM_PROPERTY_LEN];
	byte_t		type[MAX_POPMENU_ITEM];
	bool_t      bDisplayArrow;
	gdi_handle  menu_layer;

#ifdef __MMI_TOUCH_SCREEN__
	int8_t       touch_pressed_index;     // -1: up arrow;  -2:down arrow:
#endif
	
} PMENU;

#endif



#endif	



#endif
