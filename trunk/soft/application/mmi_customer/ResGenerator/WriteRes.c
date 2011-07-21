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
 *             HISTORY
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
 *==============================================================================
 *******************************************************************************/


#include "cs_types.h"
#include "custdatares.h"
#include "globalmenuitems.h"
#include "buildcfg.h"
#include "mmi_data_types.h"
#include "custdataprots.h"
#include "custmenures.h"
#include "mmi_features.h"//070306 Alpha layer
#include <string.h>
#include <stdio.h>

#include <stdarg.h>

#include "oslmemory.h"
// #include "ucs2prot.h"
#include "unicodexdcl.h"
#include "custresdef.h"
#include "gui_data_types.h"
#include "gui.h"

#define MAX_NAME_LENGTH             100
#define MENUITEM_ID_TYPE            9
#define PARENT_ITEM_ID_TYPE            8
#define NUMBER_OF_MENUITEMS_TYPE    7
#define MENU_ITEM_ORDER_TYPE        6
#define  HIDE_FLAG_TYPE             5
#define MOVEABLE_FLAG_TYPE          4
#define  DISPLAY_ATTRIBUTE_TYPE        3
#define  MENU_ITEM_STRING_ID_TYPE      2
#define  MENU_ITEM_IMAGE_ID_TYPE       1
#define TRUE                     1
#define FALSE                    0
#define IMAGE_DESCRIPTOR_SIZE       2
#define  BUFFER_SIZE             4096
#define  BUFFER_SIZE1               16

#define RES_SECTION_ALONG
#ifdef TGT_GALLITE_G800
#undef RES_SECTION_ALONG
#endif

#define   OUTPUT_DATA_FILENAME "../../mmi_customer/CustResource/custimgdatahw.h"
/* __CUSTPACK_MULTIBIN Calvin BEGIN */
#define   OUTPUT_DATA_FILENAME_EXT "../../mmi_customer/CustResource/custimgdatahwext.h" /* 040805 CustPack: Calvin added */
/* __CUSTPACK_MULTIBIN Calvin END */

#define  OUTPUT_MENUTREE_FILENAME "custmenutree_out.c"
#ifdef RES_SECTION_ALONG
#define  OUTPUT_IMAGE_NAMES_END "[]  \n#ifndef MMI_ON_WIN32\n  __attribute__((section (\".roresdata\"))) \n#endif\n=\n{"
#define  OUTPUT_IMAGE_NAMES_END2 "[]  \n#ifndef MMI_ON_WIN32\n  __attribute__((section (\".robsdata\"))) \n#endif\n=\n{"
#define  OUTPUT_IMAGE_NAMES_END3 "[]  \n#ifndef MMI_ON_WIN32\n  __attribute__((section (\".extra\"))) \n#endif\n=\n{"

#else
#define  OUTPUT_IMAGE_NAMES_END "[]=\n{"
#endif
#define  NOIMAGE_FILENAMEBUFFER "../../mmi_customer/IMAGES/NOIMAGE.BMP"
#define  DEFAULT_FILENAMEBUFFER "../../mmi_customer/Images/Default.bmp"
#define     DEFAULT_FILEBUFFER         "______MMI_CUSTOMER_IMAGES_DEFAULT_BMP"

/* 101205 audio resource Calvin Satrt */
#define   AUDIO_OUTPUT_DATA_FILENAME "../../mmi_customer/CustResource/CustAdoDataHW.h"
#define   AUDIO_OUTPUT_DATA_FILENAME_EXT "../../mmi_customer/CustResource/CustAdoDataHWExt.h"
#define  OUTPUT_AUDIO_NAMES_END "[]=\n{"
#define  NOAUDIO_FILENAMEBUFFER "../../mmi_customer/AUDIO/NOAUDIO.MP3"
#define  AUDIO_DEFAULT_FILENAMEBUFFER "../../mmi_customer/Audio/Default.mp3"
#define  AUDIO_DEFAULT_FILEBUFFER         "________CUSTOMER__AUDIO__DEFAULT_MP3"
/* 101205 audio resource Calvin End */

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
#define ENFB_IMAGE_DATA_FILENAME  "../../mmi_customer/CustResource/CustENFBImgData"//100206 ENFB support
#endif

#ifdef MMI_ON_WIN32
#include <stdafx.h>
#endif 

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif /* 0 */ 
image_type_enum ExtImageFlag = SYSTEM_IMAGE;

// __CUSTPACK_MULTIBIN Calvin BEGIN
//BOOL ExtImageFlag=FALSE; //040805 CustPack: Calvin added for distinguish which set of images is populated
// __CUSTPACK_MULTIBIN Calvin END
/* 101205 audio resource Calvin Satrt */
BOOL ExtAudioFlag = FALSE;

/* 101205 audio resource Calvin End */

#ifdef DEVELOPER_BUILD_FIRST_PASS

static U8 is_first = 1;
static U32 image_idx = 1;

/* 101205 audio resource Calvin Satrt */
static U32 audio_idx = 1;

/* 101205 audio resource Calvin End */

void Extract_Name(S8 *dest, S8 *src, U32 res_type);
MMI_BOOL Iload(S8 *string, U16 *pType);

extern int BMPLoader(char *in_filename, char *out_filename, S8 color_depth);
extern MMI_BOOL Aload(S8 *string, PU16 pType);

extern CUSTOM_STRING *gpStringRes;
extern CUSTOM_STRING_MAP *gpStringMapRes;

extern FILE *StringListFile;
extern FILE *EnumListFile;

U8 StrIsPopulate[MAX_STRING_MAP_SIZE];


extern FILE *OutputImgIDFile;
extern FILE *OutputMenutreeIDFile;
extern FILE *RepeatIDListFile;


/* 101806 E-NFB start */
#include "imagegetdimension.h"
/* 101806 E-NFB end */

#define _POPULATE_RES_C

S32 g_ressize = 0;
S8 g_resmenu_filename[MAX_FILENAME_LEN] = "";

#ifdef _POPULATE_RES_C
extern mmi_resource_base_struct g_mmi_resource_base_table[];
extern FILE *fpMenuResourceUsage;
extern FILE *fpImageResourceUsage;
extern FILE *fpAudioResourceUsage;
extern FILE *fpStringResourceUsage;

U32 g_resimg_count = 0;
U32 g_resado_count = 0;
CUSTOM_RESOURCE_SIZE g_resimg_table[MAX_IMAGE_IDS_SIZE];
CUSTOM_RESOURCE_SIZE g_resado_table[MAX_AUDIO_IDS_SIZE];
#endif /* _POPULATE_RES_C */

/* decalred in the CustImageRes.c */
extern CUSTOM_IMAGE nCustImageNames[];
extern IMAGENAME_LIST ImageNameList[];
extern CUSTOM_IMAGE_MAP ImageIdMap[];
extern CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMap[];

/* __CUSTPACK_MULTIBIN Calvin BEGIN */
/* 040805 CustPack: Calvn added */
extern IMAGENAME_LIST ImageNameListEXT[];
extern CUSTOM_IMAGE_MAP ImageIdMapEXT[];
extern CUSTOM_IMAGE_SEARCH_MAP ImageIdSearchMapEXT[];

/* Calvin end */
/* __CUSTPACK_MULTIBIN Calvin END */

/* 101205 audio resource Calvin Satrt */
extern FILE *OutputAdoIDFile;

/* decalred in the CustAudioRes.c */
extern CUSTOM_AUDIO nCustAudioNames[];
extern AUDIONAME_LIST AudioNameList[];
extern CUSTOM_AUDIO_MAP AudioIdMap[];
extern CUSTOM_AUDIO_SEARCH_MAP AudioIdSearchMap[];
extern AUDIONAME_LIST AudioNameListEXT[];
extern CUSTOM_AUDIO_MAP AudioIdMapEXT[];
extern CUSTOM_AUDIO_SEARCH_MAP AudioIdSearchMapEXT[];

/* 101205 audio resource Calvin End */

/* decalred in the CustScreenData.c */
extern CUSTOM_SCREEN nCustScreens[MAX_SCREEN_IDS];

/* declared in the CustMenuDataRes.c */
#ifdef MMI_ON_HARDWARE_P
extern const CUSTOM_MENU nCustMenus[MAX_MENU_ITEMS];
#else 
extern CUSTOM_MENU nCustMenus[MAX_MENU_ITEMS];
#endif 

/* Global Variables */
/* 101205 audio resource Calvin Satrt */
S8 filename_buffer[BUFFER_SIZE];
S8 org_filename_buffer[BUFFER_SIZE];
S8 tempBMPFile[BUFFER_SIZE];//070306 Alpha layer

S8 autofilename_buffer[BUFFER_SIZE];
S8 number_string[BUFFER_SIZE1];
S8 autoimagename_buffer[BUFFER_SIZE];
FILE *output_data_file = NULL;
FILE *image_data_file = NULL;
FILE *nfb_img_res_file = NULL;
S8 *output_image_names = NULL;
S32 n_supported_extensions = 1;

FILE *audio_data_file = NULL;
S8 *output_audio_names = NULL;

/* 101205 audio resource Calvin End */

/* 120704 Calvin added */
#define SUPPORT_N_MEDIA_TYPE     3
S8 *support_media_ext[SUPPORT_N_MEDIA_TYPE] = { "AVI", "3GP", "MP4" };

/* Calvin end */

/* __CUSTPACK_MULTIBIN Calvin BEGIN */
/* 040805 CustPack: Calvin added */
FILE *dest_file = NULL;
int toolFlag = 0;       /* decide if this is run by CustPack tool */
int resultFailFlag = 0; /* decide if this is run by CustPack tool */

/* Calvin end */
/* __CUSTPACK_MULTIBIN Calvin END */

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
FILE *enfb_img_data_file = NULL;//100206 ENFB support
int enfbFlag = 0;
int disableENFB = 0;
U32 enfb_offset = 0;
U32 enfb_size = 0;
#endif

/* 101205 audio resource Calvin modifiied */
typedef struct Resource_Info
{
    S8 *filename_ext;
    U8 file_type;
} RESOURCE_INFO;

#define MAX_SEQUENTIAL_IMAGE_FRAME        512
#define SUPPORT_N_SEQUENTIAL_IMAGE_TYPE      3
RESOURCE_INFO sequential_image[SUPPORT_N_SEQUENTIAL_IMAGE_TYPE] = { {"BMP", IMAGE_TYPE_BMP_SEQUENCE},
{"JPG", IMAGE_TYPE_JPG_SEQUENCE},
{"PNG", IMAGE_TYPE_PNG_SEQUENCE}
};  /* 101205 audio resource Calvin */

RESOURCE_INFO image_resource_info[] = 
{
    {"BMP", IMAGE_TYPE_BMP},
    {"PBM", IMAGE_TYPE_DEVICE_BITMAP},//070306 Alpha layer
    {"GIF", IMAGE_TYPE_GIF},
    {"JPG", IMAGE_TYPE_JPG},
    {"AVI", IMAGE_TYPE_AVI},
    {"3GP", IMAGE_TYPE_3GP},
    {"MP4", IMAGE_TYPE_MP4},
    {"PNG", IMAGE_TYPE_PNG},
    {"PPN", IMAGE_TYPE_PPN},//070306 Alpha layer
    {"M3D", IMAGE_TYPE_M3D},
    {"CMP", IMAGE_TYPE_SWFLASH}
};

RESOURCE_INFO audio_resource_info[] = { {"MP3", AUDIO_TYPE_MP3},
{"IMY", AUDIO_TYPE_IMY},
{"MID", AUDIO_TYPE_MID},
{"WAV", AUDIO_TYPE_WAV},
{"MMF", AUDIO_TYPE_MMF},
{"PCM", AUDIO_TYPE_PCM},
{"DVI", AUDIO_TYPE_DVI},
{"AMR", AUDIO_TYPE_AMR},
{"AAC", AUDIO_TYPE_AAC},
{"WMA", AUDIO_TYPE_WMA},
{"M4A", AUDIO_TYPE_M4A}
};

/* 101205 audio resource Calvin end */

S32 sequential_interval[MAX_SEQUENTIAL_IMAGE_FRAME];
S32 sequential_filesize[MAX_SEQUENTIAL_IMAGE_FRAME];
S32 sequential_offset[MAX_SEQUENTIAL_IMAGE_FRAME];
S32 sequential_extension_type;  /* 031705 Calvin modified */
S32 sequential_frame;

/* Calvin end */

extern S8 CustImgPath[];
extern S8 CustAdoPath[];    /* 101205 audio resource Calvin Add */

#ifdef __UCS2_ENCODING
U16 AnsiiToUnicodeString(S8 *pOutBuffer, S8 *pInBuffer);
#endif 
extern HWND hWnd;

U16 CurrMaxStringId;
extern U16 CurrMaxImageNum;
extern U16 CurrMaxImageId;
extern U16 CurrMaxMenuItemIndex;

/* __CUSTPACK_MULTIBIN Calvin BEGIN */
/* 040805 CustPack: Calvin added */
extern U16 CurrMaxImageNumEXT;
extern U16 CurrMaxImageIdEXT;

/* Calvin end */
/* __CUSTPACK_MULTIBIN Calvin END */

/* 101205 audio resource Calvin Satrt */
extern U16 CurrMaxAudioNum;
extern U16 CurrMaxAudioId;
extern U16 CurrMaxAudioNumEXT;
extern U16 CurrMaxAudioIdEXT;

/* 101205 audio resource Calvin End */

#ifdef __POPULATE_ENGINE_
void AddStringInfo(int StrId, char *pString, char *pStringDisplay);
void AddImageInfo(int ImgId, char *pImage, char *pIMageDisplay);
void AddApplicationInfo(U32 nId, S8 *pName, U32 nMinId, U32 nMaxId);
#endif /* __POPULATE_ENGINE_ */ 

/* 101806 E-NFB start */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
MMI_BOOL ENFBAssociatedIDFlag = MMI_FALSE;
MMI_BOOL ENFBAssociatedIDAdded = MMI_FALSE;
extern unsigned short CurrMaxENFBAssociatedIDNum;
extern CUSTOM_ENFB_STR ENFBAssociatedIDList[];
#endif
/* 101806 E-NFB end */
#ifndef __CYGWIN__
#include <dirent.h>
void GenOrgPath(const char* in, char* out);
#endif //__WINE__
extern FILE* FOPEN(const char*, const char*);

/* 101806 E-NFB start */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
/*****************************************************************************
 * FUNCTION
 *  SetENFBAssociatedID
 * DESCRIPTION
 *  start to record associated ID list
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL		MMI_TRUE or MMI_FALSE
 *****************************************************************************/
MMI_BOOL SetENFBAssociatedID(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* if flag = false, set flag = true, allocate memory, and no++ */
	if (ENFBAssociatedIDFlag == MMI_FALSE)
	{
		CUSTOM_ENFB_STR *enfb;
        int str_size = 256;
		int i;

		ENFBAssociatedIDFlag = MMI_TRUE;

		CurrMaxENFBAssociatedIDNum++;

		enfb = &ENFBAssociatedIDList[CurrMaxENFBAssociatedIDNum];
		enfb->count = 0;
		enfb->IDs = (U16 *)malloc(MAX_ASSOCIATED_LIST_IDS*sizeof(U16));
		enfb->Ptr = (S8 **)malloc(MAX_ASSOCIATED_LIST_IDS*sizeof(S8*));
		for (i = 0; i < MAX_ASSOCIATED_LIST_IDS ; i++)
		{
			enfb->Ptr[i] = (S8 *)malloc(str_size*sizeof(S8));
		}

		return MMI_TRUE;
	}
	/* error: cannot set flag twice */
	else
	{
		return MMI_FALSE;
	}
}


/*****************************************************************************
 * FUNCTION
 *  AddENFBAssociatedIDItem
 * DESCRIPTION
 *  add item to current associated list
 * PARAMETERS
 *  ImageID		[IN]
 *  filename	[IN]
 * RETURNS
 *  MMI_BOOL		MMI_TRUE or MMI_FALSE
 *****************************************************************************/
