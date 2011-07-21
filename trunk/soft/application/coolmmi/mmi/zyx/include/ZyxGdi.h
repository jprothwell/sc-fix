#ifndef __ZYX_GDI_H__
#define __ZYX_GDI_H__

#include "zyxtypes.h"

typedef enum
{
    ZYX_SMALL_FONT,
    ZYX_MEDIUM_FONT,
    ZYX_LARGE_FONT
}ZYX_FONT;

typedef struct 
{
    U8 bold;
    U8 italic;
    U8 underline;
    ZYX_FONT size;
    U8 color;
    U8 type;
    U8 oblique;
    U8 smallCaps;
}FontAttribute;

enum
{
    POPUP_ERROR,
    POPUP_INFO
};

typedef struct 
{
    S32 left;
    S32 top;
    S32 right;
    S32 bottom;
}ZYX_RECT;

#define CHECK_BOUNDS(x, y, x1, y1, width, height) ((x) >= (x1) && (y) >= (y1) && (x) < ((x1) + (width)) && (y) < ((y1) + (height)))

extern void ZyxDrawPoint( int x, int y, int RGB);
extern void ZyxDrawLine(int x1, int y1, int x2, int y2, int RGB);
extern void ZyxDrawRect(int x, int y, int w, int h,int RGB);
extern void ZyxFillRect(int x, int y, int w, int h, int RGB);

extern void ZyxSetTextFont(FontAttribute *f);
extern void ZyxSetTextColor( int RGB);
extern void ZyxSetTextBorderColor( int RGB);
extern int ZyxGetFontHeight(void);
extern int ZyxGetStringWidth(U16 *str);
extern int ZyxGetCharacterWidth(U16 c);
extern void ZyxDrawStringN(U16* str, int x, int y,int n,BOOL bBorder);
extern void ZyxShowPopMsg(U16 *msg,int type);

#endif
