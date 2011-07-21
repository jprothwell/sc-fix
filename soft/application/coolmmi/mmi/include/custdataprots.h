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
 *  CustDataProts.h
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
	FILENAME	: CustDataProts.h

   	PURPOSE		: To define Common Structures used by the UI Elements and Cutomization Tool 
				  for Customizing Strings and Images.

 

 

	DATE		: 2nd Oct 2003.
*************************************************************/

#ifndef _PIXCOM_CUSTDATAPROTS_H
#define _PIXCOM_CUSTDATAPROTS_H

#include "mmi_data_types.h"
#include "buildcfg.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS

extern void SetStringWithDisplay(U16 nStrId, S8 *pString, S8 *pDisplay, S8 *enum_value, S8 *resfile);
extern void SetString(U16 StrId, S8 *String, S8 *enum_value, S8 *resfile);
extern void SetImage(U16 ImageId, S8 *ImgFileName, S8 *enum_value, U32 res_type, S8 *resfile);
extern void SetImageWithBuffer(U16 nImgId, S8 *pBuffName, S8 *pDisplayName, S8 *enum_value, U32 res_type, BOOL imgFlag, S8 *resfile);
extern void SetAudioWithBuffer(U16 nAdoId, S8 *filename, S8 *pDisplayName, S8 *enum_value, U32 res_type, BOOL adoFlag, S8 *resfile);
extern void GetandWriteAudioResource(const char* sAudioSourceName,const char * sAudioDataName);
extern void SetMenuItem(U16 arg1, ...);
extern S8 g_resmenu_filename[];
extern char currentResFile[];
extern void AddApp(U32 nId, S8 *pName, U16 nMinId, U16 nMaxId);
extern void SetNFBImage(S8 *filename);
#endif /* DEVELOPER_BUILD_FIRST_PASS */ 

#ifdef DEVELOPER_BUILD_FIRST_PASS
#define ADD_APPLICATION_STRING(stringId, string)    \
    SetString(stringId, string, #stringId, __FILE__);
#define ADD_APPLICATION_STRING2(stringId, string, display)  \
    SetStringWithDisplay(stringId, string, display, #stringId, __FILE__);
#define ADD_APPLICATION_STRING3(stringId, tmp, string, display) \
    SetStringWithDisplay(stringId, string, display, tmp, __FILE__);

#define ADD_APPLICATION_IMAGE(ImageId, ImgFileName) \
    sprintf(currentResFile,"%s",__FILE__);\
    SetImage(ImageId, ImgFileName, #ImageId, RES_IMAGE, __FILE__);
#define ADD_APPLICATION_IMAGE2(ImageId, filename, display)  \
    sprintf(currentResFile,"%s",__FILE__);\
    SetImageWithBuffer(ImageId, filename, display, #ImageId, RES_IMAGE, FALSE, __FILE__);
#define ADD_APPLICATION_IMAGE3(ImageId, filename, display)  \
    sprintf(currentResFile,"%s",__FILE__);\
    SetImageWithBuffer(ImageId, filename, display, #ImageId, RES_IMAGE, TRUE, __FILE__);
#define ADD_APPLICATION_IMAGE4(ImageId, tmp, path, filename, display, isMB) \
    sprintf(currentResFile,"%s",__FILE__);\
    {char fullpath[150]; sprintf(fullpath, "%s%s", path, filename); SetImageWithBuffer(ImageId, fullpath, display, tmp, RES_IMAGE, isMB, __FILE__);}
#define  ADD_NFB_IMAGE(args)    \
    SetNFBImage (args);

#define ADD_APPLICATION_MEDIA(ImageId, ImgFileName) \
    SetImage(ImageId,ImgFileName,#ImageId, RES_MEDIA, __FILE__);
#define ADD_APPLICATION_MEDIA2(ImageId, filename, display)  \
    SetImageWithBuffer(ImageId, filename, display, #ImageId, RES_MEDIA, FALSE, __FILE__);
#define ADD_APPLICATION_MEDIA3(ImageId, filename, display)  \
    SetImageWithBuffer(ImageId, filename, display, #ImageId, RES_MEDIA, TRUE, __FILE__);
#define ADD_APPLICATION_MEDIA4(ImageId, tmp, path, filename, display, isMB) \
    {char fullpath[150]; sprintf(fullpath, "%s%s", path, filename); SetImageWithBuffer(ImageId, fullpath, display, tmp, RES_MEDIA, isMB, __FILE__);}

#define ADD_APPLICATION_AUDIO2(AudioId, filename, display)  \
    SetAudioWithBuffer(AudioId, filename, display, #AudioId, RES_AUDIO, FALSE, __FILE__);
#define ADD_APPLICATION_AUDIO3(AudioId, filename, display)  \
    SetAudioWithBuffer(AudioId, filename, display, #AudioId, RES_AUDIO, TRUE, __FILE__);

#define ADD_APPLICATION_AUDIO(A,B)		GetandWriteAudioResource(A,B)

#define ADD_APPLICATION_MENUITEM(args)  \
    SetMenuItem args;                   \
    strcpy(g_resmenu_filename, __FILE__);   \
    PrintMenuItems(#args);
#define ADD_APPLICATION_MENUITEM2(args) \
    SetMenuItem2 args;                  \
    strcpy(g_resmenu_filename, __FILE__);   \
    PrintMenuItems2(#args);

#define  ADD_APPLICATION(nId, pName, nMinId, nMaxId)    \
    AddApp(nId, pName, nMinId, nMaxId)
	#define	WM_ADDSTRINGMESSAGE	10000
	#define	WM_ADDIMAGEMESSAGE	10001
	#define	WM_GENSTRFILE		10003
	#define	WM_GENIMGFILE		10004
	#define	WM_GENMENUFILE		10005
	#define	WM_CLEARFILES		10007
	#define	WM_ADDAPPLICATION	10010
#else
	#define ADD_APPLICATION_STRING(stringId,string)
	#define ADD_APPLICATION_IMAGE(stringId,string)
	#define ADD_APPLICATION_MEDIA(stringId,string)
	#define ADD_APPLICATION_STRING2(stringId,string,display)
	#define ADD_APPLICATION_IMAGE2(ImageId,filename,display)
	#define ADD_APPLICATION_MEDIA2(ImageId,filename,display)
	#define ADD_APPLICATION_MENUITEM(args)
	#define	ADD_APPLICATION(nId,pName,nMinId,nMaxId)
#endif
 
	#define	ADD_SCREEN_STRINGS(args)		
	#define	ADD_SCREEN_IMAGES(args)			
	#define	ADD_SCREEN_MENUS(args)			
	#define	APPEND_SCREEN_IMAGE_LIST(arg1,arg2) 
	#define	APPEND_SCREEN_STRING_LIST(arg1,arg2) 
#endif //_PIXCOM_CUSTDATAPROTS_H

