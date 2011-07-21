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
 *  WriteRes.c
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#include "globalmenuitems.h"
#include "buildcfg.h"
#include "mmi_data_types.h"
#include "custdatares.h"
#include "custdataprots.h"
#include "custmenures.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "oslmemory.h"
//#include "ucs2prot.h"
#include "unicodexdcl.h"
#include "custresdef.h"
#include "gui.h"


#define MAX_NAME_LENGTH					100
#define MENUITEM_ID_TYPE				9
#define PARENT_ITEM_ID_TYPE				8
#define NUMBER_OF_MENUITEMS_TYPE		7
#define MENU_ITEM_ORDER_TYPE			6
#define	HIDE_FLAG_TYPE					5
#define MOVEABLE_FLAG_TYPE				4
#define	DISPLAY_ATTRIBUTE_TYPE			3
#define	MENU_ITEM_STRING_ID_TYPE		2
#define	MENU_ITEM_IMAGE_ID_TYPE			1
#define IMAGE_DESCRIPTOR_SIZE			2
#define  BUFFER_SIZE					4096
#define  BUFFER_SIZE1					16

//CSD added by JL
#define   OUTPUT_DATA_FILENAME "..\\..\\Customer\\CustResource\\CustImgDataHW.h"
 
// dingjian 20071217 for 176*220
#ifdef LCDSIZE_240_320
#define   OUTPUT_DATA_FILENAME_EXT "..\\..\\Customer\\CustResource\\CustImgDataHWExt_jasper.h" 
#endif 

#ifdef LCDSIZE_176_220
#define   OUTPUT_DATA_FILENAME_EXT "..\\..\\Customer\\CustResource\\CustImgDataHWExt_jasper.h" 
#endif 

/*cong.li added for product QPAD[220*176] on 2009.08.11. */
#ifdef LCDSIZE_220_176
#define   OUTPUT_DATA_FILENAME_EXT "..\\..\\Customer\\CustResource\\CustImgDataHWExt_jasper.h"
#endif 

#ifdef LCDSIZE_128_160
#define   OUTPUT_DATA_FILENAME_EXT "..\\..\\Customer\\CustResource\\CustImgDataHWExt_jasper.h" 
 
#endif
#ifdef LCDSIZE_160_128
#define   OUTPUT_DATA_FILENAME_EXT "..\\..\\Customer\\CustResource\\CustImgDataHWExt_jasper.h" 
#endif
//#define   OUTPUT_DATA_FILENAME_EXT "..\\..\\Customer\\CustResource\\CustImgDataHWExt.h" 

 
#define 	OUTPUT_MENUTREE_FILENAME "..\\..\\Customer\\CustResource\\CustMenuTree_Out.c"
#define 	OUTPUT_IMAGE_NAMES_END "[]=\n{"
#define 	NOIMAGE_FILENAMEBUFFER "..\\\\..\\\\Customer\\\\IMAGES\\\\NOIMAGE.BMP"
#define 	DEFAULT_FILENAMEBUFFER "..\\..\\mmi_Customer\\Images\\Default.bmp"
#define		DEFAULT_FILEBUFFER			"________MMI_CUSTOMER__IMAGES__DEFAULT_BMP"
 
//#define 	OUTPUT_MENUTREE_ID_FILENAME "..\\..\\Customer\\CustResource\\CustMenuTreeID_Out.c"
//#define 	OUTPUT_IMAGE_ID_FILENAME CUST_IMG_PATH"\\\\images_file_list.ini"
 

#ifdef MMI_ON_WIN32
#include <stdafx.h>
#endif

 
BOOL ExtImageFlag=FALSE;  
 

#ifdef DEVELOPER_BUILD_FIRST_PASS

static U8 is_first=1;
static U32 image_idx=1;


void Extract_Name(S8 *dest, S8* src);
MMI_BOOL Iload(S8 *string,U16* pType);


extern CUSTOM_STRING* gpStringRes;
extern CUSTOM_STRING_MAP*	 gpStringMapRes;
 
extern FILE *StringListFile;
extern FILE *EnumListFile;
 
U8 StrIsPopulate[MAX_STRING_MAP_SIZE];
 
//CSD end

 
extern FILE *OutputImgIDFile;
extern FILE *OutputMenutreeIDFile;
extern FILE *RepeatIDListFile;  
 


//decalred in the CustImageRes.c
extern CUSTOM_IMAGE nCustImageNames[];
extern IMAGENAME_LIST	ImageNameList[];
extern CUSTOM_IMAGE_MAP ImageIdMap[];
extern CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMap[];

 
/* 040805 CustPack: Calvn added */
extern IMAGENAME_LIST	ImageNameListEXT[];
extern CUSTOM_IMAGE_MAP ImageIdMapEXT[];
extern CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMapEXT[];
 
 

//decalred in the CustScreenData.c
extern CUSTOM_SCREEN nCustScreens[MAX_SCREEN_IDS];

//declared in the CustMenuDataRes.c
#ifdef MMI_ON_HARDWARE_P
extern const CUSTOM_MENU  nCustMenus[MAX_MENU_ITEMS];
#else
extern CUSTOM_MENU  nCustMenus[MAX_MENU_ITEMS];
#endif




/* Global Variables */
S8	filename_buffer[BUFFER_SIZE];
S8	imagefilename_buffer[BUFFER_SIZE];
S8	autofilename_buffer[BUFFER_SIZE];
S8	number_string[BUFFER_SIZE1];
S8	autoimagename_buffer[BUFFER_SIZE];
FILE*	output_data_file;
FILE*	image_data_file;
S8*	output_image_names = NULL;
S32 n_supported_extensions=1;

 
#define SUPPORT_N_MEDIA_TYPE		3
S8	*support_media_ext[SUPPORT_N_MEDIA_TYPE] = {"AVI", "3GP", "MP4"};
 

 
 
FILE*	dest_file;
int		toolFlag = 0;//decide if this is run by CustPack tool
int		resultFailFlag = 0;//decide if this is run by CustPack tool
 
 

 
typedef struct Sequential_Image_Info
{
	S8	*filename_ext;
	U8	file_type;	
}SEQUENTIAL_IMAGE_INFO;

#define MAX_SEQUENTIAL_IMAGE_FRAME			512
#define SUPPORT_N_SEQUENTIAL_IMAGE_TYPE		3

SEQUENTIAL_IMAGE_INFO sequential_image[SUPPORT_N_SEQUENTIAL_IMAGE_TYPE] = {	{"BMP", IMAGE_TYPE_BMP_SEQUENCE}, 
																				{"JPG", IMAGE_TYPE_JPG_SEQUENCE}, 
																				{"PNG", 23}		}; 
S32 sequential_interval[MAX_SEQUENTIAL_IMAGE_FRAME];
S32 sequential_filesize[MAX_SEQUENTIAL_IMAGE_FRAME];
S32 sequential_offset[MAX_SEQUENTIAL_IMAGE_FRAME];
S32 sequential_extension_type; 
S32 sequential_frame;
 

extern S8 CustImgPath[];

#ifdef __UCS2_ENCODING
U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer );
#endif
extern HWND hWnd;

U16 CurrMaxStringId;
extern U16 CurrMaxImageNum;
extern U16 CurrMaxImageId;
extern U16 CurrMaxMenuItemIndex;

 
 
extern U16 CurrMaxImageNumEXT;
extern U16 CurrMaxImageIdEXT;
 
 

#ifdef __POPULATE_ENGINE_
void AddStringInfo(int StrId,char* pString,char* pStringDisplay);
void AddImageInfo(int ImgId,char* pImage,char* pIMageDisplay);
void AddApplicationInfo(U32 nId, S8* pName, U32 nMinId, U32 nMaxId);
#endif 

