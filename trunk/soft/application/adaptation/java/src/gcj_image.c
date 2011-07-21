
#include "internal.h"
extern unsigned char png_only_decode;
 
#define DECODE_WHEN_DISPLAY

 int testcount=0;

unsigned int protectImageBuff[20];

void protectImageBuffForBufFix(unsigned char *buff)
{
	static int count;
	protectImageBuff[count++] = (unsigned int)buff;
}
#ifdef DECODE_WHEN_DISPLAY

extern U32	gdi_image_decoder_frame_buf[(((240+16)*(320+16)*GDI_MAINLCD_BIT_PER_PIXEL)>>3)/4];

static void * prevImgStream=NULL;
static unsigned char isLargeImgDecoded=0;


#define _OPTIMIZATION_FOR_QQ2008_

static unsigned char overlayerBuffUsed;
void* gcj_GetOverlayBuffer(unsigned int size)
{
	if (size > sizeof(gdi_image_decoder_frame_buf))
		abort();
	overlayerBuffUsed = 1;
	prevImgStream = NULL;
	isLargeImgDecoded=0;
	return gdi_image_decoder_frame_buf;
}
void gcj_ReleaseOverlayBuffer()
{
	gcj_TraceOut(0,"gcj_ReleaseOverlayBuffer------------------------");
	if (!overlayerBuffUsed)
		return;
	overlayerBuffUsed = 0;
	prevImgStream = NULL;
	isLargeImgDecoded=0;
}
void gcj_StubImageDecodeImage(const void *imgStream, unsigned char **decodedBuff, int imgWidth, int imgHeight,imgtype type,int imglen)
{
 	int err_decode=0;
	UINT32 Width=0;
	UINT32 Height=0;
	DEC_UNIT decUnit;
	gdi_handle decode_layer;
#ifdef _OPTIMIZATION_FOR_QQ2008_
	if (overlayerBuffUsed)
		return;
	#define SECOND_IMG_SIZE 
	static void * secondImagBuff=((unsigned char*)gdi_image_decoder_frame_buf)+110*1024;
	if (imgWidth*imgHeight*2> sizeof(gdi_image_decoder_frame_buf))
		abort();
	if (imgWidth*imgHeight*2< (sizeof(gdi_image_decoder_frame_buf)-110*1024))
		*decodedBuff = secondImagBuff;
	else
		*decodedBuff = gdi_image_decoder_frame_buf;
#else
	*decodedBuff = gdi_image_decoder_frame_buf;
#endif	
	//gcj_TraceOut(1,"gcj_StubDecodeImage task id=%d prevImgStream is 0x%x,imgStream is 0x%x,isLargeImgDecoded is %d,decodedBuff is 0x%x,gdi_image_decoder_frame_buf is 0x%x imgWidth=%d,imgHeight=%d type=%d \n",sxr_GetCurrentTaskId(),prevImgStream,imgStream,isLargeImgDecoded,*decodedBuff,gdi_image_decoder_frame_buf,imgWidth,imgHeight,type);
	if ((prevImgStream !=NULL && prevImgStream == imgStream) || (isLargeImgDecoded&&(*decodedBuff == gdi_image_decoder_frame_buf)))
	{
		return;
	}
	prevImgStream = imgStream;
	unsigned char *outBuff = *decodedBuff;

	 if (imgWidth*imgHeight*2 > sizeof(gdi_image_decoder_frame_buf))
	 	abort();
	 else if (imgWidth*imgHeight*2 > 100*1024)
	 {
	 	isLargeImgDecoded=1;
	 }
	 UINT32 OutBufferSize=imgWidth*imgHeight*2;
	 /*
	 if (imgWidth==80 && imgHeight==40)
	 {
	 	gcj_TraceOut(1,"gcj_StubImageDecodeImage return\n");
	 }
	 */
	if (type == TYPE_PNG)
	{
		memset(outBuff,GCJ_TRANS_COLOR,imgWidth*imgHeight*2);
		//outImg->transColor = GCJ_TRANS_COLOR;
		gdi_layer_create_using_outside_memory(0,0,imgWidth,imgHeight,&decode_layer,outBuff,imgWidth*imgHeight*2);
		gdi_layer_push_and_set_active(decode_layer);
		PngStreamBuf.PNGSTREAM = imgStream ;
		PngStreamBuf.byteleft = imglen ;
		png_resized_width = 0;
		png_resized_height = 0;
		png_only_decode = 1;
 		err_decode = MMF_PngDecodePicture(imgStream, &outBuff, &OutBufferSize,&Width, &Height,0,0);
		png_only_decode =0;
		gdi_layer_pop_and_restore_active();
		gdi_layer_free(decode_layer);
	}
	else if (type == TYPE_JPG)
	{
		err_decode = MMF_JpegDecodePicture((long)(imgStream),outBuff,imgWidth*imgHeight*2,
											imgWidth,imgHeight,
											0,0,
											imgWidth-1,imgHeight-1,
											&decUnit,0,NULL);
		Width = decUnit.width;
		Height = decUnit.height;
		err_decode = decUnit.state;
	}
	else if (type == TYPE_GIF)
	{
		memset(outBuff,GCJ_TRANS_COLOR,imgWidth*imgHeight*2);
		gdi_layer_create_using_outside_memory(0,0,imgWidth,imgHeight,&decode_layer,outBuff,imgWidth*imgHeight*2);
		gdi_layer_push_and_set_active(decode_layer);
 		err_decode = gdi_image_gif_draw_transparent_color(0,0,imgStream,imglen,0);
		gdi_layer_pop_and_restore_active();
		gdi_layer_free(decode_layer);
	}
	else
		abort();
	gcj_TraceOut(1,"gcj_StubDecodeImage type=%d,task id=%d err_decode is %d,OutBufferSize is %d,Width is %d, Height is %d,imgWidth=%d,imgHeight=%d,,imgLength=%d,decodedBuff=0x%x \n",type,sxr_GetCurrentTaskId(),err_decode,OutBufferSize,Width,Height,imgWidth,imgHeight,imglen,*decodedBuff);
	if (err_decode !=0)
	{
		//prevImgStream = NULL;
		//isLargeImgDecoded=0;
	}
	//if (test_imgage && imgWidth == 2354)
	//	abort();
	/*if (Width == 1 && Height == 27)
	{
	unsigned short * buf = gcj_StubGetScreenBuffer();
	memset(buf,0x00,LCD_WIDTH*LCD_HEIGHT*2);
	mci_LcdBlockWrite(buf,0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
	COS_Sleep(2000);
	gd_bitblt_memory(outBuff,1*2,0,0,1,27,GDI_COLOR_FORMAT_16,buf,LCD_WIDTH*2,10,10,GDI_COLOR_FORMAT_16,0,0,LCD_WIDTH,LCD_HEIGHT,0,GCJ_TRANS_COLOR);
	mci_LcdBlockWrite(buf,0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
	//COS_Sleep(2000);
	//gd_bitblt_memory(disBuff,destW*2,0,0,destW,destH,GDI_COLOR_FORMAT_16,buf,LCD_WIDTH*2,10,40,GDI_COLOR_FORMAT_16,0,0,LCD_WIDTH,LCD_HEIGHT,0,GCJ_TRANS_COLOR);
	//mci_LcdBlockWrite((unsigned short*)disBuff,0,30,destW,destH+30);
	COS_Sleep(2000);
	//abort();
	}
	*/
 }

