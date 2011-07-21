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
 *	WapDef.h
 *
 * Description:
 * ------------
 * 	This file is intends for the interface of MMI and WAP
 *	(function prototypes)
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
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/


#ifndef _WAP_PROT_H
#define _WAP_PROT_H

#ifdef WAP_SUPPORT

/*
 * Used wapadp.lib to determine MMI capabilities.
 */
extern int wap_mmi_capability_imelody(void);
extern int wap_mmi_capability_changeable_font(void);
extern int wap_mmi_capability_large_font(void);

#define WAP_IMELODY_SUPPORT()		wap_mmi_capability_imelody()
#define WAP_CHANGEABLE_FONT()		wap_mmi_capability_changeable_font()
#define WAP_LARGE_FONT()			wap_mmi_capability_large_font()
#endif /* WAP_SUPPORT */

/*
 * Start function
 */
extern void goto_mobile_suite_screen(void);
extern void widget_MMI_on_exit_CM (void);
extern void widget_MMI_on_enter_idle_screen (void);
extern int widget_MMI_check_and_display_new_msg(void);
extern int widget_MMI_check_and_display_new_msg_when_keypad_lock(U16 KeyLockLSKStringID, U16 KeyLockLSKIconID, U16 KeyLockRSKStringID, U16 KeyLockRSKIconID);

#ifdef MMS_SUPPORT
extern void goto_mms_screen(void);
extern void mms_content_insert_hdlr(char * content_source);
extern void mms_address_insert_hdlr(char * address);

/*
 * When MMS editor selects the number from the phonebook, 
 * goback the history via theis function
 */
extern void widget_MMI_fullscreen_editor_update_from_phonebook(unsigned char* data);


#endif

#endif /* _WAP_PROT_H */