void InitStrPopulate()
{
	int i=0;
	for (i=0;i<MAX_STRING_MAP_SIZE;i++)
		StrIsPopulate[i]=0;

}


 
void ConvertToPrintable(S8* original, S8* converted){
	U16 i=0, j=0, strlength=0;
	strlength =wcslen ((wchar_t*)original);
	for ( i=0; i<strlength*2; i++){
		if ( original[i]==10 ){
			converted[j]=92;
			converted[++j]=0;
			converted[++j]='n';
			j++;
		} else {
			converted[j++]=original[i];
		}
	}
	if (strlength==0){//If NULL string, fill in " "
		converted[0]=20;
		converted[1]='\0';
		converted[2]='\0';
		converted[3]='\0';
	} else {
		converted[j++]='\0';
		converted[j]='\0';
	}
}
 

 
void SetStringWithDisplay(U16 nStrId, S8* pString1, S8* pDisplay1,S8* enum_value)
{
	U16 i;
	U16 found=0;
	S8 *pString = NULL;
	S8 *pDisplay = NULL;
	ResourceInfo	sInfo;

	S8* sString = NULL;
	U16 StrTotalLen=0;
	U16 nLen=0;
 
	S8 cString[1024],cDisplay[1024];
	S8 *pEnum_value = NULL;
 


#ifdef __ASCII

		pString = pString1;
		pDisplay = pDisplay1;
#endif


#ifdef __UCS2_ENCODING

	pString = OslMalloc( (strlen(pString1) + 1) * 2 );
	AnsiiToUnicodeString(pString,pString1);

	pDisplay = OslMalloc( (strlen(pDisplay1) + 1) * 2 );
	AnsiiToUnicodeString(pDisplay,pDisplay1);

	pEnum_value = OslMalloc( (strlen(enum_value) + 1) * 2 );
	AnsiiToUnicodeString(pEnum_value,enum_value);

#endif
 
//	if(CurrMaxStringId >= MAX_STRING_IDS) return;  
//	if (gpStringMapRes[nStrId]==(U16)-1) 
	if (StrIsPopulate[nStrId]==0) 
	{
		CurrMaxStringId++;
		StrIsPopulate[nStrId]=1;
		ConvertToPrintable(pString,cString);
		ConvertToPrintable(pDisplay,cDisplay);
		fwprintf (StringListFile,L"%ls\t%d\t%ls\t%ls\r\n",(wchar_t*)pEnum_value,nStrId,(wchar_t*)cDisplay,(wchar_t*)cString);
		fprintf (EnumListFile,"%s,\n",enum_value);
	}
	else  
	{
		fprintf(RepeatIDListFile, "%d\t%s\t\t%s\n", nStrId, enum_value, "String");
	}
}
 
/**************************************************************

	FUNCTION NAME		: AddApp(U32 nId, S8* pName, U16 nMinId, U16 nMaxId)

  	PURPOSE				: Function to Add a new application into the framework

	INPUT PARAMETERS	: U32 nId - App Id
						  S8* pName - Application name.
						  U16 nMinId - Minimum Id value supported
						  U16 nMaxId - Maximum Id value

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

**************************************************************/

void AddApp(U32 nId, S8* pName, U16 nMinId, U16 nMaxId)
{
	ResourceInfo	sInfo;

	sInfo.nAppId = nId;
	sInfo.nMaxId = nMaxId;
	sInfo.nMinId = nMinId;
	sInfo.pValue = pName;
}

/**************************************************************

	FUNCTION NAME		: SetString(U16 StrId, S8* String)

  	PURPOSE				: Function to Update the Customizable Resource String & Mapping Array.  

	INPUT PARAMETERS	: (U16)StringId & (S8*) String to be Displayed.

	OUTPUT PARAMETERS	: ( U16) String Number which is Mapped with the StringId. 

	RETURNS				: StringNumber which is mapped with the StringID.

 

**************************************************************/

 
void SetString(U16 StrId, S8* String1,S8* enum_value)
{
	 
	SetStringWithDisplay(StrId, String1, String1, enum_value); 
	 
}
 
/**************************************************************

	FUNCTION NAME		: SetImage(U16 ImgId, S8* filename)

  	PURPOSE				: Function to Update the Customizable Image Filename & Mapping Array.  

	INPUT PARAMETERS	: (U16)ImageId & (S8*) filename of the image to be displayed.

	OUTPUT PARAMETERS	: ( U16) Image Number which is Mapped with the ImageId. 

	RETURNS				: Image Number which is mapped with the ImageId.

 

**************************************************************/
void SetImage(U16 ImgId, S8* filename, S8* enum_value, U32 res_type) 
{
	 
	SetImageWithBuffer(ImgId, filename, "", enum_value, res_type, FALSE); 
	 
}



/**************************************************************

	FUNCTION NAME		: SetImageWithBuffer

  	PURPOSE				: Function to copy the image file to buffer

	INPUT PARAMETERS	: (U16) --> ImageId 

						  

	OUTPUT PARAMETERS	: (S8*) --> filename of the image to be displayed.

	RETURNS				: void

 

**************************************************************/

void SetImageWithBuffer(U16 nImgId, S8* filename,S8* pDisplayName, S8* enum_value, U32 res_type, BOOL imgFlag) 
{
	U16 i;
	U16 found=0;
	S16	nStrlen;
	ResourceInfo	sInfo;
	U16 nType;
	S8 upperFileName[MAX_FILENAME_LEN];
	unsigned short *pCurrMaxImageId;
	unsigned short *pCurrMaxImageNum;
	IMAGENAME_LIST *pImageNameList;
	CUSTOM_IMAGE_MAP *pImageIdMap;

	if(imgFlag == TRUE)
	{	
		ExtImageFlag = TRUE;
		pCurrMaxImageId = &CurrMaxImageIdEXT;
		pCurrMaxImageNum = &CurrMaxImageNumEXT;
		pImageNameList = ImageNameListEXT;
		pImageIdMap = ImageIdMapEXT;
	}
	else
	{
		ExtImageFlag = FALSE;
		pCurrMaxImageId = &CurrMaxImageId;
		pCurrMaxImageNum = &CurrMaxImageNum;
		pImageNameList = ImageNameList;
		pImageIdMap = ImageIdMap;
	}

	if(*pCurrMaxImageId >= MAX_IMAGE_IDS_SIZE) return;  
	
	fprintf(OutputImgIDFile,"file name%d = %s \n", image_idx++, filename);
	

	if(strlen(filename) >= MAX_FILENAME_LEN)
	{
		filename[MAX_FILENAME_LEN-1] = '\0';
	}
	if( !(strcmp(filename,"")))
	{
		strcpy(filename_buffer, NOIMAGE_FILENAMEBUFFER);

	}
	nStrlen = strlen(filename) ;

	while (nStrlen>=0)
	{
		upperFileName[nStrlen] = toupper(filename[nStrlen]);
		--nStrlen;
	}

	for (i=0; i<*pCurrMaxImageNum;i++) 
	{
		if(!strcmp(pImageNameList[i].filename,upperFileName))
		{
			found=1;
			break;
		}
	}

	if(pImageIdMap[nImgId].nImageId != 0) 
	{
		switch (res_type){
			case RES_IMAGE:
		fprintf(RepeatIDListFile, "%d\t%s\t\t%s\n", nImgId, enum_value, "Image");
				break;
			case RES_MEDIA:
				fprintf(RepeatIDListFile, "%d\t%s\t\t%s\n", nImgId, enum_value, "Media");
				break;
			default:
				printf("Non supported resource type!\n");
		}
	}


	if(found)
	{
		pImageIdMap[nImgId].nImageId = nImgId ;
		pImageIdMap[nImgId].nImageNum = i;
		(*pCurrMaxImageId)++;
	}
		else
	{
		if ( Iload(upperFileName,&nType) )
		{
			pImageIdMap[nImgId].nImageId = nImgId ;
			strcpy(pImageNameList[*pCurrMaxImageNum].filename,upperFileName);
			pImageIdMap[nImgId].nImageNum = *pCurrMaxImageNum;
			(*pCurrMaxImageNum)++;
			(*pCurrMaxImageId)++;
		}
		 
		else 
		{
			if (toolFlag)
				resultFailFlag = 1;
			ImageIdMap[nImgId].nImageId = nImgId ;
			strcpy(ImageNameList[CurrMaxImageNum].filename,DEFAULT_FILEBUFFER);
			ImageIdMap[nImgId].nImageNum = CurrMaxImageNum;
			CurrMaxImageNum++;
			CurrMaxImageId++;
		}
		 
	}
	sInfo.pDisplayValue = pDisplayName;
	sInfo.pValue = upperFileName;
}



