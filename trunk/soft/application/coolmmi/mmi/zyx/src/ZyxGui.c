#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "zyxgdi.h"
#include "zyxsys.h"
#include "zyxfile.h"
#include "zyxmisc.h"
#include "zyxextern.h"
#include "zyxhttp.h"   
#include "adp_events.h"
//#include "mmi_sap.h"
#include "mtpnp_ad_resdef.h"

#undef __MMI_TOUCH_SCREEN__
extern pBOOL IsBackHistory;
extern const U16 gIndexIconsImageList[];

int ZyxUsableSim = CFW_SIM_END;

void SetupAppDir();
BOOL CheckMenuFile();

typedef enum
{
    ZYX_APP_SCREENID = ZYX_PLATFORM_BASE + 1,
    ZYX_LIST_SCREENID,
    ZYX_APP_EDIT_SCREENID,
    ZYX_DUMMY_SCREENID,
} SCREENID_LIST_ZYX;

static U16 connect_net_fail[] ={0x8FDE,0x63A5,0x5931,0x8D25,0};//连接失败
static U16 update_str[] = {0x6B63,0x5728,0x66F4,0x65B0,0};//正在更新
static U16 sim_err_str[]={0x8BF7,0x63D2,0x5165,'S','I','M',0x5361,0x540E,0xFF0C,0x91CD,0x65B0,0x767B,0x5F55,0xFF01,0x0000};//请插入SIM卡后，重新登录！
static U16 Tcard_err_str[]={0x8BF7,0x63D2,0x5165,0x50A8,0x5B58,0x5361,0x540E,0xFF0C,0x91CD,0x65B0,0x767B,0x5F55,0xFF01,0x0000};//请插入储存卡后，重新登录！
static U16 load_fail_str[] = {0x52A0,0x8F7D,0x5931,0x8D25,0xFF01,0};//加载失败
static U16 title_str[]={0x624B,0x673A,0x5A31,0x4E50,0};//手机娱乐
//static U16 downhint_str[] = {0x6B63,0x5728,0x4E0B,0x8F7D,0x65B0,0x5E94,0x7528,0x0000};//正在加载,请稍候
static U16 starthint_str[] = {0x6B63,0x5728,0x542f,0x52a8,0x65B0,0x5E94,0x7528,0x0000};//正在加载,请稍候

ZyxSysevtHandler pZyxSysevtHandler = NULL;
ZyxKeyevtHandler pZyxKeyevtHandler = NULL;
ZyxPenevtHandler pZyxPenevtHandler = NULL;

ZyxSysevtHandler pZyxSysevtHandler_b = NULL;
ZyxKeyevtHandler pZyxKeyevtHandler_b = NULL;
ZyxPenevtHandler pZyxPenevtHandler_b = NULL;


int ZyxAppState = 0; // TODO:0:quit, 1:foreground running,2: background running

#ifdef __MMI_TOUCH_SCREEN__    
static void Zyx_pen_down_hdlr (mmi_pen_point_struct pos);
static void Zyx_pen_up_hdlr (mmi_pen_point_struct pos);
static void Zyx_pen_move_hdlr (mmi_pen_point_struct pos);
#endif

char ZyxAppFullPath[64] = {0},ZyxAppParam[32] = {0};

void Zyx_GuiDisplayProgress(int nProgPos);

////////////////////////////////////////////////////////////////////////
#define MENU_MAX 20
U16 MenuName[MENU_MAX][10]={{0}},highlight_item_num=0;

static void ShowCategory15ScreenEx(
                            U16* title,
                            PU8  title_icon,
                            U16 left_softkey,
                            U16 left_softkey_icon,
                            U16 right_softkey,
                            U16 right_softkey_icon,
                            S32 number_of_items,
                            
                            U16** list_of_items,
                            PU8 *list_of_icons,
                            
                            S32 flags,
                            S32 highlighted_item,
                            U8 *history_buffer)
{
    dm_data_struct dm_data;
    S32 i, shortcut_width;
    U8 h_flag;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gdi_layer_lock_frame_buffer();
    
    change_left_softkey(left_softkey, left_softkey_icon);
    change_right_softkey(right_softkey, right_softkey_icon);
    SetupCategoryKeyHandlers();
    
    create_fixed_icontext_menuitems();
    MMI_current_menu_type = flags & 0x0f;
    MMI_title_string = (UI_string_type) title;
    MMI_title_icon = (PU8)title_icon;
    
    resize_fixed_list(MMI_content_width, MMI_content_height);        
    associate_fixed_icontext_list();
    for (i = 0; i < number_of_items; i++)
    {
       add_fixed_icontext_item(list_of_items[i],wgui_get_list_menu_icon(i, gIndexIconsImageList[i]));
    }
    register_fixed_list_shortcut_handler();
    register_fixed_list_keys();
    shortcut_width = set_menu_item_count(number_of_items) + 7;
    resize_menu_shortcut_handler(shortcut_width, MMI_title_height);
    move_menu_shortcut_handler(UI_device_width - MMI_menu_shortcut_box.width, MMI_title_y);
    register_hide_menu_shortcut(gui_redraw_menu_shortcut);
    resize_fixed_icontext_menuitems(0, MMI_MENUITEM_HEIGHT);
    register_fixed_list_highlight_handler(standard_list_highlight_handler);
    h_flag = set_list_menu_category_history(MMI_CATEGORY15_LIST_ID, history_buffer);
    if (h_flag)
    {
       fixed_list_goto_item_no_redraw(MMI_fixed_list_menu.highlighted_item);
    }
    else
    {
       fixed_list_goto_item_no_redraw(highlighted_item);
    }
    
    gdi_layer_unlock_frame_buffer();
    
    ExitCategoryFunction = ExitCategory15Screen;
    RedrawCategoryFunction = dm_redraw_category_screen;
    GetCategoryHistory = dm_get_category_history;
    GetCategoryHistorySize = dm_get_category_history_size;
    dm_data.S32ScrId = (S32) GetActiveScreenId();
    dm_data.S32CatId = MMI_CATEGORY15_LIST_ID;
    dm_data.S32flags = 0;
    dm_setup_data(&dm_data);
    dm_redraw_category_screen();
}   /* end of ShowCategory15Screen */

