/***************************************************************************
 * $Id: jdi_browserattributes.h,v 1.16 2009/01/28 06:00:52 sriramdevhtmlbrow Exp $
 * $Revision: 1.16 $
 * $Date: 2009/01/28 06:00:52 $
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
 * $Log: jdi_browserattributes.h,v $
 * Revision 1.16  2009/01/28 06:00:52  sriramdevhtmlbrow
 * Added new attribute "valign" to the list.
 *
 * Revision 1.15  2008/09/11 07:30:44  kavithadevhtmlbrow
 * Compact Attribute Support
 *
 * Revision 1.14  2008/07/31 04:31:41  kavithadevhtmlbrow
 * Data: Scheme for Inline Req.
 *
 * Revision 1.13  2008/07/25 14:10:17  sriramdevhtmlbrow
 * Fixed issue in ordering
 *
 * Revision 1.12  2008/07/24 11:48:56  kavithadevhtmlbrow
 * nohref in Area Tag
 *
 * Revision 1.11  2008/07/24 09:34:36  shajudevhtmlbrow
 * lang and xml-lang support added.
 *
 * Revision 1.10  2008/07/15 05:05:13  sriramdevhtmlbrow
 * Added new attributes "ijam", "ista", "ilet", "irst", "iswf" for anchor tag which are defined by imode specification
 *
 * Revision 1.9  2008/07/10 11:02:17  kavithadevhtmlbrow
 * Noshade Attribute for HR
 *
 * Revision 1.8  2008/07/08 07:23:15  kavithadevhtmlbrow
 * UTN Attribute (Senidng ID) Support
 *
 * Revision 1.7  2008/06/25 07:26:48  kavithadevhtmlbrow
 * Declare Attribute for Object Tag
 *
 * Revision 1.6  2008/05/09 10:33:16  shajudevhtmlbrow
 * Changes for firing onload on an element only once.
 *
 * Revision 1.5  2008/04/14 11:01:16  kavithadevhtmlbrow
 * gpcAttrXRequest added to avoid duplicate request being given
 *
 * Revision 1.4  2007/11/13 08:00:48  kavithadevhtmlbrow
 * charset attribute added
 *
 * Revision 1.3  2007/08/08 07:19:26  kavithadevhtmlbrow
 * DE_PPP_READWRITE_ENABLE & Password Manager
 *
 * Revision 1.2  2007/05/11 10:55:02  kavithadevhtmlbrow
 * new tags and attribute added
 *
 * Revision 1.1  2007/05/10 05:27:25  kavithadevhtmlbrow
 * Browser Attribute extern variable
 *
 *
 ***************************************************************************/

/**
 * @file jdi_attributes.h
 * @ingroup Browser
 * @brief Defines the Attributes
 */

#ifndef _JDI_ATTRIBUTE_H
#define _JDI_ATTRIBUTE_H

/* Extern Global Attribute Names */

extern JC_CONST_INT8_P gpcAttrAccept ;
extern JC_CONST_INT8_P gpcAttrAcceptcharset ;
extern JC_CONST_INT8_P gpcAttrAccesskey ;
extern JC_CONST_INT8_P gpcAttrAction ;
extern JC_CONST_INT8_P gpcAttrAlign ;
extern JC_CONST_INT8_P gpcAttrAlink ;
extern JC_CONST_INT8_P gpcAttrAlt ;
extern JC_CONST_INT8_P gpcAttrAlternate ;
extern JC_CONST_INT8_P gpcAttrAutocomplete ;

extern JC_CONST_INT8_P gpcAttrBackground ;
extern JC_CONST_INT8_P gpcAttrBehaviour ;
extern JC_CONST_INT8_P gpcAttrBgcolor ;
extern JC_CONST_INT8_P gpcAttrBgproperties ;
extern JC_CONST_INT8_P gpcAttrBorder ;

extern JC_CONST_INT8_P gpcAttrCachecontrol ;
extern JC_CONST_INT8_P gpcAttrCellpadding ;
extern JC_CONST_INT8_P gpcAttrCellspacing ;
extern JC_CONST_INT8_P gpcAttrChar ;
extern JC_CONST_INT8_P gpcAttrCharoff ;
extern JC_CONST_INT8_P gpcAttrCharset ;
extern JC_CONST_INT8_P gpcAttrChecked ;
extern JC_CONST_INT8_P gpcAttrClass ;
extern JC_CONST_INT8_P gpcAttrClassid ;
extern JC_CONST_INT8_P gpcAttrCodebase ;
extern JC_CONST_INT8_P gpcAttrColor ;
extern JC_CONST_INT8_P gpcAttrCols ;
extern JC_CONST_INT8_P gpcAttrColspan ;
extern JC_CONST_INT8_P gpcAttrColumns ;
extern JC_CONST_INT8_P gpcAttrCompact ;
extern JC_CONST_INT8_P gpcAttrContent ;
extern JC_CONST_INT8_P gpcAttrConversion ;
extern JC_CONST_INT8_P gpcAttrCoords ;

