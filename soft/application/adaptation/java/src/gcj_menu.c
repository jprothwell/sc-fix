#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"
#include "internal.h"


int buttonCommands[3];
static commandStruct *menuList = NULL;
static int menuLength;  /* number of commands in the menu */
static int offMenuCommands = 0;   /* 0 if no negative commands map to the
                                     left soft button, 1 otherwise */
unicode *menu_lable = NULL;
unicode *clear_lable = NULL;
unicode * back_lable=NULL;
unsigned char menu_length,clear_length,back_length;
SOFT_BUTTON_T leftButton,rightButton,centerButton;
/* (menuXOffset, menuYOffset) = the upper left corner of system popup menu */ 
static int menuXOffset = LCD_WIDTH*37/97;   
static int menuYOffset = LCD_HEIGHT*37/97;
static int screenWidth=LCD_WIDTH;  /* width of usable screen area */
static int screenHeight=LCD_HEIGHT; /* height of usable screen area */

extern stFontAttribute 	MMI_medium_bold_font;
static int menuCurrent; /* index of currently selected menu item */
static int scrollPosition; /* index of top menu item on screen ? */
static int scrollY;
static int itemHeight;
static int menuHeight;
static int boldHeight;
static int plainHeight;
static int itemSpace;
unicode *_digits_ =L"1234567890"; 
unsigned char  inMenu = KNI_FALSE;

void gcj_StubGrabFullScreen(unsigned char mode)
{
	if (mode == FALSE)
	{
		inFullScreenMode = 0;
	}
	else
	{
		inFullScreenMode = 1;
	}
}


void 
LCDUIsetSoftButton(BUTTON_POSION_T index, const unicode *label, int numChars)
{
	switch (index)
	{
		case BUTTON_LEFT:
			leftButton.label = label;
			leftButton.numChars = numChars;
			break;
		case BUTTON_RIGHT:
			rightButton.label = label;
			rightButton.numChars = numChars;
			break;
		case BUTTON_CENTER:
			centerButton.label = label;
			centerButton.numChars = numChars;
			break;
		default :
			abort();
	}
}
extern unsigned char return_from_native_input;
int checkforinputwindow=0;

int LCDUIcharsWidth(unicode *charArray, int n);

