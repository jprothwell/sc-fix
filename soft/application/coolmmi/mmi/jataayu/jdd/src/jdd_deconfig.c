#define MODULE_NAME "JDD_DECONFIG"
#define FILE_NAME "jdd_deconfig.c"

/***************************************************************************
 *
 * File Name : jdd_deconfig.c
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights 
 * in and to this Software or any part of this (including without limitation 
 * any images, photographs, animations, video, audio, music, text and/or 
 * "applets," incorporated into the Software), herein mentioned to as 
 * "Software", the accompanying printed materials, and any copies of the 
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu") 
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
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS 
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
 ***************************************************************************/
 
 /** 
 * @file			jdd_deconfig.c
 * @ingroup		jdd_deconfig
 * @brief		Describes the Display Engine Config related API's.
 *
 **/
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include <ddl.h>
#include <jcal.h>
#include <jdd_deconfig.h>
#include <jdi_cutils.h>
#include <jwap_mms_common.h>
/***************************************************************************
 * Macros
 **************************************************************************/

#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					OTHER_MODULE
#else
#define __MODULE_ID__					0
#endif 
#define COOLSAND

#define   WML1_CSS_STRING \
"anchor\t{text-decoration:underline; color:blue}\n"\
"card\t{display:block; unicode-bidi:embed;}\n"\
"table\t{border:1px solid; empty-cells:show}\n"\
"td\t{border:1px solid}\n"\
"table[border=\"0\"] td\t{border: 0px}\n"\
""\
"p[mode=\"wrap\"]\t{white-space: normal}\n"\
"p[mode=\"nowrap\"]\t{white-space: nowrap}\n"

#define   WML2_CSS_STRING \
"anchor, wml:anchor\t{text-decoration:underline; color:blue}\n"\
"card, wml:card\t{display:block; unicode-bidi:embed;}\n"\
"p[mode=\"wrap\"]\t{white-space:normal}\n"\
"p[mode=\"nowrap\"]\t{white-space:nowrap}\n"\
"p[wml:mode=\"wrap\"]\t{white-space:normal}\n"\
"p[wml:mode=\"nowrap\"]\t{white-space: nowrap}\n"\
""\
"table[wml:align=\"left\"]\t{text-align:left; empty-cells:show}\n"\
"table[wml:align=\"right\"]\t{text-align: right}\n"\
"table[wml:align=\"center\"]\t{text-align: center}\n"

#define COMMON_CSS_STRING \
"*\t{font-size:1em; font-family:Arial; font-weight:normal;}\n"\
""\
"*[valign=\"top\"]\t{vertical-align:top}\n"\
"*[valign=\"bottom\"]\t{vertical-align:bottom}\n"\
"*[valign=\"middle\"]\t{vertical-align:middle}\n"\
"*[valign=\"baseline\"]\t{vertical-align:baseline}\n"\
"*[xml:space=\"preserve\"]\t{white-space:pre}\n"\
""\
"*[dir=\"ltr\"]\t{direction:ltr; unicode-bidi:embed}\n"\
"*[dir=\"rtl\"]\t{direction:rtl; unicode-bidi:embed}\n"\
""\
"*:link,anchor,area\t{text-decoration:underline; color:blue}\n"\
"*:visited\t{text-decoration:underline; color:red}\n"\
""\
"abbr,acronym,cite,dfn\t{font-style:italic}\n"\
"address\t{display:block; font-style:italic}\n"\
""\
"area\t{text-indent:15px}\n"\
""\
"b\t{font-weight:bold}\n"\
""\
"bdo[dir=\"ltr\"]\t{direction:ltr}\n"\
"bdo[dir=\"rtl\"]\t{direction:rtl}\n"\
""\
"big\t{font-size:larger}\n"\
"blink\t{text-decoration:blink}\n"\
"blockquote\t{display:block; margin-left:10px}\n"\
"body,html\t{display:block;}\n"\
"br[clear=\"all\"]\t{clear:both}\n"\
"br[clear=\"both\"]\t{clear:both}\n"\
"br[clear=\"left\"]\t{clear:left}\n"\
"br[clear=\"right\"]\t{clear:right}\n"\
"caption\t{display:table-caption; text-align:center}\n"\
"caption[align=\"left\"]\t{caption-side:left; text-align:left}\n"\
"caption[align=\"right\"]\t{caption-side:right; text-align:right}\n"\
"caption[align=\"top\"]\t{caption-side:top}\n"\
"caption[align=\"bottom\"]\t{caption-side:bottom}\n"\
""\
"center\t{display:block; text-align:center}\n"\
"code\t{font-family:courier new; font-style:italic}\n"\
""\
"col\t{display:table-column}\n"\
"colgroup\t{display:table-column-group}\n"\
"colgroup > col\t{text-align:inherit; vertical-align:inherit}\n"\
""\
"dd\t{display:block; margin-left:12px}\n"\
"del\t{text-decoration:line-through}\n"\
""\
"dir\t{display:block; list-style-type:disc}\n"\
"dir[type=\"disc\"]\t{list-style-type:disc}\n"\
"dir[type=\"circle\"]\t{list-style-type:circle}\n"\
"dir[type=\"square\"]\t{list-style-type:square}\n"\
""\
"div\t{display:block}\n"\
"dl,dt\t{display:block}\n"\
"em\t{font-style:italic}\n"\
"fieldset{display:block; border:1px solid}\n"\
"form\t{display:block}\n"\
"frame\t{display:block; text-decoration:underline; color: blue}\n"\
"frameset,noframes\t{display:block}\n"\
"h1\t{display:block; font-size:22pt; margin:.67em 0}\n"\
"h2\t{display:block; font-size:20pt; margin:.83em 0}\n"\
"h3\t{display:block; font-size:18pt; margin:1em 0}\n"\
"h4\t{display:block; font-size:16pt; margin:1.33em 0}\n"\
"h5\t{display:block; font-size:14pt; margin:1.67em 0}\n"\
"h6\t{display:block; font-size:12pt; margin:2.33em 0}\n"\
"hr\t{display:block; border:1px groove; margin:4px; width:100%; color:lightgrey; -jb-halign:center}\n"\
"hr[noshade]\t{border-style:solid; color:darkgray;}\n"\
"i\t{font-style:italic}\n"\
""\
"iframe[scrolling=\"no\"][width]\t{overflow:hidden}\n"\
"iframe[scrolling=\"no\"][height]\t{overflow:hidden}\n"\
""\
"img[align=\"top\"]\t{vertical-align:text-top}\n"\
"img[align=\"middle\"]\t{vertical-align:middle}\n"\
"img[align=\"center\"]\t{vertical-align:middle}\n"\
"img[align=\"bottom\"]\t{vertical-align:text-bottom}\n"\
"img[align=\"left\"]\t{float:left}\n"\
"img[align=\"right\"]\t{float:right}\n"\
""\
"image[align=\"top\"]\t{vertical-align:text-top}\n"\
"image[align=\"middle\"]\t{vertical-align:middle}\n"\
"image[align=\"bottom\"]\t{vertical-align:text-bottom}\n"\
"image[align=\"left\"]\t{float:left}\n"\
"image[align=\"right\"]\t{float:right}\n"\
""\
"input,button,select,textarea\t{color:black}\n"\
""\
"input[type=\"image\"][align=\"top\"]\t{vertical-align:text-top}\n"\
"input[type=\"image\"][align=\"middle\"]\t{vertical-align:middle}\n"\
"input[type=\"image\"][align=\"bottom\"]\t{vertical-align:text-bottom}\n"\
"input[type=\"image\"][align=\"left\"]\t{float:left}\n"\
"input[type=\"image\"][align=\"right\"]\t{float:right}\n"\
"input,textarea\t{font-family:courier new; font-size:1em}\n"\
""\
"ins\t{text-decoration:underline}\n"\
"kbd,samp\t{font-family:courier new}\n"\
"legend\t{display:block; text-align:center; font-weight:bold; font-size:small}\n"\
""\
"li\t{display:list-item; vertical-align:inherit}\n"\
"li[type=\"disc\"]\t{list-style-type:disc}\n"\
"li[type=\"circle\"]\t{list-style-type:circle}\n"\
"li[type=\"square\"]\t{list-style-type:square}\n"\
""\
"link\t{text-decoration:underline; color:blue}\n"\
""\
"map\t{font-weight:bolder}\n"\
""\
"menu\t{display:block; list-style-type:disc}\n"\
"menu[type=\"disc\"]\t{list-style-type:disc}\n"\
"menu[type=\"circle\"]\t{list-style-type:circle}\n"\
"menu[type=\"square\"]\t{list-style-type:square}\n"\
""\
"object[align=\"top\"]\t{vertical-align:text-top}\n"\
"object[align=\"middle\"]\t{vertical-align:middle}\n"\
"object[align=\"bottom\"]\t{vertical-align:text-bottom}\n"\
"object[align=\"left\"]\t{float:left}\n"\
"object[align=\"right\"]\t{float:right}\n"\
""\
"ol\t{display:block; list-style-type:decimal; list-style-position:inherit; list-style-image:inherit;}\n"\
"ol[type=\"1\"]\t{list-style-type:decimal}\n"\
"ol[type=\"a\"]\t{list-style-type:lower-alpha}\n"\
"ol[type=\"A\"]\t{list-style-type:upper-alpha}\n"\
"ol[type=\"i\"]\t{list-style-type:lower-roman}\n"\
"ol[type=\"I\"]\t{list-style-type:upper-roman}\n"\
""\
"p\t{display:block; margin:0 0}\n"\
"pre,plaintext\t{display:block; white-space:pre; font-family:courier new}\n"\
""\
"q\t{quotes:\'\"\' \'\"\' \"\'\" \"\'\"}\n"\
"q:before\t{content:open-quote}\n"\
"q:after\t{content:close-quote}\n"\
"s,strike\t{text-decoration:line-through}\n"\
"small\t{font-size:smaller}\n"\
"strong\t{font-weight:bold}\n"\
"sub\t{vertical-align:sub}\n"\
"sup\t{vertical-align:super}\n"\
"table\t{display:table;}\n"\
"table[align=\"left\"]\t{float:left}\n"\
"table[align=\"right\"]\t{float:right}\n"\
""\
"tbody\t{display:table-row-group}\n"\
""\
"td\t{display:table-cell; vertical-align:inherit}\n"\
"td[nowrap=\"nowrap\"]\t{white-space:nowrap}\n"\
"td[nowrap=\"true\"]\t{white-space:nowrap}\n"\
"td>p:first-child\t{margin:0px}\n"\
"td>iframe\t{width:160px}\n"\
"tfoot\t{display:table-footer-group}\n"\
""\
"th\t{display:table-cell; font-weight:bold; text-align:center; vertical-align: inherit}\n"\
"tr>th[align=\"left\"]\t{text-align:left}\n"\
"tr>th[align=\"center\"]\t{text-align:center}\n"\
"tr>th[align=\"right\"]\t{text-align:right}\n"\
"tr[align]>th\t{text-align:inherit}\n"\
"th[nowrap=\"nowrap\"]\t{white-space:nowrap}\n"\
"th[nowrap=\"true\"]\t{white-space:nowrap}\n"\
""\
"th>p:first-child\t{margin:0}\n"\
"thead\t{display:table-header-group}\n"\
"tr\t{display:table-row; vertical-align:middle}\n"\
""\
"tt\t{font-weight:small; font-family:courier new}\n"\
"u\t{text-decoration:underline}\n"\
""\
"ul\t{display:block; list-style-type:disc; list-style-position:inherit; list-style-image:inherit;}\n"\
"ul[type=\"disc\"]\t{list-style-type:disc}\n"\
"ul[type=\"circle\"]\t{list-style-type:circle}\n"\
"ul[type=\"square\"]\t{list-style-type:square}\n"\
""\
"var\t{font-style:italic}\n"

