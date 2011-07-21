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
 *	 gdi_primitive.c
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI Primitive related.
 * Author:
 * -------
 * -------
 *==============================================================================
 * 				HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#include "gdi_internal.h"
#include "gd_primitive.h"
/****************************************************************************
* Global variable 
*****************************************************************************/

jmp_buf						gdi_jmpbuf;
BOOL						gdi_jmpbuf_is_used=FALSE;

/****************************************************************************
* MACRO
*****************************************************************************/
#define SWAP(t,a,b) do {t tmp=a;a=b;b=tmp;} while(0)

/* check if pixel within clip, if not will return */
#define	GDI_CLIP_POINT_TEST(x, y)\
{\
	if( (x < GDI_LAYER.clipx1) || (x > GDI_LAYER.clipx2) ||\
		(y < GDI_LAYER.clipy1) || (y > GDI_LAYER.clipy2) )\
	{\
		GDI_RETURN_VOID;	\
	}\
}

#define	GDI_CLIP_TEXT_POINT_TEST(x, y)\
{\
	if( (x < GDI_LAYER.act_text_clip.x1) || (x > GDI_LAYER.act_text_clip.x2) ||\
		(y < GDI_LAYER.act_text_clip.y1) || (y > GDI_LAYER.act_text_clip.y2) )\
	{\
		GDI_RETURN_VOID;	\
	}\
}


#define GDI_CLIP_LINE_X_TEST(X1, X2)\
{\
	if(X1 < GDI_LAYER.clipx1)\
		X1 = GDI_LAYER.clipx1;\
	if(X2 > GDI_LAYER.clipx2)\
		X2 = GDI_LAYER.clipx2;\
}

#define GDI_CLIP_LINE_Y_TEST(Y1, Y2)\
{\
	if(Y1 < GDI_LAYER.clipy1)\
		Y1 = GDI_LAYER.clipy1;\
	if(Y2 > GDI_LAYER.clipy2)\
		Y2 = GDI_LAYER.clipy2;\
}

#define GDI_CLIP_LINE_XY_TEST(x1,y1,x2,y2)\
{\
	if(x1 < GDI_LAYER.clipx1)\
		x1 = GDI_LAYER.clipx1;\
	if(x2 > GDI_LAYER.clipx2)\
		x2 = GDI_LAYER.clipx2;\
	if(y1 < GDI_LAYER.clipy1)\
		y1 = GDI_LAYER.clipy1;\
	if(y2 > GDI_LAYER.clipy2)\
		y2 = GDI_LAYER.clipy2;\
}


/* draw horizontal line */
__inline void GDI_DRAW_H_LINE(S32 x1, S32 x2, S32 y, gdi_color pixel_color)
{
	GDI_ENTER_CRITICAL_SECTION(GDI_DRAW_H_LINE)
		S32	h_index;
		for(h_index = x1; h_index <= x2; h_index++)
		{
			GDI_SET_BUFFER_PIXEL(h_index, y, pixel_color);
		}
	GDI_EXIT_CRITICAL_SECTION(GDI_DRAW_H_LINE)
}

__inline void GDI_DRAW_H_LINE_STYLE(S32 x1, S32 x2, S32 y, gdi_color pixel_color, U8 cycle, const U8 *bitvalues)
{
	GDI_ENTER_CRITICAL_SECTION(GDI_DRAW_H_LINE_STYLE)
		S32		h_index;
		kal_uint8 bitcount;
		for(h_index = x1, bitcount=0; h_index <= x2; h_index++, bitcount++, bitcount %= cycle)
		{
			if (bitvalues[bitcount])
			{
				GDI_SET_BUFFER_PIXEL(h_index, y, pixel_color);
			}
		}
	GDI_EXIT_CRITICAL_SECTION(GDI_DRAW_H_LINE_STYLE)
}

/* draw vertical line */
__inline void GDI_DRAW_V_LINE(S32 y1, S32 y2, S32 x, gdi_color pixel_color)
{
	GDI_ENTER_CRITICAL_SECTION(GDI_DRAW_V_LINE)
		S32	v_index;

		for(v_index = y1; v_index <= y2; v_index++)
		{
			GDI_SET_BUFFER_PIXEL(x,v_index,pixel_color);
		}
	GDI_EXIT_CRITICAL_SECTION(GDI_DRAW_V_LINE)
}

__inline void GDI_DRAW_V_LINE_STYLE(S32 y1, S32 y2, S32 x, gdi_color pixel_color, kal_uint8 cycle, const kal_uint8 *bitvalues)
{
	GDI_ENTER_CRITICAL_SECTION(GDI_DRAW_V_LINE_STYLE)
		S32		v_index;
		U8		bitcount;
		for(v_index = y1, bitcount=0; v_index <= y2; v_index++, bitcount++, bitcount %= cycle)
		{
			if (bitvalues[bitcount])
			{
				GDI_SET_BUFFER_PIXEL(x, v_index, pixel_color);
			}
		}
	GDI_EXIT_CRITICAL_SECTION(GDI_DRAW_V_LINE_STYLE)
}

static __inline void GDI_DRAW_RECT(S32 x1, S32 y1, S32 x2, S32 y2, gdi_color pixel_color)
{
	S32	r_index;
	for(r_index = y1; r_index <=y2; r_index++)
	{
		GDI_DRAW_H_LINE(x1, x2, r_index, pixel_color);
	}
}

static __inline void GDI_DRAW_RECT16(S32 x1, S32 y1, S32 x2, S32 y2, gdi_color pixel_color)
{
	U8* dest = GDI_LAYER.buf_ptr + ((x1+y1*GDI_LAYER.width)<<1);

	y2 -=y1; 
	y2++;

	x2-=x1;
	x2++;
	x2<<=1;
	x1 = GDI_LAYER.width<<1;
	
	while(y2--)
	{
		gdi_memset16(dest,(U16)pixel_color,x2);
		dest +=x1;
	}
}

#define GDI_COLOR_INTERPOLATE(COLOR0, COLOR1, ORIG_RATIO, RESULT) \
do {\
	U32 a0,r0,g0,b0;\
	U32 a1,r1,g1,b1;\
	U32 RATIO = (U32) ORIG_RATIO;\
	U32 _100_RATIO = 100 - RATIO;\
	gdi_act_color_to_rgb(&a0,&r0,&g0,&b0,COLOR0);\
	gdi_act_color_to_rgb(&a1,&r1,&g1,&b1,COLOR1);\
	RESULT = gdi_act_color_from_rgb(\
					(a0*RATIO+a1 * _100_RATIO)/100,\
					(r0*RATIO+r1 * _100_RATIO)/100,\
					(g0*RATIO+g1 * _100_RATIO)/100,\
					(b0*RATIO+b1 * _100_RATIO)/100);\
}while(0)

/**************************************************************

	FUNCTION NAME		: gdi_draw_point()

  	PURPOSE				: draw a point

	INPUT PARAMETERS	: S32			x
							  S32			y
							  gdi_color pixel_color

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_point(S32 x, S32 y, gdi_color pixel_color)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_point)
		GDI_CLIP_POINT_TEST(x,y);
		GDI_SET_BUFFER_PIXEL(x,y,pixel_color);
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_point)
}

/**************************************************************

	FUNCTION NAME		: gdi_draw_line_ext()

  	PURPOSE				: draw a line

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color line_color
							  is_dotted		is dotted line or not

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/

void gdi_draw_line_ext(S32 x1, S32 y1, S32 x2, S32 y2, gdi_color line_color,BOOL is_dotted)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_line)

		if(!is_dotted && y1 - y2 == 0) /* Horizontal Line */
		{
			/* within valid y clip */
			if((y1 >= GDI_LAYER.clipy1) && (y1 <= GDI_LAYER.clipy2))
			{
				if(x1 < x2)
				{
					GDI_CLIP_LINE_X_TEST(x1,x2);
					GDI_DRAW_H_LINE(x1, x2, y1, line_color);
				}
				else
				{
					GDI_CLIP_LINE_X_TEST(x2,x1);
					GDI_DRAW_H_LINE(x2, x1, y1, line_color);
				}
			}
		}
		else if(!is_dotted && x1 - x2 == 0) /* Vertical Line */
		{
			/* within valid x clip */
			if((x1 >= GDI_LAYER.clipx1) && (x1 <= GDI_LAYER.clipx2))
			{
				if(y1 < y2)
				{
					GDI_CLIP_LINE_Y_TEST(y1,y2);
					GDI_DRAW_V_LINE(y1, y2, x1, line_color);
				}
				else
				{
					GDI_CLIP_LINE_Y_TEST(y2,y1);
					GDI_DRAW_V_LINE(y2, y1, x1, line_color);
				}
			}
		}
