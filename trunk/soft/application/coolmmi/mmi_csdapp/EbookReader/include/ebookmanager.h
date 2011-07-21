#ifndef _PIXCOM_EBOOKMANAGER_H
#define _PIXCOM_EBOOKMANAGER_H

#include "filesystemdef.h"
#include "wgui_inputs.h"
#include "mmi_data_types.h"
#include "ebookgprot.h"
#include "conversions.h"

#define     MAX_EBOOK_NAME_LENGTH            FMGR_MAX_PATH_LEN
#ifdef __MMI_MY_FAVORITE__
/* under construction !*/
#else /* __MMI_MY_FAVORITE__ */
#define     EBR_DOWNLOADED_EBOOK_PATH           ":/Ebook/"
#endif /* __MMI_MY_FAVORITE__ */
#define     EBR_EBOOK_SEARCH_PDB_STR            "*.pdb"
#define     EBR_EBOOK_SEARCH_TXT_STR            "*.txt"
#define     EBR_EBOOK_SEARCH_NFO_STR            "*.nfo"
#define     EBR_EBOOK_SEARCH_TXZ_STR            "*.txz"
#define     EBR_EBOOK_SEARCH_PBZ_STR            "*.pbz"
#define     EBR_EBOOK_NFO_FILE_EXT           "nfo"

#define     MAX_DWNL_EBK_PATH_LEN            FMGR_MAX_PATH_LEN + FMGR_MAX_FILE_NAME_LEN
#define     DOC_TYPE                   "TEXtREAd"
#define     EBR_FRM_MAX_NO_OF_BOOKMARKS         10
#define     EBR_PAGE_SIZE                 GUI_PAGE_SIZE
#define     EBR_FRM_RECORD_ENTRY_SIZE        8
#define     COUNT_BITS                    3
#define     EBR_TXT_FOOTER_START          "<PIXTEL_MMI_EBOOK_2005>"
#define     EBR_TXT_FOOTER_END               "</PIXTEL_MMI_EBOOK_2005>"
#define     TXT_FOOTER_START_SIZE            23
#define     TXT_FOOTER_END_SIZE              24
#define     EBR_FRM_DATABASE_HEADER_SIZE     78
#define     FILE_NAME_LENGTH              32
#define     EBR_FILE_TYPE_TXT             "TXT"
#define     EBR_FILE_TYPE_PDB             "PDB"
#define     EBR_EXTENSION_NAME_LENGTH        3
#define     TXT_TEMP_EXT_STR              ".txz"
#define     PDB_TEMP_EXT_STR              ".pbz"
#define     NFO_FILE_NAME_LENGTH       20
#define     NFO_FILE_NAME_VALUE           9
#define     EBR_MAX_OVER_THE_RECORD_BORDER  2


//#define __MMI_EBOOK_SPEED_OPEN__

#ifndef MMI_ON_HARDWARE_P
#define     NVRAM_EF_EBOOK_SETTING_SIZE         93
#endif

#define  SEEK_REC_ENTRY(f,i) \
   MMI_FS_Seek( f, EBR_FRM_DATABASE_HEADER_SIZE + EBR_FRM_RECORD_ENTRY_SIZE * (i), FS_FILE_BEGIN )

#define ntohs(A)  ((((A) & 0xff00) >> 8) | ((A) & 0x00ff) << 8)
#define ntohl(A)  ((((A) & 0xff000000) >> 24) | (((A) & 0x00ff0000) >> 8) | \
      (((A) & 0x0000ff00) << 8) | (((A) & 0x000000ff) << 24))

#define EBR_FRM_SEARCH_BLOCK    5
#define EBR_FRM_SEARCH_BREAK_TIMER    10
#ifdef __MMI_BPP_SUPPORT__ /*BPP revise*/
#define EBR_FRM_BPP_TEMP_FILE   "ebook.bpp"
#endif 

typedef enum
{
    EBR_FRM_FILE_TYPE_PDB = 1,
    EBR_FRM_FILE_TYPE_TXT
} ebr_frm_filetype_enum;

