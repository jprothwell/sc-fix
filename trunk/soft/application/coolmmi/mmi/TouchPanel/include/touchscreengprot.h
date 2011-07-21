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
///     @file touchscreengprot.h                                              //
///     Touch panel API for MMI.                                              //
///                                                                           //
//////////////////////////////////////////////////////////////////////////////// 
#ifndef _TOUCH_SCREEN_GPROT_H
#define _TOUCH_SCREEN_GPROT_H

#ifdef MMI_ON_WIN32 /* zhuoxz,2009-4-24 */
#include "kal_non_specific_general_types.h"
#endif /* #ifdef MMI_ON_WIN32 */

#ifndef _KAL_NON_SPECIFIC_GENERAL_TYPES_H
#error "kal_non_specific_general_types.h should be included "
#endif /* _KAL_NON_SPECIFIC_GENERAL_TYPES_H */

#ifndef _PIXCOM_DATA_TYPES_H
#include "mmi_data_types.h"
#endif /* _PIXCOM_DATA_TYPES_H */

//!Shorcut entry typedef.
typedef void ( *tpShortCutAppEntry)( void);

/* Utility macro functions */
#define PEN_CHECK_BOUND(x, y, x1, y1, width, height) ((x) >= (x1) && (y) >= (y1) && (x) < ((x1) + (width)) && (y) < ((y1) + (height)))
#define PEN_ABS(a, b) (((a) > (b)) ? (a-b) : (b-a))
#define PEN_MIN(a, b) (((a) > (b)) ? (b) : (a))
#define PEN_MAX(a, b) (((a) > (b)) ? (a) : (b))

//@TODO Use the same enum type with TouchScreen.c!
#define MMI_PEN_LONGTAP_TIME				(60) /* multiple of 10ms */

/* Delay time for Pen Repeat */
#define MMI_PEN_REPEAT_TIME				(30) /* multiple of 10ms */

/* Delay time for Stroke LongTap (MMI_PEN_SUPPORT_STROKE_LONGTAP) */
#define MMI_PEN_STROKE_LONGTAP_TIME		(80) /* multiple of 10ms */

/* Sampling period of Event-Based pen events (~ 12 samples/sec) */
#define MMI_PEN_SAMPLING_PERIOD_1		(8) /* multiple of 10ms */

/* Sampling period of Stroke-Based pen events (50 samples/sec) */
#define MMI_PEN_SAMPLING_PERIOD_2		(2) /* multiple of 10ms */

/* If pen does not move over this distance, donot generate Pen Move event at all. */
#define MMI_PEN_SKIP_MOVE_OFFSET						5 

/* If pen does not move over this distance, donot generate Stroke Move event at all.*/
#define MMI_PEN_SKIP_STROKE_MOVE_OFFSET			1 

/* Maximum move offset for generating LongTap event */
#define MMI_PEN_SKIP_LONGTAP_OFFSET					10

/* Maximum move offset for generating Stroke-LongTap event (MMI_PEN_SUPPORT_STROKE_LONGTAP) */
#define MMI_PEN_SKIP_STROKE_LONGTAP_OFFSET		14
/* Minimum offset to Pen Down position in order to switch from event-based mode to stroke-based mode */
#if defined(MMI_PEN_SUPPORT_STROKE_LONGTAP) || defined(__MMI_HANDWRITING_PAD__)
#define MMI_PEN_STROKE_MIN_OFFSET					0
#else
#define MMI_PEN_STROKE_MIN_OFFSET					9 
#endif 
typedef enum {
	MMI_PEN_EVENT_DOWN,
	MMI_PEN_EVENT_UP,
	MMI_PEN_EVENT_LONG_TAP,
	MMI_PEN_EVENT_REPEAT,
	MMI_PEN_EVENT_MOVE,
	MMI_PEN_EVENT_ABORT,
	MMI_PEN_STROKE_DOWN,
	MMI_PEN_STROKE_UP,
	MMI_PEN_STROKE_MOVE,
	MMI_PEN_STROKE_LONGTAP,
	MMI_PEN_EVENT_TYPE_MAX
} mmi_pen_event_type_enum;

typedef struct
{
	S16 x;
	S16 y;
} mmi_pen_point_struct;

//!mmi_pen_handwriting_area_struct
//!Rectangle coordinates for handwriting area definition.
typedef struct
{
	S16 x1, y1, x2, y2;
} mmi_pen_handwriting_area_struct;

