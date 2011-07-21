/***************************************************************************
 * $Id: jdi_html.h,v 1.3 2008/06/16 05:52:17 browserdevjtbase Exp $
 * $Revision: 1.3 $
 * $Date: 2008/06/16 05:52:17 $
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
 * $Log: jdi_html.h,v $
 * Revision 1.3  2008/06/16 05:52:17  browserdevjtbase
 * Update.
 *
 * Revision 1.2  2008/05/27 06:24:58  shajudevhtmlbrow
 * WRT custom object implementation
 *
 * Revision 1.1  2007/01/10 11:23:55  kumardevhtmlbrow
 * *** empty log message ***
 *
 *  
 ***************************************************************************/

#ifndef _DOM_HTML_H_
#define _DOM_HTML_H_

/* Attribute Strings */
#define HTML_ATTR_ABBR               "abbr"
#define HTML_ATTR_ACCEPT_CHARSET     "accept-charset"
#define HTML_ATTR_ACCEPT             "accept"
#define HTML_ATTR_ACCESSKEY          "accesskey"
#define HTML_ATTR_ACTION             "action"
#define HTML_ATTR_ALIGN              "align"
#define HTML_ATTR_ALINK              "alink"
#define HTML_ATTR_ALT                "alt"
#define HTML_ATTR_ARCHIVE            "archive"
#define HTML_ATTR_AXIS               "axis"
#define HTML_ATTR_BACKGROUND         "background"
#define HTML_ATTR_BGCOLOR            "bgcolor"
#define HTML_ATTR_BORDER             "border"
#define HTML_ATTR_CELLPADDING        "cellpadding"
#define HTML_ATTR_CELLSPACING        "cellspacing"
#define HTML_ATTR_CHAR               "char"
#define HTML_ATTR_CHAROFF            "charoff"
#define HTML_ATTR_CHARSET            "charset"
#define HTML_ATTR_CHECKED            "checked"
#define HTML_ATTR_CITE               "cite"
#define HTML_ATTR_CLASS              "class"
#define HTML_ATTR_CLASSID            "classid"
#define HTML_ATTR_CLEAR              "clear"
#define HTML_ATTR_CODE               "code"
#define HTML_ATTR_CODEBASE           "codebase"
#define HTML_ATTR_CODETYPE           "codetype"
#define HTML_ATTR_COLOR              "color"
#define HTML_ATTR_COLS               "cols"
#define HTML_ATTR_COLSPAN            "colspan"
#define HTML_ATTR_COMPACT            "compact"
#define HTML_ATTR_CONTENT            "content"
#define HTML_ATTR_COORDS             "coords"
#define HTML_ATTR_DATA               "data"
#define HTML_ATTR_DATETIME           "datetime"
#define HTML_ATTR_DECLARE            "declare"
#define HTML_ATTR_DEFER              "defer"
#define HTML_ATTR_DIR                "dir"
#define HTML_ATTR_DISABLED           "disabled"
#define HTML_ATTR_ENCTYPE            "enctype"
#define HTML_ATTR_FACE               "face"
#define HTML_ATTR_FOR                "for"
#define HTML_ATTR_FRAME              "frame"
#define HTML_ATTR_FRAMEBORDER        "frameborder"
#define HTML_ATTR_HEADERS            "headers"
#define HTML_ATTR_HEIGHT             "height"
#define HTML_ATTR_HREF               "href"
#define HTML_ATTR_HREFLANG           "hreflang"
#define HTML_ATTR_HSPACE             "hspace"
#define HTML_ATTR_HTTP_EQUIV         "http-equiv"
#define HTML_ATTR_ID                 "id"
#define HTML_ATTR_ISMAP              "ismap"
#define HTML_ATTR_LABEL              "label"
#define HTML_ATTR_LANG               "lang"
#define HTML_ATTR_LANGUAGE           "language"
#define HTML_ATTR_LINK               "link"
#define HTML_ATTR_LONGDESC           "longdesc"
#define HTML_ATTR_MARGINHEIGHT       "marginheight"
#define HTML_ATTR_MARGINWIDTH        "marginwidth"
#define HTML_ATTR_MAXLENGTH          "maxlength"
#define HTML_ATTR_MEDIA              "media"
#define HTML_ATTR_METHOD             "method"
#define HTML_ATTR_MULTIPLE           "multiple"
#define HTML_ATTR_NAME               "name"
#define HTML_ATTR_NOHREF             "nohref"
#define HTML_ATTR_NORESIZE           "noresize"
#define HTML_ATTR_NOSHADE            "noshade"
#define HTML_ATTR_NOWRAP             "nowrap"
#define HTML_ATTR_OBJECT             "object"
#define HTML_ATTR_ONBLUR             "onblur"
#define HTML_ATTR_ONCHANGE           "onchange"
#define HTML_ATTR_ONCLICK            "onclick"
#define HTML_ATTR_ONDBLCLICK         "ondblclick"
#define HTML_ATTR_ONFOCUS            "onfocus"
#define HTML_ATTR_ONKEYDOWN          "onkeydown"
#define HTML_ATTR_ONKEYPRESS         "onkeypress"
#define HTML_ATTR_ONKEYUP            "onkeyup"
#define HTML_ATTR_ONLOAD             "onload"
#define HTML_ATTR_ONMOUSEDOWN        "onmousedown"
#define HTML_ATTR_ONMOUSEMOVE        "onmousemove"
#define HTML_ATTR_ONMOUSEOUT         "onmouseout"
#define HTML_ATTR_ONMOUSEOVER        "onmouseover"
#define HTML_ATTR_ONMOUSEUP          "onmouseup"
#define HTML_ATTR_ONRESET            "onreset"
#define HTML_ATTR_ONSELECT           "onselect"
#define HTML_ATTR_ONSUBMIT           "onsubmit"
#define HTML_ATTR_ONUNLOAD           "onunload"
#define HTML_ATTR_PROFILE            "profile"
#define HTML_ATTR_PROMPT             "prompt"
#define HTML_ATTR_READONLY           "readonly"
#define HTML_ATTR_REL                "rel"
#define HTML_ATTR_REV                "rev"
#define HTML_ATTR_ROWS               "rows"
#define HTML_ATTR_ROWSPAN            "rowspan"
#define HTML_ATTR_RULES              "rules"
#define HTML_ATTR_SCHEME             "scheme"
#define HTML_ATTR_SCOPE              "scope"
#define HTML_ATTR_SCROLLING          "scrolling"
#define HTML_ATTR_SELECTED           "selected"
#define HTML_ATTR_SHAPE              "shape"
#define HTML_ATTR_SIZE               "size"
#define HTML_ATTR_SPAN               "span"
#define HTML_ATTR_SRC                "src"
#define HTML_ATTR_STANDBY            "standby"
#define HTML_ATTR_START              "start"
#define HTML_ATTR_STYLE              "style"
#define HTML_ATTR_SUMMARY            "summary"
#define HTML_ATTR_TABINDEX           "tabindex"
#define HTML_ATTR_TARGET             "target"
#define HTML_ATTR_TEXT               "text"
#define HTML_ATTR_TITLE              "title"
#define HTML_ATTR_TYPE               "type"
#define HTML_ATTR_USEMAP             "usemap"
#define HTML_ATTR_VALIGN             "valign"
#define HTML_ATTR_VALUE              "value"
#define HTML_ATTR_VALUETYPE          "valuetype"
#define HTML_ATTR_VERSION            "version"
#define HTML_ATTR_VLINK              "vlink"
#define HTML_ATTR_VSPACE             "vspace"
#define HTML_ATTR_WIDTH              "width"

