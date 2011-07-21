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
 *
 * Filename:
 * ---------
 *	fmt_main.C
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for ¡K.
 *
 * Author:
 * -------
 * -------
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
 *
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
 
 *==============================================================================
 *******************************************************************************/
#define FMT_MAIN_C

#include "kal_release.h"


#include "stack_common.h"
#include "stack_msgs.h"
#include "app_ltlcom.h"          /* task message communiction */
#include "stacklib.h"           /* basic type for dll, evshed, stacktimer */
#include "event_shed.h"          /* event scheduler */
#include "stack_timer.h"         /* stack timer */
#include "mmiapi.h"
/*WUZC Del Start For MMIFS Ver:    on 2007-1-10 16:39 */
#if 0
    #include "app_features.h"
#endif
#include "queuegprot.h"
/*WUZC Del End  For MMIFS Ver:    on 2007-1-10 16:39 */
#include "msdc_def.h"
#include "fat_fs.h"

#include "fmt_def.h"
#include "fmt_main.h"
#include "fmt_struct.h"

#ifdef __OTG_ENABLE__
    #include "usb_host_ms_if.h"
#endif

#include "mmi_trace.h"
#ifndef FMT_NOT_PRESENT

static T_MSDC_CARD fmt_msdc_type=UNKNOWN_CARD;
static kal_bool fmt_msdc_present = KAL_FALSE;
kal_bool fmt_need_abort = KAL_FALSE;

    #ifdef __FS_CHECKDRIVE_SUPPORT__
__align(4) static kal_uint8 g_fmt_buffer[FMT_MAX_BUFFER_SIZE];
    #endif

extern fmgr_filter_struct fmgr_filter[];
extern int MSF_wapIsInit(void);

/*****************************************************************************
* FUNCTION
*	fmt_create
* DESCRIPTION
*	fmt task create function
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
#if 0
kal_bool fmt_create(comptask_handler_struct **handle)
{
    static const comptask_handler_struct fmt_handler_info =
    {
        fmt_task_main,          /* task entry function */
        fmt_init,           /* task initialization function */
        NULL,       /* task configuration function */
        fmt_reset,              /* task reset handler */
        NULL,               /* task termination handler */
    };

    *handle = (comptask_handler_struct *)&fmt_handler_info;
    return KAL_TRUE;
}
#endif 

/*****************************************************************************
* FUNCTION
*	fmt_task_main
* DESCRIPTION
*	fmt task main loop
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
#if 0
void fmt_task_main( task_entry_struct *task_entry_ptr)
{
    ilm_struct current_ilm;
    kal_uint32 my_index;

    kal_get_my_task_index(&my_index);

    fmt_init_drive();

    while ( 1 )
    {
        receive_msg_ext_q( task_info_g[task_entry_ptr->task_indx].task_ext_qid,
                           &current_ilm);

        stack_set_active_module_id( my_index, current_ilm.dest_mod_id);

        fmt_main((void *) &current_ilm);

        free_ilm( &current_ilm);
    }

}
#endif
/*****************************************************************************
* FUNCTION
*	fmt_init
* DESCRIPTION
*	fmt task init handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
kal_bool fmt_init( task_indx_type task_indx )
{
//wangrui add  for fix bug 10484 	
	fmt_init_drive();
#ifdef __SXS_NVRAM_RW__
	sxs_setNvramCallback(fmt_send_nvram_msg);
#endif
    return KAL_TRUE;
}

/*****************************************************************************
* FUNCTION
*	fmt_reset
* DESCRIPTION
*	fmt task reset handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
kal_bool fmt_reset( task_indx_type task_indx )
{
    return KAL_TRUE;
}

/*wangrui add a fun begin 20081208  for fix bug 10484 	*/

void    GetMsdcStatus(void)
{
    FS_DiskInfo info;
    kal_uint8 path[10];
    INT result;
        int card;

        kal_prompt_trace (MOD_FMT, "fmt_init_drive: mount ok");
	if (fmt_msdc_present)	return;
	
        card = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
        if ( card >= FS_NO_ERROR )
        {
            memset (path, 0, 10);
            kal_wsprintf((WCHAR*)path, "%c:\\", card);

            result = MMI_FS_GetDiskInfo((UINT8*)path, &info, FS_DI_FREE_SPACE);
	    if(result >= 0) 
           {
       	        fmt_msdc_present = KAL_TRUE;
           }
        }
	
}
/*wangrui add a fun end 20081208 */
/*****************************************************************************
* FUNCTION
*	fmt_init_drive
* DESCRIPTION
*	fmt task message dispatcher
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_init_drive (void)
{
/*wangrui add  200081201 begin for fix bug 10484 	*/
    FS_DiskInfo info;
    kal_uint8 path[10];
    int phone;
    INT result;

    phone = MMI_FS_GetDrive(FS_DRIVE_V_NORMAL, 2, FS_DRIVE_V_NORMAL | FS_DRIVE_I_SYSTEM);
    memset (path, 0, 10);

    if ( phone >= FS_NO_ERROR )
    {
        kal_wsprintf((WCHAR*)path, "%c:\\", phone);

        MMI_FS_GetDiskInfo((UINT8*)path, &info, FS_DI_FREE_SPACE);
    }

