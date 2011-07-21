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
 *  InfoNum.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: InfoNum.c

  	PURPOSE		: Information Number

 

 

	DATE		: Aug 13,03

**************************************************************/

#include "cswtype.h"

#define __NEWSIMULATOR
#include "stdc.h"
#include "l4dr1.h"
#include "unicodexdcl.h"
#include "wgui_categories.h"
#include "infonum.h"
#include "commonscreens.h"
#include "globaldefs.h"
#include "historygprot.h"
#include "eventsgprot.h"
#include "callmanagementgprot.h"
#include "gsm7bitdefaultalphabet.h"
#include "custdatares.h"
#include "simdetectiongexdcl.h"
#undef __NEWSIMULATOR

static INFONUM GlobalInfoNum[MAX_INFO_NUM_ITEMS + 1]; /*1 is added for 0th Node*/
static INFONUM *info;
static INFONUM *displayInfoNum;
static INFONUM *hiliteInfoNum;
U8 isInfoNumPresent=0;
static U8 currentLevel=0;
static U8 InfoNumHistory[MAX_INFO_NUM_LEVELS];
static U8 hiliteInfoNumIndex;
static U8 saveInfoNum;
static U8 saveInfoNumPremium;
static S8 infoItemCount=(S8)-1;
extern void goto_demo_under_construction(void);
static U8 *infoNumberToBeDialed;

/****************************************************************************
	
	FUNCTION NAME		: InfoMalloc

  	PURPOSE				: Mallocs Information for Info Num from
							Global Array
 
	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: Pointer to returned memory

 

******************************************************************************/

INFONUM *InfoMalloc()
{
	infoItemCount++;
	if(infoItemCount==MAX_INFO_NUM_ITEMS+1) return NULL;
	return (&GlobalInfoNum[(U8)(infoItemCount)]);
}

/****************************************************************************
	
	FUNCTION NAME		: StoreCurrInfoNum

  	PURPOSE				: Stores Current Menu at Information Number node passed 
							as per data from PS
 
	INPUT PARAMETERS	:	info: Information Number Node wher dats is to be saved
 

	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void StoreCurrInfoNum(INFONUM *pInfo, l4csmu_info_num_struct *infoNum)
{
	U8 temp[41*ENCODING_LENGTH];
	U8 len;
	pInfo->index=infoNum->index_level;
	if(infoNum->alpha_id_size>21) 
		len=21;
	else 
		len=infoNum->alpha_id_size;
	memcpy(temp,infoNum->alpha_id,len);
	temp[len]=0;
	#ifdef __UCS2_ENCODING
		AnsiiToUnicodeString((PS8)pInfo->alphaId,(PS8)temp);
	#else
		pfnUnicodeStrcpy(pInfo->alphaId,temp);
	#endif
	if(infoNum->ton_npi!=0xFF)
	{
		if(infoNum->digit_size>3) 
			len=3;
		else 
			len=infoNum->digit_size;
		memcpy(temp, infoNum->digit, len);
		temp[len]=0;
		#ifdef __UCS2_ENCODING
			AnsiiToUnicodeString((PS8)pInfo->number,(PS8)temp);
		#else
			pfnUnicodeStrcpy(pInfo->number,temp);
		#endif
	}
	pInfo->nsi=infoNum->network_specific;
	pInfo->psi=infoNum->premium_rate;
}


/****************************************************************************
	
	FUNCTION NAME		: InitialiseInformationArray

  	PURPOSE				: Initialised Information number array
 
	INPUT PARAMETERS	:	noInfoNum: No of nodes
 


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
 
void InitialiseInformationArray(U8 noInfoNum, l4csmu_info_num_struct *infoNum)
{
	U8 i;
	INFONUM *currInfo;
	INFONUM *addInfo;
	INFONUM *currParent;
	info=InfoMalloc();
	
	info->parent=NULL;
	info->child=NULL;
	info->next=NULL;
	info->previous=NULL;
	info->index=0;
	info->noOfChildren=0;
	pfnUnicodeStrcpy((PS8)info->alphaId,GetString(SERVICES_INFO_NUM_STRING_ID));
	currParent=info;
	currInfo=info;


	/*StoreCurrInfoNum(info, &infoNum[0]);*/

	for (i=0;i<noInfoNum; i++)
	{
		addInfo=InfoMalloc();
		if(addInfo==NULL) break;
		if(currInfo->index==infoNum[i].index_level)
		{
			/* Add Sibling of Parent*/
			currInfo->next=addInfo;
			currParent->noOfChildren++;	
			addInfo->parent=currParent;
			addInfo->previous=currInfo;
			addInfo->next=NULL;
			addInfo->child=NULL;
			addInfo->noOfChildren=0;

		}
		else if(currInfo->index<infoNum[i].index_level)
		{
			/* Add first Child of Parent */
			currInfo->child=addInfo;
			currParent=currInfo;
			currParent->noOfChildren=1;
			addInfo->parent=currParent;
			addInfo->previous=NULL;
			addInfo->next=NULL;
			addInfo->child=NULL;
			addInfo->noOfChildren=0;
		}
		else
		{
			/* Go up one level */
			while(currInfo->index!=infoNum[i].index_level)
				currInfo=currInfo->parent;
			currParent=currInfo->parent;
			
			currInfo->next=addInfo;
			currParent->noOfChildren++;	
			addInfo->parent=currParent;
			addInfo->previous=currInfo;
			addInfo->next=NULL;
			addInfo->child=NULL;
			addInfo->noOfChildren=0;

		}
		StoreCurrInfoNum(addInfo, &infoNum[i]);
		currInfo=addInfo;
	}
	isInfoNumPresent=1;
}

