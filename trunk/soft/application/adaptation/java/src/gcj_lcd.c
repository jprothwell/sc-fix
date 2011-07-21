#include "stddef.h"
#include "stdarg.h"
#include "cos.h"
#include "cswtype.h"
#include "fs_type.h"
#include "tm.h"
#include "mmi_trace.h"
#include "sxr_mem.h"
#include "lcd_sw_inc.h"
#include "csw_lcd.h"
#include "mmi_png_dec.h"
#include "mmi_jpeg_dec.h"
#include "gui_data_types.h"
#include "gdi_datatype.h"
#include "gdi_const.h"
#include "gdi_include.h"
#include "internal.h"

int inFullScreenMode = KNI_FALSE;
extern unsigned char	inMenu;
extern unsigned int FONT_COLOR;

unsigned short *gcj_StubGetScreenBuffer()
{
	unsigned char *buf;
	gdi_layer_push_and_set_active(java_baseLayer);
	gdi_layer_get_buffer_ptr(&buf);
	gdi_layer_pop_and_restore_active();
	//gcj_TraceOut(0,"gcj_StubGetScreenBuffer buff = 0x%x current taskid =%d\n",buf,sxr_GetCurrentTaskId());
	if (buf == NULL)
		abort();
	return buf;
}

unsigned short *gcj_StubGetSecondBuffer()
{
	unsigned char *buf;
	while (!java_secondLayer)
		gcj_StubThreadSleep(100);
	gdi_layer_push_and_set_active(java_secondLayer);
	gdi_layer_get_buffer_ptr(&buf);
	gdi_layer_pop_and_restore_active();
	//gcj_TraceOut(0,"gcj_StubGetSecondBuffer buff = 0x%x\n",buf);
	if (buf == NULL)
		abort();
	return buf;
}
extern int AvoidFlashingWhiteFlag;
void gcj_StubDisplayRefresh(unsigned short *dispBuff,int x1, int y1, int x2, int y2)
{
    if(AvoidFlashingWhiteFlag == 1)
        return;

    if(gcj_IsJavaPause()) return;
	
	if (x1 <0 )
		x1=0;
	if (y1 < 0)
		y1 = 0;
	if (x2 >= LCD_WIDTH)
		x2 = LCD_WIDTH-1;
	if (y2 >= LCD_HEIGHT)
		y2 = LCD_HEIGHT -1;
	//set_lcd_frame_buffer_address(dispBuff);
	//gcj_StubDisplayRefresh(0,0,176,220);
	if (!inFullScreenMode)
	{
		x1=0;
		x2 = LCD_WIDTH-1;
		y1=0;
		y2=LCD_HEIGHT-1;
		gcj_DrawCommandBar(dispBuff);
	}
	if (set_InputDone)
	{
		gcj_DrawInputSign(dispBuff);
	}
    	mci_LcdBlockWrite((unsigned short*)dispBuff,x1,y1,x2,y2);
}

 void gcj_StubForceRefreshDisplay()
 {
	 unsigned short *dispBuf = gcj_StubGetScreenBuffer();
	 gcj_StubDisplayRefresh(dispBuf,0,0,LCD_WIDTH,LCD_HEIGHT);
 }