/**************************************************************

	FUNCTION NAME		: MenuItemPresent

  	PURPOSE				: Function to find ImageId

	INPUT PARAMETERS	: (U16) --> ImageId 

						  

	OUTPUT PARAMETERS	: Nil

	RETURNS				: U16

 

**************************************************************/


U16 MenuItemPresent(U16 ItemId)
{
	U16 i;
	for(i=0;i<CurrMaxMenuItemIndex;i++)
	{
		if(nCustMenus[i].nMenuItemId == ItemId)
		{
			return TRUE;
		}
	}
	return FALSE;
}


/**************************************************************

	FUNCTION NAME		: PrintMenuItems

  	PURPOSE				: Fills the CUSTOM_MENU structure, for MenuTree Tool usage

	INPUT PARAMETERS	: (U16) --> variable argument list 

						  

	OUTPUT PARAMETERS	: Nil

	RETURNS				: void

 

**************************************************************/
static char is_dummy_code=0; // To check if it needs to generate for menu tree tool or not.
void PrintMenuItems(char *arg1)
{

//static U8 is_first=1;
FILE	*fout;

	if (is_dummy_code==1) return;

	if (is_first)
	{
		fout = fopen(OUTPUT_MENUTREE_FILENAME,"w");
		is_first= 0;
	}
	else
	{
		fout = fopen(OUTPUT_MENUTREE_FILENAME,"a");
	}

	fprintf(fout,"ADD_APPLICATION_MENUITEM2(%s);\n",arg1);
	fclose(fout);


}

/**************************************************************

	FUNCTION NAME		: PrintMenuItems2

  	PURPOSE				: Fills the CUSTOM_MENU structure, for MenuTree Tool usage

	INPUT PARAMETERS	: (U16) --> variable argument list 

						  

	OUTPUT PARAMETERS	: Nil

	RETURNS				: void

 

**************************************************************/
void PrintMenuItems2(char* arg1,...)
{


//static U8 is_first=1;
FILE	*fout;

	if (is_dummy_code==1) return;

	if (is_first)
	{
		fout = fopen(OUTPUT_MENUTREE_FILENAME,"w");
		is_first= 0;
	}
	else
	{
		fout = fopen(OUTPUT_MENUTREE_FILENAME,"a");
	}

	fprintf(fout,"ADD_APPLICATION_MENUITEM3(%s);\n",arg1);
	fclose(fout);



}

/**************************************************************

	FUNCTION NAME		: SetMenuItem

  	PURPOSE				: Fills the CUSTOM_MENU structure

	INPUT PARAMETERS	: (U16) --> variable argument list 

						  

	OUTPUT PARAMETERS	: Nil

	RETURNS				: void

 

**************************************************************/
void SetMenuItem(U16 arg1,...)
{

U16 MemberCounter = 9;
U16 i=arg1;
U16 j;
U16 menuItemID ;
va_list argumentPtr;
 
//FILE	*fout;
//static U8 is_first=1;

	//if (is_first)
	//{
	//	fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"w");
	//	is_first= 0;
	//}
 

	va_start(argumentPtr,arg1);

	/* Need to decide whether menu item already present */
	if(!MenuItemPresent(i)) 
	{
		 
		//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
		fprintf(OutputMenutreeIDFile,"ADD_APPLICATION_MENUITEM2((",arg1);
		//fclose(fout);
	
		is_dummy_code = 0;
		while(MemberCounter)
		{
			switch(MemberCounter)
			{
			case MENUITEM_ID_TYPE:
				menuItemID = i - 1;

				nCustMenus[menuItemID].nMenuItemId=i;
				//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
				fprintf(OutputMenutreeIDFile,"%d,",i);
				//fclose(fout);
			break;
			case PARENT_ITEM_ID_TYPE:
				nCustMenus[menuItemID].nParentId=i;
				//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
				fprintf(OutputMenutreeIDFile,"%d,",i);
				//fclose(fout);
			break;
			case NUMBER_OF_MENUITEMS_TYPE:
				nCustMenus[menuItemID].nNumofMenuItem=i;
				//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
				fprintf(OutputMenutreeIDFile,"%d,",i);
				//fclose(fout);
			break;
			case MENU_ITEM_ORDER_TYPE:
				if(nCustMenus[menuItemID].nNumofMenuItem==0)
					nCustMenus[menuItemID].nOrderMenuItemId=(U16 *)0;
				else 
				{
					nCustMenus[menuItemID].nOrderMenuItemId=(U16 *)malloc(sizeof(U16)*MENU_ITEM_SIZE);
					for(j=0;j<nCustMenus[menuItemID].nNumofMenuItem;j++)
					{
						//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
						fprintf(OutputMenutreeIDFile,"%d,",i);
						//fclose(fout);
						nCustMenus[menuItemID].nOrderMenuItemId[j]=i;
						i=va_arg(argumentPtr,int);
					}
				}
			break;
			case HIDE_FLAG_TYPE:
				nCustMenus[menuItemID].nHideFlag=i;
				//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
				fprintf(OutputMenutreeIDFile,"%d,",i);
				//fclose(fout);
			break;
			case MOVEABLE_FLAG_TYPE:
				nCustMenus[menuItemID].nMovable=i;
				//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
				fprintf(OutputMenutreeIDFile,"%d,",i);
				//fclose(fout);
			break;
			case DISPLAY_ATTRIBUTE_TYPE:
				nCustMenus[menuItemID].nDispAttribute=i;
				//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
				fprintf(OutputMenutreeIDFile,"%d,",i);
				//fclose(fout);
			break;
			case MENU_ITEM_STRING_ID_TYPE:
				nCustMenus[menuItemID].nStrId=i;
				//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
				fprintf(OutputMenutreeIDFile,"%d,",i);
				//fclose(fout);
			break;
			case MENU_ITEM_IMAGE_ID_TYPE:
				nCustMenus[menuItemID].nImageId=i;
				//fout = fopen(OUTPUT_MENUTREE_ID_FILENAME,"a");
				fprintf(OutputMenutreeIDFile,"%d));\n",i);
				//fclose(fout);
			break;
			}
			if(MemberCounter!=MENU_ITEM_ORDER_TYPE)
				i=va_arg(argumentPtr,int);
			MemberCounter--;
		}
		 
	}
	else
	{
			is_dummy_code = 1;
	}

	va_end(argumentPtr);
}

