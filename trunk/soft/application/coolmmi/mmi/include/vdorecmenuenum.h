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
 *	 VdoRecMenuEnum.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *   Video Recorder Menu ID enum.
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
/* Video Recoder MenuItem Resource ID List */

	/******** Start of ID ********/
	
	MENU_ID_VDOREC_APP,

	/* option menu */
	MENU_ID_VDOREC_OPTION,   //1712   Option ѡ��
	MENU_ID_VDOREC_TO_VDOPLY,  //1713  To Player   ��������
	MENU_ID_VDOREC_CAMCODER_SETTING,  //1714 CamCorder Setting  ������趨
	MENU_ID_VDOREC_VIDEO_SETTING,   //1715  Video Setting    ӰƬ�趨
	MENU_ID_VDOREC_EFFECT_SETTING,   //1716  Effect Setting  ��Ч�趨
	MENU_ID_VDOREC_SELF_RECORD,	  //1717  
	MENU_ID_VDOREC_STORAGE,   //1718   Storage  ����·��
	MENU_ID_VDOREC_RESTORE_DEFAULT,  //1719 Restore Default  ��ԭ�趨

	/* effect setting */
	MENU_ID_VDOREC_EFFECT_NORMAL,  //1720  Normal  ����used
	MENU_ID_VDOREC_EFFECT_GRAYSCALE,  //1721  Grayscale  �ҽ�
	MENU_ID_VDOREC_EFFECT_SEPIA,  //1722  Sepia   ����used
	MENU_ID_VDOREC_EFFECT_SEPIA_GREEN,  //1723  Sepia Green  ������used
	MENU_ID_VDOREC_EFFECT_SEPIA_BLUE,  //1724  Sepia Blue  ������used
	MENU_ID_VDOREC_EFFECT_COLOR_INVERT,  //1725  Color Invert  ��Ƭused
	MENU_ID_VDOREC_EFFECT_GRAY_INVERT,  //1726  Gray Invert  �ҽ׸�Ƭused
	MENU_ID_VDOREC_EFFECT_BLACKBOARD,  //1727 Blackboard  �ڰ�
	MENU_ID_VDOREC_EFFECT_WHITEBOARD,  //1728   Whiteboard  �װ�
	MENU_ID_VDOREC_EFFECT_COPPER_CARVING,  //1729  Copper Carving  ͭ����
	MENU_ID_VDOREC_EFFECT_BLUE_CARVING,  //1730 Blue Carving  ��ɫ���
	MENU_ID_VDOREC_EFFECT_EMBOSSMENT,	  //1731 Embossment  ����
	MENU_ID_VDOREC_EFFECT_CONTRAST,  //1732  Contrast  �Ա�used
	MENU_ID_VDOREC_EFFECT_JEAN,  //1733  
	MENU_ID_VDOREC_EFFECT_SKETCH,  //1734  Sketch  ����
	MENU_ID_VDOREC_EFFECT_OIL,  //1735
	
	/* storage */
	MENU_ID_VDOREC_STORAGE_PHONE,    //1736
	MENU_ID_VDOREC_STORAGE_MEMORY_CARD,	  //1737
//--------------effect-------------added by zhangl for greenstone //
     	MENU_ID_VDOREC_EFFECT_ANTIQUE,
     	MENU_ID_VDOREC_EFFECT_REDRISH,
    	MENU_ID_VDOREC_EFFECT_GREENRISH,
   	MENU_ID_VDOREC_EFFECT_BLUERISH,
     	MENU_ID_VDOREC_EFFECT_BLACKWHITE,
   	MENU_ID_VDOREC_EFFECT_NEGATIVENESS,
    /*+ zhouqin modify for dual t-flash card 20110503*/
    #if defined(DUAL_TFLASH_SUPPORT)
   	MENU_ID_VDOREC_STORAGE_MEMORY_SEC_CARD,	 
    #endif
    /*- zhouqin modify for dual t-flash card 20110503*/
	
	/******** End of ID ********/

