#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "zyxgdi.h"
#include "zyxStd.h"

static void ZyxGetRgb(int RGB,U8 *alpha,U8 *r,U8 *g,U8* b)
{
    *alpha = (RGB&0xff000000) >> 24;
    *r = (RGB&0x00ff0000) >> 16;
    *g = (RGB&0x0000ff00) >> 8;
    *b = RGB&0x000000ff;
}

void ZyxDrawPoint( int x, int y, int RGB)
{
    color_t tmp;

    ZyxGetRgb(RGB,&tmp.alpha,&tmp.r,&tmp.g,&tmp.b);
    gui_putpixel(x,y,tmp);
}

void ZyxDrawLine(int x1, int y1, int x2, int y2, int RGB)
{
    color_t tmp;

    ZyxGetRgb(RGB,&tmp.alpha,&tmp.r,&tmp.g,&tmp.b);
    gui_line(x1,y1,x2,y2,tmp);
}

void ZyxDrawRect(int x, int y, int w, int h,int RGB)
{
    U8 alpha,r,g,b;

    ZyxGetRgb(RGB,&alpha,&r,&g,&b);
    RGB = gdi_act_color_from_rgb(alpha,r,g,b);
    gdi_draw_rect(x, y, x+w, y+h,RGB);
}

void ZyxFillRect(int x, int y, int w, int h, int RGB)
{
    U8 alpha,r,g,b;

    ZyxGetRgb(RGB,&alpha,&r,&g,&b);
    RGB = gdi_act_color_from_rgb(alpha,r,g,b);
    gdi_draw_solid_rect(x, y, x+w, y+h, RGB);
}

extern U16 	gCurrLangIndex;
void ZyxSetTextFont(FontAttribute *f)
{
	stFontAttribute Font ;
    
	Font.bold = f->bold;
	Font.italic = f->italic;
	Font.underline = f->underline;
	Font.size  = f->size;
	Font.color_t = f->color;
	Font.oblique = f->oblique;
	Font.smallCaps = f->smallCaps;
 
      SetFont(Font, (U8)gCurrLangIndex);
}

void ZyxSetTextColor( int RGB)
{
    color_t tmp;

    ZyxGetRgb(RGB,&tmp.alpha,&tmp.r,&tmp.g,&tmp.b);
    gui_set_text_color(tmp);
}

void ZyxSetTextBorderColor( int RGB)
{
    color_t tmp;

    ZyxGetRgb(RGB,&tmp.alpha,&tmp.r,&tmp.g,&tmp.b);
    gui_set_text_border_color(tmp);
}

int ZyxGetFontHeight(void)
{
     return  gui_get_character_height();
}

int ZyxGetStringWidth(U16 *str)
{
    return gui_get_string_width((UI_string_type )str);
}

int ZyxGetCharacterWidth(U16 c)
{
    U8 buf[4]={0,0,0,0};
    S32 width,height;
    
    buf[0]=(U8)((c)&0xff);
    buf[1]=(U8)((c>>8) & 0xff);

    gui_measure_string_n((UI_string_type)buf,1,&width,&height);
    return width;
}

void ZyxDrawStringN(U16* str, int x, int y,int n,BOOL bBorder)
{
	U16 strTmp[40] = {0};

	n = MIN(39,n);
	Zyx_memcpy(strTmp,str,n*2);
	strTmp[n] = 0;
	
    gui_move_text_cursor(x, y);

    if (bBorder)
        gui_print_bordered_text_n((UI_string_type)strTmp,n);
    else
        gui_print_text_n((UI_string_type)strTmp,n);
}

void ZyxShowPopMsg(U16 *msg,int type)
{
    if (type == POPUP_ERROR)
        DisplayPopup((U8*)msg, IMG_GLOBAL_ERROR, 1, 2000, (U8) ERROR_TONE);
    else
        DisplayPopup((U8*)msg, IMG_GLOBAL_INFO, 1, 2000, (U8) WARNING_TONE);
}

#endif
