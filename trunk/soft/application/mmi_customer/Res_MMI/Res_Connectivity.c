/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
#include "cs_types.h"
#include "custdatares.h"
#include "mmi_features.h"
#include "stdc.h"
#include "customcfg.h"


#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"

#include "extdevicedefs.h"


/***************************************************************************************************/
/*** USB Device Resource  ***/
/***************************************************************************************************/
#ifdef __MMI_USB_SUPPORT__

void PopulateExtDevUSBRes(void)
{
    ADD_APPLICATION_STRING2(STR_USB_CONFIG_TITLE,"USB Config:","USB Detection");
    ADD_APPLICATION_IMAGE2(IMG_USB_POWER_ON,CUST_IMG_PATH"/MainLCD/Active/Poweronoff/USB_noanimation.gif","Power on with USB boot mode");
    ADD_APPLICATION_IMAGE2(IMG_USB_CHGR_CMPLT,CUST_IMG_PATH"/MainLCD/Active/Poweronoff/USB_ChgrCmplt.gif","Power on with USB boot mode");
//	ADD_APPLICATION_IMAGE2(IMG_ID_USB_CHARGING,CUST_IMG_PATH"/MainLCD/Active/Poweronoff/USB_on.gif","Power on with USB boot mode");

#ifdef __MMI_SUBLCD__
    ADD_APPLICATION_IMAGE2(IMG_SUB_USB_POWER_ON,CUST_IMG_PATH"/SubLCD/Active/SB_USB_on.gif","Power on with USB boot mode");
    ADD_APPLICATION_IMAGE2(IMG_USB_SUB_CHGR_CMPLT,CUST_IMG_PATH"/SubLCD/Active/SB_USB_ChgrCmplt.gif","Power on with USB boot mode");
#ifdef __MMI_WEBCAM__
    ADD_APPLICATION_IMAGE2(IMG_ID_WEBCAM_SUB_BG, CUST_IMG_PATH"/SubLCD/Active/SB_USB_Webcam.gif","Webcam sublcd icon");
#endif
#endif /* __MMI_SUBLCD__ */



#ifdef __MMI_WEBCAM__
    /* Webcam Image Resource */
    ADD_APPLICATION_IMAGE2(IMG_ID_WEBCAM_BG, CUST_IMG_PATH"/MainLCD/Active/webcam/webcam_bg.gif","webcam screen");
	#if 0//JiaShuo Delete for webcam
    ADD_APPLICATION_IMAGE2(IMG_ID_WEBCAM_PAUSE_JPEG_0, CUST_IMG_PATH"/MainLCD/Active/webcam/webcam_jpeg_0.jpg","Pause screen jpeg");
    ADD_APPLICATION_IMAGE2(IMG_ID_WEBCAM_PAUSE_JPEG_1, CUST_IMG_PATH"/MainLCD/Active/webcam/webcam_jpeg_1.jpg","Pause screen jpeg");
    ADD_APPLICATION_IMAGE2(IMG_ID_WEBCAM_PAUSE_JPEG_2, CUST_IMG_PATH"/MainLCD/Active/webcam/webcam_jpeg_2.jpg","Pause screen jpeg");
    ADD_APPLICATION_IMAGE2(IMG_ID_WEBCAM_PAUSE_JPEG_3, CUST_IMG_PATH"/MainLCD/Active/webcam/webcam_jpeg_3.jpg","Pause screen jpeg");
	#endif
#endif /* __MMI_WEBCAM__ */


#if defined(__MMI_USB_CDCACM_ONLY__)

    /* CDCACM Only */
    ADD_APPLICATION_STRING2(STR_USB_CONFIG_CDCACM_ONLY,"USB Detect! Set as COM port?","USB Detection");

#elif defined(__MMI_USB_MS_ONLY__)

    /* MS Only */
    ADD_APPLICATION_STRING2(STR_USB_CONFIG_MS_ONLY,"USB Detect! Shutdown for Mass storage?","USB Detection");

#else /* Both */

    /* Mass Storage & CDCACM */ 

    /* For charger/alarm power on */
    ADD_APPLICATION_STRING2(STR_USB_CONFIG_MS_ONLY,"USB Detect! Shutdown for Mass storage?","USB Detection");

    ADD_APPLICATION_STRING2(STR_USB_CONFIG_MS,"Mass Storage","USB Detection");
    ADD_APPLICATION_STRING2(STR_USB_CONFIG_CDCACM_DATA,"Data Port","USB Detection");
    ADD_APPLICATION_STRING2(STR_USB_CONFIG_CDCACM_DEBUG,"Debug Port","USB Detection");

#endif  /* __MMI_USB_CDCACM_ONLY__ && __MMI_USB_CDCACM_ONLY__  */


#ifdef __MMI_WEBCAM__
    ADD_APPLICATION_STRING2(STR_ID_USB_CONFIG_WEBCAM,"Webcam","USB Detection");
    ADD_APPLICATION_STRING2(STR_ID_USB_NOTIFY_WEBCAM_ERROR,"Webcam device error","USB Device Error");	
    ADD_APPLICATION_STRING2(STR_ID_USB_NOTIFY_WEBCAM_CAM_ERROR,"Camera Device Error","USB Device Error");	    
    ADD_APPLICATION_STRING2(STR_ID_USB_WEBCAM_STOPPED,"Webcam Stopped","Webcam stopped");			
#endif /* __MMI_WEBCAM__ */

    ADD_APPLICATION_STRING2(STR_USB_SHUTDOWN_SCR,"Shutdown for Mass Storage...","USB Detection");
    ADD_APPLICATION_STRING2(STR_USB_POWER_ON_PROCESSING,"Power on for Mass Storage...","USB Detection");
    ADD_APPLICATION_STRING2(STR_USB_CDCACM_SETTING_SCR,"COM Port Configing...","USB Detection");
    ADD_APPLICATION_STRING2(STR_ID_USB_MODE_EXPORTED_WARNING,"USB: Function not allowed!","USB Detection");
#if 0
#ifdef __USB_IN_NORMAL_MODE__
    ADD_APPLICATION_STRING2(STR_ID_USB_MODE_PROGRESS,"Waiting","USB Detection");
    ADD_APPLICATION_STRING2(STR_ID_USB_NO_MS_FOR_PC,"USB: No storage for PC","USB Detection");    
    ADD_APPLICATION_STRING2(STR_ID_USB_SYS_NOT_READY,"USB: System not ready","USB Detection");    
#endif
    ADD_APPLICATION_STRING2(STR_ID_USB_IRCOMM_OCCUPY_CDCACM, "Please turn off IrDA","USB Detection");
	
    ADD_APPLICATION_STRING2(STR_ID_USB_UART2_OCCUPIED,"UART2 is Occupied!","USB Detection");    	
	#endif
} /* End of void PopulateExtDevUSBRes(void) */