#else
 void gcj_StubImageDecodeImage(img_handle_t *outImg,const unsigned char *imgBuff, int imgOffset, int imgLength )
 {
 	int err_decode=0;
	UINT32 Width=0;
	UINT32 Height=0;
	DEC_UNIT decUnit;
	gdi_handle decode_layer;;
	unsigned char *outBuff = outImg->imgData;
	 UINT32 imgWidth = outImg->imgWidth;
	 UINT32 imgHeight = outImg->imgHeight;
	 imgtype type =  outImg->imgType;
	 
	 UINT32 OutBufferSize=imgWidth*imgHeight*2;
	 /*
	 if (imgWidth==80 && imgHeight==40)
	 {
	 	gcj_TraceOut(1,"gcj_StubImageDecodeImage return\n");
	 }
	 */
	if (type == TYPE_PNG)
	{
		memset(outBuff,GCJ_TRANS_COLOR,imgWidth*imgHeight);
		outImg->transColor = GCJ_TRANS_COLOR;
		gdi_layer_create_using_outside_memory(0,0,imgWidth,imgHeight,&decode_layer,outBuff,imgWidth*imgHeight*2);
		gdi_layer_push_and_set_active(decode_layer);
		PngStreamBuf.PNGSTREAM = imgBuff ;
		PngStreamBuf.byteleft = imgLength ;
		png_resized_width = 0;
		png_resized_height = 0;
		png_only_decode = 0;
 		err_decode = MMF_PngDecodePicture(imgBuff, &outBuff, &OutBufferSize,&Width, &Height,0,0);
		png_only_decode =0;
		gdi_layer_pop_and_restore_active();
		gdi_layer_free(decode_layer);
	}
	else if (type == TYPE_JPG)
	{
		err_decode = MMF_JpegDecodePicture((long)(imgBuff),outBuff,imgWidth*imgHeight*2,
											imgWidth,imgHeight,
											0,0,
											imgWidth-1,imgHeight-1,
											&decUnit,0,NULL);
	}
	else if (type == TYPE_GIF)
	{
		memset(outBuff,GCJ_TRANS_COLOR,imgWidth*imgHeight);
		outImg->transColor = GCJ_TRANS_COLOR;
		gdi_layer_create_using_outside_memory(0,0,imgWidth,imgHeight,&decode_layer,outBuff,imgWidth*imgHeight*2);
		gdi_layer_push_and_set_active(decode_layer);
 		err_decode = gdi_image_gif_draw(0,0,imgBuff,imgLength,0);
		gdi_layer_pop_and_restore_active();
		gdi_layer_free(decode_layer);
	}
	else
		abort();
	gcj_TraceOut(1,"gcj_StubDecodeImage count=%d err_decode is %d,OutBufferSize is %d,Width is %d, Height is %d,imgWidth=%d,imgHeight=%d,imgOffset=%d,imgLength=%d \n",testcount,err_decode,OutBufferSize,Width,Height,imgWidth,imgHeight,imgOffset,imgLength);
	if (err_decode !=0)
	{	
	}
	//protectImageBuffForBufFix(outBuff);
	if ((Width == 84 && Height == 14) || (Width == 240 && Height == 16))
	{
		//gcj_pageSpay(outBuff,outBuff+Width*Height*2);
	}
/*	
	{
		gcj_StubGraphicsDrawRegion(gcj_StubGetScreenBuffer(),outImg,0,0,imgWidth,imgHeight,0,0,0);
		gcj_StubDisplayRefresh(gcj_StubGetScreenBuffer(),0,0,LCD_WIDTH-1,LCD_HEIGHT -1);
	}
	*/
 }