void  gcj_StubEventHandlerUpdateCommandSet(commandStruct *menu ,unsigned int length)
{
	int dummy;
	int availableWidth = 0;
	int longCharsWidth = 0;
	//if (menu_lable == NULL)
	{
		menu_lable = GetString(IDLE_SCREEN_LSK_TEXT);//{0x83DC,0x5355}; //cai dan
		menu_length = UCS2Strlen(menu_lable);
	}
	//if (clear_lable == NULL)
	{
		clear_lable = 	GetString(STR_GLOBAL_CLEAR);// {0x6E05,0x9664};   //qing chu
		clear_length = UCS2Strlen(clear_lable);
	}
	//if (back_lable == NULL)
	{
		back_lable = 	GetString(STR_GLOBAL_BACK);// {0x6E05,0x9664};   //qing chu
		back_length = UCS2Strlen(back_lable);
	}
	if (inMenu)
		inMenu = KNI_FALSE;
	if (set_InputDone && !gcj_IsHavaDelKey())
	{
		//LCDUIsetSoftButton(BUTTON_CENTER, clear_lable, clear_length);
		/*
		//if (!return_from_native_input )
			checkforinputwindow ++;
		//else
		{
			return_from_native_input = 0;
			//checkforinputwindow=0;
		}
		if (checkforinputwindow ==2)
		{
			checkforinputwindow=0;
			set_InputDone = 0;
		}
		*/
	}
	if (menuList != menu && menuList !=NULL) {
		_Jv_Free(menuList);
	}
	//gcj_pageSpay(menu,menu+length);
	menuList   = menu;
	menuLength = length;
	for(int i=0;i<length;i++)
	{
		gcj_TraceOut(0,"commands[%d],priority=%d,id=%d,type=%d,numChars=%d,numLongChars=%d,useLongChars=%d",i,menu[i].priority,menu[i].id,menu[i].type,menu[i].numChars,menu[i].numLongChars,menu[i].useLongChars);
	}
	//LCDUIgetDisplayParams(&screenWidth, &screenHeight,
	//					  &dummy, &dummy, &dummy, &dummy, &dummy);
	if (length <= 0) {
		buttonCommands[BUTTON_LEFT] = buttonCommands[BUTTON_RIGHT] = buttonCommands[BUTTON_CENTER]  = BUTTON_NOT_SET;
		LCDUIsetSoftButton(BUTTON_LEFT, NULL, 0);
		LCDUIsetSoftButton(BUTTON_RIGHT, NULL, 0);
		LCDUIsetSoftButton(BUTTON_CENTER, NULL, 0);
		return;
	} else {
		if (inFullScreenMode == KNI_TRUE) {
		/* This case occurs when we are in full screen mode.
		   We want all commands on the menu screen.
		*/
		buttonCommands[BUTTON_LEFT] = BUTTON_INVOKES_MENU;
		buttonCommands[BUTTON_RIGHT] = BUTTON_INVOKES_MENU;
		buttonCommands[BUTTON_CENTER]  = BUTTON_NOT_SET;
		LCDUIsetSoftButton(BUTTON_LEFT, NULL, 0);
		LCDUIsetSoftButton(BUTTON_RIGHT, NULL, 0);
		LCDUIsetSoftButton(BUTTON_LEFT, NULL, 0);
		LCDUIsetSoftButton(BUTTON_RIGHT, NULL, 0);
		LCDUIsetSoftButton(BUTTON_CENTER, NULL, 0);
		} else
		#if 1
		{
			LCDUIsetSoftButton(BUTTON_LEFT, NULL, 0);
			LCDUIsetSoftButton(BUTTON_RIGHT, NULL, 0);
			LCDUIsetSoftButton(BUTTON_CENTER, NULL, 0);
			buttonCommands[BUTTON_CENTER]  = BUTTON_NOT_SET;
			/* highest-priority negative command on BUTTON_RIGHT soft button */
			if (menu[0].type == COMMAND_TYPE_BACK ||
				menu[0].type == COMMAND_TYPE_CANCEL ||
				menu[0].type == COMMAND_TYPE_STOP ||
				menu[0].type == COMMAND_TYPE_EXIT) {
					buttonCommands[BUTTON_RIGHT] = menu[0].id;
					offMenuCommands = 1;
				} else {
					buttonCommands[BUTTON_RIGHT] = BUTTON_NOT_SET; 
					offMenuCommands = 0;
				}
				/* decide between short and long label */
				/* First see if the long label fits, if yes use long label */
				/* else use short label */
				/* labels are clipped if there isn't sufficient space */
				
				if (menu[0].useLongChars == 1) {
					availableWidth = LCD_WIDTH/2 -1;
				longCharsWidth = menu[0].numLongChars *15;
				}
				if (offMenuCommands == 1) {
					if ((menu[0].useLongChars == 1) && (longCharsWidth <= availableWidth)) {
						LCDUIsetSoftButton(BUTTON_RIGHT, menu[0].longChars, menu[0].numLongChars);
					}else {
				  		LCDUIsetSoftButton(BUTTON_RIGHT, menu[0].chars, menu[0].numChars);
					}
				} else {
					/* blank out the left button */
					LCDUIsetSoftButton(BUTTON_RIGHT, NULL, 0);
				}
				
				if (length > 2 || (length == 2 && offMenuCommands == 0)) {
					buttonCommands[BUTTON_LEFT] = menu[0].id;
					LCDUIsetSoftButton(BUTTON_LEFT, menu[0].chars, menu[0].numChars);
					buttonCommands[BUTTON_RIGHT] = BUTTON_INVOKES_MENU;
					LCDUIsetSoftButton(BUTTON_RIGHT, menu_lable, menu_length);
					offMenuCommands=1;
					if (length ==2)
					{
						buttonCommands[BUTTON_LEFT] = menu[0].id;
						LCDUIsetSoftButton(BUTTON_LEFT, menu[0].chars, menu[0].numChars);
						buttonCommands[BUTTON_RIGHT] = menu[1].id;
						LCDUIsetSoftButton(BUTTON_RIGHT, menu[1].chars, menu[1].numChars);
						offMenuCommands=0;
					}
				} else if (length == 1 && offMenuCommands == 0 || length == 2 && offMenuCommands == 1) {
					int idx = (offMenuCommands == 0) ? 0 : 1;
					buttonCommands[BUTTON_LEFT] = menu[idx].id;
					if (menu[idx].useLongChars == 1) {
						availableWidth = LCD_WIDTH/2 -1;
						longCharsWidth = menu[idx].numLongChars*15;
					}
					if ((menu[idx].useLongChars == 1) && (longCharsWidth <= availableWidth)) {
						LCDUIsetSoftButton(BUTTON_LEFT, menu[idx].longChars, menu[idx].numLongChars);
					}else {
						LCDUIsetSoftButton(BUTTON_LEFT, menu[idx].chars, menu[idx].numChars);
					}
				} else { 
						/* length == 0 || (length == 1 & offMenuCommands == 1)*/
					buttonCommands[BUTTON_LEFT] = BUTTON_NOT_SET;
					LCDUIsetSoftButton(BUTTON_LEFT, NULL, 0);
				}
				if (set_InputDone && length>2)
				{
					//LCDUIsetSoftButton(BUTTON_CENTER, clear_lable, clear_length);
					buttonCommands[BUTTON_CENTER] = menu[1].id;
					if (menu[1].useLongChars == 1) {
						availableWidth = LCD_WIDTH/2 -1;
						longCharsWidth = menu[1].numLongChars*15;
					}
					if ((menu[1].useLongChars == 1) && (longCharsWidth <= availableWidth)) {
						LCDUIsetSoftButton(BUTTON_CENTER, menu[1].longChars, menu[1].numLongChars);
					}else {
						LCDUIsetSoftButton(BUTTON_CENTER, menu[1].chars, menu[1].numChars);
					}
				}
				if (length - offMenuCommands > 10) {
				  /* no bold font on the line */
				  itemHeight = plainHeight;
				}
			}
		#else
			{
					if (inFullScreenMode == KNI_TRUE) {
					/* This case occurs when we are in full screen mode.
					   We want all commands on the menu screen.
					*/
					buttonCommands[BUTTON_LEFT] = BUTTON_INVOKES_MENU;
					buttonCommands[BUTTON_RIGHT] = BUTTON_INVOKES_MENU;
					LCDUIsetSoftButton(BUTTON_LEFT, NULL, 0);
					LCDUIsetSoftButton(BUTTON_RIGHT, NULL, 0);
				} else {
					/* highest-priority negative command on LEFT soft button */
					if (menu[0].type == COMMAND_TYPE_BACK ||
					menu[0].type == COMMAND_TYPE_CANCEL ||
					menu[0].type == COMMAND_TYPE_STOP ||
					menu[0].type == COMMAND_TYPE_EXIT) {
						buttonCommands[BUTTON_LEFT] = menu[0].id;
					offMenuCommands = 1;
					} else {
						buttonCommands[BUTTON_LEFT] = BUTTON_NOT_SET; 
					offMenuCommands = 0;
					}
					/* decide between short and long label */
					/* First see if the long label fits, if yes use long label */
					/* else use short label */
					/* labels are clipped if there isn't sufficient space */
			
					if (menu[0].useLongChars == 1) {
						availableWidth = 
						LCDUIcharsWidth(
								menu[0].longChars, 
								MAX_SOFTBUTTON_COMMAND_LENGTH);
					longCharsWidth = 
						LCDUIcharsWidth(
								menu[0].longChars, menu[0].numLongChars);
					}
					if (offMenuCommands == 1) {
						if ((menu[0].useLongChars == 1) && 
						(longCharsWidth <= availableWidth)) {
						LCDUIsetSoftButton(BUTTON_LEFT, menu[0].longChars, 
								   menu[0].numLongChars);
					}else {
					  LCDUIsetSoftButton(BUTTON_LEFT, menu[0].chars, menu[0].numChars);
					}
					} else {
						/* blank out the left button */
						LCDUIsetSoftButton(BUTTON_LEFT, NULL, 0);
					}
			
					if (length > 2 || (length == 2 && offMenuCommands == 0)) {
						buttonCommands[BUTTON_RIGHT] = BUTTON_INVOKES_MENU;
					LCDUIsetSoftButton(BUTTON_RIGHT, menu_lable, menu_length);
					} else if (length == 1 && offMenuCommands == 0 ||
						   length == 2 && offMenuCommands == 1) {
						int idx = (offMenuCommands == 0) ? 0 : 1;
						buttonCommands[BUTTON_RIGHT] = menu[idx].id;
			
					if (menu[idx].useLongChars == 1) {
						availableWidth = 
							LCDUIcharsWidth(
								menu[idx].longChars, 
								MAX_SOFTBUTTON_COMMAND_LENGTH);
						longCharsWidth = 
							LCDUIcharsWidth(
								menu[idx].longChars, 
								menu[idx].numLongChars);
					}
					if ((menu[idx].useLongChars == 1) && 
						(longCharsWidth <= availableWidth)) {
						LCDUIsetSoftButton(BUTTON_RIGHT, menu[idx].longChars, 
								   menu[idx].numLongChars);
					}else {
						LCDUIsetSoftButton(BUTTON_RIGHT, menu[idx].chars, menu[idx].numChars);
					}
					} else { 
						/* length == 0 || (length == 1 & offMenuCommands == 1)*/
						buttonCommands[BUTTON_RIGHT] = BUTTON_NOT_SET;
					LCDUIsetSoftButton(BUTTON_RIGHT, NULL, 0);
					}
				}
			
					/*
					 * When there is 10 or more menu items, the number prefix are not 
					 * drawn. So the line height is just the height of the plain font
					 * because bold font for the number is not used. 
					 *
					 * This is necessary because it cannot be assumed that the height
					 * of the plain font is the same as the bold font. The tallest of
					 * font determines the item height.
					 */
					if (length - offMenuCommands > 10) {
					  /* no bold font on the line */
					  itemHeight = plainHeight;
					}
				}
		#endif
		}
		gcj_StubForceRefreshDisplay();
}

