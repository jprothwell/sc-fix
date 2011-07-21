#define MODULE_NAME "Jmms"
#define FILE_NAME "jmms_template.c"
 /***************************************************************************
 *
 * File Name : jmms_template.c
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
 * @file		jmms_template.c
 * @ingroup		jMMS
 * @brief		Describes the MMS template APIs.
 *
 **/
 
 #ifdef JATAAYU_SUPPORT
 /***************************************************************************
 * System Include Files
 **************************************************************************/
 
 
 /***************************************************************************
 * User Include Files
 **************************************************************************/

#include "jmms_inc.h"
#include "jmms_storedlg.h"
#include "jmms_template.h"
#include "jmms_composedlg.h"
#include "jmms_composeutils.h"
#include "jmms_addrdlg.h"
#include "messagesresourcedata.h"

 /***************************************************************************
 * Macros
 **************************************************************************/
 /***************************************************************************
 * Global variables declarations
 **************************************************************************/

const template_struct g_str_templates[] = 
{
    {STR_ID_MMS_TEMPLATE1, 0},      
    {STR_ID_MMS_TEMPLATE2, 0},      
    {STR_ID_MMS_TEMPLATE3, 0},      
    {STR_ID_MMS_TEMPLATE4, 0},     
    {STR_ID_MMS_TEMPLATE5, 0},      
    {STR_ID_MMS_TEMPLATE6, 0},     
    {STR_ID_MMS_TEMPLATE7, 0},      
    {STR_ID_MMS_TEMPLATE8, 0},      
    {STR_ID_MMS_TEMPLATE9, 0},      
    {STR_ID_MMS_TEMPLATE10, 0},      
};

 /***************************************************************************
 * Local Function Prototypes
 ***************************************************************************/

 /***************************************************************************
 * All Local Function Definitions
 ***************************************************************************/

 /***************************************************************************
 * All Global Function Definitions 
**************************************************************************/
JC_CHAR TEST1[10] = {'H','E','L','L','O'},  TEST2[10] = {'C','E','L','L','O'};

void Select_hltd_template()
{
	U16 hltd_index=0;
	JC_CHAR *pStrItem;
	JC_INT32 len = 0;
	JC_CHAR *psInputValue = JC_NULL ;
	JC_UINT32 uiStrLen = 0 ;
	JC_RETCODE rCode = JC_OK;

	jc_tcsncat(TEST1,TEST2,0) ;
	
	hltd_index =(U8) GetHighlightedItem();
		
	len = jc_wstrlen(GetString(g_str_templates[hltd_index].strId));
	pStrItem = (JC_CHAR*) jdd_MemAlloc((len + 1), sizeof(JC_CHAR));
	jc_wstrcpy(pStrItem, (UINT16*)GetString(g_str_templates[hltd_index].strId));
	
	//mmi_trace(1,"Select_hltd_template [%s]",GetString(g_str_templates[hltd_index].strId)) ;
	//mmi_trace(1,"Select_hltd_template [%s]",pStrItem) ;
	//mmi_trace(1,"Select_hltd_template [%d]",len) ;
	
	if(g_pstJMmsAppInfo->pActiveTextBuffer)
	{
		mmi_trace(1, " length in if  [%d]", jc_tcslen(g_pstJMmsAppInfo->pcInputBuff));
		jc_tcsncpy((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff,  
		 				(JC_CHAR*)g_pstJMmsAppInfo->pActiveTextBuffer,
		 				jMMS_MAX_INPUT_SIZE) ;				
	}
	mmi_trace(1, " length in afer if  [%d]", jc_tcslen(g_pstJMmsAppInfo->pcInputBuff));	
		
	uiStrLen = jMMS_MAX_INPUT_SIZE - jc_tcslen(g_pstJMmsAppInfo->pcInputBuff);
	
	jc_tcsncat((JC_CHAR*)g_pstJMmsAppInfo->pcInputBuff,(JC_CHAR*)pStrItem,\
				uiStrLen) ;
	
	mmi_trace(1, " length in sdgsd if  [%d], [%d]", jc_tcslen(g_pstJMmsAppInfo->pcInputBuff),uiStrLen);
	jMMS_MMIGetInputBoxValue (&psInputValue); 

	if(psInputValue)
	{
	   rCode =jmms_AddTextMediaToCurrSlide(psInputValue);
	}
	//raj change mem leak
	jdd_MemFree(pStrItem);
      if(rCode == JC_OK)
        {
	DeleteNHistory(1);	
	GoBackHistory();
        }
	
}

pBOOL get_templates_item_data(S32 item_index, UI_string_type str_buff_p, PU8 *img_buff_p, U8 str_img_mask)
{    
    if ((item_index < 0) || (item_index > 10))
    {
        return FALSE;
    }

    if (jc_wstrlen(GetString(g_str_templates[item_index].strId)))
    {
        jc_wstrcpy(str_buff_p, (UINT16*)GetString(g_str_templates[item_index].strId));
    }
    else
    {
        //jc_wstrcpy((S8*) str_buff_p, (S8*) GetString(STR_ID_NO_SUBJECT));
    }

    *img_buff_p = (PU8) GetImage( (U16)(IMG_GLOBAL_L1 + item_index) );

    return E_TRUE;
}

void	jMms_template_entry_handler ()
{
	U8* guiBuffer; 

	EntryNewScreen (SCR_ID_MMS_TEMPLATE, NULL, jMms_template_entry_handler, NULL) ;
	guiBuffer = GetCurrGuiBuffer (SCR_ID_MMS_TEMPLATE) ;	
	//RegisterHighlightHandler (ExecuteCurrHiliteHandler) ;
	ShowCategory184Screen(STR_ID_MMS_TEXT, IMG_SMS_ENTRY_SCRN_CAPTION,
				        STR_GLOBAL_OK, IMG_GLOBAL_OK,
				        STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
				        10,
				        get_templates_item_data,
				        0, 0,
				        guiBuffer);
	
	SetLeftSoftkeyFunction (Select_hltd_template, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	//jmms_MmsEntryRegisterEndKeyHandler();
	//calling the same end key handle as adresssubmenu
	//SetKeyHandler(jmms_ComposeEndKeyHandler, KEY_END, KEY_EVENT_DOWN) ;
}

#endif //__JATAAYU_APP__