#endif /* __MMI_USB_SUPPORT__ */





/***************************************************************************************************/
/*** Infrared Resource  ***/
/***************************************************************************************************/
#ifdef __MMI_IRDA_SUPPORT__

void PopulateExtDevIrDARes(void)
{
      /* String Resource */
	ADD_APPLICATION_STRING2(STR_IRDA_MENU_TITLE,   "Infrared Setting","Infrared Setting title");
     ADD_APPLICATION_STRING2(STR_IRDA_MENU_OPEN,    "Open","Infrared Open");
	ADD_APPLICATION_STRING2(STR_IRDA_MENU_CLOSE,  "Close","Infrared Close");
	ADD_APPLICATION_STRING2(STR_IRDA_MENU_PERIOD, "Open 10 Minutes","Infrared Open a certain period");

#if defined(__MMI_IRDA_CARD_SUPPORT__) //Priyesh - Memory Card support
	ADD_APPLICATION_STRING2(STR_IRDA_MENU_STORAGE, "Storage Location","Infrared Data Storage Location");
	ADD_APPLICATION_STRING2(STR_IRDA_MENU_ACTIVATION, "Activation Menu","Infrared Activation Menu");
	ADD_APPLICATION_STRING2(STR_IRDA_STORAGE_PHONE, "Phone","Infrared Data Storage is Phone");
	ADD_APPLICATION_STRING2(STR_IRDA_STORAGE_CARD, "External Card","Infrared Data Storage is External Card");
	ADD_APPLICATION_STRING2(STR_IRDA_NO_CARD_ERROR, "No External Card Inserted","Infrared Data Storage can't be External Card");
	ADD_APPLICATION_STRING2(STR_IRDA_FILE_RECEIVED, "Infrared File(s) Received","Infrared files received");
#endif //__MMI_IRDA_CARD_SUPPORT__ //Priyesh - Memory Card Support

	ADD_APPLICATION_STRING2(STR_IRDA_RECV_DISK_FULL, "Disk Full, IrDA Receive Fail","Received file");
	ADD_APPLICATION_STRING2(STR_IRDA_RECV_ROOT_DIR_FULL, "Root Directory Full, IrDA Receive Fail","Root Directory Full");
	ADD_APPLICATION_STRING2(STR_IRDA_RECV_FILE_IND, "File stored at '\Received' folder","Received file");

	ADD_APPLICATION_STRING2(STR_IRDA_NOTY_RECV_FILE_BROWSE, "Browse","Received file");
	ADD_APPLICATION_STRING2(STR_IRDA_NOTY_RECV_FILE_HED, "Received","Received file");
	ADD_APPLICATION_STRING2(STR_IRDA_NOTY_RECV_FILE_TIL, "Files","Received file");

	ADD_APPLICATION_STRING2(STR_IRDA_DEV_BUSY, "IR Device Busy","Device Busy");
	ADD_APPLICATION_STRING2(STR_IRDA_SEND, "Sending","IR Device Sending");
	//ADD_APPLICATION_STRING2(STR_IRDA_WRITE_FILE_FAIL, "Write File Failed","FS write failed");
#ifdef __MMI_DOWNLOADABLE_THEMES_SUPPORT__ 
	/* shantanu -for DLT */
	ADD_APPLICATION_STRING2(STR_IRDA_THEME_FILE_RECV, "Received Theme File","Received File message");
	ADD_APPLICATION_STRING2(STR_ID_DLT_DO_YOU_WANT_TO_ACTIVATE, "Do you want to Activate Theme?","Do you want to message");
	/* shantanu -for DLT */
#endif

      /* Image Resource */
      /* Status icon is populate in Res_Gui.c */
	//ADD_APPLICATION_IMAGE2(IMG_IRDA_ICON, CUST_IMG_PATH"/MainLCD/IdleScreen/Statusicons/SI_IR.bmp","IrDA active icon");
	ADD_APPLICATION_IMAGE2(IMG_IRDA_MENU_TITLE, CUST_IMG_PATH"/MAINLCD/Irda/SB_IR.pbm","IrDA menu icon");

      /*Menu Items*/
	  #if 0
#if (defined( __MMI_VERSION_2__ ) && defined( __MMI_IRDA_SUPPORT__ ) )

#if !defined(__MMI_IRDA_CARD_SUPPORT__) //Priyesh - Memory Card Support
	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_MAIN, MAIN_MENU_EXTRA_MENUID, 3,
                                                         MENU_CONN_IRDA_OPEN,
                                                         MENU_CONN_IRDA_CLOSE,
                                                         MENU_CONN_IRDA_PERIOD,
	                                                    SHOW,MOVEABLEACROSSPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_TITLE, IMG_IRDA_MENU_TITLE ));
#else //__MMI_IRDA_CARD_SUPPORT__ //Priyesh - Memory Card Support
	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_MAIN, MAIN_MENU_EXTRA_MENUID, 2,
                                                         MENU_CONN_IRDA_ACTIVATION,
														 MENU_CONN_IRDA_STORAGE,
	                                                    SHOW,MOVEABLEACROSSPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_TITLE, IMG_IRDA_MENU_TITLE ));
#endif //__MMI_IRDA_CARD_SUPPORT__ //Priyesh - Memory Card Support

#if !defined(__MMI_IRDA_CARD_SUPPORT__) //Priyesh - Memory Card Support
	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_OPEN, MENU_CONN_IRDA_MAIN, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_OPEN, IMG_IRDA_MENU_OPEN  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_CLOSE, MENU_CONN_IRDA_MAIN, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_CLOSE, IMG_IRDA_MENU_CLOSE  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_PERIOD, MENU_CONN_IRDA_MAIN, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_PERIOD, IMG_IRDA_MENU_PERIOD  ));
