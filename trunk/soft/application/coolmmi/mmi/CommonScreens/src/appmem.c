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
 *	asm.c
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends to handle all scenario of Java Content Manager and 
 *   interaction betweens MMI and Java task.
 *
 * Author:
 * -------
 * -------
 *
*==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================== 
 *******************************************************************************/
#if defined(APPMEM_SUPPORT)
#define __NEWSIMULATOR

#include "mmi_features.h" 
/*  Include: MMI header file */
#include "stdc.h"
#include "l4dr1.h"
#include "l4dr.h"
#include "unicodexdcl.h"
#include "commonscreens.h"
#include "globaldefs.h"
#include "historygprot.h"
#include "protocolevents.h"
#include "wgui_categories.h"
#include "mainmenudef.h" 
#include "screensaverdefs.h" 
#include "debuginitdef.h"
#include "profilegprots.h" 
#include "phonebookdef.h" 
#include "globalconstants.h" 
#include "timerevents.h"
#include "wrappergprot.h"

//#include "jam_msg_handler.h"
//#include "j2me_trace.h"

#undef __NEWSIMULATOR
#if !defined(MMI_ON_WIN32)
#include "javadef.h"
#include "javaprot.h"
#endif
#include "mmiapi.h"
#include "idleappdef.h"
#include "datetimetype.h"
#include "simdetectiondef.h"
#include "idleappdef.h"
#include "eventsdcl.h"
#include "unicodexdcl.h"

extern void mmi_asm_show_dialog(void* MsgStruct);
extern U16 GetCmMarkerScrnID (void);
typedef struct _mmi_show_asm_dialog_req_struct_t
{
	LOCAL_PARA_HDR
	kal_wchar*			confirm_msg;
} mmi_show_asm_dialog_req_struct;

U8* asm_memory_full_string = NULL;
kal_mutexid	asm_dialog_mutex;

void InitAppMem(void)
{
	SetProtocolEventHandler((PsFuncPtr)mmi_asm_show_dialog, MSG_ID_MMI_SHOW_ASM_DIALOG_REQ);
    asm_dialog_mutex = kal_create_mutex("ASM dialog");
}
/*****************************************************************************
* FUNCTION
*  mmi_memory_full_ok
* DESCRIPTION
*   This funciton used by mmi_asm_show_dialog on pressing Left soft button
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_memory_full_ok(void)
{

	ClearAllKeyHandler();
	
	GoBackHistory(); // this goback history is called by keypress

}

/*****************************************************************************
* FUNCTION
*  mmi_exit_mmi_show_dialog_req
* DESCRIPTION
*   This funciton is invoked when asm dialog dismissed.
*
* PARAMETERS
*  void
* RETURNS
*  void
* GLOBALS AFFECTED
*   
*****************************************************************************/
void mmi_exit_mmi_show_dialog_req(void){
    if(asm_memory_full_string != NULL){
        OslMfree(asm_memory_full_string);
        asm_memory_full_string = NULL;
    }
}

