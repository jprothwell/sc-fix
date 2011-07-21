///////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Copyright (C) 2003-2009, Coolsand Technologies, Inc.            //
//                            All Rights Reserved                             //
//                                                                            //
//      This source code is the property of Coolsand Technologies and is      //
//      confidential.  Any  modification, distribution,  reproduction or      //
//      exploitation  of  any content of this file is totally forbidden,      //
//      except  with the  written permission  of  Coolsand Technologies.      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  $HeadURL:                                                                 //
 
//  $Date:                                                                    //
//  $Revision:                                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
///                                                                           //
///     @file TouchScreen.c                                                   //
///     Translate touch panle IRQ to mmi event.                               //
///     Handle and dispatch touch panle event.                                //
///                                                                           //
//////////////////////////////////////////////////////////////////////////////// 
#include "mmi_features.h"
#undef MMI_PEN_SAVE_STROKE_IN_FILE

#if ( defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) ) && defined(MMI_ON_HARDWARE_P)

#include "csw.h"
#include "mmi_trace.h"
#include "eventsgprot.h"
#include "touch_screen_cfg.h"
#include "callmanagementgprot.h"
#include "simdetectiondef.h"
#include "tsd_m.h"
#include "kal_release.h"
#include "app_ltlcom.h"
#include "queuegprot.h"
#include "gpioinc.h"
#include "historygprot.h"
#include "sxr_sbx.h"
#ifdef __MMI_MULTI_SIM__
#include "dual_sim.h"
#endif
extern pwr_context_struct g_pwr_context[];
extern BOOL g_FsTaskIsRun;
#if defined (CAMERA_MODULE)
extern BOOL GetCameraPreviewState(VOID);
#endif
extern U8 mmi_phb_mass_processing_scr_del_callback(VOID *p);
extern U8 mmi_frm_sms_check_action_pending(VOID);
 #ifdef JATAAYU_SUPPORT
extern U8  jMMS_GetMMSAppStatus (void);
extern U8  jWap_GetWapAppStatus (void);
#endif
extern U16   gKeyPadLockFlag;
//!When the handwriting starts, curRegion will be set to this value.
//!It indicates that last pen down event is not happened to handwriting 
//!area.
#define INVALID_CURRENT_REGION (0-1)
//!Last pen down event is happened to extent handwritting area.
//!So consider to send stroke event when get pen event.
#define EXT_CURRENT_REGION (0-2)
//!If the pen moved distances is less than this value, we won't consider it 
//!as a move event.
#define MIN_MOVE_DISTANCE 5
#define MIN_NOT_STROKE_MOVE_DISTANCE 8
//!Repetition perriod of pen down event reported by TSD.
//!This value is for normal condition. 
//!We may need some other values for special applications(Like games?) for the future.
#define REPETITION_PERIOD 5
//!This value will affect how often we send the LONG_TAP event.
//!If the REPETITION_PERIOD*REPETITION_TIMES is more than this value, we send LONG_TAP event.
#define LONG_TAP_REPETITION_TIMES 200
//!If there is more than this value messages left in the mailbox, we won't send more down and press event.
#define MAX_COUNT_MESSAGES 25

#define MOVE_EVENT_PERIOD  5

static U16 g_move_event_period_num = 0;

static BOOL gWantChangeHWArea = FALSE;

typedef enum 
{
      PEN_DOWN,      
      PEN_UP,      
      PEN_MOVE,    
      PEN_LONGTAP,      
      PEN_REPEAT,   
      PEN_ABORT,    
      STROKE_MOVE,     
      STROKE_DOWN, 
      STROKE_LONGTAP,
      STROKE_UP,
} Touch_Panel_Event_E;

typedef struct
{ 
    UINT16 x;
    UINT16 y;
    Touch_Panel_Event_E event;
} TS_Event_T;

//! Handwriting context
typedef struct
{ 
    //!How many handwriting regions exist.
    UINT32 numRegions;
    //!Current handwriting region index.
    UINT32 curRegion;
    //!Is there a extend handwriting area.
    //!A extend writing area means if a stroke is started from a handwriting area and 
    //!it isn't moved out of either the starting handwriting area or the extend area,
    //!we still think it is a unfinished stroke until it is moved out.
    BOOL hasExtRegion;
    mmi_pen_handwriting_area_struct regionArray[TP_MAX_HANDWRITING_AREA_NUM];
    mmi_pen_handwriting_area_struct extRegion;
    //!How many points can be stored in pointsStore.
    UINT32 maxPoints;
    UINT32 curPointPos;
    mmi_pen_point_struct* pointsStore;
    BOOL capturePaused;
} Handwriting_Context_T;

typedef struct
{ 
    Handwriting_Context_T handwritingArea;
    //!Pen is in down state.
    BOOL penIsDown;
    //!Quick lanch enable flag
    BOOL enableQuickLaunch;
    //!Store last pen event position
    TSD_POINT_T lastPoint;
} Touch_Screen_Context_T;

//For debug usage.Print pen event name.
PRIVATE const U8 *g_PenEvtName[]={
			"PEN_DOWN",      
			"PEN_UP",      
			"PEN_MOVE",    
			"PEN_LONGTAP",      
			"PEN_REPEAT",   
			"PEN_ABORT",    
			"STROKE_MOVE", 
			"STROKE_DOWN", 
			"STROKE_LONGTAP",
			"STROKE_UP",
			"ERROR EVENT"
};

PRIVATE tpShortCutAppEntry g_ShortcutEntries[] = { TP_SHORTCUT_APP_ARRAY};
PRIVATE const UINT16 g_ShortcutArea[] = { TP_SHORTCUT_AREA};
PRIVATE mmi_pen_hdlr g_PenEventTable[MMI_PEN_EVENT_TYPE_MAX];
PRIVATE Touch_Screen_Context_T g_TouchScreenContext;

PRIVATE VOID penEvtHandler(Touch_Panel_Event_E event ,UINT16  x, UINT16 y);
PRIVATE VOID penTouchPanelEventInd(VOID *param);
PRIVATE VOID penDownHandler( UINT16 x, UINT16 y);
PRIVATE VOID penUpHandler( UINT16 x, UINT16 y);
PRIVATE VOID penMoveHandler( UINT16 x, UINT16 y);
PRIVATE VOID penAbortHandler( UINT16 x, UINT16 y);
PRIVATE VOID penLongTapHandler( UINT16 x, UINT16 y);
PRIVATE VOID strokeDownHandler( UINT16 x, UINT16 y);
PRIVATE VOID strokeUpHandler( UINT16 x, UINT16 y);
PRIVATE VOID strokeMoveHandler( UINT16 x, UINT16 y);
PRIVATE const INT8* getPenEventName(Touch_Panel_Event_E ev);
#ifndef __MMI_TP_NO_SHORTCUT_FR__
PRIVATE BOOL launchShortCut( Touch_Panel_Event_E event ,UINT16  x, UINT16 y);
PRIVATE UINT16 getShortcutIndex( UINT16 x, UINT16 y);
#endif
PRIVATE VOID sendTpEventToMmi( Touch_Panel_Event_E e, UINT16 x, UINT16 y);
PRIVATE VOID tsdCallback(TSD_POINT_T* pointTouched,TSD_PEN_STATE_T status);
PRIVATE UINT32 findPointInHandwritingArea( TSD_POINT_T* pointTouched);
PRIVATE VOID pushEndStrokeToHandwritingStore( );
PRIVATE VOID pushEndCharToHandwritingStore( );
PRIVATE VOID pushPointToHandwritingStore( mmi_pen_point_struct* point);



/* --------------------------------------------------------------------*/
/** mmi_pen_init
 * @brief Initialize touch panel.If no calibration data exists, it will
 * disable touch panel irq until calibration finished.
 * 
 * @param VOID
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_init(VOID)
{
    UINT32 i;
    TSD_REPORTING_MODE_T mode;

	mmi_trace(g_sw_TPL, "TP In mmi_pen_init");
    memset( &g_TouchScreenContext, 0, sizeof( g_TouchScreenContext));
    g_TouchScreenContext.handwritingArea.curRegion = INVALID_CURRENT_REGION;
    g_TouchScreenContext.penIsDown = FALSE;
    g_TouchScreenContext.enableQuickLaunch = FALSE;

    tsd_SetCallback( tsdCallback);

    mode.onDown = FALSE;
    mode.onPressed = FALSE;
    mode.onUp = FALSE;
    mode.repetitionPeriod = REPETITION_PERIOD;
    tsd_SetReportingMode( &mode);

    if( mmi_pen_read_calibration( ))
    { 
        mmi_pen_unblock( );
    }

    for( i=0;i<ARRAY_SIZE( g_PenEventTable); i++) 
    { 
        g_PenEventTable[i] = NULL;
    }
	SetProtocolEventHandler( penTouchPanelEventInd, MSG_ID_TP_EVENT_IND);

    tsd_Open( );
}

/* --------------------------------------------------------------------*/
/** penEvtHandler
 * @brief MMI touch panel event dispatcher.
 * 
 * @param event
 * @param x
 * @param y
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PRIVATE VOID penEvtHandler( Touch_Panel_Event_E event ,UINT16  x, UINT16 y)
{ 
    mmi_trace( g_sw_TPL, "TP In penEvtHandler,%s x:%d, y%d", 
            getPenEventName( event), x, y);
#if defined(GDI_USING_LAYER_ROTATE)
		{
			S32 xx, yy;
			xx = x;
			yy = y;
			gdi_rotate_map_lcd_to_layer(gdi_layer_get_actual_rotate(), &xx, &yy);
			x = xx;
			x = yy;
		}
#endif

#ifndef __MMI_TP_NO_SHORTCUT_FR__
    if( launchShortCut( event, x, y)) return;
#endif

    switch( event)
    { 
        case PEN_DOWN:
            penDownHandler( x, y);
            break;
        case PEN_UP:
            penUpHandler( x, y);
            break;
        case PEN_MOVE:
            penMoveHandler( x, y);
            break;
        case PEN_ABORT:
            penAbortHandler( x, y);
            break;
        case PEN_LONGTAP:
            penLongTapHandler( x, y);
            break;
        case STROKE_DOWN:
            strokeDownHandler( x, y);
            break;
        case STROKE_UP:
            strokeUpHandler( x, y);
            break;
        case STROKE_MOVE:
            strokeMoveHandler( x, y);
            break;
        default:
            break;
    }
}

PRIVATE VOID penDownHandler( UINT16 x, UINT16 y)
{
    mmi_pen_point_struct pos = { x, y};

    /*Fix Bug#11357 for cs2324c by cong.li on 2009.06.16. 
	If it is the first down from deep sleep(Dark) or screen saver(half light), 
	let it leave screen saver and return.*/
    if(!mmi_is_lcd_backlight_on())
    {//If it is the first down from deep sleep, just return.
        TurnOnBacklight(1);
	#ifdef __MMI_LCD_PARTIAL_ON__
	 LeavePartialOnScreenSaverIfOk();
	#endif
        return; 
    }
    else
    {//To light the screen when it is half light.
        TurnOnBacklight(1);
    }

    if( g_PenEventTable[MMI_PEN_EVENT_DOWN] != NULL)
        (g_PenEventTable[MMI_PEN_EVENT_DOWN])(pos);
}

PRIVATE VOID penUpHandler( UINT16 x, UINT16 y)
{ 
    mmi_pen_point_struct pos = { x, y};
    if( g_PenEventTable[MMI_PEN_EVENT_UP] != NULL)
        (g_PenEventTable[MMI_PEN_EVENT_UP])(pos);
}

PRIVATE VOID penMoveHandler( UINT16 x, UINT16 y)
{ 
    mmi_pen_point_struct pos = { x, y};
    if( g_PenEventTable[MMI_PEN_EVENT_MOVE] != NULL)
        (g_PenEventTable[MMI_PEN_EVENT_MOVE])(pos);
}

PRIVATE VOID penAbortHandler( UINT16 x, UINT16 y)
{ 
    mmi_pen_point_struct pos = { x, y};
    if( g_PenEventTable[MMI_PEN_EVENT_ABORT] != NULL)
        (g_PenEventTable[MMI_PEN_EVENT_ABORT])(pos);
}

