#include "internal.h"


//extern stFontAttribute MMI_small_font;
//extern U16 gCurrLangIndex;
#include "fontres.h"
void gcj_StubFontInit(int inp_face,int inp_style,int inp_size,int *ascent, int*descent, int*leading)
{
	S32 height;
	//height = Get_FontHeight(MMI_small_font, (U8) gCurrLangIndex);
	height = Get_CharHeightOfAllLang(LARGE_FONT);
	if (height <14)
		height = 14; //chinese font height
	*leading = 1;
	*descent = 1;
	*ascent = height-2;
}

int gcj_StubFontCharWidth(unsigned short unicode)
{
	S32 width=0;
	Get_CharWidth(unicode,&width);
	if (unicode == L'*')
		width  -= 2;
	return width;
}

int gcj_StubMaxCharWidth()
{
	S32 width=0;
	Get_CharWidth(0x83DC,&width);
	return width;
}


static void gcj_pub_char(
		U16 * buff,
                S32 x,
                S32 y,
                gdi_color color_t,
                U8 *font_data,
                U32 font_data_size,
                U16 char_width,
                U16 char_height,
                U16 clipx1,
                U16 clipy1,
                U16 clipx2,
                U16 clipy2)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 X1, Y1;
    S32 X2 = 0, Y2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    X1 = x;
    Y1 = y;
    X2 = 0;
    while (font_data_size--)
    {
        U8 pattern = *font_data++;

        if (!pattern)
        {
            U32 nTemp;

            X2 += 8;
            nTemp = X2 / char_width;
            if (nTemp)
            {
                Y1 += nTemp;
                char_height -= (U16) nTemp;
                if (!char_height)
                {
                    return;
                }
            }
            X2 %= char_width;
            X1 = x + X2;
        }
        else
        {
		for(Y2 = 0; Y2 < 8; Y2++)                                               
		{                                                                       
			if(pattern&1)                                                        
			{
				//SET_PIXEL(X1, Y1, color_t); 
				if (X1>=clipx1 && Y1>=clipy1 && X1<=clipx2 && Y2<= clipy2)
					buff[Y1*LCD_WIDTH+X1] = (U16)color_t;
			}
			++X2;                                                                
			if(X2 == (S32)( char_width ) )                                       
			{                                                                    
				X1=x;                                                             
				char_height--;                                                    
				++Y1;                                                             
				                                                   
				if(char_height == 0)                                              
					return ;                                                       
				X2=0;                                                             
			}                                                                    
			else                                                                 
			{                                                                    
				++X1;                                                             
			}                                                                    
			pattern >>=1;                                                        
		}                                                                       
           }
    	}
}
void gcj_StubGraphicsDrawChar(unsigned short *distBuff,unsigned short unicode,unsigned int x,unsigned int y,unsigned int rgbcolor,unsigned int clipx1,unsigned int clipy1,unsigned int clipx2,unsigned int clipy2)
{
	UINT32 NumChar=0;
	stFontAttribute Font;
	U8 *CharData;
	U16 nWidth,nHgt;
	U16 pixel = gcj_StubGraphicsGetPixel(rgbcolor,0,0);
	Font.bold = 0;
	Font.italic = 0;
	Font.underline = 0;
	Font.size = 0;		// (size 8..15 FOR type 2..5 ) & (size 9...15 FOR type 1 )
	Font.type = 0;		// 1...5
	
	NumChar = GetFontdata(unicode, Font, &CharData,&nWidth,&nHgt);
	//gcj_TraceOut(0,"gcj_StubGraphicsDrawChar rgbcolor=0x%x, unicode=%d,CharData=0x%x,nWidth=%d,nHgt=%d,NumChar=%d",rgbcolor,unicode,CharData,nWidth,nHgt,NumChar);
	if (unicode == L'*')
		x  -= 1;
	gcj_pub_char(distBuff,x,y,pixel,CharData,NumChar,nWidth,nHgt,clipx1,clipy1,clipx2,clipy2);
	//set_lcd_frame_buffer_address(distBuff);
	//gcj_StubDisplayRefresh(0,0,176,220);
}

