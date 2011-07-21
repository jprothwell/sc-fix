/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("COOLSAND SOFTWARE")
*  RECEIVED FROM COOLSAND AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. COOLSAND EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES COOLSAND PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE COOLSAND SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. COOLSAND SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY COOLSAND SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND COOLSAND'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE COOLSAND SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT COOLSAND'S OPTION, TO REVISE OR REPLACE THE COOLSAND SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  COOLSAND FOR SUCH COOLSAND SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 * Filename:
 * ---------
 *  History.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  
 *
 * Author:
 * -------
 *  
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/

/**************************************************************

	FILENAME	: History.c

  	PURPOSE		: Implementation of Main LCD history stack

 

 

	DATE		: Aug' 28, 2002

**************************************************************/

#define __NEWSIMULATOR 
#include "mmi_features.h"
#include "stdc.h"
#include "l4dr1.h"
#include "globalconstants.h"
#include "debuginitdef.h"
#include "mmi_data_types.h"
#include "frameworkstruct.h"
#include "custmenures.h"

#include "historygprot.h"
#include "historydef.h"
#include "historydcl.h"
#include "unicodexdcl.h"
#include "mmi_data_types.h"
#undef __NEWSIMULATOR 
#include "l4dr.h"
#include "gdi_include.h"//__mmi_ui_small_screen_support__
#include "wgui_categories_util.h"//__mmi_ui_small_screen_support__

#ifdef __DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__
#include "miscfunctions.h"
#endif
#if defined(__MMI_MAINLCD_220X176__) || defined(__PROJECT_GALLITE_C01__)
extern void cat222_title_text_stop_scrolling(void);
#endif
/* 
**   Extern Varibales 
*/
pBOOL IsBackHistory = FALSE;

/* 
**   Local Variables 
*/
static pBOOL CheckMainLCDHistoryBack=FALSE;
static historyCallback historyCBHandler[MAX_HISTORY];
static historyNode historyData[MAX_HISTORY];						/* array of history data */
static S16 currHistoryIndex  = -1;	
static pBOOL bRoundOffFlag = 0;
static U8 is_del_cb_found = MMI_FALSE;

/* 
**   Extern Function 
*/
extern void *OslMallocCHK (long nob, char* fileName, int lineNumber, ...);

/* 
**   Local Functions 
*/
static void ExecutePopHistory(void);
static U8 ClearDelScrnIDCallbackHandler(U16 ScrnID, HistoryDelCBPtr funcPtr);
static U8 SearchDelScrnIDCallbackHandler(U16 ScrnID, U16 *found_idx);
static U8 ExecTopScrnCallbackHandler(void);
static U8 ExecHistoryScrnCallBackHnadle(U16 HistoryIndex);
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
static S32 redrawing_old_screens=0; 
static S32 small_history_node=0;
 
static S32 check_for_small_history_node=0;
 
S32 is_dinit_called=0;
S32 g_count=0;
S32 is_draw_prev_screen_on_set_small_screen=0;
 
S32 no_small_screen=0;
 
#endif

#ifdef __MMI_UI_TAB_PANE__
S32 disable_history_save=0;
#endif

 
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
void set_redrawing_old_screens(void)
{
	redrawing_old_screens = 1;
}

void reset_redrawing_old_screens(void)
{
	redrawing_old_screens = 0;
}

S32 is_redrawing_old_screens()
{
	return redrawing_old_screens;
}
#endif

/**************************************************************

	FUNCTION NAME		: increment()

  	PURPOSE				: increment the global currHistoryIndex

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: S16 - Current history index

 

**************************************************************/



S16 increment()
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if( currHistoryIndex < MAX_HISTORY - 1)
	{
		++currHistoryIndex;
	}
	else
	{
		currHistoryIndex = MIN_HISTORY + 1;
		bRoundOffFlag = 1;
	}

	return currHistoryIndex;
}


extern U16 GetExitScrnID(void);

/**************************************************************

	FUNCTION NAME		: decrement()

  	PURPOSE				: decrement the global currHistoryIndex

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: S16 - current history index

 

**************************************************************/



static S8 decrement(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   S8 is_stop_delete = MMI_FALSE; //JL, if want to stop to run next decrement will return true.
   static U16 cb_history_idx = 0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   
   //found the callback history screen
   if (is_del_cb_found)
   {
   	// if topest screen is the wanted delete screen, then we don't callback it here.
   	if (GetExitScrnID()!=historyCBHandler[cb_history_idx].scrnID)
      {
	   	if (historyCBHandler[cb_history_idx].historydelCBPtr)
	   	is_stop_delete = historyCBHandler[cb_history_idx].historydelCBPtr((void*)MMI_FALSE);
	      ClearDelScrnIDCallbackHandler(historyCBHandler[cb_history_idx].scrnID, NULL);
   	}
	   //JL, if want to stop to run next decrement will return true.
      is_del_cb_found = MMI_FALSE;
   }
   else
   {
	   is_del_cb_found = SearchDelScrnIDCallbackHandler(historyData[currHistoryIndex].scrnID, &cb_history_idx);
   }
   
	if ( historyData[currHistoryIndex].guiBuffer )
		OslMfree(historyData[currHistoryIndex].guiBuffer);
	if ( historyData[currHistoryIndex].inputBuffer )
		OslMfree(historyData[currHistoryIndex].inputBuffer);
	memset(&historyData[currHistoryIndex], 0, sizeof(historyNode));
	if((currHistoryIndex > MIN_HISTORY + 1) && (currHistoryIndex < MAX_HISTORY) )
	{
		--currHistoryIndex;
	}

	else if( (currHistoryIndex == 1) && ( bRoundOffFlag == 0))
	{
			currHistoryIndex=0;

}

	else if(currHistoryIndex == 0)
	{
			currHistoryIndex = -1;
			bRoundOffFlag = 0;
}
	else if(bRoundOffFlag)
	{
			currHistoryIndex = MAX_HISTORY - 1;
			bRoundOffFlag = 0;
	}

	//JL accept for next to decrement will return true; otherwise return false.
	//return currHistoryIndex;
	return is_stop_delete/*MMI_FALSE*/;
}





#ifdef OSL_MEMORY_DUMP
/**************************************************************

	FUNCTION NAME		: AddHistoryReferenceMemoryRecord()

  	PURPOSE				: adds a history along with memory record 

	INPUT PARAMETERS	: history *addHistory, char* fileName, int lineNumber

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void AddHistoryReferenceMemoryRecord(history_t *addHistory, char* fileName, int lineNumber)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 length = 0;

#ifdef __MMI_UI_TAB_PANE__
	if(disable_history_save) return;
#endif

#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	if(redrawing_old_screens && is_dinit_called)
	{
		S32 id=historyData[currHistoryIndex].scrnID;
		S32 count=currHistoryIndex;
		currHistoryIndex=g_count;
		InsertHistoryBeforeThisScrnReference(id,addHistory);
		g_count++;
		currHistoryIndex=count;
		currHistoryIndex++;
		is_dinit_called++;
		return;
	}
	if(redrawing_old_screens && IsBackHistory != MMI_TRUE)
	{
		currHistoryIndex--;
		decrement();
		is_dinit_called=1;
	}
#endif
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* 1. Check for OK pressed or BACK pressed*/
	
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_ADD_HISTREFMEMREC_HDLR));
	

    if(IsBackHistory != MMI_TRUE) 
	{
		
		increment();
		/* 3. Store History to History Data Structure */
		memset(&historyData[currHistoryIndex], 0, sizeof(historyNode));
		historyData[currHistoryIndex].scrnID = addHistory->scrnID;
		historyData[currHistoryIndex].entryFuncPtr = addHistory->entryFuncPtr;
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
 
		if(check_for_small_history_node)
		historyData[currHistoryIndex].isSmallScreen=(U16)small_history_node;
		else
			historyData[currHistoryIndex].isSmallScreen=0;
 
#endif
		length = pfnUnicodeStrlen((PS8)addHistory->inputBuffer); 
		if ( length) 
		{
			historyData[currHistoryIndex].inputBuffer = OslMallocCHK(length*ENCODING_LENGTH + ENCODING_LENGTH, fileName, lineNumber);
			pfnUnicodeStrcpy((PS8)historyData[currHistoryIndex].inputBuffer, (PS8)addHistory->inputBuffer);
		}
		historyData[currHistoryIndex].guiBuffer = OslMallocCHK(MAX_GUI_BUFFER, fileName, lineNumber);
		memcpy(historyData[currHistoryIndex].guiBuffer, addHistory->guiBuffer,MAX_GUI_BUFFER);		
	}
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	small_history_node=0;
	if(redrawing_old_screens && IsBackHistory != MMI_TRUE)
		currHistoryIndex++;