#define   WML1COMMON_CSS_STRING \
"*\t{font-size:1em; font-family:Arial; font-weight:normal;}\n"\
""\
"*[valign=\"top\"]\t{vertical-align:top}\n"\
"*[valign=\"bottom\"]\t{vertical-align:bottom}\n"\
"*[valign=\"middle\"]\t{vertical-align:middle}\n"\
"*[valign=\"baseline\"]\t{vertical-align:baseline}\n"\
"*[xml:space=\"preserve\"]\t{white-space:pre}\n"\
""\
"*:link,anchor\t{text-decoration:underline; color:blue}\n"\
"*:visited\t{text-decoration:underline; color:red}\n"\
""\
"b\t{font-weight:bold}\n"\
"big\t{font-size:larger}\n"\
"br:before\t{content:\"\\A\"}\n"\
"em\t{font-style:italic}\n"\
"fieldset\t{display:block; border:1px solid}\n"\
"i\t{font-style:italic}\n"\
""\
"img[align=\"top\"]\t{vertical-align:text-top}\n"\
"img[align=\"middle\"]\t{vertical-align:middle}\n"\
"img[align=\"bottom\"]\t{vertical-align:text-bottom}\n"\
"img[align=\"left\"]\t{float:left}\n"\
"img[align=\"right\"]\t{float:right}\n"\
""\
"link\t{text-decoration:underline; color:blue}\n"\
"pre\t{display:block; white-space:pre; font-family:courier new}\n"\
"small\t{font-size:smaller}\n"\
"strong\t{font-weight:bold}\n"\
"table\t{display:table; empty-cells:show}\n"\
"td\t{display:table-cell}\n"\
"td>p:first-child\t{margin:0px}\n"\
"tr\t{display:table-row; vertical-align:middle}\n"\
"u\t{text-decoration:underline}\n"