#if 1

void
LCDUIinitMenus()
{
    int ascent, descent, leading;
    //menuList = NULL;
    //inMenu = KNI_FALSE;
    scrollPosition = scrollY = menuHeight = 0;
	/* get the line height of a plain font */
    gcj_StubFontInit(0,0,0,&ascent,&descent,&leading);
    plainHeight = ascent + descent + leading;
    boldHeight = ascent + descent + leading;
    
    /*
     * itemHeight is used for the line spacing for the menu items.
     * However, each line could have a number, which is drawn in bold 
     * font, and the menu text is drawn in plain font. So, the line height 
     * has to be the tallest font on the line.
     */
    itemHeight = (plainHeight > boldHeight) ? plainHeight : boldHeight;
    
    /* these are the colors used in the topbar image, so we try to 
     * match them. */
 /*    
    blackPixel = gcj_StubGraphicsGetPixel(0, 0, 0);
    whitePixel = gcj_StubGraphicsGetPixel(0xffffff, 0, 0);
    lightGrayPixel = gcj_StubGraphicsGetPixel(0xafafaf, 0, 0);
    darkGrayPixel = gcj_StubGraphicsGetPixel(0x606060, 0, 0);
    */
    /* TBD by HI */
    //menuXOffset = 30;
    //menuYOffset = 61;
    /* 
     * There are (menuLength - offMenuCommands) * itemHeight 
     * commands.  
     */
    menuHeight = (menuLength - offMenuCommands) * itemHeight; 
    /* was itemSpace = screenHeight - itemHeight - 3; */
    /* 19 = bottom bar height */
    /* -6 = spacer pixels in menu heading */
    itemSpace = screenHeight - menuYOffset - boldHeight - 6 - gcj_StubGetButtonHeight() - 11;  

    if (menuHeight < itemSpace) {
        menuHeight = itemSpace;
    }
    scrollPosition = 0;
    menuCurrent = 0;
    scrollY = 0;

    /* HI guide says:  back button only appears on the left soft button,
     * but that both buttons should dismiss the menu */
    buttonCommands[BUTTON_LEFT] = BUTTON_DISMISSES_MENU;
    buttonCommands[BUTTON_RIGHT] = BUTTON_DISMISSES_MENU;
    
    LCDUIsetSoftButton(BUTTON_LEFT, back_lable, back_length);
    //LCDUIsetSoftButton(1, NULL, 0);
    
    if (menuHeight <= itemSpace) { 
      //LCDUIsetVerticalScroll(0, 100);
    }
    inMenu = KNI_TRUE;
    //setCommandsFullScreenMode(KNI_FALSE);
}

