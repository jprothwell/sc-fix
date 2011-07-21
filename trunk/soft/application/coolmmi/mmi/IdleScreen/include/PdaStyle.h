#ifndef _PDASTYLE_
#define _PDASTYLE_
#include "mmi_features.h"


#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
#include "gui_fixed_menus.h"
#include "IdleAppDef.h"

/*+ zhouqin 2011-02-16 modify for q6 320X240 */
//#if defined(__MMI_SYMBIAN_STYLE_IDLE__)
#define IDLE_NETWORK_GAP   72
//#endif
/*-zhouqin 2011-02-16 modify for q6 320X240 */
typedef struct _symbian_context
{
    U16 pdaMatrixMenu_ID;
    S8* pdaMatrixString;
    U16 pda_Matrix_Icon_ID;
    U16 pda_Matrix_Icon_ani_ID;
    void ( *entryfrn )( void );
}g_Symbian_idle_item;

typedef enum
{
    SYMBIAN_IDLE_PEN_NONE
  , SYMBIAN_IDLE_PEN_LSK
  , SYMBIAN_IDLE_PEN_RSK
  #ifdef __MMI_WGUI_CSK_ENABLE__
  ,SYMBIAN_IDLE_PEN_CSK
  #endif
  , SYMBIAN_IDLE_PEN_OTHER_AREA
} symbian_idle_pen_state_enum;

#ifdef __MMI_MAINLCD_240X320__
#ifdef __FT_MULTI_LANG__
#define LINE1_X         			(16)
#define LINE1_Y         			(74)
#define LINE1_ITEM_NUM              SYMBIAN_IDLE_SHORTCUT_NUM
#define SYMBIAN_MATRIX_WIDTH        (35)
#define SYMBIAN_MATRIX_HEIGHT       (35)
#define LINE1_TOTAL                 LINE1_ITEM_NUM

#define LINE2_X         (5)
#define LINE2_Y         (162)
#define LINE2_ITEM_NUM              (1)
#define LINE2_ITEM_WIDTH          (200)
#define LINE2_ITEM_HEIGHT         (24)
#define LINE2_TOTAL                    LINE1_TOTAL+LINE2_ITEM_NUM

#define LINE3_X         (5)
#define LINE3_Y         (188)
#define LINE3_ITEM_NUM              (1)
#define LINE3_ITEM_WIDTH          (200)
#define LINE3_ITEM_HEIGHT         (24)
#define LINE3_TOTAL                    LINE2_TOTAL+LINE3_ITEM_NUM



#define LINE4_X         (5)
#define LINE4_Y         (214)
#define LINE4_ITEM_NUM              (1)
#define LINE4_ITEM_WIDTH          (200)
#define LINE4_ITEM_HEIGHT         (24)
#define LINE4_TOTAL                    LINE3_TOTAL+LINE4_ITEM_NUM

#define SYMBIAN_TOTAL_ITEMS LINE4_TOTAL

#define SYMBIAN_IDLE_NETWORK_NAME_X (5)
#define SYMBIAN_IDLE_NETWORK_NAME_Y (110)

#define SYMBIAN_IDLE_SIM2_NETWORK_NAME_X (5)
#define SYMBIAN_IDLE_SIM2_NETWORK_NAME_Y (136)

#else
#define LINE1_X         			(16)
#define LINE1_Y         			(74)
#define LINE1_ITEM_NUM              SYMBIAN_IDLE_SHORTCUT_NUM
#define SYMBIAN_MATRIX_WIDTH        (35)
#define SYMBIAN_MATRIX_HEIGHT       (35)
#define LINE1_TOTAL                 LINE1_ITEM_NUM

#define LINE2_X         (5)
#define LINE2_Y         (150)
#define LINE2_ITEM_NUM              (1)
#define LINE2_ITEM_WIDTH          (215)
#define LINE2_ITEM_HEIGHT         (24)
#define LINE2_TOTAL                    LINE1_TOTAL+LINE2_ITEM_NUM

#define LINE3_X         (5)
#define LINE3_Y         (176)
#define LINE3_ITEM_NUM              (1)
#define LINE3_ITEM_WIDTH          (215)
#define LINE3_ITEM_HEIGHT         (24)
#define LINE3_TOTAL                    LINE2_TOTAL+LINE3_ITEM_NUM