#else //__MMI_IRDA_CARD_SUPPORT__ //Priyesh - Memory Card Support
	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_ACTIVATION, MENU_CONN_IRDA_MAIN, 3,
                                                         MENU_CONN_IRDA_OPEN,
                                                         MENU_CONN_IRDA_CLOSE,
                                                         MENU_CONN_IRDA_PERIOD,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_ACTIVATION, IMG_IRDA_MENU_ACTIVATION ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_OPEN, MENU_CONN_IRDA_ACTIVATION, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_OPEN, IMG_IRDA_MENU_OPEN  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_CLOSE, MENU_CONN_IRDA_ACTIVATION, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_CLOSE, IMG_IRDA_MENU_CLOSE  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_PERIOD, MENU_CONN_IRDA_ACTIVATION, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_PERIOD, IMG_IRDA_MENU_PERIOD  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_STORAGE, MENU_CONN_IRDA_MAIN, 2,
                                                         MENU_CONN_IRDA_STORAGE_PHONE,
                                                         MENU_CONN_IRDA_STORAGE_CARD,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_MENU_STORAGE, IMG_IRDA_MENU_STORAGE ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_STORAGE_PHONE, MENU_CONN_IRDA_STORAGE, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_STORAGE_PHONE, IMG_IRDA_STORAGE_PHONE  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_IRDA_STORAGE_CARD, MENU_CONN_IRDA_STORAGE, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                               STR_IRDA_STORAGE_CARD, IMG_IRDA_STORAGE_CARD  ));
#endif //__MMI_IRDA_CARD_SUPPORT__ //Priyesh - Memory Card Support

#endif
#endif
}

#endif /* __MMI_IRDA_SUPPORT__ */






/***************************************************************************************************/
/*** General Bluetooth Resource  ***/
/***************************************************************************************************/
#ifdef __MMI_SUPPORT_BLUETOOTH__

