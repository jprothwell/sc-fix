/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CoolSand Inc. (C) 2005
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
/*****************************************************************************
 *
 * Filename:
 * ---------
 *  ImageGetDimension.c
 *
 * Project:
 * -------- 
 *
 * Description:
 * ------------
 *  get image dimension
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *
 *------------------------------------------------------------------------------
 *============================================================================
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs_types.h"
#include "mmi_data_types.h"
#include "mmi_features.h"
#include "custdatares.h"
#include "imagegetdimension.h"

/* header info size */
#define BMP_INFO_SIZE	26
#define GIF_INFO_SIZE	10
#define PNG_INFO_SIZE	24

/* markers of JPEG standard */
#define JPEG_MARKER_SOF0	0xC0
#define JPEG_MARKER_SOF1	0xC1
#define JPEG_MARKER_SOF2	0xC2
#define JPEG_MARKER_SOF3	0xC3
#define JPEG_MARKER_SOF15	0xCF
#define JPEG_MARKER_SOS		0xDA
#define JPEG_MARKER_DQT		0xDB
#define JPEG_MARKER_DNL		0xDC
#define JPEG_MARKER_DRI		0xDD
#define JPEG_MARKER_DHP		0xDE
#define JPEG_MARKER_EXP		0xDF
#define JPEG_MARKER_COM		0xFE
#define JPEG_MARKER_APP0	0xE0
#define JPEG_MARKER_APP15	0xEF

/* macros to get data (X) from buffer (SRC) */
#define GET_U8(SRC,X)				\
	do								\
	{								\
		X = ((U8) *(SRC)++);		\
	} while(0)

#define GET_U16(SRC,X)				\
	do								\
	{								\
		X = ((U16) (U8)*(SRC)++);	\
		X |= ((U16) *(SRC)++)<<8;	\
	} while(0)

#define GET_U24(SRC,X)					\
	do									\
	{									\
		X = ((U32) (U8)*(SRC)++);		\
		X |= ((U32) (U8)*(SRC)++)<<8;	\
		X |= ((U32) (U8)*(SRC)++)<<16;	\
	} while(0)

#define GET_U32(SRC,X)					\
	do									\
	{									\
		(X) = ((U32) (U8)*(SRC)++);		\
		(X) |= ((U32) (U8)*(SRC)++)<<8; \
		(X) |= ((U32) (U8)*(SRC)++)<<16;\
		(X) |= ((U32) (U8)*(SRC)++)<<24;\
	} while(0)

#define FLUSH(SRC,X)	\
	do					\
	{					\
		(SRC) += (X);	\
	} while(0)


static char Video_Ext[][4] = {"AVI", "3GP", "MP4"};

extern MMI_BOOL ENFBAssociatedIDFlag;
extern MMI_BOOL ENFBAssociatedIDAdded;	


#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
extern int disableENFB;
#endif

/*****************************************************************************
 * FUNCTION
 *  image_bmp_get_dimension
 * DESCRIPTION
 *  get dimension of a bmp from file
 * PARAMETERS
 *  filename    [IN]
 *  width       [OUT]
 *  height      [OUT]
 * RETURNS
 *  MMI_BOOL	MMI_TRUE or MMI_FALSE
 *****************************************************************************/
