/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__26____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

////////////////////////////////////////////////////////// Èí¼üÅÌ Ä£¿é //////////////////////////////////////////////////////////

#if __SCREEN_HEIGHT__ > 320
extern void showPNGImage(U16*, U16*, S32, S32, S32, S32, S32, S32, s32, s32, s32, s32);
#else
#include "../cc/javax_microedition_lcdui_Graphics.h"
//extern void gd_draw_rect16(U16 *imgp, int destWidth, int x1, int y1, int x2, int y2, int clipx1, int clipy1, int clipx2, int clipy2, u32 color, int dotted);
extern void javax_microedition_lcdui_Graphics_setColor_alias1(javax_microedition_lcdui_Graphics* java_this,s32 RGB);
extern javax_microedition_lcdui_Graphics* javax_microedition_lcdui_Display_screenGraphics;
extern const s32 JMETOC_SKEY_POS[];
extern s32 JMETOC_SKEY_COUNT;
#endif

#ifdef ____SMARCHBOX__COOLSAND__V1_0_0____
extern void gd_draw_char16(U16 ch,
                    U16 *dstPtr,
                    S16 destWidth,
                    S16 x,
                    S16 y,
                    U16 color,
                    stFontAttribute* font,
                    S16 clipx1,
                    S16 clipy1,
                    S16 clipx2,
                    S16 clipy2);
#include "../cc/javax_microedition_lcdui_Font.h"
#endif

/**
 * »æÖÆÈí¼üÅÌ¡£
 * @author runasea
 * @version 1.0.0.1, 2008-12-20
 */
void jmetocSKey_paint() {
  #if __SCREEN_HEIGHT__ > 320
  u16* key_img = (u16*)JMETOC_SKEY;
  #if __SCREEN_WIDTH__ == 240
    u8* base = ((gdi_layer_struct*)GDI_LAYER_MAIN_BASE_LAYER_HANDLE)->buf_ptr;
    showPNGImage((U16*)key_img, (U16*)base, __SCREEN_WIDTH__, __SCREEN_WIDTH__,
    0, __SCREEN_HEIGHT__ - 80, __SCREEN_WIDTH__, 80, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);
  #else
    u8* base = ((gdi_layer_struct*)GDI_LAYER_MAIN_BASE_LAYER_HANDLE)->buf_ptr;
    //»æÖÆ±ß¿ò
    showPNGImage((u16*)BOX_TOP, (U16*)base, 320, __SCREEN_WIDTH__,
    0, 0, 320, 40, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);
    showPNGImage((u16*)BOX_LEFT, (U16*)base, 40, __SCREEN_WIDTH__,
    0, 40, 40, 400, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);
    showPNGImage((u16*)BOX_RIGHT, (U16*)base, 40, __SCREEN_WIDTH__,
    280, 40, 40, 400, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);
    showPNGImage((u16*)BOX_BOTTOM, (U16*)base, 320, __SCREEN_WIDTH__,
    0, 440, 320, 40, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);

    base += (__SCREEN_WIDTH__ - 240);
    showPNGImage((U16*)key_img, (U16*)base, 240, __SCREEN_WIDTH__,
    0, 360, 240, 80, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);
  #endif
  #else
  s32 i, idx, top, x1, x2, y1, y2;
  if (javax_microedition_lcdui_Display_screenGraphics != null) {
    top = __SCREEN_HEIGHT__ - 80;
    for (i = 0; i < JMETOC_SKEY_COUNT; i++) {
      idx = i * 5;
      x1 = JMETOC_SKEY_POS[idx];
      y1 = JMETOC_SKEY_POS[idx + 1];
      x2 = JMETOC_SKEY_POS[idx + 2];
      y2 = JMETOC_SKEY_POS[idx + 3];
      #ifdef ____SMARCHBOX__COOLSAND__V1_0_0____
      switch (i) {
      case 4:
        gd_draw_arc16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__,
        x1, top + y1, x2 - x1, y2 - y1, 0, 360, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_arc16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__,
        x1 + 1, top + y1 + 1, x2 - 2 - x1, y2 - 2 - y1, 0, 360, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        //gd_draw_arc16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        //__SCREEN_WIDTH__,
        //x1 + 2, top + y1 + 2, x2 - 5 - x1, y2 - 5 - y1, 0, 360, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        break;
      case 0:
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, (x1 + x2) >> 1, top + y1, x1, top + y2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, (x1 + x2) >> 1, top + y1 + 2, x1 + 1, top + y2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, (x1 + x2) >> 1, top + y1, x2, top + y2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, (x1 + x2) >> 1, top + y1 + 2, x2 - 1, top + y2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        break;
      case 1:
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, (x1 + x2) >> 1, top + y2, x1, top + y1, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, (x1 + x2) >> 1, top + y2 - 2, x1 + 1, top + y1, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, (x1 + x2) >> 1, top + y2, x2, top + y1, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, (x1 + x2) >> 1, top + y2 - 2, x2 - 1, top + y1, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        break;
      case 2:
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, x1, top + ((y2 + y1) >> 1), x2, top + y1, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, x1 + 1, top + ((y2 + y1) >> 1), x2, top + y1 + 2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, x1, top + ((y2 + y1) >> 1), x2, top + y2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, x1 + 1, top + ((y2 + y1) >> 1), x2, top + y2 - 2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        break;
      case 3:
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, x2, top + ((y2 + y1) >> 1), x1, top + y1, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, x2 - 1, top + ((y2 + y1) >> 1), x1, top + y1 + 2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, x2, top + ((y2 + y1) >> 1), x1, top + y2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0, false);
        gd_draw_line16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
        __SCREEN_WIDTH__, x2 - 1, top + ((y2 + y1) >> 1), x1, top + y2 - 2, 0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60, false);
        break;
      default:
      #endif
      gd_draw_rect16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
      __SCREEN_WIDTH__,
      x1,top + y1,x2 - 1,top + y2 - 1,
      0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0x10a0,false);
      gd_draw_rect16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
      __SCREEN_WIDTH__,
      x1 + 1,top + y1 + 1,x2 - 2,top + y2 - 2,
      0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60,false);
      #ifndef ____SMARCHBOX__COOLSAND__V1_0_0____
      gd_draw_rect16((U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
      __SCREEN_WIDTH__,
      x1 + 2,top + y1 + 2,x2 - 3,top + y2 - 3,
      0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__, 0xffef60,false);
      #else
        if (i == 5) {
          gd_draw_char16('L', (U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
          __SCREEN_WIDTH__, x1 + 14, top + y1 + 3, 0xffef60,
          (stFontAttribute*)(&javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_currentFont->javax_microedition_lcdui_Font_nativeFont),
          0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);
        } else {
          gd_draw_char16('R', (U16*)javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_buf,
          __SCREEN_WIDTH__, x1 + 14, top + y1 + 3, 0xffef60,
          (stFontAttribute*)(&javax_microedition_lcdui_Display_screenGraphics->javax_microedition_lcdui_Graphics_currentFont->javax_microedition_lcdui_Font_nativeFont),
          0, 0, __SCREEN_WIDTH__, __SCREEN_HEIGHT__);
        }
      }
      #endif
    }
  }
  #endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
