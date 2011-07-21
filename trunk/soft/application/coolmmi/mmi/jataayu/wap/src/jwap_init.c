 /***************************************************************************
 *
 * File Name : jwap_init.c
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
 * @file		jwap_init.c
 * @ingroup	jWAP
 * @brief	
 *
 **/

#define MODULE_NAME "JWAP"
#define FILE_NAME "jwap_init.c"

#ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jwap_inc.h"
#include "jwap_mmi_cb.h"


 /***************************************************************************
 * Macros
 **************************************************************************/
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
extern	void jddmem_EnableMemLeakFinder (void);
JC_BOOLEAN g_bIsWapAPPActive;
extern void init_attach_status(void);


void initJWap ()
{
	mmi_trace (1, "JDD_LOG: initJWap ()") ;	
	#ifdef ENABLE_MEM_LEAK_FINDER
	jddmem_EnableMemLeakFinder ();
	#endif
	jWap_init_context () ;
	SetHiliteHandler(WAP_MAIN_SCREEN_MENUID, jWap_highlight_handler);
	 init_attach_status();

}

extern U8 r2MMIFlagCached;
extern BOOL r2lMMIFlag  ;

void jWap_SetWapAppStatus(JC_BOOLEAN bIsActive)
{
	g_bIsWapAPPActive = bIsActive;
	#if 1  //for Arabic displaying 
    if(g_bIsWapAPPActive )
    {
        r2MMIFlagCached = r2lMMIFlag;
        r2lMMIFlag = FALSE;
    }
    else
    {
        if(r2MMIFlagCached!=0xff)
        {
            r2lMMIFlag = r2MMIFlagCached;
            r2MMIFlagCached =0xff;
        }
    }
	#endif
}

// this interface is to know status wether  WAP application is running in the foreground or not
//  note that SIM selction screen in case of "always ask " will not be considered as part of WAP
JC_BOOLEAN jWap_GetWapAppStatus()
{
	return g_bIsWapAPPActive;
}

#endif //__JATAAYU_APP__



