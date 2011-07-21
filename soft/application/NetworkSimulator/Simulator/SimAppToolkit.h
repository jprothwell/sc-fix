/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
// SimAppToolkit.h: interface for the CSimAppToolkit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMAPPTOOLKIT_H__1ECF295C_2329_4A97_BC61_821065397BE8__INCLUDED_)
#define AFX_SIMAPPTOOLKIT_H__1ECF295C_2329_4A97_BC61_821065397BE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MMI_Data_Types.h"
#include "QueueGprot.h"


#define MAX_ITEMS_IN_MENU 16
#define MAX_DIGITS_USSD  182

typedef struct sat_icon_struct{
    U8 iconAttr;
    U8 iconId;
} SATICON;

typedef enum
{
   ICON_DSPL_BUT_REPLACE_TEXT=0,
   ICON_DSPL_WITH_TEXT=1
}SATICONQUALIFIER;

typedef struct {
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	isSoftKeyPreferred;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8	numOfItem;
	U8	noAlphaId;
	U8	*alphaId;
	U8	dcsOfAlphaId;
	U16	noItemData;
	U8	*itemData;
	U8	noItemIconIdList;
	U8	*itemIconIdList;
	U8	itemIconListAttr;
	U8	noNextActionIndList;
	U8	*nextActionIndList;
} SATSETUPMENUIND;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	isSoftKeyPreferred;
	U8	typeOfPresentation;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8	isItemIdAvailable;
	U8	itemId;
	U8	numOfItem;
	U8	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U16	noItemData;
	U8*	itemData;
	U8	itemIconListAttr;
	U8	noItemIconIdList;
	U8*	itemIconIdList;
	U8	noNextActionIndList;
	U8*	nextActionIndList;
} SATSELECTITEMIND;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
} SATSETUPMENUREQ;

typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATSETUPMENURES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	itemId;
	U8	isHelpInfoRequested;
} SATMENUSELECTREQ;

typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATMENUSELECTRES;

typedef struct {
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	isSoftKeyPreferred;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8	numOfItem;
	U8	noAlphaId;
	U8	alphaId[256];
	U8	dcsOfAlphaId;
	U16	noItemData;
	U8	itemData[1024];
	U8	noItemIconIdList;
	U8	itemIconIdList[256];
	U8	itemIconListAttr;
	U8	noNextActionIndList;
	U8	nextActionIndList[256];
} NETWORKSIMSATSETUPMENUIND;

typedef struct {
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	isSoftKeyPreferred;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8	isItemIdAvailable;
	U8	itemId;
	U8	numOfItem;
	U8	noAlphaId;
	U8	alphaId[256];
	U8	dcsOfAlphaId;
	U16	noItemData;
	U8	itemData[1024];
	U8	noItemIconIdList;
	U8	itemIconIdList[256];
	U8	itemIconListAttr;
	U8	noNextActionIndList;
	U8	nextActionIndList[256];
} NETWORKSIMSATSELECTITEMIND;

typedef struct
{
	U16 itemLen;
	U8 itemDcs;
	U8 itemId;
	U8 itemData[256];
} SATMENUITEM;

typedef struct
{
	U8 isHelpInfoAvailable;
	U8 cmdDetail[5];
	U16 alphaIdLen;
	U8 *alphaId;
	U8 dcsOfAlphaId;
	U8 numOfItem;
	SATMENUITEM *menuItems;
} SATSETUPMENU; 

typedef struct
{
	U8 isHelpInfoAvailable;
	U8 cmdDetail[5];
	U8 defaultIndex;
	U16 alphaIdLen;
	U8 *alphaId;
	U8 dcsOfAlphaId;
	U8 numOfItem;
	U8 isSentRsp;
	SATMENUITEM *menuItems;
} SATSUBMENU;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
	U8	itemId;
} SATSELECTITEMREQ;
typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATSELECTITEMRES; 

typedef enum
{
   SAT_DEFAULT_DCS        = 0x00, /* GSM 7-bit */
   SAT_8BIT_DCS           = 0x04, /* 8-bit */
   SAT_UCS2_DCS           = 0x08  /* UCS2 */   
} SATDCSENUM;