#if 0 && defined(GDI_USING_2D_ENGINE) // because hardware is too slow
#endif
		else
		{
			short destWidth = GDI_LAYER.width;
			short clipx1;
			short clipy1;
			short clipx2;
			short clipy2; 
			int rate;
			char xscan;
			U16 *imgp;

			imgp = (U16*)GDI_LAYER.buf_ptr;

			clipx1 = GDI_LAYER.clipx1;
			clipy1 = GDI_LAYER.clipy1;
			clipx2 = GDI_LAYER.clipx2;
			clipy2 = GDI_LAYER.clipy2;


			// only draw one dot 
			if ((x1== x2) && (y1 == y2)) {
				if ((x1 < clipx1) || (x1 > clipx2))
					GDI_RETURN_VOID;
				if ((y1 < clipy1) || (y1 > clipy2))
					GDI_RETURN_VOID;

				imgp[x1 + y1 * destWidth] = line_color;
				GDI_RETURN_VOID;
			}

			
			// exchange two point if needed.
			{
				int dx = x2 - x1;
				int dy = y2 - y1;

				//calculate slope rate
				{
					int absdx;
					int absdy;
					if (dx < 0)
						absdx = -dx;
					else
						absdx = dx;

					if (dy < 0)
						absdy = -dy;
					else
						absdy = dy;

					if (absdx >= absdy) 
					{
						xscan = 1;
						if (x1 > x2)  //exchange
						{
							int temp = x1;
							x1 = x2;
							x2 = temp;
							temp = y1;
							y1 = y2;
							y2 = temp;
						}
						rate = (dy << 20) / dx;
					} 
					else 
					{
						xscan = 0;
						if (y1 > y2) // exchange
						{
							int temp = x1;
							x1 = x2;
							x2 = temp;
							temp = y1;
							y1 = y2;
							y2 = temp;
						}
						rate = (dx << 20) / dy;
					}
				}

				//clipping
				if (xscan) 
				{
					if (y1 <= y2) 
					{
						if (y1 < clipy1) 
						{
							int orgy = y1;
							y1 = clipy1;
							if (y1 > y2)
								GDI_RETURN_VOID;
							x1 = x1 - ((orgy - y1) * dx) / dy;
						}

						if (y2 > clipy2) 
						{
							int orgy = y2;
							y2 = clipy2;
							if (y1 > y2)
								GDI_RETURN_VOID;
							x2 = x2 - ((orgy - y2) * dx) / dy;
						}
					} 
					else 
					{ /* y1 > y2 */
						if (y2 < clipy1) 
						{
							int orgy = y2;
							y2 = clipy1;
							if (y2 > y1)
								GDI_RETURN_VOID;
							x2 = x2 - ((orgy - y2) * dx) / dy;
						}

						if (y1 > clipy2) 
						{
							int orgy = y1;
							y1 = clipy2;
							if (y2 > y1)
								GDI_RETURN_VOID;
							x1 = x1 - ((orgy - y1) * dx) / dy;
						}
					}

					if (x1 < clipx1) 
					{
						int orgx = x1;
						x1 = clipx1;
						if (x1 > x2)
							GDI_RETURN_VOID;
						y1 = y1 - (((orgx - x1) * rate) >> 20);
					}

					if (x2 > clipx2) 
					{
						int orgx = x2;
						x2 = clipx2;
						if (x1 > x2)
							GDI_RETURN_VOID;
						y2 = y2 - (((orgx - x2) * rate) >> 20);
					}

				}
				else// Yscan 
				{ 
					if (x1 <= x2) 
					{
						if (x1 < clipx1) 
						{
							int orgx = x1;
							x1 = clipx1;
							if (x1 > x2)
								GDI_RETURN_VOID;
							y1 = y1 - ((orgx - x1) * dy) / dx;	/* dx will not be zero */
						}

						if (x2 > clipx2) 
						{
							int orgx = x2;
							x2 = clipx2;
							if (x1 > x2)
								GDI_RETURN_VOID;
							y2 = y2 - ((orgx - x2) * dy) / dx;	/* dx will not be zero */
						}
					} 
					else 
					{ // x1 > x2
						if (x2 < clipx1) 
						{
							int orgx = x2;
							x2 = clipx1;
							if (x2 > x1)
								GDI_RETURN_VOID;
							y2 = y2 - ((orgx - x2) * dy) / dx;	/* dx will not be zero */
						}

						if (x1 > clipx2) 
						{
							int orgx = x1;
							x1 = clipx2;
							if (x2 > x1)
								GDI_RETURN_VOID;
							y1 = y1 - ((orgx - x1) * dy) / dx;	/* dx will not be zero */
						}
					}

					if (y1 < clipy1) 
					{
						int orgy = y1;
						y1 = clipy1;
						if (y1 > y2)
							GDI_RETURN_VOID;
						x1 = x1 - (((orgy - y1) * rate) >> 20);
					}

					if (y2 > clipy2) 
					{
						int orgy = y2;
						y2 = clipy2;
						if (y1 > y2)
							GDI_RETURN_VOID;
						x2 = x2 - (((orgy - y2) * rate) >> 20);
					}
				}
			}


			if (!is_dotted) 
			{
				if (xscan == 0) 
				{
					int tempWidth = destWidth;
					int tempx = 0;
					
					unsigned short * dstImageReg = &imgp[x1];
					unsigned short * endAddrReg = dstImageReg + y2 * tempWidth;
					dstImageReg += y1 * tempWidth;
					do {
						dstImageReg[tempx >> 20] = line_color;
						dstImageReg += tempWidth;
						tempx += rate;
					}while (dstImageReg < endAddrReg);
				}
				else
				{
					int tempWidth = destWidth;
					int temprate = 0;

					unsigned short * dstImageReg = &imgp[y1 * tempWidth];
					unsigned short * endAddrReg = dstImageReg + x2;
					dstImageReg += x1;
					do 
					{
						dstImageReg[(temprate >> 20) * tempWidth] = line_color;
						dstImageReg ++;
						temprate += rate;
					}while (dstImageReg < endAddrReg);
				}
			}
			else // dotted
			{ 
				if (xscan == 0) 
				{
					int tempWidth = destWidth;
					int tempx = 0;
					unsigned short * dstImageReg = &imgp[x1];
					unsigned short * endAddrReg = dstImageReg + y2 * tempWidth;

					dstImageReg += y1 * tempWidth;
					tempWidth += tempWidth;
					rate += rate;
					do {
						dstImageReg[tempx >> 20] = line_color;
						dstImageReg += tempWidth;
						tempx += rate;
					}while (dstImageReg < endAddrReg);
				} 
				else 
				{
					int tempWidth = destWidth;
					int temprate = 0;
					unsigned short * dstImageReg = &imgp[y1 * tempWidth];
					unsigned short * endAddrReg = dstImageReg + x2;
					dstImageReg += x1;
					rate += rate;
					do 
					{
						dstImageReg[(temprate >> 20) * tempWidth] = line_color;
						dstImageReg += 2;
						temprate += rate;
					}while (dstImageReg < endAddrReg);
				}
			}
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_line)
}

/**************************************************************

	FUNCTION NAME		: gdi_draw_line()

  	PURPOSE				: draw a line

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color line_color

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_line(S32 x1, S32 y1, S32 x2, S32 y2, gdi_color line_color)
{
	gdi_draw_line_ext(x1,y1,x2,y2, line_color,FALSE);
}


/**************************************************************

	FUNCTION NAME		: gdi_draw_line_style()

  	PURPOSE				: draw a style line

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color line_color
							  U8			cycle
							  const U8  *bitvalues
	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_line_style(S32 x1, S32 y1, S32 x2, S32 y2, gdi_color line_color, U8 cycle, const U8 *bitvalues)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_line_style)
		if(y1 - y2 == 0) /* Horizontal Line */
		{
			/* within valid y clip */
			if((y1 >= GDI_LAYER.clipy1) && (y1 <= GDI_LAYER.clipy2))
			{
				if(x1 < x2)
				{
					GDI_CLIP_LINE_X_TEST(x1,x2);
					GDI_DRAW_H_LINE_STYLE(x1, x2, y1, line_color, cycle, bitvalues);
				}
				else
				{
					GDI_CLIP_LINE_X_TEST(x2,x1);
					GDI_DRAW_H_LINE_STYLE(x2, x1, y1, line_color, cycle, bitvalues);
				}
			}
		}
		else if(x1 - x2 == 0) /* Vertical Line */
		{
			/* within valid x clip */
			if((x1 >= GDI_LAYER.clipx1) && (x1 <= GDI_LAYER.clipx2))
			{
				if(y1 < y2)
				{
					GDI_CLIP_LINE_Y_TEST(y1,y2);
					GDI_DRAW_V_LINE_STYLE(y1, y2, x1, line_color, cycle, bitvalues);
				}
				else
				{
					GDI_CLIP_LINE_Y_TEST(y2,y1);
					GDI_DRAW_V_LINE_STYLE(y2, y1, x1, line_color, cycle, bitvalues);
				}
			}
		}
		else
		{
			gdi_draw_line_ext(x1,y1,x2,y2, line_color,TRUE); //FIXME: should support real dotted & cycle
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_line_style)
}



/**************************************************************

	FUNCTION NAME		: gdi_draw_rect()

  	PURPOSE				: draw a rect (border only)

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color frame_color

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_rect(S32 x1, S32 y1, S32 x2, S32 y2, gdi_color frame_color)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_rect)
		S32 nx1,ny1,nx2,ny2;

		nx1 = x1;
		ny1 = y1;
		nx2 = x2;
		ny2 = y2;

		GDI_CLIP_RECT_TEST(nx1,ny1,nx2,ny2,GDI_RETURN_VOID);

		if(GDI_LAYER.clipy1<= y1 && y1 <= GDI_LAYER.clipy2)
			GDI_DRAW_H_LINE(nx1, nx2, y1, frame_color);	/* up horizontal */

		if(GDI_LAYER.clipy1<= y2 && y2 <= GDI_LAYER.clipy2)
			GDI_DRAW_H_LINE(nx1, nx2, y2, frame_color);	/* down horizontal */

		if(GDI_LAYER.clipx1<= x1 && x1 <= GDI_LAYER.clipx2)
			GDI_DRAW_V_LINE(ny1, ny2, x1, frame_color);	/* left vertical */

		if(GDI_LAYER.clipx1<= x2 && x2 <= GDI_LAYER.clipx2)
			GDI_DRAW_V_LINE(ny1, ny2, x2, frame_color);	/* right vertical */
		
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_rect)
}