PRIVATE VOID penLongTapHandler( UINT16 x, UINT16 y)
{ 
    mmi_pen_point_struct pos = { x, y};
    if( g_PenEventTable[MMI_PEN_EVENT_LONG_TAP] != NULL)
        (g_PenEventTable[MMI_PEN_EVENT_LONG_TAP])(pos);
}
PRIVATE VOID strokeDownHandler( UINT16 x, UINT16 y)
{ 
    mmi_pen_point_struct pos = { x, y};

   TurnOnBacklight(0); // add by chenqiang , torn on back light when stroke
   
    if( g_TouchScreenContext.handwritingArea.capturePaused) return;
    if( g_PenEventTable[MMI_PEN_STROKE_DOWN] != NULL)
        (g_PenEventTable[MMI_PEN_STROKE_DOWN])(pos);
}

PRIVATE VOID strokeUpHandler( UINT16 x, UINT16 y)
{ 
    mmi_pen_point_struct pos = { x, y};

    TurnOffBacklight();// add by chenqiang , torn of back light when stroke over
	
    if( g_TouchScreenContext.handwritingArea.capturePaused) return;
    pushEndStrokeToHandwritingStore( );
    if( g_PenEventTable[MMI_PEN_STROKE_UP] != NULL)
        (g_PenEventTable[MMI_PEN_STROKE_UP])(pos);
}

PRIVATE VOID strokeMoveHandler( UINT16 x, UINT16 y)
{ 
    mmi_pen_point_struct pos = { x, y};
    if( g_TouchScreenContext.handwritingArea.capturePaused) return;

    if( (x*x+y*y) >= (MIN_MOVE_DISTANCE*MIN_MOVE_DISTANCE))
    { 
        pushPointToHandwritingStore( &pos);
    }

    if( g_PenEventTable[MMI_PEN_STROKE_MOVE] != NULL)
        (g_PenEventTable[MMI_PEN_STROKE_MOVE])(pos);
}

PRIVATE VOID penTouchPanelEventInd( VOID *param)
{
    TS_Event_T* evData = param;
	mmi_trace(g_sw_TPL, "TP In mmi_pen_touch_panel_event_ind");
    penEvtHandler( evData->event, evData->x, evData->y);
}

/* -------------------------------------------------------------------- */
/** @brief getPenEventName
 * Get pen event name according to given event id.
 * Only for debug usage.
 * @param ev Pen event id.
 * 
 * @return Pen event name string.
 */
/* -------------------------------------------------------------------- */
PRIVATE const INT8* getPenEventName(Touch_Panel_Event_E ev)
{	
    switch (ev)
    {
        case PEN_DOWN:
            return g_PenEvtName[PEN_DOWN];
            break;
        case PEN_UP:
            return g_PenEvtName[PEN_UP];
            break;	
        case PEN_MOVE:
            return g_PenEvtName[PEN_MOVE];
            break;
        case PEN_LONGTAP:
            return g_PenEvtName[PEN_LONGTAP];
            break;	
        case PEN_REPEAT:
            return g_PenEvtName[PEN_REPEAT];
            break;	
        case PEN_ABORT:
            return g_PenEvtName[PEN_ABORT];
            break;
        case STROKE_MOVE:
            return g_PenEvtName[STROKE_MOVE];
            break;
        case STROKE_DOWN:
            return g_PenEvtName[STROKE_DOWN];
            break;
        case STROKE_LONGTAP:
            return g_PenEvtName[STROKE_LONGTAP];
            break;
        case STROKE_UP:
            return g_PenEvtName[STROKE_UP];
            break;
        default:
            break;		
    }
    return g_PenEvtName[10];
}

extern MMI_BOOL mmi_obex_check_switch_application(VOID);

#define SHORTCUT_INDEX_NULL 0xFFFF

#ifndef __MMI_TP_NO_SHORTCUT_FR__
/* -------------------------------------------------------------------- */
/** @brief launchShortCut
 * Decide whether launch shorcut application or not. 
 * Should be called before any other pen event handler.
 * @param event Pen event id
 * @param x Pen position coordinate x.
 * @param y Pen position coordinate y.
 * 
 * @return If it is time to launch shortcut app, launch it and return TRUE.
 *         Else, return FALSE.
 */
/* -------------------------------------------------------------------- */
PRIVATE BOOL launchShortCut( Touch_Panel_Event_E event ,UINT16  x, UINT16 y)
{ 
	static UINT16 shortcutIndex;
	BOOL SimRemoveFlag = MMI_FALSE;
	U16 i;
	mmi_trace(g_sw_TPL, "TP In launchShortCut.");

	for(i = 0; i < MMI_SIM_NUMBER; i++)
	{
		if(g_pwr_context[i].SimRemoveFlag)
		{
			SimRemoveFlag = MMI_TRUE;
			break;
		}
	}
    if( isInCall() ||SimRemoveFlag||
#if defined (CAMERA_MODULE)
	GetCameraPreviewState()||
#endif
#ifdef JATAAYU_SUPPORT
	
#ifdef JATAAYU_SUPPORT_MMS
		jMMS_GetMMSAppStatus()||
#endif
#ifdef JATAAYU_SUPPORT_WAP 
		jWap_GetWapAppStatus()||
#endif	
#endif
            !g_TouchScreenContext.enableQuickLaunch ||
            mmi_phb_mass_processing_scr_del_callback( NULL) ||
            g_FsTaskIsRun ||
            mmi_frm_sms_check_action_pending( ) ||
            mmi_pen_is_in_calibration_process( )||gKeyPadLockFlag)
    {//Judge if the short cut is available in current context.
        shortcutIndex = SHORTCUT_INDEX_NULL;
        return FALSE;
    }
    switch( event)
    { 
        case PEN_UP:
            if( shortcutIndex != SHORTCUT_INDEX_NULL &&
                    shortcutIndex == getShortcutIndex( x, y))
            {//Call shortcut entry and return.
                if( g_ShortcutEntries[shortcutIndex] != NULL
			#ifdef	__MMI_BT_COOLSAND_SUPPORT__
					&& !mmi_obex_check_switch_application()
                     #endif
			)
                { 
                    g_ShortcutEntries[shortcutIndex]( );
                    FreeHistory(NULL);
                }
                shortcutIndex = SHORTCUT_INDEX_NULL;
                return TRUE;
            }
            shortcutIndex = SHORTCUT_INDEX_NULL;
            break;
        case PEN_DOWN:
            shortcutIndex = getShortcutIndex( x, y);
            ASSERT( shortcutIndex < ARRAY_SIZE( g_ShortcutEntries));
            break;
        default:
            break;
    }

    return FALSE;
}
#endif /* __MMI_TP_NO_SHORTCUT_FR__*/

/* -------------------------------------------------------------------- */
/** @brief mmi_pen_set_shortcut_entry
 * set the short cut entry function
 * @param index fun
 * @index: which short cut should be set
 * @fun: the new entry function pointer
*/
/* -------------------------------------------------------------------- */
PUBLIC VOID mmi_pen_set_shortcut_entry(U8 index, tpShortCutAppEntry fun)
{
	if(index > SHORTCUT_MAX)
		return;
	g_ShortcutEntries[index] = fun;
}
/* -------------------------------------------------------------------- */

#ifndef __MMI_TP_NO_SHORTCUT_FR__
/** @brief getShortcutIndex
 * Get shortcut index by given pen position. 
 * @param x 
 * @param y 
 * 
 * @return SHORTCUT_INDEX_NULL if didn't find a shortcut.
 */
/* -------------------------------------------------------------------- */
PRIVATE UINT16 getShortcutIndex( UINT16 x, UINT16 y)
{
    if( x < g_ShortcutArea[0] || x > g_ShortcutArea[1] ||
            y < g_ShortcutArea[2] || y > g_ShortcutArea[3])
    {
        return SHORTCUT_INDEX_NULL;
    }
    return x/g_ShortcutArea[4];
}
#endif /* __MMI_TP_NO_SHORTCUT_FR__*/

/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_down_handler
 * Register pen down event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
PUBLIC VOID mmi_pen_register_down_handler(mmi_pen_hdlr pen_fp)
{
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_down_handler. ptr:0x%08x",
            pen_fp);
	g_PenEventTable[MMI_PEN_EVENT_DOWN] = pen_fp;
}

/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_up_handler
 * Register pen up event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
PUBLIC VOID mmi_pen_register_up_handler(mmi_pen_hdlr pen_fp)
{
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_up_handler. ptr:0x%08x",
            pen_fp);
	g_PenEventTable[MMI_PEN_EVENT_UP] = pen_fp;
}

/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_move_handler
 * Register pen move event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
PUBLIC VOID mmi_pen_register_move_handler(mmi_pen_hdlr pen_fp)
{
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_move_handler. ptr:0x%08x",
            pen_fp);
	g_PenEventTable[MMI_PEN_EVENT_MOVE] = pen_fp;
}

PUBLIC VOID mmi_pen_register_abort_handler(mmi_pen_hdlr pen_fp)
{ 
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_abort_handler. ptr:0x%08x",
            pen_fp);
	g_PenEventTable[MMI_PEN_EVENT_ABORT] = pen_fp;
}

PUBLIC VOID mmi_pen_register_long_tap_handler(mmi_pen_hdlr pen_fp)
{ 
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_long_tap_handler. ptr:0x%08x",
            pen_fp);
	g_PenEventTable[MMI_PEN_EVENT_LONG_TAP] = pen_fp;
}