typedef enum
{
    EBR_FRM_SEARCH_DEFAULT = 0,
    EBR_FRM_SEARCH_NFO,
    EBR_FRM_SEARCH_PDB,
    EBR_FRM_SEARCH_TXT,
    EBR_FRM_SEARCH_TXZ,
    EBR_FRM_SEARCH_PBZ,
    EBR_FRM_SEARCH_MAX
} ebr_frm_search_type_enum;

typedef enum
{
    EBR_FRM_COPY_TYPE_FIRST = 1,
    EBR_FRM_COPY_TYPE_SECOND,
    EBR_FRM_COPY_TYPE_PREVIOUS,
    EBR_FRM_COPY_TYPE_NEXT
} ebr_frm_copy_type_enum;

typedef enum
{
    EBR_FRM_SRC_APP_EBOOK,
    EBR_FRM_SRC_APP_FMGR
} ebr_frm_application_type_enum;

typedef enum
{
    EBR_FRM_NFO_CREATE_SUCCESS = 0,
    EBR_FRM_NFO_CREATE_FAILURE = 1,
    EBR_FRM_NFO_CREATE_FILE_TYPE_NOT_SUPPORTED = 2,
    EBR_FRM_EMPTY_BOOK = 3
} ebr_frm_nfo_create_type_errors_enum;

typedef enum
{
    EBR_FRM_NO_CONVERSION = 0,
    EBR_FRM_CONVERSION_UNDERWAY = 1,
    EBR_FRM_NFO_CONVERSION_COMPLETED = 2,
#ifdef __MMI_EBOOK_SPEED_OPEN__
    EBR_FRM_CONVERSION_BACKGROUND = 3,
#endif
} ebr_frm_recursive_type_errors_enum;

typedef enum
{
    EBR_FRM_EXIT_BY_GOBACK_TO_BOOKSHELF = 0,
    EBR_FRM_EXIT_BY_CLOSE_BOOK = 1
} ebr_frm_exit_type_enum;

typedef struct
{
    U32 page_id;
    S32 text_offset;
    U8 shared_bytes;
    U8 last_showed_percentage;
    S32 last_text_offset_y;
    S32 page_character_num;
}ebr_frm_page_info_struct;

typedef struct
{
    U8 scroll_flag;
    U8 scroll_speed;
    U8 scroll_type;
} ebr_frm_auto_scroll_struct;

typedef struct
{
    U8 bookmark_name[(EBR_FRM_MAX_BOOKMARK_NAME_SIZE + 1) *ENCODING_LENGTH];
    U8 percentage_associated_with_book_mark;
	U8 is_full_screen;
    U32 page_id;
    U32 offset;
    U32 shared_bytes;
} ebr_frm_bookmark_struct;

typedef struct
{
    U8 font_style;
    U8 font_size;
    ebr_frm_auto_scroll_struct auto_scroll_settings;
    U8 encoding_format;
    U8 screen_mode;
    U8 alignment_style;
} ebr_frm_common_setting_struct;

typedef struct
{
    ebr_frm_common_setting_struct common_settings;
    S8 memory_type;
    U8 e_book_name[(MAX_EBOOK_NAME_LENGTH + 1) *ENCODING_LENGTH];
} ebr_frm_setting_struct;

typedef struct
{
    U8 e_book_name[(MAX_EBOOK_NAME_LENGTH + 1) *ENCODING_LENGTH];
    U8 temp_file_name[(MAX_DWNL_EBK_PATH_LEN + 1) *ENCODING_LENGTH];
    U8 display_name[(MAX_EBOOK_NAME_LENGTH + 1) *ENCODING_LENGTH];
    U8 last_showed_percentage;
    U8 temp_encoding_file_format;
    U32 current_page_id;
    U32 offset;
    U32 jump_offset;
    ebr_frm_bookmark_struct book_marks[EBR_FRM_MAX_NO_OF_BOOKMARKS];
    ebr_frm_common_setting_struct settings;
    INT32 book_size; /*file size for current encoding type */
    INT32 temp_book_size;
    S8 memory_type;
    S32 last_text_offset_y;
    INT32 fs_file_size; /* file size in file system*/
    U16 is_short;
    S8 checksum[TXT_FOOTER_START_SIZE + 1];
#ifdef __MMI_EBOOK_SPEED_OPEN__
    U8 isConvertCompleted;
    S32 total_records;
    S32 converted_records;	
#endif

} ebr_frm_book_struct;