#endif
	IsBackHistory = MMI_FALSE;
}

#else

/**************************************************************

	FUNCTION NAME		: AddHistoryReference()

  	PURPOSE				: adds a history 

	INPUT PARAMETERS	: history *addHistory

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

void AddHistoryReference(history_t *addHistory)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S32 length = 0;

#ifdef __MMI_UI_TAB_PANE__
	if(disable_history_save) return;
#endif

#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	if(redrawing_old_screens && is_dinit_called)
	{
		S16 id=historyData[currHistoryIndex].scrnID;
		S32 count=currHistoryIndex;
		currHistoryIndex=g_count;
		InsertHistoryBeforeThisScrnReference(id,addHistory);
		g_count++;
		currHistoryIndex=count;
		currHistoryIndex++;
		is_dinit_called++;
		return;
	}
	if(redrawing_old_screens && IsBackHistory != MMI_TRUE)
	{
		currHistoryIndex--;
		decrement();
		is_dinit_called=1;
	}
#endif
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/* 1. Check for OK pressed or BACK pressed*/
	
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_ADD_HISTREF_HDLR));
	

    if(IsBackHistory != MMI_TRUE) 
	{
		
		increment();
		/* 3. Store History to History Data Structure */
		memset(&historyData[currHistoryIndex], 0, sizeof(historyNode));
		historyData[currHistoryIndex].scrnID = addHistory->scrnID;
		historyData[currHistoryIndex].entryFuncPtr = addHistory->entryFuncPtr;
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
 
		if(check_for_small_history_node)
		historyData[currHistoryIndex].isSmallScreen=(U16)small_history_node;
		else
			historyData[currHistoryIndex].isSmallScreen=0;
 
#endif
		length = pfnUnicodeStrlen((PS8)addHistory->inputBuffer); 
		if ( length) 
		{
			historyData[currHistoryIndex].inputBuffer = OslMalloc(length*ENCODING_LENGTH + ENCODING_LENGTH);
			pfnUnicodeStrcpy((PS8)historyData[currHistoryIndex].inputBuffer, (PS8)addHistory->inputBuffer);
		}
		historyData[currHistoryIndex].guiBuffer = OslMalloc(MAX_GUI_BUFFER);
		memcpy(historyData[currHistoryIndex].guiBuffer, addHistory->guiBuffer,MAX_GUI_BUFFER);		
	}
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	small_history_node=0;
	if(redrawing_old_screens && IsBackHistory != MMI_TRUE)
		currHistoryIndex++;
#endif
	IsBackHistory = MMI_FALSE;
}

#endif



/**************************************************************

	FUNCTION NAME		: AddNHistory_ext()

  	PURPOSE				: adds a history with user specified size

	INPUT PARAMETERS	: history addHistory, U16 size

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
 
void AddNHistory_ext(history_t *addHistory,U16 size)
 
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
/* 1. Check for OK pressed or BACK pressed*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_ADD_NHIST_HDLR, size, IsBackHistory));
#ifdef __MMI_UI_TAB_PANE__
	if(disable_history_save) return;
#endif
 
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	if(redrawing_old_screens && is_dinit_called)
	{
		S16 id=historyData[currHistoryIndex].scrnID;
		S32 count=currHistoryIndex;
		currHistoryIndex=g_count;
		InsertHistoryBeforeThisScrnReference(id,addHistory);
		g_count++;
		currHistoryIndex=count;
		currHistoryIndex++;
		is_dinit_called++;
		return;
	}
	if(redrawing_old_screens && IsBackHistory != MMI_TRUE)
	{
		currHistoryIndex--;
		decrement();
		is_dinit_called=1;
	}
#endif
 
    if(IsBackHistory != MMI_TRUE) 
	{
		
		increment();
		/* 3. Store History to History Data Structure */
		memset(&historyData[currHistoryIndex], 0, sizeof(historyNode));
		historyData[currHistoryIndex].scrnID = addHistory->scrnID;
		historyData[currHistoryIndex].entryFuncPtr = addHistory->entryFuncPtr;
	
 
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
		if(check_for_small_history_node)
			historyData[currHistoryIndex].isSmallScreen=(U16)small_history_node;
		else
			historyData[currHistoryIndex].isSmallScreen=0;
#endif
 
		historyData[currHistoryIndex].inputBuffer = OslMalloc(size + ENCODING_LENGTH + 2);
		memcpy(historyData[currHistoryIndex].inputBuffer,&size,2);
		memcpy((PS8)(historyData[currHistoryIndex].inputBuffer + 2), (PS8)addHistory->inputBuffer, (U32)size);
	
		historyData[currHistoryIndex].guiBuffer = OslMalloc(MAX_GUI_BUFFER);
		memcpy(historyData[currHistoryIndex].guiBuffer, addHistory->guiBuffer,MAX_GUI_BUFFER);		
	}
 
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	small_history_node=0;
	if(redrawing_old_screens && IsBackHistory != MMI_TRUE)
		currHistoryIndex++;
#endif
 
	IsBackHistory = MMI_FALSE;

}
/**************************************************************

	FUNCTION NAME		: GetHistory()

  	PURPOSE				: Used to get the history buffer for a screen id

	INPUT PARAMETERS	: U16 scrnID, history *ptrHistory

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - Status , success or failure

 

**************************************************************/
U8  GetHistory(U16 scrnID, history_t *ptrHistory)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;
	U8 Status = ST_FAILURE;
	U16 nSize;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GET_HIST_HDLR, scrnID, currHistoryIndex));

	count = currHistoryIndex;
	do
	{
		//S16 nSize;
		if(scrnID == historyData[count].scrnID)
		{
			ptrHistory->entryFuncPtr = historyData[count].entryFuncPtr;
			ptrHistory->scrnID		 = historyData[count].scrnID;
			if ( historyData[count].guiBuffer )
				memcpy(ptrHistory->guiBuffer, historyData[count].guiBuffer, MAX_GUI_BUFFER);
			else 
				memset(ptrHistory->guiBuffer, 0, MAX_GUI_BUFFER);
			if ( historyData[count].inputBuffer )
 
			{
				memcpy(&nSize, historyData[count].inputBuffer, 2);
				memcpy(ptrHistory->inputBuffer, historyData[count].inputBuffer+2, (nSize>MAX_INPUT_BUFFER)? MAX_INPUT_BUFFER:nSize );
			}
			else
				memset(ptrHistory->inputBuffer, 0, MAX_INPUT_BUFFER);
			Status = ST_SUCCESS;
			break;
		}

		if(count > MIN_HISTORY + 1 && count < MAX_HISTORY )
		{
			--count;
		}
		else 
		{
			if(bRoundOffFlag)
				count = MAX_HISTORY - 1;
			else
				break; /* Status is by default FAILURE so no need to set again*/
		}

	}while((count != currHistoryIndex) && (historyData[count].entryFuncPtr != NULL)); 

	return Status;

	
}