typedef enum
{
   SAT_CMD_PERFORMED_SUCCESSFULLY=	0,
   SAT_CMD_PERFORMED_WITH_PARTIAL_COMPREHENSION=	0X01,
   SAT_CMD_PERFORMED_WITH_MISSING_INFO=	0X02,
   SAT_REFRESH_PERFORMED_WITH_ADDITIONAL_EF_READ=	0X03,
   SAT_CMD_PERFORMED_BUT_ICON_COULD_NOT_DSPL=	0X04,
   SAT_CMD_PERFORMED_BUT_MODIFIED_CC_BY_SIM=	0X05,
   SAT_CMD_PERFORMED_LIMITED_SERVICE=	0X06,
   SAT_CMD_PERFORMED_WITH_MODIFIED=	0X07,
   SAT_PROACTIVE_CMD_TERMINATED_BY_USER=	0X10,
   SAT_BACKWARD_MOVE_IN_PROACTIVE_SESSION_BY_USER=	0X11,
   SAT_NO_RESPONSE_FROM_USER=	0X12,
   SAT_HELP_INFO_REQUEST_BY_USER=	0X13,
   SAT_USSD_SS_TRANSACTION_TERMINATION_BY_USER=	0X14,
   SAT_ME_CURRENTLY_UNABLE_TO_PROCESS_CMD=	0X20,
   SAT_NET_CURRENTLY_UNABLE_TO_PROCESS_CMD=	0X21,
   SAT_USER_DID_NOT_ACCEPT_THE_PROACTIVE_CMD=	0X22,
   SAT_USER_CLEAR_DOWN_CALL_BEFORE_CONNECTION_RELEASE=	0X23,
   SAT_ACTION_IN_CONTRACTION_WITH_CURRENT_TIMER_STATE=	0X24,
   SAT_INTERACTION_WITH_CALL_CTRL_BY_SIM_TEMPORARY_ERROR=	0X25,
   SAT_LAUNCH_BROWER_GENERIC_ERROR_CODE=	0X26,
   SAT_CMD_BEYOND_ME_CAP=	0X30,
   SAT_CMD_TYPE_NOT_UNDERSTAND_BY_ME=	0X31,
   SAT_CMD_DATA_NOT_UNDERSTAND_BY_ME=	0X32,
   SAT_CMD_NUMBER_NOT_KNOWN_BY_ME=	0X33,
   SAT_SS_RETURN_ERROR=	0X34,
   SAT_SMS_RP_ERROR=	0X35,
   SAT_ERROR_REQUIRED_VALUE_MISSING=	0X36,
   SAT_USSD_RETURN_ERROR=	0X37,
   SAT_MULTICARD_CMD_ERROR=	0X38,
   SAT_INTERACTION_WITH_CALL_CTRL_BY_SIM_PERMANENT_ERROR=	0X39,
   SAT_BEARER_INDEPENDENT_PROTOCOL_ERROR=	0X3A
}SATTERMINALRESPONSE;

typedef enum
{
   SAT_PRI_NORMAL=0,
   SAT_PRI_HIGH=1
}SATDSPLPRIORITY;

typedef enum
{
   SAT_CLEAR_AFTER_DELAY=0,
   SAT_CLEAR_BY_USR=1
}SATCLEARTEXT;


typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	priority;
	U8	clearTextType;
	U8	immediateRes;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U16	noTextString;
	U8*	textString;
	U8	dcsOfTextString;
} SATDISPLAYTEXTIND;

typedef struct
{
	U8	cmdDetail[5];
	U8	priority;
	U8	clearTextType;
	U8	immediateRes;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U16	noTextString;
	U8	textString[256];
	U8	dcsOfTextString;
} NETWORKSIMSATDISPLAYTEXTIND;

typedef struct
{
	U8	cmdDetail[5];
	U16 textStringLen;
	U8 *textString;
	U8	clearTextType;
	U8	immediateRes;
	U8 dcsOfTextString;
	U8 sentRsp;
}SATDISPLAYTEXT;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
	U8	additionalInfo;
} SATDISPLAYTEXTREQ;

typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATDISPLAYTEXTRES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	typeOfInput;
	U8	isInputRevealedToUser;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8	lenOfMinInput;
	U8	lenOfMaxInput;
	U16	noTextString;
	U8*	textString;
	U8	dcsOfTextString;
	U16	noDefaultText;
	U8*	defaultText;
	U8	dcsOfDefaultText;
} SATGETINPUTIND;

