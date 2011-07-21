#ifndef __GCJ_PORT_INTERNAL_
#define __GCJ_PORT_INTERNAL_
#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"
#include "lcd_sw_inc.h"
#include "lcd_sw_rnd.h"
#include "csw_lcd.h"
#include "mmi_png_dec.h"
#include "mmi_jpeg_dec.h"
#include "gui_data_types.h"
#include "gdi_datatype.h"
#include "gdi_const.h"
#include "gdi_include.h"
#include "idleappdef.h"
#include "globaldefs.h"
#include "settingprofile.h"
#include "wgui_categories_inputs.h"
#include "wgui_categories_inputsenum.h"
#include "gui_inputs.h"
#include "settingprofile.h"
#include "gpioinc.h"

#include "settingdefs.h"
#include "commonscreens.h"
#include "mdi_audio.h"
#include "javaprot.h"
#include "javadef.h"
#include "dual_sim.h"
#include "settingprofile.h"
#ifdef JATAAYU_SUPPORT
#include "jwap_str_id.h"
#endif
#include "java.h"
#define KNI_TRUE 1
#define KNI_FALSE 0

//#define gcj_TraceOut(trace_level, fmt, ...) 

//#define USE_SYS_EVENT
//#define USE_SYS_MUTEX
typedef unsigned short unicode;

extern gdi_handle	java_baseLayer;

typedef enum
{
    GCJ_MSG_COND_WAIT,
    GCJ_MSG_COND_WAIT_TIMEOUT,
    GCJ_MSG_PTHREAD_SIG,
    GCJ_MSG_MUTEX,
    GCJ_MSG_SEMPHONE,
    GCJ_MSG_GCJ_KILL,
} GCJ_BODY_MSG_T ;


typedef enum 
{
	ENORMAL,
	EINTR,
	ETIMEDOUT,
	EMUTEX,
	ESEMAPHONE,
	EKILL,
    	GCJ_RETURN_EVENT,
}COND_WAIT_RET_T;

#ifndef USE_SYS_MUTEX
#define MAX_THREAD 30
typedef struct mutexaddr
{
	unsigned char taskId;
	unsigned int	callAddr;
}_TASK_INFO_T;

typedef struct gcj_mutex_t
{
	unsigned char sign;
	_TASK_INFO_T taskList[MAX_THREAD];
	char index;
	char lockedId;
	char lockCount;
	unsigned char isKill;
	unsigned char killTaskId;
}_GCJ_MUTEX_T;
#else
typedef struct gcj_mutex_t
{
	unsigned char id;
	unsigned char taskId;
}_GCJ_MUTEX_T;
#endif

typedef enum _event_type{
    KEY_EVENT     =  1,
    PEN_EVENT     =  2,
    COMMAND_EVENT =  3,
    SYSTEM_EVENT  =  4,
    MM_EOM_EVENT  =  8,
}EVENT_TYPE;

typedef enum _key_type{
    PRESSED  = 1,  // key, pen
    RELEASED = 2,  // key, pen
    REPEATED = 3,  // key
    DRAGGED  = 3,  // pen
    TYPED    = 4,
    IME      = 5,
}KEY_TYPE_T;

typedef enum _key_code{
	UP = 1,
	DOWN = 6,
	LEFT = 2,
	RIGHT = 5,
	FIRE = 8,
	GAME_A = 9,
	GAME_B = 10,
	GAME_C = 11,
	GAME_D = 12,
	KEY_NUM0 = 48,
	KEY_NUM1 = 49,
	KEY_NUM2 = 50,
	KEY_NUM3 = 51,
	KEY_NUM4 = 52,
	KEY_NUM5 = 53,
	KEY_NUM6 = 54,
	KEY_NUM7 = 55,
	KEY_NUM8 = 56,
	KEY_NUM9 = 57,
	KEY_STARJ = 42,
	KEY_POUNDJ = 35,
	KEY_LEFT_SOFT = -6,
	KEY_RIGHT_SOFT = -7,
	KEY_CLEARJ = -8,
	SYSTEM_KEY_POWER = 1,
	SYSTEM_KEY_SEND  = 2,
	SYSTEM_KEY_END   = 3,
	SYSTEM_KEY_CLEAR = 4,
}KEY_CODE_T;

typedef enum {
    MENU_REQUESTED   = -1,
    MENU_DISMISSED   = -2,    
}COMMAND_TYPE_T;

typedef enum {
/**
 * Minor ID indicating a SYSTEM_EVENT to suspend all current
 * activity
 */
	SUSPEND_ALL		= 1,

/**
 * Minor ID indicating a SYSTEM_EVENT to resume the currently
 * suspended MIDlet (that is, the followup to a previous
 * SUSPEND_CURRENT event)
 */
	RESUME_ALL 		= 2,

/**
 * Minor ID indicating a SYSTEM_EVENT to stop all MIDlets
 * (active and paused) and exit the system
 */
	SHUTDOWN			= 3,

/**
 * Minor ID indicating a SYSTEM_EVENT to pause the currently
 * active MIDlet (and optionally return to the selector)
 */
	SUSPEND_CURRENT	= 4,

/**
 * Minor ID indicating a SYSTEM_EVENT to resume the currently
 * paused MIDlet
 */
	RESUME_PREVIOUS	= 5,

/**
 * Minor ID indicating a SYSTEM_EVENT to kill the currently
 * active MIDlet (and optionally return to the selector)
 */
	KILL_CURRENT		= 6,   
}SYSTEM_TYPE_T;