MMI_BOOL AddENFBAssociatedIDItem(U16 ImageID, S8 *filename, U32 type)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* if flag = true, record this item: imagd ID & pointer variable */
	if (ENFBAssociatedIDFlag == MMI_TRUE)
	{
		CUSTOM_ENFB_STR *enfb;
        int i;

		enfb = &ENFBAssociatedIDList[CurrMaxENFBAssociatedIDNum];
		enfb->IDs[enfb->count] = ImageID;
		Extract_Name(enfb->Ptr[enfb->count], filename, type);
        for (i = 0; i < strlen(enfb->Ptr[enfb->count]); i++)
        {
            enfb->Ptr[enfb->count][i] = toupper(enfb->Ptr[enfb->count][i]);
        }
		enfb->count++;

		return MMI_TRUE;
	}
	/* if flag = false, do not record this item */
	else
	{
		return MMI_FALSE;
	}
}


/*****************************************************************************
 * FUNCTION
 *  ResetENFBAssociatedID
 * DESCRIPTION
 *  end of one associated ID list
 * PARAMETERS
 *  void
 * RETURNS
 *  MMI_BOOL		MMI_TRUE or MMI_FALSE
 *****************************************************************************/
MMI_BOOL ResetENFBAssociatedID(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	/* if flag = true, set flag = false */
	if (ENFBAssociatedIDFlag == MMI_TRUE)
	{
		ENFBAssociatedIDFlag = MMI_FALSE;

		/* if current associated list is empty, reuse it for next list */
		if (ENFBAssociatedIDList[CurrMaxENFBAssociatedIDNum].count == 0)
		{
			CurrMaxENFBAssociatedIDNum--;
		}

		return MMI_TRUE;
	}
	/* error: can not set flag = false twice */
	else
	{
		return MMI_FALSE;
	}
}


/*****************************************************************************
 * FUNCTION
 *  GetCurrENFBAssociatedID
 * DESCRIPTION
 *  end of one associated ID list
 * PARAMETERS
 *  void
 * RETURNS
 *  U32		current associated ID
 *****************************************************************************/
U16 GetCurrENFBAssociatedID(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
	if (ENFBAssociatedIDFlag == MMI_TRUE)
	{
	    return CurrMaxENFBAssociatedIDNum;
	}
	else
	{
        return 0;
	}
}


/*****************************************************************************
 * FUNCTION
 *  IfItemExistENFBAssociatedID
 * DESCRIPTION
 *  test if the ImageID has already existed in one associated ID list
 * PARAMETERS
 *  ImageID     [IN]
 * RETURNS
 *  MMI_BOOL	MMI_TRUE or MMI_FALSE
 *****************************************************************************/
MMI_BOOL IfItemExistENFBAssociatedID(S8 *filename, U32 type)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
    int i, j, k;
    int str_size = 256;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/
    for (i = 1; i <= (int)CurrMaxENFBAssociatedIDNum; i++)
    {
		CUSTOM_ENFB_STR *enfb;
        S8 *ptr;

		enfb = &ENFBAssociatedIDList[i];
        for (j = 0; j <= (int)enfb->count; j++)
        {
            ptr = (S8 *)malloc(str_size*sizeof(S8));
            Extract_Name(ptr, filename, type);
            for (k = 0; k < strlen(ptr); k++)
            {
                ptr[k] = toupper(ptr[k]);
            }
            if (strcasecmp((char *)ptr, (char *)enfb->Ptr[j]) == 0)
            {
                return MMI_TRUE;
            }
        }
    }
    return MMI_FALSE;
}
#endif
/* 101806 E-NFB end */


#if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE)
U32 GetOffsetFromResourceBase(U16 ImageID)
{
    U32 i = 0;
    
    while (1)
    {
        if (strncmp(g_mmi_resource_base_table[i].appname, "END", 3) == 0)
        {
            break;
        }
        else if ((ImageID >= g_mmi_resource_base_table[i].min) &&
                 (ImageID < g_mmi_resource_base_table[i].max))
        {
            return i;
        }
        else
        {
            i++;
        }
    }

    return 0xFFFFFFFF;
}


#define RESPRT(fp, app, base, ID, strID, size, data, resfile)   \
    do                                                          \
    {                                                           \
        fprintf(fp, "%s\t%d\t%d\t%s\t%d\t%s\t%s\n",             \
                app, base, ID, strID, size, data, resfile);     \
    } while (0)


#define RESPRTONERECORD(fp, nImgId, data, enum_value, resfile)      \
    do                                                              \
    {                                                               \
        if (fp != NULL)                                             \
        {                                                           \
            U32 offset = GetOffsetFromResourceBase(nImgId);         \
            if (offset == 0xFFFFFFFF)                               \
            {                                                       \
                RESPRT(fp, "NO_APP", -1, nImgId, enum_value,        \
                       g_ressize, data, resfile);                   \
            }                                                       \
            else                                                    \
            {                                                       \
                RESPRT(fp,                                          \
                       g_mmi_resource_base_table[offset].appname,   \
                       g_mmi_resource_base_table[offset].min,       \
                       nImgId, enum_value, g_ressize, data,         \
                       resfile);                                    \
            }                                                       \
        }                                                           \
    } while (0)
#endif /* #if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE) */


/*****************************************************************************
 * FUNCTION
 *  InitStrPopulate
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void InitStrPopulate()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < MAX_STRING_MAP_SIZE; i++)
    {
        StrIsPopulate[i] = 0;
    }

}



/*****************************************************************************
 * FUNCTION
 *  ConvertToPrintable
 * DESCRIPTION
 *  
 * PARAMETERS
 *  original        [?]     
 *  converted       [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void ConvertToPrintable(S8 *original, S8 *converted)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i = 0, j = 0, strlength = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    strlength = wcslen((wchar_t*) original);
    //strlength = strlen((wchar_t*) original);
    for (i = 0; i < strlength * 2; i++)
    {
        if (original[i] == 10)
        {
            converted[j] = 92;
            converted[++j] = 0;
            converted[++j] = 'n';
            j++;
        }
        else
        {
            converted[j++] = original[i];
        }
    }
    if (strlength == 0)
    {   /* If NULL string, fill in " " */
        converted[0] = 20;
        converted[1] = '\0';
        converted[2] = '\0';
        converted[3] = '\0';
    }
    else
    {
        converted[j++] = '\0';
        converted[j] = '\0';
    }
}




/*****************************************************************************
 * FUNCTION
 *  SetStringWithDisplay
 * DESCRIPTION
 *  
 * PARAMETERS
 *  nStrId          [IN]        
 *  pString1        [?]         
 *  pDisplay1       [?]         
 *  enum_value      [?]         
 * RETURNS
 *  void
 *****************************************************************************/
void SetStringWithDisplay(U16 nStrId, S8 *pString1, S8 *pDisplay1, S8 *enum_value, S8 *resfile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i;
    U16 found = 0;
    S8 *pString = NULL;
    S8 *pDisplay = NULL;
    ResourceInfo sInfo;

    S8 *sString = NULL;
    U16 StrTotalLen = 0;
    U16 nLen = 0;

    S8 cString[1024], cDisplay[1024];
    S8 *pEnum_value = NULL;
	char tabUnicode[8], StrIDbuf[1024],StrIDbuf1[1024];
		int characterSize=0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    g_ressize = 1;  //strlen(pString1);
#ifdef _POPULATE_RES_C
    if (strstr(pString1, "\n") == NULL)
    {
        RESPRTONERECORD(fpStringResourceUsage, nStrId, pString1, enum_value, resfile);
    }
    else
    {
        RESPRTONERECORD(fpStringResourceUsage, nStrId, "N/A", enum_value, resfile);
    }
#endif /* _POPULATE_RES_C */

#ifdef __ASCII

    pString = pString1;
    pDisplay = pDisplay1;
#endif /* __ASCII */ 

#ifdef __UCS2_ENCODING

    pString = OslMalloc((strlen(pString1) + 1) * 2);
    AnsiiToUnicodeString(pString, pString1);

   	
    pDisplay = OslMalloc((strlen(pDisplay1) + 1) * 2);
    AnsiiToUnicodeString(pDisplay, pDisplay1);

    pEnum_value = OslMalloc((strlen(enum_value) + 1) * 2);
    AnsiiToUnicodeString(pEnum_value, enum_value);

#endif /* __UCS2_ENCODING */ 
    /* Leo remove, no need to set limit */
    //      if(CurrMaxStringId >= MAX_STRING_IDS) return;   /* Can't add more than max number of string ids allowed */
    //      if (gpStringMapRes[nStrId]==(U16)-1) 
    if (StrIsPopulate[nStrId] == 0)
    {
        CurrMaxStringId++;
        StrIsPopulate[nStrId] = 1;
        ConvertToPrintable(pString, cString);
        ConvertToPrintable(pDisplay, cDisplay);
	#if 1

	tabUnicode[0] = '\t';
	tabUnicode[1] = '\0';
	tabUnicode[2] = '\0';
	tabUnicode[3] = '\0';
    
	characterSize=UCS2Strlen(pEnum_value)*ENCODING_LENGTH;
	fwrite(pEnum_value,characterSize,1,StringListFile);
	fwrite(tabUnicode,2,1,StringListFile);       

	sprintf(StrIDbuf,"%d\t",   nStrId);
      AnsiiToUnicodeString(StrIDbuf1,StrIDbuf);

	characterSize=UCS2Strlen(StrIDbuf1)*ENCODING_LENGTH;

	fwrite(StrIDbuf1,characterSize,1,StringListFile);       
	
	characterSize=UCS2Strlen(cDisplay)*ENCODING_LENGTH;
	fwrite(cDisplay,characterSize,1,StringListFile);
	fwrite(tabUnicode,2,1,StringListFile);       

	characterSize=UCS2Strlen(cString)*ENCODING_LENGTH;
	fwrite(cString,characterSize,1,StringListFile);
	tabUnicode[0] = '\r';
	tabUnicode[1] = '\0';
	tabUnicode[2] = '\n';
	tabUnicode[3] = '\0';
	fwrite(tabUnicode,4,1,StringListFile);       


//	sprintf(tmp,"%ls\t%d\t%ls\t%d%ls\r\n",    (wchar_t*) pEnum_value,
//            nStrId,
//            (wchar_t*) cDisplay, characterSize,tmp1,
//            (wchar_t*) cString);

//	characterSize=AnsiiToUnicodeString(tmp1,tmp);
	
//	fwrite(tmp1,characterSize-2,1,StringListFile);
	#else
        fwprintf(
            StringListFile,
            L"%ls\t%d\t%ls\t%ls\r\n",
            (wchar_t*) pEnum_value,
            nStrId,
            (wchar_t*) cDisplay,
            (wchar_t*) cString);
#endif
        fprintf(EnumListFile, "%s,\n", enum_value);
    }
    else    
    {
        fprintf(RepeatIDListFile, "%d\t%s\t\t%s\n", nStrId, enum_value, "String");
    }
}



/*****************************************************************************
 * FUNCTION
 *  AddApp
 * DESCRIPTION
 *  Function to Add a new application into the framework
 * PARAMETERS
 *  nId         [IN]        App Id
 *  pName       [IN]        Application name.
 *  nMinId      [IN]        Minimum Id value supported
 *  nMaxId      [IN]        Maximum Id value
 * RETURNS
 *  void
 *****************************************************************************/
void AddApp(U32 nId, S8 *pName, U16 nMinId, U16 nMaxId)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    ResourceInfo sInfo;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    sInfo.nAppId = nId;
    sInfo.nMaxId = nMaxId;
    sInfo.nMinId = nMinId;
    sInfo.pValue = pName;
}


/*****************************************************************************
 * FUNCTION
 *  SetString
 * DESCRIPTION
 *  Function to Update the Customizable Resource String & Mapping Array.
 * PARAMETERS
 *  StrId           [IN]        
 *  String1         [?]         
 *  enum_value      [?]         
 *  U16(?)          [OUT]       String Number which is Mapped with the StringId.
 * RETURNS
 *  StringNumber which is mapped with the StringID.(?)
 *****************************************************************************/
void SetString(U16 StrId, S8 *String1, S8 *enum_value, S8 *resfile)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* __CUSTPACK_MULTIBIN Calvin BEGIN */
    SetStringWithDisplay(StrId, String1, String1, enum_value, resfile);  /* 040805 CustPack: Calvin revised */
    /* __CUSTPACK_MULTIBIN Calvin END */
}



/*****************************************************************************
 * FUNCTION
 *  SetImage
 * DESCRIPTION
 *  Function to Update the Customizable Image Filename & Mapping Array.
 * PARAMETERS
 *  ImgId           [IN]        
 *  filename        [?]         
 *  enum_value      [?]         
 *  res_type        [IN]        
 *  U16(?)          [OUT]       Image Number which is Mapped with the ImageId.
 * RETURNS
 *  Image Number which is mapped with the ImageId.(?)
 *****************************************************************************/
void SetImage(U16 ImgId, S8 *filename, S8 *enum_value, U32 res_type, S8 *resfile)  /* 120304 Calvin modified for media resource */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* __CUSTPACK_MULTIBIN Calvin BEGIN */
    SetImageWithBuffer(ImgId, filename, "", enum_value, res_type, FALSE, resfile);       /* 040805 CustPack: Calvin revised */
    /* __CUSTPACK_MULTIBIN Calvin END */
}


/*****************************************************************************
 * FUNCTION
 *  SetNFBImage
 * DESCRIPTION
 *  
 * PARAMETERS
 *  filename        [?]     
 * RETURNS
 *  void
 *****************************************************************************/
void SetNFBImage(S8 *filename)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 nType;
    S16 nStrlen;
    S8 upperFileName[MAX_FILENAME_LEN];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (strlen(filename) >= MAX_FILENAME_LEN)
    {
        filename[MAX_FILENAME_LEN - 1] = '\0';
    }
    if (!(strcasecmp(filename, "")))
    {
        strcpy(filename_buffer, NOIMAGE_FILENAMEBUFFER);

    }
    nStrlen = strlen(filename);

    while (nStrlen >= 0)
    {
        upperFileName[nStrlen] = toupper(filename[nStrlen]);
        --nStrlen;
    }

    ExtImageFlag = NFB_PROGRESS_IMAGE;
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    disableENFB = 1;//100206 ENFB support
#endif
    Iload(upperFileName, &nType);
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    disableENFB = 0;
#endif

}


/*****************************************************************************
 * FUNCTION
 *  SetImageWithBuffer
 * DESCRIPTION
 *  Function to copy the image file to buffer
 * PARAMETERS
 *  nImgId              [IN]        
 *  filename            [?]         
 *  pDisplayName        [?]         
 *  enum_value          [?]         
 *  res_type            [IN]        
 *  imgFlag             [IN]        
 *  S8(?)               [OUT]       -> filename of the image to be displayed.
 *  U16(?)              [IN]        -> ImageId
 * RETURNS
 *  void
 *****************************************************************************/
