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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * SubLCDHistory.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for sublcd history control
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
/**************************************************************

	FILENAME	: SubLCDHistory.c

	PURPOSE		: Implementation of Sub-LCD history stack

 

	AUTHOR		: Deepali

	DATE		: Sept 23, 2003

**************************************************************/

#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "debuginitdef.h"
#include "frameworkstruct.h"
#include "historygprot.h"
#include "sublcdhistorydef.h"
#include "sublcdhistorygprot.h"
#undef __NEWSIMULATOR 
#include "l4dr.h"

/* 
**   Extern Varibales 
*/

/* 
**   Local Variables 
*/
#ifdef __MMI_SUBLCD__
static SubLCDHistoryNode SubLCDHistoryData[MAX_SUBLCD_HISTORY];						/* array of history data */
static pBOOL IsBackSubLCDHistory=FALSE;
static signed char NodeIndex= -1;
static FuncPtr gCurrExitSubLCDFn=NULL;
static FuncPtr gCurrEntrySubLCDFn=NULL;
static SubLcdHistoryCBPtr ExecuteRootSubLcdHistoryScreenPtr=NULL;
#endif

/**************************************************************

	FUNCTION NAME		: ExecSubLCDCurrExitHandler()

  	PURPOSE				: Execute the Current Exit handler of SubLCD

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/

void ExecSubLCDCurrExitHandler(void)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_EXE_SUBLCD_CUR_EXIT_HDLR));

	if((gCurrExitSubLCDFn!=NULL)&&(IsBackSubLCDHistory==FALSE))
	{
		gCurrExitSubLCDFn();
		gCurrExitSubLCDFn=NULL;
	}
	IsBackSubLCDHistory=FALSE;
#endif
}

/**************************************************************

	FUNCTION NAME		: AddSubLCDHistory()

  	PURPOSE				: Saves Sublcd function in History

	INPUT PARAMETERS	: SubLCDHistoryNode *CurrSubLCDHistory

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/

void AddSubLCDHistory(SubLCDHistoryNode *CurrSubLCDHistory)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_ADD_SUBLCD_HIST_HDLR));

	//CSD added by JL for back history.
   if(IsBackSubLCDHistory != TRUE) 
	{
		FuncPtr CurrSubLCDEntryFn=CurrSubLCDHistory->entryFuncPtr;
		if(CurrSubLCDEntryFn!=NULL)
		{
			SubLCDHistoryData[++NodeIndex].entryFuncPtr=CurrSubLCDEntryFn;
			SubLCDHistoryData[NodeIndex].scrnID=0;
		}
	}
   IsBackSubLCDHistory = FALSE;
#endif
}

/**************************************************************

	FUNCTION NAME		: AddSubLCDHistoryWithScrID()

  	PURPOSE				: Saves Sublcd function in History with ScrID

	INPUT PARAMETERS	: SubLCDHistoryNode *CurrSubLCDHistory, U16 scrID

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/

void AddSubLCDHistoryWithScrID(SubLCDHistoryNode *CurrSubLCDHistory,U16 scrID)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_ADD_SUBLCD_HIST_WITHSCRID_HDLR));

	//CSD added by JL for back history.
   if(IsBackSubLCDHistory != TRUE) 
	{
		FuncPtr CurrSubLCDEntryFn=CurrSubLCDHistory->entryFuncPtr;
		if(CurrSubLCDEntryFn!=NULL)
		{
			SubLCDHistoryData[++NodeIndex].entryFuncPtr=CurrSubLCDEntryFn;
			SubLCDHistoryData[NodeIndex].scrnID=scrID;
		}
	}
   IsBackSubLCDHistory = FALSE;
#endif
}
/**************************************************************

	FUNCTION NAME		: SetSubLCDExitHandler()

  	PURPOSE				: Set Exit handler for Current screen

	INPUT PARAMETERS	: FuncPtr CurrSubLCDEntryFn

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/

void SetSubLCDExitHandler(FuncPtr CurrSubLCDEntryFn)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_ADD_SUBLCD_EXIT_HDLR));

	gCurrExitSubLCDFn=CurrSubLCDEntryFn;
#endif
}

/**************************************************************

	FUNCTION NAME		: GoBackSubLCDHistory()

  	PURPOSE				: Go back to history On SubLCD Screen

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/

void GoBackSubLCDHistory(void)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_GO_BACK_SUBLCD_HDLR));

	if(NodeIndex >= 0)
	{
		IsBackSubLCDHistory=TRUE;
		if ( SubLCDHistoryData[NodeIndex].entryFuncPtr )
			SubLCDHistoryData[NodeIndex].entryFuncPtr();
		DecrementSubLCD();
	}
	else
	{
		//SubLCDIdleScreen();
		if (ExecuteRootSubLcdHistoryScreenPtr!=NULL)
			ExecuteRootSubLcdHistoryScreenPtr(NULL);
	}
#endif
}

/**************************************************************

	FUNCTION NAME		: DecrementSubLCD()

  	PURPOSE				: decrement the global history index of sub-LCD

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/
void DecrementSubLCD(void)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	memset(&SubLCDHistoryData[NodeIndex],0,sizeof(SubLCDHistoryNode));	
 
	if ( NodeIndex>0 )
		NodeIndex--;
#endif
}

/**************************************************************

	FUNCTION NAME		: DinitSubLCDHistory()

  	PURPOSE				: De-intialize SubLCD Screen history

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/
void InitSubLCDHistory(SubLcdHistoryCBPtr sublcd_root_screen_cb_ptr)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    ExecuteRootSubLcdHistoryScreenPtr = sublcd_root_screen_cb_ptr ;
#endif

}


/**************************************************************

	FUNCTION NAME		: DinitSubLCDHistory()

  	PURPOSE				: De-intialize SubLCD Screen history

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/
void DinitSubLCDHistory(void)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	signed char count;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__ 
	if(is_redrawing_old_screens())
	   return;
#endif
	if(	NodeIndex>-1)
	{
		for(count=NodeIndex;count>=0;count--)
		{
			SubLCDHistoryData[count].entryFuncPtr=NULL;
		}
		NodeIndex=-1;
	}
#endif
}

/**************************************************************

	FUNCTION NAME		: ShowSubLCDScreen()

  	PURPOSE				: Calls entry function of the specified screen on the
						  sub-LCD screen

	INPUT PARAMETERS	: FuncPtr SubLCDEntryFn

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/

void ShowSubLCDScreen(FuncPtr SubLCDEntryFn)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_UI_SMALL_SCREEN_SUPPORT__ 
   if(is_redrawing_old_screens())
	   return;
#endif
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_SHOW_SUBLCD_HDLR));

	if(!IsMainLCDHistoryBack())
	{
		SubLCDEntryFn();
		gCurrEntrySubLCDFn=SubLCDEntryFn;
	}
#endif
}

/**************************************************************

	FUNCTION NAME		: ForceSubLCDScreen()

  	PURPOSE				: force to call entry function of a screen SubLCD Screen

	INPUT PARAMETERS	: FuncPtr SubLCDEntryFn

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/

void ForceSubLCDScreen(FuncPtr SubLCDEntryFn)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
 
#if 0 
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
 
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_FORCE_SUBLCD_HDLR));

	SubLCDEntryFn();
	gCurrEntrySubLCDFn=SubLCDEntryFn;
#endif
}


 
#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#ifdef __MMI_SUBLCD__
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
 
/**************************************************************

	FUNCTION NAME		: GoBackBothLCDHistory()

  	PURPOSE				: Go back to history on Main LCD & SubLCD Screen

	INPUT PARAMETERS	: none

	OUTPUT PARAMETERS	: none

	RETURNS				: void

 

**************************************************************/