/**************************************************************

	FUNCTION NAME		: SetMenuItem

  	PURPOSE				: Fills the CUSTOM_MENU structure

	INPUT PARAMETERS	: (U16) --> variable argument list 

						  

	OUTPUT PARAMETERS	: Nil

	RETURNS				: void

 

**************************************************************/
void SetMenuItem2(U16 arg1,...)
{

U16 MemberCounter = 9;
U16 i=arg1;
U16 j;
U16 menuItemID ;
va_list argumentPtr;

	va_start(argumentPtr,arg1);

	/* Need to decide whether menu item already present */
	if(!MenuItemPresent(i)) 
	{
		fprintf(OutputMenutreeIDFile,"ADD_APPLICATION_MENUITEM2((",arg1);
	
		is_dummy_code = 0;
		while(MemberCounter)
		{
			switch(MemberCounter)
			{
				case MENUITEM_ID_TYPE:
					menuItemID = i - 1;
	
					nCustMenus[menuItemID].nMenuItemId=i;
					fprintf(OutputMenutreeIDFile,"%d,",i);
				break;
	
				case PARENT_ITEM_ID_TYPE:
					nCustMenus[menuItemID].nParentId=i;
					fprintf(OutputMenutreeIDFile,"%d,",i);
				break;
	
				case MENU_ITEM_ORDER_TYPE:
					if ( /* nCustMenus[menuItemID].nNumofMenuItem==0 || */ i == MENU_ITEM_END)
					{
						nCustMenus[menuItemID].nOrderMenuItemId=(U16 *)0;
					}
					else 
					{
						U16 number_of_child=0;
						nCustMenus[menuItemID].nOrderMenuItemId=(U16 *)malloc(sizeof(U16)*MENU_ITEM_SIZE);
						while ( i < MENU_ITEM_END )
						{
							nCustMenus[menuItemID].nOrderMenuItemId[number_of_child]=i;
							i=va_arg(argumentPtr,int);
							number_of_child++;
						}
						nCustMenus[menuItemID].nNumofMenuItem=number_of_child; //Set number of child field
						fprintf (OutputMenutreeIDFile,"%d,",number_of_child);
						for ( i=0; i<number_of_child; i++ )
							fprintf (OutputMenutreeIDFile,"%d,",nCustMenus[menuItemID].nOrderMenuItemId[i]);
						i=va_arg(argumentPtr,int); // read in next argument.
					}
				break;
	
				case HIDE_FLAG_TYPE:
					nCustMenus[menuItemID].nHideFlag=i;
					fprintf(OutputMenutreeIDFile,"%d,",i);
				break;
	
				case MOVEABLE_FLAG_TYPE:
					nCustMenus[menuItemID].nMovable=i;
					fprintf(OutputMenutreeIDFile,"%d,",i);
				break;
	
				case DISPLAY_ATTRIBUTE_TYPE:
					nCustMenus[menuItemID].nDispAttribute=i;
					fprintf(OutputMenutreeIDFile,"%d,",i);
				break;
	
				case MENU_ITEM_STRING_ID_TYPE:
					nCustMenus[menuItemID].nStrId=i;
					fprintf(OutputMenutreeIDFile,"%d,",i);
				break;
	
				case MENU_ITEM_IMAGE_ID_TYPE:
					nCustMenus[menuItemID].nImageId=i;
					fprintf(OutputMenutreeIDFile,"%d));\n",i);
				break;
			}
			if(MemberCounter!=MENU_ITEM_ORDER_TYPE)
				i=va_arg(argumentPtr,int);

			if(MemberCounter==PARENT_ITEM_ID_TYPE)//Skip NUMBER_OF_MENUITEMS_TYPE field
				MemberCounter-=2;
			else
				MemberCounter--;
		}
	}
	else
	{
			is_dummy_code = 1;
	}

	va_end(argumentPtr);
}


/**************************************************************

	FUNCTION NAME		: Get_Image_Identifier

  	PURPOSE				: Identifies the image , BMP , ppf etc

	INPUT PARAMETERS	: S32 

						  

	OUTPUT PARAMETERS	: Nil

	RETURNS				: U8

 
						  image type will be supported by the frame work 
						  rt. now it just a added hook to make the design complete

**************************************************************/


U8 Get_Image_Identifier(S32 extension_type)
{	
	S32 ret;
	if(extension_type<0) 
		ret = 0;
	
	else if(extension_type==0)
		ret = 1;
		
	return (U8)ret;
	
}


/**************************************************************

	FUNCTION NAME		: Load_Image_Data

  	PURPOSE				: Identifies the image , BMP , ppf etc

	INPUT PARAMETERS	: FILE *output_file --> Name of the file to which buffer 
						  is to be written

						  FILE *input_file -->  Name of the input image file 	

						  U8 image_identifier --> image identifier 
						  
						  S32 n_frames --> no of frames

						  

	OUTPUT PARAMETERS	: U32 *offset file offset --> file offset 
						  U8*  pImgData --> buffer containing image data	

	RETURNS				: S32 --> file size

 

**************************************************************/
 
 
S32 Load_Image_Data(U32 *offset,FILE *output_file,FILE *input_file,U8* pImgData,
					 U8 image_identifier, S32 n_frames)
{   
	U32 file_size,i,o=0;
	U8	ch;
	fseek(input_file,0,SEEK_END);
	file_size=ftell(input_file);
    fseek(input_file,0,SEEK_SET);
	
	if(toolFlag)
	{
		fprintf(dest_file,"%c%c",image_identifier,n_frames);
		if ( image_identifier == IMAGE_TYPE_GIF  || image_identifier == IMAGE_TYPE_3GP || 
			image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI || 
		 	image_identifier == IMAGE_TYPE_PNG)  
		    fprintf(dest_file,"%c%c%c%c%c%c",file_size&0xff,(file_size&0xff00)>>8,(file_size&0xff0000)>>16,(file_size&0xff000000)>>24,0,0);
		else
		    fprintf(dest_file,"%c%c%c%c",file_size&0xff,(file_size&0xff00)>>8,(file_size&0xff0000)>>16,(file_size&0xff000000)>>24);
	}
	else
	{
	fprintf(output_data_file,"\n\t0x%02X, 0x%02X, ",image_identifier,n_frames);
	if ( 	image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP || 
		 	image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
		 	image_identifier == IMAGE_TYPE_PNG)  
	    fprintf(output_file,"0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x00, 0x00,\n\t",file_size&0xff,(file_size&0xff00)>>8,(file_size&0xff0000)>>16,(file_size&0xff000000)>>24);
	else
	    fprintf(output_file,"0x%02X, 0x%02X, 0x%02X, 0x%02X,\n\t",file_size&0xff,(file_size&0xff00)>>8,(file_size&0xff0000)>>16,(file_size&0xff000000)>>24);
	}
	
	pImgData[0] = (U8)image_identifier;
	pImgData[1] = (U8)n_frames;
	pImgData[2] = (U8)(file_size&0xff);
	pImgData[3] = (U8)((file_size&0xff00)>>8); 
	pImgData[4] = (U8)((file_size&0xff0000)>>16); 
	pImgData[5] = (U8)((file_size&0xff000000)>>24); 

    for(i=0;i<file_size;i++)
    {	
		ch = fgetc(input_file);

		if(toolFlag){
			fputc(ch, dest_file);
	    		(*offset)++;
			(pImgData)[i+6] = ch;
		}
		else
		{
			fprintf(output_file,"0x%02X, ",ch);
	    		(*offset)++;
			(pImgData)[i+6] = ch;
	        	o++;
	    		if((o%16)==0)
	        	{	
	        		fprintf(output_file,"\n\t");
	        	}
		}
    }

	if ( 	image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP || 
		image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
		image_identifier == IMAGE_TYPE_PNG) 
	{
		return file_size + 8;
	}
	else
		return file_size + 6;
}

/**************************************************************

	FUNCTION NAME		: Write_Sequential_Image_Header

  	PURPOSE				: Construct the header for sequential images

	INPUT PARAMETERS	: FILE *output_file --> output file handle
						  U8 image_identifier --> sequential image type
						  S32 n_frames --> the number of the frames
						  S32 *seq_filesize --> pointer to a list of file sizes
						  
	OUTPUT PARAMETERS	: S32 *seq_offset --> pointer to a list of offsets 

						  
	RETURNS				: void

 

**************************************************************/
void Write_Sequential_Image_Header(FILE *output_file, U8 image_identifier, S32 n_frames, S32 *seq_filesize, S32 *seq_offset)
{
	S32 i;
	S32 offset = 8;
	S32 real_size;

	if(toolFlag == 1)
	{
		fprintf(dest_file,"%c%c%c%c%c%c%c%c",image_identifier,n_frames,0,0,0,0,0,0);//the major header
		for(i =0; i<n_frames; i++)
		{
			int interval = sequential_interval[i];
			seq_offset[i] = offset;
			real_size = ((seq_filesize[i]+3)>>2)<<2;
			fprintf(dest_file,"%c%c%c%c",seq_offset[i]&0xff,(seq_offset[i]&0xff00)>>8,(seq_offset[i]&0xff0000)>>16,(seq_offset[i]&0xff000000)>>24);//the offset for each file
			fprintf(dest_file,"%c%c%c%c",seq_filesize[i]&0xff,(seq_filesize[i]&0xff00)>>8,(seq_filesize[i]&0xff0000)>>16,(seq_filesize[i]&0xff000000)>>24);//the size for each file
			fprintf(dest_file,"%c%c%c%c",interval&0xff,(interval&0xff00)>>8,(interval&0xff0000)>>16,(interval&0xff000000)>>24);//the interval for each file
			offset += real_size;
		}
	}
	else
	{
		fprintf(output_file,"\n\t0x%02X, 0x%02X, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\t",image_identifier,n_frames);//the major header
		for(i =0; i<n_frames; i++)
		{
			int interval = sequential_interval[i];
			seq_offset[i] = offset;
			real_size = ((seq_filesize[i]+3)>>2)<<2;
			fprintf(output_file,"0x%02X, 0x%02X, 0x%02X, 0x%02X, ",seq_offset[i]&0xff,(seq_offset[i]&0xff00)>>8,(seq_offset[i]&0xff0000)>>16,(seq_offset[i]&0xff000000)>>24);//the offset for each file
			fprintf(output_file,"0x%02X, 0x%02X, 0x%02X, 0x%02X, ",seq_filesize[i]&0xff,(seq_filesize[i]&0xff00)>>8,(seq_filesize[i]&0xff0000)>>16,(seq_filesize[i]&0xff000000)>>24);//the size for each file
			fprintf(output_file,"0x%02X, 0x%02X, 0x%02X, 0x%02X,\n\t",interval&0xff,(interval&0xff00)>>8,(interval&0xff0000)>>16,(interval&0xff000000)>>24);//the interval for each file
			offset += real_size;
		}
	}
}