void
LCDUIpaintMenu()
{
    int curX = menuXOffset;
    int curY = menuYOffset;
    int i;
    short clip[4];
    
    int availableWidth = 0;
    int longCharsWidth = 0;
    
    commandStruct *menu;
    int length;
    
    /* we might lop off the first item, because it's mapped to a button */
    
    menu = menuList + offMenuCommands;
    length = menuLength - offMenuCommands;
    
    clip[0] = menuXOffset;
    clip[1] = menuYOffset;
    clip[2] = screenWidth;
    clip[3] = screenHeight;
    
    LCDUIfillRect(LIGHT_GRAY_COLOR, clip, NULL, 0, 
		  menuXOffset, menuYOffset, screenWidth - menuXOffset, 
		  screenHeight - menuYOffset);

    /* draw top border of menu */
    LCDUIdrawLine(DARK_GRAY_COLOR, clip, NULL, 0, menuXOffset, menuYOffset, 
		  screenWidth, menuYOffset);
    LCDUIdrawLine(0xC3D4F7, clip, NULL, 0, menuXOffset, menuYOffset + 1, 
		  screenWidth, menuYOffset + 1);
    
    /* draw left side border of menu */
    LCDUIdrawLine(DARK_GRAY_COLOR, clip, NULL, 0, menuXOffset, menuYOffset + 1,
		  menuXOffset, screenHeight);
    LCDUIdrawLine(0xC3D4F7, clip, NULL, 0, menuXOffset + 1, menuYOffset + 1,
		  menuXOffset + 1, screenHeight);
    curY += 3; /* top border width + 1 extra pixel */
    curX += 3; /* left side border width + 1 extra pixel */

    curX += (8 + LCDUIcharsWidth(_digits_, 1));
    /* draw the menu header, bold & centered */
    LCDUIdrawChars(blackPixel, clip, NULL, 0,
                   menuXOffset + ((screenWidth - menuXOffset) / 2), 
		   menuYOffset + 2, 0, menu_lable, menu_length);
    
    curY += boldHeight;
    
    /* draw the title separator lines */
    LCDUIdrawLine(DARK_GRAY_COLOR, clip, NULL, 0, menuXOffset + 2 , curY - 1, 
		  screenWidth, curY - 1);
    LCDUIdrawLine(0xC3D4F7, clip, NULL, 0, menuXOffset + 2, curY, 
		  screenWidth, curY);
    curY += 3; /* for title separator line and 1 extra pixel */
    /* draw visible commands */
    for (i = scrollPosition; (curY - menuYOffset - boldHeight - 6) < 
	     itemSpace && i < length; ++i, curY += itemHeight) {
        if (length < 10) {
            LCDUIdrawChars(blackPixel, clip, NULL, 0,
                           menuXOffset + 6, curY, 0, _digits_ + i, 1);
        }
	gcj_TraceOut(0,"commands[%d],priority=%d,id=%d,type=%d,numChars=%d,numLongChars=%d,useLongChars=%d",i,menu[i].priority,menu[i].id,menu[i].type,menu[i].numChars,menu[i].numLongChars,menu[i].useLongChars);
	/* decide between short and long label */
	/* First see if the long label fits, if yes use long label */
	/* else use short label */
	/* labels are clipped if there isn't sufficient space */
	
	if (menu[i].useLongChars == 1) {
	    availableWidth = screenWidth - curX;
	    
	    longCharsWidth = LCDUIcharsWidth(menu[i].longChars, 
					     menu[i].numLongChars);
	}
        if (i == menuCurrent) {
            LCDUIfillRect(blackPixel, clip, NULL, 0,
                          curX, curY, screenWidth, itemHeight);
	    curX += 2;
	    if ((menu[i].useLongChars == 1) && (longCharsWidth <= 
						availableWidth)) {
	        LCDUIdrawChars(whitePixel, clip, NULL, 0,
			       curX, curY, 0,
			       menu[i].longChars, menu[i].numLongChars);
	    } else {
	        LCDUIdrawChars(whitePixel, clip, NULL, 0,
			       curX, curY, 0,
			       menu[i].chars, menu[i].numChars);
	    }
	    curX -= 2;
        } else {
	    curX += 2;
	    if ((menu[i].useLongChars == 1) && (longCharsWidth <= 
						availableWidth)) {
                LCDUIdrawChars(blackPixel, clip, NULL, 0,
			       curX, curY, 0,
			       menu[i].longChars, menu[i].numLongChars);
	    } else {
                LCDUIdrawChars(blackPixel, clip, NULL, 0,
			       curX, curY, 0,
			       menu[i].chars, menu[i].numChars);
	    }
	    curX -= 2;
        }
	
    }
    
    if (menuHeight > itemSpace) { 
	//LCDUIsetVerticalScroll(scrollY * 100 / (menuHeight - itemSpace),
	//		       (itemSpace * 100) / menuHeight);
    }
    gcj_StubForceRefreshDisplay();
}