/**************************************************************

	FUNCTION NAME		: GetHistoryScrID()

  	PURPOSE				: Used to get the reference of the history buffer for a screen id

	INPUT PARAMETERS	: U16 scrnID, historyNode **ptrHistory

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - Status

 

**************************************************************/
U8  GetHistoryScrID(U16 scrnID, historyNode **ptrHistory)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;
	U8 Status = ST_FAILURE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GET_HIST_SCRID_HDLR, scrnID, currHistoryIndex));

	count = currHistoryIndex;
	do
	{
		if(scrnID == historyData[count].scrnID)
		{
			*ptrHistory=&historyData[count];
			Status = ST_SUCCESS;
			break;
		}

		if(count > MIN_HISTORY + 1 && count < MAX_HISTORY )
		{
			--count;
		}
		else 
		{
			if(bRoundOffFlag)
				count = MAX_HISTORY - 1;
			else
				break; /* Status is by default FAILURE so no need to set again*/
		}

	}while((count != currHistoryIndex) && (historyData[count].entryFuncPtr != NULL)); 

	return Status;

	
}

/**************************************************************

	FUNCTION NAME		: GetNHistory()

  	PURPOSE				: Used to get the history for a screen id, and also the history buffer size

	INPUT PARAMETERS	: U16 scrnID, history *ptrHistory

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - Status

 

**************************************************************/
U8  GetNHistory(U16 scrnID, history_t *ptrHistory)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;
	U8 Status = ST_FAILURE;
	U16 size;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GET_NHIST_HDLR, scrnID, currHistoryIndex));

	count = currHistoryIndex;
	do
	{
		if(scrnID == historyData[count].scrnID)
		{
			ptrHistory->entryFuncPtr = historyData[count].entryFuncPtr;
			ptrHistory->scrnID		 = historyData[count].scrnID;
			if ( historyData[count].guiBuffer )
				memcpy(ptrHistory->guiBuffer, historyData[count].guiBuffer, MAX_GUI_BUFFER);
			else 
				memset(ptrHistory->guiBuffer, 0, MAX_GUI_BUFFER);
			if ( historyData[count].inputBuffer )
			{
				memcpy((PS8)&size, historyData[count].inputBuffer, 2);
				memcpy(ptrHistory->inputBuffer, (PS8)(historyData[count].inputBuffer + 2), size);
			}
			else
				memset(ptrHistory->inputBuffer, 0, MAX_INPUT_BUFFER);
			Status = ST_SUCCESS;
			break;
		}

		if(count > MIN_HISTORY + 1 && count < MAX_HISTORY )
		{
			--count;
		}
		else 
		{
			if(bRoundOffFlag)
				count = MAX_HISTORY - 1;
			else
				break; /* Status is by default FAILURE so no need to set again*/
		}

	}while((count != currHistoryIndex) && (historyData[count].entryFuncPtr != NULL)); 

	return Status;

	
}


/**************************************************************

	FUNCTION NAME		: CheckIsBackHistory()

  	PURPOSE				: Is in GoBackHistory

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL CheckIsBackHistory(void)
{
	return IsBackHistory;
}


/**************************************************************

	FUNCTION NAME		: GoBackHistory()

  	PURPOSE				: deletes previous screen history

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/

#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
extern void CallBackPowerOnAnimationCompleteWrapper(void);
#endif

extern BOOL gSecuritySetting;
 #ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
 extern U8 change_window_style;
 #endif
void GoBackHistory(void)
{
   //MCI_USBDisconnect(); //added by licheng to disconected usb mode when press power-key
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   #if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
   BOOL Exit_From_FM = (g_DirectMode && currHistoryIndex == -1);
   #endif
   gSecuritySetting = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GO_BACK_HIST_HDLR));
	
	 #ifdef __MMI_CHANGE_WIN_WITH_DYNAMIC__
//	 	change_window_style = 11;
	 #endif
#if defined(__MMI_MAINLCD_220X176__) || defined(__PROJECT_GALLITE_C01__)
	if(MMI_CATEGORY222_ID == g_dm_data.S32CatId)
		cat222_title_text_stop_scrolling();
#endif
	ExecutePopHistory();

#if defined(__DIRECT_ENTRY_FACTORY_MODE_ON_BOOTUP__)
       if (Exit_From_FM)
          CallBackPowerOnAnimationCompleteWrapper();
#endif

}

/**************************************************************

	FUNCTION NAME		: GoBackToHistory(U16 )

  	PURPOSE				: Goes back to specified screen 

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
U8 GoBackToHistory(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;
	U8 Status = ST_FAILURE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GO_BACK_TO_HIST_HDLR, scrnid));

	count = currHistoryIndex;

	do
	{
		if( historyData[currHistoryIndex].scrnID  == scrnid )
		{
				Status = ST_SUCCESS;
				break;
		}		
		
		/* Chack added when amit is doing testing on h/w for call management*/
//micha0731
		if(currHistoryIndex > 0 ) 
		{			
			if (decrement())
			{
				Status = ST_SUCCESS;
				break;
			}
		}
//micha0731	
	}while( (count != currHistoryIndex ) && ( historyData[currHistoryIndex].entryFuncPtr != NULL )
							&& (currHistoryIndex > 0)  );

	if(Status)
	{
		ExecutePopHistory();
	}
//micha0731
	else if ( currHistoryIndex > -1)
	{
      //To make sure the callback will run at topist screen
      ExecTopScrnCallbackHandler();
		(*(historyData[0].entryFuncPtr))();
		currHistoryIndex = -1;
	}

	return Status;
}

/**************************************************************

	FUNCTION NAME		: GoBacknHistory()

  	PURPOSE				: Goes 'n' screens back

	INPUT PARAMETERS	: U16 nHistory

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void GoBacknHistory(U16 nHistory)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 Index = 0;
	S16 Count = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GO_BACK_NHIST_HDLR, nHistory));

	if (bRoundOffFlag)
	{
		Index = MAX_HISTORY + currHistoryIndex;
	}
	else 
	{
        Index = currHistoryIndex;
	}

	if( Index >= nHistory)
	{
		for(Count = nHistory ; Count >= 1; --Count)
		{
			//JL, if want to stop to run next decrement will return true.
			if (decrement())
				break;
		}
		ExecutePopHistory();
	}
}

/**************************************************************

	FUNCTION NAME		: ExecutePopHistory()

  	PURPOSE				: executes current scrn & then deletes from history

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
static void ExecutePopHistory(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_EXE_POP_HIST_HDLR, IsBackHistory, CheckMainLCDHistoryBack));

	IsBackHistory = MMI_TRUE;
	CheckMainLCDHistoryBack=MMI_TRUE;

	if( historyData[currHistoryIndex].entryFuncPtr && currHistoryIndex > 0) 
	{
		U16 id;
		//To make sure the callback will run at topist screen
		ExecTopScrnCallbackHandler();
		
		//entryFuncPtr = (*(historyData[currHistoryIndex].entryFuncPtr));
		//decrement(MMI_TRUE);
		//entryFuncPtr();

		id = historyData[currHistoryIndex].scrnID;
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
		if(historyData[currHistoryIndex].isSmallScreen)
		{
			U16 i;
			U16 scr_count=0;
			g_count=currHistoryIndex;
			i=currHistoryIndex-1;
			while(historyData[i].isSmallScreen && i>0)
				i--;
			redrawing_old_screens=1;
			gdi_layer_lock_frame_buffer();
			while(i<g_count)
			{
				currHistoryIndex=i;
				if(i==0 && historyData[i].isSmallScreen)
				{
					clear_screen();	
				}
				 
					IsBackHistory = MMI_TRUE;
					CheckMainLCDHistoryBack=MMI_TRUE;
				 
				is_dinit_called=0;
				(*(historyData[i].entryFuncPtr))();\
				if(is_dinit_called>1)
					i=i + is_dinit_called - 1;
				i++;
				scr_count++;
			}
			gdi_layer_unlock_frame_buffer();
			 
			IsBackHistory = MMI_TRUE;
			CheckMainLCDHistoryBack=MMI_TRUE;
			 
			is_dinit_called=0;
			currHistoryIndex=g_count;
		(*(historyData[currHistoryIndex].entryFuncPtr))();
		if(id == historyData[currHistoryIndex].scrnID)
			decrement();
			redrawing_old_screens=0;
		}
		else
#endif
		{
		(*(historyData[currHistoryIndex].entryFuncPtr))();
		if(id == historyData[currHistoryIndex].scrnID)
			decrement();
	}
	}
	else
	{
		//To make sure the callback will run at topist screen
		ExecTopScrnCallbackHandler();
		/* Go to idle screen*/
		currHistoryIndex = -1;
		if((historyData[0].entryFuncPtr))
		{
		 (*(historyData[0].entryFuncPtr))();
		}

		IsBackHistory = MMI_FALSE;
		
	}

	CheckMainLCDHistoryBack=MMI_FALSE;
   	// reset delete CB flag, while go back screen.
	is_del_cb_found = MMI_FALSE;

}
/**************************************************************

	FUNCTION NAME		: GetCurrScrnId()

  	PURPOSE				: Gets the id of the screen on the top of history stack.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: U16 - Current Screen Id

 

**************************************************************/
U16 GetCurrScrnId(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	return (historyData[currHistoryIndex].scrnID);
}