void ShowMenuItems(U8 *guiBuffer)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16* nStrItemList[10]={0};
    PU8   nIconItemList[10]={0};
    U16 nNumofItem=0;
    
    while (nNumofItem < g_total_items)
    {
        nIconItemList[nNumofItem] = NULL;
        nStrItemList[nNumofItem] = (U16*)MenuName[nNumofItem];
        nNumofItem ++;
    }
    
    ShowCategory15ScreenEx(
        title_str,
        NULL,
        STR_GLOBAL_OK,
        IMG_GLOBAL_OK,
        STR_GLOBAL_BACK,
        IMG_GLOBAL_BACK,
        
        nNumofItem,
        (U16**)nStrItemList,
        (PU8 *)nIconItemList,
        
        1,
        0,
        guiBuffer);
    
    SetLeftSoftkeyFunction(mmi_zyx_item_entry, KEY_EVENT_UP);
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP);
    
    return;
}

void ZyxCurrHiliteHandler(S32 index)
{
    highlight_item_num = index;
}

void Zyx_Select_Entry(void);

#if 0
static void ZYX_List_entryCallback(UINT8 nResult){
	switch (nResult){
		case 0:
		ZyxTrace("[gprs] Start OK\n");
		Zyx_Select_Entry();
		break;
		case 3:
		{
			ZyxTrace("[gprs] Inactived\n");
			//GoBackHistory();
			ZyxShowPopMsg(connect_net_fail,POPUP_ERROR);
		}
		break;
		default:
		{
			ZyxTrace("[gprs] Start Failed\n");
			ZyxShowPopMsg(connect_net_fail,POPUP_ERROR);
		}
		break;
	}
}
#endif

int g_zyx_wait_count = 50;
int g_zyx_wait_timer = -1;
int g_zyx_wait_done = 0;
int g_zyx_wait_cancel = 0;

static void  ZyxSysevtHandlerGUI( void* param)
{
	mmi_ps_gprs_status_update_ind_struct* pp;

	pp = (mmi_ps_gprs_status_update_ind_struct*)param;

	ZyxTrace("[gprs] Start cb\n");
	if (g_zyx_wait_cancel){
		ZyxTrace("[gprs] cancel\n");
		return;
	}
	g_zyx_wait_done = 1;

	switch (pp->status){
		case 0:
		{
			Zyx_GuiDisplayProgress(100);
			ZyxTrace("[gprs] Start OK\n");
			//Zyx_Select_Entry();
			Zyx_App_Entry();
		}
		break;
		case 1:	//失败
		case 2:	//失败
		case 3: 	//断开
		default:
		ZyxTrace("[gprs] Start Failed\n");
		ZyxRegSysevtCallback(0);
		GoBackHistory();
		ZyxShowPopMsg(connect_net_fail,POPUP_ERROR);
		break;
	}
	
	//ZyxStopTimer(g_zyx_wait_timer);
}

//extern UINT32 adp_PrepareGPRS(Soc_GPRS_Callback cb, UINT32 arg_simid, UINT8 apnsize, char *apnName);
UINT32 adp_PrepareGPRS(UINT32 arg_simid, UINT8 apnsize, char *apnName);
void SetProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID);
void SetSlaveProtocolEventHandler(PsFuncPtr funcPtr, U32 eventID);

void Zyx_Dummy_Entry(void)
{
    EntryNewScreen(ZYX_DUMMY_SCREENID, NULL, Zyx_Dummy_Entry, NULL);
    GoBackHistory();
}

static void Zyx_Wait_Exit(void){
	g_zyx_wait_cancel = 1;
	//ZyxExitDownload();
}

static U8 Zyx_Wait_Delete(void *para)
{
	ZyxTrace("Zyx_Wait_Delete do\n");
    ZyxExitDownload();
    return 0;
}    

static void Zyx_Wait_Cancel(void)
{
	g_zyx_wait_cancel = 1;
	GoBackHistory();
}    

static void Zyx_Wait_Timer_CB(int timer_id){
	if (g_zyx_wait_done == 0 && g_zyx_wait_cancel == 0)
	{
		g_zyx_wait_count += 3;
		if (g_zyx_wait_count > 100){
			g_zyx_wait_cancel = 1;
			ZyxRegSysevtCallback(0);
			GoBackHistory();
			ZyxShowPopMsg(connect_net_fail,POPUP_ERROR);
		}
		else
		{
			Zyx_GuiDisplayProgress(g_zyx_wait_count);
			g_zyx_wait_timer = ZyxStartTimer(1000, (ZyxTimerHandler)Zyx_Wait_Timer_CB);
		}
	}
}

static void Zyx_Wait_Timer_CB_q(int timer_id){
	GoBackHistory();
}