#define   WML2COMMON_CSS_STRING \
"*\t{font-size:1em; font-family:Arial; font-weight:normal;}\n"\
""\
"*[valign=\"top\"]\t{vertical-align:top}\n"\
"*[valign=\"bottom\"]\t{vertical-align:bottom}\n"\
"*[valign=\"middle\"]\t{vertical-align:middle}\n"\
"*[valign=\"baseline\"]\t{vertical-align:baseline}\n"\
"*[xml:space=\"preserve\"]\t{white-space:pre}\n"\
""\
"*[dir=\"ltr\"]\t{direction:ltr; unicode-bidi:embed}\n"\
"*[dir=\"rtl\"]\t{direction:rtl; unicode-bidi:embed}\n"\
""\
"*:link,anchor\t{text-decoration:underline; color:blue}\n"\
"*:visited\t{text-decoration:underline; color:red}\n"\
""\
"abbr,acronym,cite,dfn\t{font-style:italic}\n"\
"address\t{display:block; font-style:italic}\n"\
"b\t{font-weight:bold}\n"\
"big\t{font-size:larger}\n"\
"blockquote\t{display:block; margin-left:10px}\n"\
"body,html\t{display:block;}\n"\
""\
"caption\t{display:table-caption; text-align:center}\n"\
"caption[align=\"left\"]\t{caption-side:left; text-align:left}\n"\
"caption[align=\"right\"]\t{caption-side:right; text-align:right}\n"\
"caption[align=\"top\"]\t{caption-side:top}\n"\
"caption[align=\"bottom\"]\t{caption-side:bottom}\n"\
""\
"code\t{font-family:courier new; font-style:italic}\n"\
""\
"col\t{display:table-column}\n"\
"colgroup\t{display:table-column-group}\n"\
"colgroup > col\t{text-align:inherit; vertical-align:inherit}\n"\
""\
"dd\t{display:block; margin-left:12px}\n"\
"div\t{display:block}\n"\
"dl,dt\t{display:block}\n"\
"em\t{font-style:italic}\n"\
"fieldset\t{display:block; border:1px solid}\n"\
"form\t{display:block}\n"\
"h1\t{display:block; font-size:22pt; margin:.67em 0}\n"\
"h2\t{display:block; font-size:20pt; margin:.83em 0}\n"\
"h3\t{display:block; font-size:18pt; margin:1em 0}\n"\
"h4\t{display:block; font-size:16pt; margin:1.33em 0}\n"\
"h5\t{display:block; font-size:14pt; margin:1.67em 0}\n"\
"h6\t{display:block; font-size:12pt; margin:2.33em 0}\n"\
"hr\t{display:block; border:1px solid; margin:4px; width:100%; color:lightgrey; -jb-halign:center}\n"\
"i\t{font-style:italic}\n"\
""\
"img[align=\"top\"]\t{vertical-align:text-top}\n"\
"img[align=\"middle\"]\t{vertical-align:middle}\n"\
"img[align=\"bottom\"]\t{vertical-align:text-bottom}\n"\
"img[align=\"left\"]\t{float:left}\n"\
"img[align=\"right\"]\t{float:right}\n"\
""\
"input,button,select,textarea\t{color:black}\n"\
"input,textarea\t{font-family:courier new; font-size:1em}\n"\
"kbd,samp\t{font-family:courier new}\n"\
"input[type=\"text\"]\t{height:22px !important; font-size:1em !important}\n"\
"input[type=\"password\"]\t{height:22px !important; font-size:1em !important}\n"\
""\
"li\t{display:list-item; vertical-align:inherit}\n"\
"li[type=\"disc\"]\t{list-style-type:disc}\n"\
"li[type=\"circle\"]\t{list-style-type:circle}\n"\
"li[type=\"square\"]\t{list-style-type:square}\n"\
""\
"link\t{text-decoration:underline; color:blue}\n"\
""\
"object[align=\"top\"]\t{vertical-align:text-top}\n"\
"object[align=\"middle\"]\t{vertical-align:middle}\n"\
"object[align=\"bottom\"]\t{vertical-align:text-bottom}\n"\
"object[align=\"left\"]\t{float:left}\n"\
"object[align=\"right\"]\t{float:right}\n"\
""\
"ol\t{display:block; list-style-type:decimal; list-style-position:inherit; list-style-image:inherit;}\n"\
"ol[type=\"1\"]\t{list-style-type:decimal}\n"\
"ol[type=\"a\"]\t{list-style-type:lower-alpha}\n"\
"ol[type=\"A\"]\t{list-style-type:upper-alpha}\n"\
"ol[type=\"i\"]\t{list-style-type:lower-roman}\n"\
"ol[type=\"I\"]\t{list-style-type:upper-roman}\n"\
""\
"p\t{display:block; margin:1em 0}\n"\
"pre\t{display:block; white-space:pre; font-family:courier new}\n"\
""\
"q\t{quotes:'\"' '\"' \"'\" \"'\"}\n"\
"q:before\t{content:open-quote}\n"\
"q:after\t{content:close-quote}\n"\
"small\t{font-size:smaller}\n"\
"strong\t{font-weight:bold}\n"\
"table\t{display:table; empty-cells:hide;}\n"\
"table[align=\"left\"]\t{float:left}\n"\
"table[align=\"right\"]\t{float:right}\n"\
""\
"tbody\t{display:table-row-group}\n"\
""\
"td\t{display:table-cell; vertical-align:inherit}\n"\
"td[nowrap=\"nowrap\"]\t{white-space:nowrap}\n"\
"td[nowrap=\"true\"]\t{white-space:nowrap}\n"\
"td>p:first-child\t{margin:0px}\n"\
"tfoot\t{display:table-footer-group}\n"\
""\
"th\t{display:table-cell; font-weight:bold; text-align:center; vertical-align:inherit}\n"\
"tr>th[align=\"left\"]\t{text-align:left}\n"\
"tr>th[align=\"center\"]\t{text-align:center}\n"\
"tr>th[align=\"right\"]\t{text-align:right}\n"\
"tr[align]>th\t{text-align:inherit}\n"\
"th[nowrap=\"nowrap\"]\t{white-space:nowrap}\n"\
"th[nowrap=\"true\"]\t{white-space:nowrap}\n"\
""\
"th>p:first-child\t{margin:0}\n"\
"thead\t{display:table-header-group}\n"\
"tr\t{display:table-row; vertical-align:middle}\n"\
""\
"u\t{text-decoration:underline}\n"\
"ul\t{display:block; list-style-type:disc; list-style-position:inherit; list-style-image:inherit;}\n"\
"ul[type=\"disc\"]\t{list-style-type:disc}\n"\
"ul[type=\"circle\"]\t{list-style-type:circle}\n"\
"ul[type=\"square\"]\t{list-style-type:square}\n"\
""\
"marquee\t{display: -wap-marquee; -wap-marquee-loop:infinite; -wap-marquee-style:scroll; -wap-marquee-dir:rtl; -wap-marquee-speed:normal}\n"\
"marquee[behavior=\"scroll\"]\t{-wap-marquee-style:scroll}\n"\
"marquee[behavior=\"slide\"]\t{-wap-marquee-style:slide}\n"\
"marquee[behavior=\"alternate\"]\t{-wap-marquee-style:alternate}\n"\
""\
"var\t{font-style:italic}\n"

#define	CFG_DEFAULT_URI					"about:blank"

#define CFG_PAGE_SIZE					(1024*100)
#define CFG_PAGE_HTML_MAX_SIZE          (1024*45)

#define CFG_CSS_SIZE					0
#define CFG_IMAGE_SIZE				(100*1024)
#define CFG_OBJECT_SIZE				(100*1024)
#define CFG_DRM_SIZE					-1

#define CFG_MAX_POINTS					50
#define CFG_TEXT_POINTS					1
#define CFG_FORM_POINTS					2
#define CFG_IMAGE_POINTS				10
#define CFG_ICON_POINTS					5
#define CFG_IMAGETEXT_POINTS			5
#define CFG_OBJECT_POINTS				10
#define CFG_SUBWINDOW_POINTS			5
#define CFG_WND_BORDER_POINTS			1
#define CFG_WALLPAPER_POINTS			5
#define CFG_RENDERING_TIMEOUT			500
#define CFG_RENDERING_TIMEOUT_COUNT		100
#define CFG_SAVEPAGE_TIMEOUT			0
	
#define CFG_LAYOUT_TIMER				10
#define CFG_LINE_SPACING				2
#define CFG_TAB_SPACE					4
#define CFG_SLOW_SCROLL					10
#define CFG_MEDIUM_SCROLL				20
#define CFG_FAST_SCROLL					30
#define CFG_LEFT_MARGIN					2
#define CFG_TOP_MARGIN					2
	
#define CFG_DEF_INPUT_SIZE				20
#define CFG_DEF_RADIO_WIDTH				13
#define CFG_DEF_RADIO_HEIGHT			13
#define CFG_DEF_CHECKBOX_WIDTH			13
#define CFG_DEF_CHECKBOX_HEIGHT			13
	
