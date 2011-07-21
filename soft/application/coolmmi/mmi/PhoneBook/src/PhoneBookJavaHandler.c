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
 * PhoneBookHandlerJava.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for providing phonebook interface for java package
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
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _MMI_PHONEBOOKJAVAHANDLER_C
#define _MMI_PHONEBOOKJAVAHANDLER_C

#if defined(__J2ME__)

/*  Include: MMI header file */
#define __NEWSIMULATOR 
#include "mmi_features.h" 
#include "stdc.h"
#include "l4dr1.h"
#include "mmi_data_types.h"
#include "globaldefs.h"
#include "commonscreens.h"
#include "phonebooktypes.h"
#include "phonebookdef.h"
#include "phonebookprot.h"
#include "phonebookgprot.h"
#include "phonebookjavahandler.h"
#include "settingdefs.h" /*for fdn on check*/
#include "settingsgdcl.h"
#include "nvram_user_defs.h"
#include "nvramprot.h"
#include "simdetectiondef.h"
#include "nvram_data_items.h"

#undef __NEWSIMULATOR 

/* 
** Typedef 
*/

/* 
** Define
*/

/* 
** Local Variable
*/
static U8 g_phb_java_storage;
static U8 g_phb_java_sort;

#if defined(__MMI_PHB_EMAIL_SORT__)
extern U8 g_phb_email_sorted;
#endif

/* 
** Local Function
*/

/* 
** Global Variable
*/
extern U16 PhoneBookEntryCount;
extern U8 g_phb_enter_from;
extern U16 g_phb_name_index[];

extern S8 pbName[];
extern S8 pbNumber[];
extern S8 pbHomeNumber[];
extern S8 pbOfficeNumber[];
extern S8 pbFaxNumber[];
extern S8 pbEmailAddress[];

extern MMI_PHB_ENTRY_BCD_STRUCT		PhoneBook[];

#if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
extern PHB_OPTIONAL_FIELDS_STRUCT	PhoneBookOptionalFields;
#endif

