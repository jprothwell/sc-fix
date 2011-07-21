#ifndef WGUI_CATEGORIES_MEDIAMENU_SCREEN_H
#define WGUI_CATEGORIES_MEDIAMENU_SCREEN_H
#if defined(__SG_MMI_MEDIA_SPECIAL_MENU__)
#define MMI_MEDIA_ICON_X          30//50
#define MMI_MEDIA_ICON_W         40
#define MMI_MEDIA_ICON_H          48
#define MMI_MEDIA_MENU_STR_X          MMI_MEDIA_ICON_X + MMI_MEDIA_ICON_W + 5
#define MMI_MEDIA_MENU_STR_W         UI_DEVICE_WIDTH - MMI_MEDIA_MENU_STR_X - 5
#define MMI_MEDIA_MENU_STR_H          MMI_MEDIA_ICON_H
#define MMI_MEDIA_MENU_LINE_X          MMI_MEDIA_MENU_STR_X
#define MMI_MEDIA_MENU_LINE_Y          150
#define MMI_MEDIA_MENU_HALF_BALL_X          180
#define MMI_MEDIA_MENU_HALF_BALL_Y          68
#define MMI_MEDIA_MENU_HALF_BALL_H          165
#define MMI_MEDIA_MENU_DEFALT_HL_ITEM 	4
#define MMI_MEDIA_MENU_HIGHLIGHT_ITEM_Y		(MMI_MEDIA_MENU_LINE_Y-MMI_MEDIA_ICON_H)

#define MMI_MEDIA_MENU_MAX_NUM         7// MMEDIA_ENUM_TOTAL
#define MAX_SMALL_BALL 			2
#define MAX_BIG_BALL 			2
#define BALL_RECT_X2 			UI_DEVICE_WIDTH - 1
#ifdef __SG_MMI_MEDIA_SPECIAL_MENU_WITH_SOFTKEY__
#define BALL_RECT_Y2			 UI_DEVICE_HEIGHT - MMI_BUTTON_BAR_HEIGHT -1
#else
#define BALL_RECT_Y2			 UI_DEVICE_HEIGHT -1
#endif


typedef struct
{
	S32 highlighted_item;
	U16 media_menu_id[MMI_MEDIA_MENU_MAX_NUM];
	U16 media_select_image_id[MMI_MEDIA_MENU_MAX_NUM];
	U16 media_unselect_image_id[MMI_MEDIA_MENU_MAX_NUM];
	U16 media_select_title_id[MMI_MEDIA_MENU_MAX_NUM];
	void  (*item_display_function)(S32 index);
	void  (*item_highlighted_function)(void* item, S32 index);
}media_fixed_menu_struct;

typedef struct
{
	U16 MenuId;
	U16 ImageId;
	U16 SelImageId;
	U16 strId;
}media_resource_struct;

typedef struct
{
	int ball_x ;//球的中心x坐标
	int ball_y;//球的中心y坐标
	int ball_b;//球的峰值，即x最小值
	int ball_c;//球的运动轴心
	int offset ;
}ball_struct;

typedef struct _media_menu_category_history
{
    U16 history_ID;
    S16 highlighted_item;
} media_menu_category_history;

#define SMALL_BALL_A 			0.03
#define BIG_BALL_A				0.02
#define SMALL_BALL_R 6//小球的半径
#define BIG_BALL_R 11//大球的半径

#endif //__SG_MMI_MEDIA_SPECIAL_MENU__
#endif	//WGUI_CATEGORIES_MEDIAMENU_SCREEN_H