#define CFG_EXTRA_INPUT_WIDTH			4
#define CFG_EXTRA_INPUT_HEIGHT			4
#define CFG_EXTRA_LIST_WIDTH			16
#define CFG_EXTRA_LIST_HEIGHT			0
#define CFG_EXTRA_LISTITEM_HEIGHT		0
#define CFG_EXTRA_COMBO_WIDTH			16
#define CFG_EXTRA_COMBO_HEIGHT			0
#define CFG_EXTRA_OPTION_SPACE			3
#define CFG_EXTRA_BUTTON_WIDTH			4
#define CFG_EXTRA_BUTTON_HEIGHT			4
#define CFG_EXTRA_ALTIMAGE_WIDTH		5
#define CFG_EXTRA_ALTIMAGE_HEIGHT		4
#define CFG_EXTRA_TEXTAREA_WIDTH		26
#define CFG_EXTRA_TEXTAREA_HEIGHT		2
#define CFG_EXTRA_TEXTAREA_LINESPACING	2

#define CFG_DEF_INPUT_ROWS				1
#define CFG_MAX_INPUT_ROWS				1
#define CFG_DEF_INPUT_COLS				20
#define CFG_MAX_INPUT_COLS				0
#define CFG_DEF_LIST_ROWS				1
#define CFG_MAX_LIST_ROWS				1
	
#define CFG_LIST_FIRST_LEVEL			10
#define CFG_LIST_SECOND_LEVEL			6
#define CFG_LIST_THIRD_LEVEL			3
#define CFG_LIST_FOURTH_LEVEL			1
#define CFG_DEF_LIST_TAB_SPACE			2
#define CFG_LIST_MAX_CHAR_ALLIND		3
#define CFG_LIST_MAX_CHAR_MAXIND		3
#define CFG_INPUT_BORDER_SPACE			0
#define CFG_OBJECT_BORDER_SPACE			2
#define CFG_LINK_BORDER_SPACE			0

#define CFG_INPUT_VERTICAL_LINE_SPACE	0
#define CFG_OBJECT_VERTICAL_LINE_SPACE	0
#define CFG_INLINE_VERTICAL_LINE_SPACE	0

#define CFG_MIN_TBL_INLINE_CHAR			3
#define CFG_MIN_TBL_INPUT_COL			6
	
#define CFG_PIXEL_PER_INCH				96
	
#define CFG_MAX_REDIRECTION				25
#define CFG_MAX_SENT_REQUEST			2

#define CFG_MAX_INTERPAGE				8
#define CFG_MAX_NAVIGATION				15

#define CFG_DEF_CELLPADDING				2
#define CFG_DEF_CELLSPACING				2

#define	CFG_MARQUEE_INFINITE			-1
#define CFG_WAP_MARQUEE_DEFAULT_LOOP_COUNT		1
#define CFG_THIN_BORDER_WIDTH			1
#define CFG_MEDIUM_BORDER_WIDTH			2
#define CFG_THICK_BORDER_WIDTH			3
#define CFG_MARQUEE_TIMER				300
#define CFG_SLOW_MARQUEE				2
#define CFG_NORMAL_MARQUEE				5
#define CFG_FAST_MARQUEE				10

#define CFG_BORDER_SPACING				0

#define CFG_FILE_WRITE_LEN				2048
#define CFG_MAX_BUFFER_LEN				0
#define	CFG_MAX_POST_DATA_LEN			2048

#define CFG_PASSWORD_CHAR				'*'

#define CFG_MIN_DISPLAY_WIDTH			40	

#define CFG_MAX_STYLE_WIDTH				2048
#define CFG_MAX_STYLE_HEIGHT			2048

#define CFG_MAX_EMBEDPAGE				100
#define CFG_MAX_NESTED_CHILD			200

#define CFG_COMPACT_PERCENTAGE			50

static const JC_INT8 *gpacMimeTypes [] = 
{
	"text/plain", 
	"application/xml",
	"application/vnd.wap.wmlscriptc", 
	"application/vnd.wap.multipart.related", 
	"application/vnd.wap.multipart.mixed", 
	"application/vnd.wap.wmlc", 
	"application/xhtml+xml", 
	"application/vnd.wap.xhtml+xml", 
	"application/wml+xml",
	"application/vnd.wap.wml+xml",
	"text/vnd.wap.wml",
	"text/html",
	"x-multipart/related",
	"multipart/related",
	"multipart/mixed",
	//"application/x-javascript",
	//"application/x-ecmascript",
	//"application/javascript",
	//"application/ecmascript",
	//"text/javascript",
	//"text/ecmascript",
	JC_NULL
} ;


static const JC_INT8 *gpacImageTypes [] =
{
	"image/bmp",
	"image/vnd.wap.wbmp",
	"image/gif", 
	//"image/tiff", 
	"image/png", 
	"image/jpeg", 
	"image/jpg",
	JC_NULL
} ;

static const JC_INT8 *gpacIconTypes [] =
{
	"image/x-icon",
	//"image/vnd.microsoft.icon",
	"image/png",
	"image/gif",
	JC_NULL

} ;

/*
static const JC_INT8 *gpacCSSMediaTypes [] =
{
	"all",
	"screen",
	"handheld",
	JC_NULL
} ;
*/

static const JC_INT8 *gpacObjectTypes [] =
{
	"audio/sp-midi",
	"audio/midi", 
	"audio/wav",
	"audio/x-wav",
	"audio/mpeg",
	"video/mpeg",
	JC_NULL
} ;


static const JC_INT8 *gpacLinkTypes [] =
{
	"alternate",
	"author",
	"appendix",
	"bookmark",
	"chapter",
	"contents",
	"copyright",
	"glossary",
	"help",
	"home",
	"index",
	"next",
	"prev",
	"search",
	"section",
	"start",
	"subsection",
	JC_NULL
} ;


static const JC_INT8 *gpacImageExt[] = 
{
	".gif",
	".jpeg",
	".jpg",
	//".tiff",
	//".tif",
	".png",
	".wbmp",
	".bmp",
	JC_NULL
} ;


static const JC_INT8 *gpacTextExt[] = 
{
	".txt",
	JC_NULL
} ;


static const JC_INT8 *gpacObjectExt[] = 
{
	".wav",
	".mid",
	".mpg",
	JC_NULL
} ;


static const JC_INT8 *gpacMarkupExt[] = 
{
	".html",
	".htm",
	".xhtml",
	JC_NULL
} ;

static const JC_INT8 *gpacDRMTypes [] = 
{
	"application/vnd.oma.drm.message",
	"application/vnd.oma.drm.content",
	"application/vnd.oma.drm.rights+xml",
	"application/vnd.oma.drm.rights+wbxml",
	JC_NULL 
} ;

									
/**
 * @brief This function gives the list of mime types supported
 * @param No parameter
 * @return Return mime types
 */
const JC_INT8 ** jdd_DEConfigMimeTypes (void) 
{
	return gpacMimeTypes ;
}


/**
 * @brief This function gives the list of image types supported
 * @param No parameter
 * @return Return image mime types
 */
const JC_INT8 ** jdd_DEConfigImageTypes (void) 
{
	return gpacImageTypes ;
}

/**
 * @brief This function gives the list of icon types supported in increasing order of preference
 *		  (i.e. from least prefered to most prefered.
 * @param No parameter
 * @return Return icon types
 */
const JC_INT8 ** jdd_DEConfigIconTypes (void)
{
	return gpacIconTypes ;
}

/**
 * @brief This function gives the list of object types supported
 * @param No parameter
 * @return Return object types
 */
const JC_INT8 ** jdd_DEConfigObjectTypes (void)
{
	return gpacObjectTypes ;
}

/**
 * @brief This function gives the list of link types supported
 * @param No parameter
 * @return Return link types
 */
const JC_INT8 ** jdd_DEConfigLinkTypes (void)
{
	return gpacLinkTypes ;
}

/**
 * @brief This function gives the list of image extension supported
 * @param No parameter
 * @return Return image extension
 */
const JC_INT8 ** jdd_DEConfigImageExt (void)
{
	return gpacImageExt ;
}