/**************************************************************

	FUNCTION NAME		: gdi_draw_solid_rect()

  	PURPOSE				: draw a solid rect

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color rect_color

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_solid_rect(S32 x1, S32 y1, S32 x2, S32 y2, gdi_color rect_color)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_solid_rect)
		GDI_CLIP_RECT_TEST(x1,y1,x2,y2,GDI_RETURN_VOID);
		if(GDI_LAYER.bits_per_pixel == 16)
		{
			#if 0 && defined(GDI_USING_2D_ENGINE) // because hardware is too slow
			#else
			GDI_DRAW_RECT16(x1, y1, x2, y2, rect_color);
			#endif
		}
		else
		{
			GDI_DRAW_RECT(x1, y1, x2, y2, rect_color);
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_solid_rect)
}


void gdi_draw_greyscale_rect(S32 x1,S32 y1,S32 x2,S32 y2, S32 white_value,S32 black_value)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_greyscale_rect)	
	S32 x, y;
	gdi_color color_t;
	U32 a,r, g, b; 
	gd_get_pixel_func get_ptr = gd_get_pixel[gdi_act_layer->cf];
	gd_put_pixel_func put_ptr = gd_put_pixel[gdi_act_layer->cf];
	S32 BW256,WB_B;

	GDI_CLIP_RECT_TEST(x1,y1,x2,y2,GDI_RETURN_VOID);
	
#if 0//defined(ISP_SUPPORT) && defined(CSD_TARGET)		
#endif
		{
			if (white_value < 0)
			{
				white_value = 0;
			}
		
			if (white_value > 128)
			{
				white_value++;
			}
		
			if (white_value > 256)
			{
				white_value = 256;
			}
		
			if (black_value < 0)
			{
				black_value = 0;
			}
		
			if (black_value > 128)
			{
				black_value++;
			}
		
			if (black_value > 256)
			{
				black_value = 256;
			}
		
			black_value = 256 - black_value;
		
			BW256 = (white_value*black_value)<<8;
		
			WB_B = black_value*(256 - white_value);
		
			for (y = y1; y <= y2; y++)
			{
				for (x = x1; x <= x2; x++)
				{
					color_t = get_ptr(x,y);
					gdi_act_color_to_rgb(&a,&r,&g,&b,color_t);
					r = (r + g + b)/3;
					//r = r + (256-r) * w / 256
					//r = r * b / 256
					r = (BW256 + r*WB_B) >> 16;
					if (r > 255)
						r = 255;
					put_ptr(x,y,gdi_act_color_from_rgb(a,r,r,r));
				}
			}
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_greyscale_rect)	
}
void gdi_effect_alpha_blending_rect(gdi_handle src_layer_handle,S32 x1,S32 y1,S32 x2,S32 y2, U32 transparent_color_a,U32 transparent_color_r,U32 transparent_color_g,U32 transparent_color_b)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_effect_transparent_rect)	
		S32 x, y;
		gdi_color color_t;
		U32 a, r, g, b;
		U8* src_buf = gdi_act_layer->buf_ptr;
		S32 src_layer_width = gdi_act_layer->width;
		gd_get_buf_pixel_func get_ptr = gd_get_buf_pixel[gdi_act_layer->vcf];
		gd_put_pixel_func put_ptr = gd_put_pixel[gdi_act_layer->vcf];
		S32 opacity_value = transparent_color_a * 256 / 255;
		S32 inverse_opacity_value = 256 - transparent_color_a * 256 / 255;
		S32 offset_x = 0;
		S32 offset_y = 0;

		transparent_color_r *= opacity_value;
		transparent_color_g *= opacity_value;
		transparent_color_b *= opacity_value;

		if ((src_layer_handle != GDI_LAYER_EMPTY_HANDLE) && ((gdi_handle)gdi_act_layer != src_layer_handle))
		{
			GDI_ASSERT(src_layer_handle != GDI_ERROR_HANDLE);
			get_ptr = gd_get_buf_pixel[((gdi_layer_struct*)src_layer_handle)->vcf];
			src_layer_width = ((gdi_layer_struct*)src_layer_handle)->width;
			src_buf = ((gdi_layer_struct*)src_layer_handle)->buf_ptr;
			offset_x = gdi_act_layer->offset_x - ((gdi_layer_struct*)src_layer_handle)->offset_x;
			offset_y = gdi_act_layer->offset_y - ((gdi_layer_struct*)src_layer_handle)->offset_y;
		}

		GDI_CLIP_RECT_TEST(x1,y1,x2,y2,GDI_RETURN_VOID);

		for (y = y1; y <= y2; y++)
		{
			for (x = x1; x <= x2; x++)
			{
				color_t = get_ptr(src_buf,src_layer_width,x + offset_x,y + offset_y);
				gdi_act_color_to_rgb(&a,&r,&g,&b,color_t);
				r = ((transparent_color_r) +  (r * inverse_opacity_value)) >> 8;
				g = ((transparent_color_g) +  (g * inverse_opacity_value)) >> 8;
				b = ((transparent_color_b) +  (b * inverse_opacity_value)) >> 8;
				put_ptr(x,y,gdi_act_color_from_rgb(a,r,g,b));
			}
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_effect_transparent_rect)	
}


void gdi_darw_alpha_blending_screen(gdi_image_cache_bmp_struct *old_bmp_ptr,gdi_image_cache_bmp_struct *new_bmp_ptr,U32 transparent_color_a)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_darw_alpha_blending_screen)	
		S32 x, y;
		gdi_color color_t;
		U32 a, r, g, b;
		U8* src_buf = gdi_act_layer->buf_ptr;
		S32 src_layer_width = gdi_act_layer->width;
		gd_get_buf_pixel_func get_ptr = gd_get_buf_pixel[gdi_act_layer->vcf];
		gd_put_pixel_func put_ptr = gd_put_pixel[gdi_act_layer->vcf];
		S32 opacity_value = transparent_color_a * 256 / 255;
		S32 inverse_opacity_value = 256 - transparent_color_a * 256 / 255;
		U32 new_color_r = 0;
		U32 new_color_g = 0;
 		U32 new_color_b = 0;

		for (y = 0; y < gdi_act_layer->height; y++)
		{
			for (x = 0; x < src_layer_width; x++)
			{
				color_t = get_ptr(new_bmp_ptr->buf_ptr,src_layer_width,x,y);
				gdi_act_color_to_rgb(&a,&new_color_r,&new_color_g,&new_color_b,color_t);
				new_color_r *= opacity_value;
				new_color_g *= opacity_value;
				new_color_b *= opacity_value;
				color_t = get_ptr(old_bmp_ptr->buf_ptr,src_layer_width,x,y);
				gdi_act_color_to_rgb(&a,&r,&g,&b,color_t);
				r = ((new_color_r) +  (r * inverse_opacity_value)) >> 8;
				g = ((new_color_g) +  (g * inverse_opacity_value)) >> 8;
				b = ((new_color_b) +  (b * inverse_opacity_value)) >> 8;
				put_ptr(x,y,gdi_act_color_from_rgb(a,r,g,b));
			}
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_darw_alpha_blending_screen)	
}

/**************************************************************

	FUNCTION NAME		: gdi_draw_frame_rect()

  	PURPOSE				: draw a rect with frame and filling.

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color rect_color
							  gdi_color	frame_color
							  S32			frame_width

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_frame_rect(	S32 x1, S32 y1, S32 x2, S32 y2, gdi_color	rect_color, gdi_color	frame_color, S32			frame_width)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_frame_rect)
		S32 i;

		GDI_CLIP_RECT_TEST(x1,y1,x2,y2,GDI_RETURN_VOID);

		/* draw interial color */
		GDI_DRAW_RECT(x1+frame_width, y1+frame_width, x2-frame_width, y2-frame_width, rect_color);

		/* draw frame */
		for( i = 0 ; i < frame_width; i++)
		{
			GDI_DRAW_H_LINE(x1+i, x2-i, y1+i, frame_color);	/* up horizontal */
			GDI_DRAW_H_LINE(x1+i, x2-i, y2-i, frame_color);	/* down horizontal */
			GDI_DRAW_V_LINE(y1+i, y2-i, x1+i, frame_color);	/* left vertical */
			GDI_DRAW_V_LINE(y1+i, y2-i, x2-i, frame_color);	/* right vertical */
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_frame_rect)
}


/**************************************************************

	FUNCTION NAME		: gdi_draw_round_rect()

  	PURPOSE				: draw a rect with round frame

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color rect_color
							  gdi_color	frame_color
							  S32			frame_width

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_round_rect(	S32 x1, S32 y1, S32 x2, S32 y2, gdi_color	rect_color, gdi_color	frame_color, S32			frame_width)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_round_rect)
		S32			i;
		S32			shift;
		gdi_color	result;

		GDI_CLIP_RECT_TEST(x1,y1,x2,y2,GDI_RETURN_VOID);

		/* draw interial color */
		GDI_DRAW_RECT(x1+frame_width, y1+frame_width, x2-frame_width, y2-frame_width, rect_color);


		/* draw frame */
		for( i = 0 ; i < frame_width; i++)
		{
			shift = frame_width - i;
			GDI_DRAW_H_LINE(x1+ shift, x2-shift, y1+i, frame_color);	/* up horizontal */
			GDI_DRAW_H_LINE(x1+ shift, x2-shift, y2-i, frame_color);	/* down horizontal */
			GDI_DRAW_V_LINE(y1+shift, y2-shift, x1+i, frame_color);	/* left vertical */
			GDI_DRAW_V_LINE(y1+shift, y2-shift, x2-i, frame_color);	/* right vertical */
		}

		/* draw anti_aliase pixels, make it smooth */
		GDI_COLOR_INTERPOLATE(frame_color, rect_color, 10, result);


		GDI_SET_BUFFER_PIXEL(x1+frame_width+1, y1+frame_width, result);
		GDI_SET_BUFFER_PIXEL(x1+frame_width,	y1+frame_width+1, result);

		GDI_SET_BUFFER_PIXEL(x1+frame_width+1, y2-frame_width, result);
		GDI_SET_BUFFER_PIXEL(x1+frame_width,	y2-frame_width-1, result);

		GDI_SET_BUFFER_PIXEL(x2-frame_width-1, y1+frame_width, result);
		GDI_SET_BUFFER_PIXEL(x2-frame_width,	y1+frame_width+1, result);

		GDI_SET_BUFFER_PIXEL(x2-frame_width-1, y2-frame_width, result);
		GDI_SET_BUFFER_PIXEL(x2-frame_width,	y2-frame_width-1, result);

		GDI_COLOR_INTERPOLATE(frame_color, rect_color, 30, result);
		GDI_SET_BUFFER_PIXEL(x1+frame_width	, y1+frame_width, result);
		GDI_SET_BUFFER_PIXEL(x1+frame_width	, y2-frame_width, result);
		GDI_SET_BUFFER_PIXEL(x2-frame_width	, y1+frame_width, result);
		GDI_SET_BUFFER_PIXEL(x2-frame_width	, y2-frame_width, result);
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_round_rect)
}

typedef struct _color
{	byte r;
	byte g;
	byte b;
	byte alpha;
} color_t;
extern void gui_fill_transparent_color(S32 x1, S32 y1, S32 x2, S32 y2, color_t c);
void gdi_draw_round_rect_with_transparent(	S32 x1, S32 y1, S32 x2, S32 y2, color_t	transparent_color,gdi_color	frame_color, S32			frame_width)
{

		S32			i;
		S32			shift;
		gdi_color	result;
		/* draw interial color */
		gui_fill_transparent_color(x1+frame_width, y1+frame_width, x2-frame_width, y2-frame_width, transparent_color);


		/* draw frame */
		for( i = 0 ; i < frame_width; i++)
		{
			shift = frame_width - i;
			GDI_DRAW_H_LINE(x1+ shift, x2-shift, y1+i, frame_color);	/* up horizontal */
			GDI_DRAW_H_LINE(x1+ shift, x2-shift, y2-i, frame_color);	/* down horizontal */
			GDI_DRAW_V_LINE(y1+shift, y2-shift, x1+i, frame_color);	/* left vertical */
			GDI_DRAW_V_LINE(y1+shift, y2-shift, x2-i, frame_color);	/* right vertical */
		}

}

/**************************************************************

	FUNCTION NAME		: gdi_draw_button_rect()

  	PURPOSE				: draw a rect with button style

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color rect_color
							  gdi_color	frame_color
							  S32			frame_width
							  U16			button_height
							  BOOL		invert

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_button_rect(	S32 x1, S32 y1, S32 x2, S32 y2, gdi_color	rect_color, gdi_color	frame_color, U16		frame_width, U16		button_height, BOOL		invert)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_button_rect)
		S32		i;
		U16		shift;
		U16		ratio_light;
		U16		ratio_dark;

		gdi_color result_light;
		gdi_color retult_dark;
		gdi_color black = gdi_act_color_from_rgb(0xff,0,0,0);
		gdi_color white = gdi_act_color_from_rgb(0xff,255,255,255);

		/* draw rect and frame */
		GDI_CLIP_RECT_TEST(x1,y1,x2,y2,GDI_RETURN_VOID);
		gdi_draw_frame_rect(x1, y1, x2, y2, rect_color, frame_color, frame_width);


		/* add button style */
		ratio_light = 70;
		ratio_dark	= 80;
		for(i = 0; i < button_height; i++)
		{
			shift = button_height+frame_width-i-1;

			if(invert == TRUE)
			{
				GDI_COLOR_INTERPOLATE(rect_color, white, ratio_light, retult_dark);	/* light */
				GDI_COLOR_INTERPOLATE(rect_color, black, ratio_dark, result_light);	/* dark */
			}
			else
			{
				GDI_COLOR_INTERPOLATE(rect_color, white, ratio_light, result_light); /* light*/
				GDI_COLOR_INTERPOLATE(rect_color, black, ratio_dark, retult_dark);	 /* dark */
			}

			GDI_DRAW_H_LINE(x1+shift, x2-shift-1, y1+shift, result_light);	/* up horizontal */
			GDI_DRAW_V_LINE(y1+shift, y2-shift, x1+shift, result_light);		/* left vertical */

			GDI_DRAW_H_LINE(x1+shift, x2-shift, y2-shift, retult_dark);		/* down vertical */
			GDI_DRAW_V_LINE(y1+shift, y2-shift, x2-shift, retult_dark);		/* left vertical */
			ratio_light = ratio_light - 10;
			ratio_dark	= ratio_dark  - 10;
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_button_rect)
}


/**************************************************************

	FUNCTION NAME		: gdi_draw_shadow_rect()

  	PURPOSE				: draw a rect with shadow style

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color rect_color
							  gdi_color	frame_color
							  S32			frame_width
							  gdi_color	shadow_color
							  S32		shadow_height

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_shadow_rect(	S32 x1, S32 y1, S32 x2, S32 y2, gdi_color	rect_color, gdi_color	frame_color, S32			frame_width, gdi_color	shadow_color, S32			shadow_height)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_shadow_rect)
		S32		 i;

		/* draw rect and frame */
		GDI_CLIP_RECT_TEST(x1,y1,x2,y2,GDI_RETURN_VOID);
		gdi_draw_frame_rect(x1, y1, x2, y2, rect_color, frame_color, frame_width);

		for( i = 0 ; i < shadow_height; i++)
		{
			GDI_DRAW_H_LINE(x1+i+2, x2+i+1, y2+i+1, shadow_color);	/* horizontal */
			GDI_DRAW_V_LINE(y1+i+2, y2+i+1, x2+i+1, shadow_color);	/* vertical */
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_shadow_rect)
}


