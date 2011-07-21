/***************************************************************************
 * $Id: jcal.h,v 1.5 2007/11/20 14:45:23 browserdevjtbase Exp $
 * $Revision: 1.5 $
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
 * $Log: jcal.h,v $
 * Revision 1.5  2007/11/20 14:45:23  browserdevjtbase
 * Updated.
 *
 * Revision 1.4  2006/12/28 09:47:59  browserdevjtbase
 * *** empty log message ***
 *
 * Revision 1.3  2006/11/16 11:32:26  browserdevjtbase
 * Update
 *
 * Revision 1.2  2006/05/02 11:19:21  kumardevjtbase
 * *** empty log message ***
 *
 * Revision 1.7  2006/03/04 11:50:34  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.6  2006/01/25 07:22:16  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.4  2005/12/12 15:29:02  kumardevhtmlbrow
 * Providing the logging framework for all the modules with the new JDD log abstraction
 *
 * Revision 1.3  2005/09/12 12:17:44  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.2  2005/03/24 10:57:50  kumardevhtmlbrow
 * Changed as per the coding standard
 *
 *  
 ***************************************************************************/

/** @mainpage
 * @version 2.5
 * @section JCF Jataayu Client Framework (JCF)
 * 
 * JCF provides the highly portable and modular framework for the terminal side products. 
 * All the terminal products has been developed based on the JCF. JCF has been architecture with two extreme and 
 * conflicting principles such as:
 * @li Code should be completely portable
 * @li Product should look and feel completely like a native application
 * 
 * @subsection JCFDesign Design Goals
 * @li <b>Portability</b>: The product should be portable on a range of handheld and embedded operating systems. 
 * While there would necessarily be use of operating system specific features to obtain the best solution possible, 
 * these should be isolated sufficiently that these diverse platforms use substantially a single source base. 
 * @li <b>Modularity</b>: The product should lend itself to being reused in parts protocols, 
 * processors, etc in order to meet differing needs. 
 * @li <b>Compactness</b>: The product should be optimized for size while still retaining the earlier two attributes.
 * @li <b>Standards compliance</b>: The product should adhere closely to all applicable standards.
 * @li <b>Easily localizable</b>: The product should allow easy localizing into different natural languages. 
 * All UI resources should be managed external to the code and wherever necessary double-byte character representation should be used.
 * 
 * @subsection JCFArchitecture Typical Architecture
 * The general product architecture based on JCF
 * <img src="jcf.gif" >
 * @li DI App (Device Independent application) + Primary MMI, Protocols are device indepdent code, which shall be easily ported on any
 * platform without any change.
 * @li DD App (Device Dependent application) + Secondary MMI, DDL are device dependent code, which needs to be created/modified for
 * any new platform.
 * 
 * @subsection DataTypes Abstract Datatypes
 * The generic data types that are referred by the core component are described below:
 *
 * <table border="1">
 * <tr> <td> <b>JC_INT8</b> </td> <td> Refers to signed 8 bit data. Can be mapped to "char" datatype in Win32 platform..</td> </tr>
 * <tr> <td> <b>JC_UINT8</b> </td> <td> Refers to unsigned 8 bit data. Can be mapped to "unsigned char" datatype in Win32 platform..</td> </tr>
 * <tr> <td> <b>JC_INT16</b> </td> <td> Refers to signed 16 bit data. Can be mapped to "short" datatype in Win32 platform.. </td> </tr>
 * <tr> <td> <b>JC_UINT16</b> </td> <td> Refers to unsigned 16 bit data. Can be mapped to "unsigned short" datatype in Win32 platform.. </td> </tr>
 * <tr> <td> <b>JC_INT32</b> </td> <td> Refers to signed 32 bit data. Can be mapped to "int" datatype in Win32 platform.</td> </tr>
 * <tr> <td> <b>JC_UINT32</b> </td> <td> Refers to unsigned 32 bit data. Can be mapped to "unsigned int" datatype in Win32 platform. </td> </tr>
 * <tr> <td> <b>JC_INT64</b> </td> <td> Refers to signed 64 bit data. Can be mapped to "long" datatype in Win32 platform.</td> </tr>
 * <tr> <td> <b>JC_UINT64</b> </td> <td> Refers to unsigned 64 bit data. Can be mapped to "unsigned long" datatype in Win32 platform. </td> </tr>
 * <tr> <td> <b>JC_FLOAT</b> </td> <td> Refers to float datatype. Can be mapped to "float" datatype in Win32 platform. </td> </tr>
 * <tr> <td> <b>JC_DOUBLE</b> </td> <td> Refers to double datatype. Can be mapped to "double" datatype in Win32 platform. </td> </tr>
 * <tr> <td> <b>JC_BOOLEAN</b> </td> <td> Refers to the datatype where it holds only two values such as E_TRUE and E_FALSE. It can be defined as the enumeration also.</td> </tr>
 * </table>
 * <small> If the platform does not support 64 bit data type, it can be mapped to 32 bit datatype. </small>
 *
 * @subsection JDD Device Dependent Layer (JDD)
 * JDD layer is the abstraction layer defined in the JCF for the platform specific API calls,
 * which is used by the all the terminal products. 
 *
 * The JDD Layer can be further decomposed into the following components, each performing 
 * a specific task:
 * @li MMI
 * @li Memory
 * @li Networking
 * @li File System
 * @li Timer
 * @li Tasks
 * @li Queue
 * @li Semaphore
 * @li Data Connection
 * @li Mime Handler
 * @li Float 
 * @li Math
 *
 * @subsection JDI Device Independet Layer (JDI)
 * The JDI layer consists of the product core implementation, provided by Jataayu. 
 * This layer comprises of a set of well-defined interface APIs that the Secondary MMI will 
 * call to realize the functionality. 
 * In case of browser, the JDI Layer would typically include functions 
 * for fetching a page, handling redirections, parsing the received content, and displaying 
 * the page as per the layout information in the received content.
 * In case of MMS, the JDI Layer would include composing, preview and view of MMS message.
 *
 * @section MMI MMI Architecture
 * The display area that is available in the terminal device can be classified into two 
 * sections such as:
 * @li Primary MMI
 * @li Secondary MMI
 * <img src="mmi.jpg"> 
 * The terminal applications of Jataayu has been designed and developed with the two extreme principles such as:
 * @li Application core component should be completely portable across different platform with different capabilities.
 * @li Application should look and feel completely like a "native" application.
 * 
 * @subsection PMMI Primary MMI
 * Primary MMI contents are generally controlled by external content. Primary MMI 
 * owns and controls the display area reserved for showing the external contents. 
 * In case of Browser, it uses this area for displaying the page contents which may 
 * consists of text/label, images, input box, button, list box.
 * In case of MMS, it uses this area for message view, preview.
 *
 * @subsection SMMI Secondary MMI
 * Secondary MMI is the layer responsible for the User Interface, which is mainly 
 * controlled by device characteristics. This layer can be built specific to a platform 
 * needs. The Secondary MMI layer uses the APIs that are exposed by the primary MMI Layer.
 * 
 * The Secondary MMI can be developed using the 
 * environment specific tools if available (example in Windows the Secondary MMI can 
 * developed using the Resource Editor).
 *
 * @section nc Naming Convention
 * All the API calls shall start with jdd_<ModuleName> (Jataayu Device Dependent layer),
 * here the module name describes abstraction layer module such as Mem, MMI, Net, etc.
 * @Note Unless otherwise it is specified all the JDD API should return JC_OK as the 
 * return value for success, when the return code is JC_RETCODE.
 *
 * This document describes the Jataayu device dependent abstraction layer for all 
 * mobile terminal applications that are developed in Jataayu Software. 
 * This document describes the list of platform dependent calls that needs to be 
 * implemented in a specific platform for Jataayu mobile terminal applications to work. 
 *
 * This document shall be available to prospective and current clients under NDA to 
 * understand the device dependent abstraction layer of the Jataayu mobile terminal 
 * applications. 
 *
 * @defgroup Memory Memory Abstraction Layer
 * @defgroup MMI MMI Abstraction Layer
 * @defgroup File File Abstraction Layer
 * @defgroup Queue Queue Abstraction Layer
 * @defgroup Task Task Abstraction Layer
 * @defgroup Network Network Abstraction Layer
 * @defgroup Timer Timer Abstraction Layer
 * @defgroup Mime Mime Abstraction Layer
 * @defgroup Log Log Abstraction Layer
 * @defgroup DataConnection Data Connection Abstraction Layer
 * @defgroup MISC Miscellaneous Abstraction Layer
 * @defgroup Semaphore Semaphore Abstraction Layer
 * @defgroup TAPI Telephony Abstraction Layer 
 * @defgroup Float Floating point abstraction layer
 * @defgroup Math Mathematical abstraction layer
 * @defgroup DRM DRM Abstraction 
 * @defgroup general General Information 
 */

#ifndef JCAL_H
#define JCAL_H

/** @cond */

#include <jcerror.h>
#include <jcutils.h>
#include <jcevent.h>

#include <jdd_memapi.h>
#include <jdd_fileapi.h>
#include <jdd_mmidatatype.h>
#include <jdd_mmiapi.h>
#include <jdd_queueapi.h>
#include <jdd_taskapi.h>
#include <jdd_semapi.h>
#include <jdd_timerapi.h>
#include <jdd_miscapi.h>
#include <jdd_mimeapi.h>
#include <jdd_logapi.h>
#include <jdd_dataconnection.h>
#include <jdd_tapi.h>
#include <jdd_floatapi.h>
#include <jdd_mathapi.h>
#include <jdd_audioapi.h>


/** @endcond */

#endif

/* END OF FILE */