static void Zyx_Wait_entry(void){
	EntryNewScreen(ZYX_DUMMY_SCREENID, Zyx_Wait_Exit, Zyx_Wait_entry, NULL);

#if 0
	if (g_zyx_wait_stat){
		GoBackHistory();
		return ;
	}
#endif
	ZyxTrace("Zyx_Wait_Delete register\n");
	SetDelScrnIDCallbackHandler(ZYX_DUMMY_SCREENID, Zyx_Wait_Delete);
	ClearAllKeyHandler();
	
	entry_full_screen();
	gdi_layer_set_source_key(FALSE,GDI_COLOR_TRANSPARENT);
	gdi_layer_clear(GDI_COLOR_BLACK);
	gdi_layer_reset_clip();
	gui_set_font(&MMI_medium_font);
	gui_set_text_color(UI_COLOR_WHITE);
	ZyxFlush(0, 0, ZyxGetLCDWidth(), ZyxGetLCDHeight());
	
    SetRightSoftkeyFunction(Zyx_Wait_Cancel, KEY_EVENT_UP); 
	
#ifdef __MMI_TOUCH_SCREEN__    
	wgui_register_pen_down_handler(Zyx_pen_down_hdlr);
	wgui_register_pen_up_handler(Zyx_pen_up_hdlr);
	wgui_register_pen_move_handler(Zyx_pen_move_hdlr);
#endif

	if (g_zyx_wait_done){
		ZyxStartTimer(10, (ZyxTimerHandler)Zyx_Wait_Timer_CB_q);
		return ;
	}

	g_zyx_wait_cancel = 0;
	g_zyx_wait_done = 0;
	g_zyx_wait_count = 0;
	
	Zyx_GuiDisplayProgress(g_zyx_wait_count);
	ZyxFlush(0, 0, ZyxGetLCDWidth(), ZyxGetLCDHeight());


#ifdef __MMI_GPRS_FEATURES__
	SetProtocolEventHandler(ZyxSysevtHandlerGUI,MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
#ifdef __MMI_DUAL_SIM__
	SetSlaveProtocolEventHandler(ZyxSysevtHandlerGUI,MSG_ID_MMI_NW_GPRS_CONNECTED_RSP);
#endif
#endif
	
	adp_PrepareGPRS(Zyx_get_usable_sim(), 5, "CMWAP");
	g_zyx_wait_timer = ZyxStartTimer(1000, (ZyxTimerHandler)Zyx_Wait_Timer_CB);
}

void Zyx_Wait_Start(void)
{
    U64 total,free;
    BOOL bTCardExist = FALSE;
    bTCardExist = ZyxGetDriveInfo(DEVICE_TCARD, &total, &free);
    
    if (bTCardExist == 0)
    {
    	g_zyx_wait_done = 0;
    	Zyx_Wait_entry();
    }
    else
    {
		ZyxShowPopMsg(Tcard_err_str,POPUP_ERROR);
		return ;
    }
}


int zyx_select_sim_temp = 0;
void ZyxSelectSimHilitHdr(S32 index) 
{ 
    zyx_select_sim_temp = index; 
} 

void ZYX_List_entry1(void);

void ZyxSelectSimLSKHdr(void) 
{ 
    ZyxUsableSim = zyx_select_sim_temp; 

	Zyx_Wait_Start(); 
} 

void Zyx_Sim_entry(void) 
{
    U8 *guiBuffer; 
    U16 screenTopId;
    U8* PopUpList[2]; 

    PopUpList[0] = GetString(STRING_MTPNP_SIM1); 
    PopUpList[1] = GetString(STRING_MTPNP_SIM2); 
              
    screenTopId = GetActiveScreenId(); 
                    
    EntryNewScreen(ZYX_LIST_SCREENID, NULL, Zyx_Sim_entry, 0); 
    
    guiBuffer = GetCurrGuiBuffer(ZYX_APP_SCREENID); 

    if (guiBuffer == NULL) 
    {
		zyx_select_sim_temp = 0;//kk_debug_type; 
    } 
    
    RegisterHighlightHandler(ZyxSelectSimHilitHdr); 
    
    ShowCategory353Screen(0, 0, 
            STR_GLOBAL_OK, 0, 
            STR_GLOBAL_BACK, 0, 
            2,//sizeof(strItems)/sizeof(strItems[0]), 
            (U8 **)PopUpList ,//strItems, 
            (U16*)gIndexIconsImageList, 
            0, 
            0, 
            zyx_select_sim_temp, 
            guiBuffer); 

    SetLeftSoftkeyFunction(ZyxSelectSimLSKHdr, KEY_EVENT_UP); 
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP); 
} 

void Zyx_Sim_Check(void)
{
    E_MTPNP_AD_SIMCARD_USABLE_SIDE simStatus; 

    simStatus = MTPNP_AD_Get_UsableSide(); 
    
    if (MTPNP_AD_DUALSIM_USABLE == simStatus) 
    { 
            Zyx_Sim_entry();
    } 
    else if (MTPNP_AD_SIMCARD1_USABLE == simStatus) 
    { 
            ZyxUsableSim = CFW_SIM_0; 
			Zyx_Wait_Start(); 
    } 
    else if (MTPNP_AD_SIMCARD2_USABLE == simStatus) 
    { 
			ZyxUsableSim = CFW_SIM_1; 
			Zyx_Wait_Start(); 
    }
    else
    {
			ZyxUsableSim =  CFW_SIM_END;
			ZyxShowPopMsg(sim_err_str,POPUP_ERROR);
    }
}

U8 Zyx_Select_del(void *para)
{
    ZyxExitDownload();
    return 0;
}

