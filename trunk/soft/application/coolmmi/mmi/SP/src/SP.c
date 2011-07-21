/*******************************************************************************
 * Filename:
 * ---------
 *	SP.c
 *
 * Project:
 * --------
 *   
 *
 * Description:
 * ------------
 *   This file is for SP.
 *
 * Author:
 *   niej
 *
 ********************************************************************************/
#ifdef __SP__
#include "sp.h"
#include "eventsgprot.h"
#include "mmi_trace.h"
#include "globalmenuitems.h"
#include "globaldefs.h"
#include "globalconstants.h"
#include "historygprot.h"
#include "smsapi.h"
#include "messagesmiscell.h"
#include "commonscreens.h"
#include "messagesresourcedata.h"
#include "globaldefs.h"
#include "wgui_categories_popup.h"
#include "messagesexdcl.h"
#include "mainmenudef.h"
#include "globaldefs.h"
#include "ems.h"
#include "wgui_categories_inputsenum.h"
#include "settingprofile.h"
#include "sscstringhandle.h"
#include "smsguiinterfacetype.h"
#include "smsal_l4c_enum.h"

Sp_Scr_Info sp_level1_scr;
U8 sp_level_2_index = 0;
U8 sp_level_3_service_index = 0;
SP_Network_Type sp_network_name;

const SP_SMS_Service_Code_Struct SP_New_World_Table[] = 
{ /*Level2 Menu id                             order         cost       subscribe     explain*/
  {MENU_ID_SP_NEW_WORLD_JOKE,                 "LTNZ40001",  "YWZF309",  "",           43552},
  {MENU_ID_SP_NEW_WORLD_THUMB_FUN,            "PINZ40001",  "43574",    "",           43553},
  {MENU_ID_SP_NEW_WORLD_FOCUS_NEWS,           "KBXW40001",  "YWZF325",  "X40001",     43554},
  {MENU_ID_SP_NEW_WORLD_COUPLE_TALK_IN_NIGHT, "KBJK40001",  "YWZF307",  "JK40001",    43555},
  {MENU_ID_SP_NEW_WORLD_SOUND_COLOR_EXP_HEART,"",           "YWZF306",  "JKNZ40001",  43556},
  {MENU_ID_SP_NEW_WORLD_PUZZLE,               "YXLY40001",  "YWZF318",  "LY40001",    43557},
  {MENU_ID_SP_NEW_WORLD_LUCKY_SAME_CITY,      "",           "YWZF314",  "ZQ40001",    43558},
  {MENU_ID_SP_NEW_WORLD_GYM_NEWS,             "KBXWTY40001","YWZF326",  "SST40001",   43559},
  {MENU_ID_SP_NEW_WORLD_LOVE_GAMES,           "YXQG40001",  "YWZF327",  "CXYX40001",  43560},
  {MENU_ID_SP_NEW_WORLD_CHALLENGER,           "TZHNZ40001", "43574",    "",           43561},
  {MENU_ID_SP_NEW_WORLD_LOVE_CAMPUS,          "",           "YWZF328",  "JJXY40001",  43562},
  {MENU_ID_SP_NEW_WORLD_SMS_NOVEL,            "WW40001",    "YWZF329",  "MJ40001",    43563},
  {MENU_ID_SP_NEW_WORLD_FREE_EXPERIENCE,      "",           "YWZF320",  "MFNZ40001",  43564},
  {MENU_ID_SP_NEW_WORLD_CUSTOMER_SERVICE,     "0000",       "",         "02283719696",NULL},
};                                           /*cancel*/            /*call customer service*/         

const SP_SMS_Service_Code_Struct SP_Fun_Everyday_Table[] = 
{ /*Level2 Menu id                              order         cost        subscribe     explain*/
  {MENU_ID_SP_FUN_EVERYDAY_SMS_EXP_AFFECTION,   "LTCQ40001",  "YWZF308",  "",           43565},
  {MENU_ID_SP_FUN_EVERYDAY_WHITE_COLLAR_SECRET, "BAI40001",   "YWZF330",  "",           43566},
  {MENU_ID_SP_FUN_EVERYDAY_JOY_TEST,            "CSNZ40001",  "43574",    "",           43567},
  {MENU_ID_SP_FUN_EVERYDAY_SWEET_WORD,          "HBNZ40001",  "43573",    "LSGS40001",  43568},
  {MENU_ID_SP_FUN_EVERYDAY_GUESS_WIN,           "DANZ40001",  "43574",    "TPZC40001",  43569},
  {MENU_ID_SP_FUN_EVERYDAY_FUN_NEWS,            "KBXWYL40001","YWZF331",  "YL40001",    43570},
  {MENU_ID_SP_FUN_EVERYDAY_SUPER_BRAIN_POWER,   "",           "YWZF319",  "ZNT40001",   43571},
  {MENU_ID_SP_FUN_EVERYDAY_WUSHU_COMPETE,       "KBI40001",   "YWZF322",  "ZB40001",    43572},
  {MENU_ID_SP_FUN_EVERYDAY_CUSTOMER_SERVICE,    "0000",       "",         "02283719696",NULL}, 
};                                               