/**************************************************************

	FUNCTION NAME		: DeleteBeyondScrTillScr()

  	PURPOSE				: To delete scrns beyond scrn A 
						  till scrn B 	

	INPUT PARAMETERS	: U16 scrnid1,U16 scrnid2

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/
U8 DeleteBeyondScrTillScr(U16 beyondScrnid,U16 tillScrnid)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
U16 count = 0,count1 = 0,i = 0;
S16 endScreenPresent=-1, startScreenPresent=-1;
U16 screensDeleted=0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_DEL_BEYOND_SCR_TILL_SCR_HDLR, beyondScrnid, tillScrnid));

 if(currHistoryIndex > 0)
	 count = currHistoryIndex;
	 count1=currHistoryIndex;
	 
	 while(count)
	 {
		  if(historyData[count].scrnID==beyondScrnid)
		  {
		   startScreenPresent=count;
		   break;
		  }
	  --count;
	 }

	if(startScreenPresent > 0)
	{
		 --count;
		 //if tillscrn is at index 0
		 while(count||((count==0)&&(historyData[0].scrnID==tillScrnid)))
		 {
			  if(historyData[count].scrnID==tillScrnid)
			  {
			   endScreenPresent=count;
			   break;
			  }
			  --count;
		 }
	}
	else
	{
		return ST_FAILURE;
	}

	 if(historyData[count].scrnID!=tillScrnid)
	 {
		 return ST_FAILURE;
	 }

	 for(count=startScreenPresent-1; count>=endScreenPresent; count--)
	 {
		  if ( historyData[count].guiBuffer )
		   OslMfree(historyData[count].guiBuffer);
		  if ( historyData[count].inputBuffer )
		   OslMfree(historyData[count].inputBuffer);
		  --currHistoryIndex;
		  screensDeleted++;
	 }
	//i = endScreenPresent+1;

	memcpy(historyData+endScreenPresent, historyData+startScreenPresent, sizeof(historyNode)*(count1-startScreenPresent+1));
	i = count1-endScreenPresent+1;
	memset(historyData+(currHistoryIndex+1),0,(sizeof(historyNode)*screensDeleted));
	return ST_SUCCESS;	
}


/**************************************************************

	FUNCTION NAME		: DeleteBetweenScreen

  	PURPOSE				: Deletes Between two Screens including the boundaries

	INPUT PARAMETERS	: U16 StartScrId, U16 EndScrId

	OUTPUT PARAMETERS	: nil

	RETURNS				: U16 - status

 

**************************************************************/

U16 DeleteBetweenScreen(U16 StartScrId, U16 EndScrId)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	 U16 count = 0,count1 = 0,i = 0;//Renwy modify 2008/07/19
	 S16 endScreenPresent=-1, startScreenPresent=-1;
	 U16 screensDeleted=0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	 MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_DEL_BETWEEN_SCR_HDLR, StartScrId, EndScrId));

	 if(currHistoryIndex > 0)
	 count = currHistoryIndex;
	 count1=currHistoryIndex;
	 
	 while(count)
	 {
		  if(historyData[count].scrnID==EndScrId)
		  {
		   endScreenPresent=count;
		   break;
		  }
	  --count;
	 }

	if(endScreenPresent >= 0)
	{
		 while(count)
		 {
			  if(historyData[count].scrnID==StartScrId)
			  {
			   startScreenPresent=count;
			   break;
			  }
			  --count;
		 }
	}
	else
		return ST_FAILURE;

	 if(endScreenPresent==-1 || startScreenPresent==-1) return ST_FAILURE;
	 for(count=startScreenPresent; count<=endScreenPresent; count++)
	 {
	 	   // Not support for stop callback at this funciton
	 	   ExecHistoryScrnCallBackHnadle(count);
		   
		  if ( historyData[count].guiBuffer )
		   OslMfree(historyData[count].guiBuffer);
		  if ( historyData[count].inputBuffer )
		   OslMfree(historyData[count].inputBuffer);
		  --currHistoryIndex;
		  screensDeleted++;
	 }
	i = endScreenPresent+1;
	memcpy(historyData+startScreenPresent,historyData+i, sizeof(historyNode)*((count1-endScreenPresent)));
	i = currHistoryIndex+1;
	memset(historyData+i,0,(sizeof(historyNode)*screensDeleted));
	return ST_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: DeleteScreen()

  	PURPOSE				: Deletes Screen from History

	INPUT PARAMETERS	: U16 ScrId

	OUTPUT PARAMETERS	: nil

	RETURNS				: U16 - status

 

**************************************************************/

U16 DeleteScreenIfPresent(U16 ScrId)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count,i;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_DEL_SCR_IF_PRESENT_HDLR, ScrId));


	if(!IsScreenPresent(ScrId))
	{
		return ST_FAILURE;
	}
	
	count = currHistoryIndex;
	
	// Locating the screen
	while(count)
	{
		if(historyData[count].scrnID==ScrId)
		{

			break;
		}
		--count;
	}

      // Not support for stop callback at this funciton
      ExecHistoryScrnCallBackHnadle(count);

	// Deleting the node
	if ( historyData[count].guiBuffer )
		OslMfree(historyData[count].guiBuffer);
	if ( historyData[count].inputBuffer )
		OslMfree(historyData[count].inputBuffer);

	// Shifting All the nodes by one notch
	for(i=count+1;i<=currHistoryIndex;i++,count++)
	{
		memcpy(&historyData[count],&historyData[i], sizeof(historyNode));	
	}
	
	--currHistoryIndex;
	memset(&historyData[currHistoryIndex+1], 0, sizeof(historyNode));
	return ST_SUCCESS;
}	

/**************************************************************

	FUNCTION NAME		: InsertHistoryBeforeThisScrnReference ()

  	PURPOSE				: Insert new screen into history before this screen

	INPUT PARAMETERS	: U16 ScrId

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void InsertHistoryBeforeThisScrnReference (U16 scrnId, history_t *addHistory)
{
	U16 count=0;
	U16	i=0;
	S32 len=0;
	// find this screen in history
	if (!IsScreenPresent (scrnId))
	{
		return ;
	}
	count = currHistoryIndex;
	// Locating the screen
	while (count)
	{
		if (historyData[count].scrnID==scrnId)
		{
			break;
		}
		--count;
	}
	//
	increment();
	/* Shifting All the nodes by one notch */
	for(i=currHistoryIndex; i!=count; i--)
	{
		memcpy (&historyData[i], &historyData[i-1], sizeof (historyNode));	
	}
	/* insert new screen in hostory	 */
	memset (&historyData[count], 0, sizeof (historyNode));
	historyData[count].scrnID = addHistory->scrnID;
	historyData[count].entryFuncPtr = addHistory->entryFuncPtr;
	len = pfnUnicodeStrlen ((PS8)addHistory->inputBuffer); 
	if ( len) 
	{
		historyData[count].inputBuffer = OslMalloc (len*ENCODING_LENGTH + ENCODING_LENGTH);
		pfnUnicodeStrcpy ((PS8)historyData[count].inputBuffer, (PS8)addHistory->inputBuffer);
	}
	historyData[count].guiBuffer = OslMalloc( MAX_GUI_BUFFER);
	memcpy (historyData[count].guiBuffer, addHistory->guiBuffer, MAX_GUI_BUFFER);		
	/* return success */
	return ;
}