#else
LCDUIpaintMenu()
{
    int curX = menuXOffset;
    int curY = menuYOffset;
    int i;
    short clip[4];
    gdi_color color_t;
    unsigned short *buff = gcj_StubGetScreenBuffer();
    if (buff==NULL)
		abort();
    UINT32 charWidths,charHeights;
    int ascent,descent,leading;
    //gcj_StubFontInit(0,0,0,&ascent,&descent,&leading);
    //charHeights = ascent + descent +leading;
    charHeights = Get_FontHeight(MMI_medium_bold_font, (U8) gCurrLangIndex);
    charHeights +=1;
    itemSpace =  itemHeight =boldHeight= charHeights;
    int availableWidth = 0;
    int longCharsWidth = 0;
    
    commandStruct *menu;
    int length;
    
    /* we might lop off the first item, because it's mapped to a button */
    
    menu = menuList + offMenuCommands;
    length = menuLength - offMenuCommands;
    
    clip[0] = menuXOffset;
    clip[1] = menuYOffset;
    clip[2] = LCD_WIDTH;
    clip[3] = LCD_HEIGHT-20;
    int menu_width = clip[2]-clip[0];
    int menu_height = clip[3]-clip[1];
    color_t = 44405;//gcj_StubGraphicsGetPixel(LIGHT_GRAY_COLOR,0,0);
    gd_fill_rect16(buff,LCD_WIDTH,clip[0],clip[1],clip[2],clip[3],clip[0],clip[1],clip[2],clip[3],color_t);
    /* draw top border of menu */
    color_t = gcj_StubGraphicsGetPixel(DARK_GRAY_COLOR,0,0);  	
    gd_draw_line16(buff,LCD_WIDTH,clip[0],clip[1],clip[2],clip[1],clip[0],clip[1],clip[2],clip[1],color_t,0);
    color_t = gcj_StubGraphicsGetPixel(0x00ffffff,0,0);  	
    gd_draw_line16(buff,LCD_WIDTH,clip[0],clip[1]+1,clip[2],clip[1]+1,clip[0],clip[1]+1,clip[2],clip[1]+1,color_t,0);
    
    /* draw left side border of menu */
    color_t = gcj_StubGraphicsGetPixel(DARK_GRAY_COLOR,0,0);  	
    gd_draw_line16(buff,LCD_WIDTH,clip[0],clip[1],clip[0],clip[3],clip[0],clip[1],clip[0],clip[3],color_t,0);
    color_t = gcj_StubGraphicsGetPixel(0x00ffffff,0,0);  	
    gd_draw_line16(buff,LCD_WIDTH,clip[0]+1,clip[1],clip[0]+1,clip[3],clip[0]+1,clip[1],clip[0]+1,clip[3],color_t,0);
    curY += 3; /* top border width + 1 extra pixel */
    curX += 3; /* left side border width + 1 extra pixel */
    
    curX += (8 + gcj_StubFontCharWidth( _digits_[0]));
    /* draw the menu header, bold & centered */
    int tmp=0;
    for(i=0;i<menu_length;i++)
    {
		gcj_StubGraphicsDrawChar(buff,menu_lable[i],menuXOffset+(menu_width - menu_length*15)/2+tmp,curY,0x00000000);
		tmp+=15;
    }
    
    curY += charHeights;
    
    /* draw the title separator lines */
    color_t = gcj_StubGraphicsGetPixel(DARK_GRAY_COLOR,0,0);  	
    gd_draw_line16(buff,LCD_WIDTH,clip[0]+2,curY - 1,clip[2],curY - 1,clip[0],clip[1],clip[2],clip[3],color_t,0);
    color_t = gcj_StubGraphicsGetPixel(0x00ffffff,0,0);  	
    gd_draw_line16(buff,LCD_WIDTH,clip[0]+2,curY,clip[2],curY,clip[0],clip[1],clip[2],clip[3],color_t,0);
    curY += 3; /* for title separator line and 1 extra pixel */
#if 1
    /* draw visible commands */
   
    for (i = scrollPosition;  i < length; ++i, curY += itemHeight) {
        if (length < 10) {
			gcj_StubGraphicsDrawChar(buff,_digits_[i],menuXOffset+6,curY,0x00000000);
        }
	
	/* decide between short and long label */
	/* First see if the long label fits, if yes use long label */
	/* else use short label */
	/* labels are clipped if there isn't sufficient space */
	
	if (menu[i].useLongChars == 1) {
	    availableWidth = menu_width - curX;
	    
	    longCharsWidth = 15*menu[i].numLongChars;
	}

        if (i == menuCurrent) {
		color_t = gcj_StubGraphicsGetPixel(0x00000000,0,0); 	
		gd_fill_rect16(buff,LCD_WIDTH,curX,curY,clip[2],curY+itemHeight,clip[0],clip[1],clip[2],clip[3],color_t);
	    curX += 2;
		
		color_t = gcj_StubGraphicsGetPixel(0x00ffffff,0,0); 	
	    if ((menu[i].useLongChars == 1) && (longCharsWidth <= 
						availableWidth)) {
		int j;
		tmp=0;
		for (j=0;j<menu[i].numLongChars;j++)
		{
			gcj_StubGraphicsDrawChar(buff,menu[i].longChars[j],curX+tmp,curY,color_t);
			tmp+=15;
		}
	    } else {
			int j;
			tmp=0;
			for (j=0;j<menu[i].numChars;j++)
			{
				gcj_StubGraphicsDrawChar(buff,menu[i].chars[j],curX+tmp,curY,color_t);
				tmp+=15;
			}
	    }
	    curX -= 2;
        } else {
	    curX += 2;
		color_t = gcj_StubGraphicsGetPixel(0x00000000,0,0); 	
	    if ((menu[i].useLongChars == 1) && (longCharsWidth <= 
						availableWidth)) {
			int j;
			tmp=0;
			for (j=0;j<menu[i].numLongChars;j++)
			{
				gcj_StubGraphicsDrawChar(buff,menu[i].longChars[j],curX+tmp,curY,color_t);
				tmp+=15;
			}
			} else {
				int j;
				tmp=0;
				for (j=0;j<menu[i].numChars;j++)
				{
					gcj_StubGraphicsDrawChar(buff,menu[i].chars[j],curX+tmp,curY,color_t);
					tmp+=15;
				}
	    }
	    curX -= 2;
        }
	
    }
    /*
    if (menuHeight > itemSpace) { 
	LCDUIsetVerticalScroll(scrollY * 100 / (menuHeight - itemSpace),
			       (itemSpace * 100) / menuHeight);
    }
    LCDUIrefresh(menuXOffset, menuYOffset, screenWidth, screenHeight);
    */
	#endif
	buttonCommands[BUTTON_RIGHT] = BUTTON_DISMISSES_MENU;
	gcj_StubForceRefreshDisplay();
}
#endif
static void
hideMenu()
{
    if(inMenu) {
		/*
	if (set_InputDone)
		return_from_native_input=1;
		*/
	/* restore the arrows */
	//LCDUIsetVerticalScroll(0, 100);
	
	//restoreEmulatorScreenMode();
	
	/* restore the soft buttons */
	//LCDUIupdateCommandSet(menuList, menuLength);
	gcj_StubEventHandlerUpdateCommandSet(menuList, menuLength);
	gcj_StubForceRefreshDisplay();
    }
    inMenu = KNI_FALSE;
}