const SP_SMS_Service_Code_Struct SP_Fun_Interact_CT_Table[] = 
{
  /*Level2 Menu id                              order   cost  subscribe       explain*/
  {MENU_ID_SP_FUN_INTERACT_CT_WORD_NIGHT,       "",     "",   "125905925383", NULL},
  {MENU_ID_SP_FUN_INTERACT_CT_EXTRACT_LIFE,     "",     "",   "125905926383", NULL},
  {MENU_ID_SP_FUN_INTERACT_CT_ART_CITY,         "",     "",   "125905928383", NULL},
  {MENU_ID_SP_FUN_INTERACT_CT_THEATER,          "",     "",   "125905931383", NULL},
  {MENU_ID_SP_FUN_INTERACT_CT_MUSIC_INTER,      "",     "",   "125905939383", NULL},
  {MENU_ID_SP_FUN_INTERACT_CT_FREE_PIC_RING,    "",     "",   "125905923383", NULL},    
  {MENU_ID_SP_FUN_INTERACT_CT_ORIGINAL_PERSON,  "",     "",   "125905920383", NULL},
  {MENU_ID_SP_FUN_INTERACT_CT_LOVE_GAME,        "",     "",   "125905924383", NULL},
  {MENU_ID_SP_FUN_INTERACT_CT_GAME_WORLD,       "",     "",   "125905927383", NULL},
  {MENU_ID_SP_FUN_INTERACT_CT_CAR_CLUB,         "",     "",   "125905922383",  NULL}, 
};

const SP_SMS_Service_Code_Struct SP_Fun_Interact_CU_Table[] = 
{
  /*Level2 Menu id                                 order   cost   subscribe           explain*/
  {MENU_ID_SP_FUN_INTERACT_CU_PIC_RING_WORLD,       "",     "",   "10159087101383",   NULL},
  {MENU_ID_SP_FUN_INTERACT_CU_SKY_AFFECTION,        "",     "",   "10159087102383",   NULL},
  {MENU_ID_SP_FUN_INTERACT_CU_BISEXUAL_SCIENCE,     "",     "",   "10159087303383",   NULL},
  {MENU_ID_SP_FUN_INTERACT_CU_RING_SORT,            "",     "",   "10159087223383",   NULL},
  {MENU_ID_SP_FUN_INTERACT_CU_ORIGINAL_PERSON,      "",     "",   "10159087301383",   NULL},
  {MENU_ID_SP_FUN_INTERACT_CU_MUSIC_INDEFINITE,     "",     "",   "10159087103383",   NULL},
  {MENU_ID_SP_FUN_INTERACT_CU_TOP_PREVAIL,          "",     "",   "10159087104383",   NULL},
  {MENU_ID_SP_FUN_INTERACT_CU_HEALTH_LIFE,          "",     "",   "10159087302383",   NULL},
  {MENU_ID_SP_FUN_INTERACT_CU_STAR_BLESS,           "",     "",   "10159087221383",   NULL}, 
  {MENU_ID_SP_FUN_INTERACT_CU_ORDER_SONG_TRUE_FEEL, "",     "",   "10159087222383",   NULL},
}; 

SP_Level1_2_Level2 Level1_Level2_Table[]=
{
  {MENU_ID_SP_NEW_WORLD,        14,     SP_New_World_Table},
  {MENU_ID_SP_FUN_EVERYDAY,     9,      SP_Fun_Everyday_Table},
  {MENU_ID_SP_FUN_INTERACT_CT,  10,     SP_Fun_Interact_CT_Table},
  {MENU_ID_SP_FUN_INTERACT_CU,  10,     SP_Fun_Interact_CU_Table},
};
extern S8 gHomePLMNNum[MMI_SIM_NUMBER][MAX_PLMN_LEN_MMI+1];

static void Highlight_Mobile_Eden(void);
static void Highlight_New_World(void);
static void Highlight_Fun_Everyday(void);
static void Highlight_Fun_Interact_CT(void);
//static void Highlight_Fun_Interact_CU(void);
static void Entry_Fun_Interact_CU(void);
static void Highlight_SP_Order(void);
static void Highlight_SP_Subscribe(void);
static void Highlight_SP_Cost(void);
static void Highlight_SP_Explain(void);
static void Highlight_SP_Cancel(void);
static void Highlight_SP_Call_Customer_Service(void);
static void Highlight_SP_MenuItem(void (*func)(void));


static void Entry_MobileEden(void);
static void Draw_MobileEdenScreen(void);
static void Entry_SP_Level1(void);
static void SP_Level_2_Get_Index (S32 nIndex);
static void SP_Pre_Entry_Level2(void);
static void Entry_SP_Level2(void);

static void Sp_Make_Call (void);
static void SP_Entry_Make_Call (void);
static void Sp_Exit_Make_Call (void);
static void Sp_Send_Msg_Rsp(void* number, module_type mod, U16 result);
static void Entry_SP_Msg_Service (void);
static BOOL SP_Get_Level2_Service_Code(U16 Level1_MenuId, U16 Level2_MenuId,  SP_SMS_Service_Code_Struct *Sp_Service_Code);
static void Sp_Entry_Explain_Screen(U16 explain_str_id);
static void SP_EDEN_GoBackHistory(void);
static void SP_Level1_GoBackHistory(void);
static int N_Multiplication(int base, int index);
static void Sp_Exit_Explain_Screen(void);
static void Get_Expalin_Screen_Inputbuffer(U8* inputBuffer);