//#ifdef __FS_CARD_SUPPORT__
    kal_prompt_trace (MOD_FMT, "fmt_init_drive: check card");
//#if defined( __MSDC_NOT_SUPPORT_HOT_PLUG__) || defined(__MSDC_TFLASH_DAT3_1BIT_HOT_PLUG__) || defined (MILAN_BB)
    fmt_msdc_type = MMC_CARD;
#if 0
/* under construction !*/
#endif
    {
#if 0
        int card;

        kal_prompt_trace (MOD_FMT, "fmt_init_drive: mount ok");

        card = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
		if ( card >= FS_NO_ERROR )
        {
            memset (path, 0, 10);
            kal_wsprintf((WCHAR*)path, "%c:\\", card);

            result = MMI_FS_GetDiskInfo((UINT8*)path, &info, FS_DI_FREE_SPACE);
	    if(result >= 0) 
           {
       	        fmt_msdc_present = KAL_TRUE;
           }
		
        }
#endif
    }    
//#endif
    kal_prompt_trace (MOD_FMT, "fmt_init_drive: check card done");
//#endif
/*wangrui add  200081201 end 	*/
}

/*****************************************************************************
* FUNCTION
*	fmt_main
* DESCRIPTION
*	fmt task message dispatcher
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_main( ilm_struct *ilm_ptr)
{
    switch ( ilm_ptr->msg_id )
    {
#ifdef __OTG_ENABLE__
        case MSG_ID_OTG_MS_INSERT_IND:
            fmt_otg_insert_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
        case MSG_ID_OTG_MS_REMOVE_IND:    
            fmt_otg_remove_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break; 
        case MSG_ID_OTG_DISPLAY_IND:
            fmt_otg_display_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
        case MSG_ID_OTG_CARD_DETECT_IND:
            fmt_otg_card_detect_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
#endif         

#ifdef __FS_CARD_SUPPORT__
        case MSG_ID_MSDC_CARD_DETECT_IND:
            fmt_card_detect_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
        case MSG_ID_MMI_FMT_UNMOUNT_REQ:
            fmt_unmount_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
#endif			
#ifdef __FS_MOVE_SUPPORT__
        case MSG_ID_MMI_FMT_COPY_REQ:
			
            fmt_copy_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
#endif         
        case MSG_ID_MMI_FMT_FORMAT_REQ:
            fmt_format_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
        case MSG_ID_MMI_FMT_FORMAT_RSP:
            fmt_format_done_hdlr (ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
#ifdef __FS_CHECKDRIVE_SUPPORT__         
        case MSG_ID_MMI_FMT_CHECK_DRIVE_REQ:
            fmt_check_drive_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;         
#endif         
        case MSG_ID_MMI_FMT_DELETE_REQ:
            fmt_delete_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
        case MSG_ID_MMI_FMT_GET_MSDC_STATUS_REQ:
            fmt_get_msdc_status_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
            break;
#ifdef __SXS_NVRAM_RW__    // add by chenzy 20091109
        case MSG_ID_MMI_FMT_READ_NVRAM_IND:
         fmt_read_nvram_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
          break;
        case MSG_ID_MMI_FMT_WRITE_NVRAM_IND:
         fmt_write_nvram_hdlr(ilm_ptr->local_para_ptr, ilm_ptr->peer_buff_ptr);
          break;
#endif		
        default:
            break;
    }

}

#ifdef __SXS_NVRAM_RW__ // add by chenzy 20091109

void fmt_send_nvram_msg(u8 *data,u8 type)
{
	TRACE_EF(g_sw_FMGR, "start the fmt_send_nvram_msg function!\n");

	u8 bei = 0;
	u8 yu  = 0;
	u16 size = 0;
	u8* tempdata = COS_MALLOC(size);
	bei = (int)data[2];
	yu = (int)data[3];
	size = (u16)(bei*255+yu);
	if(type)
	{		
		memset(tempdata,'\0',size);    
		memcpy(tempdata, &data[4],size);
	}
	sxs_FlwNvram_t *FlwNvramT = (sxs_FlwNvram_t *)data;
	if(type)
	{
		FlwNvramT->Data = tempdata;
	}
	FlwNvramT->Size = size;
	
	ilm_struct *ilm_ptr = allocate_ilm( MOD_FMT );
	if(type)
	{
		ilm_ptr->msg_id = MSG_ID_MMI_FMT_WRITE_NVRAM_IND;
	}
	else
	{
		ilm_ptr->msg_id = MSG_ID_MMI_FMT_READ_NVRAM_IND;
	}
	ilm_ptr->peer_buff_ptr = NULL;
	ilm_ptr->local_para_ptr = (local_para_struct *)FlwNvramT;
	SEND_ILM( MOD_FMT, MOD_FMT, MMI_L4C_SAP, ilm_ptr);
	
	TRACE_EF(g_sw_FMGR, "end the fmt_send_nvram_msg function!\n");
}

/*****************************************************************************
* FUNCTION
*	fmt_read_nvram_hdlr
* DESCRIPTION
*	deal the  message handler from the sxs which from the pc to read the nvram message
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/

void fmt_read_nvram_hdlr(local_para_struct *local_para_ptr,
                              peer_buff_struct *peer_buff_ptr)
{
  TRACE_EF(g_sw_FMGR, "start the fmt_read_nvram_hdlr");
  
  sxs_FlwNvram_t *msg_ptr;
  msg_ptr = (sxs_FlwNvram_t *)local_para_ptr;
    
  S16 nvramerror;
  S32 revalue = 0;
  S32 ii = 0;
  u8 readbuf[msg_ptr->Size];
  
  revalue=ReadRecord ((U16)msg_ptr ->Lid, (U16)msg_ptr ->Recorder,(void*)readbuf,(U16)msg_ptr->Size,&nvramerror);
  if(revalue >= 0)
  {
  	mmi_trace_hex(g_sw_FMGR, revalue, readbuf);

    	unsigned char retvalue[3*revalue];
	unsigned char temp[3];
   	memset (retvalue, '\0', 3*revalue);
	 for(ii = 0; ii < revalue; ii++)
        {
            sprintf(temp, "/%x", *(readbuf + ii));
            strcat(retvalue, temp);
        }
	 retvalue[3*revalue] = '\0';

    sxs_nvramprintf( _SXR|TSTDOUT|TSMAP(2)|TNB_ARG(4), "%d,%s,%d,%d",
            1,retvalue,msg_ptr ->Lid,msg_ptr ->Recorder);
  }
  else
  {
    sxs_nvramprintf( _SXR|TSTDOUT|TNB_ARG(3), "%d,%d,%d",
            0,msg_ptr ->Lid,msg_ptr ->Recorder);    
  }
 
  TRACE_EF(g_sw_FMGR, "end the fmt_read_nvram_hdlr");
}
/*****************************************************************************
* FUNCTION
*	fmt_write_nvram_hdlr
* DESCRIPTION
*	deal the  message handler from the sxs which from the pc to read the nvram message
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_write_nvram_hdlr(local_para_struct *local_para_ptr,
                              peer_buff_struct *peer_buff_ptr)
{
  TRACE_EF(g_sw_FMGR, "start the fmt_write_nvram_hdlr");
  
  sxs_FlwNvram_t *msg_ptr;
  msg_ptr = (sxs_FlwNvram_t *)local_para_ptr;
  S16 nvramerror;
  S32 revalue = 0;

  mmi_trace_hex(g_sw_FMGR, msg_ptr->Size, msg_ptr->Data);
  
TRACE_EF(g_sw_FMGR, "write the nvram record the size is %d!",msg_ptr->Size); 

  revalue=WriteRecord ((U16)msg_ptr ->Lid, (U16)msg_ptr ->Recorder,(void*)msg_ptr->Data,(U16)msg_ptr->Size,&nvramerror);
  if(revalue >= 0)
  {
    TRACE_EF(g_sw_FMGR, "write the nvram record is ok!"); 
  }
  else
  {
    TRACE_EF(g_sw_FMGR, "write the nvram record is error!");   
  }

  TRACE_EF(g_sw_FMGR, "end the fmt_write_nvram_hdlr");
}
#endif


    #ifdef __OTG_ENABLE__	

/*****************************************************************************
* FUNCTION
*	fmt_otg_card_detect_hdlr
* DESCRIPTION
*	msdc detect message handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_otg_card_detect_hdlr(local_para_struct *local_para_ptr,
                              peer_buff_struct *peer_buff_ptr)
{
    ilm_struct *ilm_ptr = NULL;
    fmt_mmi_msdc_ind_struct *param_ptr=NULL;
    usb_host_ms_media_struct *ms_ind;
    kal_uint32* media_array;
    kal_int32 fs_ret;
    kal_uint8 media_size;
    kal_uint8 index;
    kal_int8 plug_in=-1;

    ms_ind = (usb_host_ms_media_struct*)local_para_ptr;
    media_array = (kal_uint32*)&ms_ind->media_array;    
    media_size = ms_ind->media_size;

    for ( index=0; index<media_size; index++ )
    {
        if ( media_array[index]!=0 )
        {
            fs_ret = MMI_FS_TestMSDC( (void*) media_array[index] );
            kal_prompt_trace(MOD_FMT, "fmt call FS_TestMSDC get (%d)\n", fs_ret);
            if ( fs_ret >= FS_NO_ERROR )
                plug_in = 1;
            else
                plug_in = 0;
        }
    }

    if ( plug_in != -1 )
    {
        /* First allocate a memory for an interlayer message structure */
        ilm_ptr = allocate_ilm( MOD_FMT );
        param_ptr = (fmt_mmi_msdc_ind_struct *)OslConstructDataPtr
                    (sizeof (fmt_mmi_msdc_ind_struct) );
        param_ptr->type = SD_CARD;
        param_ptr->cause = 0;

        if ( plug_in==1 )
        {
            ilm_ptr->msg_id = MSG_ID_FMT_MMI_CARD_PLUG_IN_IND;
            fmt_msdc_present = KAL_TRUE;
        }
        else
        {
            ilm_ptr->msg_id = MSG_ID_FMT_MMI_CARD_PLUG_OUT_IND;
            fmt_msdc_present = KAL_FALSE;
        }

        ilm_ptr->peer_buff_ptr = NULL;
        ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

        SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);  
    }
}
    #endif

    #ifdef __FS_CARD_SUPPORT__