void  gcj_StubEventHandlerPaintMenu(void)
{
	if (!menuList)
		return;
	inMenu = KNI_TRUE;
    	LCDUIinitMenus();
	LCDUIpaintMenu();
}

void  gcj_StubEventHandlerDismissMenu(void)
{
	hideMenu();
}

/*
 * This function repaints only the screen area that is changed by key 
 * up/down. curr is the index of the newly hiligthed menu item, and 
 * prev is the index of the previusly hiligthed menu item.
 */
void
LCDUIpaintMenuPartial(int first, int second)
{
#if 1
    commandStruct *menu = menuList + offMenuCommands;
    /*int curX = 0, curY1 = 0, curY2 = 0;*/
    int curX = menuXOffset, curY1 = menuYOffset, curY2 = menuYOffset;
    short clip[4];
    int bg, fg;
    
    int availableWidth = 0;
    int longCharsWidth = 0;
    
    clip[0] = menuXOffset;
    clip[1] = menuYOffset;
    clip[2] = screenWidth - menuXOffset;
    clip[3] = screenHeight - menuYOffset;
    
    /* paint first */
    curX += (3 + 8 + LCDUIcharsWidth(
				  _digits_, 1));

    curY1 += (boldHeight + 6 + (first - scrollPosition)*itemHeight);
    
    if (first == menuCurrent) {
	bg = blackPixel;
	fg = whitePixel;
    } else {
	bg = LIGHT_GRAY_COLOR;
	fg = blackPixel;
    }
    
    LCDUIfillRect(bg, clip, NULL, 0,
		  curX, curY1, screenWidth - curX, itemHeight);
    curX += 2;
    availableWidth = screenWidth - curX;
    
    /* decide between short and long label */
    if (menu[first].useLongChars == 1) {
        longCharsWidth = LCDUIcharsWidth(
					 menu[first].longChars, 
					 menu[first].numLongChars);
    }
    if ((menu[first].useLongChars == 1) && 
	(longCharsWidth <= availableWidth)) {
	LCDUIdrawChars(fg, clip, NULL, 0,
		       curX, curY1, 0,
		       menu[first].longChars, menu[first].numLongChars);
    } else {
        LCDUIdrawChars(fg, clip, NULL, 0,
		       curX, curY1, 0,
		       menu[first].chars, menu[first].numChars);
    }
    curX -= 2;

    /* paint second */
    curY2 = curY1 + itemHeight;
    
    if (second == menuCurrent) {
	bg = blackPixel;
	fg = whitePixel;
    } else {
	bg = LIGHT_GRAY_COLOR;
	fg = blackPixel;
    }
    
    LCDUIfillRect(bg, clip, NULL, 0,
		  curX, curY2, screenWidth - curX, itemHeight);
    curX += 2;
    /* decide between short and long label */
    if (menu[second].useLongChars == 1) {
        longCharsWidth = LCDUIcharsWidth(
					 menu[second].longChars, 
					 menu[second].numLongChars);
    }
    if ((menu[second].useLongChars == 1) && 
	(longCharsWidth <= availableWidth)) {
        LCDUIdrawChars(fg, clip, NULL, 0,
		       curX, curY2,0 ,
		       menu[second].longChars, menu[second].numLongChars);
    } else {
        LCDUIdrawChars(fg, clip, NULL, 0,
		       curX, curY2, 0,
		       menu[second].chars, menu[second].numChars);
    }

    curX -= 2;
    /* 
     * LCDUIrefresh expects the upper left and lower right corner of the 
     * rectangle to refresh.
     */
    //LCDUIrefresh(curX, curY1, screenWidth, curY2 + itemHeight);
    gcj_StubForceRefreshDisplay();
	#endif
}

