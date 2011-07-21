/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of COOLSAND Inc. (C) 2005
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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    gpio_hw.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intended for GPIO driver.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *============================================================================
 ****************************************************************************/
#ifndef _GPIO_SW_H
#define _GPIO_SW_H

#define INPUT			      0		/* IO in input */
#define ALL_INPUT		      0x0000 
#define OUTPUT			      1		/* IO in output */
#define ALL_OUTPUT		   0xffff 

#if ( (defined(MT6205)) || (defined(MT6205B)) )
   #define GPIOMode               0x00
   /*GPIO mode1*/
   #define GPIO_DAICLK 			    0x01 /*GPIO0*/
   #define GPIO_DAIPCMOUT			 0x01 /*GPIO1*/
   #define GPIO_DAIPCMIN			 0x01 /*GPIO2*/
   #define GPIO_DAIRST  		    0x01 /*GPIO3*/
   #define GPIO_BPI_BUS4			 0x01 /*GPIO4*/
   #define GPIO_BPI_BUS5			 0x01 /*GPIO5*/
   #define GPIO_BPI_BUS6			 0x01 /*GPIO6*/
   #define GPIO_BPI_BUS7			 0x01 /*GPIO7*/
   #define GPIO_BSI_CS1	   		 0x02 /*GPIO7*/
   /*GPIO mode2*/
   #define GPIO_LCD_DATA			 0x01 /*GPIO8*/
   #define GPIO_LCD_A0 		   	 0x01 /*GPIO9*/
   #define GPIO_LCD_CLK			    0x01 /*GPIO10*/
   #define GPIO_LCD_CS0			    0x01 /*GPIO11*/
   #define GPIO_LCD_CS1			    0x01 /*GPIO12*/
   
   /*GPIO mode3*/
   #define GPIO_PWM			       0x01 /*GPIO16*/
   #define GPIO_UCTS2			    0x01 /*GPIO17*/
   #define GPIO_UDSR1			    0x02 /*GPIO17*/
   #define GPIO_URTS2			    0x01 /*GPIO18*/
   #define GPIO_UDTR1			    0x02 /*GPIO18*/
   #define GPIO_URXD2			    0x01 /*GPIO19*/
   #define GPIO_UTXD2			    0x01 /*GPIO20*/
   #define GPIO_UREF_CK   	       0x01 /*GPIO21*/
   
   /*GPIO mode4*/
   #define GPO_XLCDE  	   		 0x01 /*GPO0*/
   #define GPO_EA22     			 0x02 /*GPO0*/
   #define GPO_SRCLKENAN		    0x01 /*GPO1*/
   #define GPO_SRCLKENA 		    0x01 /*GPO2*/
#endif    


extern void GPIO_InitIO(char direction, char port);
extern char GPIO_ReadIO(char port);
extern kal_uint16 GPIO_ReadAll(void);
extern void GPIO_WriteIO(char data, char port);
extern void GPIO_WriteAll(kal_uint16 write_data);
extern char GPIO_ReturnMode(char port);
extern char GPO_ReturnMode(char port);
extern char GPIO_ReturnDir(char port);
extern char GPIO_ReturnDout(char port);
extern char GPO_ReturnDout(char port);
extern void GPO_WriteIO(char data,char port);
extern void GPO_WriteAll(kal_uint16 write_data);
extern void GPIO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada);
extern void GPO_ModeSetup(kal_uint16 pin, kal_uint16 conf_dada);
extern void initGPIO(void);
extern void GPIO_ALLIO(void);
extern void GPIO_PullenSetup(kal_uint16 pin, kal_bool enable);
extern void GPIO_DinvSetup(kal_uint16 pin, kal_bool enable);

#endif

