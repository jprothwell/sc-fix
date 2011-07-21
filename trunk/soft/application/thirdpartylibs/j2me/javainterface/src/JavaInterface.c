#ifdef __J2ME__
#include "mmi_features.h"
#include "wgui_categories.h"
#include "globalconstants.h"    /* need MAX_SUB_MENUS */
#include "historygprot.h"
#include "commonscreens.h"
#include "debuginitdef.h"
#include "fat_fs.h"
#include "filemanagergprot.h"			/* file path / file error  */
#include "filemanagerdef.h"	  		/* file error text */
#include "filemgr.h"
#include "vdoplystrenum.h"
#include "settingdefs.h"
#include "settingprofile.h"
#include "funandgamesdefs.h"
#include "ucs2prot.h"
#include "mdi_audio.h"
#include "javaprot.h"
#include "javadef.h"
#include "jam_msg_handler.h"
#include "javacall_lifecycle.h"
#include "javacall_keypress.h"
#include "javacall_penevent.h"
#include "javacall_events.h"
#include "custom_mmi_default_value.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "dual_sim.h"
#include "adp_events.h"
#include "mmiapi.h"
#include "wgui_status_icons.h"
#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif

//Option
#define J2ME_SUPPORT_BACKGROUND
#define J2ME_SUPPORT_PAUSE
//Option


typedef enum
{
    JVM_LIFECYCLE_START,
    JVM_LIFECYCLE_PAUSE,
    JVM_LIFECYCLE_RESUME,
    JVM_LIFECYCLE_SWITCH_BACKGROUND,
    JVM_LIFECYCLE_SWITCH_FOREGROUND,
    JVM_LIFECYCLE_SHUTDOWN
} jvm_lifecycle_notify;

U8 g_mmi_java_aud_volume_level = 0xff;  /* 0~6 */
U8 g_mmi_java_is_vib_on = MMI_FALSE;        /* on-off */
U8 g_mmi_java_wap_prof_id = 0;
#ifdef __MMI_MULTI_SIM__
U8 g_mmi_java_sim_id = 0;
#endif

#define JAVA_ERR_SUCCEED            (0)
#define JAVA_ERR_CORRUPT_FAT		(-1)
#define JAVA_ERR_PHONE_DISK_FULL	(-2)
#define JAVA_ERR_CARD_DISK_FULL	    (-3)
#define JAVA_ERR_WRITE_PROTECT		(-4)
#define JAVA_ERR_ROOT_DIR_FULL		(-5)
#define JAVA_ERR_NO_DISK            (-2004)

jvm_state_enum g_jam_vm_state = JVM_TERMINATE_STATE;
gdi_handle JvmBaseLayer = GDI_LAYER_EMPTY_HANDLE;;
java_context_struct g_java;
unsigned short LifecycleSemaphore = 0;
U32 g_jam_lock_screen = FALSE;

extern void javanotify_install_midlet_wparams(const char* httpUrl,
                                       int silentInstall, int forceUpdate);
extern int checkUsbstate(void);
extern VOID mmi_display_usb_busy(void);
extern BOOL g_hasMountedTFlashFS;
extern BOOL g_hasMountedNandFlashFS;

extern int _active_socket_count;
extern BOOL j2me_socket_io_activated;



void jvm_lifecycle_nofify(jvm_lifecycle_notify state)
{
    mmi_trace(1,"jvm_lifecycle_nofify state=%d",state);
    
    if(state == JVM_LIFECYCLE_START)
    {
        jvm_start_java_req();
    }
    else if(state == JVM_LIFECYCLE_PAUSE)
    {
        g_jam_lock_screen = TRUE;
        javanotify_pause();
    }
    else if(state == JVM_LIFECYCLE_RESUME)
    {
        javanotify_resume();
    }
    else if(state == JVM_LIFECYCLE_SHUTDOWN)
    {
        javanotify_shutdown();
    }
    else if(state == JVM_LIFECYCLE_SWITCH_BACKGROUND)
    {
        g_jam_vm_state = JVM_BG_RUNNING_STATE;
        g_jam_lock_screen = TRUE;
        return;
    }
    else if(state == JVM_LIFECYCLE_SWITCH_FOREGROUND)
    {
        g_jam_vm_state = JVM_FG_RUNNING_STATE;
        g_jam_lock_screen = FALSE;
        javanotify_select_foreground_app();
        return;
    }
    
    LifecycleSemaphore = sxr_NewSemaphore(0);
	sxr_TakeSemaphore(LifecycleSemaphore);
    sxr_FreeSemaphore(LifecycleSemaphore);
    LifecycleSemaphore = 0;
    
}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_send_ilm
 * DESCRIPTION
 *  send message
 * PARAMETERS
 *
 * RETURNS
 *
 *****************************************************************************/

