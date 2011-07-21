//加载菜单
extern void jmetoc_smmu_load_menu();
extern void jmetoc_smmu_clean_all();
extern UINT8 checksim(void);

//菜单总数
typedef enum{
#ifdef _MMI_SUPPORT_JME_GEO_
	JMETOC_MENU_ITEM_GEO,
#endif
#ifdef _MMI_SUPPORT_JME_QQ_
	JMETOC_MENU_ITEM_QQ,
#endif
#ifdef _MMI_SUPPORT_JME_MSN_
	JMETOC_MENU_ITEM_MSN,
#endif
	JMETOC_MENU_ITEM_APN,
	JMETOC_MENU_ITEM_PROXY,
	JMETOC_MENU_ITEM_SIM_SEL,
	JMETOC_MENU_ITEM_NUM
}jmetoc_menu_item;


int jmetoc_menu_select_index;
//菜单名称
  U16 *no_SDCard_menu_item_lables[JMETOC_MENU_ITEM_NUM] = {
#ifdef _MMI_SUPPORT_JME_GEO_	
    (U16*)"\x47\x00\x6F\x00\x6F\x00\x67\x00\x6C\x00\x65\x00\x30\x57\xFE\x56\x00" , // Google地图
#endif //	#ifdef _MMI_SUPPORT_JME_GEO_	
#ifdef _MMI_SUPPORT_JME_QQ_
    (U16*)"\x4B\x62\x3A\x67\x51\x00\x51\x00\x00",                                 // 手机QQ
#endif
#ifdef _MMI_SUPPORT_JME_MSN_	
   // (U16*)"\x4B\x62\x3A\x67\x4D\x00\x53\x00\x4E\x00\x00",                                 // 手机MSN
   (U16*)"\x4D\x00\x53\x00\x4E\x00\x00",                                 //MSN

#endif //#ifdef _MMI_SUPPORT_JME_MSN_	
    (U16*)"\x41\x00\x50\x00\x4E\x00\x20\x00\x73\x00\x65\x00\x74\x00\x00",  // APN set
    (U16*)"\x50\x00\x72\x00\x6F\x00\x78\x00\x79\x00\x20\x00\x73\x00\x65\x00\x74\x00\x00",  // Proxy set
    (U16*)"\x53\x00\x65\x00\x6C\x00\x65\x00\x63\x00\x74\x00\x20\x00\x53\x00\x49\x00\x4D\x00\x00"  // Select SIM
   };

//菜单图标
#include "qq_icon.h"
#ifdef _MMI_SUPPORT_JME_GEO_
#include "jmetoc_geo_icon.h"
#endif
///////////////////////////////////////////////////////////// 嵌入 ROM /////////////////////////////////////////////////////////////
// 嵌入入口定义方法如下（假设应用名称为xxx，而名称的大写为XXX）：
// 将相关的应用模块引入，一般包含 "xxx_bin.h" "xxx_sym.h" "xxx_res.h" 三个文件。
// 添加启动入口，一般入口函数写法如下：
// void jmetoc_xxx_entry() {
//   jmetoc_shell(L"xxx");
//   memAppEntry(XXX_BIN, XXX_BIN_LEN, XXX_SYM, XXX_SYM_LEN, XXX_RES, XXX_RES_LEN);
//   startJMETOC_VM();
// }
// 其中 memAppEntry 中的所有参数，均在三个引入的头文件中定义。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void memAppEntry(void*, U32, void*, U32, void*, U32);
extern void jmetoc_shell(U16*);
extern void startJMETOC_VM();
extern void jmetoc_app_name_topath(u16* name);
void mdi_audio_suspend_background_play(void);

extern VOID mmi_display_gprs_active(void) ;
extern BOOL MTPNP_AD_Is_Flight_Mode(void);
VOID mmi_display_InFlightMode(void) ;
extern BOOL jmetoc_app_bg;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下为发布工具自动添加


/**
 * Google地图入口
 * @author jmetoc deployer
 * @version 1.0.0.1
 */
#ifdef _MMI_SUPPORT_JME_GEO_
#include "jmetoc_geo_bin.h"
#include "jmetoc_geo_sym.h"
#include "jmetoc_geo_res.h"
#endif
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
#ifdef __MMI_COMPRESS_CODE_SECTION__
extern char *xcpu_overlay_qq_space;
#else
char JMETOC_SYM_DECOMPRESSED[201732];
char *QQ_SYM_DECOMPRESSED=JMETOC_SYM_DECOMPRESSED;
char *GEO_SYM_DECOMPRESSED=JMETOC_SYM_DECOMPRESSED;
char *MSN_SYM_DECOMPRESSED=JMETOC_SYM_DECOMPRESSED;

#endif
/*******ATTENTION,the size of the arrary is must to be according with the size before compressed****/
#ifdef _MMI_SUPPORT_JME_GEO_	

#include "cutdownflashusage.h"
int getjme_GEO_SYM_size(void) ;

