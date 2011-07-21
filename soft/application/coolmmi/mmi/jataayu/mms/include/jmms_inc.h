 /***************************************************************************
 *
 * File Name : jmms_inc.h
 *
 * IMPORTANT NOTICE
 *
 * Please note that any and all title and/or intellectual property rights
 * in and to this Software or any part of this (including without limitation
 * any images, photographs, animations, video, audio, music, text and/or
 * "applets," incorporated into the Software), herein mentioned to as
 * "Software", the accompanying printed materials, and any copies of the
 * Software, are owned by Jataayu Software Ltd., Bangalore ("Jataayu")
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
 * THIS SOFTWARE IS PROVIDED TO YOU "AS IS" WITHOUT WARRANTY OF ANY KIND
 * AND ANY AND ALL REPRESENTATION AND WARRANTIES, EITHER EXPRESS
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
 ***************************************************************************/

 /**
 * @file		jmms_inc.h
 * @ingroup		jMMS
 * @brief		Describes the MMS Init related API's.
 *
 **/
#ifndef __JMMS_INCLUDE
#define __JMMS_INCLUDE

 /***************************************************************************
 * System Include Files
 **************************************************************************/
#ifdef MMI_ON_HARDWARE_P
#include "cs_types.h"
#endif

#include "mmi_trace.h"
#include "historygprot.h"
#include "wgui_categories.h"
#include "globaldefs.h"
#include "eventsgprot.h"
#include "wgui_status_icons.h"
#include "commonscreens.h"

#include "ddl.h"
#include "jcal.h"
#include "messagingdatatypes.h"
#include "messagingapi.h"
#include "jdi_cutils.h"
#include "jdd_messagingconfig.h"


#include "jmms_init.h"
#include "jmms_str_id.h"
#include "jwap_str_id.h"
#include "jdi_communicator.h"
#include "jmms_communicator.h"
#include "jmms_utils.h"
#include "jmms_interface.h"
#include "jmms_mmi_cb.h"
#include "jmms_view.h"
#include "jmms_storeutils.h"

 /***************************************************************************
 * User Include Files
 **************************************************************************/
 /***************************************************************************
 * Macros
 **************************************************************************/
#define JRET(X)	(X * -1)
#define JMMS_NEW_LINE	"\r\n"
#define JMMS_DASH_LINE	"--------\n"

/***************************************************************************
* Global variables declarations
**************************************************************************/
/***************************************************************************
* Local Function Prototypes
***************************************************************************/

/***************************************************************************
* All Local Function Definitions
***************************************************************************/

/***************************************************************************
* All Global Function Definitions
**************************************************************************/

#endif // __JMMS_INCLUDE