void mmi_java_send_message(S32 src_id, S32 dst_id, S32 sap_id, S32 msg_id, void *local_para_ptr, void *peer_buff_ptr)
{

    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    MYQUEUE Message;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    Message.oslSrcId = src_id;
    Message.oslDestId = dst_id;
    Message.oslMsgId = msg_id;
    Message.oslDataPtr = (local_para_struct *) local_para_ptr;
    Message.oslPeerBuffPtr = (peer_buff_struct *) peer_buff_ptr;
    Message.oslSapId = sap_id;

    OslMsgSendExtQueue(&Message);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_is_no_network_service
 * DESCRIPTION
 *  This function is to detect if network is available or not
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL mmi_java_is_no_network_service(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

#ifdef  __MMI_MULTI_SIM__
	if(MTPNP_AD_Get_UsableSide_Number() > 0)
    {
        return MMI_FALSE;
    }
    else
    {
        return MMI_TRUE;
    }
#else
    if (mmi_bootup_is_sim_valid() == MMI_TRUE &&
        mmi_bootup_get_active_flight_mode() == MMI_FALSE)
    {
        return MMI_FALSE;
    }
    else
    {
        return MMI_TRUE;
    }
#endif
}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_store_setting
 * DESCRIPTION
 *  store java setting back to NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_store_setting(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    WriteValue(NVRAM_JAVA_SETTING_SOUND, &g_mmi_java_aud_volume_level, DS_BYTE, &error);
    WriteValue(NVRAM_JAVA_SETTING_VIB, &g_mmi_java_is_vib_on, DS_BYTE, &error);
#if 0//JiaShuo Delete for J2ME
#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    WriteValue(NVRAM_JAVA_SETTING_VK, &g_mmi_java_vk_permission, DS_BYTE, &error);
#endif 
#endif
#ifdef __MMI_MULTI_SIM__
    WriteValue(NVRAM_JAVA_SETTING_SIM_ID, &g_mmi_java_sim_id, DS_BYTE, &error);
#endif
    WriteValue(NVRAM_JAVA_SETTING_PROF_ID, &g_mmi_java_wap_prof_id, DS_BYTE, &error);
}


/*****************************************************************************
 * FUNCTION
 *  mmi_java_load_setting
 * DESCRIPTION
 *  load java setting from NVRAM
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_load_setting(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 error;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ReadValue(NVRAM_JAVA_SETTING_SOUND, &g_mmi_java_aud_volume_level, DS_BYTE, &error);

    /* if not init, init as 4 */
    if (g_mmi_java_aud_volume_level == 0xff)
    {
        g_mmi_java_aud_volume_level = 4;
        WriteValue(NVRAM_JAVA_SETTING_SOUND, &g_mmi_java_aud_volume_level, DS_BYTE, &error);
    }

    ReadValue(NVRAM_JAVA_SETTING_VIB, &g_mmi_java_is_vib_on, DS_BYTE, &error);

    /* if not init, init as ON */
    if (g_mmi_java_is_vib_on == 0xff)
    {
        g_mmi_java_is_vib_on = TRUE;
        WriteValue(NVRAM_JAVA_SETTING_VIB, &g_mmi_java_is_vib_on, DS_BYTE, &error);
    }

#if 0//JiaShuo Delete for J2ME
#ifdef __MMI_FOURWAYS_KEY_DISABLE__
    ReadValue(NVRAM_JAVA_SETTING_VK, &g_mmi_java_vk_permission, DS_BYTE, &error);

    /* if not init, init as first wap profile */
    if (g_mmi_java_vk_permission == 0xff)
    {
        g_mmi_java_vk_permission = 0;
        WriteValue(NVRAM_JAVA_SETTING_VK, &g_mmi_java_vk_permission, DS_BYTE, &error);
    }
#endif /* __MMI_FOURWAYS_KEY_DISABLE__ */ 
#endif

    ReadValue(NVRAM_JAVA_SETTING_PROF_ID, &g_mmi_java_wap_prof_id, DS_BYTE, &error);
    if (g_mmi_java_wap_prof_id == 0xff)
    {
        g_mmi_java_wap_prof_id = 0;
        WriteValue(NVRAM_JAVA_SETTING_PROF_ID, &g_mmi_java_wap_prof_id, DS_BYTE, &error);
    }

#ifdef __MMI_MULTI_SIM__
    ReadValue(NVRAM_JAVA_SETTING_SIM_ID, &g_mmi_java_sim_id, DS_BYTE, &error);

    if (g_mmi_java_sim_id == 0xff)
    {
        g_mmi_java_sim_id = MTPNP_AD_GET_UsableSide_Index();
    }
    else
    {
        if(!MTPNP_PFAL_Is_Card_Usable(g_mmi_java_sim_id))
        {
            g_mmi_java_sim_id = MTPNP_AD_GET_UsableSide_Index();
        }
    }
    
    WriteValue(NVRAM_JAVA_SETTING_SIM_ID, &g_mmi_java_sim_id, DS_BYTE, &error);
#endif

}



