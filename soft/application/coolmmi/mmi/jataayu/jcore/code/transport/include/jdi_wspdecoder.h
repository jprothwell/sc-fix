/****************************************************************
 * $Id: jdi_wspdecoder.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
 * $Revision: 1.2 $
 * $DateTime: $
 * 
 * IMPORTANT NOTICE
 * Please note that any and all title and/or intellectual property 
 * rights in and to this Software or any part of this	(including 
 * without limitation any images, photographs, animations,	video, 
 * audio, music, text and/or "applets," incorporated into	   the 
 * Software), herein mentioned to as "Software", the  accompanying 
 * printed materials, and any copies of the Software, are owned by 
 * Jataayu Software (P) Ltd., Bangalore ("Jataayu") or	 Jataayu's 
 * suppliers as the case may be. The Software is protected		by 
 * copyright, including without limitation by applicable copyright 
 * laws, international treaty provisions,	other	  intellectual 
 * property laws and applicable laws in the country in	which  the 
 * Software is being used. You shall not modify,adapt or translate 
 * the Software,without prior express written consent from Jataayu.
 * You shall not reverse engineer, decompile,	disassemble		or 
 * otherwise alter the Software,except and only to the extent that 
 * such activity is  expressly  permitted   by   applicable	   law 
 * notwithstanding this limitation. Unauthorized  reproduction  or 
 * redistribution of this program or any  portion of it may result 
 * in severe civil and criminal penalties and will  be  prosecuted 
 * to the maximum extent possible under the law.  Jataayu reserves 
 * all rights not expressly granted.
 *
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY 
 * KIND AND ANY AND ALL  REPRESENTATION  AND   WARRANTIES,   EITHER 
 * EXPRESS OR IMPLIED, INCLUDING  BUT  NOT  LIMITED  TO THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY ACCURACY OF INFORMATIONAL CONTENT, 
 * AND/OR FITNESS FOR A  PARTICULAR  PURPOSE  OR  USE,  TITLE    OR 
 * INFRINGEMENT ARE  EXPRESSLY  DISCLAIMED  TO  THE  FULLEST EXTENT 
 * PERMITTED BY LAW. YOU ASSUME THE ENTIRE RISK AS TO THE  ACCURACY 
 * AND THE USE OF THIS SOFTWARE.JATAAYU SHALL NOT BE LIABLE FOR ANY 
 * CONSEQUENTIAL,  INCIDENTAL,  INDIRECT,  EXEMPLARY,   SPECIAL  OR 
 * PUNITIVE DAMAGES INCLUDING  WITHOUT LIMITATION ANY LOSS OF DATA, 
 * OR; LOSS OF PROFIT,SAVINGS BUSINESS OR GOODWILL OR OTHER SIMILAR 
 * LOSS RESULTING FROM  OR OUT OF  THE USE OR INABILITY TO USE THIS 
 * SOFTWARE, EVEN IF JATAAYU HAS BEEN ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE, OR FOR ANY CLAIM BY ANY THIRD PARTY.
 ******************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: jdi_wspdecoder.h,v $
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.10  2006/04/28 07:36:22  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.9  2006/03/04 05:48:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/10/14 13:31:33  kumardevhtmlbrow
 * Updated the parsing function to get the content type
 *
 * Revision 1.7  2005/10/14 13:29:46  kumardevhtmlbrow
 * Updated for using internal parsing of wap sms message
 *
 * Revision 1.6  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.5  2005/08/25 04:58:22  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/08/23 08:09:41  kumardevhtmlbrow
 * Changed the wsp utils added WSP_ENCODER_ENABLE & WSP_DECODER_ENABLE switch
 * Added the additonal parameter for decoding the parameter as part of the header value.
 *
 * Revision 1.3  2005/08/20 10:35:37  kumardevhtmlbrow
 * 1. Updated QD_MOR and socket event update.
 *
 * Revision 1.2  2005/08/17 09:41:37  kumardevhtmlbrow
 * no message
 *
 * Revision 1.1  2005/08/16 13:07:18  kumardevhtmlbrow
 * Initial Version
 *
 *
 *  
 *****************************************************************/
#ifndef _JDI_WSP_DECODER_H
#define _JDI_WSP_DECODER_H

#ifdef WSP_DECODER_ENABLE

#include <jdi_mimeinfo.h>

#ifdef __cplusplus
extern "C" {
#endif

JC_RETCODE jdi_WSPDecVarint (const JC_UINT8		*pucBuffer,	
							 JC_UINT32 			uiSize, 
							 JC_UINT32 			*puiInteger, 
							 JC_UINT32			*puiCount) ;

JC_RETCODE jdi_WSPDecInitialize (JC_UINT8		*pucBuffer,	
								 JC_UINT32		uiLength, 
								 JC_BOOLEAN		bUseSameMemory,
								 JC_HANDLE		vMemHandle, 
								 JC_HANDLE 		*pvHandle) ;

void jdi_WSPDecDeInitialize (JC_HANDLE	vHandle) ;

JC_RETCODE jdi_WSPDecAllHeaders (JC_HANDLE			vHandle, 
								 JC_BOOLEAN			bParseParameters, 
								 ST_MIME_HEADERS	**ppstMimeHeaders) ;

JC_RETCODE jdi_WSPDecContentTypeHeader (JC_HANDLE		vMemHandle,
										JC_UINT8		*pucBuffer,
										JC_UINT32		uiLength,
										JC_INT8			**ppcContentType) ;

#ifdef __cplusplus
}
#endif

#endif

#endif

/* End of File */



