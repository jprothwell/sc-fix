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
/**************************************************************

	FILENAME	: SimAppToolkit.cpp

  	PURPOSE		: Plugin for SAT

	REMARKS		: nil

	AUTHOR		:  Neeraj

	DATE		: Oct 5,03

**************************************************************/


#include "stdafx.h"
#include "SimAppToolkit.h"
#include "MemAllocator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define __ASCII
#define ENCODING_LENGTH 1

//////////////////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////////////////
SATMENUITEM mainMenu[]=
{
	{12,SAT_8BIT_DCS,0,"SELECT ITEM"},
	{12,SAT_8BIT_DCS,1,"DISPLAY TEXT"},
	{12,SAT_8BIT_DCS,2,"GET INPUT"},
	{12,SAT_8BIT_DCS,3,"GET INKEY"}
};

SATMENUITEM subMenu[]=
{
	{12,SAT_8BIT_DCS,4,"GAMES"},
	{12,SAT_8BIT_DCS,5,"HOROSCOPE"},
	{12,SAT_8BIT_DCS,7,"NEWS"},
	{12,SAT_8BIT_DCS,8,"STOCKS"},
	{12,SAT_8BIT_DCS,9,"RAILWAYS"},
	{12,SAT_8BIT_DCS,10,"AIRWAYS"},
	{12,SAT_8BIT_DCS,11,"CRICKET"},
	{12,SAT_8BIT_DCS,12,"YOUR CITY"},
	{12,SAT_8BIT_DCS,13,"CONTACT US"},
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimAppToolkit::CSimAppToolkit()
{

}

CSimAppToolkit::~CSimAppToolkit()
{

}


//////////////////////////////////////////////////////////////////////
// Member functions
//////////////////////////////////////////////////////////////////////

void *CSimAppToolkit::SetupMenuPlugin(void)
{
	return(CreateMainMenu((PU8)"PIXTEL SAT",mainMenu,4));
}
/**************************************************************

	FUNCTION NAME		: CreateMainMenu

  	PURPOSE				: Creates the SAT main menu

	INPUT PARAMETERS	: U8 * alphaId, SATMENUITEM *menuPtr,U8 noOfMenuItems

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: Plugin for SAT main menu. This will return a buffer for SAT main menu
							description.
**************************************************************/

void *CSimAppToolkit::CreateMainMenu(U8 * alphaId, SATMENUITEM *menuPtr,U8 noOfMenuItems)
{
	SATSETUPMENUIND *SetupMenu;
	SATMENUITEM MenuItem[MAX_ITEMS_IN_MENU];
	U8 *tempDataPtr;
	U16 i;
	U8 *lenLSB;
	U8 *lenMSB;
	U16 tempLen;
	U8 tempMenuItems[1024];
	SetupMenu = (SATSETUPMENUIND *)CMemAllocator::MyMallocDataPtr (sizeof (SATSETUPMENUIND));
	SetupMenu->isHelpInfoAvailable =TRUE;
	SetupMenu->isSoftKeyPreferred =TRUE;
	SetupMenu->isIconAvailable =FALSE;
	SetupMenu->noAlphaId=strlen((PS8)alphaId);
	SetupMenu->dcsOfAlphaId=SAT_8BIT_DCS;
	SetupMenu->alphaId=(U8 *)malloc(SetupMenu->noAlphaId);
	memcpy(SetupMenu->alphaId,alphaId,SetupMenu->noAlphaId);
	for(i=0;i<noOfMenuItems;i++)
	{
		MenuItem[i].itemId=menuPtr[i].itemId;
		MenuItem[i].itemDcs=menuPtr[i].itemDcs;
		MenuItem[i].itemLen=strlen((PS8)menuPtr[i].itemData);
		tempDataPtr=SATCovertStringForSIM(menuPtr[i].itemData, MenuItem[i].itemLen, MenuItem[i].itemDcs, &tempLen);
		MenuItem[i].itemLen=tempLen;
		memcpy(MenuItem[i].itemData,tempDataPtr,MenuItem[i].itemLen);
		free(tempDataPtr);
	}
	SetupMenu->numOfItem=noOfMenuItems;
	tempDataPtr=tempMenuItems;
	SetupMenu->noItemData=0;
	for(i=0;i<SetupMenu->numOfItem;i++)
	{
		lenLSB=(U8 *)&MenuItem[i].itemLen;
		lenMSB = lenLSB+1;
		*tempDataPtr++ = *lenMSB;
		*tempDataPtr++ = *lenLSB;
		*tempDataPtr++ = MenuItem[i].itemId;
		*tempDataPtr++ = MenuItem[i].itemDcs;
		SetupMenu->noItemData+=4;
		memcpy(tempDataPtr,MenuItem[i].itemData,MenuItem[i].itemLen);
		tempDataPtr += MenuItem[i].itemLen;
		SetupMenu->noItemData += MenuItem[i].itemLen;
	}
	SetupMenu->itemData=(U8 *)malloc(SetupMenu->noItemData);
	memcpy(SetupMenu->itemData,tempMenuItems,SetupMenu->noItemData);
	
	SetupMenu->noItemIconIdList=0;
	SetupMenu->noNextActionIndList=0;

	return (void *)SetupMenu;
}

/**************************************************************

	FUNCTION NAME		: SelectItemPlugin

  	PURPOSE				: Plugin for Select indication in SAT

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: Returns buffer for SAT sub menu
**************************************************************/

void *CSimAppToolkit::SelectItemPlugin(void)
{
	return(CreateSubMenu((PU8)"PIXTEL SAT",subMenu,9));
}

/**************************************************************

	FUNCTION NAME		: CreateSubMenu

  	PURPOSE				: Creates a sat sub menu with the alpha id

	INPUT PARAMETERS	: U8 * alphaId, SATMENUITEM *menuPtr,U8 noOfMenuItems

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: Returns buffer for SAT sub menu with the alpha id 
**************************************************************/


void *CSimAppToolkit::CreateSubMenu(U8 * alphaId, SATMENUITEM *menuPtr,U8 noOfMenuItems)
{
	
	SATSELECTITEMIND *SetupMenu;
	SATMENUITEM MenuItem[MAX_ITEMS_IN_MENU];
	U8 *tempDataPtr;
	U16 i;
	U8 *lenLSB;
	U8 *lenMSB;
	U16 tempLen;
	U8 tempMenuItems[1024];
	SetupMenu = (SATSELECTITEMIND *)CMemAllocator::MyMallocDataPtr (sizeof (SATSELECTITEMIND));
	SetupMenu->isHelpInfoAvailable =TRUE;
	SetupMenu->isSoftKeyPreferred =TRUE;
	SetupMenu->isIconAvailable =FALSE;
	SetupMenu->isItemIdAvailable=FALSE;
	SetupMenu->noAlphaId=strlen((PS8)alphaId);
	SetupMenu->dcsOfAlphaId=SAT_8BIT_DCS;
	SetupMenu->alphaId=(U8 *)malloc(SetupMenu->noAlphaId);
	memcpy(SetupMenu->alphaId,alphaId,SetupMenu->noAlphaId);
	for(i=0;i<noOfMenuItems;i++)
	{
		MenuItem[i].itemId=menuPtr[i].itemId;
		MenuItem[i].itemDcs=menuPtr[i].itemDcs;
		MenuItem[i].itemLen=strlen((PS8)menuPtr[i].itemData);
		tempDataPtr=SATCovertStringForSIM(menuPtr[i].itemData, MenuItem[i].itemLen, MenuItem[i].itemDcs, &tempLen);
		MenuItem[i].itemLen=tempLen;
		memcpy(MenuItem[i].itemData,tempDataPtr,MenuItem[i].itemLen);
		free(tempDataPtr);
	}

	SetupMenu->numOfItem=noOfMenuItems;
	tempDataPtr=tempMenuItems;
	SetupMenu->noItemData=0;
	for(i=0;i<SetupMenu->numOfItem;i++)
	{
		lenLSB=(U8 *)&MenuItem[i].itemLen;
		lenMSB = lenLSB+1;
		*tempDataPtr++ = *lenMSB;
		*tempDataPtr++ = *lenLSB;
		*tempDataPtr++ = MenuItem[i].itemId;
		*tempDataPtr++ = MenuItem[i].itemDcs;
		SetupMenu->noItemData+=4;
		memcpy(tempDataPtr,MenuItem[i].itemData,MenuItem[i].itemLen);
		tempDataPtr += MenuItem[i].itemLen;
		SetupMenu->noItemData += MenuItem[i].itemLen;
	}
	
	SetupMenu->itemData=(U8 *)malloc(SetupMenu->noItemData);
	memcpy(SetupMenu->itemData,tempMenuItems,SetupMenu->noItemData);
	
	SetupMenu->noItemIconIdList=0;
	SetupMenu->noNextActionIndList=0;
	
	return (void *)SetupMenu;
}
/**************************************************************

	FUNCTION NAME		: DisplayTextPlugin

  	PURPOSE				: Plugin for Display Text indication

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: Returns buffer for SAT display text
**************************************************************/

void *CSimAppToolkit::DisplayTextPlugin(void)
{
	return(CreateDisplayText("This is a sample text to test the functionality of Display Text",SAT_8BIT_DCS));
}
/**************************************************************

	FUNCTION NAME		: CreateSubMenu

  	PURPOSE				: Creates a buffer for sat display text 
	INPUT PARAMETERS	: U8 * alphaId, U8 dcs

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: nil
**************************************************************/

void *CSimAppToolkit::CreateDisplayText(S8 * text, U8 dcs)
{
	SATDISPLAYTEXTIND *displayText;
	displayText = (SATDISPLAYTEXTIND *)CMemAllocator::MyMallocDataPtr (sizeof (SATDISPLAYTEXTIND));
	displayText->clearTextType=FALSE;
	displayText->immediateRes=FALSE;
	displayText->isIconAvailable=FALSE;
	displayText->priority=0;
	displayText->dcsOfTextString = dcs;
	displayText->noTextString = strlen(text);
	displayText->textString=SATCovertStringForSIM((U8 *)text,displayText->noTextString, displayText->dcsOfTextString, &displayText->noTextString);
	return (void *)displayText;
}
/**************************************************************

	FUNCTION NAME		: GetInputPlugin

  	PURPOSE				: Plugin for GetInput indication from MMI

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: Returns buffer for SAT getInput indication
**************************************************************/

void *CSimAppToolkit::GetInputPlugin(void)
{
	return(CreateGetInput("Enter a String?",8,20,TYPE_SMS_DEFAULT_SET,0,"New Delhi", SAT_8BIT_DCS));
}
/**************************************************************

	FUNCTION NAME		: CreateGetInput

  	PURPOSE				: Plugin for GetInput indication from MMI

	INPUT PARAMETERS	: S8 * text, U8 min, U8 max, U8 type, U8 reveal, S8 *input, U8 dcs

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: Returns the buffer for getinput message.
**************************************************************/

void *CSimAppToolkit::CreateGetInput(S8 * text, U8 min, U8 max, U8 type, U8 reveal, S8 *input, U8 dcs)
{
	SATGETINPUTIND *getInput;
	getInput = (SATGETINPUTIND *)CMemAllocator::MyMallocDataPtr (sizeof (SATGETINPUTIND));
	getInput->isHelpInfoAvailable = FALSE;
	getInput->lenOfMinInput=min;
	getInput->lenOfMaxInput=max;
	getInput->typeOfInput=type;
	getInput->isIconAvailable=FALSE;
	getInput->isInputRevealedToUser = reveal;
	getInput->dcsOfTextString = dcs;
	getInput->noTextString = strlen(text);
	getInput->textString=SATCovertStringForSIM((U8 *)text, getInput->noTextString, getInput->dcsOfTextString, &getInput->noTextString);
	getInput->dcsOfDefaultText = dcs;
	getInput->noDefaultText = strlen(input);
	getInput->defaultText=SATCovertStringForSIM((U8 *)input, getInput->noDefaultText, getInput->dcsOfDefaultText, &getInput->noDefaultText);
	return (void *)getInput;
}
/**************************************************************

	FUNCTION NAME		: GetInputPlugin

  	PURPOSE				: Plugin for GetInkey indication from MMI

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: Returns buffer for SAT getInkey indication
**************************************************************/

void *CSimAppToolkit::GetInkeyPlugin(void)
{
	return(CreateGetInkey("What is your initial?", TYPE_SMS_DEFAULT_SET, SAT_UCS2_DCS));
}

/**************************************************************

	FUNCTION NAME		: CreateGetInkey

  	PURPOSE				: Plugin for CreateGetInkey indication from MMI

	INPUT PARAMETERS	: S8 * text, U8 type, U8 dcs

	OUTPUT PARAMETERS	: nil

	RETURNS				: void*

	REMARKS				: Returns the buffer for getinkey message.
**************************************************************/

void *CSimAppToolkit::CreateGetInkey(S8 * text, U8 type, U8 dcs)
{
	SATGETINKEYIND *getInkey;
	getInkey = (SATGETINKEYIND *)CMemAllocator::MyMallocDataPtr (sizeof (SATGETINKEYIND));
	getInkey->isHelpInfoAvailable = FALSE;
	getInkey->typeOfInput=type;
	getInkey->dcsOfTextString = dcs;
	getInkey->isIconAvailable=FALSE;
	getInkey->noTextString = strlen(text);
	getInkey->textString=SATCovertStringForSIM((U8 *)text,getInkey->noTextString, getInkey->dcsOfTextString, &getInkey->noTextString);
	return (void *)getInkey;
}
/**************************************************************

	FUNCTION NAME		: SATCovertStringForSIM

  	PURPOSE				: Internal conversion strings that care of UCS2 and Little endian/ Big endian issue

	INPUT PARAMETERS	: U8 *data, U16 len, U8 format, U16 *outLen

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8*

	REMARKS				: Returns the string after conversion
**************************************************************/


U8 * CSimAppToolkit::SATCovertStringForSIM(U8 *data, U16 len, U8 format, U16 *outLen)
{
	U8 *convertedData;

	if (len > 0)
    {
		switch (format)
		{
		#if 0
/* under construction !*/
			#ifdef __ASCII
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
			#endif
/* under construction !*/
			#ifdef __UCS2_ENCODING
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
			#endif
/* under construction !*/
/* under construction !*/
		#endif	

			case SAT_8BIT_DCS:
			#ifdef __ASCII
				convertedData = (U8 *)malloc((len)+1);
				memcpy(convertedData,data,len);
				memset((void *)&convertedData[len*ENCODING_LENGTH],0,1);
				if(outLen!=NULL) *outLen=len+1;
			#endif
			#ifdef __UCS2_ENCODING
				convertedData = malloc((len/ENCODING_LENGTH)+1);
				UnicodeNToAnsii((PS8)convertedData,(PS8)data,len);
				memset((void *)&convertedData[len/ENCODING_LENGTH],0,1);
				if(outLen!=NULL) *outLen=len/ENCODING_LENGTH+1;
			#endif
			break;

			case SAT_UCS2_DCS:
			#ifdef __ASCII
					convertedData = (U8 *)malloc((len*2)+2);
					AnsiiNToUnicodeString((PS8)convertedData,(PS8)data,len);
					memset((void *)&convertedData[len*2],0,2);
					if(outLen!=NULL) *outLen=len*2+2;
			#endif
			#ifdef __UCS2_ENCODING
					convertedData = malloc((len)+ENCODING_LENGTH);
					memcpy(convertedData,data,len);
					memset((void *)&convertedData[len],0,ENCODING_LENGTH);
					if(outLen!=NULL) *outLen=len+ENCODING_LENGTH;
			#endif
			#ifdef MMI_ON_HARDWARE_P
			{
				U16 *tempData;
				int i;
				tempData=(U16 *)convertedData;
				for(i=0;i<len*2/ENCODING_LENGTH;i+=2)
				{
					SWAP_USHORT(tempData);
					tempData++;
				}
			}
			#endif
			break;
		}

		return convertedData;
	}
	else
	{
		U8 noOfNulls;
		if(format==SAT_UCS2_DCS) noOfNulls=2;
		else noOfNulls=1;
		convertedData = (U8 *)malloc(noOfNulls);
		memset((void *)convertedData,0,noOfNulls);
		if(outLen!=NULL) *outLen=noOfNulls;
	 	return convertedData;
	}
}

/**************************************************************

	FUNCTION NAME		: AnsiiNToUnicodeString

  	PURPOSE				: Function to convert ANSII strings to unicode

	INPUT PARAMETERS	: S8 *pOutBuffer, S8 *pInBuffer, U32 len  

	OUTPUT PARAMETERS	: nil

	RETURNS				: U16

	REMARKS				: Returns the string after conversion
**************************************************************/

U16 CSimAppToolkit::AnsiiNToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer, U32 len  )
{
	
		S16 count = -1;
	U8 charLen = 0;
	U8  arrOut[2];

	while(len)
	{
		
		UnicodeToUCS2Encoding((U16)*pInBuffer,&charLen,arrOut);

		pOutBuffer[++count] = arrOut[0];
		pOutBuffer[++count] = arrOut[1];
		pInBuffer++;
		len--;

	}

	
	return count + 1;
}

/**************************************************************

	FUNCTION NAME		: UnicodeToUCS2Encoding

  	PURPOSE				: Converts a UNICODE string to UCS2

	INPUT PARAMETERS	: U16 unicode,U8 *charLength,U8 *arrOut

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8

	REMARKS				: Conversion of unicode to UCS
**************************************************************/

U8 CSimAppToolkit::UnicodeToUCS2Encoding(U16 unicode,U8 *charLength,U8 *arrOut)
{
	U8 status = 1;
	U8 index = 0;

	if(*arrOut != 0)
	{
		
		if( unicode < 256 )
		{
			arrOut[index++] = *((U8*)(&unicode));
			arrOut[index] = 0;
			
		}
		else
		{
			arrOut[index++] = *((U8*)(&unicode));
			arrOut[index] =   *(((U8*)(&unicode)) + 1);

		}
		*charLength = 2;
	}	
	else
	{
		
		status = 0;
	}

	return status;

}