/**
 * @brief This function gives the list of text extension supported
 * @param No parameter
 * @return Return text extension
 */
const JC_INT8 ** jdd_DEConfigTextExt (void)
{
	return gpacTextExt ;
}


/**
 * @brief This function gives the list of object extension supported
 * @param No parameter
 * @return Return object extension
 */
const JC_INT8 ** jdd_DEConfigObjectExt (void)
{
	return gpacObjectExt ;
}


/**
 
 * @param No parameter
 
 */
const JC_INT8 ** jdd_DEConfigMarkupExt (void)
{
	return gpacMarkupExt ;
}

/**
 * @brief This function gives the list of DRM types supported
 * @param No parameter
 * @return Return DRM types
 */
const JC_INT8 ** jdd_DEConfigDRMTypes (void)
{
	return gpacDRMTypes ;
}


/**
 * @brief This function gives the maximum page size allowed depending on the mime type.
 * @param No parameter
 * @return Page size, zero if no limit
 * 
 * This function gives maximum page size allowed depending upon the mime type passed. Incase of no limit,
 * zero has to be returned.
 */

JC_INT32 jdd_DEConfigPageSizeByMime (JC_INT8	*pcMimeType)
{
    if (JC_NULL != pcMimeType
     && 0 == jc_strncasecmp(pcMimeType, "text/html", 9))
    {
        return CFG_PAGE_HTML_MAX_SIZE ;
    }
	return CFG_PAGE_SIZE ;
}


/**
 * @brief This function gives the maximum page size allowed
 * @param No parameter
 * @return Page size, zero if no limit
 */
JC_INT32 jdd_DEConfigPageSize (void) 
{
	return CFG_PAGE_SIZE ;
}

/**
 * @brief	This function gives the maximum CSS content size allowed.
 * @param No parameter.
 * @return	Returns the CSS size. Incase of no limit, returns zero.
 *
 * This function returns the maximum allowed CSS content size. Incase of no limit, zero is returned.
 * Before processing the received CSS data, a comparision of the CSS's size with this value
 * ascertains whether or not the CSS data is to be processed.
 */
JC_INT32 jdd_DEConfigCSSSize (void)
{
	return CFG_CSS_SIZE ;
}

/**
 * @brief This function gives the maximum image size allowed
 * @param No parameter
 * @return Image size, zero if no limit
 */
JC_INT32 jdd_DEConfigImageSize (void) 
{
	return CFG_IMAGE_SIZE ;
}


/**
 * @brief This function gives the maximum object size allowed
 * @param No parameter
 * @return Image size, zero if no limit
 */
JC_INT32 jdd_DEConfigObjectSize (void) 
{
	return CFG_OBJECT_SIZE ;
}

/**
 * @brief This function gives the maximum object size allowed
 * @param No parameter
 * @return Image size, zero if no limit
 */
JC_INT32 jdd_DEConfigDRMSize (void) 
{
	return CFG_DRM_SIZE ;
}


/**
 * @brief This function gets points for every control
 * @param[in]  uiCtrlCnt     Control count
 * @param[out] puiCtrlPoints Reference to hold the points
 * @return No return value
 */
void jdd_DEConfigGetCtrlPoints (JC_UINT32		uiCtrlCnt,
								JC_UINT32		*puiCtrlPoints) 
{
	JC_UINT32		uiIndex ;

	if ((JC_NULL != puiCtrlPoints) && (uiCtrlCnt > 0))
	{
		for (uiIndex = 0; uiIndex < uiCtrlCnt; uiIndex++)
		{
			switch (uiIndex)
			{
				case E_DE_CFG_TEXT_BOX :
					puiCtrlPoints[uiIndex] = CFG_TEXT_POINTS ;
					break ;

				case E_DE_CFG_INPUT_BOX :
				case E_DE_CFG_BUTTON :
				case E_DE_CFG_CHECK_BOX :
				case E_DE_CFG_RADIO_BOX :
				case E_DE_CFG_LIST_BOX :
				case E_DE_CFG_COMBO_BOX :
					puiCtrlPoints[uiIndex] = CFG_FORM_POINTS ;
					break ;

				case E_DE_CFG_IMAGE :
				case E_DE_CFG_PICTOGRAM :
					puiCtrlPoints[uiIndex] = CFG_IMAGE_POINTS ;
					break ;

				case E_DE_CFG_IMAGE_TEXT :
					puiCtrlPoints[uiIndex] = CFG_IMAGETEXT_POINTS ;
					break ;

				case E_DE_CFG_ICON :
					puiCtrlPoints[uiIndex] = CFG_ICON_POINTS ;
					break ;

				case E_DE_CFG_OBJECT :
					puiCtrlPoints[uiIndex] = CFG_OBJECT_POINTS ;
					break ;

				case E_DE_CFG_SUBWINDOW :
					puiCtrlPoints[uiIndex] = CFG_SUBWINDOW_POINTS ;
					break ;

				case E_DE_CFG_WND_BORDER :
					puiCtrlPoints[uiIndex] = CFG_WND_BORDER_POINTS ;
					break ;

				case E_DE_CFG_WALLPAPER :
					puiCtrlPoints[uiIndex] = CFG_WALLPAPER_POINTS ;
					break ;

				default :
					puiCtrlPoints[uiIndex] = 0 ;
			}
		}
	}

	return ;
}


/*
 * @brief This function gives the maximum points for the control to be 
 *        rendered at a time
 * @param No parameter
 
 */
JC_UINT32 jdd_DEConfigGetMaxPoints (void) 
{
	return CFG_MAX_POINTS ;
}


/*
 * @brief This function gives the timeout for rendering
 * @param No parameter
 * @return Timeout value, zero if no limit
 */
JC_UINT32 jdd_DEConfigGetRenderingTimeout (void) 
{
	return CFG_RENDERING_TIMEOUT ;
}

/*
 
 * @param No parameter
 * @return Timeout count, zero if no limit
 */
JC_UINT32 jdd_DEConfigGetRenderingTimeoutCount (void) 
{
	return CFG_RENDERING_TIMEOUT_COUNT ;
}

/**
 * @brief This function gives the layout timer value 
 * @param No paramter
 * @return Layout timer value, should be in millisecs
 *
 * When rendering timerout happens, this value gives the timer value after
 * which it continues the layout
 */
JC_UINT32 jdd_DEConfigGetLayoutTimer (void)
{
	return CFG_LAYOUT_TIMER ;
}


/**
 * @brief This funciton gives the line spacing value
 * @param No parameter
 * @return Line spacing value
 */
JC_UINT32 jdd_DEConfigGetLineSpacing (void)
{
	return CFG_LINE_SPACING ;
}


/*
 * @brief This function gives tab space count
 * @param No parameter
 * @return Tab space count
 */
JC_UINT32 jdd_DEConfigGetTabSpace (void) 
{
	return CFG_TAB_SPACE ;
}


/*
 * @brief This function gives the slow scroll level value
 * @param No parameter
 * @return Slow scroll level
 */
JC_UINT32 jdd_DEConfigGetSlowScroll (void) 
{
	return CFG_SLOW_SCROLL ;
}


/*
 * @brief This function gives the medium scroll level value
 * @param No parameter
 * @return Medium scroll level
 */
JC_UINT32 jdd_DEConfigGetMediumScroll (void) 
{
	return CFG_MEDIUM_SCROLL ;
}


/*
 * @brief This function gives the fast scroll level value
 * @param No parameter
 * @return Fast scroll level
 */
JC_UINT32 jdd_DEConfigGetFastScroll (void) 
{
	return CFG_FAST_SCROLL ;
}


/*
 * @brief This function gives the left margin of the rendering window
 * @param No parameter
 * @return Left margin value
 */