typedef struct
{
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	typeOfInput;
	U8	isInputRevealedToUser;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8	lenOfMinInput;
	U8	lenOfMaxInput;
	U16	noTextString;
	U8	textString[256];
	U8	dcsOfTextString;
	U16	noDefaultText;
	U8	defaultText[256];
	U8	dcsOfDefaultText;
} NETWORKSIMSATGETINPUTIND;

typedef struct
{
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	typeOfInput;
	U8	lenOfMinInput;
	U8	lenOfMaxInput;
	U16	lenOfTextString;
	U8*	textString;
	U8	dcsOfTextString;
	U16	lenOfDefaultText;
	U8*	defaultText;
	U8	dcsOfDefaultText;
	U16 lenOfInputString;
	U8  *inputString;
	U8	dcsOfInputString;
	U8 sentRsp;
} SATGETINPUT;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
	U8	dcsOfTextString;
	U8	noTextString;
	U8	textString[1];
} SATGETINPUTREQ;

typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATGETINPUTRES;

typedef enum
{
   TYPE_ONLY_DIGIT,
   TYPE_SMS_DEFAULT_SET,
   TYPE_UCS2,
   TYPE_YES_NO    
}SATINPUTYPE;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	typeOfInput;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U16	noTextString;
	U8*	textString;
	U8	dcsOfTextString;
} SATGETINKEYIND;

typedef struct
{
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	typeOfInput;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U16	noTextString;
	U8	textString[256];
	U8	dcsOfTextString;
} NETWORKSIMSATGETINKEYIND;

typedef struct
{
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	typeOfInput;
	U16	lenOfTextString;
	U8	*textString;
	U8	dcsOfTextString;
	U8  inputString[2];
	U8	dcsOfInputString;
	U8  sentRsp;
} SATGETINKEY;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
	U8	isYesSelected;
	U8	dcsOfTextString;
	U8	noTextString;
	U8	textString[1];
} SATGETINKEYREQ;

typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATGETINKEYRES;

typedef struct
{
   U8   flag;    /*l4c_result_enum*/
   U16   cause;
} L4CRESULT;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	condition;
	U32	duration;
	U8	isChangedFromSS;
	U8	isIcon1Available;
	SATICON	iconInfo1;
	U8	isIcon2Available;
	SATICON	iconInfo2;
	U8	sizeOfCcp;
	U8	ccp[13];
	U8	sizeOfSubaddr;
	U8	subAddr[21];
	U8  isAlphaId1Present;
	U8	noAlphaId1;
	U8*	alphaId1;
	U8	dcsOfAlphaId1;
	U8	noAddr;
	U8*	addr;
	U8  isAlphaId2Present;
	U8	noAlphaId2;
	U8*	alphaId2;
	U8	dcsOfAlphaId2;
} SATCALLSETUPIND;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	condition;
	U32	duration;
	U8	isChangedFromSS;
	U8	noAddr;
	U8	addr[41];
	U8	noSubaddr;
	U8	subAddr[21];
	U8	noCcp;
	U8	ccp[15];
	U8  alphaId1Present;
	U16	noAlphaId1;
	U8*	alphaId1;
	U8	dcsOfAlphaId1;
	U8  alphaId2Present;
	U16	noAlphaId2;
	U8*	alphaId2;
	U8	dcsOfAlphaId2;
	U8* number;
	U8  isFromHistory;
	U8  isSentRsp;
	U8  exitCause;
	U8  passedConfirmScreen;
} SATCALLSETUP;

typedef enum
{
   SETUP_CALL_IF_NO_ANOTHER_CALL,
   SETUP_CALL_WITH_REDIAL_IF_NO_ANOTHER_CALL,
   SETUP_CALL_PUT_ANOTHER_HOLD,    
   SETUP_CALL_WITH_REDIAL_PUT_ANOTHER_HOLD,   
   SETUP_CALL_DISCON_ANOTHER,    
   SETUP_CALL_WITH_REDIAL_DISCON_ANOTHER   
}SATSETUPCALLCONDITION;

typedef enum
{
   SAT_NO_SPECIFIC_CAUSE,
   SAT_SCREEN_BUSY,
   SAT_ME_CURRENTLY_BUSY_ON_CALL,
   SAT_ME_CURRENTLY_BUSY_ON_SS,
   SAT_NO_SERVICE,
   SAT_ACCESS_CTRL_CLASS_BAR,
   SAT_RR_NOT_BRANTED,
   SAT_NOT_IN_SPEECH_MODE,
   SAT_ME_CURRENTLY_BUSY_ON_USSD,
   SAT_ME_CURRENTLY_BUSY_ON_DTMF
} SATADDITIONALINFOENUM;