void GoBackBothLCDHistory(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

#ifdef __MMI_SUBLCD__
	GoBackSubLCDHistory();
#endif
	GoBackHistory();
//CSD end
}


/**************************************************************

	FUNCTION NAME		: GoBackToSubLCDHistory( )

  	PURPOSE				: Goes back to specified screen on sub-LCD 

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/
U8 GoBackToSubLCDHistory(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/

#ifdef __MMI_SUBLCD__
	S16 count = 0;
#endif
	U8 Status = ST_FAILURE;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD__
	
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_GOBACK_SUBLCD_HIST_HDLR, scrnid));

	count = NodeIndex;

	do
	{
		if( SubLCDHistoryData[NodeIndex].scrnID  == scrnid )
		{
				Status = ST_SUCCESS;
				break;
		}		
		
		if(NodeIndex > 0 ) 
		{			
			DecrementSubLCD();
		}

	}while( (count != NodeIndex ) && ( SubLCDHistoryData[NodeIndex].entryFuncPtr != NULL )
							&& (NodeIndex > 0)  );

	if(Status)
	{
		GoBackSubLCDHistory();
	}

	else if ( NodeIndex > -1)
	{
		(*(SubLCDHistoryData[0].entryFuncPtr))();
		NodeIndex = -1;
	}

#endif

	return Status;	

}

/**************************************************************

 

	 
						  
	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/
U8 GoBeyondSubLCDMarkerScr(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD__
	S16 count = 0;
#endif
	U8 Status = ST_FAILURE;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	
#ifdef __MMI_SUBLCD__
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_GO_BEYOND_SUBLCD_MARKER_SCR_HDLR, scrnid));

	count = NodeIndex;

	if(!IsSubLCDScreenPresent(scrnid))
	{
		return ST_FAILURE;
	}

	do
	{
		if( SubLCDHistoryData[NodeIndex].scrnID  == scrnid )
		{
			

			Status = ST_SUCCESS;
			break;
		}		
		DecrementSubLCD();
	
	}while( (count != NodeIndex ) && ( SubLCDHistoryData[NodeIndex].entryFuncPtr != NULL ) );

	if(Status)
	{
		DecrementSubLCD();
		GoBackSubLCDHistory();
	}

#endif	

	return Status;

}

/**************************************************************

	FUNCTION NAME		: DeleteUptoSubLCDScrID()

  	PURPOSE				: Go to a screen on sub-LCD and delete history of all ahead of it 	

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/
U8 DeleteUptoSubLCDScrID(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD__
	S16 count = 0;
#endif
	U8 Status = ST_FAILURE;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD__
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_DEL_UPTO_SUBLCD_SCR_HDLR, scrnid));

	count = NodeIndex;
	if(!IsSubLCDScreenPresent(scrnid))
	{
		return ST_FAILURE;
	}

	do
	{
		if( SubLCDHistoryData[NodeIndex ].scrnID  == scrnid )
		{
			
			Status = ST_SUCCESS;
			break;
		}		
		DecrementSubLCD();
	
	}while( (count != NodeIndex ) && ( SubLCDHistoryData[NodeIndex].entryFuncPtr != NULL ) );

#endif

	return Status;

}

/**************************************************************

	FUNCTION NAME		: DeleteNSubLCDScrId()

  	PURPOSE				: Deletes up to screen id on sub-LCD and go  beyond it one node
						  

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: U8 - status

 

**************************************************************/
U8 DeleteNSubLCDScrId(U16 scrnid)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
#ifdef __MMI_SUBLCD__
	S16 count = 0;
