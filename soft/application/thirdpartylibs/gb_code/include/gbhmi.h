#ifndef  _Gb_HMI_H_
#define _Gb_HMI_H_
#include "mmi_features.h"
#include "eb_types.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "touchscreengprot.h"
#endif
#ifdef __MMI_GB_V5__
#define GB_CANDIDATE_COUNT 15
#else
#define GB_CANDIDATE_COUNT 7
#endif
#define GB_CANDIDATE_LEN 32

#if defined(__MMI_MAINLCD_220X176__)/*cong.li added for product QPAD[220*176] on 2009.08.11. */
#define GB_MAX_INPUT_LEN 10
#else
#define GB_MAX_INPUT_LEN 7
#endif

#if defined(__MMI_MAINLCD_128X160__)
#define GB_CH_CANDIDATE_COUNT 7
#define GB_CH_SYMBOL_CANDIDATE_COUNT 7
#elif defined(__MMI_MAINLCD_128X128__)
#define GB_CH_CANDIDATE_COUNT 7
#define GB_CH_SYMBOL_CANDIDATE_COUNT 7
#elif defined(__MMI_MAINLCD_160X128__)
#define GB_CH_CANDIDATE_COUNT 10
#define GB_CH_SYMBOL_CANDIDATE_COUNT 10
#elif defined(__MMI_MAINLCD_176X220__)
#define GB_CH_CANDIDATE_COUNT 10
#define GB_CH_SYMBOL_CANDIDATE_COUNT 10
#elif defined(__MMI_MAINLCD_220X176__)/*cong.li added for product QPAD[220*176] on 2009.08.11. */
#define GB_CH_CANDIDATE_COUNT 13
#define GB_CH_SYMBOL_CANDIDATE_COUNT 13
#elif defined (__MMI_MAINLCD_240X320__)
#ifdef __MMI_GB_TR_CHINESE__
#define GB_CH_CANDIDATE_COUNT 9
#define GB_CH_SYMBOL_CANDIDATE_COUNT 9
#else
#define GB_CH_CANDIDATE_COUNT 10
#define GB_CH_SYMBOL_CANDIDATE_COUNT 10
#endif
#else
#define GB_CH_CANDIDATE_COUNT 7
#define GB_CH_SYMBOL_CANDIDATE_COUNT 7
#endif

//#define MAX_PHONETIC_BUFFERSIZE   128
#define GB_SM_INPUTMETHOD_PINYING      1
#define GB_SM_INPUTMETHOD_BIHUA          2
#define GB_CANDIDATE_MAX_BUFFER_SIZE 128
#define GB_COMPOSITION_BUFFER_SIZE         128

#define GB_IN_CATEGORY_5_FLAG                                                       0x0001
#define GB_IN_CATEGORY_28_FLAG                                                     0x0002
#define GB_IN_CATEGORY_SINGLELINE_INPUTBOX_FLAG                    0x0004
#define GB_IN_CATEGORY_INLINE_MULTILINE_INPUTBOX_FLAG              0x0008

#if defined(__MMI_MESSAGES_CONCAT_SMS__)
#define EMSEditor_Chinese_Length						536
#define EMSEditor_Enghish_Length						1224 //wangzl:modify from 1224 for bug 6021 //guojian restore for bug 10360
#else
#define EMSEditor_Chinese_Length						140
#define EMSEditor_Enghish_Length						320
#endif


#ifdef MMI_LOW_MEMORY_PHONE
#define GB_CANDIDATE_BUFFER_SIZE 6 //8 /* Number of characters per selection page */
#else
#define GB_CANDIDATE_BUFFER_SIZE ((UI_device_width)/coolsand_UI_get_character_width(0x4E00)-2) //7 8 /* Number of characters per selection page */
#endif


typedef enum
{
    INPUT_NONE,
    INPUT_EZI_STROKE,
    INPUT_EZI_PINYIN,
    INPUT_EZI_BOPOMOFO,
    INPUT_EZI_CANGJIE,
    INPUT_EZI_ABC,
    INPUT_ABC,
    INPUT_123,
    INPUT_SYM,
    INPUT_EZI_MULTITAP_PINYIN,
    INPUT_EZI_MULTITAP_BOPOMOFO,
    INPUT_EZI_THAI,
    INPUT_EZI_RUSSIAN,
    INPUT_EZI_FRENCH,
    INPUT_EZI_GERMAN,
    INPUT_EZI_ITALIAN,
    INPUT_EZI_SPANISH,
    INPUT_EZI_ARABIC,
    INPUT_EZI_PORTUGUESE,
    INPUT_EZI_TURKISH,
    INPUT_EZI_MALAY,
    INPUT_EZI_INDONESIAN,
    INPUT_EZI_VIETNAMESE
}INPUT_MODE;

