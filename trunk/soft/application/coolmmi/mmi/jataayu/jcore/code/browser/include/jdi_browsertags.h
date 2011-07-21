/***************************************************************************
 * $Id: jdi_browsertags.h,v 1.6 2008/05/27 14:33:15 kavithadevhtmlbrow Exp $
 * $Revision: 1.6 $
 * $Date: 2008/05/27 14:33:15 $
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
 * $Log: jdi_browsertags.h,v $
 * Revision 1.6  2008/05/27 14:33:15  kavithadevhtmlbrow
 * Marquee Tag added
 *
 * Revision 1.5  2007/05/22 13:51:22  sriramdevhtmlbrow
 * Removed macros for tag names and added constant global strings
 *
 * Revision 1.4  2007/05/11 13:34:36  sriramdevhtmlbrow
 * Removed the tag "quote" which is not a valid tag
 *
 * Revision 1.3  2007/05/11 13:17:06  sriramdevhtmlbrow
 * Added new tag
 *
 * Revision 1.2  2007/05/11 10:55:02  kavithadevhtmlbrow
 * new tags and attribute added
 *
 * Revision 1.1  2007/05/10 05:27:13  kavithadevhtmlbrow
 * Browser Tags extern variable
 *
 *
 ***************************************************************************/

/**
 * @file jdi_tags.h
 * @ingroup Browser
 * @brief Defines the Tags
 */


#ifndef _DE_TAGS_H
#define _DE_TAGS_H


/* Extern Global Tag Names */

extern JC_CONST_INT8_P gpcTagA ;
extern JC_CONST_INT8_P gpcTagAbbr ;
extern JC_CONST_INT8_P gpcTagAccess ;
extern JC_CONST_INT8_P gpcTagAcronym ;
extern JC_CONST_INT8_P gpcTagAddress ;
extern JC_CONST_INT8_P gpcTagAnchor ;
extern JC_CONST_INT8_P gpcTagApplet ;
extern JC_CONST_INT8_P gpcTagArea ;

extern JC_CONST_INT8_P gpcTagB ;
extern JC_CONST_INT8_P gpcTagBase ;
extern JC_CONST_INT8_P gpcTagBasefont ;
extern JC_CONST_INT8_P gpcTagBdo ;
extern JC_CONST_INT8_P gpcTagBgsound ;
extern JC_CONST_INT8_P gpcTagBig ;
extern JC_CONST_INT8_P gpcTagBlink ;
extern JC_CONST_INT8_P gpcTagBlockquote ;
extern JC_CONST_INT8_P gpcTagBody ;
extern JC_CONST_INT8_P gpcTagBr ;
extern JC_CONST_INT8_P gpcTagButton ;

extern JC_CONST_INT8_P gpcTagCaption ;
extern JC_CONST_INT8_P gpcTagCard ;
extern JC_CONST_INT8_P gpcTagCenter ;
extern JC_CONST_INT8_P gpcTagCite ;
extern JC_CONST_INT8_P gpcTagCode ;
extern JC_CONST_INT8_P gpcTagCol ;
extern JC_CONST_INT8_P gpcTagColgroup ;

extern JC_CONST_INT8_P gpcTagDd ;
extern JC_CONST_INT8_P gpcTagDel ;
extern JC_CONST_INT8_P gpcTagDfn ;
extern JC_CONST_INT8_P gpcTagDir ;
extern JC_CONST_INT8_P gpcTagDiv ;
extern JC_CONST_INT8_P gpcTagDl ;
extern JC_CONST_INT8_P gpcTagDo ;
extern JC_CONST_INT8_P gpcTagDt ;

extern JC_CONST_INT8_P gpcTagEm ;
extern JC_CONST_INT8_P gpcTagEmbed ;

extern JC_CONST_INT8_P gpcTagFieldset ;
extern JC_CONST_INT8_P gpcTagFont ;
extern JC_CONST_INT8_P gpcTagForm ;
extern JC_CONST_INT8_P gpcTagFrame ;
extern JC_CONST_INT8_P gpcTagFrameset ;

extern JC_CONST_INT8_P gpcTagGo ;

extern JC_CONST_INT8_P gpcTagH1 ;
extern JC_CONST_INT8_P gpcTagH2 ;
extern JC_CONST_INT8_P gpcTagH3 ;
extern JC_CONST_INT8_P gpcTagH4 ;
extern JC_CONST_INT8_P gpcTagH5 ;
extern JC_CONST_INT8_P gpcTagH6 ;
extern JC_CONST_INT8_P gpcTagHead ;
extern JC_CONST_INT8_P gpcTagHr ;
extern JC_CONST_INT8_P gpcTagHtml ;

