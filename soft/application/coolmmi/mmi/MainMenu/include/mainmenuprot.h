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
 * MainMenuProt.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intends for main menu functions prototype
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/

/**********************************************************************************
	Filename:		mainmenu_prot.h
	Author:
	Date Created:	September-07-2002
	Contains:		Main menu application
**********************************************************************************/

void exit_main_menu(void);
void handle_mainmenu_right_softkey_up(void);
void highlight_mainmenu_phonebook(void);
void highlight_mainmenu_messages(void);
void highlight_mainmenu_funandgames(void);
void highlight_mainmenu_organizer(void);
void highlight_mainmenu_settings(void);
void highlight_mainmenu_connect(void);
#ifdef __MMI_MESSAGES_CLUB__
void highlight_mainmenu_msgclub (void);
#endif
void highlight_mainmenu_shortcuts(void);
void highlight_mainmenu_services(void);
void initalize_main_menu_application(void);
void main_menu_shortcut_executer(S32 index);
void goto_main_menu(void);
void exit_main_menu(void);
void highlight_mainmenu_profiles(void);
#ifdef __MMI_MAINLCD_220X176__
void highlight_mainmenu_alarm(void);
void highlight_mainmenu_tools(void);
#endif
#ifdef __T800_MAIN_MENU__
void highlight_mainmenu_callcenter(void);
#endif
void highlight_mainmenu_callhistory(void);

void highlight_mainmenu_multimedia(void);
void highlight_mainmenu_filemngr(void);
void highlight_mainmenu_extra(void);
void EntryMainMultimedia(void);
void EntryMainFileMngr(void);
void ExitMainFileMngr(void);
void EntryMainExtra(void);
extern BOOL is_phb_init_done(void);
	/*wangrui Add for fixbug 10195Start :  on 2008-10-27*/
extern BOOL is_phb_init_Finishi(void);
	/*wangrui Add for fixbug 10195Start end*/

#ifdef __MMI_BI_DEGREE_MAIN_MENU_STYLE__
void EntryMainAVZone(void);
void highlight_mainmenu_avzone(void);
#endif
//void PopulateMainMenuRes(void);