extern FILE *usedImageList;
char currentResFile[50];
void SetImageWithBuffer(U16 nImgId, S8 *filename, S8 *pDisplayName, S8 *enum_value, U32 res_type, BOOL imgFlag, S8 *resfile)      /* 120304 Calvin modified for media resource */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i;
    U16 found = 0;
    S16 nStrlen;
    ResourceInfo sInfo;
    U16 nType;
    S8 upperFileName[MAX_FILENAME_LEN];
    unsigned short *pCurrMaxImageId;
    unsigned short *pCurrMaxImageNum;
    IMAGENAME_LIST *pImageNameList;
    CUSTOM_IMAGE_MAP *pImageIdMap;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (imgFlag == TRUE)
    {
        ExtImageFlag = EXT_IMAGE;
        pCurrMaxImageId = &CurrMaxImageIdEXT;
        pCurrMaxImageNum = &CurrMaxImageNumEXT;
        pImageNameList = ImageNameListEXT;
        pImageIdMap = ImageIdMapEXT;
    #ifdef __MMI_RESOURCE_ENFB_SUPPORT__
        #ifdef MULTIPLE_BINARY_FILES
        disableENFB = 1;//100206 ENFB support
        #endif
    #endif
    }
    else
    {
        ExtImageFlag = SYSTEM_IMAGE;
        pCurrMaxImageId = &CurrMaxImageId;
        pCurrMaxImageNum = &CurrMaxImageNum;
        pImageNameList = ImageNameList;
        pImageIdMap = ImageIdMap;
    }

    if (*pCurrMaxImageId >= MAX_IMAGE_IDS_SIZE)
    {
        return; /* Can't add more than max number of image ids allowed */
    }

    fprintf(OutputImgIDFile, "file name%d = %s \n", image_idx++, filename);

    if (strlen(filename) >= MAX_FILENAME_LEN)
    {
        filename[MAX_FILENAME_LEN - 1] = '\0';
    }
    if (!(strcasecmp(filename, "")))
    {
        strcpy(filename_buffer, NOIMAGE_FILENAMEBUFFER);

    }
    nStrlen = strlen(filename);

    while (nStrlen >= 0)
    {
        upperFileName[nStrlen] = toupper(filename[nStrlen]);
        --nStrlen;
    }

    for (i = 0; i < *pCurrMaxImageNum; i++)
    {
        if (!strcasecmp(pImageNameList[i].filename, upperFileName))
        {
            found = 1;
            break;
        }
    }

    if (pImageIdMap[nImgId].nImageId != 0)  
    {
        switch (res_type)
        {
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

    g_ressize = 0;

    if (found)
    {
        pImageIdMap[nImgId].nImageId = nImgId;
        pImageIdMap[nImgId].nImageNum = i;
        (*pCurrMaxImageId)++;
    #ifdef __MMI_RESOURCE_ENFB_SUPPORT__//100206 ENFB support
        if ((disableENFB == 0) && (ENFBAssociatedIDFlag == MMI_TRUE))
        {
            if (IfItemExistENFBAssociatedID(upperFileName, res_type))//already in other associated group
            {
                AddENFBAssociatedIDItem(nImgId, upperFileName, res_type);
            }
            else
            {
                fprintf(RepeatIDListFile, "\n\n!!!!!!!!!!!!%d\t%s\t\t%s!!!!!!!!!!!!!\n\n", nImgId, enum_value, upperFileName);
            }
        }
    #endif

#if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE)
        {
            S8 str1[MAX_IMAGE_NAMES_SIZE];
            S8 str2[MAX_IMAGE_NAMES_SIZE];
            U32 i, j;

            strcpy(str1, filename);
            for (j = 0; j < strlen(str1); j++)
            {
                str1[j] = toupper(str1[j]);
            }
            for (i = 0; i < g_resimg_count; i++)
            {
                strcpy(str2, g_resimg_table[i].filename);
                for (j = 0; j < strlen(str1); j++)
                {
                    str2[j] = toupper(str2[j]);
                }
                if (strcasecmp(str1, str2) == 0)
                {
                    g_ressize = g_resimg_table[i].size;
                    break;
                }
            }
        }
        RESPRTONERECORD(fpImageResourceUsage, nImgId, filename, enum_value, resfile);
#endif /* #if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE) */
    }
    else
    {
    #if 0
        if (res_type == RES_JAVA_GAME)
        {
            printf("Populating Java Game Resource\n");
            FILE *data_file = FOPEN(upperFileName, "rb+");

            output_data_file = FOPEN(JAVA_GAME_FILENAME, "ab+");
            if (data_file == NULL)
            {

            }
            else
            {
                U8 ch = 0, count = 0;
                S32 data;

                printf("Open data file successfully\n");
                Extract_Name(output_image_names, upperFileName, RES_JAVA_GAME);
                fprintf(output_data_file, "const U8 ");
                fprintf(output_data_file, output_image_names);
                fprintf(output_data_file, "[]={\n\t");
                while (!feof(data_file))
                {
                    ch = fgetc(data_file);
                    count++;
                    fprintf(output_data_file, "0x%02X,", ch);
                    if (count == 16)
                    {
                        fprintf(output_data_file, "\n\t");
                        count = 0;
                    }
                }
                fprintf(output_data_file, "\n};");
                pImageIdMap[nImgId].nImageId = nImgId;
                strcpy(pImageNameList[*pCurrMaxImageNum].filename, upperFileName);
                pImageIdMap[nImgId].nImageNum = *pCurrMaxImageNum;
                (*pCurrMaxImageNum)++;
                (*pCurrMaxImageId)++;
                fclose(output_data_file);
                fclose(data_file);
            }
        }
        else 
	#endif
	fprintf(usedImageList,"%s:%s\n",currentResFile,filename);
	if (Iload(upperFileName, &nType))
        {
            pImageIdMap[nImgId].nImageId = nImgId;
            strcpy(pImageNameList[*pCurrMaxImageNum].filename, upperFileName);
            pImageIdMap[nImgId].nImageNum = *pCurrMaxImageNum;
            (*pCurrMaxImageNum)++;
            (*pCurrMaxImageId)++;
        #ifdef __MMI_RESOURCE_ENFB_SUPPORT__//100206 ENFB support
            if ((disableENFB == 0) && (ENFBAssociatedIDFlag == MMI_TRUE))
            {
                if (ENFBAssociatedIDAdded == MMI_TRUE)//already in other associated group
                {
                    AddENFBAssociatedIDItem(nImgId, upperFileName, res_type);
                    ENFBAssociatedIDAdded = MMI_FALSE;
                }
            }
        #endif

#if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE)
            g_resimg_table[g_resimg_count].size = g_ressize;
            strcpy(g_resimg_table[g_resimg_count].filename, filename);
            g_resimg_count ++;
            RESPRTONERECORD(fpImageResourceUsage, nImgId, filename, enum_value, resfile);
#endif /* #if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE) */
        }
        /* __CUSTPACK_MULTIBIN Calvin BEGIN */
        else    /* 040805 CustPack: Calvin modified else case to let new set of images share the same resource of default image */
        {
            if (toolFlag)
            {
                resultFailFlag = 1;
            }
            ImageIdMap[nImgId].nImageId = nImgId;
            strcpy(ImageNameList[CurrMaxImageNum].filename, DEFAULT_FILEBUFFER);
            ImageIdMap[nImgId].nImageNum = CurrMaxImageNum;
            CurrMaxImageNum++;
            CurrMaxImageId++;
        }
        /* __CUSTPACK_MULTIBIN Calvin END */
    }
    sInfo.pDisplayValue = pDisplayName;
    sInfo.pValue = upperFileName;
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    disableENFB = 0;//100206 ENFB support
#endif
}

/* 101205 audio resource Calvin Satrt */


/*****************************************************************************
 * FUNCTION
 *  SetAudioWithBuffer
 * DESCRIPTION
 *  Function to copy the audio file to buffer
 * PARAMETERS
 *  nAdoId              [IN]        
 *  filename            [?]         
 *  pDisplayName        [?]         
 *  enum_value          [?]         
 *  res_type            [IN]        
 *  adoFlag             [IN]        
 *  S8(?)               [OUT]       -> filename of the audio to be generated.
 *  U16(?)              [IN]        -> AudioId
 * RETURNS
 *  void
 *****************************************************************************/
void SetAudioWithBuffer(U16 nAdoId, S8 *filename, S8 *pDisplayName, S8 *enum_value, U32 res_type, BOOL adoFlag, S8 *resfile)      /* 120304 Calvin modified for media resource */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i;
    U16 found = 0;
    S16 nStrlen;
    U16 nType;
    S8 upperFileName[MAX_FILENAME_LEN];
    unsigned short *pCurrMaxAudioId;
    unsigned short *pCurrMaxAudioNum;
    AUDIONAME_LIST *pAudioNameList;
    CUSTOM_AUDIO_MAP *pAudioIdMap;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (adoFlag == TRUE)
    {
        ExtAudioFlag = TRUE;
        pCurrMaxAudioId = &CurrMaxAudioIdEXT;
        pCurrMaxAudioNum = &CurrMaxAudioNumEXT;
        pAudioNameList = AudioNameListEXT;
        pAudioIdMap = AudioIdMapEXT;
    }
    else
    {
        ExtAudioFlag = FALSE;
        pCurrMaxAudioId = &CurrMaxAudioId;
        pCurrMaxAudioNum = &CurrMaxAudioNum;
        pAudioNameList = AudioNameList;
        pAudioIdMap = AudioIdMap;
    }

    if (*pCurrMaxAudioId >= MAX_AUDIO_IDS_SIZE)
    {
        return; /* Can't add more than max number of audio ids allowed */
    }

    fprintf(OutputAdoIDFile, "file name%d = %s \n", audio_idx++, filename);

    if (strlen(filename) >= MAX_FILENAME_LEN)
    {
        filename[MAX_FILENAME_LEN - 1] = '\0';
    }
    if (!(strcasecmp(filename, "")))
    {
        strcpy(filename_buffer, NOAUDIO_FILENAMEBUFFER);

    }
    nStrlen = strlen(filename);

    while (nStrlen >= 0)
    {
        upperFileName[nStrlen] = toupper(filename[nStrlen]);
        --nStrlen;
    }

    for (i = 0; i < *pCurrMaxAudioNum; i++)
    {
        if (!strcasecmp(pAudioNameList[i].filename, upperFileName))
        {
            found = 1;
            break;
        }
    }

    if (pAudioIdMap[nAdoId].nAudioId != 0)
    {
        switch (res_type)
        {
            case RES_AUDIO:
                fprintf(RepeatIDListFile, "%d\t%s\t\t%s\n", nAdoId, enum_value, "Audio");
                break;
            default:
                printf("Non supported resource type!\n");
        }
    }

    g_ressize = 0;

    if (found)
    {
        pAudioIdMap[nAdoId].nAudioId = nAdoId;
        pAudioIdMap[nAdoId].nAudioNum = i;
        (*pCurrMaxAudioId)++;

#if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE)
        {
            S8 str1[MAX_AUDIO_NAMES_SIZE];
            S8 str2[MAX_AUDIO_NAMES_SIZE];
            U32 i, j;

            strcpy(str1, filename);
            for (j = 0; j < strlen(str1); j++)
            {
                str1[j] = toupper(str1[j]);
            }
            for (i = 0; i < g_resado_count; i++)
            {
                strcpy(str2, g_resado_table[i].filename);
                for (j = 0; j < strlen(str1); j++)
                {
                    str2[j] = toupper(str2[j]);
                }
                if (strcasecmp(str1, str2) == 0)
                {
                    g_ressize = g_resado_table[i].size;
                    break;
                }
            }
        }
        RESPRTONERECORD(fpAudioResourceUsage, nAdoId, filename, enum_value, resfile);
#endif /* #if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE) */
    }
    else
    {
        if (Aload(upperFileName, &nType))
        {
            pAudioIdMap[nAdoId].nAudioId = nAdoId;
            strcpy(pAudioNameList[*pCurrMaxAudioNum].filename, upperFileName);
            pAudioIdMap[nAdoId].nAudioNum = *pCurrMaxAudioNum;
            (*pCurrMaxAudioNum)++;
            (*pCurrMaxAudioId)++;

#if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE)
            g_resado_table[g_resado_count].size = g_ressize;
            strcpy(g_resado_table[g_resado_count].filename, filename);
            g_resado_count ++;
            RESPRTONERECORD(fpAudioResourceUsage, nAdoId, filename, enum_value, resfile);
#endif /* #if defined(_POPULATE_RES_C) && !defined(PRODUCTION_RELEASE) */
        }
        else
        {
            if (toolFlag)
            {
                resultFailFlag = 1;
            }
            AudioIdMap[nAdoId].nAudioId = nAdoId;
            strcpy(AudioNameList[CurrMaxAudioNum].filename, AUDIO_DEFAULT_FILEBUFFER);
            AudioIdMap[nAdoId].nAudioNum = CurrMaxAudioNum;
            CurrMaxAudioNum++;
            CurrMaxAudioId++;
        }
    }
}

/* 101205 audio resource Calvin End */

/************************************************************
**FUNCTION:		GetandWriteAudioResource
**DESCRIPTION:	
**
**
*************************************************************/
void GetandWriteAudioResource(const char* sAudioSourceName,const char * sAudioDataName)
{
	int tmp = 3;
	FILE *fpAudSource,*fpAudData;
	long data;

	{
		int i = 0;
		
		fpAudSource = fopen(sAudioSourceName,"rb");
		fpAudData = fopen(sAudioDataName,"wb");

		
		if(fpAudSource == NULL)
		{
			//printf("fpAudSource = 0x%08x\n",fpAudSource);
			//printf("fpAudData = 0x%08x\n",fpAudData);
			return;
		}
		fseek(fpAudSource,0,SEEK_SET);
		while(0 != fread(&data,1,sizeof(long),fpAudSource))
		{
			if((i != 0)&&(0 == i%8))
				fprintf(fpAudData,"\n");
			fprintf(fpAudData,"0x%08X,\t",data);
			data = 0;
			i++;
		}

		fflush(fpAudData);
		fclose(fpAudSource);
		fclose(fpAudData);
	}
}
/*****************************************************************************
 * FUNCTION
 *  MenuItemPresent
 * DESCRIPTION
 *  Function to find ImageId
 * PARAMETERS
 *  ItemId      [IN]        
 *  U16(?)      [IN]        -> ImageId
 * RETURNS
 *  U16
 *****************************************************************************/
U16 MenuItemPresent(U16 ItemId)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (i = 0; i < CurrMaxMenuItemIndex; i++)
    {
        if (nCustMenus[i].nMenuItemId == ItemId)
        {
            return TRUE;
        }
    }
    return FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  PrintMenuItems
 * DESCRIPTION
 *  Fills the CUSTOM_MENU structure, for MenuTree Tool usage
 * PARAMETERS
 *  arg1        [?]         
 *  U16(?)      [IN]        -> variable argument list
 * RETURNS
 *  void
 *****************************************************************************/
static char is_dummy_code = 0;  /* To check if it needs to generate for menu tree tool or not. */
void PrintMenuItems(char *arg1)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /* static U8 is_first=1; */
    FILE *fout = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  

    if (is_first)
    {
        fout = FOPEN(OUTPUT_MENUTREE_FILENAME, "w");
        is_first = 0;
    }
    else
    {
        fout = FOPEN(OUTPUT_MENUTREE_FILENAME, "a");
    }

    fprintf(fout, "ADD_APPLICATION_MENUITEM2(%s);\n", arg1);
    if(fout != NULL)
    { 
    fclose(fout);
        fout = NULL;
    }

}


/*****************************************************************************
 * FUNCTION
 *  PrintMenuItems2
 * DESCRIPTION
 *  Fills the CUSTOM_MENU structure, for MenuTree Tool usage
 * PARAMETERS
 *  arg1        [?]         
 *  U16(?)      [IN]        -> variable argument list
 * RETURNS
 *  void
 *****************************************************************************/
void PrintMenuItems2(char *arg1, ...)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /* static U8 is_first=1; */
    FILE *fout = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
  

    if (is_first)
    {
        fout = FOPEN(OUTPUT_MENUTREE_FILENAME, "w");
        is_first = 0;
    }
    else
    {
        fout = FOPEN(OUTPUT_MENUTREE_FILENAME, "a");
    }

    fprintf(fout, "ADD_APPLICATION_MENUITEM3(%s);\n", arg1);
    if(fout != NULL)
    { 
    fclose(fout);
        fout = NULL;
    }

}


/*****************************************************************************
 * FUNCTION
 *  SetMenuItem
 * DESCRIPTION
 *  Fills the CUSTOM_MENU structure
 * PARAMETERS
 *  arg1        [IN]        
 *  U16(?)      [IN]        -> variable argument list
 * RETURNS
 *  void
 *****************************************************************************/
void SetMenuItem(U16 arg1, ...)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    U16 MemberCounter = 9;
    U16 i = arg1;
    U16 j;
    U16 menuItemID;
    va_list argumentPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //FILE  *fout;
    //static U8 is_first=1;

    //if (is_first)
    //{
    //   fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"w");
    //   is_first= 0;
    //}

#ifdef _POPULATE_RES_C
    va_start(argumentPtr, arg1);