JC_UINT32 jdd_DEConfigGetLeftMargin (void) 
{
	return CFG_LEFT_MARGIN ;
}


/*
 * @brief This function gives the right margin of the rendering window
 * @param No parameter
 * @return Left margin value
 */
JC_UINT32 jdd_DEConfigGetRightMargin (void) 
{
	return CFG_LEFT_MARGIN ;
}


/*
 * @brief This function gives the top margin of the rendering window
 * @param No parameter
 * @return Top margin value
 */
JC_UINT32 jdd_DEConfigGetTopMargin (void) 
{
	return CFG_TOP_MARGIN ;
}


/**
 * @breif This function gets the default/extra config sizes for form control
 * @param[out] pstCfgSize  Reference to config size to be filled
 * @return No return value
 */
void jdd_DEConfigGetDefaultSize (ST_DE_CFG_SIZE	 *pstCfgSize) 
{
	if (JC_NULL != pstCfgSize) 
	{
		pstCfgSize->uiDefInputSize = CFG_DEF_INPUT_SIZE ;
		pstCfgSize->uiDefRadioWidth = CFG_DEF_RADIO_WIDTH ;
		pstCfgSize->uiDefRadioHeight = CFG_DEF_RADIO_HEIGHT ;
		pstCfgSize->uiDefCheckWidth = CFG_DEF_CHECKBOX_WIDTH ;
		pstCfgSize->uiDefCheckHeight = CFG_DEF_CHECKBOX_HEIGHT ;

		pstCfgSize->uiExtraInputWidth = CFG_EXTRA_INPUT_WIDTH ;
		pstCfgSize->uiExtraInputHeight = CFG_EXTRA_INPUT_HEIGHT ;
		pstCfgSize->uiExtraListWidth = CFG_EXTRA_LIST_WIDTH ;
		pstCfgSize->uiExtraListHeight = CFG_EXTRA_LIST_HEIGHT ;
		pstCfgSize->uiExtraListItemHeight = CFG_EXTRA_LISTITEM_HEIGHT ;
		pstCfgSize->uiExtraComboWidth = CFG_EXTRA_COMBO_WIDTH ;
		pstCfgSize->uiExtraComboHeight = CFG_EXTRA_COMBO_HEIGHT ;
		pstCfgSize->uiExtraOptionSpace = CFG_EXTRA_OPTION_SPACE ;
		pstCfgSize->uiExtraButtonWidth = CFG_EXTRA_BUTTON_WIDTH ;
		pstCfgSize->uiExtraButtonHeight = CFG_EXTRA_BUTTON_HEIGHT ;
		pstCfgSize->uiExtraAltImageWidth = CFG_EXTRA_ALTIMAGE_WIDTH ;
		pstCfgSize->uiExtraAltImageHeight = CFG_EXTRA_ALTIMAGE_HEIGHT ;
		pstCfgSize->uiExtraTextareaWidth = CFG_EXTRA_TEXTAREA_WIDTH ;
		pstCfgSize->uiExtraTextareaHeight = CFG_EXTRA_TEXTAREA_HEIGHT ;
		pstCfgSize->uiTextareaLineSpacing = CFG_EXTRA_TEXTAREA_LINESPACING ;

		pstCfgSize->uiDefInputRows = CFG_DEF_INPUT_ROWS ;
		pstCfgSize->uiMaxInputRows = CFG_MAX_INPUT_ROWS ;
		pstCfgSize->uiDefInputCols = CFG_DEF_INPUT_COLS ;
		pstCfgSize->uiMaxInputCols = CFG_MAX_INPUT_COLS ;
		pstCfgSize->uiDefListRows = CFG_DEF_LIST_ROWS ;
		pstCfgSize->uiMaxListRows = CFG_MAX_LIST_ROWS ;

		pstCfgSize->uiListIndentSpace[0] = CFG_LIST_FIRST_LEVEL ;
		pstCfgSize->uiListIndentSpace[1] = CFG_LIST_SECOND_LEVEL ;
		pstCfgSize->uiListIndentSpace[2] = CFG_LIST_THIRD_LEVEL ;
		pstCfgSize->uiListIndentSpace[3] = CFG_LIST_FOURTH_LEVEL ;
		pstCfgSize->uiListTabSpace = CFG_DEF_LIST_TAB_SPACE ;
		pstCfgSize->uiListMaxCharAllInd = CFG_LIST_MAX_CHAR_ALLIND ;
		pstCfgSize->uiListMaxCharMaxInd = CFG_LIST_MAX_CHAR_MAXIND ;
		pstCfgSize->uiInputBorderSpace = CFG_INPUT_BORDER_SPACE ;
		pstCfgSize->uiObjectBorderSpace = CFG_OBJECT_BORDER_SPACE ;
		pstCfgSize->uiLinkBorderSpace = CFG_LINK_BORDER_SPACE ;

		pstCfgSize->uiInputVerticalLineSpace = CFG_INPUT_VERTICAL_LINE_SPACE ;
		pstCfgSize->uiObjectVerticalLineSpace = CFG_OBJECT_VERTICAL_LINE_SPACE ;
		pstCfgSize->uiInlineVerticalLineSpace = CFG_INLINE_VERTICAL_LINE_SPACE ;

		pstCfgSize->uiMinTblInlineChar = CFG_MIN_TBL_INLINE_CHAR ;
		pstCfgSize->uiMinTblInputCols = CFG_MIN_TBL_INPUT_COL ;

	}

	return ;
}


/**
 * @brief This funciton gets the PPI for the current screen
 * @param No parameter
 * @return PPI value
 */
JC_UINT32 jdd_DEConfigGetPPI (void)
{
	return CFG_PIXEL_PER_INCH ;
}


/**
 * @brief This function gets the maximum redirection count
 * @param No parameter
 
 */
JC_UINT32 jdd_DEConfigMaxRedirection (void) 
{
	return CFG_MAX_REDIRECTION ;
}

/**
 * @brief This function gets the maximum request count needs to be sent at a time
 * @param No parameter
 
 */
JC_UINT32 jdd_DEConfigMaxRequestCount (void)
{
	return CFG_MAX_SENT_REQUEST ;
}

/**
 * @brief This function gets the maximum interpage count
 * @param No parameter
 
 */
JC_UINT32 jdd_DEConfigMaxInterpage (void) 
{
	return CFG_MAX_INTERPAGE ;
}


/**
 * @brief This function gets the maximum navigation count
 * @param No parameter
 
 */
JC_UINT32 jdd_DEConfigGetNavigationCount (void) 
{
	return CFG_MAX_NAVIGATION ;
}


/**
 * @brief This function gets the default cell padding value
 * @param No parameter
 * @return cell padding value
 */
JC_UINT32 jdd_DEConfigGetCellPadding (void)
{
	return CFG_DEF_CELLPADDING ;
}


/**
 * @brief This function gets the default cell spacing value
 * @param No parameter
 * @return cell spacing value
 */
JC_UINT32 jdd_DEConfigGetCellSpacing (void)
{
	return CFG_DEF_CELLSPACING ;
}


/**
 * @brief This function gets the marquee infinite value
 * @param No parameter
 * @return marquee infinite value
 */
JC_INT32 jdd_DEConfigGetMarqueeInfinite (void)
{
	return CFG_MARQUEE_INFINITE ;
}


/**
 * @brief This function gets the default loop count for wap-marquee
 * @param No parameter
 * @return marquee default loop count
 */
JC_INT32 jdd_DEConfigGetDefaultWAPMarqueeLoopCount (void)
{
	return CFG_WAP_MARQUEE_DEFAULT_LOOP_COUNT ;
}


/**
 * @brief This function gets the thin border value
 * @param No parameter
 * @return thin border value
 */