int gcj_StubMenuKeyEvent(int type,int code)
{
	int maxMeunItem = menuLength - offMenuCommands;
	int tmp =menuCurrent;
	commandStruct * menu = menuList + offMenuCommands;
#if 0
	if (type == 2)
	{
		switch (code)
		{
			case UP:
				menuCurrent--;
				if (menuCurrent<0)
					menuCurrent=maxMeunItem-1;
				LCDUIpaintMenu();
				break;
			case DOWN:
				menuCurrent++;
				if (menuCurrent>=maxMeunItem)
					menuCurrent = 0;
				LCDUIpaintMenu();
				break;
			case FIRE:
			case KEY_CLEARJ:
				menuEventCommand(menu,menuCurrent);
				//hideMenu();
			default :
				break;
		}
		//LCDUIpaintMenuPartial(tmp,menuCurrent);
	}
	#endif
	if (type == RELEASED)
	{
		switch (code)
		{
			case UP:
				if (menuCurrent > 0 || scrollY > 0) {
					
					if (menuCurrent > 0) {
					--menuCurrent;
				
					if (menuCurrent >= scrollPosition) {
						LCDUIpaintMenuPartial(menuCurrent, menuCurrent + 1);
					} else {
						--scrollPosition;
						scrollY -= itemHeight;
						LCDUIpaintMenu();
					}
					} else {
					scrollY = 0;
					LCDUIpaintMenu();
					}
					}
				break;
			case DOWN:
				if (menuCurrent < menuLength - offMenuCommands - 1 ||
					(scrollY + menuYOffset + 6 + boldHeight + itemSpace) < menuHeight) {
				  
					if (menuCurrent < menuLength - offMenuCommands - 1) {
						int bottomMenuCurrent;
					
					++menuCurrent;
					bottomMenuCurrent = (menuCurrent - scrollPosition + 1) *
						itemHeight;
					
					if (bottomMenuCurrent <= itemSpace) {
						LCDUIpaintMenuPartial(menuCurrent-1, menuCurrent);
					} else {			
						++ scrollPosition;
						scrollY += itemHeight;
						LCDUIpaintMenu();
					}
					} else {
					LCDUIpaintMenu();
					}
					}
				break;
			case FIRE:
			//case KEY_CLEARJ:
				menuEventCommand(menu,menuCurrent);
				//inMenu = FALSE;
				break;
		}
	}
	return inMenu;
}

