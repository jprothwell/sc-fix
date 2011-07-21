/****************************************************************/
/* Copyright (C) 2003, CII Technologies, Inc. */
/* All Rights Reserved */
/****************************************************************/
/* File Name : EngineerModePatch.c */
/* Description : */
/* this file is patch for EM/FM */
/* Revision : */
 
/* add new file */
/****************************************************************/
#include "mmi_features.h"
#include "engineermodepatch.h"


#if (defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__) ) 	
//------------------------------------------------------------
//include
#include "hal_timers.h"
#include "sxr_tls.h"
#include "tgt_m.h"

//---------------------------------------------------------
//Local	Variable
static UINT16 PDN_CON0;
static UINT16 PDN_CON1;
static UINT16 PDN_CON2;
static UINT16 PDN_CON3;

/*************************************************************************
* FUNCTION
*  INT_QueryPowerDownStatus
*
* DESCRIPTION
*  Used to read-back the 4 power down control registers.
*
* CALLS
*
* PARAMETERS
*  Pointer of type CONFIG_PDN_TYPE.
*
* RETURNS
*
* GLOBALS AFFECTED
*
*************************************************************************/
void INT_QueryPowerDownStatus(CONFIG_PDN_TYPE *PDN_ptr)
{
   PDN_ptr->pdn_con0 = PDN_CON0;//*(volatile kal_uint16 *)
   PDN_ptr->pdn_con1 = PDN_CON1;//*(volatile kal_uint16 *)
   PDN_ptr->pdn_con2 = PDN_CON2;//*(volatile kal_uint16 *)
   PDN_ptr->pdn_con3 = PDN_CON3;//*(volatile kal_uint16 *)
}

//---------------------------------------------------------
#define   UINT16P             unsigned short *
#define   UINT32P             unsigned int   *
/* VERSION Registers */
#define CONFIG_base              (0x80000000)   /* Config registers  (Clk, Power Down, Ver) */
#define HW_VER    ((UINT16P)(CONFIG_base+0x0000)) /* Hardware ver register */
#define FW_VER    ((UINT16P)(CONFIG_base+0x0004)) /* Firmware ver register */
#define HW_CODE   ((UINT16P)(CONFIG_base+0x0008)) /* Hardware ver register */
//---------------------------------------------------------
/*************************************************************************
* FUNCTION
*  release_bb_chip
*
* DESCRIPTION
*   This function returns the version number of the baseband chip
*
* CALLS
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*************************************************************************/
kal_char* release_bb_chip(void)
{
   static kal_char str_return[]="MT6205B, E00";

#if defined(FPGA)
   strcpy(str_return, "FPGA");
#elif defined(MT6208)
   strcpy(str_return, "MT6208");
#elif defined(MT6205)
   strcpy(str_return, "MT6205");
#else /* future chips will definitely apply this rule */
   kal_uint16 hw_code;
   kal_uint8 majrev; /* bit 8~11 of HW_VER register */
   kal_uint8 ffix; /* bit 0~3 of HW_VER register */
   kal_uint16 hw_version;
   kal_uint16 temp_num;

   hw_code = *HW_CODE;
   hw_version = *HW_VER;
   majrev = (hw_version >> 8) & 0x000f; /* get MAJREV from HW_VER */
   ffix = hw_version & 0x000f; /* get FFIX from HW_VER */

   /* convert the HW_CODE to natural representation*/
   temp_num = (hw_code & 0x000f);
   hw_code >>= 4;
   temp_num += (hw_code & 0x000f) * 10;
   hw_code >>= 4;
   temp_num += (hw_code & 0x000f) * 100;
   hw_code >>= 4;
   temp_num += (hw_code & 0x000f) * 1000;

   /* construct the string for the chips MTxxxxx, ECOx */
   if (majrev == 0x0a) /* directiver A, which often skip the directive */
   {
      sprintf(str_return, "MT%4d, E%02d", temp_num, ffix);
   }
   else
   {
      sprintf(str_return, "MT%4d%1X, E%02d", temp_num, majrev, ffix);
   }
#endif

   return str_return;
}
//---------------------------------------------------------
 
