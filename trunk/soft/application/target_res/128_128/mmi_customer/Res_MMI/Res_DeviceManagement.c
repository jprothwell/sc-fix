/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2006
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

/**************************************************************

	FILENAME	: Res_DeviceManagement.c

  	PURPOSE		: Populate Resource for 

 

 

	DATE		: April-10-2006

**************************************************************/
#include "cs_types.h"
#include "custdatares.h"
#include "mmi_features.h"

#include "stdc.h"
#include "customcfg.h"


#ifdef FIRST_PASS
#include "buildcfg.h"
#endif

//#if defined(__MMI_DEVICE_MANAGEMENT__)

//#include "DeviceManagementDef.h"

#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"


/*********************************
 ** DM FWU Menu Enum
 **********************************/
typedef enum{

#if defined(__MMI_FWU_VIA_TFTP__)
   MMI_DM_FWU_TFTP_MENUCNT_ENUM,             /* TFTP Support */
#endif

#if defined(__MMI_FWU_VIA_HTTP__)
	MMI_DM_FWU_HTTP_MENUCNT_ENUM,            /* HTTP Support */
#endif	

#if defined(__MMI_FWU_VIA_FILE__)
    MMI_DM_FWU_FILE_MENUCNT_ENUM,            /* File */
#endif

	MMI_EM_MENUCNT_NUM
} MMI_DM_FWU_MENUCNT_ENUM;



/**************************************************************

	FUNCTION NAME		: PopulateDMMenuRes()

  	PURPOSE				: Populate the Device Management Menu Res

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

**************************************************************/
void PopulateDMMenuRes(void)
{

    /************************ Menu Items ************************/
	#if 0
	ADD_APPLICATION_MENUITEM((SERVICES_FWU_MENU_ID, MAIN_MENU_SERVICES_MENUID, MMI_EM_MENUCNT_NUM,
    #if defined(__MMI_FWU_VIA_TFTP__)	
		FWU_TFTP_MENU_ID, 
    #endif 		
		FWU_HTTP_MENU_ID, 
		FWU_FILE_MENU_ID,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_DM_FWU, IMG_ID_DM_FWU));
#endif
#if defined(__MMI_FWU_VIA_TFTP__)   
    /* From TFTP */
	ADD_APPLICATION_MENUITEM((FWU_TFTP_MENU_ID, SERVICES_FWU_MENU_ID, 2,
		FWU_TFTP_UPGRADE_MENU_ID, 
		FWU_TFTP_SETTING_MENU_ID,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_DM_FWU_TFTP, IMG_ID_DM_FWU_FROM_TFTP));

	ADD_APPLICATION_MENUITEM((FWU_TFTP_UPGRADE_MENU_ID, FWU_TFTP_MENU_ID, 0,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_DM_DOWNLOAD_NOW, IMG_GLOBAL_L1));

	ADD_APPLICATION_MENUITEM((FWU_TFTP_SETTING_MENU_ID, FWU_TFTP_MENU_ID, 0,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_DM_SETTINGS, IMG_GLOBAL_L2));
#endif /* #if defined(__MMI_FWU_VIA_TFTP__)	*/

#if defined(__MMI_FWU_VIA_HTTP__)
    /* From HTTP */
	ADD_APPLICATION_MENUITEM((FWU_HTTP_MENU_ID, SERVICES_FWU_MENU_ID, 2,
		FWU_HTTP_UPGRADE_MENU_ID, 
		FWU_HTTP_SETTING_MENU_ID,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_DM_FWU_HTTP, IMG_ID_DM_FWU_FROM_HTTP));

	ADD_APPLICATION_MENUITEM((FWU_HTTP_UPGRADE_MENU_ID, FWU_HTTP_MENU_ID, 0,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_DM_DOWNLOAD_NOW, IMG_GLOBAL_L1));

	ADD_APPLICATION_MENUITEM((FWU_HTTP_SETTING_MENU_ID, FWU_HTTP_MENU_ID, 0,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_DM_SETTINGS, IMG_GLOBAL_L2));
#endif /* #if defined(__MMI_FWU_VIA_HTTP__) */

#if defined(__MMI_FWU_VIA_FILE__)
    /* From File */
	ADD_APPLICATION_MENUITEM((FWU_FILE_MENU_ID, SERVICES_FWU_MENU_ID, 0,
		SHOW, NONMOVEABLE, DISP_LIST, STR_ID_DM_FWU_FILE, IMG_ID_DM_FWU_FROM_FILE));
#endif /*__MMI_FWU_VIA_FILE__*/

    /************************ Strings ************************/
	//ADD_APPLICATION_STRING2(STR_ID_DM_FWU ,"Firmware Upgrade","Title String for Firmware Upgrade main screen");

#if defined(__MMI_FWU_VIA_TFTP__)
	ADD_APPLICATION_STRING2(STR_ID_DM_FWU_TFTP, "From TFTP", "Title string for From TFTP screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_DM_SERVER_NAME, "Server Name", "Server name of TFTP");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_DM_PORT, "Port", "Port of TFTP");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_DM_FILENAME, "File Name", "Filename of TFTP");
#endif /* #if defined(__MMI_FWU_VIA_TFTP__) */

#if defined(__MMI_FWU_VIA_HTTP__)
	ADD_APPLICATION_STRING2(STR_ID_DM_FWU_HTTP, "From HTTP", "Title string for From HTTP screen");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_DM_URL, "URL", "URL of HTTP");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_DM_DEFAULT_URL, "http://", "Default URL");
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_DM_HTTP_NETWORK, "HTTP Network", "Http network");    
//del by reslim v0.1    ADD_APPLICATION_STRING2(STR_ID_DM_HTTP_DEFAULT, "Default", "Default");        
#endif /* #if defined(__MMI_FWU_VIA_HTTP__) */
#if 0
	ADD_APPLICATION_STRING2(STR_ID_DM_FWU_FILE, "From File", "Title string for From File screen");
	ADD_APPLICATION_STRING2(STR_ID_DM_DOWNLOAD_NOW, "Download Now", "Do firmware upgrade");
	ADD_APPLICATION_STRING2(STR_ID_DM_SETTINGS, "Settings", "Firmware upgrade settings");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_EDIT, "Edit", "To edit focused item");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_SELECT, "Select", "To select focused item");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_DOWNLOADING, "Downloading", "Upgrading");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_DOWNLOADING_WARNING, "This function currently not allow", "Upgrading");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_UPGRADE_NOW, "Done!\n\nUpgrade Now?", "Upgrading");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_SHUTDOWN_NOTIFY, "Shutdown for Upgrade", "Upgrading");

