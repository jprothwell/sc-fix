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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <tchar.h>
//#include "unicodexdcl.h"
//#include "ucs2prot.h"
#include "cs_types.h"
#include "mmi_data_types.h"

#include "mmi_features.h" 
#include "wap.h"
#include "voicememodef.h"
#include "funandgamesdefs.h"
#include "sscstringhandle.h"
#include "shortcutsdefs.h"
#include "callsetupenum.h"
#include "settingdefs.h"
//micha0202
//#include "networksetupdefs.h"
#include "networksetupgprot.h"
#include "phonebookdef.h"/*nvram_revise*/
#include "vobjectsresdef.h"

#include "scheduledpoweronoff.h"
#include "screensaverdefs.h"
#include "wallpaperdefs.h"
#ifdef __MMI_POWER_ON_OFF_DISPLAY__
#include "poweronoffdisplaydefs.h"
#endif
#include "organizerdef.h"
//#include "ExtraDef.h"
//micha0601
#include "servicedefs.h"
#include "satdefs.h"
#include "infonum.h"
#include "profilesenum.h"
#include "wgui_categories.h"
#include "wgui_categories_defs.h"
#if defined(__MMI_BMI__)
#include "bmi.h"
#endif

#ifdef __MMI_NOTEPAD__
#include "notepadenum.h"
#endif

#if defined(__MMI_CALORIE__)
/* under construction !*/
#endif
#if defined(__MMI_MENSTRUAL__)
#include "healthmenstural.h"
#endif
#ifdef __MMI_DICTIONARY__
#include "dictionarymain.h"
#endif
#if defined(__MMI_CURRENCY_CONVERTER__)
#include "currencyconvertor.h"
#elif defined(__MMI_EXTENDED_CURRENCY_CONVERTER__)
#include "currencyconvertore.h"
#endif


#if defined(__MMI_UNIT_CONVERTER__) || defined	(__MMI_EXTENDED_UNIT_CONVERTER__)
#include "unitconvertor.h"
#endif


#include "worldclock.h"


#if defined(__MMI_CALCULATOR__)
#include "gdi_include.h"
#include "calculator.h"
#endif

#include "todolistenum.h"
#include "alarmdef.h"

#include "custresdef.h"
#include "calendardef.h"
#if defined(__MMI_NITZ__)
	#include "nitz.h"
#endif
#include "personalizationdef.h"
#include "bootup.h"
#include "quickaccessmenuitem_def.h"
#include "simdetectiondef.h"
#include "messagesresourcedata.h"
#include "mainmenudef.h"
#include "t9def.h"//renamed file as suggested by elvis on date 20th aug 2003,changes done for t9
#include "idleappdef.h"
//#include "gui_themes.h"
//#include "wgui_categories.h"
#include "alarmdef.h"
#include "wgui_categories.h"
#include "statusiconres.h"
//#include "mmi_data_types.h"
//#include "wgui_fixed_menus.h"
#include "funandgamesdefs.h"
#include "gamedefs.h"
/***  Include necessory include files */
/***	You can copy from Res_Games.c   Kenneth */
#ifdef IS_F1RACE_BASE_GAME
/* under construction !*/
#endif /* IS_F1RACE_BASE_GAME */

#ifdef IS_HANOI_BASE_GAME
/* under construction !*/
#endif /* IS_HANOI_BASE_GAME */

#ifdef IS_RICHES_BASE_GAME
/* under construction !*/
#endif /* IS_RICHES_BASE_GAME */

#ifdef IS_COLORBALL_BASE_GAME
/* under construction !*/
#endif /* IS_COLORBALL_BASE_GAME */

#ifdef IS_SNAKE_BASE_GAME
/* under construction !*/
#endif /* IS_SNAKE_BASE_GAME */

#ifdef IS_SMASH_BASE_GAME
/* under construction !*/
#endif /* IS_SMASH_BASE_GAME */

#ifdef IS_DOLL_BASE_GAME
/* under construction !*/
#endif /* IS_DOLL_BASE_GAME */

#ifdef IS_NINJA_BASE_GAME
/* under construction !*/
#endif /* IS_NINJA_BASE_GAME */

#ifdef IS_MAJUNG_BASE_GAME
#include "majungdefs.h"
#endif /* IS_MAJUNG_BASE_GAME */

#ifdef IS_MEMORY_BASE_GAME
/* under construction !*/
#endif /* IS_MEMORY_BASE_GAME */

#ifdef IS_COPTER_BASE_GAME
#include "copterdefs.h"
#endif /* IS_COPTER_BASE_GAME */

#ifdef IS_PUZZLE_BASE_GAME
#include "puzzledefs.h"
#endif /* IS_PUZZLE_BASE_GAME */
/// WX
#ifdef IS_DFEX_BASE_GAME
#include "dfexdefs.h"
#endif /* IS_DFEX_BASE_GAME */
#ifdef __MMI_GAME_BQMET__
#include "bqmetdefs.h"
#endif


#ifdef IS_PANDA_BASE_GAME
#include "pandadefs.h"
#endif /* IS_PANDA_BASE_GAME */

#ifdef IS_STAIR_BASE_GAME
/* under construction !*/
#endif /* IS_STAIR_BASE_GAME */

#ifdef IS_UFO_BASE_GAME 
#include "ufodefs.h"
#endif /* IS_UFO_BASE_GAME */