//---------------------------------------------------------
const kal_char *release_dsp_fw(void)
{
	const static kal_char str_return[]="release_dsp_fw";
	return (str_return);
}
//---------------------------------------------------------
const kal_char *release_dsp_ptch(void)
{
	const static kal_char str_return[]="release_dsp_ptch";
	return (str_return);
}
//---------------------------------------------------------
const kal_char* release_verno(void) 
{ 
   return tgt_GetBuildVerNo(); 
} 

const kal_char* release_build(void) 
{ 
   const static kal_char build_str[] = "BUILD_NO"; 
   return build_str; 
} 
const kal_char* build_date_time(void) 
{ 
   return tgt_GetBuildDateTime(); 
} 

//---------------------------------------------------------
const kal_char* release_branch(void) 
{ 
   const static kal_char build_branch_str[] = "05B PSI"; 
   return build_branch_str; 
} 
//---------------------------------------------------------
const kal_char* release_hw_ver(void) 
{ 
   const static kal_char hw_ver_str[] = "PSI_HW"; 
   return hw_ver_str; 
} 
//---------------------------------------------------------
/*************************************************************************
* FUNCTION
*  INT_VersionNumbers
*
* DESCRIPTION
*   This function returns the version number of the followings
*   1. Chip version
*   2. DSP version
*   3. DSP patch version
*   4. MCU software version
*   5. Baseband board version
*
* CALLS
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
*************************************************************************/
void INT_VersionNumbers(version_struct * ptr_version)
{
   ptr_version->bb_chip = release_bb_chip();
   ptr_version->dsp_fw = release_dsp_fw();
   ptr_version->dsp_ptch = release_dsp_ptch();
   ptr_version->mcu_sw = release_verno();
   ptr_version->mcu_sw_branch = release_branch();
   ptr_version->bb_board = release_hw_ver();
}

/*************************************************************************
* FUNCTION
*  INT_SetPowerDownStatus
*
* DESCRIPTION
*  Setting 4 power down control registers.
*
* CALLS
*
* PARAMETERS
*  Pointer of type CONFIG_PDN_TYPE.
*
* RETURNS
*
* GLOBALS AFFECTED
*
*************************************************************************/
void INT_SetPowerDownStatus(CONFIG_PDN_TYPE *PDN_ptr)
{
   PDN_CON0 = PDN_ptr->pdn_con0;//*(volatile kal_uint16 *)
   PDN_CON1 = PDN_ptr->pdn_con1;//*(volatile kal_uint16 *)
   PDN_CON2 = PDN_ptr->pdn_con2;//*(volatile kal_uint16 *)
   PDN_CON3 = PDN_ptr->pdn_con3;//*(volatile kal_uint16 *)
}

//---------------------------------------------------------
//stub
//---------------------------------------------------------mdi_audio_block
#ifdef MMI_ON_HARDWARE_P

#else
BOOL mdi_audio_block(int mod_id)
{
	return (0);
}
#endif

