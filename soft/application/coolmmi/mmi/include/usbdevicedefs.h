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

/*****************************************************************************
 *
 * Filename:
 * ---------
 * USBDeviceDefs.h
 *
 * Project:
 * --------
 *   MMI
 *
 * Description:
 * ------------
 *   
 *
 * Author:
 * -------
 
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 * 
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/


#ifndef MMI_USBDEVICE_DEFS_H
#define MMI_USBDEVICE_DEFS_H

#ifdef __MMI_USB_SUPPORT__


/***************************************************************************** 
* Definations
*****************************************************************************/

/* Number of menu items in usb config page */
#define MAX_USB_MENUITEM   2  /* 3 */

/* The delay time period when usb config screen is allowed to display */
#define PENDING_USBDETECTION_HDLR_TIMER_DUR  3000 /* 3 sec */

/* The display time of shutdown for mass storage notification screen */
#define USB_SHUTDOWN_SCR_TIMER_DUR  2000 /* 2 sec */





/***************************************************************************** 
* Enumation
*****************************************************************************/

/*****************************/
/* MMI Usb config menu items */
/*****************************/
typedef enum{

MMI_USB_CONFIG_MENU_MS = 0,
#ifdef __MMI_WEBCAM__
	MMI_USB_CONFIG_MENU_WEBCAM,
#endif
MMI_USB_CONFIG_MENU_CDCACM,
MMI_USB_CONFIG_MENU_RESERVED,

MMI_USB_CONFIG_MENU_END
} MMI_USB_CONFIG_MENU;

typedef enum{
	MMI_USB_WEBCAM_DISCONNECT,
	MMI_USB_WEBCAM_CONNECTED,
	MMI_USB_WEBCAM_LISTENING,
	MMI_USB_WEBCAM_RUNNING,		
	MMI_USB_WEBCAM_PAUSE
	
} MMI_USB_WEBCAM_STATE;




/***************************************************************************** 
* Structure
*****************************************************************************/

/********************/
/* MMI Usb context **/
/********************/
typedef struct {

   /* MMI usb state (previous, only use when interrupt at config page) */
   MMI_USB_STATUS usb_status_prev;

   /* MMI usb state (current)*/
   MMI_USB_STATUS usb_status;

   /* Previous PS and TST  port setting, for restore when plug out usb device */
   U8 ps_port_prev;
   U8 tst_port_prev;

   /*   Usb config screen might interrupt every screen, if user press END key at usb    */
   /* config screen, shall distory config screen itself and execute the END key handler */
   /* register by original screen                                                       */
   FuncPtr EndKeyFuncPtr;

   /*
      Bit 0: Before Power on Flag
      Bit 1: Pending Timer Flag
      Bit 2: Usb plug out during switch port
   */
   U32 flag;  

   #ifdef __MMI_WEBCAM__
		U16 		webcam_state;
   	BOOL		webcam_is_sub_display;
		BOOL		webcam_is_clam_close;
		FuncPtr	webcam_prev_end_key_hdlr;
   #endif /* __MMI_WEBCAM__ */

} MMI_USB_CONTEXT;




/***************************************************************************** 
* Extern Global Function
*****************************************************************************/
BOOL PermitUsbCfgScreenDisplay( void );
void SendUsbConfigToHW(U8 mode);
void UsbConfigRspHdlr(void *info );
void UsbBatStatusIndHdlr(void *info);
void UsbGpioDetectIndHdlr(void * info);
void EntryUsbDetectQueryScr(void);
void ShowUsbScr(void);
void ShowUsbChgCmpltScr(void);
void ShowSubLCDUsbScr(void);
void ExitSubLCDUsbScr(void);
void ShowSubLCDUsbChgrCmpltScr(void);
void ExitSubLCDUsbChgrCmpltScr(void);
void UsbPwnOnThenPwnOn(void);
void InitUsbModeEventHandler(void);
void UsbDetectScrHlightHdlr(S32 index);
void UsbDetectScrLskHdlr(void);
void UsbDetectScrRskHdlr(void);
void UsbDetectScrENDKeyHdlr(void);
void ExitUsbDetectQueryScr(void);
void UsbUartSwitchRspHdlr(void *info);
void SendUsbUartSwitchToHW(U8 app, U8 new_port);
void SendUsbGetUartPortToHW(void);
void UsbSendMsgToHW(U32 msg_id,  void *local_param_ptr,  void *peer_buf_ptr);
void UsbConfigFirstPage( U8 mode );
void UsbGetUartPortRspHdlr(void *info);
void UsbCfgMSTimeoutHdlr(void);
void UsbConfigMassStorage(void);
void UsbConfigCDCACM(void);
U8 GetUsbDefAppSetting(void);

//extern void PendingUsbDetectionAtPowerOnHdlr(void);

#endif /* __MMI_USB_SUPPORT__ */

#endif /* MMI_USBDEVICE_DEFS_H */