#endif
	U8 Status = ST_FAILURE;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/


#ifdef __MMI_SUBLCD__
	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_DEL_N_SUBLCD_SCR_HDLR, scrnid));

	count = NodeIndex;

	if(!IsSubLCDScreenPresent(scrnid))
	{
		return ST_FAILURE;
	}

	do
	{
		if( SubLCDHistoryData[NodeIndex].scrnID  == scrnid )
		{
			Status = ST_SUCCESS;
			break;
		}		
		DecrementSubLCD();
	
	}while( (count != NodeIndex ) && ( SubLCDHistoryData[NodeIndex].entryFuncPtr != NULL ) );

	if(Status)
	{
		DecrementSubLCD();
	}
#endif
	return Status;
}

/**************************************************************

	FUNCTION NAME		: DeleteSubLCDScreen()

  	PURPOSE				: Deletes Screen from sub-LCD history

	INPUT PARAMETERS	: U16 ScrId

	OUTPUT PARAMETERS	: nil

	RETURNS				: U16 - status

 

**************************************************************/
U16 DeleteSubLCDScreen(U16 ScrId)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	U16 count,i;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_DEL_SUBLCD_SCR_HDLR, ScrId));

	if(!IsSubLCDScreenPresent(ScrId))
	{
		return ST_FAILURE;
	}
	
	count = NodeIndex;
	
	// Locating the screen
	while(count)
	{
		if(SubLCDHistoryData[count].scrnID==ScrId)
		{

			break;
		}
		--count;
	}

	// Shifting All the nodes by one notch
	for(i=count+1;i<=NodeIndex;i++,count++)
	{
		memcpy(&SubLCDHistoryData[count],&SubLCDHistoryData[i], sizeof(SubLCDHistoryNode));	
	}
	
	--NodeIndex;
	memset(&SubLCDHistoryData[NodeIndex+1], 0, sizeof(SubLCDHistoryNode));