/**************************************************************

	FUNCTION NAME		: gdi_draw_gradient_rect()

  	PURPOSE				: draw a rect with gradient filling

	INPUT PARAMETERS	: S32			x1
							  S32			y1
							  S32			x2
							  S32			y2
							  gdi_color rect_color
							  gdi_color	frame_color
							  S32			frame_width
							  gdi_gradient_rect_style_enum style

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_draw_gradient_rect(	S32 x1, S32 y1, S32 x2, S32 y2, gdi_color	rect_color_start, gdi_color	rect_color_end, gdi_color	frame_color, S32			frame_width, gdi_gradient_rect_style_enum style)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_draw_gradient_rect)
		S32		 i,j;
		S32		 ratio;
		S32		 size;
		gdi_color result;

		GDI_CLIP_RECT_TEST(x1,y1,x2,y2,GDI_RETURN_VOID);

		switch(style)
		{
		case GDI_GRADIENT_RECT_HOR:
			{
				size = x2 - x1;
				if(size ==0) size = 1;

				for( i = x1 ; i <= x2; i++)
				{
					ratio = (i-x1)*100/size;
					GDI_COLOR_INTERPOLATE(rect_color_end, rect_color_start, ratio, result);	/* light */
					GDI_DRAW_V_LINE(y1, y2, i, result);	/* left vertical */
				}
				break;
			}

		case GDI_GRADIENT_RECT_VER:
			{
				size = y2 - y1;
				if(size ==0) size = 1;

				for( i = y1 ; i <= y2; i++)
				{
					ratio = (i-y1)*100/size;
					GDI_COLOR_INTERPOLATE(rect_color_end, rect_color_start, ratio, result);	/* light */
					GDI_DRAW_H_LINE(x1, x2, i, result);	/* left vertical */
				}
				break;
			}

		case GDI_GRADIENT_RECT_DIA:
			{
				size = (x2-x1) + (y2-y1);

				for( i = y1 ; i <= y2; i++)
				{
					for(j = x1 ; j <= x2; j++)
					{
						ratio = ((i-y1)+(j-x1))*100/size;
						GDI_COLOR_INTERPOLATE(rect_color_end, rect_color_start, ratio, result);	/* light */
						GDI_SET_BUFFER_PIXEL(j, i, result);
					}
				}
				break;
			}

		case GDI_GRADIENT_RECT_DIA_INV:
			{
				size = (x2-x1) + (y2-y1);

				for( i = x1 ; i <= x2; i++)
				{
					for(j = y1 ; j <= y2; j++)
					{
						ratio = ((i-x1)+(j-y1))*100/size;
						GDI_COLOR_INTERPOLATE(rect_color_end, rect_color_start, ratio, result);	/* light */
						GDI_SET_BUFFER_PIXEL(i, j, result);
					}
				}
				break;
			}
		case GDI_GRADIENT_RECT_FLIP:
			{
				size = y2 - y1;
				if(size ==0) size = 1;

				for( i = y1 ; i <= y1+((y2-y1)>>1)-1; i++)
				{
					ratio = (i-y1)*2*100/size;
					if (ratio>100) ratio=100;
					GDI_COLOR_INTERPOLATE(rect_color_start, rect_color_end, ratio, result);	/* light */
					GDI_DRAW_H_LINE(x1, x2, i, result);	/* left vertical */
				}
				for( i = y1+((y2-y1)>>1) ; i <= y2; i++)
				{
					ratio = (y2-i)*2*100/size;
					if (ratio>100) ratio=100;
					GDI_COLOR_INTERPOLATE(rect_color_start, rect_color_end, ratio, result);	/* light */
					GDI_DRAW_H_LINE(x1, x2, i, result);	/* left vertical */
				}
				break;
			}
		}
		/* draw frame */
		for( i = 0 ; i < frame_width; i++)
		{
			GDI_DRAW_H_LINE(x1+i, x2-i, y1+i, frame_color);	/* up horizontal */
			GDI_DRAW_H_LINE(x1+i, x2-i, y2-i, frame_color);	/* down horizontal */
			GDI_DRAW_V_LINE(y1+i, y2-i, x1+i, frame_color);	/* left vertical */
			GDI_DRAW_V_LINE(y1+i, y2-i, x2-i, frame_color);	/* right vertical */
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_draw_gradient_rect)
}


/**************************************************************

	FUNCTION NAME		: gdi_2d_memory_blt()

  	PURPOSE				: 2d memory blt

	INPUT PARAMETERS	: U8 *src_ptr
							  S32 src_pitch
							  S32 src_offset_x
							  S32 src_offset_y,
							  S32 src_width
							  S32 src_height,
							  U8 *dest_ptr
							  S32 dest_pitch,
							  S32 dest_offset_x
							  S32 dest_offset_y,
							  gdi_rect_struct dest_clip)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_2d_memory_blt(	U8 *src_ptr, S32 src_pitch, S32 src_offset_x, S32 src_offset_y, S32 src_width, S32 src_height, U8 *dest_ptr, S32 dest_pitch, S32 dest_offset_x, S32 dest_offset_y, gdi_rect_struct dest_clip)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_2d_memory_blt)
		U8		*dest_buf_ptr;
		U8		*src_buf_ptr;
		U8		*dest_line_start_ptr;
		U8		*src_line_start_ptr;


		//S32	i;
		S32	y_jump, x_jump;
		S32	jump_size;
		S32	start, end;
		S32	x_draw_count, y_draw_count;

		dest_buf_ptr	= dest_ptr;
		src_buf_ptr		= src_ptr;
		src_buf_ptr		+= ((src_offset_y*src_pitch*GDI_LAYER.bits_per_pixel)>>3);
		src_buf_ptr 	+= ((src_offset_x*GDI_LAYER.bits_per_pixel)>>3);

		/* if not totally out of clip region */
		if(!(	(dest_offset_x					> dest_clip.x2) ||
				(dest_offset_y					> dest_clip.y2) ||
				(dest_offset_x+src_width	< dest_clip.x1) ||
				(dest_offset_y+src_height	< dest_clip.y1)) )
		{
			/* adjust source ptr to the start of drawing point */

			/* jump to start y line */
			y_jump = 0;
			if(dest_offset_y < dest_clip.y1)
			{
				y_jump		= dest_clip.y1-dest_offset_y;
				src_buf_ptr	+=((y_jump*src_pitch*GDI_LAYER.bits_per_pixel)>>3);
			}

			/* jump to start x */
			x_jump = 0;
			if(dest_offset_x < dest_clip.x1)
			{
				x_jump = dest_clip.x1-dest_offset_x;
				src_buf_ptr += (x_jump*GDI_LAYER.bits_per_pixel)>>3;
				
			}

			src_line_start_ptr = src_buf_ptr;


			/* adjust dest ptr to the start of drawing point */
			/* find start ptr for actual drawing */
			if(dest_offset_x < dest_clip.x1)
			{
				jump_size =(((y_jump+dest_offset_y)*dest_pitch+dest_clip.x1)*GDI_LAYER.bits_per_pixel)>>3;
				dest_line_start_ptr = dest_ptr+jump_size;
			}
			else
			{
				jump_size = (((y_jump+dest_offset_y)*dest_pitch+dest_offset_x)*GDI_LAYER.bits_per_pixel)>>3;
				dest_line_start_ptr = dest_ptr+jump_size;
			}


			/* find x_draw_count and y_draw_count */
			start	= (dest_offset_x < dest_clip.x1)							? dest_clip.x1 : dest_offset_x;
			end	= (dest_offset_x+(S32)src_width-1 > dest_clip.x2)	? dest_clip.x2 : dest_offset_x+(S32)src_width-1;
			x_draw_count = end-start+1;


			start	= (dest_offset_y < dest_clip.y1)							? dest_clip.y1 : dest_offset_y;
			end	= (dest_offset_y+(S32)src_height-1 > dest_clip.y2)	? dest_clip.y2 : dest_offset_y+(S32)src_height-1;
			y_draw_count = end-start+1;


			{
				U16 *src;
				U16 *dest;
				S32 x,y;
				for(y=0;y< y_draw_count;y++)
				{
					dest	= (U16*)(dest_line_start_ptr+((dest_pitch*y*GDI_LAYER.bits_per_pixel)>>3));
					src	= (U16*)(src_line_start_ptr+((src_pitch*y*GDI_LAYER.bits_per_pixel)>>3));

					for(x=0;x<x_draw_count;x++)
					{
						U16 c = *src;
						if(c == GDI_COLOR_TRANSPARENT)  c--;
						
						*dest = c;
						
						dest++;
						src++;
					}
				}
			}
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_2d_memory_blt)
}


/**************************************************************

	FUNCTION NAME		: gdi_2d_memory_blt_without_transpant_check()

  	PURPOSE				: 2d memory blt

	INPUT PARAMETERS	: U8 *src_ptr
							  S32 src_pitch
							  S32 src_offset_x
							  S32 src_offset_y,
							  S32 src_width
							  S32 src_height,
							  U8 *dest_ptr
							  S32 dest_pitch,
							  S32 dest_offset_x
							  S32 dest_offset_y,
							  gdi_rect_struct dest_clip)

	OUTPUT PARAMETERS	: nil

	RETURNS				: void

 

***************************************************************/
void gdi_2d_memory_blt_without_transpant_check(	U8 *src_ptr, S32 src_pitch, S32 src_offset_x, S32 src_offset_y, S32 src_width, S32 src_height, U8 *dest_ptr, S32 dest_pitch, S32 dest_offset_x, S32 dest_offset_y, gdi_rect_struct dest_clip)
{
	GDI_ENTER_CRITICAL_SECTION(gdi_2d_memory_blt)
		U8		*dest_buf_ptr;
		U8		*src_buf_ptr;
		U8		*dest_line_start_ptr;
		U8		*src_line_start_ptr;


		//S32	i;
		S32	y_jump, x_jump;
		S32	jump_size;
		S32	start, end;
		S32	x_draw_count, y_draw_count;

		dest_buf_ptr	= dest_ptr;
		src_buf_ptr		= src_ptr;
		src_buf_ptr		+= ((src_offset_y*src_pitch*GDI_LAYER.bits_per_pixel)>>3);
		src_buf_ptr 	+= ((src_offset_x*GDI_LAYER.bits_per_pixel)>>3);

		/* if not totally out of clip region */
		if(!(	(dest_offset_x					> dest_clip.x2) ||
				(dest_offset_y					> dest_clip.y2) ||
				(dest_offset_x+src_width	< dest_clip.x1) ||
				(dest_offset_y+src_height	< dest_clip.y1)) )
		{
			/* adjust source ptr to the start of drawing point */

			/* jump to start y line */
			y_jump = 0;
			if(dest_offset_y < dest_clip.y1)
			{
				y_jump		= dest_clip.y1-dest_offset_y;
				src_buf_ptr	+=((y_jump*src_pitch*GDI_LAYER.bits_per_pixel)>>3);
			}

			/* jump to start x */
			x_jump = 0;
			if(dest_offset_x < dest_clip.x1)
			{
				x_jump = dest_clip.x1-dest_offset_x;
				src_buf_ptr += (x_jump*GDI_LAYER.bits_per_pixel)>>3;
				
			}

			src_line_start_ptr = src_buf_ptr;


			/* adjust dest ptr to the start of drawing point */
			/* find start ptr for actual drawing */
			if(dest_offset_x < dest_clip.x1)
			{
				jump_size =(((y_jump+dest_offset_y)*dest_pitch+dest_clip.x1)*GDI_LAYER.bits_per_pixel)>>3;
				dest_line_start_ptr = dest_ptr+jump_size;
			}
			else
			{
				jump_size = (((y_jump+dest_offset_y)*dest_pitch+dest_offset_x)*GDI_LAYER.bits_per_pixel)>>3;
				dest_line_start_ptr = dest_ptr+jump_size;
			}


			/* find x_draw_count and y_draw_count */
			start	= (dest_offset_x < dest_clip.x1)							? dest_clip.x1 : dest_offset_x;
			end	= (dest_offset_x+(S32)src_width-1 > dest_clip.x2)	? dest_clip.x2 : dest_offset_x+(S32)src_width-1;
			x_draw_count = end-start+1;


			start	= (dest_offset_y < dest_clip.y1)							? dest_clip.y1 : dest_offset_y;
			end	= (dest_offset_y+(S32)src_height-1 > dest_clip.y2)	? dest_clip.y2 : dest_offset_y+(S32)src_height-1;
			y_draw_count = end-start+1;


			{
				U16 *src;
				U16 *dest;
				S32 x,y;
				for(y=0;y< y_draw_count;y++)
				{
					dest	= (U16*)(dest_line_start_ptr+((dest_pitch*y*GDI_LAYER.bits_per_pixel)>>3));
					src	= (U16*)(src_line_start_ptr+((src_pitch*y*GDI_LAYER.bits_per_pixel)>>3));

					for(x=0;x<x_draw_count;x++)
					{
						U16 c = *src;
						
						*dest = c;
						
						dest++;
						src++;
					}
				}
			}
		}
	GDI_EXIT_CRITICAL_SECTION(gdi_2d_memory_blt)
}