/* --------------------------------------------------------------------*/
/** mmi_pen_dummy_hdlr
 * @brief Dummy handler.
 * 
 * @param pos
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_dummy_hdlr( mmi_pen_point_struct pos)
{
	UI_UNUSED_PARAMETER(pos);
}


/* --------------------------------------------------------------------*/
/** findPointInHandwritingArea
 * @brief Check a given point is in handwriting area.
 * 
 * @param pointTouched
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PRIVATE UINT32 findPointInHandwritingArea( TSD_POINT_T* pointTouched)
{ 
    UINT32 i;
	mmi_trace( g_sw_TPL, "TP In findPointInHandwritingArea");
    //Check regular handwriting area first!!
    if( g_TouchScreenContext.handwritingArea.numRegions)
    { 
        ASSERT( num_regions <= TP_MAX_HANDWRITING_AREA_NUM);
        for( i=0;i<g_TouchScreenContext.handwritingArea.numRegions;i++)
        { 
            if( pointTouched->x > g_TouchScreenContext.handwritingArea.regionArray[i].x1 &&
                    pointTouched->x < g_TouchScreenContext.handwritingArea.regionArray[i].x2 &&
                    pointTouched->y > g_TouchScreenContext.handwritingArea.regionArray[i].y1 &&
                    pointTouched->y < g_TouchScreenContext.handwritingArea.regionArray[i].y2)
            { 
                mmi_trace( g_sw_TPL, "TP find in major area. x1:%d x2:%d y1:%d y2:%d",
                    g_TouchScreenContext.handwritingArea.extRegion.x1,
                    g_TouchScreenContext.handwritingArea.extRegion.x2,
                    g_TouchScreenContext.handwritingArea.extRegion.y1,
                    g_TouchScreenContext.handwritingArea.extRegion.y2);
                return i;
            }
        }
    }
    if( g_TouchScreenContext.handwritingArea.hasExtRegion)
    { 
        if( pointTouched->x > g_TouchScreenContext.handwritingArea.extRegion.x1 &&
                pointTouched->x < g_TouchScreenContext.handwritingArea.extRegion.x2 &&
                pointTouched->y > g_TouchScreenContext.handwritingArea.extRegion.y1 &&
                pointTouched->y < g_TouchScreenContext.handwritingArea.extRegion.y2)
        { 
            mmi_trace( g_sw_TPL, "TP find in ext area. x1:%d x2:%d y1:%d y2:%d",
                    g_TouchScreenContext.handwritingArea.extRegion.x1,
                    g_TouchScreenContext.handwritingArea.extRegion.x2,
                    g_TouchScreenContext.handwritingArea.extRegion.y1,
                    g_TouchScreenContext.handwritingArea.extRegion.y2);
            return EXT_CURRENT_REGION; 
        }
    }

    mmi_trace( g_sw_TPL, "TP The point is not in handwriting area!");
    return INVALID_CURRENT_REGION;
}

/* --------------------------------------------------------------------*/
/** tsdCallback
 * @brief Handle touch panel IRQ sent from edrv. Translate them into mmi
 * event and send.
 * Current we only think three event types: DOWN, UP and MOVE.
 * Others( LONG TAP, REPEAT...) will be implemented or deleted after I get
 * cleared with them...
 * VERY IMPORTANT: DO NOT do any actions will cause high CPU loading capacity
 * in this function (like draw screen, etc..). Actually, this function is running
 * at the IRQ context. 
 * 
 * @param pointTouched
 * @param status
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PRIVATE VOID tsdCallback(TSD_POINT_T* pointTouched,TSD_PEN_STATE_T status)
{ 
    TSD_REPORTING_MODE_T mode;
    UINT32 tempRegion;
    UINT32 a,b;
    static BOOL pendingStrokeDown;
    static BOOL isStrokeDown;
    //Repetition times checking for long tap
    static INT32 longTapCounter;
    sxr_SbxInfo_t  mailboxInfo;
    BOOL mailboxIsFull = FALSE;

    if (GetMmiTaskHandle(MOD_MMI))
        sxr_GetSbxInfo( ((TASK_HANDLE*)GetMmiTaskHandle(MOD_MMI))->nMailBoxId, &mailboxInfo);
    else
        return;

    if (mailboxInfo.NbAvailMsg > MAX_COUNT_MESSAGES)
    {
        mailboxIsFull = TRUE;
    }

    if(gWantChangeHWArea)
        return;
    
    tsd_GetReportingMode( &mode);
    mmi_trace( g_sw_TPL, "TP The tsdCallback   status = %d !",status);

    switch( status)
    { 
        case TSD_PEN_DOWN:
            ASSERT( g_TouchScreenContext.handwritingArea.curRegion == INVALID_CURRENT_REGION);
            if(mailboxIsFull)
            {//If there is too many messages left in the mailbox,
             //just ignore this down event and wait for the next one.
                mode.onDown = TRUE; mode.onPressed = FALSE; mode.onUp = FALSE;
                tsd_SetReportingMode( &mode);
                return;
            }
            if( !IsBacklightOn())
            {//If the backlight is off, turn it on and ignore next up event.
                mode.onDown = TRUE; mode.onPressed = FALSE; mode.onUp = FALSE;
                tsd_SetReportingMode( &mode);
                sendTpEventToMmi( PEN_DOWN, pointTouched->x, pointTouched->y);
                return;
            }

            g_TouchScreenContext.penIsDown = TRUE;
            //Enable pressed and up irq first.
            mode.onDown = FALSE; mode.onPressed = TRUE; mode.onUp = TRUE;
            tsd_SetReportingMode( &mode);
            g_TouchScreenContext.handwritingArea.curRegion = findPointInHandwritingArea( pointTouched);
            memcpy( &g_TouchScreenContext.lastPoint, pointTouched, sizeof( g_TouchScreenContext.lastPoint));
            sendTpEventToMmi( PEN_DOWN, pointTouched->x, pointTouched->y);
            if( g_TouchScreenContext.handwritingArea.curRegion == INVALID_CURRENT_REGION ||
                    g_TouchScreenContext.handwritingArea.curRegion == EXT_CURRENT_REGION)
            {//Couldn't be a handwriting event.
                g_TouchScreenContext.handwritingArea.curRegion = INVALID_CURRENT_REGION;
                pendingStrokeDown = FALSE;
            }
            else
            {
                //The point is in handwriting area!
                //Set the stroke down pending flag to true.
             
                //sendTpEventToMmi( STROKE_DOWN, pointTouched->x, pointTouched->y);
                pendingStrokeDown = TRUE;
            }
            //Reset long tap checking counter.
            longTapCounter = 0;
            break;
        case TSD_PEN_PRESSED:
            if(mailboxIsFull)
            {//If the mailbox is full, ignore this press event.
                return;
            }
            a = ABS( g_TouchScreenContext.lastPoint.x - pointTouched->x);
            b = ABS( g_TouchScreenContext.lastPoint.y - pointTouched->y);
            tempRegion = findPointInHandwritingArea( pointTouched);
            if( g_TouchScreenContext.handwritingArea.curRegion == INVALID_CURRENT_REGION)
            {//Last pend down is not in handwriting area! 
                if( (a*a+b*b) <= (MIN_NOT_STROKE_MOVE_DISTANCE*MIN_NOT_STROKE_MOVE_DISTANCE))
                { 
                    if(longTapCounter == -1) return;
                    longTapCounter++;
                    if(longTapCounter*mode.repetitionPeriod >= LONG_TAP_REPETITION_TIMES)
                    {
                        longTapCounter = -1;
                        sendTpEventToMmi( PEN_LONGTAP,
                                g_TouchScreenContext.lastPoint.x,
                                g_TouchScreenContext.lastPoint.y);
                    }
                    return;
                }
			if(g_move_event_period_num >= MOVE_EVENT_PERIOD)
			{
				g_move_event_period_num = 0;
			}
			else
			{
				g_move_event_period_num++;
				return;
			}
		
                		sendTpEventToMmi( PEN_MOVE, pointTouched->x, pointTouched->y);
            }
            else if( g_TouchScreenContext.handwritingArea.curRegion == tempRegion ||
                    tempRegion == EXT_CURRENT_REGION)
            {//Still in the same handwriting area or the extent area!
                if( pendingStrokeDown)
                {
                    if( (a*a+b*b) <= (MIN_MOVE_DISTANCE*MIN_MOVE_DISTANCE)) return;
                    //There is a down stroke pending.
                    //and we r sure it is a stroke event.Because the move distance is 
                    //long enough. So send the abort event first then send the stroke
                    //down event.
                    pendingStrokeDown = FALSE;
                    isStrokeDown = TRUE;
                    sendTpEventToMmi( PEN_ABORT, pointTouched->x, pointTouched->y);
                    sendTpEventToMmi( STROKE_DOWN,
                            g_TouchScreenContext.lastPoint.x, g_TouchScreenContext.lastPoint.y);
                    sendTpEventToMmi( STROKE_MOVE, pointTouched->x, pointTouched->y);
                }
                else
                {//We don't check the move distance here because we need a smooth
                 //pen track displayed on the screen
                    sendTpEventToMmi( STROKE_MOVE, pointTouched->x, pointTouched->y);
                }
            }
            else
            {//The pen is out of the last handwriting area!
                    if( (a*a+b*b) <= (MIN_MOVE_DISTANCE*MIN_MOVE_DISTANCE)) return;
                    g_TouchScreenContext.penIsDown = FALSE;
                    //Disable next pressed and up event!Cause we have already sent the up event!
                    mode.onDown = TRUE; mode.onPressed = FALSE; mode.onUp = FALSE;
                    tsd_SetReportingMode( &mode);
                    g_TouchScreenContext.handwritingArea.curRegion = INVALID_CURRENT_REGION;
                    if(isStrokeDown)
                    {
                        sendTpEventToMmi( STROKE_UP, pointTouched->x, pointTouched->y);
                        isStrokeDown = FALSE;
                    }
                }
            memcpy( &g_TouchScreenContext.lastPoint, pointTouched, sizeof( g_TouchScreenContext.lastPoint));
            break;
        case TSD_PEN_UP:
            g_TouchScreenContext.penIsDown = FALSE;
            mode.onDown = TRUE; mode.onPressed = FALSE; mode.onUp = FALSE;
            tsd_SetReportingMode( &mode);
            if( g_TouchScreenContext.handwritingArea.curRegion == INVALID_CURRENT_REGION ||
                    pendingStrokeDown)
            { 
                pendingStrokeDown = FALSE;
                sendTpEventToMmi( PEN_UP,
                        g_TouchScreenContext.lastPoint.x, g_TouchScreenContext.lastPoint.y);
            }
            else
            { 
                isStrokeDown = FALSE ;
                g_TouchScreenContext.handwritingArea.curRegion = INVALID_CURRENT_REGION;
                sendTpEventToMmi( STROKE_UP,
                        g_TouchScreenContext.lastPoint.x, g_TouchScreenContext.lastPoint.y);
            }
            break;
        default:
            ASSERT( 0);
            break;
    }
}

/* -------------------------------------------------------------------- */
/** @brief sendTpEventToMmi
 * Send touch panel event to mmi task.
 * @param e Touch panel event ID.
 * @param x Current position x.
 * @param y Current position y.
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
PRIVATE VOID sendTpEventToMmi( Touch_Panel_Event_E e, UINT16 x, UINT16 y)
{ 
    ilm_struct ilm;
    TS_Event_T* data;

    data = OslConstructDataPtr(sizeof(TS_Event_T));
    data->x = x;
    data->y = y;
    data->event = e;

	ilm.dest_mod_id = MOD_MMI;
	ilm.src_mod_id = MOD_L4C;  
	ilm.sap_id = DRIVER_PS_SAP; 
	ilm.local_para_ptr = (VOID*)data;
	ilm.peer_buff_ptr = NULL;
	ilm.msg_id = MSG_ID_TP_EVENT_IND; 
	
	OslMsgSendExtQueue(&ilm);  
}

/* --------------------------------------------------------------------*/
/** mmi_pen_block
 * @brief Disable touch panel reporting.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_block( )
{ 
    mmi_trace( g_sw_TPL, "TP In mmi_pen_block");
    TSD_REPORTING_MODE_T mode;

    tsd_GetReportingMode( &mode);
    mode.onDown = FALSE;
    mode.onPressed = FALSE;
    mode.onUp = FALSE;
    tsd_SetReportingMode( &mode);
}

/* --------------------------------------------------------------------*/
/** mmi_pen_unblock 
 * @brief Enable touch panel reporting.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_unblock( )
{ 
    mmi_trace( g_sw_TPL, "TP In mmi_pen_unblock");
    TSD_REPORTING_MODE_T mode;

    tsd_GetReportingMode( &mode);
    mode.onDown = TRUE;
    mode.onPressed = FALSE;
    mode.onUp = FALSE;
    tsd_SetReportingMode( &mode);
}

/* --------------------------------------------------------------------*/
/** mmi_pen_is_down
 * @brief Check if the pen is down.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC BOOL mmi_pen_is_down( )
{ 
    return g_TouchScreenContext.penIsDown;
}


/* -------------------------------------------------------------------- */
/** @brief mmi_pen_change_handwriting_area
 * Set handwriting area. Should be called before handwriting stroke capture
 * begins.
 * @param num_regions How many regions is the touch screen cut into. Sometimes
 * handwriting library support multiple handwriting area for fast input purpose.
 * @param region_array Rectangles to describe handwriting area.
 * @param ext_region Rectangles to describe external handwriting area.
 * CAREFUL:These two parameters passed into will be used until the handwriting
 * ends! So please use a static variable for this usage!
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */

PUBLIC VOID mmi_pen_change_handwriting_area(
		UINT32 num_regions, 
		const mmi_pen_handwriting_area_struct *region_array, 
		const mmi_pen_handwriting_area_struct *ext_region)
{ 
    gWantChangeHWArea = TRUE;
    mmi_trace( g_sw_TPL,
            "TP In mmi_pen_change_handwriting_area. num_regions:%d, region_array:%08x, ext_region:%08x",
            num_regions, region_array, ext_region);

    ASSERT( num_regions <= TP_MAX_HANDWRITING_AREA_NUM);

    g_TouchScreenContext.handwritingArea.numRegions = num_regions;
    if( num_regions != 0)
    { 
         memcpy( g_TouchScreenContext.handwritingArea.regionArray,
                 region_array,
                 sizeof( mmi_pen_handwriting_area_struct)*num_regions);
    }
    
    if( ext_region != NULL)
    { 
        g_TouchScreenContext.handwritingArea.hasExtRegion = TRUE;
        memcpy( &g_TouchScreenContext.handwritingArea.extRegion, 
                ext_region, 
                sizeof( g_TouchScreenContext.handwritingArea.extRegion));
    }
    else
    { 
        g_TouchScreenContext.handwritingArea.hasExtRegion = FALSE;
    }
    gWantChangeHWArea = FALSE;
//    g_TouchScreenContext.handwritingArea.curRegion = INVALID_CURRENT_REGION;
}

/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_stroke_down_handler
 * Register stroke down event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
PUBLIC VOID mmi_pen_register_stroke_down_handler(mmi_pen_hdlr pen_fp)
{ 
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_stroke_down_handler. ptr:0x%08x",
            pen_fp);
	g_PenEventTable[MMI_PEN_STROKE_DOWN] = pen_fp;
}

/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_stroke_move_handler
 * Register stroke move event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
