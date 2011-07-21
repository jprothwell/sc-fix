/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
 *	mtk_resgenerator.cpp
 *
 * Project:
 * --------
 *
 * Description:
 * ------------
 *   To Generator Resource C files.
 *
 * Author:
 * -------
 * -------
 *
*==============================================================================
 * 				HISTORY
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 *============================================================================== *******************************************************************************/

#include "cs_types.h"
#include "stdc.h"
#include <stdio.h>
#include <stdlib.h>
#include "mmi_data_types.h"
#include "custdatares.h"
#include "custmenures.h"
#include "custresdef.h"
#include "mmi_features.h"

#define RES_SECTION_ALONG
#ifdef TGT_GALLITE_G800
#undef RES_SECTION_ALONG
#endif

/* Extern variable */
extern "C" 	CUSTOM_IMAGE                nCustImageNames[];
extern "C" 	CUSTOM_IMAGE_MAP            ImageIdMap[];
extern "C" 	CUSTOM_IMAGE_SEARCH_MAP     ImageIdSearchMap[];
extern "C" 	IMAGENAME_LIST              ImageNameList[];
extern "C" 	CUSTOM_IMAGE_MAP            ImageIdMapEXT[];
extern "C" 	CUSTOM_IMAGE_SEARCH_MAP     ImageIdSearchMapEXT[];
extern "C" 	IMAGENAME_LIST              ImageNameListEXT[];

extern "C" 	CUSTOM_AUDIO                nCustAudioNames[];
extern "C" 	CUSTOM_AUDIO_MAP            AudioIdMap[];
extern "C" 	CUSTOM_AUDIO_SEARCH_MAP     AudioIdSearchMap[];
extern "C" 	AUDIONAME_LIST              AudioNameList[];
extern "C" 	CUSTOM_AUDIO_MAP            AudioIdMapEXT[];
extern "C" 	CUSTOM_AUDIO_SEARCH_MAP     AudioIdSearchMapEXT[];
extern "C" 	AUDIONAME_LIST              AudioNameListEXT[];

extern "C" 	CUSTOM_MENU                 nCustMenus[];
extern "C" 	unsigned short              CurrMaxMenuItemIndex; 	//Max menu Ids

extern "C" 	unsigned short              CurrMaxImageId; 		//Max String Ids
extern "C" 	unsigned short              CurrMaxSearchImageId; 	//Max String Ids
extern "C" 	unsigned short              CurrMaxImageNum; 		//Max Number Ids
extern "C" 	unsigned short              CurrMaxImageIdEXT;
extern "C" 	unsigned short              CurrMaxSearchImageIdEXT; 	
extern "C" 	unsigned short              CurrMaxImageNumEXT; 		
extern "C"	image_type_enum             ExtImageFlag;

extern "C" 	unsigned short              CurrMaxAudioId; 		//Max String Ids
extern "C" 	unsigned short              CurrMaxSearchAudioId; 	//Max String Ids
extern "C" 	unsigned short              CurrMaxAudioNum; 		//Max Number Ids
extern "C" 	unsigned short              CurrMaxAudioIdEXT;
extern "C" 	unsigned short              CurrMaxSearchAudioIdEXT; 	
extern "C" 	unsigned short              CurrMaxAudioNumEXT; 		
extern "C"	BOOL                        ExtAudioFlag;

extern "C"	FILE*                       dest_file;
extern "C"	int                         toolFlag;
extern "C"	int                         resultFailFlag;

extern "C" unsigned short               CurrMaxENFBAssociatedIDNum;
extern "C" CUSTOM_ENFB_STR              ENFBAssociatedIDList[];

/* Extern functiosn */
extern "C" void InitUnicodeSupport(void);
extern "C" void PopulateResData(void);
extern "C" void Extract_Name(S8 *dest, S8* src, U32 res_type);
extern "C" void InitStrPopulate();
extern "C" void PopulateAudioResData(void);