#ifdef IS_DANCE_BASE_GAME
#include "dancedefs.h"
#endif /* IS_DANCE_BASE_GAME */

#ifdef IS_MAGICSUSHI_BASE_GAME
#include "magicsushidefs.h"
#endif /* IS_MAGICSUSHI_BASE_GAME */

#ifdef IS_FRUIT_BASE_GAME
/* under construction !*/
#endif /* IS_FRUIT_BASE_GAME */

#ifdef IS_VSMJ_BASE_GAME
/* under construction !*/
#endif /* IS_VSMJ_BASE_GAME */

/* Motion Game */
#ifdef IS_DICE_BASE_MOTIONGAME
#include "motiondicedefs.h"
#endif /* IS_DICE_BASE_MOTIONGAME */

#ifdef IS_DJ_BASE_MOTIONGAME
#include "motiondjdefs.h"
#endif /* IS_DJ_BASE_MOTIONGAME */

/***  End copy from Res_Games.c  */

#ifdef __J2ME__
#include "javagamedefs.h"
#endif

#include "composeringtonedefs.h"
#include "downloaddefs.h"
#include "composeringtoneenginedefs.h"
#include "gamedefs.h"
#include "factorymodedef.h"
#include "engineermodedef.h"
//#include "kal_non_specific_general_types.h"
//#include "custom_mmi_default_value.h"/*NVRAM_REVISE*/
//#include "custom_data_account.h"
//#include "DataAccount.h"
//typedef U16			UI_image_ID_type;
#include "frameworkstruct.h"
#include "commonscreens.h"
#include "callsdefs.h"
#include "settingdefs.h"
#include "phonebookdef.h"
#include "callhistoryenum.h"
#include "callmanagementiddef.h"
#include "cmcommonstringid.h"
#include "cmcommonimageid.h"
#include "helpdef.h"
   
//for audio player
// dependent headers for audioplayerdef.h"
#include "frameworkstruct.h"
#include "globalconstants.h"
#include "filemgr.h"
#include "audioplayerdef.h"
// for ab repeater
#include "abrepeaterdef.h"

//For File Manager
#ifdef __MMI_FILE_MANAGER__
#include "filemanagerdef.h"   
#endif

#ifdef __MMI_EBOOK_READER__
#include "ebookdefs.h"
#endif

//For Sound Recorder
#ifdef __MMI_SOUND_RECORDER__
#include "soundrecorderdef.h"   
#endif

#ifdef __MMI_EXT_DEVICE__
#include "extdevicedefs.h"
#endif

#ifdef __MMI_CAMERA__
#include "mmi_features_camera.h"
#include "cameraresdefs.h"
#endif

#ifdef __MMI_BARCODEREADER__
#include "mmi_features_barcodereader.h"
#include "barcodereaderresdefs.h"
#endif

#ifdef __MMI_IMAGE_VIEWER__
#include "imageviewerresdefs.h"
#endif 

#ifdef __MMI_VIDEO_PLAYER__
#include "mmi_features_video.h"
#include "vdoplystrenum.h"			/* Video Player String ID Enum */
#endif 

#ifdef __MMI_VIDEO_RECORDER__
#include "mmi_features_video.h"
#include "vdorecstrenum.h"			/* Video Recorder String ID Enum */
#endif 

#ifdef __MMI_VIDEO_EDITOR__
#include "mmi_features_video.h"
#include "vdoedtdefs.h"             /* video editor id enum. */
#endif

#ifdef __MMI_FM_RADIO__
#include "fmradiodef.h"
#endif

#ifdef __MMI_FM_RADIO_SCHEDULE_REC__
#include "fmschedulerecdef.h"
#endif


#ifdef __MMI_PHOTOEDITOR__
#include "phoedtstrenum.h"			/* PhotoEditor String ID Enum */
#endif

#ifdef __J2ME__
#include "javadef.h"
#endif /* !__J2ME__ */
#include "mmiapi_res.h" // add by hjf

#include "chatappdef.h"
//#include "EmailAppGProt.h"
#ifdef __MMI_IMPS__
#include "mmi_imps_enum.h"
#endif

#ifdef  __MMI_STOPWATCH__
#include "stopwatchdef.h"
#endif

#if defined(__MMI_VRSD__)
#include "vrsdenum.h"
#endif

#if defined(__MMI_VRSI__)
#include "vrsienum.h"
#endif

#if defined (JATAAYU_SUPPORT)
#include "jwap_str_id.h"
#include "jmms_str_id.h"
#include "prv_str_id.h"
#include "jprofilehandlerdefs.h"
#include "jprovdefs.h"
#include "messagesresourcedata.h"
#include "servicedefs.h"
#include "dataaccount.h"
#endif

//micha_PoC
#ifdef __MMI_POC__
#include "pocgprot.h"
#endif

#ifdef __MMI_VOIP__
#include "voipdef.h"
#endif

#ifdef __SYNCML_SUPPORT__
#include "syncmldef.h"
#endif

#ifdef __MMI_PICTURE_EDITOR_SUPPORT__
	#include "pictureeditordefs.h"
#endif

#ifdef __MMI_BG_SOUND_EFFECT__
	#include "bgsounddef.h"
#endif

#ifdef __MMI_UNIFIED_MESSAGE__
	#include "unifiedmessagedef.h"
#endif

