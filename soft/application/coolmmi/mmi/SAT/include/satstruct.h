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

/*******************************************************************************
 * Filename:
 * ---------
 *	SATStruct.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   SIM application toolkit related functions
 *
 * Author:
 * -------
 * -------
 *
*==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/

/**************************************************************

	FILENAME	: SATStruct.h

  	PURPOSE		: SAT application 

 

 

	DATE		: Mar 24,03

**************************************************************/
#ifndef _COOLSAND_SATSTRUCT_H
#define _COOLSAND_SATSTRUCT_H
  #include "satdefs.h"		//wangbei add 20060724

#define MAX_SAT_ITEMDATA_LEN 128
typedef struct sat_icon_struct{
    U8 iconAttr;
    U8 iconId;
} SATICON;


typedef struct
{
	U16 itemLen;
	U8 itemDcs;
	U8 itemId;
	U8 itemData[MAX_SAT_ITEMDATA_LEN];
	U8 nextAction;
	U8 IconID;
} SATMENUITEM;

typedef struct
{
	U8 cmdDetail[5];
	U8 isHelpInfoAvailable;
	
	U8 dcsOfAlphaId;
	U8 numOfItem;

	U8 isSentRsp;
	U16 alphaIdLen;
	U8  alphaId[MAX_SAT_ITEMDATA_LEN];
	
	U8 isIconAvailable;
	U8 IconID;
	U8 iconAttr;

	U8 noItemIconIdList;
	U8 itemIconListAttr;
	U8 noNextActionIndList;
	
 
	U8 selectedMenuItem;

	SATMENUITEM *menuItems;
	
} SATSETUPMENU; 

typedef struct
{
	U8 cmdDetail[5];
	U8 isHelpInfoAvailable;
	U8 defaultIndex;
	U8 dcsOfAlphaId;
	U16 alphaIdLen;
	U8 numOfItem;
	U8 isSentRsp;
	U8 *alphaId;
	U8 isIconAvailable;
	U8 IconID;
	U8 iconAttr;
	U8 noItemIconIdList;
	U8 itemIconListAttr;
	U8 noNextActionIndList;
	SATMENUITEM *menuItems;
#ifdef __MMI_MULTI_SIM__
	U8 CMDfromSIM;
#endif	
} SATSUBMENU;

typedef struct
{
	U8	cmdDetail[5];
	U16 textStringLen;
	U8 *textString;
	U8	clearTextType;
	U8	immediateRes;
	U8 dcsOfTextString;
	U8 sentRsp;
	U8	isIconAvailable;
	SATICON	iconInfo;
#ifdef __MMI_MULTI_SIM__
    U8 CMDfromSIM;
#endif	
}SATDISPLAYTEXT;


typedef struct
{
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U16	typeOfInput;
	U8	lenOfMinInput;
	
	U8	lenOfMaxInput;
	U8	dcsOfTextString;
	U16	lenOfTextString;
	
	U8*	textString;
	U8*	defaultText;
	U8* inputString;
	U16	lenOfDefaultText;
	U16 lenOfInputString;
	U8	dcsOfDefaultText;
	U8	dcsOfInputString;
	U8  sentRsp;
	U8	isIconAvailable;
	SATICON	iconInfo;
} SATGETINPUT;

typedef struct
{
	U8	cmdDetail[5];
	U8	isHelpInfoAvailable;
	U8	typeOfInput;
	U8	dcsOfTextString;
	U8	*textString;
	U8  inputString[4];
	U16	lenOfTextString;
	U8	dcsOfInputString;
	U8  sentRsp;
	U8	isIconAvailable;
	SATICON	iconInfo;
} SATGETINKEY;


typedef struct
{
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
	U8	isIcon1Available;
	SATICON	iconInfo1;
	U8	isIcon2Available;
	SATICON	iconInfo2;
} SATCALLSETUP;

typedef struct
{
	U8	cmdDetail[5];
	U8  informUser;
	U16	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	stage1Res;
	U8  passedConf;
	U8	isSmsPackedRequired;
	U8	noAddr;
	U8	addr[41];
	U8	tpduLength;
	U8	tpdu[175];
	U8	isIconAvailable;
	SATICON	iconInfo;
} SATSENDSMS;

typedef struct
{
	U8	cmdDetail[5];
	U8  informUser;
	U16	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	stage1Res;
	U8	isIconAvailable;
 
	U8	sentRsp;
	U8	ssStringLen;
	U8	*ssString;	/* original 7-bit sms packed SS string */
	U8	*textString;	/* display SS string */
	SATICON	iconInfo;
} SATSENDSS;

typedef struct
{
    U8 cmdDetail[5];
    U8 informUser;
    U16 noAlphaId;
    U8 *alphaId;
    U8 dcsOfAlphaId;
    U8 stage1Res;
    /* Display USSD reponse */
    U8 sentRsp;
    U8 ussdStringLen;
    U8 *ussdString; /* original USSD string */
    U8 *textString; /* display USSD string */
#ifdef __USAT__
    U8 frameId;
    U16 noTextAttribute;
    U8 *textFormatting;
#endif /* __USAT__ */ 
    U8 isIconAvailable;
    SATICON iconInfo;
} SATSENDUSSD;


typedef struct
{
	U8	cmdDetail[5];
	U8	informUser;
	U16	noAlphaId;
	U8*	alphaId;
	U8	dcsOfAlphaId;
	U8	toneId;
	U8  sentRsp;
	U8	isIconAvailable;
	SATICON	iconInfo;
} SATPLAYTONE;