#endif /* _POPULATE_RES_C */

    RESPRT(fpMenuResourceUsage, "NO_APP", -1, i, "unknown", 1, "none", g_resmenu_filename);

    /* Need to decide whether menu item already present */
    if (!MenuItemPresent(i))
    {
        /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
        fprintf(OutputMenutreeIDFile, "ADD_APPLICATION_MENUITEM2((", arg1);
        /* fclose(fout); */

        is_dummy_code = 0;
        while (MemberCounter)
        {
            switch (MemberCounter)
            {
                case MENUITEM_ID_TYPE:
                    menuItemID = i - 1;

                    nCustMenus[menuItemID].nMenuItemId = i;
                    /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    /* fclose(fout); */
                    break;
                case PARENT_ITEM_ID_TYPE:
                    nCustMenus[menuItemID].nParentId = i;
                    /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    /* fclose(fout); */
                    break;
                case NUMBER_OF_MENUITEMS_TYPE:
                    nCustMenus[menuItemID].nNumofMenuItem = i;
                    /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    /* fclose(fout); */
                    break;
                case MENU_ITEM_ORDER_TYPE:
                    if (nCustMenus[menuItemID].nNumofMenuItem == 0)
                    {
                        nCustMenus[menuItemID].nOrderMenuItemId = (U16*) 0;
                    }
                    else
                    {
                        nCustMenus[menuItemID].nOrderMenuItemId = (U16*) malloc(sizeof(U16) * MENU_ITEM_SIZE);
                        for (j = 0; j < nCustMenus[menuItemID].nNumofMenuItem; j++)
                        {
                            /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                            fprintf(OutputMenutreeIDFile, "%d,", i);
                            /* fclose(fout); */
                            nCustMenus[menuItemID].nOrderMenuItemId[j] = i;
                            i = va_arg(argumentPtr, int);
                        }
                    }
                    break;
                case HIDE_FLAG_TYPE:
                    nCustMenus[menuItemID].nHideFlag = i;
                    /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    /* fclose(fout); */
                    break;
                case MOVEABLE_FLAG_TYPE:
                    nCustMenus[menuItemID].nMovable = i;
                    /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    /* fclose(fout); */
                    break;
                case DISPLAY_ATTRIBUTE_TYPE:
                    nCustMenus[menuItemID].nDispAttribute = i;
                    /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    /* fclose(fout); */
                    break;
                case MENU_ITEM_STRING_ID_TYPE:
                    nCustMenus[menuItemID].nStrId = i;
                    /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    /* fclose(fout); */
                    break;
                case MENU_ITEM_IMAGE_ID_TYPE:
                    nCustMenus[menuItemID].nImageId = i;
                    /* fout = FOPEN(OUTPUT_MENUTREE_ID_FILENAME,"a"); */
                    fprintf(OutputMenutreeIDFile, "%d));\n", i);
                    /* fclose(fout); */
                    break;
            }
            if (MemberCounter != MENU_ITEM_ORDER_TYPE)
            {
                i = va_arg(argumentPtr, int);
            }

            MemberCounter--;
        }
    }
    else
    {
        is_dummy_code = 1;
    }

    va_end(argumentPtr);
}


/*****************************************************************************
 * FUNCTION
 *  SetMenuItem2
 * DESCRIPTION
 *  Fills the CUSTOM_MENU structure
 * PARAMETERS
 *  arg1        [IN]        
 *  U16(?)      [IN]        -> variable argument list
 * RETURNS
 *  void
 *****************************************************************************/
void SetMenuItem2(U16 arg1, ...)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    U16 MemberCounter = 9;
    U16 i = arg1;
    U16 j;
    U16 menuItemID;
    va_list argumentPtr;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    va_start(argumentPtr, arg1);

#ifdef _POPULATE_RES_C
    RESPRT(fpMenuResourceUsage, "NO_APP", -1, i, "unknown", 1, "none", g_resmenu_filename);
#endif /* _POPULATE_RES_C */

    /* Need to decide whether menu item already present */
    if (!MenuItemPresent(i))
    {
        fprintf(OutputMenutreeIDFile, "ADD_APPLICATION_MENUITEM2((", arg1);

        is_dummy_code = 0;
        while (MemberCounter)
        {
            switch (MemberCounter)
            {
                case MENUITEM_ID_TYPE:
                    menuItemID = i - 1;

                    nCustMenus[menuItemID].nMenuItemId = i;
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    break;

                case PARENT_ITEM_ID_TYPE:
                    nCustMenus[menuItemID].nParentId = i;
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    break;

                case MENU_ITEM_ORDER_TYPE:
                    if ( /* nCustMenus[menuItemID].nNumofMenuItem==0 || */ i == MENU_ITEM_END)
                    {
                        nCustMenus[menuItemID].nOrderMenuItemId = (U16*) 0;
                        fprintf(OutputMenutreeIDFile, "0,");
                        i = va_arg(argumentPtr, int);
                    }
                    else
                    {
                        U16 number_of_child = 0;

                        nCustMenus[menuItemID].nOrderMenuItemId = (U16*) malloc(sizeof(U16) * MENU_ITEM_SIZE);
                        while (i < MENU_ITEM_END)
                        {
                            nCustMenus[menuItemID].nOrderMenuItemId[number_of_child] = i;
                            i = va_arg(argumentPtr, int);

                            number_of_child++;
                        }
                        nCustMenus[menuItemID].nNumofMenuItem = number_of_child;        /* Set number of child field */
                        fprintf(OutputMenutreeIDFile, "%d,", number_of_child);
                        for (i = 0; i < number_of_child; i++)
                        {
                            fprintf(OutputMenutreeIDFile, "%d,", nCustMenus[menuItemID].nOrderMenuItemId[i]);
                        }
                        i = va_arg(argumentPtr, int);   /* read in next argument. */
                    }
                    break;

                case HIDE_FLAG_TYPE:
                    nCustMenus[menuItemID].nHideFlag = i;
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    break;

                case MOVEABLE_FLAG_TYPE:
                    nCustMenus[menuItemID].nMovable = i;
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    break;

                case DISPLAY_ATTRIBUTE_TYPE:
                    nCustMenus[menuItemID].nDispAttribute = i;
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    break;

                case MENU_ITEM_STRING_ID_TYPE:
                    nCustMenus[menuItemID].nStrId = i;
                    fprintf(OutputMenutreeIDFile, "%d,", i);
                    break;

                case MENU_ITEM_IMAGE_ID_TYPE:
                    nCustMenus[menuItemID].nImageId = i;
                    fprintf(OutputMenutreeIDFile, "%d));\n", i);
                    break;
            }
            if (MemberCounter != MENU_ITEM_ORDER_TYPE)
            {
                i = va_arg(argumentPtr, int);
            }

            if (MemberCounter == PARENT_ITEM_ID_TYPE)   /* Skip NUMBER_OF_MENUITEMS_TYPE field */
            {
                MemberCounter -= 2;
            }
            else
            {
                MemberCounter--;
            }
        }
    }
    else
    {
        is_dummy_code = 1;
    }

    va_end(argumentPtr);
}


/*****************************************************************************
 * FUNCTION
 *  Get_Image_Identifier
 * DESCRIPTION
 *  Identifies the image , BMP , ppf etc
 *  
 *  This function will be refined later when multiple
 *  image type will be supported by the frame work
 *  rt. now it just a added hook to make the design complete
 * PARAMETERS
 *  extension_type      [IN]        
 * RETURNS
 *  U8
 *****************************************************************************/
U8 Get_Image_Identifier(S32 extension_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 ret;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (extension_type < 0)
    {
        ret = 0;
    }

    else if (extension_type == 0)
    {
        ret = 1;
    }

    return (U8) ret;

}


/*****************************************************************************
 * FUNCTION
 *  Load_Image_Data
 * DESCRIPTION
 *  Identifies the image , BMP , ppf etc
 * PARAMETERS
 *  offset                  [OUT]       File offset --> file offset
 *  output_file             [IN]        -> Name of the file to which buffer
 *  input_file              [IN]        ->  Name of the input image file
 *  pImgData                [OUT]       -> buffer containing image data
 *  image_identifier        [IN]        -> image identifier
 *  n_frames                [IN]        -> no of frames
 *  is(?)                   [IN]        To be written
 * RETURNS
 *  S32 --> file size
 *****************************************************************************/
/* __CUSTPACK_MULTIBIN Calvin BEGIN */
/* 040805 CustPack: Calvin modified */
S32 Load_Image_Data(
        U32 *offset,
        FILE *output_file,
        FILE *input_file,
        U8 *pImgData,
        U8 image_identifier,
        S32 n_frames)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 file_size, i, o = 0;
    U8 ch;
#ifdef COMPRESS_IMG_RESOURCE	
    U8 cmdbuff[100];
    FILE *tmpf=NULL;
    U32 file_size_lzma=0;
    U8 tmpname[100];
    U8 lzmaname[100];
	
    strcpy(tmpname,output_image_names);
    strcat(tmpname,".bin");
    strcpy(lzmaname,tmpname);
    strcat(lzmaname,".lzma");
    //tmpf=fopen(tmpname,"w+");
    tmpf=FOPEN(tmpname, "wb");
    sprintf(cmdbuff,"./compress/lzma/lzma -f -v -9 %s",tmpname);	
#endif
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    if (toolFlag)
    {
        fprintf(dest_file, "%c%c", image_identifier, n_frames);
        if (image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP ||
            image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
            image_identifier == IMAGE_TYPE_PNG || image_identifier == IMAGE_TYPE_M3D ||
            image_identifier == IMAGE_TYPE_SWFLASH)
        {
            fprintf(
                dest_file,
                "%c%c%c%c%c%c",
                file_size & 0xff,
                (file_size & 0xff00) >> 8,
                (file_size & 0xff0000) >> 16,
                (file_size & 0xff000000) >> 24,
                0,
                0);
        }
        else
        {
            fprintf(
                dest_file,
                "%c%c%c%c",
                file_size & 0xff,
                (file_size & 0xff00) >> 8,
                (file_size & 0xff0000) >> 16,
                (file_size & 0xff000000) >> 24);
        }
    }
    /* 100206 ENFB support Start */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    else if (enfbFlag == 1) /* write ENFB header to CustImgDataxxx.h and image header/data to ENFB image data file */
    {
        fprintf(enfb_img_data_file, "%c%c", image_identifier, n_frames);
        if (image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP ||
            image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
            image_identifier == IMAGE_TYPE_PNG || image_identifier == IMAGE_TYPE_M3D ||
            image_identifier == IMAGE_TYPE_SWFLASH)
        {
            fprintf(
                enfb_img_data_file,
                "%c%c%c%c%c%c",
                file_size & 0xff,
                (file_size & 0xff00) >> 8,
                (file_size & 0xff0000) >> 16,
                (file_size & 0xff000000) >> 24,
                0,
                0);
            enfb_size = 8 + file_size;
        }
        else
        {
            fprintf(
                enfb_img_data_file,
                "%c%c%c%c",
                file_size & 0xff,
                (file_size & 0xff00) >> 8,
                (file_size & 0xff0000) >> 16,
                (file_size & 0xff000000) >> 24);
            enfb_size = 6 + file_size;
        }
        
        /* write ENFB header to CustImgDataxxx.h */
        fprintf(
            output_file,
            "\n\t0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\t",
            (U8) 255,//type
            (U8) ENFBAssociatedIDAdded,//associated id
            (U8) 0,//reserved
            (U8) 0,//reserved
            (U8) (enfb_offset & 0xff),
            (U8) ((enfb_offset >> 8) & 0xff),
            (U8) ((enfb_offset >> 16) & 0xff),
            (U8) ((enfb_offset >> 24) & 0xff),
            (U8) (enfb_size & 0xff),
            (U8) ((enfb_size >> 8) & 0xff),
            (U8) ((enfb_size >> 16) & 0xff),
            (U8) ((enfb_size >> 24) & 0xff));
        
        enfb_offset += enfb_size;
    }
#endif
/* 100206 ENFB support End */   
    else
    {
        fprintf(output_data_file, "\n\t0x%02X, 0x%02X, ", image_identifier, n_frames);
        if (image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP ||
            image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
            image_identifier == IMAGE_TYPE_PNG || image_identifier == IMAGE_TYPE_M3D ||
            image_identifier == IMAGE_TYPE_SWFLASH)
        {
            fprintf(
                output_file,
                "0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x00, 0x00,\n\t",
                file_size & 0xff,
                (file_size & 0xff00) >> 8,
                (file_size & 0xff0000) >> 16,
                (file_size & 0xff000000) >> 24);
        }
        else
        {
            fprintf(
                output_file,
                "0x%02X, 0x%02X, 0x%02X, 0x%02X,\n\t",
                file_size & 0xff,
                (file_size & 0xff00) >> 8,
                (file_size & 0xff0000) >> 16,
                (file_size & 0xff000000) >> 24);
        }
    }

    pImgData[0] = (U8) image_identifier;
    pImgData[1] = (U8) n_frames;
    pImgData[2] = (U8) (file_size & 0xff);
    pImgData[3] = (U8) ((file_size & 0xff00) >> 8);
    pImgData[4] = (U8) ((file_size & 0xff0000) >> 16);
    pImgData[5] = (U8) ((file_size & 0xff000000) >> 24);
#ifdef COMPRESS_IMG_RESOURCE
	 fprintf(output_file,"\n#ifndef COMPRESS_IMG_RESOURCE\n\t");
#endif	

    for (i = 0; i < file_size; i++)
    {
        ch = fgetc(input_file);

        if (toolFlag)
        {
            fputc(ch, dest_file);
            (*offset)++;
            (pImgData)[i + 6] = ch;
        }
    /* 100206 ENFB support Start */
    #ifdef __MMI_RESOURCE_ENFB_SUPPORT__
        else if (enfbFlag == 1) /* write image data to ENFB image data file */
        {
            fputc(ch, enfb_img_data_file);
            (*offset)++;
            (pImgData)[i + 6] = ch;
        }
    #endif
    /* 100206 ENFB support End */   
        else
        {
            fprintf(output_file, "0x%02X, ", ch);
#ifdef COMPRESS_IMG_RESOURCE	
		fputc(ch, tmpf);
#endif			
            (*offset)++;
            (pImgData)[i + 6] = ch;
            o++;
            if ((o % 16) == 0)
            {
                fprintf(output_file, "\n\t");
            }
        }
    }
#ifdef COMPRESS_IMG_RESOURCE
    fprintf(output_file,"\n#else\n");
    fclose(tmpf);
    system(cmdbuff);
    tmpf=FOPEN(lzmaname, "rb");
    fseek(tmpf, 0, SEEK_END);
    file_size_lzma = ftell(tmpf);
    fseek(tmpf, 0, SEEK_SET);
    if (file_size <= file_size_lzma)
    {
     	    fseek(input_file, 0, SEEK_SET);
	    o=0;
	    fprintf(output_file,"\t'n','c',\n\t");
	    for (i = 0; i < file_size; i++)
	    {
	        ch = fgetc(input_file);
	        fprintf(output_file, "0x%02X, ", ch);
	        o++;
              if ((o % 16) == 0)
              {
                  fprintf(output_file, "\n\t");
              }
	    }
	    file_size +=2;
    }
    else
    {
     	    fseek(tmpf, 0, SEEK_SET);
	    o=0;
	    fprintf(output_file,"\t'c','p',\n\t");
	    for (i = 0; i < file_size_lzma; i++)
	    {
	        ch = fgetc(tmpf);
	        fprintf(output_file, "0x%02X, ", ch);
	        o++;
              if ((o % 16) == 0)
              {
                  fprintf(output_file, "\n\t");
              }
	    }
	    file_size = file_size_lzma+2;	
    }
    fclose(tmpf);
    fprintf(output_file,"\n#endif");
#endif	
    if (image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP ||
        image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
        image_identifier == IMAGE_TYPE_PNG || image_identifier == IMAGE_TYPE_M3D ||
        image_identifier == IMAGE_TYPE_SWFLASH)
    {
        return file_size + 8;
    }
    else
    {
        return file_size + 6;
    }
}

/* 101205 audio resource Calvin Satrt */


/*****************************************************************************
 * FUNCTION
 *  Load_Audio_Data
 * DESCRIPTION
 *  load and write audio header and data
 * PARAMETERS
 *  offset                  [OUT]       File offset --> file offset
 *  output_file             [IN]        -> Name of the file to which buffer
 *  input_file              [IN]        ->  Name of the input audio file
 *  pImgData                [OUT]       -> buffer containing image data
 *  audio_identifier        [IN]        -> audio identifier
 *  is(?)                   [IN]        To be written
 * RETURNS
 *  S32 --> file size
 *****************************************************************************/
S32 Load_Audio_Data(U32 *offset, FILE *output_file, FILE *input_file, U8 *pImgData, U8 audio_identifier)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 file_size, i, o = 0;
    U8 ch;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    fseek(input_file, 0, SEEK_END);
    file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    if (toolFlag)
    {
        fprintf(
            dest_file,
            "%c%c%c%c%c%c%c%c",
            audio_identifier,
            file_size & 0xff,
            (file_size & 0xff00) >> 8,
            (file_size & 0xff0000) >> 16,
            (file_size & 0xff000000) >> 24,
            0,
            0,
            0);
    }
    else
    {
        fprintf(
            output_file /* output_data_file */ ,
            "\n\t0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x00, 0x00, 0x00,\n\t",
            audio_identifier,
            file_size & 0xff,
            (file_size & 0xff00) >> 8,
            (file_size & 0xff0000) >> 16,
            (file_size & 0xff000000) >> 24);
    }

    pImgData[0] = (U8) audio_identifier;
    pImgData[1] = (U8) (file_size & 0xff);
    pImgData[2] = (U8) ((file_size & 0xff00) >> 8);
    pImgData[3] = (U8) ((file_size & 0xff0000) >> 16);
    pImgData[4] = (U8) ((file_size & 0xff000000) >> 24);

    for (i = 0; i < file_size; i++)
    {
        ch = fgetc(input_file);

        if (toolFlag)
        {
            fputc(ch, dest_file);
            (*offset)++;
            (pImgData)[i + 6] = ch;
        }
        else
        {
            fprintf(output_file, "0x%02X, ", ch);
            (*offset)++;
            (pImgData)[i + 6] = ch;
            o++;
            if ((o % 16) == 0)
            {
                fprintf(output_file, "\n\t");
            }
        }
    }

    return file_size + 8;
}