typedef struct {
    KEY_TYPE_T keytype;
    KEY_CODE_T keycode;
}KEY_EVENT_DATA;

typedef struct {
    KEY_TYPE_T keytype;
    int x;
    int y;
}PEN_EVENT_DATA;

typedef struct {
    COMMAND_TYPE_T commandtype;
}COMMAND_EVENT_DATA;

typedef struct {
    SYSTEM_TYPE_T systemtype;
}SYSTEM_EVENT_DATA;

typedef struct {
    int playerid;
    int curmtime;
}MM_EVENT_DATA;

typedef struct _event_handler{
	EVENT_TYPE java_event_type;
    union {
        KEY_EVENT_DATA keyEventData;
        PEN_EVENT_DATA penEventData;
        COMMAND_EVENT_DATA commandEventData;
        SYSTEM_EVENT_DATA systemEventData;
        MM_EVENT_DATA mmEventData;
    }java_event_data;
}EVENT_HANDLER_T;


#define MAX_ACTION_STONE 1
typedef struct _event_action {
	int inProcessing;
	int isFull;
	int pushIndex;
	int popIndex;
	EVENT_HANDLER_T data[MAX_ACTION_STONE];
}EVNET_ACTION_T;

typedef enum _inputmod{
    /** The 'none' input mode. */
    IM_NONE         = 0,
    /** The 'roman caps' input mode. */
    IM_ROMAN_CAPS   = 1,
    /** The 'lowercase roman' input mode. */
    IM_ROMAN_SMALL  = 2,
    /** The 'numeric' input mode. */
    IM_NUMERIC      = 3,
    /** The 'symbol' input mode. */
    IM_SYMBOL       = 4,
#if defined(__NATIVE_JAVA_NO_TP_IME_SUPPORT__)	
    //sm chinese bihua
    IM_SYM_SM       = 5,
#endif	
}INPUT_MODE_T;

#define GCJ_TRANS_COLOR	0x0000
#define blackPixel	0x00000000
#define whitePixel	0xffffffff
#define lightGrayPixel	LIGHT_GRAY_COLOR
#define darkGrayPixel	DARK_GRAY_COLOR

typedef enum 
{
	TYPE_PNG,
	TYPE_JPG,
	TYPE_GIF,
	TYPE_RAW,
}imgtype;
typedef struct _img_handle{
	 unsigned char * imgData;
	 unsigned int	 imgWidth;
	 unsigned int imgHeight;
	 imgtype imgType;
	 unsigned char isTransColor;
	 unsigned short transColor;
 }img_handle_t;

#define LIGHT_GRAY_COLOR  0x00AFAFAF // light gray
										 
										 /** Button border color for dark gray border */
#define DARK_GRAY_COLOR  0x00606060  // dark gray

typedef struct SoftButton{
	unicode * label;
	int numChars;
}SOFT_BUTTON_T;

typedef enum {
	BUTTON_LEFT,
	BUTTON_CENTER,
	BUTTON_RIGHT,
}BUTTON_POSION_T;

typedef struct {
    int priority;
    unicode *chars;
    unsigned int numChars:8;
    unsigned int useLongChars:1;
    unicode *longChars;
    unsigned int numLongChars:8;
    unsigned int type:4;
    unsigned int id:20;
} commandStruct;

typedef struct {
    int idx;         /* index of popup element */
    int numChars;    /* length of "chars" array */
    unicode *chars;  /* string of popup element */
    int useImage;    /* < 0 if "image" is valid for this element */
    int imageHeight;
    int imageWidth;
    void* image;
} popupElementStruct;
typedef void (*backGroundAction)(void);


#define COMMAND_TYPE_SCREEN 1
#define COMMAND_TYPE_BACK   2
#define COMMAND_TYPE_CANCEL 3
#define COMMAND_TYPE_OK     4
#define COMMAND_TYPE_HELP   5
#define COMMAND_TYPE_STOP   6
#define COMMAND_TYPE_EXIT   7
#define COMMAND_TYPE_ITEM   8

#include "wgui_softkeys.h"
extern void command_press(void);
#define BUTTON_NOT_SET        -3
#define BUTTON_DISMISSES_MENU -2
#define BUTTON_INVOKES_MENU   -1

/* The max number of characters in a soft button command label */
#define MAX_SOFTBUTTON_COMMAND_LENGTH 12

extern void menuEventCommand(commandStruct menu[],int index);

extern int inFullScreenMode;
extern unsigned char set_InputDone;
extern unicode input_mode_sign[10];
extern unicode *menu_lable ;
extern unicode *clear_lable;
extern unicode * back_lable;
extern unsigned char menu_length,clear_length,back_length;
extern SOFT_BUTTON_T leftButton,rightButton,centerButton;
extern int buttonCommands[3];

extern Png_StreamBuf PngStreamBuf;
extern int png_resized_width;
extern int png_resized_height;
extern gdi_handle java_baseLayer;
extern gdi_handle java_secondLayer;


extern unsigned char  inMenu;
extern unsigned char popInMenu;

extern int popMenuKeyHandle(EVENT_HANDLER_T *eventData);
extern int inMenuKeyHandle(EVENT_HANDLER_T *eventData);
extern void gcj_net_reset();
extern void gcj_interface_reset();
extern void gcj_image_reset();
extern void gcj_popupmenu_reset();
extern void gcj_menu_reset();
extern void gcj_RegisterBackGroundAction(backGroundAction f);

unsigned char gcj_StubIsPrepareToQuit();

#endif