/*****************************************************************************
* FUNCTION
*	fmt_card_detect_hdlr
* DESCRIPTION
*	msdc detect message handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_card_detect_hdlr(local_para_struct *local_para_ptr,
                          peer_buff_struct *peer_buff_ptr)
{
    fmt_mmi_msdc_ind_struct *param_ptr=NULL;
    msdc_card_detect_ind_struct *msg_ptr;
    ilm_struct *ilm_ptr = NULL;
    kal_int32 fs_ret=FS_NO_ERROR;

    /* scan queue for multiple detect ind */
    msg_ptr = (msdc_card_detect_ind_struct*)local_para_ptr;
    fs_ret =MMI_FS_TestMSDC((void *)msg_ptr->media_array);
/*wangrui add  200081201 begin for fix bug 10484 	*/
    fmt_msdc_type = MMC_CARD;

    /* First allocate a memory for an interlayer message structure */
    ilm_ptr = allocate_ilm( MOD_FMT );
    param_ptr = (fmt_mmi_msdc_ind_struct *)OslConstructDataPtr
                (sizeof (fmt_mmi_msdc_ind_struct) );
    param_ptr->type = fmt_msdc_type;

    param_ptr->cause = fs_ret;

    if ( fs_ret == FS_NO_ERROR )
    {
        int card;
        kal_uint8 path[10];
        FS_DiskInfo info;

        card = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);

        if ( card >= FS_NO_ERROR )
        {
            kal_wsprintf((WCHAR*)path, "%c:\\", card);

            MMI_FS_GetDiskInfo((UINT8*)path, &info, FS_DI_BASIC_INFO|FS_DI_FREE_SPACE);
        }

        ilm_ptr->msg_id = MSG_ID_FMT_MMI_CARD_PLUG_IN_IND;
        fmt_msdc_present = KAL_TRUE;
    }
    else
    {
        ilm_ptr->msg_id = MSG_ID_FMT_MMI_CARD_PLUG_OUT_IND;
        fmt_msdc_present = KAL_FALSE;
    }

    kal_prompt_trace(MOD_FMT, "FS return: %d\n", fs_ret);

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);