/* 101205 audio resource Calvin End */


/*****************************************************************************
 * FUNCTION
 *  Write_Sequential_Image_Header
 * DESCRIPTION
 *  Construct the header for sequential images
 * PARAMETERS
 *  output_file             [IN]        -> output file handle
 *  image_identifier        [IN]        -> sequential image type
 *  n_frames                [IN]        -> the number of the frames
 *  seq_filesize            [IN]        -> pointer to a list of file sizes
 *  seq_offset              [OUT]       -> pointer to a list of offsets
 * RETURNS
 *  void
 *****************************************************************************/
void Write_Sequential_Image_Header(
        FILE *output_file,
        U8 image_identifier,
        S32 n_frames,
        S32 *seq_filesize,
        S32 *seq_offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i;
    S32 offset = 8;
    S32 real_size;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (toolFlag == 1)
    {
        fprintf(dest_file, "%c%c%c%c%c%c%c%c", image_identifier, n_frames, 0, 0, 0, 0, 0, 0);   /* the major header */
        for (i = 0; i < n_frames; i++)
        {
            int interval = sequential_interval[i];

            seq_offset[i] = offset;
            real_size = ((seq_filesize[i] + 3) >> 2) << 2;
            fprintf(dest_file, "%c%c%c%c", seq_offset[i] & 0xff, (seq_offset[i] & 0xff00) >> 8, (seq_offset[i] & 0xff0000) >> 16, (seq_offset[i] & 0xff000000) >> 24);  /* the offset for each file */
            fprintf(dest_file, "%c%c%c%c", seq_filesize[i] & 0xff, (seq_filesize[i] & 0xff00) >> 8, (seq_filesize[i] & 0xff0000) >> 16, (seq_filesize[i] & 0xff000000) >> 24);  /* the size for each file */
            fprintf(dest_file, "%c%c%c%c", interval & 0xff, (interval & 0xff00) >> 8, (interval & 0xff0000) >> 16, (interval & 0xff000000) >> 24);      /* the interval for each file */
            offset += real_size;
        }
    }
/* 100206 ENFB support Start */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    else if (enfbFlag == 1) /* write ENFB header to CustImgDataxxx.h and image header/data to ENFB image data file */
    {
        S32 data_size = 0;
        for (i = 0; i < n_frames; i++)
        {
            real_size = ((seq_filesize[i] + 3) >> 2) << 2;
            data_size += real_size;
        }
        enfb_size = 8 + 12*n_frames + data_size;
        
        /* write ENFB header to CustImgDataxxx.h */
        fprintf(
            output_file,
            "\n\t0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X\t",
            (U8) 255,//type
            (U8) ENFBAssociatedIDAdded,//associated id
            (U8) 0,//reserved
            (U8) 0,//reserved
            (U8) (enfb_offset & 0xff),
            (U8) ((enfb_offset >> 8) & 0xff),
            (U8) ((enfb_offset >> 16) & 0xff),
            (U8) ((enfb_offset >> 24) & 0xff),
            (U8) (enfb_size & 0xff),
            (U8) ((enfb_size >> 8) & 0xff),
            (U8) ((enfb_size >> 16) & 0xff),
            (U8) ((enfb_size >> 24) & 0xff));
        
        /* image header/data to ENFB image data file */
        fprintf(enfb_img_data_file, "%c%c%c%c%c%c%c%c", image_identifier, n_frames, 0, 0, 0, 0, 0, 0);   /* the major header */
        for (i = 0; i < n_frames; i++)
        {
            int interval = sequential_interval[i];

            seq_offset[i] = offset;
            real_size = ((seq_filesize[i] + 3) >> 2) << 2;
            fprintf(enfb_img_data_file, "%c%c%c%c", seq_offset[i] & 0xff, (seq_offset[i] & 0xff00) >> 8, (seq_offset[i] & 0xff0000) >> 16, (seq_offset[i] & 0xff000000) >> 24);  /* the offset for each file */
            fprintf(enfb_img_data_file, "%c%c%c%c", seq_filesize[i] & 0xff, (seq_filesize[i] & 0xff00) >> 8, (seq_filesize[i] & 0xff0000) >> 16, (seq_filesize[i] & 0xff000000) >> 24);  /* the size for each file */
            fprintf(enfb_img_data_file, "%c%c%c%c", interval & 0xff, (interval & 0xff00) >> 8, (interval & 0xff0000) >> 16, (interval & 0xff000000) >> 24);      /* the interval for each file */
            offset += real_size;
        }
        
        enfb_offset += enfb_size;
    }
#endif
/* 100206 ENFB support End */  
    else
    {
        fprintf(output_file, "\n\t0x%02X, 0x%02X, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\n\t", image_identifier, n_frames);        /* the major header */
        for (i = 0; i < n_frames; i++)
        {
            int interval = sequential_interval[i];

            seq_offset[i] = offset;
            real_size = ((seq_filesize[i] + 3) >> 2) << 2;
            fprintf(output_file, "0x%02X, 0x%02X, 0x%02X, 0x%02X, ", seq_offset[i] & 0xff, (seq_offset[i] & 0xff00) >> 8, (seq_offset[i] & 0xff0000) >> 16, (seq_offset[i] & 0xff000000) >> 24);        /* the offset for each file */
            fprintf(output_file, "0x%02X, 0x%02X, 0x%02X, 0x%02X, ", seq_filesize[i] & 0xff, (seq_filesize[i] & 0xff00) >> 8, (seq_filesize[i] & 0xff0000) >> 16, (seq_filesize[i] & 0xff000000) >> 24);        /* the size for each file */
            fprintf(output_file, "0x%02X, 0x%02X, 0x%02X, 0x%02X,\n\t", interval & 0xff, (interval & 0xff00) >> 8, (interval & 0xff0000) >> 16, (interval & 0xff000000) >> 24); /* the interval for each file */
            offset += real_size;

            g_ressize += seq_filesize[i];
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  Write_Sequential_Image_File
 * DESCRIPTION
 *  Write raw date from a frame of an image to the output file
 * PARAMETERS
 *  output_file     [IN]        -> output file handle
 *  input_file      [IN]        -> input file handle
 *  filesize        [IN]        -> the input file size
 * RETURNS
 *  void
 *****************************************************************************/
void Write_Sequential_Image_File(FILE *output_file, FILE *input_file, S32 filesize)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 i;
    U32 o = 0;
    U8 ch;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (toolFlag == 1)
    {
        for (i = 0; i < filesize; i++)
        {
            ch = fgetc(input_file);
            fprintf(dest_file, "%c", ch);
        }

        for (i = filesize; i % 4 != 0; i++)
        {
            fprintf(dest_file, "%c", 0);    /* for 4 byte alignment */
        }
    }
/* 100206 ENFB support Start */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    else if (enfbFlag == 1) /* write ENFB header to CustImgDataxxx.h and image header/data to ENFB image data file */
    {
        for (i = 0; i < filesize; i++)
        {
            ch = fgetc(input_file);
            fprintf(enfb_img_data_file, "%c", ch);
        }

        for (i = filesize; i % 4 != 0; i++)
        {
            fprintf(enfb_img_data_file, "%c", 0);    /* for 4 byte alignment */
        }
    }
#endif
/* 100206 ENFB support End */  
    else
    {
        for (i = 0; i < filesize; i++)
        {
            ch = fgetc(input_file);
            fprintf(output_file, "0x%02X, ", ch);
            o++;
            if ((o % 16) == 0)
            {
                fprintf(output_file, "\n\t");
            }
        }

        for (i = filesize; i % 4 != 0; i++)
        {
            fprintf(output_file, "0x00, "); /* for 4 byte alignment */
        }

        if ((o % 16) != 0)
        {
            fprintf(output_file, "\n\t");
        }
    }
}

/* Calvin end */
/* __CUSTPACK_MULTIBIN Calvin END */

/* 101205 audio resource Calvin Satrt */


/*****************************************************************************
 * FUNCTION
 *  Extract_Name
 * DESCRIPTION
 *  Exract image name from the supplied string
 * PARAMETERS
 *  dest            [OUT]       -> Extrected file name from string
 *  src             [IN]        -> string containing file name and path
 *  res_type        [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void Extract_Name(S8 *dest, S8 *src, U32 res_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 iLen = strlen(src);
    U16 nCount = 0;

    S8 *pSrc = src;
    char a, b;
    S8 *pCustPath;
    U32 ExtFlag;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (res_type)
    {
        case RES_IMAGE:
        case RES_MEDIA:
			#if 0
        case RES_JAVA_GAME:
			#endif
            pCustPath = CustImgPath;
            ExtFlag = ExtImageFlag;
            break;

        case RES_AUDIO:
            pCustPath = CustAdoPath;
            ExtFlag = ExtAudioFlag;
            break;

        default:
            printf("res_type in Extract_Name is invalid!!");
            pCustPath = CustImgPath;
            ExtFlag = ExtImageFlag;
            break;
    }

    iLen = strlen(pCustPath);

    for (nCount = 0; nCount < iLen; nCount++)
    {
        a = toupper(pCustPath[nCount]);
        b = toupper(src[nCount]);
        if (a != b)
        {
            break;
        }
    }
    if (iLen == nCount)
    {
        pSrc = &src[iLen];
    }

    iLen = strlen(pSrc);
    nCount = 0;

    while (iLen)
    {
        if ((pSrc[nCount] == '.') || (pSrc[nCount] == '-') || (pSrc[nCount] == ' ') || (pSrc[nCount] == '/'))
        {
            dest[nCount] = '_';
        }
        else
        {
            dest[nCount] = pSrc[nCount];
        }

        ++nCount;
        --iLen;
    }

    /* __CUSTPACK_MULTIBIN Calvin BEGIN */
    if (ExtFlag == 1)   /* 040805 CustPack: Calvin added */
    {
        dest[nCount] = '_'; /* add a '_' in the end to distinguish from the original */
        dest[nCount + 1] = '\0';
    }
    /* __CUSTPACK_MULTIBIN Calvin END */
    else
    {
        dest[nCount] = '\0';
    }
    return;
}

/* 101205 audio resource Calvin End */

/* 101205 audio resource Calvin Satrt */


/* 070306 Alpha layer Start */
/*****************************************************************************
 * FUNCTION
 *  ExtractExtension
 * DESCRIPTION
 *  Function Extracting the extension
 * PARAMETERS
 *  src             [?]         
 *  res_type        [IN]        -> resource type
 *  string(?)       [IN]        -> string containing file name
 * RETURNS
 *  U8
 *****************************************************************************/
U8 ExtractExtension(S8 *src, U32 res_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j;
    S32 type_number;
    U8 ret = 0;
    S8 *arr;
    RESOURCE_INFO *pResource_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = strlen(src) - 1;
    arr = src+i;

    for (j = 0; j < 5 ; j++)
    {
        if(*arr == '.')
        {
            arr++;
            break;
        }
        else
    {
            arr--;
        }
    }

    if(j >= 5)
    {
        return ret;
    }

    switch (res_type)
    {
        case RES_IMAGE:
        case RES_MEDIA:
            ret = IMAGE_TYPE_INVALID;
            pResource_info = image_resource_info;
            type_number = sizeof(image_resource_info) / sizeof(RESOURCE_INFO);
            break;

        case RES_AUDIO:
            ret = AUDIO_TYPE_INVALID;
            pResource_info = audio_resource_info;
            type_number = sizeof(audio_resource_info) / sizeof(RESOURCE_INFO);
            break;

        default:
            printf("res_type in Extract_Name is invalid!!");
            ret = IMAGE_TYPE_INVALID;
            pResource_info = image_resource_info;
            type_number = sizeof(image_resource_info) / sizeof(RESOURCE_INFO);
            break;
    }

    for (i = 0; i < type_number; i++)
    {
        if (!(strcasecmp(arr, pResource_info[i].filename_ext)))
        {
            ret = pResource_info[i].file_type;
        }
    }

    return ret;
}


/*****************************************************************************
 * FUNCTION
 *  ChangeExtension
 * DESCRIPTION
 *  Function Change the extension
 * PARAMETERS
 *  src             [IN]        -> string containing file name 
 *  res_type        [IN]        -> resource type
 *  ext_type        [IN]        -> extension type
 * RETURNS
 *  BOOL
 *****************************************************************************/
BOOL ChangeExtension(S8 *src, U32 res_type, S32 ext_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j;
    S32 type_number;
    U8 ret = 0;
    S8 *arr;
    RESOURCE_INFO *pResource_info;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    i = strlen(src) - 1;
    arr = src+i;

    for (j = 0; j < 5 ; j--)
    {
        if(*arr == '.')
        {
            arr++;
            break;
        }
        else
        {
            arr--;
        }
    }

    if (j >= 5)
    {
        return FALSE;
    }

    switch (res_type)
    {
        case RES_IMAGE:
        case RES_MEDIA:
            ret = IMAGE_TYPE_INVALID;
            pResource_info = image_resource_info;
            type_number = sizeof(image_resource_info) / sizeof(RESOURCE_INFO);
            break;

        case RES_AUDIO:
            ret = AUDIO_TYPE_INVALID;
            pResource_info = audio_resource_info;
            type_number = sizeof(audio_resource_info) / sizeof(RESOURCE_INFO);
            break;

        default:
            printf("res_type in Extract_Name is invalid!!");
            ret = IMAGE_TYPE_INVALID;
            pResource_info = image_resource_info;
            type_number = sizeof(image_resource_info) / sizeof(RESOURCE_INFO);
            break;
    }

    for (i = 0; i < type_number; i++)
    {
        if (pResource_info[i].file_type == ext_type)
        {
            break;
        }
    }

    if (i >= type_number)
    {
        return FALSE;
    }

    for (j = 0; j < 5; j++)
    {
        *(arr+j) = *(pResource_info[i].filename_ext+j);
    }

    return TRUE;
}
/* 070306 Alpha layer End */


/*****************************************************************************
 * FUNCTION
 *  isEmsImage
 * DESCRIPTION
 *  Decide if the iamge is used for EMS picture
 *  
 *  use EMS_IMAGE_PATH (defined in CustDataRes.h) to decide if a EMS picture
 * PARAMETERS
 *  path        [IN]        -> string containing file name and path
 * RETURNS
 *  U8 --> 1: EMS picture, 0: Not EMS picture
 *****************************************************************************/
U8 isEmsImage(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S8 ems_path[] = EMS_IMAGE_PATH;
    U8 pos = 0, ems_len = 0, i = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ems_len = strlen(ems_path);
    pos = strlen(path) - 1;
    while (path[pos] != '/')
    {
        /* __CUSTPACK_MULTIBIN Calvin BEGIN */
        if (pos == 0)   /* 040805 CustPack: Calvin added */
        {
            return 0;
        }
        /* __CUSTPACK_MULTIBIN Calvin END */
        pos--;
    }
    pos -= ems_len + 1;
    for (i = 0; i < ems_len; i++)
    {
        if (ems_path[i] != path[pos])
        {
            return 0;
        }
        else
        {
            pos++;
        }
    }

    return 1;
}


/*****************************************************************************
 * FUNCTION
 *  isMainmenuImage
 * DESCRIPTION
 *  Decide if the iamge is used for Mainmenu picture
 * PARAMETERS
 *  path        [IN]        -> string containing file name and path
 * RETURNS
 *  U8 --> 1: Matrix Mainmenu picture, 0: Not Matrix mainmenu picture
 *****************************************************************************/
MMI_BOOL isMainmenuImage(S8 *path)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if ((strstr(path, "/MATRIX/") != NULL) && (strstr(path, ".GIF") != NULL))
    {
        return MMI_TRUE;
    }
    else
    {
        return MMI_FALSE;
    }
}


