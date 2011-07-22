/*************************************************************************
 *
 * MODULE NAME:    rdabt.h
 * PROJECT CODE:   rdabt
 * DESCRIPTION:    rdabt include file.
 * MAINTAINER:     mql
 * CREATION DATE:  24/04/10
 *
 * SOURCE CONTROL: $Id: rdabt_drv.h 6568 2011-03-16 03:52:55Z huazeng $
 *
 * LICENSE:
 *     This source code is copyright (c) 2010-2011 rda.
 *     All rights reserved.
 *
 * NOTES TO USERS:
 *   None.
 *				 
 *   V3.20
 *************************************************************************/
#ifndef  __RDA_BT_H__
#define __RDA_BT_H__

#include "hal_timers.h"

#define RDABT_DELAY(DURATION) sxr_Sleep(DURATION*5 MS_WAITING)


extern kal_uint8 bt_sleephdl;

typedef enum{
	RDA_NONE_CHIP_ENUM = 0,
	RDA_BT_R5_5868_ENUM,
	RDA_BT_R7_5868PLUS_ENUM,
	RDA_BT_R8_5870_ENUM,
	RDA_BT_R10_5868E_ENUM,
	RDA_BT_R10_5872_ENUM,
	RDA_BT_R10_AL_5868H_ENUM,
	RDA_BT_R10_AL_5872H_ENUM,
	RDA_BT_R10_5870E_ENUM,
	RDA_BT_R11_5872P_ENUM,
	RDA_BT_R11_5875_ENUM

}RDABT_CHIP_ENUM;

#define  __RDABT_USE_IIC__


//#define __RDA_BT_5868_SERIES__
//#define __RDA_BT_587x_SERIES__
//#define __RDA_BT_5875_SERIES__

#ifdef __RDA_BT_5868_SERIES__
//#define __RDA_CHIP_R5_5868__
//#define __RDA_CHIP_R7_5868plus__
//#define __RDA_CHIP_R10_5868E__
//#define __RDA_CHIP_R10_5868H__
#endif

#ifdef __RDA_BT_587x_SERIES__
//#define __RDA_CHIP_R8_5870__
//#define __RDA_CHIP_R10_5872__
//#define __RDA_CHIP_R10_5872H__
//#define __RDA_CHIP_R10_5870E__
//#define __RDA_CHIP_R11_5872P__
#endif

#ifdef __RDA_BT_5875_SERIES__
//#define __RDA_CHIP_R11_5875__
#endif


#ifdef __RDA_BT_587x_SERIES__
//#define __587x_USE_DCDC__

#ifdef __RDA_CHIP_R10_5872H__
//#define __RDA_USE_LPO_32K__
#endif

#endif


#ifdef __RDA_BT_5875_SERIES__
//#define __5875_USE_DCDC__
#endif

//���������r5��r7��r10-5868e �е�����2�ֻ���3�֣��򿪴˺꣬������ر�
//make sure�__RDA5868_USE_IIC__  is defined
#ifdef __RDA_BT_5868_SERIES__
#define __RDA_AUTO_CHIP_5868__
#endif

#ifdef __RDA_BT_587x_SERIES__
#define __RDA_AUTO_CHIP_587x__
#endif


#if !defined(__RDA_BT_5868_SERIES__) && !defined(__RDA_BT_587x_SERIES__) && !defined(__RDA_BT_5875_SERIES__)
#error "please define at least one chip"
#endif

#if defined( __RDABT_USE_IIC__) && (defined(__RDA_CHIP_R5_5868__)||defined(__RDA_CHIP_R7_5868plus__))
#define __RDA_SOFT_RESET__
#endif


#if defined ( __RDA_CHIP_R7_5868plus__) 
#define __RDA_SLEEP_ROM__
#endif

#if defined(__RDA_CHIP_R11_5872P__) || defined(__RDA_CHIP_R11_5875__)
//#define __RDA_ESERMODE_CHANGE__
#endif


void GPO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada);
void GPIO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada);
void GPIO_WriteIO(char data, char port);
void GPIO_InitIO(char direction, char port);
void BtRadio_SetupGPIO(char val, unsigned char pin);

void RDA_bt_Power_On_Reset(void);
void rda_bt_pin_to_low(void);
void rda_bt_ldoon_toggle_high(void);