/*****************************************************************************

	FUNCTION NAME		: void Init_SP(void)

  	PURPOSE				: Set HiliteHandler for SP.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Init_SP(void)
{
  mmi_trace(g_sw_SP,"niej InitSP");
  sp_network_name = SP_NETWORK_OTHER;
  
  SetHiliteHandler(MENU_ID_SP_MOBILE_EDEN, Highlight_Mobile_Eden);
  
  SetHiliteHandler(MENU_ID_SP_NEW_WORLD, Highlight_New_World);
  SetHiliteHandler(MENU_ID_SP_FUN_EVERYDAY, Highlight_Fun_Everyday);
  SetHiliteHandler(MENU_ID_SP_FUN_INTERACT_CT, Highlight_Fun_Interact_CT);
  
  SetHiliteHandler(MENU_ID_SP_ORDER, Highlight_SP_Order);
  SetHiliteHandler(MENU_ID_SP_SUBSCRIBE,Highlight_SP_Subscribe);
  SetHiliteHandler(MENU_ID_SP_COST,Highlight_SP_Cost);
  SetHiliteHandler(MENU_ID_SP_EXPLAIN,Highlight_SP_Explain);
  SetHiliteHandler(MENU_ID_SP_CANCEL,Highlight_SP_Cancel);
  SetHiliteHandler(MENU_ID_SP_CALL_CUSTOMER_SERVICE,Highlight_SP_Call_Customer_Service);  
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_Mobile_Eden(void)

  	PURPOSE				: Set HiliteHandler for Mobile Paradise menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_Mobile_Eden(void)
{	
  Highlight_SP_MenuItem(Entry_MobileEden);
}


/*****************************************************************************

	FUNCTION NAME		: void Highlight_New_World(void)

  PURPOSE				: Set HiliteHandler for New Style World menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_New_World(void)
{	
    mmi_trace(g_sw_SP,"niej Highlight_New_World");
  	sp_level1_scr.sp_menu_id = MENU_ID_SP_NEW_WORLD;
  	sp_level1_scr.sp_scr_id = SCR_SP_NEW_WORLD;
  	Highlight_SP_MenuItem(Entry_SP_Level1);
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_Fun_Everyday(void)

  PURPOSE				: Set HiliteHandler for Fun Daily  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_Fun_Everyday(void)
{
	sp_level1_scr.sp_menu_id = MENU_ID_SP_FUN_EVERYDAY;
	sp_level1_scr.sp_scr_id = SCR_SP_FUN_EVERYDAY;
	Highlight_SP_MenuItem(Entry_SP_Level1);
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_Fun_Interact_CT(void)

  PURPOSE				: Set HiliteHandler for Zone of Fun Tone  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_Fun_Interact_CT(void)
{
	sp_level1_scr.sp_menu_id = MENU_ID_SP_FUN_INTERACT_CT;
	sp_level1_scr.sp_scr_id = SCR_SP_FUN_INTERACT_CT;
	Highlight_SP_MenuItem(Entry_SP_Level1);
}

/*****************************************************************************

	FUNCTION NAME		: void void Highlight_Fun_Interact_CU(void)

  PURPOSE				: Set HiliteHandler for Zone of Fun Tone  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Entry_Fun_Interact_CU(void)
{
  mmi_trace(g_sw_SP,"niej Entry_Fun_Interact_CU");
	sp_level1_scr.sp_menu_id = MENU_ID_SP_FUN_INTERACT_CU;
 	sp_level1_scr.sp_scr_id = SCR_SP_FUN_INTERACT_CU;

  Entry_SP_Level1();
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_SP_Order(void)

  PURPOSE				: Set HiliteHandler for Request  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_SP_Order(void)
{
  mmi_trace(g_sw_SP,"niej Highlight_SP_Order");   
  sp_level_3_service_index = SP_SERVICE_ORDER;
  Highlight_SP_MenuItem(Entry_SP_Msg_Service);
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_SP_Subscribe(void)

  PURPOSE				: Set HiliteHandler for Subscription  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_SP_Subscribe(void)
{
   mmi_trace(g_sw_SP,"niej Highlight_SP_Subscribe");
   sp_level_3_service_index = SP_SERVICE_SUBSCRIBE;
   Highlight_SP_MenuItem(Entry_SP_Msg_Service);
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_SP_Cost(void)

  PURPOSE				: Set HiliteHandler for Price Explanation  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_SP_Cost(void)
{
   mmi_trace(g_sw_SP,"niej Highlight_SP_Cost");
   sp_level_3_service_index = SP_SERVICE_COST;
   Highlight_SP_MenuItem(Entry_SP_Msg_Service);
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_SP_Explain(void)

  PURPOSE				: Set HiliteHandler for Description  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_SP_Explain(void)
{
   mmi_trace(g_sw_SP,"niej Highlight_SP_Explain");
   sp_level_3_service_index = SP_SERVICE_EXPLAIN;
   Highlight_SP_MenuItem(Entry_SP_Msg_Service);
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_SP_Call_Customer_Service(void)

  PURPOSE				: Set HiliteHandler for call customer service  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_SP_Call_Customer_Service(void)
{
   mmi_trace(g_sw_SP,"niej Highlight_SP_Call_Customer_Service");
   sp_level_3_service_index = SP_SERVICE_CUSTOMER;
   Highlight_SP_MenuItem(Entry_SP_Msg_Service);
}

/*****************************************************************************

	FUNCTION NAME		: void Highlight_SP_Cancel(void)

  PURPOSE				: Set HiliteHandler for cancel  menuitem.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Highlight_SP_Cancel(void)
{
   mmi_trace(g_sw_SP,"niej Highlight_SP_Explain");
   sp_level_3_service_index = SP_SERVICE_CANCEL;
   Highlight_SP_MenuItem(Entry_SP_Msg_Service);
}

/*****************************************************************************

	FUNCTION NAME		: void Entry_SP_Level1(void)

  PURPOSE		: From level1 go to next screen .
              The association between entry function and menu as following
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |                   |             |
                              ->New Style World                 |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...
                          <-----              <-----
                   SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory
                     
              eg. when into Mobile Paradise ->New Style World->, this function will be
                  called.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void Entry_SP_Level1(void)
{	
	U16 nStrItemList[SP_MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem; 	 /* Stores no of children in the submenu*/		
	U8* guiBuffer;		 /* Buffer holding history data */
	mmi_trace(g_sw_SP,"niej Entry_SP_Level1 sp_level1_scr.sp_scr_id[%d],sp_level1_scr.sp_menu_id[%d]",
            sp_level1_scr.sp_scr_id, sp_level1_scr.sp_menu_id);
  
	EntryNewScreen(sp_level1_scr.sp_scr_id, NULL, Entry_SP_Level1, NULL);	
	
	guiBuffer = GetCurrGuiBuffer(sp_level1_scr.sp_scr_id); 	
  
	nNumofItem = GetNumOfChild(sp_level1_scr.sp_menu_id);
	GetSequenceStringIds_Ext(sp_level1_scr.sp_menu_id, nStrItemList);
	SetParentHandler(sp_level1_scr.sp_menu_id);

	RegisterHighlightHandler (SP_Level_2_Get_Index);
	ShowCategory52Screen(GetStringIdOfItem(sp_level1_scr.sp_menu_id),
						  MAIN_MENU_TITLE_FUNANDGAMES_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,(PU16)gIndexIconsImageList,
						  NULL,
						  0,0,guiBuffer);

	SetLeftSoftkeyFunction (SP_Pre_Entry_Level2, KEY_EVENT_UP);
	SetKeyHandler (SP_Pre_Entry_Level2, KEY_RIGHT_ARROW, KEY_EVENT_DOWN);
	
	SetRightSoftkeyFunction(SP_Level1_GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(SP_Level1_GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************

	FUNCTION NAME		: void SP_Level1_GoBackHistory(void)

  PURPOSE		: The association between entry function and menu as following
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |                   |             |
                              ->New Style World                 |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...

                           <-----              <-----
                    SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory
                                                      
              eg. when backe to Mobile Paradise from Mobile Paradise ->New Style World, 
              this function will be called.

	INPUT PARAMETERS	: void

	OUTPUT PARAMETERS	: void

	RETURNS				: void

 

******************************************************************************/
void SP_Level1_GoBackHistory(void)
{  
  sp_level_2_index = 0;
  GoBackHistory();  
}

/*****************************************************************************

	FUNCTION NAME		: SP_Get_Level2_Service_Code

  PURPOSE				: get the service code .

 
	 

	OUTPUT PARAMETERS	: Sp_Service_Code: include the service code(Request, Subscrib...)

	RETURNS				: TRUE is Successful, FALSE is failure

 
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |        |          |    |        |
                              ->New Style World        |        |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...

                           <-----              <-----
                  SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory

******************************************************************************/
BOOL SP_Get_Level2_Service_Code(U16 Level1_MenuId, U16 Level2_MenuId, SP_SMS_Service_Code_Struct *Sp_Service_Code)
{
  int i = 0;
  int j = 0;
  if(!Sp_Service_Code)
  {
    mmi_trace(g_sw_SP,"niej Sp_Service_Code is NULL");
    return FALSE;
  }
  for(i=0; i<(sizeof(Level1_Level2_Table)/sizeof(Level1_Level2_Table[0])); i++)
  {
    if(Level1_Level2_Table[i].Level1_Menu_Id == Level1_MenuId)
    {
      for(j=0; j<Level1_Level2_Table[i].Num_Of_Submenu;j++)
      {
        if(Level1_Level2_Table[i].SubMenu_Service_Code[j].Level2_Menu_Id == Level2_MenuId)
        {
          memcpy(Sp_Service_Code,&Level1_Level2_Table[i].SubMenu_Service_Code[j], 
                         sizeof(Level1_Level2_Table[i].SubMenu_Service_Code[j]));
          return TRUE;
        }
      }
    }
  }
  
  return FALSE;

}

/*****************************************************************************

	FUNCTION NAME		: Entry_SP_Level2

  PURPOSE	: From level2 go to next screen .
            SP_Pre_Entry_Level2 will do some preparation for SP_Pre_Entry_Level2.

	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 

 
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |        |          |    |        |
                              ->New Style World        |        |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...

                           <-----              <-----
                    SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory

******************************************************************************/
void Entry_SP_Level2(void)
{	
	U16 nStrItemList[SP_MAX_SUB_MENUS]; /* Stores the strings id of submenus returned */
	U16 nNumofItem = 0; 	 /* Stores no of children in the submenu*/		
	U8* guiBuffer = NULL;		 /* Buffer holding history data */
 
	U16 ScrId = sp_level1_scr.sp_scr_id+sp_level_2_index+1;
  U16 MenuId = sp_level1_scr.sp_menu_id+sp_level_2_index+1;
  
	EntryNewScreen(ScrId, NULL, Entry_SP_Level2, NULL);	
	
	guiBuffer = GetCurrGuiBuffer(ScrId); 
  nNumofItem = GetNumOfChild(MenuId);
	GetSequenceStringIds_Ext(MenuId, nStrItemList);

	SetParentHandler(MenuId);
	RegisterHighlightHandler(ExecuteCurrHiliteHandler);	
	ShowCategory52Screen(GetStringIdOfItem(MenuId),
						  MAIN_MENU_TITLE_FUNANDGAMES_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,(PU16)gIndexIconsImageList,
						  NULL,
						  0,0,guiBuffer);
	
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************

	FUNCTION NAME		: SP_Pre_Entry_Level2

  PURPOSE	: From level2 go to next screen .
            SP_Pre_Entry_Level2 will do some preparation for SP_Pre_Entry_Level2.
            if you want go to the next screen from Mobile Paradise->New Style World->Joke,
            this function will be used.
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 

 
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |        |          |    |        |
                              ->New Style World        |        |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...

                           <-----              <-----
                   SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory

******************************************************************************/
void SP_Pre_Entry_Level2(void)
{
  mmi_trace(g_sw_SP,"niej Entry_SP_Level2");
  U16 nNumofItem; 
  U16 MenuId = sp_level1_scr.sp_menu_id+sp_level_2_index+1;
  nNumofItem = GetNumOfChild(MenuId);
  if(nNumofItem>0)
  {
    Entry_SP_Level2();
  }
  else
  {
    SP_Entry_Make_Call();
  }
}

/*****************************************************************************

	FUNCTION NAME		: SP_Level_2_Get_Index

  PURPOSE	:get the index of menuitem hilighted in level2, 
	INPUT PARAMETERS	: nIndex
	OUTPUT PARAMETERS	: void

	RETURNS				: 

 
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |        |          |    |        |
                              ->New Style World        |        |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...

                           <-----              <-----
                   SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory
******************************************************************************/
void SP_Level_2_Get_Index (S32 nIndex)
{
  mmi_trace(g_sw_SP,"niej sp_level_2_index[%d]", nIndex);
	sp_level_2_index	=	(U8)nIndex;
}


/*****************************************************************************

	FUNCTION NAME		: Entry_MobileEden

  PURPOSE	:From Mobile Paradise into the next screen.           
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 
 
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |        |          |    |        |
                              ->New Style World        |        |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...

                           <-----              <-----
                  SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory
******************************************************************************/
void Entry_MobileEden(void)
{
  mmi_trace(g_sw_SP,"niej SP_BASE[%d]", SP_BASE);
  /*get network name*/
  if((strncmp(gHomePLMNNum[0], "46000", 5)==0)
    ||(strncmp(gHomePLMNNum[0], "46002", 5)==0))
  {
    sp_network_name = SP_NETWORK_CT;
  }
  else if(strncmp(gHomePLMNNum[0], "46001", 5)==0)
  {
    sp_network_name = SP_NETWORK_CU;
  }
  else
  {
    sp_network_name = SP_NETWORK_OTHER;
  }

  if(sp_network_name == SP_NETWORK_OTHER)
  {
    DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, NULL);		
  }
  else
  {
    Draw_MobileEdenScreen();
  }
}

/*****************************************************************************

	FUNCTION NAME		: Draw_MobileEdenScreen

  PURPOSE	:draw Level1.           
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 

 
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |        |          |    |        |
                              ->New Style World        |        |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...

                           <-----              <-----
                   SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory

******************************************************************************/
void Draw_MobileEdenScreen(void)
{	
	U16 nStrItemList[5];  
	U16 nTempStrItemList[5];
	int i = 0;
	U16 nNumofItem = 0; 	 /* Stores no of children in the submenu*/	
	U8* guiBuffer = NULL;		 /* Buffer holding history data */
	
  
  mmi_trace(g_sw_SP,"niej Draw_MobileEdenScreen");

	EntryNewScreen(SCR_SP_MOBILE_EDEN, NULL, Draw_MobileEdenScreen, NULL);
	
	guiBuffer = GetCurrGuiBuffer(SCR_SP_MOBILE_EDEN); 	
	GetSequenceStringIds_Ext(MENU_ID_SP_MOBILE_EDEN,nTempStrItemList);
	/*in the submenu of MENU_ID_SP_MOBILE_EDEN, 
	the first 3 items support China Telecom, the last item support China Unicom*/

  if(sp_network_name == SP_NETWORK_CU)
	{
    mmi_trace(g_sw_SP,"niej sp_network_name is SP_NETWORK_CU");
		nNumofItem = 1;
		nStrItemList[0] = nTempStrItemList[3];   
   // SetLeftSoftkeyFunction(Entry_Fun_Interact_CU,KEY_EVENT_UP);
	}
	else if (sp_network_name == SP_NETWORK_CT)
	{
    mmi_trace(g_sw_SP,"niej sp_network_name is SP_NETWORK_CT nNumofItem[%d]", nNumofItem);
		nNumofItem = 3;
		for(i=0; i<nNumofItem; i++)
		{
			nStrItemList[i] = nTempStrItemList[i];
      mmi_trace(g_sw_SP,"niej nStrItemList[%d]:[%d]",i, nStrItemList[i]);
		}
    RegisterHighlightHandler(ExecuteCurrHiliteHandler);
	}
	else
	{
		mmi_trace(g_sw_SP,"niej sp_network_name[%d] can't support this function", sp_network_name);
	}	

	SetParentHandler(MENU_ID_SP_MOBILE_EDEN);	

	mmi_trace(g_sw_SP,"niej ShowCategory52Screen ");
	ShowCategory52Screen(STR_MOBILE_EDEN,MAIN_MENU_TITLE_FUNANDGAMES_ICON,
						  STR_GLOBAL_OK, IMG_GLOBAL_OK,
						  STR_GLOBAL_BACK, IMG_GLOBAL_BACK,
						  nNumofItem,nStrItemList,(PU16)gIndexIconsImageList,
						  NULL,
						  0,0,guiBuffer);
  
   if(sp_network_name == SP_NETWORK_CU)
   {
      SetLeftSoftkeyFunction(Entry_Fun_Interact_CU,KEY_EVENT_UP);
   }
	
	SetRightSoftkeyFunction(SP_EDEN_GoBackHistory,KEY_EVENT_UP);
	SetKeyHandler(SP_EDEN_GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);

}

/*****************************************************************************

	FUNCTION NAME		: Entry_MobileEden

  PURPOSE	:From Mobile Paradise back to  the previous screen.           
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 
 
  
                         Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2
                              |                   |             |
                              |    /Level1/       | /Level2/    |
              Mobile Paradise |        |          |    |        |
                              ->New Style World        |        |
                                                ---->Joke
                                                           -------->Request
                                                           -------->Price Explanation
                                                      ...

                           <-----              <-----
                  SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory
******************************************************************************/
void SP_EDEN_GoBackHistory(void)
{
  mmi_trace(g_sw_SP,"niej SP_EDEN_GoBackHistory");
  sp_level1_scr.sp_menu_id = MENU_ID_SP_NEW_WORLD;
  sp_level1_scr.sp_scr_id = SCR_SP_NEW_WORLD;
  GoBackHistory();
}


void Highlight_SP_MenuItem(void (*func)(void))
{
	ChangeLeftSoftkey(STR_GLOBAL_OK, IMG_GLOBAL_OK);
	ChangeRightSoftkey(STR_GLOBAL_BACK,IMG_GLOBAL_BACK);
	SetKeyHandler(GoBackHistory, KEY_LEFT_ARROW, KEY_EVENT_DOWN);	
	SetLeftSoftkeyFunction(func,KEY_EVENT_UP);
	SetKeyHandler(func, KEY_RIGHT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);
}

/*****************************************************************************

	FUNCTION NAME		: Entry_SP_Msg_Service

  PURPOSE	:From Mobile Paradise back to  the previous screen.           
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 
 
  
             Entry_MobileEden   Entry_SP_Level1  SP_Pre_Entry_Level2  Entry_SP_Msg_Service
                  |                   |             |                    |
                  |    /Level1/       | /Level2/    |                    |
  Mobile Paradise |        |          |    |        |                    |
                  ->New Style World        |        |                    |
                                    ---->Joke                            |
                                               -------->Request          |
                                               
                                                               ------------->
                                          ...

               <-----              <-----
      SP_EDEN_GoBackHistory  SP_Level1_GoBackHistory
******************************************************************************/
void Entry_SP_Msg_Service (void)
{
	S8			textbuf[SP_CODE_LENGTH*ENCODING_LENGTH];
	EMSData*	pEMS;
	byte			result;
	history_t		temphistory;
	U8 service = sp_level_3_service_index;	
	S8 codeNum[SP_LONG_CODE_LENGTH];  
  S8 dest_number[12]= "10664321";//now only china Mobile has SP, the destination number is only one.
  SP_SMS_Service_Code_Struct sp_level2_service_code;
  int i=0;
  mmi_trace(g_sw_SP,"niej service[%d]", service);
  
	/****************phone number********************/
	memset(textbuf, 0, SP_CODE_LENGTH*ENCODING_LENGTH);
	memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
	AnsiiToUnicodeString((S8*)textbuf, (S8*)dest_number);
	pfnUnicodeStrcat(g_msg_cntx.smsPhoneNumber, textbuf);	
  mmi_trace(g_sw_SP,"niej 1textbuf[%s]", textbuf);
  mmi_trace(g_sw_SP,"niej g_msg_cntx.smsPhoneNumber[%s]", g_msg_cntx.smsPhoneNumber);
		
	/*Service code*/
	ReleaseEMSEditBuffer();
	memset(textbuf, 0, SP_CODE_LENGTH*ENCODING_LENGTH);	
	memset(codeNum, 0, SP_LONG_CODE_LENGTH-1); 
  SP_Get_Level2_Service_Code(sp_level1_scr.sp_menu_id, 
                             sp_level1_scr.sp_menu_id+sp_level_2_index+1,
                             &sp_level2_service_code);
	switch(service)
	{
		case SP_SERVICE_SUBSCRIBE:
			memcpy(codeNum, sp_level2_service_code.Code_Subscribe, SP_LONG_CODE_LENGTH-1);				
			break;
      
		case SP_SERVICE_ORDER:
		  memcpy(codeNum, sp_level2_service_code.Code_Order, SP_LONG_CODE_LENGTH-1);		
			break;
      
    case SP_SERVICE_COST:
      if((MENU_ID_SP_NEW_WORLD_THUMB_FUN == sp_level2_service_code.Level2_Menu_Id)
        ||(MENU_ID_SP_NEW_WORLD_CHALLENGER == sp_level2_service_code.Level2_Menu_Id)
        ||(MENU_ID_SP_FUN_EVERYDAY_JOY_TEST == sp_level2_service_code.Level2_Menu_Id)
        ||(MENU_ID_SP_FUN_EVERYDAY_SWEET_WORD == sp_level2_service_code.Level2_Menu_Id)
        ||(MENU_ID_SP_FUN_EVERYDAY_GUESS_WIN == sp_level2_service_code.Level2_Menu_Id)
        )
      {
        int len = strlen(sp_level2_service_code.Code_Cost);
        int strid = 0;
        for(; i<len; i++)
        {         
          strid += (sp_level2_service_code.Code_Cost[i]-48)*N_Multiplication(10, (len-i-1));   
        }
        mmi_trace(g_sw_SP,"niej strid[%d]",strid);
        
			  Sp_Entry_Explain_Screen(strid);//43519 is id of title in next screen(Price Explanation)
        return;
      }
      else
      {
        memcpy(codeNum, sp_level2_service_code.Code_Cost, SP_LONG_CODE_LENGTH-1);	
      }      	
      break;
      
	  case SP_SERVICE_CANCEL:
      memcpy(codeNum, sp_level2_service_code.Code_Order, SP_LONG_CODE_LENGTH-1);		
			break;

    case SP_SERVICE_EXPLAIN:
			Sp_Entry_Explain_Screen(sp_level2_service_code.Explain_StrId);//43520 is id of title in next screen(Description)
			return;

    case SP_SERVICE_CUSTOMER:
      SP_Entry_Make_Call();
			return;
      
    default:
      mmi_trace(g_sw_SP,"niej invalid service[%d]", service);
      return;
	}
	mmi_trace(g_sw_SP,"niej codeNum[%s]",codeNum);
  memset(textbuf, 0, SP_CODE_LENGTH*ENCODING_LENGTH);
	AnsiiToUnicodeString(textbuf, (S8*)codeNum);  
	mmi_trace(g_sw_SP,"niej 2textbuf[%s]",textbuf);
	memset (& temphistory, 0, sizeof (temphistory));
	GetHistory (SCR_ID_MSG_WRITE, &temphistory);
	pEMS = GetEMSDataForEdit (0, 0);

	result = AppendEMSString_Ext(INPUT_TYPE_ALPHANUMERIC_SENTENCECASE,
					pEMS, (U8*)textbuf, SMSAL_DEFAULT_DCS, temphistory.guiBuffer  );
	
  mmi_trace(g_sw_SP,"niej result[%d]", result);
	if(result)
	{
		mmi_frm_sms_send_struct * sendData=OslMalloc(sizeof(mmi_frm_sms_send_struct));
		memset((S8*)sendData, 0, sizeof(mmi_frm_sms_send_struct));
		memset(sendData->number, 0, SP_MAX_DIGITS_SMS);	
		if(pfnUnicodeStrlen((S8*)g_msg_cntx.smsPhoneNumber))
		UnicodeToAnsii((S8*)sendData->number, (S8*)g_msg_cntx.smsPhoneNumber);

    mmi_trace(g_sw_SP,"niej sendData->number[%s]", sendData->number);
		mmi_frm_sms_send_sms(Sp_Send_Msg_Rsp, MOD_MMI, sendData);
		OslMfree(sendData);
	}
	else
	{
		DisplayPopup ((PU8)GetString (STR_SMS_DOES_NOT_SUPPORT), IMG_GLOBAL_UNFINISHED, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);		
	}

}

/*****************************************************************************

	FUNCTION NAME		: Sp_Send_Msg_Rsp

  PURPOSE	:after send sms, handle the respose           
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 
 
******************************************************************************/
void Sp_Send_Msg_Rsp(void* number, module_type mod, U16 result)
{
	switch(result)
	{
		case MMI_FRM_SMS_OK:
			DisplayPopup ((PU8)GetString(STR_SMS_SEND_SUCCESS), IMG_SEND_SUCCESS_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			mmi_frm_sms_delete_screen_history();
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_ABORT:
			DisplayPopup ((PU8)GetString (STR_ABORT_SENDING_SMS), IMG_GLOBAL_ACTIVATED, 1, MESSAGES_POPUP_TIME_OUT, (U8)SUCCESS_TONE);
			mmi_frm_sms_delete_screen_history();
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_NOTREADY:
			DisplayPopup ((PU8)GetString(STR_SMS_MSG_NOT_READY_YET), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);			
			AlmEnableSPOF();		
		break;
		case MMI_FRM_SMS_RETURN:
		case MMI_FRM_SMS_END:
		break;
		default:
			DisplayPopup ((PU8)GetString(STR_SMS_SEND_FAILED), IMG_SEND_FAIL_PIC_MSG, 1, MESSAGES_POPUP_TIME_OUT, (U8)ERROR_TONE);
			mmi_frm_sms_delete_screen_history();
			AlmEnableSPOF();		
		break;
	}
}


/*****************************************************************************

	FUNCTION NAME		: SP_Entry_Make_Call

  PURPOSE	:when the sp service is IVR, question call or don't call.          
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 
 
******************************************************************************/
void SP_Entry_Make_Call (void)
{
	S8			textbuf[SP_CODE_LENGTH*ENCODING_LENGTH];
	SP_SMS_Service_Code_Struct sp_level2_service_code;
  mmi_trace(g_sw_SP,"niej SP_Entry_Make_Call");
	SP_Get_Level2_Service_Code(sp_level1_scr.sp_menu_id, 
                             sp_level1_scr.sp_menu_id+sp_level_2_index+1,
                             &sp_level2_service_code);
	EntryNewScreen(SCR_ID_MSG_CONFIRMATION, Sp_Exit_Make_Call, NULL, NULL);
	
 
	memset(textbuf, 0, SP_CODE_LENGTH*ENCODING_LENGTH);
	memset(g_msg_cntx.smsPhoneNumber, 0, (MAX_DIGITS+1)*ENCODING_LENGTH);
	AnsiiToUnicodeString(textbuf, (S8*)sp_level2_service_code.Code_Subscribe);
  mmi_trace(g_sw_SP, "niej Code_Subscribe[%s]", sp_level2_service_code.Code_Subscribe);
	pfnUnicodeStrcpy(g_msg_cntx.smsPhoneNumber, textbuf);
  mmi_trace(1, "g_msg_cntx.smsPhoneNumber[%s]", g_msg_cntx.smsPhoneNumber);
		
	ShowCategory154Screen(0, 0,
							STR_GLOBAL_YES, IMG_SMS_COMMON_NOIMAGE,
							STR_GLOBAL_NO, IMG_SMS_COMMON_NOIMAGE,
							(PU8)get_string_lang (STR_GLOBAL_DIAL, SSC_SCHINESE), 
							(PU8)g_msg_cntx.smsPhoneNumber, 
							IMG_GLOBAL_QUESTION, NULL);

	SetLeftSoftkeyFunction (Sp_Make_Call, KEY_EVENT_UP);
	SetRightSoftkeyFunction (GoBackHistory, KEY_EVENT_UP);
	playRequestedTone(WARNING_TONE);
}

/*****************************************************************************

	FUNCTION NAME		: Sp_Make_Call

  PURPOSE	:make call.          
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 
 
******************************************************************************/
void Sp_Make_Call (void)
{
  mmi_trace(g_sw_SP,"niej Sp_Make_Call");
	MakeCall(g_msg_cntx.smsPhoneNumber);
}

/*****************************************************************************

	FUNCTION NAME		: Sp_Exit_Make_Call

  PURPOSE	:exit call.          
	INPUT PARAMETERS	: void
	OUTPUT PARAMETERS	: void

	RETURNS				: 
 
******************************************************************************/
void Sp_Exit_Make_Call (void)
{
  mmi_trace(g_sw_SP,"niej Sp_Exit_Make_Call");
	stopRequestedTone(WARNING_TONE);
}

/*****************************************************************************

	FUNCTION NAME		: Sp_Entry_Explain_Screen

  PURPOSE	:entry explain screen, give the product intruduction.          
	INPUT PARAMETERS	: explain_str_id is the id of product introduction string.
	OUTPUT PARAMETERS	: void

	RETURNS				: 
 
******************************************************************************/
void Sp_Entry_Explain_Screen(U16 explain_str_id)
{
	PS8 buffer;
	S32 bufferSize;
  U8* inputBuffer = NULL;	
  mmi_trace(g_sw_SP,"niej Sp_Entry_Explain_Screen"); 
	EntryNewScreen(SCR_SP_EXPLAIN, Sp_Exit_Explain_Screen, NULL, NULL);
   
  inputBuffer = GetCurrInputBuffer(SCR_SP_EXPLAIN);
  if(inputBuffer == NULL)
  {
   
    	buffer	= GetString(explain_str_id);
	    bufferSize	= pfnUnicodeStrlen(buffer);    
      ShowCategory74Screen( STR_EXPLAIN_TITLE, 
								 MAIN_MENU_TITLE_FUNANDGAMES_ICON,
								 0, 0,
								 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								 (PU8)buffer, bufferSize, NULL);
  }
  else
  {    
      bufferSize = pfnUnicodeStrlen(inputBuffer);          
            
      ShowCategory74Screen( STR_EXPLAIN_TITLE, 
								 MAIN_MENU_TITLE_FUNANDGAMES_ICON,
								 0, 0,
								 STR_GLOBAL_BACK,IMG_GLOBAL_BACK,
								 (PU8)inputBuffer, bufferSize, NULL);
  }

  

	/* go back to game menu */
	SetKeyHandler(GoBackHistory,KEY_LEFT_ARROW,KEY_EVENT_DOWN);
	SetRightSoftkeyFunction(GoBackHistory,KEY_EVENT_UP);

}
	
  
void Sp_Exit_Explain_Screen(void)
{
	history_t	h;
	U16 inputBufferSize;

	h.scrnID = SCR_SP_EXPLAIN;
	h.entryFuncPtr = Sp_Entry_Explain_Screen;
	Get_Expalin_Screen_Inputbuffer(h.inputBuffer);
	AddHistory( h );
}

void Get_Expalin_Screen_Inputbuffer(U8* inputBuffer)
{
  mmi_trace(g_sw_SP, "niej void Get_Expalin_Screen_Inputbuffer");
  if(inputBuffer != NULL)
  {
    memset(inputBuffer,0,MAX_INPUT_BUFFER);
    memcpy(inputBuffer, MMI_multiline_inputbox.text,
          pfnUnicodeStrlen(MMI_multiline_inputbox.text)*ENCODING_LENGTH);  
    
  }
}
/*****************************************************************************

	FUNCTION NAME		: N_Multiplication

  PURPOSE	:  eg. base=10, index =3, the return value is 10*10*10       
	INPUT PARAMETERS	:
	OUTPUT PARAMETERS	:

	RETURNS				: The return value is base^index.
 
******************************************************************************/
int N_Multiplication(int base, int index)
{
	int value = 1;
  int i = 0;
	for(; i<index; i++)
	{
		value *= base;
	}
	return value;
}
#endif//__SP__