void cdfu_geo_init(void)
{
#ifdef __MMI_COMPRESS_CODE_SECTION__
	cdfu_app_init(JMETOC_GEO_SYM,getjme_GEO_SYM_size(),xcpu_overlay_qq_space);
#else
	cdfu_app_init(JMETOC_GEO_SYM,getjme_GEO_SYM_size(),GEO_SYM_DECOMPRESSED);
#endif
}
#endif //#ifdef _MMI_SUPPORT_JME_GEO_	

#endif
#ifdef _MMI_SUPPORT_JME_GEO_	



void jmetoc_jmetoc_geo_entry() {
	#ifdef MMI_ON_HARDWARE_P
	
if(jmetoc_app_bg)
{
    mmi_display_gprs_active();
    return;
}
#endif
mmi_trace(1, "!!!!!QQ  jmetoc_jmetoc_geo_entry  ");

 // mdi_audio_suspend_background_play();
  
  jmetoc_shell((u16*)L"jmetoc_geo");
  jmetoc_app_name_topath((u16*)L"jmetoc_geo");
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
  cdfu_do_init(CDFU_GEO);
#ifdef __MMI_COMPRESS_CODE_SECTION__
  memAppEntry((void*)JMETOC_GEO_BIN, JMETOC_GEO_BIN_LEN, (void*)xcpu_overlay_qq_space, JMETOC_GEO_SYM_LEN, (void*)JMETOC_GEO_RES, JMETOC_GEO_RES_LEN);
#else
  memAppEntry((void*)JMETOC_GEO_BIN, JMETOC_GEO_BIN_LEN, (void*)GEO_SYM_DECOMPRESSED, JMETOC_GEO_SYM_LEN, (void*)JMETOC_GEO_RES, JMETOC_GEO_RES_LEN);
#endif
#else  
  memAppEntry((void*)JMETOC_GEO_BIN, JMETOC_GEO_BIN_LEN, (void*)JMETOC_GEO_SYM, JMETOC_GEO_SYM_LEN, (void*)JMETOC_GEO_RES, JMETOC_GEO_RES_LEN);
#endif
  startJMETOC_VM();
}
#endif

#ifdef _MMI_SUPPORT_JME_MSN_
#include "msn_bin.h"
#include "msn_sym.h"
#include "msn_res.h"	

#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
/*******ATTENTION,the size of the arrary is must to be according with the size before compressed****/

#include "cutdownflashusage.h"

void cdfu_msn_init(void)
{
#ifdef __MMI_COMPRESS_CODE_SECTION__
	cdfu_app_init(MSN_SYM,MSN_SYM_LEN,xcpu_overlay_qq_space);
#else
	cdfu_app_init(MSN_SYM,MSN_SYM_LEN,MSN_SYM_DECOMPRESSED);
#endif
}
#endif

void jmetoc_jmetoc_msn_entry() 
{
if(checksim()==0)
{
	mmi_display_noSimCard();
	return;
}

#ifdef MMI_ON_HARDWARE_P
//	if(jmetoc_app_bg)
	{
//	    mmi_display_gprs_active();
//	    return;
	}
#endif
mmi_trace(1, "!!!!!QQ  jmetoc_jmetoc_msn_entry  ");

 // mdi_audio_suspend_background_play();
  
  jmetoc_shell((u16*)L"msn");
  jmetoc_app_name_topath((u16*)L"msn");
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
  cdfu_do_init(CDFU_MSN);
#ifdef __MMI_COMPRESS_CODE_SECTION__
  memAppEntry((void*)MSN_BIN, MSN_BIN_LEN, (void*)xcpu_overlay_qq_space, MSN_SYM_LEN, (void*)MSN_RES, MSN_RES_LEN);
#else
  memAppEntry((void*)MSN_BIN, MSN_BIN_LEN, (void*)MSN_SYM_DECOMPRESSED, MSN_SYM_LEN, (void*)MSN_RES, MSN_RES_LEN);
#endif
#else  
  memAppEntry((void*)MSN_BIN, MSN_BIN_LEN, (void*)MSN_SYM,MSN_SYM_LEN, (void*)MSN_RES, MSN_RES_LEN);
#endif
  startJMETOC_VM();
}
#endif //#ifdef _MMI_SUPPORT_JME_MSN_	
#ifdef _MMI_SUPPORT_JME_GEO_	

int getjme_GEO_SYM_size(void) 
{
return sizeof(JMETOC_GEO_SYM);
}
#endif //#ifdef _MMI_SUPPORT_JME_GEO_	

#ifdef _MMI_SUPPORT_JME_QQ_
/**
 * QQ入口
 * @author jmetoc deployer
 * @version 1.0.0.1
 */
#if 0
#include "qq_res.h"	//注意资源仍然要引入
extern U32 qq_app_entry(S32* mid);
extern U32 (*jmetoc_app_entry)(S32* mid);
extern void libAppEntry(void* res, U32 res_len);
void jmetoc_qq_entry() {
	if(checksim()==0)
{
	mmi_display_noSimCard();
	return;
}

  jmetoc_shell((U16*)L"QQ");
  jmetoc_app_name_topath((U16*) L"QQ");
  libAppEntry((void*)QQ_RES, QQ_RES_LEN);
  jmetoc_app_entry = qq_app_entry;
  startJMETOC_VM();
}