JC_UINT32 jdd_DEConfigGetThinBorderWidth (void)
{
	return CFG_THIN_BORDER_WIDTH ;
}


/**
 * @brief This function gets the medium border value
 * @param No parameter
 * @return medium border value
 */
JC_UINT32 jdd_DEConfigGetMediumBorderWidth (void)
{
	return CFG_MEDIUM_BORDER_WIDTH ;
}


/**
 * @brief This function gets the thick border value
 * @param No parameter
 * @return thick border value
 */
JC_UINT32 jdd_DEConfigGetThickBorderWidth (void)
{
	return CFG_THICK_BORDER_WIDTH ;
}


/**
 * @brief This function gives the marquee timer value 
 * @param No paramter
 * @return marquee timer value, should be in millisecs
 */
JC_UINT32 jdd_DEConfigGetMarqueeTimer (void)
{
	return CFG_MARQUEE_TIMER ;
}


/**
 * @brief This function gets the slow marquee pixel move value
 * @param No parameter
 * @return slow marquee value
 */
JC_UINT32 jdd_DEConfigGetSlowMarquee (void)
{
	return CFG_SLOW_MARQUEE ;
}


/**
 * @brief This function gets the normal marquee pixel move value
 * @param No parameter
 * @return normal marquee value
 */
JC_UINT32 jdd_DEConfigGetNormalMarquee (void)
{
	return CFG_NORMAL_MARQUEE ;
}


/**
 * @brief This function gets the fast marquee pixel move value
 * @param No parameter
 * @return fast marquee value
 */
JC_UINT32 jdd_DEConfigGetFastMarquee (void)
{
	return CFG_FAST_MARQUEE ;
}

/**
 * @brief This function gets the number of pixels that needs to be provided
 *		  between the border and the internal controls.
 * @param No parameter
 * @return fast marquee value
 */
JC_UINT32 jdd_DEConfigGetBorderSpacing (void)
{
	return CFG_BORDER_SPACING ;
}

/**
 * @brief This function gets the CSS Buffer based on the type
 * @param[in]  eType        CSS file type
 * @param[out] ppucBuffer   Reference to hold the css buffer
 * @param[out] puiLength    Reference to hold the buffer length
 * @param[out] pbFreeMemory Reference to hold the flag for freeing the memory
 * @retval JC_OK  Success on gettnig the buffer
 * @retval Others Error on getting the buffer
 */
JC_RETCODE jdd_DEConfigGetCSSBuffer (E_DE_CFG_CSS_TYPE		eType,
									 JC_INT8				**ppcBuffer,
									 JC_UINT32				*puiLength,
									 JC_BOOLEAN				*pbFreeMemory) 
{
//	JC_CHAR			*pmFileName = JC_NULL ;
//	JC_HANDLE		vFileSysHandle ;
//	JC_HANDLE		vFileHandle ;
	JC_RETCODE		rCode ;

//	rCode = JC_OK ;
//	pmFileName = JC_NULL ;
	if ((JC_NULL == ppcBuffer) || (JC_NULL == puiLength) || 
		(JC_NULL == pbFreeMemory))
	{
		rCode = JC_ERR_INVALID_PARAMETER ;
	}
	else
	{
		switch (eType)
		{
			case E_DE_WML1_CSS :
    			*pbFreeMemory = E_FALSE;
    			*ppcBuffer = WML1_CSS_STRING;
    			*puiLength = jc_strlen(*ppcBuffer);
    			rCode = JC_OK;
//				rCode = jdi_CUtilsCharToTchar (JC_NULL, CFG_WML1_CSS, &pmFileName) ;
				break ;

			case E_DE_WML2_CSS :
    			*pbFreeMemory = E_FALSE;
    			*ppcBuffer = WML2_CSS_STRING;
    			*puiLength = jc_strlen(*ppcBuffer);
    			rCode = JC_OK;
//				rCode = jdi_CUtilsCharToTchar (JC_NULL, CFG_WML2_CSS, &pmFileName) ;
				break ;

			case E_DE_WML1_COMMON_CSS :
    			*pbFreeMemory = E_FALSE;
    			*ppcBuffer = WML1COMMON_CSS_STRING;
    			*puiLength = jc_strlen(*ppcBuffer);
    			rCode = JC_OK;
//				rCode = jdi_CUtilsCharToTchar (JC_NULL, CFG_WML1_COMMON_CSS, &pmFileName) ;
				break ;

			case E_DE_WML2_COMMON_CSS :
    			*pbFreeMemory = E_FALSE;
    			*ppcBuffer = WML2COMMON_CSS_STRING;
    			*puiLength = jc_strlen(*ppcBuffer);
    			rCode = JC_OK;
//				rCode = jdi_CUtilsCharToTchar (JC_NULL, CFG_WML2_COMMON_CSS, &pmFileName) ;
				break ;

			case E_DE_COMMON_CSS :
          *pbFreeMemory = E_FALSE;
          *ppcBuffer = COMMON_CSS_STRING;
          *puiLength = jc_strlen(*ppcBuffer);
    			rCode = JC_OK ;
//				rCode = jdi_CUtilsCharToTchar (JC_NULL, CFG_COMMON_CSS, &pmFileName) ;
				break ;

			default :
				rCode = JC_ERR_INVALID_PARAMETER ;
				break ;
		}
	}

#if 0
	if (JC_OK == rCode)
	{
		rCode = jdd_FSInitialize (&vFileSysHandle) ;
		if (JC_OK == rCode)
		{
			vFileHandle = jdd_FSOpen (vFileSysHandle, pmFileName, 
									  E_OPEN_READ_MODE) ;
			if (JC_NULL == vFileHandle)
			{
				rCode = JC_ERR_FILE_OPEN ;
			}
			else
			{
				rCode = jdd_FSGetSize (vFileHandle, puiLength) ;
				if ((JC_OK == rCode) && (*puiLength > 0))
				{
					*ppcBuffer = (JC_INT8 *) jdd_MemAlloc ((*puiLength + 1), 
														   sizeof (JC_INT8)) ;
					if (JC_NULL == *ppcBuffer)
					{
						rCode = JC_ERR_MEMORY_ALLOCATION ;
					}
					else
					{
						jdd_FSRead (*ppcBuffer, sizeof (JC_INT8), 
									*puiLength, vFileHandle) ;
						*pbFreeMemory = E_TRUE ;
					}
				}
				jdd_FSClose (vFileHandle) ;
			}
			jdd_FSDeinitialize (vFileSysHandle) ;
		}
		jdd_MemFree (pmFileName) ;
	}
#endif

	return rCode ;
}


/**
 * @brief This function gets the number of bytes that can be written 
 *		  into a file at a time. If it is 0 then it is assumed that 
 *		  there is no limit on the file write operation
 * @param No parameter
 * @return Length of a file write operation
 */
JC_UINT32 jdd_DEConfigGetFileWriteLength (void)
{
	return CFG_FILE_WRITE_LEN ;
}


/**
 * @brief This function gets the buffer length on which it decides when to
 *		  write the content in to the file. If it is -1 never write the 
 *		  content buffer to file. If it is 0 each time content need to write.
 *		  If it is > 0  then append the content to the buffer until it reaches
 *		  the limit, and once it reaches it needs to write in the file.
 *		  there is no limit on the file write operation
 * @param No parameter
 * @return buffer length
 */

JC_INT32 jdd_DEConfigGetMaxBufferLength (void)
{
	return CFG_MAX_BUFFER_LEN ;
}


/**
 * @brief This function gets config path where the temporary content files
 *		  to store.
 * @param No parameter
 * @return config path
 */