typedef struct
{
	LOCAL_PARA_HDR
	U8	satCallType;
	U8	noAddr;
	U8	addr[41];
	U8	noSubAddr;
	U8	subAddr[21];
	U8	noCcp;
	U8	ccp[15];
	U8	isChangedFromSS;
} SATCALLSETUPSTAGE1REQ;

typedef struct
{
	LOCAL_PARA_HDR
	L4CRESULT	result;
	U8	terminalRes;
	U8	additionalRes;
} SATCALLSETUPSTAGE1RES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
	U8	cause;
} SATCALLSETUPSTAGE2REQ;
typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATCALLSETUPSTAGE2RES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8	isSmsPackedRequired;
	U8	sizeOfAddr;
	U8	addr[41];
	U8  isAlphaIdPresent;
	U8	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	noSmsTpdu;
	U8*	smsTpdu;
} SATSENDSMSIND;

typedef struct
{
	U8	cmdDetail[5];
	U8  informUser;
	U16	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	stage1Res;
} SATSENDSMS;

typedef struct
{
	LOCAL_PARA_HDR
	U8	isSmsPackedRequired;
	U8	noAddr;
	U8	addr[41];
	U8	tpduLength;
	U8	tpdu[175];
} SATSENDSMSSTAGE1REQ;

typedef struct
{
	LOCAL_PARA_HDR
	U8	res;
	U16	cause;
} SATSENDSMSSTAGE1RES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
	U8	cause;
} SATSENDSMSSTAGE2REQ;


typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATSENDSMSSTAGE2RES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	isChangedFromSetupcall;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8  isAlphaIdPresent;
	U8	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	noSsString;
	U8*	ssString;
} SATSENDSSIND;

typedef struct
{
	U8	cmdDetail[5];
	U8  informUser;
	U16	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	stage1Res;
} SATSENDSS;

typedef struct
{
	LOCAL_PARA_HDR
	U8	input[50];
	U8	dcs;
	U8	length;
} SATSENDSSSTAGE1REQ;

typedef struct
{
	LOCAL_PARA_HDR
	U8	res;
	U8	length;
	U8	additionInfo[255];
} SATSENDSSSTAGE1RES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
	U8	sizeOfAdditionalInfo;
	U8	additionalInfo[255];
} SATSENDSSSTAGE2REQ;

typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATSENDSSSTAGE2RES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	isChangedFromSetupcall;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8  isAlphaIdPresent;
	U8	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	noUssdString;
	U8*	ussdString;
	U8	dcsOfUssdString;
} SATSENDUSSDIND;

typedef struct
{
	U8	cmdDetail[5];
	U8  informUser;
	U16	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	stage1Res;
} SATSENDUSSD;

typedef struct
{
	LOCAL_PARA_HDR
	U8	ussdString[MAX_DIGITS_USSD];
	U8	ussdDcs;
} SATSENDUSSDSTAGE1REQ;

typedef struct
{
	LOCAL_PARA_HDR
	U8	res;
	U8	length;
	U8	additionInfo[255];
} SATSENDUSSDSTAGE1RES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
	U8	cause;
} SATSENDUSSDSTAGE2REQ;

typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATSENDUSSDSTAGE2RES;

typedef struct
{
	LOCAL_PARA_HDR
	U8	mmiInfoType;
	U8  isAlphaIdPresent;
	U8	noAlphaId;
	U8*	alphaId;
	U8	dcs_of_alpha_id;
} SATMMIINFOIND;

typedef struct
{
	U8 syncExit;
} SATMMIINFO;
typedef enum
{
   SAT_OPERATION_CHANGED_BY_SIM,
   SAT_NUM_CHANGED_BY_SIM,
   SAT_ALPHA_ID_IND,
   SIM_MEM_PROBLEM,
   SIM_SECRET_CODE_REJ,
   SIM_TECH_PROBLEM
}SATMMIINFOTYPE;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	toneId;
	U32	duration;
	U8	isIconAvailable;
	SATICON	iconInfo;
	U8	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	isAlphaIdPresent;
} SATPLAYTONEIND;

