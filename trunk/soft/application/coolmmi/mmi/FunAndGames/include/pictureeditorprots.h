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
 *  PictureEditorProts.h
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

	FILENAME	: PictureEditorProts.h

  	PURPOSE		: Funtion Prototypes for the Picture Editor Application

 

 

	DATE		: Jan-13-2004

**************************************************************/

#ifndef PICTUREEDITORPROTS_H
#define PICTUREEDITORPROTS_H

#include "gui_data_types.h"
#include "pictureeditordefs.h"
#include "downloaddefs.h"

 
#ifdef __MMI_TOUCH_SCREEN__

#include "kal_non_specific_general_types.h"
#include "touchscreengprot.h"

typedef enum {						
	GUI_IMAGE_EDITOR_PEN_NONE,
	GUI_IMAGE_EDITOR_PEN_TITLE_EVENT	
}mmi_image_editor_pen_enum;

#endif // __MMI_TOUCH_SCREEN__ 
/*~]*/
/**********************Global function prototypes************************/
//Picture Editor 
void InitPictureEditor(void);
void InitPicEditPen(void);
void EntryScr4PicEditSelectMenu(void);
void ExitScr4PicEditSelectMenu(void);
void ChangePicEditSelectMenuSoftKey(S32 nIndex);
void EntryPicEditAddPic(void);

void ShowPicEditPrevImage(void);
void ShowPicEditNextImage(void);
void ShowPicEditImage(void);
void EntryScr4PicEditViewOpt(void);
void ExitScr4PicEditViewOpt(void);
void SelPEViewOptEdit(void);
void SelPEViewOptDelete(void);
void SelPEViewOptDeleteAll(void);
void EntryPicEditViewOpt(void);
void EntryScr4PicEditDeleteConfirm(void );
void ExitScr4PicEditDeleteConfirm(void);
void PicEditDeleteImage(void);
MMI_BOOL SetDeletePicEditImage(void);
void EntryScr4PicEditDeleteAllConfirm(void );
void ExitScr4PicEditDeleteAllConfirm(void);
void PicEditDeleteAllImages(void);
void SetDeleteAllPicEditImage(void);

void EntryScr4PicEditEditOpt(void);
void ExitScr4PicEditEditOpt(void);
void SelPEEditOptSavePic(void);
void SelPEEditOptSavePicAs(void);
void SelPEEditOptPenSetting(void);
void SelPEEditOptFillPic(void);
void SelPEEditOptMark(void);
void SelPEEditOptCut(void);
void SelPEEditOptCopy(void);
void SelPEEditOptPaste(void);
void SelPEEditOptUndo(void);
void SelPEEditOptRedo(void);
void SelPEEditOptZoom(void);
void SelPEEditOptFullSize(void);
void SelPEEditOptResize(void);
void SelPEEditOptHelp(void);
void EntryPEEditOpt(void);
void EntryScr4PicEditSaveAs(void);
void ExitScr4PicEditSaveAs(void);
void SetPicEditCurrPic(S32 nIndex);
void PicEditSaveImageAs(void);
void EntryScr4PicEditSaveAsConfirm(void );
void ExitScr4PicEditSaveAsConfirm(void);
void GoBackPicEditSaveAsConfirmHistory( void );
void ReplacePicEditSelectedImage(void);
void EntryScr4PicEditPenSetting(void);
void ExitScr4PicEditPenSetting(void);
void GoBackPicEditPenSettingHistory( void );
void PicEditPenSettingInlineEdit(S32 nIndex) ;
void PicEditPenSettingSelectInlineEdit(S32 index) ;
void ChangePenSettingSoftkey(void);
void EntryScr4PicEditPenSettingSaveConfirm(void );
void ExitScr4PicEditPenSettingSaveConfirm(void);
void GoBackPicEditPenSettingConfirmHistory( void );
void SavePicEditPenSetting(void);
void EntryScr4PicEditFillPic(void);
void ExitScr4PicEditFillPic(void);
void ChangeSoftkeyFillPic(S32 index) ;
void PicEditFillPicSelectInlineEdit(S32 index) ;
void EntryScr4PicEditFillPicSaveConfirm(void );
void ExitScr4PicEditFillPicSaveConfirm(void);
void GoBackPicEditFillPicConfirmHistory( void );
void SavePicEditFillPic(void);
void EntryScr4PicEditScrResize(void);
void ExitScr4PicEditScrResize(void);
void FillPicEditResizeInLine(InlineItem *ResizeInline);
void HighlightResizeInlineItemHandler(S32 index) ;
void ChangeResizeSoftKey(S32 nIndex);
void SetResizeInlineItemValues(void);
void ValidateScrWidthHeight(void);
void EntryScr4PicEditHelp(void);
void ExitScr4PicEditHelp(void);
void EntryScr4PicEditCustomColor(void);
void ExitScr4PicEditCustomColor(void);
void SavePicEditCustomColor(void);
void EntryScr4PicEditSaveImageConfirm(void );
void ExitScr4PicEditSaveImageConfirm(void);
void GoBackPicEditSaveImageConfirmHistory( void );
void PicEditSaveImage(void);
void SetPicEditPenAtCenter(BOOL bFlag);
void SetPicEditPenBoundary(void);
s32 SavePicEditImageDataBuff(void);
U8 SearchPicEditFileName(S8* sFile);
U8* GetPicEditCurrentImageName(void);
U16 GetPicEditCurrentImageId(void);
BOOL SetPicEditPenMarkBoundary(void);
U8 GetPicEditScr(void);
BOOL CheckPicEditPenSettingChange(void);
void ReadPicEditListFromNVRAM(void);
void WritePicEditListToNVRAM(void);
S8 AddPictureEditorImage(S8 *fileName);
S8 AddPictureEditorImageAtIndex(S8 *fileName,U8 nIndex);
S8 DeletePictureEditorImage(U16 ImageId);
U8 GetPictureEditorImageInfo(DYNIMAGEINFO ** pictureEditorInfo);
S8 *GetPictureEditorImageNameInFileSystem(U16 ImageId);
S8 *GetPictureEditorImageNameWithPathInFileSystem(U16 ImageId);

