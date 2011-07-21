/****************************************************************
 * $Id: jdi_wspencoder.h,v 1.2 2006/05/02 11:19:21 kumardevjtbase Exp $
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
 * $Log: jdi_wspencoder.h,v $
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.4  2006/03/04 05:48:35  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.2  2005/08/23 08:09:41  kumardevhtmlbrow
 * Changed the wsp utils added WSP_ENCODER_ENABLE & WSP_DECODER_ENABLE switch
 * Added the additonal parameter for decoding the parameter as part of the header value.
 *
 * Revision 1.1  2005/08/16 13:07:18  kumardevhtmlbrow
 * Initial Version
 *
 *
 *  
 *****************************************************************/
#ifndef _JDI_WSP_ENCODER_H
#define _JDI_WSP_ENCODER_H

#ifdef WSP_ENCODER_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

JC_RETCODE jdi_WSPEncVarint (JC_UINT32	uiValue, 
							 JC_UINT32	*puiSize,	
							 JC_UINT8	*pucBuffer) ;

#ifdef __cplusplus
}
#endif

#endif

#endif

/* End of File */