typedef struct  /* 16 bytes total */
{
    pBOOL version;      /* 1 = plain text, 2 = compressed */
    pBOOL reserved1;
    U32 doc_size;       /* in bytes, when uncompressed */
    pBOOL num_records;  /* PDB header numRecords - 1 */
    pBOOL rec_size;     /* usually RECORD_SIZE_MAX */
    U32 reserved2;
} doc_record0;

typedef struct
{
    S32 gcurrent_record_id;
    S32 gnext_record_id;
    S32 g_start_id;
    S32 g_end_id;
    S32 prev_end_id;
    U16 g_offset;
    U16 g_jump_offset;
    S32 num_records;
    S32 g_last_nfo_id_allocated;
    ebr_frm_filetype_enum gbook_type;
    pBOOL ebr_frm_end_book_flag;
    S32 nfo_id;
    S16 g_fmgr_book_type;
    S8 memory_type_selected;
    S16 src_application;

    U8 ebr_search_strlen;
    pBOOL highlight_string_flag;
    S8 ebr_last_search_string_input[(EBR_MAX_STR_SEARCH_LENGTH + 1) *ENCODING_LENGTH];
    S32 ebr_last_search_record_id;
    S32 ebr_last_offset_string;
    U16 ebr_last_text_offset;
    U32 ebr_last_page_id;
    ebr_frm_book_struct g_nfo_book_struct;

    pBOOL ebr_last_search_type;
    U8 is_moving_file; /* flag to check if it is moving file into ebook folder */

    pBOOL is_book_in_unicode;
    pBOOL book_is_in_other_encoding_format;
    S32 search_record_id_first;
    pBOOL find_next_first_flag;
    S32 current_highlight_book_id;
    pBOOL search_flag_is_on;
    pBOOL find_next_flag;
    pBOOL pdb_converted_to_text;
    pBOOL search_found_flag;
    pBOOL last_record_flag;
    BOOL jump_at_same_page_flag;
    BOOL is_short;
    FS_HANDLE g_file_hnd_offset;
    S32 g_record_id;
    S32 state_recursive_conversion;
    void (*g_recursive_callback_fn) (void);
    pBOOL is_in_valid;
    U8 reset_last_display_info;
    ebr_frm_page_info_struct page_info_struct;

    U8 nfo_name[(MAX_EBOOK_NAME_LENGTH + 1) *ENCODING_LENGTH];
    U8 nfo_path[(MAX_DWNL_EBK_PATH_LEN + 1) * ENCODING_LENGTH];
    U8 e_book_name[(MAX_EBOOK_NAME_LENGTH + 1) *ENCODING_LENGTH];
    U8 book_name_path[(MAX_DWNL_EBK_PATH_LEN + 1) * ENCODING_LENGTH];
    U8 temp_book_name_path[(MAX_DWNL_EBK_PATH_LEN + 1) * ENCODING_LENGTH];
    S8 fmgr_file_name[(FMGR_MAX_FILE_LEN + 1) *ENCODING_LENGTH];
    S8 fmgr_file_extension[(FMGR_MAX_EXT_LEN + 1) *ENCODING_LENGTH];
    S8 fmgr_file_path[FMGR_MAX_PATH_LEN *ENCODING_LENGTH + ENCODING_LENGTH];
    S8 old_file_path[FMGR_MAX_PATH_LEN * ENCODING_LENGTH + ENCODING_LENGTH];
    U8 buffer[EBR_RECORD_SIZE_MAX + EBR_MAX_STR_SEARCH_LENGTH];
    U8 start[(EBR_RECORD_SIZE_MAX + EBR_MAX_STR_SEARCH_LENGTH) *2];
} g_current_book_context;

