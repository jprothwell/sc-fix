/***************************************************************************
 * $Id: ddlplatform.h,v 1.8 2007/09/18 08:58:24 kavithadevhtmlbrow Exp $
 * $Revision: 1.8 $
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
 * $Log: ddlplatform.h,v $
 * Revision 1.8  2007/09/18 08:58:24  kavithadevhtmlbrow
 * Updated Unicode defines
 *
 * Revision 1.7  2007/01/10 12:00:48  kumardevhtmlbrow
 * Script Integration
 *
 * Revision 1.6  2006/12/28 12:01:51  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.5  2005/09/12 12:17:43  kumardevhtmlbrow
 * Given the additional new line at the end ....
 *
 * Revision 1.4  2005/06/17 08:17:36  kumardevhtmlbrow
 * *** empty log message ***
 *
 * Revision 1.3  2005/03/22 10:59:41  kumardevhtmlbrow
 * Update the license agreement
 *
 *
 ***************************************************************************/

#ifndef _DDLPLATFORM_H
#define _DDLPLATFORM_H

#define INTEL

#include "sul.h"

#define HAVE_UNICODE

#ifdef HAVE_UNICODE
#ifndef _UNICODE
	#define _UNICODE
#endif
#ifndef UNICODE
	#define UNICODE
#endif
#endif

#if 0
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <float.h>
#endif

#include <stdlib.h>
#include <string.h>

extern int	strcasecmp(const char *, const char *);
UINT8* itoa( INT32 value, UINT8* string, UINT32 radix );

#define stricmp strcasecmp
//#include <wchar.h>
#ifdef MMI_ON_HARDWARE_P
#include <cs_types.h>
#endif


#include <unicodedcl.h>
#include <stdio.h>
#include <ctype.h>
#include "debuginitdef.h"

/*
#include "pixcomfontengine.h"
#include "gui_data_types.h"
#include "gui.h" */
#ifdef __cplusplus
extern "C"
{
#endif

#include "mmi_trace.h"

#ifdef __cplusplus
}
#endif

#define DISABLE_ASSERT

#ifdef DISABLE_ASSERT
// To automatically disable assert, uncomment this.
//#define DBG_NO_ASSERT
#endif

// For DBG_ASSERT
#ifndef MMI_ON_WIN32
#include "dbg.h"
#endif

#ifdef DISABLE_ASSERT
#define jdd_AssertOnFail(a,b) 
#else
#define jdd_AssertOnFail(a,b) DBG_ASSERT(a,b)
#endif



#endif