void Zyx_Select_Entry(void)
{
    U8 *guiBuffer;
    U64 total,free;
    int ret;

    SetupAppDir();
    ZyxGetImei();
    ZyxGetImsi();
    ZyxGetSc();

    gui_set_font(&MMI_medium_font);
    gui_set_text_color(UI_COLOR_WHITE);
    
#ifdef __USB_IN_NORMAL_MODE__
    if (mmi_usb_is_in_mass_storage_mode())
    {
        mmi_usb_app_unavailable_popup(0);  
        return;
    }
#endif     

    //get t card status
    ret = ZyxGetDriveInfo(DEVICE_TCARD, &total, &free);
    if (ret == 0)
    {
        int bg_width = 150,bg_height=30;
        S32 txt_wid,txt_height;

		EntryNewScreen(ZYX_LIST_SCREENID, NULL, Zyx_Select_Entry, NULL);
		guiBuffer = GetCurrGuiBuffer(ZYX_LIST_SCREENID);
		RegisterHighlightHandler(ZyxCurrHiliteHandler);
		SetDelScrnIDCallbackHandler(ZYX_LIST_SCREENID,Zyx_Select_del);

		CheckMenuFile();
        ParseMenuItems();
        if (g_total_items == 0) //prepare download
        {
#if (0)//def __FLIGHT_MODE_SUPPORT__
			if (mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
			{
			 GoBackHistory();
			 DisplayPopup((PU8)GetString(STR_FLIGHT_MODE_SILENT),//飞行模式
			    (U16) IMG_GLOBAL_WARNING, (U8) 1, (U32) ST_NOTIFYDURATION, (U8) WARNING_TONE);                
			return;
			}
#endif
	
             ZyxUsableSim = Zyx_get_usable_sim();
             if (ZyxUsableSim == CFW_SIM_END)//no sim avalable
             {
                 GoBackHistory();
                ZyxShowPopMsg(sim_err_str,POPUP_ERROR);
                return;
             }
		}

		ShowMenuItems(guiBuffer);	
       
       if (g_total_items == 0) //show download screen
       {
            gdi_draw_solid_rect((ZyxGetLCDWidth()-bg_width)/2, 
            (ZyxGetLCDHeight()-bg_height)/2, 
            (ZyxGetLCDWidth()-bg_width)/2+bg_width,
            (ZyxGetLCDHeight()-bg_height)/2+bg_height,
            GDI_COLOR_RED);
            gui_measure_string(update_str,&txt_wid,&txt_height);
            gui_print_truncated_text((ZyxGetLCDWidth()-txt_wid)/2,
            (ZyxGetLCDHeight()-txt_height)/2,txt_wid,update_str);
            SetLeftSoftkeyFunction(NULL, KEY_EVENT_UP);
			ZyxFlush(0, 0, ZyxGetLCDWidth(), ZyxGetLCDHeight());
            
            //DownMenuFile();
        }
    }
    else
    {
		ZyxShowPopMsg(Tcard_err_str,POPUP_ERROR);
		ZyxSocketExit(0);
    }
}

void ZYX_List_entry(void)
{
	//Zyx_Wait_Start();
	Zyx_Sim_Check();
}

void Zyx_GuiDisplayProgress(int nProgPos)
{
    S32 txt_wid,txt_height;
    int bg_width = 150,bg_height=40,bar_height=10;
    gdi_rect_struct window;

    nProgPos = nProgPos > 100 ? 100 : nProgPos;
    
    window.x1 = (ZyxGetLCDWidth()-bg_width)/2;	
    window.y1 = (ZyxGetLCDHeight()-bg_height)/2;	
    window.x2 = (ZyxGetLCDWidth()-bg_width)/2+bg_width;	
    window.y2 = (ZyxGetLCDHeight()-bg_height)/2+bg_height;	

    gdi_draw_solid_rect(window.x1, window.y1,window.x2-window.x1,window.y2-window.y1,GDI_COLOR_GRAY);
    
    gui_measure_string(starthint_str,&txt_wid,&txt_height);
    gui_print_truncated_text(window.x1+(bg_width-txt_wid)/2,window.y1+3,txt_wid,starthint_str);
    
    gdi_draw_solid_rect(window.x1, window.y2-bar_height,
        window.x1+bg_width*nProgPos/100,window.y2-1,GDI_COLOR_RED);
    
	ZyxFlush(0, 0, ZyxGetLCDWidth(), ZyxGetLCDHeight());
}

////////////////////////////////////////////////////////////////////////


static void CancelLoading(void)
{
    ZyxCancelDownload();
    GoBackHistory();
}

#ifdef __MMI_TOUCH_SCREEN__
static void mmi_load_pendown_hdlr (mmi_pen_point_struct pos)
{
    if (PEN_CHECK_BOUND(pos.x,pos.y,UI_device_width-MMI_SOFTKEY_WIDTH,UI_device_height-MMI_SOFTKEY_HEIGHT,
            MMI_SOFTKEY_WIDTH,MMI_SOFTKEY_HEIGHT))
   {
        CancelLoading();
   }
}
#endif


static void Zyx_App_Exit(void)
{
    //ZyxCancelDownload();
    pZyxSysevtHandler_b = pZyxSysevtHandler;
    pZyxKeyevtHandler_b = pZyxKeyevtHandler;
    pZyxPenevtHandler_b = pZyxPenevtHandler;
    
	ZyxAudioStop();
    if (pZyxSysevtHandler)
    {
		ZyxTrace("zyx app pause cb\n");
        pZyxSysevtHandler(ZYX_PAUSE,0);
    }

    if (ZyxAppState == 1) //app not quit
        ZyxAppState = 2;
	
    //pZyxSysevtHandler = NULL;
    pZyxSysevtHandler = pZyxSysevtHandler_b;
    pZyxKeyevtHandler = NULL;
    pZyxPenevtHandler = NULL;
     
    ZyxTrace("zyx app exit app state=%d",ZyxAppState);    
}

static U8 Zyx_App_Del(void *para)
{
    if (pZyxSysevtHandler)
    {
		ZyxTrace("zyx app quit cb\n");
        pZyxSysevtHandler(ZYX_QUIT,0);
    }
	
    ZyxAppState = 0;

    pZyxSysevtHandler = NULL;
    pZyxKeyevtHandler = NULL;
    pZyxPenevtHandler = NULL;

	ZyxSysLockExit();
    ZyxTrace("zyx app del app state=%d",ZyxAppState);
    return 0;
}    

extern pBOOL IsBackHistory;
void Zyx_App_Entry(void)
{
    pBOOL bBackHistory = IsBackHistory;
    U64 total,free;
    int ret;
    ZFILE hFile;

	ZyxSysLockInit();
#if(0)//def __FLIGHT_MODE_SUPPORT__
       if (mmi_bootup_get_active_flight_mode() == FLIGHTMODE_SILENT)
       {
        DisplayPopup((PU8)GetString(STR_FLIGHT_MODE_SILENT),//飞行模式
            (U16) IMG_GLOBAL_WARNING, (U8) 1, (U32) ST_NOTIFYDURATION, (U8) WARNING_TONE);                
        return;
       }
#endif   
     ZyxUsableSim = Zyx_get_usable_sim();
     if (ZyxUsableSim == CFW_SIM_END)//no sim avalable
     {
        ZyxShowPopMsg(sim_err_str,POPUP_ERROR);
        return;
     }

    EntryNewScreen(ZYX_APP_SCREENID, Zyx_App_Exit, Zyx_App_Entry, NULL);
    SetDelScrnIDCallbackHandler(ZYX_APP_SCREENID, Zyx_App_Del);
    ClearAllKeyHandler();

    entry_full_screen();
    gdi_layer_set_source_key(FALSE,GDI_COLOR_TRANSPARENT);
    gdi_layer_clear(GDI_COLOR_BLACK);
    gdi_layer_reset_clip();
    gui_set_font(&MMI_medium_font);
    gui_set_text_color(UI_COLOR_WHITE);

    //get t card status
    ret = ZyxGetDriveInfo(DEVICE_TCARD, &total, &free);

#ifdef __MMI_TOUCH_SCREEN__    
    wgui_register_pen_down_handler(Zyx_pen_down_hdlr);
    wgui_register_pen_up_handler(Zyx_pen_up_hdlr);
    wgui_register_pen_move_handler(Zyx_pen_move_hdlr);
#endif

    if (bBackHistory && ZyxAppState == 2) //resume
    {
        if (ret == 0)
       {
           if (pZyxSysevtHandler)
           {
			   	ZyxTrace("zyx app resume cb\n");
                pZyxSysevtHandler(ZYX_RESUME,0);
           }
           ZyxAppState = 1;
           ZyxTrace("zyx app resume app state=%d",ZyxAppState);
		   pZyxKeyevtHandler = pZyxKeyevtHandler_b;
		   pZyxPenevtHandler = pZyxPenevtHandler_b;
		   pZyxSysevtHandler = pZyxSysevtHandler_b;
       }
        else
       {
            ZyxAppQuit();
       }
    }
    else
    {
        if (ret == 0) //load app
        {
            hFile = ZyxFileOpen(ZyxAppFullPath,ZFS_MODE_READ);
           // if (hFile < 0)
           if(0)
            {
                DownExeFile();
                ChangeRightSoftkey(STR_GLOBAL_BACK, IMG_GLOBAL_BACK);
                show_right_softkey();
                SetRightSoftkeyFunction(CancelLoading,KEY_EVENT_UP);
                #ifdef __MMI_TOUCH_SCREEN__	
                wgui_register_pen_down_handler(mmi_load_pendown_hdlr);
                #endif
				ZyxFlush(0, 0, ZyxGetLCDWidth(), ZyxGetLCDHeight());
            }
            else
            {
                ZyxFileClose(hFile);
                ZyxTrace("zyx app open");
                #ifndef WIN32
               // ret = ZyxLoadAppRun(ZyxAppFullPath,ZyxAppParam);
               
                 ZyxTrace("/////////////////fanfanfan//////////////");
	          	ZyxTrace("/////////////////fanfanfan//////////////");

				extern void EKP_App_Main(void * p /*,char * argv[]*/);
                 EKP_App_Main(NULL);

                #else
                {
                    extern void ZyxMain(void *para);
                    ZyxMain(ZyxAppParam);
                    ret = 0;
                }
                #endif
                if (ret == 0) //load success
                {
                    ZyxAppState = 1;
                }
                else //load fail
                {
                    ZyxTrace("zyx app load fail=%d",ret);
                    GoBackHistory();
                    ZyxShowPopMsg(load_fail_str,POPUP_ERROR);
                }
            }
        }
        else
        {
            GoBackHistory();
#ifdef __USB_IN_NORMAL_MODE__
    if (mmi_usb_is_in_mass_storage_mode())
    {
        mmi_usb_app_unavailable_popup(0);  
        return;
    }
    else
#endif             
            ZyxShowPopMsg(Tcard_err_str,POPUP_ERROR);
        }
    }
}


//key related functions
int Zyx_get_sys_keycode(int sys_keycode)
{
   switch(sys_keycode) 
    {
    case KEY_0:
        return ZYX_KEY_0;
    case KEY_1:
        return ZYX_KEY_1;
    case KEY_2:
        return ZYX_KEY_2;
    case KEY_3:
        return ZYX_KEY_3;
    case KEY_4:
        return ZYX_KEY_4;
    case KEY_5:
        return ZYX_KEY_5;
    case KEY_6:
        return ZYX_KEY_6;
    case KEY_7:
        return ZYX_KEY_7;
    case KEY_8:
        return ZYX_KEY_8;
    case KEY_9:
        return ZYX_KEY_9;
    case KEY_LSK:
        return ZYX_KEY_LEFT_SOFTKEY;
    case KEY_RSK:
        return ZYX_KEY_RIGHT_SOFTKEY;
    case KEY_CSK:
        return ZYX_KEY_CENTER_SOFTKEY;
    case KEY_UP_ARROW:
        return ZYX_KEY_UP;
    case KEY_DOWN_ARROW:
        return ZYX_KEY_DOWN;
    case KEY_LEFT_ARROW:
        return ZYX_KEY_LEFT;
    case KEY_RIGHT_ARROW:
        return ZYX_KEY_RIGHT;
    case KEY_SEND:
        return ZYX_KEY_CALL;
    case KEY_END:
        return ZYX_KEY_END;
    case KEY_CLEAR:
        return ZYX_KEY_CLEAR;
    case KEY_STAR:
        return ZYX_KEY_STAR;
    case KEY_POUND:
        return ZYX_KEY_UNKOWN;
        //return ZYX_KEY_POUND;
    case KEY_VOL_UP:
        return ZYX_KEY_VOL_UP;
    case KEY_VOL_DOWN:
        return ZYX_KEY_VOL_DOWN;
    case KEY_CAMERA:
        return ZYX_KEY_CAMERA;
    case KEY_ENTER:
        return ZYX_KEY_OK;
    default:
        return ZYX_KEY_UNKOWN;
    }
}

int Zyx_get_sys_keytype(int sys_keytype)
{
     switch(sys_keytype) 
     {
    case KEY_EVENT_DOWN:
        return ZYX_KEY_EVENT_DOWN;
    case KEY_EVENT_UP:
        return ZYX_KEY_EVENT_UP;
    case KEY_REPEAT:
        return ZYX_KEY_EVENT_REPEAT;
    default:
        return ZYX_KEY_EVENT_UNKOWN;
    }
}

void Zyx_key_handler(S16 keycode,S16 keytype)
{
 	ZyxSysLockEnter();   
    if (ZyxCheckCallback((void*)pZyxKeyevtHandler))
    {
        pZyxKeyevtHandler(Zyx_get_sys_keycode(keycode),Zyx_get_sys_keytype(keytype));
        ZyxTrace("zyx key code=%d,type=%d",keycode,keytype);      
    }
 	ZyxSysLockLeave();   
}

//touch screen ralated functions
#ifdef __MMI_TOUCH_SCREEN__    
int Zyx_get_sys_pentype(int sys_pentype)
{
    switch(sys_pentype) 
     {
    case MMI_PEN_EVENT_DOWN:
        return ZYX_PEN_EVENT_DOWN;
    case MMI_PEN_EVENT_UP:
        return ZYX_PEN_EVENT_UP;
    case MMI_PEN_EVENT_MOVE:
        return ZYX_PEN_EVENT_MOVE;
    default:
        return ZYX_PEN_EVENT_UNKOWN;
    }
}

void Zyx_pen_down_hdlr (mmi_pen_point_struct pos)
{
    if(ZyxCheckCallback((void*)pZyxPenevtHandler))
        pZyxPenevtHandler(Zyx_get_sys_pentype(MMI_PEN_EVENT_DOWN),pos.x,pos.y);
}

void Zyx_pen_up_hdlr (mmi_pen_point_struct pos)
{
    if(ZyxCheckCallback((void*)pZyxPenevtHandler))
        pZyxPenevtHandler(Zyx_get_sys_pentype(MMI_PEN_EVENT_UP),pos.x,pos.y);    
}

void Zyx_pen_move_hdlr (mmi_pen_point_struct pos)
{
    if(ZyxCheckCallback((void*)pZyxPenevtHandler))
        pZyxPenevtHandler(Zyx_get_sys_pentype(MMI_PEN_EVENT_MOVE),pos.x,pos.y);
}
#endif

void SetupAppDir()
{
    char dlfile[256] = {0};
    
    sprintf(dlfile,"%c:\\ZYXAPP",ZyxGetDrive(DEVICE_TCARD));
    ZyxCreateDir(dlfile);
    
    sprintf(dlfile,"%c:\\ZYXAPP\\bin",ZyxGetDrive(DEVICE_TCARD));
    ZyxCreateDir(dlfile);
    
    sprintf(dlfile,"%c:\\ZYXAPP",ZyxGetDrive(DEVICE_NORMAL));
    ZyxCreateDir(dlfile);
    
    sprintf(dlfile,"%c:\\ZYXAPP\\menu",ZyxGetDrive(DEVICE_NORMAL));
    ZyxCreateDir(dlfile);
}

///////////////////////////////////////////////////////////////////////////////

static U16 *pZyxEditorContent = NULL;
static int ZyxEditorSize = 0;
static ZyxEditorCb ZyxEditorCbBak = NULL;

static void ZyxInputConfirm()
{
    if (ZyxEditorCbBak)
        ZyxEditorCbBak(TRUE);
    GoBackHistory();
}


void ZyxInputCancel()
{
    if (ZyxEditorCbBak)
        ZyxEditorCbBak(FALSE);
    GoBackHistory();
}

void ZyxeditorValidation (U8 *txt, U8 *cursor, S32 len)
{
    EnableLeftSoftkey(STR_GLOBAL_OK,IMG_GLOBAL_OK);
    SetLeftSoftkeyFunction( ZyxInputConfirm, KEY_EVENT_UP);
}

static void ZyxEditorEntry()
{

    EntryNewScreen(ZYX_APP_EDIT_SCREENID,NULL,ZyxEditorEntry,NULL);

    ShowCategory5Screen_ext(0,0,	STR_GLOBAL_OK, 0,STR_GLOBAL_BACK, 0,
    INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,(unsigned char*)pZyxEditorContent, 
    ZyxEditorSize, NULL,0,NULL,NULL	); 

    SetLeftSoftkeyFunction(ZyxInputConfirm, KEY_EVENT_UP);
    SetCategory5RightSoftkeyFunction(ZyxInputCancel, KEY_EVENT_UP);
    RegisterInputBoxValidationFunction(ZyxeditorValidation);
}

void ZyxEnterEditor(U16 *content,int size,ZyxEditorCb cb)
{

    pZyxEditorContent = content;
    ZyxEditorSize= size;
    ZyxEditorCbBak = cb;
    ZyxEditorEntry();
}

void ZyxOpenSMS(U16 *sms_content)
{
#if 0
   EMSData *pEMS;

    pEMS = (EMSData*)GetEMSDataForEdit (0, 1);
    AppendCategory28String(
     0,
     pEMS,
     (U8*) sms_content,
     NULL); 
	mmi_msg_entry_write_msg();
#endif    
}


//进彩信界面
void ZyxOpenMMS(U16 *phone_number, U16 *mms_subject, U16 *mms_content, U16 *file_path)
{
}

BOOL Zyx_On_idle_state()
{
#if 0
    if (ZyxAppState != 0)
        return FALSE;
    else if(g_idle_context.IsOnIdleScreen || g_keylock_context.gKeyPadLockFlag || g_idle_context.ScreenSaverRunFlag ||!IsBacklightOn())
        return TRUE;
    else
        return FALSE;
#endif
    return TRUE;
}



////////////////////////menu related//////////////////
static char BinFileUrl[128];
static char menu_xml_file[] =  "menu.xml";
#define MEMU_DATA_FILE "menu.dat"
#define MENU_URL "http://zbsc2.3gzyx.com/wbsc/cms/appdownload.do?code=%s&type=%s&DPI=%s&version=%s"
#define URL_LEN (128)
typedef struct
{
    U16 AppName[20];
    char AppEngName[32];
    char BinUrl[URL_LEN];
}MENU_DATA;

int g_total_items = 0;

const char g_menu_data[] = {
#include "menu_dat.h"
};
const char g_menu_xml[] = {
#include "menu_xml.h"
};

void mmi_zyx_item_entry(void)
{
    MENU_DATA menu_data={{0}};
    char destFile[128],*p;
    ZFILE hFile;
    ZFILE fp;
    
    if (g_total_items == 0)
        return;
        
    sprintf(destFile,"%c:\\ZYXAPP\\",ZyxGetDrive(DEVICE_NORMAL));
    ZyxCreateDir(destFile);
    sprintf(destFile,"%c:\\ZYXAPP\\menu\\",ZyxGetDrive(DEVICE_NORMAL));
    ZyxCreateDir(destFile);
    sprintf(destFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_NORMAL),MEMU_DATA_FILE);
    hFile = ZyxFileOpen(destFile,ZFS_MODE_READ|ZFS_MODE_WRITE|ZFS_MODE_CREATE);
    if (hFile){
		ZyxFileWrite(hFile, (char*)g_menu_data, sizeof(g_menu_data));
		ZyxFileClose(hFile);
    }

    sprintf(destFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_NORMAL),MEMU_DATA_FILE);	
    fp = ZyxFileOpen(destFile,ZFS_MODE_READ);	
    ZyxFileSeek(fp, highlight_item_num*sizeof(menu_data), ZFS_BEGIN);	
    ZyxFileRead(fp,(char*)&menu_data,sizeof(menu_data));
    ZyxFileClose(fp);

    strcpy(BinFileUrl,menu_data.BinUrl);
    p = strrchr(menu_data.BinUrl,'/')+1;
    sprintf(ZyxAppFullPath,"%c:\\ZYXAPP\\bin\\%s",ZyxGetDrive(DEVICE_TCARD),p);	
    ZyxAppParam[0] = 0;

    Zyx_App_Entry();
}