/****************************************************************************
	
	FUNCTION NAME		: EnterScrInfoNumber

  	PURPOSE				: Entry Screen for Information Number App
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void EnterScrInfoNumber(void)
{
	U8 i;
	currentLevel=0;
	displayInfoNum=info;
	for(i=0;i<MAX_INFO_NUM_LEVELS;i++) InfoNumHistory[currentLevel]=0;
	DisplayInformationNumber();
}

/****************************************************************************
	
	FUNCTION NAME		: EnterScrInfoNumber

  	PURPOSE				: Entry Screen for each Information Number Screen Level
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void DisplayInformationNumber(void)
{
	U8 *Strings[MAX_SUB_MENUS];
	U8 *Hints[MAX_SUB_MENUS];
	INFONUM *currInfo=displayInfoNum;
	U16 nNumofItem;

	EntryNewScreen(SERVICES_INFO_NUM_SCREEN_ID, ExitInfoNumScreen, NULL, NULL);

	saveInfoNum=1;
	nNumofItem=0;
	currInfo=currInfo->child;
	Strings[nNumofItem]=currInfo->alphaId;
	Hints[nNumofItem]=hintData[nNumofItem];
	if(currInfo->child)
		Hints[nNumofItem]=NULL;
	else
		Hints[nNumofItem]=currInfo->number;
	nNumofItem++;
//micha0202, remove compile warning
	currInfo=currInfo->next;
	while(currInfo) /*Ignore the warning, intentional */
	{
		Strings[nNumofItem]=currInfo->alphaId;
		if(currInfo->child)
			Hints[nNumofItem]=NULL;
		else
			Hints[nNumofItem]=currInfo->number;
		nNumofItem++;
		currInfo=currInfo->next;
	}
	RegisterHighlightHandler(InfoNumberScrHiliteHandler);
	displayInfoNum=displayInfoNum->child;
	ShowCategory53Screen(SERVICES_INFO_NUM_STRING_ID,SERVICES_INFO_NUM_IMAGE_ID,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
						nNumofItem,Strings,(PU16)gIndexIconsImageList,Hints,LIST_MENU,InfoNumHistory[currentLevel],0);
	SetLeftSoftkeyFunction(GoAheadInfoNum,KEY_EVENT_UP);
	SetKeyHandler(GoAheadInfoNum,KEY_SEND,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackInfoNum,KEY_EVENT_UP);
	SetKeyHandler(GoBackInfoNum,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
}