#endif

unsigned int LIGHT_COLOR = LIGHT_GRAY_COLOR;	//argb value
unsigned int DARK_COLOR = DARK_GRAY_COLOR;	//argb value
unsigned int FONT_COLOR = blackPixel;	//argb value


void gcj_SetJavaColor(unsigned int lightColor,unsigned darkColor,unsigned fontColor)
{
	LIGHT_COLOR = lightColor;
	DARK_COLOR = darkColor;
	FONT_COLOR = fontColor;
}

#define RGB24ToRGB16(r,g,b) \
	(((r&0x00f8)<<8)|((g&0x00fc)<<3)|(b&0xf8)>>3)

int gcj_StubGraphicsGetPixel( int rgb, int gray,int isGray)
{
	if (rgb == LIGHT_GRAY_COLOR)
		rgb = LIGHT_COLOR;
	if (rgb == DARK_GRAY_COLOR)
		rgb = DARK_COLOR;
	UINT8 r = (rgb >> 16) & 0xff;
	UINT8 g = (rgb >> 8)& 0xff;
	UINT8 b = (rgb) & 0xff;
	if (!isGray)
		return RGB24ToRGB16(r,g,b);
	else
		return (r*76 + g*150 + b*29) >> 8;
}


extern void gdi_2d_memory_blt(
	U8 *src_ptr,
	S32 src_pitch,
	S32 src_offset_x,
	S32 src_offset_y,
	S32 src_width,
	S32 src_height,
	U8 *dest_ptr,
	S32 dest_pitch,
	S32 dest_offset_x,
	S32 dest_offset_y,
	gdi_rect_struct dest_clip);

