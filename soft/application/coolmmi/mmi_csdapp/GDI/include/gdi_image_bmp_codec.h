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
 *  gdi_font_show_char.h
 * Project:
 * --------
 
 * Description:
 * ------------
 *  GDI BMP codec macro
 * Author:
 * -------
 * -------
 *==============================================================================
 *             HISTORY
 
 *------------------------------------------------------------------------------
 *------------------------------------------------------------------------------
 
 *==============================================================================
 *******************************************************************************/
#undef GDI_RESIZER_PUT
#undef GDI_RESIZER_PUT_PIXEL
#undef GDI_RESIZER_INCREASE_DEST

#ifndef GDI_IMAGE_BMP_USING_RESIZER
#if GDI_IMAGE_BMP_CODEC_COLOR_FORMAT == GDI_COLOR_FORMAT_8
#define GDI_RESIZER_PUT_PIXEL(COLOR)         *((U8*)GDI_RESIZER.dest) = (U8)COLOR
#define GDI_RESIZER_INCREASE_DEST            GDI_RESIZER.dest ++
#define GDI_RESIZER_PUT(COLOR,WANT_DRAW)     GDI_NON_RESIZER_PUT_X(COLOR,WANT_DRAW)
#elif GDI_IMAGE_BMP_CODEC_COLOR_FORMAT==GDI_COLOR_FORMAT_16
#define GDI_RESIZER_PUT_PIXEL(COLOR)         *((U16*)GDI_RESIZER.dest) = (U16)COLOR
#define GDI_RESIZER_INCREASE_DEST            GDI_RESIZER.dest +=2
#define GDI_RESIZER_PUT(COLOR,WANT_DRAW)     GDI_NON_RESIZER_PUT_X(COLOR,WANT_DRAW)
#elif GDI_IMAGE_BMP_CODEC_COLOR_FORMAT==GDI_COLOR_FORMAT_24
#define GDI_RESIZER_PUT_PIXEL(COLOR)                       \
         do                                                      \
         {                                                       \
            U32 *ptr = (U32*)(GDI_RESIZER.dest);                 \
            switch(((U32)ptr)&0x3)                               \
            {                                                    \
            case 0:                                              \
               *ptr = (*ptr&0xff000000)|(COLOR&0x00ffffff);      \
               break;                                            \
            case 1:                                              \
               ptr= (U32*)(((U32)ptr)&(~0x3));                   \
               *ptr = (*ptr&0x000000ff)|(COLOR<<8);              \
               break;                                            \
            case 2:                                              \
               ptr= (U32*)(((U32)ptr)&(~0x3));                   \
               *ptr = (*ptr&0x0000ffff)|(COLOR<<16); ptr++;      \
               *ptr = (*ptr&0xffffff00)|((COLOR&0x00ff0000)>>16);\
               break;                                            \
            default:                                             \
               ptr= (U32*)(((U32)ptr)&(~0x3));                   \
               *ptr = (*ptr&0x00ffffff)|(COLOR<<24); ptr++;      \
               *ptr = (*ptr&0xffff0000)|((COLOR&0x00ffff00)>>8); \
            }                                                    \
         }while(0)