//!mmi_pen_polygon_area_struct
//!Polygon description for mmi_pen_check_inside_polygon function.
typedef struct
{
	mmi_pen_point_struct* points;
    //!How many points exist in this polygon.
	S16 num;
} mmi_pen_polygon_area_struct;

//!Pen event callback function.
typedef void (*mmi_pen_hdlr)(mmi_pen_point_struct pos);

/* --------------------------------------------------------------------*/
/** mmi_pen_dummy_hdlr
 * @brief Dummy handler.
 * 
 * @param pos
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern void mmi_pen_dummy_hdlr(mmi_pen_point_struct);

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
extern void mmi_pen_init(void);

/* --------------------------------------------------------------------*/
/** mmi_pen_block
 * @brief Disable touch panel IRQ.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern void mmi_pen_block(void);

/* --------------------------------------------------------------------*/
/** mmi_pen_unblock 
 * @brief Enable touch panel IRQ.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern void mmi_pen_unblock(void);


/* --------------------------------------------------------------------*/
/** mmi_pen_start_calibration
 * @brief Start calibration process. 
 * Here is the calibration procedure:
 * 1, Call mmi_pen_start_calibration, and draw first point on the screen.
 * 2, After 1st point is touched, call mmi_pen_set_calibration_data( x, y, 0)
 * and then draw the 2nd point on the screen.
 * 3, Do steps 2 until all the 4 points is touched.
 * 4, Call mmi_pen_end_calibration to store the calibration data in to flash.
 *
 *
 * @return 
 */
/* --------------------------------------------------------------------*/
extern void mmi_pen_start_calibration( );

/* --------------------------------------------------------------------*/
/** mmi_pen_set_calibration_data
 * @brief After user touch the calibration point on the screen, call this
 * function to pass the XY value back. This function will store and check
 * the given point coordinates.
 * 
 * @param x 
 * @param y
 * @param index Which point is touched.
 * 
 * @return If the given point coordinates is invalid, return FALSE.
 */
/* --------------------------------------------------------------------*/
extern BOOL mmi_pen_set_calibration_data( UINT16 x, UINT16 y, UINT16 index);

/* --------------------------------------------------------------------*/
/** mmi_pen_set_calibration_data
 * @brief If the calibration procedure is ended normally, this function 
 * will store it to flash. Otherwise, just return.
 * IMPORTANT: If there is no defualt calibration data stored in the flash,
 * the last calibration data will be stored as the defualt.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern void mmi_pen_end_calibration( );

/* Event-based API */
extern void mmi_pen_register_down_handler(mmi_pen_hdlr pen_fp);
extern void mmi_pen_register_long_tap_handler(mmi_pen_hdlr pen_fp);
extern void mmi_pen_register_repeat_handler(mmi_pen_hdlr pen_fp);
extern void mmi_pen_register_move_handler(mmi_pen_hdlr pen_fp);
extern void mmi_pen_register_up_handler(mmi_pen_hdlr pen_fp);
extern void mmi_pen_register_abort_handler(mmi_pen_hdlr pen_fp);
extern void mmi_pen_register_stroke_longtap_handler(mmi_pen_hdlr pen_fp);


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
extern void mmi_pen_change_handwriting_area( 
        UINT32 num_regions,
        const mmi_pen_handwriting_area_struct *region_array,
        const mmi_pen_handwriting_area_struct *ext_region);

//extern void mmi_pen_editor_do_confirm(void);

/* --------------------------------------------------------------------*/
/** mmi_pen_read_calibration
 * @brief Read calibration data from flash.
 * 
 * @return If no data stored in the flash, return FALSE.
 */
/* --------------------------------------------------------------------*/
extern BOOL mmi_pen_read_calibration( );