void PopulateExtDevBTRes(void)
{
   /* String Resource */
#ifdef __BT_ACCEL_SUPPORT__
	ADD_APPLICATION_STRING(STR_BT_MENU_TITLE,"Bluetooth");
	ADD_APPLICATION_STRING(STR_BT_SWITCH,"Power");
	ADD_APPLICATION_STRING(STR_BT_SHHF,"Search Audio Device");
	ADD_APPLICATION_STRING(STR_BT_MYDEV,"My Device");
	ADD_APPLICATION_STRING(STR_BT_SET,"Setting");
	ADD_APPLICATION_STRING(STR_BT_DISCON,"Disconnect Device");
	ADD_APPLICATION_STRING(STR_BT_SET_VIS,"Visibility");
	ADD_APPLICATION_STRING(STR_BT_SET_NAME,"Change Device Name");
	ADD_APPLICATION_STRING(STR_BT_SET_AUTH,"Authentication");
	ADD_APPLICATION_STRING(STR_BT_SET_PATH,"Set Audio Path");
	ADD_APPLICATION_STRING(STR_BT_SET_PATH_PHONE,"Leave in Phone");
	ADD_APPLICATION_STRING(STR_BT_SET_PATH_HF,"Forward to Earphone");

	ADD_APPLICATION_STRING(STR_BT_PWRON,"Power on Bluetooth...");
	ADD_APPLICATION_STRING(STR_BT_SEARCH,"Searching...");
	ADD_APPLICATION_STRING(STR_BT_SEARCH_FOUND,"Found");
	ADD_APPLICATION_STRING(STR_BT_SEARCH_RES,"Device Found");
	ADD_APPLICATION_STRING(STR_BT_LSK_CONN,"Conn.");
	ADD_APPLICATION_STRING(STR_BT_PAIR,"Pair");
	ADD_APPLICATION_STRING(STR_BT_PAIRING,"Pairing...");
	ADD_APPLICATION_STRING(STR_BT_ENTER_PIN,"Enter Bluetooth Passcode:");
	ADD_APPLICATION_STRING(STR_BT_CONN_CNF,"Connect Device?");
	ADD_APPLICATION_STRING(STR_BT_PWRON_CNF,"Power on Bluetooth?");
	ADD_APPLICATION_STRING(STR_BT_MYDEVOPT_SCH,"Search new device");
	ADD_APPLICATION_STRING(STR_BT_SCH_ALL_DEV,"Inquiry BT Device");
	ADD_APPLICATION_STRING(STR_BT_HW_ERROR,"Hardware Error!");
	ADD_APPLICATION_STRING(STR_BT_MYDEVOPT_CONN,"Connect");
	ADD_APPLICATION_STRING(STR_BT_MYDEVOPT_DEL,"Delete");
	ADD_APPLICATION_STRING(STR_BT_BUSY,"Bluetooth device busy");
	ADD_APPLICATION_STRING(STR_BT_ENLIST_TRDEV_CNF,"Add to trust list?");
	ADD_APPLICATION_STRING(STR_BT_LSK_DISC,"Disc.");
	ADD_APPLICATION_STRING(STR_BT_PROFCON_HS,"Bluetooth Headset Connected");
	ADD_APPLICATION_STRING(STR_BT_PROFCON_HF,"Bluetooth HeadFree Connected");	
	ADD_APPLICATION_STRING(STR_BT_SCO_CON,"Bluetooth Voice Channel Established");
	ADD_APPLICATION_STRING(STR_BT_CM_SETPATH,"Swap Bluetooth Voice Path");
	ADD_APPLICATION_STRING(STR_BT_MYDEVOPT_REN,"Rename");
	ADD_APPLICATION_STRING(STR_OPP_DEV_BUSY,"Bluetooth Device Busy");
	ADD_APPLICATION_STRING(STR_OPP_NOTY_RECV_ACCEPT,"Receive Data?");
	ADD_APPLICATION_STRING(STR_BT_SCO_DISCON,"Bluetooth Voice Channel Released");

	ADD_APPLICATION_STRING2(STR_BT_SET_VIS_PROC, "Setting...","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_SET_NAME_DONE, "Done! Reboot to take effect","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_SET_NAME_DENY, "Ascii Character Only!","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_SET_NAME_LENGTH_LIMITATION, "Cannot be NULL!","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AUD_PATH_PHONE, "Phone","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AUD_PATH_HF, "HandFree","Bluetooth Setting title");
	
#else
	ADD_APPLICATION_STRING2(STR_BT_MENU_TITLE, "Bluetooth","Bluetooth Main Menu");
	ADD_APPLICATION_STRING2(STR_BT_LSK_DISC, "Disc.","LSK");
	ADD_APPLICATION_STRING2(STR_BT_CM_SETPATH, "Swap Bluetooth Voice Path","Bluetooth Setting title");	
		ADD_APPLICATION_STRING2(STR_BT_CONNECT_LAST_HEADSET, "Connect Default Bluetooth Headset","LSK");
	ADD_APPLICATION_STRING2(STR_BT_NO_DEFAULT_HEADSET, "No Default Bluetooth Headset","Popup string");	

	ADD_APPLICATION_STRING2(STR_BT_LSK_CONN, "Conn.","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_POWER, "Power","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_SWITCHING, "Switching","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_POWEROFF, "Power off","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_SWITCHINGOFF, "Switching","Power menu");

	ADD_APPLICATION_STRING2(STR_BT_SWITCHED_ON, "Bluetooth Switched On","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_SWITCHED_OFF, "Bluetooth Switched Off","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_RELEASE_ALL_CONN, "Release All Connections","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_RELEASE_DEV_CONN, "Release Device Connections","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_RELEASING, "Releasing","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_CON_RELEASED, "Connection Released","Power menu");
	ADD_APPLICATION_STRING2(STR_BT_SEARCHING, "Searching","Searching");
	ADD_APPLICATION_STRING2(STR_BT_SCH_AUD_DEV, "Inquiry Audio Device","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_SCH_OPP_DEV, "Inquiry Opp Device","Bluetooth Setting title");

	ADD_APPLICATION_STRING2(STR_BT_SCH_ALL_DEV, "Inquiry BT Device","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_SCH_RESULT, "Discovered Device","Discovered Device");
	ADD_APPLICATION_STRING2(STR_BT_INQUIRY_STOP, "Stop Searching","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_INQUIRY_STOPPING, "Stop Searching","Discovered Device");
	ADD_APPLICATION_STRING2(STR_BT_INQUIRY_CANT, "have been a conect device, can't  Searching!","have been a conect device, can't  Searching"); 
	ADD_APPLICATION_STRING2(STR_BT_BOND, "Bond","Bond");
	ADD_APPLICATION_STRING2(STR_BT_BONDING, "Bonding","Bonding");
	ADD_APPLICATION_STRING2(STR_BT_BOND_FAILED, "Bonding Failed","Bonding Failed");
	ADD_APPLICATION_STRING2(STR_BT_SEND, "Send","Bond");
	ADD_APPLICATION_STRING2(STR_BT_ENTER_PASSCODE, "Passcode", "Passcode");
	ADD_APPLICATION_STRING2(STR_BT_PAIR_REQ, "Pair","Pair");
	ADD_APPLICATION_STRING2(STR_BT_PAIR_FAILED, "Pairing Failed","Pairing Failed");
	ADD_APPLICATION_STRING2(STR_BT_PAIR_SUCCEED, "Paired","Paired");
	ADD_APPLICATION_STRING2(STR_BT_START_PAIRING, "Pairing", "Pairing");
	ADD_APPLICATION_STRING2(STR_BT_CONNECT, "Connect","Connect");
	ADD_APPLICATION_STRING2(STR_BT_CONNECT_AND_SET_AS_DEFAULT_HF, "Connect and Set As Default Handsfree","Connect");
	ADD_APPLICATION_STRING2(STR_BT_ACCEPT_CONNECT_FROM, "Accept Connect From","Accept Connect From");
	ADD_APPLICATION_STRING2(STR_BT_CONNECTING, "Connecting", "Connecting");
	ADD_APPLICATION_STRING2(STR_BT_DISCONNECT, "Disconnect","Disconnect");
	ADD_APPLICATION_STRING2(STR_BT_DISCONNECTING, "Disconnecting", "Disconnecting");
	ADD_APPLICATION_STRING2(STR_BT_REFRESH, "Refresh", "Connecting");
	ADD_APPLICATION_STRING2(STR_BT_REFRESH_SERVICE_LIST, "Refresh Service List","Disconnect");
	ADD_APPLICATION_STRING2(STR_BT_REFRESHING, "Refreshing", "Disconnecting");
	ADD_APPLICATION_STRING2(STR_BT_DELETE, "Delete", "Delete");
	ADD_APPLICATION_STRING2(STR_BT_DELETING, "Deleting", "Deleting");
	ADD_APPLICATION_STRING2(STR_BT_DELETED, "Deleted", "Deleted");
	ADD_APPLICATION_STRING2(STR_OBEX_NOTIFY_CANNOT_SWITCH_APPLICATION, "Can't switch,please close current application", "Can't switch,please close current application");
	
	ADD_APPLICATION_STRING2(STR_BT_MYDEVICE, "Paired Device","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_CONN, "Connect","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_DISCONN, "Disconnect","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_CONNECTED_PROF, "Connected Service","Bluetooth Setting title");
        ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_SET_DEFAULT_HS, "Set As Default Handsfree Headset","Bluetooth Setting title"); 
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_REN, "Rename","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_AUTH, "Authorisation","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_DEL, "Delete","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_DEL_ALL, "Delete All","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_SUP_SER, "Service List","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_MYDEV_OPT_SCH, "Inquiry New Device","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_ACT_DEV, "Active Device","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET, "Setting","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET_VIS, "Visibility","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET_NAME, "Change Device Name","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET_AUTH, "Authentication","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET_AUDIO_PATH, "Audio Path","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET_AUDIO_PATH_PHONE, "Leave in Phone","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET_AUDIO_PATH_HF, "Forward to BT Headset","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET_FTP, "FTP","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_SET_SIMAP, "Activate SIMAP", "Bluetooth Setting title");    
	ADD_APPLICATION_STRING2(STR_BT_ABOUT, "About","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_BD_NAME, "Device Name:","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_SERVICE, "Supported Service:","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_BD_ADDR, "BD Addr:","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_CONN_ED, "Connected","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DISCON_ED, "Disconnected","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_ESTAB, "Established","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_RELEASE, "Released","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AUDIO_SCO_CON, "Transfer Voice to BT Headset","Transfer Voice to BT Headset");
	ADD_APPLICATION_STRING2(STR_BT_AUDIO_SCO_DISCON, "Transfer Voice Back to Phone","Transfer Voice Back to Phone");
	ADD_APPLICATION_STRING2(STR_BT_PROF_SPP, "SPP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_LAP, "LAP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_DUN, "DUN Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_SYNC, "SYNC Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_OPP, "OPP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_FTP, "FTP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_FAX, "FAX Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_CTP, "CTP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_AUDIO_SOURCE, "AUDIO_SOURCE Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_AUDIO_SINK, "AUDIO_SINK Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_AVRCP_T, "AVRCP_T Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_A2DP, "A2DP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_AVRCP, "AVRCP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_ICP, "ICP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_PAN_PANU, "PAN_PANU Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_PAN_NAP, "PAN_NAP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_PAN_GN, "PAN_GN Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_DIRECT_PRINTING, "DIRECT_PRINTING Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_OBEX_IMAGING, "OBEX_IMAGING Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_OBEX_IMAGING_RESPONDER, "OBEX_IMAGING_RESPONDER Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_HF, "Handsfree Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_HS, "Headset Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_BASIC_PRINTING, "BPP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_HID, "HID Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_SAP, "SAP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_PBAP, "PBAP Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_OBEX_BPPS, "BPPS Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROF_UNKNOWN, "Unknown Profile","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_NOT_READY, "Initializing Bluetooth Now, Wait a Moment !","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_HARDWARE_ERROR, "Hardware Error!","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEV_BUSY, "Device Busy","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_FAILED, "Failed","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_NOT_SUPPORT, "Not Support!","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_NOT_ALLOW, "Not Allow!","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_NO_SUPPORTED_PROFILE, "Service List Empty","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PWR_ON_CNF, "Power on Bluetooth First ?","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DISCONN_ALL_CONN_CNF, "Release All Connections First ?", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DISCONN_DEV_CONN_CNF, "Release Device Connections First ?", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_TRY_HFP_FIRST, "Try to connect with HFP first!", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_ONE_HEADSET_ALREADY_CONNECTED, "Already Connected with One Bluetooth Headset!", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PAIRED_DEV_FULL, "Paired Device Full, Delete Some Devices First!", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROHIBIT_BY_A2DP, "This Service is Prohibited by A2DP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROHIBIT_VIS_BY_A2DP, "Prohibited by A2DP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_SWITCH_SPEECH_PATH, "Switch Speech To", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_WAIT_PAIR_RESULT, "Waiting for Pairing Result", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_WAIT_INQUIRY_FINISH, "Cancelling Inquiry, Please Wait!", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DISABLE_VISIBILITY, "Set visibility to be off", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_ENABLE_VISIBILITY, "Set visibility to be ON", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_OPERATION_NOT_ALLOWED_DIS_HFP_FIRST, "Operation Not Allowed, Please Disconnect HFP First!", "Bluetooth Setting title");

	ADD_APPLICATION_STRING2(STR_BT_SET_NAME_LENGTH_LIMITATION, "Cannot be NULL!","Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROHIBIT_VIS_BY_MAX_ACL, "Prohibited by max links reached", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_PROHIBIT_BY_SIMAP, "Prohibited by SIMAP, Shall Deactivate SIMAP First!", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_ACTIVATING, "Activating", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_DEACTIVATING, "Deactivating", "Bluetooth Setting title");    

	ADD_APPLICATION_STRING2(STR_BT_REMOTE_CONTROL, "Remote Control", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_REMOTE_CTRL_LAST_DEV, "Last Device", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_REMOTE_CTRL_NO_LAST_DEV_INFO, "No Last Device Information", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_REMOTE_CTRL_PAIRED_DEV, "Paired Device", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_REMOTE_CTRL_DESKTOP, "Desktop", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_REMOTE_CTRL_MEDIA_PLAYER, "Media Player", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_REMOTE_CTRL_PRESENTER, "Presenter", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_REMOTE_CONTROL_DISCONNECT_CONFIRM, "Exit Remote Control ?", "Bluetooth Setting title");
#endif

#ifdef __MMI_AVRCP_DEBUG__
#if 0
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_UNIT_COMMAND, "UNIT COMMAND", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_SUBUNIT_COMMAND, "SUBUNIT COMMAND", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_SELECT, "SELECT", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_UP, "UP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_DOWN, "DOWN", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_LEFT, "LEFT", "Bluetooth Setting title");	
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_RIGHT, "RIGHT", "Bluetooth Setting title");	
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_RIGHT_UP, "RIGHT_UP", "Bluetooth Setting title");	
    ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_RIGHT_DOWN, "RIGHT_DOWN", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_LEFT_UP, "LEFT_UP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_LEFT_DOWN, "LEFT_DOWN", "Bluetooth Setting title");	
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_ROOT_MENU, "ROOT MENU", "Bluetooth Setting title");	
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_SETUP, "SETUP MENU", "Bluetooth Setting title");	
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_CONTENTS, "CONTENTS MENU", "Bluetooth Setting title");	
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_FAVORITE, "FAVORITE MENU", "Bluetooth Setting title");	
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_EXIT, "EXIT", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_0, "0", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_1, "1", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_2, "2", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_3, "3", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_4, "4", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_5, "5", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_6, "6", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_7, "7", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_8, "8", "Bluetooth Setting title");	
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_9, "9", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_DOT, "DOT", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_ENTER, "ENTER", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_CLEAR, "CLEAR", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_CHANNEL_UP, "CHANNEL_UP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_CHANNEL_DOWN, "CHANNEL_DOWN", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_PREVIOUS_CHANNEL, "PREVIOUS_CHANNEL", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_SOUND_SELECT, "SOUND_SELECT", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_INPUT_SELECT, "INPUT_SELECT", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_DISPLAY_INFO, "DISPLAY_INFO", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_HELP, "HELP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_PAGE_UP, "PAGE_UP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_PAGE_DOWN, "PAGE_DOWN", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_POWER, "POWER", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_VOLUME_UP, "VOLUME_UP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_VOLUME_DOWN, "VOLUME_DOWN", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_MUTE, "MUTE", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_PLAY, "PLAY", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_STOP, "STOP", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_PAUSE, "PAUSE", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_RECORD, "RECORD", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_REWIND, "REWIND", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_FAST_FORWARD, "FAST_FORWARD", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_EJECT, "EJECT", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_FORWARD, "FORWARD", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_BACKWARD, "BACKWARD", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_ANGLE, "ANGLE", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_SUBPICTURE, "SUBPICTURE", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_F1, "F1", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_F2, "F2", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_F3, "F3", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_F4, "F4", "Bluetooth Setting title");
	ADD_APPLICATION_STRING2(STR_BT_AVRCP_CMD_F5, "F5", "Bluetooth Setting title");
	#endif