//Picture Editor Engine
void ShowCategoryScr4PicEdit(U16 colorStringId,color_t penColor,U16 pixelStringId,U8 penPixel, U16 penImageId,U16 left_softkey,U16 left_softkey_icon,U16 right_softkey,U16 right_softkey_icon,U16 background_imageid,U8 *imageData);
void ShowPicEditTitleBar(U16 colorStringId,color_t penColor,U16 pixelStringId,U8 penPixel,U16 penImageId);
void RedrawPicEditTitleBar(void);
void ShowPicEditImageWindow(U8 *imageData);
void ShowPicEditMarkBox(U16 scrMinX,U16 scrMinY);
void DisplayPECursor(void);
void EntryScr4PicEditShowImage(void);
void ExitScr4PicEditShowImage(void);
void ChangePicEditShowImageSoftkey(void);
void EntryScr4PicEditEditImage(void);
void ExitScr4PicEditEditImage(void);
void ChangePicEditEditImageSoftkey(void);
void GoBackHistory4PicEditEditImage(void);
void PECommitLastAction(void);
void PECancelLastAction(void);
void SetPicEditZoomWindow(BOOL bFlag);

//Picture Editor Handlers - Key
void InitPicEditKeyHandlers(void);
void InitPicEditHiliteHandlers(void);
void DrawPicOnKey_1(void);
void DrawPicOnKey_2(void);
void DrawPicOnKey_3(void);
void DrawPicOnKey_4(void);
void DrawPicOnKey_5(void);
void DrawPicOnKey_6(void);
void DrawPicOnKey_7(void);
void DrawPicOnKey_8(void);
void DrawPicOnKey_9(void);
void DrawPECursor(U16 key);
void ChangePEZoomFactor(void);
void ChangePEPenSize(void);
void ChangePEPenColor(void);
 
#if defined (__MMI_UI_STYLE_4__)
void ChangePEFillColor(void);
#endif
 
BOOL CheckPenMove(U8 movX, U8 movY);
void SetPenMove(U8 movX, U8 movY);

 

#ifdef __MMI_TOUCH_SCREEN__
void mmi_register_image_editor_pen_handlers(void);
void mmi_image_editor_pen_up_handler(mmi_pen_point_struct point);
void mmi_image_editor_pen_down_handler(mmi_pen_point_struct point);
void mmi_image_editor_pen_move_handler(mmi_pen_point_struct point);
void mmi_image_editor_pen_abort_handler(mmi_pen_point_struct point);
 
#if 0 //should be defined(__MMI_UI_STYLE_4__)
/* under construction !*/
#endif
 
pBOOL mmi_general_image_editor_pen_handler(mmi_pen_event_type_enum pen_event,S16 x, S16 y, mmi_image_editor_pen_enum *image_editor_event);
pBOOL HandleImageEditorTitleEvent(U16 X,U16 Y);
void SetPenPosition(U16 movX, U16 movY);
void DrawPECursorAt(U16 x,U16 y);

#endif	//__MMI_TOUCH_SCREEN_
/*~]*/

//Picture Editor Handlers - File
byte *ParseImageBuffer(byte *imageBuffer, PE_File_Header *imageFileHeader, PE_Info_Header *imageInfoHeader);
void SetImageBuffer(S8* fileBuff,PE_File_Header *imageFileHeader,PE_Info_Header *imageInfoHeader);
void PasteBoxInImageBuffer(byte *destBuffer,byte *srcBuffer);
void DrawZoomImage(byte* inBuffer, U32 nImage_X, U32 nImage_Y, U8 nZoom_Factor, U32 nScr_X, U32 nScr_Y, U32 nScr_Height, U32 nScr_Width );
void InitRedoUndoBuffers(void);
void SavePEContext(void);
void ExchangePEContext(void);
void SaveDataBuffer(void);
void SetPicEditEmptyImageDataBuff(void);
BOOL SetPicEditImageDataBuff(U8 *fileName);
void ResizePicEditImageDataBuff(void);
void GetPaletteInfo(byte* info_palette);
void SetCurrentImageParams(void);
U16 GetImageWidth(U16 xMin, U16 xMax);
color_t GetPEColor(UINT32 selColor);
void SyncPicEditFileSystem(S8 *DirPath, DYNAMICINFO* dynInfo);

/**********************Extern Global Variables************************/
extern InlineItem	cat92_inline_items[];
extern s32 cat92_color_index;
extern InlineItem	cat93_inline_items[];
extern U8 hintData[MAX_SUB_MENUS][MAX_SUB_MENU_HINT_SIZE];

extern void UI_draw_dashed_rectangle(s32 x1,s32 y1,s32 x2,s32 y2,color_t c);

#endif   //PICTUREEDITORPROTS_H