BOOL CheckMenuFile()
{
    char destFile[128]={0};
    ZFILE hFile;
    BOOL bXmlExist = FALSE;

    sprintf(destFile,"%c:\\ZYXAPP\\",ZyxGetDrive(DEVICE_NORMAL));
    ZyxCreateDir(destFile);
    sprintf(destFile,"%c:\\ZYXAPP\\menu\\",ZyxGetDrive(DEVICE_NORMAL));
    ZyxCreateDir(destFile);
    sprintf(destFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_NORMAL),menu_xml_file);
    hFile = ZyxFileOpen(destFile,ZFS_MODE_READ|ZFS_MODE_WRITE|ZFS_MODE_CREATE);
    if (hFile){
	ZyxFileWrite(hFile, (char*)g_menu_xml, sizeof(g_menu_xml));
	ZyxFileClose(hFile);
    }

    sprintf(destFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_NORMAL),menu_xml_file);
    hFile = ZyxFileOpen(destFile,ZFS_MODE_READ);
    if (hFile >= 0)
    {
       bXmlExist = TRUE;
       ZyxFileClose(hFile);
    }
    else
    {
       sprintf(destFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_TCARD),menu_xml_file);
       hFile = ZyxFileOpen(destFile,ZFS_MODE_READ);
       if (hFile >= 0)
       {
           char newFile[128]={0};

           sprintf(newFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_NORMAL),menu_xml_file);
           ZyxFileRename(destFile, newFile);
           bXmlExist = TRUE;
           ZyxFileClose(hFile);                
       }
    }   


    return bXmlExist;
}

