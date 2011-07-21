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
 * IrdaMMIScr.h
 *
 * Project:
 * --------
 *   MMI
 *
 * Description:
 * ------------
 *   
 *
 * Author:
 * -------
 
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * $Log$
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

#ifndef __IRDA_MMI_SCR_H__
#define __IRDA_MMI_SCR_H__

/***************************************************************************** 
* Predefine header checking
*****************************************************************************/
#ifndef _PIXCOM_DATA_TYPES_H
#error "MMI_Data_Types.h should be included"
#endif
#ifndef __MMI_FEATURES__
#error "MMI_features.h should be included"
#endif
#ifndef __IRDA_MMI_GPROTS_H__
#error "IrdaMMIGprots.h should be included"
#endif


#ifdef __MMI_IRDA_SUPPORT__


/***************************************************************************** 
* MMI IRDA Scr Ir port Status
*****************************************************************************/
typedef enum {

   MMIR_SCR_IROPEN = 0,
   MMIR_SCR_IRCLOSE,
   MMIR_SCR_IRPERIOD,

   MMIR_SCR_IRLAST
   
} MMIR_SCR_IRSTATUS;


/***************************************************************************** 
* MMI IRDA Scr Context
*****************************************************************************/
typedef struct {

   /* Total pending received objects counter */
   U32 flag;
   
   MMIR_SCR_IRSTATUS irda_state;
   
} MMI_IRDA_SCR_CONTEX;

extern MMI_IRDA_SCR_CONTEX *mmi_irda_scr_cntx_p;

#define MMI_IRDASCR_CNTX(a) (mmi_irda_scr_cntx_p->a)

/***************************************************************************** 
* MMI IRDA Scr flag defination and operation
*****************************************************************************/
/* MMI IrDA flag bit-wise flag defination */
#define MMIIRDASRC_MASK_RESERVED_0      0x00000001
#define MMIIRDASRC_MASK_RESERVED_1      0x00000002
#define MMIIRDASRC_MASK_RESERVED_2      0x00000004
#define MMIIRDASRC_MASK_RESERVED_3      0x00000008
#define MMIIRDASRC_MASK_RESERVED_4      0x00000010
#define MMIIRDASRC_MASK_RESERVED_5      0x00000020
#define MMIIRDASRC_MASK_RESERVED_6      0x00000040
#define MMIIRDASRC_MASK_RESERVED_7      0x00000080

/* MMI usb context bit-wise flag operation */
#define MMIIRDASRC_GET_FLAG(a)       ( ((mmi_irda_scr_cntx_p->flag) & a) == 0 ? MMI_FALSE : MMI_TRUE )
#define MMIIRDASRC_SET_FLAG(a)       ( (mmi_irda_scr_cntx_p->flag) |=  a )
#define MMIIRDASRC_RESET_FLAG(a)   ( (mmi_irda_scr_cntx_p->flag) &=  ~a )




/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
void mmi_irda_setup_highlight_handler(void);
void mmi_irda_setup_selection_highlight_handler(S32 index);
void mmi_irda_entry_setup_menu(void);
void mmi_irda_exec_open_menu(void);
void mmi_irda_exec_close_menu(void);
void mmi_irda_exec_period_menu(void);
void mmi_irda_scr_data_passing_ind(void);
void mmi_irda_scr_data_stop_ind(void);
void mmi_irda_scr_irda_close_ind(void);
//MMI_IRDA_NFY_TYPE mmi_vobj_irda_receive_ind(void* path_src);
MMI_IRDA_NFY_TYPE mmi_irda_obj_receive_ind(void* path_src, void* filename);

#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__
/*Shantanu-Added for DLT support  */
void mmi_irda_theme_idle_notify_scr(void *,void *);  
int SetIrdaThmDownloadStroageType(void *);
void irda_theme_activate_function(void);
void ActivateThemeDownloadedFromIRDA(void);
/*Shantanu-Added for DLT support  */
#endif

void mmi_irda_notify_scr_goback_func(void);
void mmi_irda_notify_scr_endkey_func(void);
void mmi_irda_notify_scr_browse(void);
//void mmi_irda_notify_scr_browse_cb(void* filename);
void mmi_irda_notify_scr_browse_cb(void* filename, int isShort );


 
void mmi_irda_exit_fmgr_fwd(void);
U8 mmi_irda_sending_screen_del_cb(void *p);
void mmi_irda_fmgr_forward_to_irda_callback( void *buf_p,  U32 result );
v 

#if defined(__MMI_IRDA_CARD_SUPPORT__) //Priyesh - Memory Card Support
void mmi_irda_entry_setup_menu(void);
void mmi_msg_set_Irda_menu_highlight_handler(void);
void HighlightIrdaActivationHandler(void);
void HighlightIrdaStorageHandler(void);
void mmi_irda_entry_activation_menu(void);
void mmi_irda_entry_storage_menu(void);
void mmi_irda_storage_exec(void);
void HintIrdaActivation(U16);
void HintIrdaStorageLocation(U16);
void mmi_irda_storage_selection_highlight_handler(S32);
#endif //__MMI_IRDA_CARD_SUPPORT__ //Priyesh - Memory Card Support



#endif /* __MMI_IRDA_SUPPORT__ */

#endif /* __IRDA_MMI_SCR_H__ */