void gcj_DrawInputSign(unsigned char *buff)
{
	int i=0;
	unsigned int startX,startY;
	unsigned int  charWidth=0;
	gdi_color color_t;
	int ascent,descent,leading;
	unicode tmpbuff[10];
	AnsiiToUnicodeString(tmpbuff,"System");
	gcj_StubFontInit(0,0,0,&ascent,&descent,&leading);
	int height=ascent + descent-3;
	int numChar = UCS2Strlen(input_mode_sign);
	int maxWidth = LCDUIcharsWidth(tmpbuff,6);
	startX = LCD_WIDTH-LCDUIcharsWidth(input_mode_sign,numChar)-2;
	startY = 3;
	color_t = gcj_StubGraphicsGetPixel(LIGHT_GRAY_COLOR,0,0);
	gd_fill_rect16(buff,LCD_WIDTH,LCD_WIDTH-maxWidth-1,startY,LCD_WIDTH,startY+height,LCD_WIDTH-maxWidth-1,startY,LCD_WIDTH-1,LCD_HEIGHT-1,color_t);
	for (i=0;i<numChar;i++)
	{
		gcj_StubGraphicsDrawChar(buff,input_mode_sign[i],startX+charWidth,startY,FONT_COLOR,0,0,LCD_WIDTH,LCD_HEIGHT);//0x7fd2ed);
		charWidth += gcj_StubFontCharWidth(input_mode_sign[i]);
	}
}
void gcj_DrawCommandBar(unsigned char *buff)
{
	UINT32 charWidths,charHeights;
	int ascent,descent,leading;
	gcj_StubFontInit(0,0,0,&ascent,&descent,&leading);
	charHeights = ascent + descent +leading;
	int commandBarHeight = gcj_StubGetButtonHeight();
	int i;
	gdi_color color_t;
	color_t =gcj_StubGraphicsGetPixel(DARK_GRAY_COLOR,0,0);
	gd_draw_line16(buff,LCD_WIDTH,0,LCD_HEIGHT-commandBarHeight,LCD_WIDTH,LCD_HEIGHT-commandBarHeight,0,LCD_HEIGHT-commandBarHeight,LCD_WIDTH,LCD_HEIGHT-commandBarHeight,color_t,0);
	color_t = gcj_StubGraphicsGetPixel(0xC3D4F7,0,0);
	gd_draw_line16(buff,LCD_WIDTH,0,LCD_HEIGHT-commandBarHeight+1,LCD_WIDTH,LCD_HEIGHT-commandBarHeight+1,0,LCD_HEIGHT-commandBarHeight+1,LCD_WIDTH,LCD_HEIGHT-commandBarHeight+1,color_t,0);
	color_t = gcj_StubGraphicsGetPixel(LIGHT_GRAY_COLOR,0,0);
	gd_fill_rect16(buff,LCD_WIDTH,0,LCD_HEIGHT-commandBarHeight+1+1,LCD_WIDTH,LCD_HEIGHT-2,0,LCD_HEIGHT-commandBarHeight+1+1,LCD_WIDTH-1,LCD_HEIGHT-1,color_t);
	color_t = gcj_StubGraphicsGetPixel(DARK_GRAY_COLOR,0,0);
	gd_draw_line16(buff,LCD_WIDTH,0,LCD_HEIGHT-1,LCD_WIDTH,LCD_HEIGHT-1,0,LCD_HEIGHT-commandBarHeight,LCD_WIDTH,LCD_HEIGHT-1,color_t,0);
	//color_t = gcj_StubGraphicsGetPixel(LIGHT_GRAY_COLOR,0,0);
	//gd_draw_line16(buff,LCD_WIDTH,0,LCD_HEIGHT-19,LCD_WIDTH-1,LCD_HEIGHT-1,0,LCD_HEIGHT-19,LCD_WIDTH-1,LCD_HEIGHT-1,color_t,0);
	if (leftButton.label != NULL)
	{	unsigned int startX,startY;
		unsigned int  charWidth=0;
		charWidths = LCDUIcharsWidth(leftButton.label,leftButton.numChars);
		startX = 1;
		startY = LCD_HEIGHT-charHeights -3;
		//gd_fill_rect16(buff,LCD_WIDTH,startX,startY-leading,startX+charWidths-1,LCD_HEIGHT-1,0,0,LCD_WIDTH-1,LCD_HEIGHT-1,color);
		//Get_StringWidthHeight(label,&charWidths,&charHeights);
		for (i=0;i<leftButton.numChars;i++)
		{
			gcj_StubGraphicsDrawChar(buff,leftButton.label[i],startX+charWidth,startY,FONT_COLOR,0,0,LCD_WIDTH,LCD_HEIGHT);//0x7fd2ed);
			charWidth += gcj_StubFontCharWidth(leftButton.label[i]);
			//gcj_TraceOut(0,"gcj_DrawCommandBar leftButton %d  charWidth=%d,charHeights=%d,numChars=%d,unicode=%d",i,charWidth,charHeights,leftButton.numChars,leftButton.label[i]);
		}
	}
	if (rightButton.label != NULL)
	{	unsigned int startX,startY;
		unsigned int  charWidth=0;
		charWidths = LCDUIcharsWidth(rightButton.label,rightButton.numChars);
		startX = LCD_WIDTH-charWidths-1;
		startY = LCD_HEIGHT-charHeights -3;
		//gd_fill_rect16(buff,LCD_WIDTH,startX,startY-leading,startX+charWidths-1,LCD_HEIGHT-1,0,0,LCD_WIDTH-1,LCD_HEIGHT-1,color);
		//Get_StringWidthHeight(label,&charWidths,&charHeights);
		for (i=0;i<rightButton.numChars;i++)
		{
			gcj_StubGraphicsDrawChar(buff,rightButton.label[i],startX+charWidth,startY,FONT_COLOR,0,0,LCD_WIDTH,LCD_HEIGHT);
			charWidth += gcj_StubFontCharWidth(rightButton.label[i]);
			//gcj_TraceOut(0,"gcj_DrawCommandBar rightButton %d  charWidth=%d,charHeights=%d,numChars=%d,unicode=%d",i,charWidth,charHeights,rightButton.numChars,rightButton.label[i]);
		}
	}
	if (set_InputDone && centerButton.label !=NULL && !inMenu)// && !gcj_IsHavaDelKey())
	{	unsigned int startX,startY;
		unsigned int  charWidth=0;
		charWidths = LCDUIcharsWidth(centerButton.label,centerButton.numChars);
		startX = (LCD_WIDTH-charWidths-1)/2;
		startY = LCD_HEIGHT-charHeights -3;
		//gd_fill_rect16(buff,LCD_WIDTH,startX,startY-leading,startX+charWidths-1,LCD_HEIGHT-1,0,0,LCD_WIDTH-1,LCD_HEIGHT-1,color);
		//Get_StringWidthHeight(label,&charWidths,&charHeights);
		for (i=0;i<centerButton.numChars;i++)
		{
			gcj_StubGraphicsDrawChar(buff,centerButton.label[i],startX+charWidth,startY,FONT_COLOR,0,0,LCD_WIDTH,LCD_HEIGHT);
			charWidth += gcj_StubFontCharWidth(centerButton.label[i]);
			//gcj_TraceOut(0,"gcj_DrawCommandBar rightButton %d  charWidth=%d,charHeights=%d,numChars=%d,unicode=%d",i,charWidth,charHeights,rightButton.numChars,rightButton.label[i]);
		}
	}
}