extern JC_CONST_INT8_P gpcAttrData ;
extern JC_CONST_INT8_P gpcAttrDeclare ;
extern JC_CONST_INT8_P gpcAttrDefer ;
extern JC_CONST_INT8_P gpcAttrDirection ;
extern JC_CONST_INT8_P gpcAttrDisabled ;
extern JC_CONST_INT8_P gpcAttrDomain ;
extern JC_CONST_INT8_P gpcAttrDown ;

extern JC_CONST_INT8_P gpcAttrEmptyok ;
extern JC_CONST_INT8_P gpcAttrEnctype ;
extern JC_CONST_INT8_P gpcAttrEnterbackward ;
extern JC_CONST_INT8_P gpcAttrEnterforward ;
extern JC_CONST_INT8_P gpcAttrEscape ;

extern JC_CONST_INT8_P gpcAttrFace ;
extern JC_CONST_INT8_P gpcAttrFixed ;
extern JC_CONST_INT8_P gpcAttrFor ;
extern JC_CONST_INT8_P gpcAttrFormat ;
extern JC_CONST_INT8_P gpcAttrFrameborder ;

extern JC_CONST_INT8_P gpcAttrHeight ;
extern JC_CONST_INT8_P gpcAttrHref ;
extern JC_CONST_INT8_P gpcAttrHspace ;
extern JC_CONST_INT8_P gpcAttrHttpequiv ;

extern JC_CONST_INT8_P gpcAttrIcon ;
extern JC_CONST_INT8_P gpcAttrId ;
extern JC_CONST_INT8_P gpcAttrIjam ;
extern JC_CONST_INT8_P gpcAttrIlet ;
extern JC_CONST_INT8_P gpcAttrImage ;
extern JC_CONST_INT8_P gpcAttrIname ;
extern JC_CONST_INT8_P gpcAttrInfinite ;
extern JC_CONST_INT8_P gpcAttrInputmode ;
extern JC_CONST_INT8_P gpcAttrIrst ;
extern JC_CONST_INT8_P gpcAttrIsmap ;
extern JC_CONST_INT8_P gpcAttrIsta ;
extern JC_CONST_INT8_P gpcAttrIswf ;
extern JC_CONST_INT8_P gpcAttrIvalue ;

extern JC_CONST_INT8_P gpcAttrLabel ;
extern JC_CONST_INT8_P gpcAttrLang ;
extern JC_CONST_INT8_P gpcAttrLanguage ;
extern JC_CONST_INT8_P gpcAttrLeft ;
extern JC_CONST_INT8_P gpcAttrLink ;
extern JC_CONST_INT8_P gpcAttrLocalsrc ;
extern JC_CONST_INT8_P gpcAttrLoop ;

extern JC_CONST_INT8_P gpcAttrMarginheight ;
extern JC_CONST_INT8_P gpcAttrMarginwidth ;
extern JC_CONST_INT8_P gpcAttrMaxlength ;
extern JC_CONST_INT8_P gpcAttrMedia ;
extern JC_CONST_INT8_P gpcAttrMethod ;
extern JC_CONST_INT8_P gpcAttrMultiple ;

extern JC_CONST_INT8_P gpcAttrName ;
extern JC_CONST_INT8_P gpcAttrNewcontext ;
extern JC_CONST_INT8_P gpcAttrNo ;
extern JC_CONST_INT8_P gpcAttrNohref ;
extern JC_CONST_INT8_P gpcAttrNoshade ;

extern JC_CONST_INT8_P gpcAttrOnblur ;
extern JC_CONST_INT8_P gpcAttrOnchange ;
extern JC_CONST_INT8_P gpcAttrOnclick ;
extern JC_CONST_INT8_P gpcAttrOndblclick ;
extern JC_CONST_INT8_P gpcAttrOnenterbackward ;
extern JC_CONST_INT8_P gpcAttrOnenterforward ;
extern JC_CONST_INT8_P gpcAttrOnfocus ;
extern JC_CONST_INT8_P gpcAttrOnkeydown ;
extern JC_CONST_INT8_P gpcAttrOnkeypress ;
extern JC_CONST_INT8_P gpcAttrOnkeyup ;
extern JC_CONST_INT8_P gpcAttrOnload ;
extern JC_CONST_INT8_P gpcAttrOnmousedown ;
extern JC_CONST_INT8_P gpcAttrOnmousemove ;
extern JC_CONST_INT8_P gpcAttrOnmouseout ;
extern JC_CONST_INT8_P gpcAttrOnmouseover ;
extern JC_CONST_INT8_P gpcAttrOnmouseup ;
extern JC_CONST_INT8_P gpcAttrOnpick ;
extern JC_CONST_INT8_P gpcAttrOnreset ;
extern JC_CONST_INT8_P gpcAttrOnselect ;
extern JC_CONST_INT8_P gpcAttrOnsubmit ;
extern JC_CONST_INT8_P gpcAttrOntimer ;
extern JC_CONST_INT8_P gpcAttrOnunload ;