static MMI_BOOL image_bmp_get_dimension(S8 *filename, U32 *width, U32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FILE *fp;
	U8	buf[BMP_INFO_SIZE];
	U8	*src;
	U16	bfType;
	U32	bfSize;
	U32	biWidth;
	U32	biHeight;
	U32	biSize;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	/* open file and read header to buffer */
    fp = fopen((char *)filename, "rb");
	if (fp == NULL)
	{
		printf("Open file error:%s\n", filename);
		return MMI_FALSE;
	}
	if (fread(buf, sizeof(char), BMP_INFO_SIZE, fp) != BMP_INFO_SIZE)
	{
		printf("Read file error:%s\n", filename);
		return MMI_FALSE;
	}
	src = buf;
    fclose(fp);

	/* read file header */
	GET_U16(src, bfType);
	GET_U32(src, bfSize);
	FLUSH(src, 8);

	/* read info header */
    GET_U32(src, biSize);
	if (biSize == 12)
	{
		GET_U16(src, biWidth);
		GET_U16(src, biHeight);
	}
	else
	{
		GET_U32(src, biWidth);
		GET_U32(src, biHeight);
	}

	*width = biWidth;
	*height = biHeight;

	/* test if it is BMP file */
	if (bfType != 0x4d42)	/* BM */
	{
		printf("Non-standard file error:%s\n", filename);
		return MMI_FALSE;
	}
	/* core info header, currently not support */
	if (bfSize == BMP_INFO_SIZE)
	{
		printf("Non-standard file error:%s\n", filename);
		return MMI_FALSE;
	}

    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  image_gif_get_dimension
 * DESCRIPTION
 *  get dimension of a gif from file
 * PARAMETERS
 *  filename    [IN]
 *  width       [OUT]
 *  height      [OUT]
 * RETURNS
 *  MMI_BOOL	MMI_TRUE or MMI_FALSE
 *****************************************************************************/
static MMI_BOOL image_gif_get_dimension(S8 *filename, U32 *width, U32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FILE *fp;
	U8	buf[GIF_INFO_SIZE];
	U8	*src;
    U32 header;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* open file and read header to buffer */
    fp = fopen((char *)filename, "rb");
	if (fp == NULL)
	{
		printf("Open file error:%s\n", filename);
		return MMI_FALSE;
	}
	if (fread(buf, sizeof(char), GIF_INFO_SIZE, fp) != GIF_INFO_SIZE)
	{
		printf("Read file error:%s\n", filename);
		return MMI_FALSE;
	}
	src = buf;
    fclose(fp);

    /* test if it is BMP file */
    GET_U24(src, header);
    if (header != 0x464947) /* 'GIF' */
    {
        *width = 0;
        *height = 0;
        return MMI_FALSE;
    }
    
    /* read GIF dimention */
    FLUSH(src, 3);
    GET_U16(src, *width);
    GET_U16(src, *height);

    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  image_png_get_dimension
 * DESCRIPTION
 *  get dimension of a png from file
 * PARAMETERS
 *  filename    [IN]
 *  width       [OUT]
 *  height      [OUT]
 * RETURNS
 *  MMI_BOOL	MMI_TRUE or MMI_FALSE
 *****************************************************************************/
static MMI_BOOL image_png_get_dimension(S8 *filename, U32 *width, U32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FILE *fp;
	U8 buf[PNG_INFO_SIZE];
    U8 *src;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /* open file and read header to buffer */
    fp = fopen((char *)filename, "rb");
	if (fp == NULL)
	{
		printf("Open file error:%s\n", filename);
		return MMI_FALSE;
	}
	if (fread(buf, sizeof(char), PNG_INFO_SIZE, fp) != PNG_INFO_SIZE)
	{
		printf("Read file error:%s\n", filename);
		return MMI_FALSE;
	}
    src = buf;
    fclose(fp);

    /* test if it is BMP file and get PNG dimension */
    if (memcmp(src, "\x89PNG\x0d\x0a\x1a\x0a", 8) == 0)  /* PNG signature */
    {
        if (*((U32*) (src + 8)) == 13 << 24)             /* must be IHDR, length = 13,but LSB */
        {
            if (memcmp(src + 12, "IHDR", 4) == 0)        /* must be 'IHDR' */
            {
                src += 16;
                *width = (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | (src[3]);
                src += 4;
                *height = (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | (src[3]);
                return MMI_TRUE;
            }
        }
    }
        
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  image_jpg_get_dimension
 * DESCRIPTION
 *  get dimension of a jpg from file
 * PARAMETERS
 *  filename    [IN]
 *  width       [OUT]
 *  height      [OUT]
 * RETURNS
 *  MMI_BOOL	MMI_TRUE or MMI_FALSE
 *****************************************************************************/
static MMI_BOOL image_jpg_get_dimension(S8 *filename, U32 *width, U32 *height)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    FILE *fp;

	U32 index, index_offset;
	U16 marker_length;
	U8  jpg_byte1, jpg_byte2, tmp_byte;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	/* open file and read header to buffer */
    fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("Open file error:%s\n", filename);
		return MMI_FALSE;
	}

	index = 0;

	jpg_byte1 = fgetc(fp);
	jpg_byte2 = fgetc(fp);

	/* check first two byte to see if is valid jpeg file */
    if ((jpg_byte1 != 0xFF) || (jpg_byte2 != 0xD8))
	{
		return MMI_FALSE;
	}

    *width = *height = 0;

	/* parse bytestream */
	do
	{
		if (feof(fp))
		{
			break;
		}

		do
		{
			tmp_byte = fgetc(fp);
		} while (tmp_byte != 0xFF);

		jpg_byte1 = 0xFF;
		jpg_byte2 = fgetc(fp);

		switch (jpg_byte2)
		{
			case JPEG_MARKER_SOF0:
			case JPEG_MARKER_SOF2:
				marker_length = fgetc(fp) << 8;
				marker_length |= fgetc(fp);
				marker_length -= 2;

				fgetc(fp);  /* precision */
				*height = fgetc(fp) << 8;
				*height |= fgetc(fp);
				*width = fgetc(fp) << 8;
				*width |= fgetc(fp);

				/* set terminate byte */
				jpg_byte2 = 0xD9;
				break;

			case JPEG_MARKER_SOS:
				break;

			case 0xC8:
			case 0x00:
				break;

			default:
				if ((jpg_byte2 == JPEG_MARKER_SOF1) ||
				    ((jpg_byte2 >= JPEG_MARKER_SOF3) && (jpg_byte2 <= JPEG_MARKER_SOF15)) ||
					(jpg_byte2 == JPEG_MARKER_DQT) || (jpg_byte2 == JPEG_MARKER_DNL) ||
					(jpg_byte2 == JPEG_MARKER_DRI) || (jpg_byte2 == JPEG_MARKER_DHP) ||
					(jpg_byte2 == JPEG_MARKER_EXP) || (jpg_byte2 == JPEG_MARKER_COM) ||
					((jpg_byte2 >= JPEG_MARKER_APP0) && (jpg_byte2 <= JPEG_MARKER_APP15)))
				{
					U8 *dummy;

					index_offset = fgetc(fp) << 8;
					index_offset |= fgetc(fp);
					index_offset -= 2;

					dummy = (U8 *)malloc(index_offset*sizeof(U8));
					fread(dummy, sizeof(U8), index_offset, fp);
					free(dummy);
				}
				break;
		}
	} while (!((jpg_byte1 == 0xFF) && (jpg_byte2 == 0xD9)));
	
    fclose(fp);

    return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  Image_Test
 * DESCRIPTION
 *  test the resource is video, over-half-size image, or others
 * PARAMETERS
 *  filename    [IN]
 *  width       [OUT]
 *  height      [OUT]
 * RETURNS
 *  int			>0: video or over-half-size image, =0: others
 *****************************************************************************/
S32 Image_Test(S8 *filename, U32 *width, U32 *height)
{
#ifdef __MMI_RESOURCE_ENFB_SUPPORT__
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	S8 ext[4];	/* filename extension */
	U32 lw, lh;		/* resolution of LCD size */
	U32 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (disableENFB)
    {
        return ENFB_IMAGE_NONE;
    }
    else if (ENFBAssociatedIDFlag == MMI_TRUE)
    {
        //ENFBAssociatedIDAdded = MMI_TRUE;
        return ENFB_IMAGE_ASSOCIATE;
    }

	/* get filename extension and convert it to upper case */
	for(i = 0; i < 3; i++)
	{
		ext[i] = toupper(filename[strlen(filename) - 3 + i]);
	}
    ext[3] = '\0';

	*width = *height = 0;

    /* if video file, return true */
    for (i = 0; i < (sizeof(Video_Ext) / 4); i++)
    {
        if (strncmp(ext, Video_Ext[i], 3) == 0)
        {
            return ENFB_IMAGE_VIDEO;
        }
    }

	/* get dimension of BMP or PBM file */
    if ((strncmp(ext, "BMP", 3) == 0) ||
        (strncmp(ext, "PBM", 3) == 0))
	{
		if(image_bmp_get_dimension(filename, width, height) == MMI_FALSE)
		{
			return ENFB_IMAGE_NONE;
		}
	}
	/* get dimension of GIF file */
	else if ((strncmp(ext, "GIF", 3) == 0))
	{
		if(image_gif_get_dimension(filename, width, height) == MMI_FALSE)
		{
			return ENFB_IMAGE_NONE;
		}
	}
	/* get dimension of PNG file */
	else if ((strncmp(ext, "PNG", 3) == 0) ||
             (strncmp(ext, "PPN", 3) == 0))
	{
		if(image_png_get_dimension(filename, width, height) == MMI_FALSE)
		{
			return ENFB_IMAGE_NONE;
		}
	}
	/* get dimension of JPG file */
	else if ((strncmp(ext, "JPG", 3) == 0))
	{
		if(image_jpg_get_dimension(filename, width, height) == MMI_FALSE)
		{
			return ENFB_IMAGE_NONE;
		}
	}
	/* other files will return false directly */
	else
	{
		return ENFB_IMAGE_NONE;
	}

	/* set main LCD size */
	lw = lh = 0;
#ifdef __MMI_MAINLCD_128X128__
	lw = 128;
	lh = 128;
#endif
#ifdef __MMI_MAINLCD_128X160__
	lw = 128;
	lh = 160;
#endif
#ifdef __MMI_MAINLCD_160X128__
	lw = 160;
	lh = 128;
#endif
#ifdef __MMI_MAINLCD_176X220__
	lw = 176;
	lh = 220;
#endif
#ifdef __MMI_MAINLCD_240X320__
	lw = 240;
	lh = 320;
#endif

    //printf("\nPBM --> %s --> %d x %d\n", filename, *width, *height);

	/* test whether the image file is over half of the LCD size */
	if ((*width) * (*height) >= 0.5 * lw * lh)
	{
	    printf("enfb file = %s\n", filename);
		return ENFB_IMAGE_DIMENSION;
	}

	return ENFB_IMAGE_NONE;
#else
	return 0;   // ENFB_IMAGE_NONE;
#endif
}