#endif /* __MMI_AVRCP_DEBUG__ */

   /* Image Resource */
   /* Status icon is populate in Res_Gui.c */
	ADD_APPLICATION_IMAGE2(IMG_BT_MENU_TITLE, CUST_IMG_PATH"/MainLCD/Bluetooth/SB_BT.pbm","Bluetooth menu icon");
	//ADD_APPLICATION_IMAGE2(IMG_BT_SCH_TITLE, CUST_IMG_PATH"/MainLCD/Bluetooth/OBT_Search.gif","Bluetooth menu icon");

	ADD_APPLICATION_IMAGE2(IMG_BT_DEV_MOB, CUST_IMG_PATH"/MainLCD/Bluetooth/OBT_MOB.pbm","Bluetooth menu icon");
	ADD_APPLICATION_IMAGE2(IMG_BT_DEV_LAP, CUST_IMG_PATH"/MainLCD/Bluetooth/OBT_LAP.pbm","Bluetooth menu icon");
	ADD_APPLICATION_IMAGE2(IMG_BT_DEV_EAR, CUST_IMG_PATH"/MainLCD/Bluetooth/OBT_EAR.pbm","Bluetooth menu icon");
	ADD_APPLICATION_IMAGE2(IMG_BT_DEV_OTHER, CUST_IMG_PATH"/MainLCD/Bluetooth/OBT_other.pbm","Bluetooth menu icon");


}