/**************************************************************

	FUNCTION NAME		: Write_Sequential_Image_File

  	PURPOSE				: Write raw date from a frame of an image to the output file

	INPUT PARAMETERS	: FILE *output_file --> output file handle
						  FILE *input_file --> input file handle
						  S32 filesize --> the input file size
						  
	OUTPUT PARAMETERS	: void 

						  
	RETURNS				: void

 

**************************************************************/
void Write_Sequential_Image_File(FILE *output_file, FILE *input_file, S32 filesize)
{
	U32	i;
	U32 o=0;
	U8	ch;

	if(toolFlag == 1)
	{
		for(i=0;i<filesize;i++)
	    	{	
			ch = fgetc(input_file);
			fprintf(dest_file,"%c",ch);
	    	}

		for(i=filesize; i%4!=0; i++)
		{
			fprintf(dest_file,"%c",0);// for 4 byte alignment
		}
	}
	else{
		for(i=0;i<filesize;i++)
	    	{	
			ch = fgetc(input_file);
			fprintf(output_file,"0x%02X, ",ch);
			o++;
			if((o%16)==0)
			{	
				fprintf(output_file,"\n\t");
			}
	    	}

		for(i=filesize; i%4!=0; i++)
		{
			fprintf(output_file,"0x00, ");// for 4 byte alignment
		}

		if((o%16)!=0)
		{
			fprintf(output_file,"\n\t");
		}
	}
}
 
 

/**************************************************************

	FUNCTION NAME		: Extract_Name

  	PURPOSE				: Exract image name from the supplied string

	INPUT PARAMETERS	: S8* src --> string containing file name and path
	  

	OUTPUT PARAMETERS	: S8 *dest--> Extrected file name from string 

						  
	RETURNS				: void

 

**************************************************************/

void Extract_Name(S8 *dest, S8* src)
{
 	U32	iLen	=	strlen(src);
	U16	nCount	=	0;

	S8 *pSrc=src;
	char	a,b;
	iLen	=	strlen(CustImgPath);

	for (nCount=0;nCount<iLen;nCount++)
	{
		a = toupper(CustImgPath[nCount]);
		b = toupper(src[nCount]);
		if (a!=b) 
			break;
	}
	if (iLen==nCount) pSrc=&src[iLen];

	iLen	=	strlen(pSrc);
	nCount=0;

	while(iLen)
	{
		if((pSrc[nCount] == '.')||(pSrc[nCount] == '-')||(pSrc[nCount] == ' ')||(pSrc[nCount] == '\\'))
			dest[nCount] = '_';
		else
			dest[nCount] = pSrc[nCount];
		
		++nCount;
		--iLen;
	}

	 
	if(ExtImageFlag == TRUE) 
	{
		dest[nCount] = '_';//add a '_' in the end to distinguish from the original
		dest[nCount+1] = '\0';
	}
	 
	else
	{
		dest[nCount] = '\0';
	}
	return ;

}




/**************************************************************

	FUNCTION NAME		: ExtractExtension

  	PURPOSE				: Function Extracting the extension

	INPUT PARAMETERS	: S8 *string --> string containing file name 
	  

	OUTPUT PARAMETERS	: 

						  
	RETURNS				: U8 

 

**************************************************************/

U8 ExtractExtension( S8* src)
{
	S32 i;
	U8 ret = 0;
	
	
#if 0 // __UCS2_ENCODING
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
/* under construction !*/
/* under construction !*/
#endif

//#ifdef __ASCII
	S32 j;
	S8 arr[4];

	i = strlen(src);
	

	j = 0;
    
	i = i -1;
	while(j < 3)
	{
			
		arr[j] = src[i];
		--i;
		++j;
	}

	arr[j] = '\0';

	/* bmp is compared as pmb because extracted revers way*/
	if( !(strcmp(arr,"PMB")))
	{
		ret =  IMAGE_TYPE_BMP; 
	}
	/* gif is compared as fig because extracted revers way*/
	else if( (!strcmp(arr,"FIG")))
	{
		ret = IMAGE_TYPE_GIF; 

	}
	/* JPG is compared as GPJ because extracted revers way, detail value ref: gui.h for image type */
	else if( (!strcmp(arr,"GPJ")))
	{
		ret = IMAGE_TYPE_JPG; 

	}
	else if( (!strcmp(arr,"IVA")))
	{
		ret = IMAGE_TYPE_AVI;

	}
	else if( (!strcmp(arr,"PG3")))
	{
		ret = IMAGE_TYPE_3GP;

	}
	else if( (!strcmp(arr,"4PM")))
	{
		ret = IMAGE_TYPE_MP4;

	}
	 
	else if( (!strcmp(arr,"GNP")))
	{
		ret = IMAGE_TYPE_PNG;

	}
	 
	else
	{
	
			ret = IMAGE_TYPE_INVALID; 
	}
//#endif

	return ret;
}

/**************************************************************

	FUNCTION NAME		: isEmsImage

  	PURPOSE				: Decide if the iamge is used for EMS picture

	INPUT PARAMETERS	: S8 *path --> string containing file name and path
	  

	OUTPUT PARAMETERS	: 

						  
	RETURNS				: U8 --> 1: EMS picture, 0: Not EMS picture

 

**************************************************************/

U8 isEmsImage(S8 *path){
	S8 ems_path[] = EMS_IMAGE_PATH;
	U8 pos=0, ems_len=0, i=0;

	ems_len = strlen(ems_path);
	pos = strlen (path)-1;
	while (path[pos]!='\\') {
		 
		if(pos == 0) 
			return 0;
		 
		pos--;
	}
	pos-=ems_len+1;
	for ( i=0; i<ems_len; i++){
		if ( ems_path[i] != path[pos] ) return 0;
		else pos++;
	}

	return 1;
}

/**************************************************************

	FUNCTION NAME		: isMainmenuImage

  	PURPOSE				: Decide if the iamge is used for Mainmenu picture

	INPUT PARAMETERS	: S8 *path --> string containing file name and path
	  

	OUTPUT PARAMETERS	: 

						  
	RETURNS				: U8 --> 1: Matrix Mainmenu picture, 0: Not Matrix mainmenu picture

 

**************************************************************/
MMI_BOOL isMainmenuImage(S8 *path)
{
if ( (strstr(path,"\\MATRIX\\")!= NULL) && (strstr(path,".GIF")!=NULL) ) 
	return MMI_TRUE;
else
	return MMI_FALSE;
}

/**************************************************************

	FUNCTION NAME		: isSubmenuImage

  	PURPOSE				: Decide if the iamge is used for Submenu picture

	INPUT PARAMETERS	: S8 *path --> string containing file name and path
	  

	OUTPUT PARAMETERS	: 

						  
	RETURNS				: U8 --> 1: Submenu picture, 0: Not Submenu picture

 

**************************************************************/

U8 isSubmenuImage(S8 *path){
#ifndef __MMI_SLIM_LISTMENU_NUMBER__
	return 0;
#else
	S8 submenu_path[] = SUBMENU_IMAGE_PATH;
	U8 pos=0, submenu_len=0, i=0;

	if ( strstr(path,submenu_path)!=NULL )
		return 1;
	else
		return 0;
/*
	submenu_len = strlen(submenu_path);
	pos = strlen (path)-1;
	while (path[pos]!='\\') pos--;
	pos-=submenu_len+1;
	for ( i=0; i<submenu_len; i++){
		if ( submenu_path[i] != path[pos] ) return 0;
		else pos++;
	}
	return 1;
*/
#endif
}

 
 
