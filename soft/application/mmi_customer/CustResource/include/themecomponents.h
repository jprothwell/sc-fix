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
extern CONST_HARDWARE U8 _MAINLCD_SUBMENU_SB_BG1_BMP[];
extern CONST_HARDWARE U8 _MAINLCD_SUBMENU_SB_BG2_BMP[];
extern CONST_HARDWARE U8 _MAINLCD_SUBMENU_SB_BG3_BMP[];
extern CONST_HARDWARE U8 _MAINLCD_MAINMENU_MATRIX_MM_BG1_GIF[];
#if !defined(__MMI_RESOURCE_SLIM_ON_128X160__)
extern CONST_HARDWARE U8 _MAINLCD_MAINMENU_MATRIX_MM_BG2_GIF[];
extern CONST_HARDWARE U8 _MAINLCD_MAINMENU_MATRIX_MM_BG3_GIF[];
#endif

//qiff add UI resource,del for too slow display
#if 0 //defined __MMI_MAINLCD_240X320__
extern CONST_HARDWARE U8  __MAINLCD__THEME__SELECT_BG_BMP[];
#endif

extern CONST_HARDWARE U8 _MAINLCD_TITLEBAR_TB_BG_1_GIF[];

extern CONST_HARDWARE U8 IMG_TITLE_1[];
extern CONST_HARDWARE U8 IMG_TITLE_2[];
extern CONST_HARDWARE U8 IMG_TITLE_3[];
//{100,255,255,100}
CONST_HARDWARE UI_filled_area LSK_up_1={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //changede by panxu 2007-3-23 
UI_IMAGE_ID_NULL,
NULL,
{255,255,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area LSK_down_1={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //changede by panxu 2007-3-23 
UI_IMAGE_ID_NULL,
NULL,
{255,255,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area RSK_up_1={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area RSK_down_1={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t LSK_up_txt_col_1={255,255,0,100};

CONST_HARDWARE color_t LSK_down_txt_col_1={255,255,0,100};

CONST_HARDWARE color_t RSK_up_txt_col_1={255,255,0,100};

CONST_HARDWARE color_t RSK_down_txt_col_1={255,255,0,100};

CONST_HARDWARE UI_filled_area mtab_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area ibox_1={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area sbar_bkgrnd_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{60,60,60,100}, //changed by panxu for new images
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area sbar_indicator_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{245,245,245,100}, //changed by lin for theme1@20070817
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};

CONST_HARDWARE UI_filled_area Title_1={
UI_FILLED_AREA_TYPE_BITMAP,
(U8*)_MAINLCD_TITLEBAR_TB_BG_1_GIF,
NULL,
//PMT NEERAJ START 20050616
{240,204,120,100},
//PMT NEERAJ END 20050616
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t Title_text_color_1={255,255,255,100};

CONST_HARDWARE UI_filled_area list_bkgrnd_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{50,50,255,100},
//{0,0,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area list_normal_item_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{91,45,45,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area list_selected_item_1={
//qiff add UI resource,del for too slow display
#if 0 //defined __MMI_MAINLCD_240X320__
UI_FILLED_AREA_TYPE_BITMAP,
(UI_image_type)__MAINLCD__THEME__SELECT_BG_BMP,
#else
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
#endif
NULL,
{160,223,77}, //changed by lin for theme 1 @20070901
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_bkgrnd_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{50,50,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_normal_item_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{91,45,45,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_selected_item_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{252,164,67,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area fibox_normal_1={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area fibox_selected_1={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area mm_bkgrnd_1={
UI_FILLED_AREA_TYPE_BITMAP,
(UI_image_type)_MAINLCD_MAINMENU_MATRIX_MM_BG1_GIF,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area popup_description_bkgrnd_1={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,120,100},
{0,0,0,100},
{252,164,67,100},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area PIN_ibox_bkgrnd_1={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};

CONST_HARDWARE UI_filled_area virtual_keyboard_down_bkgrnd_1={
UI_FILLED_AREA_TYPE_COLOR,
NULL,
NULL,
{0,128,240,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};



CONST_HARDWARE color_t mtab_selector_txt_col_1={91,45,45,100};

CONST_HARDWARE color_t mtab_normal_txt_col_1={91,45,45,100};

CONST_HARDWARE color_t mtab_selected_txt_col_1={255,255,255,100};

CONST_HARDWARE color_t ibox_normal_txt_col_1={0,0,0,100};

CONST_HARDWARE color_t ibox_selected_txt_col_1={255,255,255,100};

CONST_HARDWARE color_t ibox_selector_col_1={91,45,45,100};

CONST_HARDWARE color_t ibox_cursor_col_1={91,45,45,100};

CONST_HARDWARE color_t title_scut_txt_col_1={255,255,0,100};

#ifdef __MMI_MAINLCD_128X160__
CONST_HARDWARE color_t list_normal_txt_col_1={45,45,45,100};
#else
CONST_HARDWARE color_t list_normal_txt_col_1={255,255,255,100};
#endif

#ifdef __MMI_MAINLCD_128X160__
CONST_HARDWARE color_t list_selected_txt_col_1={45,45,45,100};
#else
CONST_HARDWARE color_t list_selected_txt_col_1={0,0,0,100};
#endif

CONST_HARDWARE color_t scut_indicator_txt_col_1={91,45,45,100};

CONST_HARDWARE color_t fibox_selector_col_1={91,45,45,100};

CONST_HARDWARE color_t fibox_normal_txt_col_1={0,0,0,100};

CONST_HARDWARE color_t fibox_selected_txt_col_1={255,255,255,100};

CONST_HARDWARE color_t fibox_cursor_col_1={91,45,45,100};

CONST_HARDWARE color_t popup_description_txt_col_1={91,45,45,100};

CONST_HARDWARE color_t button_bar_col_1={0,0,0,100};

CONST_HARDWARE color_t information_bar_col_1={255,243,102,100};

CONST_HARDWARE color_t input_method_txt_col_1={91,45,45,100};

CONST_HARDWARE color_t virtual_keyboard_key_down_text_col_1={255,255,255,100};

CONST_HARDWARE color_t virtual_keyboard_key_up_text_col_1={0,0,0,100};

CONST_HARDWARE color_t virtual_keyboard_dead_key_down_text_col_1={255,255,255,100};

CONST_HARDWARE color_t virtual_keyboard_dead_key_up_text_col_1={8,100,8,100};

CONST_HARDWARE color_t virtual_keyboard_disp_area_text_col_1={0,0,0,100};

CONST_HARDWARE UI_filled_area LSK_up_2={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area LSK_down_2={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area RSK_up_2={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area RSK_down_2={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t LSK_up_txt_col_2={90,90,90,100};

CONST_HARDWARE color_t LSK_down_txt_col_2={192,192,192,100};

CONST_HARDWARE color_t RSK_up_txt_col_2={90,90,90,100};

CONST_HARDWARE color_t RSK_down_txt_col_2={192,192,192,100};

CONST_HARDWARE UI_filled_area mtab_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area ibox_2={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area sbar_bkgrnd_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{200,227,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area sbar_indicator_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{202,161,45,100},//modified by lin for theme2@20070817
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};

CONST_HARDWARE UI_filled_area Title_2={
UI_FILLED_AREA_TYPE_BITMAP,
(U8*)IMG_TITLE_2,
NULL,
//PMT NEERAJ START 20050616
{192,204,232,100},
//PMT NEERAJ END 20050616
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t Title_text_color_2={255,255,255,100};

CONST_HARDWARE UI_filled_area list_bkgrnd_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{0,0,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area list_normal_item_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{91,45,45,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area list_selected_item_2={
//	PMT VIKAS START 20050525
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{217,184,57,100}, //modified by lin for theme2@20070820
//PMT VIKAS END
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_bkgrnd_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_normal_item_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{91,45,45,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_selected_item_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{67,155,252,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area fibox_normal_2={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area fibox_selected_2={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area mm_bkgrnd_2={
UI_FILLED_AREA_TYPE_BITMAP,
#if defined(__MMI_RESOURCE_SLIM_ON_128X160__)
(UI_image_type)_MAINLCD_MAINMENU_MATRIX_MM_BG1_GIF,
#else
(UI_image_type)_MAINLCD_MAINMENU_MATRIX_MM_BG2_GIF,
#endif
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area popup_description_bkgrnd_2={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{208,224,248,100},
{0,0,0,100},
{67,155,252,100},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area PIN_ibox_bkgrnd_2={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t mtab_selector_txt_col_2={41,69,136,100};

CONST_HARDWARE color_t mtab_normal_txt_col_2={41,69,136,100};

CONST_HARDWARE color_t mtab_selected_txt_col_2={255,255,255,100};

CONST_HARDWARE color_t ibox_normal_txt_col_2={41,69,136,100};

CONST_HARDWARE color_t ibox_selected_txt_col_2={255,255,255,100};

CONST_HARDWARE color_t ibox_selector_col_2={54,91,177,100};

CONST_HARDWARE color_t ibox_cursor_col_2={41,69,136,100};

CONST_HARDWARE color_t title_scut_txt_col_2={255,255,255,100};

CONST_HARDWARE color_t list_normal_txt_col_2={41,69,136,100};

CONST_HARDWARE color_t list_selected_txt_col_2={255,255,255,100};

CONST_HARDWARE color_t scut_indicator_txt_col_2={41,69,136,100};

CONST_HARDWARE color_t fibox_selector_col_2={54,91,177,100};

CONST_HARDWARE color_t fibox_normal_txt_col_2={41,69,136,100};

CONST_HARDWARE color_t fibox_selected_txt_col_2={255,255,255,100};

CONST_HARDWARE color_t fibox_cursor_col_2={41,69,136,100};

CONST_HARDWARE color_t popup_description_txt_col_2={0,0,96,100};

CONST_HARDWARE color_t button_bar_col_2={0,0,0,100};

CONST_HARDWARE color_t information_bar_col_2={206,231,255,100};

CONST_HARDWARE color_t input_method_txt_col_2={41,69,136,100};

CONST_HARDWARE UI_filled_area LSK_up_3={
UI_FILLED_AREA_TYPE_NO_BACKGROUND,//modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area LSK_down_3={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area RSK_up_3={
UI_FILLED_AREA_TYPE_NO_BACKGROUND, //modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area RSK_down_3={
UI_FILLED_AREA_TYPE_NO_BACKGROUND,//modified by lin for theme@20070620
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t LSK_up_txt_col_3={90,90,90,100};

CONST_HARDWARE color_t LSK_down_txt_col_3={192,192,192,100};

CONST_HARDWARE color_t RSK_up_txt_col_3={90,90,90,100};

CONST_HARDWARE color_t RSK_down_txt_col_3={192,192,192,100};

CONST_HARDWARE UI_filled_area mtab_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area ibox_3={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area sbar_bkgrnd_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{238,238,238,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area sbar_indicator_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{162,65,80,100},//modified by lin for theme3@20070817
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};

CONST_HARDWARE UI_filled_area Title_3={
UI_FILLED_AREA_TYPE_BITMAP,
(U8*)IMG_TITLE_3,
NULL,
//PMT NEERAJ START 20050616
{176,164,152,100},
//PMT NEERAJ END 20050616
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t Title_text_color_3={255,255,255,100};

CONST_HARDWARE UI_filled_area list_bkgrnd_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{0,0,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area list_normal_item_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area list_selected_item_3={
//	PMT VIKAS START 20050525
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{217,107,228,100}, //modified by lin for theme3@20070820
//PMT VIKAS END
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_bkgrnd_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_normal_item_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area matrix_selected_item_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{166,159,159,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area fibox_normal_3={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area fibox_selected_3={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area mm_bkgrnd_3={
UI_FILLED_AREA_TYPE_BITMAP,
#if defined(__MMI_RESOURCE_SLIM_ON_128X160__)
(UI_image_type)_MAINLCD_MAINMENU_MATRIX_MM_BG1_GIF,
#else
(UI_image_type)_MAINLCD_MAINMENU_MATRIX_MM_BG3_GIF,
#endif
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area popup_description_bkgrnd_3={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{250,250,250,100},
{0,0,0,100},
{166,159,159,100},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area PIN_ibox_bkgrnd_3={
UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_SINGLE_BORDER,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t mtab_selector_txt_col_3={90,90,90,100};

CONST_HARDWARE color_t mtab_normal_txt_col_3={90,90,90,100};

CONST_HARDWARE color_t mtab_selected_txt_col_3={255,255,255,100};

CONST_HARDWARE color_t ibox_normal_txt_col_3={0,0,0,100};

CONST_HARDWARE color_t ibox_selected_txt_col_3={255,255,255,100};

CONST_HARDWARE color_t ibox_selector_col_3={95,95,95,100};

CONST_HARDWARE color_t ibox_cursor_col_3={90,90,90,100};

CONST_HARDWARE color_t title_scut_txt_col_3={255,255,255,100};

CONST_HARDWARE color_t list_normal_txt_col_3={64,0,0,100};

CONST_HARDWARE color_t list_selected_txt_col_3={255,255,255,100};

CONST_HARDWARE color_t scut_indicator_txt_col_3={90,90,90,100};

CONST_HARDWARE color_t fibox_selector_col_3={95,95,95,100};

CONST_HARDWARE color_t fibox_normal_txt_col_3={0,0,0,100};

CONST_HARDWARE color_t fibox_selected_txt_col_3={255,255,255,100};

CONST_HARDWARE color_t fibox_cursor_col_3={90,90,90,100};

CONST_HARDWARE color_t popup_description_txt_col_3={64,28,23,100};

CONST_HARDWARE color_t button_bar_col_3={0,0,0,100};

CONST_HARDWARE color_t information_bar_col_3={238,238,238,100};

CONST_HARDWARE color_t input_method_txt_col_3={90,90,90,100};

CONST_HARDWARE UI_filled_area Disabled_items={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{0,0,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area Unused={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{0,0,0,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area General_BG={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area inline_edit_noboundry={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE UI_filled_area common_screen_BG={
UI_FILLED_AREA_TYPE_COLOR,
UI_IMAGE_ID_NULL,
NULL,
{255,255,255,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


CONST_HARDWARE color_t Unused_color={0,0,0,100};

CONST_HARDWARE color_t black_color={0,0,0,100};

CONST_HARDWARE color_t white_color={255,255,255,100};

CONST_HARDWARE color_t Disabled_text_col={255,255,255,100};

CONST_HARDWARE color_t Title_text_border_col={0,0,0,100};

CONST_HARDWARE color_t Button_bar_col={192,192,192,100};

CONST_HARDWARE color_t Status_bar_col={255,255,255,100};

CONST_HARDWARE UI_filled_area dial_inputbox_bg_filler={
UI_FILLED_AREA_TYPE_COLOR,
NULL,
NULL,
{128,128,128,100},
{0,0,0,100},
{0,0,0,0},
{0,0,0,100},
0};


