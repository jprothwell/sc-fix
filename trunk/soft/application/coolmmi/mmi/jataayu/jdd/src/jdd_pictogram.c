#define MODULE_NAME						"jdd"
#define FILE_NAME						"jdd_pictogram.c"
#ifdef JATAAYU_SUPPORT
/***************************************************************************
 * $Id: jdd_pictogram.c,v 1.10 2005/12/12 15:29:02 kumardevhtmlbrow Exp $
 * $Revision: 1.10 $
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
 * $Log: jdd_pictogram.c,v $
 * Revision 1.10  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.9  2005/09/23 08:30:34  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/09/23 08:11:58  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.7  2005/09/20 06:10:33  shajudevhtmlbrow
 * Resource leak avoided.
 *
 * Revision 1.6  2005/09/12 12:17:43  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.5  2005/08/25 14:44:56  shajudevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/07/21 06:34:02  shajudevhtmlbrow
 * GetPictogramSize function implemented and one crashing problem in radio box corrected
 *
 * Revision 1.3  2005/07/21 06:22:41  shajudevhtmlbrow
 * GetPictogramSize function implemented and one crashing problem in radio box corrected
 *
 * Revision 1.2  2005/07/20 12:52:35  shajudevhtmlbrow
 * Pictogram is added.
 *
 *  
 ***************************************************************************/


 
/**
 * @file	 jdd_pictogram.c
 * @ingroup	 jdd files
 * @brief	 implementation of pictogram for mmi module.
 *
 * This file contains implementation of pictogram. There are implementation of 
 * functions to check whether pictogram is supported  and creating a pictogram.
 */



#include <ddl.h>

#ifdef JDD_LOG_ENABLED
#define __MODULE_ID__					OTHER_MODULE
#else
#define __MODULE_ID__					0
#endif

#include <jcal.h>

JC_BOOLEAN jdd_MMIIsPictogram (JC_CHAR *pmURL)
{
	JC_BOOLEAN bisAvailable = E_FALSE ;
	
	
	return bisAvailable ;
}

JC_RETCODE jdd_MMIGetPictogramSize (JC_UINT32  uiWindowID,
									JC_CHAR		*pmURI,
									JC_UINT32	*puiWidth,
									JC_UINT32	*puiHeight) 
{
	
	JC_RETCODE rCode = JC_OK ;

	
	return rCode ;
}

#endif