//---------------------------------------------------------kal_milli_secs_to_ticks
kal_uint32 kal_milli_secs_to_ticks(U8 tt )
{
#if defined(__PROJECT_GALLITE_C01__)
	return (tt MS_WAITING);
#else
	return (0);
#endif
}
//---------------------------------------------------------kal_sleep_task
void kal_sleep_task(kal_uint32 time_in_ticks)
{
#if defined(__PROJECT_GALLITE_C01__)
	sxr_Sleep(time_in_ticks);
#endif
}
//---------------------------------------------------------
unsigned char netname[][16] = {
#if 1
	"Channel 0:",
	"Channel 1:",
	"Channel 2:",
	"Channel 3:",
#else
	"BTemp",
	"VAUX",
	"Current",
	"VChgr",
#endif
	"VBAT:",
	"VMIC:",
};
//---------------------------------------------------------adc_map_tbl
const  GPIO_MAP_ENTRY adc_map_tbl[] = {
/*GPIO_LABELID_0 */      {GPIO_VAILD, 0, netname[0], 0 },
/*GPIO_LABELID_1 */      {GPIO_VAILD, 1, netname[1], 0 },
/*GPIO_LABELID_2 */      {GPIO_VAILD, 2, netname[2], 0 },
/*GPIO_LABELID_3 */      {GPIO_VAILD, 3, netname[3], 0 },
/*GPIO_LABELID_4 */      {GPIO_INVAILD, 4, netname[4], 0 },//Fix Bug#11557 for CP1606 by cong.li on 2009.06.11. BTW, now just support ADC chanel 0 to chanel 3.
/*GPIO_LABELID_5 */      {GPIO_INVAILD, 5, netname[5], 0 }
};
//---------------------------------------------------------UART_PutBytes
/*kal_uint16 UART_PutBytes(UART_PORT port, 
                                             kal_uint8 *Buffaddr, 
                                             kal_uint16 Length, 
                                             module_type ownerid)
{
	return (0);
}
//---------------------------------------------------------UART_GetBytes
kal_uint16 UART_GetBytes(UART_PORT port, 
                                              kal_uint8 *Buffaddr, 
                                              kal_uint16 Length, 
                                              kal_uint8 *status, 
                                              module_type ownerid)
{
	return (0);
}*/
//---------------------------------------------------------UART_GetOwnerID
module_type UART_GetOwnerID(UART_PORT port)
{
	return (MOD_NIL);
}
//---------------------------------------------------------UART_SetOwner
void UART_SetOwner (UART_PORT port, kal_uint8 owner)
{
}
//---------------------------------------------------------gpio_map_tbl
const GPIO_MAP_ENTRY gpio_map_tbl[] = {
/*GPIO_LABELID_0 */      {GPIO_VAILD, 0, netname[0], 0 },
/*GPIO_LABELID_1 */      {GPIO_VAILD, 1, netname[1], 0 },
/*GPIO_LABELID_2 */      {GPIO_VAILD, 2, netname[2], 0 },
/*GPIO_LABELID_3 */      {GPIO_VAILD, 3, netname[3], 0 },
/*GPIO_LABELID_4 */      {GPIO_INVAILD, 4, netname[4], 0 },
/*GPIO_LABELID_5 */      {GPIO_INVAILD, 5, netname[5], 0 }
};
//---------------------------------------------------------kal_wstrcpy
/*
WCHAR *kal_wstrcpy(WCHAR *to, const WCHAR *from)
{
	return (NULL);
}
*/
//---------------------------------------------------------PWM1_Level_Info
kal_uint32 PWM1_Level_Info[PWM_MAX_LEVEL][2] = {                  
                  {64, 20},  // PWM 1  Level 1 
                  {64, 40},  // PWM 1  Level 2
                  {64, 60},  // PWM 1  Level 3
                  {64, 80},  // PWM 1  Level 4
                  {64, 100} // PWM 1 Level 5 
};
//---------------------------------------------------------PWM2_Level_Info
kal_uint32 PWM2_Level_Info[PWM_MAX_LEVEL][2] = {                  
                  {64, 20},  // PWM 1  Level 1 
                  {64, 40},  // PWM 1  Level 2
                  {64, 60},  // PWM 1  Level 3
                  {64, 80},  // PWM 1  Level 4
                  {64, 100} // PWM 1 Level 5 
};
//---------------------------------------------------------Alter_Level_Info
kal_uint32 Alter_Level_Info[PWM_MAX_LEVEL][2] = {                  
                  {64, 20},  // PWM 1  Level 1 
                  {64, 40},  // PWM 1  Level 2
                  {64, 60},  // PWM 1  Level 3
                  {64, 80},  // PWM 1  Level 4
                  {64, 100} // PWM 1 Level 5 
};
//---------------------------------------------------------eint_map_tbl
const GPIO_MAP_ENTRY eint_map_tbl[] = {
/*GPIO_LABELID_0 */      {GPIO_VAILD, 0, netname[0], 0 },
/*GPIO_LABELID_1 */      {GPIO_VAILD, 1, netname[1], 0 },
/*GPIO_LABELID_2 */      {GPIO_VAILD, 2, netname[2], 0 },
/*GPIO_LABELID_3 */      {GPIO_VAILD, 3, netname[3], 0 },
/*GPIO_LABELID_4 */      {GPIO_INVAILD, 4, netname[4], 0 },
/*GPIO_LABELID_5 */      {GPIO_INVAILD, 5, netname[5], 0 }
};
//---------------------------------------------------------
kal_uint8   MainLCD_Contrast_Level_Info[LCD_CONTRAST_MAX_LEVEL] = {
                   /* Main LCD contrast level 1 ~ 15 */
                  120, 122, 124, 126, 128, 130, 132, 134, 136, 138, 
                  140, 142, 144, 146, 148,    
};
//---------------------------------------------------------
kal_uint8   SubLCD_Contrast_Level_Info[LCD_CONTRAST_MAX_LEVEL] = {
                   /* Sub LCD contrast level 1 ~ 15 */
                  20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 
                  40, 42, 44, 46, 48,    
};
//---------------------------------------------------------
kal_uint32   Battery_Level_Info[BATTERY_MAX_LEVEL] = {
                  /* Battery voltage Level */
                  3200000, /* Low Battery Power off */
                  3500000, /* Low Battery */
                  /* battery level 1 ~ 8 */
                  3698671, 3801801, 4003424, 9999999, 
                  9999999, 9999999, 9999999, 9999999
};
//---------------------------------------------------------
kal_uint8   MainLCD_Bias_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Main LCD Bias Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
kal_uint8   MainLCD_Linerate_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Main LCD Linerate Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
kal_uint8   MainLCD_Temp_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Main LCD Temperature Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
//---------------------------------------------------------
kal_uint8   SubLCD_Bias_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Sub LCD Bias Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
kal_uint8   SubLCD_Linerate_Level_Info[LCD_PARAM_MAX_LEVEL] = {
                   /* Sub LCD Linerate Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
kal_uint8   SubLCD_Temp_Level_Info[LCD_PARAM_MAX_LEVEL]= {
                   /* Sub LCD Temperature Param (Reserved) */
                  0, 0, 0, 0, 0,    
};
//---------------------------------------------------------SWDBG_Profile
kal_uint8 SWDBG_Profile = 0;
//---------------------------------------------------------TST_PORT
UART_PORT TST_PORT = 0;
//---------------------------------------------------------PS_UART_PORT
UART_PORT PS_UART_PORT = 0;
//---------------------------------------------------------TST_BAUDRATE
UART_baudrate TST_BAUDRATE = 0;
//---------------------------------------------------------PS_BAUDRATE
UART_baudrate PS_BAUDRATE = 0;
//---------------------------------------------------------g_mmi_frm_enable_debug_assert
extern BOOL g_mmi_frm_enable_debug_assert ;
//---------------------------------------------------------L1sim_Enable_Enhanced_Speed
void L1sim_Enable_Enhanced_Speed(kal_bool enable)
{
}
//---------------------------------------------------------get_pdu_ptr
void* get_pdu_ptr(peer_buff_struct *peer_buff_ptr, kal_uint16 *length_ptr)
{
	static kal_char str_return[]="get_pdu_ptr";
	return (str_return);
}
//---------------------------------------------------------INT_MemoryDumpFlag
kal_uint32 INT_MemoryDumpFlag = 0;
//---------------------------------------------------------nvram_report_last_exception
void nvram_report_last_exception(void *task_name_p, void *type_p, void *time_p, void *status_p)
{
}
//---------------------------------------------------------SST_AdvRAMTest
kal_bool SST_AdvRAMTest(void)
{
	return (0);
}
//---------------------------------------------------------free_int_peer_buff
void free_int_peer_buff(peer_buff_struct *pdu_ptr, kal_char* file,kal_uint32 line)
{
}
//---------------------------------------------------------High_Speed_SIM
kal_bool High_Speed_SIM = 1;
//---------------------------------------------------------
#endif //defined(MMI_ON_HARDWARE_P) && defined(__MMI_ENGINEER_MODE__)