/**************************************************************

	FUNCTION NAME		: GetScreenCountInHistory()

  	PURPOSE				: Get the screen numbers in the history

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: S16 - screen count

 

**************************************************************/
S16 GetScreenCountInHistory(void)
{
   return (currHistoryIndex+1);
}


/**************************************************************

	FUNCTION NAME		: GetCurrInputBuffer()

  	PURPOSE				: gets current screen input buffer

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8* - history buffer

 

**************************************************************/
U8* GetCurrInputBuffer(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GET_CUR_INPUT_BUF_HDLR, scrnid));

	count = currHistoryIndex;
	
	if( historyData[currHistoryIndex].scrnID  == scrnid )
	   {
				return (historyData[count].inputBuffer);
				
	   }		
	else
		{

			return NULL;
		}
}

/**************************************************************

	FUNCTION NAME		: GetCurrNInputBuffer()

  	PURPOSE				: Retrieve the input current screen input buffer
						  	

	INPUT PARAMETERS	: U16 scrnid, U16 *size

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8*

 
						  strings , so along with pointer size is also 
						  returned 

**************************************************************/

U8* GetCurrNInputBuffer(U16 scrnid,U16 *size)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = currHistoryIndex;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GET_CUR_NINPUT_BUF_HDLR, scrnid));

	if( historyData[currHistoryIndex].scrnID  == scrnid )
	   {
				memcpy(size,historyData[count].inputBuffer,2);
				return ((historyData[count].inputBuffer )+ 2);
	   }		
	else
		{
			return NULL;
		}
}

/**************************************************************

	FUNCTION NAME		: GetCurrGuiBuffer(U16 )

  	PURPOSE				: gets current screen gui buffer

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8*

 

**************************************************************/
U8* GetCurrGuiBuffer(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_GET_CUR_GUI_BUF_HDLR, scrnid, currHistoryIndex));

	count = currHistoryIndex;
	
	if( historyData[count].scrnID  == scrnid )
	{
			return (historyData[count].guiBuffer);
	
	}		

	else
	{
		return NULL;
	}
}




extern void InitSubLCDHistory(SubLcdHistoryCBPtr sublcd_root_screen_cb_ptr);


/**************************************************************

	FUNCTION NAME		: InitHistory(void)

  	PURPOSE				: Initialize the history data structure 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

*****************************************************************/

void InitHistory(MainLcdHistoryCBPtr main_root_ptr, SubLcdHistoryCBPtr sub_root_ptr)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   //U16 count =0;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	memset(historyData , 0, sizeof(historyNode) * MAX_HISTORY);
	//CSD added by JL for history callback
	memset(historyCBHandler, 0, sizeof(historyCallback) * MAX_HISTORY);

	InitSubLCDHistory(sub_root_ptr);
	
}


extern void SubLCDHistoryDump(void);
/*************************************************************************

	FUNCTION NAME		: HistoryDump(void)

  	PURPOSE				: Dumps the history to file or debug window

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************************/

void HistoryDump(void)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count= 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	count = currHistoryIndex;
	do
	{
		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_DUMP_HDLR, count, historyData[count].scrnID));

	    if(count > MIN_HISTORY  && count < MAX_HISTORY )
		{
			--count;
		}
		else 
		{
			if(bRoundOffFlag)
			{
			 count = MAX_HISTORY - 1;
			}
			else
				break;
		}
	
	}while((count != currHistoryIndex) && (historyData[count].entryFuncPtr != NULL) );
	
   SubLCDHistoryDump();
}

/*************************************************************************

	FUNCTION NAME		: HistoryReplace(void)

  	PURPOSE				: To replace a specific history

	INPUT PARAMETERS	: in_scrn_id, in_src_func

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBool

 

***************************************************************************/
pBOOL HistoryReplace(U16 out_scrn_id, U16 in_scrn_id, FuncPtr in_src_func)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count= 0;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_HIST_REPLACE_HDLR, out_scrn_id, in_scrn_id));

	for (count = 0; ((count < MAX_HISTORY)&&((count <= currHistoryIndex) && (currHistoryIndex != -1))); count++) 
	{
		// Find the matched source history and then to write into destinate history
		if(historyData[count].scrnID  == out_scrn_id )
		{
			historyData[count].scrnID = in_scrn_id;
			historyData[count].entryFuncPtr= in_src_func;
			if ( historyData[count].guiBuffer )
				OslMfree(historyData[count].guiBuffer);
			if ( historyData[count].inputBuffer )
				OslMfree(historyData[count].inputBuffer);
			historyData[count].guiBuffer = NULL;
			historyData[count].inputBuffer = NULL;
			return MMI_TRUE;
		}
	}

	return MMI_FALSE;

}

/**************************************************************

	FUNCTION NAME		: DeleteNHistory()

  	PURPOSE				: Deletes 'n' history nodes from current history index  

	INPUT PARAMETERS	: U16 n

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 
							entry functions of the deleted nodes

**************************************************************/
void DeleteNHistory(U16 DeleteCount)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 Index = 0;
	S16 Count = 0;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_DEL_NHIST_HDLR, DeleteCount));

	if (bRoundOffFlag)
	{
		Index = MAX_HISTORY + currHistoryIndex;
	}
	else 
	{
        Index = currHistoryIndex;
	}

	//To make sure the callback will run at topist screen
	ExecTopScrnCallbackHandler();

	if( Index >= DeleteCount)
	{
		for(Count = DeleteCount ; Count >= 1; --Count)
		{
			//JL, if want to stop to run next decrement will return true.
			if (decrement())
				break;
		}
	}
}

/**************************************************************

	FUNCTION NAME		: ExecutecurrHisIndEntryFunc()

  	PURPOSE				: executes current history handles  Entry function

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void ExecutecurrHisIndEntryFunc(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	if( historyData[currHistoryIndex].entryFuncPtr )
	{
		(*(historyData[currHistoryIndex].entryFuncPtr))();
	}
}

/**************************************************************

	FUNCTION NAME		: DeleteUptoScrID()

  	PURPOSE				: Go to a screen and delete historyall ahead of it 	

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/
U8 DeleteUptoScrID(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;
	U8 Status = ST_FAILURE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_DEL_UPTO_SCRID_HDLR, scrnid, currHistoryIndex));

	count = currHistoryIndex;

	if(!IsScreenPresent(scrnid))
	{
		return ST_FAILURE;
	}

	//To make sure the callback will run at topist screen
	ExecTopScrnCallbackHandler();

	do
	{
		if( historyData[currHistoryIndex].scrnID  == scrnid )
		{
			
			Status = ST_SUCCESS;
			break;
		}		
		//JL, if want to stop to run next decrement will return true.
		if (decrement())
		{
			Status = ST_SUCCESS;
			break;
		}
	
	}while( (count != currHistoryIndex ) && ( historyData[currHistoryIndex].entryFuncPtr != NULL ) );
	return Status;
}


/**************************************************************

 

  
						  

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/

U8 GoBeyondMarkerScr(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;
	U8 Status = ST_FAILURE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_GO_BEYOND_MARKER_SCR_HDLR, scrnid, currHistoryIndex));

	count = currHistoryIndex;

	if(!IsScreenPresent(scrnid))
	{
		return ST_FAILURE;
	}

	do
	{
		if( historyData[currHistoryIndex].scrnID  == scrnid )
		{
			

			Status = ST_SUCCESS;
			break;
		}		
		//JL, if want to stop to run next decrement will return true.
		if (decrement())
		{
			Status = ST_SUCCESS;
			break;
		}
	
	}while( (count != currHistoryIndex ) && ( historyData[currHistoryIndex].entryFuncPtr != NULL ) );

	if(Status)
	{
		decrement();
		ExecutePopHistory();
	}
	return Status;
}


	/**************************************************************

		FUNCTION NAME		: DeleteNScrId()

  		PURPOSE				: Deletes up to screen id and go  beyond it one node
							  

		INPUT PARAMETERS	: U16 scrnid

		OUTPUT PARAMETERS	: nil

		RETURNS				: U8 - status

 

	**************************************************************/

