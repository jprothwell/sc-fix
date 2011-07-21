/***************************************************************************
 * $Id: jdi_browsersettings.h,v 1.12 2007/06/15 10:49:12 sriramdevhtmlbrow Exp $
 * $Revision: 1.12 $
 * $DateTime: $
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, AND/OR FITNESS 
 * FOR A PARTICULAR PURPOSE OR USE, TITLE OR INFRINGEMENT ARE EXPRESSLY 
 * DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. YOU ASSUME THE 
 * ENTIRE RISK AS TO THE ACCURACY AND THE USE OF THIS SOFTWARE. JATAAYU 
 * SHALL NOT BE LIABLE FOR ANY CONSEQUENTIAL, INCIDENTAL, INDIRECT, 
 * EXEMPLARY, SPECIAL OR PUNITIVE DAMAGES INCLUDING WITHOUT LIMITATION 
 * ANY LOSS OF DATA, OR; LOSS OF PROFIT, SAVINGS BUSINESS OR GOODWILL 
 * OR OTHER SIMILAR LOSS RESULTING FROM OR OUT OF THE USE OR INABILITY 
 * TO USE THIS SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdi_browsersettings.h,v $
 * Revision 1.12  2007/06/15 10:49:12  sriramdevhtmlbrow
 * Updated copyright information
 *
 *  
 ***************************************************************************/

#ifndef _JDI_BROWSER_SETTINGS_H
#define _JDI_BROWSER_SETTINGS_H

typedef enum _e_mode_value
{
	E_MODE_VALUE_NATURAL ,
	E_MODE_VALUE_F2S,
	E_MODE_VALUE_CONFORMANCE,
#ifdef BROWSER_MOBILE_WEB
	E_MODE_VALUE_MOBILEWEB
#endif
} E_MODE_VALUE ;

typedef enum _e_image_value
{
	E_IMAGE_VALUE_ENABLE ,
	E_IMAGE_VALUE_DISABLE ,
	E_IMAGE_VALUE_SMARTIMAGE
} E_IMAGE_VALUE ;

typedef enum _e_selectable_image_value
{
	E_SELECTABLEIMAGE_VALUE_ON ,
	E_SELECTABLEIMAGE_VALUE_OFF
} E_SELECTABLEIMAGE_VALUE ;

typedef enum _e_direct_image_value
{
	E_DIRECTIMAGE_VALUE_ON ,
	E_DIRECTIMAGE_VALUE_OFF
} E_DIRECTIMAGE_VALUE ;

typedef enum _e_fullscreen_value
{
	E_FULLSCREEN_VALUE_ON ,
	E_FULLSCREEN_VALUE_OFF 		
} E_FULLSCREEN_VALUE	;

typedef enum e_textsize_value
{
	E_TEXTSIZE_VALUE_LARGER ,
	E_TEXTSIZE_VALUE_LARGEST ,
	E_TEXTSIZE_VALUE_MEDIUM ,
	E_TEXTSIZE_VALUE_SMALLER ,
	E_TEXTSIZE_VALUE_SMALLEST
} E_TEXTSIZE_VALUE ;

typedef enum _e_audio_value
{
	E_AUDIO_VALUE_ENABLE ,
	E_AUDIO_VALUE_DISABLE
} E_AUDIO_VALUE ;

typedef enum _e_scriptiong_value
{
	E_SCRIPTING_VALUE_ENABLE ,
	E_SCRIPTING_VALUE_DISABLE 
} E_SCRIPTING_VALUE ;

typedef enum _e_popupwindow_value
{
	E_POPUPWINDOW_VALUE_ENABLE ,
	E_POPUPWINDOW_VALUE_DISABLE  
} E_POPUPWINDOW_VALUE ;

typedef enum _e_accesskey_value
{
	E_ACCESSKEY_DISPLAY_END ,
	E_ACCESSKEY_DISPLAY_FRONT ,
	E_ACCESSKEY_DISPLAY_NONE ,
	E_ACCESSKEY_DISABLED
} E_ACCESSKEY_VALUE ;

typedef enum e_max_display_width_value
{
	E_MAX_DISPLAY_WIDTH_400 ,
	E_MAX_DISPLAY_WIDTH_800
} E_MAX_DISPLAY_WIDTH_VALUE ;

typedef enum _e_meta_refresh_value
{
	E_META_REFRESH_ENABLE,
	E_META_REFRESH_DISABLE
} E_META_REFRESH_VALUE ;