/*****************************************************************************
* FUNCTION
*  mmi_asm_show_dialog
* DESCRIPTION
*   Display the asm dialog when receiving the MSG_ID_MMI_SHOW_ASM_DIALOG_REQ
*
* PARAMETERS
*  mmi_show_asm_dialog_req_struct MsgStruct
* RETURNS
*  void
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_asm_show_dialog(void* MsgStruct)
{
	mmi_show_asm_dialog_req_struct *msg;

	msg=(mmi_show_asm_dialog_req_struct*)MsgStruct;

	//DisplayPopup((PU8)msg->message, IMG_GLOBAL_WARNING, 1, 5000, (U8)WARNING_TONE);
	//vm_has_been_started = KAL_FALSE;

	EntryNewScreen(SCR_ID_ASM_MEM_FULL, mmi_exit_mmi_show_dialog_req,NULL,NULL);

	ShowCategory74Screen(STR_ID_ASM_MEM_FULL_TITLE,
						0,
						STR_GLOBAL_OK,IMG_GLOBAL_OK,
						0, 0,
                        (PU8)msg->confirm_msg,
						pfnUnicodeStrlen((const S8*)msg->confirm_msg), NULL);

	SetLeftSoftkeyFunction(mmi_memory_full_ok,KEY_EVENT_UP);
}

/*****************************************************************************
* FUNCTION
*  get_clean_list_str
* DESCRIPTION
*   Get the string to inform the user closing proper applications for getting
*   enough memory for the request parameter (size).
*
* PARAMETERS
*  size --> request size
*  string_id --> the string_id of the application name
*  mem_full_string --> pointer to a string containing the message how many applications
*  		has to be closed for running the requested AP. The string is allocated from
*       control buffer and should be freed properly.
* RETURNS
*  void
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void get_clean_list_str(S32 size, S32 string_id, U8** mem_full_string)
{
#if 0
    KAL_APPMEM_APP_INFO *temp_info;
    UI_string_type temp_string;
    S32 counter = 0;
    S32 buffer_size = 150;
    S32 clean_list_scan = 0;
    BOOL mem_enough = KAL_TRUE;
    temp_info = kal_app_mem_sync_alloc_clean_list(size);
    /* the memory is not enough for allocate the size even free up all memory */
    if(temp_info == NULL){
        temp_string = get_string(STR_ID_ASM_MEM_FULL_3);
        *mem_full_string = get_ctrl_buffer((pfnUnicodeStrlen((const S8*)temp_string) << 1) + 2);
        memcpy(&(*mem_full_string)[0], temp_string, pfnUnicodeStrlen((const S8*)temp_string) << 1);
        ((UI_string_type)(*mem_full_string))[pfnUnicodeStrlen((const S8*)temp_string)] = 0;
        return;
    }
    /* the required memory is enough and no memory has to be freed */
    if(temp_info[0].app_string_id == 0){
        *mem_full_string = NULL;
        free_ctrl_buffer(temp_info);
        return;
    }
    *mem_full_string = NULL;
    do{
        buffer_size += 100;
        if(buffer_size >= 1000)
            break;
        clean_list_scan = 0;
        counter = 0;
        if(*mem_full_string != NULL){
            free_ctrl_buffer(*mem_full_string);
            *mem_full_string = NULL;
        }

        *mem_full_string = get_ctrl_buffer(buffer_size);

        temp_string = get_string(STR_ID_ASM_MEM_FULL_1);
        memcpy(&(*mem_full_string)[counter], temp_string, pfnUnicodeStrlen((const S8*)temp_string) << 1);
        counter += pfnUnicodeStrlen((const S8*)temp_string) << 1;

        ((UI_string_type)(*mem_full_string))[counter >> 1] = (U16)' ';
        counter += 2;

        temp_string = get_string((UI_string_ID_type)string_id);
        memcpy(&(*mem_full_string)[counter], temp_string, pfnUnicodeStrlen((const S8*)temp_string) << 1);
        counter += pfnUnicodeStrlen((const S8*)temp_string) << 1;

        ((UI_string_type)(*mem_full_string))[counter >> 1] = (U16)'.';
        counter += 2;
        ((UI_string_type)(*mem_full_string))[counter >> 1] = (U16)' ';
        counter += 2;

        temp_string = get_string(STR_ID_ASM_MEM_FULL_2);
        memcpy(&(*mem_full_string)[counter], temp_string, pfnUnicodeStrlen((const S8*)temp_string) << 1);
        counter += pfnUnicodeStrlen((const S8*)temp_string) << 1;
        mem_enough = KAL_TRUE;
        while(temp_info[clean_list_scan].app_string_id != 0){
            
            if(counter + 4 >= buffer_size){
                mem_enough = KAL_FALSE;
                break;
            }
            ((UI_string_type)(*mem_full_string))[counter >> 1] = (U16)'\n';
            counter += 2;

            temp_string = get_string(temp_info[clean_list_scan].app_string_id);
            if(counter + (pfnUnicodeStrlen((const S8*)temp_string) << 1) + 2>= buffer_size){
                mem_enough = KAL_FALSE;
                break;
            }
            memcpy(&(*mem_full_string)[counter], temp_string, pfnUnicodeStrlen((const S8*)temp_string) << 1);
            counter += pfnUnicodeStrlen((const S8*)temp_string) << 1;

            if(temp_info[clean_list_scan].app_hint_id != 0){

                if(counter + 4 >= buffer_size){
                    mem_enough = KAL_FALSE;
                    break;
                }
                ((UI_string_type)(*mem_full_string))[counter >> 1] = (U16)'(';
                counter += 2;

                temp_string = get_string(temp_info[clean_list_scan].app_hint_id);
                if(counter + (pfnUnicodeStrlen((const S8*)temp_string) << 1) + 2 >= buffer_size){
                    mem_enough = KAL_FALSE;
                    break;
                }
                memcpy(&(*mem_full_string)[counter], temp_string, pfnUnicodeStrlen((const S8*)temp_string) << 1);
                counter += pfnUnicodeStrlen((const S8*)temp_string) << 1;

                if(counter + 4 >= buffer_size){
                    mem_enough = KAL_FALSE;
                    break;
                }
                ((UI_string_type)(*mem_full_string))[counter >> 1] = (U16)')';
                counter += 2;
            }

            clean_list_scan ++;
        }
        if(mem_enough)
            break;
    }while(1);
    
    ((UI_string_type)(*mem_full_string))[counter >> 1] = 0; /* add NULL terminator */

    free_ctrl_buffer(temp_info);