#ifdef __J2ME__
    ilm_ptr = allocate_ilm( MOD_FMT );
    param_ptr = (fmt_mmi_msdc_ind_struct *)OslConstructDataPtr
                (sizeof (fmt_mmi_msdc_ind_struct) );
    param_ptr->type = fmt_msdc_type;
    param_ptr->cause = fs_ret;

    if ( fs_ret == FS_NO_ERROR )
    {
        ilm_ptr->msg_id = MSG_ID_FMT_MMI_CARD_PLUG_IN_IND;
    }
    else// if (fs_ret == FS_MSDC_NOT_PRESENT)
    {
        ilm_ptr->msg_id = MSG_ID_FMT_MMI_CARD_PLUG_OUT_IND;
    }
    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr =  (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_JASYN, MMI_L4C_SAP, ilm_ptr);
#endif

    return;
}

/*****************************************************************************
* FUNCTION
*	fmt_unmount_hdlr
* DESCRIPTION
*	unmount request handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_unmount_hdlr(local_para_struct *local_para_ptr,
                      peer_buff_struct *peer_buff_ptr)
{
    mmi_fmt_unmount_req_strcut *msg_ptr;
    mmi_fmt_unmount_rsp_strcut *param_ptr;
    ilm_struct *ilm_ptr = NULL;
    kal_int32 fs_ret;

    msg_ptr = (mmi_fmt_unmount_req_strcut *)local_para_ptr;

    fs_ret = MMI_FS_CloseMSDC(msg_ptr->index, msg_ptr->mode);

    /* First allocate a memory for an interlayer message structure */
    ilm_ptr = allocate_ilm( MOD_FMT );

    param_ptr = (mmi_fmt_unmount_rsp_strcut *)OslConstructDataPtr
                (sizeof (mmi_fmt_unmount_rsp_strcut) );

    param_ptr->result = fs_ret;

    ilm_ptr->msg_id = MSG_ID_MMI_FMT_UNMOUNT_RSP;

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);

    return;
}

    #endif