#define GDI_RESIZER_INCREASE_DEST            GDI_RESIZER.dest +=3
#define GDI_RESIZER_PUT(COLOR,WANT_DRAW)     GDI_NON_RESIZER_PUT_X(COLOR,WANT_DRAW)
#elif GDI_IMAGE_BMP_CODEC_COLOR_FORMAT==GDI_COLOR_FORMAT_32
#define GDI_RESIZER_PUT_PIXEL(COLOR)         *((U32*)GDI_RESIZER.dest) = (U32)COLOR
#define GDI_RESIZER_INCREASE_DEST            GDI_RESIZER.dest +=4
#define GDI_RESIZER_PUT(COLOR,WANT_DRAW)     GDI_NON_RESIZER_PUT_X(COLOR,WANT_DRAW)
#elif GDI_IMAGE_BMP_CODEC_COLOR_FORMAT==GDI_COLOR_FORMAT_MAINLCD
#define GDI_RESIZER_PUT_PIXEL(COLOR)         DRV_MAINLCD_SET_BUFFER_PIXEL((GDI_RESIZER.offset_dx + GDI_RESIZER.want_sx),(GDI_RESIZER.offset_dy + GDI_RESIZER.want_sy),COLOR)
#define GDI_RESIZER_INCREASE_DEST
#define GDI_RESIZER_PUT(COLOR,WANT_DRAW)     GDI_NON_RESIZER_PUT_X(COLOR,WANT_DRAW)
#elif GDI_IMAGE_BMP_CODEC_COLOR_FORMAT==GDI_COLOR_FORMAT_SUBLCD
#define GDI_RESIZER_PUT_PIXEL(COLOR)         DRV_SUBLCD_SET_BUFFER_PIXEL((GDI_RESIZER.offset_dx + GDI_RESIZER.want_sx),(GDI_RESIZER.offset_dy + GDI_RESIZER.want_sy),COLOR)
#define GDI_RESIZER_INCREASE_DEST
#define GDI_RESIZER_PUT(COLOR,WANT_DRAW)     GDI_NON_RESIZER_PUT_X(COLOR,WANT_DRAW)
#else 
#error "WE didn't support this color_t format "
#endif 

#else /* GDI_IMAGE_BMP_USING_RESIZER */ 
#define GDI_RESIZER_PUT(COLOR,WANT_DRAW)     gdi_resizer_put(COLOR,WANT_DRAW)
#endif /* GDI_IMAGE_BMP_USING_RESIZER */ 


/*****************************************************************************
 * FUNCTION
 *  GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL
 * DESCRIPTION
 *  get dimension of a bmp from memory
 * PARAMETERS
 *  BMP_src     [IN]        
 *  width       [OUT]       
 *  height      [OUT]       
 * RETURNS
 *  GDI_RESULT
 *****************************************************************************/
#ifdef GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL
GDI_RESULT GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL(U8 *BMP_src, S32 *width, S32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(gdi_image_bmp_get_dimension) U16 bfType;
    U32 bfSize;
    U32 biWidth;
    U32 biHeight;
    U32 biSize;

    INIT_SRC(src, BMP_src);

    GDI_DEBUG_ASSERT(BMP_src != NULL);
    COMPRESSION_MODE(0, 0, 0);  /* reset the compression mode */

    /* read file header */
    GET_word(src, bfType) GET_dword(src, bfSize) FLUSH(src, 8);
    /* read info header */
    GET_dword(src, biSize);
    if (biSize == 12)
    {
    GET_word(src, biWidth) GET_word(src, biHeight)}
    else
    {
    GET_dword(src, biWidth) GET_dword(src, biHeight)}

    *width = biWidth;
    *height = biHeight;

    /* test if it is BMP file */
    if (bfType != 0x4d42)   /* BM */
    {
        GDI_RETURN(GDI_FAILED);
    }
    /* core info header, currently not support */
    if (bfSize == 26)
    {
        GDI_RETURN(GDI_FAILED);
    }

    GDI_RETURN(GDI_SUCCEED);
GDI_EXIT_CRITICAL_SECTION(gdi_image_bmp_get_dimension)}
#endif /* GDI_IMAGE_BMP_GET_DIMENSION_INTERNAL */ 