void 
LCDUIfillRect(int pixel, short *clip, void* dst, int dotted, 
	      int x, int y, int width, int height)
{
    unsigned short *buff = gcj_StubGetScreenBuffer();
    if (buff==NULL)
		abort();
    gdi_color color = gcj_StubGraphicsGetPixel(pixel,0,0);
    gd_fill_rect16(buff,LCD_WIDTH,x,y,x+width-1,y+height-1,clip[0],clip[1],clip[0]+clip[2],clip[1]+clip[3],color);
}
/*
 * Fill a triangle defined by the given three points (x1,y1) (x2,y2)
 * and (x3,y3) with the current color.
 */
void 
LCDUIfillTriangle(int pixel, short *clip, void* dst, int dotted, 
		  int x1, int y1, int x2, int y2, int x3, int y3)
{
    unsigned short *buff = gcj_StubGetScreenBuffer();
    if (buff==NULL)
		abort();
    gdi_color color = gcj_StubGraphicsGetPixel(pixel,0,0);
    gd_fill_triangle16(buff,LCD_WIDTH,x1,y1,x2,y2,x3,y3,clip[0],clip[1],clip[0]+clip[2],clip[1]+clip[3],color,dotted);
}

/*
 * Draw a line between two points (x1,y1) and (x2,y2).
 */
void 
LCDUIdrawLine(int pixel, short *clip, void* dst, int dotted, 
	      int x1, int y1, int x2, int y2)
{
    unsigned short *buff = gcj_StubGetScreenBuffer();
    if (buff==NULL)
		abort();
    gdi_color color = gcj_StubGraphicsGetPixel(pixel,0,0);
    gd_draw_line16(buff,LCD_WIDTH,x1,y1,x2,y2,clip[0],clip[1],clip[0]+clip[2],clip[1]+clip[3],color,dotted);
}
void
LCDUIdrawRegionTransform(short *clip, void *dst, void *img,
                         int x_dest, int y_dest, int anchor,
                         int x_src, int y_src,
                         int srcWidth, int srcHeight, 
                         int transform, int isRegion) {
    unsigned short *buff = gcj_StubGetScreenBuffer();
    if (buff==NULL)
		abort();
	img_handle_t img_t;
	img_t.imgData = img;
	img_t.imgHeight = srcHeight;
	img_t.imgWidth = srcWidth;
	img_t.isTransColor =0;
	gcj_StubGraphicsDrawRegion(buff,&img_t,x_src,y_src,srcWidth,srcHeight,transform,x_dest,y_dest,LCD_WIDTH,LCD_HEIGHT);
						 
}
int LCDUIcharsWidth(unicode *charArray, int n)
{
	int i=0;
	int width=0;
	for (i=0;i<n;i++)
		width += gcj_StubFontCharWidth(charArray[i]);
	return width;
}

void 
LCDUIdrawChars(int pixel, short *clip, void* dst, int dotted, 
	       int x, int y, int anchor, const unicode *chararray, int n)
{
 //   gcj_TraceOut(0,"LCDUIdrawChars x=%d,y=%d,chararray=0x%x,n=%d",x,y,chararray,n);
    unsigned short *buff = gcj_StubGetScreenBuffer();
    if (buff==NULL)
		abort();
	int j;
	int tmp=0;
	for (j=0;j<n;j++)
	{
		gcj_StubGraphicsDrawChar(buff,chararray[j],x+tmp,y,pixel,0,0,LCD_WIDTH,LCD_HEIGHT);
	//	gcj_TraceOut(0,"LCDUIdrawChars numChars=%d,%ds,x=%d,unicode=%d",n,j,tmp+x,chararray[j]);
		tmp+=gcj_StubFontCharWidth(chararray[j]);
	}
}


void gcj_StubGetLcdCaps(unsigned int *width,unsigned int *height)
{
	*width = LCD_WIDTH;
	*height = LCD_HEIGHT;
}

int gcj_StubGetButtonHeight()
{
	int ascent,descent,leading;
	gcj_StubFontInit(0,0,0,&ascent,&descent,&leading);
	return ascent+descent+leading+5;
}