PUBLIC VOID mmi_pen_register_stroke_move_handler( mmi_pen_hdlr pen_fp)
{ 
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_stroke_move_handler. ptr:0x%08x",
            pen_fp);
	g_PenEventTable[MMI_PEN_STROKE_MOVE] = pen_fp;
}

/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_stroke_up_handler
 * Register stroke up event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
PUBLIC VOID mmi_pen_register_stroke_up_handler(mmi_pen_hdlr pen_fp)
{ 
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_stroke_up_handler. ptr:0x%08x",
            pen_fp);
	g_PenEventTable[MMI_PEN_STROKE_UP] = pen_fp;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_disable_quick_launch
 * @brief Disable quick launch application by the touch panel.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_disable_quick_launch( )
{ 
    g_TouchScreenContext.enableQuickLaunch = FALSE;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_disable_quick_launch
 * @brief Enable quick launch application by the touch panel.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_enable_quick_launch( )
{ 
    g_TouchScreenContext.enableQuickLaunch = TRUE;
}


/* --------------------------------------------------------------------*/
/** mmi_pen_stop_capture_strokes
 * @brief Stop capture strokes. Even if this function is invoked before the last
 * stroke up event, it will be still sent to mmi task.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_stop_capture_strokes()
{ 
    g_TouchScreenContext.handwritingArea.numRegions = 0;
    g_TouchScreenContext.handwritingArea.curRegion = INVALID_CURRENT_REGION;
    g_TouchScreenContext.handwritingArea.hasExtRegion = FALSE;
    g_TouchScreenContext.handwritingArea.pointsStore = NULL;
    g_TouchScreenContext.handwritingArea.curPointPos = 0;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_resume_capture_strokes
 * @brief Resume stroke capture from last pause action.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_resume_capture_strokes()
{ 
	mmi_trace( g_sw_TPL, "TP In mmi_pen_resume_capture_strokes.");
    g_TouchScreenContext.handwritingArea.capturePaused = FALSE;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_pause_capture_strokes 
 * @brief This function will pause stroke capture until resume function called.
 * If the stroke capture is paused, all of the stroke event won't be sent.
 * And also the stroke track will be stopped.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_pause_capture_strokes()
{ 
	mmi_trace( g_sw_TPL, "TP In mmi_pen_pause_capture_strokes.");
    g_TouchScreenContext.handwritingArea.capturePaused = TRUE;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_end_strokes_of_character
 * @brief Call this function when the LAST stroke of a character is done.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_end_strokes_of_character(void)
{ 
    pushEndCharToHandwritingStore( );
}

/* --------------------------------------------------------------------*/
/** mmi_pen_peek_stroke_state
 * @brief Check if a stroke is finished.
 * 
 * @param has_unfinished_stroke
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_peek_stroke_state(BOOL *has_unfinished_stroke)
{ 
    if( mmi_pen_is_down( )) 
    { 
        *has_unfinished_stroke = TRUE; 
    }
    else
    { 
        *has_unfinished_stroke = FALSE;
    }
}

/* --------------------------------------------------------------------*/
/** mmi_pen_begin_strokes_of_character
 * @brief Call this function to start record strokes of a character. Call 
 * it every time before enter a new character.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_begin_strokes_of_character(void)
{ 
    g_TouchScreenContext.handwritingArea.curPointPos = 0;
    memset( g_TouchScreenContext.handwritingArea.pointsStore, 
            0, 
            sizeof( mmi_pen_point_struct)*g_TouchScreenContext.handwritingArea.maxPoints);
}


/* --------------------------------------------------------------------*/
/** mmi_pen_start_capture_strokes
 * @brief Start capture strokes.
 * 
 * @param max_points How many points can be stored in the given points array.
 * @param point_array Array address for storing strokes.
 * @param num_regions How many handwriting areas exist.
 * @param region_array Handwriting area coordinate array.
 * @param ext_region Extent handwriting area coordinate.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
PUBLIC VOID mmi_pen_start_capture_strokes( UINT32 max_points, 
        mmi_pen_point_struct *point_array,
        UINT32 num_regions,
        const mmi_pen_handwriting_area_struct *region_array,
        const mmi_pen_handwriting_area_struct *ext_region)
{ 
    ASSERT( max_points > 0);
    ASSERT( g_TouchScreenContext.handwritingArea.pointsStore != NULL);

    g_TouchScreenContext.handwritingArea.maxPoints = max_points;
    g_TouchScreenContext.handwritingArea.curPointPos = 0;
    g_TouchScreenContext.handwritingArea.pointsStore = point_array;
    g_TouchScreenContext.handwritingArea.capturePaused = FALSE;
    memset( g_TouchScreenContext.handwritingArea.pointsStore, 
            0, 
            sizeof( mmi_pen_point_struct)*g_TouchScreenContext.handwritingArea.maxPoints);
    mmi_pen_change_handwriting_area( num_regions, region_array, ext_region);
}

PRIVATE VOID pushPointToHandwritingStore( mmi_pen_point_struct* point)
{ 

	mmi_trace( g_sw_TPL, "TP In pushPointToHandwritingStore. %d %d", 
            point->x, point->y);
//    ASSERT( g_TouchScreenContext.handwritingArea.pointsStore != NULL);
    if(g_TouchScreenContext.handwritingArea.pointsStore == NULL)
    	{
		return;
    	}
    if( g_TouchScreenContext.handwritingArea.curPointPos >= 
            g_TouchScreenContext.handwritingArea.maxPoints - 2)
    {//Points stores if full. Leave one space for end stroke and end handwriting symbol.
        mmi_trace( g_sw_TPL, "TP Points store is full");
        return; 
    }

    memcpy( &g_TouchScreenContext.handwritingArea.pointsStore[g_TouchScreenContext.handwritingArea.curPointPos],
            point, sizeof( mmi_pen_point_struct));
    g_TouchScreenContext.handwritingArea.curPointPos++;
}

PRIVATE VOID pushEndCharToHandwritingStore( )
{ 
	mmi_pen_point_struct point= { TP_HANDWRITING_CHAR_END_X, TP_HANDWRITING_CHAR_END_Y};
	mmi_trace(g_sw_TPL, "TP In pushEndStrokeToHandwritingStore.");
    ASSERT( g_TouchScreenContext.handwritingArea.pointsStore != NULL);
    ASSERT( g_TouchScreenContext.handwritingArea.curPointPos < 
            g_TouchScreenContext.handwritingArea.maxPoints);

	    if(g_TouchScreenContext.handwritingArea.pointsStore == NULL)
    	{
		return;
    	}

    if( g_TouchScreenContext.handwritingArea.curPointPos >= 
            g_TouchScreenContext.handwritingArea.maxPoints)
    {//Points stores if full. Leave one space for end symbol.
        mmi_trace( g_sw_TPL, "TP Points store is full");
        return; 
    }
    memcpy( &g_TouchScreenContext.handwritingArea.pointsStore[g_TouchScreenContext.handwritingArea.curPointPos],
            &point, sizeof( mmi_pen_point_struct));
    g_TouchScreenContext.handwritingArea.curPointPos++;
}

PRIVATE VOID pushEndStrokeToHandwritingStore( )
{ 
    mmi_pen_point_struct point= { TP_HANDWRITING_STROKE_END_X, TP_HANDWRITING_STROKE_END_Y};
    mmi_trace(g_sw_TPL, "TP In pushEndStrokeToHandwritingStore.");
    ASSERT( g_TouchScreenContext.handwritingArea.pointsStore != NULL);
    ASSERT( g_TouchScreenContext.handwritingArea.curPointPos < 
    g_TouchScreenContext.handwritingArea.maxPoints);

    if(g_TouchScreenContext.handwritingArea.pointsStore == NULL)
    {
        return;
    }

    if( g_TouchScreenContext.handwritingArea.curPointPos >= 
    g_TouchScreenContext.handwritingArea.maxPoints - 1)
    {//Points stores if full. Leave one space for end symbol.
        mmi_trace( g_sw_TPL, "TP Points store is full");
        return; 
    }
    memcpy( &g_TouchScreenContext.handwritingArea.pointsStore[g_TouchScreenContext.handwritingArea.curPointPos],
    &point, sizeof( mmi_pen_point_struct));
    g_TouchScreenContext.handwritingArea.curPointPos++;
}

/* --------------------------------------------------------------------*/
/** mmi_pen_check_inside_polygon
 * @brief Check if a point is in given polygon.
 * 
 * @param polygon See definition of mmi_pen_polygon_area_struct.
 * @param pos Given point to check.
 * 
 * @return If the point is in the polygon, return TRUE.
 */
/* --------------------------------------------------------------------*/
PUBLIC BOOL mmi_pen_check_inside_polygon( mmi_pen_polygon_area_struct* polygon, mmi_pen_point_struct pos )
{ 
    BOOL c = FALSE;
    S16 i, j;
    S16 nPoints = polygon->num;
    mmi_pen_point_struct *outline = polygon->points;
    S16 x = pos.x;
    S16 y = pos.y;

    for( i = 0, j = nPoints-1; i < nPoints; j = i++ )
    {
        mmi_pen_point_struct* a = &outline[i];
        mmi_pen_point_struct* b = &outline[j];
        if( ((a->y <= y && y < b->y) || (b->y <= y && y < a->y)) &&
            (x < (b->x - a->x)*(y - a->y)/(b->y - a->y) + a->x) )
            c = !c;
    }
    
    return c;
}

PUBLIC BOOL mmi_pen_check_inside_rect( mmi_pen_polygon_area_struct* polygon, mmi_pen_point_struct pos )
{ 
    BOOL c = FALSE;
    S16 nPoints = polygon->num;
    mmi_pen_point_struct *outline = polygon->points;
    S16 x = pos.x;
    S16 y = pos.y;

    if( nPoints!= 4)
    {
        return mmi_pen_check_inside_polygon(polygon,pos);
    }
    else
    {
        if((x>=outline[0].x&&x<=outline[3].x)&&(y>=outline[0].y&&y<=outline[3].y))
        {
            return TRUE;
        }
        return FALSE;
        
        return c;
    }
}


//@TODO:Below functions is only for compiling purpose now.
//Some of them will be deleted and others will be implemented in the future.
PUBLIC VOID mmi_pen_register_repeat_handler(mmi_pen_hdlr pen_fp)
{ 

}

PUBLIC VOID mmi_pen_reset(void)
{ 
    TSD_REPORTING_MODE_T mode;
    
    mmi_trace( g_sw_TPL, "TP in mmi_pen_reset");

    tsd_GetReportingMode( &mode);
    
    //Only when the pen is in down state we need to reset.
    //Disable all irq except next down.
    if( g_TouchScreenContext.penIsDown)
    { 
        mode.onDown = TRUE; mode.onPressed = FALSE; mode.onUp = FALSE;
        tsd_SetReportingMode( &mode);
        penAbortHandler( g_TouchScreenContext.lastPoint.x,
                g_TouchScreenContext.lastPoint.y);
    }

    g_TouchScreenContext.penIsDown = FALSE;
}

#elif ( defined(__MMI_TOUCH_SCREEN__) || defined(__MMI_HANDWRITING_PAD__) ) && defined(MMI_ON_WIN32) 
/*****************************************************************************
 *
 *	WIN32 integration 
 *	PC Simulator & MODIS
 *
 ****************************************************************************/

#include "stdc.h"
#include "mmi_data_types.h"
#include "timerevents.h"
#include "keybrd.h"
#include "frameworkstruct.h"
#include "eventsgprot.h"
#include "eventsdcl.h"
#include "eventsdef.h"
#include "gui_data_types.h"
#include "gui.h"
#include "idleappprot.h"

#include "kal_non_specific_general_types.h"
#include "touchscreengprot.h"
#include "debuginitdef.h"

#include "lcd_sw_rnd.h" /* lcd width/height */
#include "time.h" /* time() */

#include "gdi_include.h"
#include "screenrotationgprot.h"

//#include "touch_panel_custom.h"    /* get handwriting pad setting */

#if !defined(MMI_ON_WIN32)
static void mmi_pen_MNT_start_timer(void);
static void mmi_pen_MNT_flush_queue(void);
#endif 

/***************************************************************************** 
* Define
*****************************************************************************/

#define MMI_PEN_MAX_HANDWRITING_REGION (3)

#undef MMI_PEN_UNIT_TEST
extern void mmi_pen_enable(void);
extern void mmi_pen_disable(void);

/***************************************************************************** 
* Typedef 
*****************************************************************************/

typedef struct 
{
	/* mmi_pen_block() & mmi_pen_unblock() */
	U32 is_blocked : 1;

	/* mmi_pen_enable() & mmi_pen_disable() */
	U32 is_enabled : 1;

	/* mmi_pen_reset() set 'is_pen_down' to 0 */
	U32 is_pen_down : 1;

	/* The current pen down/move/up is a stroke */
	U32 is_stroke_event : 1;

	/* Checking offset with g_pen_stroke_min_offset */
	U32 is_pending_stroke_event : 1;  

	/* A first stroke is created. This flag is cleared in mmi_pen_begin_strokes_of_character() */
	U32 is_stroke_created : 1;  

	/* Stroke LongTap event is generated. */
	U32 is_stroke_longtap_event_generated : 1;  	

	/* Set in mmi_pen_end_strokes_of_character(), 
	 * do not enqueue incoming strokes until mmi_pen_begin_strokes_of_character() is invoked again */
	U32 is_all_stroke_finished : 1;  

	/* 1: long-tap 0: repeat */
	U32 is_waiting_long_tap_event : 1;

	/* 
	 * Set by mmi_pen_change_handwriting_area() and mmi_pen_stop_capture_strokes().
	 * Close stroke and reset touch screen when pen is up. 
	 */
	U32 reset_stroke_on_pen_up : 1;

	/* The position when pen is tapped down */
	mmi_pen_point_struct pen_down_pos; 

	/* The current pen position */
	mmi_pen_point_struct pen_current_pos;

	/* For Pen Repeat */
 	mmi_pen_point_struct pen_repeat_origin_pos; 

	/* For stroke */
	S32 num_points_queued;
} mmi_pen_context_struct;


/***************************************************************************** 
* Local Variable
*****************************************************************************/

static mmi_pen_context_struct g_pen_cntx;

static mmi_pen_hdlr g_pen_event_table[MMI_PEN_EVENT_TYPE_MAX];
static void (*g_pen_stroke_pre_move)(void);
static void (*g_pen_stroke_post_move)(void);

static int g_pen_num_stroke_area; /* 0 if stroke area is not enabled */
static mmi_pen_handwriting_area_struct g_pen_stroke_areas[MMI_PEN_MAX_HANDWRITING_REGION];
static mmi_pen_handwriting_area_struct g_pen_ext_stroke_area;
static int g_pen_stroke_min_offset;
static S32 g_pen_stroke_max_points;
static mmi_pen_point_struct *g_pen_stroke_points;

/***************************************************************************** 
* Local Function
*****************************************************************************/
PUBLIC VOID mmi_pen_enable_quick_launch( )
{
  return;
}
PUBLIC VOID mmi_pen_disable_quick_launch( )
{
  return;
}
PUBLIC VOID mmi_pen_pause_capture_strokes()
{
}
PUBLIC VOID mmi_pen_resume_capture_strokes()
{
}
static void mmi_pen_simulator_fix_point_bound(S16 *x, S16 *y)
{
	if (*x < 0)
		*x = 0;
	else if (*x >= UI_device_width)
		*x = UI_device_width - 1;
	if (*y < 0)
		*y = 0;
	else if (*y >= UI_device_height)
		*y = UI_device_height - 1;
}


static S16 mmi_pen_simulator_pixel_diff(mmi_pen_point_struct pos1, mmi_pen_point_struct pos2)
{
	S16 diff_x = pos1.x - pos2.x;
	S16 diff_y = pos1.y - pos2.y;
	if (diff_x < 0)
	{
		diff_x = -diff_x;
	}
	if (diff_y < 0)
	{
		diff_y = -diff_y;
	}
	return diff_x + diff_y;
}


static BOOL mmi_pen_simulator_in_stroke_area(S16 x, S16 y)
{
	int i;

	if (g_pen_cntx.is_stroke_created &&
	  x >= g_pen_ext_stroke_area.x1 && x <= g_pen_ext_stroke_area.x2 &&
	  y >= g_pen_ext_stroke_area.y1 && y <= g_pen_ext_stroke_area.y2)
	{
		return MMI_TRUE;
	}
	
	for (i = 0; i < g_pen_num_stroke_area; i++)
	{
		if ( x >= g_pen_stroke_areas[i].x1 && x <= g_pen_stroke_areas[i].x2 &&
		   y >= g_pen_stroke_areas[i].y1 && y <= g_pen_stroke_areas[i].y2)
		{
			return MMI_TRUE;
		}
	}
	return MMI_FALSE;
}


static void mmi_pen_simulator_push_stroke_point(S16 x, S16 y)
{
	if (g_pen_cntx.num_points_queued >= g_pen_stroke_max_points - 2)
		return;
	g_pen_stroke_points[g_pen_cntx.num_points_queued].x = x;
	g_pen_stroke_points[g_pen_cntx.num_points_queued].y = y;
	g_pen_cntx.num_points_queued++;
}


static void mmi_pen_simulator_push_stroke_end(void)
{	
	if (g_pen_cntx.num_points_queued >= g_pen_stroke_max_points - 1)
	{
		MMI_ASSERT(0); // queue full
		return;
	}
#if defined(__MMI_HANWANG__)
	g_pen_stroke_points[g_pen_cntx.num_points_queued].x = -1;
	g_pen_stroke_points[g_pen_cntx.num_points_queued].y = 0;
#elif defined(__MMI_PENPOWER__)
	g_pen_stroke_points[g_pen_cntx.num_points_queued].x = -1;
	g_pen_stroke_points[g_pen_cntx.num_points_queued].y = -1;
#else
	g_pen_stroke_points[g_pen_cntx.num_points_queued].x = -1;
	g_pen_stroke_points[g_pen_cntx.num_points_queued].y = 0;
#endif
	g_pen_cntx.num_points_queued++;
}


static void mmi_pen_simulator_push_char_end(void)
{
	if (g_pen_cntx.num_points_queued >= g_pen_stroke_max_points)
	{
		MMI_ASSERT(0); // queue full
		return;
	}
#if defined(__MMI_HANWANG__)
	g_pen_stroke_points[g_pen_cntx.num_points_queued].x = -1;
	g_pen_stroke_points[g_pen_cntx.num_points_queued].y = -1;
#elif defined(__MMI_PENPOWER__)
	g_pen_stroke_points[g_pen_cntx.num_points_queued].x = -1;
	g_pen_stroke_points[g_pen_cntx.num_points_queued].y = 0;
#else
	g_pen_stroke_points[g_pen_cntx.num_points_queued].x = -1;
	g_pen_stroke_points[g_pen_cntx.num_points_queued].y = -1;
#endif
	g_pen_cntx.num_points_queued++;
}


static void mmi_pen_simulator_repeat_hdlr(void)
{
	MMI_ASSERT(g_pen_cntx.is_enabled && g_pen_cntx.is_pen_down);

	if (g_pen_cntx.is_waiting_long_tap_event)
	{
		if (g_pen_event_table[MMI_PEN_EVENT_LONG_TAP])
		{
			(g_pen_event_table[MMI_PEN_EVENT_LONG_TAP])(g_pen_cntx.pen_down_pos);
		}

		g_pen_cntx.is_waiting_long_tap_event = 0;	
	}
	else
	{
		if (g_pen_event_table[MMI_PEN_EVENT_REPEAT])
		{
			(g_pen_event_table[MMI_PEN_EVENT_REPEAT])(g_pen_cntx.pen_repeat_origin_pos);
		}
	}

	/* touch screen might be disabled/reset inside pen handler */
	if (g_pen_cntx.is_enabled && g_pen_cntx.is_pen_down)
	{
		coolsand_UI_start_timer(MMI_PEN_REPEAT_TIME*10, mmi_pen_simulator_repeat_hdlr);	
	}
}


static void mmi_pen_simulator_stroke_longtap_hdlr(void)
{
	MMI_ASSERT(g_pen_cntx.is_enabled && g_pen_cntx.is_pen_down && g_pen_cntx.is_stroke_event);

	g_pen_cntx.is_stroke_longtap_event_generated = 1;

	if (g_pen_event_table[MMI_PEN_STROKE_LONGTAP])
	{
		(g_pen_event_table[MMI_PEN_STROKE_LONGTAP])(g_pen_cntx.pen_current_pos);
	}
}


void mmi_pen_simulator_button_down_hdlr(S16 x, S16 y)
{
	mmi_pen_point_struct pos;

//	if (!g_pen_cntx.is_enabled || g_pen_cntx.is_all_stroke_finished)
//		return;
#if defined(GDI_USING_LAYER_ROTATE)
	{
		S32 xx, yy;
		xx = x;
		yy = y;
		gdi_rotate_map_lcd_to_layer(gdi_layer_get_actual_rotate(), &xx, &yy);
		x = xx;
		y = yy;
	}
#endif

	MMI_DBG_ASSERT(!g_pen_cntx.is_pen_down);

	if(coolsand_UI_cancel_timer == NULL) return; //tmp

	mmi_pen_simulator_fix_point_bound(&x, &y);
	pos.x = x;
	pos.y = y;
	g_pen_cntx.pen_down_pos = g_pen_cntx.pen_current_pos = g_pen_cntx.pen_repeat_origin_pos = pos;
	g_pen_cntx.is_pen_down = 1;
	g_pen_cntx.is_stroke_event = 0;
	g_pen_cntx.is_pending_stroke_event = 0;
	g_pen_cntx.is_stroke_longtap_event_generated = 0;
	/* Not reset is_stroke_created */
	g_pen_cntx.is_waiting_long_tap_event = 0;

	if (mmi_pen_simulator_in_stroke_area(x, y))
	{ 
		if (!g_pen_cntx.is_stroke_created && g_pen_stroke_min_offset > 0)
		{
			g_pen_cntx.is_pending_stroke_event = 1;

			/* Fire pen down event. For example, it will display key down on virtual keyboard */ 
			if (g_pen_event_table[MMI_PEN_EVENT_DOWN])
			{
				(g_pen_event_table[MMI_PEN_EVENT_DOWN])(pos);
			}
		}
		else
		{
			if (!g_pen_cntx.is_stroke_created)
			{
				coolsand_UI_start_timer(MMI_PEN_STROKE_LONGTAP_TIME*10, mmi_pen_simulator_stroke_longtap_hdlr);
			}

			g_pen_cntx.is_stroke_event = 1;
			g_pen_cntx.is_stroke_created = 1;

			mmi_pen_simulator_push_stroke_point(x, y);
			
			if (g_pen_event_table[MMI_PEN_STROKE_DOWN])
			{
				(g_pen_event_table[MMI_PEN_STROKE_DOWN])(pos);
			}
		}
	}
	else
	{

		/* Note that Pen Down handler might call mmi_pen_reset() */
		if (g_pen_event_table[MMI_PEN_EVENT_DOWN])
		{
			(g_pen_event_table[MMI_PEN_EVENT_DOWN])(pos);
		}

		/* touch screen might be disabled inside pen handler */
		if (g_pen_cntx.is_enabled && g_pen_cntx.is_pen_down)
		{
			g_pen_cntx.is_waiting_long_tap_event = 1;
			coolsand_UI_start_timer(MMI_PEN_REPEAT_TIME*10, mmi_pen_simulator_repeat_hdlr);
		}
	}
}


void mmi_pen_simulator_button_move_hdlr(S16 x, S16 y)
{
	mmi_pen_point_struct pos;
	BOOL restart_repeat_timer = MMI_FALSE;

//	if (!g_pen_cntx.is_enabled || !g_pen_cntx.is_pen_down)
//		return;
#if defined(GDI_USING_LAYER_ROTATE)
		{
			S32 xx, yy;
			xx = x;
			yy = y;
			gdi_rotate_map_lcd_to_layer(gdi_layer_get_actual_rotate(), &xx, &yy);
			x = xx;
			y = yy;
		}
#endif
	
	mmi_pen_simulator_fix_point_bound(&x, &y);
	pos.x = x;
	pos.y = y;
	g_pen_cntx.pen_current_pos = pos;

	if(coolsand_UI_cancel_timer == NULL) return;//tmp

	if (g_pen_cntx.is_pending_stroke_event)
	{
		if ( mmi_pen_simulator_pixel_diff(pos, g_pen_cntx.pen_down_pos) > g_pen_stroke_min_offset)
		{
			g_pen_cntx.is_pending_stroke_event = 0;
			g_pen_cntx.is_stroke_event = 1;
			g_pen_cntx.is_stroke_created = 1;

			/* Abort previous pen down event */
			if (g_pen_event_table[MMI_PEN_EVENT_ABORT])
			{
				(g_pen_event_table[MMI_PEN_EVENT_ABORT])(pos);
			}

			mmi_pen_simulator_push_stroke_point(g_pen_cntx.pen_down_pos.x, g_pen_cntx.pen_down_pos.y);
			mmi_pen_simulator_push_stroke_point(x, y);

			if (g_pen_event_table[MMI_PEN_STROKE_DOWN])
			{
				(g_pen_event_table[MMI_PEN_STROKE_DOWN])(g_pen_cntx.pen_down_pos);
			}

			if (g_pen_stroke_pre_move)
				g_pen_stroke_pre_move();
			
			if (g_pen_event_table[MMI_PEN_STROKE_MOVE])
			{
				(g_pen_event_table[MMI_PEN_STROKE_MOVE])(pos);
			}

			if (g_pen_stroke_post_move)
				g_pen_stroke_post_move();
			
		}
		/* Otherwise, ignore the move event */
	}
	else if (g_pen_cntx.is_stroke_event)
	{
		if (g_pen_stroke_pre_move)
			g_pen_stroke_pre_move();

		if (g_pen_event_table[MMI_PEN_STROKE_MOVE])
		{
			(g_pen_event_table[MMI_PEN_STROKE_MOVE])(pos);
		}

		if (g_pen_stroke_post_move)
			g_pen_stroke_post_move();
		
		mmi_pen_simulator_push_stroke_point(x, y);

		if (mmi_pen_simulator_pixel_diff(pos, g_pen_cntx.pen_down_pos) > MMI_PEN_SKIP_STROKE_LONGTAP_OFFSET)
		{
			coolsand_UI_cancel_timer(mmi_pen_simulator_stroke_longtap_hdlr);
		}
	}
	else
	{
		if (g_pen_event_table[MMI_PEN_EVENT_MOVE])
		{
			(g_pen_event_table[MMI_PEN_EVENT_MOVE])(pos);
		}
		
		if ( mmi_pen_simulator_pixel_diff(pos, g_pen_cntx.pen_repeat_origin_pos) > MMI_PEN_SKIP_LONGTAP_OFFSET)
		{
			g_pen_cntx.is_waiting_long_tap_event = 0; /* Wait Pen Repeat because Pen LongTap only happens at the pen down position */
			g_pen_cntx.pen_repeat_origin_pos = pos;
			coolsand_UI_cancel_timer(mmi_pen_simulator_repeat_hdlr);
			if (g_pen_cntx.is_enabled && g_pen_cntx.is_pen_down)
			{
				coolsand_UI_start_timer(MMI_PEN_REPEAT_TIME*10, mmi_pen_simulator_repeat_hdlr);
			}
		}
	}
}


void mmi_pen_simulator_button_up_hdlr(S16 x, S16 y)
{
	mmi_pen_point_struct pos;

	//if (!g_pen_cntx.is_enabled || !g_pen_cntx.is_pen_down)
	//	return;
	if(coolsand_UI_cancel_timer == NULL) return;//tmp
#if defined(GDI_USING_LAYER_ROTATE)
		{
			S32 xx, yy;
			xx = x;
			yy = y;
			gdi_rotate_map_lcd_to_layer(gdi_layer_get_actual_rotate(), &xx, &yy);
			x = xx;
			y = yy;
		}
#endif

	mmi_pen_simulator_fix_point_bound(&x, &y);
	pos.x = x;
	pos.y = y;
	g_pen_cntx.pen_current_pos = pos;
	g_pen_cntx.is_pen_down = 0;

	if (g_pen_cntx.is_pending_stroke_event)
	{
		if (g_pen_event_table[MMI_PEN_EVENT_UP])
		{
			(g_pen_event_table[MMI_PEN_EVENT_UP])(g_pen_cntx.pen_down_pos);
		}
	}
	else if (g_pen_cntx.is_stroke_event)
	{
		if (g_pen_event_table[MMI_PEN_STROKE_UP])
		{
			(g_pen_event_table[MMI_PEN_STROKE_UP])(pos);
		}
		mmi_pen_simulator_push_stroke_point(x, y);
		mmi_pen_simulator_push_stroke_end();

		coolsand_UI_cancel_timer(mmi_pen_simulator_stroke_longtap_hdlr);
		if (g_pen_cntx.is_stroke_longtap_event_generated)
		{
			/* Clear strokes */
			mmi_pen_end_strokes_of_character();
			mmi_pen_begin_strokes_of_character();
		}
	}
	else
	{
		coolsand_UI_cancel_timer(mmi_pen_simulator_repeat_hdlr);
	
		if (g_pen_event_table[MMI_PEN_EVENT_UP])
		{
			(g_pen_event_table[MMI_PEN_EVENT_UP])(pos);
		}
	}

	if (g_pen_cntx.reset_stroke_on_pen_up)
	{
		if (g_pen_stroke_max_points > 0)
		{
			mmi_pen_end_strokes_of_character();
			mmi_pen_reset();
			mmi_pen_begin_strokes_of_character();
		}
		else
		{
			mmi_pen_reset();
		}
	}
}


/***************************************************************************** 
* Global Variable
*****************************************************************************/

/***************************************************************************** 
* Global Function
*****************************************************************************/

/* Dummy pen handler */
void mmi_pen_dummy_hdlr(mmi_pen_point_struct pos)
{
	UI_UNUSED_PARAMETER(pos);
}


void mmi_pen_config_sampling_period(kal_uint32 low, kal_uint32 high)
{
	// DUMMY
}


void mmi_pen_config_timeout_period(kal_uint32 longtap, kal_uint32 repeat, kal_uint32 stroke_longtap)
{
	// DUMMY
}

void mmi_pen_config_move_offset(kal_uint32 event_based, kal_uint32 stroke_based, kal_uint32 long_tap, kal_uint32 stroke_long_tap)
{
	// DUMMY
}

/*****************************************************************************
* FUNCTION
*	mmi_pen_init
* DESCRIPTION
*	Initialize pen system
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_init(void)
{
#ifdef __MMI_HANDWRITING_PAD__
	mmi_frm_setup_default_pen_handler();
#endif
	// DUMMY
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_block
* DESCRIPTION
*	Block pen system 
*
*	Note: typically used for keypad lock in idle screen
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_block(void)
{
	mmi_pen_disable();
	g_pen_cntx.is_blocked = 1;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_unblock
* DESCRIPTION
*	Unblock pen system 
*
*	Note: typically used for keypad lock in idle screen
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_unblock(void)
{
	g_pen_cntx.is_blocked = 0;
	mmi_pen_enable();
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_enable
* DESCRIPTION
*	Enable pen system
*
*	Note: typically on Keypad Up
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_enable(void)
{
	if (g_pen_cntx.is_blocked || g_pen_cntx.is_enabled)
	{
		return;
	}

	memset(&g_pen_cntx, 0, sizeof(g_pen_cntx));
	g_pen_cntx.is_enabled = 1;

#if !defined(MMI_ON_WIN32)
	mmi_pen_MNT_start_timer();
#endif
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_disable
* DESCRIPTION
*	Disable pen system
*
*	Note: typically on Keypad Down because we don't want to use keypad and touch screen 
*	at the same time
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_disable(void)
{
	if (g_pen_cntx.is_blocked || !g_pen_cntx.is_enabled)
	{
		return;
	}

	coolsand_UI_cancel_timer(mmi_pen_simulator_repeat_hdlr);

	if (g_pen_cntx.is_pen_down)
	{
		if (g_pen_event_table[MMI_PEN_EVENT_ABORT])
		{
			(g_pen_event_table[MMI_PEN_EVENT_ABORT])(g_pen_cntx.pen_current_pos);
		}
	}
	
	memset(&g_pen_cntx, 0, sizeof(g_pen_cntx));

#if !defined(MMI_ON_WIN32)
	mmi_pen_MNT_flush_queue();
#endif 
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_reset
* DESCRIPTION
*	Reset the status of touch screen
*	- Flush event queue
*	- If the pen is currently tapped down, ignore all subsequent pen events until the pen is up.
*
*	Note: typically on MMI screen switching
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_reset(void)
{
	if (g_pen_cntx.is_blocked || !g_pen_cntx.is_enabled)
	{
		return;
	}

	coolsand_UI_cancel_timer(mmi_pen_simulator_repeat_hdlr);

	if (g_pen_cntx.is_pen_down)
	{
		coolsand_UI_cancel_timer(mmi_pen_simulator_repeat_hdlr);
	
		if (g_pen_event_table[MMI_PEN_EVENT_ABORT])
		{
			(g_pen_event_table[MMI_PEN_EVENT_ABORT])(g_pen_cntx.pen_current_pos);
		}
	}
	
	memset(&g_pen_cntx, 0, sizeof(g_pen_cntx));
	g_pen_cntx.is_enabled = 1;

#if !defined(MMI_ON_WIN32)
	mmi_pen_MNT_flush_queue();
#endif 
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_get_state
* DESCRIPTION
*	Get the current state of touch screen 
*
* PARAMETERS
*	enabled			OUT	
*	pen_down			OUT	
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_get_state(kal_bool *is_enabled, kal_bool *is_pen_down)
{
	if (g_pen_cntx.is_enabled)
	{
		*is_enabled = KAL_TRUE;
		if (g_pen_cntx.is_pen_down)
		{
			*is_pen_down = KAL_TRUE;
		}
		else
		{
			*is_pen_down = KAL_FALSE;
		}
	}
	else
	{
		*is_enabled = KAL_FALSE;
		*is_pen_down = KAL_FALSE;
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_start_calibration
* DESCRIPTION
*	Start pen calibration
*
* PARAMETERS
*	num		IN		number of calibration points
*	points	IN		calibration points
* RETURNS
*  void
* GLOBALS AFFECTED
*  
 
*	After mmi_pen_reset(), the calibration process is terminated.
*****************************************************************************/
void mmi_pen_start_calibration(kal_uint16 num, const mmi_pen_point_struct *points)
{
	// DUMMY
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_set_calibration_data
* DESCRIPTION
*	Assign driver calibration data 
*
* PARAMETERS
*	data		IN		
* RETURNS
*  void
* GLOBALS AFFECTED
*  
 
*	
*****************************************************************************/
 BOOL mmi_pen_set_calibration_data( UINT16 x, UINT16 y, UINT16 index)

{
    return 1;
	// DUMMY 
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_read_calibration_data
* DESCRIPTION
*	Read the current value of driver calibration data 
*
* PARAMETERS
*	data		OUT		
* RETURNS
*  void
* GLOBALS AFFECTED
*  
 
*	
*****************************************************************************/
/*void mmi_pen_read_calibration_data(mmi_pen_calibration_struct *data)
{
	// DUMMY
}
*/

/*****************************************************************************
* FUNCTION
*	mmi_pen_register_down_handler
* DESCRIPTION
*	Register the Pen Down handler
*
* PARAMETERS
*	pen_fp		IN		callback handler
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_down_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_EVENT_DOWN] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_long_tap_handler
* DESCRIPTION
*	Register the Pen LongTap handler
*
*	LongTap handler is invoked when  the pen is tapped for a period of time 
*	and stays at the same place where it is tapped down.
*
*	It is invoked atmost one time before pen up.
*
* PARAMETERS
*	pen_fp		IN		callback handler 
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_long_tap_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_EVENT_LONG_TAP] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_repeat_handler
* DESCRIPTION
*	Register the Pen Repeat handler.
*
*	Repeat handler is invoked after LongTap handler.
*	However, unlike LongTap handler, Repeat handler is invoked even if 
*	it does not stays at the same place as Pen Down.
*
*	it might be invoked more than one times before pen up.
*
* PARAMETERS
*	pen_fp		IN		callback handler
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_repeat_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_EVENT_REPEAT] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_move_handler
* DESCRIPTION
*	Register the Pen Move handler.
*
*	The invocation frequency of Pen Move handler is typically less than driver sampling rate.
*
* PARAMETERS
*	pen_fp		IN		callback handler
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_move_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_EVENT_MOVE] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_up_handler
* DESCRIPTION
*	Register the Pen Up handler.
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_up_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_EVENT_UP] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_abort_handler
* DESCRIPTION
*	Register the Pen Abort handler.
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_abort_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_EVENT_ABORT] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_start_capture_strokes
* DESCRIPTION
*	Start to capture stroke inside the hand-written area.
*	It is typically used in entry function of editor screen.
*	The points are en-queued in 'point_array' in the following format. 
*	Each point contains two 16-bit integers for x, y coordinates. Two strokes are separated with 
 
*	when mmi_pen_end_strokes_of_character(). The parameter 'max_points' should include 
 
*	(x0, y0) (x1, y1) ¡K (0xffff, 0), ¡K(0xffff, 0), ¡K(0xffff, 0xffff)
*
*	We has the following types of stroke-based handler
*	- Stroke Down
*	- Stroke Move
*	- Stroke Up
*	However, it does not have Stroke LongTap nor Stroke Repeat handler.
*
*	If the pen is tapped down outside the hand-written area, it will invoke Event-based handler,
*	otherwise, it will invoke Stroke-based handler if pen moves across MMI_PEN_STROKE_MIN_OFFSET pixels
*
*
* PARAMETERS
*	max_points		IN	Maximum number of points can be en-queued.
*	point_array		IN	Array for storing sampled points in strokes.
*	num_regions		IN	Number of handwriting regions
*	region_array	IN	Handwriting regions 
*	ext_region		IN	Handwriting region after the first stroke is generated. (NULL to ignore it)
*
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_start_capture_strokes(
					kal_uint32 max_points, mmi_pen_point_struct *point_array, 
					kal_uint32 num_regions, const mmi_pen_handwriting_area_struct *region_array, 
					const mmi_pen_handwriting_area_struct *ext_region)
{
	if ( num_regions > MMI_PEN_MAX_HANDWRITING_REGION)
		num_regions = MMI_PEN_MAX_HANDWRITING_REGION ;

	g_pen_num_stroke_area = num_regions;
	memcpy(g_pen_stroke_areas, region_array, sizeof(mmi_pen_handwriting_area_struct) * num_regions);
	
	if (ext_region)
	{
		g_pen_ext_stroke_area = *ext_region;
	}
	else
	{
		g_pen_ext_stroke_area.x1 = -1;
		g_pen_ext_stroke_area.x2 = -1;
		g_pen_ext_stroke_area.y1 = -1;
		g_pen_ext_stroke_area.y2 = -1;
	}
	
	g_pen_stroke_min_offset = MMI_PEN_STROKE_MIN_OFFSET;

	g_pen_stroke_max_points = max_points;
	g_pen_stroke_points = point_array;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_change_handwriting_area
* DESCRIPTION
*	Change hand-written area.
*	It is typically used when changing input language.
*
*	It will
*	1. reset handwriting and g_pen_stroke_min_offset_square
*	2. mmi_pen_reset() to flush queue.
*
* PARAMETERS
*	num_regions		IN	Number of handwriting regions
*	region_array	IN	Handwriting regions 
*	ext_region		IN	Handwriting region after the first stroke is generated. (NULL to ignore it)
*
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_change_handwriting_area(
		kal_uint32 num_regions, 
		const mmi_pen_handwriting_area_struct *region_array, 
		const mmi_pen_handwriting_area_struct *ext_region)
{
	if (g_pen_stroke_max_points == 0)
	{
		// MMI_DBG_ASSERT(0);
		return;
	}

	if ( num_regions > MMI_PEN_MAX_HANDWRITING_REGION)
		num_regions = MMI_PEN_MAX_HANDWRITING_REGION ;

	g_pen_num_stroke_area = num_regions;
	memcpy(g_pen_stroke_areas, region_array, sizeof(mmi_pen_handwriting_area_struct) * num_regions);
	
	if (ext_region)
	{
		g_pen_ext_stroke_area = *ext_region;
	}
	else
	{
		g_pen_ext_stroke_area.x1 = -1;
		g_pen_ext_stroke_area.x2 = -1;
		g_pen_ext_stroke_area.y1 = -1;
		g_pen_ext_stroke_area.y2 = -1;
	}

 	/* 
	 * We will call mmi_pen_reset() *later* on Pen Up or Stroke Up event.
	 *
	 * There might be many pending events in driver queue if MMI task is blocked by 
	 * other time-consuming jobs. If there are pending stroke events in driver queue, 
	 * it is better to flush driver queue.
	 *
	 */
	if (!g_pen_cntx.is_pen_down)
	{
		/* Clear strokes */
		mmi_pen_end_strokes_of_character();
		mmi_pen_reset();
		mmi_pen_begin_strokes_of_character();
	}
	else
	{
		/* 
		 * NOTE: we *cannot* call mmi_pen_reset().
		 *
		 * For example, assume mmi_pen_change_handwriting_area() is invoked on Button Down.
		 * If we invoke mmi_pen_reset() here, it will send MMI_PEN_EVENT_ABORT and reset the button.
		 */
		g_pen_cntx.reset_stroke_on_pen_up = 1;
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_stop_capture_strokes
* DESCRIPTION
*	Stop capturing stroke 
*
*	Note: it should be used before mmi_pen_reset() because it does not flush driver pen queue
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_stop_capture_strokes(void)
{
	g_pen_num_stroke_area = 0;
	g_pen_stroke_max_points = 0;
	g_pen_stroke_points = NULL;


	/* 
	 * We will call mmi_pen_reset() *later* on Pen Up or Stroke Up event.
	 *
	 * There might be many pending events in driver queue if MMI task is blocked by 
	 * other time-consuming jobs. If there are pending stroke events in driver queue, 
	 * it is better to flush driver queue.
	 *
	 */
	if (!g_pen_cntx.is_pen_down)
	{
		mmi_pen_reset();
	}
	else
	{
		/* 
		 * NOTE: we *cannot* call mmi_pen_reset().
		 *
		 * For example, assume mmi_pen_stop_capture_strokes() is invoked when scrollbar of inline editor 
		 * is scrolled on Pen Move event and the highlighed single-line editor is un-highlighted. 
		 * If we invoke mmi_pen_reset() here, it will send MMI_PEN_EVENT_ABORT and reset the scrolling of scrollbar.
		 */
		g_pen_cntx.reset_stroke_on_pen_up = 1;
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_stroke_down_handler
* DESCRIPTION
*	Register the Stroke Down handler
*
* PARAMETERS
*	pen_fp		IN		callback handler
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_stroke_down_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_STROKE_DOWN] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_stroke_move_handler
* DESCRIPTION
*	Register the Stroke Move handler
*
* PARAMETERS
*	pen_fp		IN		callback handler
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
PUBLIC VOID mmi_pen_register_stroke_move_handler( mmi_pen_hdlr pen_fp)
{ 
	mmi_trace(g_sw_TPL, "TP In mmi_pen_register_stroke_move_handler. ptr:0x%08x",
            pen_fp);
	g_pen_event_table[MMI_PEN_STROKE_MOVE] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_stroke_up_handler
* DESCRIPTION
*	Register the Stroke Up handler
*
* PARAMETERS
*	pen_fp		IN		callback handler
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_stroke_up_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_STROKE_UP] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_register_stroke_up_handler
* DESCRIPTION
*	Register the Stroke Up handler
*
* PARAMETERS
*	pen_fp		IN		callback handler
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_register_stroke_longtap_handler(mmi_pen_hdlr pen_fp)
{
	g_pen_event_table[MMI_PEN_STROKE_LONGTAP] = pen_fp;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_peek_stroke_state
* DESCRIPTION
*	
*
* PARAMETERS
*	has_unfinished_stroke			OUT	Whether there are more strokes to come
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_peek_stroke_state(BOOL *has_unfinished_stroke)
{
	if (g_pen_cntx.is_pen_down && g_pen_cntx.is_stroke_created)
	{
		*has_unfinished_stroke = MMI_TRUE;
	}
	else
	{
		*has_unfinished_stroke = MMI_FALSE;
	}
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_begin_strokes_of_character
* DESCRIPTION
*	Start to write a new character
*
*	It is typically coupled with mmi_pen_end_strokes_of_character(), which stops en-queueing 
*	strokes inside 'point_array'. 
*	In mmi_pen_begin_strokes_of_character(), MMI continue to en-queue strokes from the head of 'point_array'.
*
*	(However, simulator does not implement all of these)
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_begin_strokes_of_character(void)
{
	MMI_DBG_ASSERT(g_pen_num_stroke_area > 0);

	g_pen_cntx.num_points_queued = 0;
	g_pen_cntx.is_stroke_created = 0;
	g_pen_cntx.is_all_stroke_finished = 0;
}


/*****************************************************************************
* FUNCTION
*	mmi_pen_end_strokes_of_character
* DESCRIPTION
*	End a written character.
*
*	The following procedures are executed
*	o Reset handwriting area to original size
*	o Stop en-queueing new strokes inside 'point_array'. (But incoming strokes are still en-queued in driver ring buffer)
 
* 	This API should be used only if there is no unfinished stroke (checked by mmi_pen_peek_stroke_state()). 
*	After it returns, we may pass 'point_array' to handwriting recognition engine.
*
*	(However, simulator does not implement all of these)
*
* PARAMETERS
*	void
* RETURNS
*  void
* GLOBALS AFFECTED
*  
*****************************************************************************/
void mmi_pen_end_strokes_of_character(void)
{
	MMI_ASSERT(!g_pen_cntx.is_pen_down);
	mmi_pen_simulator_push_char_end();
	g_pen_cntx.is_all_stroke_finished = 1; /* To block further Pen Down */

#ifdef MMI_PEN_SAVE_STROKE_IN_FILE
	{
		extern void mmi_pen_save_stroke_in_file(mmi_pen_point_struct *, S32);
		mmi_pen_save_stroke_in_file(g_pen_stroke_points, g_pen_cntx.num_points_queued);
	}
#endif /* MMI_PEN_SAVE_STROKE_IN_FILE */
}

/* -------------------------------------------------------------------- */
/** @brief mmi_pen_set_shortcut_entry
 * set the short cut entry function
 * @param index fun
 * @index: which short cut should be set
 * @fun: the new entry function pointer
*/
/* -------------------------------------------------------------------- */
PUBLIC VOID mmi_pen_set_shortcut_entry(U8 index, tpShortCutAppEntry fun)
{

}

/****************************************************************
 * 
 * MNT support 
 *
 * Because the pen handlers might take a long time to execute, race condition is easy to happen.
 *
 ***************************************************************/
#if !defined(MMI_ON_WIN32)

#define MMI_PEN_MNT_QUEUE_SIZE 1024

typedef struct {
	S16 type; /* 0 down, 1 move, 2 up */
	S16 x;
	S16 y;
} mmi_pen_MNT_queue_item_struct;

static volatile int g_mmi_pen_MNT_qhead; /* modified by MMI thread */
static volatile int g_mmi_pen_MNT_qtail; /* modified by windows message loop thread */
static mmi_pen_MNT_queue_item_struct g_mmi_pen_MNT_queue[MMI_PEN_MNT_QUEUE_SIZE];

static BOOL g_mmi_pen_MNT_timer_flag;

/* By Windows main thread */
static void mmi_pen_MNT_enqueue_point(S16 type, S16 x, S16 y)
{
	int tail = g_mmi_pen_MNT_qtail;
	g_mmi_pen_MNT_queue[tail].type = type;
	g_mmi_pen_MNT_queue[tail].x = x;
	g_mmi_pen_MNT_queue[tail].y = y;
	if (++tail == MMI_PEN_MNT_QUEUE_SIZE)
	{
		tail = 0;
	}
	g_mmi_pen_MNT_qtail = tail;
	MMI_ASSERT(tail != g_mmi_pen_MNT_qhead);
}


void mmi_pen_MNT_enqueue_down(S16 x, S16 y)
{
	mmi_pen_MNT_enqueue_point(0, x, y);
}


void mmi_pen_MNT_enqueue_move(S16 x, S16 y)
{
	mmi_pen_MNT_enqueue_point(1, x, y);
}


void mmi_pen_MNT_enqueue_up(S16 x, S16 y)
{
	mmi_pen_MNT_enqueue_point(2, x, y);
}


/* By MMI thread */
#ifdef __MMI_HANDWRITING_PAD__
void mmi_pen_draw_control_area()
{
   int i;

   coolsand_UI_lock_double_buffer();
   for (i=0; i<TP_AREA_MAX_NUM; i++)
   {
      if (IS_ENABLE_FLAG(tp_area_table[i].flag, TP_CONTROL_AREA))
      {
		 int index = 0;
         mmi_pen_point_struct pos1, pos2;

		 pos1 = tp_area_table[i].pos[index];
		 index++;
         while ( (tp_area_table[i].pos[index].x != -1) && (tp_area_table[i].pos[index].y != -1) )
         {
			pos2 = tp_area_table[i].pos[index];
			gdi_draw_line(pos1.x, pos1.y, pos2.x, pos2.y, GDI_COLOR_RED);
			pos1 = pos2;
            index++;
         }
		 pos2 = tp_area_table[i].pos[0];
		 gdi_draw_line(pos1.x, pos1.y, pos2.x, pos2.y, GDI_COLOR_RED);

      }
      else
      {
         /* handwriting area */
      }
   }
   coolsand_UI_unlock_double_buffer();
   coolsand_UI_BLT_double_buffer(0, 0, UI_device_width-1, UI_device_height-1);

}
#endif /* __MMI_HANDWRITING_PAD__ */


static void mmi_pen_MNT_timer_hdlr(void)
{
	mmi_pen_MNT_queue_item_struct *pt, *pt_end;
	int new_head, saved_tail;
	MMI_BOOL has_events = MMI_FALSE;

	if (!g_pen_cntx.is_enabled)
	{
		g_mmi_pen_MNT_qhead = g_mmi_pen_MNT_qtail = 0;

		StartTimer(PEN_POLLING_TIMER, 20, mmi_pen_MNT_timer_hdlr);
		return;
	}

	g_mmi_pen_MNT_timer_flag = MMI_TRUE;

	saved_tail = g_mmi_pen_MNT_qtail;
	pt = g_mmi_pen_MNT_queue + g_mmi_pen_MNT_qhead;
	
	if (saved_tail < g_mmi_pen_MNT_qhead)
	{
		pt_end = g_mmi_pen_MNT_queue + MMI_PEN_MNT_QUEUE_SIZE;
		new_head = 0;
	}
	else
	{
		pt_end = g_mmi_pen_MNT_queue + saved_tail;
		new_head = saved_tail;
	}

	if (pt < pt_end)
	{
		has_events = MMI_TRUE;
	}

	while (pt < pt_end)
	{
#ifdef __MMI_SCREEN_ROTATE__
		if (mmi_frm_get_screen_rotate() != MMI_FRM_SCREEN_ROTATE_0)
		{
			S32 tmp_x = pt->x, tmp_y = pt->y;
			 
			gdi_rotate_map_absolute_hw_to_lcd(&tmp_x, &tmp_y);
			//gdi_layer_map_rotated_coordinates(gdi_layer_get_base_layer_rotation(), &tmp_x, &tmp_y);
			pt->x = (S16) tmp_x;
			pt->y = (S16) tmp_y;
		}
#endif /* __MMI_SCREEN_ROTATE__ */

		switch (pt->type)
		{
			case 0:
				mmi_pen_simulator_button_down_hdlr(pt->x, pt->y);
				break;
			case 1:
				mmi_pen_simulator_button_move_hdlr(pt->x, pt->y);
				break;
			case 2:
				mmi_pen_simulator_button_up_hdlr(pt->x, pt->y);
				break;
			default:
				ASSERT(0);
		}
		pt++;
		if (!g_mmi_pen_MNT_timer_flag)
			break;  /* mmi_pen_reset was invoked */
	}

	/* Update queue when no mmi_pen_disable() and mmi_pen_reset() inside previous pen handler */
	if (g_mmi_pen_MNT_timer_flag)
	{
		g_mmi_pen_MNT_qhead = new_head;
	}

	g_mmi_pen_MNT_timer_flag = MMI_FALSE;

	if (has_events)
	{
		TurnOnBacklight(MMI_TRUE);
#ifdef __MMI_LCD_PARTIAL_ON__
		/* Switch screen and flush pen events */
		LeavePartialOnScreenSaverIfOk();
#endif /* __MMI_LCD_PARTIAL_ON__ */
	}

#ifdef __MMI_HANDWRITING_PAD__
	mmi_pen_draw_control_area();
#endif 

	StartTimer(PEN_POLLING_TIMER, 20, mmi_pen_MNT_timer_hdlr);
}


static void mmi_pen_MNT_flush_queue(void)
{
	g_mmi_pen_MNT_qtail = 0;
	g_mmi_pen_MNT_qhead = 0;
	g_mmi_pen_MNT_timer_flag = MMI_FALSE;
}


static void mmi_pen_MNT_start_timer(void)
{
	StartTimer(PEN_POLLING_TIMER, 20, mmi_pen_MNT_timer_hdlr);
}
#endif /* !defined(MMI_ON_WIN32) */


/****************************************************************
 * Unit test
 ***************************************************************/

#ifdef MMI_PEN_UNIT_TEST

/* The simple drawing application is not efficent in order 
 * to follow PC Simualator architecture 
 */

#include "wgui_categories.h"
#include "globaldefs.h"
#include "historygprot.h"

static mmi_pen_point_struct  mmi_pen_test_last_pos;

static U8 mmi_pen_draw_buffer[LCD_WIDTH * LCD_HEIGHT / 8];

static void mmi_pen_test_clear_screen(void)
{
	memset(mmi_pen_draw_buffer, 0, sizeof(mmi_pen_draw_buffer));
}


static void mmi_pen_test_draw_point(S32 x, S32 y)
{
	S32 offset;

	if (x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT)
	{
		return;
	}

	offset = y * LCD_WIDTH + x;
	mmi_pen_draw_buffer[offset >> 3] |= (1 << (offset & 0x07) );
}


static void mmi_pen_test_draw_from_buffer(S32 x1, S32 y1, S32 x2, S32 y2)
{
	S32 x, y;
	S32 offset;
	color c;
	c.r = 255; c.g = c.b = 0; c.alpha = 255;
	coolsand_UI_push_clip();
	coolsand_UI_set_clip(x1, y1, x2, y2);
	
	for (y = y1; y <= y2; y++)
	{
		offset = y * LCD_WIDTH;
		for (x = x1; x <= x2; x++)
		{
			if (mmi_pen_draw_buffer[offset >> 3] & (1 << (offset & 0x07)))
			{
				coolsand_UI_putpixel(x, y, c);
			}
			offset ++;	
		}
	}
	coolsand_UI_pop_clip();
}


static void mmi_pen_test_refresh_screen(void)
{
	coolsand_UI_lock_double_buffer();
	mmi_pen_test_draw_from_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


static void mmi_pen_test_draw_line(S16 x1, S16 y1, S16 x2, S16 y2)
{
	S16 x_diff = abs(x2-x1);
	S16 y_diff = abs(y2-y1);
	S16 x_incr, y_incr;
	if (x1 > x2) 
		 x_incr = -1;
	else
		x_incr = 1;
	if (y1 > y2)
		y_incr = -1;
	else
		y_incr = 1;

	if (x_diff >= y_diff)
	{           
		S16 pr_diff 	= y_diff<<1;
		S16 pru_diff 	= pr_diff - (x_diff<<1);
		S16 p 		= pr_diff - x_diff;

		for (; x_diff>=0; x_diff--)
		{
			mmi_pen_test_draw_point(x1,y1);
			if (p > 0)
			{ 
				x1+=x_incr;
				y1+=y_incr;
				p+=pru_diff;
			}
			else                
			{
				x1+=x_incr;
				p+=pr_diff;      
			}
		}		
	}
	else
	{
		S16 pr_diff 	= x_diff<<1; 
		S16 pru_diff 	= pr_diff - (y_diff<<1);
		S16 p 		= pr_diff - y_diff; 

		for (; y_diff>=0; y_diff--)
		{
			mmi_pen_test_draw_point(x1,y1);
			if (p > 0)             
			{ 
				x1+=x_incr; 
				y1+=y_incr; 
				p+=pru_diff;    
			}
			else                    
			{
				y1+=y_incr;
				p+=pr_diff;      
			}
		}		
	}		
}

static void mmi_pen_test_draw_rectangle(S16 x1, S16 y1, S16 x2, S16 y2)
{
	mmi_pen_test_draw_line(x1, y1, x1, y2);
	mmi_pen_test_draw_line(x1, y2, x2, y2);
	mmi_pen_test_draw_line(x2, y2, x2, y1);
	mmi_pen_test_draw_line(x2, y1, x1, y1);
}


static void mmi_pen_test_draw_filled_rectangle(S16 x1, S16 y1, S16 x2, S16 y2)
{
	S16 x , y;
	for (x = x1; x <= x2; x++)
	{
		for (y = y1; y <= y2; y++)
		{
			mmi_pen_test_draw_point(x, y);
		}
	}
}


static void mmi_pen_test_down(mmi_pen_point_struct pos)
{
	mmi_pen_test_last_pos = pos;
}


static void mmi_pen_test_move(mmi_pen_point_struct pos)
{
	mmi_pen_test_draw_line(mmi_pen_test_last_pos.x, mmi_pen_test_last_pos.y, pos.x, pos.y);
	mmi_pen_test_last_pos = pos;

	mmi_pen_test_refresh_screen();
}


static void mmi_pen_test_up(mmi_pen_point_struct pos)
{
	
}


static void mmi_pen_test_long_tap(mmi_pen_point_struct pos)
{
	mmi_pen_test_draw_filled_rectangle((S16)(pos.x - 2), (S16)(pos.y - 2), (S16)(pos.x + 2), (S16)(pos.y + 2));
	mmi_pen_test_refresh_screen();
}


static void mmi_pen_test_repeat(mmi_pen_point_struct pos)
{
	mmi_pen_test_draw_rectangle((S16)(pos.x - 4), (S16)(pos.y - 4), (S16)(pos.x + 4), (S16)(pos.y + 4));	
	mmi_pen_test_refresh_screen();
}


static void mmi_exit_pen_test_screen(void)
{
	mmi_pen_reset();
}


static void mmi_redraw_pen_test_screen(void)
{
	coolsand_UI_lock_double_buffer();
	RedrawCategoryFunction();
	mmi_pen_test_draw_from_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
	coolsand_UI_unlock_double_buffer();
	coolsand_UI_BLT_double_buffer(0, 0, UI_device_width - 1, UI_device_height - 1);
}


static void mmi_entry_pen_test_screen(void)
{
	EntryNewScreen(0, NULL, mmi_exit_pen_test_screen, NULL);

	ShowCategory66Screen(STR_GLOBAL_UNFINISHED, 0, 0, 0, STR_GLOBAL_BACK, 0, (U8*) L"", 0, NULL);
	RedrawCategoryFunction = mmi_redraw_pen_test_screen; /* For PC simulator architecture */
	
	mmi_pen_test_clear_screen();

	mmi_pen_register_down_handler(mmi_pen_test_down);
	mmi_pen_register_move_handler(mmi_pen_test_move);
	mmi_pen_register_up_handler(mmi_pen_test_up);
	mmi_pen_register_long_tap_handler(mmi_pen_test_long_tap);
	mmi_pen_register_repeat_handler(mmi_pen_test_repeat);
	
	SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
}


void mmi_pen_unit_test(void)
{
	mmi_entry_pen_test_screen();
}


#endif /* MMI_PEN_UNIT_TEST */

PUBLIC BOOL mmi_pen_check_inside_rect( mmi_pen_polygon_area_struct* polygon, mmi_pen_point_struct pos )
{ 
    return FALSE;
}


PUBLIC BOOL mmi_pen_check_inside_polygon( mmi_pen_polygon_area_struct* polygon, mmi_pen_point_struct pos )
{ 
    return FALSE;
}

PUBLIC BOOL mmi_pen_is_down( )
{ 
        return FALSE;
}

#elif ( !defined(__MMI_TOUCH_SCREEN__) || !defined(__MMI_HANDWRITING_PAD__) ) && !defined(__COOLSAND_TARGET__) && !defined(MMI_ON_WIN32)

#include "mmi_data_types.h"

void mmi_pen_MNT_enqueue_down(S16 x, S16 y)
{

}


void mmi_pen_MNT_enqueue_move(S16 x, S16 y)
{

}


void mmi_pen_MNT_enqueue_up(S16 x, S16 y)
{

}
PUBLIC VOID mmi_pen_register_down_handler(VOID* pen_fp)
{
}
 
PUBLIC VOID mmi_pen_register_up_handler(VOID* pen_fp)
{
}
 
PUBLIC VOID mmi_pen_register_move_handler(VOID* pen_fp)
{
}

#endif