/* Element strings */
#define HTML_ELEMENT_ANCHOR          "A"
#define HTML_ELEMENT_ABBR            "ABBR"
#define HTML_ELEMENT_ACRONYM         "ACRONYM"
#define HTML_ELEMENT_ADDRESS         "ADDRESS"
#define HTML_ELEMENT_APPLET          "APPLET"
#define HTML_ELEMENT_AREA            "AREA"
#define HTML_ELEMENT_B               "B"
#define HTML_ELEMENT_BASE            "BASE"
#define HTML_ELEMENT_BASEFONT        "BASEFONT"
#define HTML_ELEMENT_BDO             "BDO"
#define HTML_ELEMENT_BIG             "BIG"
#define HTML_ELEMENT_BLOCKQUOTE      "BLOCKQUOTE"
#define HTML_ELEMENT_BODY            "BODY"
#define HTML_ELEMENT_BR              "BR"
#define HTML_ELEMENT_BUTTON          "BUTTON"
#define HTML_ELEMENT_CAPTION         "CAPTION"
#define HTML_ELEMENT_CENTER          "CENTER"
#define HTML_ELEMENT_CITE            "CITE"
#define HTML_ELEMENT_CODE            "CODE"
#define HTML_ELEMENT_COL             "COL"
#define HTML_ELEMENT_COLGROUP        "COLGROUP"
#define HTML_ELEMENT_DD              "DD"
#define HTML_ELEMENT_DEL             "DEL"
#define HTML_ELEMENT_DFN             "DFN"
#define HTML_ELEMENT_DIR             "DIR"
#define HTML_ELEMENT_DIV             "DIV"
#define HTML_ELEMENT_DL              "DL"
#define HTML_ELEMENT_DT              "DT"
#define HTML_ELEMENT_EM              "EM"
#define HTML_ELEMENT_EMBED           "EMBED"
#define HTML_ELEMENT_FIELDSET        "FIELDSET"
#define HTML_ELEMENT_FONT            "FONT"
#define HTML_ELEMENT_FORM            "FORM"
#define HTML_ELEMENT_FRAME           "FRAME"
#define HTML_ELEMENT_FRAMESET        "FRAMESET"
#define HTML_ELEMENT_H1              "H1"
#define HTML_ELEMENT_H2              "H2"
#define HTML_ELEMENT_H3              "H3"
#define HTML_ELEMENT_H4              "H4"
#define HTML_ELEMENT_H5              "H5"
#define HTML_ELEMENT_H6              "H6"
#define HTML_ELEMENT_HEAD            "HEAD"
#define HTML_ELEMENT_HR              "HR"
#define HTML_ELEMENT_HTML            "HTML"
#define HTML_ELEMENT_I               "I"
#define HTML_ELEMENT_IFRAME          "IFRAME"
#define HTML_ELEMENT_IMG             "IMG"
#define HTML_ELEMENT_INPUT           "INPUT"
#define HTML_ELEMENT_INS             "INS"
#define HTML_ELEMENT_ISINDEX         "ISINDEX"
#define HTML_ELEMENT_KBD             "KBD"
#define HTML_ELEMENT_LABEL           "LABEL"
#define HTML_ELEMENT_LEGEND          "LEGEND"
#define HTML_ELEMENT_LI              "LI"
#define HTML_ELEMENT_LINK            "LINK"
#define HTML_ELEMENT_MAP             "MAP"
#define HTML_ELEMENT_MENU            "MENU"
#define HTML_ELEMENT_META            "META"
#define HTML_ELEMENT_NOFRAMES        "NOFRAMES"
#define HTML_ELEMENT_NOSCRIPT        "NOSCRIPT"
#define HTML_ELEMENT_OBJECT          "OBJECT"
#define HTML_ELEMENT_OL              "OL"
#define HTML_ELEMENT_OPTGROUP        "OPTGROUP"
#define HTML_ELEMENT_OPTION          "OPTION"
#define HTML_ELEMENT_P               "P"
#define HTML_ELEMENT_PARAM           "PARAM"
#define HTML_ELEMENT_PRE             "PRE"
#define HTML_ELEMENT_Q               "Q"
#define HTML_ELEMENT_S               "S"
#define HTML_ELEMENT_SAMP            "SAMP"
#define HTML_ELEMENT_SCRIPT          "SCRIPT"
#define HTML_ELEMENT_SELECT          "SELECT"
#define HTML_ELEMENT_SMALL           "SMALL"
#define HTML_ELEMENT_SPAN            "SPAN"
#define HTML_ELEMENT_STRIKE          "STRIKE"
#define HTML_ELEMENT_STRONG          "STRONG"