typedef struct
{
    U32 next_recordlist_id;
    pBOOL num_records;
} ebr_record_list_type;

typedef struct  /* 78 bytes total */
{
    U8 name[FILE_NAME_LENGTH];
    pBOOL attributes;
    pBOOL version;
    U32 creation_date;
    U32 modification_date;
    U32 last_back_up_date;
    U32 modification_number;
    U32 app_info_id;
    U32 sort_info_id;
    U8 type[4];
    U8 creator[4];
    U32 unique_id_seed;
    ebr_record_list_type record_list;
} ebr_frm_database_hdr_type;

extern void ebr_frm_timer_launch_popup(void);
extern void ebr_create_temp_file_for_bpp(void);
extern void ebr_frm_set_page_info(U32 page_id, S32 text_offset, U8 shared_bytes, U8 last_showed_percentage, S32 last_text_offset_y, S32 page_character_num);
extern void ebr_frm_get_page_info(PU32 page_id, PS32 text_offset, PU8 shared_bytes, PU8 last_showed_percentage, PS32 last_text_offset_y, PS32 page_character_num);
extern void ebr_frm_entry_refresh_book_shelf(void);
extern S32 ebr_frm_create_nfo_file(U8 *display_name, U8 *ebook_file_name_with_path, U8 *ebook_file_name);
extern S16 ebr_frm_fill_nfo_file(FS_HANDLE file_hnd, U8 *display_name, U8 *ebook_file_name);
extern S16 ebr_frm_decode_header(U8 *ebook_file_name);
extern void ebr_frm_decompress_pdb(U8 *b, INT32 *size, ebr_frm_copy_type_enum copy_type);
extern void ebr_frm_start_search_timer(void);
extern pBOOL ebr_frm_search_string_algo(ebr_app_search_type_enum search_type, S8 *ebr_search_string_input, MMI_BOOL timeout);
extern S16 ebr_frm_bm_search(S16 *text, S16 *pat, S32 *start_pos,U32 size);
extern void ebr_frm_paging_multiline_input_box_get_page(U32 start_id, U32 end_id, PU8 buffer_local);
extern MMI_BOOL ebr_frm_get_record(S32 end_id);
extern void ebr_frm_get_record_for_bookmark(U32 selected_index);
extern S16 ebr_frm_get_data_to_open_book(gui_page_info_app *, U8 *buffer_data, S8 *book_display_name);
extern void ebr_frm_jump_to_record(U32 percentage_val, ebr_jump_type_enum jump_type);
extern void ebr_frm_convert_to_upper(S16 *decoded_buffer);
extern S16 ebr_frm_pre_create_bookshelf_screen(void);
extern void ebr_frm_remove_bookmark(U32 bookmark_index);
extern void ebr_frm_add_book_mark(void);
extern S32 ebr_frm_delete_ebook(void);
extern void ebr_frm_save_settings_in_nfo_file(void);
extern pBOOL ebr_frm_ebook_exit(ebr_frm_exit_type_enum type);
extern U8 *ebr_frm_get_book_type(void);
extern S16 ebr_get_drive_type(void);
extern void ebr_frm_add_book_mark_pre_entry(void);
extern S32 ebr_frm_read_from_nvram(void);
extern void ebr_frm_rwrite_into_nvram(void);
extern S32 ebr_frm_get_total_countof_nfo_file(void);
extern void ebr_frm_write_settings_in_nvram(void);
extern void ebr_frm_get_default_settings_value(EBOOKSET_CNTX *g_ebookset_cntx_p, S16 ebr_set_local_setting_flag);
extern S32 ebr_frm_save_settings_scroll(void);
extern S32 ebr_frm_save_settings_screen_mode(void);
extern S16 ebr_fmgr_delete_from_ebook(S16 drive_type, S8 *file_name, ebr_frm_filetype_enum book_type);
extern FS_HANDLE ebr_frm_get_nfo_handle(U8 *filename);
extern void ebr_frm_close_the_ebook(void);
extern S8 *ebr_fmgr_get_file_name(void);
extern S32 ebr_set_file_read_only(S8 *file_name_path);
extern S32 ebr_restore_file_read_only(S8 *file_name_path);
extern S8 *ebr_frm_get_file_path(S8 memory_type_selected, ebr_frm_search_type_enum search_type_path);
extern void ebr_frm_set_settings_and_save(EBOOKSET_CNTX *g_ebookset_cntx_p);
extern void ebr_frm_recalc_book_size(void);
extern void ebr_frm_set_settings_and_save_in_nvram(EBOOKSET_CNTX *g_ebookset_cntx_p);
extern pBOOL ebr_get_book_mark_name_list(S32 item_index, UI_string_type str_buff, PU8 *img_buff_p, U8 str_img_mask);
extern void ebr_frm_get_default_name_of_book_marks(S8* bookmark_name);
extern S16 ebr_frm_get_count_of_book_marks(void);
extern S16 ebr_frm_get_full_screen_flag_status(void);
extern S16 ebr_frm_get_auto_scroll_flag_status(void);