/*****************************************************************************
* FUNCTION
*	fmt_get_msdc_status_hdlr
* DESCRIPTION
*	msdc status request handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_get_msdc_status_hdlr(local_para_struct *local_para_ptr,
                              peer_buff_struct *peer_buff_ptr)
{
    mmi_fmt_get_msdc_status_rsp_strcut *param_ptr;
    ilm_struct *ilm_ptr = NULL;

    ilm_ptr = allocate_ilm( MOD_FMT );

    param_ptr = (mmi_fmt_get_msdc_status_rsp_strcut *)OslConstructDataPtr
                (sizeof (mmi_fmt_get_msdc_status_rsp_strcut) );
   GetMsdcStatus();///*wangrui add a fun begin 20081208 */ 	

    param_ptr->present = fmt_msdc_present;
    param_ptr->type = fmt_msdc_type;

    ilm_ptr->msg_id = MSG_ID_MMI_FMT_GET_MSDC_STATUS_RSP;

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);

    return;
}

    #ifdef __FS_MOVE_SUPPORT__
/*****************************************************************************
* FUNCTION
*	fmt_copy_hdlr
* DESCRIPTION
*	copy request handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_copy_hdlr(local_para_struct *local_para_ptr,
                   peer_buff_struct *peer_buff_ptr)
{

    mmi_fmt_copy_req_strcut *msg_ptr;
    mmi_fmt_copy_rsp_strcut *param_ptr;
    ilm_struct *ilm_ptr = NULL;
    kal_int32 fs_ret;
    TBM_ENTRY(0x284F);
    TRACE_FMGR_FUNCTION();

    msg_ptr = (mmi_fmt_copy_req_strcut *)local_para_ptr;
    fmt_need_abort = KAL_FALSE;

    if ( msg_ptr->action == FMT_COPY )
    {
         INT32 iCpyRet = FS_ERROR_RESERVED;
         INT64  size = 0;
         FS_DiskInfo diskInfo;
         iCpyRet = MMI_FS_GetDiskInfo(msg_ptr->dest_file, &diskInfo, FS_DI_FREE_SPACE);

         if (iCpyRet < 0)
         {
             ilm_ptr = allocate_ilm( MOD_FMT );

             param_ptr = (mmi_fmt_copy_rsp_strcut *)OslConstructDataPtr(sizeof (mmi_fmt_copy_rsp_strcut) );

             param_ptr->result =  FS_ERROR_RESERVED;

             ilm_ptr->msg_id = MSG_ID_MMI_FMT_COPY_RSP;

             ilm_ptr->peer_buff_ptr = NULL;
             ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

             SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);
             TBM_EXIT(0x284F);
             return ;
         }
         
      		mmi_trace(1,"gdm:in fmt_copy_hdlr(), free=0x%x", (U32)diskInfo.FreeClusters);
         size = diskInfo.FreeClusters -MMI_FS_GetFileSizeByName(msg_ptr->src_file);
         if (size < 0)
         {
             mmi_trace(1, " MMI_FS_Copy no enough space.");
             ilm_ptr = allocate_ilm( MOD_FMT );

             param_ptr = (mmi_fmt_copy_rsp_strcut *)OslConstructDataPtr(sizeof (mmi_fmt_copy_rsp_strcut) );

             param_ptr->result = FS_DISK_FULL;

             ilm_ptr->msg_id = MSG_ID_MMI_FMT_COPY_RSP;

             ilm_ptr->peer_buff_ptr = NULL;
             ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;


             SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);
             TBM_EXIT(0x284F);
             return ;
         }
       //fmt_need_abort = KAL_FALSE;
    	 fs_ret = MMI_FS_Move(msg_ptr->src_file, msg_ptr->dest_file, FS_MOVE_COPY, NULL,1);       
    }
    
    else
    {      
       fs_ret = MMI_FS_Move(msg_ptr->src_file, msg_ptr->dest_file, FS_MOVE_KILL, NULL,1);
       if (ERR_FS_FILE_EXIST == fs_ret)
       {
         fs_ret = FS_FILE_EXISTS;       
       }
    }

	
    // OslFreeDataPtr(msg_ptr); //OK
    /* First allocate a memory for an interlayer message structure */
    ilm_ptr = allocate_ilm( MOD_FMT );

    param_ptr = (mmi_fmt_copy_rsp_strcut *)OslConstructDataPtr(sizeof (mmi_fmt_copy_rsp_strcut) );

    param_ptr->result = fs_ret;

    ilm_ptr->msg_id = MSG_ID_MMI_FMT_COPY_RSP;

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);
    TBM_EXIT(0x284F);	
    return;
}

/*****************************************************************************
* FUNCTION
*	fmt_copy_rsp_hdlr
* DESCRIPTION
*	call back function for FS_Move
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
int fmt_copy_rsp_hdlr(int action, UINT total, UINT completed, UINT handle)
{
    if ( fmt_need_abort == KAL_TRUE )
    {
        if ( handle > 0 )
        {
            fmt_need_abort = KAL_FALSE;
            MMI_FS_Abort (handle);
        }
    }
    else
    {
        /* send message to MMI to show progress */
    }
    return KAL_TRUE;
}
    #endif