#else


#include "qq_bin.h"
#include "qq_sym.h"
#include "qq_res.h"
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
/*******ATTENTION,the size of the arrary is must to be according with the size before compressed****/

#include "cutdownflashusage.h"
int getjme_QQ_SYM_size(void) ;


void cdfu_qq_init(void)
{
#ifdef __MMI_COMPRESS_CODE_SECTION__
	cdfu_app_init(QQ_SYM,getjme_QQ_SYM_size(),xcpu_overlay_qq_space);
#else
	cdfu_app_init(QQ_SYM,getjme_QQ_SYM_size(),QQ_SYM_DECOMPRESSED);
#endif
}
#endif

void jmetoc_qq_entry() {
	
// mdi_audio_suspend_background_play();
if(checksim()==0)
{
	mmi_display_noSimCard();
	return;
}
if(MTPNP_AD_Is_Flight_Mode())
{
	mmi_display_InFlightMode();
	return;

}
  jmetoc_shell((u16*)L"qq"); 
  jmetoc_app_name_topath((u16*)L"qq");
#ifdef COMPRESS_OPTIMIZE_FLASH_SIZE
  cdfu_do_init(CDFU_QQ);
#ifdef __MMI_COMPRESS_CODE_SECTION__
  memAppEntry((void*)QQ_BIN, QQ_BIN_LEN, (void*)xcpu_overlay_qq_space, QQ_SYM_LEN, (void*)QQ_RES, QQ_RES_LEN);
#else
  memAppEntry((void*)QQ_BIN, QQ_BIN_LEN, (void*)QQ_SYM_DECOMPRESSED, QQ_SYM_LEN, (void*)QQ_RES, QQ_RES_LEN);
#endif
#else
  memAppEntry((void*)QQ_BIN, QQ_BIN_LEN, (void*)QQ_SYM, QQ_SYM_LEN, (void*)QQ_RES, QQ_RES_LEN);

#endif
  startJMETOC_VM();
}
int getjme_QQ_SYM_size(void) 
{
return sizeof(QQ_SYM);
}
#endif
#endif
/**
 * 获得菜单图标
 * @author jmetoc deployer
 * @version 1.0.0.1
 */
u8* jmetoc_getstaticmenuicon(int i) {
  switch (i) {
#ifdef _MMI_SUPPORT_JME_GEO_
	case JMETOC_MENU_ITEM_GEO:
		return (u8*)____ICON_JMETOC_GEO____;
#endif
#ifdef _MMI_SUPPORT_JME_QQ_
	case JMETOC_MENU_ITEM_QQ:
		return (u8*)____ICON_QQ____;
#endif
#ifdef _MMI_SUPPORT_JME_MSN_
	case JMETOC_MENU_ITEM_MSN:
		return (u8*)____ICON_QQ____;
#endif
	case JMETOC_MENU_ITEM_APN:
		return (u8*)____ICON_QQ____;
	case JMETOC_MENU_ITEM_PROXY:
		return (u8*)____ICON_QQ____;
	case JMETOC_MENU_ITEM_SIM_SEL:
		return (u8*)____ICON_QQ____;
	default:
	       return 0;
  }
}

extern void jmetoc_appLink(U16* name);
/**
 * 动态回调。
 * @author jmetoc deployer
 * @version 1.0.0.1
 */
void jmetoc_menu_dy_app_callback() {
  U16* appname = smarchbox_apps_menu_name[jmetoc_menu_select_index];
  if (appname[0] == '0') {
    jmetoc_mtk_entry();
  } else {
    jmetoc_appLink(appname);
  }
}

/**
 * 获得回调方法
 * @author jmetoc deployer
 * @version 1.0.0.1
 */
U32 DataAccountReadyCheck_QQApn(void);
U32 VM_select_sim_entry(void);
void VM_setProxyIp(void);
u32 jmetoc_getstaticmenucallback(int i) {
mmi_trace(1, "!!!!!QQ  jmetoc_getstaticmenucallback, i:0x%x  ",i);
  switch (i) {
#ifdef _MMI_SUPPORT_JME_GEO_
	case JMETOC_MENU_ITEM_GEO:
		return (u32)jmetoc_jmetoc_geo_entry;
#endif
#ifdef _MMI_SUPPORT_JME_QQ_
	case JMETOC_MENU_ITEM_QQ:
		return (u32)jmetoc_qq_entry;
#endif
#ifdef _MMI_SUPPORT_JME_MSN_
	case JMETOC_MENU_ITEM_MSN:
		return (u32)jmetoc_jmetoc_msn_entry;
#endif
	case JMETOC_MENU_ITEM_APN:
		return (u32)DataAccountReadyCheck_QQApn;
	case JMETOC_MENU_ITEM_PROXY:
		return (u32)VM_setProxyIp;
	case JMETOC_MENU_ITEM_SIM_SEL:
		return (u32)VM_select_sim_entry;	
  default:
    jmetoc_menu_select_index = i;
    return (u32)jmetoc_menu_dy_app_callback;
  }

}