S8 ToUpper(S8 ch)
{
	return toupper(ch);
}
 
 

/**************************************************************

	FUNCTION NAME		: Iload

  	PURPOSE				: Load the file image to buffer and writes to CustImgRes.c

	INPUT PARAMETERS	: S8 *string --> string containing file name and path
	  

	OUTPUT PARAMETERS	: PU16 pType --> Image Type

						  
	RETURNS				: void* --> buffer containing image data

 

**************************************************************/

MMI_BOOL Iload(S8 *string,PU16 pType)
{
    U32	image_counter=0;
    U32	image_offset=0;
    U32	failure_counter=0;
	U8*		pBuffer = NULL;
    S32		j,sequence_counter,n_frames;
    MMI_BOOL		failure_flag;
    U8		image_identifier;
    S32		n_images = 0;
	S32     file_size1 = 0;
	S32     file_size = 0;
	S32     nImageSize = 0;
	U8*    pBuffer1 = NULL;
	S32    extension_type = -1 ;
	U8     staticFlag = 1;  /*Non animated image*/
	 
	//S8		tempDestFile[] = {"..\\\\..\\\\Customer\\\\IMAGES\\TEMPIMAGE"};
	S8		tempDestFile[] = {"TEMPIMAGE"}; 
	int ret = 1;
	  
	S8		tempCommand[200];
	size_t	size;
	U32		 o=0;
	S16		filenameLen;
	char *curr_output_data_filename;

	 
	if(ExtImageFlag == TRUE) 
	{
		curr_output_data_filename = OUTPUT_DATA_FILENAME_EXT;
	}
	else
	{
		curr_output_data_filename = OUTPUT_DATA_FILENAME;
	}

	if(toolFlag == 1){ 
		curr_output_data_filename = "temp";
	}
	
    	strcpy(filename_buffer,curr_output_data_filename); 
	 
    
    output_data_file=fopen(filename_buffer,"ab");
	while(!output_data_file)
	{
		Sleep(50);
		output_data_file=fopen(filename_buffer,"ab");
	}
//Move behind
	//fprintf(output_data_file,"const U8 ");

	output_image_names = (S8*)malloc(100);
	strcpy(filename_buffer,string);
	if( !(strcmp(filename_buffer,"")))
	{

		strcpy(filename_buffer, NOIMAGE_FILENAMEBUFFER);
	}


	Extract_Name(output_image_names,filename_buffer);
#ifdef __MMI_SLIM_LISTMENU_NUMBER__
	if ( isSubmenuImage(filename_buffer) )
	{
		int i,len;
		len=strlen(filename_buffer);
		
		for (i=len;i>0;i--)
		{
			if ( filename_buffer[i]=='U' && filename_buffer[i-1]=='N' && filename_buffer[i-2]=='E'&& filename_buffer[i-3]=='M'&& filename_buffer[i-4]=='B'&& filename_buffer[i-5]=='U'&& filename_buffer[i-6]=='S')
			{
				filename_buffer[i+1]='2';
				break;
			}
			else
			{
				filename_buffer[i+1]=filename_buffer[i];
			}
			
		}
		filename_buffer[len+1]='\0';
	}
#endif

    failure_flag=MMI_TRUE;
    extension_type = -1;
    sequence_counter=0;
	
	filenameLen = strlen(filename_buffer);

	 
	if ( isEmsImage(filename_buffer) )
//	if (filename_buffer[filenameLen-3] =='P' && filename_buffer[filenameLen-2] == 'I' && filename_buffer[filenameLen-1] =='C' )
	{
		image_data_file = fopen(filename_buffer,"rb");
		if ( image_data_file!=NULL )
		{
			 
			if(toolFlag == 0) 
			{
				fprintf(output_data_file,"const U8 ");
				fprintf(output_data_file,output_image_names);
				fprintf(output_data_file,"[]=");
				fclose(output_data_file);
			}
			//sprintf(tempCommand,"%s %s -b %s", EMSCONVERTER_FILENAME,filename_buffer,OUTPUT_DATA_FILENAME);//,output_image_names);
			//system(tempCommand);
			ret = bmp2ems(filename_buffer, curr_output_data_filename, 1, 0); 
			if(toolFlag)
			{
				if(ret = 0)
					resultFailFlag = 1;
			}
			 
		}
		else
		{
//			fclose(image_data_file);
			failure_flag = MMI_FALSE;	
		}
	}
	else if((filename_buffer[filenameLen-1] == 'P' || filename_buffer[filenameLen-1] == 'M')&& (filename_buffer[filenameLen-2] == 'R'|| filename_buffer[filenameLen-2] == 'B') && filename_buffer[filenameLen-3] == 'P' )
	{
		image_data_file = fopen(filename_buffer,"rb");
		if ( image_data_file!=NULL )
		{
			if(filename_buffer[filenameLen-2] == 'R')
			{
				//sprintf(tempCommand,"%s %s %s", GIFCONVERTER_FILENAME,filename_buffer,tempDestFile);
				 
				GIFLoader(filename_buffer, tempDestFile); 
			}
			else
			{
				//sprintf(tempCommand,"%s %s %s", BMPCONVERTER_FILENAME,filename_buffer,tempDestFile);
				 
				 
				ret = BMPLoader(filename_buffer, tempDestFile); 
				if(toolFlag)
				{
					if(ret = 0)
						resultFailFlag = 1;
				}
				 
			}
			 
			fclose(image_data_file);

			 
			if(toolFlag == 0) 
			{
				image_data_file=fopen(tempDestFile,"rb");
				if(image_data_file != NULL)
				{
					strcpy(( char*)autoimagename_buffer,(const char*)output_image_names);
					strcat(( char*)autoimagename_buffer,"[]=\n");
					fprintf(output_data_file,"const U8 ");
					fprintf(output_data_file,autoimagename_buffer);
				
					fseek(image_data_file,0,SEEK_END);
					file_size=ftell(image_data_file);
					fseek(image_data_file,0,SEEK_SET);
				
					while(!feof(image_data_file))
					{
						size = fread(tempCommand,sizeof(char),200,image_data_file);
						if(size)
						{
							fwrite(tempCommand,sizeof(char),size,output_data_file);
						}
					}
					fputs("\n",output_data_file);

				}
			}
			 
		}
		else
			failure_flag=MMI_FALSE;
	}
	else
	{
		image_data_file=fopen(filename_buffer,"rb");
 
		if (image_data_file==NULL) {
			Sleep(50);
			if ( filename_buffer[filenameLen-1] == 'P' && filename_buffer[filenameLen-2] == 'M' && filename_buffer[filenameLen-3] == 'B' ){
				filename_buffer[filenameLen-1] = 'F';
				filename_buffer[filenameLen-2] = 'I';
				filename_buffer[filenameLen-3] = 'G';
			}
			else if ( isMainmenuImage(filename_buffer ) )
			{
				filename_buffer[filenameLen-1] = 'M';
				filename_buffer[filenameLen-2] = 'B';
				filename_buffer[filenameLen-3] = 'P';
				image_data_file = fopen(filename_buffer,"rb");
				if ( image_data_file != NULL )
				{
					//sprintf(tempCommand,"%s %s %s", BMPCONVERTER_FILENAME,filename_buffer,tempDestFile);
					//system(tempCommand);
					 
					ret = BMPLoader(filename_buffer, tempDestFile); 
					if(toolFlag)
					{
						if(ret = 0)
							resultFailFlag = 1;
					}
					 

					 
					if(toolFlag == 0) 
					{
						image_data_file=fopen(tempDestFile,"rb");
						if(image_data_file != NULL)
						{
							strcpy(( char*)autoimagename_buffer,(const char*)output_image_names);
							strcat(( char*)autoimagename_buffer,"[]=\n");
							fprintf(output_data_file,"const U8 ");
							fprintf(output_data_file,autoimagename_buffer);
						
							fseek(image_data_file,0,SEEK_END);
							file_size=ftell(image_data_file);
							fseek(image_data_file,0,SEEK_SET);
						
							while(!feof(image_data_file))
							{
								size = fread(tempCommand,sizeof(char),200,image_data_file);
								if(size)
								{
									fwrite(tempCommand,sizeof(char),size,output_data_file);
								}
							}
							fputs("\n",output_data_file);
			
						}
						fclose (image_data_file);
					}
					fclose(output_data_file);
					return MMI_TRUE;
					 
				}
			}
			else if ( filename_buffer[filenameLen-1] == 'F' && filename_buffer[filenameLen-2] == 'I' && filename_buffer[filenameLen-3] == 'G' ){
				filename_buffer[filenameLen-1] = 'P';
				filename_buffer[filenameLen-2] = 'M';
				filename_buffer[filenameLen-3] = 'B';
			}
			image_data_file=fopen(filename_buffer,"rb");
		}
 
		if(image_data_file==NULL) 
		{ 

			
			/*Add extension add search */
			FILE *fail=fopen("fail.txt","a+");
			if ( fail != NULL )
			{
				 
				if(toolFlag==0) 
				{
					fprintf (fail,"%s\n",filename_buffer);
					fclose(fail);
				}
				failure_flag = MMI_FALSE; 
				 
			}

#if 0
#ifdef __MMI_BMP_ANIMATION__
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
/* under construction !*/
#endif
#endif

#if 1
			for(j=0;j<SUPPORT_N_SEQUENTIAL_IMAGE_TYPE;j++) //Find the right extension file name
			{
				itoa(0,number_string,10);
				extension_type=-1;
				strcpy(autofilename_buffer,filename_buffer);
				strcat(autofilename_buffer,"\\\\");
				//strcat(autofilename_buffer,output_image_names);
				strcat(autofilename_buffer,number_string);
	    			strcat(autofilename_buffer,".");
				strcat(autofilename_buffer,sequential_image[j].filename_ext);
				image_data_file=fopen(autofilename_buffer,"rb");
				if(image_data_file!=NULL)
				{
					failure_flag=MMI_TRUE;
					extension_type=sequential_image[j].file_type;
					staticFlag = 0;
					*pType = (U16)extension_type;
					sequential_extension_type = j; 
					fclose(image_data_file);
					break;
	             		}
			}
#endif

			 
			 
			 
		}
		else
		{
			// patch added by vikram for getting image file size
			fseek(image_data_file,0,SEEK_END);
			nImageSize = ftell(image_data_file);
			fseek(image_data_file,0,SEEK_SET);

		}

#if 0
#ifdef __MMI_BMP_ANIMATION__
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
	#endif

	#if 1
		if( ( failure_flag == MMI_TRUE) && !(staticFlag) ) 
		{
			/*	Sequence found	*/
        		/*	Find how many files are in sequence	*/

				 j=0;
				 sequence_counter=0;
				 n_frames=0;
				 while(1)
				 {  
				   itoa(j,number_string,10);
				   strcpy(autofilename_buffer,"");
				   strcpy(autofilename_buffer,filename_buffer);
				   strcat(autofilename_buffer,"\\\\");
				   //strcat(autofilename_buffer,output_image_names);
				   strcat(autofilename_buffer,number_string);
				   strcat(autofilename_buffer,".");
				   strcat(autofilename_buffer,sequential_image[sequential_extension_type].filename_ext); 
				   image_data_file=fopen(autofilename_buffer,"rb");
				   if(image_data_file!=NULL)
				   {   
				   	sequence_counter++;
					fseek(image_data_file,0,SEEK_END);
					sequential_filesize[j] = ftell(image_data_file);
					fseek(image_data_file,0,SEEK_SET);
					fclose(image_data_file);
				   }
				   else
				   {   /*	Sequence complete	*/
               			if(j>0) break;
				   }
					j++;
				  }
				  // read during
					{
						int i;
						strcpy(autofilename_buffer,filename_buffer);
						strcat(autofilename_buffer,"\\\\interval.txt");

						image_data_file = fopen(autofilename_buffer,"r");
						if(image_data_file==NULL)
							printf("ERROR: Can't find %s\n",autofilename_buffer);

						//printf("read interval value:%d\n",sequence_counter);
						for(i=0;i<sequence_counter;i++)
						{
							if(fscanf(image_data_file,"%d",&sequential_interval[i])!=1)
								printf("ERROR: interval value missing!!\n");
							//printf("interval[%d] = %d\n",i,sequential_interval[i]);
						}
						fclose(image_data_file);
					}

				   //n_frames=sequence_counter;
				   sequential_frame = sequence_counter;

				  /*Construct the data structure name	*/
				   strcpy(autoimagename_buffer,output_image_names);
				   strcat(autoimagename_buffer,OUTPUT_IMAGE_NAMES_END);
				   fprintf(output_data_file,"__align(4) const U8 ");
				   fprintf(output_data_file,autoimagename_buffer);

              			Write_Sequential_Image_Header(output_data_file,sequential_image[sequential_extension_type].file_type, 
              											sequential_frame, sequential_filesize, sequential_offset); 

				   /*	Load all files in the sequence	*/
				   j=0;
				   sequence_counter=0;
				   while(1)
				   {
						itoa(j,number_string,10);
						strcpy(autofilename_buffer,"");
						strcpy(autofilename_buffer,filename_buffer);
						strcat(autofilename_buffer,"\\\\");
						//strcat(autofilename_buffer,output_image_names);
						strcat(autofilename_buffer,number_string);
						strcat(autofilename_buffer,".");
						strcat(autofilename_buffer,sequential_image[sequential_extension_type].filename_ext); 
						image_data_file=fopen(autofilename_buffer,"rb");
						if(image_data_file!=NULL)
						{
							Write_Sequential_Image_File(output_data_file, image_data_file, sequential_filesize[j]);
							sequence_counter++;
							fclose(image_data_file);
						}
						else
						{   /*	Sequence complete	*/
			        			if(j>0) break;
						}
								j++;
					}
					fprintf(output_data_file,"\n};\n");
		  }/*End of Animated image processor block */
      #endif

      
		  if(( failure_flag == MMI_TRUE) && (staticFlag))
		  { 
       		/*	Currently assuming that all single files are BMP	*/
			if(nImageSize == 0)
			{
				image_identifier = IMAGE_TYPE_INVALID; 
			}
			else
			{
			 
				image_identifier = ExtractExtension(filename_buffer);
			}
			/* To be changed later for multiple file types*/
			*pType = ExtractExtension(filename_buffer);
       	     
			 /*	Load the data	*/
			
			/*Construct the data structure name	*/
       		strcpy(autoimagename_buffer,output_image_names);
			strcat(autoimagename_buffer,OUTPUT_IMAGE_NAMES_END);
			if ( 	image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP || 
				image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
				image_identifier == IMAGE_TYPE_PNG) 
			{
				fprintf(output_data_file,"__align(4) const U8 ");
			}
			else
				fprintf(output_data_file,"const U8 ");
			fprintf(output_data_file,autoimagename_buffer);
		
			
			fseek(image_data_file,0,SEEK_END);

			if ( 	image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP || 
				image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
				image_identifier == IMAGE_TYPE_PNG) 
			{
				file_size1 = file_size1 + ftell(image_data_file) + 8;
			}
			else
			file_size1 = file_size1 + ftell(image_data_file) + 6;
			fseek(image_data_file,0,SEEK_SET);
        
			pBuffer = (U8*) malloc(file_size1);
			if( pBuffer == NULL)
			{
				/* Add Debug Message*/
			}
			pBuffer1 = pBuffer;
			Load_Image_Data(&image_offset,output_data_file,image_data_file,pBuffer1,
											image_identifier,0);
			free(pBuffer);
			fprintf(output_data_file,"\n };\n");
			fclose(image_data_file);
		  }
      }      

	 
	if(failure_flag == MMI_FALSE) 
	{
		//printf("*************failed !!!********************\n");
		for(j = 0; j < SUPPORT_N_MEDIA_TYPE; j++){
			S32 l;

			for(l = 0; l < 3; l++){
				filename_buffer[filenameLen-1-l] = support_media_ext[j][2-l];
			}
			//printf("%s__%s\n", support_media_ext[j], filename_buffer);

			image_data_file = fopen(filename_buffer,"rb");
			if(image_data_file!=NULL){
				break;
			}
		}

		if(image_data_file!=NULL){
			failure_flag = MMI_TRUE;
			//printf("got %s \n", filename_buffer);

			fseek(image_data_file,0,SEEK_END);
			nImageSize = ftell(image_data_file);
			fseek(image_data_file,0,SEEK_SET);

       		/*	Currently assuming that all single files are BMP	*/
			if(nImageSize == 0)
			{
				image_identifier = IMAGE_TYPE_INVALID; 
			}
			else
			{
			 
				image_identifier = ExtractExtension(filename_buffer);
			}
			/* To be changed later for multiple file types*/
			*pType = ExtractExtension(filename_buffer);
       	     
			 /*	Load the data	*/
			
			/*Construct the data structure name	*/
       		strcpy(autoimagename_buffer,output_image_names);
			strcat(autoimagename_buffer,OUTPUT_IMAGE_NAMES_END);
			
			if ( 	image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP || 
				image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
				image_identifier == IMAGE_TYPE_PNG) 
			{
				fprintf(output_data_file,"__align(4) const U8 ");
			}
			else
				fprintf(output_data_file,"const U8 ");
			fprintf(output_data_file,autoimagename_buffer);
		
			
			fseek(image_data_file,0,SEEK_END);

			if ( 	image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP || 
				image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
				image_identifier == IMAGE_TYPE_PNG) 
			{
				file_size1 = file_size1 + ftell(image_data_file) + 8;
			}
			else
			file_size1 = file_size1 + ftell(image_data_file) + 6;
			fseek(image_data_file,0,SEEK_SET);
        
			pBuffer = (U8*) malloc(file_size1);
			if( pBuffer == NULL)
			{
				/* Add Debug Message*/
			}
			pBuffer1 = pBuffer;
			Load_Image_Data(&image_offset,output_data_file,image_data_file,pBuffer1,
											image_identifier,0);
			free(pBuffer);
			fprintf(output_data_file,"\n };\n");
			fclose(image_data_file);
		  }
      }      
	 
	
      if(image_data_file!=NULL) 
		  fclose(image_data_file);
	if ( output_data_file!=NULL ) 
   fclose(output_data_file);
	return failure_flag;
}