#endif
	return ST_SUCCESS;
}

/**************************************************************

	FUNCTION NAME		: IsSubLCDScreenPresent()

  	PURPOSE				: Queries history for presence of a screen Id in sub-LCD history						  

	INPUT PARAMETERS	: U16 scrnid

	OUTPUT PARAMETERS	: nil

	RETURNS				: pBOOL

 

**************************************************************/
pBOOL IsSubLCDScreenPresent(U16 scrnId)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
	for (count = 0; ((count < MAX_SUBLCD_HISTORY)&&((count <= NodeIndex) && (NodeIndex != -1))); count++) {
		if(SubLCDHistoryData[count].scrnID  == scrnId )
			return TRUE;
	}
#endif

	return FALSE;
}


/**************************************************************

	FUNCTION NAME		: GetCurrSubLCDScreenFunc()

  	PURPOSE				: Queries present screen exit function						  

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: FuncPtr

 

**************************************************************/

FuncPtr GetCurrSubLCDScreenFunc(void)
{
#ifdef __MMI_SUBLCD__
    return gCurrExitSubLCDFn;
#endif
	return 0;
}



/**************************************************************

	FUNCTION NAME		: GetCurrSubLCDScreenEntryFunc()

  	PURPOSE				: Queries current screen entry function						  

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: FuncPtr

 

**************************************************************/

FuncPtr GetCurrSubLCDScreenEntryFunc(void)
{
#ifdef __MMI_SUBLCD__
    return gCurrEntrySubLCDFn;
#endif
	return 0;
}




/*************************************************************************

	FUNCTION NAME		: SubLCDHistoryDump(void)

  	PURPOSE				: Dumps the Sub-LCD history to file or debug window

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************************/
void SubLCDHistoryDump(void)
{
#ifdef __MMI_SUBLCD__
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
	S16 count= 0;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

	if (NodeIndex >= 0)
	{
	   count = NodeIndex;

   	do
   	{
   		MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_DUMP_HDLR, 
   		           count, SubLCDHistoryData[count].scrnID, SubLCDHistoryData[count].entryFuncPtr));
   
   	    if(count > 0  && count < MAX_SUBLCD_HISTORY )
   		{
   			--count;
   		}
   		else 
   		{
				break;
   		}
   	
   	}while((count != NodeIndex) && (SubLCDHistoryData[count].entryFuncPtr != NULL) );
   }
   
   MMI_TRACE( (MMI_TRACE_G1_FRM, MMI_FRM_INFO_SUBHIST_DUMP_ROOTHDLR));

#endif
}



