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
#include "mmi_features.h"
#include "fontres.h"
#include "pixcomfontengine.h"

/*	Default fonts								*/
stFontAttribute		UI_DEFAULT_FONT={	0,0,0,MEDIUM_FONT,0,1	};
/*	MMI fonts									*/
stFontAttribute		MMI_small_font=			{	0,0,0,SMALL_FONT,0,1	};
stFontAttribute		MMI_medium_font=		{	0,0,0,MEDIUM_FONT,0,1	};
stFontAttribute		MMI_medium_bold_font=	{	1,0,0,MEDIUM_FONT,0,1	};
//CSD Add By Elvis for dialling font
#ifdef __MMI_DIALLING_FONT__
stFontAttribute     MMI_large_font=         {   0,0,0,MEDIUM_FONT,0,1    };
#else
stFontAttribute		MMI_large_font=			{	0,0,0,LARGE_FONT,0,1	};
#endif
//CSD end
stFontAttribute		MMI_default_font=		{	0,0,0,MEDIUM_FONT,0,1	};

/*	Dialer input box functions	*/
/* font value of dialer input box */
stFontAttribute wgui_dialer_box_f1={	0,0,0,DIALER_FONT,0,1	};
stFontAttribute wgui_dialer_box_f2={	0,0,0,MEDIUM_FONT,0,1	};
/*	stFontAttribute wgui_dialer_box_f3={	0,0,0,SMALL_FONT,0,1	};	*/

stFontAttribute		MMI_sublcd_font=			{	0,0,0,SUBLCD_FONT,0,1	};

stFontAttribute		MMI_virtual_keyboard_font=			{	0,0,0,VIRTUAL_KEYBOARD_FONT,0,1	};
//stFontAttribute		MMI_virtual_keyboard_font=			{	0,0,0,MEDIUM_FONT,0,1	};