BOOL ParseMenuItems()
{
    char *utf8,*pBegin,*pEnd,buf[URL_LEN];
    ZFILE handle,fp;
    long length;
    MENU_DATA menu_data;
    char destFile[128];

    g_total_items = 0;	

    sprintf(destFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_NORMAL),menu_xml_file);
    handle = ZyxFileOpen(destFile,ZFS_MODE_READ);
    if(handle<0)
    {
        return FALSE;
    }

    length = ZyxFileSeek(handle, 0, ZFS_END);
    utf8 = (char*)ZyxAlloc(length+1);
    if ( utf8 ) 
    {
        sprintf(destFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_NORMAL),MEMU_DATA_FILE);	
        fp = ZyxFileOpen(destFile,ZFS_MODE_WRITE);
        if (fp < 0)
            fp = ZyxFileOpen(destFile,ZFS_MODE_CREATE);	

        if (fp < 0)
            return FALSE;
        
        memset(utf8, 0, length+1);
        ZyxFileSeek(handle, 0, ZFS_BEGIN);	
        ZyxFileRead(handle, utf8, length);
        
        pBegin =utf8;
        while(1)
        {
            memset((void *)&menu_data, 0, sizeof(menu_data));
            pBegin = strstr(pBegin,"<name>");
            if (pBegin)
            {
                pBegin += 6;
                pEnd = strstr(pBegin,"</name>");
            }
            else
                break;
            length = pEnd-pBegin;
            if (length >= 20) length = 19;
            strncpy(buf,pBegin,length);
            pBegin = pEnd;
            buf[length] = 0;
            ZyxUTF8ToUCS((char*)buf, (U16*)menu_data.AppName,20);
            
            pBegin = strstr(pBegin,"<ename>");
            if (pBegin)
            {
                pBegin += 7;
                pEnd = strstr(pBegin,"</ename>");
            }
            else
                break;
            length = pEnd-pBegin;
            if (length >= 32) length = 32-1;
            strncpy(buf,pBegin,length);
            pBegin = pEnd;
            buf[length] = 0;
            strcpy((char*)menu_data.AppEngName,buf);
            
            pBegin = strstr(pBegin,"<binurl>");
            if (pBegin)
            {
                pBegin += 8;
                pEnd = strstr(pBegin,"</binurl>");
            }
            else
                break;
            length = pEnd-pBegin;
            if (length >= URL_LEN) length = URL_LEN-1;
            strncpy(buf,pBegin,length);
            pBegin = pEnd;
            buf[length] = 0;
            strcpy((char*)menu_data.BinUrl,buf);
            
            ZyxFileWrite(fp,(char*)&menu_data,sizeof(menu_data)); 
            memcpy((void*)MenuName[g_total_items],(void*)menu_data.AppName,sizeof(menu_data.AppName));
            
            g_total_items ++;
        }
        ZyxFree(utf8);
        ZyxFileClose(fp);
    }
    ZyxFileClose(handle);

    if (g_total_items ==0)
        return FALSE;
    else
        return TRUE;
}