JC_RETCODE jdd_DEConfigGetConfigPath (JC_CHAR		*pmPath,
									  JC_UINT32		*puiSize) 
{
	JC_UINT32		uiLen ;
	JC_RETCODE		rCode ;

	uiLen = jc_strlen (CFG_DE_CONFIG_PATH) ;
	if ((JC_NULL != pmPath) && (uiLen < *puiSize))
	{
		jdi_CUtilsTcsStrCpy (CFG_DE_CONFIG_PATH, pmPath) ;
		rCode = JC_OK ;

		if (jdd_FSIsDirExist (JC_NULL, pmPath))
		{
			jdd_FSDelDir (JC_NULL, pmPath) ;
		}
		rCode = jdd_FSMakeDir (JC_NULL, pmPath) ;
	}
	else
	{
		*puiSize = uiLen + 1 ;
		rCode = JC_ERR_CFG_INSUFFICIENT_MEMORY ;
	}
	return rCode ;
}

/**
 * @brief This function gets the path separator to be used in file and 
 *		  directory names.
 * @param[in,out] pcSlash	holds the path separator
 * @return No return value
 */
void jdd_DEConfigGetPathSeparator (JC_INT8	*pcSlash)
{
	jc_strcpy (pcSlash, CFG_PATH_SEPARATOR) ;
}


/**
 * @brief This function gets the flag for redrawing when bgimage is set
 * @param No parameter
 * @retval E_TRUE  Redraw is required when bgimage is set
 * @retval E_FALSE Redraw is not required when bgimage is set 
 */
JC_BOOLEAN jdd_DEConfigRedrawBgImage (void) 
{
	return E_FALSE ;
}



/**
 * @brief This function gets the password character used to find the size of input box
 * @param No parameter
 * @return Password character
 */
JC_CHAR jdi_DEConfigGetPasswordChar (void)
{
	return CFG_PASSWORD_CHAR ;
}


/**
 * @brief This function gets the flag for setting timer when refresh event 
 *        occurs through refresh tag.
 * @param No parameter
 * @retval E_TRUE  Set Timer on refresh (if exists)
 * @retval E_FALSE Ignore Timer on refresh 
 */
JC_BOOLEAN jdd_DEConfigTimerOnRefresh (void) 
{
	return E_TRUE ;
}


/**
 * @brief This function gets the scale type when to apply image scaling
 * @param No parameter
 * @return Returns scaling type
 */
E_DE_CFG_SCALE_TYPE jdd_DEConfigScaleImage (void)
{
	return E_SCALE_NODIM ;
}


/**
 * @brief This function gets the scale type when to apply object scaling
 * @param No parameter
 * @return Returns scaling type
 */
E_DE_CFG_SCALE_TYPE jdd_DEConfigScaleObject (void)
{
	return E_SCALE_NODIM ;
}


/**
 * @brief This function gets the flag for display default accesskey for invalid accesskey 
 * @param No parameter
 * @retval E_TRUE  Ignore Accesskey
 * @retval E_FALSE Display default accesskey (if free)
 */
JC_BOOLEAN jdd_DEConfigIgnoreInvalidAccesskey (void)
{
	return E_TRUE ;
}


/**
 * @brief This function gets the minimum display area width
 * @param No parameter
 * @return Returns minimum display area width
 */
JC_INT32 jdd_DEConfigGetMinDisplayWidth (void)
{
	return CFG_MIN_DISPLAY_WIDTH ;
}


/**
 * @brief This function gets the maximum width of an object/block specified by style,
 *        When exceeds ignores this width and uses object width.
 * @param No parameter
 * @return Returns object maximum width
 */
JC_INT32 jdd_DEConfigGetStyleMaxWidth (void)
{
	return CFG_MAX_STYLE_WIDTH ;
}


/**
 * @brief This function gets the maximum height of an object/block specified by style,
 *        When exceeds ignores this width and uses object width.
 * @param No parameter
 * @return Returns object maximum height
 */
JC_INT32 jdd_DEConfigGetStyleMaxHeight (void)
{
	return CFG_MAX_STYLE_HEIGHT ;
}


/**
 * @brief This function gets the flag for saving non-cachable content
 * @param No parameter
 * @retval E_TRUE  Save both cachable and non-cachable content
 * @retval E_FALSE Save only cachable content
 */
JC_BOOLEAN jdd_DEConfigSaveNonCachableContent (void) 
{
	return E_TRUE ;
}


/**
 * @brief This function ignores the negative or zero dimension attribute for 
 *        image and object 
 * @param No parameter
 * @retval E_TRUE  Ignores and uses the image/object's dimension
 * @retval E_FALSE Creates the control with zero dimension
 */
JC_BOOLEAN  jdd_DEConfigIgnoreNegativeZeroDimension (void)
{
	return E_FALSE ;
}


/**
 * @brief This function returns the flag for adjust textarea height
 * @param No parameter
 * @retval E_TRUE  Adjust textarea height
 * @retval E_FALSE Use the specified height
 */
JC_BOOLEAN jdd_DEConfigAdjustTextareaHeight (void) 
{
	return E_FALSE ;
}


/*
 * @brief This function gives the timeout for savepage functionality
 * @param No parameter
 * @return Timeout value, zero if no limit
 */
JC_UINT32 jdd_DEConfigGetSavePageTimeout (void)
{
	return CFG_SAVEPAGE_TIMEOUT ;
}

/**
 * @brief This function gets the number of bytes of post data that can be read 
 *		  from an input file incase of file upload.
 * @param No parameter
 
 */
JC_UINT32 jdd_DEConfigGetMaxPostDataLength (void)
{
	return CFG_MAX_POST_DATA_LEN ;
}


/**
 * @brief This function gets the maximum embed page count
 * @param No parameter
 
 */
JC_UINT32 jdd_DEConfigMaxEmbedPage (void) 
{
	return CFG_MAX_EMBEDPAGE ;
}


/**
 * @brief This function gets the maximum nested child XML tree can hold
 * @param No parameter
 
 */
JC_UINT32 jdd_DEConfigMaxNestedChild (void) 
{
#ifdef XML_DEBUG_INFO
	return 0 ;
#else
	return CFG_MAX_NESTED_CHILD ;
#endif
}

/**
 * @brief This function gets the percentage value by which UL/OL should be compacted
 *		  when compact attribute has been specified for UL and OL elements.
 * @param No parameter
 * @return percentage by which UL/OL should be compacted
 */
JC_UINT32 jdd_DEConfigCompactPercentage (void) 
{
	return CFG_COMPACT_PERCENTAGE ;
}

/**
 * @brief This function obtains the default URI for the browser windows.
 * @param    No parameter
 * @retval	BROWSER_DEFAULT_URI	Specifies the default browser URI.
 *
 * This function obtains the default URI for the browser windows.
 */
JC_INT8 * jdd_DEConfigGetDefaultURI (void) 
{
	return CFG_DEFAULT_URI ;
}

/**
 * @breif This function specifies the flag for creating the main window
 * @param     No parameter
 
 * @retval E_FALSE If window with background property to be created with the visible width and height
 *
 * This function used to get a flag for dimension to be used for creating the main window when 
 * its background property are specified like background color.
 */
JC_BOOLEAN jdd_DEConfigCreateFullWindow (void) 
{
	return E_TRUE ;
}


/**
 * @breif This function returns true if the select tag always need to be trated as listbox. The "size" 
 * attribute should be ignored and all items should be visible.
 * @param     No parameter
 * @retval E_TRUE	If the select tag should be treated as listbox always and size attribute should 
 *					be ignored
 * @retval E_FALSE	The decision whether select tag should be treated as combo box or list box will be
 *					taken by DE considering "size" attribute.
 *
 */
JC_BOOLEAN jdd_DEConfigListBoxForSelect (void) 
{
	return E_FALSE ;
}

/* END OF FILE */
 
#endif