extern S32 ebr_fmgr_is_book_shelf_path(PS8 path);
extern U8 *ebr_get_selected_file_name(void);
extern void ebr_set_selected_file_name(FMGR_FILE_INFO_STRUCT *file_info);
extern S8 *ebr_frm_get_book_name(void);
extern S32 ebr_frm_get_book_size(void);
extern S32 ebr_frm_get_record_size_pdb(FS_HANDLE file_hnd, S32 offset, S32 rec_num);
extern S32 ebr_frm_restore_default_book_shelf_settings(void);
extern void ebr_frm_delete_all_from_book_shelf(void);
extern S32 ebr_frm_restore_default_book_settings(void);
extern ebr_memory_type_enum ebr_frm_get_memory_type_selected(void);
extern FS_HANDLE ebr_frm_fmgr_get_nfo_handle(S8 *file_name);
extern void ebr_frm_find_next_nfo_id(void);
extern void ebr_fmgr_get_file_name_call_back(S8 *fmgr_file_name, S8 *file_name);
extern U32 ebr_fmgr_get_file_size(S8 *fmgr_file_name);
extern S32 ebr_frm_fmgr_pre_entry_open_book(S8 *file_name, S8 *file_path, S8 *file_ext);
extern void ebr_frm_update_nfo_file(S8 *new_file_name, ebr_frm_filetype_enum book_type);
extern S32 ebr_frm_get_record_size_txt(S32 rec_num);
extern S32 ebr_frm_create_temp_file_txt(S32 rec_num);
extern S8 *ebr_frm_get_path_for_bookshelf(void);
/* FMGR_Revise */
//extern void ebr_frm_fmgr_delete_nfo_file(fmgr_tree_action_enum action_type);
extern void ebr_fmgr_get_display_name_call_back(S8 *fmgr_file_name, ebr_frm_filetype_enum book_type, S8 *file_name);

extern ebr_memory_type_enum ebr_frm_get_drive_type(void);
extern S16 ebr_frm_change_drive_type(void);
extern void ebr_fmgr_set_file_type(S8* file_name);
extern void ebr_frm_get_encoded_data(S8 *inBuffer, S8 *outBuffer);
extern void ebr_frm_delete_unused_nfo_file(void);
extern void ebr_frm_open_nfo_file_and_delete_unused_nfo(U8 *filename);
extern U8 *ebr_frm_get_nfo_file_name_path(const U8 *e_book_name);
extern U8 *ebr_frm_get_nfo_file_name(const U8 *e_book_name);
extern void ebr_frm_start_decompression(U8 *decompressed_buffer, U8 *compressed_buffer, INT32 *size);
extern void copy_record_in_passed_buffer(
                FS_HANDLE file_hnd,
                S32 record_id,
                U8 *output_buffer,
                U8 *input_buffer,
                INT32 *size);