/***************************************************************************************************/
/*** BCHS FTP OPP Resources ***/
/***************************************************************************************************/
#if defined(__MMI_FTS_SUPPORT__) || defined(__MMI_FTC_SUPPORT__)

void PopulateExtDevFTPRes(void)
{
    /* String Resource */
#ifdef __MMI_FTS_SUPPORT__
    
	ADD_APPLICATION_STRING2(STR_FTP_SETTING, "FTP Setting","FTP Setting string");
	ADD_APPLICATION_STRING2(STR_FTP_SHARED_FOLDER, "Shared Folder","Shared Folder string");
	ADD_APPLICATION_STRING2(STR_FTP_ACCESS_RIGHT, "Access Right","Access Right string");
       ADD_APPLICATION_STRING2(STR_FTP_RECEIVE_FOLDER, "Receive Folder","Receive Folder string"); 	ADD_APPLICATION_STRING(STR_FTP_OPTION_FULLY_CONTROL,"Fully Control");

	ADD_APPLICATION_STRING2(STR_FTP_OPTION_FULLY_CONTROL, "Fully Control","Fully Control string");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_READ_ONLY, "Read Only","Read Only string");
	ADD_APPLICATION_STRING2(STR_FTP_CON_AUTH, "Accept FTP request? From","Accept FTP connect request string");
	ADD_APPLICATION_STRING2(STR_FTP_RECV_FILE_IND, "File stored at shared folder","Received file");
	ADD_APPLICATION_STRING2(STR_FTP_SERVER_BUSY, "FTP server is busy, please try again later","FTP server is busy");
		ADD_APPLICATION_STRING2(STR_FTP_DISCONN_CLIENT, "Disconnect client first!","Disconnect client first");
	ADD_APPLICATION_STRING2(STR_FTP_QUESTION_MARK, "?","Question mark");

   /* FTP Server Setting Menu */
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_SETTING, 
											MENU_CONN_BT_SET, 
											3,
                                 MENU_CONN_FTP_SHARED_FOLDER,
                                 MENU_CONN_FTP_ACCESS_RIGHT,/* Should be the last one */
                                 MENU_CONN_FTP_RECEIVE_FOLDER,
	                              SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                           STR_FTP_SETTING, IMG_FTP_SETTING_TITLE ));
   /* FTP Server Setting Shared Folder */
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_SHARED_FOLDER, MENU_CONN_FTP_SETTING, 2,
	                                                    MENU_FMGR_GEN_OPTION_OPEN,
	                                                    MENU_FMGR_SELECT_DONE,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_SHARED_FOLDER, IMG_FTP_SHARED_FOLDER));

 /* FTP Server Setting Shared Folder */
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_RECEIVE_FOLDER, MENU_CONN_FTP_SETTING, 3,
	                                                    MENU_FMGR_GEN_OPTION_OPEN,
	                                                    MENU_FMGR_SELECT_DONE,
	                                                    MENU_FMGR_FOLDER_CREATE,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_RECEIVE_FOLDER, IMG_FTP_RECEIVE_FOLDER));

   /* FTP Server Setting Access Right */
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_ACCESS_RIGHT, MENU_CONN_FTP_SETTING, 2,
	                                                    MENU_CONN_FTP_ACCESS_RIGHT_OPTION_FULLY_CONTROL,
	                                                    MENU_CONN_FTP_ACCESS_RIGHT_OPTION_READ_ONLY,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_ACCESS_RIGHT, IMG_FTP_ACCESS_RIGHT));

	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_ACCESS_RIGHT_OPTION_FULLY_CONTROL, MENU_CONN_FTP_ACCESS_RIGHT, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_FULLY_CONTROL, 0  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_ACCESS_RIGHT_OPTION_READ_ONLY, MENU_CONN_FTP_ACCESS_RIGHT, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_READ_ONLY, 0  ));
