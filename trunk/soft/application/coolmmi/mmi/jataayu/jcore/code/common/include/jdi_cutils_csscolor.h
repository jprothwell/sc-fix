/***************************************************************************
 * $Id: jdi_cutils_csscolor.h,v 1.2 2009/03/16 10:50:39 browserdevjtbase Exp $
 * $Revision: 1.2 $
 * $Date: 2009/03/16 10:50:39 $
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
 * $Log: jdi_cutils_csscolor.h,v $
 * Revision 1.2  2009/03/16 10:50:39  browserdevjtbase
 * Core Updates
 *
 * Revision 1.2  2009/03/02 09:50:21  kavithadevhtmlbrow
 * Default Color (x-default-color : -2)
 *
 * Revision 1.1  2008/08/26 07:29:19  kavithadevhtmlbrow
 * Move CSS Color Handling from Browser to Common
 *
 *
 *  
 ***************************************************************************/

#ifndef _JDI_CUTILS_CSSCOLOR_H
#define _JDI_CUTILS_CSSCOLOR_H

#ifdef __cplusplus
extern "C" {
#endif

#define CSS_DEF_COLOR_VALUE			"x-default-color"

/**
 * @brief This function validates the color value for the given color name by 
 *		  checking in the color groups
 * @param[in]  pcColorString  color name
 * @retval E_TRUE  Valid property value 
 * @retval E_FALSE Invalid property-value  
 */
JC_RETCODE jdi_CUtilsValidateKeywordColorValue (JC_INT8		*pcColorString) ;

/**
 * @brief This function provides the css color value based on the strint passed
 * @param[in]  bStdMode		  Flag that specifies mode of the page
 * @param[in]  iDefaultValue  Default Color value for invalid values
 * @param[in]  pcColorString  Reference to the css color string
 * @return required color value 
 */
JC_INT32 jdi_CUtilsGetColorValue (JC_BOOLEAN		bStdMode,
								  JC_INT32			iDefaultValue,
								  JC_INT8			*pcColorString) ;

#ifdef __cplusplus
}
#endif

#endif

/* End of File */