#define LINE4_X         (5)
#define LINE4_Y         (202)
#define LINE4_ITEM_NUM              (1)
#define LINE4_ITEM_WIDTH          (215)
#define LINE4_ITEM_HEIGHT         (24)
#define LINE4_TOTAL                    LINE3_TOTAL+LINE4_ITEM_NUM
/*


#define LINE5_X         0
#define LINE5_Y         260-3
#define LINE5_ITEM_NUM              0
#define LINE5_ITEM_WIDTH          MAIN_LCD_DEVICE_WIDTH/LINE5_ITEM_NUM
#define LINE5_ITEM_HEIGHT         MMI_MENUITEM_HEIGHT
#define LINE5_TOTAL                    LINE4_TOTAL+LINE5_ITEM_NUM
*/
#define SYMBIAN_TOTAL_ITEMS LINE4_TOTAL

#define SYMBIAN_IDLE_NETWORK_NAME_X (5)
#define SYMBIAN_IDLE_NETWORK_NAME_Y (120)

#define SYMBIAN_IDLE_SIM2_NETWORK_NAME_X (102)
#define SYMBIAN_IDLE_SIM2_NETWORK_NAME_Y SYMBIAN_IDLE_NETWORK_NAME_Y
#endif
#elif defined (__MMI_MAINLCD_320X240__)
#if defined (__MMI_WIN_STYLE_SUPPORT__)
#define LINE1_X         			(4)
#define LINE1_Y         			(28)
#define LINE1_ITEM_NUM              (SYMBIAN_IDLE_SHORTCUT_NUM)
#define SYMBIAN_MATRIX_WIDTH        (52)
#define SYMBIAN_MATRIX_HEIGHT       (52)
#define LINE1_TOTAL                 LINE1_ITEM_NUM

#define LINE2_X         (5)
#define LINE2_Y         (134)
#define LINE2_ITEM_NUM              (1)
#define LINE2_ITEM_WIDTH          (310)
#define LINE2_ITEM_HEIGHT         (44)
#define LINE2_TOTAL                    LINE1_TOTAL+LINE2_ITEM_NUM

#define LINE3_X         (5)
#define LINE3_Y         (173)
#define LINE3_ITEM_NUM              (1)
#define LINE3_ITEM_WIDTH          (310)
#define LINE3_ITEM_HEIGHT         (30)
#define LINE3_TOTAL                    LINE2_TOTAL+LINE3_ITEM_NUM

#else
#define LINE1_X         			(4)
#define LINE1_Y         			(182)
#define LINE1_ITEM_NUM              (SYMBIAN_IDLE_SHORTCUT_NUM)
#define SYMBIAN_MATRIX_WIDTH        (52)
#define SYMBIAN_MATRIX_HEIGHT       (52)
#define LINE1_TOTAL                 LINE1_ITEM_NUM

#define LINE2_X         (0)
#define LINE2_Y         (25)//(19)
#define LINE2_ITEM_NUM              (1)
#define LINE2_ITEM_WIDTH          (41)
#define LINE2_ITEM_HEIGHT         (51)
#define LINE2_TOTAL                    LINE1_TOTAL+LINE2_ITEM_NUM

#endif

/*
#define LINE5_X         0
#define LINE5_Y         260-3
#define LINE5_ITEM_NUM              0
#define LINE5_ITEM_WIDTH          MAIN_LCD_DEVICE_WIDTH/LINE5_ITEM_NUM
#define LINE5_ITEM_HEIGHT         MMI_MENUITEM_HEIGHT
#define LINE5_TOTAL                    LINE4_TOTAL+LINE5_ITEM_NUM
*/

#if defined (__MMI_WIN_STYLE_SUPPORT__)
#define SYMBIAN_TOTAL_ITEMS LINE3_TOTAL
#elif defined (__MMI_BLACK_BERRY_IDLE_STYLE__)
#define SYMBIAN_TOTAL_ITEMS LINE2_TOTAL
#else
#define SYMBIAN_TOTAL_ITEMS LINE2_TOTAL
#endif

#if defined (__MMI_WIN_STYLE_SUPPORT__)
#define SYMBIAN_IDLE_NETWORK_NAME_X (10)
#define SYMBIAN_IDLE_NETWORK_NAME_Y (84)