/*****************************************************************************
* FUNCTION
*	fmt_abort_copy
* DESCRIPTION
*	abort copy by MMI
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_abort_copy (void)
{
   fmt_need_abort = KAL_TRUE;
}

/*****************************************************************************
* FUNCTION
*	fmt_get_abort_copy
* DESCRIPTION
*	abort copy by MMI
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
kal_bool fmt_get_abort_copy (void)
{
    return fmt_need_abort;
}

/*****************************************************************************
* FUNCTION
*	fmt_format_hdlr
* DESCRIPTION
*	format request handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_format_hdlr(local_para_struct *local_para_ptr,
                     peer_buff_struct *peer_buff_ptr)
{
    mmi_fmt_format_req_strcut *msg_ptr;
    ilm_struct *ilm_ptr = NULL;
    kal_int32 fs_ret;
    mmi_fmt_format_rsp_strcut *param_ptr;

    TRACE_FMGR_FUNCTION();
    msg_ptr = (mmi_fmt_format_req_strcut *)local_para_ptr;
  
    TRACE_EF(g_sw_FMGR, "fmt_format_hdlr msg_ptr->drive =%c",msg_ptr->drive);

    fs_ret = MMI_FS_GeneralFormat(&(msg_ptr->drive), msg_ptr->level, NULL);

    //send rep message to MMI.

    ilm_ptr = allocate_ilm( MOD_FMT );


    param_ptr = (mmi_fmt_format_rsp_strcut *)OslConstructDataPtr (sizeof (mmi_fmt_format_rsp_strcut) );

    param_ptr->result = fs_ret;

    ilm_ptr->msg_id = MSG_ID_MMI_FMT_FORMAT_RSP;

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);

#if 0    

    if ( msg_ptr->drive != 0 )
    {
#if (defined (MMS_SUPPORT) && !defined (APP_STORAGE_IN_SYS_DRV))
        kal_int32 ch;

        ch = MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
        /* not allow format public drive before WAP ready */
        if ( (ch != msg_ptr->drive) && MSF_wapIsInit() == KAL_TRUE )
        {
            fs_ret = FS_DEVICE_BUSY;
        }
        else
#endif         
        {
            kal_uint8 drive[20];

            kal_wsprintf((WCHAR*)drive, "\\\\.\\%c:", msg_ptr->drive);
            fs_ret = MMI_FS_GeneralFormat((UINT8 *)drive, msg_ptr->level,
                                          NULL);
        }
    }
    else
    {
        fs_ret = FS_DRIVE_NOT_FOUND;
    }

    /* First allocate a memory for an interlayer message structure */
    ilm_ptr = allocate_ilm( MOD_FMT );

#ifdef WAP_SUPPORT
    if ( fs_ret >= FS_NO_ERROR && is_app_present(APP_WAP) )
    {
        mmi_fmt_format_req_strcut *param_ptr;

        param_ptr = (mmi_fmt_format_req_strcut *)OslConstructDataPtr
                    (sizeof (mmi_fmt_format_req_strcut) );

        param_ptr->drive = msg_ptr->drive;
        param_ptr->level = msg_ptr->level;

        ilm_ptr->msg_id = MSG_ID_MMI_FMT_FORMAT_REQ;

        ilm_ptr->peer_buff_ptr = NULL;
        ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

        SEND_ILM( MOD_FMT, MOD_WAP, MMI_L4C_SAP, ilm_ptr);
    }
    else
#endif


    
    {
        TRACE_FMGR_FUNCTION();
        mmi_fmt_format_rsp_strcut *param_ptr;

        param_ptr = (mmi_fmt_format_rsp_strcut *)OslConstructDataPtr
                    (sizeof (mmi_fmt_format_rsp_strcut) );

        param_ptr->result = fs_ret;

        ilm_ptr->msg_id = MSG_ID_MMI_FMT_FORMAT_RSP;

        ilm_ptr->peer_buff_ptr = NULL;
        ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

        SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);
    }
#endif
#ifdef __NATIVE_JAVA__
	gcj_creat_java_dir();
#endif
    return;
}

/*****************************************************************************
* FUNCTION
*	fmt_format_done_hdlr
* DESCRIPTION
*	format done handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_format_done_hdlr(local_para_struct *local_para_ptr,
                          peer_buff_struct *peer_buff_ptr)
{
    mmi_fmt_format_rsp_strcut *msg_ptr;
    mmi_fmt_format_rsp_strcut *param_ptr;
    ilm_struct *ilm_ptr = NULL;

    msg_ptr = (mmi_fmt_format_rsp_strcut *)local_para_ptr;

    /* First allocate a memory for an interlayer message structure */
    ilm_ptr = allocate_ilm( MOD_FMT );

    param_ptr = (mmi_fmt_format_rsp_strcut *)OslConstructDataPtr
                (sizeof (mmi_fmt_format_rsp_strcut));

    param_ptr->result = msg_ptr->result;

    ilm_ptr->msg_id = MSG_ID_MMI_FMT_FORMAT_RSP;

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);

    return;
}

    #ifdef __FS_CHECKDRIVE_SUPPORT__