MMI_BOOL GenerateENFBAssociatedIDFile(void)
{
	FILE *fp;
	CUSTOM_ENFB_STR *enfb;
	int i, j;

	/* open file */
	fp = fopen(ENFB_DATA_FILENAME, "w");
	if(fp == NULL)
	{
		return MMI_FALSE;
	}

	/* print copyright */
	fprintf (fp, "/**\n *	Copyright Notice\n *	?2008- 2009, coolsand Inc., 1489 43rd Ave. W.,\n *	Vancouver, B.C. V6M 4K8 Canada. All Rights Reserved.\n *  (It is illegal to remove this copyright notice from this software or any\n *  portion of it)\n */\n");
	fprintf(fp, "\n/************************************************************** \n FILENAME\t: CustENFBAssociatedMap.c \n PURPOSE\t: ENFB associated list file. \n REMARKS\t: nil \n AUTHOR\t\t: Customization Tool \n DATE\t\t: . \n **************************************************************/\n");

	/* print include file */
	fprintf(fp, "#include \"CustDataRes.h\"\n\n");

    /* dump extern variable */
	for (i = 1; i <= (int)CurrMaxENFBAssociatedIDNum; i++)
	{
		enfb = (CUSTOM_ENFB_STR *)&ENFBAssociatedIDList[i];
		for (j = 0; j < (int)enfb->count; j++)
		{
            fprintf(fp, "extern const U8 %s[];\n", enfb->Ptr[j]);
		}
	}
	fprintf(fp, "\n");

    /* print list count */
#ifdef __EXE_DLL__
    fprintf(fp, "#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
#endif
    fprintf(fp, "const U32 CurrMaxENFBAssociatedIDNum = %d;\n\n", CurrMaxENFBAssociatedIDNum + 1);

	/* dump ID list */
	for (i = 1; i <= (int)CurrMaxENFBAssociatedIDNum; i++)
	{
		enfb = (CUSTOM_ENFB_STR *)&ENFBAssociatedIDList[i];
		fprintf(fp, "const U16 Associated_IDs_%d[] = {", i);
		for (j = 0; j < (int)enfb->count - 1; j++)
		{
			fprintf(fp, "%d, ", enfb->IDs[j]);
		}
		fprintf(fp, "%d};\n", enfb->IDs[enfb->count - 1]);
	}
	fprintf(fp, "\n");

	/* dump ptr list */
	for (i = 1; i <= (int)CurrMaxENFBAssociatedIDNum; i++)
	{
		enfb = (CUSTOM_ENFB_STR *)&ENFBAssociatedIDList[i];
		fprintf(fp, "const CUSTOM_IMAGE Associated_Ptr_%d[] = {\n", i);
		for (j = 0; j < (int)enfb->count - 1; j++)
		{
			fprintf(fp, "\t(U8*)&%s,\n", enfb->Ptr[j]);
		}
		fprintf(fp, "\t(U8*)&%s\n};\n", enfb->Ptr[enfb->count - 1]);
	}
	fprintf(fp, "\n");

    /* dump CUSTOM_ENFB list */
#ifdef __EXE_DLL__
    fprintf(fp, "#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
#endif
    fprintf(fp, "const CUSTOM_ENFB AssociatedID_map[] = \n{\n");
    if (CurrMaxENFBAssociatedIDNum > 0)
    {
        fprintf(fp, "\t{0, 0, 0},\n");
    }
    else
    {
        fprintf(fp, "\t{0, 0, 0}\n");
    }
    if (CurrMaxENFBAssociatedIDNum > 0)
    {
        for (i = 1; i < (int)CurrMaxENFBAssociatedIDNum; i++)
        {
            enfb = (CUSTOM_ENFB_STR *)&ENFBAssociatedIDList[i];
            fprintf(fp, "\t{%d, (U16 *)&Associated_IDs_%d, (CUSTOM_IMAGE *)&Associated_Ptr_%d},\n", enfb->count, i, i);
        }
        fprintf(fp, "\t{%d, (U16 *)&Associated_IDs_%d, (CUSTOM_IMAGE *)&Associated_Ptr_%d}\n",
                ENFBAssociatedIDList[CurrMaxENFBAssociatedIDNum].count,
                CurrMaxENFBAssociatedIDNum, CurrMaxENFBAssociatedIDNum);
    }
	fprintf(fp, "};\n");

	/* free memory */
	for (i = 1; i <= (int)CurrMaxENFBAssociatedIDNum; i++)
	{
		enfb = (CUSTOM_ENFB_STR *)&ENFBAssociatedIDList[i];
		free(enfb->IDs);
		for (j = 0; j < (int)enfb->count; j++)
		{
			free(enfb->Ptr[j]);
		}
		free(enfb->Ptr);
	}

    printf("Output CustENFBImgMap.c... done\n");

	fclose(fp);
	return MMI_TRUE;
}

void UnzipImage()
{
	char tempCommand[128];
	#if 0
	sprintf(tempCommand,"rd /S/Q %s\\mainlcd\\", CUST_IMG_PATH);
	system(tempCommand);
	
	sprintf(tempCommand,"rd /S/Q %s\\sublcd\\", CUST_IMG_PATH);
	system(tempCommand);
	#endif
#define USING_PACKAGE_GAMEIMAGES
#define CUST_GAME_IMG_PATH4ZIP			"../../mmi_customer/Images"
#if defined(USING_PACKAGE_GAMEIMAGES)
#ifndef __CYGWIN__	
	sprintf(tempCommand, "7za x -y %s/GameImages.zip -o%s/", CUST_GAME_IMG_PATH4ZIP, CUST_GAME_IMG_PATH4ZIP);
#else
	sprintf(tempCommand, "7za.exe x -y %s/GameImages.zip -o%s/", CUST_GAME_IMG_PATH4ZIP, CUST_GAME_IMG_PATH4ZIP);
#endif
	system(tempCommand);
#endif

#ifndef __CYGWIN__	
	sprintf(tempCommand, "7za x -y %s/image.zip -o%s/", CUST_IMG_PATH4ZIP, CUST_IMG_PATH4ZIP);
#else
	sprintf(tempCommand, "7za.exe x -y %s/image.zip -o%s/", CUST_IMG_PATH4ZIP, CUST_IMG_PATH4ZIP);
#endif
	system(tempCommand);


#if defined(CUST_SUBLCD_PATH)
#ifndef __CYGWIN__	
	sprintf(tempCommand, "7za x -y %s/image.zip -o%s/", CUST_SUBLCD_PATH, CUST_IMG_PATH);
#else
	sprintf(tempCommand, "7za.exe x -y %s/image.zip -o%s/",CUST_SUBLCD_PATH, CUST_IMG_PATH);
#endif
	system(tempCommand);
#endif /* defined (CUST_SUBLCD_PATH) */
}


void PopulateImageSearch_Ext(BOOL imgFlag)
{
	U16 i=0;
	U16 j=0;
	U16 index=0;
	U16 start=0;
	CUSTOM_IMAGE_MAP *pImageIdMap;
	CUSTOM_IMAGE_SEARCH_MAP *pImageIdSearchMap;
	unsigned short *pCurrMaxSearchImageId;
	unsigned short *pCurrMaxImageId;

	if(imgFlag == TRUE)
	{
		pImageIdMap = ImageIdMapEXT;
		pImageIdSearchMap = ImageIdSearchMapEXT;
		pCurrMaxSearchImageId = &CurrMaxSearchImageIdEXT;
		pCurrMaxImageId = &CurrMaxImageIdEXT;
	}
	else
	{
		pImageIdMap = ImageIdMap;
		pImageIdSearchMap = ImageIdSearchMap;
		pCurrMaxSearchImageId = &CurrMaxSearchImageId;
		pCurrMaxImageId = &CurrMaxImageId;
	}

	for (i=0;i<MAX_IMAGE_IDS_SIZE;i++) {
		if (pImageIdMap[i].nImageNum!=(U16)-1)
		{
			if (!start)
			{
				pImageIdSearchMap[j].minImageId=pImageIdSearchMap[j].maxImageId=i;
				pImageIdSearchMap[j].index=index;
				index++;
				start=1;
			}
			else 
			{
				pImageIdSearchMap[j].maxImageId=i;
				index++;
			}
		}
		else	
		{
			if (start) 
			{
				j++;
				start=0;
			}
		}
	}
	
	*pCurrMaxSearchImageId=j;
	j=0;
	for (i=0;i<MAX_IMAGE_IDS_SIZE;i++) 
	{
		if ((pImageIdMap[i].nImageNum!=(U16)-1)&&(i!=j))
		{
			pImageIdMap[j].nImageNum=pImageIdMap[i].nImageNum;
			pImageIdMap[j].nImageId=pImageIdMap[i].nImageId;

			j++;
			pImageIdMap[i].nImageNum=-1;
		}
	}
	*pCurrMaxImageId=j;
}

void PopulateImageSearch()
{
	PopulateImageSearch_Ext(TRUE);//for newly separated set of images, such as power on animation
	PopulateImageSearch_Ext(FALSE);//for original set of images
}


void PopulateAudioSearch_Ext(BOOL adoFlag)
{
	U16 i=0;
	U16 j=0;
	U16 index=0;
	U16 start=0;
	CUSTOM_AUDIO_MAP *pAudioIdMap;
	CUSTOM_AUDIO_SEARCH_MAP *pAudioIdSearchMap;
	unsigned short *pCurrMaxSearchAudioId;
	unsigned short *pCurrMaxAudioId;

	if(adoFlag == TRUE)
	{
		pAudioIdMap = AudioIdMapEXT;
		pAudioIdSearchMap = AudioIdSearchMapEXT;
		pCurrMaxSearchAudioId = &CurrMaxSearchAudioIdEXT;
		pCurrMaxAudioId = &CurrMaxAudioIdEXT;
	}
	else
	{
		pAudioIdMap = AudioIdMap;
		pAudioIdSearchMap = AudioIdSearchMap;
		pCurrMaxSearchAudioId = &CurrMaxSearchAudioId;
		pCurrMaxAudioId = &CurrMaxAudioId;
	}

	for (i=0;i<MAX_AUDIO_IDS_SIZE;i++) 
	{
		if (pAudioIdMap[i].nAudioNum!=(U16)-1)
		{
			if (!start) 
			{
				pAudioIdSearchMap[j].minAudioId=pAudioIdSearchMap[j].maxAudioId=i;
				pAudioIdSearchMap[j].index=index;
				index++;
				start=1;
			}
			else 
			{
				pAudioIdSearchMap[j].maxAudioId=i;
				index++;
			}
		}
		else	
		{
			if (start) 
			{
				j++;
				start=0;
			}
		}
	}
	
	*pCurrMaxSearchAudioId=j;
	j=0;
	for (i=0;i<MAX_AUDIO_IDS_SIZE;i++) 
	{
		if ((pAudioIdMap[i].nAudioNum!=(U16)-1)&&(i!=j))
		{
			pAudioIdMap[j].nAudioNum=pAudioIdMap[i].nAudioNum;
			pAudioIdMap[j].nAudioId=pAudioIdMap[i].nAudioId;

			j++;
			pAudioIdMap[i].nAudioNum=-1;
		}
	}
	*pCurrMaxAudioId=j;
}

void PopulateAudioSearch()
{
	PopulateAudioSearch_Ext(TRUE);//for newly separated set of images, such as power on animation
	PopulateAudioSearch_Ext(FALSE);//for original set of images
}

extern "C" void Trace(char *fmt,...)
{
	va_list MyList;
	char nPrintableStr[1000];//Assumed..this can chane if need to dump more bytes

	va_start(MyList,fmt);
	vsprintf(nPrintableStr,fmt,MyList);
	va_end(MyList);
	printf(nPrintableStr);
}


int GenerateImageResFile_Ext(BOOL imgFlag)
{
	FILE	*pFile;
	S8 pBuffName[200];
	int i=0;
	char Suffix[3] = "\",";
	char Prefix[MAX_FILENAME_LEN+4] = "\"";
	IMAGENAME_LIST*	pImageName;
	U16 nSize;
	char *str_CustImgDataHW;
	char *str_mtk_CurrMaxImageNum;
	char *str_CurrMaxImageNum;
	char *str_mtk_nCustImageNames;
	char *str_nCustImageNames;
	char *str_ImageNameList;
	char *str_mark;
	char *curr_img_data_filename;
	char *str_CustGameData;
	
	if(imgFlag == TRUE){
		ExtImageFlag = EXT_IMAGE;
		pImageName = (IMAGENAME_LIST*)ImageNameListEXT;
		nSize = CurrMaxImageNumEXT;
		str_CustImgDataHW = "custimgdatahwext.h";
		str_mtk_CurrMaxImageNum = "CSD_CurrMaxImageNumEXT";
		str_CurrMaxImageNum = "CurrMaxImageNumEXT";
		str_mtk_nCustImageNames = "CSD_nCustImageNamesEXT";
		str_nCustImageNames = "nCustImageNamesEXT";
		str_ImageNameList = "ImageNameListEXT";
		str_mark = "//";
		curr_img_data_filename = IMG_DATA_FILENAME_EXT;
	}
	else{
		ExtImageFlag = SYSTEM_IMAGE;
		pImageName = (IMAGENAME_LIST*)ImageNameList;
		nSize = CurrMaxImageNum;
		str_CustImgDataHW = "custimgdatahw.h";
		str_mtk_CurrMaxImageNum = "CSD_CurrMaxImageNum";
		str_CurrMaxImageNum = "CurrMaxImageNum";
		str_mtk_nCustImageNames = "CSD_nCustImageNames";
		str_nCustImageNames = "nCustImageNames";
		str_ImageNameList = "ImageNameList";
		str_mark = "";
		curr_img_data_filename = IMG_DATA_FILENAME;
	}
	
	if(pFile = fopen(curr_img_data_filename,"w"))
	{
		fputs ("/**\n *	Copyright Notice\n *	?2008 - 2009, Coolsand Inc., 1489 43rd Ave. W.,\n *	Vancouver, B.C. V6M 4K8 Canada. All Rights Reserved.\n *  (It is illegal to remove this copyright notice from this software or any\n *  portion of it)\n */\n",pFile);
		fputs ("\n/************************************************************** \n FILENAME	: CustImageRes.c \n PURPOSE		: Image Resource file. \n REMARKS		: nil \n AUTHOR		: Customization Tool \n DATE		: . \n **************************************************************/\n",pFile);

		fprintf(pFile, "#include \"custdatares.h\"\n");
		//if ( imgFlag == FALSE )
		//fprintf ( pFile, "#include \"custgamedatahw.h\"\n");
		fprintf(pFile, "#include \"%s\"\n#include \"custresdef.h\"\n\n%s",str_CustImgDataHW, str_mark);

#ifdef __EXE_DLL__
		fprintf(pFile,"#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
#endif
#ifdef __MULTI_BIN_LOAD__
		//fprintf(pFile,"const unsigned short  mtk_CurrMaxImageNum=%d;\n",nSize);
			#ifdef RES_SECTION_ALONG
			fprintf(pFile,"const unsigned short  %s=%d;\n#ifndef MMI_ON_WIN32\n  __attribute__((section (\".roresdata\"))) \n#endif\n\n",str_mtk_CurrMaxImageNum, nSize);
			#else
			fprintf(pFile,"const unsigned short  %s=%d;\n",str_mtk_CurrMaxImageNum, nSize);
			#endif			
#else
		//fprintf(pFile,"unsigned short  CurrMaxImageNum=%d;\n",nSize);
		#ifdef RES_SECTION_ALONG
		fprintf(pFile, "unsigned short  %s=%d;\n\n#ifndef MMI_ON_WIN32\n  __attribute__((section (\".roresdata\"))) \n#endif\n \n",str_CurrMaxImageNum, nSize);
		#else
		fprintf(pFile,"unsigned short  %s=%d;\n",str_CurrMaxImageNum, nSize);
		#endif
#endif

		//Writing the Image filename inside the file.
		if(nSize)
		{
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
#ifdef __MULTI_BIN_LOAD__
			//fputs("const CUSTOM_IMAGE	mtk_nCustImageNames[]={\n",pFile);
			fprintf(pFile, "const CUSTOM_IMAGE	%s[]={\n",str_mtk_nCustImageNames);
#else
			//fputs("const CUSTOM_IMAGE	nCustImageNames[]={\n",pFile);
			fprintf(pFile, "const CUSTOM_IMAGE	%s[]={\n",str_nCustImageNames);
#endif

			for(i=0;i<nSize;i++)
			{
				Extract_Name (pBuffName,pImageName[i].filename, RES_IMAGE);
				fprintf (pFile,"{(U8*)&%s},\n",pBuffName);

			}

			fputs("};\n\n\n\n",pFile);
		}
		else
		{
#ifdef __MULTI_BIN_LOAD__
			fprintf(pFile, "const CUSTOM_IMAGE	%s[]={{0}};\n\n\n\n",str_mtk_nCustImageNames);
#else
			fprintf(pFile, "const CUSTOM_IMAGE	%s[]={{0}};\n\n\n\n",str_nCustImageNames);
#endif
			//fputs("#ifdef MMI_ON_HARDWARE_P\nconst CUSTOM_IMAGE	nCustImageNames[];\n#else\nCUSTOM_IMAGE	nCustImageNames[MAX_IMAGE_NAMES_SIZE];\n#endif\n",pFile);
			//fprintf(pFile,"#ifdef MMI_ON_HARDWARE_P\nconst CUSTOM_IMAGE	%s[];\n#else\nCUSTOM_IMAGE	%s[MAX_IMAGE_NAMES_SIZE];\n#endif\n",str_nCustImageNames,str_nCustImageNames);
			//fputs("#if defined(DEVELOPER_BUILD_FIRST_PASS) || defined(CUSTOMIZATION_BUILD_FIRST_PASS)|| defined(CUSTOMIZATION_BUILD_SECOND_PASS) \n\n",pFile);

			//fputs("IMAGENAME_LIST	ImageNameList[MAX_IMAGE_NAMES_SIZE];",pFile);
			//fprintf(pFile,"IMAGENAME_LIST	%s[MAX_IMAGE_NAMES_SIZE];",str_ImageNameList);
			//fputs("\n#endif\n",pFile);
		}

		if(imgFlag == TRUE){
			#ifdef __EXE_DLL__
				fprintf(pFile,"#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
			#endif
			#ifdef __MULTI_BIN_LOAD__
				fprintf(pFile,"const CUSTPACK_IMAGE_HEADER CSD_image_header={%d, (CUSTOM_IMAGE *)%s};\n\n\n\n", nSize, str_mtk_nCustImageNames);
			#else
				fprintf(pFile,"const CUSTPACK_IMAGE_HEADER CSD_image_header={%d, (CUSTOM_IMAGE *)%s};\n\n\n\n", nSize, str_nCustImageNames);
			#endif
		}
		
		fclose(pFile);
	}
	else
	{
		return 0;
	}

	return 1;
}

/**************************************************************

	FUNCTION NAME		: GenerateImageResFile	

	PURPOSE				: It generates the image resource file. 
							
	INPUT PARAMETERS	: NONE

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: The image resource file is generated with the list of values 
							passed
**************************************************************/
void GenerateImageResFile(void)
{
	GenerateImageResFile_Ext(TRUE);
	GenerateImageResFile_Ext(FALSE);
}


int GenerateAudioResFile_Ext(BOOL adoFlag)
{
	FILE	*pFile;
	S8 pBuffName[200];
	int i=0;
	char Suffix[3] = "\",";
	char Prefix[MAX_FILENAME_LEN+4] = "\"";
	AUDIONAME_LIST*	pAudioName;
	U16 nSize;
	char *str_CustAdoDataHW;
	char *str_mtk_CurrMaxAudioNum;
	char *str_CurrMaxAudioNum;
	char *str_mtk_nCustAudioNames;
	char *str_nCustAudioNames;
	char *str_AudioNameList;
	char *str_mark;
	char *curr_ado_data_filename;
	
	if(adoFlag == TRUE){
		ExtAudioFlag = TRUE;
		pAudioName = (AUDIONAME_LIST*)AudioNameListEXT;
		nSize = CurrMaxAudioNumEXT;
		str_CustAdoDataHW = "CustAdoDataHWExt.h";
		str_mtk_CurrMaxAudioNum = "CSD_CurrMaxAudioNumEXT";
		str_CurrMaxAudioNum = "CurrMaxAudioNumEXT";
		str_mtk_nCustAudioNames = "CSD_nCustAudioNamesEXT";
		str_nCustAudioNames = "nCustAudioNamesEXT";
		str_AudioNameList = "AudioNameListEXT";
		str_mark = "//";
		curr_ado_data_filename = ADO_DATA_FILENAME_EXT;
	}
	else{
		ExtAudioFlag = FALSE;
		pAudioName = (AUDIONAME_LIST*)AudioNameList;
		nSize = CurrMaxAudioNum;
		str_CustAdoDataHW = "CustAdoDataHW.h";
		str_mtk_CurrMaxAudioNum = "CSD_CurrMaxAudioNum";
		str_CurrMaxAudioNum = "CurrMaxAudioNum";
		str_mtk_nCustAudioNames = "CSD_nCustAudioNames";
		str_nCustAudioNames = "nCustAudioNames";
		str_AudioNameList = "AudioNameList";
		str_mark = "";
		curr_ado_data_filename = ADO_DATA_FILENAME;
	}
	
	if(pFile = fopen(curr_ado_data_filename,"w"))
	{
		fputs ("/**\n *	Copyright Notice\n *	?2008- 2009, coolsand Inc., 1489 43rd Ave. W.,\n *	Vancouver, B.C. V6M 4K8 Canada. All Rights Reserved.\n *  (It is illegal to remove this copyright notice from this software or any\n *  portion of it)\n */\n",pFile);
		fputs ("\n/************************************************************** \n FILENAME	: CustAudioRes.c \n PURPOSE		: Audio Resource file. \n REMARKS		: nil \n AUTHOR		: Customization Tool \n DATE		: . \n **************************************************************/\n",pFile);

		fprintf(pFile, "#include \"custdatares.h\"\n#include \"%s\"\n#include \"custresdef.h\"\n\n%sconst S8 CustAdoPath[]=CUST_ADO_PATH;\n",str_CustAdoDataHW, str_mark);
#ifdef __EXE_DLL__
		fprintf(pFile,"#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
#endif
#ifdef __MULTI_BIN_LOAD__
		fprintf(pFile,"const unsigned short  %s=%d;\n",str_mtk_CurrMaxAudioNum, nSize);
#else
		fprintf(pFile,"unsigned short  %s=%d;\n",str_CurrMaxAudioNum, nSize);
#endif

		//Writing the Audio filename inside the file.
		if(nSize)
		{
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
#ifdef __MULTI_BIN_LOAD__
			fprintf(pFile, "const CUSTOM_AUDIO	%s[]={\n",str_mtk_nCustAudioNames);
#else
			fprintf(pFile, "const CUSTOM_AUDIO	%s[]={\n",str_nCustAudioNames);
#endif

			for(i=0;i<nSize;i++)
			{
				Extract_Name (pBuffName,pAudioName[i].filename, RES_AUDIO);
				fprintf (pFile,"(U8*)&%s,\n",pBuffName);

			}

			fputs("};\n\n\n\n",pFile);
		}
		else
		{
#ifdef __MULTI_BIN_LOAD__
			fprintf(pFile, "const CUSTOM_AUDIO	%s[];\n",str_mtk_nCustAudioNames);
#else
			fprintf(pFile, "const CUSTOM_AUDIO	%s[];\n",str_nCustAudioNames);
#endif
			//fprintf(pFile,"#ifdef MMI_ON_HARDWARE_P\nconst CUSTOM_AUDIO	%s[];\n#else\nCUSTOM_AUDIO	%s[MAX_AUDIO_NAMES_SIZE];\n#endif\n",str_nCustAudioNames,str_nCustAudioNames);
			//fputs("#if defined(DEVELOPER_BUILD_FIRST_PASS) || defined(CUSTOMIZATION_BUILD_FIRST_PASS)|| defined(CUSTOMIZATION_BUILD_SECOND_PASS) \n\n",pFile);

			//fprintf(pFile,"AUDIONAME_LIST	%s[MAX_AUDIO_NAMES_SIZE];",str_AudioNameList);
			//fputs("\n#endif\n",pFile);
		}

		if(adoFlag == TRUE){
			#ifdef __EXE_DLL__
				fprintf(pFile,"#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
			#endif
			#ifdef __MULTI_BIN_LOAD__
				fprintf(pFile,"const CUSTPACK_AUDIO_HEADER mtk_audio_header={%d, (CUSTOM_AUDIO *)%s};", nSize, str_mtk_nCustAudioNames);
			#else
				fprintf(pFile,"const CUSTPACK_AUDIO_HEADER mtk_audio_header={%d, (CUSTOM_AUDIO *)%s};", nSize, str_nCustAudioNames);
			#endif
		}
		
		fclose(pFile);
	}
	else
	{
		return 0;
	}

	return 1;
}

/**************************************************************

	FUNCTION NAME		: GenerateAudioResFile	

	PURPOSE				: It generates the audio resource file. 
							
	INPUT PARAMETERS	: NONE

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: The audio resource file is generated with the list of values 
							passed
**************************************************************/
void GenerateAudioResFile(void)
{
	GenerateAudioResFile_Ext(TRUE);
	GenerateAudioResFile_Ext(FALSE);
}


int GenerateImageMapFile_Ext(BOOL imgFlag)
{
	FILE	*pFile;
	CUSTOM_IMAGE_MAP*	pMap;
	int i;
	U16 nSize;
	char *str_CustImageMap;
	char *str_CurrMaxImageId;
	char *str_ImageIdMap;
	char *curr_img_map_filename;

	if(imgFlag == TRUE){
		ExtImageFlag = EXT_IMAGE;
		pMap = (CUSTOM_IMAGE_MAP*)ImageIdMapEXT;
		nSize = CurrMaxImageIdEXT;
		str_CustImageMap = "CustImageMapExt.c";
		str_CurrMaxImageId = "CurrMaxImageIdEXT";
		str_ImageIdMap = "ImageIdMapEXT";
		curr_img_map_filename = IMG_MAP_FILENAME_EXT;
	}
	else{
		ExtImageFlag = SYSTEM_IMAGE;
		pMap = (CUSTOM_IMAGE_MAP*)ImageIdMap;
		nSize = CurrMaxImageId;
		str_CustImageMap = "CustImageMap.c";
		str_CurrMaxImageId = "CurrMaxImageId";
		str_ImageIdMap = "ImageIdMap";
		curr_img_map_filename = IMG_MAP_FILENAME;
	}

	if(pFile = fopen(curr_img_map_filename,"w"))
	{
		fputs ("/**\n *	Copyright Notice\n *	?2008- 2009, coolsand Inc., 1489 43rd Ave. W.,\n *	Vancouver, B.C. V6M 4K8 Canada. All Rights Reserved.\n *  (It is illegal to remove this copyright notice from this software or any\n *  portion of it)\n */\n",pFile);
		//fputs ("\n/************************************************************** \n FILENAME	: CustImageMap.c \n PURPOSE		: Image Map file. \n REMARKS		: nil \n AUTHOR		: Customization Tool \n DATE		: . \n **************************************************************/\n",pFile);
		fprintf (pFile, "\n/************************************************************** \n FILENAME	: %s \n PURPOSE		: Image Map file. \n REMARKS		: nil \n AUTHOR		: Customization Tool \n DATE		: . \n **************************************************************/\n",str_CustImageMap);

		fputs("#include \"custdatares.h\"\n\n\n",pFile);
		//for Lisen change
#ifdef __EXE_DLL__
		fprintf(pFile,"#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
#endif
		//fprintf(pFile,"unsigned short CurrMaxImageId=%d;\n",nSize);
		fprintf(pFile,"unsigned short %s=%d;\n",str_CurrMaxImageId, nSize);
		
		//Writing the Image Map Array inside the file.

		if(nSize)
		{
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			//fputs("const CUSTOM_IMAGE_MAP ImageIdMap[]={\n",pFile);
			fprintf(pFile, "const CUSTOM_IMAGE_MAP %s[]={\n",str_ImageIdMap, pFile);
			for( i=0;i<nSize;i++)
			{
				fprintf(pFile,"\t{%d,%d},\n",pMap[i].nImageId,pMap[i].nImageNum);
			}
			fputs("};\n\n\n\n",pFile);
		}
		else
		{
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			//fputs("const CUSTOM_IMAGE_MAP ImageIdMap[];\n",pFile);
			fprintf(pFile, "const CUSTOM_IMAGE_MAP %s[]={{0,0},};\n\n\n\n",str_ImageIdMap, pFile);
		}
		fclose(pFile);
	}
	else
	{
		return 0;
	}
	return 1;
}

/**************************************************************

	FUNCTION NAME		: GenerateImageMapFile	

	PURPOSE				: It generates the string map resource file. 
							
	INPUT PARAMETERS	: NONE

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: The image map resource file is generated with the list of values 
							passed
**************************************************************/
void GenerateImageMapFile(void)
{
	GenerateImageMapFile_Ext(TRUE);
	GenerateImageMapFile_Ext(FALSE);
}


int GenerateAudioMapFile_Ext(BOOL adoFlag)
{
	FILE	*pFile;
	CUSTOM_AUDIO_MAP*	pMap;
	int i;
	U16 nSize;
	char *str_CustAudioMap;
	char *str_CurrMaxAudioId;
	char *str_AudioIdMap;
	char *curr_ado_map_filename;

	if(adoFlag == TRUE){
		ExtAudioFlag = TRUE;
		pMap = (CUSTOM_AUDIO_MAP*)AudioIdMapEXT;
		nSize = CurrMaxAudioIdEXT;
		str_CustAudioMap = "CustAudioMapExt.c";
		str_CurrMaxAudioId = "CurrMaxAudioIdEXT";
		str_AudioIdMap = "AudioIdMapEXT";
		curr_ado_map_filename = ADO_MAP_FILENAME_EXT;
	}
	else{
		ExtAudioFlag = FALSE;
		pMap = (CUSTOM_AUDIO_MAP*)AudioIdMap;
		nSize = CurrMaxAudioId;
		str_CustAudioMap = "CustAudioMap.c";
		str_CurrMaxAudioId = "CurrMaxAudioId";
		str_AudioIdMap = "AudioIdMap";
		curr_ado_map_filename = ADO_MAP_FILENAME;
	}

	if(pFile = fopen(curr_ado_map_filename,"w"))
	{
		fputs ("/**\n *	Copyright Notice\n *	?2008- 2009, coolsand Inc., 1489 43rd Ave. W.,\n *	Vancouver, B.C. V6M 4K8 Canada. All Rights Reserved.\n *  (It is illegal to remove this copyright notice from this software or any\n *  portion of it)\n */\n",pFile);
		fprintf (pFile, "\n/************************************************************** \n FILENAME	: %s \n PURPOSE		: Audio Map file. \n REMARKS		: nil \n AUTHOR		: Customization Tool \n DATE		: . \n **************************************************************/\n",str_CustAudioMap);

		fputs("#include \"CustDataRes.h\"\n\n\n",pFile);
		//for Lisen change
#ifdef __EXE_DLL__
		fprintf(pFile,"#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
#endif
		fprintf(pFile,"unsigned short %s=%d;\n",str_CurrMaxAudioId, nSize);
		
		//Writing the Audio Map Array inside the file.

		if(nSize)
		{
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			fprintf(pFile, "const CUSTOM_AUDIO_MAP %s[]={\n",str_AudioIdMap, pFile);
			for( i=0;i<nSize;i++)
			{
				fprintf(pFile,"\t{%d,%d},\n",pMap[i].nAudioId,pMap[i].nAudioNum);
			}
			fputs("};\n\n\n\n",pFile);
		}
		else
		{
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			fprintf(pFile, "const CUSTOM_AUDIO_MAP %s[];\n",str_AudioIdMap, pFile);
		}
		fclose(pFile);
	}
	else
	{
		return 0;
	}
	return 1;
}

/**************************************************************

	FUNCTION NAME		: GenerateAudioMapFile	

	PURPOSE				: It generates the string map resource file. 
							
	INPUT PARAMETERS	: NONE

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: The audio map resource file is generated with the list of values 
							passed
**************************************************************/
void GenerateAudioMapFile(void)
{
	GenerateAudioMapFile_Ext(TRUE);
	GenerateAudioMapFile_Ext(FALSE);
}


int GenerateImageMapSearchFile_Ext(BOOL imgFlag)
{
	FILE	*pFile;
	CUSTOM_IMAGE_SEARCH_MAP*	pMap = (CUSTOM_IMAGE_SEARCH_MAP*)ImageIdSearchMap;
	int i;
	U16 nSize;
	char *str_CurrMaxSearchImageId;
	char *str_ImageIdSearchMap;
	char *curr_img_map_filename;

	if(imgFlag == TRUE){
		ExtImageFlag = EXT_IMAGE;
		pMap = (CUSTOM_IMAGE_SEARCH_MAP*)ImageIdSearchMapEXT;
		nSize = CurrMaxSearchImageIdEXT;
		str_CurrMaxSearchImageId = "CurrMaxSearchImageIdEXT";
		str_ImageIdSearchMap = "ImageIdSearchMapEXT";
		curr_img_map_filename = IMG_MAP_FILENAME_EXT;
	}
	else{
		ExtImageFlag = SYSTEM_IMAGE;
		pMap = (CUSTOM_IMAGE_SEARCH_MAP*)ImageIdSearchMap;
		nSize = CurrMaxSearchImageId;
		str_CurrMaxSearchImageId = "CurrMaxSearchImageId";
		str_ImageIdSearchMap = "ImageIdSearchMap";
		curr_img_map_filename = IMG_MAP_FILENAME;
	}

	
	if(pFile = fopen(curr_img_map_filename,"a"))
	{
#ifdef __EXE_DLL__
		fprintf(pFile,"#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
#endif
		//fprintf(pFile,"unsigned short CurrMaxSearchImageId=%d;\n",nSize);
		fprintf(pFile,"unsigned short %s=%d;\n",str_CurrMaxSearchImageId, nSize);
		//Writing the Image Map Array inside the file.
		if(nSize)
		{
#ifdef __EXE_DLL__
			fputs("#if  !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			//fputs("CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMap[]={\n",pFile);
			//fputs("const CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMap[]={\n",pFile);
			fprintf(pFile, "const CUSTOM_IMAGE_SEARCH_MAP %s[]={\n",str_ImageIdSearchMap);
			for( i=0;i<nSize;i++)
			{
				fprintf(pFile,"\t{%d,%d,%d},\n",pMap[i].minImageId,pMap[i].maxImageId,pMap[i].index);
			}
			fputs("};\n\n\n\n",pFile);
		}
		else
			//fputs("CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMap[MAX_IMAGE_IDS];",pFile);
			fprintf(pFile, "CUSTOM_IMAGE_SEARCH_MAP %s[]={{0,0,0},};\n\n\n\n",str_ImageIdSearchMap);
		fclose(pFile);
	}
	else
	{
		return 0;
	}
	return 1;
}

/**************************************************************

	FUNCTION NAME		: GenerateImageMapSearchFile	

	PURPOSE				: The imagemap search structures are appended to the image
							resource file.
							
							
	INPUT PARAMETERS	: LPVOID pList,UINT16 nSize

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: The data is appended to the existing image resource file. SO this function shd be called 
							after the image resource file is created.
**************************************************************/
void GenerateImageMapSearchFile(void)
{	
	GenerateImageMapSearchFile_Ext(TRUE);
	GenerateImageMapSearchFile_Ext(FALSE);
}


int GenerateAudioMapSearchFile_Ext(BOOL adoFlag)
{
	FILE	*pFile;
	CUSTOM_AUDIO_SEARCH_MAP*	pMap = (CUSTOM_AUDIO_SEARCH_MAP*)AudioIdSearchMap;
	int i;
	U16 nSize;
	char *str_CurrMaxSearchAudioId;
	char *str_AudioIdSearchMap;
	char *curr_ado_map_filename;

	if(adoFlag == TRUE){
		ExtAudioFlag = TRUE;
		pMap = (CUSTOM_AUDIO_SEARCH_MAP*)AudioIdSearchMapEXT;
		nSize = CurrMaxSearchAudioIdEXT;
		str_CurrMaxSearchAudioId = "CurrMaxSearchAudioIdEXT";
		str_AudioIdSearchMap = "AudioIdSearchMapEXT";
		curr_ado_map_filename = ADO_MAP_FILENAME_EXT;
	}
	else{
		ExtAudioFlag = FALSE;
		pMap = (CUSTOM_AUDIO_SEARCH_MAP*)AudioIdSearchMap;
		nSize = CurrMaxSearchAudioId;
		str_CurrMaxSearchAudioId = "CurrMaxSearchAudioId";
		str_AudioIdSearchMap = "AudioIdSearchMap";
		curr_ado_map_filename = ADO_MAP_FILENAME;
	}

	
	if(pFile = fopen(curr_ado_map_filename,"a"))
	{
#ifdef __EXE_DLL__
		fprintf(pFile,"#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n");
#endif
		fprintf(pFile,"unsigned short %s=%d;\n",str_CurrMaxSearchAudioId, nSize);
		//Writing the Audio Map Array inside the file.
		if(nSize)
		{
#ifdef __EXE_DLL__
			fputs("#if  !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			fprintf(pFile, "const CUSTOM_AUDIO_SEARCH_MAP %s[]={\n",str_AudioIdSearchMap);
			for( i=0;i<nSize;i++)
			{
				fprintf(pFile,"\t{%d,%d,%d},\n",pMap[i].minAudioId,pMap[i].maxAudioId,pMap[i].index);
			}
			fputs("};\n\n\n\n",pFile);
		}
		else
			fprintf(pFile, "CUSTOM_AUDIO_SEARCH_MAP %s[];",str_AudioIdSearchMap);
		fclose(pFile);
	}
	else
	{
		return 0;
	}
	return 1;
}

/**************************************************************

	FUNCTION NAME		: GenerateAudioMapSearchFile	

	PURPOSE				: The audiomap search structures are appended to the audio
							resource file.
							
							
	INPUT PARAMETERS	: LPVOID pList,UINT16 nSize

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: The data is appended to the existing audio resource file. SO this function shd be called 
							after the audio resource file is created.
**************************************************************/
void GenerateAudioMapSearchFile(void)
{	
	GenerateAudioMapSearchFile_Ext(TRUE);
	GenerateAudioMapSearchFile_Ext(FALSE);
}


/**************************************************************

	FUNCTION NAME		: GenerateMenuFile	

	PURPOSE				: The menu file is generated with the structure passed
							
	INPUT PARAMETERS	: LPVOID pList,UINT16 nSize

	OUTPUT PARAMETERS	: nil

	RETURNS				: ERR

	REMARKS				: The menu file is generated using the structure values passed.
**************************************************************/
int GenerateMenuFile(U16 nSize)
{
	FILE*		pFile;
	CUSTOM_MENU*	pCustMenu	= (CUSTOM_MENU*)nCustMenus;
	S8 nItemData[100]; /* Assumed ..*/
	S8 MenuItemString[500]; /* Assumed ..*/
	int i,j;

	if(pFile = fopen(MENU_DATA_FILENAME,"w"))
	{
		fputs ("/**\n *	Copyright Notice\n *	?2008 - 2009, coolsand Inc., 1489 43rd Ave. W.,\n *	Vancouver, B.C. V6M 4K8 Canada. All Rights Reserved.\n *  (It is illegal to remove this copyright notice from this software or any\n *  portion of it)\n */\n",pFile);
		fputs ("\n/************************************************************** \n FILENAME	: CustMenuRes.c \n PURPOSE		: Menu Resource file. \n REMARKS		: nil \n AUTHOR		: Customization Tool \n DATE		: . \n **************************************************************/\n",pFile);

		//Put the headers needed here
		fputs("#include \"custmenures.h\"\n\n\n",pFile);
//		fputs("#include \"GlobalMenuItems.h\"\n\n\n",pFile);
		fprintf(pFile,"unsigned short  CurrMaxMenuItemIndex=%d;\n",nSize);
		//Writing the String Array inside the file.			
		if(nSize)
		{
			
//			fputs("#ifdef MMI_ON_HARDWARE_P\n\n",pFile);
			for(i=0;i<nSize;i++)
			{
				if(pCustMenu[i].nNumofMenuItem)
				{
					sprintf(MenuItemString,"const U16 nOrderMenuItem_%d[]={",i);
					for(j=0;j<(pCustMenu[i].nNumofMenuItem-1);j++)
					{
						sprintf(nItemData,"%d,",pCustMenu[i].nOrderMenuItemId[j]);
						strcat(MenuItemString,nItemData);
					}
					sprintf(nItemData,"%d};",pCustMenu[i].nOrderMenuItemId[j]);
					strcat(MenuItemString,nItemData);			
					fprintf(pFile,"%s\n",MenuItemString);
				}
				
			}
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			fputs("\n const CUSTOM_MENU		nCustMenus[]={\n",pFile);//071706 menu resource
			for(i=0;i<nSize;i++)
			{
				if(pCustMenu[i].nNumofMenuItem)
					sprintf(MenuItemString,"{%d,%d,%d,%d,%d,%d,%d,%d,(U16 *)nOrderMenuItem_%d},",pCustMenu[i].nMenuItemId,pCustMenu[i].nParentId,pCustMenu[i].nNumofMenuItem,pCustMenu[i].nHideFlag,pCustMenu[i].nMovable,pCustMenu[i].nDispAttribute,pCustMenu[i].nStrId,pCustMenu[i].nImageId,i);
				else
					sprintf(MenuItemString,"{%d,%d,%d,%d,%d,%d,%d,%d,(U16 *)0},",pCustMenu[i].nMenuItemId,pCustMenu[i].nParentId,pCustMenu[i].nNumofMenuItem,pCustMenu[i].nHideFlag,pCustMenu[i].nMovable,pCustMenu[i].nDispAttribute,pCustMenu[i].nStrId,pCustMenu[i].nImageId);
				fprintf(pFile,"%s\n",MenuItemString);

			}

			fputs("};\n",pFile);
/*
			fputs("#else\n\n",pFile);
			for(i=0;i<nSize;i++)
			{
				if(pCustMenu[i].nNumofMenuItem)
				{
					sprintf(MenuItemString,"U16 nOrderMenuItem_%d[MENU_ITEM_SIZE]={",i);
					for(j=0;j<(pCustMenu[i].nNumofMenuItem-1);j++)
					{
						sprintf(nItemData,"%d,",pCustMenu[i].nOrderMenuItemId[j]);
						strcat(MenuItemString,nItemData);
				}
					sprintf(nItemData,"%d};",pCustMenu[i].nOrderMenuItemId[j]);
					strcat(MenuItemString,nItemData);
				}
				else
					sprintf(MenuItemString,"U16 nOrderMenuItem_%d[MENU_ITEM_SIZE];",i);
				fprintf(pFile,"%s\n",MenuItemString);				
			}
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			fputs("CUSTOM_MENU		nCustMenus[]={\n",pFile);
			for(i=0;i<nSize;i++)
			{
				sprintf(MenuItemString,"{%d,%d,%d,%d,%d,%d,%d,%d,(U16 *)nOrderMenuItem_%d},",pCustMenu[i].nMenuItemId,pCustMenu[i].nParentId,pCustMenu[i].nNumofMenuItem,pCustMenu[i].nHideFlag,pCustMenu[i].nMovable,pCustMenu[i].nDispAttribute,pCustMenu[i].nStrId,pCustMenu[i].nImageId,i);
				fprintf(pFile,"%s\n",MenuItemString);
			}

			fputs("};\n#endif\n\n\n",pFile);
*/
		}
		else
		{
			fputs("#ifdef MMI_ON_HARDWARE_P \n const \n #endif \n",pFile);
#ifdef __EXE_DLL__
			fputs("#if !defined (MMI_ON_HARDWARE_P)\n__declspec(dllexport) \n#endif\n",pFile);
#endif
			fputs("CUSTOM_MENU		nCustMenus[];",pFile);
		}
		fclose(pFile);
	}
	else
	{
		return 0;
	}
	return 1;
}

extern "C" void ToolPopulateInitial(void);
extern "C" void ToolPopulateDone(void);
extern "C" MMI_BOOL Iload(S8 *string,U16* pType);
extern "C" MMI_BOOL Aload(S8 *string,U16* pType);
extern "C" S8 ToUpper(S8 ch);

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
extern "C" int disableENFB;
#endif
extern "C" FILE *usedImageList=NULL;
#if 0
/************************************************************
**FUNCTION:		GetandWriteAudioResource
**DESCRIPTION:	
**
**
*************************************************************/
void WriteBackAudioResource(const char* sAudioSourceName,const unsigned int *data, unsigned int   len)
{
	FILE *fpAudSource;

	{
		unsigned int  i = 0;
		unsigned int  temp_data = 0;

		fpAudSource = fopen(sAudioSourceName,"wb");

		for(i=0; i< len;i++)
			{
				temp_data = data[i];
				fseek(fpAudSource,0,SEEK_END);
				fwrite(&temp_data, sizeof(temp_data), 1, fpAudSource);
			}

		fclose(fpAudSource);
	}
}
/*static const  unsigned int game01_mid[] =    //puzzle:Added by jinzh:20070731
{
	#include "game01_mid.ring.txt"
};

static const  unsigned int game02_mid[] =    //monkey:Added by jinzh:20070731
{
	#include "game02_mid.ring.txt"
};

static const  unsigned int game03_mid[] =    //ufo:Added by jinzh:20070731
{
	#include "game03_mid.ring.txt"
};

static const  unsigned int camera01_mid[] =    //快门音1
{
	#include "camera01_mid.ring.txt"
};

static const  unsigned int camera02_mid[] =    //快门音2
{
	#include "camera02_mid.ring.txt"
};

static const  unsigned int camera03_mid[] =    //快门音3
{
	#include "camera03_mid.ring.txt"
};
static const  unsigned int camera_cnt_01_mid[] =    //continus capture快门音1
{
	#include "camera01_mid_pcm.tab"
};

 static const  unsigned int camera_cnt_02_mid[] =    //continus capture快门音2
{
	#include "camera02_mid_pcm.tab"
};

static const  unsigned int camera_cnt_03_mid[] =    //continus capture快门音3
{
	#include "camera03_mid_pcm.tab"
};

static const  unsigned int camera_cnt_count_down[] =    //倒计时
{
	#include "daojishi_pcm.tab"
};

static const  unsigned int power01_mid[] =
{
	#include "power01_mid.ring.txt"
};

static const unsigned int power02_mid[] = 
{
	#include "power02_mid.ring.txt"
};

static const unsigned int power03_mid[] = 
{
	#include "power03_mid.ring.txt"
};

static const unsigned int power04_mid[] = 
{
	#include "power04_mid.ring.txt"
};

static const unsigned int power05_mid[] = 
{
	#include "power05_mid.ring.txt"
};

static const unsigned int power06_mid[] = 
{
	#include "power06_mid.ring.txt"
};


 static const  unsigned int cs01_mid[] =
{		
	#include "cs01_mid.ring.txt"
};

static const unsigned int cs02_mid[] = 
{
	#include "cs02_mid.ring.txt"
};

static const unsigned int cs03_mid[] = 
{
	#include "cs03_mid.ring.txt"
};

static const unsigned int cs04_mid[] = 
{
	#include "cs04_mid.ring.txt"
};

static const unsigned int cs05_mid[] = 
{
	#include "cs05_mid.ring.txt"
};


static const unsigned int cs06_mid[] = 
{
	#include "cs06_mid.ring.txt"
};

static const unsigned int cs07_mid[] = 
{
	#include "cs07_mid.ring.txt"
};

static const unsigned int cs08_mid[] = 
{
	#include "cs08_mid.ring.txt"
};

static const unsigned int cs09_mid[] = 
{
	#include "cs09_mid.ring.txt"
};

 static const unsigned int cs10_mid[] = 
{
	#include "cs10_mid.ring.txt"
};;

const unsigned int s1imy01_imy[] = 
{
	#include "s1imy01_imy.ring.txt"
};


 const unsigned int s1imy02_imy[] = 
{
	#include "s1imy02_imy.ring.txt"	
};


const unsigned int s1imy03_imy[] = 
{
	#include "s1imy03_imy.ring.txt"
};

const unsigned int s1imy04_imy[] = 
{
	#include "s1imy04_imy.ring.txt"
};

const unsigned int s1imy05_imy[] = 
{
	#include "s1imy05_imy.ring.txt"
};


const unsigned int s1imy06_imy[] = 
{
	#include "s1imy06_imy.ring.txt"
};

const unsigned int s1imy07_imy[] = 
{
	#include "s1imy07_imy.ring.txt"
};


 const unsigned int s1imy08_imy[] = 
{
	#include "s1imy08_imy.ring.txt"
};


const unsigned int s1imy09_imy[] = 
{
	#include "s1imy09_imy.ring.txt"
};


const unsigned int s1imy10_imy[] = 
{
	#include "s1imy10_imy.ring.txt"
};*/
void PopulateAudioResData(void)
{
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs01_mid.mid","../../mmi_customer/CustResource/ringData/include/cs01_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs02_mid.mid","../../mmi_customer/CustResource/ringData/include/cs02_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs03_mid.mid","../../mmi_customer/CustResource/ringData/include/cs03_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs04_mid.mid","../../mmi_customer/CustResource/ringData/include/cs04_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs05_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs05_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs06_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs06_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs07_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs07_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs08_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs08_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs09_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs09_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/cs10_mid.mp3","../../mmi_customer/CustResource/ringData/include/cs10_mid.ring.txt");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy01_imy.wav","../../mmi_customer/CustResource/ringData/include/s1imy01_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy02_imy.wav","../../mmi_customer/CustResource/ringData/include/s1imy02_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy03_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy03_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy04_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy04_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy05_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy05_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy06_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy06_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy07_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy07_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy08_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy08_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy09_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy09_imy.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/s1imy10_imy.mp3","../../mmi_customer/CustResource/ringData/include/s1imy10_imy.ring.txt");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/game01_mid.amr","../../mmi_customer/CustResource/ringData/include/game01_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/game02_mid.amr","../../mmi_customer/CustResource/ringData/include/game02_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/game03_mid.amr","../../mmi_customer/CustResource/ringData/include/game03_mid.ring.txt");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/camera01_mid_pcm.wav","../../mmi_customer/CustResource/ringData/include/camera01_mid_pcm.tab");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/camera02_mid_pcm.wav","../../mmi_customer/CustResource/ringData/include/camera02_mid_pcm.tab");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/camera03_mid_pcm.wav","../../mmi_customer/CustResource/ringData/include/camera03_mid_pcm.tab");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/daojishi_pcm.wav","../../mmi_customer/CustResource/ringData/include/daojishi_pcm.tab");

	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power01_mid.mp3","../../mmi_customer/CustResource/ringData/include/power01_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power02_mid.mp3","../../mmi_customer/CustResource/ringData/include/power02_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power03_mid.mp3","../../mmi_customer/CustResource/ringData/include/power03_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power04_mid.mp3","../../mmi_customer/CustResource/ringData/include/power04_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power05_mid.wav","../../mmi_customer/CustResource/ringData/include/power05_mid.ring.txt");
	ADD_APPLICATION_AUDIO("../../mmi_customer/aud_source/power06_mid.wav","../../mmi_customer/CustResource/ringData/include/power06_mid.ring.txt");

/*	WriteBackAudioResource("../../mmi_customer/aud_source/cs01_mid.mid",cs01_mid,sizeof(cs01_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs02_mid.mid",cs02_mid,sizeof(cs02_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs03_mid.mid",cs03_mid,sizeof(cs03_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs04_mid.mid",cs04_mid,sizeof(cs04_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs05_mid.mid",cs05_mid,sizeof(cs05_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs06_mid.mid",cs06_mid,sizeof(cs06_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs07_mid.mid",cs07_mid,sizeof(cs07_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs08_mid.mid",cs08_mid,sizeof(cs08_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs09_mid.mid",cs09_mid,sizeof(cs09_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/cs10_mid.mid",cs10_mid,sizeof(cs10_mid)/4);

	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy01_imy.wav",s1imy01_imy,sizeof(s1imy01_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy02_imy.wav",s1imy02_imy,sizeof(s1imy02_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy03_imy.wav",s1imy03_imy,sizeof(s1imy03_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy04_imy.wav",s1imy04_imy,sizeof(s1imy04_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy05_imy.wav",s1imy05_imy,sizeof(s1imy05_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy06_imy.wav",s1imy06_imy,sizeof(s1imy06_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy07_imy.wav",s1imy07_imy,sizeof(s1imy07_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy08_imy.wav",s1imy08_imy,sizeof(s1imy08_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy09_imy.wav",s1imy09_imy,sizeof(s1imy09_imy)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/s1imy10_imy.wav",s1imy10_imy,sizeof(s1imy10_imy)/4);

	WriteBackAudioResource("../../mmi_customer/aud_source/game01_mid.amr",game01_mid,sizeof(game01_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/game02_mid.amr",game02_mid,sizeof(game02_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/game03_mid.amr",game03_mid,sizeof(game03_mid)/4);
	
	WriteBackAudioResource("../../mmi_customer/aud_source/camera01_mid.pcm",camera01_mid,sizeof(camera01_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/camera02_mid.pcm",camera02_mid,sizeof(camera02_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/camera03_mid.pcm",camera03_mid,sizeof(camera03_mid)/4);

	WriteBackAudioResource("../../mmi_customer/aud_source/camera01_mid_pcm.pcm",camera_cnt_01_mid,sizeof(camera_cnt_01_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/camera02_mid_pcm.pcm",camera_cnt_02_mid,sizeof(camera_cnt_02_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/camera03_mid_pcm.pcm",camera_cnt_03_mid,sizeof(camera_cnt_03_mid)/4);

	WriteBackAudioResource("../../mmi_customer/aud_source/daojishi_pcm.pcm",camera_cnt_count_down,sizeof(camera_cnt_count_down)/4);

	WriteBackAudioResource("../../mmi_customer/aud_source/power01_mid.mp3",power01_mid,sizeof(power01_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/power02_mid.mp3",power02_mid,sizeof(power02_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/power03_mid.mp3",power03_mid,sizeof(power03_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/power04_mid.mp3",power04_mid,sizeof(power04_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/power05_mid.wav",power05_mid,sizeof(power05_mid)/4);
	WriteBackAudioResource("../../mmi_customer/aud_source/power06_mid.wav",power06_mid,sizeof(power06_mid)/4);*/
}
#endif

int main(int argc,char *argv[])
{
	char tempCommand[200];

	usedImageList=fopen("./usedImageList","w");
	if (usedImageList == NULL){
		printf("usedImageList could not open!\n");
		return 0;
	}
	if(argc == 5)//Check if run by CustPack tool
	{
		if(strcmp(argv[1], "-f")==0 && (strcmp(argv[2], "-i")==0 || strcmp(argv[2], "-a")==0))//-i: image, -a: audio
		{
			U16 nType;
			S16	nStrlen;
			S8 upperFileName[MAX_FILENAME_LEN];
		
			dest_file=fopen(argv[4],"wb");
  			if (dest_file == NULL){
  				resultFailFlag = 1;
				printf("dest_file could not open!\n");
  			}

			toolFlag = 1;
			ToolPopulateInitial();

			nStrlen = strlen(argv[3]) ;
			while (nStrlen>=0)
			{
				upperFileName[nStrlen] = ToUpper(argv[3][nStrlen]);
				--nStrlen;
			}

        #ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    		disableENFB = 1;
    	#endif
			if(strcmp(argv[2], "-i")==0)
			{
				Iload(upperFileName, &nType);
			}
			else if(strcmp(argv[2], "-a")==0)
			{
				Aload(upperFileName, &nType);
			}
		#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
			disableENFB = 0;
		#endif
			//ToolPopulateDone();

  			if(dest_file != NULL)
  			{
				fclose(dest_file);
  			}

  			if(resultFailFlag==1)
  			{
  				sprintf(tempCommand,"del -F %s", argv[4]);
				system(tempCommand);
  			}
		}
		else
		{
			printf("Usage: mtk_resgenerator.exe -f [-i or -a] ImageFilename OutputFilename\n");
		}
		fclose(usedImageList);
		return 1;
	}

	if(argc==3)
	{
		if(strcmp(argv[1], "-g")==0 && strcmp(argv[2], "-x")==0)
		{
			UnzipImage();
		}
		else
		{
			printf("Usage: mtk_resgenerator.exe -g -x\n");
			fclose(usedImageList);
			return 0;
		}
	}
	
	if((argc==2 || argc==3) && strcmp(argv[1], "-g")==0)
    {
        printf("Coolsand Resource Generate System 1.01.00\n");
        InitUnicodeSupport();
        printf("InitStrPopulate\n");
        InitStrPopulate();
        printf("PopulateResData\n");
        PopulateResData();

        printf("generate image resource\n");
        PopulateImageSearch();
        GenerateImageResFile();
        GenerateImageMapFile();
        GenerateImageMapSearchFile();

//        PopulateAudioSearch();
//        GenerateAudioResFile();
//        GenerateAudioMapFile();
//        GenerateAudioMapSearchFile();
	printf("generate Audio resource\n");
	PopulateAudioResData();

        printf("generate Menu resource\n");
        GenerateMenuFile(CurrMaxMenuItemIndex);

//        GenerateENFBAssociatedIDFile();
	 fclose(usedImageList);
        return 1;
    }
  else if ((argc==2 ) && strcmp(argv[1], "-s")==0)   //only update string and Menu
  {
        printf("Coolsand Resource Generate System 1.01.00\n");
        InitUnicodeSupport();
        printf("InitStrPopulate\n");
        InitStrPopulate();
        printf("PopulateResData\n");
        PopulateResData();
        printf("generate Menu resource\n");
        GenerateMenuFile(CurrMaxMenuItemIndex);
	 fclose(usedImageList);
        return 1;
        
  }
  
	printf("Usage: mtk_resgenerator.exe -g\n");
	fclose(usedImageList);
	return 0;
}
