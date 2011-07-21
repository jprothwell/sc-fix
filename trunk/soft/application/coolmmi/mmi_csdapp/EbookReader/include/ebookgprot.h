#ifndef _PIXCOM_EBOOKGPROT_H
#define _PIXCOM_EBOOKGPROT_H

#include "filemgr.h"
#ifdef __MMI_BPP_SUPPORT__ /*BPP revise*/
#include "btmmibppspgprot.h"
#endif

#define     EBR_RECORD_SIZE_MAX              4096
#define     EBR_FRM_MAX_BOOKMARK_NAME_SIZE      14
#define     EBR_MAX_STR_SEARCH_LENGTH           24
#define     EBR_DISPLAY_INFO_DATA               20
#define     EBR_ST_NOTIFYDURATION               1000

typedef enum
{
    EBR_CARD_MEMORY = 0,
    EBR_PHONE_MEMORY,
/*+ zhouqin modify for dual t-flash card 20110503*/
#if defined(DUAL_TFLASH_SUPPORT)
    EBR_SEC_CARD_MEMORY,
#endif
/*- zhouqin modify for dual t-flash card 20110503*/
    EBR_MAX_MEMORY
} ebr_memory_type_enum;

typedef enum
{
    EBR_APP_CASE_SENSITIVE,
    EBR_APP_CASE_IN_SENSITIVE
} ebr_app_search_type_enum;

typedef enum
{
    EBR_JUMP_PERCENTAGE = -1,
    EBR_JUMP_BEGIN = 0,
    EBR_JUMP_MIDDLE,
    EBR_JUMP_END
} ebr_jump_type_enum;

typedef struct
{
    S32 inlineHighligtItem;
    S32 inlineHighligtItemFontStyle;
    S32 inlineHighligtItemFontSize;
    S32 inlineHighligtItemScrollBy;
    S32 inlineHighligtItemAutoScroll;
    S32 inlineHighligtItemScrollSpeed;
    S32 inlineHighligtItemFulScreenMode;
    S32 inlineHighligtItemEncodingFormat;
    S32 inlineHighlightItemAlignmentStyle;
    S32 inlineHighligtItemSelectMemory;    
    pBOOL is_usb_plugged_in;
} EBOOKSET_CNTX;

extern void highlight_mainmenu_ebook(void);
extern void ebr_frm_fmgr_set_nfo_name(U8 *nfo_name);
extern void ebr_app_change_drive_type(void);
extern void fmgr_ebook_delete_highlight(void);
extern void fmgr_display_ebook_delete_popup(void);
extern S16 ebr_frm_fmgr_get_application_type(void);
extern void ebr_fmgr_move_to_ebook_folder(void);
extern void ebr_app_display_pop_up_to_change_memory(ebr_memory_type_enum memory_selected);
extern void ebr_fmgr_entry_open(void);
extern void fmgr_move_ebook_file(void);
extern void ebr_fmgr_delete_nfo_file_and_move(void);
extern void fmgr_ebook_single_delete(void);
extern void ebr_fmgr_open_ebook(void);
extern void ebr_fmgr_delete_file(S8 *filepath, U8 deletetype);
extern void ebr_fmgr_unmount_delete_screen(void);
extern void ebr_app_delete_all_ebook_relavant_screen(void);
extern void highlight_fmgr_open_ebook(void);
extern void fmgr_ebook_move_rsp(void *info);
extern U8 ebr_frm_fmgr_is_moving_file(void);
extern void ebr_frm_fmgr_reset_for_new_file(void);
extern void ebr_entry_book_shel_screen(void);
extern void ebr_exit_book_shel_screen(void);
extern void ebr_frm_fmgr_delete_nfo_id_for_new_file(void);
extern S8 *ebr_get_book_mark_name(void);
extern void entry_duplicate_book_mark_pop_up(void);
extern void ebr_app_display_pop_up_maximum_book_marks(void);
extern void ebr_app_display_pop_up_book_mark_sucessfully_added(void);
extern void ebr_frm_check_is_ebook_folder_created(void);

extern void ebr_frm_init(void);
extern S32 ebr_frm_get_current_recursive_conversion_state(void);
extern void ebr_frm_change_default_drive(S8);
extern void ebr_frm_convert_record_record_recursively(void);
extern void ebr_frm_convert_record_record_for_bpp(void);
extern void ebr_frm_register_recursive_callback_fn(void (*fn) (void));
extern void ebr_frm_set_current_recursive_conversion_state(S32 state);
extern void ebr_frm_stop_recursion(void);

extern int ebr_frm_get_drive_status(void);
extern void ebr_app_display_pop_up_book_mark_name_empty(void);
extern pBOOL ebr_frm_get_auto_scroll_status(void);
extern S16 get_highlight_id_from_nvram(void);
extern void ebr_frm_reset_highlight_id(void);
extern S32 ebr_frm_get_page_size(void);
extern S32 ebr_frm_get_book_size_from_nfo(void);
extern void ebr_frm_set_file_name(void);
extern void ebr_frm_set_src_application_type(void);
extern void ebr_frm_fmgr_move_ebook_file(void);
extern void ebr_frm_set_file_info_from_fmgr(S8 *file_name, S8 *file_path, S8 *file_ext, BOOL is_short);
extern void ebr_app_add_to_book_shelf(void);
extern void mmi_ebook_usb_mode_on(void);
extern void mmi_ebook_usb_mode_off(void);
extern void ebr_display_error_pop_up(S32 result);
extern void ebr_entry_new_book_name(void);
extern S32 ebr_app_check_nfo_from_fmgr(S8 *file_name, S8 *file_path, S8 *file_ext, BOOL is_short);
extern void ebr_frm_open_book_from_file_manager(S8 *file_name, S8 *file_path, U16 icon_id, BOOL is_short, BOOL entry_from_myfavorite);
extern void ebr_please_wait_actual_screen(void);
extern void ebr_file_move_wait_screen(void);
extern void exit_file_move_wait_screen(void);

extern void ebr_please_wait_actual_screen_2(void);

extern void ebr_frm_fmgr_set_drive_type(ebr_memory_type_enum drive_type);
extern U16 mmi_ebook_get_menuId(void);
extern void ebr_frm_get_and_set_current_page_values(void);
#ifdef __MMI_INDIC_ALG__
extern pBOOL ebr_frm_check_for_full_characters(S16 *pattern);
#endif 

extern pBOOL ebr_frm_check_file_validity(S8 *file_path, S8 *file_name);

extern pBOOL ebr_frm_check_drive_space(void);
extern void ebr_app_dispaly_less_memory_pop_up(void);
extern void ebr_app_display_pop_up_error_opening_book(void);

extern void ebr_please_wait_move_screen(void);
extern void ebr_app_clear_all_key_handlers(void);
extern S32 ebr_frm_get_book_temp_size_from_nfo(void);
extern pBOOL ebr_frm_check_nfo_file_size(void);

extern void ebr_fmgr_kernel_hide_ext_name(S8 *str_buf_ptr, FMGR_FILE_INFO_STRUCT *info_ptr);
extern void ebr_frm_remove_last_directory(void);
extern pBOOL ebr_frm_check_file_exist(void);
extern MMI_BOOL mmi_ebr_is_memory_type_changed(S8 memoryType);
extern void ebr_frm_get_last_search_string(S8** last_search_string);
extern MMI_BOOL mmi_ebr_is_short_file_name(void);
extern void gui_paging_reset_auto_scroll(void);  

/*BPP revise*/
#endif /* _PIXCOM_EBOOKGPROT_H */ 