/*****************************************************************************
* FUNCTION
*	fmt_check_drive_hdlr
* DESCRIPTION
*	check drive request handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_check_drive_hdlr(local_para_struct *local_para_ptr,
                          peer_buff_struct *peer_buff_ptr)
{
    mmi_fmt_check_drive_req_strcut *msg_ptr;
    mmi_fmt_check_drive_rsp_strcut *param_ptr;
    ilm_struct *ilm_ptr = NULL;
    kal_int32 fs_ret=0;

    msg_ptr = (mmi_fmt_check_drive_req_strcut *)local_para_ptr;

    if ( msg_ptr->drive != 0 )
    {
        kal_int32 len;

        len = MMI_FS_GetCheckBufferSize(msg_ptr->drive);
        if ( len > FMT_MAX_BUFFER_SIZE )
            len = FMT_MAX_BUFFER_SIZE;
        fs_ret = MMI_FS_CheckDrive(msg_ptr->drive, g_fmt_buffer, len);
    }
    else
    {
        fs_ret = FS_DRIVE_NOT_FOUND;
    }

    /* First allocate a memory for an interlayer message structure */
    ilm_ptr = allocate_ilm( MOD_FMT );

    param_ptr = (mmi_fmt_check_drive_rsp_strcut *)OslConstructDataPtr
                (sizeof (mmi_fmt_check_drive_rsp_strcut) );

    param_ptr->result = fs_ret;

    ilm_ptr->msg_id = MSG_ID_MMI_FMT_CHECK_DRIVE_RSP;

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);

    return;
}
    #endif

/*****************************************************************************
* FUNCTION
*	fmt_delete_hdlr
* DESCRIPTION
*	delete request handler
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
void fmt_delete_hdlr(local_para_struct *local_para_ptr,
                     peer_buff_struct *peer_buff_ptr)
{
    
    mmi_fmt_delete_req_strcut *msg_ptr;
    mmi_fmt_delete_rsp_strcut *param_ptr;
    ilm_struct *ilm_ptr = NULL;
    kal_int32 fs_ret=0;
    TBM_ENTRY(0x2853);


    msg_ptr = (mmi_fmt_delete_req_strcut *)local_para_ptr;
    TRACE_EF(g_sw_FMGR, "################ fmt_delete_hdlr(),msg_ptr->type is %x",msg_ptr->type);
    if ( msg_ptr->type == FS_RECURSIVE_TYPE )
    {
        fs_ret = MMI_FS_RemoveDir((UINT8*)msg_ptr->path);

    }
    else if ( msg_ptr->type == FS_DIR_TYPE )
    {
        if ( FMGR_FILTER_IS_SET(((FMGR_FILTER*)(msg_ptr->filter)), FMGR_TYPE_ALL) )
        {
            //fs_ret = MMI_FS_RemoveDir((UINT8*)msg_ptr->path);
            fs_ret = MMI_FS_XDelete((UINT8*)msg_ptr->path, 0);

        }
        else
        {
            fs_ret = fmt_delete_by_filter_hdlr(msg_ptr->path, (FMGR_FILTER*)msg_ptr->filter);

	}
    }
    else if ( msg_ptr->type == FS_FILE_TYPE )
    {
        fs_ret = MMI_FS_Delete((UINT8*)msg_ptr->path);
    }

    /* First allocate a memory for an interlayer message structure */

    ilm_ptr = allocate_ilm( MOD_FMT );
    param_ptr = (mmi_fmt_delete_rsp_strcut *)OslConstructDataPtr
                (sizeof (mmi_fmt_delete_rsp_strcut) );

    TRACE_EF(g_sw_FMGR, "################ fmt_delete_hdlr() ,msg_ptr->result = %d", fs_ret);

    param_ptr->result = fs_ret;

    ilm_ptr->msg_id = MSG_ID_MMI_FMT_DELETE_RSP;

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);

    TRACE_EF(g_sw_FMGR, "################ fmt_delete_hdlr()");
#ifdef __NATIVE_JAVA__
    gcj_creat_java_dir();
#endif
    TBM_EXIT(0x2853);
    return;

#if 0
    mmi_fmt_delete_req_strcut *msg_ptr;
    mmi_fmt_delete_rsp_strcut *param_ptr;
    ilm_struct *ilm_ptr = NULL;
    kal_int32 fs_ret=0;

    msg_ptr = (mmi_fmt_delete_req_strcut *)local_para_ptr;

    if ( msg_ptr->type == FS_RECURSIVE_TYPE )
    {
        fs_ret = MMI_FS_XDelete((UINT8*)msg_ptr->path, FS_FILE_TYPE | FS_DIR_TYPE | FS_RECURSIVE_TYPE);
    }
    else if ( msg_ptr->type == FS_DIR_TYPE )
    {
        if ( FMGR_FILTER_IS_SET(((FMGR_FILTER*)(msg_ptr->filter)), FMGR_TYPE_ALL) )
            fs_ret = MMI_FS_XDelete ((UINT8*)msg_ptr->path, FS_FILE_TYPE);
        else
            fs_ret = fmt_delete_by_filter_hdlr(msg_ptr->path, (FMGR_FILTER*)msg_ptr->filter);
    }
    else if ( msg_ptr->type == FS_FILE_TYPE )
    {
        fs_ret = MMI_FS_Delete((UINT8*)msg_ptr->path);
    }

    fmt_need_abort = KAL_FALSE;
    /* First allocate a memory for an interlayer message structure */
    ilm_ptr = allocate_ilm( MOD_FMT );

    param_ptr = (mmi_fmt_delete_rsp_strcut *)OslConstructDataPtr
                (sizeof (mmi_fmt_delete_rsp_strcut) );

    param_ptr->result = fs_ret;

    ilm_ptr->msg_id = MSG_ID_MMI_FMT_DELETE_RSP;

    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr = (local_para_struct *)param_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);

    return;