typedef enum e_author_css_value
{
	E_AUTHOR_CSS_ENABLE,
	E_AUTHOR_CSS_DISABLE
} E_AUTHOR_CSS_VALUE ;

typedef enum e_embedded_style_value
{
	E_EMBEDDED_STYLE_BLOCK_LEVEL,
	E_EMBEDDED_STYLE_PAGE_LEVEL
} E_EMBEDDED_STYLE_VALUE ;

typedef enum _e_strict_xhtml_value
{
	E_STRICT_XHTML_DISABLE,
	E_STRICT_XHTML_ENABLE
} E_STRICT_XHTML_VALUE ;

typedef struct _settings_display 
{
	E_MODE_VALUE				eModeValue ;
	E_IMAGE_VALUE				eImageValue ;
	E_SELECTABLEIMAGE_VALUE		eSelectableImageValue ;
	E_DIRECTIMAGE_VALUE			eDirectImageValue ;
	E_FULLSCREEN_VALUE			eFullScreenValue ;
	E_TEXTSIZE_VALUE			eTextSizeValue ;
	E_AUDIO_VALUE				eAudioValue ;
	E_SCRIPTING_VALUE			eScriptingValue ;
	E_POPUPWINDOW_VALUE			ePopupWindowValue ;
	E_ACCESSKEY_VALUE			eAccesskeyValue ;
	E_MAX_DISPLAY_WIDTH_VALUE	eMaxDisplayWidth ;
	E_META_REFRESH_VALUE		eMetaRefreshValue ;
	E_STRICT_XHTML_VALUE		eStrictXhtmlValue ;
	JC_CHAR						*pmMinFontSize ;

} ST_SETTINGS_DISPLAY ;

typedef struct _settings_css
{
	E_AUTHOR_CSS_VALUE			eAuthorCSSValue ;
	E_EMBEDDED_STYLE_VALUE		eEmbeddedStyleValue ;
	JC_INT8						*pacCSSMediaTypes [MAX_CSS_MEDIA_TYPES] ;
	JC_UINT32					uiSelectTypeCount ;
} ST_SETTINGS_CSS ;

typedef enum _serviceindication_value
{
	E_SERVICEINDICATION_VALUE_VISUALALERT ,
	E_SERVICEINDICATION_VALUE_AUDIOALERT ,
	E_SERVICEINDICATION_VALUE_VIBRATEALERT ,
	E_SERVICEINDICATION_VALUE_SILENT
} E_SERVICEINDICATION_VALUE	;

typedef enum _serviceloading_value
{
	E_SERVICELOADING_VALUE_AUTOLOAD ,
	E_SERVICELOADING_VALUE_AUDIOALERT ,
	E_SERVICELOADING_VALUE_VIBRATEALERT ,
	E_SERVICELOADING_VALUE_SILENT
} E_SERVICELOADING_VALUE	;

typedef enum e_cache_operation
{
	E_CACHEOPERATION_ALLOW ,
	E_CACHEOPERATION_IGONORE ,
} E_CACHEOPERATION ;
typedef struct _settings_push
{
	E_SERVICEINDICATION_VALUE	eServiceIndicationValue ;
	E_SERVICELOADING_VALUE		eServiceLoadingValue ;
	E_CACHEOPERATION			eCacheOperation ;
} ST_SETTINGS_PUSH ;
typedef enum _e_cache_value
{
	E_CACHE_VALUE_ENABLE ,
	E_CACHE_VALUE_DISABLE
} E_CACHE_VALUE ;

typedef enum _e_cookie_value
{
	E_COOKIE_VALUE_ENABLE ,
	E_COOKIE_VALUE_DISABLE
} E_COOKIE_VALUE ;

typedef struct _settings_cachecookie
{
	E_CACHE_VALUE		eCacheValue ;
	E_COOKIE_VALUE		eCookieValue ;
} ST_SETTINGS_CASHCOOKIE ;

typedef struct _settings_shortcuts
{
	JC_CHAR		*pmKeywords ;
	JC_CHAR		*pmScheme ;
} ST_SETTINGS_SHORTCUTS ;

typedef struct _browser_settings
{
	ST_SETTINGS_DISPLAY		stSettingsDisplay ;
	ST_SETTINGS_CSS			stSettingsCSS ;
	ST_SETTINGS_PUSH		stSettingsPush ;
	ST_SETTINGS_CASHCOOKIE	stSettingsCashCookie ;
	ST_SETTINGS_SHORTCUTS	stSettingsShortcuts ;
} ST_BROWSER_SETTINGS ;
#endif