GDI_RESULT GDI_IMAGE_BMP_DRAW_INTERNAL(


/*****************************************************************************
 * FUNCTION
 *  GDI_IMAGE_BMP_DRAW_INTERNAL
 * DESCRIPTION
 *  
 * PARAMETERS
 *  offset_x            [IN]        
 *  offset_y            [IN]        
 *  is_resized          [IN]        
 *  resized_width       [IN]        
 *  resized_height      [IN]        
 *  BMP_src             [?]         
 *  size                [IN]        
 * RETURNS
 *  
 *****************************************************************************/
            S32 offset_x,
            S32 offset_y,
            BOOL is_resized,
            S32 resized_width,
            S32 resized_height,
            U8 *BMP_src,
            U32 size)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    GDI_ENTER_CRITICAL_SECTION(GDI_IMAGE_BMP_DRAW_INTERNAL) bitmap_file_header bmp_fileheader;
    bitmap_info_header bmp_infoheader;
    U32 paletteSize;
    U8 r = 0, g = 0, b = 0, a = 0;
    S32 i, j;
    U32 U8PerLine;
    S32 dataCount;
    U32 dummyCount;
    S32 pos_x = 0, pos_y = 0;
    U8 tmp_byte = 0;
    gdi_color palette[256];
    gdi_color replace_source_key = 0;
    int jump = 0;
    U32 c = 0;

#ifdef SUPPORT_BMP_COMPRESSION
    S32 r_mask = 0;
    S32 g_mask = 0;
    S32 b_mask = 0;
    S32 r_shift = 0;
    S32 g_shift = 0;
    S32 b_shift = 0;
    S32 r_bits = 0;
    S32 g_bits = 0;
    S32 b_bits = 0;