#define SYMBIAN_IDLE_SIM2_NETWORK_NAME_X (10)
#define SYMBIAN_IDLE_SIM2_NETWORK_NAME_Y (106)

#else
#define SYMBIAN_IDLE_NETWORK_NAME_X (5)
#define SYMBIAN_IDLE_NETWORK_NAME_Y (80)

#define SYMBIAN_IDLE_SIM2_NETWORK_NAME_X (102)
#define SYMBIAN_IDLE_SIM2_NETWORK_NAME_Y SYMBIAN_IDLE_NETWORK_NAME_Y

#endif

#define SYMBIAN_IDLE_MENU_NAME_X (5)
#define SYMBIAN_IDLE_MENU_NAME_Y (158)
#define SYMBIAN_IDLE_MENU_NAME_W (310)
#define SYMBIAN_IDLE_MENU_NAME_H (24)
#endif

#define  PDA_IDLESCREEN_TEXT_GAP  5
extern U16 Symbian_unread_msg_num( void );

extern void ShowCategorySymbianScreen( U16 left_softkey
                                     , U16 left_softkey_icon
                                     , U16 right_softkey
                                     , U16 right_softkey_icon
                                     , S32 number_of_items
                                     , g_Symbian_idle_item* pda_idle_shortcut
                                     , S32 highlighted_item
                                     , U8* history_buffer );

extern void RedrawCategorySymbianScreen( void );
extern void ExitCategorySymbianScreen( void );
extern void Gui_Symbian_Idlescreen( void );
extern void Gui_Symbian_goto_next_item( fixed_matrix_menu* m );
extern void Gui_Symbian_goto_N_item( fixed_matrix_menu* m );

extern void Gui_Symbian_goto_previous_item( fixed_matrix_menu* m );
extern void Gui_Symbian_goto_previous_row( fixed_matrix_menu* m );
extern void Gui_Symbian_goto_next_row( fixed_matrix_menu* m );
extern void Gui_Symbian_menu_locate_highlighted_item( fixed_matrix_menu* m );
extern void Gui_Symbian_goto_item( fixed_matrix_menu* m, S32 i );

extern void Gui_Symbian_Show_Idlescreen( fixed_matrix_menu* m );
extern void configure_Symbian_Idlescreen( S32 item_width, S32 item_height, S32 n_columns, S32 n_rows );
extern void Gui_Symbian_goto_item_no_redraw( S32 item_index );
extern void Gui_Symbian_highlight_handler( S32 item_index );

extern void initial_Symbian_Info( g_Symbian_idle_item* pda_idle_shortcut );
extern void Gui_Symbian_custom_highlight_handler( S32 index );

extern void EntryScrProfilesToneSetup( void );
extern void mmi_msg_entry_write_msg( void );
extern void EntryAlmMenu( void );
extern void EntryScrProfiles( void );
extern void mmi_vdoply_entry_app( void );
extern void mmi_game_entry_app_screen( void );
extern void ReqReadMissedNumByKey( void );
extern void mmi_msg_pre_entry_inbox_list( void );
extern void mmi_vdorec_entry_app( void );
extern void mmi_camera_entry_app_screen( void );

extern BOOL mmi_audply_is_active( void );
extern void idle_screen_sat_display_text_background_redraw( S32 x1, S32 y1, S32 x2, S32 y2 );

extern void Gui_Symbian_show_icontext_menuitem( void* item, void* common_item_data, S32 x, S32 y );

extern void mmi_PdaStyleRegisterPenHandle( void );

extern void EntrySymbianMatrix1( void );
extern void EntrySymbianMatrix2( void );
extern void EntrySymbianMatrix3( void );
extern void EntrySymbianMatrix4( void );
extern void EntrySymbianMatrix5( void );
extern void EntrySymbianMatrix6( void );
extern void EntrySymbianSetting1( void );
extern void EntrySymbianSetting2( void );
extern void EntrySymbianSetting3( void );
extern void EntrySymbianSetting4( void );
extern void EntrySymbianSetting5( void );
extern void Symbian_idle_screen_show_network_name(void);
extern void resetSymbianIdleHilightItem(void);
extern S32 getSymbianIdleHilightItem(void);
extern void setSymbianIdleHilightItem(S32 index);


#endif//_PDASTYLE_
#endif