extern JC_CONST_INT8_P gpcAttrPath ;
extern JC_CONST_INT8_P gpcAttrPreserve ;
extern JC_CONST_INT8_P gpcAttrPrev ;
extern JC_CONST_INT8_P gpcAttrPrompt ;

extern JC_CONST_INT8_P gpcAttrReadonly ;
extern JC_CONST_INT8_P gpcAttrRefresh ;
extern JC_CONST_INT8_P gpcAttrRel ;
extern JC_CONST_INT8_P gpcAttrRev ;
extern JC_CONST_INT8_P gpcAttrRight ;
extern JC_CONST_INT8_P gpcAttrRows ;
extern JC_CONST_INT8_P gpcAttrRowspan ;

extern JC_CONST_INT8_P gpcAttrScroll ;
extern JC_CONST_INT8_P gpcAttrSelected ;
extern JC_CONST_INT8_P gpcAttrSendreferer ;
extern JC_CONST_INT8_P gpcAttrShape ;
extern JC_CONST_INT8_P gpcAttrShortcuticon ;
extern JC_CONST_INT8_P gpcAttrSize ;
extern JC_CONST_INT8_P gpcAttrSlide ;
extern JC_CONST_INT8_P gpcAttrSpan ;
extern JC_CONST_INT8_P gpcAttrSrc ;
extern JC_CONST_INT8_P gpcAttrStandby ;
extern JC_CONST_INT8_P gpcAttrStart ;
extern JC_CONST_INT8_P gpcAttrStyle ;
extern JC_CONST_INT8_P gpcAttrStylesheet ;

extern JC_CONST_INT8_P gpcAttrTarget ;
extern JC_CONST_INT8_P gpcAttrText ;
extern JC_CONST_INT8_P gpcAttrTitle ;
extern JC_CONST_INT8_P gpcAttrTrue ;
extern JC_CONST_INT8_P gpcAttrType ;

extern JC_CONST_INT8_P gpcAttrUnesc ;
extern JC_CONST_INT8_P gpcAttrUp ;
extern JC_CONST_INT8_P gpcAttrUsemap ;
extern JC_CONST_INT8_P gpcAttrUtn ;

extern JC_CONST_INT8_P gpcAttrValign ;
extern JC_CONST_INT8_P gpcAttrValue ;
extern JC_CONST_INT8_P gpcAttrVlink ;
extern JC_CONST_INT8_P gpcAttrVspace ;

extern JC_CONST_INT8_P gpcAttrWidth ;
extern JC_CONST_INT8_P gpcAttrWmlColumns ;
extern JC_CONST_INT8_P gpcAttrWmlEmptyok ;
extern JC_CONST_INT8_P gpcAttrWmlFormat ;
extern JC_CONST_INT8_P gpcAttrWmlIname ;
extern JC_CONST_INT8_P gpcAttrWmlIvalue ;
extern JC_CONST_INT8_P gpcAttrWmlLocalsrc ;
extern JC_CONST_INT8_P gpcAttrWmlName ;
extern JC_CONST_INT8_P gpcAttrWmlNewcontext ;
extern JC_CONST_INT8_P gpcAttrWmlOnenterbackward ;
extern JC_CONST_INT8_P gpcAttrWmlOnenterforward ;
extern JC_CONST_INT8_P gpcAttrWmlOnpick ;
extern JC_CONST_INT8_P gpcAttrWmlUsexmlfragments ;
extern JC_CONST_INT8_P gpcAttrWmlValue ;

extern JC_CONST_INT8_P gpcAttrXData ;
extern JC_CONST_INT8_P gpcAttrXFired ;
extern JC_CONST_INT8_P gpcAttrXRequest ;
extern JC_CONST_INT8_P gpcAttrXmlLang ;
extern JC_CONST_INT8_P gpcAttrXmlSpace ;

extern JC_CONST_INT8_P gpcAttrYes ;

#endif