#endif
}
/*****************************************************************************
* FUNCTION
*	fmt_delete_by_filter_hdlr
* DESCRIPTION
*	delete file according to filter
* PARAMETERS
*	None
* RETURNS
*	None.
* GLOBALS AFFECTED
*	None.
*****************************************************************************/
kal_int32 fmt_delete_by_filter_hdlr(kal_uint8 *path, FMGR_FILTER *filter_mask)
{
    return MMI_FS_XDelete(path, 0);
#if 0    
    FS_HANDLE       handle;
    FS_DOSDirEntry  file_info;
    kal_uint8       file_name[40];
    kal_int32       fs_ret;
    kal_uint8       i;

    /*WUZC Modify Start For MMIFS Ver:    on 2007-1-12 18:21 */
    //kal_wstrcat((UINT8*)path, (UINT8*)L"*.*");
    strcat(path, "*.*");
    /*WUZC Modify End  For MMIFS Ver:    on 2007-1-12 18:21 */

    handle = MMI_FS_FindFirst((UINT8 *)path, 0, 0, &file_info, (UINT8 *)file_name, 40);
    if ( handle < 0 )
        return FS_NO_ERROR;

    mmi_fmgr_remove_last_dir((kal_char*)path);

    fs_ret = MMI_FS_SetCurrentDir((UINT8 *)path);
    if ( fs_ret < 0 )
        return fs_ret;

    while ( 1 )
    {
        for ( i = 0 ; i < FMGR_MAX_FILTER_COUNT ; i++ )
        {
            if ( FMGR_FILTER_IS_SET(filter_mask, fmgr_filter[i].type) )
            {
                if ( strncmp((char *)file_info.Extension, (char *)fmgr_filter[i].ext, 3) == 0 )
                {
                    fs_ret = MMI_FS_Delete((UINT8 *)file_name);
                    if ( fs_ret < 0 )
                    {
                        MMI_FS_FindClose(handle);
                        return fs_ret;
                    }
                    break;
                }
            }
        }

        fs_ret = MMI_FS_FindNext (handle, &file_info, (UINT8 *)file_name, 40);
        if ( fs_ret < 0 || fmt_need_abort == KAL_TRUE )
            break;
    }

    MMI_FS_FindClose (handle);
    return FS_NO_ERROR;
#endif    
}

    #ifdef __OTG_ENABLE__

extern kal_uint8 USBHost_Media_Handle[];
extern FS_Driver FS_OTGDrv;

void fmt_otg_insert_hdlr(local_para_struct *local_para_ptr,
                         peer_buff_struct *peer_buff_ptr)
{
    kal_int32 fs_ret;
    void *array[10];
    int i, done;

    /* IT's hard code HERE */
    for ( i=0; i<4; i++ )
    {
        array[i] = &(USBHost_Media_Handle[i]);
    }
    fs_ret = MMI_FS_ConfigExtDevice( FS_SETUP_EXT_DEVICE, &FS_OTGDrv, 4, array, &done);
    kal_prompt_trace(MOD_FMT, "fmt call FS_ConfigExtDevice get (%d), done = %d \n", fs_ret, done);
}

void fmt_otg_remove_hdlr(local_para_struct *local_para_ptr,
                         peer_buff_struct *peer_buff_ptr)
{
    kal_int32 fs_ret;
    void *array[10];
    int i, done;

    /* IT's hard code HERE */
    for ( i=0; i<4; i++ )
    {
        array[i] = &(USBHost_Media_Handle[i]);
    }
    fs_ret = MMI_FS_ConfigExtDevice( FS_RELEASE_EXT_DEVICE, &FS_OTGDrv, 4, array, &done);
    kal_prompt_trace(MOD_FMT, "fmt call FS_ConfigExtDevice get (%d), done = %d \n", fs_ret, done);
}

void fmt_otg_display_hdlr(local_para_struct *local_para_ptr,
                          peer_buff_struct *peer_buff_ptr)
{
    ilm_struct *ilm_ptr = NULL;

    hold_local_para(local_para_ptr);

    ilm_ptr = allocate_ilm( MOD_FMT );
    ilm_ptr->msg_id = MSG_ID_FMT_MMI_OTG_DISPLAY_IND;
    ilm_ptr->peer_buff_ptr = NULL;
    ilm_ptr->local_para_ptr =  local_para_ptr;

    SEND_ILM( MOD_FMT, MOD_MMI, MMI_L4C_SAP, ilm_ptr);
}
    #endif
#endif