#endif
}

/*****************************************************************************
* FUNCTION
*  mmi_asm_show_memory_full_dialog
* DESCRIPTION
*   Display the information to inform the user closing proper applications for getting
*   enough memory for the application indicating by the string_id with the request size.
*
* PARAMETERS
*  size --> request size
*  string_id --> the string_id of the application name
*  mod_type --> the module id of the caller.
* RETURNS
*  void
* GLOBALS AFFECTED
*   external_global
*****************************************************************************/
void mmi_asm_show_memory_full_dialog(S32 size, S32 string_id, int mod_type)
{
#if 0
	ilm_struct *ilm_ptr;
	mmi_show_asm_dialog_req_struct *msg;

	kal_take_mutex(asm_dialog_mutex);

    if(asm_memory_full_string != NULL){
        free_ctrl_buffer(asm_memory_full_string);
        asm_memory_full_string = NULL;
        DeleteScreenIfPresent(SCR_ID_ASM_MEM_FULL);
    }

    get_clean_list_str(size, string_id, &asm_memory_full_string);
    if(asm_memory_full_string != NULL){
	    msg = (mmi_show_asm_dialog_req_struct*)construct_local_para(
	                        (kal_uint16)(sizeof(mmi_show_asm_dialog_req_struct)),0);
	    msg->confirm_msg = (UI_string_type)asm_memory_full_string;
		ilm_ptr = allocate_ilm( mod_type );
		ilm_ptr->msg_id = (kal_uint16) MSG_ID_MMI_SHOW_ASM_DIALOG_REQ;/* Set the message id */
		ilm_ptr->peer_buff_ptr = NULL;/* there are no peer message */
		ilm_ptr->local_para_ptr = (local_para_struct *)msg;
	
	    while(IsScreenPresent(GetCmMarkerScrnID()))
	        kal_sleep_task(10);
		SEND_ILM(mod_type, MOD_MMI, 0,ilm_ptr);
    }
    kal_give_mutex(asm_dialog_mutex);
#endif
}


#endif /* APPMEM_SUPPORT */