//void rdabt_uart_start(void);
//void rdabt_uart_configure(unsigned baud);
//void rdabt_uart_stop(void);
//unsigned char rdabt_uart_tx(kal_uint8 *buf, unsigned short num_to_send, unsigned short *num_send);
#define rdabt_uart_stop rda5868_uart_stop
#define rdabt_uart_start rda5868_uart_start
#define rdabt_uart_configure rda5868_uart_configure
#define rdabt_uart_rx rda5868_uart_rx
#define rdabt_uart_tx rda5868_uart_tx

void rdabt_wirte_memory(unsigned int addr,const unsigned int *data,unsigned char len,unsigned char memory_type);

void rdabt_iic_rf_write_data(unsigned char regaddr, const unsigned short *data, unsigned char datalen);
void rdabt_iic_rf_read_data(unsigned char regaddr, unsigned short *data, unsigned char datalen);
void rdabt_iic_core_read_data(unsigned int regaddr, unsigned int *data, unsigned char datalen);
void rdabt_iic_core_write_data(unsigned int regaddr, const unsigned int *data, unsigned char datalen);

void rda_bt_power_on(void);
void rda_bt_rf_init(void);
void rda_bt_uart_init(void);



#ifdef __RDA_CHIP_R5_5868__
void RDABT_rf_Intialization_r5(void);
void RDABT_core_Intialization_r5(void);
void	Rdabt_Pskey_Write_rf_r5(void);
#endif

#ifdef __RDA_CHIP_R7_5868plus__
void RDABT_rf_Intialization_r7(void);
void RDABT_core_Intialization_r7(void);
void Rdabt_Pskey_Write_rf_r7(void);
#endif

#ifdef __RDA_CHIP_R8_5870__
void RDABT_rf_Intialization_r8(void);
void RDABT_core_Intialization_r8(void);
void Rdabt_Pskey_Write_rf_r8(void);
#endif


#if defined( __RDA_CHIP_R10_5872__) || defined(__RDA_CHIP_R10_5868E__) || defined(__RDA_CHIP_R10_5872H__) || defined(__RDA_CHIP_R10_5868H__) || defined(__RDA_CHIP_R10_5870E__)
void Rdabt_Pskey_Write_r10(void);
void Rdabt_trap_write_r10(void);
void Rdabt_patch_write_r10(void);
void Rdabt_acl_patch_write_r10(void);
void Rdabt_unsniff_prerxon_write_r10(void);
void Rdabt_setfilter_r10(void);

#if defined( __RDA_CHIP_R10_5872__) || defined(__RDA_CHIP_R10_5868E__)
void RDABT_rf_Intialization_r10(void);
void RDABT_core_Intialization_r10(void);
void  Rdabt_core_uart_Intialization_r10(void);
void  Rdabt_sleep_patch_off_write_r10(void);
void Rdabt_Pskey_Write_rf_r10(void);
#endif

#if defined( __RDA_CHIP_R10_5872H__) || defined(__RDA_CHIP_R10_5868H__)
void RDABT_rf_Intialization_r10_al(void);
void RDABT_core_Intialization_r10_al(void);
void Rdabt_Pskey_Write_rf_r10_al(void);
void rdabt_DC_write_r10_al(void);
#endif

#if defined(__RDA_CHIP_R10_5870E__)
void RDABT_core_Intialization_r10_e(void);
void RDABT_rf_Intialization_r10_e(void);
void Rdabt_Pskey_Write_rf_r10_e(void);
void rdabt_DC_write_r10_e(void);

#endif


#endif


#if defined(__RDA_CHIP_R11_5872P__) || defined(__RDA_CHIP_R11_5875__)
void Rdabt_setfilter_r11(void);
void Rdabt_unsniff_prerxon_write_r11(void);
void Rdabt_trap_write_r11(void);
void Rdabt_Pskey_Write_r11(void);
void Rdabt_esermode_write_r11(void);

#if defined(__RDA_CHIP_R11_5872P__)
void RDABT_core_Intialization_r11_p(void);
void RDABT_rf_Intialization_r11_p(void);
void Rdabt_Pskey_Write_rf_r11_p(void);
void rdabt_DC_write_r11_p(void);
#endif

#if defined(__RDA_CHIP_R11_5875__)
void RDABT_core_Intialization_r11(void);
void RDABT_rf_Intialization_r11(void);
void Rdabt_Pskey_Write_rf_r11(void);
void rdabt_DC_write_r11(void);
#endif

#endif



#if defined( __RDA_AUTO_CHIP_5868__) || defined(__RDA_AUTO_CHIP_587x__)
kal_uint8 rdabt_get_chipid(void);
#endif

#endif

