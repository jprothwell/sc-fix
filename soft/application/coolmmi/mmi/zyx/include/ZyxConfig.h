#ifdef __ZYX_PLATFORM_SUPPORT__

#ifdef ZYX_MID_BUILD

#define __NEWSIMULATOR 
#include "mmi_features.h"

/*  Include: MMI header file */
#include "mmi_data_types.h"
#include "globaldefs.h"
#include "protocolevents.h"

#include "timerevents.h"
#include "historygprot.h"
#include "sublcdhistorygprot.h"

#include "wgui_categories.h"
#include "wgui_status_icons.h"
#include "wgui_categories_inputs.h"

#include "gpioinc.h"

#include "alarmprot.h"
#include "alarmdef.h"
#include "alarmgprot.h"
#include "alarmframeworkprot.h"
#include "datetimegprot.h"

#include "filemgr.h"
#include "profilegprots.h"
#include "profilemgrgprot.h"
#include "profiles_prot.h"

#include "keybrd.h"		// for clearkeyevents

#include "unicodexdcl.h"
#include "custdatares.h"

#include "debuginitdef.h"
#include "settingprofile.h"

#include "nvramenum.h"
#include "nvramtype.h"
#include "nvramprot.h"
#include "audioinc.h"
#include "timerevents.h"
#include "organizerdef.h"
#include "profilesdefs.h"
#include "wrappergprot.h"
#include "simdetectiongexdcl.h"
#include "commonscreens.h"
#include "wgui_categories_popup.h"
#include "wgui_status_icons.h"

#ifdef __MMI_TOUCH_SCREEN__
#include "wgui_touch_screen.h"
#endif

#include "dual_sim.h"

#undef __NEWSIMULATOR 
#endif

////////////////////////////////////////////////////////////////////////////////////////////
//
//			配置信息
//
////////////////////////////////////////////////////////////////////////////////////////////
#define ZYX_CMWAP_ACCOUNT 14   //系统CMWAP上网帐号
#define ZYX_CMNET_ACCOUNT 19  //系统CMNET上网帐号
//参见厂商和手机型号.xls
#define FACTORY_ID   "9999"	// 厂商ID，四位数
#define MODEL_ID      "001"	//机型ID，三位数

#define LCD_SIZE_DEF "240X320"

//////////////////////////////////////////////
///下面的定义请不要修改
//////////////////////////////////////////////

//版本信息
#define MIDWARE_VERSION "CST_2002"

//升级序列号
#define UPDATE_SERIAL           "240X3209991001"//LCD_SIZE_DEF##FACTORY_ID##MODEL_ID
#endif

