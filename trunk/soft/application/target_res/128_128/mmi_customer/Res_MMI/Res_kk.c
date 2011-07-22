/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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

/**************************************************************

	FILENAME	: Res_.c

  	PURPOSE		: Populate Resource for 

 

 

	DATE		: August-11-2003

**************************************************************/
#include "cs_types.h"
#include "custdatares.h"
#include "mmi_features.h"

#include "stdc.h"
#include "customcfg.h"
#include "custresdef.h"
#include "globalmenuitems.h"
#include "custdatares.h"


#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#ifdef _KK_SUPPORT_

#include "kk_res_iddef.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"
#include "todolistenum.h"

#ifdef _KK_SUPPORT_

//note!!!!!!!!!!!!!!!!!
// for difference prj, pls adjust ../../ depth

#ifdef __MMI_MAINLCD_176X220__
#define VC_ICON_BASE_PATH_VC       "../../../../coolmmi/mmi/kk/res/videochat/img/176X220/"
#endif

#ifdef __MMI_MAINLCD_220X176__
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/220X176/"
#endif

#ifdef __MMI_MAINLCD_240X320__
//#define VC_ICON_BASE_PATH_VC       "..\\\\..\\\\mmi\\\\kk\\\\res\\\\videochat\\\\img\\\\240X320"
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/240X320/"
#endif

#ifdef __MMI_MAINLCD_240X400__
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/240X400/"
#endif

#ifdef __MMI_MAINLCD_320X480__
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/320X480/"
#endif

#ifdef __MMI_MAINLCD_320X240__
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/320X240/"
#endif

#ifdef __MMI_MAINLCD_400X240__
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/240X400/"
#endif

#ifdef __MMI_MAINLCD_480X320__
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/240X400/"
#endif


#ifdef __MMI_MAINLCD_272X400__
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/272X400/"
#endif

#ifdef __MMI_MAINLCD_272X480__
#define VC_ICON_BASE_PATH_VC       "../../coolmmi/mmi/kk/res/videochat/img/272X480/"
#endif
#ifdef _KK_SUPPORT_
#include "kk_res_iddef.h"
#endif
#endif

void PopulateResourcesKK(void)
{
	ADD_APPLICATION_STRING2(VIDEOCHAT_MENU_STRINGID,"KK","KK");

	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_000	,VC_ICON_BASE_PATH_VC"sdefault.gif"," icon sdefault.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_001	,VC_ICON_BASE_PATH_VC"001.gif"," icon 001.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_002	,VC_ICON_BASE_PATH_VC"002.gif"," icon 002.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_CHECKBOX1	,VC_ICON_BASE_PATH_VC"checkbox1.gif"," icon checkbox1.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_CHECKBOX2	,VC_ICON_BASE_PATH_VC"checkbox2.gif"," icon checkbox2.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_WAITING	,VC_ICON_BASE_PATH_VC"waiting.gif"," icon waiting.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_UP_ARROW     ,VC_ICON_BASE_PATH_VC"up_arrow.gif","up_arrow.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_DOWN_ARROW      ,VC_ICON_BASE_PATH_VC"down_arrow.gif","down_arrow.gif");

//bruce0912010
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_SLOGO	,VC_ICON_BASE_PATH_VC"slogo.gif"," icon slogo.gif");
	//ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_LOGO	,VC_ICON_BASE_PATH_VC"\\\\logo.gif"," icon logo.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_KK_TITLE      ,VC_ICON_BASE_PATH_VC"v2_title.gif","v2_title.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_KK_TITLE_MIDDLE      ,VC_ICON_BASE_PATH_VC"v2_title_middle.gif","v2_title_middle.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_KK_TITLE_MINI      ,VC_ICON_BASE_PATH_VC"v2_title_mini.gif","v2_title_mini.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_CH      ,VC_ICON_BASE_PATH_VC"v2_video_title_ch.gif","v2_video_title_ch.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_KK_VIDEO_TITLE_EN      ,VC_ICON_BASE_PATH_VC"v2_video_title_en.gif","v2_video_title_en.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_CHAT_WAITING      ,VC_ICON_BASE_PATH_VC"v2_chat_waitting.gif","v2_chat_waitting.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_CHAT_WAITING_MINI      ,VC_ICON_BASE_PATH_VC"v2_chat_waitting_mini.gif","v2_chat_waitting_mini.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_DURING_TIME      ,VC_ICON_BASE_PATH_VC"v2_during_time.gif","v2_during_time.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_GPRS_SIG00      ,VC_ICON_BASE_PATH_VC"v2_gprs_sig0.gif","v2_gprs_sig0.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_GPRS_SIG11      ,VC_ICON_BASE_PATH_VC"v2_gprs_sig1.gif","v2_gprs_sig1.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_GPRS_SIG22      ,VC_ICON_BASE_PATH_VC"v2_gprs_sig2.gif","v2_gprs_sig2.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_GPRS_SIG33      ,VC_ICON_BASE_PATH_VC"v2_gprs_sig3.gif","v2_gprs_sig3.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_GPRS_SIG44      ,VC_ICON_BASE_PATH_VC"v2_gprs_sig4.gif","v2_gprs_sig4.gif");
	ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_BUDDY_NAME      ,VC_ICON_BASE_PATH_VC"v2_buddy_name.gif","v2_buddy_name.gif");
	//ADD_APPLICATION_IMAGE2(IMG_VIEWCHAT_V2_ONLINE_NUM      ,VC_ICON_BASE_PATH_VC"\\\\v2_online_users.gif","v2_online_users.gif");
ADD_APPLICATION_MENUITEM((MENU_ID_KK_MAINMENU, MAIN_MENU_MULTIMEDIA_MENUID,
	0, SHOW, MOVEABLEACROSSPARENT|SHORTCUTABLE, 1, VIDEOCHAT_MENU_STRINGID, IMG_VIEWCHAT_SLOGO));

}
#endif

#endif


