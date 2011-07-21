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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * EmsMiscell.H
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for MMI SMS APP.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**************************************************************

	FILENAME	: MessagesMiscell.h

  	PURPOSE		: ---

 

	AUTHOR		: Hiran and Magesh k

	DATE		: ---

**************************************************************/

#ifndef _PIXCOM_EMS_MISCELL_H
#define _PIXCOM_EMS_MISCELL_H


//CSD added for compile issue
#include "l4dr.h"
//CSD end
#include "mmi_data_types.h"
#include "ems.h"


typedef struct
{
	EMSData   emsData;
	U8        validFlag;
}msg_ems_data_struct;
//
#if defined(__MMI_MESSAGES_EMS__)
extern U8 		GetFmtTextSize (void);
extern void		SetFmtTextSize (void);
extern void		SetFmtTextStyle (void);
extern U8 		GetFmtTextAlignment (void);
extern void		SetFmtTextAlignment (void);
extern void		SetFmtTextNewParagraph (void);
extern void		EntryScrEmsFormatTextMenu (void);
extern void		ExitScrEmsFormatTextMenu (void);
extern void		EntryScrFmtTextSizeMenu (void);
extern void		EntryScrFmtTextStyleMenu (void);
extern void		EntryScrFmtAlignmentMenu (void);
extern void		EntryScrFmtNewParagraphMenu (void);

extern void     HighlightEditOptionInsertObject (void) ;
extern void	HighlightEmsFormattingTextHandler (void);

extern void     HighlightInsObjPicture          (void) ;
extern void     HighlightInsObjMyPicture        (void) ;
extern void     HighlightInsObjPredefinedAnimation (void) ;
extern void     HighlightInsObjMyAnimation      (void) ;
extern void     HighlightInsObjMelody			(void) ;
extern void     HighlightInsObjMyMelody         (void) ;
extern void     HighlightInsObjPredefinedSound  (void) ;
//
extern void EntryEMSObjectList (void);
extern void ExitEMSObjectList (void);
extern void EntryScrInsertObjectMenu (void);
extern void ExitScrInsertObjectMenu (void);
extern void PreEntryScrSaveObjectName (void);
extern void EntryScrSaveObjectName (void);
extern void ExitScrSaveObjectName (void);
extern void EntryScrSaveObjectList (void);
extern void ExitScrSaveObjectList (void);
extern void EntryScrPreviewEmsObject (void);
extern void ExitScrPreviewEmsObject (void);
extern void GetEmsDwonloadObjectDisplayListDetails (U16 *lskStrID, U8** strItemListPtr);
extern void PreviewEmsObjectIsDone (void);
extern void SaveEmsObjectNameAndDownLoadEmsObject (void);
extern void GetEmsObjectHiliteIndex (S32 nIndex);
extern void ChangeEmsObjectTitleWithIndex (S32 nIndex);
extern void StopEmsObjectIMelodyAndSoundIfAny (void);
extern void PlayEmsManufacturerSound (void);
#if defined(__MMI_IMELODY_SUPPORT__)
extern void PlayWapDownlaodIMelody (void);
extern void PlayUserComposeIMelody (void);
extern void PlayEmsManufacturerIMelody (void);
extern void PlayEmsUserDefinedIMelody (void);
#endif // defined(__MMI_IMELODY_SUPPORT__)
#ifdef __DOWNLOAD__
extern void EntryScrnListUserDefinedPictureOrAnimations (void);
extern void ExitScrnListUserDefinedPictureOrAnimations (void);
void UpdateImageDataForSyncDownloadFileSystemFunc (void);
void RefreshEmsDwonloadObjectNamesAfterDelete (void);
extern void PreDeleteEmsDownloadObjectFrmFunAndGames (void);
extern void DeleteEmsDownloadObjectFrmFunAndGames (void);
extern void ViewEmsDownloadObjectFrmFunAndGames (void);
extern void SendImagesByEMS (void);
extern void EntryFunctionSendByEms (void);
extern void EntryScrnEmsObjectOptionsList (void);
extern void ExitScrnEmsObjectOptionsList (void);
extern void FreeDownloadEmsPictureObjectNamesBuff (void);
extern void AllocDownloadEmsPictureObjectNamesBuff (U8 nObjects);
extern void FreeDownloadEmsAnimtaionObjectNamesBuff (void);
extern void AllocDownloadEmsAnimationObjectNamesBuff (U8 nObjects);
extern void GotoIdleAfterFreeDownloadEmsPictureAnimtaionObjectNamesBuff (void);
extern void GoBackFrmEntryScrnListUserDefinedPictureOrAnimations (void);
extern S8 ReadDownloadObjectNamesFromNvram (void);
#if defined(__MMI_IMELODY_SUPPORT__)
extern void MemsetEmsMelodyInfoStruct (void);
extern void UpdateEmsMelodyInfoStruct (void);
U8* GetEmsMelodyNameWithPathInFileSystem (U16 AudioId);
#endif
void HighlightEmsObjectViewHandler (void);
void HighlightSendByEMSHandler (void);
void HighlightEmsObjectDeleteHandler (void);
#endif
void EntryEmsSaveObjectOption (void);
void ExitEmsSaveObjectOption (void);
void HighlightEmsSaveObjectDone (void);
void mmi_ems_create_ems_folder (void);

extern U8 EmsPictureNum(void);
#if defined(__MMI_IMELODY_SUPPORT__)
extern U8 EmsMelodyNum(void);
#endif
#endif // defined(__MMI_MESSAGES_EMS__)
#if defined (__MMI_VCARD__) || defined (__MMI_VCALENDAR__)
void mmi_ems_send_vobject (void);
EMSTATUS mmi_ems_wrap_vobject(void);
#endif

//
extern void ErrorHandlingWhileInsertingObject (EMSTATUS emsObjectStatus);
#endif	/* _PIXCOM_EMS_MISCELL_H */