#endif /* SUPPORT_BMP_COMPRESSION */ 
    INIT_SRC(src, BMP_src);

    GDI_DEBUG_ASSERT(BMP_src != NULL);
    COMPRESSION_MODE(0, 0, 0);  /* reset the compression mode */

    /* read file header */
    GET_word(src, bmp_fileheader.bfType)
        GET_dword(src, bmp_fileheader.bfSize)
        GET_word(src, bmp_fileheader.bfReserved1)
        GET_word(src, bmp_fileheader.bfReserved2) GET_dword(src, bmp_fileheader.bfOffBits)
        /* test if it is BMP file */
        if (bmp_fileheader.bfType != 0x4d42)    /* BM */
        {
        GDI_RETURN(GDI_FAILED);
        }

    #if IS_FROM_FILE == 0
    /* size not correct */
    if (bmp_fileheader.bfSize != size)
    {
        GDI_RETURN(GDI_FAILED);
    }
    #endif /* IS_FROM_FILE == 0 */ 

    /* core info header, currently not support */
    if (bmp_fileheader.bfSize == 26)
    {
        GDI_RETURN(GDI_FAILED);
    }

    /* read info header */
    GET_dword(src, bmp_infoheader.biSize) if (bmp_infoheader.biSize == 12)        /* OS/2 bmp format */
    {
        GET_word(src, bmp_infoheader.biWidth)
            GET_word(src, bmp_infoheader.biHeight)
            GET_word(src, bmp_infoheader.biPlanes)
            GET_word(src, bmp_infoheader.biBitCount) bmp_infoheader.biCompression = GDI_BI_RGB;
        bmp_infoheader.biSizeImage = 0;
        bmp_infoheader.biXPelsPerMeter = 0;
        bmp_infoheader.biYPelsPerMeter = 0;
        bmp_infoheader.biClrUsed = 0;
        bmp_infoheader.biClrImportant = 0;

    }
    else    /* microsoft windows bmp format */
    {
        if (bmp_infoheader.biSize < 40)
        {
            GDI_RETURN(GDI_FAILED);
        }

    GET_dword(src, bmp_infoheader.biWidth)
            GET_dword(src, bmp_infoheader.biHeight)
            GET_word(src, bmp_infoheader.biPlanes)
            GET_word(src, bmp_infoheader.biBitCount)
            GET_dword(src, bmp_infoheader.biCompression)
            GET_dword(src, bmp_infoheader.biSizeImage)
            GET_dword(src, bmp_infoheader.biXPelsPerMeter)
            GET_dword(src, bmp_infoheader.biYPelsPerMeter)
            GET_dword(src, bmp_infoheader.biClrUsed) GET_dword(src, bmp_infoheader.biClrImportant)}

    if (bmp_infoheader.biWidth == 0 || bmp_infoheader.biHeight == 0 || (bmp_infoheader.biCompression != GDI_BI_RGB
        #ifdef SUPPORT_BMP_COMPRESSION
                                                                        && bmp_infoheader.biCompression != GDI_BI_RLE8
                                                                        && bmp_infoheader.biCompression != GDI_BI_RLE4
                                                                        && bmp_infoheader.biCompression !=
                                                                        GDI_BI_BITFIELDS
#else 
#ifdef SUPPORT_BMP_FORMAT_565
		    &&bmp_infoheader.biCompression !=GDI_BI_BITFIELDS
#endif
                                                                        
        #endif /* SUPPORT_BMP_COMPRESSION */ 
        ))
        GDI_RETURN(GDI_FAILED);

    if ((bmp_infoheader.biWidth > 2048) || (bmp_infoheader.biHeight > 2048))
    {
        GDI_RETURN(GDI_FAILED); /* GDI_BMP_ERR_FILE_TOO_LARGE */
    }

    /* setup resizer */
     
    if (!is_resized || resized_width > (S32) bmp_infoheader.biWidth || resized_height > (S32) bmp_infoheader.biHeight)
    {
        resized_width = bmp_infoheader.biWidth;
        resized_height = bmp_infoheader.biHeight;
    }

    if (resized_width == 0 || resized_height == 0)
    {
        return (GDI_SUCCEED);
    }

    if (!gdi_resizer_init(
            bmp_infoheader.biWidth,
            bmp_infoheader.biHeight,
            0,
            0,
            bmp_infoheader.biWidth - 1,
            bmp_infoheader.biHeight - 1,
            offset_x,
            offset_y + resized_height - 1,
            offset_x + resized_width - 1,
            offset_y))
    {
        GDI_RETURN(GDI_SUCCEED);
    }

    if (gdi_act_layer_info->source_key_enable)
    {
        replace_source_key = gdi_act_layer_info->source_key ^ 1;
    }
    /* setup palette */
    if (bmp_infoheader.biBitCount <= 8)
    {
        gd_color_from_rgb_func palette_color_from_rgb;

        paletteSize = 1 << (bmp_infoheader.biBitCount);
        if (bmp_infoheader.biClrUsed != 0 && bmp_infoheader.biClrUsed < paletteSize)
        {
            paletteSize = bmp_infoheader.biClrUsed;
        }

        if (gdi_act_layer->vcf == GDI_COLOR_FORMAT_8)
        {
            palette_color_from_rgb = GDI_PALETTE_COLOR_FROM_RGB;
        }
        else
        {
            palette_color_from_rgb = gdi_act_color_from_rgb;
        }

        /* read palette */
        for (i = 0; i < (S32) paletteSize; i++)
        {
            GET_byte(src, b);
            GET_byte(src, g);
            GET_byte(src, r);
            GET_byte(src, a);

            palette[i] = palette_color_from_rgb(255, r, g, b);

            if (palette[i] == gdi_act_layer_info->source_key && gdi_act_layer_info->source_key_enable)
            {
                palette[i] ^= 1;
            }
        }

        /* current layer is index color layer */
        if (gdi_act_layer->vcf == GDI_COLOR_FORMAT_8)
            for (i = 0; i < (S32) paletteSize; i++)
            {
                gdi_layer_set_palette((U8) i, palette[i]);
                palette[i] = i;
            }
    }