typedef struct
{
	U8	cmdDetail[5];
	U8	informUser;
	U16	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	toneId;
	U8  sentRsp;
} SATPLAYTONE;

typedef struct
{
	LOCAL_PARA_HDR
	U8	cmdDetail[5];
	U8	res;
} SATPLAYTONEREQ;
typedef struct
{
	LOCAL_PARA_HDR
	MMI_BOOL	result;
} SATPLAYTONERES;

typedef enum
{
   SAT_TONE_DIAL=0x01,
   SAT_TONE_CALLED_SUBSCRIBER_BUSY=0x02,
   SAT_TONE_CONGESTION=0x03,
   SAT_TONE_RADIO_PATH_ACK=0x04,
   SAT_TONE_RADIO_NO_PATH=0x05,
   SAT_TONE_ERROR=0x06,
   SAT_TONE_CALL_WAITING=0x07,
   SAT_TONE_RING=0x08,
   SAT_TONE_GENERAL_BEEP=0x10,
   SAT_TONE_POS_ACK=0x11,
   SAT_TONE_NEG_ACK=0x12
}SATTONEID;

typedef enum
{
	SAT_CALL_DISCONNECT_BEFORE_CONNECT=0x00,
	SAT_CALL_DISCONNECT_AFTER_CONNECT,
	SAT_CALL_USER_BUSY,
	SAT_CALL_USER_NOANSWER
}SATCALLEXITCAUSE;


#ifdef MMI_ON_WIN32

/*typedef enum
{
   L4C_OK,
   L4C_ERROR
} L4CRESULTENUM;*/

/*typedef enum
{
   L4C_NO_CAUSE,
   L4C_GEN_CAUSE,
   L4C_WRONG_PASSWORD,
   RMMI_INVALID_CMD,
   TCM_INVALID_PARAMETER,
   TCM_NSAPI_NOT_IN_USE

} L4CCAUSEENUM;*/

#endif

typedef struct
{
	U8 width;
	U8 height;
	U8 coding;
	U16 instance;
	U16 offset;
	U16 length;
	U8 cycle;
	U8 curcycle;
	U16 reqlength;
}SATIMAGEINSTANCE;

typedef struct
{
	U16 offset;
	U16 length;
	U8 cycle;
	U8 curcycle;
	U16 reqlength;
}SATCOLORINSTANCE;

typedef struct
{
	U8 red;
	U8 green;
	U8 blue;
}SATCOLORDATA;

typedef struct 
{
	U8 iconId;
	U8 noOfImages;
	SATIMAGEINSTANCE *imageInst;
	U8 *imageData;
	SATCOLORINSTANCE *colorInst;
	SATCOLORDATA *colorData;
}SATIMAGERECORD;

typedef struct
{
	U8 fileIdx;
	U8 para;
	U16 length;
}SATREADSIMFILEREQ;

typedef struct
{
	L4CRESULT result;
	U16 length;
	U8 data[260];
}SATREADSIMFILERES;

typedef enum
{
   SATBASICCODING=0x11,
   SATCOLORCODING=0x21
}SATIMAGECODING;



class CSimAppToolkit  
{
public:
	CSimAppToolkit();
	virtual ~CSimAppToolkit();
	void *SetupMenuPlugin(void);
	void *SelectItemPlugin(void);
	void *DisplayTextPlugin(void);
	void *GetInputPlugin(void);
	void *GetInkeyPlugin(void);
	void *CreateMainMenu(U8 * alphaId, SATMENUITEM *menuPtr,U8 noOfMenuItems);
	void *CreateSubMenu(U8 * alphaId, SATMENUITEM *menuPtr,U8 noOfMenuItems);
	void *CreateDisplayText(S8 * text, U8 dcs);
	void *CreateGetInput(S8 *text, U8 min, U8 max, U8 type, U8 reveal, S8 *input, U8 dcs);
	void *CreateGetInkey(S8 *text, U8 type, U8 dcs);
	U16 AnsiiNToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer, U32 len);
	U8 UnicodeToUCS2Encoding(U16 unicode,U8 *charLength,U8 *arrOut);
	U8 *SATCovertStringForSIM(U8 *data, U16 len, U8 format, U16 *outLen);
};



#endif // !defined(AFX_SIMAPPTOOLKIT_H__1ECF295C_2329_4A97_BC61_821065397BE8__INCLUDED_)