#endif

#ifdef __MMI_FTC_SUPPORT__
	ADD_APPLICATION_STRING2(STR_FTP_FILE_BROWSER, "File Browser","File Browser string");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_GET, "Get","Get File string");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_REFRESH, "Refresh","Refresh string");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_DETAIL, "Detail","File detailed info string");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_BACK_TO_ROOT_DIR, "Back to Root Dir","Back to Root Dir string");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_DISCONNECT, "Disconnect","Disconnect string");
#ifdef __MMI_FTC_MULTIPULL_SUPPORT__
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_ADD_TO_LIST, "Add To List","Add To List");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_SHOW_LIST, "Show List","Show List");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_GET_LIST, "Get List","Get List");
	ADD_APPLICATION_STRING2(STR_FTP_OPTION_GET_ALL, "Get All","Get All");
	ADD_APPLICATION_STRING2(STR_FTP_GET_ALL_CONFIRM_HEAD, "Get all the","Get all the");
	ADD_APPLICATION_STRING2(STR_FTP_GET_ALL_CONFIRM_TAIL, "files?","files?");
	ADD_APPLICATION_STRING2(STR_FTP_LIST_OPTION_UP, "Up","Up");
	ADD_APPLICATION_STRING2(STR_FTP_LIST_OPTION_DOWN, "Down","Down");
	ADD_APPLICATION_STRING2(STR_FTP_LIST_FILE_DEL_CONFIRM, "Delete?","Delete?");
	ADD_APPLICATION_STRING2(STR_FTP_LIST_ALL_FILE_DEL_CONFIRM, "Delete all the files in list?","Delete all the files in list?");
	ADD_APPLICATION_STRING2(STR_FTP_LIST_FIRST_FILE, "The file is already the first file","The file is already the first file");
	ADD_APPLICATION_STRING2(STR_FTP_LIST_LAST_FILE, "The file is already the last file","The file is already the last file");
	ADD_APPLICATION_STRING2(STR_FTP_CLEAR_LIST, "It will clean the list,continue?","It will clean the list,continue?");
	ADD_APPLICATION_STRING2(STR_FTP_FILE_ALREADY_IN_LIST, "The file is already in list","The file is already in list");
#endif
	ADD_APPLICATION_STRING2(STR_FTP_STATUS_DISCONNECTING, "Disconnecting","Disconnecting string");
	ADD_APPLICATION_STRING2(STR_FTP_STATUS_CONNECTING, "Connecting","Connecting string");
	ADD_APPLICATION_STRING2(STR_FTP_STATUS_RECEIVING, "Receiving","Receiving string");
	ADD_APPLICATION_STRING2(STR_FTP_CON_EXIT, "Exit File Browser?","Confirm disconnection of current FTP session");
	ADD_APPLICATION_STRING2(STR_FTP_NOTY_RECV_FILE_BROWSE, "Browse","Received file");
	ADD_APPLICATION_STRING2(STR_FTP_NOTY_RECV_FILE_HED, "Received","Received file");
	ADD_APPLICATION_STRING2(STR_FTP_NOTY_RECV_FILE_TIL, "Files","Received file");
	ADD_APPLICATION_STRING2(STR_FTP_DETAIL_DATE, "Date","Modified Date");
	ADD_APPLICATION_STRING2(STR_FTP_DETAIL_SIZE, "Size","File Size");
	ADD_APPLICATION_STRING2(STR_FTP_NOTY_SHOW_PARTIAL_CONTENT_ONLY, "Display partial content only","Display partial content only");

   /* FTP Client File Browser folder/file option*/
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_FILE_OPTION, 0, 5
							#ifdef __MMI_FTC_MULTIPULL_SUPPORT__
									+4
							#endif
									,
	                                                    MENU_CONN_FTP_FILE_OPTION_GET,
	                                                    MENU_CONN_FTP_GEN_OPTION_REFRESH,
	                                                    MENU_CONN_FTP_GEN_OPTION_DETAIL,
	                                                    MENU_CONN_FTP_GEN_OPTION_BACK_TO_ROOT_DIR,
	                                                    MENU_CONN_FTP_GEN_OPTION_DISCONNECT,
							#ifdef __MMI_FTC_MULTIPULL_SUPPORT__
                                                           MENU_CONN_FTP_FILE_OPTION_ADD_TO_LIST,
                                                           MENU_CONN_FTP_GEN_OPTION_SHOW_LIST,
                                                           MENU_CONN_FTP_GEN_OPTION_GET_LIST_FILE, 
                                                           MENU_CONN_FTP_FILE_OPTION_GET_ALL, 
							#endif
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_GLOBAL_OPTIONS, 0  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_FOLDER_OPTION, 0, 5,
	                                                    MENU_CONN_FTP_FOLDER_OPTION_OPEN,
	                                                    MENU_CONN_FTP_GEN_OPTION_REFRESH,
	                                                    MENU_CONN_FTP_GEN_OPTION_DETAIL,
	                                                    MENU_CONN_FTP_GEN_OPTION_BACK_TO_ROOT_DIR,
	                                                    MENU_CONN_FTP_GEN_OPTION_DISCONNECT,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_GLOBAL_OPTIONS, 0  ));
	
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_EMPTY_FOLDER_OPTION, 0, 3,
	                                                    MENU_CONN_FTP_GEN_OPTION_REFRESH,
	                                                    MENU_CONN_FTP_GEN_OPTION_BACK_TO_ROOT_DIR,
	                                                    MENU_CONN_FTP_GEN_OPTION_DISCONNECT,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_GLOBAL_OPTIONS, 0  ));
	
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_FILE_OPTION_GET, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_GET, 0  ));
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_FOLDER_OPTION_OPEN, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_GLOBAL_OPEN, 0  ));
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_GEN_OPTION_REFRESH, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_REFRESH, 0  ));
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_GEN_OPTION_DETAIL, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_DETAIL, 0  ));
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_GEN_OPTION_BACK_TO_ROOT_DIR, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_BACK_TO_ROOT_DIR, 0  ));
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_GEN_OPTION_DISCONNECT, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_DISCONNECT, 0  ));
#ifdef __MMI_FTC_MULTIPULL_SUPPORT__
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_FILE_OPTION_ADD_TO_LIST, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_ADD_TO_LIST, 0  ));
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_GEN_OPTION_SHOW_LIST, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_SHOW_LIST, 0  ));
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_GEN_OPTION_GET_LIST_FILE, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_GET_LIST, 0  ));
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_FILE_OPTION_GET_ALL, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_OPTION_GET_ALL, 0  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_LIST_OPTION, 0, 6,
								   MENU_CONN_FTP_GEN_OPTION_GET_LIST_FILE,
								   MENU_CONN_FTP_GEN_OPTION_DETAIL,
	                                                    MENU_CONN_FTP_LIST_OPTION_UP,
	                                                    MENU_CONN_FTP_LIST_OPTION_DOWN,
	                                                    MENU_CONN_FTP_LIST_OPTION_DELETE,
	                                                    MENU_CONN_FTP_LIST_OPTION_DELETE_ALL,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_GLOBAL_OPTIONS, 0  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_LIST_OPTION_UP, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_LIST_OPTION_UP, 0  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_LIST_OPTION_DOWN, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_FTP_LIST_OPTION_DOWN, 0  ));

	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_LIST_OPTION_DELETE, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_BT_MYDEV_OPT_DEL, 0  ));
	
	ADD_APPLICATION_MENUITEM((MENU_CONN_FTP_LIST_OPTION_DELETE_ALL, 0, 0,
	                                                    SHOW,MOVEABLEWITHINPARENT,DISP_LIST,
		                                                 STR_BT_MYDEV_OPT_DEL_ALL, 0  ));
	