/* 
** Global Function
*/
#define MMI_PHB_JAVA_HANDLER
/*****************************************************************************
* FUNCTION
*  mmi_phb_handler_init
* DESCRIPTION
*   This function initializes phonebook handler app.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_handler_init(void)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   SetProtocolEventHandler(mmi_phb_java_write_req		,MSG_ID_MMI_PHB_JAVA_SET_ENTRY_REQ);
   SetProtocolEventHandler(mmi_phb_java_read_req		,MSG_ID_MMI_PHB_JAVA_GET_ENTRY_REQ);
   SetProtocolEventHandler(mmi_phb_java_search_req	,MSG_ID_MMI_PHB_JAVA_SEARCH_ENTRY_REQ);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_write_req
* DESCRIPTION
*   This function write a phonebook entry from JAVA request
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_write_req(void* info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   mmi_phb_handler_set_entry_req_struct* LocalPtr;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   LocalPtr = (mmi_phb_handler_set_entry_req_struct*)info;

   if(!g_phb_cntx.phb_ready || g_phb_cntx.processing )
   {
      mmi_phb_java_write_rsp(MMI_PHB_JAVA_NOT_READY, 0xffff);
      return;
   }

   g_phb_java_storage = LocalPtr->storage;
   g_phb_java_sort = LocalPtr->sort_type;
   
   /*
   * This is an [Add] operation.
   */
   if(LocalPtr->index == 0xffff)
   {
      /*
	*  Check if name and number are both empty
	*/
	if(!pfnUnicodeStrlen(LocalPtr->entry_ptr->name) && !pfnUnicodeStrlen(LocalPtr->entry_ptr->number))
	{
         mmi_phb_java_write_rsp(MMI_PHB_JAVA_ERROR, 0xffff);
         return;
	}
	
      /*
      * Can support Email sorting in NVRAM only
      */
#if defined(__MMI_PHB_EMAIL_SORT__)      
      if((g_phb_java_sort == MMI_PHB_BY_EMAIL) && (g_phb_java_storage != MMI_NVRAM))
      	mmi_phb_java_write_rsp(MMI_PHB_JAVA_NOT_SUPPORT, 0xffff);
      /*Email sorting not populated after bootup.*/
      else if(g_phb_java_sort == MMI_PHB_BY_EMAIL && g_phb_email_sorted == 0)
      {
      	mmi_phb_java_util_rebuild_email_sorting();
      	g_phb_email_sorted = 1;
      }
#else
      if(g_phb_java_sort == MMI_PHB_BY_EMAIL)
      	mmi_phb_java_write_rsp(MMI_PHB_JAVA_NOT_SUPPORT, 0xffff);
#endif

	 /*
	 * Check free space and assign storage location
	 */
      if((LocalPtr->storage == MMI_STORAGE_BOTH) && (PhoneBookEntryCount < MAX_PB_ENTRIES))
      {
         if(g_phb_cntx.phone_used < g_phb_cntx.phone_total)	/*Save to NVRAM first*/
            g_phb_cntx.selected_storage = MMI_NVRAM;
         else
            g_phb_cntx.selected_storage = MMI_SIM;	
      }
      else if(((LocalPtr->storage == MMI_SIM) && (g_phb_cntx.sim_used < g_phb_cntx.sim_total)) || \
                 ((LocalPtr->storage == MMI_NVRAM) && (g_phb_cntx.phone_used < g_phb_cntx.phone_total)))
         g_phb_cntx.selected_storage = LocalPtr->storage;
      else
      {
         mmi_phb_java_write_rsp(MMI_PHB_JAVA_STORAGE_FULL, 0xffff);
         return;
      }

      /*
      * Prepare data for name and number field
      */
      mmi_phb_util_clear_buffer(TRUE);
      memcpy(pbName, LocalPtr->entry_ptr->name, (MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH);
      memcpy(pbNumber, LocalPtr->entry_ptr->number,(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);

      /*
      * Prepare data for optional fields
      */
      memcpy(pbHomeNumber, LocalPtr->entry_ptr->homeNumber,(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);
      memcpy(pbOfficeNumber, LocalPtr->entry_ptr->officeNumber,(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);
      memcpy(pbFaxNumber, LocalPtr->entry_ptr->faxNumber,(MAX_PB_NUMBER_LENGTH+1+1)*ENCODING_LENGTH);
      memcpy(pbEmailAddress, LocalPtr->entry_ptr->emailAddress,(MAX_PB_EMAIL_LENGTH+1)*ENCODING_LENGTH);

      /*
      * prepare data for optional id
      */
      g_phb_cntx.selected_pic_index = 0;
      g_phb_cntx.selected_ring_index = 0;
      g_phb_cntx.selected_grp_index = 0;
      g_phb_cntx.image_location = MMI_PHB_IMAGE_NO_SELECT;

      g_phb_enter_from = MMI_PHB_ENTER_FROM_JAVA;
      mmi_phb_op_add_entry_req();
   }
   /*
   * Support [Delete] here. Implement later.
   */
   else if(LocalPtr->entry_ptr == NULL)
   {
      mmi_phb_java_write_rsp(MMI_PHB_JAVA_NOT_SUPPORT, 0xffff);   
   }
   /*
   * Support [Update] here. Implement later.
   */   
   else
   {
      mmi_phb_java_write_rsp(MMI_PHB_JAVA_NOT_SUPPORT, 0xffff);
   }
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_write_rsp
* DESCRIPTION
*   This function return write result to JAVA
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_write_rsp(U16 result, U16 store_index)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   mmi_phb_handler_set_entry_rsp_struct *MsgPtr;
   MYQUEUE Message;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   MsgPtr = (mmi_phb_handler_set_entry_rsp_struct*) OslConstructDataPtr(sizeof(mmi_phb_handler_set_entry_rsp_struct));
   MsgPtr->index = 0xffff;
   
   /*Decide error cause*/
   if(( MsgPtr->result = result) == MMI_PHB_JAVA_SUCCESS)
      MsgPtr->index = mmi_phb_java_util_get_index_by_store_index(g_phb_java_sort, g_phb_java_storage, store_index);
   else if(( MsgPtr->result = result) == MMI_PHB_JAVA_NOT_SUPPORT)
      /*Do nothing*/;
   else if(( MsgPtr->result = result) == MMI_PHB_JAVA_STORAGE_FULL)
      /*Do nothing*/;
   else if(( MsgPtr->result = result) == MMI_PHB_JAVA_NOT_READY)
      /*Do nothing*/;
   else if (result == MMI_SIM_EF_RECORD_FULL)
      MsgPtr->result = MMI_PHB_JAVA_NUMBER_TOO_LONG;
   else if(g_SecuritySetupContext.FdlStatus[0])
      MsgPtr->result = MMI_PHB_JAVA_FDN_ON;
   else
      MsgPtr->result = MMI_PHB_JAVA_ERROR;

   Message.oslSrcId=MOD_MMI;
   Message.oslDestId=MOD_JASYN;
   Message.oslMsgId = MSG_ID_MMI_PHB_JAVA_SET_ENTRY_RSP;
   Message.oslDataPtr = (oslParaType *)MsgPtr;
   Message.oslPeerBuffPtr= NULL;

   OslMsgSendExtQueue(&Message);
   
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_read_req
* DESCRIPTION
*   This function initializes phonebook handler app.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_read_req(void* info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U16 pos;
   U16 count = 0;
   U16 store_index = 0;
   mmi_phb_handler_get_entry_req_struct* LocalPtr;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   LocalPtr = (mmi_phb_handler_get_entry_req_struct*)info;

   if(!g_phb_cntx.phb_ready || g_phb_cntx.processing )
   {
      mmi_phb_java_read_rsp(MMI_PHB_JAVA_NOT_READY, 0);
      return;
   }

   /*
   * Get entry data
   */
   if((LocalPtr->count > 0) && (LocalPtr->entry_ptr != NULL))
   {
      /*
      *  Check valid index first.
      */
      if( ((LocalPtr->storage == MMI_STORAGE_BOTH) && (LocalPtr->index > PhoneBookEntryCount)) || \
           ((LocalPtr->storage == MMI_NVRAM) && (LocalPtr->index > g_phb_cntx.phone_used)) || \
           ((LocalPtr->storage == MMI_SIM) && (LocalPtr->index > g_phb_cntx.sim_used)))
      {
         mmi_phb_java_read_rsp(MMI_PHB_JAVA_OUT_OF_INDEX, 0);
         return;
      }

      /*
      *  Sort By NAME
      */
      if(LocalPtr->sort_type == MMI_PHB_BY_NAME)
      {
         /*Find index position*/
         for(pos = 0 ; pos < PhoneBookEntryCount ; pos++)
         {
            store_index = g_phb_name_index[pos];
            
            if((LocalPtr->storage == MMI_STORAGE_BOTH) || \
            	(LocalPtr->storage == MMI_NVRAM && store_index < MAX_PB_PHONE_ENTRIES) || \
            	(LocalPtr->storage == MMI_SIM && store_index >= MAX_PB_PHONE_ENTRIES))
            	count++;

            if(count == LocalPtr->index)
            	break;
         }
         /*Copy entry by assigned count number*/
         count = 0;
         mmi_phb_java_util_copy_entry_from_phb(store_index, &LocalPtr->entry_ptr[count++]);

         while((++pos < PhoneBookEntryCount) && (count < LocalPtr->count))
         {
            store_index = g_phb_name_index[pos];
            
            if((LocalPtr->storage == MMI_STORAGE_BOTH) || \
            	(LocalPtr->storage == MMI_NVRAM && store_index < MAX_PB_PHONE_ENTRIES) || \
            	(LocalPtr->storage == MMI_SIM && store_index >= MAX_PB_PHONE_ENTRIES))
            {
                 mmi_phb_java_util_copy_entry_from_phb(store_index, &LocalPtr->entry_ptr[count++]);
            }
         }
         
         mmi_phb_java_read_rsp(MMI_PHB_JAVA_SUCCESS, count);
         return;
      }
      /*
      * Sort By Email, only support email sorting in NVRAM
      */
#if defined(__MMI_PHB_EMAIL_SORT__)           
      else if((LocalPtr->sort_type == MMI_PHB_BY_EMAIL) && (LocalPtr->storage == MMI_NVRAM))
      {
         S16 pError;
         U16 g_phb_email_index[MAX_PB_PHONE_ENTRIES];

         /*Email sorting not populated after bootup.*/
         if(g_phb_email_sorted == 0)
         {
        	 mmi_phb_java_util_rebuild_email_sorting();
      	 g_phb_email_sorted = 1;
         }

         ReadRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

         /*Find first index position*/
         pos = LocalPtr->index - 1;
         
         /*Copy entry by assigned count number*/
         store_index = g_phb_email_index[pos];
         mmi_phb_java_util_copy_entry_from_phb(store_index, &LocalPtr->entry_ptr[count++]);

         while((++pos < g_phb_cntx.phone_used) && (count < LocalPtr->count))
         {
            store_index = g_phb_email_index[pos];
            
            if(store_index < MAX_PB_PHONE_ENTRIES) /*In NVRAM*/
                 mmi_phb_java_util_copy_entry_from_phb(store_index, &LocalPtr->entry_ptr[count++]);
         }
      
         mmi_phb_java_read_rsp(MMI_PHB_JAVA_SUCCESS, count);
         return;      
      }
#endif      
   }
   /*
   * Get total count by storage, no need to return data.
   */
   else if(LocalPtr->count ==0)
   {
      if(LocalPtr->storage == MMI_STORAGE_BOTH)
      {
         mmi_phb_java_read_rsp(MMI_PHB_JAVA_SUCCESS, PhoneBookEntryCount);
         return;
      }
      else if(LocalPtr->storage == MMI_NVRAM)
      {
         mmi_phb_java_read_rsp(MMI_PHB_JAVA_SUCCESS, g_phb_cntx.phone_used);
         return;      
      }
      else if(LocalPtr->storage == MMI_SIM) 
      {
         mmi_phb_java_read_rsp(MMI_PHB_JAVA_SUCCESS, g_phb_cntx.sim_used);
         return;
      }
   }

    /*
   * Error
   */
   mmi_phb_java_read_rsp(MMI_PHB_JAVA_NOT_SUPPORT,0);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_read_rsp
* DESCRIPTION
*   This function initializes phonebook handler app.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_read_rsp(U16 result, U16 count)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   mmi_phb_handler_get_entry_rsp_struct *MsgPtr;
   MYQUEUE Message;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MsgPtr = (mmi_phb_handler_get_entry_rsp_struct*) OslConstructDataPtr(sizeof(mmi_phb_handler_get_entry_rsp_struct));

   MsgPtr->result = result;
   MsgPtr->count = count;

   Message.oslSrcId=MOD_MMI;
   Message.oslDestId=MOD_JASYN;
   Message.oslMsgId = MSG_ID_MMI_PHB_JAVA_GET_ENTRY_RSP;
   Message.oslDataPtr = (oslParaType *)MsgPtr;
   Message.oslPeerBuffPtr= NULL;

   OslMsgSendExtQueue(&Message);

}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_search_req
* DESCRIPTION
*   This function initializes phonebook handler app.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_search_req(void* info)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U16 store_index;
   U16 index;
   S32 distance;
   U8 same_count;
   mmi_phb_handler_search_entry_req_struct* LocalPtr;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   LocalPtr = (mmi_phb_handler_search_entry_req_struct*)info;

   if(!g_phb_cntx.phb_ready || g_phb_cntx.processing )
   {
      mmi_phb_java_search_rsp(MMI_PHB_JAVA_NOT_READY, 0xffff);
      return;
   }

   g_phb_java_sort = LocalPtr->sort_type;
   g_phb_java_storage = LocalPtr->storage;

   /*Check if search pattern empty*/
   if(pfnUnicodeStrlen((S8*)LocalPtr->pattern) == 0)
   {
   	mmi_phb_java_search_rsp(MMI_PHB_JAVA_NOT_FOUND, 0xffff);
   	return;
   }

   /*
   * search by name
   */
   if(LocalPtr->search_type == MMI_PHB_BY_NAME)
   {
       /*Find the closest entry*/
       index = mmi_phb_search_name_binary_search(0, (PhoneBookEntryCount - 1), LocalPtr->pattern);

	   store_index = g_phb_name_index[index];

       /*Check if the closest entry prefix match the pattern*/
       mmi_phb_compare_ucs2_strings(LocalPtr->pattern, (S8*)PhoneBook[store_index].alpha_id.name, &same_count, &distance);

       if(pfnUnicodeStrlen(LocalPtr->pattern) == same_count)
       {
          mmi_phb_java_util_copy_entry_from_phb(store_index, LocalPtr->entry_ptr);
          mmi_phb_java_search_rsp(MMI_PHB_JAVA_SUCCESS, store_index);
       }
       else
       	mmi_phb_java_search_rsp(MMI_PHB_JAVA_NOT_FOUND, 0xffff);
   }
   /*
   * search by email
   */
#if defined(__MMI_PHB_EMAIL_SORT__)    
   else if(LocalPtr->search_type == MMI_PHB_BY_EMAIL)
   {
         U16 g_phb_email_index[MAX_PB_PHONE_ENTRIES];
         S16 pError;

	/*Email sorting not populated after bootup.*/
	if(g_phb_email_sorted == 0)
	{
		mmi_phb_java_util_rebuild_email_sorting();
		g_phb_email_sorted = 1;
	}

         ReadRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

	/*Find the closest entry*/
	store_index = mmi_phb_search_email_binary_search(0, (g_phb_cntx.phone_used -1), LocalPtr->pattern);
	if(store_index >=g_phb_cntx.phone_used) store_index = g_phb_cntx.phone_used-1; 
	store_index = g_phb_email_index[store_index];

      /*Check if the closest entry prefix match the pattern*/
      #if 0
      ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(store_index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
      #endif
      ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
      AnsiiToUnicodeString(pbEmailAddress, (PS8)PhoneBookOptionalFields.emailAddress);
      
       mmi_phb_compare_ucs2_strings(LocalPtr->pattern, pbEmailAddress, &same_count, &distance);

       if(pfnUnicodeStrlen(LocalPtr->pattern) == same_count
	   || distance == 0)
       {
          mmi_phb_java_util_copy_entry_from_phb(store_index, LocalPtr->entry_ptr);
          mmi_phb_java_search_rsp(MMI_PHB_JAVA_SUCCESS, store_index);
       }
       else
       	mmi_phb_java_search_rsp(MMI_PHB_JAVA_NOT_FOUND, 0xffff);
      
   }
#endif   
   /*
   * search by number
   */
   else if(LocalPtr->search_type == MMI_PHB_BY_NUMBER)
   {
      U32 num;
      U8 number_ASCII[MAX_PB_NUMBER_LENGTH+1+1];

      /*Number not sorted yet*/
      if(g_phb_cntx.populate_count != 0xffff)
      {
         mmi_phb_java_search_rsp(MMI_PHB_JAVA_NOT_READY, 0xffff);
         return;
      }

      memset(number_ASCII, 0, ENCODING_LENGTH);
      UnicodeToAnsii((S8*)number_ASCII, LocalPtr->pattern);

      num = mmi_phb_util_convert_number_to_int(number_ASCII);
      if(num < INVALID_NUMBER) 
      {
         store_index = mmi_phb_lookup_table_search(num, 0, (U16)(g_phb_cntx.lookup_table_count-1),(S8*)number_ASCII); 

         if(store_index < 0xffff) /* storage location in array, begin from 0.*/
         {
            mmi_phb_java_util_copy_entry_from_phb(store_index, LocalPtr->entry_ptr);
            mmi_phb_java_search_rsp(MMI_PHB_JAVA_SUCCESS, store_index);
         }
         else
            mmi_phb_java_search_rsp(MMI_PHB_JAVA_NOT_FOUND, 0xffff);         
      }
   }
   else
      mmi_phb_java_search_rsp(MMI_PHB_JAVA_NOT_SUPPORT, 0xffff);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_search_rsp
* DESCRIPTION
*   This function initializes phonebook handler app.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_search_rsp(U16 result, U16 store_index)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   mmi_phb_handler_search_entry_rsp_struct *MsgPtr;
   MYQUEUE Message;

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
   MsgPtr = (mmi_phb_handler_search_entry_rsp_struct*) OslConstructDataPtr(sizeof(mmi_phb_handler_search_entry_rsp_struct));

   /*Decide error cause*/
   MsgPtr->result = result;
   MsgPtr->index = store_index;
   
   if(result == MMI_PHB_JAVA_SUCCESS)
   {
      MsgPtr->index = mmi_phb_java_util_get_index_by_store_index(g_phb_java_sort, g_phb_java_storage, store_index);
   }

   Message.oslSrcId=MOD_MMI;
   Message.oslDestId=MOD_JASYN;
   Message.oslMsgId = MSG_ID_MMI_PHB_JAVA_SEARCH_ENTRY_RSP;
   Message.oslDataPtr = (oslParaType *)MsgPtr;
   Message.oslPeerBuffPtr= NULL;

   OslMsgSendExtQueue(&Message);
}


#define MMI_PHB_JAVA_HANDLER_UTILITIES
#if 0
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
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif

/*****************************************************************************
* FUNCTION
*  mmi_phb_util_get_index_by_record_index
* DESCRIPTION
*   This function initializes phonebook handler app.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
U16 mmi_phb_java_util_get_index_by_store_index(U8 sort_type, U8 storage, U16 in_store_index)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   U16 i;
   U16 count = 0;
   U16 store_index;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   /*
   * Sort By Name
   */
   if(sort_type == MMI_PHB_BY_NAME)
   {
      for(i = 0 ; i <PhoneBookEntryCount ; i++)
      {
         store_index = g_phb_name_index[i];
      
         if( (storage == MMI_STORAGE_BOTH) || \
              ((storage == MMI_NVRAM) && (store_index < MAX_PB_PHONE_ENTRIES)) || \
         	    ((storage == MMI_SIM) && (store_index >= MAX_PB_PHONE_ENTRIES)))
         	     count++;

         if(store_index == in_store_index)
            return count;
      }
   }
   /*
   * Sort By Email, Only for storage in NVRAM only.
   */
#if defined(__MMI_PHB_EMAIL_SORT__)     
   else if(sort_type == MMI_PHB_BY_EMAIL)
   {
      S16 pError;
      U16 g_phb_email_index[MAX_PB_PHONE_ENTRIES];
      
      if(storage != MMI_NVRAM)
      	return 0xffff;

      ReadRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);

      for(i = 0 ; i < g_phb_cntx.phone_used ; i++)
      {
         store_index = g_phb_email_index[i];

         count++;

         if(store_index == in_store_index)
         	return count;
      }
   }
#endif   

   return 0xffff;
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_util_copy_entry_from_phb
* DESCRIPTION
*   This function 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_util_copy_entry_from_phb(U16 store_index, mmi_phb_handler_entry_struct* entry)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
   S16 pError;
   
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/

   memcpy(entry->name, PhoneBook[store_index].alpha_id.name,(MAX_PB_NAME_LENGTH+1)*ENCODING_LENGTH);
   mmi_phb_convert_get_ucs2_number(entry->number, store_index);

   if(store_index < MAX_PB_PHONE_ENTRIES) /*In NVRAM*/
   {
    #if 0
      ReadRecord(NVRAM_EF_PHB_FIELDS_LID, (U16)(store_index + 1), (void*)&PhoneBookOptionalFields, OPTIONAL_FIELDS_RECORD_SIZE, &pError);
    #endif
    #if !defined(__MMI_PHB_NO_OPTIONAL_FIELD__)
       ReadRecord(NVRAM_EF_PHB_SOMEFIELDS_LID, 1, (void*)&PhoneBookOptionalFields, store_index*OPTIONAL_FIELDS_RECORD_SIZE, &pError);
     AnsiiToUnicodeString(entry->homeNumber, (PS8)PhoneBookOptionalFields.homeNumber);
      AnsiiToUnicodeString(entry->officeNumber, (PS8)PhoneBookOptionalFields.officeNumber);
      AnsiiToUnicodeString(entry->faxNumber, (PS8)PhoneBookOptionalFields.faxNumber);
      AnsiiToUnicodeString(entry->emailAddress, (PS8)PhoneBookOptionalFields.emailAddress);
      #endif
   }
   else /*clear buffer*/
   {
      memset(entry->homeNumber,0,2);
      memset(entry->officeNumber,0,2);
      memset(entry->faxNumber,0,2);
      memset(entry->emailAddress,0,2);
   }
   
}

#if defined(__MMI_PHB_EMAIL_SORT__)
/*****************************************************************************
* FUNCTION
*  mmi_phb_java_util_rebuild_email_sorting
* DESCRIPTION
*   This function 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_util_rebuild_email_sorting(void)
{
	/*----------------------------------------------------------------*/
	/* Local Variables                                                */
	/*----------------------------------------------------------------*/
	U16 i,j;
	U16 store_index;
	S16 pError;
	U16* g_phb_email_index;
	U16 pop_count = 0;
	U16 pos;

	/*----------------------------------------------------------------*/
	/* Code Body                                                      */
	/*----------------------------------------------------------------*/

	g_phb_cntx.processing = TRUE;
	mmi_phb_java_entry_processing_email();

	
	g_phb_email_index = OslMalloc(MAX_PB_PHONE_ENTRIES*sizeof(U16));
	memset(g_phb_email_index, 0xff, MAX_PB_PHONE_ENTRIES*sizeof(U16));

	for(i = 0 ; i < PhoneBookEntryCount ; i++)
	{
		store_index = g_phb_name_index[i];

		if(store_index < MAX_PB_PHONE_ENTRIES) /*Entry in phone*/
		{
			++pop_count;

			if(pop_count == 1)
			{
				g_phb_email_index[(pop_count - 1)] = store_index;
			}
			else
			{
				pos = mmi_phb_sort_email_find_insert_pos(0, (pop_count-2), store_index, g_phb_email_index);

				for(j = (pop_count - 1); j > pos ; j--)
				{
					g_phb_email_index[j] = g_phb_email_index[j-1];
				}
				g_phb_email_index[pos] = store_index;
		
			}
	
		}
	}

	WriteRecord(NVRAM_EF_PHB_EMAIL_SORT_LID, 1, (void*)g_phb_email_index, MAX_PB_PHONE_ENTRIES*sizeof(U16), &pError);
	OslMfree(g_phb_email_index);

	g_phb_cntx.processing = FALSE;
	//DisplayPopup((PU8)GetString(STR_GLOBAL_DONE), IMG_GLOBAL_ACTIVATED, TRUE, PHB_NOTIFY_TIMEOUT, SUCCESS_TONE);
	GoBackHistory();
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_entry_processing_email
* DESCRIPTION
*   This function 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_entry_processing_email(void) 
{
	EntryNewScreen(SCR_ID_PHB_SORTING_EMAIL, mmi_phb_java_exit_processing_email, NULL, NULL);
	
	ShowCategory8Screen(STR_SCR_PBOOK_CAPTION,IMG_SCR_PBOOK_CAPTION, 
		0, 0, 0, 0, 
		STR_ID_PHB_SORTING_EMAIL, IMG_PROCESSING_PHONEBOOK, NULL);

	ClearAllKeyHandler();
	ClearKeyHandler(KEY_END, KEY_EVENT_UP);
	ClearKeyHandler(KEY_END, KEY_EVENT_DOWN);
	ClearKeyHandler(KEY_END, KEY_LONG_PRESS);
	ClearKeyHandler(KEY_END, KEY_REPEAT);
}

/*****************************************************************************
* FUNCTION
*  mmi_phb_java_exit_processing_email
* DESCRIPTION
*   This function 
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   none
*****************************************************************************/
void mmi_phb_java_exit_processing_email(void) 
{
	history_t   h;
	S16		nHistory	=	0;

	if(g_phb_cntx.processing) 
	{
		g_phb_cntx.end_scr_id = SCR_ID_PHB_SORTING_EMAIL;
		h.scrnID = SCR_ID_PHB_SORTING_EMAIL;
		h.entryFuncPtr = mmi_phb_java_entry_processing_email;
		pfnUnicodeStrcpy((PS8)h.inputBuffer, (PS8)&nHistory);
		GetCategoryHistory(h.guiBuffer);
		AddHistory(h);
	}
}

#endif

#endif /*defined(__J2ME__)*/
#endif /*_MMI_PHONEBOOKJAVAHANDLER_C*/