#ifdef MMI_ON_HARDWARE_P /* zhuoxz,2009-4-6 */
#if defined(CSD_TARGET)
__inline int GDI_BYTE16SET(int sizeReg, int add, int a)
{
#if 1
	while (add<sizeReg)
	{

	   	((unsigned int*)add)[0] = a;
		add += 4;
	}
	return add;
#else
	#ifdef __RVCT__
	#pragma arm	
	#endif
	    __asm
		{
		        MOV r0, a;
		        MOV r1, r0;
		        MOV r2, r0;
		        MOV r3, r0;
		        MOV r4, add;
		        MOV r5, sizeReg;
		    CPYLOOP:
		        STMIA r4!, {r0, r1, r2, r3};
		        CMP r5, r4;
		        BGT CPYLOOP;
		        MOV add, r4;
		};
	#ifdef __RVCT__
	#pragma thumb
	#endif
	return add;
#endif
}
#endif /* MMI_ON_HARDWARE_P */

void gdi_memset16(U8* address,U16 pixel,int size)
{
	if(size < 20)
	{
		register int add = (int)address;
		int sizeR = size;
		int postcount;
		if(add & 0x3)
		{
			*((unsigned short*)add) = pixel;
			add += 2;
			sizeR -= 2;
		}
		postcount = sizeR & 0x3;
		sizeR -= postcount;
		if(sizeR > 0)
		{
			register unsigned int a;
			register int sizeReg = sizeR;
			register int endAdd = add + sizeReg;
			a = pixel + (pixel << 16);
			do
			{
				((unsigned int*)add)[0] = a;
				add += 4;
			}while(add < endAdd);
		}
		if(postcount != 0)
		{
			*((unsigned short*)add) = pixel;
		}
	}
	else
	{
		register int add = (int)address;
		int sizeR = size;
		int postcount;
		register unsigned int a;
		if(add & 0x3)
		{
			*((unsigned short*)add) = pixel;
			add += 2;
			sizeR -= 2;
		}
		postcount = (((unsigned int)sizeR) &0x00f);
		sizeR -= postcount;
		{
			register int sizeReg = sizeR + (int)add;
			a = pixel + (pixel << 16);
			add = GDI_BYTE16SET(sizeReg, add, a);
		}
		sizeR = ((postcount >> 2) << 2);
		if(sizeR > 0)
		{
			register int sizeReg = sizeR;
			register int endAdd = add + sizeReg;
			do
			{
				((unsigned int*)add)[0] = a;
				add += 4;
			}while(add < endAdd);
		}
		if((postcount & 0x3) != 0)
		{
			*((unsigned short*)add) = pixel;
		}
	}
}
#else
void gdi_memset16(U8* address,U16 pixel,int size)
{
        register int add = (int)address;
        int sizeR = size;
        int postcount;
        if(add & 0x3)
	{
		*((unsigned short*)add) = pixel;
		add += 2;
		sizeR -= 2;
	}
	postcount = sizeR & 0x3;
	sizeR -= postcount;
	if(sizeR > 0)
	{
		register unsigned int a;
		register int sizeReg = sizeR;
		register int endAdd = add + sizeReg;
		a = pixel + (pixel << 16);
		do
		{
			((unsigned int*)add)[0] = a;
			add += 4;
		}while(add < endAdd);
	}
	if(postcount != 0)
	{
		*((unsigned short*)add) = pixel;
	}
}

#endif

// 		size of pixels  = (A*pixels + B) >> C
static const S32 bitsperpixels[] ={	1,	8,	16,	24,	32};
static const S32 sizeofpixelsA[] ={	1,	1,	2,	3,	4};
static const S32 sizeofpixelsB[] ={	7,	0,	0,	0,	0};	
static const S32 sizeofpixelsC[] ={	3,	0,	0,	0,	0};
S32 gdi_sizeof_pixels(gdi_color_format cf,S32 width,S32 height)
{
	if(cf <GDI_COLOR_FORMAT_STANDARD_END)	
		return ((width * sizeofpixelsA[cf] + sizeofpixelsB[cf]) >> sizeofpixelsC[cf]) * height;
	
	if(cf == GDI_COLOR_FORMAT_MAINLCD)
		return ((width*GDI_MAINLCD_BIT_PER_PIXEL+7)>>3)*height;
	
#ifdef __MMI_SUBLCD__
	if(cf == GDI_COLOR_FORMAT_SUBLCD)
		return ((width*GDI_SUBLCD_BIT_PER_PIXEL+7)>>3)*height;
#endif

	GDI_ASSERT(0);
	return 0;
}

gdi_color_format gdi_get_color_format(S32 bits)
{
	if(bits == 1)
		return GDI_COLOR_FORMAT_1;
	else if(bits == 8)
		return GDI_COLOR_FORMAT_8;
	else if(bits == 16)
		return GDI_COLOR_FORMAT_16;
	else if(bits == 18)
		return GDI_COLOR_FORMAT_24;
	else if(bits == 24)
		return GDI_COLOR_FORMAT_24;
	else if(bits == 32)
		return GDI_COLOR_FORMAT_32;
	
	else if(bits == DRV_MAINLCD_BIT_PER_PIXEL)
		return GDI_COLOR_FORMAT_MAINLCD;
	
#ifdef __MMI_SUBLCD__
	else if(bits == DRV_SUBLCD_BIT_PER_PIXEL)
		return GDI_COLOR_FORMAT_SUBLCD;
#endif

	else
		GDI_ASSERT(0);
	return GDI_COLOR_FORMAT_32; // default we use the maximum data type
}

S32 gdi_bits_per_pixel(gdi_color_format cf)
{
	if(cf <GDI_COLOR_FORMAT_STANDARD_END)
		return bitsperpixels[cf];
	
	if(cf == GDI_COLOR_FORMAT_MAINLCD)
		return GDI_MAINLCD_BIT_PER_PIXEL;
	
#ifdef __MMI_SUBLCD__
	if(cf == GDI_COLOR_FORMAT_SUBLCD)
		return GDI_SUBLCD_BIT_PER_PIXEL;
#endif

	GDI_ASSERT(0);
	return 0;
}


/**************************************************************

	GDI Software Resizer
	
***************************************************************/
#if 0
#if defined(CSD_TARGET)
#pragma arm section rwdata = "INTERNRW" , rodata = "INTERNCONST" , zidata = "INTERNZI"
#endif
#endif

gdi_resizer_struct GDI_RESIZER;
gdi_resizer_put_func gdi_resizer_put;

 
//#define GDI_NEED_RESIZER_BUFFER		GDI_LCD_WIDTH
//#if GDI_NEED_RESIZER_BUFFER 			< GDI_LCD_HEIGHT
//	#undef GDI_NEED_RESIZER_BUFFER
//	#define GDI_NEED_RESIZER_BUFFER GDI_LCD_HEIGHT
//#endif

//#ifdef __MMI_SUBLCD__
//	#if GDI_NEED_RESIZER_BUFFER 			< GDI_SUBLCD_WIDTH
//		#undef GDI_NEED_RESIZER_BUFFER
//		#define GDI_NEED_RESIZER_BUFFER GDI_SUBLCD_WIDTH
//	#endif
	
//	#if GDI_NEED_RESIZER_BUFFER 			< GDI_SUBLCD_HEIGHT
//		#undef GDI_NEED_RESIZER_BUFFER
//		#define GDI_NEED_RESIZER_BUFFER GDI_SUBLCD_HEIGHT
//	#endif
//#endif

S16 gdi_resizer_buffer[GDI_NEED_RESIZER_BUFFER];
#if 0
#if defined(CSD_TARGET)
#pragma arm section rwdata, rodata , zidata
#endif



#if defined(CSD_TARGET)
#pragma arm section code = "INTERNCODE"
#endif
#endif