static BOOL mmi_java_check_valid_drv(void)
{
    S32 ret_drv;
    
	/* path is memory card */
	ret_drv 	= MMI_FS_GetDrive(FS_DRIVE_V_REMOVABLE, 1, FS_NO_ALT_DRIVE);
    
    if(ret_drv >= 0)
	{
#if defined(MMI_ON_WIN32)
		return TRUE;
#endif
		if('d' == ret_drv || 'D' == ret_drv)
		{
			return g_hasMountedTFlashFS;
		}
		if('e' == ret_drv || 'E' == ret_drv)
		{
			return g_hasMountedNandFlashFS;
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}
	

	return TRUE;
}

static S32 mmi_java_create_system_folder(S8* filepath)
{
   /*----------------------------------------------------------------*/
   /* Local Variables                                                */
   /*----------------------------------------------------------------*/
    S8              buf_createpath[128];
    S8              buf_filepath[FMGR_PATH_BUFFER_SIZE],tmp[256];
    S32         fs_ret = -1;
    S32         str_len;
    S32         cmp_ret;
    S32         i;
    FS_HANDLE   file_handle= FS_ERROR_RESERVED;
    S8 slash_tmp[10];

   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    file_handle = MMI_FS_Open((const UINT8 *)filepath, FS_OPEN_DIR | FS_READ_ONLY);

    /* path already exist */
    //if(file_handle== FS_FOLDER_HANDLE)
    if(file_handle>=0)
    {
        MMI_FS_Close(file_handle);
        return 0; /* already exist, 0 means successful in file system */
    }
    
    /* create directory if it is not there */
    pfnUnicodeStrcpy((PS8)buf_filepath, (PS8)filepath);


    str_len = pfnUnicodeStrlen((PS8)buf_filepath);
    str_len *= ENCODING_LENGTH;
    AnsiiToUnicodeString(slash_tmp,(PS8)"/");

    /* skip drive. "C:\" */
    for(i = 3*ENCODING_LENGTH ; i < str_len; i+=2)
    {
        cmp_ret = pfnUnicodeStrncmp((PS8)&buf_filepath[i], slash_tmp, 1);
        if(cmp_ret == 0)
        {
            /* create dir */

            pfnUnicodeStrncpy((PS8)buf_createpath, (PS8)buf_filepath, i/ENCODING_LENGTH);
            buf_createpath[i]       = '\0';
            buf_createpath[i+1] = '\0';
            fs_ret = MMI_FS_CreateDir((const UINT8 *)buf_createpath);   
            UnicodeToAnsii(tmp,buf_createpath);
            mmi_trace(TRUE,"mmi_java_create_system_folder,fs_ret is %d,buf_createpath is %s",fs_ret,tmp);
            if(fs_ret < 0)
                return fs_ret;
        }
    }

    return fs_ret;
}

static void mmi_java_disply_popup(PU8 str_ptr,
												U16 img_id, 
												U16 image_on_bottom, 
												U16 tone_duration, 
												U16 tone_id)
{
	DisplayPopup((PU8)str_ptr, (U16)img_id, (U8)image_on_bottom, (U32)tone_duration, (U8)tone_id);
}

static BOOL mmi_java_check_and_display_error(void)
{
   /*----------------------------------------------------------------*/
   /* Code Body                                                      */
   /*----------------------------------------------------------------*/
    if( g_java.last_error != JAVA_ERR_SUCCEED)
    {
        ClearAllKeyHandler();
        switch(g_java.last_error)
        {
        case JAVA_ERR_NO_DISK:
            mmi_java_disply_popup((PU8)GetString(STR_ID_VDOPLY_NOTIFY_NO_MEMORY_CARD), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
            break;

        case JAVA_ERR_CORRUPT_FAT:
            mmi_java_disply_popup((PU8)GetString(FMGR_FS_FAT_ALLOC_ERROR_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
            break;

        case JAVA_ERR_PHONE_DISK_FULL:
            mmi_java_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
            break;  

        case JAVA_ERR_WRITE_PROTECT:
            mmi_java_disply_popup((PU8)GetString(FMGR_FS_WRITE_PROTECTION_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
            break;          

        case JAVA_ERR_CARD_DISK_FULL:
            mmi_java_disply_popup((PU8)GetString(FMGR_FS_DISK_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
            break;
            
        case JAVA_ERR_ROOT_DIR_FULL:
            mmi_java_disply_popup((PU8)GetString(FMGR_FS_ROOT_DIR_FULL_TEXT), IMG_GLOBAL_WARNING, 1, ST_NOTIFYDURATION, WARNING_TONE);
            break;

        default:    
            MMI_ASSERT(0); /* use assert here, make sure all error case is handled properly */
        }

        g_java.last_error = JAVA_ERR_SUCCEED;
        return TRUE;
    }
    else 
        return FALSE;
}

#ifdef J2ME_SUPPORT_BACKGROUND
/*****************************************************************************
 * FUNCTION
 *  mmi_java_highlight_handler
 * DESCRIPTION
 *  Handle Function: revoked when selected item changed
 * PARAMETERS
 *  index       [IN]        Index of selected item
 * RETURNS
 *  void
 *****************************************************************************/
static S32 mmi_java_highlight_index;

static void mmi_java_highlight_handler(S32 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_java_highlight_index = index;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_entry_terminate_dialog_exit
 * DESCRIPTION
 *
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_java_entry_terminate_dialog_exit(void)
{
#if 0//JiaShuo Delete for J2ME
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    free_asking_string();
#endif
}

/*****************************************************************************
 * FUNCTION
 *  is_sms_activated
 * DESCRIPTION
 *  check if there is any pending sms requests
 * PARAMETERS
 *  void
 * RETURNS
 *  TRUE if there is pending sms request, FALSE otherwise.
 *****************************************************************************/
kal_bool is_sms_activated(void)
{
#if 0//JiaShuo Modify for J2ME
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int state = jwa_sms_get_state();

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (state == JWA_SMS_STATE_SEND ||
        state == JWA_SMS_STATE_GET_INCOMING_MSG ||
        state == JWA_SMS_STATE_REGISTER_PORT)
    {
        sprintf(_kvmLogStr,"is_sms_activated: TRUE state= %d\n", state);
        Kprintf();  
        
        return TRUE;
    }
    else
    {
        sprintf(_kvmLogStr,"is_sms_activated: FALSE state= %d\n", state);
        Kprintf();  
    
        return FALSE;
    }
#else
    return FALSE;
#endif
}

/*****************************************************************************
 * FUNCTION
 *  jam_vm_canbe_minimize
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
kal_bool jam_vm_canbe_minimize(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_bool canbe_minimize = KAL_TRUE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef J2ME_SUPPORT_BACKGROUND

    if (is_sms_activated()
#ifdef SUPPORT_JSR_205
        || is_mms_activated()
#endif 
#ifdef SUPPORT_JSR_184
        || is_graphics3d_activated()
#endif 
#ifdef __SUPPORT_JBT__
        || (jbt_get_power_status_cmd() != JBT_POWER_OFF)
#endif 
        )
    {
        canbe_minimize = KAL_FALSE;
    }
#else /* J2ME_SUPPORT_BACKGROUND */ 
    canbe_minimize = KAL_FALSE;
#endif /* J2ME_SUPPORT_BACKGROUND */ 

    return canbe_minimize;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_terminate_dialog_yes_hdlr
 * DESCRIPTION
 *  Handler Function:   revoked when select "YES" in "TERMINATE" dialog
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_java_terminate_dialog_without_pause_yes_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (mmi_java_highlight_index == 0)  /* minimize */
    {
#if 0//JiaShuo Delete for J2ME        
        jam_enter_minimize_mode(KAL_FALSE);
        DeleteScreenIfPresent(SCR_JAVA_APP_MIDS_OPTION);
        DeleteScreenIfPresent(SCR_JAVA_APP_OPTION_SELECT);
#endif
        DeleteScreenIfPresent(SCR_JAVA);
    }
    else
    {
#if 0//JiaShuo Modify for J2ME
        jam_enter_terminate_mode();
        mmi_java_entry_terminating(MMI_FALSE);
#else
        if(g_jam_vm_state == JVM_PAUSED_STATE)
        {
            jvm_lifecycle_nofify(JVM_LIFECYCLE_RESUME);
        }
        jvm_lifecycle_nofify(JVM_LIFECYCLE_SHUTDOWN);
#endif
    }

    if(GetActiveScreenId() == SCR_JAVA_TERMINATE_DIALOG)
    {
        GoBackHistory();
    }
    else
    {
        DeleteScreenIfPresent(SCR_JAVA_TERMINATE_DIALOG);
    }

    if (mmi_java_highlight_index == 0)
    {
        ShowStatusIcon(STATUS_ICON_JAVA_PAUSE);
        UpdateStatusIcons();
    }
    else
    {
		HideStatusIcon(STATUS_ICON_JAVA_PAUSE);
        UpdateStatusIcons();
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_java_terminate_dialog_no_hdlr
 * DESCRIPTION
 *  Handler Function:   revoked when select "NO" in "TERMINATE" dialog
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_terminate_dialog_without_pause_no_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GoBackHistory();
}


/*****************************************************************************
 * FUNCTION
 *  mmi_java_entry_terminate_dialog
 * DESCRIPTION
 *  The entry function to ask user if they want to terminate/Pause Java
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_entry_terminate_dialog_without_pause(void)  /* called by JAVA adaptor, also in mmi task */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U8 *icon_items[2];
    U8 *text_items[2];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(1,"mmi_java_entry_terminate_dialog_without_pause");
    
    EntryNewScreen(
        SCR_JAVA_TERMINATE_DIALOG,
        mmi_java_entry_terminate_dialog_exit,
        mmi_java_entry_terminate_dialog_without_pause,
        NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_JAVA_TERMINATE_DIALOG);

    RegisterHighlightHandler(mmi_java_highlight_handler);

    memset(icon_items, 0, sizeof(icon_items));

    text_items[0] = (PU8) GetString(STR_JAVA_MINIMIZE);
    text_items[1] = (PU8) GetString(STR_JAVA_TERMINATE);

    ShowCategory174Screen(
        GetString(STR_MENU3108_JAVA),//format_asking_string(STR_JAVA_CHANGE_STATE_TITLE),
        NULL,
        (PU8) GetString(STR_GLOBAL_OK),
        (PU8) GetImage(IMG_GLOBAL_OK),
        (PU8) GetString(STR_GLOBAL_BACK),
        (PU8) GetImage(IMG_GLOBAL_BACK),
        2,
        text_items,
        icon_items,
        NULL,
        0,
        0,
        guiBuffer);

    SetLeftSoftkeyFunction(mmi_java_terminate_dialog_without_pause_yes_hdlr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(mmi_java_terminate_dialog_without_pause_no_hdlr, KEY_EVENT_UP);
}

#endif

#if defined(J2ME_SUPPORT_PAUSE) || defined(J2ME_SUPPORT_BACKGROUND)
/*****************************************************************************
 * FUNCTION
 *  is_socket_activated
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool is_socket_activated(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    mmi_trace(1,"is_socket_activated--> _active_socket_count=%d j2me_socket_io_activated=%d",_active_socket_count,j2me_socket_io_activated);

    if (_active_socket_count != 0 || j2me_socket_io_activated)
    {
        return KAL_TRUE;
    }
    else
    {
        return KAL_FALSE;
    }
}

/*****************************************************************************
 * FUNCTION
 *  is_datagram_activated
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  kal_bool
 *****************************************************************************/
kal_bool is_datagram_activated(void)
{
#if 0//JiaShuo Modify for J2ME
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (_active_datagram_count != 0)
    {
        return KAL_TRUE;
    }
    else
    {
        return KAL_FALSE;
    }
#else
    return KAL_FALSE;
#endif
}

/*****************************************************************************
 * FUNCTION
 *  is_stream_activated
 * DESCRIPTION
 *  get if there is any activated stream.
 * PARAMETERS
 *  void
 * RETURNS
 *  TRUE if some stream is activated, FALSE otherwise.
 *****************************************************************************/
kal_bool is_stream_activated(void)
{
#if 0//JiaShuo Modify for J2ME
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i = 0;
    kal_bool result = KAL_FALSE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    for (; i < J2ME_STREAM_MAX_NUMBER; i++)
    {
        if (j2me_stream[i].id != J2ME_STREAM_INVALID)
        {
            result = KAL_TRUE;
            break;
        }
    }
    
    //kal_trace(TRACE_GROUP_3, FUNC_J2ME_STREAM_IS_ACTIVATE, result);
    
    return result;
#else
    return KAL_FALSE;
#endif
}

/*****************************************************************************
 * FUNCTION
 *  jam_vm_canbe_paused
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
kal_bool jam_vm_canbe_paused(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_bool canbe_paused = KAL_TRUE;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef J2ME_SUPPORT_PAUSE
    if (is_socket_activated() || is_datagram_activated()
#ifdef SUPPORT_JSR_205
        || is_mms_activated()
#endif 
        || is_sms_activated() || is_stream_activated()
#ifdef SUPPORT_HTTP_OVER_WSP
        || is_http_activated(KAL_TRUE,0)
#endif 
#ifdef SUPPORT_JSR_184
        || is_graphics3d_activated()
#endif 
#ifdef __SUPPORT_JBT__
        || (jbt_get_power_status_cmd() != JBT_POWER_OFF)
#endif 
        )
    {
        canbe_paused = KAL_FALSE;
    }
#else /* J2ME_SUPPORT_PAUSE */ 
    canbe_paused = KAL_FALSE;
#endif /* J2ME_SUPPORT_PAUSE */ 

    return canbe_paused;
}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_terminate_dialog_yes_hdlr
 * DESCRIPTION
 *  Handler Function:   revoked when select "YES" in "TERMINATE" dialog
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_java_terminate_dialog_yes_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef J2ME_SUPPORT_BACKGROUND
    if (mmi_java_highlight_index == 0)  /* minimize */
    {
#if 0//JiaShuo Delete for J2ME
        jam_enter_minimize_mode(KAL_FALSE);
        DeleteScreenIfPresent(SCR_JAVA_APP_MIDS_OPTION);
        DeleteScreenIfPresent(SCR_JAVA_APP_OPTION_SELECT);
#endif
        DeleteScreenIfPresent(SCR_JAVA);
    }
    else if( mmi_java_highlight_index == 1)   /* pause */
    {
#if 0//JiaShuo Delete for J2ME
        jam_enter_pause_mode();
        DeleteScreenIfPresent(SCR_JAVA_APP_MIDS_OPTION);
        DeleteScreenIfPresent(SCR_JAVA_APP_OPTION_SELECT);
#endif
        DeleteScreenIfPresent(SCR_JAVA);
    }
    else
    {
#if 0//JiaShuo Delete for J2ME
        jam_enter_terminate_mode();
        mmi_java_entry_terminating(MMI_FALSE);
#endif
        jvm_lifecycle_nofify(JVM_LIFECYCLE_RESUME);
        jvm_lifecycle_nofify(JVM_LIFECYCLE_SHUTDOWN);
    }
#else
    if (mmi_java_highlight_index == 0)  /* pause */
    {
#if 0//JiaShuo Delete for J2ME
        jam_enter_pause_mode();
        DeleteScreenIfPresent(SCR_JAVA_APP_MIDS_OPTION);
        DeleteScreenIfPresent(SCR_JAVA_APP_OPTION_SELECT);
#endif
        DeleteScreenIfPresent(SCR_JAVA);
    }
    else /* terminate */
    {
#if 0//JiaShuo Delete for J2ME
        jam_enter_terminate_mode();
        mmi_java_entry_terminating(MMI_FALSE);
#endif
        jvm_lifecycle_nofify(JVM_LIFECYCLE_RESUME);
        jvm_lifecycle_nofify(JVM_LIFECYCLE_SHUTDOWN);
    }
#endif

    if(GetActiveScreenId() == SCR_JAVA_TERMINATE_DIALOG)
    {
        GoBackHistory();
    }
    else
    {
        DeleteScreenIfPresent(SCR_JAVA_TERMINATE_DIALOG);
    }
    
    if (mmi_java_highlight_index == 0 
        #ifdef J2ME_SUPPORT_BACKGROUND
        || mmi_java_highlight_index == 1
        #endif
        )
    {
        ShowStatusIcon(STATUS_ICON_JAVA_PAUSE);
        UpdateStatusIcons();
    }
    else
    {
        HideStatusIcon(STATUS_ICON_JAVA_PAUSE);
        UpdateStatusIcons();
    }

}


/*****************************************************************************
 * FUNCTION
 *  mmi_java_terminate_dialog_no_hdlr
 * DESCRIPTION
 *  Handler Function:   revoked when select "NO" in "TERMINATE" dialog
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_terminate_dialog_no_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GoBackHistory();
}
/*****************************************************************************
 * FUNCTION
 *  mmi_java_entry_terminate_dialog
 * DESCRIPTION
 *  The entry function to ask user if they want to terminate/Pause Java
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_entry_terminate_dialog(void)  /* called by JAVA adaptor, also in mmi task */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;
    U8 *icon_items[3];
    U8 *text_items[3];

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(
        SCR_JAVA_TERMINATE_DIALOG,
        mmi_java_entry_terminate_dialog_exit,
        mmi_java_entry_terminate_dialog,
        NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_JAVA_TERMINATE_DIALOG);

    RegisterHighlightHandler(mmi_java_highlight_handler);

    memset(icon_items, 0, sizeof(icon_items));

#ifdef J2ME_SUPPORT_BACKGROUND
    text_items[0] = (PU8) GetString(STR_JAVA_MINIMIZE);
    text_items[1] = (PU8) GetString(STR_JAVA_PAUSE);
    text_items[2] = (PU8) GetString(STR_JAVA_TERMINATE);
#else
    text_items[0] = (PU8) GetString(STR_JAVA_PAUSE);
    text_items[1] = (PU8) GetString(STR_JAVA_TERMINATE);
#endif

    ShowCategory174Screen(
        GetString(STR_MENU3108_JAVA),//format_asking_string(STR_JAVA_CHANGE_STATE_TITLE),
        NULL,
        (PU8) GetString(STR_GLOBAL_OK),
        (PU8) GetImage(IMG_GLOBAL_OK),
        (PU8) GetString(STR_GLOBAL_BACK),
        (PU8) GetImage(IMG_GLOBAL_BACK),
#ifdef J2ME_SUPPORT_BACKGROUND
        3,
#else
        2,
#endif
        text_items,
        icon_items,
        NULL,
        0,
        0,
        guiBuffer);

    SetLeftSoftkeyFunction(mmi_java_terminate_dialog_yes_hdlr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(mmi_java_terminate_dialog_no_hdlr, KEY_EVENT_UP);
}
#endif
/*****************************************************************************
 * FUNCTION
 *  mmi_java_force_terminate_dialog_yes_hdlr
 * DESCRIPTION
 *  Handle Function: revoked when select "YES" in "FORCE TERMINATE" dialog
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_java_force_terminate_dialog_yes_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#if 0//JiaShuo Delete for J2ME
    jam_enter_terminate_mode();

    mmi_java_entry_terminating(MMI_FALSE);
#endif
    mmi_trace(1,"mmi_java_force_terminate_dialog_yes_hdlr %d",g_jam_vm_state);

    jvm_lifecycle_nofify(JVM_LIFECYCLE_RESUME);
    jvm_lifecycle_nofify(JVM_LIFECYCLE_SHUTDOWN);

    if(GetActiveScreenId() == SCR_JAVA_FORCE_TERMINATE_DIALOG)
    {
        GoBackHistory();
    }
    else
    {
        DeleteScreenIfPresent(SCR_JAVA_FORCE_TERMINATE_DIALOG);
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_java_force_terminate_dialog_no_hdlr
 * DESCRIPTION
 *  Handle Function: revoked when select "NO" in "FORCE TERMINATE" dialog
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void mmi_java_force_terminate_dialog_no_hdlr(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GoBackHistory();
}

/*****************************************************************************
 * FUNCTION
 *  mmi_java_entry_force_terminate_dialog
 * DESCRIPTION
 *  The entry function to ask if user real want to terminate java? (can not pause)
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_java_entry_force_terminate_dialog(void)    /* called by JAVA adaptor, also in mmi task */
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 *guiBuffer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    EntryNewScreen(SCR_JAVA_FORCE_TERMINATE_DIALOG, NULL, mmi_java_entry_force_terminate_dialog, NULL);

    guiBuffer = GetCurrGuiBuffer(SCR_JAVA_FORCE_TERMINATE_DIALOG);

    ShowCategory74Screen(
        STR_JAVA_TERMINATE,
        NULL,//mmi_java_get_title_icon(),
        STR_GLOBAL_YES,
        IMG_GLOBAL_YES,
        STR_GLOBAL_NO,
        IMG_GLOBAL_NO,
        (PU8) GetString(STR_JAVA_FORCE_TERMINATE),
        UCS2Strlen(GetString(STR_JAVA_FORCE_TERMINATE)),
        guiBuffer);

    SetLeftSoftkeyFunction(mmi_java_force_terminate_dialog_yes_hdlr, KEY_EVENT_UP);
    SetRightSoftkeyFunction(mmi_java_force_terminate_dialog_no_hdlr, KEY_EVENT_UP);
}
/*****************************************************************************
 * FUNCTION
 *  jvm_keypad_collector
 * DESCRIPTION
 *  collect all key event, insert key event into pool buffer except
 *  END key
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void jvm_keypad_collector(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint16 Keycode, Keytype;
	javacall_keypress_type key_type;
	javacall_key key_code;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GetkeyInfo(&Keycode, &Keytype);

	switch (Keytype)
	{
		case KEY_EVENT_DOWN:
			key_type = JAVACALL_KEYPRESSED;
			break;
		case KEY_EVENT_UP:
			key_type = JAVACALL_KEYRELEASED;
			break;
		case KEY_REPEAT:
			key_type = JAVACALL_KEYREPEATED;
			break;
		case KEY_LONG_PRESS:
			key_type = JAVACALL_KEYPRESSED;
			break;
		case KEY_FULL_PRESS:
			key_type = JAVACALL_KEYPRESSED;
			break;
		default :
			key_type = JAVACALL_KEYTYPED;
			break;
	}
	switch (Keycode)
	{
		case KEY_0:
			key_code = JAVACALL_KEY_0;
			break;
		case KEY_1:
			key_code = JAVACALL_KEY_1;
			break;
		case KEY_2:
			key_code = JAVACALL_KEY_2;
			break;
		case KEY_3:
			key_code = JAVACALL_KEY_3;
			break;
		case KEY_4:
			key_code = JAVACALL_KEY_4;
			break;
		case KEY_5:
			key_code = JAVACALL_KEY_5;
			break;
		case KEY_6:
			key_code = JAVACALL_KEY_6;
			break;
		case KEY_7:
			key_code = JAVACALL_KEY_7;
			break;
		case KEY_8:
			key_code = JAVACALL_KEY_8;
			break;
		case KEY_9:
			key_code = JAVACALL_KEY_9;
			break;
		case KEY_LSK:
			key_code = JAVACALL_KEY_SOFT1;
			break;
		case KEY_RSK:
			key_code = JAVACALL_KEY_SOFT2;
			break;
		case KEY_CSK:
			key_code = JAVACALL_KEY_SELECT;
			break;
		case KEY_UP_ARROW:
			key_code = JAVACALL_KEY_UP;
			break;
		case KEY_DOWN_ARROW:
			key_code = JAVACALL_KEY_DOWN;
			break;
		case KEY_LEFT_ARROW:
			key_code = JAVACALL_KEY_LEFT;
			break;
		case KEY_RIGHT_ARROW:
			key_code = JAVACALL_KEY_RIGHT;
			break;
		case KEY_SEND:
			key_code = JAVACALL_KEY_SEND;
			break;
		case KEY_END:
            if (Keytype == KEY_EVENT_DOWN)
            {
#if defined(J2ME_SUPPORT_PAUSE) || defined(J2ME_SUPPORT_BACKGROUND)
                if (jam_vm_canbe_minimize())
                {
                    mmi_java_entry_terminate_dialog_without_pause();
                }
                else
                {
                    mmi_java_entry_force_terminate_dialog();
                }
                return;
#else
                mmi_java_entry_force_terminate_dialog();
                return;
#endif
            }
            else
            {
                return;
            }
		case KEY_CLEAR:
			key_code = JAVACALL_KEY_CLEAR;
			break;
		case KEY_STAR:
			key_code = JAVACALL_KEY_ASTERISK;
			break;
		case KEY_POUND:
			key_code = JAVACALL_KEY_POUND;
			break;
		case KEY_ENTER:
			key_code = JAVACALL_KEY_SELECT;
			break;
		default :
			mmi_trace(1,"current Coskeycode = %d\n",Keycode);
			key_code = Keycode;
			break;
	}
	javanotify_key_event(key_code, key_type);
    
}

/*****************************************************************************
 * FUNCTION
 *  jvm_register_keypad
 * DESCRIPTION
 *  this function will register keypad handler to JVM_redirect_keyevent() function.
 * CALLS
 *  
 * PARAMETERS
 *  is_resume       [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void jvm_register_keypad(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 keyCode = 0;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearAllKeyHandler();
    clear_left_softkey();
    clear_right_softkey();
    register_default_hide_softkeys();

    for (keyCode = 0; keyCode < MAX_KEYS; keyCode++)
    {
        SetKeyHandler((FuncPtr) jvm_keypad_collector, keyCode, KEY_EVENT_UP);
        SetKeyHandler((FuncPtr) jvm_keypad_collector, keyCode, KEY_EVENT_DOWN);
        SetKeyHandler((FuncPtr) jvm_keypad_collector, keyCode, KEY_EVENT_REPEAT);
        SetKeyHandler((FuncPtr) jvm_keypad_collector, keyCode, KEY_EVENT_LONG_PRESS);
    }
}

void jvm_unregister_keypad(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ClearAllKeyHandler();
    clear_left_softkey();
    clear_right_softkey();
    register_default_hide_softkeys();
}

#ifdef __MMI_TOUCH_SCREEN__
static void jvm_pen_down_hdlr(mmi_pen_point_struct pos)
{
    mmi_trace(1,"jvm_pen_down_hdlr x=%d y=%d",pos.x,pos.y);
    javanotify_pen_event(pos.x, pos.y, JAVACALL_PENPRESSED);
}

static void jvm_pen_up_hdlr(mmi_pen_point_struct pos)
{
    mmi_trace(1,"jvm_pen_up_hdlr x=%d y=%d",pos.x,pos.y);
    javanotify_pen_event(pos.x, pos.y, JAVACALL_PENRELEASED);
}

static void jvm_pen_move_hdlr(mmi_pen_point_struct pos)
{
    mmi_trace(1,"jvm_pen_move_hdlr x=%d y=%d",pos.x,pos.y);
    javanotify_pen_event(pos.x, pos.y, JAVACALL_PENDRAGGED);
}


void jvm_register_pen_events(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    //jvm_tp_flush();
    wgui_register_pen_down_handler(jvm_pen_down_hdlr);
    wgui_register_pen_up_handler(jvm_pen_up_hdlr);
    wgui_register_pen_move_handler(jvm_pen_move_hdlr);
    //wgui_register_pen_long_tap_handler(jvm_pen_long_tap_hdlr);
    //wgui_register_pen_repeat_handler(jvm_pen_repeat_hdlr);
    //wgui_register_pen_abort_handler(jvm_pen_abort_hdlr);
    //mmi_pen_register_stroke_down_handler(jvm_pen_stroke_down_hdlr);
    //mmi_pen_register_stroke_move_handler(jvm_pen_stroke_move_hdlr);
    //mmi_pen_register_stroke_up_handler(jvm_pen_stroke_up_hdlr);
}

void jvm_unregister_pen_events(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    wgui_register_general_pen_handler();
    /* wgui_register_general_pen_stroke_handler(); */
}

#else /* __MMI_TOUCH_SCREEN__ */ 
void jvm_register_pen_events(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

void jvm_unregister_pen_events(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}

#endif /* __MMI_TOUCH_SCREEN__ */ 

/*****************************************************************************
 * FUNCTION
 *  mmi_java_highlight_menu
 * DESCRIPTION
 *  Highlight handler routine for Java menu item in FunAndGames menu.
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
/* void Highlight_JAVA_MENU(void) */
void mmi_java_highlight_menu(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    SetLeftSoftkeyFunction(EntryScrJavaMenu, KEY_EVENT_UP);
    SetKeyHandler(EntryScrJavaMenu, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
}

void jam_add_history(kal_uint16 scrnID, FuncPtr func)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    /* history currHistory; */

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*
     * currHistory.scrnID = scrnID;
     * currHistory.entryFuncPtr = func;
     * app_ucs2_strcpy((kal_int8*) currHistory.inputBuffer, (kal_int8*) L"");
     * GetCategoryHistory(currHistory.guiBuffer);
     * AddHistory(currHistory);
     */
    GenericExitScreen(scrnID, func);
}

void jam_reentry_screen(void)    /* this function is call by MMI */
{
	U8 *guiBuffer;
	guiBuffer = GetCurrGuiBuffer(SCR_JAVA);
	
	EntryNewScreen(SCR_JAVA, ExitScrJavaMenu, NULL, NULL);
	entry_full_screen();
    gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	gdi_layer_get_base_handle(&JvmBaseLayer);

	jvm_register_keypad();
    jvm_register_pen_events();
    
    mmi_trace(1,"jam_reentry_screen g_jam_vm_state=%d",g_jam_vm_state);

    if(g_jam_vm_state == JVM_PAUSED_STATE)
    {
        jvm_lifecycle_nofify(JVM_LIFECYCLE_RESUME);
    }
    else if(g_jam_vm_state == JVM_BG_BLOCKING_STATE)
    {
        g_jam_vm_state = JVM_FG_RUNNING_STATE;
    }
    else if(g_jam_vm_state == JVM_BG_RUNNING_STATE)
    {
        jvm_lifecycle_nofify(JVM_LIFECYCLE_SWITCH_FOREGROUND);
    }
    else if(g_jam_vm_state == JVM_TERMINATE_STATE)
    {
        //already terminated,no action
    }
}

void jvm_start_java_req(void)
{
    mmi_java_send_message(MOD_MMI, MOD_J2ME,
        MMI_J2ME_SAP,MSG_ID_MMI_JAVA_MID_START_REQ, NULL, NULL);
}    

void jvm_exit_java_req(void)
{
    mmi_java_send_message(MOD_J2ME, MOD_MMI,
        J2ME_MMI_SAP,MSG_ID_MMI_J2ME_EXIT_SCREEN_REQ, NULL, NULL);
}

void mmi_java_exit_req_handler(void)
{
    if(GetActiveScreenId() == SCR_JAVA)
    {
        GoBackHistory();
    }
    
    HideStatusIcon(STATUS_ICON_JAVA_PAUSE);
    UpdateStatusIcons();
}
    

void InitJAVA(void)
{
    SetProtocolEventHandler((PsFuncPtr) mmi_java_exit_req_handler, MSG_ID_MMI_J2ME_EXIT_SCREEN_REQ);
    mmi_java_load_setting();
}

/*****************************************************************************
 * FUNCTION
 *  EntryScrJavaMenu
 * DESCRIPTION
 *  Enter the Java menu
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void EntryScrJavaMenu(void)
{
	U8 *guiBuffer;
    S32 create_result;
	S8  file_temp[100];
    
    g_java.last_error = JAVA_ERR_SUCCEED;

    if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}

    if(mmi_java_check_valid_drv())
    {
        AnsiiToUnicodeString(file_temp,(PS8)"D:/CSD_JVM/appdb/");
        create_result = mmi_java_create_system_folder(file_temp);

        if(create_result == FS_WRITE_PROTECTION)
        {
            g_java.last_error = JAVA_ERR_WRITE_PROTECT;
        }
        else if(create_result == FS_DISK_FULL)
        {
            g_java.last_error = JAVA_ERR_CARD_DISK_FULL;
        }
        else if(create_result == FS_ROOT_DIR_FULL)
        {
            g_java.last_error = JAVA_ERR_ROOT_DIR_FULL;
        }
        else if(create_result == FS_ERROR_RESERVED)
        {
            g_java.last_error = JAVA_ERR_CORRUPT_FAT;
        }
        
    }
    else
    {
        g_java.last_error = JAVA_ERR_NO_DISK;
    }

    if(mmi_java_check_and_display_error() == TRUE)
    {
        return;
    }
    
	guiBuffer = GetCurrGuiBuffer(SCR_JAVA);
	
	EntryNewScreen(SCR_JAVA, ExitScrJavaMenu, NULL, NULL);
	entry_full_screen();
    gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	gdi_layer_get_base_handle(&JvmBaseLayer);
    
	jvm_register_keypad();
    jvm_register_pen_events();
    mdi_audio_suspend_background_play();
    
    if(g_jam_vm_state == JVM_TERMINATE_STATE)
    {
        jvm_lifecycle_nofify(JVM_LIFECYCLE_START);
    }
    else if(g_jam_vm_state == JVM_PAUSED_STATE)
    {
        jvm_lifecycle_nofify(JVM_LIFECYCLE_RESUME);
    }
    else if(g_jam_vm_state == JVM_BG_RUNNING_STATE)
    {
        jvm_lifecycle_nofify(JVM_LIFECYCLE_SWITCH_FOREGROUND);
    }
    else
    {
        mmi_trace(1,"EntryScrJavaMenu ERROR STATE %d",g_jam_vm_state);
    }
    
}


/*****************************************************************************
 * FUNCTION
 *  ExitScrJavaMenu
 * DESCRIPTION
 *  Main Menu->FunAndGames->Java Exit Function
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void ExitScrJavaMenu(void)
{
    mmi_trace(1,"ExitScrJavaMenu g_jam_vm_state=%d",g_jam_vm_state);

    leave_full_screen();
    
    jvm_unregister_keypad();
    jvm_unregister_pen_events();
    
    if(g_jam_vm_state == JVM_TERMINATE_STATE)
    {
        mdi_audio_resume_background_play();
    }
    else if(g_jam_vm_state == JVM_BG_BLOCKING_STATE)
    {
        jam_add_history(SCR_JAVA, jam_reentry_screen);
    }
    else if(g_jam_vm_state == JVM_FG_RUNNING_STATE)
    {
        jam_add_history(SCR_JAVA, jam_reentry_screen);
#if defined(J2ME_SUPPORT_PAUSE) || defined(J2ME_SUPPORT_BACKGROUND)
        if(jam_vm_canbe_paused())
        {
            jvm_lifecycle_nofify(JVM_LIFECYCLE_PAUSE);
        }
        else if(jam_vm_canbe_minimize())
        {
            jvm_lifecycle_nofify(JVM_LIFECYCLE_SWITCH_BACKGROUND);
        }
        else
        {
            jvm_lifecycle_nofify(JVM_LIFECYCLE_SHUTDOWN);
        }
#else
        jvm_lifecycle_nofify(JVM_LIFECYCLE_SHUTDOWN);
#endif
    }
    else
    {
        mmi_trace(1,"ExitScrJavaMenu ERROR STATE %d",g_jam_vm_state);
    }
}

/*****************************************************************************
 * FUNCTION
 *  jam_usb_mode_check
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  
 *****************************************************************************/
kal_bool jam_usb_mode_check(void)
{
    return (g_jam_vm_state == JVM_FG_RUNNING_STATE) ||
    (g_jam_vm_state == JVM_BG_RUNNING_STATE) ||
    (g_jam_vm_state == JVM_PAUSED_STATE);
}


/*****************************************************************************
 * FUNCTION
 *  jam_enter_usb_mode
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void jam_enter_usb_mode(void)
{
    jvm_lifecycle_nofify(JVM_LIFECYCLE_RESUME);
    jvm_lifecycle_nofify(JVM_LIFECYCLE_SHUTDOWN);
    
	HideStatusIcon(STATUS_ICON_JAVA_PAUSE);
	UpdateStatusIcons();
}

void j2me_enter_idle_screen_notify(void)
{
}

BOOL java_is_launch_after_install(void)
{
	return FALSE;
}

void mmi_java_send_mid_start_req(void)
{
	EntryNewScreen(SCR_JAVA, NULL, NULL, NULL);
	entry_full_screen();

}

void j2me_enter_incoming_call_notify(void)
{
}

void EntryScrJavaEM(void)
{
}


void mmi_java_local_install_content_routing(kal_uint8 *file_path)
{
	S8 asicnamepath[JAVACALL_MAX_FILE_NAME_LENGTH];
	S8 namepath[JAVACALL_MAX_FILE_NAME_LENGTH]="file:///";
	U8 *guiBuffer;
    S32 create_result;
	S8  SystemFolderName[100];
    mmi_java_install_mids_req_struct *msg;
    g_java.last_error = JAVA_ERR_SUCCEED;

    if(checkUsbstate())
	{
		mmi_display_usb_busy();
		return;
	}

    if(mmi_java_check_valid_drv())
    {
        AnsiiToUnicodeString(SystemFolderName,(PS8)"D:/CSD_JVM/appdb/");
        create_result = mmi_java_create_system_folder(SystemFolderName);

        if(create_result == FS_WRITE_PROTECTION)
        {
            g_java.last_error = JAVA_ERR_WRITE_PROTECT;
        }
        else if(create_result == FS_DISK_FULL)
        {
            g_java.last_error = JAVA_ERR_CARD_DISK_FULL;
        }
        else if(create_result == FS_ROOT_DIR_FULL)
        {
            g_java.last_error = JAVA_ERR_ROOT_DIR_FULL;
        }
        else if(create_result == FS_ERROR_RESERVED)
        {
            g_java.last_error = JAVA_ERR_CORRUPT_FAT;
        }
    }
    else
    {
        g_java.last_error = JAVA_ERR_NO_DISK;
    }

    if(mmi_java_check_and_display_error() == TRUE)
    {
        return;
    }
	
	guiBuffer = GetCurrGuiBuffer(SCR_JAVA);
	
	EntryNewScreen(SCR_JAVA, ExitScrJavaMenu, NULL, NULL);
	entry_full_screen();
    gdi_lcd_set_active(GDI_LCD_MAIN_LCD_HANDLE);
	gdi_layer_get_base_handle(&JvmBaseLayer);
    
	jvm_register_keypad();
    jvm_register_pen_events();
    mdi_audio_suspend_background_play();
    
	UnicodeToAnsii(asicnamepath, file_path);
	strcat(namepath,asicnamepath);
    
    msg = NEW_LOCAL_PTR(mmi_java_install_mids_req_struct);
    msg->local_file_path = (U8*)COS_MALLOC(JAVACALL_MAX_FILE_NAME_LENGTH);
    strcpy(msg->local_file_path,namepath);
	mmi_trace(1,"mmi_java_local_install_content_routing local_file_path = %s \n",msg->local_file_path);
    
    mmi_java_send_message(MOD_MMI, MOD_J2ME,
        MMI_J2ME_SAP,MSG_ID_MMI_JAVA_INSTALL_MIDS_REQ, msg, NULL);
    
}

void jvm_launch_url_req(char* urlString)
{
}

unsigned char IsJavaOn()
{
    //TODO:needs more accurate state check
    return (g_jam_vm_state == JVM_FG_RUNNING_STATE) ||
    (g_jam_vm_state == JVM_BG_RUNNING_STATE) ||
    (g_jam_vm_state == JVM_PAUSED_STATE);
}


void JavaMainTask(void *pData)
{
    COS_EVENT ev ;
    TASK_HANDLE * hTask;
	ilm_struct* Message = NULL;
    hTask = (TASK_HANDLE *)GetMmiTaskHandle(MOD_J2ME);
    SUL_ZeroMemory32(&ev, sizeof(COS_EVENT) );
    mmi_trace(1,"----------------------JavaMainTask Begin---------------------------\n");

    while (1)
    {
        GET_COSMSG(MOD_J2ME, &ev);
        
        javacall_events_init();

        if (ev.nEventId == MSG_ID_MMI_JAVA_MID_START_REQ)
        {
            javanotify_start();
        }
        else if(ev.nEventId == MSG_ID_MMI_JAVA_INSTALL_MIDS_REQ)
        {
            Message = (ilm_struct *)ev.nParam1;
            mmi_java_install_mids_req_struct *msg = (mmi_java_install_mids_req_struct *)Message->local_para_ptr;
            javanotify_install_midlet_wparams(msg->local_file_path,0,0);
            COS_FREE(msg->local_file_path);            
        }
        
        JavaTask();

        javacall_events_finalize();
        
        MMI_FreeEvent(&ev);
    }
}


#endif /* __J2ME__ */ 

