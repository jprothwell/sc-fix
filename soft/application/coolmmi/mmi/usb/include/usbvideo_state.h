/*****************************************************************************
 *
 * Filename:
 * ---------
 *   usbvideo_state.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file intends for usb video class state machine definitions
 *
 * Author:
 * -------
 * -------
 *
*============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
 
#ifndef USBVIDEO_STATE_H
#define USBVIDEO_STATE_H

#if 0//JiaShuo Delete for webcam
#include "usbvideo_custom.h"


#define USBVIDEO_PRE_PAUSE_TIMEOUT	100  /* 10ms unit*/
#define USBVIDEO_INCALL_JPEG_TIMEOUT	100  /* 10ms unit*/
#endif

/***********************************************
	function and global variable
************************************************/
extern void USBVideo_Plug_Out_Hdlr(void);
extern void USBVideo_Parse_Med_Msg(ilm_struct *p_recv_ilm);

#endif /* USBVIDEO_STATE_H */