U8 DeleteNScrId(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count = 0;
	U8 Status = ST_FAILURE;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_DEL_NSCRID_HDLR, scrnid, currHistoryIndex));

	count = currHistoryIndex;

	if(!IsScreenPresent(scrnid))
	{
		return ST_FAILURE;
	}

	//To make sure the callback will run at topist screen
	ExecTopScrnCallbackHandler();

	do
	{
		if( historyData[currHistoryIndex].scrnID  == scrnid )
		{
			

			Status = ST_SUCCESS;
			break;
		}		
		//JL, if want to stop to run next decrement will return true.
		if (decrement())
		{
			Status = ST_SUCCESS;
			break;
		}
	
	}while( (count != currHistoryIndex ) && ( historyData[currHistoryIndex].entryFuncPtr != NULL ) );

	if(Status)
	{
		decrement();
	}
	return Status;
}

/**************************************************************

	FUNCTION NAME		: IsScreenPresent()

  	PURPOSE				: Queries history for presence of a screen Id						  

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsScreenPresent(U16 scrnId)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count;
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	
	for (count = 0; ((count < MAX_HISTORY)&&((count <= currHistoryIndex) && (currHistoryIndex != -1))); count++) {
		if(historyData[count].scrnID  == scrnId )
		{
			MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_IS_SCR_PRESENT_HDLR, scrnId, MMI_TRUE));
			return MMI_TRUE;
	   }
	}
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_IS_SCR_PRESENT_HDLR, scrnId, MMI_FALSE));
	return MMI_FALSE;
	
}

/**************************************************************

	FUNCTION NAME		: IsMainLCDHistoryBack()

  	PURPOSE				: Queries MainLCD history is back now or not 

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsMainLCDHistoryBack(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
  return CheckMainLCDHistoryBack;
   
}

/**************************************************************

	FUNCTION NAME		: GetPreviousScrnIdOf()

  	PURPOSE				: Queries previous scrn id of this Id						  

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL GetPreviousScrnIdOf (U16 scrnId, U16* previousScrnId)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/


	for (count = 0; ((count < MAX_HISTORY)&&((count <= currHistoryIndex) && (currHistoryIndex != -1))); count++) 
	{
		if(historyData[count].scrnID  == scrnId)
		{
			if((count-1)>=0)
			{
				*previousScrnId=historyData[count-1].scrnID;
				MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_GET_PREVIOUS_SCR_IDOF_HDLR, scrnId, historyData[count-1].scrnID));
				return MMI_TRUE;
			}
			MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_GET_PREVIOUS_SCR_IDOF_2_HDLR, scrnId));
			return MMI_FALSE;
		}
	}
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_GET_PREVIOUS_SCR_IDOF_2_HDLR, scrnId));
	return MMI_FALSE;	
}
/**************************************************************

	FUNCTION NAME		: GetNextScrnIdOf()

  	PURPOSE				: Queries next scrn id of this Id						  

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL GetNextScrnIdOf (U16 scrnId, U16* nextScrnId)
{
	S16 count;

	for (count = 0; ((count < MAX_HISTORY)&&((count <= currHistoryIndex) && (currHistoryIndex != -1))); count++) 
	{
		if(historyData[count].scrnID  == scrnId)
		{
			if((count+1)<=currHistoryIndex)
			{
				*nextScrnId=historyData[count+1].scrnID;
				return MMI_TRUE;
			}
			return MMI_FALSE;
		}
	}
	return MMI_FALSE;	
}
/**************************************************************

 

  
						  
	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8

 

**************************************************************/

U8 IsCurrMarkerScreen(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_IS_CUR_MARKER_SCR_HDLR, scrnid, historyData[currHistoryIndex].scrnID));
	return ( historyData[currHistoryIndex].scrnID  == scrnid );
}

/**************************************************************

	FUNCTION NAME		: ExecuteRootHistoryScreen()

  	PURPOSE				: To execute the root history screen.

	INPUT PARAMETERS	: funcPtr: reserve not use

	OUTPUT PARAMETERS	: U8

	RETURNS				: U8

 

**************************************************************/
U8 ExecuteRootMainHistoryScreen(void *funcPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
 	U8 Status = ST_SUCCESS;
    S16 count = currHistoryIndex;
   //MCI_USBDisconnect(); //added by licheng to disconected usb mode when press power-key
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/*reserve not decrement: Memory is not being released bcos of one particular case of keypad lock*/

	//To make sure the callback will run at topist screen, and callback for APP
	ExecTopScrnCallbackHandler();
   if (currHistoryIndex <=0 )
   {
		if (historyData[0].entryFuncPtr) 
		{
			(*(historyData[0].entryFuncPtr))();
		}
		return ST_SUCCESS;
     	
   }
    
   // down popup all others history to callback for APP
	do
	{
		if( count == 0 )
		{
				Status = ST_SUCCESS;
				break;
		}		
		if(count > 0 ) 
		{			
			if (ExecHistoryScrnCallBackHnadle(count))
			{
				   Status = ST_FAILURE;
				   break;
			}
		}
		/* To make sure back to idle screen will be free the history */
		{
			if ( historyData[count].guiBuffer )
				OslMfree(historyData[count].guiBuffer);
			historyData[count].guiBuffer = NULL;
			if ( historyData[count].inputBuffer )
				OslMfree(historyData[count].inputBuffer);
			historyData[count].inputBuffer = NULL;
		}
		count--;
	}while( ( historyData[count].entryFuncPtr != NULL )
							&& (count > 0)  );

	currHistoryIndex = count;
	PRINT_INFORMATION(("ExecuteRootMainHistoryScreen currHistoryIndex=%d\n", currHistoryIndex));

	if (Status ==ST_FAILURE)
	{
		/*  release itself first */
		{
			if ( historyData[count].guiBuffer )
				OslMfree(historyData[count].guiBuffer);
			historyData[count].guiBuffer = NULL;
			if ( historyData[count].inputBuffer )
				OslMfree(historyData[count].inputBuffer);
			historyData[count].inputBuffer = NULL;
		}
		/* entry itself */
		if (historyData[count].entryFuncPtr) 
		{
			(*(historyData[count].entryFuncPtr))();
		}
		/* remove one more up node */
		memset(&historyData[count], 0, sizeof(historyNode));
		decrement();
      		if((currHistoryIndex > MIN_HISTORY + 1) && (currHistoryIndex < MAX_HISTORY) )
      		{
      			--currHistoryIndex;
      		}

      		else if( (currHistoryIndex == 1) && ( bRoundOffFlag == 0))
      		{
      			currHistoryIndex=0;

        	}

      		else if(currHistoryIndex == 0)
      		{
      			currHistoryIndex = -1;
      			bRoundOffFlag = 0;
         	}
      		else if(bRoundOffFlag)
      		{
      			currHistoryIndex = MAX_HISTORY - 1;
      			bRoundOffFlag = 0;
      		}

	}
	else
	{
		if (historyData[currHistoryIndex].entryFuncPtr) 
		{
			(*(historyData[currHistoryIndex].entryFuncPtr))();
		}
	}
	
   return Status;
}