int MenuOnDownCb(int result,char *outFileName)
{
  if (result == 100)
  {
        Zyx_Dummy_Entry();
  }
  else if (result > 300)
  {
       GoBackHistory();
       ZyxShowPopMsg(connect_net_fail,POPUP_ERROR);
  }
  return 1;
}
 
void DownMenuFile()
{
    char destFile[128],strURL[128], buf[10]={0};
    ZyxUserInfo user_info;
				
    ZyxGetUserinfo(&user_info);
    sprintf(destFile,"%c:\\ZYXAPP\\menu\\%s",ZyxGetDrive(DEVICE_NORMAL),menu_xml_file);
    
    strncpy(buf,user_info.UpdateSerial,7);
    sprintf(strURL,MENU_URL,user_info.factory,user_info.modelid,buf,user_info.MidVer);	

    ZyxHttpDownload(strURL,destFile,MenuOnDownCb);
}

int ExeOnDownCb(int result,char *outFileName)
{
   if (result < 100)
  {
        Zyx_GuiDisplayProgress(result);
  }
  else if (result == 100)
  {
        GoBackHistory();
        Zyx_App_Entry();
  }
  else if (result > 300)
  {
       GoBackHistory();
       ZyxShowPopMsg(connect_net_fail,POPUP_ERROR);
  }
  return 1;
}

