#include "cs_types.h"
#include "custdatares.h"
#ifndef RES_CCA_C
#define RES_CCA_C
#include "mmi_features.h"
#ifdef __MMI_CCA_SUPPORT__

#include "stdc.h"
#include "customcfg.h"
#include "centralconfigagentui.h"

#ifdef FIRST_PASS
#include "buildcfg.h"
#endif


#ifdef DEVELOPER_BUILD_FIRST_PASS
#include "populateres.h"


void PopulateCCARes(void)
{
	/*Add Strings*/
	ADD_APPLICATION_STRING2(STR_ID_CCA_PROGRESS_TITLE, "New Setting", "Configuration progressing title");
    ADD_APPLICATION_STRING2(STR_ID_CCA_PROGRESS_CONTENT, "Configuring system, please wait...", "Configuration progressing window content");
    ADD_APPLICATION_STRING2(STR_ID_CCA_PREINFO_TITLE, "New Setting", "Settings info title");
    ADD_APPLICATION_STRING2(STR_ID_CCA_PREINFO_CONTENT, "Settings for:", "Settings info window content");
    ADD_APPLICATION_STRING2(STR_ID_CCA_POSTINFO_TITLE, "Summary Report", "Summary report title");

    ADD_APPLICATION_STRING2(STR_ID_CCA_CONFIGURED, "Configured", "Configured");
    ADD_APPLICATION_STRING2(STR_ID_CCA_INVALID_SETTING, "Invalid Setting", "Invalid Setting");
    ADD_APPLICATION_STRING2(STR_ID_CCA_SETTING_SKIPPED, "Skipped", "Setting Skipped");

    //ADD_APPLICATION_STRING2(STR_ID_CCA_POSTINFO_CONTENT_SUCCEED, "Summary report content (succeed)", "Configuration complete.");
    //ADD_APPLICATION_STRING2(STR_ID_CCA_POSTINFO_CONTENT_FAIL, "Summary report content (fail)", "Configuration failed.");
    //ADD_APPLICATION_STRING2(STR_ID_CCA_POSTINFO_CONTENT_SUCCEEDwError, "Summary report content (succeed with error)", "Configuration complete with error.");

    ADD_APPLICATION_STRING2(STR_ID_CCA_DTACCT, "Data Account", "Data Account application name");
    ADD_APPLICATION_STRING2(STR_ID_CCA_BROWSER, "Browser", "Browser application name");
    ADD_APPLICATION_STRING2(STR_ID_CCA_MMS, "MMS", "MMS application name");
    ADD_APPLICATION_STRING2(STR_ID_CCA_SIP, "SIP", "SIP application name");
    ADD_APPLICATION_STRING2(STR_ID_CCA_WLAN, "Wireless LAN", "WLAN application name");
    ADD_APPLICATION_STRING2(STR_ID_CCA_FOTA, "Firmware Upgrade", "FOTA application name");

    ADD_APPLICATION_STRING2(STR_ID_CCA_DISCARD, "Discard", "Discard");
    ADD_APPLICATION_STRING2(STR_ID_CCA_MEMFULL, "Memory Full", "Memory Full");
    ADD_APPLICATION_STRING2(STR_ID_CCA_PROFILE, "Profile", "Profile");


	/*Add Image*/
	//ADD_APPLICATION_IMAGE2(IMG_ID_VRSI_APP,CUST_IMG_PATH"/MainLCD/VoiceRecg/SB_Speak.pbm","Main Icon for Voice Command App");
}


#endif /*END #ifdef DEVELOPER_BUILD_FIRST_PASS*/
#endif /*END #ifdef __MMI_CCA_SUPPORT__ */
#endif /*END #ifndef RES_CCA_C*/
