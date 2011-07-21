/***************************************************************************
 * $Id: ddl.h,v 1.19 2007/06/28 08:59:28 shreyasdevhtmlbrow Exp $
 * $Revision: 1.19 $
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
 * $Log: ddl.h,v $
 * Revision 1.19  2007/06/28 08:59:28  shreyasdevhtmlbrow
 * Added changes to handle RSS content
 *
 * Revision 1.18  2007/05/04 13:37:58  kumardevhtmlbrow
 * Re-ordering of the header file as ddldefines.h refer some of the macro's in ddlplatform.h
 *
 * Revision 1.17  2007/01/10 12:00:48  kumardevhtmlbrow
 * Script Integration
 *
 * Revision 1.16  2005/09/12 12:17:43  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.15  2005/09/06 16:16:33  kumardevhtmlbrow
 * no message
 *
 * Revision 1.14  2005/08/25 04:58:22  kumardevhtmlbrow
 * no message
 *
 * Revision 1.13  2005/08/23 16:34:37  kumardevhtmlbrow
 * no message
 *
 * Revision 1.12  2005/08/09 12:18:43  kavithadevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.11  2005/08/03 12:35:04  kumardevhtmlbrow
 * no message
 *
 * Revision 1.10  2005/08/02 12:06:44  kumardevhtmlbrow
 * no message
 *
 * Revision 1.9  2005/07/30 11:06:55  kavithadevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.8  2005/06/28 15:30:21  kumardevhtmlbrow
 * no message
 *
 * Revision 1.7  2005/06/24 13:36:43  kumardevhtmlbrow
 * no message
 *
 * Revision 1.6  2005/05/18 15:36:39  kumardevhtmlbrow
 * Update
 *
 * Revision 1.5  2005/04/19 12:25:08  kumardevhtmlbrow
 * no message
 *
 * Revision 1.4  2005/04/06 12:53:20  kavithadevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/04/05 13:07:24  kavithadevhtmlbrow
 * Updating for XML Parser
 *
 * Revision 1.2  2005/03/22 10:59:41  kumardevhtmlbrow
 * Update the license agreement
 *
 *
 ***************************************************************************/

#ifndef _DDL_H
#define _DDL_H

#include <ddlplatform.h>
#include <ddldefines.h>
#include <ddlbase.h>
#include <ddlutils.h>

#define COMMON_APPLICATION 				0x010000
#define BROWSER_APPLICATION 			0x020000
#define MMS_APPLICATION 	     		0x030000
#define IM_APPLICATION	     			0x040000
#define DM_APPLICATION	     			0x050000
#define EMAIL_APPLICATION     			0x060000
#define EMS_APPLICATION        			0x070000
#define PROVISIONING_APPLICATION		0x080000
#define TRANSPORT_APPLICATION			0x090000
#define ABSTRACT_APPLICATION			0x0A0000
#define DOWNLOAD_CLIENT_APPLICATION		0x0B0000
#define SCRIPT_ENGINE_APPLICATION		0x0C0000
#define RSS_CLIENT_APPLICATION			0x0D0000

#endif