#define HTML_ELEMENT_STYLE           "STYLE"
#define HTML_ELEMENT_SUB             "SUB"
#define HTML_ELEMENT_SUP             "SUP"
#define HTML_ELEMENT_TABLE           "TABLE"
#define HTML_ELEMENT_TBODY           "TBODY"
#define HTML_ELEMENT_TD              "TD"
#define HTML_ELEMENT_TEXTAREA        "TEXTAREA"
#define HTML_ELEMENT_TFOOT           "TFOOT"
#define HTML_ELEMENT_TH              "TH"
#define HTML_ELEMENT_THEAD           "THEAD"
#define HTML_ELEMENT_TITLE           "TITLE"
#define HTML_ELEMENT_TR              "TR"
#define HTML_ELEMENT_TT              "TT"
#define HTML_ELEMENT_U               "U"
#define HTML_ELEMENT_UL              "UL"
#define HTML_ELEMENT_VAR             "VAR"

/* Other string literals */
#define STR_HTML                     "HTML"
#define STR_XHTML                    "XHTML"
#define STR_SELECT_MULTIPLE          "select-multiple"
#define STR_SELECT_ONE               "select-one"
#define STR_TEXTAREA                 "textarea"
#define STR_INPUT_TYPE_TEXT          "text"
#define STR_INPUT_TYPE_PASSWORD      "password"
#define STR_INPUT_TYPE_RADIO         "radio"
#define STR_INPUT_TYPE_CHECKBOX      "checkbox"
#define STR_INPUT_TYPE_BUTTON        "button"
#define STR_INPUT_TYPE_HIDDEN        "hidden"
#define STR_INPUT_TYPE_SUBMIT        "submit"
#define STR_INPUT_TYPE_RESET         "reset"
#define STR_INPUT_TYPE_IMAGE         "image"
#define STR_INPUT_TYPE_FILE          "file"

#define STR_BOOLEAN_TRUE             "true"

#endif

/* End of File */
