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
 *  DownloadProtos.h
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

	FILENAME	: DownloadProtos.h

  	PURPOSE		: Resource IDs for the FunAndGames Application

 

 

	DATE		: 

**************************************************************/

#ifndef DOWNLOADPROTOS_H
#define DOWNLOADPROTOS_H

#include "audioinc.h"
#include "downloaddefs.h"
/* 
** Define
*/

/* 
** Typedef 
*/

/* 
** Extern Global Variable
*/

/* 
** Extern Global Function
*/
void InitDownload(void);
void InitDownloadNVRAM(void);

U8 GetFullScreenFactoryImagesInfo(DYNIMAGEINFO ** factoryFileInfo);
U8 GetAssociatedFactoryImagesInfo(DYNIMAGEINFO ** factoryFileInfo);
U8 GetDownloadImagesInfo(DYNIMAGEINFO ** downloadFileInfo);
U8 GetFactoryTonesInfo(DYNAUDIOINFO ** factoryAudioInfo);
U8 GetDownloadTonesInfo(DYNAUDIOINFO ** downloadAudioInfo);
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
U8 GetThemeV2TonesInfo(DYNAUDIOINFO  **downloadThemeTones); //ritesh
#endif
#if defined(__MMI_IMELODY_SUPPORT__)
U8 GetFactoryIMelodyInfo(DYNAUDIOINFO ** factoryAudioInfo);
U8 GetDownloadIMelodyInfo(DYNAUDIOINFO ** downloadAudioInfo);
#endif

S8 AddDownloadedImage(S8 *fileName);
S8 AddDownloadedTone(S8 *fileName);
#if defined(__MMI_IMELODY_SUPPORT__)
S8 AddDownloadedIMelody(S8 *fileName);
#endif

pBOOL DeleteDownloadImageIndexByFileName(U8 * imageName );
pBOOL DeleteDownloadAudioIndexByFileName( U8 * audioName);
#if defined(__MMI_IMELODY_SUPPORT__)
pBOOL DeleteDownloadImelodyIndexByFileName(U8 *imelodyName);
#endif

U8 GetFreeDownloadImageSlot(void);
U8 GetFreeDownloadAudioSlot(void);
#if defined(__MMI_IMELODY_SUPPORT__)
U8 GetFreeDownloadImelodySlot(void);
#endif

S8 *GetImageNameInFileSystem(U16 ImageId);
S8 *GetImageFileSystem(U16 ImageId);
S8 *GetAudioNameWithPathInFileSystem(U16 AudioId, PS8 AudioData);

void PlayAudioFileSystem(U16 AudioId, U8 style);
void StopAudioFileSystem(U16 AudioId);
U16 GetCurrentAudioID (void);
#if 0 //modified by lin for saver screen @20070620
U8 getFactoryImageRelativeIndex(U16 ImageID);
#else
#define getFactoryImageRelativeIndex(ImageID) 0
#endif //modified by lin for saver screen @20070620

void SyncDownloadFileSystem(S8 *DirPath, DYNAMICINFO* dynInfo);

void HighlightDwnlMenu(void);
void Highlight_DOWNLOAD_IMAGE(void);
void Highlight_DOWNLOAD_AUDIO(void);

void EntryDwnlMenu(void);

void EntryDwnlImageMenu(void);
void EntryDwnlAudioMenu(void);
void ExitDwnlAudioMenu(void);

void EntryDwnlImageOption(void);

void EntryDwnlAudioOption(void);
void EntryDwnlDeleteImageConfirmation(void);

void EntryDwnlDeleteImageProgress(void);
void EntryDwnlDeleteAudioConfirm(void);
void EntryDwnlDeleteAudioProgress(void);
void EntryDwnlImageView(void);

void ExitDwnlImageView(void);
void HighlightDwnlAudioOthers(void);
void EntryForbiddenScreen(void);

void GetDownloadImageIndex(S32 nIndex);
void HighlightDwnlImage(void);
void HighlightDwnlAudio(void);
void EntryDwnlImageStandard(void);
void HighlightDwnlImageStandard(void);

void GetDownloadAudioIndex(S32 nIndex);
void EntryDwnlIMedloy(void);
void ExitDwnlIMedloy(void);
void EntryDwnlOtherTones(void);
void ExitDwnlOtherTones(void);
void HighlightDwnlAudioSend(void);
void HighlightDwnlAudioDelete(void);
void HighlightDwnlImageView(void);
void HighlightDwnlImageDelete(void);

void ViewNextDownloadImage(void);
void ViewPreDownloadImage(void);

void DataFileListInfoReqInd(void *msg);
void DataFileModifyReqInd(void *msg);
void DataFileExecuteReqInd(void *msg);
U8 EntryDwnlPCToolDisplayImage(PS8 filename);
U8 EntryDwnlPCToolPlayAudio(PS8 filename);
void ExitDwnlPCToolPlayAudio(void);

void PlayDownloadAudio(void);
U8 GetFileExtension(S8*	pFilename);


U8 DeleteDwnlFileByName(PS8 Filename, U16  Type);
S8 DeleteDownloadedImage(U16 ImageId);
S8 DeleteDownloadedAudio(U16 AudioId);


extern INT pfError(INT err);
#if defined(__MMI_RING_COMPOSER__) && defined(__MMI_IMELODY_SUPPORT__)
extern U8 GetComposedIMelodyInfo(DYNAUDIOINFO ** downloadAudioInfo);
extern S8 *GetComposedIMelodyNameInFileSystem(U16 AudioId);
#endif

#if defined(__MMI_MESSAGES_EMS__) && defined(__MMI_IMELODY_SUPPORT__)
extern U8 GetDownloadEmsIMelodyInfo (DYNAUDIOINFO ** downloadAudioInfo);
#endif // defined(__MMI_MESSAGES_EMS__)



#endif


