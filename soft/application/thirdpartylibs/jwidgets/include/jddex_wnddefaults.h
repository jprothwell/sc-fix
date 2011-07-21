/***************************************************************************
*
* File Name : jddex_wnddefaults.h
*
* IMPORTANT NOTICE
*
* Please note that any and all title and/or intellectual property rights
* in and to this Software or any part of this (including without limitation
* any images, photographs, animations, video, audio, music, text and/or
* "applets," incorporated into the Software), herein mentioned to as
* "Software", the accompanying printed materials, and any copies of the
* Software, are owned by Jataayu Software (P) Ltd., Bangalore ("Jataayu")
* or Jataayu's suppliers as the case may be. The Software is protected by
* copyright, including without limitation by applicable copyright laws,
* international treaty provisions, other intellectual property laws and
* applicable laws in the country in which the Software is being used.
* You shall not modify, adapt or translate the Software, without prior
* express written consent from Jataayu. You shall not reverse engineer,
* decompile, disassemble or otherwise alter the Software, except and
* only to the extent that such activity is expressly permitted by
* applicable law notwithstanding this limitation. Unauthorized reproduction
* or redistribution of this program or any portion of it may result in severe
* civil and criminal penalties and will be prosecuted to the maximum extent
* possible under the law. Jataayu reserves all rights not expressly granted.
*
* THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND
* AND ANY AND ALL REPRESENTATION
*
***************************************************************************
*
*
* File Description
* ----------------
*
* Purpose			:	macro definitons for defaults used by widgets
*
* Created By		:
* Created Date		:
*
*
*
* Current Revision	:
*
***************************************************************************
*
*
* Revision Details
* ----------------
*
* 1. Modified By	:
*    Modified Date	:
*	 Purpose		:
*
*
*
*
***************************************************************************/
#ifndef _JDDEX_WNDDEFAULTS_
#define _JDDEX_WNDDEFAULTS_

#ifdef __cplusplus
    extern   "C"
    {
#endif

#define JDDEX_DEFAULT_BUTTONCOLOR	jddex_PlatformGetRGBPalette(190,190,190)
#define JDDEX_DEFAULT_BGCOLOR		jddex_PlatformGetRGBPalette(0x0,0x0,0x0)
#define JDDEX_DEFAULT_FGCOLOR		jddex_PlatformGetRGBPalette(0xFF,0xFF,0xFF)

#define JDDEX_DEFAULT_BLACK		    jddex_PlatformGetRGBPalette(0x0,0x0,0x0)
#define JDDEX_DEFAULT_WHITE  		jddex_PlatformGetRGBPalette(0xFF,0xFF,0xFF)
#define JDDEX_DEFAULT_RED   		jddex_PlatformGetRGBPalette(0xFF,0x00,0x00)
#define JDDEX_DEFAULT_BLUE   		jddex_PlatformGetRGBPalette(0x00,0x00,0xFF)

#define JDDEX_BUTTON_BORDER_WIDTH 	1
#define JDDEX_IMAGE_BORDER_WIDTH 	1
#define JDDEX_SCROLL_BORDER_WIDTH 	1
#define JDDEX_DEFAULT_ICON_BORDER_WIDTH 	1
#define JDDEX_DEFAULT_INPUT_BORDER_WIDTH 	1
#define JDDEX_HIGHLIGHTED_INPUT_BORDER_WIDTH 	2
#define JDDEX_DEFAULT_LISTBOX_BORDER_WIDTH 		1
#define JDDEX_DEFAULT_COMBOBOX_BORDER_WIDTH 	1
#define JDDEX_DEFAULT_ICON_WIDTH				8
#define JDDEX_DEFAULT_ICON_HEIGHT				8
#define JDDEX_DEFAULT_COLOR  		-1
#define JDDEX_MAX_INPUT_CHAR 		255


#define CST_LCD_WIDTH  jwap_GetLcdWidth()
#define CST_LCD_HEIGHT jwap_GetLcdHeight()
#define CST_SOFTKEY_HEIGHT jwap_GetSoftkeyHeight() 
#define CST_TITLE_HEIGHT jwap_GetTitleHeight()

#define JDDEX_INVALIDATERECT_AVAIALABLE


#if 0
//for qcif
#define CST_LCD_WIDTH  176
#define CST_LCD_HEIGHT 220
#define CST_SOFTKEY_HEIGHT 20
#define CST_TITLE_HEIGHT (24+24)

/* for 221
#define CST_LCD_WIDTH  240
#define CST_LCD_HEIGHT 320
#define CST_SOFTKEY_HEIGHT 26
#define CST_TITLE_HEIGHT 26
*/
#endif
JC_UINT32 jwap_GetLcdWidth();
JC_UINT32 jwap_GetLcdHeight();
JC_UINT32 jwap_GetSoftkeyHeight();
JC_UINT32 jwap_GetTitleHeight();
JC_UINT32 jwap_GetScrollbarbarArrowHight();
JC_UINT32 jwap_GetScrollbarbarArrowWidth();
JC_UINT32 jwap_GetScrollbarWidth();
#ifdef __cplusplus
    }
#endif

#endif //_JDDEX_WNDDEFAULTS_