typedef struct
{
	U16 instance;
	U16 offset;
	U16 length;
	U16 reqlength;
	U8 width;
	U8 height;
	U8 coding;
	U8 cycle;
	U8 curcycle;
}SATIMAGEINSTANCE;

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
	U8 noOfCLUT;
	U16 locOfCLUT;
	SATCOLORDATA *CLUTData;
}SATIMAGERECORD;


typedef struct 
{
    U8 cmdDetail[5];
    U8 browserMode;
    U8 browserIdentity;
    U8 provisionFileId[10];
    U16 noAlphaId;
    U8 *alphaId;
    U8 isAlphaIdPresent;
    U8 dcsOfAlphaId;
    U16 noUrl;
    U8 *url;
    U16 noBearer;
    U8 *bearer;
    U16 noGateway;
    U8 *gateway;
    U8 dcsOfGateway;
    U8 passedConf;
#ifdef __USAT__
    U8 frameId;
    U16 noTextAttribute;
    U8 *textFormatting;
#endif /* __USAT__ */ 
    U8 isIconAvailable;
    SATICON iconInfo;
} SATLAUNCHBROWSER;


typedef struct {
	LOCAL_PARA_HDR
	U8 browserMode;
	U8 browserIdentity;
	U16 noUrl;
	U8 *url;
	U16 noBearer;
	U8 *bearer;
	U16 noGateway;
	U8 *gateway;
} WAPLAUNCHBROWSERREQ;


typedef struct {
	LOCAL_PARA_HDR
	U8 res;
	U8 additionalInfo;	
} WAPLAUNCHBROWSERRES;

typedef struct
{
	U8	cmdDetail[5];
	U8	isIconAvailable;
	SATICON	iconInfo;
	U16	noAlphaId;
	U8*	alphaId;
	U8	isAlphaIdPresent;
	U8	dcsOfAlphaId;
	U8	noDtmf;
	U8*	dtmf;
	U8	stage1Res;
	U8	passedConf;
} SATSENDDTMF;

typedef struct
{
	U8	cmdDetail[5];
	U8	isIconAvailable;
	SATICON iconInfo;
	U16	noAlphaId;
	U8*	alphaID;
	U8	isAlphaIdPresent;
	U8	dcsOfAlphaId;
	U8	informUser;
}SATRUNATCOMMAND;

typedef struct
{
    U8*	iconData;
    U8	iconID;
    U8	gotFromSIM;
    U8	iconAvailable;
} SATICONIMAGE;

 
#define MAX_FILE_CHANGE_NOTIFY_CALLBACK 3

typedef struct
{
    U8 isSATPresent[MMI_SIM_NUMBER];
    U8 SATPrevioustatus[MMI_SIM_NUMBER];
    U8 isInSATScreen;
    U8 isInSATMenu;
    U8 isInSATCall;
    U8 gotStage1Rsp;
    U8 lastSATCmd;
    U8 highlightedIndex;
    U8 waitTimeExpired;
    U8 currGetIconIndex;
    U8 isCMDIconSupported;
    U8 getImageStage;
#ifdef __MMI_SAT_DISPLAY_TEXT_WITH_KEYPAD_LOCK__
    U8 idleSATCmd;
#endif
    U16 waitString;
    U16 waitImage;
    U8 *exitCondition;
    U8 *idleTextPtr;
    U8 *idleIconPtr;
    U8 *guiBufferPointer;
    void *scr_asm_ptr;
    FuncPtr postponedSATScreen;
    FuncPtr noResponseFuncPtr;
    FuncPtr helpFuncPtr;
    FuncPtr historyFuncPtr;
    FuncPtr showAfterCallFuncPtr;
    FuncPtrShort simFuncPtr[MAX_FILE_CHANGE_NOTIFY_CALLBACK];
    SATICONIMAGE iconList[MAX_ICON_SUPPORT];	
#ifdef __MMI_MULTI_SIM__
    U8 CMDfromSIM;
    U8 satIndex;
#endif	
} SATCONTEXT;


//micha1016
typedef struct
{
	U8	cmdDetail[5];
	U16 noTextString;
	U8 *textString;
	U8 dcsOfTextString;
	U8	isIconAvailable;
	SATICON	iconInfo;
}SATDISPLAYIDLETEXT;


#ifdef __SATCE__

typedef struct
{
	U8	cmdDetail[5];
	U8	isIconAvailable;
	SATICON iconInfo;
	U16	noAlphaId;
	U8*	alphaID;
	U8	isAlphaIdPresent;
	U8	dcsOfAlphaId;
	U8	informUser;
	U8	Stage1Res;	
	U8	Stage2Res;
} SATOPENCHANNEL;


typedef struct
{
	U8	cmdDetail[5];
	U8	isIconAvailable;
	SATICON iconInfo;
	U16	noAlphaId;
	U8*	alphaID;
	U8	isAlphaIdPresent;
	U8	dcsOfAlphaId;
	U8	informUser;
} SATCLOSECHANNEL;

typedef struct
{
	U8	cmdDetail[5];
	U8	isIconAvailable;
	SATICON iconInfo;
	U16	noAlphaId;
	U8*	alphaID;
	U8	isAlphaIdPresent;
	U8	dcsOfAlphaId;
	U8	informUser;
	U8	Stage1Res;	
	U8	Stage2Res;
} SATSENDDATA;

typedef struct
{
	U8	cmdDetail[5];
	U8	isIconAvailable;
	SATICON iconInfo;
	U16	noAlphaId;
	U8*	alphaID;
	U8	isAlphaIdPresent;
	U8	dcsOfAlphaId;
	U8	informUser;
	U8	Stage1Res;	
	U8	Stage2Res;
} SATRECVDATA;


#endif

#endif /* _COOLSAND_SATSTRUCT_H */