/****************************************************************************
	
	FUNCTION NAME		: InfoNumberScrHiliteHandler

  	PURPOSE				: Hilite Handler for each Information Number Screen Level
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void InfoNumberScrHiliteHandler(S32 index)
{
	U8 i;
	hiliteInfoNumIndex=(U8)index;
	hiliteInfoNum=displayInfoNum;
	for(i=0;i<index;i++)
		hiliteInfoNum=hiliteInfoNum->next;
	if(hiliteInfoNum->noOfChildren)
		ChangeLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
	else
		ChangeLeftSoftkey(SERVICES_INFO_DIAL_STRING_ID,0);

}

/****************************************************************************
	
	FUNCTION NAME		: GoAheadInfoNum

  	PURPOSE				: LSK Handler for each Information Number Screen Level
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void GoAheadInfoNum(void)
{
#if 0 //dyj del 20060515
	if(hiliteInfoNum->noOfChildren)
	{
		if (hiliteInfoNum->nsi && g_pwr_context.RoamingFlag)
			DisplayRoamingInfoNum();
		else
		{
		saveInfoNum=0;
		displayInfoNum=hiliteInfoNum;
		InfoNumHistory[currentLevel]=hiliteInfoNumIndex;
		currentLevel++;
		DisplayInformationNumber();
	}
	}
	else
	{
		infoNumberToBeDialed=hiliteInfoNum->number;
		if (hiliteInfoNum->nsi && g_pwr_context.RoamingFlag)
			DisplayRoamingInfoNum();
		else if (hiliteInfoNum->psi)
			DisplayPremimumInfoNum();
		else 
			MakeCall((PS8)infoNumberToBeDialed);
	}
#endif
}

/****************************************************************************
	
	FUNCTION NAME		: GoBackInfoNum

  	PURPOSE				: RSK Handler for each Information Number Screen Level
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void GoBackInfoNum(void)
{
	if(currentLevel)
	{
		saveInfoNum=0;
		InfoNumHistory[currentLevel]=0;
		currentLevel--;
		displayInfoNum=hiliteInfoNum->parent->parent;
		DisplayInformationNumber();
	}
	else
	{
		displayInfoNum=info;
		GoBackHistory();
	}
}

/****************************************************************************
	
	FUNCTION NAME		: ExitInfoNumScreen

  	PURPOSE				: Exit Handler for each Information Number Screen Level
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void ExitInfoNumScreen(void)
{
	if(saveInfoNum)
	{
		history_t   currHistory;
		S16		nHistory =	0;
		displayInfoNum=hiliteInfoNum->parent;
		InfoNumHistory[currentLevel]=hiliteInfoNumIndex;
		currHistory.scrnID = SERVICES_INFO_NUM_SCREEN_ID;
		currHistory.entryFuncPtr = DisplayInformationNumber;
		pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&nHistory);
		GetCategoryHistory(currHistory.guiBuffer);
		AddHistory(currHistory);
		
	}
}

/****************************************************************************
	
	FUNCTION NAME		: DisplayPremimumInfoNum

  	PURPOSE				: Displays Premium Info Number Screen
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void DisplayPremimumInfoNum(void)
{
	EntryNewScreen(SERVICES_INFO_PREMIUM_SCREEN_ID, ExitDisplayPremimumInfoNum, NULL, NULL);

	saveInfoNumPremium=1;
	ShowCategory2Screen(SERVICES_INFO_NUM_STRING_ID,SERVICES_INFO_NUM_IMAGE_ID,
						SERVICES_INFO_DIAL_STRING_ID,IMG_GLOBAL_OK,
						STR_GLOBAL_BACK,IMG_GLOBAL_BACK,SERVICES_INFO_PREMIUM_STRING_ID,NULL);
	SetLeftSoftkeyFunction(DisplayPremimumInfoNumOK,KEY_EVENT_UP);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}

/****************************************************************************
	
	FUNCTION NAME		: DisplayPremimumInfoNum

  	PURPOSE				: Exit Handler for Premium Info Number Screen
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void ExitDisplayPremimumInfoNum(void)
{
	if(saveInfoNumPremium)
	{
		history_t   currHistory;
		S16		nHistory =	0;
		currHistory.scrnID = SERVICES_INFO_PREMIUM_SCREEN_ID;
		currHistory.entryFuncPtr = DisplayPremimumInfoNum;
		pfnUnicodeStrcpy ((S8*)currHistory.inputBuffer, (S8*)&nHistory);
		GetCategoryHistory(currHistory.guiBuffer);
		AddHistory(currHistory);
		
	}
}
/****************************************************************************
	
	FUNCTION NAME		: DisplayPremimumInfoNumOK

  	PURPOSE				: LSK Handler for Premium Info Number Screen
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/
void DisplayPremimumInfoNumOK(void)
{
	saveInfoNumPremium=0;
	MakeCall((PS8)infoNumberToBeDialed);
}

/****************************************************************************
	
	FUNCTION NAME		: DisplayRoamingInfoNum

  	PURPOSE				: Entry function for Roaming Info Number Screen
 
	INPUT PARAMETERS	: nil	


	OUTPUT PARAMETERS	: nil
	
	RETURNS				: nil

 

******************************************************************************/

void DisplayRoamingInfoNum(void)
{
	EntryNewScreen(SERVICES_INFO_ROAMING_SCREEN_ID, NULL, DisplayRoamingInfoNum, NULL);

	saveInfoNumPremium=1;
	ShowCategory2Screen(SERVICES_INFO_NUM_STRING_ID,SERVICES_INFO_NUM_IMAGE_ID,
						0,0,
						STR_GLOBAL_BACK,IMG_GLOBAL_BACK,SERVICES_INFO_ROAMING_STRING_ID,NULL);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