void gdi_resizer_put_null(gdi_color c,BOOL want_draw)
{
	// keep empty
}
void gdi_resizer_put_8(gdi_color c,BOOL want_draw)
{
//	do
	{
		if(want_draw)
			*((U8*)GDI_RESIZER.dest) = (U8)c;

		GDI_RESIZER.dest+=1;
		// generate next_position
		if(GDI_RESIZER.next_want_sx >= GDI_RESIZER.want_sx_table_end)
		{
			GDI_RESIZER.next_want_sx = GDI_RESIZER.want_sx_table;

			if(GDI_RESIZER.dir_x>0)
				GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (GDI_RESIZER.want_dx1+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )*1;
			else
				GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (GDI_RESIZER.want_dx2+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )*1;
			//GDI_RESIZER.want_sy = (((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.y_scale_factor) >> 16);
			GDI_RESIZER.want_sy = ((((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.src_height_range << 1) +  GDI_RESIZER.dest_height_range) / (GDI_RESIZER.dest_height_range << 1));

			if(GDI_RESIZER.next_want_dy < GDI_RESIZER.want_dy1 || GDI_RESIZER.next_want_dy > GDI_RESIZER.want_dy2)
			{
				gdi_resizer_put=gdi_resizer_put_null;
				return;
			}
			GDI_RESIZER.next_want_dy += GDI_RESIZER.dir_y;
		}
		GDI_RESIZER.want_sx = *GDI_RESIZER.next_want_sx++;
	}//while(x== GDI_RESIZER.want_sx && y == GDI_RESIZER.want_sy);
}

void gdi_resizer_put_16(gdi_color c,BOOL want_draw)
{
//	do
	{
		if(want_draw)
			*((U16*)GDI_RESIZER.dest) = (U16)c;

		GDI_RESIZER.dest+=2;
		// generate next_position
		if(GDI_RESIZER.next_want_sx >= GDI_RESIZER.want_sx_table_end)
		{
			GDI_RESIZER.next_want_sx = GDI_RESIZER.want_sx_table;

			if(GDI_RESIZER.dir_x>0)
				GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (GDI_RESIZER.want_dx1+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )*2;
			else
				GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (GDI_RESIZER.want_dx2+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )*2;
			//GDI_RESIZER.want_sy = (((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.y_scale_factor) >> 16);
			GDI_RESIZER.want_sy = ((((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.src_height_range << 1) +  GDI_RESIZER.dest_height_range) / (GDI_RESIZER.dest_height_range << 1));

			if(GDI_RESIZER.next_want_dy < GDI_RESIZER.want_dy1 || GDI_RESIZER.next_want_dy > GDI_RESIZER.want_dy2)
			{
				gdi_resizer_put=gdi_resizer_put_null;
				return;
			}
			GDI_RESIZER.next_want_dy += GDI_RESIZER.dir_y;
		}
		GDI_RESIZER.want_sx = *GDI_RESIZER.next_want_sx++;
	}//while(x== GDI_RESIZER.want_sx && y == GDI_RESIZER.want_sy);
}
	
void gdi_resizer_put_24(gdi_color c,BOOL want_draw)
{
//	do
	{
		if(want_draw)
		{
			U32 *ptr = (U32*)(GDI_RESIZER.dest);
			switch(((U32)ptr)&0x3)
			{
			case 0:
				*ptr = (*ptr&0xff000000)|(c&0x00ffffff);
				break;
			case 1:
				ptr= (U32*)(((U32)ptr)&(~0x3));
				*ptr = (*ptr&0x000000ff)|(c<<8);
				break;
			case 2:
				ptr= (U32*)(((U32)ptr)&(~0x3));
				*ptr = (*ptr&0x0000ffff)|(c<<16); ptr++;
				*ptr = (*ptr&0xffffff00)|((c&0x00ff0000)>>16);
				break;
			default:
				ptr= (U32*)(((U32)ptr)&(~0x3));
				*ptr = (*ptr&0x00ffffff)|(c<<24); ptr++;
				*ptr = (*ptr&0xffff0000)|((c&0x00ffff00)>>8);
				break;
			}
		}

		GDI_RESIZER.dest+=3;
		// generate next_position
		if(GDI_RESIZER.next_want_sx >= GDI_RESIZER.want_sx_table_end)
		{
			GDI_RESIZER.next_want_sx = GDI_RESIZER.want_sx_table;

			if(GDI_RESIZER.dir_x>0)
				GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (GDI_RESIZER.want_dx1+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )*3;
			else
				GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (GDI_RESIZER.want_dx2+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )*3;
			//GDI_RESIZER.want_sy = (((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.y_scale_factor) >> 16);
			GDI_RESIZER.want_sy = ((((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.src_height_range << 1) +  GDI_RESIZER.dest_height_range) / (GDI_RESIZER.dest_height_range << 1));

			if(GDI_RESIZER.next_want_dy < GDI_RESIZER.want_dy1 || GDI_RESIZER.next_want_dy > GDI_RESIZER.want_dy2)
			{
				gdi_resizer_put=gdi_resizer_put_null;
				return;
			}
			GDI_RESIZER.next_want_dy += GDI_RESIZER.dir_y;
		}
		GDI_RESIZER.want_sx = *GDI_RESIZER.next_want_sx++;
	}//while(x== GDI_RESIZER.want_sx && y == GDI_RESIZER.want_sy);
}

void gdi_resizer_put_32(gdi_color c,BOOL want_draw)
{
//	do
	{
		if(want_draw)
		{
			 *(U32*)(GDI_RESIZER.dest) = (U32)c;
		}

		GDI_RESIZER.dest+=4;
		// generate next_position
		if(GDI_RESIZER.next_want_sx >= GDI_RESIZER.want_sx_table_end)
		{
			GDI_RESIZER.next_want_sx = GDI_RESIZER.want_sx_table;

			if(GDI_RESIZER.dir_x>0)
				GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (GDI_RESIZER.want_dx1+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )*4;
			else
				GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (GDI_RESIZER.want_dx2+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )*4;
			//GDI_RESIZER.want_sy = (((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.y_scale_factor) >> 16);
			GDI_RESIZER.want_sy = ((((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.src_height_range << 1) +  GDI_RESIZER.dest_height_range) / (GDI_RESIZER.dest_height_range << 1));

			if(GDI_RESIZER.next_want_dy < GDI_RESIZER.want_dy1 || GDI_RESIZER.next_want_dy > GDI_RESIZER.want_dy2)
			{
				gdi_resizer_put=gdi_resizer_put_null;
				return;
			}
			GDI_RESIZER.next_want_dy += GDI_RESIZER.dir_y;
		}
		GDI_RESIZER.want_sx = *GDI_RESIZER.next_want_sx++;
	}//while(x== GDI_RESIZER.want_sx && y == GDI_RESIZER.want_sy);
}

#if  GDI_MAINLCD_BIT_PER_PIXEL ==  1 || !defined(GDI_USING_LAYER)
void gdi_resizer_put_mainlcd(gdi_color c,BOOL want_draw)
{
//	do
	{
        /* 
         * Use GDI_RESIZER.next_want_sx - GDI_RESIZER.want_sx_table instead of GDI_RESIZER.want_sx
         * as jumping to the next pixel
         * GDI_RESIZER.next_want_dy - 1 is the current y position
         */
		if(want_draw)
			DRV_MAINLCD_SET_BUFFER_PIXEL(
				GDI_RESIZER.want_dx1 + (int)(GDI_RESIZER.next_want_sx - GDI_RESIZER.want_sx_table) - 1, 
				GDI_RESIZER.next_want_dy - 1,
				c);

		// generate next_position
		if(GDI_RESIZER.next_want_sx >= GDI_RESIZER.want_sx_table_end)
		{
			GDI_RESIZER.next_want_sx = GDI_RESIZER.want_sx_table;

			//GDI_RESIZER.want_sy = (((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.y_scale_factor) >> 16);
			GDI_RESIZER.want_sy = ((((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.src_height_range << 1) +  GDI_RESIZER.dest_height_range) / (GDI_RESIZER.dest_height_range << 1));

			if(GDI_RESIZER.next_want_dy < GDI_RESIZER.want_dy1 || GDI_RESIZER.next_want_dy > GDI_RESIZER.want_dy2)
			{
				gdi_resizer_put=gdi_resizer_put_null;
				return;
			}
			GDI_RESIZER.next_want_dy += GDI_RESIZER.dir_y;
		}
		GDI_RESIZER.want_sx = *GDI_RESIZER.next_want_sx++;
	}//while(x== GDI_RESIZER.want_sx && y == GDI_RESIZER.want_sy);
}
#elif GDI_MAINLCD_BIT_PER_PIXEL == 16
	#define gdi_resizer_put_mainlcd 	gdi_resizer_put_16
#elif GDI_MAINLCD_BIT_PER_PIXEL == 24
	#define gdi_resizer_put_mainlcd 	gdi_resizer_put_24
#elif GDI_MAINLCD_BIT_PER_PIXEL == 32
	#define gdi_resizer_put_mainlcd 	gdi_resizer_put_32
#else
	#error "Unknown MAINLCD_BIT_PER_PIXEL"
#endif

#ifdef __MMI_SUBLCD__
#if GDI_SUBLCD_BIT_PER_PIXEL ==  1 || !defined(GDI_USING_LAYER)
void gdi_resizer_put_sublcd(gdi_color c,BOOL want_draw)
{
//	do
	{
        /* 
         * Use GDI_RESIZER.next_want_sx - GDI_RESIZER.want_sx_table instead of GDI_RESIZER.want_sx
         * as jumping to the next pixel
         * GDI_RESIZER.next_want_dy - 1 is the current y position
         */
		if(want_draw)
			DRV_SUBLCD_SET_BUFFER_PIXEL(
				GDI_RESIZER.want_dx1 + (int)(GDI_RESIZER.next_want_sx - GDI_RESIZER.want_sx_table) - 1, 
				GDI_RESIZER.next_want_dy - 1,
				c);

		// generate next_position
		if(GDI_RESIZER.next_want_sx >= GDI_RESIZER.want_sx_table_end)
		{
			GDI_RESIZER.next_want_sx = GDI_RESIZER.want_sx_table;

			//GDI_RESIZER.want_sy = (((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.y_scale_factor) >> 16);
			GDI_RESIZER.want_sy = ((((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * GDI_RESIZER.src_height_range << 1) +  GDI_RESIZER.dest_height_range) / (GDI_RESIZER.dest_height_range << 1));

			if(GDI_RESIZER.next_want_dy < GDI_RESIZER.want_dy1 || GDI_RESIZER.next_want_dy > GDI_RESIZER.want_dy2)
			{
				gdi_resizer_put=gdi_resizer_put_null;
				return;
			}
			GDI_RESIZER.next_want_dy += GDI_RESIZER.dir_y;
		}
		GDI_RESIZER.want_sx = *GDI_RESIZER.next_want_sx++;
	}//while(x== GDI_RESIZER.want_sx && y == GDI_RESIZER.want_sy);
}
#elif GDI_SUBLCD_BIT_PER_PIXEL == 16
	#define gdi_resizer_put_sublcd 	gdi_resizer_put_16
#elif GDI_SUBLCD_BIT_PER_PIXEL == 24
	#define gdi_resizer_put_sublcd 	gdi_resizer_put_24
#elif GDI_SUBLCD_BIT_PER_PIXEL == 32
	#define gdi_resizer_put_sublcd 	gdi_resizer_put_32
#else
		#error "Unknown SUBLCD_BIT_PER_PIXEL"
#endif
#endif

void gdi_non_resizer_put_8(gdi_color c,BOOL want_draw)
{
	#define GDI_RESIZER_PUT_PIXEL(COLOR)			*((U8*)GDI_RESIZER.dest) = (U8)COLOR
	#define GDI_RESIZER_INCREASE_DEST				GDI_RESIZER.dest ++
	GDI_NON_RESIZER_PUT_X(c,want_draw);
	#undef GDI_RESIZER_PUT_PIXEL
	#undef GDI_RESIZER_INCREASE_DEST
}

void gdi_non_resizer_put_16(gdi_color c,BOOL want_draw)
{
	#define GDI_RESIZER_PUT_PIXEL(COLOR)		*((U16*)GDI_RESIZER.dest) = (U16)COLOR
	#define GDI_RESIZER_INCREASE_DEST			GDI_RESIZER.dest +=2
	GDI_NON_RESIZER_PUT_X(c,want_draw);
	#undef GDI_RESIZER_PUT_PIXEL
	#undef GDI_RESIZER_INCREASE_DEST
}

void gdi_non_resizer_put_24(gdi_color c,BOOL want_draw)
{
	#define GDI_RESIZER_PUT_PIXEL(COLOR)			\
			do\
			{\
				U32 *ptr = (U32*)(GDI_RESIZER.dest);\
				switch(((U32)ptr)&0x3)\
				{\
				case 0:\
					*ptr = (*ptr&0xff000000)|(COLOR&0x00ffffff);\
					break;\
				case 1:\
					ptr= (U32*)(((U32)ptr)&(~0x3));\
					*ptr = (*ptr&0x000000ff)|(COLOR<<8);\
					break;\
				case 2:\
					ptr= (U32*)(((U32)ptr)&(~0x3));\
					*ptr = (*ptr&0x0000ffff)|(COLOR<<16); ptr++;\
					*ptr = (*ptr&0xffffff00)|((COLOR&0x00ff0000)>>16);\
					break;\
				default:\
					ptr= (U32*)(((U32)ptr)&(~0x3));\
					*ptr = (*ptr&0x00ffffff)|(COLOR<<24); ptr++;\
					*ptr = (*ptr&0xffff0000)|((COLOR&0x00ffff00)>>8);\
				}\
			}while(0)

	#define GDI_RESIZER_INCREASE_DEST				GDI_RESIZER.dest +=3
	GDI_NON_RESIZER_PUT_X(c,want_draw);
	#undef GDI_RESIZER_PUT_PIXEL
	#undef GDI_RESIZER_INCREASE_DEST
}
void gdi_non_resizer_put_32(gdi_color c,BOOL want_draw)
{
	#define GDI_RESIZER_PUT_PIXEL(COLOR)			*((U32*)GDI_RESIZER.dest) = (U32)COLOR
	#define GDI_RESIZER_INCREASE_DEST				GDI_RESIZER.dest +=4
	GDI_NON_RESIZER_PUT_X(c,want_draw);
	#undef GDI_RESIZER_PUT_PIXEL
	#undef GDI_RESIZER_INCREASE_DEST
}

#if  GDI_MAINLCD_BIT_PER_PIXEL ==  1 || !defined(GDI_USING_LAYER)
void gdi_non_resizer_put_mainlcd(gdi_color c,BOOL want_draw)
{
	#define GDI_RESIZER_PUT_PIXEL(COLOR)			DRV_MAINLCD_SET_BUFFER_PIXEL((GDI_RESIZER.offset_dx + GDI_RESIZER.want_sx),(GDI_RESIZER.offset_dy + GDI_RESIZER.want_sy),COLOR)
	#define GDI_RESIZER_INCREASE_DEST				
	GDI_NON_RESIZER_PUT_X(c,want_draw);
	#undef GDI_RESIZER_PUT_PIXEL
	#undef GDI_RESIZER_INCREASE_DEST
}
#elif GDI_MAINLCD_BIT_PER_PIXEL == 16
	#define gdi_non_resizer_put_mainlcd 	gdi_non_resizer_put_16
#elif GDI_MAINLCD_BIT_PER_PIXEL == 24
	#define gdi_non_resizer_put_mainlcd 	gdi_non_resizer_put_24
#elif GDI_MAINLCD_BIT_PER_PIXEL == 32
	#define gdi_non_resizer_put_mainlcd 	gdi_non_resizer_put_32
#else
	#error "Unknown MAINLCD_BIT_PER_PIXEL"
#endif

#ifdef __MMI_SUBLCD__
#if GDI_SUBLCD_BIT_PER_PIXEL ==  1 || !defined(GDI_USING_LAYER)
void gdi_non_resizer_put_sublcd(gdi_color c,BOOL want_draw)
{
	#define GDI_RESIZER_PUT_PIXEL(COLOR)			DRV_SUBLCD_SET_BUFFER_PIXEL((GDI_RESIZER.offset_dx + GDI_RESIZER.want_sx),(GDI_RESIZER.offset_dy + GDI_RESIZER.want_sy),COLOR)
	#define GDI_RESIZER_INCREASE_DEST				
	GDI_NON_RESIZER_PUT_X(c,want_draw);
	#undef GDI_RESIZER_PUT_PIXEL
	#undef GDI_RESIZER_INCREASE_DEST
}
#elif GDI_MAINLCD_BIT_PER_PIXEL == 16
	#define gdi_non_resizer_put_sublcd 	gdi_non_resizer_put_16
#elif GDI_MAINLCD_BIT_PER_PIXEL == 24
	#define gdi_non_resizer_put_sublcd 	gdi_non_resizer_put_24
#elif GDI_MAINLCD_BIT_PER_PIXEL == 32
	#define gdi_non_resizer_put_sublcd 	gdi_non_resizer_put_32
#else
	#error "Unknown MAINLCD_BIT_PER_PIXEL"
#endif
#endif


void gdi_resizer_set_want_sy(S32 want_sy)
{
	S32 dy;
	// sometime we need to stop auto increase dy, and manual set the position
	GDI_RESIZER.dir_y = 0; 
	// default : the pixel line (want_sy) will not be accept !!
	GDI_RESIZER.want_sy = want_sy+1; 

	// non resize handler
	if(GDI_RESIZER.want_sx_table==NULL)
	{
		if ((want_sy) >= GDI_RESIZER.want_start_sy && (want_sy) <= GDI_RESIZER.want_end_sy)
		{
			GDI_RESIZER.want_sx = GDI_RESIZER.want_start_sx;
			GDI_RESIZER.want_sy = want_sy;
			 
			//GDI_RESIZER.dest = gdi_act_layer->buf_ptr + ((GDI_RESIZER.want_sx+GDI_RESIZER.want_sy*gdi_act_layer->width* bitsperpixels[gdi_act_layer->vcf])>>3);
			GDI_RESIZER.dest = gdi_act_layer->buf_ptr 
								+ (((GDI_RESIZER.want_dx1
								+(GDI_RESIZER.want_dy1+GDI_RESIZER.want_sy-GDI_RESIZER.want_start_sy)*gdi_act_layer->width)
								* bitsperpixels[gdi_act_layer->vcf])>>3);
		}
		return ;
	}
	
	
	for(;;)
	{
		// want_sy project to dy
		dy = ((want_sy * GDI_RESIZER.dest_height_range << 1) +  GDI_RESIZER.src_height_range) / (GDI_RESIZER.src_height_range << 1);
		
		//check this want_dy will also project to want_sy
		if( want_sy ==  ((((dy) * GDI_RESIZER.src_height_range << 1) +  GDI_RESIZER.dest_height_range) / (GDI_RESIZER.dest_height_range << 1)))
			break;

		dy ++;
		// check dy + 1 will project to want_sy
		if( want_sy == ((((dy) * GDI_RESIZER.src_height_range << 1) +  GDI_RESIZER.dest_height_range) / (GDI_RESIZER.dest_height_range << 1)))
			break;

		
		return;// this line will not be accept !!
	}

	dy += GDI_RESIZER.offset_dy;

	// check this destination is out of clipping area ?
	if(dy < GDI_RESIZER.want_dy1 || dy > GDI_RESIZER.want_dy2)
		return;// this line will not be accept !!

	GDI_RESIZER.want_sy = want_sy;
	GDI_RESIZER.next_want_dy = dy;
	GDI_RESIZER.next_want_sx = GDI_RESIZER.want_sx_table;
	GDI_RESIZER.want_sx = *GDI_RESIZER.next_want_sx++;
	
	if(GDI_RESIZER.dir_x>0)
		GDI_RESIZER.dest = gdi_act_layer->buf_ptr 
							+ ((	(	GDI_RESIZER.want_dx1
									+GDI_RESIZER.next_want_dy* gdi_act_layer->width )
									* bitsperpixels[gdi_act_layer->vcf])>>3);
	else
		GDI_RESIZER.dest = gdi_act_layer->buf_ptr 
							+ ((		(GDI_RESIZER.want_dx2
									+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )
									* bitsperpixels[gdi_act_layer->vcf])>>3);
	
}
#if 0
#if defined(CSD_TARGET)
#pragma arm section code
#endif
#endif

const static gdi_resizer_put_func gdi_resizer_put_table[]=
{
	(gdi_resizer_put_func)gd_null_pointer_function, // 1
	gdi_resizer_put_8, // 8
	gdi_resizer_put_16, // 16
	gdi_resizer_put_24, // 24
	gdi_resizer_put_32, // 32
	gdi_resizer_put_mainlcd, // main
#ifdef __MMI_SUBLCD__
	gdi_resizer_put_sublcd
#else
	(gdi_resizer_put_func)gd_null_pointer_function, // sub
#endif
};
const static gdi_resizer_put_func gdi_non_resizer_put_table[]=
{
	(gdi_resizer_put_func)gd_null_pointer_function, // 1
	gdi_non_resizer_put_8, // 8
	gdi_non_resizer_put_16, // 16
	gdi_non_resizer_put_24, // 24
	gdi_non_resizer_put_32, // 32
	gdi_non_resizer_put_mainlcd, // main
#ifdef __MMI_SUBLCD__
	gdi_non_resizer_put_sublcd // sub
#else
	(gdi_resizer_put_func)gd_null_pointer_function // sub
#endif
};
BOOL gdi_resizer_init(
				S32 src_width,S32 src_height, 
				S32 src_clipx1, S32 src_clipy1,S32 src_clipx2,S32 src_clipy2,
				S32 dest_x1,S32 dest_y1,S32 dest_x2,S32 dest_y2)
{
	S32 dest_width;
	S32 dest_height;
	S32 dir_x;
	S32 dir_y;
	//S32 multiply_factor;
	S32 src_width_range;
	S32 src_height_range;
	S32 dest_width_range;
	S32 dest_height_range;
	
	GDI_RESIZER.want_sx = GDI_RESIZER.want_sy = -1;
	
	// we don't support 1, 8 bites color format in this resizer
	if(src_width  <=0) return FALSE;
	if(src_height <=0) return FALSE;

	dir_x=dir_y=1;

	if(dest_x1>dest_x2)
	{
		GDI_DEBUG_ASSERT(0); // current, we only support dir_x >0
		SWAP(S32,dest_x1,dest_x2);
		dir_x = -1;
	}
	if(dest_y1>dest_y2)
	{
		SWAP(S32,dest_y1,dest_y2);
		dir_y = -1;
	}
	

	GDI_RESIZER.dest = gdi_act_layer->buf_ptr;
	GDI_RESIZER.offset_dx = dest_x1;
	GDI_RESIZER.offset_dy = dest_y1;
	dest_width = dest_x2-dest_x1+1;
	dest_height = dest_y2-dest_y1+1;
	
	
	GDI_RESIZER.want_dx1 = dest_x1;
	GDI_RESIZER.want_dy1 = dest_y1;
	GDI_RESIZER.want_dx2 = dest_x2;
	GDI_RESIZER.want_dy2 = dest_y2;

	GDI_RESIZER.dir_x = dir_x;
	GDI_RESIZER.dir_y = dir_y;
	
	// check the clipping area
	GDI_CLIP_RECT_TEST(GDI_RESIZER.want_dx1,GDI_RESIZER.want_dy1,GDI_RESIZER.want_dx2,GDI_RESIZER.want_dy2,return FALSE); // no pixel will be render !!

	 
	/* Use round off to compute want_sx and want_sy */
	if (1 == src_width)
	{
		src_width = 2;
	}
	if (1 == dest_width)
	{
		dest_width = 2;
	}
	if (1 == src_height)
	{
		src_height = 2;
	}
	if (1 == dest_height)
	{
		dest_height = 2;
	}
	src_width_range = src_width - 1;
	src_height_range = src_height - 1;
	dest_width_range  = dest_width - 1;
	dest_height_range  = dest_height - 1;
	//multiply_factor = ((src_width_range) <<16)/ (dest_width_range);
	// make y_scale_factor
	//GDI_RESIZER.y_scale_factor 		= ((src_height_range)<<16)  / (dest_height_range);
	//GDI_RESIZER.y_inv_scale_factory	= ((dest_height_range)<<16 )  / (src_height_range);
	GDI_RESIZER.src_height_range 		= src_height_range;
	GDI_RESIZER.dest_height_range	= dest_height_range;

	{
		int dx1,dy1,dx2,dy2;
		// check the source clipping area
		dx1 = dest_x1 + (src_clipx1*(dest_width_range<<1) + src_width_range)/(src_width_range<<1);
		dx2 = dest_x1 + (src_clipx2*(dest_width_range<<1) + src_width_range)/(src_width_range<<1);		
		
		dy1 = dest_y1 + (src_clipy1*(dest_height_range<<1) + src_height_range)/(src_height_range<<1);
		dy2 = dest_y1 + (src_clipy2*(dest_height_range)<<1) + src_height_range/(src_height_range<<1);		

		GDI_CLIP_TWO_RECT(
			GDI_RESIZER.want_dx1, GDI_RESIZER.want_dy1,
			GDI_RESIZER.want_dx2, GDI_RESIZER.want_dy2,
			dx1,dy1,
			dx2,dy2,
			return FALSE);

		 
		while ( (((GDI_RESIZER.want_dx1 - GDI_RESIZER.offset_dx) * src_width_range << 1) + dest_width_range) / (dest_width_range << 1) < src_clipx1 )
			GDI_RESIZER.want_dx1++;

		while((((GDI_RESIZER.want_dy1 - GDI_RESIZER.offset_dy) * src_height_range << 1) + dest_height_range) / (dest_height_range << 1) < src_clipy1)
			GDI_RESIZER.want_dy1++;

		while ( (((GDI_RESIZER.want_dx2 - GDI_RESIZER.offset_dx) * src_width_range << 1) + dest_width_range) / (dest_width_range << 1) > src_clipx2 )
			GDI_RESIZER.want_dx2--;

		while((((GDI_RESIZER.want_dy2 - GDI_RESIZER.offset_dy) * src_height_range << 1) + dest_height_range) / (dest_height_range << 1)> src_clipy2)
			GDI_RESIZER.want_dy2--;
	}


	// non resize handler
	if(src_width == dest_width && src_height == dest_height)
	{
		U32 bytes_per_pixel = bitsperpixels[gdi_act_layer->vcf]>>3;
		GDI_RESIZER.want_sx_table = NULL;
		GDI_RESIZER.dest = gdi_act_layer->buf_ptr;
		if(dir_x >0)
		{
			GDI_RESIZER.want_sx 		=
			GDI_RESIZER.want_start_sx = GDI_RESIZER.want_dx1 - dest_x1;
			GDI_RESIZER.want_end_sx 	= GDI_RESIZER.want_dx2 - dest_x1;
			
			GDI_RESIZER.dest 		+= GDI_RESIZER.want_dx1*bytes_per_pixel;
		}
		else
		{
			GDI_RESIZER.want_sx 		=
			GDI_RESIZER.want_start_sx = GDI_RESIZER.want_dx2 - dest_x1;
			GDI_RESIZER.want_end_sx  = GDI_RESIZER.want_dx1 - dest_x1;
			GDI_RESIZER.dest 		+= GDI_RESIZER.want_dx2*bytes_per_pixel;			
		}
		if(dir_y >0)
		{
			GDI_RESIZER.want_sy 		=
			GDI_RESIZER.want_start_sy = GDI_RESIZER.want_dy1 - dest_y1;
			GDI_RESIZER.want_end_sy 	= GDI_RESIZER.want_dy2 - dest_y1;
			
			GDI_RESIZER.dest 		+= GDI_RESIZER.want_dy1*gdi_act_layer->width*bytes_per_pixel;
			GDI_RESIZER.dest_pitch_jump = (gdi_act_layer->width - (GDI_RESIZER.want_dx2-GDI_RESIZER.want_dx1+1)) * bytes_per_pixel; ///TODO: only support dir_x>0
		}
		else
		{
			GDI_RESIZER.want_sy 		=
			GDI_RESIZER.want_start_sy = GDI_RESIZER.want_dy2 - dest_y1;
			GDI_RESIZER.want_end_sy  = GDI_RESIZER.want_dy1 - dest_y1;
			
			GDI_RESIZER.dest 		+= GDI_RESIZER.want_dy2*gdi_act_layer->width*bytes_per_pixel;
			GDI_RESIZER.dest_pitch_jump = -(gdi_act_layer->width + (GDI_RESIZER.want_dx2-GDI_RESIZER.want_dx1+1)) * bytes_per_pixel; ///TODO: only support dir_x>0
		}
		gdi_resizer_put = gdi_non_resizer_put_table[gdi_act_layer->cf];

		return TRUE;		
	}


	// assign want_sx_table memory space
	GDI_DEBUG_ASSERT( GDI_RESIZER.want_dx2-GDI_RESIZER.want_dx1+1<= GDI_NEED_RESIZER_BUFFER);
	GDI_RESIZER.want_sx_table = gdi_resizer_buffer;

	// make want_x table
	{
		S32 x;
		S32 i;
		S32 start_dx,end_dx;

		if(dir_x>0)
		{
			start_dx = GDI_RESIZER.want_dx1;
			end_dx = GDI_RESIZER.want_dx2;
		}
		else
		{
			start_dx = GDI_RESIZER.want_dx2;
			end_dx = GDI_RESIZER.want_dx1;
		}
			
		for(i=0,x=start_dx;x<=end_dx;x+=dir_x,i++)
		{
			S32 d = x - dest_x1;
			GDI_RESIZER.want_sx_table[i] = (S16)((((d) * src_width_range << 1) + dest_width_range) / (dest_width_range << 1));
		}
		GDI_RESIZER.want_sx_table_end = GDI_RESIZER.want_sx_table+i;
		GDI_RESIZER.next_want_sx = GDI_RESIZER.want_sx_table;
	}

	if(dir_y>0)
		GDI_RESIZER.next_want_dy = GDI_RESIZER.want_dy1;
	else
		GDI_RESIZER.next_want_dy = GDI_RESIZER.want_dy2;

	// set the put pixel function pointer
	gdi_resizer_put = gdi_resizer_put_table[gdi_act_layer->cf];
	// init want position
	if(dir_x>0)
		GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (((GDI_RESIZER.want_dx1+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )* bitsperpixels[gdi_act_layer->vcf])>>3);
	else
		GDI_RESIZER.dest = gdi_act_layer->buf_ptr + (((GDI_RESIZER.want_dx2+ GDI_RESIZER.next_want_dy* gdi_act_layer->width )* bitsperpixels[gdi_act_layer->vcf])>>3);
	
	GDI_RESIZER.want_sx = *GDI_RESIZER.next_want_sx++;
	GDI_RESIZER.want_sy = ((((GDI_RESIZER.next_want_dy  - GDI_RESIZER.offset_dy) * src_height_range << 1) +  dest_height_range) / (dest_height_range << 1));
	GDI_RESIZER.next_want_dy +=dir_y;
	return TRUE;
}
/*****************************************************************************
 * FUNCTION
 *  gdi_fill_dot_rect
 * DESCRIPTION
 *  
 * PARAMETERS
 * RETURNS
 *  
 *****************************************************************************/
void gdi_fill_dot_rect(S32 x1,S32 y1,S32 x2,S32 y2,gdi_color c)
{
    GDI_ENTER_CRITICAL_SECTION(gdi_layer_fill_dot_rect)
    {
        gd_fill_dot_rect[gdi_act_layer->vcf](
                                                gdi_act_layer->buf_ptr,
                                                gdi_act_layer->width,
                                                gdi_act_layer->height,
                                                x1,y1,
                                                x2,y2,
                                                gdi_act_layer->clipx1,
                                                gdi_act_layer->clipy1,
                                                gdi_act_layer->clipx2,
                                                gdi_act_layer->clipy2,
                                                c);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_layer_fill_dot_rect)
}

void gdi_bitblt_internal(U8* src_ptr, U32 src_pitch, 
			S32 src_offset_x, S32 src_offset_y, 
			U32 src_width, U32 src_height,
			gdi_color_format src_cf,
			U8* dest_ptr, U32 dest_pitch,
			S32 dest_offset_x, S32 dest_offset_y,
			S32 dest_clip_x1, S32 dest_clip_y1, S32 dest_clip_x2, S32 dest_clip_y2,
			gdi_color_format dest_cf, U32 dest_buf_size,
			BOOL tilt,
			BOOL enable_src_key, gdi_color src_key,
			BOOL is_alpha_blending, U8 alpha_value,
			BOOL is_rop_mode, U8 rop_value,
			U8 transform_value,
			U8 transform_direction)
{
#if defined(GDI_USING_2D_ENGINE_V2)
	gdi_2d_buffer_struct src;
	gdi_2d_begin();
    if (GDI_TRANSFORM_DIRECTION_RB == transform_direction)
    {
	    gdi_2d_set_buffer(&src,src_ptr,src_pitch,
		    		src_width,src_height,
			    	src_offset_x + src_width - 1, src_offset_y + src_height - 1,
				    src_offset_x,src_offset_y,
				    src_offset_x + src_width - 1, src_offset_y + src_height - 1,
				    src_cf);
	    gdi_2d_set_dest_buffer(dest_ptr, dest_pitch,
		    		src_width, src_height,
			    	dest_offset_x + src_width - 1, dest_offset_y + src_height - 1,
				    dest_clip_x1, dest_clip_y1,
				    dest_clip_x2, dest_clip_y2,
				    dest_cf);
    }
    else
    {
	    gdi_2d_set_buffer(&src,src_ptr,src_pitch,
		    		src_width,src_height,
			    	src_offset_x,src_offset_y,
				    src_offset_x,src_offset_y,
				    src_offset_x + src_width - 1, src_offset_y + src_height - 1,
				    src_cf);
	    gdi_2d_set_dest_buffer(dest_ptr, dest_pitch,
		    		src_width, src_height,
			    	dest_offset_x, dest_offset_y,
				    dest_clip_x1, dest_clip_y1,
				    dest_clip_x2, dest_clip_y2,
				    dest_cf);
    }
	gdi_2d_bitblt(tilt,
			enable_src_key, src_key,
			is_alpha_blending, alpha_value,
			is_rop_mode, rop_value,
			transform_value,
			transform_direction,
			&src, 1);
	gdi_2d_end();
#endif
}

void gdi_bits_draw(S32 x,S32 y,U8* src,S32 src_size,S32 w,S32 h,S32 bits_per_pixel,gdi_color palette[])
{
    GDI_ENTER_CRITICAL_SECTION(gdi_image_bits_draw)
    {
        gd_image_bits_draw[gdi_act_layer->vcf](
                                                gdi_act_layer->buf_ptr,
                                                gdi_act_layer->width,
                                                gdi_act_layer->height,
                                                x,y,
                                                x+w-1,y+h-1,
                                                src,w,h,
                                                0,0,w-1,h-1,
                                                src_size,bits_per_pixel,palette,
                                                gdi_act_layer->clipx1,
                                                gdi_act_layer->clipy1,
                                                gdi_act_layer->clipx2,
                                                gdi_act_layer->clipy2);
    }
    GDI_EXIT_CRITICAL_SECTION(gdi_image_bits_draw)
}