typedef enum
{
    GBKEY0           = 0,
    GBKEY1           = 1,
     GBKEY2           = 2,
     GBKEY3           = 3,
     GBKEY4           =4,
     GBKEY5           = 5,
     GBKEY6           = 6,
     GBKEY7           = 7,
     GBKEY8           = 8,
     GBKEY9           = 9,
     GBKEY_LSK,			
     GBKEY_RSK,			
     GBKEY_UP_ARROW,			
     GBKEY_DOWN_ARROW,		
     GBKEY_LEFT_ARROW,		
     GBKEY_RIGHT_ARROW,		
     GBKEY_SEND				
    
}KEYID;
typedef enum
{
    GBIMESTATE_IDLE,
    GBIMESTATE_INPUT,
   // GBIMESTATE_SELECT,
    GBIMESTATE_ASSATION
}GBIMESTATE;

typedef enum {
	MULTILINE_INPUTBOX_UNKNOWN,
	MULTILINE_INPUTBOX_ORGANIZER_TODOLIST,
	MULTILINE_INPUTBOX_PHONEBOOK_SEARCH_ENTRY,
	MULTILINE_INPUTBOX_PHONEBOOK_ADD_NEW_ENTRY_TO_SIM,
	MULTILINE_INPUTBOX_PHONEBOOK_ADD_NEW_ENTRY_TO_PHONE_NAME,
	MULTILINE_INPUTBOX_PHONEBOOK_ADD_NEW_ENTRY_TO_PHONE_COMPANY_NAME,
	MULTILINE_INPUTBOX_PHONEBOOK_EDIT_NAME,
	MULTILINE_INPUTBOX_PHONEBOOK_CALL_GROUP_NAME,
	MULTILINE_INPUTBOX_PHONEBOOK_OWNER_NUM,
	MULTILINE_INPUTBOX_SETTING_PHONESETUP_GREETING_TEXT,
	MULTILINE_INPUTBOX_VOICE_MAIL,
	MULTILINE_INPUTBOX_SAT_SEND_MESS,
	MULTILINE_INPUTBOX_EMS_MESS_SETTING,
	MULTILINE_INPUTBOX_SETTING_FIXED_DIAL,
	MULTILINE_INPUTBOX_FMGR_FILENAME_EDITOR,
	MULTILINE_INPUTBOX_SOUNDRECORD_RENAME,
} mmi_editor_multiline_inputbox_enum;


#define __GB_CANDIDATE_AREA_WITH_ARROW__
#define __GB_COMPOSITION_AREA_WITH_ARROW__
#define GB_ARROW_WIDTH	8
typedef enum
{
	GB_LEFT_ARROW,
	GB_RIGHT_ARROW,
	GB_UP_ARROW,
	GB_DOWN_ARROW
}gb_arrow_type;
extern void InitGbInputMethod_Ara(void);
#ifdef __MMI_SMART_PERSIAN__
extern void InitGbInputMethod_Per(void);
#endif
extern void GBSetInputboxLSKFunction(FuncPtr f);//Renwy add 2008/09/28 for clean warning
extern void GbKeyRSKLongPressHandler();
extern void GB_Reset(void);
extern void GbRegisterKeyHandler(byte mode);
void GbClearKeyStarAndPoundHandler(void);
void GbClearKeyNavigationHandler(void);
void GbClearAllInputKeyHandler(void);
void GbKeyArrowUPHandler(void);
void GbKeyArrowRightHandler(void);
void GbKeyArrowDownHandler(void);
void GbKeyArrowLeftHandler(void);
void GbKeyStarPressHandler(void);
void GBIMEinsertChar( unsigned short gbchar);
#ifdef __MMI_TOUCH_SCREEN__
extern void gb_register_arrow_handler(void);
BOOL gb_is_pen_in_composition_area(mmi_pen_point_struct pos);
void gb_display_syllable_by_index(U8 index);
BOOL gb_is_pen_in_candidate_area(mmi_pen_point_struct pos);
void gb_set_candidate_by_index(U8 index);
void gb_composition_area_left_arrow_pen_up_handler(void);
void gb_composition_area_right_arrow_pen_up_handler(void);
void gb_set_syllable_by_index(U8 index);
void gb_input_character_by_candidate_index(U8 index);
void gb_candidate_area_left_arrow_pen_up_handler(void);
void gb_candidate_area_right_arrow_pen_up_handler(void);
#endif
#endif