#ifdef SUPPORT_BMP_COMPRESSION
    if (bmp_infoheader.biCompression == GDI_BI_BITFIELDS &&
        (bmp_infoheader.biBitCount == 16 || bmp_infoheader.biBitCount == 32))
    {
        U32 tmp;

        GET_dword(src, r_mask);
        GET_dword(src, g_mask);
        GET_dword(src, b_mask);

        tmp = r_mask;
        for (r_shift = 0;; r_shift++, tmp >>= 1)
            if (tmp & 1)
            {
                break;
            }
        for (r_bits = 0;; r_bits++, tmp >>= 1)
            if (!(tmp & 1))
            {
                break;
            }
        r_bits = 8 - r_bits;

        tmp = g_mask;
        for (g_shift = 0;; g_shift++, tmp >>= 1)
            if (tmp & 1)
            {
                break;
            }
        for (g_bits = 0;; g_bits++, tmp >>= 1)
            if (!(tmp & 1))
            {
                break;
            }
        g_bits = 8 - g_bits;

        tmp = b_mask;
        for (b_shift = 0;; b_shift++, tmp >>= 1)
            if (tmp & 1)
            {
                break;
            }
        for (b_bits = 0;; b_bits++, tmp >>= 1)
            if (!(tmp & 1))
            {
                break;
            }
        b_bits = 8 - b_bits;
    }