extern void gd_bitblt_memory(
                U8 *src,
                U32 src_pitch,
                S16 src_x,
                S16 src_y,
                U16 src_width,
                U16 src_height,
                U8 src_color_mode,
                U8 *dest,
                U32 dest_pitch,
                S16 dest_x,
                S16 dest_y,
                U8 dest_color_mode,
                S16 clipx1,
                S16 clipy1,
                S16 clipx2,
                S16 clipy2,
                kal_bool src_key_enable,
                kal_uint32 src_key);
#if 0
void gcj_StubGraphicsDrawImage(unsigned short *distBuff,unsigned char *imgbuf,unsigned int startx,unsigned int starty,unsigned int imgw,unsigned int imgh)
{
	if (distBuff == NULL)
		abort();
	if (imgw ==0 || imgh == 0)
		return;
	/*
	int i =0;
	unsigned int nextline= starty * 176;
	unsigned char *distBuff = (unsigned char *)ScreenBuffer + nextline+startx;
	for (i=0;i < imgh; i++)
	{
		memcpy(distBuff,imgbuf,imgw);
		distBuff += nextline;
	}
	*/
	gd_bitblt_memory(imgbuf,imgw,startx,starty,imgw,imgh,3,distBuff,176,startx,starty,3,0,0,176-1,220-1,0,0);
    //mci_LcdBlockWrite((unsigned short*)imgbuf,startx,starty,startx+imgw-1,starty+imgh-1);
		
}
#endif
#ifdef DECODE_WHEN_DISPLAY

