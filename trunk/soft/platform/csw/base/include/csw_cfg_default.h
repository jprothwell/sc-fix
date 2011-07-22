//**********************************************************************************
// change record:
// 2006.8.16  hameina get the original file, with the [REG_HKEY_SYSTEM_ROOT/Time]
//            and the values defined below.
// 2006.8.25  hameina add key REG_HKEY_CURRENT_COMM include both subkeys and values
//
// 2006.8.31  chenwenfeng changed the defaulte value of REG_HKEY_SYSTEM_ROOT
//            /Time/TM_FORMAT 0->538
//2006.9.9   chenwenfeng changed the defaulte value of TM_FORMAT 02,1a->1a,02
//2006.9.21  nieyongfeng [mod] 0000->00
//2006.9.22  chenwenfeng [mod] "\"TM_FORMAT\"=hex: (1a,02->  1a,02,00,00)\n"
//2006.10.13  wangxiaojing [+] EMOD_NWNM   EMOD_SIMNM_1(len:48)   EMOD_SIMNM_2(len:44)   EMOD_SIGN
//2007.8.17  hameina[+] "[REG_HKEY_CURRENT_COMM/general]\n" ---->subvalue IMSI
//***************************************************************************************
#ifndef __REG_DEFAULT_H__
#define __REG_DEFAULT_H__
#include "cfw_multi_sim.h"
#if(CFW_SIM_NUMBER==1)
#include "csw_cfg_default1.h"
#elif(CFW_SIM_NUMBER==2)
#include "csw_cfg_default2.h"
#elif(CFW_SIM_NUMBER==3)
#include "csw_cfg_default3.h"
#elif(CFW_SIM_NUMBER==4)
#include "csw_cfg_default4.h"
#endif
#endif//__REG_DEFAULT_H__