/*****************************************************************************
 * FUNCTION
 *  isSubmenuImage
 * DESCRIPTION
 *  Decide if the iamge is used for Submenu picture
 *  
 *  use SUBMENU_IMAGE_PATH (defined in CustDataRes.h) to decide if a EMS picture
 * PARAMETERS
 *  path        [IN]        -> string containing file name and path
 * RETURNS
 *  U8 --> 1: Submenu picture, 0: Not Submenu picture
 *****************************************************************************/
U8 isSubmenuImage(S8 *path)
{
#ifndef __MMI_SLIM_LISTMENU_NUMBER__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return 0;
#else /* __MMI_SLIM_LISTMENU_NUMBER__ */ 
    S8 submenu_path[] = SUBMENU_IMAGE_PATH;
    U8 pos = 0, submenu_len = 0, i = 0;

    if (strstr(path, submenu_path) != NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    /*
     * submenu_len = strlen(submenu_path);
     * pos = strlen (path)-1;
     * while (path[pos]!='/') pos--;
     * pos-=submenu_len+1;
     * for ( i=0; i<submenu_len; i++){
     * if ( submenu_path[i] != path[pos] ) return 0;
     * else pos++;
     * }
     * return 1;
     */
#endif /* __MMI_SLIM_LISTMENU_NUMBER__ */ 
}

/* __CUSTPACK_MULTIBIN Calvin BEGIN */
/* 040805 CustPack: Calvin added */


/*****************************************************************************
 * FUNCTION
 *  ToUpper
 * DESCRIPTION
 *  
 * PARAMETERS
 *  ch      [IN]        
 * RETURNS
 *  
 *****************************************************************************/
S8 ToUpper(S8 ch)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return toupper(ch);
}

/* Calvin end */
/* __CUSTPACK_MULTIBIN Calvin END */


/*****************************************************************************
 * FUNCTION
 *  Iload
 * DESCRIPTION
 *  Load the file image to buffer and writes to CustImgRes.c
 * PARAMETERS
 *  string      [IN]        -> string containing file name and path
 *  pType       [OUT]       -> Image Type
 * RETURNS
 *  void* --> buffer containing image data
 *****************************************************************************/
MMI_BOOL Iload(S8 *string, PU16 pType)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 image_counter = 0;
    U32 image_offset = 0;
    U32 failure_counter = 0;
    U8 *pBuffer = NULL;
    S32 j, sequence_counter, n_frames;
    MMI_BOOL failure_flag;
    U8 image_identifier;
    S32 n_images = 0;
    S32 file_size1 = 0;
    S32 file_size = 0;
    S32 nImageSize = 0;
    U8 *pBuffer1 = NULL;
    S32 extension_type = -1;
    U8 staticFlag = 1;  /* Non animated image */
    /* 070306 Alpha layer Start */
    //#ifdef __MMI_ALPHA_LAYER_SUPPORT__
    S32 ext_type = -1;
    //#endif
    /* 070306 Alpha layer End */

    // __CUSTPACK_MULTIBIN Calvin BEGIN
    //S8         tempDestFile[] = {"../../mmi_customer/IMAGES/TEMPIMAGE"};
    S8 tempDestFile[] = { "TEMPIMAGE" };    /* 040805 CustPack: Calvin modified */
    int ret = 1;

    /* __CUSTPACK_MULTIBIN Calvin END */
    S8 tempCommand[200];
    size_t size;
    U32 o = 0;
    S16 filenameLen;
    char *curr_output_data_filename;

#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    U32 enfb_width, enfb_height;//100206 ENFB support
    S32 enfb_ret;
#endif

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ressize = 0;

    if (ExtImageFlag == EXT_IMAGE)
    {
        curr_output_data_filename = OUTPUT_DATA_FILENAME_EXT;
    }
    else if (ExtImageFlag == SYSTEM_IMAGE)
    {
        curr_output_data_filename = OUTPUT_DATA_FILENAME;
    }
    else
    {
        curr_output_data_filename = NFB_IMAGE_DATA_FILENAME;
        nfb_img_res_file = FOPEN(NFB_IMAGE_RES_FILENAME, "ab");
    }

    if (toolFlag == 1)
    {   /* 040805 CustPack: Calvin added */
        curr_output_data_filename = "temp";
    }

    strcpy(filename_buffer, curr_output_data_filename); /* 040805 CustPack: Calvin modified */
    /* __CUSTPACK_MULTIBIN Calvin END */

    output_data_file = FOPEN(filename_buffer, "ab");
    while (!output_data_file)
    {
        Sleep(50);
        output_data_file = FOPEN(filename_buffer, "ab");
    }
    //Move behind
    //fprintf(output_data_file,"const U8 ");

    output_image_names = (S8*) malloc(MAX_FILENAME_LEN);
    strcpy(filename_buffer, string);
    if (!(strcasecmp(filename_buffer, "")))
    {

        strcpy(filename_buffer, NOIMAGE_FILENAMEBUFFER);
    }

    Extract_Name(output_image_names, filename_buffer, RES_IMAGE);
#ifdef __MMI_SLIM_LISTMENU_NUMBER__
    if (isSubmenuImage(filename_buffer))
    {
        int i, len;

        len = strlen(filename_buffer);

        for (i = len; i > 0; i--)
        {
            if (filename_buffer[i] == 'U' && filename_buffer[i - 1] == 'N' && filename_buffer[i - 2] == 'E' &&
                filename_buffer[i - 3] == 'M' && filename_buffer[i - 4] == 'B' && filename_buffer[i - 5] == 'U' &&
                filename_buffer[i - 6] == 'S')
            {
                filename_buffer[i + 1] = '2';
                break;
            }
            else
            {
                filename_buffer[i + 1] = filename_buffer[i];
            }

        }
        filename_buffer[len + 1] = '\0';
    }
#endif /* __MMI_SLIM_LISTMENU_NUMBER__ */ 

    failure_flag = MMI_TRUE;
    extension_type = -1;
    sequence_counter = 0;

/* 070306 Alpha layer Start */
#ifdef __MMI_ALPHA_LAYER_SUPPORT__
    //printf("inside 1\n");
    ext_type = ExtractExtension(filename_buffer, RES_IMAGE);
    if (ext_type == IMAGE_TYPE_GIF || ext_type == IMAGE_TYPE_BMP)
    {
        //printf("inside GIF..BMP  %s\n", filename_buffer);
        ChangeExtension(filename_buffer, RES_IMAGE, IMAGE_TYPE_PNG);
        //printf("out GIF..BMP  %s\n", filename_buffer);
    }
    else if (ext_type == IMAGE_TYPE_DEVICE_BITMAP)
    {
        ChangeExtension(filename_buffer, RES_IMAGE, IMAGE_TYPE_PPN);
    }

    image_data_file = FOPEN(filename_buffer, "rb");
    if (image_data_file == NULL)
    {
        ChangeExtension(filename_buffer, RES_IMAGE, ext_type);
    }
    else
    {
        fclose(image_data_file);
        image_data_file = NULL;
    }
#endif

    ext_type = ExtractExtension(filename_buffer, RES_IMAGE);
    if (ext_type == IMAGE_TYPE_PPN)
    {
        //ChangeExtension(filename_buffer, RES_IMAGE, IMAGE_TYPE_PNG);
        strcpy((char*)tempBMPFile, (const char*)filename_buffer);
        ChangeExtension(tempBMPFile, RES_IMAGE, IMAGE_TYPE_DEVICE_BITMAP);
#ifndef __CYGWIN__
        sprintf(tempCommand,"convert png:%s bmp:%s", filename_buffer, tempBMPFile);
#else
        sprintf(tempCommand,"convert.exe png:%s bmp:%s", filename_buffer, tempBMPFile);
#endif
        //sprintf(tempCommand,"convert.exe png:%s bmp:%s", filename_buffer, "4.pbm");
        //printf("%s\n",tempCommand);
        system(tempCommand);
        strcpy((char*)filename_buffer, (const char*)tempBMPFile);
        //printf("%s\n",filename_buffer);
    }
