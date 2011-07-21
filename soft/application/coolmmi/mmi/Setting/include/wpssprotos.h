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
 *  WPSSProtos.h
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
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: WPSSProtos.h

  	PURPOSE		: Resource IDs for the FunAndGames Application

 

 

	DATE		: 

**************************************************************/


#ifndef WPSSRPROTOS_H
#define WPSSPROTOS_H

#include "wallpaperdefs.h"
#include "downloaddefs.h"
#include "phonesetup.h"

void HighlightPhnsetScreenSaver(void);

void EntryPhnsetScrSvr(void);
void EntryPhnsetScrSvrSettings(void);
void EntryPhnsetScrSvrSelect(void);
void EntryPhnsetScrSvrSystem(void);
void EntryPhnsetScrSvrDwnl(void);
void EntryPhnsetScrSvrFmgr(void);
void EntryPhnsetScrSvrSysView(void);
void PhnsetScrSvrSysViewLskHdlr(void);
PS8 PhnsetGetCurScrSvrFileName(void);

//void ExitScrSaverMenu(void);
void ExitPhnsetScrSvrSettings(void);
//void ExitScrScreenSaverSelect(void);
//void ExitScrSaverSystem(void);
//void ExitScrSaverDownload(void);
void ExitPhnsetScrSvrSysView(void);
void ExitScrSetScreenSaver(void);

void SetScreenSaverLSK( S32 );
void SetScreenSaverSelectLSK( S32 );
void PhnsetScrSvrSysHiliteHdlr( S32 );
void PhnsetDwnlScrSvrHiliteHdlr( S32 );
U8 PhnsetConstDwnlScrSvrList(U8 **Names, U16 *DLSSImgID, S16 *HligtItem );
U8 PhnsetConstSysScrSvrList(U8 **pSysSSStrList, U16 *pSysSSImgIDList, S16 *HighliteItem );


void ScreenSaverKeyPressHandler(void);
void PhnsetScrSvrInlineHiliteHdlr(S32 nIndex);
void PhnsetScrSvrFillInline(InlineItem *);
void PhnsetScrSvrSettingLskHdlr(void);

//void ScreenSaverDoNothing(void);
BOOL PhnsetRstScrSvrDefault(void);
void PhnsetGetScrSvrSetings(U8*, U8*);
void PhnsetGetCurScrSvr(U16 *);
void PhnsetReadNvramScrSvrSettings(void);
void ssDecrementCounter(void);
void ssIncrementCounter(void);
void setScreenSaverPopupTimeout(void);
void DeleteScreenSaverViewHistory(void);
PS8 PhnsetGetCurScrSvrName(void);
PS8 PhnsetGetScrSvrNameById(U16);
void viewScreenSaver(void);
SET_SS_SETTING_STATUS PhnsetGetCurScrSvrStatus(void);
void PhnsetScrSvrSysViewPre(void);
void PhnsetScrSvrSysViewNext(void);
void UpdateDeletedImageScreenSaver(U16);
BOOL PhnsetIsScrSvrAnalogClock(void);
BOOL PhnsetIsScrSvrDigitalClock(void);
BOOL PhnsetIsScrSvrPartialOn(void);
 
BOOL PhnsetIsScrSvrCurrentTheme(void);
BOOL PhnsetIsWPCurrentTheme(void);
 
void EntryPhnsetScrSvrDwnlView(void);
void PhnsetScrSvrDwnlViewLskHdlr(void);
void ExitPhnsetScrSvrDwnlView(void);
void PhnsetScrSvrDwnlViewNext(void);
void PhnsetScrSvrDwnlViewPre(void);

void InitScreenSaver(void);
void HintPhnsetScrSvrSetting(U16 index);
void HintPhnsetScrSvrSelect(U16 index);
void HighlightPhnsetScrSvrSetting(void);
void HighlightPhnsetScrSvrSelect(void);
void HighlightPhnsetScrSvrSystem(void);
void HighlightPhnsetScrSvrDwnl(void);
void HighlightPhnsetScrSvrFmgr(void);
S32 PhnsetScreenSaverBadFileCallBack(void);
void PhnsetScrSvrSelectFmgrCallBack(void *path, int is_short);
void PhnsetScrSvrForwardFromFmgr(void *path, int is_short);
void PhnsetSetScrSvrByFileName( S8 *path );
/*Added by liuxl for fixing bug-12621 on 20090623 */
void PhnsetRstWallPaperDefaultTmp(void);
void PhnsetWallPaperRestore(U16 WpID, BOOL IsRestore);
void PhnsetGetCurWallPaper(U16 *wallPaper);

/************Wall Paper Functions *****************/
U8 PhnsetConstSysWallpaperList( U8 **pSysWallpaperStrList, U16 *pSysWallpaperImgIDList, S16 *HighliteItem );
U8 PhnsetConstDwnlWallpaperList(U8 **Names, U16 *DLWallpaperImgID, S16 *HiliteItem );

void HighlightPhnsetMainWallpaper(void); 
#ifdef __MMI_SUB_WALLPAPER__
void HighlightPhnsetSubWallpaper(void); 
#endif
void HighlightPhnsetWallpaper(void);
#ifdef __DAILY_WALLPAPER__
void HighlightPhnsetWallpaperFix(void);
void HintPhnsetWallpaperRotate(U16 index);
BOOL PhnsetGetWallpaperRotateStatus( void );
void EntryPhnsetWallpaperFixRotate(void);
//void ExitScrWallpaperFixRotate(void);
U8 PhnsetGetCurWallPaperIdx(void);
#endif
void EntryPhnsetWallpaperSel(void);
void ExitScrWallpaperSelect(void);
void EntryPhnsetWallpaperSys(void);
void EntryPhnsetWallpaperFmgr(void);
//void ExitWallpaperSystem(void);
void PhnsetWallpaperSysHiliteHandler( S32 nIndex);
void EntryPhnsetWallpaperSysView(void);
void PhnsetWallpaperSysViewPre(void);
void PhnsetWallpaperSysViewNext(void);
void ExitPhnsetWallpaperSysView(void);
void PhnsetWallpaperSet(void);
 