/************************************************************************
	FUNCTION NAME		: DinitHistory()

  	PURPOSE				: Free the history node

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void DinitHistory(void)
{

   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count = 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_DINIT_HIST_HDLR));
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	if(redrawing_old_screens)
	{
		is_dinit_called=1;
		return;
	}
#endif
	if(currHistoryIndex > 0)
	{
		for(count = currHistoryIndex ; count > 0; --count)
	{
		if ( historyData[count].guiBuffer )
		OslMfree(historyData[count].guiBuffer);
		if ( historyData[count].inputBuffer )
		OslMfree(historyData[count].inputBuffer);
			memset(&historyData[count], 0, sizeof(historyNode));

		}

	}

	if (historyData[0].guiBuffer)
	OslMfree(historyData[0].guiBuffer);
	if (historyData[0].inputBuffer)
	OslMfree(historyData[0].inputBuffer);
	memset(&historyData[0],0,sizeof(historyData));
	currHistoryIndex = -1;
}	
/**************************************************************

	FUNCTION NAME		: DeleteBetweenScreens

  	PURPOSE				: Deletes Between Screens including the boundary.

	INPUT PARAMETERS	: U16 StartScrId, U16 EndScrId

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/

U16 DeleteBetweenScreens(U16 StartScrId, U16 EndScrId)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	 U16 count = 0,count1 = 0,i = 0;
	 S16 endScreenPresent=-1, startScreenPresent=-1;
	 U16 screensDeleted=0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	 MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_DEL_BETWEEN_SCRS_HDLR, StartScrId, EndScrId));

	 if(currHistoryIndex > 0)
	 count = currHistoryIndex;
	 count1=currHistoryIndex;
	 
	 while(count)
	 {
		  if(historyData[count].scrnID==EndScrId)
		  {
		   endScreenPresent=count;
		   break;
		  }
	  --count;
	 }

	if(endScreenPresent >= 0)
	{
		 while(count)
		 {
			  if(historyData[count].scrnID==StartScrId)
			  {
			   startScreenPresent=count;
			   break;
			  }
			  --count;
		 }
	}
	else
		return ST_FAILURE;

	 if(endScreenPresent==-1 || startScreenPresent==-1) return ST_FAILURE;
	 for(count=startScreenPresent; count<=endScreenPresent; count++)
	 {
		  if ( historyData[count].guiBuffer )
		   OslMfree(historyData[count].guiBuffer);
		  if ( historyData[count].inputBuffer )
		   OslMfree(historyData[count].inputBuffer);
		  --currHistoryIndex;
		  screensDeleted++;
	 }
	i = endScreenPresent+1;
	memcpy(historyData+startScreenPresent,historyData+i, sizeof(historyNode)*((count1-endScreenPresent)));
/* to clear the history of unwanted entries
	i = count1-screensDeleted+1;
	memset(historyData+i,0,(sizeof(historyNode)*screensDeleted));
	*/
	return ST_SUCCESS;
}


/**************************************************************

	FUNCTION NAME		: SetDelScrnIDCallbackHandler

  	PURPOSE				: Register callback fucntion for delete screen occur.

	INPUT PARAMETERS	: U16 ScrnID, HistoryDelCBPtr funcPtr

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/

U8 SetDelScrnIDCallbackHandler(U16 ScrnID, HistoryDelCBPtr funcPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SET_SCREEN_ID_CB_HDLR, ScrnID, funcPtr));   

	//CSD added by JL for history callback
	for(count = 0; count < MAX_HISTORY; count++)
	{
		//add new one, while scrnID and historydelCBPtr is NULL, means not found we can add a new one.
		if ( (historyCBHandler[count].scrnID == 0) && (historyCBHandler[count].historydelCBPtr == NULL) )
		{
			historyCBHandler[count].scrnID = ScrnID;
			historyCBHandler[count].historydelCBPtr = funcPtr;
			return MMI_TRUE;
		}
		
		//already exist, to overwrite, while ScrnID and funcPtr are not NULL, and fount ScrnID.
/*
 
		if ( (historyCBHandler[count].scrnID == ScrnID) && (historyCBHandler[count].historydelCBPtr != NULL) )
*/
		if (historyCBHandler[count].scrnID == ScrnID)
		{
			historyCBHandler[count].historydelCBPtr = funcPtr;
			return MMI_TRUE;
		}
		
	}

	MMI_ASSERT(MMI_FALSE);

	return MMI_FALSE;

}

/**************************************************************

	FUNCTION NAME		: ClearDelScrnIDCallbackHandler

  	PURPOSE				: Clear callback fucntion for delete screen occur.

	INPUT PARAMETERS	: U16 ScrnID, HistoryDelCBPtr funcPtr

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/
static U8 ClearDelScrnIDCallbackHandler(U16 ScrnID, HistoryDelCBPtr funcPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count, found_idx;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_CLR_SCREEN_ID_CB_HDLR, ScrnID, funcPtr));   
   
	//CSD added by JL for history callback
	for(count = 0; count < MAX_HISTORY; count++)
	{
		//found the delete one.
		if (historyCBHandler[count].scrnID == ScrnID) 
		{
			break;
		}
	}
	
	for (found_idx=count; found_idx<MAX_HISTORY-1; found_idx++)
	{
		historyCBHandler[found_idx].scrnID = historyCBHandler[found_idx+1].scrnID;
		historyCBHandler[found_idx].historydelCBPtr = historyCBHandler[found_idx+1].historydelCBPtr;
	}
	// set the last one.
	historyCBHandler[MAX_HISTORY-1].scrnID = 0;
	historyCBHandler[MAX_HISTORY-1].historydelCBPtr = NULL;
	
	return 0;	
}

U8 ClearDelScrnIDCallbackHandlerEx(U16 ScrnID, HistoryDelCBPtr funcPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count, found_idx;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_CLR_SCREEN_ID_CB_HDLR, ScrnID, funcPtr));   
   
	//CSD added by JL for history callback
	for(count = 0; count < MAX_HISTORY; count++)
	{
		//found the delete one.
		if (historyCBHandler[count].scrnID == ScrnID) 
		{
			break;
		}
	}
	
	for (found_idx=count; found_idx<MAX_HISTORY-1; found_idx++)
	{
		historyCBHandler[found_idx].scrnID = historyCBHandler[found_idx+1].scrnID;
		historyCBHandler[found_idx].historydelCBPtr = historyCBHandler[found_idx+1].historydelCBPtr;
	}
	// set the last one.
	historyCBHandler[MAX_HISTORY-1].scrnID = 0;
	historyCBHandler[MAX_HISTORY-1].historydelCBPtr = NULL;
	
	return 0;	
}

/**************************************************************

	FUNCTION NAME		: SearchDelScrnIDCallbackHandler

  	PURPOSE				: Search index of the delete callback screen.

	INPUT PARAMETERS	: U16 ScrnID, U16 *found_idx

	OUTPUT PARAMETERS	: found or not.

	RETURNS				: U8 - status

 

**************************************************************/
static U8 SearchDelScrnIDCallbackHandler(U16 ScrnID, U16 *found_idx)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for(count = 0; count < MAX_HISTORY; count++)
	{
		//found the delete one.
		if (historyCBHandler[count].scrnID == ScrnID) 
		{
   	   		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SRCH_DEL_SCRN_CB_HDLR, ScrnID, count));   
   	   		
			*found_idx = count;
			return MMI_TRUE;
		}
	}
	
	return MMI_FALSE;
	
}

/**************************************************************

	FUNCTION NAME		: ExecTopScrnCallbackHandler

  	PURPOSE				: Check if topist view screen exist in delete screen will be callback.

	INPUT PARAMETERS	: U16 ScrnID, U16 *found_idx

	OUTPUT PARAMETERS	: found or not.

	RETURNS				: U8 - status

 

**************************************************************/
static U8 ExecTopScrnCallbackHandler(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U8 is_stop_delete = MMI_FALSE;
   U16 cb_history_idx;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   if (SearchDelScrnIDCallbackHandler(GetExitScrnID(), &cb_history_idx))
   {
   	   if (historyCBHandler[cb_history_idx].historydelCBPtr)
  	   	 is_stop_delete = historyCBHandler[cb_history_idx].historydelCBPtr((void*)MMI_TRUE);

   	   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EXEC_TOP_SCRN_CB_HDLR, is_stop_delete));   
   	   
	   ClearDelScrnIDCallbackHandler(historyCBHandler[cb_history_idx].scrnID, NULL);
   }

   
   return is_stop_delete;
}