extern ebr_frm_nfo_create_type_errors_enum ebr_frm_reset_for_new_book(void);
extern void ebr_frm_reset_auto_scroll(void);
extern void ebr_frm_set_data_for_display(void);
extern pBOOL ebr_frm_search_string_algo_find_next(void);
extern void ebr_frm_convert_page_id_to_record_id(
                S32 page_id,
                S32 text_offset,
                S32 shared_bytes,
                S32 *record_id,
                S32 *record_offset);
extern void ebr_fmgr_set_file_name(PS8 fmgr_file_name);
extern void ebr_frm_remove_nfo_id_info_from_file(S32 drive_type, PS8 file_name, ebr_frm_filetype_enum book_type);
extern ebr_frm_nfo_create_type_errors_enum ebr_frm_open_book_create_nfo_file_pdb(U8 *filename);
extern ebr_frm_nfo_create_type_errors_enum ebr_frm_open_book_create_nfo_file_txt(U8 *filename);
extern MMI_BOOL ebr_frm_is_empty_file(FS_HANDLE file_hnd, ebr_frm_filetype_enum type);
extern ebr_frm_nfo_create_type_errors_enum ebr_frm_add_to_book_shelf(void);
extern U8 mmi_ebook_util_get_chset(U8 src_chset);
extern void ebr_frm_get_book_encoding_type(void);
extern void ebr_frm_open_nfo_file_and_restore_default(
                S32 memory_type_selected,
                U8 *filename,
                ebr_frm_setting_struct *g_setting_list);
extern void ebr_frm_restore_default_settings_for_each_book(ebr_frm_setting_struct *g_setting_list);
extern void ebr_frm_get_encoding_type_from_book(void);
extern void ebr_frm_create_temp_file_path(void);

extern S32 ebr_frm_create_temp_file_in_ucs2(FS_HANDLE file_hnd);
extern S32 ebr_frm_create_temp_file_in_ucs2_for_bpp(FS_HANDLE file_hnd);


extern pBOOL ebr_frm_check_if_file_exists_or_not(void);
extern void ebr_frm_set_file_type(void);
extern void ebr_frm_fmgr_set_application_type(void);
extern void ebr_frm_fmgr_set_file_type(S8 *filename);
extern S32 ebr_frm_check_nfo_file_present(U8 *filename);
extern void ebr_frm_create_book_path(void);
extern void ebr_frm_delete_all_unused_nfo_file(void);
extern void ebr_frm_open_nfo_file_and_delete_all_unused_nfo(S8 memory_type, U8 *filename);
extern void ebr_frm_create_new_file_path(void);
extern void ebr_frm_delete_temp_file(void);
extern void ebr_frm_delete_temp_file_no_open_file(S8* filename);
extern void ebr_frm_split_path_name(S8 *path_name, S8 *path, S8 *name);
extern U16 ebr_frm_mmi_chset_text_to_ucs2_str(
            FS_HANDLE file_hnd,
            U8 *dest,
            S16 dest_size,
            U8 *src,
            S16 src_size,
            TextEncodingEnum encoding,
            U32 *over_the_record_border);
extern S16 ebr_frm_mmi_chset_binary_search(
            const kal_uint8 key,
            const kal_uint8 *lookup_table,
            kal_int16 start,
            kal_int16 end);
extern S32 ebr_frm_mmi_chset_utf8_to_ucs2_string(
            FS_HANDLE file_hnd,
            U8 *dest,
            S16 dest_size,
            U8 *src,
            S16 src_size,
            U32 *over_the_record_border);
extern void ebr_frm_reset_find_next_data(void);
extern void ebr_frm_restore_default_nvram(void);
extern void ebr_frm_reset_search_flag(void);
extern kal_int32 ebr_frm_mmi_chset_utf8_to_ucs2_string(
                    FS_HANDLE file_hnd,
                    U8 *dest,
                    S16 dest_size,
                    U8 *src,
                    S16 src_size,
                    U32 *over_the_record_border);
#ifdef __MMI_TOUCH_SCREEN__
extern void ebr_frm_get_data_during_scrolling(S32 page_id, PU8 buffer_local);
#endif  /* __MMI_TOUCH_SCREEN__ */
#endif /* _PIXCOM_EBOOKMANAGER_H */ 