void ThemeManagerSetWallpaper(U16);
void ThemeManagerSetScreensaver(U16);
 
void EntryPhnsetWallpaperDwnl(void);
void ExitWallpaperDownload(void);
void PhnsetWallpaperDwnlHiliteHandler( S32 nIndex);
void EntryPhnsetWallpaperDwnlView(void);
void PhnsetWallpaperDwnlViewPre(void);
void PhnsetWallpaperDwnlViewNext(void);
void ExitPhnsetWallpaperDwnlView(void);
void PhnsetWallpaperDwnlSet(void);
pBOOL PhnsetRstWallpaperDefault(void);
//void getCurrentWallpaper(U16 *wallPaper);
void readWallpaperSettings(void);
PS8 PhnsetGetCurWallpaperName(void);
#ifdef __MMI_SUB_WALLPAPER__
PS8 PhnsetGetCurSubWallpaperName(void);
#endif
PS8 PhnsetGetWallpaperNameById(U16 Id);
void UpdateDeletedImageWallpaper(U16 ImageId);
void InitWallpaper(void);
void HighlightPhnsetWallpaperSys(void);
void HighlightPhnsetWallpaperDwnl(void);
void HighlightPhnsetWallpaperFmgr(void);
void PhnsetWallpaperSelectFmgrCallBack(void *path, int is_short);
void PhnsetWallpaperBadFileCallBack(U16 imgID);
void PhnsetWallpaperSubBadFileCallBack(U16 imgID);  
void PhnsetWallpaperForwardFromFmgr(void *path, int is_short);
#ifdef __MMI_SUB_WALLPAPER__
void PhnsetSubWallpaperForwardFromFmgr(void *path, int is_short);
#endif
void PhnsetSetWallpaperByImgID(U16 imgID);
void PhnsetSetWallpaperByFileName( S8 *path );
BOOL PhnsetSetDisplayFromAT(U8 idx, U8 type);
U8 PhnsetGetWallpaperIdxBound(void);
PS8 PhnsetGetFileNamePointer( PS8 path );


/*************WallPaper Functions********************/

BOOL PhnsetStopInvaildUserDefImg(S8 *path);

typedef enum{
	PHNSET_TYPE_DISP_VIDEO,
	PHNSET_TYPE_DISP_IMAGE,
	PHNSET_TYPE_DISP_MAX
}PHNSET_TYPE_DISP;

PHNSET_TYPE_DISP PhnsetGetDisplayType(U16 id);
void PhnsetWpSsPlayCallBack(S32 result);
void PhnsetWpUsrDefPreViewENDKeyHdlr(void);

#ifdef __MMI_POWER_ON_OFF_DISPLAY__
void readPwrOnOffDisplaySettings(void);
void HighlightPhnsetPowerOn(void);
void HighlightPhnsetPowerOff(void);
void HighlightPhnsetPwrOnFmgr(void);
void HighlightPhnsetPwrOffFmgr(void);
void HighlightPhnsetPowerOnSys(void);
void HighlightPhnsetPowerOffSys(void);
void HintPhnsetPwrOnDisplay(U16 index);
void HintPhnsetPwrOffDisplay(U16 index);
void EntryPhnsetPowerOnSel(void);
void EntryPhnsetPowerOffSel(void);
void EntryPhnsetPowerOnOffSys(void);
void EntryPhnsetPwrOnOffSysView(void);
void PhnsetPwrOnOffSysViewPre(void);
void PhnsetPwrOnOffSysViewNext(void);
void PhnsetPowerOnOffSysHiliteHandler( S32 nIndex);
U8 PhnsetConstSysPwrOnOffList( U8 **pSysPwrOnOffStrList, U16 *pSysPwrOnOffImgIDList, S16 *HighliteItem 
);
void PhnsetGetPwrOnDisp(U16 *id);
PS8 PhnsetGetPwrOnFileName(void);
void PhnsetGetPwrOffDisp(U16 *id);
PS8 PhnsetGetPwrOffFileName(void);
pBOOL PhnsetRstPwrOnDisplayDefault(void);
pBOOL PhnsetGetPwrOnDisplayDefault(U16 *id);
pBOOL PhnsetRstPwrOffDisplayDefault(void);
pBOOL PhnsetGetPwrOffDisplayDefault(U16 *id);
void PhnsetPwrOnOffPlayCallBack(S32 result);
void PhnsetPwrOnOffBadDisplayPopup(PS8 error_str);
void PhnsetSetPwrOnOffDisplay( void);
void PhnsetSetPwrOnOffDisplayByImgID( U16 imgID );
void PhnsetSetPwrOnOffDisplayByFileName( S8 *path );
void PhnsetPwrOnOffSelectFmgrCallBack(void *path, int is_short);

#ifdef _FILE_MGR_H_
void PhnsetPwrOnOffForwardFromFmgr(void *path, fmgr_tree_action_enum action, int is_short);
#endif

#endif	/* WPSSPROTOS_H */

#endif