#endif
#endif

/* General */
	ADD_APPLICATION_STRING2(STR_FTP_CON_DISCONNECTED, "FTP connection disconnected","FTP connection disconnected string");
	ADD_APPLICATION_STRING2(STR_FTP_RECV_DISK_FULL, "Disk Full", "Disk Full");
	ADD_APPLICATION_STRING2(STR_FTP_RECV_ROOT_DIR_FULL, "Root Directory Full","Root Directory Full");
	ADD_APPLICATION_STRING2(STR_FTP_WRITE_FILE_FAIL, "Write File Failed","FS write failed");
}

#endif /* __MMI_FTS_SUPPORT__ || __MMI_FTC_SUPPORT__ */

#ifdef __MMI_OPP_SUPPORT__
void PopulateExtDevOPPRes(void)
{
      /* String Resource */
	ADD_APPLICATION_STRING2(STR_OPP_RECV_DISK_FULL, "Disk Full","Disk Full");
	ADD_APPLICATION_STRING2(STR_OPP_RECV_ROOT_DIR_FULL, "Root Directory Full","Root Directory Full");
	ADD_APPLICATION_STRING2(STR_OPP_WRITE_FILE_FAIL, "Write File Failed","FS write failed");
	ADD_APPLICATION_STRING2(STR_OPP_RECV_FILE_IND, "File stored at '\Received' folder","Received file");
	ADD_APPLICATION_STRING2( STR_OPP_NO_CARD_ERROR, "No Card", "No Card" );
	ADD_APPLICATION_STRING2(STR_OPP_NOTY_RECV_FILE_BROWSE, "Browse","Received file");
	ADD_APPLICATION_STRING2(STR_OPP_NOTY_RECV_FILE_HED, "Received","Received file");
	ADD_APPLICATION_STRING2(STR_OPP_NOTY_RECV_FILE_TIL, "File","Received file");
	ADD_APPLICATION_STRING2(STR_OPP_NOTY_RECV_ACCEPT, "Receive Data?","Accept new file");

	ADD_APPLICATION_STRING2(STR_OPP_DEV_BUSY, "Bluetooth Device Busy","Device Busy");
	ADD_APPLICATION_STRING2(STR_OPP_SEND, "Sending","Blutooth Device Sending");
	ADD_APPLICATION_STRING2(STR_OPP_CON_DISCONNECTED, "OPP connection disconnected","OPP connection disconnected string");
	        ADD_APPLICATION_STRING2(STR_OPP_UNKNOWN_FILE_TYPE, "Unknown file type","Unknown file type");
        ADD_APPLICATION_STRING2(STR_OPP_NOTY_RECV_FOLDER, "Folder","Folder");
    ADD_APPLICATION_STRING2(STR_OPP_NOTY_RECV_MULTI_FILE_TIL, "files","files");
        ADD_APPLICATION_STRING2(STR_OPP_NOTY_CON_BUSY, "Connction is busy","Connection is busy String");
        ADD_APPLICATION_STRING2(STR_OPP_NOTY_DISCONNECTING, "Disconnecting","Disconnecting String");
#ifdef __MMI_FTC_SUPPORT__
 	ADD_APPLICATION_STRING2(STR_FTP_NOTY_RECV_FILE_TIL_SINGLE, "file","file");
#endif
 	ADD_APPLICATION_STRING2(STR_GLOBAL_RECEIVED, "Received ","Received ");
	ADD_APPLICATION_STRING2(STR_GLOBAL_RECEIVING, "Receiving","Receiving");
	ADD_APPLICATION_STRING2(STR_GLOBAL_DETAILS, "Details","Details");
#ifdef __MMI_FTC_SUPPORT__
	ADD_APPLICATION_STRING2(STR_FTP_DEV_BUSY, "Bluetooth Device Busy","Device Busy");
	ADD_APPLICATION_STRING2(STR_FTPC_NOTIFY_BROWSE_FILES_FAILED, "Set folder failed","Set folder failed");	
	ADD_APPLICATION_STRING2(STR_FTPC_NOTIFY_GOEP_STATUS_BUSY, "Bluetooth Device Busy","Device Busy");	
#endif
	ADD_APPLICATION_STRING2(STR_BT_CANCELLING, "Cancelling","Cancelling");

}

#endif  /* __MMI_OPP_SUPPORT__ */

#endif  /* __MMI_BT_SUPPORT__ */



#endif /* DEVELOPER_BUILD_FIRST_PASS */