/* 070306 Alpha layer End */

    filenameLen = strlen(filename_buffer);

    if (isEmsImage(filename_buffer))
        /* if (filename_buffer[filenameLen-3] =='P' && filename_buffer[filenameLen-2] == 'I' && filename_buffer[filenameLen-1] =='C' ) */
    {
        image_data_file = FOPEN(filename_buffer, "rb");
        if (image_data_file != NULL)
        {
            /* __CUSTPACK_MULTIBIN Calvin BEGIN */
            if (toolFlag == 0)  /* 040805 CustPack: Calvin modified */
            {
                fprintf(output_data_file, "const U8 ");
                fprintf(output_data_file, output_image_names);
#ifdef RES_SECTION_ALONG
                fprintf(output_data_file, "[]  \n#ifndef MMI_ON_WIN32\n  __attribute__((section (\".roresdata\"))) \n#endif\n =");
#else
                fprintf(output_data_file, "[]=\n");
#endif
                if(output_data_file != NULL)
                { 
                fclose(output_data_file);
                    output_data_file = NULL;
                }
            }
            //sprintf(tempCommand,"%s %s -b %s", EMSCONVERTER_FILENAME,filename_buffer,OUTPUT_DATA_FILENAME);//,output_image_names);
            //system(tempCommand);
            ret = bmp2ems(filename_buffer, curr_output_data_filename, 1, 0);    /* 040805 CustPack: Calvin modified */
            if (toolFlag)
            {
                if (ret == 0)//100206 ENFB support
                {
                    resultFailFlag = 1;
                }
            }
            /* __CUSTPACK_MULTIBIN Calvin END */
        }
        else
        {
            /* fclose(image_data_file); */
            failure_flag = MMI_FALSE;
        }
    }
    else if ((filename_buffer[filenameLen - 1] == 'P' || filename_buffer[filenameLen - 1] == 'M') &&
             (filename_buffer[filenameLen - 2] == 'R' || filename_buffer[filenameLen - 2] == 'B') &&
             filename_buffer[filenameLen - 3] == 'P')
    {
        image_data_file = FOPEN(filename_buffer, "rb");
        if (image_data_file != NULL)
        {
            if (filename_buffer[filenameLen - 2] == 'R')
            {
                //sprintf(tempCommand,"%s %s %s", GIFCONVERTER_FILENAME,filename_buffer,tempDestFile);
                //system(tempCommand);//010505 Calvin moved
                GIFLoader(filename_buffer, tempDestFile);   /* 010505 Calvin added */
            }
            else
            {
                //sprintf(tempCommand,"%s %s %s", BMPCONVERTER_FILENAME,filename_buffer,tempDestFile);
                //system(tempCommand);//010505 Calvin moved
                // __CUSTPACK_MULTIBIN Calvin BEGIN
                ret = BMPLoader(filename_buffer, tempDestFile, __MMI_DEVICE_BMP_FORMAT__); /* 010505 Calvin added */
                if (toolFlag)
                {
                    if (ret = 0)
                    {
                        resultFailFlag = 1;
                    }
                }
                /* __CUSTPACK_MULTIBIN Calvin END */
            }
            /* system(tempCommand);//010505 Calvin moved */
            if(image_data_file != NULL)
            { 
            fclose(image_data_file);
                image_data_file = NULL;
            }

            /* __CUSTPACK_MULTIBIN Calvin BEGIN */
            if (toolFlag == 0)  /* 040805 CustPack: Calvin modified */
            {
                image_data_file = FOPEN(tempDestFile, "rb");
                if (image_data_file != NULL)
                {
                    strcpy((char*)autoimagename_buffer, (const char*)output_image_names);
#ifdef RES_SECTION_ALONG
                    strcat((char*)autoimagename_buffer, "[]   \n#ifndef MMI_ON_WIN32\n  __attribute__((section (\".roresdata\"))) \n#endif\n =\n");
#else
                    strcat((char*)autoimagename_buffer, "[]=\n");
#endif
                    fprintf(output_data_file, "const U8 ");
                    fprintf(output_data_file, autoimagename_buffer);
                    if (ExtImageFlag == NFB_PROGRESS_IMAGE)
                    {
                        if (nfb_img_res_file != NULL)
                        {
                            fprintf(nfb_img_res_file, "(U8*)&%s,\n", output_image_names);
                            if(nfb_img_res_file != NULL)
                            { 
                            fclose(nfb_img_res_file);
                                nfb_img_res_file = NULL;
                            }
                        }
                        else
                        {
                            printf("nfb_img_res_file not opened\n");
                        }
                    }

                    fseek(image_data_file, 0, SEEK_END);
                    file_size = ftell(image_data_file);
                    fseek(image_data_file, 0, SEEK_SET);

                    while (!feof(image_data_file))
                    {
                        size = fread(tempCommand, sizeof(char), 200, image_data_file);
                        if (size)
                        {
                            fwrite(tempCommand, sizeof(char), size, output_data_file);
                        }
			   else 
			   	break;
                    }
                    fputs("\n", output_data_file);

                }
            }
            /* __CUSTPACK_MULTIBIN Calvin END */
        }
        else
        {
            failure_flag = MMI_FALSE;
        }
    }
    else
    {
	image_data_file = NULL;

		
        image_data_file = FOPEN(filename_buffer, "rb");
        if (image_data_file == NULL)
        {

            Sleep(50);
            if (filename_buffer[filenameLen - 1] == 'P' && filename_buffer[filenameLen - 2] == 'M' &&
                filename_buffer[filenameLen - 3] == 'B')
            {
                filename_buffer[filenameLen - 1] = 'F';
                filename_buffer[filenameLen - 2] = 'I';
                filename_buffer[filenameLen - 3] = 'G';
            }
            else if (isMainmenuImage(filename_buffer))
            {
                filename_buffer[filenameLen - 1] = 'M';
                filename_buffer[filenameLen - 2] = 'B';
                filename_buffer[filenameLen - 3] = 'P';
                image_data_file = FOPEN(filename_buffer, "rb");
                if (image_data_file != NULL)
                {
                    //sprintf(tempCommand,"%s %s %s", BMPCONVERTER_FILENAME,filename_buffer,tempDestFile);
                    //system(tempCommand);
                    // __CUSTPACK_MULTIBIN Calvin BEGIN
                    ret = BMPLoader(filename_buffer, tempDestFile, __MMI_DEVICE_BMP_FORMAT__);     /* 010505 Calvin added */
                    if (toolFlag)
                    {
                        if (ret = 0)
                        {
                            resultFailFlag = 1;
                        }
                    }
                    /* __CUSTPACK_MULTIBIN Calvin END */

                    /* __CUSTPACK_MULTIBIN Calvin BEGIN */
                    if (toolFlag == 0)  /* 040805 CustPack: Calvin modified */
                    {
                        image_data_file = FOPEN(tempDestFile, "rb");
                        if (image_data_file != NULL)
                        {
                            strcpy((char*)autoimagename_buffer, (const char*)output_image_names);
#ifdef RES_SECTION_ALONG
                    strcat((char*)autoimagename_buffer, "[]   \n#ifndef MMI_ON_WIN32\n  __attribute__((section (\".roresdata\"))) \n#endif\n =\n");
#else
                    strcat((char*)autoimagename_buffer, "[]=\n");
#endif
                            fprintf(output_data_file, "const U8 ");
                            fprintf(output_data_file, autoimagename_buffer);
                            if (ExtImageFlag == NFB_PROGRESS_IMAGE)
                            {
                                if (nfb_img_res_file != NULL)
                                {
                                    fprintf(nfb_img_res_file, "(U8*)&%s,\n", output_image_names);
                                    fclose(nfb_img_res_file);
                                    nfb_img_res_file = NULL;
                                }
                                else
                                {
                                    printf("nfb_img_res_file not opened\n");
                                }
                            }

                            fseek(image_data_file, 0, SEEK_END);
                            file_size = ftell(image_data_file);
                            fseek(image_data_file, 0, SEEK_SET);

                            while (!feof(image_data_file))
                            {
                                size = fread(tempCommand, sizeof(char), 200, image_data_file);
                                if (size)
                                {
                                    fwrite(tempCommand, sizeof(char), size, output_data_file);
                                }
                            }
                            fputs("\n", output_data_file);

                        }
                        if(image_data_file != NULL)
                        { 
                        fclose(image_data_file);
                            image_data_file = NULL;
                    }
                    }
                    if(output_data_file != NULL)
                    { 
                    fclose(output_data_file);
                        output_data_file = NULL;
                    }
                    return MMI_TRUE;
                    /* __CUSTPACK_MULTIBIN Calvin END */
                }
            }
            else if (filename_buffer[filenameLen - 1] == 'F' && filename_buffer[filenameLen - 2] == 'I' &&
                     filename_buffer[filenameLen - 3] == 'G')
            {
                filename_buffer[filenameLen - 1] = 'P';
                filename_buffer[filenameLen - 2] = 'M';
                filename_buffer[filenameLen - 3] = 'B';
            }
            image_data_file = FOPEN(filename_buffer, "rb");
        }
		
//     dwAttr = GetFileAttributes(filename_buffer);  
//	if( (dwAttr != 0xFFFFFFFF) &&  (dwAttr & FILE_ATTRIBUTE_DIRECTORY) )  
	if( ExtractExtension(filename_buffer, RES_IMAGE) == IMAGE_TYPE_INVALID)  

  {
		   FILE *fail = NULL;
	        if (image_data_file != NULL) 
            { 
                fclose(image_data_file);
                image_data_file = NULL;
            }
//        if (image_data_file == NULL)
//        {
                failure_flag = MMI_FALSE;   /* 021705 Calvin moved from down */


        /* 070306 Alpha layer Start */
        /* Find the right extension file name */
        #ifdef __MMI_ALPHA_LAYER_SUPPORT__
            for (j = SUPPORT_N_SEQUENTIAL_IMAGE_TYPE-1; j >= 0; j--)//search from PNG       
        #else
            for (j = 0; j < SUPPORT_N_SEQUENTIAL_IMAGE_TYPE; j++)//search from BMP       
        #endif
        /* 070306 Alpha layer End */
            {
                //itoa(0, number_string, 10);
		  sprintf(number_string, "%d", 0);		
                extension_type = -1;
                strcpy(autofilename_buffer, filename_buffer);
                strcat(autofilename_buffer, "/");
                /* strcat(autofilename_buffer,output_image_names); */
                strcat(autofilename_buffer, number_string);
                strcat(autofilename_buffer, ".");
                strcat(autofilename_buffer, sequential_image[j].filename_ext);
                image_data_file = FOPEN(autofilename_buffer, "rb");
                if (image_data_file != NULL)
                {
                    failure_flag = MMI_TRUE;
                    extension_type = sequential_image[j].file_type;
                    staticFlag = 0;
                    *pType = (U16) extension_type;
                    sequential_extension_type = j;  /* 031705 Calvin modified */
                    fclose(image_data_file);
                    image_data_file = NULL;
                    break;
                }
            }

		if(!failure_flag)
		{

			fail = FOPEN("./res_temp/fail.txt", "a+");

       	     if (fail != NULL)
	            {
	                /* __CUSTPACK_MULTIBIN Calvin BEGIN */
	                if (toolFlag == 0)  /* 040805 CustPack: Calvin modified */
	                {
	                    fprintf(fail, "%s\n", filename_buffer);
                        if(fail != NULL)
                        { 
	                    fclose(fail);
                            fail = NULL;
                        }
	                }
	                /* __CUSTPACK_MULTIBIN Calvin END */
	            }
		}

            // __CUSTPACK_MULTIBIN Calvin BEGIN    
            //failure_flag = MMI_FALSE;//021705 Calvin moved to up
            // __CUSTPACK_MULTIBIN Calvin END
        }
       else if (image_data_file == NULL)
       {
           FILE *fail = FOPEN("./res_temp/fail.txt", "a+");

            if (fail != NULL)
            {
                /* __CUSTPACK_MULTIBIN Calvin BEGIN */
                if (toolFlag == 0)  /* 040805 CustPack: Calvin modified */
                {
                    fprintf(fail, "%s\n", filename_buffer);
                    fclose(fail);
                    fail = NULL;
                }
                /* __CUSTPACK_MULTIBIN Calvin END */
            }
                failure_flag = MMI_FALSE;   /* 021705 Calvin moved from down */

	 }
        else
        {
            /* patch added by vikram for getting image file size */
            fseek(image_data_file, 0, SEEK_END);
            nImageSize = ftell(image_data_file);
            fseek(image_data_file, 0, SEEK_SET);

        }

        if ((failure_flag == MMI_TRUE) && !(staticFlag))
        {
            /* Sequence found */
            /* Find how many files are in sequence */

            j = 0;
            sequence_counter = 0;
            n_frames = 0;

		
            while (1)
            {
                //itoa(j, number_string, 10);
		  sprintf(number_string, "%d", j);				
                strcpy(autofilename_buffer, "");
                strcpy(autofilename_buffer, filename_buffer);
                strcat(autofilename_buffer, "/");
                /* strcat(autofilename_buffer,output_image_names); */
                strcat(autofilename_buffer, number_string);
                strcat(autofilename_buffer, ".");
                strcat(autofilename_buffer, sequential_image[sequential_extension_type].filename_ext);  /* 031705 Calvin modified */
                image_data_file = FOPEN(autofilename_buffer, "rb");
                if (image_data_file != NULL)
                {
                    sequence_counter++;
                    fseek(image_data_file, 0, SEEK_END);
                    sequential_filesize[j] = ftell(image_data_file);
                    fseek(image_data_file, 0, SEEK_SET);
                    fclose(image_data_file);
                    image_data_file = NULL;
                }
                else
                {   /*   Sequence complete */
                    if (j > 0)
                    {
                        break;
                    }
                }
                j++;
            }
            /* read during */
            {
                int i;

                strcpy(autofilename_buffer, filename_buffer);
                strcat(autofilename_buffer, "/interval.txt");

                /* 070306 interval missing Start */
                image_data_file = NULL;
                image_data_file = FOPEN(autofilename_buffer, "r");
                if (image_data_file == NULL)
                {
                    printf("ERROR: Can't find %s\n", autofilename_buffer);
                    staticFlag = 1;
                    failure_flag = MMI_FALSE;
                }
                else
                {
                    for (i = 0; i < sequence_counter; i++)
                    {
                        if (fscanf(image_data_file, "%d", &sequential_interval[i]) != 1)
                        {
                            printf("ERROR: %s, interval [%d] value missing!!\n", autofilename_buffer, i);
                            staticFlag = 1;
                            failure_flag = MMI_FALSE;
                            break;
                        }
                    }
                }
                if (image_data_file != NULL)
                {
                    fclose(image_data_file);
                    image_data_file = NULL;
                }
                /* 070306 interval missing End */
            }

            /* 070306 interval missing Start */
            if(!staticFlag)
            {               
            /* 100206 ENFB support Start */
            #ifdef __MMI_RESOURCE_ENFB_SUPPORT__                
                //itoa(0, number_string, 10);
		  sprintf(number_string, "%d", 0);			
                strcpy(autofilename_buffer, "");
                strcpy(autofilename_buffer, filename_buffer);
                strcat(autofilename_buffer, "/");
                strcat(autofilename_buffer, number_string);
                strcat(autofilename_buffer, ".");
                strcat(autofilename_buffer, sequential_image[sequential_extension_type].filename_ext);  /* 031705 Calvin modified */
                enfb_ret = Image_Test(autofilename_buffer, &enfb_width, &enfb_height);
                if (enfb_ret!= ENFB_IMAGE_NONE)
                {
                    enfbFlag = 1;
                    if (enfb_ret == ENFB_IMAGE_ASSOCIATE)
                    {
                        ENFBAssociatedIDAdded = MMI_TRUE;
                    }
                }
                //printf("sequential %s --> %d x %d\n", autofilename_buffer, enfb_width, enfb_height);
            #endif
            /* 100206 ENFB support Start */
            
                /* n_frames=sequence_counter; */
                sequential_frame = sequence_counter;

                /* Construct the data structure name  */
                strcpy(autoimagename_buffer, output_image_names);
#ifdef RES_SECTION_ALONG
				if( (strstr(autoimagename_buffer, "_MAINMENU_")))
					strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END2);
				else if ( (strstr(autoimagename_buffer, "_ACTIVE_POWERONOFF_")) 
                #if defined(__MMI_RESOURCE_SLIM_ON_128X160__) && defined(__MMI_GAME__)
                    ||(strstr(autoimagename_buffer, "_IMAGES_GAMEIMAGES_")) 
                #endif
                )
					strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END3);
				else
#endif					
                strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END);
                fprintf(output_data_file, "__align(4) const U8 ");
                fprintf(output_data_file, autoimagename_buffer);

                Write_Sequential_Image_Header(output_data_file, sequential_image[sequential_extension_type].file_type, sequential_frame, sequential_filesize, sequential_offset);   /* 031705 Calvin modified */

                /* Load all files in the sequence   */
                j = 0;
                sequence_counter = 0;
                while (1)
                {
                    memset(autofilename_buffer,0,BUFFER_SIZE);
                    //itoa(j, number_string, 10);
		      sprintf(number_string, "%d", j);					
                    strcpy(autofilename_buffer, "");
                    strcpy(autofilename_buffer, filename_buffer);
                    strcat(autofilename_buffer, "/");
                    /* strcat(autofilename_buffer,output_image_names); */
                    strcat(autofilename_buffer, number_string);
                    strcat(autofilename_buffer, ".");
                    strcat(autofilename_buffer, sequential_image[sequential_extension_type].filename_ext);  /* 031705 Calvin modified */
                    image_data_file = FOPEN(autofilename_buffer, "rb");
					
                    if (image_data_file != NULL)
                    {
                        Write_Sequential_Image_File(output_data_file, image_data_file, sequential_filesize[j]);
                        sequence_counter++;
                        fclose(image_data_file);
                        image_data_file = NULL;
                    }
                    else
                    {   /*   Sequence complete */
                        if (j > 0)
                        {
                            break;
                        }
                    }
                    j++;
                }
                fprintf(output_data_file, "\n};\n");
            /* 100206 ENFB support Start */
            #ifdef __MMI_RESOURCE_ENFB_SUPPORT__
                enfbFlag = 0;
            #endif
            /* 100206 ENFB support Start */
            }
            /* 070306 interval missing End */
        }   /* End of Animated image processor block */

        if ((failure_flag == MMI_TRUE) && (staticFlag))
        {
            /* Currently assuming that all single files are BMP   */
            if (nImageSize == 0)
            {
                image_identifier = IMAGE_TYPE_INVALID;  /* 031705 Calvin modified */
            }
            else
            {
                image_identifier = ExtractExtension(filename_buffer, RES_IMAGE);        /* 101205 audio resource Calvin */
            }
            /* To be changed later for multiple file types */
            *pType = ExtractExtension(filename_buffer, RES_IMAGE);      /* 101205 audio resource Calvin */
            /*   Load the data  */

            /* Construct the data structure name */
            strcpy(autoimagename_buffer, output_image_names);
#ifdef RES_SECTION_ALONG
				if((strstr(autoimagename_buffer, "_MAINMENU_")))
					strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END2);
				else if ( (strstr(autoimagename_buffer, "_ACTIVE_POWERONOFF_")) 
                #if defined(__MMI_RESOURCE_SLIM_ON_128X160__) && defined(__MMI_GAME__)
                    ||(strstr(autoimagename_buffer, "_IMAGES_GAMEIMAGES_")) 
                #endif
                )
					strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END3);
				else
#endif				
            strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END);
            if (image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP ||
                image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
                image_identifier == IMAGE_TYPE_PNG || image_identifier == IMAGE_TYPE_M3D ||
                image_identifier == IMAGE_TYPE_SWFLASH)
            {
                fprintf(output_data_file, "__align(4) const U8 ");
            }
            else
            {
                fprintf(output_data_file, "const U8 ");
            }
            fprintf(output_data_file, autoimagename_buffer);

            fseek(image_data_file, 0, SEEK_END);

            if (image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP ||
                image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
                image_identifier == IMAGE_TYPE_PNG || image_identifier == IMAGE_TYPE_M3D ||
                image_identifier == IMAGE_TYPE_SWFLASH)
            {
                file_size1 = file_size1 + ftell(image_data_file) + 8;
            }
            else
            {
                file_size1 = file_size1 + ftell(image_data_file) + 6;
            }
            fseek(image_data_file, 0, SEEK_SET);

            pBuffer = (U8*) malloc(file_size1);
            if (pBuffer == NULL)
            {
                /* Add Debug Message */
            }
            pBuffer1 = pBuffer;

        /* 100206 ENFB support Start */
        #ifdef __MMI_RESOURCE_ENFB_SUPPORT__
            enfb_ret = Image_Test(filename_buffer, &enfb_width, &enfb_height);
            if (enfb_ret != ENFB_IMAGE_NONE)
            {
                enfbFlag = 1;
                if (enfb_ret == ENFB_IMAGE_ASSOCIATE)
                {
                    ENFBAssociatedIDAdded = MMI_TRUE;
                }
            }
        #endif
        /* 100206 ENFB support End */

            g_ressize = Load_Image_Data(&image_offset, output_data_file, image_data_file, pBuffer1, image_identifier, 0);

        /* 100206 ENFB support Start */
        #ifdef __MMI_RESOURCE_ENFB_SUPPORT__
            enfbFlag = 0;
        #endif
        /* 100206 ENFB support Start */

            //printf("load_image_data\n");//070306 Alpha layer
            if (ExtImageFlag == NFB_PROGRESS_IMAGE)
            {
                printf("NFB_PROGRESS_IMAGE\n");
                if (nfb_img_res_file != NULL)
                {
                    printf("nfb_img_res_file\n");
                    fprintf(nfb_img_res_file, "(U8*)&%s,\n", output_image_names);
                    fclose(nfb_img_res_file);
                    nfb_img_res_file = NULL;
                }
                else
                {
                    printf("nfb_img_res_file not opened\n");
                }
            }

            free(pBuffer);
            fprintf(output_data_file, "\n };\n");
            if(image_data_file != NULL)
            { 
            fclose(image_data_file);
                image_data_file = NULL;
            }
        }
    }

    /* 120704 Calvin added for populating with .bmp or .gif but actually extsion name is not */
    if (failure_flag == MMI_FALSE)
    {
        /* printf("*************failed !!!********************\n"); */
        for (j = 0; j < SUPPORT_N_MEDIA_TYPE; j++)
        {
            S32 l;

            for (l = 0; l < 3; l++)
            {
                filename_buffer[filenameLen - 1 - l] = support_media_ext[j][2 - l];
            }
            /* printf("%s__%s\n", support_media_ext[j], filename_buffer); */

            image_data_file = FOPEN(filename_buffer, "rb");
            if (image_data_file != NULL)
            {
                break;
            }
        }

        if (image_data_file != NULL)
        {
            failure_flag = MMI_TRUE;
            /* printf("got %s \n", filename_buffer); */

            fseek(image_data_file, 0, SEEK_END);
            nImageSize = ftell(image_data_file);
            fseek(image_data_file, 0, SEEK_SET);

            /* Currently assuming that all single files are BMP   */
            if (nImageSize == 0)
            {
                image_identifier = IMAGE_TYPE_INVALID;  /* 031705 Calvin modified */
            }
            else
            {
                image_identifier = ExtractExtension(filename_buffer, RES_IMAGE);        /* 101205 audio resource Calvin */
            }
            /* To be changed later for multiple file types */
            *pType = ExtractExtension(filename_buffer, RES_IMAGE);      /* 101205 audio resource Calvin */
            /*   Load the data  */

            /* Construct the data structure name */
            strcpy(autoimagename_buffer, output_image_names);
#ifdef RES_SECTION_ALONG
				if( (strstr(autoimagename_buffer, "_MAINMENU_")))
					strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END2);
				else if ( (strstr(autoimagename_buffer, "_ACTIVE_POWERONOFF_")) 
                #if defined(__MMI_RESOURCE_SLIM_ON_128X160__) && defined(__MMI_GAME__)
                    ||(strstr(autoimagename_buffer, "_IMAGES_GAMEIMAGES_")) 
                #endif
                )
					strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END3);
				else
