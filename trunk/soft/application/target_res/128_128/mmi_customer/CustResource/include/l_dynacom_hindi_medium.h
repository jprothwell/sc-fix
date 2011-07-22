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
/*******************************************************************************
 * Filename:
 * ---------
 *  L_DynaCom_Hindi_Medium.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   This file is intended for empty use only.
 *
 * Author:
 * -------
 
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 * $Revision:   1.3  $
 * $Modtime:   Aug 02 2006 10:23:24  $
 * $Log:   O:/PSI_MMI/archives/2___work/Soft_11.13/soft/mmi_customer/CustResource/include/L_DynaCom_Hindi_Medium.h-arc  $
 * 
 *    Rev 1.3   Nov 30 2006 11:04:00   lin
 * 恢复CSW更新前的版本
 * 
 *    Rev 1.1   Nov 24 2006 18:44:18   lin
 * No change.
 * 
 *    Rev 1.0   Nov 13 2006 15:29:06   zhangxb
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
//beginning:guoyt added for adding hindi lang
const U8 Hindi16_DWidth[262] = {7,6,2,12,12,14,9,9,9,11,12,11,9,9,9,9,13,14,14,13,11,11,10,10,9,10,12,11,11,10,8,9,8,9,11,10,10,10,10,9,9,9,11,9,10,10,10,7,7,10,11,11,9,10,9,10,8,5,6,5,5,7,8,7,7,7,8,8,7,8,5,6,5,6,5,13,5,9,7,5,11,11,10,11,8,9,11,10,12,10,8,8,3,5,6,6,7,7,8,9,8,9,8,7,4,10,11,10,7,11,10,11,11,11,11,11,11,9,10,9,11,8,8,7,8,7,8,10,8,9,8,7,8,7,8,8,8,8,9,8,7,7,7,9,7,8,8,8,6,6,8,10,10,7,8,7,8,7,19,11,19,19,18,17,18,10,10,10,10,10,10,10,10,15,16,17,17,15,16,15,10,10,15,9,13,8,9,8,5,9,8,9,7,8,6,7,7,5,5,5,5,5,5,5,7,7,7,8,7,8,8,8,9,9,5,5,5,6,6,11,10,10,10,11,11,10,10,10,9,10,9,10,8,10,10,10,9,9,9,8,8,6,9,7,11,11,10,10,18,17,18,17,18,19,5,5,11,10,9,15,10,9,};
const U8 Hindi16_Width[263]= {
0x8 ,0x7 ,0x5 ,0x2 ,0xE ,0xE ,0x10,0xB ,0xB ,0xB ,0xD ,0xE ,0xD ,0xB ,0xB ,0xB ,
0xB ,0xF ,0xF ,0x10,0x10,0xD ,0xD ,0xC ,0xC ,0xB ,0xC ,0xE ,0xD ,0xD ,0xC ,0xA ,
0xB ,0xA ,0xB ,0xD ,0xC ,0xC ,0xC ,0xC ,0xB ,0xB ,0xB ,0xD ,0xB ,0xC ,0xC ,0xC ,
0x9 ,0x9 ,0xC ,0xD ,0xD ,0xB ,0xC ,0xB ,0xC ,0xA ,0x5 ,0x6 ,0x7 ,0x9 ,0x9 ,0x8 ,
0x7 ,0x7 ,0x7 ,0x8 ,0x8 ,0x7 ,0x8 ,0x7 ,0x8 ,0x7 ,0x8 ,0x5 ,0xD ,0x5 ,0x9 ,0x7 ,
0x5 ,0xD ,0xD ,0xC ,0xD ,0xA ,0xB ,0xD ,0xC ,0xE ,0xC ,0x8 ,0x8 ,0x3 ,0x5 ,0x6 ,
0x6 ,0x7 ,0x7 ,0x8 ,0x9 ,0x8 ,0x9 ,0x8 ,0x7 ,0x4 ,0xC ,0xD ,0xC ,0x9 ,0xD ,0xC ,
0xD ,0xD ,0xD ,0xD ,0xD ,0xD ,0xB ,0xC ,0xB ,0xD ,0xA ,0xA ,0x9 ,0xA ,0x9 ,0xA ,
0xC ,0xB ,0xB ,0xA ,0x9 ,0xA ,0x9 ,0xA ,0xA ,0xA ,0xA ,0xB ,0xA ,0x9 ,0x9 ,0x9 ,
0xB ,0x9 ,0xA ,0xA ,0xA ,0x8 ,0x8 ,0xA ,0xC ,0xC ,0x9 ,0xA ,0x9 ,0xA ,0x9 ,0x15,
0xD ,0x15,0x15,0x14,0x13,0x15,0xC ,0xC ,0xC ,0xC ,0xC ,0xC ,0xC ,0xC ,0x11,0x12,
0x13,0x13,0x11,0x12,0x11,0xC ,0xC ,0x11,0xB ,0xF ,0xA ,0xB ,0xA ,0x7 ,0xB ,0xA ,
0xB ,0x7 ,0xA ,0x8 ,0x7 ,0x7 ,0x7 ,0x7 ,0x7 ,0x7 ,0x9 ,0xB ,0xC ,0x9 ,0x9 ,0x9 ,
0x8 ,0x7 ,0x8 ,0x8 ,0x8 ,0x9 ,0x9 ,0x7 ,0x7 ,0x7 ,0x8 ,0x8 ,0xD ,0xC ,0xC ,0xC ,
0xD ,0xD ,0xC ,0xC ,0xC ,0xB ,0xC ,0xB ,0xC ,0xA ,0xC ,0xC ,0xC ,0xB ,0xB ,0xB ,
0xA ,0xA ,0x8 ,0xB ,0x9 ,0xD ,0xD ,0xC ,0xC ,0x14,0x13,0x14,0x13,0x14,0x15,0x6 ,
0x6 ,0xD ,0xC ,0xB ,0x11,0xC ,0xB ,};

const U32 Hindi16_Offset[264]= {

0x0000,0x0010,0x001E,0x0028,0x002C,0x0048,0x0064,0x0084,0x009A,0x00B0,0x00C6,0x00E0,0x00FC,0x0116,0x012C,0x0142,
0x0158,0x016E,0x018C,0x01AA,0x01CA,0x01EA,0x0204,0x021E,0x0236,0x024E,0x0264,0x027C,0x0298,0x02B2,0x02CC,0x02E4,
0x02F8,0x030E,0x0322,0x0338,0x0352,0x036A,0x0382,0x039A,0x03B2,0x03C8,0x03DE,0x03F4,0x040E,0x0424,0x043C,0x0454,
0x046C,0x047E,0x0490,0x04A8,0x04C2,0x04DC,0x04F2,0x050A,0x0520,0x0538,0x054C,0x0556,0x0562,0x0570,0x0582,0x0594,
0x05A4,0x05B2,0x05C0,0x05CE,0x05DE,0x05EE,0x05FC,0x060C,0x061A,0x062A,0x0638,0x0648,0x0652,0x066C,0x0676,0x0688,
0x0696,0x06A0,0x06BA,0x06D4,0x06EC,0x0706,0x071A,0x0730,0x074A,0x0762,0x077E,0x0796,0x07A6,0x07B6,0x07BC,0x07C6,
0x07D2,0x07DE,0x07EC,0x07FA,0x080A,0x081C,0x082C,0x083E,0x084E,0x085C,0x0864,0x087C,0x0896,0x08AE,0x08C0,0x08DA,
0x08F2,0x090C,0x0926,0x0940,0x095A,0x0974,0x098E,0x09A4,0x09BC,0x09D2,0x09EC,0x0A00,0x0A14,0x0A26,0x0A3A,0x0A4C,
0x0A60,0x0A78,0x0A8E,0x0AA4,0x0AB8,0x0ACA,0x0ADE,0x0AF0,0x0B04,0x0B18,0x0B2C,0x0B40,0x0B56,0x0B6A,0x0B7C,0x0B8E,
0x0BA0,0x0BB6,0x0BC8,0x0BDC,0x0BF0,0x0C04,0x0C14,0x0C24,0x0C38,0x0C50,0x0C68,0x0C7A,0x0C8E,0x0CA0,0x0CB4,0x0CC6,
0x0CF0,0x0D0A,0x0D34,0x0D5E,0x0D86,0x0DAC,0x0DD6,0x0DEE,0x0E06,0x0E1E,0x0E36,0x0E4E,0x0E66,0x0E7E,0x0E96,0x0EB8,
0x0EDC,0x0F02,0x0F28,0x0F4A,0x0F6E,0x0F90,0x0FA8,0x0FC0,0x0FE2,0x0FF8,0x1016,0x102A,0x1040,0x1054,0x1062,0x1078,
0x108C,0x10A2,0x10B0,0x10C4,0x10D4,0x10E2,0x10F0,0x10FE,0x110C,0x111A,0x1128,0x113A,0x1150,0x1168,0x117A,0x118C,
0x119E,0x11AE,0x11BC,0x11CC,0x11DC,0x11EC,0x11FE,0x1210,0x121E,0x122C,0x123A,0x124A,0x125A,0x1274,0x128C,0x12A4,
0x12BC,0x12D6,0x12F0,0x1308,0x1320,0x1338,0x134E,0x1366,0x137C,0x1394,0x13A8,0x13C0,0x13D8,0x13F0,0x1406,0x141C,
0x1432,0x1446,0x145A,0x146A,0x1480,0x1492,0x14AC,0x14C6,0x14DE,0x14F6,0x151E,0x1544,0x156C,0x1592,0x15BA,0x15E4,
0x15F0,0x15FC,0x1616,0x162E,0x1644,0x1666,0x167E,};

const U32 Hindi16_RangeOffset[11]= {
0,1,58,76,81,106,122,159,186,193,238,};

const U8  Hindi16_Data[5802]= {
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xFF,0x60,0x30,0x18,0xC,0x6,0x83,0xC1,0x60,0x30,0x18,0xC,0x6,0xFF,0x0,0x80,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x10,0x0,0x0,0x0,0x0,0x0,
0x4,0x0,0x6,0x0,0x82,0xE7,0x7,0x22,0x80,0x8,0xFC,0x3,0x88,0x0,0x22,0x84,
0x8,0x1E,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0xE7,0x7,0x22,0x80,0x8,0xFC,0x3,0x88,0x0,0x22,0x84,0x8,0x1E,0x2,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x9E,0x7F,
0x20,0x9,0x20,0x9,0xFC,0x9,0x20,0x9,0x20,0x9,0x21,0x9,0x1E,0x9,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x41,0xE0,0x3,0x1,
0xF8,0x1,0x10,0x82,0xE0,0x3,0x2,0x20,0x0,0x0,0x0,0x80,0x1,0x2,0x10,0x0,
0x81,0xFF,0x41,0xE0,0x3,0x1,0xF8,0x1,0x10,0x82,0xE0,0x3,0x2,0x20,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x21,0x0,0x1,0x3E,0x0,0x2,0x10,0x82,
0xE0,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,
0x2,0x40,0x7,0x3E,0x1,0x28,0x0,0x85,0x20,0xE0,0x3,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0xFF,0x7,0x1,0x58,0x3,0x59,0x0,
0x3E,0x60,0x9,0x44,0x1,0x90,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xFE,0x1F,0x4,0xB0,0x3,0x89,0x20,0x11,0x4,0x1,0x11,0x40,
0x12,0x80,0x1,0x0,0x0,0x0,0x0,0x20,0x0,0x12,0x60,0x0,0x80,0xFF,0x9,0x41,
0x8,0x42,0x18,0x83,0x0,0x8,0x80,0x0,0x18,0x0,0x0,0x0,0x0,0x0,0x0,0x2,
0x60,0x0,0x84,0xFF,0x9,0x41,0x8,0x42,0x18,0x83,0x0,0x8,0x80,0x0,0x18,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x9,0x41,0x8,0x42,0x18,0x83,
0x0,0x8,0x80,0x0,0x18,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x40,0x0,0x84,0xFF,
0x9,0x41,0x8,0x42,0x18,0x83,0x0,0x8,0x80,0x0,0x18,0x0,0x0,0x0,0x0,0x0,
0x20,0x0,0x20,0x1,0x60,0x0,0x0,0x70,0xFE,0x41,0x12,0x20,0x9,0xFC,0x4,0x48,
0x2,0x24,0x21,0x92,0xE0,0x48,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x20,0x0,0x60,0x0,0x40,0x70,0xFE,0x41,0x12,0x20,0x9,0xFC,0x4,0x48,0x2,0x24,
0x21,0x92,0xE0,0x48,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,
0x0,0x4,0x0,0x8,0x9E,0x7F,0x20,0x9,0x20,0x9,0xFC,0x9,0x20,0x9,0x20,0x9,
0x21,0x9,0x1E,0x9,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x0,0x5,
0x0,0xA,0x0,0xC,0x9E,0x7F,0x20,0x9,0x20,0x9,0xFC,0x9,0x20,0x9,0x20,0x9,
0x21,0x9,0x1E,0x9,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xFE,0x1F,0x4,0xB0,0x6,0x31,0x21,0x22,0x64,0x4,0x4B,0x0,0x1,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x41,0x20,
0xB,0x25,0xC1,0x24,0x18,0x7,0xFC,0x0,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,0x80,0x8,0x8A,0x90,0x8,0x89,0x60,0x8,
0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x82,
0x20,0x8,0x9C,0x20,0x8,0xC2,0xC0,0xB,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x80,0xFF,0x41,0xE0,0x3,0x41,0xF8,0x0,0x8,0x42,0xE0,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x80,0xE0,0x9,
0x88,0x40,0x8,0xC4,0x80,0xB,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xC0,0xFF,0x7,0x8,0x18,0x3,0x21,0x81,0xF1,0x40,0x28,0x8,
0x9,0x3C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xFE,0x1F,0x40,0x0,0x8,0xF8,0x21,0x24,0x84,0x4,0x91,0xC0,0x11,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x44,0xF8,0x8,0x1,
0xE1,0x3F,0x80,0x84,0x90,0xE0,0x11,0x8,0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xF0,0x7F,0x80,0xE0,0x8,0x90,0x0,0xF,0x91,0xE0,0x8,0x80,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x87,0x0,0x3,0x2,0x4,
0x10,0x80,0x8,0x1C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,
0x21,0x80,0x3,0x22,0x8,0x42,0x10,0x44,0xC0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xFC,0x7,0xC1,0x7,0x1,0x7C,0x0,0x42,0x8,0x1E,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x41,0x80,0x3,0x2,0x88,0x41,
0x12,0x64,0xC0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xFE,0x9F,0x48,0x10,0x9,0x22,0x41,0x24,0x70,0x4,0x80,0x0,0x10,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x80,0x0,0x8,0xFE,0x10,
0x8,0x81,0x20,0x8,0x84,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xE,0x10,0x7D,0x91,0x0,0x9,0x8C,0x30,0x8,0xC4,0x80,0xB,0x80,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x10,0xC0,0x1,0x2,0x10,
0x0,0x21,0x20,0x6,0x9C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xE,0x10,0x7D,0x91,0x20,0x8,0x8C,0x20,0x8,0xC2,0xC0,0xB,0x80,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x1,0x1,0x8,0x7E,0x8,0x42,
0x11,0x8C,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,
0x1,0x1,0x8,0x7E,0x8,0x42,0x11,0x8C,0x0,0x4,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x80,0xFF,0x9,0x41,0x8,0x42,0x10,0x2,0x1F,0x80,0x0,0x4,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x9F,0x8,0x10,
0x7,0x22,0x41,0x26,0xB0,0x4,0x50,0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x80,0xFF,0x1,0xC1,0x9,0x45,0x48,0x82,0x1C,0xB8,0x0,0x4,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6,0x90,0x7E,0x89,0xA0,0x8,
0x88,0xE0,0xF,0x89,0x60,0x8,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xF0,0x7F,0x88,0x80,0x8,0x88,0xE0,0xF,0x89,0x60,0x8,0x80,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,0x80,0x8,
0x84,0x30,0x8,0xC6,0x80,0xB,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xE0,0x1F,0x2,0x4,0x9,0xC,0x10,0x40,0x0,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xE0,0x1F,0x2,0x4,0x9,0xC,0x10,0x40,0x20,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x80,0x0,0x8,0xCE,0x10,0xB,0x99,
0x20,0x8,0x84,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xFE,0x1F,0x10,0x0,0x2,0xEE,0x20,0x22,0x44,0x84,0x88,0xE0,0xE,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x10,0x0,0x2,0xEE,
0x20,0x22,0x44,0x84,0x88,0xE0,0xE,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x80,0xFF,0x1,0xC1,0x9,0x41,0x8,0x82,0x18,0xB8,0x0,0x4,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x7C,0x91,0x10,0x9,0x90,0x90,
0x8,0x86,0x40,0x8,0x88,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0xFF,0x19,0x41,0x9,0x52,0x10,0x3,0x1F,0x80,0x0,0x4,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x84,0x40,0x8,0x85,0xE0,0xF,0x84,
0x80,0x8,0x90,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,
0xC1,0x7,0x1,0x78,0x20,0x82,0x8,0x2,0x10,0x80,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x4,0x0,0x0,0x0,0x0,0x80,0x13,0x5,0x6,0x2,0x45,0xE,
0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0x11,0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,
0x0,0x78,0x8,0x11,0x44,0xE0,0x7,0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x0,0x0,
0x0,0x0,0x0,0x0,0x38,0x88,0x0,0xE1,0x1F,0x4,0x8,0x10,0x20,0x40,0x80,0x0,
0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x1C,0x20,0x1F,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x70,
0x44,0x4C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x70,0x4,0x7C,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x10,0x38,0x44,0x1C,0x0,0x0,
0x1,0x12,0xC,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x1,0x6,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40,
0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0x5,0xA,
0xC,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x80,0x84,0x1,
0xF8,0x11,0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x0,0x1,0x6,0x8,0x7F,
0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x40,0x40,0x40,0xF8,0x11,
0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x4,0x5,0xA,0xC,0x7F,0x8,0x8,
0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x82,0x20,0x0,0x0,0x0,0x0,0x4,0x20,0x2,0x38,0x38,0x80,0x8,0x3,0x91,0x38,
0x12,0x3C,0x81,0x80,0x10,0xE0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x84,0x10,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xE0,0x1F,0x0,0x0,0x80,0x80,0x80,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x88,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x4,0xB0,0x6,0x31,0x21,0x22,
0x64,0x4,0x4B,0x0,0x1,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xFE,0x1F,0x41,0x20,0xB,0x25,0xC1,0x24,0x18,0x7,0xFC,0x20,0x10,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,0x80,0x8,
0x8A,0x90,0x8,0x89,0x60,0x8,0x80,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xFE,0x1F,0x40,0x0,0x8,0xF8,0x21,0x22,0x84,0x4,0x91,0xC0,
0x11,0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,0xC1,0x7,
0x1,0x7C,0x0,0x42,0x8,0x1E,0x0,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0xFF,0x41,0x80,0x3,0x2,0x88,0x41,0x12,0x64,0xC0,0x1,0x0,0x8,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x9F,0x8,0x10,0x7,0x22,0x41,0x26,
0xB0,0x4,0x50,0x20,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xF0,0x7F,0x88,0x80,0x8,0x84,0x30,0x8,0xC6,0x80,0xB,0x82,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0xFF,0x7,0x1,0x58,0x3,
0x59,0x0,0x1E,0x60,0x19,0x44,0x4,0x90,0x0,0x60,0x0,0x8,0x0,0xC,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x10,0x60,0x7,0x89,0x90,0x8,0x41,0x20,0x2,
0x64,0x0,0x2,0xC0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x36,0x49,0xA2,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x36,0x49,0x22,0x40,0x20,0xC0,0x0,0x0,0x49,0x92,0x24,0x0,0x0,0x0,0x0,0x94,
0x52,0x4A,0x29,0xA5,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30,0x92,0xC4,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x23,0x49,0xC,0x41,0x20,0x8,0x0,0x0,0x0,0x0,
0x0,0x0,0xE0,0x88,0x40,0x20,0xF,0x2,0x2,0x2,0x0,0x0,0x0,0x0,0x0,0x0,
0xE0,0x80,0x40,0x18,0x90,0x89,0x83,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x42,
0x42,0x42,0x24,0x18,0x24,0x24,0x18,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,
0x80,0x38,0x51,0x42,0x78,0x1,0x2,0x4,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x1C,0x2,0x2,0xC,0x2,0x32,0x3C,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x40,0x80,0x0,0x71,0xA2,0xC4,0x9,0x23,0x82,0x3,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x20,0x10,0x8,0x4,0x2,0x2,0x42,0x3C,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x60,0x48,0x24,0xC,0xC,0x8,0x4,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x57,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x60,0x7E,0x89,0x90,0x8,
0xFE,0x10,0x8,0x89,0xE0,0x8,0x90,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xFE,0x1F,0x40,0xC0,0xF,0x11,0x41,0x24,0x70,0x4,0x84,0x0,
0x11,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x80,
0x0,0x8,0xFF,0x40,0x8,0x82,0x20,0x8,0x84,0x80,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xE0,0x1F,0x4,0xB,0x19,0x30,0x50,0x90,0x0,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x0,0xA4,0x9F,0x44,0xE0,0x8,0x13,0x1,0x2F,
0x40,0x4,0xC8,0x0,0x16,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xC0,0x7C,0x92,0x20,0x9,0x9C,0x20,0xE,0xA1,0x0,0x9,0x88,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x0,0xA4,0x9F,0x44,0xE0,0x8,0x32,
0x21,0x21,0x20,0x6,0xB8,0x0,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xFE,0x1F,0x4,0xB0,0x6,0x31,0x21,0x22,0x64,0x4,0x4B,0x20,
0x1,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,
0x40,0x0,0x8,0xF8,0x21,0x24,0x84,0x4,0xD1,0xC0,0x15,0x40,0x0,0x4,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x40,0x0,0x8,0xF8,0x21,0x24,
0x84,0x4,0xD1,0xC0,0x15,0x40,0x80,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xFE,0x9F,0x8,0x10,0x7,0x22,0x41,0x26,0xB0,0x4,0x52,0x20,0x2,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x9F,0x8,0x10,
0x7,0x22,0x41,0x26,0xB0,0x4,0x52,0x20,0x2,0x0,0x80,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x80,0xFF,0x9,0x41,0x8,0x42,0x10,0x2,0x1F,0xA0,0x80,0x4,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x84,0x40,0x8,
0x85,0xE0,0xF,0xC4,0x80,0xA,0x90,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x80,0xFF,0x21,0x0,0x1,0x39,0x30,0x2,0x11,0x10,0x0,0x1,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x2,0x40,0x0,0x79,
0xC0,0x12,0x90,0x4,0x4,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xFC,0x7,0x81,0x5,0x71,0x44,0x90,0x81,0x5,0x10,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x47,0x0,0x19,0x25,0x98,0x60,0x4,0x1E,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x1F,0x2,0x4,0xA,0x12,0x24,
0x30,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x27,0x80,0x0,
0x1C,0x8,0x20,0x4,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,
0x1F,0x84,0xF,0x1,0x3E,0x80,0x8,0xE1,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xFC,0x7,0x80,0x7,0x8,0x10,0x40,0x4,0xE,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x0,0x60,0xC,0x21,0x61,0x1C,0x84,
0x20,0x4,0x3C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,
0x1,0x0,0x0,0xF8,0x8,0x41,0x8,0x44,0xC0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x80,0xFF,0x41,0xE0,0x3,0x1,0xF8,0x7,0x8,0x42,0xE0,0x1,
0x2,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,0x80,0x3,0x10,0xC0,
0x11,0x81,0x3,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x1F,0x2,
0x6,0x2,0x2,0x4,0x10,0xC1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xFC,0x87,0x0,0x7,0x22,0x4,0x11,0x84,0x8,0x1C,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xE0,0x1F,0x84,0xF,0x1,0x3E,0x80,0x8,0xE1,0x1,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,0x1,0x7,0x2,0xC4,0x90,0x84,0xC,0x1C,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x27,0x82,0x8,0x22,0x88,
0xC0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,
0x0,0x0,0x7E,0x4,0x10,0x80,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xE,0x44,0x17,0x1,0x4,0xC,0xC,0x40,0x4,0xE,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x41,0x80,0x3,0x2,0x8,0x40,0x8,0xC4,
0xC0,0x9,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE,0x44,0x17,0x81,0x0,
0xC,0x8,0x20,0x4,0xF,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,
0x1F,0x0,0x0,0x3E,0x2,0x14,0x30,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xE0,0x1F,0x0,0x0,0x3E,0x2,0x14,0x30,0x0,0x40,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xE0,0x9F,0x0,0x1,0x2,0x4,0xF0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x89,0x40,0x1C,0x22,0x90,0x1,0xB,0x40,
0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x1F,0x0,0x7,0x5,
0x12,0xC8,0xE0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6,0xA4,0x97,
0x80,0x2,0x8,0xF8,0x91,0x80,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xFC,0x87,0x0,0x2,0x8,0xF8,0x91,0x80,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x87,0x0,0x2,0x4,0xC,0x60,0x4,0xE,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7F,0x8,0x8,0x9,0x6,0x4,0x8,
0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7F,0x8,0x8,0x9,0x6,0x4,0x8,
0x12,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,0x0,0x0,0x4E,0xC4,
0x90,0x81,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,
0x7F,0x0,0x0,0x0,0xEE,0x10,0x11,0x11,0x11,0x11,0xEE,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x0,0x0,0x0,0xEE,0x10,0x11,0x11,
0x11,0x11,0xEE,0x0,0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x1F,0x0,
0x7,0x1,0x2,0x88,0xE0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x38,0x17,0x41,0x4,0x10,0x24,0x60,0x0,0x1,0x8,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xE0,0x9F,0x1,0x5,0x12,0x44,0xF0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x47,0x0,0x1,0x5,0xF8,0x41,0x0,0x2,0x10,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0x1F,0x84,0xF,0x1,0x3C,0x8,
0x10,0x20,0x80,0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xFE,0xFF,0x1F,0x4,0x4,0xB0,0xB0,0x6,0x71,0x31,0x21,0x22,0x22,
0x64,0x64,0x4,0xB,0x4B,0x0,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x4,0x80,0x6,0x3F,0x41,0x22,
0x44,0x84,0x48,0x20,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0xFF,0x1F,0x4,0x4,0xF8,0xB0,0x6,0x41,
0x31,0xE1,0x23,0x22,0x80,0x64,0x84,0x10,0x4B,0xE0,0x1,0x1,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xFE,0xFF,0x1F,0x4,0x41,0xF8,0x20,0xB,0x41,0x24,0xE1,0xE3,0x24,0x80,0x18,
0x87,0x10,0xFC,0xE0,0x1,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0xFF,0x7F,0x10,0x88,0xF0,
0x81,0x8,0x41,0x8A,0xF0,0x91,0x8,0x20,0x89,0x10,0x62,0x8,0x1E,0x80,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x80,0xFF,0xFF,0x41,0x4,0xE1,0x23,0x8,0x41,0x4E,0xF8,0x8,0x2,0x48,0x18,
0x42,0xBC,0xE0,0x1,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0xFF,0x1F,0x0,0x40,0x70,0x0,
0x8,0x10,0xF8,0x1,0x2E,0x24,0x44,0x84,0x4,0x7,0x91,0x0,0xC0,0x11,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x40,
0x0,0x7,0xE,0x90,0x4,0x72,0x20,0xC,0xBC,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x10,0xE0,0x1,0x1,0xE0,0x0,0x44,0x20,0x6,
0x9C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x40,
0x0,0x7,0xA,0x90,0x4,0x72,0x20,0xC,0xBC,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x10,0xE0,0x1,0x1,0x10,0x1,0x3E,0xA0,0x6,
0x9C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x40,
0x80,0x7,0x4,0x40,0x4,0x7B,0x70,0xA,0x98,0x80,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,0xE0,0x8,0x81,0xE0,0x8,0x88,0xC0,0xF,
0x8C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,
0xE0,0x8,0x81,0xE0,0x8,0xC4,0x80,0xB,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x8,0xE0,0x0,0x1,0x10,0x1,0x3E,0x10,0x6,
0x9E,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xE0,0xFF,0x1F,0x2,0x1,0x6,0x3,0x2,0x1,0x2,0x1,0x4,0x2,0x10,0x89,0xC0,
0xE1,0x0,0x0,0x0,0x2,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xFC,0xFF,0x87,0x40,0x0,0x83,0x3,0x2,0x11,0x4,0x82,0x10,0x8,
0x82,0x48,0x4,0x1C,0xE,0x0,0x0,0x80,0x0,0x0,0x4,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0xFF,0x21,0x20,0x80,0x83,0x3,0x22,
0x22,0x8,0xA,0x42,0x50,0x10,0x44,0x44,0xC0,0xC1,0x1,0x0,0x0,0x20,0x0,0x0,
0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0xFF,
0x41,0x10,0xE1,0x83,0x8,0x1,0x45,0xF8,0x24,0x2,0x28,0x11,0x42,0x86,0xE0,0x1,
0x4,0x0,0x0,0x20,0x0,0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xE0,0xFF,0x1F,0x4,0x82,0xCF,0x7,0x81,0x0,0x3E,0x1F,0x80,0x40,0x8,
0x85,0xE0,0xF1,0x0,0x0,0x0,0x2,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xFC,0xFF,0x7,0x81,0xC0,0x87,0x3,0x1,0x1,0x7C,0x62,
0x0,0x4A,0x42,0x48,0x6,0x1E,0xE,0x0,0x0,0x80,0x0,0x0,0x4,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xFF,0x1F,0x0,0x4,0x0,0x8,0xBE,
0x1F,0x82,0x20,0x14,0x45,0x30,0x8C,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x84,0x70,0x8,0x81,0xE0,0x8,0x91,
0x90,0xF,0x92,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,
0x7F,0x84,0x70,0x8,0x81,0xE0,0x8,0x91,0x90,0xC,0xB2,0x40,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xFF,0x1F,0x0,0x2,0xC0,
0x7,0xAE,0x0,0x22,0x1E,0x24,0x44,0x10,0x88,0x40,0x10,0x0,0x40,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x41,0xC0,0x3,0x2,0xE0,0x1,
0x12,0xD4,0xD0,0x0,0x9,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xF8,0xFF,0x11,0x10,0x88,0x8,0x85,0x5,0x1F,0x3,0x41,0x1,0x91,0x0,0x41,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x98,0x97,0x40,0x2,
0x7E,0x4,0x90,0x80,0x3,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0xFF,0x1,0x0,0x0,0xF8,0x8,0x41,0x8,0x44,0xC0,0x1,0x4,0x40,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,0x0,0x0,0x7F,0x10,0x20,0x80,0x0,0x4,
0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF8,0x1,0xC,0x9,0x4,0x41,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x0,0xE9,0x49,0x80,0x3,0x13,0xC0,0x3,
0x4,0x20,0x1,0x6,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x30,0x27,
0x81,0x4,0x1C,0x88,0x10,0x2,0x4,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0xC0,0x0,0xE9,0x49,0x80,0x3,0x32,0x48,0x0,0x22,0xE0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x41,0x22,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,0xC1,0x7,0x1,0x78,0x20,0x83,0xA,0x22,
0x10,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7F,0x0,0x0,0x11,0xE,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x41,0x20,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x28,0x22,
0x80,0x4A,0xC4,0x1,0xF8,0x11,0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x0,
0x1,0x0,0xF8,0x11,0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x41,0x20,0x20,
0xF8,0x11,0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x41,0x22,0x20,0xF8,0x11,
0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x78,0xA,0x11,0x44,0xE0,0x7,0x1,
0x2,0x4,0x8,0x10,0x20,0x40,0x0,0x0,0x0,0x0,0x0,0xE0,0x91,0x50,0x4,0x43,
0xA0,0x1F,0x10,0x80,0x0,0x4,0x20,0x0,0x1,0x8,0x40,0x0,0x0,0x0,0x0,0x0,
0x0,0xC0,0x23,0x42,0x29,0x18,0x4,0xF2,0x3,0x4,0x40,0x0,0x4,0x40,0x0,0x4,
0x40,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x38,0x88,0x2,0xE1,0x1F,0x4,
0x8,0x10,0x20,0x40,0x80,0x0,0x1,0x0,0x0,0x0,0x0,0x40,0x78,0x88,0x0,0xE1,
0x1F,0x4,0x8,0x10,0x20,0x40,0x80,0x0,0x1,0x0,0x0,0x0,0x0,0x40,0x78,0x8A,
0x0,0xE1,0x1F,0x4,0x8,0x10,0x20,0x40,0x80,0x0,0x1,0x0,0x0,0x0,0x0,0x10,
0x1,0x12,0xC,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40,
0x42,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x11,0xA,0xC,
0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x11,0x4A,0xC,
0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x14,0x5,0xA,
0xC,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x48,0x54,0xD0,
0xC0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x48,
0x54,0xD2,0xC0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x40,0x42,0x40,0xF8,0x11,0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x44,
0xC1,0x40,0xF8,0x11,0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x44,0xC9,0x40,
0xF8,0x11,0x8,0x4,0x2,0x81,0x40,0x0,0x0,0x0,0x0,0x14,0x5,0xA,0xC,0x7F,
0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x14,0xD,0xA,0xC,0x7F,
0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xFE,0x1F,0x41,0x20,0xB,0x25,0xC1,0x24,0x18,0x7,0xC2,0x80,0x14,0x20,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,0x80,0x8,
0x8A,0x90,0x8,0xC9,0x60,0xA,0x90,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0xF0,0x7F,0x82,0x20,0x8,0x9C,0x20,0x8,0xC2,0xC0,0xB,0x90,0x80,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x80,0xE0,0x9,
0x88,0x40,0x8,0xC4,0x80,0xB,0x90,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xFE,0x1F,0x40,0x0,0x8,0xF8,0x21,0x22,0x44,0x4,0xC9,0xC0,
0x14,0x40,0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,
0x44,0xF8,0x8,0x1,0xE1,0x3F,0x40,0x86,0xA8,0xE0,0x12,0x8,0x0,0x2,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xE,0x10,0x7D,0x91,0x0,0x9,0x8C,0x30,0x8,0xC4,
0x80,0xB,0x90,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,
0x7F,0x10,0xC0,0x1,0x2,0x10,0x0,0x21,0x20,0x6,0x9C,0x80,0x0,0x4,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0xE,0x10,0x7D,0x91,0x20,0x8,0x8C,0x20,0x8,0xC2,
0xC0,0xB,0x90,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,
0x1,0x1,0x8,0x7E,0x8,0x42,0x11,0xCC,0x0,0x5,0x4,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,0x80,0x8,0x88,0xE0,0xF,0xC9,0x60,0xA,
0x90,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x1,0xC1,
0x9,0x45,0x48,0x82,0x1C,0xB8,0x80,0x4,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x6,0x90,0x7E,0x89,0xA0,0x8,0x88,0xE0,0xF,0xC9,0x60,0xA,0x90,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,0xC1,0x7,0x1,0x78,
0x20,0x83,0xA,0x22,0x10,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,
0x7F,0x88,0x80,0x8,0x84,0x30,0x8,0xC6,0x80,0xB,0x90,0x80,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x82,0x20,0x8,0x9C,0x20,0x8,0xC2,
0xC0,0xB,0x90,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,
0x7F,0x10,0xC0,0x1,0x2,0x10,0x0,0x21,0x20,0x6,0x9C,0x80,0x0,0x4,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x89,0x40,0x1C,0x22,0x90,0x1,0xB,0x40,
0x20,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0x1,0x0,
0x0,0xF8,0x88,0x40,0x8,0x44,0xC0,0x1,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x80,0xFF,0x1,0xC1,0x9,0x41,0x8,0x82,0x18,0xB8,0x80,0x4,0x2,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x87,0x0,0x2,0x4,0xC,0x60,0x4,
0xE,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFC,0x7,0x81,0x5,
0x71,0x44,0x90,0x81,0x5,0x10,0x8,0x0,0x0,0x0,0x0,0x14,0x4D,0xA,0xC,0x7F,
0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x80,0xFF,0x11,0x80,0xC,0x25,0x30,0x81,0x31,0xF0,0x21,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xE0,0x1F,0x2,0x4,0xA,0x12,0x24,0x30,0x0,0x40,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0x1F,0x4,0xB0,0x6,0x31,
0x21,0x22,0x64,0x4,0x4B,0x20,0x1,0x2,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0xFE,0x1F,0x41,0x20,0xB,0x25,0xC1,0x24,0x18,0x7,0xC2,0x80,
0x14,0x24,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,
0x80,0x8,0x8A,0x90,0x8,0xC9,0x60,0xA,0x90,0x20,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,0x80,0x8,0x84,0x30,0x8,0xC6,0x80,0xB,
0x90,0xA0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xF0,0xFF,0x7F,0x0,0x80,0x0,0x0,0x8,0x4E,0xCE,0x10,0x13,0xB,0x19,
0x99,0x20,0x20,0x8,0x4,0x84,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0xFF,0x21,0x10,0x81,0x81,
0x8,0x2,0x42,0x8,0xC,0x42,0xC0,0x18,0x44,0xB8,0xC0,0x1,0x4,0x0,0x0,0x20,
0x0,0x0,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0xF0,0xFF,0x7F,0x8,0x88,0xC0,0x81,0x8,0x22,0x84,0x10,0x34,0x8,0x41,0xC6,0x20,
0x82,0xB,0x1C,0x80,0x0,0x0,0x0,0x8,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0xFF,0xFF,0x41,0x10,0xE1,0x83,0x8,0x1,
0x42,0xF8,0xC,0x2,0xC8,0x18,0x42,0xB8,0xE0,0x1,0x4,0x0,0x0,0x20,0x0,0x0,
0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0xFF,
0x7F,0x10,0x88,0xC0,0x81,0x8,0x2,0x84,0x10,0x33,0x8,0x49,0xC6,0x20,0x83,0xB,
0x1C,0x80,0x0,0x0,0x0,0x8,0x0,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFE,0xFF,0x1F,0x0,0x44,0x30,0x86,0x8,0x21,
0x9,0xC1,0xB8,0x20,0x10,0x32,0x6,0x21,0xB8,0xC0,0x3,0x10,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0xC0,0x0,0x24,0x9F,0x44,0xE0,0x9,0xC3,0x1,0x33,0x90,0x5,0x92,0x80,
0x10,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xC0,0x7C,0x92,
0x20,0x9,0x9C,0x20,0xE,0x91,0x80,0xA,0x90,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x80,0xFF,0x19,0x41,0xB,0x62,0xE0,0x81,0x0,0x84,0xC0,0x3,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xE0,0xFF,
0x1F,0x2,0x4,0x4,0x8,0x88,0x1F,0x94,0x20,0x24,0x41,0x30,0x8C,0x0,0x0,0x1,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF0,0x7F,0x88,
0xE0,0x8,0x81,0xE0,0x8,0x88,0xC0,0xF,0x8C,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x80,0xFF,0x19,0x41,0xB,0x62,0xE0,0x81,0x0,0x84,0xC0,0x3,
0xC,0x90,0x40,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,};



//end:guoyt added for adding hindi lang