void gcj_StubGraphicsDrawRegion(unsigned short *disBuff, unsigned char *img_buff,unsigned int img_width,unsigned int img_height,unsigned int x_src,unsigned int y_src,unsigned int width,unsigned int height,unsigned int transform,unsigned int x_dest,unsigned int y_dest,unsigned int destW,unsigned int destH)
{
	if (x_dest+width > LCD_WIDTH)
		return;
	//gcj_TraceOut(0,"gcj_StubGraphicsDrawRegion imgw=%d,imgh=%d,disBuff=0x%x,imgbuf=0x%x,x_src=%d,y_src=%d,width=%d,height=%d,x_dest=%d,y_dest=%d,destW=%d,destH=%d",img_width,img_height,disBuff,img_buff,x_src,y_src,width,height,x_dest,y_dest,destW,destH);
	gd_bitblt_memory(img_buff,img_width*2,x_src,y_src,width,height,GDI_COLOR_FORMAT_16,disBuff,destW*2,x_dest,y_dest,GDI_COLOR_FORMAT_16,0,0,destW,destH,1,GCJ_TRANS_COLOR);
	/*if ((img_width==33 && img_height == 31 && y_dest == 134) || (img_width==1 && img_height==27 && x_dest == 170))
	{
		int x2 = x_src+img_width-1;
		int y2 = y_src+img_height-1;
		if (x2 >= LCD_WIDTH)
			x2 = LCD_WIDTH-1;
		if (y2 >= LCD_HEIGHT)
			y2 = LCD_HEIGHT -1;
		unsigned short * buf = gcj_StubGetScreenBuffer();
		memset(buf,0x00,LCD_WIDTH*LCD_HEIGHT*2);
		mci_LcdBlockWrite(buf,0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
		COS_Sleep(2000);
		gd_bitblt_memory(img_buff,img_width*2,0,0,img_width,img_height,GDI_COLOR_FORMAT_16,buf,LCD_WIDTH*2,10,10,GDI_COLOR_FORMAT_16,0,0,LCD_WIDTH,LCD_HEIGHT,1,GCJ_TRANS_COLOR);
		//mci_LcdBlockWrite(buf,0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
		//COS_Sleep(2000);
		gd_bitblt_memory(disBuff,destW*2,0,0,destW,destH,GDI_COLOR_FORMAT_16,buf,LCD_WIDTH*2,10,40,GDI_COLOR_FORMAT_16,0,0,LCD_WIDTH,LCD_HEIGHT,1,GCJ_TRANS_COLOR);
		mci_LcdBlockWrite(buf,0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
		gcj_TraceOut(0,"this disbuf = 0x%x",disBuff);
		COS_Sleep(2000);
	}
	*/
}


#else
void gcj_StubGraphicsDrawRegion(unsigned short *distBuff,img_handle_t*imgHandle,unsigned int x_src,unsigned int y_src,unsigned int width,unsigned int height,unsigned int transform,unsigned int x_dest,unsigned int y_dest,unsigned int destW,unsigned int destH)
{
	if (distBuff == NULL)
		abort();
	unsigned int imgw = imgHandle->imgWidth;
	unsigned int imgh = imgHandle->imgHeight;
	unsigned char havaTransColor = imgHandle->isTransColor;
	unsigned short transColor = imgHandle->transColor;
	unsigned char *imgbuf = imgHandle->imgData;
	//if (y_dest >LCD_HEIGHT-40)
		//gcj_TraceOut(0,"gcj_StubGraphicsDrawRegion  imgbuff = 0x%x,imgw=%d,imgh=%d,x_src=%d,y_src=%d,width=%d,height=%d,x_dest=%d,y_dest=%d",imgbuf,imgw,imgh,x_src,y_src,width,height,x_dest,y_dest);
	//gcj_TraceOut(0,"gcj_StubGraphicsDrawRegion imgw=%d,imgh=%d,havaTransColor=%d,imgbuf=0x%x,x_src=%d,y_src=%d,width=%d,height=%d,x_dest=%d,y_dest=%d",imgw,imgh,havaTransColor,imgbuf,x_src,y_src,width,height,x_dest,y_dest);
	if (x_dest+width > LCD_WIDTH)
		return;
	gd_bitblt_memory(imgbuf,imgw*2,x_src,y_src,width,height,GDI_COLOR_FORMAT_16,distBuff,destW*2,x_dest,y_dest,GDI_COLOR_FORMAT_16,0,0,LCD_WIDTH-1,LCD_HEIGHT-1,havaTransColor,transColor);
/*	if (x_dest <0 )
		x_dest=0;
	if (y_dest < 0)
		y_dest = 0;
	int x2 = x_src+imgw-1;
	int y2 = y_src+imgh-1;
	if (x2 >= LCD_WIDTH)
		x2 = LCD_WIDTH-1;
	if (y2 >= LCD_HEIGHT)
		y2 = LCD_HEIGHT -1;
	mci_LcdBlockWrite((unsigned short*)imgbuf,x_src,y_src,x2,y2);
*/
}
#endif


void gcj_image_reset()
{
	prevImgStream = NULL;
	isLargeImgDecoded=0;
	overlayerBuffUsed=0;
	//gcj_StubInitMutex(&decodeMutex);
}