#endif				
            strcat(autoimagename_buffer, OUTPUT_IMAGE_NAMES_END);

            if (image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP ||
                image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
                image_identifier == IMAGE_TYPE_PNG || image_identifier == IMAGE_TYPE_M3D ||
                image_identifier == IMAGE_TYPE_SWFLASH)
            {
                fprintf(output_data_file, "__align(4) const U8 ");
            }
            else
            {
                fprintf(output_data_file, "const U8 ");
            }
            fprintf(output_data_file, autoimagename_buffer);

            fseek(image_data_file, 0, SEEK_END);

            if (image_identifier == IMAGE_TYPE_GIF || image_identifier == IMAGE_TYPE_3GP ||
                image_identifier == IMAGE_TYPE_MP4 || image_identifier == IMAGE_TYPE_AVI ||
                image_identifier == IMAGE_TYPE_PNG || image_identifier == IMAGE_TYPE_M3D ||
                image_identifier == IMAGE_TYPE_SWFLASH)
            {
                file_size1 = file_size1 + ftell(image_data_file) + 8;
            }
            else
            {
                file_size1 = file_size1 + ftell(image_data_file) + 6;
            }
            fseek(image_data_file, 0, SEEK_SET);

            pBuffer = (U8*) malloc(file_size1);
            if (pBuffer == NULL)
            {
                /* Add Debug Message */
            }
            pBuffer1 = pBuffer;

        /* 100206 ENFB support Start */
        #ifdef __MMI_RESOURCE_ENFB_SUPPORT__
            enfb_ret = Image_Test(filename_buffer, &enfb_width, &enfb_height);
            if (enfb_ret != ENFB_IMAGE_NONE)
            {
                enfbFlag = 1;
                if (enfb_ret == ENFB_IMAGE_ASSOCIATE)
                {
                    ENFBAssociatedIDAdded = MMI_TRUE;
                }
            }
        #endif
        /* 100206 ENFB support End */
        
            g_ressize = Load_Image_Data(&image_offset, output_data_file, image_data_file, pBuffer1, image_identifier, 0);
   
        /* 100206 ENFB support Start */
        #ifdef __MMI_RESOURCE_ENFB_SUPPORT__
            enfbFlag = 0;
        #endif
        /* 100206 ENFB support End */
        
            //printf("load_image_data\n");//070306 Alpha layer
            if (ExtImageFlag == NFB_PROGRESS_IMAGE)
            {
                printf("NFB_PROGRESS_IMAGE\n");
                if (nfb_img_res_file != NULL)
                {
                    printf("nfb_img_res_file\n");
                    fprintf(nfb_img_res_file, "(U8*)&%s,\n", output_image_names);
                    fclose(nfb_img_res_file);
                    nfb_img_res_file = NULL;
                }
                else
                {
                    printf("nfb_img_res_file not opened\n");
                }
            }

            free(pBuffer);
            fprintf(output_data_file, "\n };\n");
            fclose(image_data_file);
            image_data_file = NULL;
        }
    }
    /* Calvin end */

    if (image_data_file != NULL)
    {
        fclose(image_data_file);
        image_data_file = NULL;
    }
    if (output_data_file != NULL)
    {
        fclose(output_data_file);
        output_data_file = NULL;
    }
    return failure_flag;
}

/* 101205 audio resource Calvin Satrt */


/*****************************************************************************
 * FUNCTION
 *  Aload
 * DESCRIPTION
 *  Load the file audio to buffer and writes to CustAdoRes.c
 * PARAMETERS
 *  string      [IN]        -> string containing file name and path
 *  pType       [OUT]       -> Audio Type
 * RETURNS
 *  void* --> buffer containing Audio data
 *****************************************************************************/
MMI_BOOL Aload(S8 *string, PU16 pType)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U32 audio_offset = 0;
    U8 *pBuffer = NULL;
    S32 j, n_frames;
    MMI_BOOL failure_flag;
    U8 audio_identifier;
    S32 file_size1 = 0;
    S32 file_size = 0;
    S32 nAudioSize = 0;
    U8 *pBuffer1 = NULL;
    U8 staticFlag = 1;  /* Non animated audio */

    /* S8           tempDestFile[] = {"TEMPAUDIO"}; */
    int ret = 1;

    /* S8           tempCommand[200]; */
    size_t size;
    U32 o = 0;
    S16 filenameLen;
    char *curr_output_data_filename;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_ressize = 0;

    if (ExtAudioFlag == TRUE)   /* 040805 CustPack: Calvin added */
    {
        curr_output_data_filename = AUDIO_OUTPUT_DATA_FILENAME_EXT;
    }
    else
    {
        curr_output_data_filename = AUDIO_OUTPUT_DATA_FILENAME;
    }

    if (toolFlag == 1)
    {   /* 040805 CustPack: Calvin added */
        curr_output_data_filename = "temp";
    }

    strcpy(filename_buffer, curr_output_data_filename); /* 040805 CustPack: Calvin modified */

    output_data_file = FOPEN(filename_buffer, "ab");
    while (!output_data_file)
    {
        Sleep(50);
        output_data_file = FOPEN(filename_buffer, "ab");
    }

    output_audio_names = (S8*) malloc(100);
    strcpy(filename_buffer, string);
    if (!(strcasecmp(filename_buffer, "")))
    {

        strcpy(filename_buffer, NOAUDIO_FILENAMEBUFFER);
    }

    Extract_Name(output_audio_names, filename_buffer, RES_AUDIO);

    failure_flag = MMI_TRUE;
    filenameLen = strlen(filename_buffer);

#if 0
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
#endif /* 0 */ 
    {
        audio_data_file = FOPEN(filename_buffer, "rb");
        if (audio_data_file == NULL)
        {
            /* Add extension add search */
            FILE *fail = FOPEN("./Res_temp/fail.txt", "a+");

            if (fail != NULL)
            {
                if (toolFlag == 0)  /* 040805 CustPack: Calvin modified */
                {
                    fprintf(fail, "%s\n", filename_buffer);
                    fclose(fail);
                    fail = NULL;
                }
                failure_flag = MMI_FALSE;   /* 021705 Calvin moved from down */
            }
        }
        else
        {
            /* patch added by vikram for getting audio file size */
            fseek(audio_data_file, 0, SEEK_END);
            nAudioSize = ftell(audio_data_file);
            fseek(audio_data_file, 0, SEEK_SET);
        }

        if ((failure_flag == MMI_TRUE) && (staticFlag))
        {
            /* Currently assuming that all single files are BMP   */
            if (nAudioSize == 0)
            {
                audio_identifier = AUDIO_TYPE_INVALID;  /* 031705 Calvin modified */
            }
            else
            {
                audio_identifier = ExtractExtension(filename_buffer, RES_AUDIO);        /* 101205 audio resource Calvin */
            }
            /* To be changed later for multiple file types */
            *pType = ExtractExtension(filename_buffer, RES_AUDIO);      /* 101205 audio resource Calvin */

            /*   Load the data  */

            /* Construct the data structure name */
            strcpy(autoimagename_buffer, output_audio_names);
            strcat(autoimagename_buffer, OUTPUT_AUDIO_NAMES_END);
            fprintf(output_data_file, "__align(4) const U8 ");
            fprintf(output_data_file, autoimagename_buffer);

            fseek(audio_data_file, 0, SEEK_END);
            file_size1 = file_size1 + ftell(audio_data_file) + 8;
            fseek(audio_data_file, 0, SEEK_SET);

            pBuffer = (U8*) malloc(file_size1);
            if (pBuffer == NULL)
            {
                /* Add Debug Message */
            }
            pBuffer1 = pBuffer;

            g_ressize = Load_Audio_Data(&audio_offset, output_data_file, audio_data_file, pBuffer1, audio_identifier);

            free(pBuffer);
            fprintf(output_data_file, "\n };\n");
            if(audio_data_file != NULL)
            { 
            fclose(audio_data_file);
                audio_data_file = NULL;
            }
        }
    }

#if 0
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
#endif /* 0 */ 

    if (audio_data_file != NULL)
    {
        fclose(audio_data_file);
        audio_data_file = NULL;
    }

    if (output_data_file != NULL)
    {
        fclose(output_data_file);
        output_data_file = NULL;
    }

    return failure_flag;
}

/* 101205 audio resource Calvin End */


/*****************************************************************************
 * FUNCTION
 *  ClearHWIMageFile
 * DESCRIPTION
 *  Clear image file
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ClearHWIMageFile()
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FILE *fp = NULL;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    fp = FOPEN(OUTPUT_DATA_FILENAME, "w+");
    if (fp)
    {
        fprintf(fp, "#if ( defined (MMI_ON_WIN32) || !defined (__COOLSAND_TARGET__) )\n\t#define __align(x)\n#endif\n");
        fclose(fp);
        fp = NULL;
    }

    /* __CUSTPACK_MULTIBIN Calvin BEGIN */
    fp = FOPEN(OUTPUT_DATA_FILENAME_EXT, "w+"); /* 040805 CustPack: Calvin added */
    /* __CUSTPACK_MULTIBIN Calvin END */
    if (fp)
    {
        fprintf(fp, "#if ( defined (MMI_ON_WIN32) || !defined (__COOLSAND_TARGET__) )\n\t#define __align(x)\n#endif\n");
        fclose(fp);
        fp = NULL;
    }

    /* 101205 audio resource Calvin Satrt */
    fp = FOPEN(AUDIO_OUTPUT_DATA_FILENAME, "w+");
    if (fp)
    {
        fprintf(fp, "#if ( defined (MMI_ON_WIN32) || !defined (__COOLSAND_TARGET__) )\n\t#define __align(x)\n#endif\n");
        fclose(fp);
        fp = NULL;
    }

    fp = FOPEN(AUDIO_OUTPUT_DATA_FILENAME_EXT, "w+");
    if (fp)
    {
        fprintf(fp, "#if ( defined (MMI_ON_WIN32) || !defined (__COOLSAND_TARGET__) )\n\t#define __align(x)\n#endif\n");
        fclose(fp);
        fp = NULL;
    }
    /* 101205 audio resource Calvin End */
    
    /* 100206 ENFB support Satrt */
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    enfb_img_data_file = FOPEN(ENFB_IMAGE_DATA_FILENAME, "w+");
    if (enfb_img_data_file)
    {
        fclose(enfb_img_data_file);
        enfb_img_data_file = NULL;
    }
    enfb_img_data_file = FOPEN(ENFB_IMAGE_DATA_FILENAME, "ab");
#endif
    /* 100206 ENFB support End */
}
#endif /* DEVELOPER_BUILD_FIRST_PASS */ 

#if defined(CUSTOMIZATION_BUILD_FIRST_PASS) || defined(CUSTOMIZATION_BUILD_SECOND_PASS)
CURR_SCREEN_RESOURCE gsCurrScreenInfo;
extern CUSTOM_MENU nCustMenus[MAX_MENU_ITEMS];
extern U16 CurrMaxMenuItemIndex;


/*****************************************************************************
 * FUNCTION
 *  SetCurrentScreenStrings
 * DESCRIPTION
 *  SEt the current screen strings
 * PARAMETERS
 *  nNum        [IN]        ..
 * RETURNS
 *  void
 *****************************************************************************/
void SetCurrentScreenStrings(U16 nNum, ...)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 nId;
    U16 nCount = 0;
    va_list vlist;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    va_start(vlist, nNum);

    gsCurrScreenInfo.nNoOfStrings = nNum;

    while (nNum > nCount)
    {
        nId = va_arg(vlist, U16);
        gsCurrScreenInfo.nStringIdList[nCount] = nId;
        ++nCount;
    }
    va_end(vlist);
    return;
}


/*****************************************************************************
 * FUNCTION
 *  SetCurrentScreenImages
 * DESCRIPTION
 *  SEt the current screen images
 * PARAMETERS
 *  nNum        [IN]        ..
 * RETURNS
 *  void
 *****************************************************************************/
void SetCurrentScreenImages(U16 nNum, ...)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 nId;
    U16 nCount = 0;
    va_list vlist;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    va_start(vlist, nNum);

    gsCurrScreenInfo.nNoOfImages = nNum;

    while (nNum > nCount)
    {
        nId = va_arg(vlist, U16);
        gsCurrScreenInfo.nImageIdList[nCount] = nId;
        ++nCount;
    }
    va_end(vlist);
    return;
}


/*****************************************************************************
 * FUNCTION
 *  AppendCurrentScreenImages
 * DESCRIPTION
 *  SEt the current screen images
 * PARAMETERS
 *  nNum        [IN]        
 *  pList       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void AppendCurrentScreenImages(int nNum, U16 *pList)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 nCount = 0;
    U16 i = 0;
    U8 flag = 0;
    U16 nOrgCount = gsCurrScreenInfo.nNoOfImages;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gsCurrScreenInfo.nNoOfImages = nNum + gsCurrScreenInfo.nNoOfImages;

    while (nNum > nCount)
    {
        flag = 0;
        //for(i = nOrgCount+1;i<(nOrgCount+nCount ) ;i++)
        //for(i = (nOrgCount+nCount);i>0 ;i--)
        for (i = 0; i < nCount; i++)
        {
            if (gsCurrScreenInfo.nImageIdList[i + nOrgCount] == pList[nCount])
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
        {
            gsCurrScreenInfo.nImageIdList[nOrgCount + nCount] = pList[nCount];
        }

        ++nCount;
    }
    return;
}


/*****************************************************************************
 * FUNCTION
 *  AppendCurrentScreenStrings
 * DESCRIPTION
 *  SEt the current screen images
 * PARAMETERS
 *  nNum        [IN]        
 *  pList       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void AppendCurrentScreenStrings(int nNum, U16 *pList)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 nCount = 0;
    U16 nOrgCount = gsCurrScreenInfo.nNoOfStrings;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gsCurrScreenInfo.nNoOfStrings = nNum + gsCurrScreenInfo.nNoOfStrings;

    while (nNum > nCount)
    {
        gsCurrScreenInfo.nStringIdList[nOrgCount + nCount] = pList[nCount];
        ++nCount;
    }
    return;
}


/*****************************************************************************
 * FUNCTION
 *  SetCurrentScreenMenus
 * DESCRIPTION
 *  SEt the current screen menus
 * PARAMETERS
 *  nNum        [IN]        ..
 * RETURNS
 *  void
 *****************************************************************************/
void SetCurrentScreenMenus(U16 nNum, ...)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 nId;
    U16 nCount = 0;
    U16 nParentIdCount = 0;
    va_list vlist;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    va_start(vlist, nNum);

    gsCurrScreenInfo.nNoOfMenus = nNum;

    while (nNum > nCount)
    {
        nId = va_arg(vlist, U16);
        gsCurrScreenInfo.sMenuInfo[nCount].nId = nId;
        nParentIdCount = 0;
        while (nParentIdCount > CurrMaxMenuItemIndex)
        {
            if (nCustMenus[nParentIdCount].nMenuItemId == nId)
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

#endif /* defined(CUSTOMIZATION_BUILD_FIRST_PASS) || defined(CUSTOMIZATION_BUILD_SECOND_PASS) */ 

#ifndef __CYGWIN__
void GenOrgPath(const char* in, char* out)
{
    BOOL            findFlag;
    HANDLE          hList;
    TCHAR           szDir[MAX_PATH+1];
    TCHAR           szSubDir[MAX_PATH+1];
    WIN32_FIND_DATA FileData;


    char* tok;
    char* tempStr = malloc(strlen(in)+1);
    memcpy(tempStr, in, strlen(in)+1);
    if(in[0] == '/') 
    { 
        out[0] = '/';
        out[1] = 0;
    }
    else
    { 
        out[0] = '.';
        out[1] = '/';
        out[2] = 0;
    }
    // Get the proper directory path
    sprintf(szDir, "%s/*", out);

    tok = strtok(tempStr, "/");
    while(tok != NULL)
    { 
        findFlag = FALSE;
        // Get the first file
        hList = FindFirstFile(szDir, &FileData);
        if (hList == INVALID_HANDLE_VALUE)
        { 
            strcat(out, tok);
            free(tempStr);
            FindClose(hList);
            return;
        }
        do
        { 
            if(strcasecmp(FileData.cFileName, tok) == 0)
            { 
                findFlag = TRUE;
                strcat(out, FileData.cFileName);
                strcat(out, "/");
            }
        }
        while(!findFlag && FindNextFile(hList, &FileData));
        FindClose(hList);
        if(!findFlag)
        { 
            strcat(out, tok);
            strcat(out, "/");
        }
        sprintf(szDir, "%s/*", out);
        tok = strtok(NULL, "/");
    }
    free(tempStr);
    out[strlen(out)-1] = 0;
}
#endif //__WINE__

FILE* FOPEN(const char* filename, const char* mode)
{ 
#ifndef __CYGWIN__
    FILE* f = NULL;
    char* tmpFileName = malloc(strlen(filename) + 10);
    GenOrgPath(filename, tmpFileName);
    f = fopen(tmpFileName, mode);
    free(tmpFileName);
    return f;
#else
    return fopen(filename, mode);
#endif
}

