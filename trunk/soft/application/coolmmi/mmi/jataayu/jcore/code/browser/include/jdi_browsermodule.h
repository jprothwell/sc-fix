/***************************************************************************
 * $Id: jdi_browsermodule.h,v 1.12 2007/02/20 13:03:07 babithapkdevhtmlbrow Exp $
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
 * $Log: jdi_browsermodule.h,v $
 * Revision 1.12  2007/02/20 13:03:07  babithapkdevhtmlbrow
 * Added BOM_MODULE
 *
 * Revision 1.11  2006/04/18 11:36:47  kavithadevhtmlbrow
 * Bidi udpated
 *
 * Revision 1.10  2006/01/24 16:13:34  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.9  2005/12/12 15:29:01  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.8  2005/09/12 12:20:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/08/26 12:28:46  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/08/11 14:00:16  kumardevhtmlbrow
 * no message
 *
 * Revision 1.5  2005/07/18 14:29:25  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/06/23 09:49:45  kumardevhtmlbrow
 * no message
 *
 * Revision 1.3  2005/06/17 08:17:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.2  2005/05/12 06:51:50  kavithadevhtmlbrow
 * Added DE Module
 *
 * Revision 1.1  2005/05/12 05:28:38  kumardevhtmlbrow
 * Cache interface and browser module
 *
 *  
 ***************************************************************************/

/**
 * @mainpage
 *
 * @version 0.9(Draft) 
 * Interface Description
 *
 * @section Introduction
 * The jBrowser5 Documentation gives a detailed description of the interface that has to be used
 * by an application. The documentation clearly describes the usage of each function, the 
 * significance of each variable, usage of the data structures and enumerations etc. The
 * document will ease the complexity faced while interfacing the application and the Browser.
 *
 * @section Conventions Naming Conventions
 * Following Specifies the naming convention used in the jBrowser5 code: 
 * @li Names of all the functions, exposed to the application, starts with "jdi_". 
 * @li Names of all the structures starts with "ST_". 
 * @li Names of all the enumerations and its members starts with "E_". 
 * @li Names of all the unions starts with "U_". 
 *
 * @section Startup Startup Code
 * @li The application has to initialize all the JDD modules (Net, timer, MMI etc) 
 *	   that are going to be used by the browser.
 * @li Then the application needs to start the transport task. This will ensure that the queue
 *	   for the transport is created 
 * @li Then the queue for the application has to be created. 
 *
 * @section Flow Basic Flow
 * This section describes the basic flow that has to be used in order to fetch a page. 
 * @li The browser @link jdi_browserengine.h.jdi_BEInitialize initialize function @endlink has to be called before 
 *	   calling any browser related functions. 
 * @li Then the application has to @link jdi_browserengine.h.jdi_BESetProperties set the browser properties @endlink
 * @li The application has to specify the @link jdi_browserengine.h.jdi_BESetDataConn data connection @endlink
 *	   and the @link jdi_browserengine.h.jdi_BESetProfile profile @endlink information before giving any fetch request. 
 * @li The application can then give the @link jdi_browserengine.h.jdi_BEFetchRequest fetch @endlink
 *	   request to the browser. 
 * @li Finally application has to @link jdi_browserengine.h.jdi_BEDeinitialize deinitialize @endlink the browser. 
 * 
 * @section Event Event Handling
 * The browser and the transport communicate in the form of events. This section briefly 
 * describes the way this communication takes place. 
 * @li As mentioned in the startup section queues are created for the transport and the 
 *	   application. 
 * @li For a fetch request the browser sends events to the transport queue 
 * @li The transport task while receive the event details from the transport queue and it 
 *	   processes the event data. 
 * @li The response event from the transport will be sent to the application queue. 
 * @li The application has to make sure that it receives the transport event from the queue 
 *	   and send the event data to the browser using the function jdi_BESetEvent. 
 *
 * @defgroup BE Browser Engine
 * @defgroup ARR Adaptive Rendering Rules
 * @defgroup History History 
 * @defgroup IV Input Validation
 
 * @defgroup Push Push
 */

#ifndef _JDI_BROWSER_MODULE_H
#define _JDI_BROWSER_MODULE_H

/** @cond */

#define CACHE_MODULE						TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0100)
#define DE_MODULE							TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0200)
#define NAVIGATION_MODULE					TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0300)
#define HISTORY_MODULE						TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0400)
#define PUSH_MODULE                         TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0500)
#define BE_MODULE							TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0600)
#define COOKIE_MODULE						TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0700)
#define BOOKMARK_MODULE						TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0800)
#define WMLSCRIPT_MODULE					TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0900)
#define INPUTVALIDATION_MODULE				TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0A00)
#define WMLVARIABLE_MODULE					TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0B00)
#define BOM_MODULE					        TERMINAL_MODULE_CODE (BROWSER_APPLICATION, 0x0C00)

#ifndef BIDI_ENABLED
#undef DE_BIDI_ENABLE
#endif

/** @endcond */

#endif

/* End of File */