/* --------------------------------------------------------------------*/
/** mmi_pen_is_down
 * @brief Check if the pen is down.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern BOOL mmi_pen_is_down( );
/* --------------------------------------------------------------------*/
/** mmi_pen_is_in_calibration_process
 * @brief Check if it is in touch panel calibation process.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern PUBLIC BOOL mmi_pen_is_in_calibration_process( );

/* --------------------------------------------------------------------*/
/** mmi_pen_disable_quick_launch
 * @brief Disable quick launch application by the touch panel.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern PUBLIC VOID mmi_pen_disable_quick_launch( );
/* --------------------------------------------------------------------*/
/** mmi_pen_disable_quick_launch
 * @brief Enable quick launch application by the touch panel.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern PUBLIC VOID mmi_pen_enable_quick_launch( );
/* --------------------------------------------------------------------*/
/** mmi_pen_set_shortcut_entry
 * @brief set short cut entry function.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern PUBLIC VOID mmi_pen_set_shortcut_entry(UINT8 index, tpShortCutAppEntry fun);
/* --------------------------------------------------------------------*/
/** mmi_pen_stop_capture_strokes
 * @brief Stop capture strokes. Even if this function is invoked before the last
 * stroke up event, it will be still sent to mmi task.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern VOID mmi_pen_stop_capture_strokes(void); 
/* --------------------------------------------------------------------*/
/** mmi_pen_end_strokes_of_character
 * @brief Call this function when the LAST stroke of a character is done.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern VOID mmi_pen_end_strokes_of_character(void); 
/* --------------------------------------------------------------------*/
/** mmi_pen_peek_stroke_state
 * @brief Check if a stroke is finished.
 * 
 * @param has_unfinished_stroke
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern VOID mmi_pen_peek_stroke_state(BOOL *has_unfinished_stroke);
/* --------------------------------------------------------------------*/
/** mmi_pen_begin_strokes_of_character
 * @brief Call this function to start record strokes of a character. Call 
 * it every time before enter a new character.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern VOID mmi_pen_begin_strokes_of_character(void);
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
extern VOID mmi_pen_start_capture_strokes( 
        UINT32 max_points,
        mmi_pen_point_struct *point_array,
        UINT32 num_regions,
        const mmi_pen_handwriting_area_struct *region_array,
        const mmi_pen_handwriting_area_struct *ext_region);
/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_stroke_down_handler
 * Register stroke down event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
extern VOID mmi_pen_register_stroke_down_handler(mmi_pen_hdlr pen_fp);
/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_stroke_move_handler
 * Register stroke move event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
extern VOID mmi_pen_register_stroke_move_handler( mmi_pen_hdlr pen_fp);
/* -------------------------------------------------------------------- */
/** @brief mmi_pen_register_stroke_up_handler
 * Register stroke up event handler. Called by upper layer.
 * @param pen_fp 
 * 
 * @return 
 */
/* -------------------------------------------------------------------- */
extern VOID mmi_pen_register_stroke_up_handler(mmi_pen_hdlr pen_fp);


/* --------------------------------------------------------------------*/
/** mmi_pen_check_inside_polygon
 * @brief Check if a point is in given polygon.
 * 
 * @param polygon See mmi_pen_polygon_area_struct definition.
 * @param pos 
 * 
 * @return If the point is in given polygon, return TRUE.
 */
/* --------------------------------------------------------------------*/
extern BOOL mmi_pen_check_inside_polygon( mmi_pen_polygon_area_struct* polygon, mmi_pen_point_struct pos );
extern BOOL mmi_pen_check_inside_rect( mmi_pen_polygon_area_struct* polygon, mmi_pen_point_struct pos );
/* --------------------------------------------------------------------*/
/** mmi_pen_pause_capture_strokes 
 * @brief This function will pause stroke capture until resume function called.
 * If the stroke capture is paused, all of the stroke event won't be sent.
 * And also the stroke track will be stopped.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern VOID mmi_pen_pause_capture_strokes();
/* --------------------------------------------------------------------*/
/** mmi_pen_resume_capture_strokes
 * @brief Resume stroke capture from last pause action.
 * 
 * @return 
 */
/* --------------------------------------------------------------------*/
extern VOID mmi_pen_resume_capture_strokes();

extern VOID mmi_pen_reset(void); 

/* --------------------------------------------------------------------*/
/** mmi_pen_restore_default_calibration
 * @brief If there is a defualt calibration, restore it as the current
 * calibration data.
 * 
 * @return TRUE if success.
 */
/* --------------------------------------------------------------------*/
PUBLIC BOOL mmi_pen_restore_default_calibration( );


//Below definitions will be deleted.
//Actually, __MMI_HANDWRITING_PAD__ macro won't used anymore.
#ifdef __MMI_HANDWRITING_PAD__
extern S32 mmi_pen_check_inside_control_area();
#define IS_ENABLE_FLAG(data, f)        ( (data & (f)) == (f) )
#define TP_CONTROL_AREA       (0x80000000)
#define TP_HANDWRITING_AREA   (0x40000000)
#endif

#endif /* _TOUCH_SCREEN_GPROT_H */