//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_RESULT_FILE_NOT_FOUND, "File Not Found", "Upgrading");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_RESULT_ACCESS_DENIED, "Access Denied", "Upgrading");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_RESULT_SOC_ERROR, "Network Error", "Upgrading");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_RESULT_TIMEOUT, "Time Out", "Upgrading");
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_RESULT_AUTH_FAIL, "Authentication Fail", "Upgrading");    
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_RESULT_FINAL_CHECK_FAIL, "Checksum Fail", "Upgrading");    

//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_DOWNLOAD_RETRY, "Retry?", "Retry");    
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_EMPTY_USERNAME, "Empty Username", "Empty Username");    

//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_EMPTY_SERVERNAME, "Empty Servername", "Empty Server name");    
//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_EMPTY_FILERNAME, "Empty Filename", "Empty File name");    

//del by reslim v0.1	ADD_APPLICATION_STRING2(STR_ID_DM_INVALID_PORT, "Invalid Port", "Invalid Port");    

    /************************ Images ************************/
	ADD_APPLICATION_IMAGE2(IMG_ID_DM_FWU, CUST_IMG_PATH"/MainLCD/Submenu/Services/FOTA/FW_Update.pbm","IMG_FIRMWARE_UPGRADE submenu");
    
        ADD_APPLICATION_IMAGE2(IMG_ID_DM_FWU, CUST_IMG_PATH"/MainLCD/Submenu/Services/FOTA/FW_wap.pbm","IMG_ID_DM_FWU_FROM_TFTP submenu");
        ADD_APPLICATION_IMAGE2(IMG_ID_DM_FWU, CUST_IMG_PATH"/MainLCD/Submenu/Services/FOTA/FW_URL.pbm","IMG_ID_DM_FWU_FROM_HTTP submenu");
        ADD_APPLICATION_IMAGE2(IMG_ID_DM_FWU, CUST_IMG_PATH"/MainLCD/Submenu/Services/FOTA/FW_folder.pbm","IMG_ID_DM_FWU_FROM_FILE submenu");
		#endif
}

#endif  /* DEVELOPER_BUILD_FIRST_PASS */

//#endif  /* __MMI_DEVICE_MANAGEMENT__ */