#endif /* SUPPORT_BMP_COMPRESSION */ 
    /* calculate U8PerLine, for latter to skip stuffing U8 */
    U8PerLine = 4 * ((bmp_infoheader.biWidth * bmp_infoheader.biBitCount + 31) / 32);

    if (bmp_infoheader.biSizeImage == 0)
    {
        bmp_infoheader.biSizeImage = U8PerLine * bmp_infoheader.biHeight;
        dataCount = (S32) bmp_infoheader.biSizeImage;
    }
    else
    {
        dataCount = (S32) U8PerLine *bmp_infoheader.biHeight;
    }

    /* seek to start of draw data */
    SEEK(src, BMP_src, bmp_fileheader.bfOffBits);

    pos_x = offset_x;
    pos_y = offset_y + bmp_infoheader.biHeight - 1;
    /* 8 bit color, use palette */

    COMPRESSION_MODE(bmp_infoheader.biCompression, bmp_infoheader.biWidth, bmp_infoheader.biHeight);

    switch (bmp_infoheader.biBitCount)
    {
        case 8:
            jump = 0;
            if (bmp_infoheader.biCompression == GDI_BI_RLE8 || bmp_infoheader.biCompression == GDI_BI_RLE4)
            {
                U8PerLine = bmp_infoheader.biWidth;
            }

            dummyCount = U8PerLine - bmp_infoheader.biWidth;
            for (pos_y = bmp_infoheader.biHeight - 1; pos_y >= 0; pos_y--)
            {
                for (pos_x = 0; pos_x < (S32) (S32) bmp_infoheader.biWidth; pos_x++)
                {
                    if (IS_GDI_RESIZER_WANT_DRAW(pos_x, pos_y))
                    {
                        if (jump)
                        {
                            FLUSH(src, jump);
                            jump = 0;
                        }
                        GET_byte(src, c);
                    #if IS_FROM_FILE
                        GDI_RESIZER_PUT(palette[c], TRUE);
                    #else 
                        GDI_RESIZER_PUT(palette[c], (BOOL) (c != 0));
                    #endif 
                    }
                    else
                    {
                        jump++;
                    }
                }
                jump += dummyCount;
            }
            break;
        case 16:
            jump = 0;
            U8PerLine = 2 * (bmp_infoheader.biWidth + bmp_infoheader.biWidth % 2);
            dummyCount = U8PerLine - bmp_infoheader.biWidth * 2;
#if 0			
            for (pos_y = bmp_infoheader.biHeight - 1; pos_y >= 0; pos_y--)
            {
                for (pos_x = 0; pos_x < (S32) bmp_infoheader.biWidth; pos_x++)
                {
                    if (IS_GDI_RESIZER_WANT_DRAW(pos_x, pos_y))
                    {
                        if (jump)
                        {
                            FLUSH(src, jump);
                            jump = 0;
                        }
                        GET_word(src, c);    /* RGB 555 */
                    #ifdef SUPPORT_BMP_COMPRESSION

                        if (bmp_infoheader.biCompression == GDI_BI_BITFIELDS)
                        {
                            b = ((c & b_mask) >> b_shift) << b_bits;
                            g = ((c & g_mask) >> g_shift) << g_bits;
                            r = ((c & r_mask) >> r_shift) << r_bits;
                            a = 0x00;
                            a = ~a;
                        }
                        else
                    #endif /* SUPPORT_BMP_COMPRESSION */ 
                        {
                            b = (c & 0x1f) << 3;
                            g = ((c >> 5) & 0x1f) << 3;
                            r = ((c >> 10) & 0x1f) << 3;
                        }
                         
                    #if IS_FROM_FILE
                        if (gdi_act_layer_info->source_key_enable &&
                            gdi_act_layer_info->source_key == gdi_act_color_from_rgb(0xFF, r, g, b))
                        {
                            GDI_RESIZER_PUT(replace_source_key, 1);
                        }
                        else
                    #endif /* IS_FROM_FILE */ 
                            GDI_RESIZER_PUT(gdi_act_color_from_rgb(0xFF, r, g, b), 1);
                    }
                    else
                    {
                        jump += 2;
                    }
                }
                jump += dummyCount;
            }
#endif			
#if 0

			for(pos_y = bmp_infoheader.biHeight-1; pos_y >= 0; pos_y--)
			{
				for(pos_x = 0 ; pos_x < (S32)bmp_infoheader.biWidth; pos_x++)
				{
					if(IS_GDI_RESIZER_WANT_DRAW(pos_x,pos_y))
					{
						if(jump){	FLUSH(src,jump);jump = 0;}
						GET_U16(src,c); // RGB 555
#ifdef SUPPORT_BMP_COMPRESSION

						if(bmp_infoheader.biCompression == GDI_BI_BITFIELDS)
						{
							b = ((c & b_mask) >> b_shift) << b_bits;
							g = ((c & g_mask) >> g_shift) << g_bits;
							r = ((c & r_mask) >> r_shift) << r_bits;
							a = 0x00;
							a=~a;
						}
						else
#endif
						{
							b = (c & 0x1f) <<3;
							g = ((c>>5) & 0x1f)<<3;
							r = ((c>>10) & 0x1f)<<3;
						}
						GDI_RESIZER_PUT(gdi_act_color_from_rgb(0xFF,r,g,b),1);
					}
					else
						jump+=2;
				}
				jump+=dummyCount;
			}
#else
#if (defined(SUPPORT_BMP_COMPRESSION) || defined(SUPPORT_BMP_FORMAT_565))

		if(bmp_infoheader.biCompression == GDI_BI_BITFIELDS)
			for(pos_y = bmp_infoheader.biHeight-1; pos_y >= 0; pos_y--)
			{
				for(pos_x = 0 ; pos_x < (S32)bmp_infoheader.biWidth; pos_x++)
				{
					if(IS_GDI_RESIZER_WANT_DRAW(pos_x,pos_y))
					{
						if(jump){	FLUSH(src,jump);jump = 0;}
						GET_word(src,c); // RGB 555
#ifdef SUPPORT_BMP_COMPRESSION
						{
							b = ((c & b_mask) >> b_shift) << b_bits;
							g = ((c & g_mask) >> g_shift) << g_bits;
							r = ((c & r_mask) >> r_shift) << r_bits;
							a = 0x00;
							a=~a;
						}
                    #if IS_FROM_FILE
                        if (gdi_act_layer_info->source_key_enable &&
                            gdi_act_layer_info->source_key == gdi_act_color_from_rgb(0xFF, r, g, b))
                        {
                            GDI_RESIZER_PUT(replace_source_key, 1);
                        }
                        else
                    #endif /* IS_FROM_FILE */ 

							GDI_RESIZER_PUT(gdi_act_color_from_rgb(0xFF,r,g,b),1);
#else
                    #if IS_FROM_FILE
                        if (gdi_act_layer_info->source_key_enable &&
                            gdi_act_layer_info->source_key == gdi_act_color_from_rgb(0xFF, r, g, b))
                        {
                            GDI_RESIZER_PUT(replace_source_key, 1);
                        }
                        else
                    #endif /* IS_FROM_FILE */ 
							GDI_RESIZER_PUT(c,1);
#endif
					}
					else
						jump+=2;
				}
				jump+=dummyCount;
			}
		else
#endif

		for(pos_y = bmp_infoheader.biHeight-1; pos_y >= 0; pos_y--)
			{
				for(pos_x = 0 ; pos_x < (S32)bmp_infoheader.biWidth; pos_x++)
				{
					if(IS_GDI_RESIZER_WANT_DRAW(pos_x,pos_y))
					{
						if(jump){	FLUSH(src,jump);jump = 0;}
						GET_word(src,c); // RGB 555
						{
							b = (c & 0x1f) <<3;
							g = ((c>>5) & 0x1f)<<3;
							r = ((c>>10) & 0x1f)<<3;
						}
                    #if IS_FROM_FILE
                        if (gdi_act_layer_info->source_key_enable &&
                            gdi_act_layer_info->source_key == gdi_act_color_from_rgb(0xFF, r, g, b))
                        {
                            GDI_RESIZER_PUT(replace_source_key, 1);
                        }
                        else
                    #endif /* IS_FROM_FILE */ 
							GDI_RESIZER_PUT(gdi_act_color_from_rgb(0xFF,r,g,b),1);
					}
					else
						jump+=2;
				}
				jump+=dummyCount;
			}
							
#endif				
			
            break;

        case 24:
            jump = 0;
            U8PerLine = 4 * ((bmp_infoheader.biWidth * 24 + 31) / 32);
            dummyCount = U8PerLine - bmp_infoheader.biWidth * 3;
            for (pos_y = bmp_infoheader.biHeight - 1; pos_y >= 0; pos_y--)
            {
                for (pos_x = 0; pos_x < (S32) bmp_infoheader.biWidth; pos_x++)
                {
                    if (IS_GDI_RESIZER_WANT_DRAW(pos_x, pos_y))
                    {
                       // U32 c;

                        if (jump)
                        {
                            FLUSH(src, jump);
                            jump = 0;
                        }
                        GET_U24(src, c);
                        b = c & 0xff;
                        g = (c >> 8) & 0xff;
                        r = (c >> 16) & 0xff;
                         
                    #if IS_FROM_FILE
                        if (gdi_act_layer_info->source_key_enable &&
                            gdi_act_layer_info->source_key == gdi_act_color_from_rgb(0xFF, r, g, b))
                        {
                            GDI_RESIZER_PUT(replace_source_key, 1);
                        }
                        else
                    #endif /* IS_FROM_FILE */ 
                            GDI_RESIZER_PUT(gdi_act_color_from_rgb(0xFF, r, g, b), 1);
                    }
                    else
                    {
                        jump += 3;
                    }
                }
                jump += dummyCount;
            }
            break;
        case 32:
            jump = 0;
            U8PerLine = 4 * (bmp_infoheader.biWidth);
            dummyCount = U8PerLine - bmp_infoheader.biWidth * 4;
            for (pos_y = bmp_infoheader.biHeight - 1; pos_y >= 0; pos_y--)
            {
                for (pos_x = 0; pos_x < (S32) bmp_infoheader.biWidth; pos_x++)
                {
                    if (IS_GDI_RESIZER_WANT_DRAW(pos_x, pos_y))
                    {
                        if (jump)
                        {
                            FLUSH(src, jump);
                            jump = 0;
                        }
                    #ifdef SUPPORT_BMP_COMPRESSION
                        if (bmp_infoheader.biCompression == GDI_BI_BITFIELDS)
                        {
                            GET_dword(src, c);
                            b = ((c & b_mask) >> b_shift) << b_bits;
                            g = ((c & g_mask) >> g_shift) << g_bits;
                            r = ((c & r_mask) >> r_shift) << r_bits;
                            a = 0x00;
                        }
                        else
                    #endif /* SUPPORT_BMP_COMPRESSION */ 
                        {
                            GET_byte(src, b);
                            GET_byte(src, g);
                            GET_byte(src, r);
                            GET_byte(src, a);
                        }

                         
                    #if IS_FROM_FILE
                        if (gdi_act_layer_info->source_key_enable &&
                            gdi_act_layer_info->source_key == gdi_act_color_from_rgb(a, r, g, b))
                        {
                            GDI_RESIZER_PUT(replace_source_key, 1);
                        }
                        else
                    #endif /* IS_FROM_FILE */ 
                            GDI_RESIZER_PUT(gdi_act_color_from_rgb(~a, r, g, b), 1);    /* use imagemagick to convert PNG to BMP, the alpha will be inverse */
                    }
                    else
                    {
                        jump += 4;
                    }
                }
                jump += dummyCount;
            }
            break;

        case 4:
            pos_y = bmp_infoheader.biHeight - 1;
            while (dataCount > 0 && pos_y >= 0)
            {
                pos_x = 0;
                for (j = 0; j < (S32) U8PerLine; j++)
                {
                  //  U32 
                  c = 0;

                    GET_byte(src, c);

                    // 4 bit per color, so read one U8 may fill two pixel 
                    // pixel 1
                    if (pos_x < (S32) bmp_infoheader.biWidth)
                    {
                        tmp_byte = c >> 4;
                        if (IS_GDI_RESIZER_WANT_DRAW(pos_x, pos_y))
                    #if IS_FROM_FILE
                            GDI_RESIZER_PUT(palette[tmp_byte], TRUE);
                    #else 
                            GDI_RESIZER_PUT(palette[tmp_byte], (BOOL) (tmp_byte != 0));
                    #endif 
                        pos_x++;

                        /* pixel 2 */
                        tmp_byte = c & 0xf;
                        if (pos_x < (S32) bmp_infoheader.biWidth)
                            if (IS_GDI_RESIZER_WANT_DRAW(pos_x, pos_y))
                    #if IS_FROM_FILE
                                GDI_RESIZER_PUT(palette[tmp_byte], TRUE);
                    #else 
                                GDI_RESIZER_PUT(palette[tmp_byte], (BOOL) (tmp_byte != 0));
                    #endif 
                        pos_x++;
                    }
                    dataCount--;
                }
                pos_y--;
            }
            break;
        case 1:
        {
            gdi_color color0, color1;

            pos_x = 0;
            pos_y = bmp_infoheader.biHeight - 1;

            color0 = palette[0];
            color1 = palette[1];

            while (dataCount > 0)
            {
                for (j = 0; j < (S32) U8PerLine; j++)
                {
                    /* 1 bit per color, so read one U8 may fill eight pixel */
                    GET_byte(src, tmp_byte);
                    for (i = 0; i < 8; i++)
                    {
                        if (pos_x < (S32) bmp_infoheader.biWidth)
                        {
                            if (IS_GDI_RESIZER_WANT_DRAW(pos_x, pos_y))
                            {
                                if (tmp_byte & 0x80)
                                {
                                    GDI_RESIZER_PUT(color1, 1);
                                }
                                else
                                {
                                    GDI_RESIZER_PUT(color0, 1);
                                }
                            }
                            tmp_byte <<= 1;
                            pos_x++;
                        }
                    }
                    dataCount--;
                }
                pos_y--;
                pos_x = 0;
            }
        }
            break;
        default:
            GDI_RETURN(GDI_FAILED);
    }
    GDI_RETURN(GDI_SUCCEED);
GDI_EXIT_CRITICAL_SECTION(GDI_IMAGE_BMP_DRAW_INTERNAL)}