#ifdef __MMI_UNIFIED_COMPOSER__
	#include "unifiedcomposerdef.h"
#endif

#if defined(__MMI_DOWNLOAD_AGENT__)
	#include "dlagentresdef.h"
#endif

#ifdef __MMI_SWFLASH__
	#include "swflashdef.h"
#endif

#if defined(__FOTA_ENABLE__)
    #include "devicemanagementdef.h"
#endif 

#ifdef __MMI_CCA_SUPPORT__
    #include "centralconfigagentui.h"
#endif
#include "plmnenum.h"
#ifdef __MMI_MULTI_SIM__
#include "mtpnp_ad_resdef.h"
#endif
#if defined(__MMI_ANALOGTV__)
#include "analogtvresdefs.h"		
#endif




#include "enum_list.h"

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
#define ENFB_STRING_DATA_FILENAME  "../../mmi_customer/CustResource/CustENFBStrData"
#define ENFB_STRING_HEADER_FILENAME  "../../mmi_customer/CustResource/CustENFBStr.h"
#endif

//get fontres data
extern const U16 CSD_gMaxDeployedLangs;
extern const sLanguageDetails CSD_gLanguageArray[];

int *mtk_gLanguageFlag;

//#define MAX_STRING_MAP_SIZE 45000
#include "custdatares.h"
const wchar_t* DELIMETER=L"\t\0";

/*
typedef struct Custom_StrMap_Search
{
	U16 nMinStrId;		//Minimum StringID defined by the developer.
	U16 nMaxStrId;		//Maximum StringID defined by the developer.
	U16 nMapIndex;		//Index of str in Custom_String_MAP.
}CUSTOM_STRMAP_SEARCH;
*/

typedef struct StrRes{
	wchar_t **Str;		//pointer array of lists to store Strings
	int *StrID;			//id pointers  of each string
	int *StrRef;		//pointers of Reference id in hash table of each string
	int MaxID;			//Maximum number of string ids
	int MaxNum;			//Maximum number of strings
	int TotalLength;
}StrRes;

typedef struct StrResSet{
	struct StrRes *strres;
	int iLang;
}StrResSet;

typedef struct HashItem{
	int pointer;		//index in string array
	struct HashItem *next;
}HashItem;


int mask_length(int val){
	int l=0,v;
	v = val/2;
	while (v>=1){
		l++;
		v=v/2;
	}
	return l;
}

int pow(int base, int index){
	int i,ret=1;
	for (i=0; i<index; i++)
		ret = ret*base;
	return ret;
}

int fmod(int denominator,int numerator){
	int ret=denominator;
	while (ret>=numerator)
		ret=ret-numerator;
	return ret;
}
/*********************************************************************
Function Name:			GetHashValue
Function Description:	Calculate the hash value of input string
Return Value:		
Function Input:
						string:		input string
						hash_rang:	the max hash value
*********************************************************************/

int GetHashValue(wchar_t *string,int hash_range){
	int length = mask_length(hash_range);
	int mask=(int)pow(2,length)-1;
	int i=0;
	long val=0;
	for (i=0;i<(int)wcslen(string);i++){
		val += ((string[i]+val*37)&mask);
	}
	val = (int)fmod(val,hash_range);
	return (int)val;
}
/*********************************************************************
Function Name:			Trans
Function Description:	Transform UCS2-encoding "\","n" to "\n"
Return Value:			
						0:			no transformation is done.
						1:			transformation is done.
Function Input:
						string:		input string
						buf:		output string
*********************************************************************/
int Trans (wchar_t *string, wchar_t *buf){
	wchar_t *p=NULL;
	const wchar_t cr[]=L"\\n";
	unsigned int i=0;
	int l=0,ret=0;
	for (i=0; i<wcslen(string); i++){
		if ( string[i]==L'\\' && string[i+1]==L'n' ){
			buf [l]='\n';
			i++;
			ret=1;
		} else buf[l]=string[i];
		l++;
	}
	buf[l]=L'\0';
	return ret;
}

/************************************************************************************

************************************************************************************/
int PreParseTable(FILE *file)
{
	return 1;	
}

/************************************************************************************

************************************************************************************/
void UCS2ASCII (char *w_in, char *a_out){
	int i=0;
	while ( w_in[i]!='\0' || w_in[i+1]!='\0'  ){
		a_out[i>>1] = w_in[i];
		i+=2;
	}
	a_out[i>>1]='\0';
	i=i>>1;
	i--;
	while (i>=0)
	{
		if ( a_out[i]==0x0D ){
			a_out[i]='\0';
			break;
		}
		else i--;
		
	}
}

static void wtoc(char* Dest, const wchar_t* Source)
{
    int i = 0;
    //wprintf(L"%s", Source);
    while(Source[i] != '\0')
    {
        Dest[i] = (CHAR)Source[i];
        ++i;
    }
}

/************************************************************************************
Function Name:			ifLanguageUsed
Function Description:		Decide if the translated language should be used for
				string resource, according to SSC string.
Return Value:			-1: The language is not used.
				>=0: The index in FontRes
Function Input:			ssc_ref:     SSC string defined in ref_list.txt
************************************************************************************/
int ifLanguageUsed( wchar_t* ssc_ref)
{
	int i =0;
	char ascii_ssc[10];
	UCS2ASCII ((char*)ssc_ref,ascii_ssc);
    //memset(ascii_ssc, 0, 10);
    //wtoc(ascii_ssc, ssc_ref);
	for ( i=0; i<CSD_gMaxDeployedLangs; i++ )
	{
		if ( strcmp ( CSD_gLanguageArray[i].aLangSSC, ascii_ssc ) ==0 )
		{
			return i;
		}
	}
	return -1;
}