extern JC_CONST_INT8_P gpcTagI ;
extern JC_CONST_INT8_P gpcTagIframe ;
extern JC_CONST_INT8_P gpcTagImage ;
extern JC_CONST_INT8_P gpcTagImg ;
extern JC_CONST_INT8_P gpcTagInput ;
extern JC_CONST_INT8_P gpcTagIns ;
extern JC_CONST_INT8_P gpcTagIsindex ;

extern JC_CONST_INT8_P gpcTagKbd ;

extern JC_CONST_INT8_P gpcTagLabel ;
extern JC_CONST_INT8_P gpcTagLegend ;
extern JC_CONST_INT8_P gpcTagLi ;
extern JC_CONST_INT8_P gpcTagLink ;

extern JC_CONST_INT8_P gpcTagMap ;
extern JC_CONST_INT8_P gpcTagMarquee ;
extern JC_CONST_INT8_P gpcTagMenu ;
extern JC_CONST_INT8_P gpcTagMeta ;

extern JC_CONST_INT8_P gpcTagNoembed ;
extern JC_CONST_INT8_P gpcTagNoframes ;
extern JC_CONST_INT8_P gpcTagNoop ;
extern JC_CONST_INT8_P gpcTagNoscript ;

extern JC_CONST_INT8_P gpcTagObject ;
extern JC_CONST_INT8_P gpcTagOl ;
extern JC_CONST_INT8_P gpcTagOnevent ;
extern JC_CONST_INT8_P gpcTagOptgroup ;
extern JC_CONST_INT8_P gpcTagOption ;

extern JC_CONST_INT8_P gpcTagP ;
extern JC_CONST_INT8_P gpcTagParam ;
extern JC_CONST_INT8_P gpcTagPre ;
extern JC_CONST_INT8_P gpcTagPrev ;

extern JC_CONST_INT8_P gpcTagQ ;

extern JC_CONST_INT8_P gpcTagRefresh ;

extern JC_CONST_INT8_P gpcTagS ;
extern JC_CONST_INT8_P gpcTagSamp ;
extern JC_CONST_INT8_P gpcTagScript ;
extern JC_CONST_INT8_P gpcTagSelect ;
extern JC_CONST_INT8_P gpcTagSetvar ;
extern JC_CONST_INT8_P gpcTagSmall ;
extern JC_CONST_INT8_P gpcTagSpan ;
extern JC_CONST_INT8_P gpcTagStrike ;
extern JC_CONST_INT8_P gpcTagStrong ;
extern JC_CONST_INT8_P gpcTagStyle ;
extern JC_CONST_INT8_P gpcTagSub ;
extern JC_CONST_INT8_P gpcTagSup ;

extern JC_CONST_INT8_P gpcTagTinline ;
extern JC_CONST_INT8_P gpcTagTable ;
extern JC_CONST_INT8_P gpcTagTbody ;
extern JC_CONST_INT8_P gpcTagTd ;
extern JC_CONST_INT8_P gpcTagTemplate ;
extern JC_CONST_INT8_P gpcTagTextarea ;
extern JC_CONST_INT8_P gpcTagTfoot ;
extern JC_CONST_INT8_P gpcTagTh ;
extern JC_CONST_INT8_P gpcTagThead ;
extern JC_CONST_INT8_P gpcTagTimer ;
extern JC_CONST_INT8_P gpcTagTitle ;
extern JC_CONST_INT8_P gpcTagTr ;
extern JC_CONST_INT8_P gpcTagTt ;

extern JC_CONST_INT8_P gpcTagU ;
extern JC_CONST_INT8_P gpcTagUl ;

extern JC_CONST_INT8_P gpcTagVar ;

extern JC_CONST_INT8_P gpcTagWml ;
extern JC_CONST_INT8_P gpcTagWmlAccess ;
extern JC_CONST_INT8_P gpcTagWmlAnchor ;
extern JC_CONST_INT8_P gpcTagWmlCard ;
extern JC_CONST_INT8_P gpcTagWmlDo ;
extern JC_CONST_INT8_P gpcTagWmlGetvar ;
extern JC_CONST_INT8_P gpcTagWmlGo ;
extern JC_CONST_INT8_P gpcTagWmlNoop ;
extern JC_CONST_INT8_P gpcTagWmlOnevent ;
extern JC_CONST_INT8_P gpcTagWmlPrev ;
extern JC_CONST_INT8_P gpcTagWmlRefresh ;
extern JC_CONST_INT8_P gpcTagWmlSetvar ;
extern JC_CONST_INT8_P gpcTagWmlTimer ;

extern JC_CONST_INT8_P gpcTagXmlStylesheet ;


#endif  /** _DE_TAGS_H **/