#define PRESS_OUT_OF_BOUNDS (-1)
#define PRESS_ON_TITLE (-2)

static int itemIndexWhenPressed = PRESS_OUT_OF_BOUNDS; 

static boolean containsPoint(int x, int y) {
    return (x >= menuXOffset && x <= screenWidth &&
            y >= menuYOffset && y <= screenHeight);
}

static int itemIndexAtPointerPosition(int x, int y) {
    int ret;
    int item_topoffset = menuYOffset + 1 + 3 + boldHeight + 3;
    
    if (!containsPoint(x, y)) {
        ret = PRESS_OUT_OF_BOUNDS; 
    } else if (y < item_topoffset) {
        ret = PRESS_ON_TITLE;
    } else {
        ret = (y - item_topoffset) / itemHeight;
    }
	gcj_TraceOut(0,"itemIndexAtPointerPosition ret=%d",ret);
    return ret;
}


int gcj_StubMenuPointEvent(int type,int x,int y)
{
	commandStruct * menu = menuList + offMenuCommands;
    
    if(type == PRESSED)
    {
        itemIndexWhenPressed = itemIndexAtPointerPosition(x, y);

        if(itemIndexWhenPressed >= 0)
        {
            menuCurrent = itemIndexWhenPressed;
            LCDUIpaintMenu();
        }
    }
    else if(type == RELEASED)
    {
        int itemIndexWhenReleased = itemIndexAtPointerPosition(x, y);

        if(itemIndexWhenReleased == PRESS_OUT_OF_BOUNDS)
        {
            hideMenu();
            return 1;
        }
        
        if (itemIndexWhenReleased == itemIndexWhenPressed) 
        {
            if (itemIndexWhenPressed >= 0) 
            {
                menuCurrent = itemIndexWhenPressed;
                if(menuCurrent < menuLength)
                {
			        menuEventCommand(menu,menuCurrent);
                }
            }
        }
        
        itemIndexWhenPressed = PRESS_OUT_OF_BOUNDS;
    }
    
    return 1;
}

int inMenuKeyHandle(EVENT_HANDLER_T *eventData)
{
	commandStruct * menu = menuList + offMenuCommands;

	if (eventData->java_event_data.keyEventData.keytype == PRESSED)
	{
		switch (eventData->java_event_data.keyEventData.keycode)
		{
			case UP:
				if (menuCurrent > 0 || scrollY > 0) {
					
					if (menuCurrent > 0) {
					--menuCurrent;
				
					if (menuCurrent >= scrollPosition) {
						LCDUIpaintMenuPartial(menuCurrent, menuCurrent + 1);
					} else {
						--scrollPosition;
						scrollY -= itemHeight;
						LCDUIpaintMenu();
					}
					} else {
					scrollY = 0;
					LCDUIpaintMenu();
					}
					}
				break;
			case DOWN:
				if (menuCurrent < menuLength - offMenuCommands - 1 ||
					(scrollY + menuYOffset + 6 + boldHeight + itemSpace) < menuHeight) {
				  
					if (menuCurrent < menuLength - offMenuCommands - 1) {
						int bottomMenuCurrent;
					
					++menuCurrent;
					bottomMenuCurrent = (menuCurrent - scrollPosition + 1) *
						itemHeight;
					
					if (bottomMenuCurrent <= itemSpace) {
						LCDUIpaintMenuPartial(menuCurrent-1, menuCurrent);
					} else {			
						++ scrollPosition;
						scrollY += itemHeight;
						LCDUIpaintMenu();
					}
					} else {
					LCDUIpaintMenu();
					}
					}
				break;
			case FIRE:
			case KEY_CLEARJ:
				menuEventCommand(menu,menuCurrent);
				return 1;
				break;
            default:
                break;
		}
	}
	return 0;
}

void gcj_menu_reset()
{
	if (menuList !=NULL) {
		_Jv_Free(menuList);
	}
	//return_from_native_input = 0;
	//checkforinputwindow=0;
}

