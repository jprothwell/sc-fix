/**
 *	Copyright Notice
 *	?2008 - 2009, Coolsand Inc., 1489 43rd Ave. W.,
 *	Vancouver, B.C. V6M 4K8 Canada. All Rights Reserved.
 *  (It is illegal to remove this copyright notice from this software or any
 *  portion of it)
 */

/************************************************************** 
 FILENAME	: CustImageRes.c 
 PURPOSE		: Image Resource file. 
 
 AUTHOR		: Customization Tool 
 DATE		: . 
 **************************************************************/
#include "custdatares.h"
#include "custimgdatahwext.h"
#include "custresdef.h"

//const unsigned short  CSD_CurrMaxImageNumEXT=3;
#ifndef MMI_ON_WIN32
  __attribute__((section (".roresdata"))) 
#endif

const CUSTOM_IMAGE	CSD_nCustImageNamesEXT[]={
{(U8*)&_MAINLCD_ACTIVE_POWERONOFF_POON_GIF_},
{(U8*)&_MAINLCD_IDLESCREEN_WALLPAPER_WALL03_GIF_},
{(U8*)&_MAINLCD_IDLESCREEN_WALLPAPER_WALL02_GIF_},
};



const CUSTPACK_IMAGE_HEADER CSD_image_header={3, (CUSTOM_IMAGE *)CSD_nCustImageNamesEXT};