/*********************************************************************
Function Name:			GetStrResList
Function Description:	Generate a non-duplicate string list from input file, using hash table
Return Value:			Number of all strings
Function Input:
						file:		input file contains string lists
						result:		non-duplicate string list
*********************************************************************/

int GetStrResList (FILE *file, StrResSet *result){
	FILE *f=file;
	struct HashItem **ht=NULL,*extra=NULL,*temp=NULL;
	struct StrRes *strres=NULL;
	wchar_t tmp0[1024], *tmp=NULL, *tok=NULL, *buf=NULL;
	int i=0,j=0,k=0,h=0,iMaxID=-2,l=0,STRID=0;//;,*iMaxNum=NULL;
	int iLang=0;//number of languages
	int LangIndex=0;//Language index in CSD_gLanguageArray
	int hash_range=0;//the value-range of hash function
	int num=0;//number of distinct strings
	int nStringLen=0;//String Length for UCS2 ENCODING
	int order=0;
	int position=0;//the position to insert a new string
	int buffer_size;

    memset(tmp0, 0, sizeof(wchar_t)*1024);
	//Read in number of languages in the file
	fgetws(tmp0,1024,f);
	tok = wcstok(tmp0,DELIMETER);
	tok = wcstok(NULL,DELIMETER);
	iLang = _wtoi(tok);

	//number language should be from FontRes.c
	
	buffer_size=2048*iLang;
	tmp=malloc(sizeof(wchar_t)*buffer_size);
	memset(tmp, '0',buffer_size*sizeof(wchar_t));
	//Read maximum string number defined at the end of file.
	while (tmp[0]!=(wchar_t)'#' && tmp[1]!=(wchar_t)'#'){
		fgetws(tmp,buffer_size,f);
		iMaxID++;
	}
	tok = wcstok(tmp,DELIMETER);
	tok = wcstok(NULL,DELIMETER);
/*
	Number of Languages used should be referenced from FontRes.c
	strres = malloc (sizeof(struct StrRes)*iLang);
	ht = malloc(sizeof(struct HashItem*)*iLang);
*/
	strres = malloc (sizeof(struct StrRes)*CSD_gMaxDeployedLangs);
	ht = malloc (sizeof(struct HashItem*)*CSD_gMaxDeployedLangs);

	mtk_gLanguageFlag = malloc (sizeof(int)*CSD_gMaxDeployedLangs);
	for (h=0; h<CSD_gMaxDeployedLangs; h++){
		mtk_gLanguageFlag[h]=0;
	}

	for (j=0;j<iLang;j++){
		rewind(f);//reset to the start of file
		num=0;
//skip first two row (number of languages and language description), and start from third row to read in SSC String.
		fgetws(tmp,buffer_size,f);fgetws(tmp,buffer_size,f);fgetws(tmp,buffer_size,f);
//		printf ("\n");
		tok = wcstok(tmp,DELIMETER);//INDEX
		tok = wcstok(NULL,DELIMETER);//ENUM
		tok = wcstok(NULL,DELIMETER);//MODULE NAME
		tok = wcstok(NULL,DELIMETER);//MAX LENGTH
		tok = wcstok(NULL,DELIMETER);//DESCRIPTION
//change to corresponding language column according to j
		for (k=0;k<j+1;k++){
			tok = wcstok(NULL,DELIMETER);
		}
		LangIndex = ifLanguageUsed (tok);
		
		if ( LangIndex<0 ) // The language is not used in font resource.
			continue;
		rewind(f);
		
		mtk_gLanguageFlag[LangIndex]=1;
		
		strres[LangIndex].Str=(wchar_t**)malloc(sizeof(wchar_t*)*iMaxID);
		strres[LangIndex].StrID=(int*)malloc(sizeof(int)*iMaxID+1);
		strres[LangIndex].StrRef=(int*)malloc(sizeof(int)*iMaxID);
//Set default value
		for (l=0;l<iMaxID;l++){
			strres[LangIndex].StrID[l]=-1;
			strres[LangIndex].StrRef[l]=-1;
			strres[LangIndex].TotalLength=0;
		}

//set hash_range as maximum id times 1.1(Can be changed)
		hash_range=(int)(iMaxID*1.1);

//initialize hash table
		ht[LangIndex]=(struct HashItem*)malloc(sizeof(struct HashItem)*hash_range);
		for (k=0;k<hash_range;k++){
			ht[LangIndex][k].next=NULL;
			ht[LangIndex][k].pointer=-1;
		}
//skip first two row (number of languages and language description), and start from third row.
		fgetws(tmp,buffer_size,f);fgetws(tmp,buffer_size,f);fgetws(tmp,buffer_size,f);fgetws(tmp,buffer_size,f);

		k=0;
		i=0;//number of Ref(non-duplicate strings, excluding NULL)
		l=-1;//number of ID (all strings)
//start to read file
		while ( !feof(f) ){
//Clear the tempopary buffer
			if ( buf!=NULL ){
				tok=NULL;
				free(buf);
				buf=NULL;
			}
//The last line of file
			if (tmp[0]==(wchar_t)'#')
				break;

			l++;
//Read in ID of the string
			tok = wcstok(tmp,DELIMETER);
			tok = wcstok(NULL,DELIMETER); //skip INDEX
			tok = wcstok(NULL,DELIMETER); //skip originally existing Module Name, JL
			tok = wcstok(NULL,DELIMETER); //skip originally existing Max String Length, JL
			//STRID=_wtoi(tok);
			STRID = ENUM_VALUE_MAP[l];
//string description
			tok = wcstok(NULL,DELIMETER);
//change to corresponding language column according to j
			for (k=0;k<j+1;k++){
				tok = wcstok(NULL,DELIMETER);
				if (tok!=NULL && tok[wcslen(tok)-1]==(wchar_t)'\n'){//Remove carriage-return
					tok[wcslen(tok)-2]='\0';
				}
			}
//decise if the read-in string is " " (blank)
			if ( tok !=NULL && wcslen(tok) ==1 && tok[0] ==20 )
				tok = NULL;
//Handling NULL read-in string. Insert its id into id list, it's Ref is default value -1;
			if (tok==NULL){
				order=0;
//sorting on id ascendant
				while (order<l){
					if (STRID>strres[LangIndex].StrID[order])
						order++;
					else
						break;
				}
//insert the string into the list
				position=order;
				order=l;
				while (order>position){
					strres[LangIndex].StrRef[order]=strres[LangIndex].StrRef[order-1];
					strres[LangIndex].StrID[order]=strres[LangIndex].StrID[order-1];
					order--;
				}
				strres[LangIndex].StrID[position]=STRID;
			}
//End of handling NULL string

			if (tok!=NULL){
				//在雙位元下把\n換成0X0A
                buf = malloc ((wcslen(tok)+1)*sizeof(wchar_t));
//Transform "\""n", if no transformation is done, free the memory used.
				if ( Trans(tok,buf)==0 ){
					free (buf);
					buf=NULL;
				}else
					tok=buf;
//calculate hash value
				h=GetHashValue(tok,hash_range);
				if (ht[LangIndex][h].pointer==-1){//no duplicate hash value
//					int z=0;
					ht[LangIndex][h].pointer=i;
					ht[LangIndex][h].next=NULL;
//					z=wcslen(tok);
					strres[LangIndex].Str[i] =(wchar_t*) malloc ((wcslen(tok)+1)*sizeof(wchar_t));
					wcscpy(strres[LangIndex].Str[i],tok);
//insert the string into the list
					order=0;
					while (order<l){
						if (STRID>strres[LangIndex].StrID[order])
							order++;
						else
							break;
					}
					position=order;
					order=l;
					while (order>position){
						strres[LangIndex].StrRef[order]=strres[LangIndex].StrRef[order-1];
						strres[LangIndex].StrID[order]=strres[LangIndex].StrID[order-1];
						order--;
					}
					strres[LangIndex].StrRef[position]=i;
					strres[LangIndex].StrID[position]=STRID;
					strres[LangIndex].TotalLength +=  wcslen(tok)+1; 
					i++;
//end of insert string into the list
				} else {
//there are duplicate hash value, check if the string is really the same with that has the same hash value
					int found =0;
					if ( wcscmp ( tok,strres[LangIndex].Str[ht[LangIndex][h].pointer] )==0 ){
	//The two string is the same
						order=0;
						while (order<l){
							if (STRID>strres[LangIndex].StrID[order])
								order++;
							else
								break;
						}
						position=order;
						order=l;
						while (order>position){
							strres[LangIndex].StrRef[order]=strres[LangIndex].StrRef[order-1];
							strres[LangIndex].StrID[order]=strres[LangIndex].StrID[order-1];
							order--;
						}
						strres[LangIndex].StrRef[position]=ht[LangIndex][h].pointer;
						strres[LangIndex].StrID[position]=STRID;
	//read in next line
						fgetws(tmp,buffer_size,f);
						continue;
					}
/*					Although the two strings have the same hash value, but they are different
					Compare the strings in linked list...
*/
					temp = &ht[LangIndex][h];
					while (temp->next!=NULL){
						temp=temp->next;
						if ( wcscmp ( tok,strres[LangIndex].Str[temp->pointer] )==0 ){
	//find a identical string in linked list, set the flag on
							found =1;
							order=0;
							while (order<l){
								if (STRID>strres[LangIndex].StrID[order])
									order++;
								else
									break;
							}
							position=order;
							order=l;
							while (order>position){
								strres[LangIndex].StrRef[order]=strres[LangIndex].StrRef[order-1];
								strres[LangIndex].StrID[order]=strres[LangIndex].StrID[order-1];
								order--;
							}
							strres[LangIndex].StrRef[position]=temp->pointer;
							strres[LangIndex].StrID[position]=STRID;
							break;
						}
					}
					if ( found ==0 ){
	/*					The input string is different with all other strings in hash table.
						Add the string in linked list.
	*/
						extra = (struct HashItem*)malloc(sizeof(struct HashItem));
						extra->next=NULL;
						extra->pointer=i;
						temp->next=extra;
						strres[LangIndex].Str[i] = malloc ((wcslen(tok)+1)*sizeof(wchar_t));
						wcscpy(strres[LangIndex].Str[i],tok);
						order=0;
						while (order<l){
							if (STRID>strres[LangIndex].StrID[order])
								order++;
							else
								break;
						}
						position=order;
						order=l;
						while (order>position){
							strres[LangIndex].StrRef[order]=strres[LangIndex].StrRef[order-1];
							strres[LangIndex].StrID[order]=strres[LangIndex].StrID[order-1];
							order--;
						}
						strres[LangIndex].StrRef[position]=i;
						strres[LangIndex].StrID[position]=STRID;
						strres[LangIndex].TotalLength +=  wcslen(tok)+1; 
						i++;
					} else found =0; // reset the flag
				}
			}
			fgetws(tmp,buffer_size,f);
		}
		strres[LangIndex].MaxID=l+1;
		strres[LangIndex].MaxNum=i;
	}
	result->iLang=CSD_gMaxDeployedLangs;
	result->strres=strres;
	free(tmp);
	return iMaxID;
}