/**************************************************************

	FUNCTION NAME		: ClearHWIMageFile

  	PURPOSE				: Clear image file

	INPUT PARAMETERS	: void
	  

	OUTPUT PARAMETERS	: nil


	RETURNS				: void

 

**************************************************************/

void ClearHWIMageFile()
{
	FILE* fp;
	
	fp=fopen(OUTPUT_DATA_FILENAME,"w+");
	if(fp)
	{
		fprintf (fp,"#if ( defined (MMI_ON_WIN32) || defined (__AMNT__) )\n\t#define __align(x)\n#endif\n");
		fclose(fp);
	}

	 
	fp=fopen(OUTPUT_DATA_FILENAME_EXT,"w+"); 
	 
	if(fp)
	{
		fprintf (fp,"#if ( defined (MMI_ON_WIN32) || defined (__AMNT__) )\n\t#define __align(x)\n#endif\n");
		fclose(fp);
	}
}
#endif


#if defined(CUSTOMIZATION_BUILD_FIRST_PASS) || defined(CUSTOMIZATION_BUILD_SECOND_PASS)
CURR_SCREEN_RESOURCE	gsCurrScreenInfo;
extern CUSTOM_MENU  nCustMenus[MAX_MENU_ITEMS];
extern U16 CurrMaxMenuItemIndex;