static U8 ExecHistoryScrnCallBackHnadle(U16 HistoryIndex)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
      // Not support for stop callback at this funciton
       U16 cb_history_idx;
       U8 is_stop_allow = MMI_FALSE;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   
	if (SearchDelScrnIDCallbackHandler(historyData[HistoryIndex].scrnID, &cb_history_idx))
	{
	   if (historyCBHandler[cb_history_idx].historydelCBPtr)
		  is_stop_allow = historyCBHandler[cb_history_idx].historydelCBPtr((void*)MMI_FALSE);
	   ClearDelScrnIDCallbackHandler(historyCBHandler[cb_history_idx].scrnID, NULL);

	}
	
	if (is_stop_allow==MMI_TRUE)
	{
	     MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_EXEC_HIST_SCRN_CB_HDLR, HistoryIndex));   
	}
     
	return is_stop_allow;
}
/**************************************************************

	FUNCTION NAME		: set_small_screen

  	PURPOSE			: Called by category to tell history that the screen being drawn is small.

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS			: none

 

**************************************************************/

void set_small_screen(void)
{
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__	
	 
	if(no_small_screen) return;
	 
	if(!redrawing_old_screens)
	{
		if(is_draw_prev_screen_on_set_small_screen)
		{
			//gdi_layer_lock_frame_buffer();
			if(currHistoryIndex==-1)
			{
				clear_screen();
			}
			else
			{
				//PreRedrawCategoryFunction();
				clear_screen();

			}
			//gdi_layer_unlock_frame_buffer();
			
		}
		if(currHistoryIndex==-1)
			clear_screen();
	}
	is_draw_prev_screen_on_set_small_screen=0;
	small_history_node=1;
#endif
}

/*****************************************************************************
 * FUNCTION
 *  mmi_is_redrawing_bk_screens
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
S32 mmi_is_redrawing_bk_screens()
{
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mmi_execute_scrn_exit_func())
    {
        if (redrawing_old_screens == MMI_HIST_SMALLSCREEN_ACTIVE ||
            redrawing_old_screens == MMI_HIST_SMALLSCREEN_ACTIVE_IN_EXIT_FUNC)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return (redrawing_old_screens == MMI_HIST_SMALLSCREEN_ACTIVE) ? 1 : 0;
    }
#else /* __MMI_UI_SMALL_SCREEN_SUPPORT__ */ 
    return 0;
#endif /* __MMI_UI_SMALL_SCREEN_SUPPORT__ */ 
}


 
void enable_check_for_small_screen(void)
{
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__	
	check_for_small_history_node=1;
#endif
}

void disable_check_for_small_screen(void)
{
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__	
	check_for_small_history_node=0;
#endif
}
 
/**************************************************************

	FUNCTION NAME		: reset_small_screen

  	PURPOSE			: Called by category to tell history that it is exiting from small screen.

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS			: none

 

**************************************************************/
void reset_small_screen(void)
{
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__	
	small_history_node=0;
 
	no_small_screen=0;
 
#endif
}

 
/**************************************************************

	FUNCTION NAME		: force_no_small_screen

  	PURPOSE			: Called by Application to disable small screen history saving.

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS			: none

 

**************************************************************/
void force_no_small_screen(void)
{
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__
	no_small_screen=1;
#endif
}
 
#ifdef __MMI_UI_TAB_PANE__
/************************************************************************
	FUNCTION NAME		: disable_history_saving

  	PURPOSE				: Disable history saving

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void disable_history_saving(void)
{
	disable_history_save=1;
}
/************************************************************************
	FUNCTION NAME		: enable_history_saving

  	PURPOSE				: Enables history saving

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void enable_history_saving(void)
{
	disable_history_save=0;
}
/************************************************************************
	FUNCTION NAME		: change_curr_gui_buffer

  	PURPOSE				: Changes the current gui buffer of the current screen

	INPUT PARAMETERS	: nil

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void change_curr_gui_buffer(U8 *gui_buffer)
{
	memcpy(historyData[currHistoryIndex].guiBuffer, gui_buffer,MAX_GUI_BUFFER);		
}
#endif

static U8 ExecHistoryScrnCallBackHandle(U16 HistoryIndex)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
      // Not support for stop callback at this funciton
       U16 cb_history_idx;
       U8 is_stop_allow = MMI_FALSE;
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   
	if (SearchDelScrnIDCallbackHandler(historyData[HistoryIndex].scrnID, &cb_history_idx))
	{
	   if (historyCBHandler[cb_history_idx].historydelCBPtr)
		  is_stop_allow = historyCBHandler[cb_history_idx].historydelCBPtr((void*)MMI_FALSE);
	   ClearDelScrnIDCallbackHandler(historyCBHandler[cb_history_idx].scrnID, NULL);

	}
     
	return is_stop_allow;
}

/**************************************************************

	FUNCTION NAME		: ExecuteRootHistoryScreen()

  	PURPOSE				: To execute the root history screen.

	INPUT PARAMETERS	: funcPtr: reserve not use

	OUTPUT PARAMETERS	: U8

	RETURNS				: U8

 

**************************************************************/
U8 FreeHistory(void *funcPtr)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
 	U8 Status = ST_SUCCESS;
    S16 count = currHistoryIndex;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	/*reserve not decrement: Memory is not being released bcos of one particular case of keypad lock*/

	//To make sure the callback will run at topist screen, and callback for APP
	ExecTopScrnCallbackHandler();

	do
	{
		if( count == 0 )
		{
				Status = ST_SUCCESS;
				break;
		}		
		if(count > 0 ) 
		{			
			if (ExecHistoryScrnCallBackHandle(count))
			{
				   Status = ST_FAILURE;
				   break;
			}
		}
		/* To make sure back to idle screen will be free the history */
		{
			if ( historyData[count].guiBuffer )
				OslMfree(historyData[count].guiBuffer);
			historyData[count].guiBuffer = NULL;
			if ( historyData[count].inputBuffer )
				OslMfree(historyData[count].inputBuffer);
			historyData[count].inputBuffer = NULL;
		}
		count--;
	}while( ( historyData[count].entryFuncPtr != NULL )
							&& (count > 0)  );

	currHistoryIndex = count;
	PRINT_INFORMATION(("ExecuteRootMainHistoryScreen currHistoryIndex=%d\n", currHistoryIndex));

	if (Status ==ST_FAILURE)
	{
		/*  release itself first */
		{
			if ( historyData[count].guiBuffer )
				OslMfree(historyData[count].guiBuffer);
			historyData[count].guiBuffer = NULL;
			if ( historyData[count].inputBuffer )
				OslMfree(historyData[count].inputBuffer);
			historyData[count].inputBuffer = NULL;
		}
		/* entry itself */
		if (historyData[count].entryFuncPtr) 
		{
			(*(historyData[count].entryFuncPtr))();
		}
		/* remove one more up node */
		memset(&historyData[count], 0, sizeof(historyNode));
		decrement();
      		if((currHistoryIndex > MIN_HISTORY + 1) && (currHistoryIndex < MAX_HISTORY) )
      		{
      			--currHistoryIndex;
      		}

      		else if( (currHistoryIndex == 1) && ( bRoundOffFlag == 0))
      		{
      			currHistoryIndex=0;

        	}

      		else if(currHistoryIndex == 0)
      		{
      			currHistoryIndex = -1;
      			bRoundOffFlag = 0;
         	}
      		else if(bRoundOffFlag)
      		{
      			currHistoryIndex = MAX_HISTORY - 1;
      			bRoundOffFlag = 0;
      		}

	}
		
   return Status;
}