int main(int argc, char **argv){
    FILE *f = NULL;
    FILE *fout_res = NULL;
    FILE *fout_map = NULL;
    int i = 0, iLang = 0, str_id = 0, MaxID = 0, str_res_id = 0;
    unsigned short z = 0;
    int *MaxNum = NULL;
    int *duplate = NULL;
    int start = 0, index_map = 0, index = 0, num_map_search = 0;
    int min_id = 0, max_id = 0;
    struct StrRes *strres = NULL;
    struct StrResSet Strresset;

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    FILE *fout_enfb = NULL;
    U32 enfb_size = 0, enfb_offset = 0;
    U32 enfb_str_mem_size, enfb_max_mem_size = 0;
#endif
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
		U32 nMaxStringSize = 0;
#endif
    /* open CustStrList.txt */
    if (argc == 1 )
    {
        f = fopen("../../mmi_customer/ResGenerator/CustStrList.txt", "rb");
        if (f == NULL)
        {
            printf("File Open Error (CustStrList.txt) ...!\n");
            exit(1);
        }
    }
    else
    {
        f = fopen(argv[1], "rb");
        if (f == NULL)
        {
            printf("File Open Error (%s) ...!\n", argv[1]);
            exit(1);
        }
    }
    
    /* get string table */
    MaxID = GetStrResList(f, &Strresset);
    iLang = Strresset.iLang;
    
    /* open output file */
//    fout_res = fopen("CustStrRes.c", "wb");
//    fout_map = fopen("CustStrMap.c", "wb");
    fout_res = fopen(STR_DATA_FILENAME, "wb");
    fout_map = fopen(STR_MAP_FILENAME, "wb");
    strres = Strresset.strres;

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    fout_enfb = fopen(ENFB_STRING_DATA_FILENAME, "wb");
    if(fout_enfb == NULL)
    {
        printf("File open error (%s) ...!\n", ENFB_STRING_DATA_FILENAME);
        exit(1);
    }
#endif

    /* print output file header */
    fprintf(fout_res, "#include \"custdatares.h\"\n");
    fprintf(fout_res, "#include \"fontdcl.h\"\n\n\n");
    fprintf(fout_res, "#if ( defined (MMI_ON_WIN32) || !defined (__COOLSAND_TARGET__) )\n\t#define __align(x)\n#endif\n\n\n");
    fprintf(fout_map, "#include \"custdatares.h\"\n");

    /* output string resource file */
    for(i = 0; i < CSD_gMaxDeployedLangs; i++)
    {
        int old_i = i;
        unsigned long nTotalLen=0;
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	 unsigned long nStringSize=0;
	 unsigned char filename[250];
	 FILE *fStrBin = NULL;
	 sprintf(filename, "..\\..\\mmi_customer\\CustResource\\StrRes_%d.bin",old_i+1);
	 fStrBin = fopen(filename,"wb");
#endif	 
        if(mtk_gLanguageFlag[i] == 0)
        {
            printf("!!! Please provide translated strings of %s in ref_list.txt !!!\n", CSD_gLanguageArray[i].aLangSSC);
            i = 0;
        }
        
        /* output language structure title */
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	fprintf(fout_res, "#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE \n");
	fprintf(fout_res,"__align(4) const CUSTOM_STRING StrRes_%d[] = {\n",old_i+1);
	fprintf(fout_res, "#ifdef COMPRESS_USE_LZMA\n");
	fprintf(fout_res,"\t#include \"StrRes_%d_lzma.tab\"\n",old_i+1);
	fprintf(fout_res, "#else\n");
	fprintf(fout_res,"\t#include \"StrRes_%d_huffman.tab\"\n",old_i+1);
	fprintf(fout_res, "#endif\n");
	fprintf(fout_res,"};\n#else\n");
#endif
        fprintf(fout_res, "__align(4) const CUSTOM_STRING StrRes_%d[] = {\n", old_i + 1);
        fprintf(fout_map, "const CUSTOM_STRING_MAP StrMap_%d[] = {\n", old_i + 1);

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
        enfb_size = 0;
        enfb_str_mem_size = 0;
#endif

        /* output string resource header */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
        fputc(0, fout_enfb);
        fputc(0, fout_enfb);
        enfb_size += 2;
#else
        fputs("0x00, 0x00,\n", fout_res);
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	 nStringSize +=2;
	 fputc(0x00,fStrBin);
	 fputc(0x00,fStrBin);
#endif	 
#endif

        /* output CustStrRes.c */
        for(str_res_id = 0; str_res_id < strres[i].MaxNum; str_res_id++)
        {
            unsigned short nStringLen = 0;

#ifndef __UCS2_ENCODING
            fprintf(fout_res, "\"%s\",\n", strres[i].Str[str_res_id]);
#else /* __UCS2_ENCODING */
            nStringLen = wcslen(strres[i].Str[str_res_id]);
           
           // for(z = 0; z < nStringLen*2; ++z)
            for(z = 0; z < nStringLen; ++z)
            {
                unsigned char* abc = (unsigned char*)strres[i].Str[str_res_id];
                wchar_t* tmpWStr = strres[i].Str[str_res_id];
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
                fputc(abc[z], fout_enfb);
                enfb_size++;
#else /* __MMI_RESOURCE_ENFB_SUPPORT__ */
                fprintf(fout_res, "0x%X,0x%X,", tmpWStr[z]&0xff,
                        tmpWStr[z]>>8);
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
		   nStringSize +=2;
		   fputc(tmpWStr[z]&0xff,fStrBin);
		   fputc(tmpWStr[z]>>8,fStrBin);
#endif
             //   fprintf(fout_res, "0x%X,", abc[z]);
                        
#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */
            }

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
            fputc(0, fout_enfb);
            fputc(0, fout_enfb);
            enfb_size += 2;
#else /* __MMI_RESOURCE_ENFB_SUPPORT__ */
            fputs("0x00, 0x00,\n", fout_res);
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	     nStringSize +=2;
	   	 fputc(0x00,fStrBin);
	   	 fputc(0x00,fStrBin);
#endif		 
#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */
#endif /* __UCS2_ENCODING */
        }
        
        /* output ENFB string resource header */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
        fprintf(fout_res,
                "\t0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\n",
                (U8)255,    //type
                (U8)0,      //reserved
                (U8)0,      //reserved
                (U8)0,      //reserved
                (U8)(enfb_offset & 0xFF),
                (U8)((enfb_offset >> 8) & 0xFF),
                (U8)((enfb_offset >> 16) & 0xFF),
                (U8)((enfb_offset >> 24) & 0xFF),
                (U8)(enfb_size & 0xFF),
                (U8)((enfb_size >> 8) & 0xFF),
                (U8)((enfb_size >> 16) & 0xFF),
                (U8)((enfb_size >> 24) & 0xFF));
        enfb_offset += enfb_size;
        enfb_str_mem_size += enfb_size;
#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
        enfb_size = 0;
#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */

        /* output CustStrMap.c */
        for(str_res_id = 0; str_res_id < strres[i].MaxID; str_res_id++)
        {
            unsigned int nStringLen = 0;
            int iterator = 0;
            wchar_t* wcString;
            
            if (strres[i].StrRef[str_res_id] == -1)
            {
                nTotalLen = 0;
            }
            else
            {
                nTotalLen = 2;
            }
            
            for(iterator = 0; iterator < strres[i].StrRef[str_res_id]; iterator++)
            {
                nStringLen = 0;
                nStringLen = wcslen(strres[i].Str[iterator]);
                wcString = NULL;
                nTotalLen += ((nStringLen + 1)*2);
            }

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
            fputc(((nTotalLen/2) & 0xFF), fout_enfb);
            fputc((((nTotalLen/2) >> 8) & 0xFF), fout_enfb);
            fputc((((nTotalLen/2) >> 16) & 0xFF), fout_enfb);
            fputc((((nTotalLen/2) >> 24) & 0xFF), fout_enfb);
            enfb_size += 4;
#else /* __MMI_RESOURCE_ENFB_SUPPORT__ */
            fprintf(fout_map, "\t%d,\n", nTotalLen/2);
#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */
        }
        
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
        fprintf(fout_map, "\t%d, %d\n", enfb_offset, enfb_size);
        enfb_offset += enfb_size;
        enfb_str_mem_size += enfb_size;
        if(enfb_str_mem_size > enfb_max_mem_size)
        {
            enfb_max_mem_size = enfb_str_mem_size;
        }
#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */

        fprintf(fout_res, "};\n");
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	 fprintf(fout_res, "#endif\n");
#endif
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
//	fprintf(fout_res, "#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE \n");
//	fprintf(fout_res, "CUSTOM_STRING ch_StrRes_%d[%d];\n",old_i+1,nStringSize);
//	fprintf(fout_res, "#endif\n\n");
		if ( nStringSize > nMaxStringSize ) nMaxStringSize = nStringSize;
      fclose(fStrBin);
#endif
        fprintf(fout_map, "};\n");
        fprintf(fout_res, "extern const CUSTOM_STRING_MAP StrMap_%d[];\n", old_i + 1);
        i = old_i;
    }
    
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    { /* output ENFB string resource header for RESMEM */
        FILE *fp = fopen(ENFB_STRING_HEADER_FILENAME, "w");
        
        if (fp == NULL)
        {
            printf("File open error (%s) ...!\n", ENFB_STRING_HEADER_FILENAME);
            exit(1);
        }
        fprintf(fp, "\n#ifdef __MMI_RESOURCE_ENFB_SUPPORT__\n");
        fprintf(fp, "#define MaxENFBStringSize\t%d\n", enfb_max_mem_size);
        fprintf(fp, "#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */\n\n");
        fclose(fp);
    }
#endif /* __MMI_RESOURCE_ENFB_SUPPORT__ */
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	fprintf(fout_res, "#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE \n");
	fprintf(fout_res, "CUSTOM_STRING ch_StrRes_all[%d];\n",nMaxStringSize);
	fprintf(fout_res, "#endif\n\n");
#endif

    /* output STRMAP_SEARCH array */
    fprintf(fout_res, "const CUSTOM_STRMAP_SEARCH StrMapSearch[] = {\n");
    for(str_res_id = 0; str_res_id < MAX_STRING_MAP_SIZE; str_res_id++)
    {
        if(start == 1)
        {
            if(strres[0].StrID[index_map + 1] == strres[0].StrID[index_map] + 1)
            {
                max_id += 1;
                index_map += 1;
            }
            else
            {
                start = 0;
                fprintf(fout_res, "\t{%d, %d, %d},\n", min_id, max_id, index);
                index_map += 1;
                index = index_map;
            }
        }
        else if(start == 0)
        {
            if(str_res_id == strres[0].StrID[index_map])
            {
                start = 1;
                num_map_search += 1;
                min_id = str_res_id;
                max_id = str_res_id;
            }
        }
        else if(start == -1)
        {
            if(str_res_id == strres[0].StrID[index_map])
            {
                start = 1;
                num_map_search += 1;
                min_id = str_res_id;
                max_id = str_res_id;
            }
        }
    }
    fprintf(fout_res, "};\n\n");
	
    /* output language list array */
    fprintf(fout_res, "const StringResList CSD_gStringList[MAX_LANGUAGES] = {\n");
    for(i = 0; i < CSD_gMaxDeployedLangs; i++)
    {
        int old_i = i;
        
        if(mtk_gLanguageFlag[i] == 0)
        {
            i = 0;
        }
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	  fprintf(fout_res, "#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE \n");
        fprintf(fout_res, "{(CUSTOM_STRING*)StrRes_%d, sizeof(StrRes_%d), (CUSTOM_STRING*)ch_StrRes_all, %d, (CUSTOM_STRING_MAP*)StrMap_%d, %d, (CUSTOM_STRMAP_SEARCH *)StrMapSearch, %d},\n", old_i + 1, old_i + 1, strres[i].MaxNum + 1, old_i + 1, strres[i].MaxID + 1, num_map_search);
	  fprintf(fout_res, "#else\n");
        fprintf(fout_res, "{(CUSTOM_STRING*)StrRes_%d, %d, (CUSTOM_STRING_MAP*)StrMap_%d, %d, (CUSTOM_STRMAP_SEARCH *)StrMapSearch, %d},\n", old_i + 1, strres[i].MaxNum + 1, old_i + 1, strres[i].MaxID + 1, num_map_search);
	  fprintf(fout_res, "#endif\n");
#else
        fprintf(fout_res, "{(CUSTOM_STRING*)StrRes_%d, %d, (CUSTOM_STRING_MAP*)StrMap_%d, %d, (CUSTOM_STRMAP_SEARCH *)StrMapSearch, %d},\n", old_i + 1, strres[i].MaxNum + 1, old_i + 1, strres[i].MaxID + 1, num_map_search);
#endif
        i = old_i;
    }
    fprintf(fout_res, "};\n");
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
	  fprintf(fout_res, "#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE \n");
	  fprintf(fout_res, "#include \"mmi_trace.h\"\n#include \"cos.h\"\n#include \"oslmemory.h\"\n");
	  fprintf(fout_res, "extern void cdfu_app_init(const unsigned char *bufin,  unsigned int bufinlen, unsigned char *bufDes);\n\n");
	  fprintf(fout_res, "extern U16 	gCurrLangIndex;\n");
	  fprintf(fout_res, "unsigned char gCurrentDecompressLan=-1;\nunsigned char *gCurrentDecompressBuff=NULL;\n");
	  fprintf(fout_res, "void ch_strdec(void)\n{\n");
 	  fprintf(fout_res, "\tif (gCurrLangIndex == gCurrentDecompressLan)\n");
	  fprintf(fout_res, "\t\treturn;\n");	

        for(i = 0; i < CSD_gMaxDeployedLangs; i++)
        {
        	fprintf(fout_res, "\tif (gCurrLangIndex == %d)\n",i);
        	fprintf(fout_res, "\t{\n\t");
		#if 0	
		fprintf(fout_res, "\tif (gCurrentDecompressLan !=%d )\n\t\t",i);
		fprintf(fout_res, "\tcdfu_do_deinit(CDFU_RES, gCurrentDecompressBuff);\n\t");
		fprintf(fout_res, "\tch_StrRes_%d = OslMalloc(ch_StrRes_%d_size);\n\t",i+1,i+1);
		#endif
    	  	fprintf(fout_res, "\tcdfu_app_init(StrRes_%d,sizeof(StrRes_%d),ch_StrRes_all);\n\t",i+1,i+1);
		fprintf(fout_res, "\tgCurrentDecompressLan = %d;\n\t",i);
		#if 0
		fprintf(fout_res, "\tgCurrentDecompressBuff = ch_StrRes_%d;\n\t",i+1);
		#endif
		fprintf(fout_res, "}\n");
        }
	  fprintf(fout_res, "}\n#endif\n");
#endif
    fclose(f);
    fclose(fout_res);
    fclose(fout_map);
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    fclose(fout_enfb);
#endif

    return 0;
}
