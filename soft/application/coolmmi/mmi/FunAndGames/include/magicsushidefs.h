/*****************************************************************************
* Copyright Statement:
* --------------------
* This software is protected by Copyright and the information contained
* herein is confidential. The software may not be copied and the information
* contained herein may not be used or disclosed except with the written
* permission of UNISCOPE Inc. (C) 2005
*
*****************************************************************************/
/*============================================================================
*
*
* $Id: MagicsushiDefs.h,v 1.3 2007/08/16 07:37:06 liugang Exp $
*
* $Date: 2007/08/16 07:37:06 $
*
* $Name: 1.3 $
*
* $Locker$
*
* $Revision: 1.3 $
*
* $State: Exp $
*
* HISTORY
* Below this line, this part is controlled by CVS. DO NOT MODIFY!!
*----------------------------------------------------------------------------
* $Log: MagicsushiDefs.h,v $
* Revision 1.3  2007/08/16 07:37:06  liugang
* UNI@liugang_20070816 808 all 2.6寸屏的改动 __MMI_UNI_MAINLCD_240X400__
*
* Revision 1.2  2007/08/16 06:16:05  liugang
* UNI@liugang_20070816 808 all 2.6寸屏的改动 __MMI_UNI_MAINLCD_240X400__
*
* Revision 1.1  2007/05/17 07:21:56  baiyanjun
* UNI@baiyanjun_20070517 15:10:01 COOLSAND U25 U26 06B V30 version
*
*
*----------------------------------------------------------------------------
* Upper this line, this part is controlled by CVS. DO NOT MODIFY!!
*============================================================================
****************************************************************************/
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2002
*
*****************************************************************************/
/*****************************************************************************
*
 * Filename:
 * ---------
 * MagicsushiDefs.h
 *
 * Project:
 * --------
 
 *
 * Description:
 * ------------
 *  Game id define Magicsushi.
 *
 * Author:
 * -------
 * -------
 *
 
 ****************************************************************************/
#ifndef GAME_MAGICSUSHI_DEFS_H
#define GAME_MAGICSUSHI_DEFS_H

typedef enum {
    STR_ID_GX_MAGICSUSHI_GAME_NAME = GAME_MAGICSUSHI_BASE + 1,
    STR_ID_GX_MAGICSUSHI_NEW,
    STR_ID_GX_MAGICSUSHI_RESUME,
    STR_ID_GX_MAGICSUSHI_GRADE,
    STR_ID_GX_MAGICSUSHI_HELP,
    STR_ID_GX_MAGICSUSHI_HELP_DESCRIPTION,
    STR_ID_GX_MAGICSUSHI_COUNT
} mmi_magicsushi_str_id_enum;

/* Image ID */
typedef enum {
    /* Image */
    IMG_ID_GX_MAGICSUSHI_GAME_ICON = GAME_MAGICSUSHI_BASE + 1,
    IMG_ID_GX_MAGICSUSHI_GAME_BACKGROUND,
    IMG_ID_GX_MAGICSUSHI_NUMBER_0,
    IMG_ID_GX_MAGICSUSHI_NUMBER_1,
    IMG_ID_GX_MAGICSUSHI_NUMBER_2,
    IMG_ID_GX_MAGICSUSHI_NUMBER_3,
    IMG_ID_GX_MAGICSUSHI_NUMBER_4,
    IMG_ID_GX_MAGICSUSHI_NUMBER_5,
    IMG_ID_GX_MAGICSUSHI_NUMBER_6,
    IMG_ID_GX_MAGICSUSHI_NUMBER_7,
    IMG_ID_GX_MAGICSUSHI_NUMBER_8,
    IMG_ID_GX_MAGICSUSHI_NUMBER_9,
    IMG_ID_GX_MAGICSUSHI_TYPE_NULL,
    IMG_ID_GX_MAGICSUSHI_TYPE_0,
    IMG_ID_GX_MAGICSUSHI_TYPE_1,
    IMG_ID_GX_MAGICSUSHI_TYPE_2,
    IMG_ID_GX_MAGICSUSHI_TYPE_3,
    IMG_ID_GX_MAGICSUSHI_TYPE_4,
    IMG_ID_GX_MAGICSUSHI_TYPE_5,
    IMG_ID_GX_MAGICSUSHI_TYPE_6,
    IMG_ID_GX_MAGICSUSHI_TYPE_7,
    IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC1,
    IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC2,
    IMG_ID_GX_MAGICSUSHI_TYPE_MAGIC3,
    IMG_ID_GX_MAGICSUSHI_TYPE_NULL2,
    IMG_ID_GX_MAGICSUSHI_SELECTED,
    IMG_ID_GX_MAGICSUSHI_PROGRESS,
    IMG_ID_GX_MAGICSUSHI_GAMEOVER,
    IMG_ID_GX_MAGICSUSHI_UPLEVEL,
    IMG_ID_GX_MAGICSUSHI_NOMOREMOVE,
    IMG_ID_GX_MAGICSUSHI_CURSOR,
    IMG_ID_GX_MAGICSUSHI_UP,
    IMG_ID_GX_MAGICSUSHI_DOWN,
    IMG_ID_GX_MAGICSUSHI_COUNT
} mmi_magicsushi_img_id_enum;

#endif /* GAME_MAGICSUSHI_DEFS_H */ 