void DownExeFile()
{
    Zyx_GuiDisplayProgress(0);
    ZyxHttpDownload(BinFileUrl,ZyxAppFullPath,ExeOnDownCb);
}

#endif
#if 0
int kk_select_sim_temp = 0;
void ZyxSelectSimHilitHdr(S32 index) 
{ 
    kk_select_sim_temp = index; 
} 

void ZyxSelectSimLSKHdr(void) 
{ 
    kk_select_sim = kk_select_sim_temp; 

#ifndef WIN32 
    NET_PrepareConnect(); 
#endif 
} 

void KKEntrySelectSim(void) 
{ 
    E_MTPNP_AD_SIMCARD_USABLE_SIDE simStatus; 

    simStatus = MTPNP_AD_Get_UsableSide(); 
    
    if (MTPNP_AD_DUALSIM_USABLE == simStatus) 
    { 
            whichSim = kk_select_sim ; 
    } 
    else if (MTPNP_AD_SIMCARD1_USABLE == simStatus) 
    { 
            whichSim = 0; 
    } 
    else 
    { 
            whichSim = 1; 
    } 

            
    if (MTPNP_AD_DUALSIM_USABLE == simStatus) 
            KKEntrySelectSim(); 
    else 
            NET_PrepareConnect(); 
}


void KKEntrySelectSim(void) 
{
    U8 *guiBuffer; 
            
    U8*                PopUpList[2]; 

    PopUpList[0] = GetString(STRING_MTPNP_SIM1); 
    PopUpList[1] = GetString(STRING_MTPNP_SIM2); 
              
    screenTopId = GetActiveScreenId(); 
                    
    EntryNewScreen(SCR_ID_VIDEOCHAT_MENU, NULL, KKEntrySelectSim, 0); 
    
    guiBuffer = GetCurrGuiBuffer(SCR_ID_VIDEOCHAT_MENU); 

    if (guiBuffer == NULL) 
    {
		kk_select_sim_temp = 0;//kk_debug_type; 
    } 
    
    RegisterHighlightHandler(ZyxSelectSimHilitHdr); 
    
    ShowCategory353Screen(0, 0, 
            STR_GLOBAL_OK, 0, 
            STR_GLOBAL_BACK, 0, 
            2,//sizeof(strItems)/sizeof(strItems[0]), 
            (U8 **)PopUpList ,//strItems, 
            (U16*)gIndexIconsImageList, 
            0, 
            0, 
            kk_select_sim_temp, 
            guiBuffer); 

    SetLeftSoftkeyFunction(ZyxSelectSimLSKHdr, KEY_EVENT_UP); 
    SetRightSoftkeyFunction(GoBackHistory, KEY_EVENT_UP); 
} 
#endif