/**************************************************************

	FUNCTION NAME		: SetCurrentScreenStrings

  	PURPOSE				: SEt the current screen strings

	INPUT PARAMETERS	: U16 nNum,...
	  

	OUTPUT PARAMETERS	: nil


	RETURNS				: void

 

**************************************************************/

void SetCurrentScreenStrings(U16 nNum,...)
{
	U16	nId;
	U16	nCount	=	0;
	va_list vlist;
	va_start(vlist,nNum);
	
	gsCurrScreenInfo.nNoOfStrings = nNum;

	while(nNum>nCount)
	{
		nId = va_arg(vlist,U16);
		gsCurrScreenInfo.nStringIdList[nCount] = nId;
		++nCount;
	}
	va_end(vlist);
	return;
}

/**************************************************************

	FUNCTION NAME		: SetCurrentScreenImages

  	PURPOSE				: SEt the current screen images

	INPUT PARAMETERS	: U16 nNum,...
	  

	OUTPUT PARAMETERS	: nil


	RETURNS				: void

 

**************************************************************/

void SetCurrentScreenImages(U16 nNum,...)
{
	U16	nId;
	U16	nCount	=	0;
	va_list vlist;
	va_start(vlist,nNum);
	
	gsCurrScreenInfo.nNoOfImages = nNum;

	while(nNum>nCount)
	{
		nId = va_arg(vlist,U16);
		gsCurrScreenInfo.nImageIdList[nCount] = nId;
		++nCount;
	}
	va_end(vlist);
	return;
}

/**************************************************************

	FUNCTION NAME		: AppendCurrentScreenImages

  	PURPOSE				: SEt the current screen images

	INPUT PARAMETERS	: U16 nNum,U16* pList
	  

	OUTPUT PARAMETERS	: nil


	RETURNS				: void

 

**************************************************************/

void AppendCurrentScreenImages(int nNum,U16* pList)
{
	U16	nCount	=	0;
	U16 i = 0;
	U8 flag = 0;
	U16 nOrgCount = gsCurrScreenInfo.nNoOfImages;
	gsCurrScreenInfo.nNoOfImages = nNum+gsCurrScreenInfo.nNoOfImages;

	while(nNum>nCount)
	{
		flag = 0;
		//for(i = nOrgCount+1;i<(nOrgCount+nCount ) ;i++)
		//for(i = (nOrgCount+nCount);i>0 ;i--)
		for(i = 0;i<nCount ;i++)
		{
			if(gsCurrScreenInfo.nImageIdList[i+nOrgCount] == pList[nCount])
			{
				flag = 1;
				break;
			}
		}

		if(flag == 0)
		gsCurrScreenInfo.nImageIdList[nOrgCount+nCount] = pList[nCount];
		
		++nCount;
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: AppendCurrentScreenStrings

  	PURPOSE				: SEt the current screen images

	INPUT PARAMETERS	: U16 nNum,U16* pList
	  

	OUTPUT PARAMETERS	: nil


	RETURNS				: void

 

**************************************************************/

void AppendCurrentScreenStrings(int nNum,U16* pList)
{
	U16	nCount	=	0;
	U16 nOrgCount = gsCurrScreenInfo.nNoOfStrings;
	gsCurrScreenInfo.nNoOfStrings = nNum+gsCurrScreenInfo.nNoOfStrings;

	while(nNum>nCount)
	{
		gsCurrScreenInfo.nStringIdList[nOrgCount+nCount] = pList[nCount];
		++nCount;
	}
	return;
}

/**************************************************************

	FUNCTION NAME		: SetCurrentScreenMenus

  	PURPOSE				: SEt the current screen menus

	INPUT PARAMETERS	: U16 nNum,...
	  

	OUTPUT PARAMETERS	: nil


	RETURNS				: void

 

**************************************************************/

void SetCurrentScreenMenus(U16 nNum,...)
{
	U16	nId;
	U16	nCount	=	0;
	U16	nParentIdCount	=	0;
	va_list vlist;
	va_start(vlist,nNum);
	
	gsCurrScreenInfo.nNoOfMenus = nNum;

	while(nNum>nCount)
	{
		nId = va_arg(vlist,U16);
		gsCurrScreenInfo.sMenuInfo[nCount].nId = nId;
		nParentIdCount = 0;
		while(nParentIdCount > CurrMaxMenuItemIndex)
		{
			if(nCustMenus[nParentIdCount].nMenuItemId == nId)
			{
				gsCurrScreenInfo.sMenuInfo[nCount].nParentId = nCustMenus[nParentIdCount].nParentId;
				break;
			}
			++nParentIdCount;
		}
		++nCount;
	}
	va_end(vlist);
	return;
}


#endif

