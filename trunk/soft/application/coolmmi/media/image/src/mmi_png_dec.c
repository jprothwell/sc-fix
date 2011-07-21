

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SHEEN_VC_DEBUG
#include "cs_types.h"
//#include "dsm_cf.h"
#include "ts.h"
#include "cos.h"
#endif
 

#include "zlib.h"
#include "png.h"
#include "mmi_png_dec.h"

#ifndef SHEEN_VC_DEBUG
#include "mmi_trace.h"
#include "fs_type.h"
#include "fat_fs.h"
#include "imagep_debug.h"
#include "gdi_datatype.h"
#include "gdi_include.h"
#endif


const BYTE IHDR[5] = { 73,  72,  68,  82, '\0'};
const BYTE IDAT[5] = { 73,  68,  65,  84, '\0'};
const BYTE IEND[5] = { 73,  69,  78,  68, '\0'};
const BYTE PLTE[5] = { 80,  76,  84,  69, '\0'};
const BYTE bKGD[5] = { 98,  75,  71,  68, '\0'};
const BYTE cHRM[5] = { 99,  72,  82,  77, '\0'};
const BYTE gAMA[5] = {103,  65,  77,  65, '\0'};
const BYTE hIST[5] = {104,  73,  83,  84, '\0'};
const BYTE iCCP[5] = {105,  67,  67,  80, '\0'};
const BYTE iTXt[5] = {105,  84,  88, 116, '\0'};
const BYTE pHYs[5] = {112,  72,  89, 115, '\0'};
const BYTE sBIT[5] = {115,  66,  73,  84, '\0'};
const BYTE sPLT[5] = {115,  80,  76,  84, '\0'};
const BYTE sRGB[5] = {115,  82,  71,  66, '\0'};
const BYTE tEXt[5] = {116,  69,  88, 116, '\0'};
const BYTE tIME[5] = {116,  73,  77,  69, '\0'};
const BYTE tRNS[5] = {116,  82,  78,  83, '\0'};
const BYTE zTXt[5] = {122,  84,  88, 116, '\0'};

extern IHDRCHUNK IHDRInfo;
extern Png_StreamBuf PngStreamBuf;
extern FILECHUNKS FileChunks;
extern z_stream PNGzstream;
extern UINT32 IDATSize; // IDAT块数据大小
extern UINT32 CurRowNumber;            // 当前行号
extern UINT32 CurPassRowNum;           // 当前Pass的行数
extern UINT32 CurPassRowBytes;         // 当前行的字节数
extern UINT32 CurInterlaceRowWidth;    // 当前交织行的宽度
extern BYTE CurPass;                   // 当前的Pass号
extern BYTE *tRNSIndex ; // 索引图像透明信息
extern bKGDINFO bKGDInfo;      // 背景块信息
extern UINT32 RowBytes;        // 原始图像一行的字节数
extern UINT32 transform ;   // 变换信息
extern UINT8 ImagePass;        // 图像Pass数
extern UINT8 PixelDepth;       // 像素比特深度
extern FS_HANDLE	PngFileHandle;
extern BYTE *RowBuffer;         // 行缓冲区
extern BYTE *PreRow ;            // 前一行数据
extern BYTE *FinalRowData ;      // 变换处理后的行数据
extern BYTE RowBytesTempArray[];
extern BYTE RowBytesArray[];
extern BYTE FinalRowArray[];
extern ROWINFO RowInfo;                // 行信息
extern UINT16 TransNum;

U8	*dest_line_start_ptr;        //合并a层后的输出起始地址
U8	*alpha_dest_row_ptr;         //合并a层后的输出行起始地址
U16	*alpha_dest_output_ptr;      //合并a层后的输出临时地址
U16	*alpha_dest_output_ptr1;      //合并a层后的输出临时地址
U16	*alpha_dest_output_ptr2;      //合并a层后的输出临时地址
unsigned char png_only_decode=0;

UINT32 png_resized_width;
UINT32 png_resized_height;
UINT32 ResizePng_Need;
#ifndef SHEEN_VC_DEBUG
extern void gdi_get_alpha_blending_source_layer(gdi_handle *src_layer_handle);
extern void gdi_image_layer_copy(S32 x1, S32 y1, S32 x2, S32 y2, gdi_handle dst_layer, gdi_handle src_layer);
#endif
int MMF_PngDecodePicture(BYTE *FileContent,           // 输入PNG文件内容
			  BYTE **PNGOutBuffer,             // 解码输出缓存地址
			  UINT32 *OutBufferSize,             // 输入文件内容大小
			  UINT32 *ImgWidth,            // 图像宽度
			  UINT32 *ImgHeigh,             // 图像高度
			  S32 ox,
			  S32 oy
//			  PLTECHUNK *pBkgColor;
			  )
{
//IMGDP_ENTRY(10020);

	BYTE *PngFileContent = FileContent;
//	PLTECHUNK *ppBkgColor = pBkgColor;
	UINT32 ChunkLength = 0;
	CHUNKTYPE ChunkType;
//	UINT32 ChunkCRC = 0;

//	BYTE *OutBuffer;

	int errcode;             
	BYTE PNGSignature[] = {137, 80, 78, 71, 13, 10, 26, 10};

/************************************************************************************/
/*******elisa added for png alpha display with the mmi backbround**2009-10-9*********/
/************************************************************************************/

             
	//U8	*src_buf_ptr;

	//U8	*src_line_start_ptr;
	S16	dest_offset_x, dest_offset_y;
	S32	y_jump, x_jump;
	S32	jump_size;
	S32	start, end;
	S32	x_draw_count, y_draw_count;
	gdi_rect_struct	dest_clip;
	S32 tempX = ox;
	S32 tempY = oy;
	
/************************************************************************************/
/*******elisa end  2009-10-9*********************************************************/
/************************************************************************************/
	
 // mmi_trace(1,"gdi_image png start1  dest_line_start_ptr%d    ",dest_line_start_ptr);

		if(tempX < 0)
				{
				 	tempX = 0;
				}
			if(tempY < 0)
				{
				 	tempY = 0;
				}
// init all global parameters
       PNG_Init();
	
//	mmi_trace(1,"state is %d",PNGzstream.state->dummy);
	
	if (memcmp(PNGSignature, PngFileContent, 8) != 0)
	{
		//printf("This is not a PNG file!\n");
//		exit(0);
	}
	else
	{
		PngFileContent += 8;
		PngStreamBuf.byteleft -=8;
		//printf("It is a PNG file!\n");
	}

//CSW_TRACE(MMI_TS_ID,"Enter PNGdecoder!\n");

	ChunkLength = ((*PngFileContent) << 24) + ((*(PngFileContent + 1)) << 16)
		+ ((*(PngFileContent + 2)) << 8) + (*(PngFileContent + 3));
	PngFileContent += 4;
	PngStreamBuf.byteleft -=4;
	
	ChunkType.First = *PngFileContent;
	ChunkType.Second = *(PngFileContent + 1);
	ChunkType.Third = *(PngFileContent + 2);
	ChunkType.Forth = *(PngFileContent + 3);
	
	if (memcmp(&ChunkType, IHDR, 4) == 0)
	{
		errcode = IHDRHandle(&PngFileContent, ChunkLength);
//    	mmi_trace(1,"gdi_image png start  png_resized_width%d    ",png_resized_width);
//	    mmi_trace(1,"gdi_image png start  png_resized_height%d    ",png_resized_height);
 //   	mmi_trace(1,"gdi_image png start  IHDRInfo.Width%d    ",IHDRInfo.Width);
//	    mmi_trace(1,"gdi_image png start  IHDRInfo.Height%d    ",IHDRInfo.Height);
		if (errcode != 0)
		{
			return errcode;
		}
		else
      {     
        if((png_resized_width == 0)&&(png_resized_height == 0))
          {
            ResizePng_Need=0;
            png_resized_width=IHDRInfo.Width;
            png_resized_height=IHDRInfo.Height;
          }
        else if((png_resized_width == IHDRInfo.Width)&&(png_resized_height == IHDRInfo.Height))
			{
				
				ResizePng_Need=0;
			
			}
			else
			{
				ResizePng_Need=1;
				if(png_resized_width == 0)
				{
					png_resized_width=IHDRInfo.Width;
				}
				if(png_resized_height == 0)
				{
					png_resized_height=IHDRInfo.Height;
				}

// guosz resize added				return RESIZEERR;
			}
		}
	}
	else
	{
		return IHDRERR;
	}
	if (!png_only_decode)
	{
		if((png_resized_width > LCD_WIDTH))//||(png_resized_height > LCD_HEIGHT))
		{
	//    	mmi_trace(1,"gdi_image png start  png_resized_width%d    ",png_resized_width);
	//	    mmi_trace(1,"gdi_image png start  png_resized_height%d    ",png_resized_height);
	  		return RESIZEERR;
		}
	}
/************************************************************************************/
/*******elisa added for png alpha display with the mmi backbround**2009-10-9*********/
/************************************************************************************/

	*ImgWidth = (UINT32)IHDRInfo.Width;//(((IHDRInfo.Width << 4) + 31) >> 5 ) << 1;
	*ImgHeigh = (UINT32)IHDRInfo.Height;

	//alpha_dest_buf_ptr	= gdi_act_layer->buf_ptr;
	//src_buf_ptr	= *PNGOutBuffer;

	dest_offset_x = ox;
	dest_offset_y = oy;

	dest_clip.x1 = gdi_act_layer->clipx1;
	dest_clip.y1 = gdi_act_layer->clipy1;
	dest_clip.x2 = gdi_act_layer->clipx2;
	dest_clip.y2 = gdi_act_layer->clipy2;
		
		/* if not totally out of clip region */
		if(!(	(dest_offset_x					> dest_clip.x2) ||
				(dest_offset_y					> dest_clip.y2) ||
				(dest_offset_x+(S32)(*ImgWidth)	< dest_clip.x1) ||
				(dest_offset_y+(S32)(*ImgHeigh)	< dest_clip.y1)) )
		{


			/* adjust source ptr to the start of drawing point */

			/* jump to start y line */
			y_jump = 0;
			if(dest_offset_y < 0)
			{
				y_jump		=  dest_clip.y1;
			}
			else if(dest_offset_y < dest_clip.y1)
			{
				y_jump		= dest_clip.y1;
			}
			else
				{
					y_jump = dest_offset_y;
				}

			/* jump to start x */
			x_jump = 0;
			if(dest_offset_x < 0)
			{
				x_jump = dest_clip.x1;
				
			}
			else if(dest_offset_x < dest_clip.x1)
			{
				x_jump = dest_clip.x1;				
			}
			else
				{
					x_jump = dest_offset_x;
				}
/*			y_jump = 0;
			if(dest_offset_y < 0)
			{
				y_jump		= 0; // -dest_offset_y;
			}
			else if(dest_offset_y < dest_clip.y1)
			{
				y_jump		= dest_clip.y1; //-dest_offset_y;
			}
			else
				{
					y_jump = dest_offset_y;
				}

			x_jump = 0;
			if(dest_offset_x < 0)
			{
				x_jump = 0;//-dest_offset_x;
				
			}
			else if(dest_offset_x < dest_clip.x1)
			{
				x_jump = dest_clip.x1; //-dest_offset_x;				
			}
			else
				{
					x_jump = dest_offset_x;
				}
*/
		//	src_line_start_ptr = src_buf_ptr;


			/* adjust dest ptr to the start of drawing point */
			/* find start ptr for actual drawing */
				jump_size = (((y_jump)*gdi_act_layer->width + x_jump)*GDI_LAYER.bits_per_pixel)>>3;
				dest_line_start_ptr = gdi_act_layer->buf_ptr + jump_size;
			/* find x_draw_count and y_draw_count */
			start	= (dest_offset_x < dest_clip.x1)							? dest_clip.x1 : dest_offset_x;
			end	= (dest_offset_x+(S32)(*ImgWidth)-1 > dest_clip.x2)	? dest_clip.x2 : dest_offset_x+(S32)(*ImgWidth)-1;
			x_draw_count = end-start+1;


			start	= (dest_offset_y < dest_clip.y1)							? dest_clip.y1 : dest_offset_y;
			end	= (dest_offset_y+(S32)(*ImgHeigh)-1 > dest_clip.y2)	? dest_clip.y2 : dest_offset_y+(S32)(*ImgHeigh)-1;
			y_draw_count = end-start+1;

		}
    else
    {
      return RESIZEERR;
      // wrong position, out of screen
    }

/************************************************************************************/
/*******elisa end  2009-10-9*********************************************************/
/************************************************************************************/
#ifndef SHEEN_VC_DEBUG
{
gdi_handle 		baselayer,activelayer;
              gdi_get_alpha_blending_source_layer(&baselayer);
		gdi_layer_get_active(&activelayer);  
		if((activelayer != baselayer)&&(activelayer != -1)&&(baselayer != -1))
			{
			
				gdi_image_layer_copy(tempX,tempY,ox+IHDRInfo.Width-1,oy +IHDRInfo.Height -1,activelayer,baselayer);
			}
}
#endif

	for (;;)
	{
		ChunkLength = ((*PngFileContent) << 24) + ((*(PngFileContent + 1)) << 16)
			+ ((*(PngFileContent + 2)) << 8) + (*(PngFileContent + 3));
		PngFileContent += 4;
		PngStreamBuf.byteleft -=4;
		
		if((INT32)ChunkLength+4 > MAXPNG_BUF){
			mmi_trace(TRUE,"too large png, not support!---zhangl\n");
			return ChunkExtra;
		}
		if((INT32)ChunkLength > (PngStreamBuf.byteleft+8)){
			INT32 byte_load=0;
			memmove(PngStreamBuf.PNGSTREAM,PngFileContent,PngStreamBuf.byteleft);
			MMI_FS_Read(PngFileHandle, (PngStreamBuf.PNGSTREAM+PngStreamBuf.byteleft), (MAXPNG_BUF-PngStreamBuf.byteleft),&byte_load );			
			PngStreamBuf.byteleft+=byte_load;
			PngFileContent=PngStreamBuf.PNGSTREAM;
	}
		
		ChunkType.First = *PngFileContent;
		ChunkType.Second = *(PngFileContent + 1);
		ChunkType.Third = *(PngFileContent + 2);
		ChunkType.Forth = *(PngFileContent + 3);
		
		if (memcmp(&ChunkType, cHRM, 4) == 0)
		{
			errcode = cHRMHandle(&PngFileContent, ChunkLength);

			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, gAMA, 4) == 0)
		{
			errcode = gAMAHandle(&PngFileContent, ChunkLength);

			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, iCCP, 4) == 0)
		{
			errcode = iCCPHandle(&PngFileContent, ChunkLength);
			
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, sBIT, 4) == 0)
		{
			sBITInfo SigBit;
			errcode = sBITHandle(&PngFileContent, ChunkLength, &SigBit);
			
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, sRGB, 4) == 0)
		{
			errcode = sRGBHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, PLTE, 4) == 0)
		{
			errcode = PLTEHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, bKGD, 4) == 0)
		{
			errcode = bKGDHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, hIST, 4) == 0)
		{
			errcode = hISTHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, tRNS, 4)  == 0)
		{
//			TRANSINFO *TransInfo = (TRANSINFO *)malloc(sizeof(TRANSINFO));
			errcode = tRNSHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, pHYs, 4) == 0)
		{
			errcode = pHYsHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, sPLT, 4) == 0)
		{
			errcode = sPLTHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if ( memcmp(&ChunkType, IDAT, 4) == 0)
		{
			errcode = IDATHandle(&PngFileContent, ChunkLength, PNGOutBuffer, OutBufferSize,ox,oy);
			if ( errcode != 0)
			{
				return errcode;
			}

		   // *ImgWidth = (UINT32)(((png_resized_width << 4) + 31) >> 5 ) << 1;
		   // *ImgHeigh = (UINT32)png_resized_height;
			dest_line_start_ptr = *PNGOutBuffer;

		}
		else if (memcmp(&ChunkType, tIME, 4) == 0)
		{
			errcode = tIMEHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, iTXt, 4) == 0)
		{
			errcode = iTXtHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, tEXt, 4) == 0)
		{
			errcode = tEXtHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, zTXt, 4) == 0)
		{
			errcode = zTXtHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
		}
		else if (memcmp(&ChunkType, IEND, 4) == 0)
		{
			errcode = IENDHandle(&PngFileContent, ChunkLength);
			if ( errcode != 0)
			{
				return errcode;
			}
			
//IMGDP_EXIT(10020);

			return 0;
		}
		else
		{
			errcode = UnknownHandle(&PngFileContent, ChunkLength);
			if (errcode != 0)
			{
				return errcode;
			}
		}
	}
	

	return 0;
}


VOID PNG_Init(VOID){
	
	 //INIT  PNG文件中出现的各个chunk
/*	 FileChunks.bKGDNuM=0;
	 FileChunks.cHRMNum=0;  
	 FileChunks.gAMANum=0;  
	 FileChunks.hISTNum=0;  
	 FileChunks.iCCPNum=0;  
	 FileChunks.IDATNum=0;  
	 FileChunks.IENDNum=0;  
	 FileChunks.IHDRNum=0;  
	 FileChunks.iTXtNum=0;  
	 FileChunks.pHYsNum=0;  
	 FileChunks.PLTENum=0;  
	 FileChunks.sBITNum=0;  
	 FileChunks.sPLTNum=0;  
	 FileChunks.sRGBNum=0;  
	 FileChunks.tEXtNum=0;  
	 FileChunks.tIMENum=0;  
	 FileChunks.tRNSNum=0;  
	 FileChunks.zTXtNum=0; 
*/
	
	memset(&FileChunks,0,sizeof(FILECHUNKS)); 
	transform=0;
	TransNum=0;
 	IDATSize=0;
	PNGzstream.next_in=NULL;
	PNGzstream.avail_in=0;
	PNGzstream.avail_out=0;
	PNGzstream.total_in=0;
	PNGzstream.total_out=0;
	PNGzstream.adler=0;
	PNGzstream.state=Z_NULL;
	PNGzstream.msg=NULL;
	PNGzstream.next_out=NULL;
	PNGzstream.data_type=0;
	PNGzstream.zalloc=NULL;
	PNGzstream.zfree=NULL;
	PNGzstream.opaque=NULL;
	PNGzstream.adler=0;
	PNGzstream.reserved=0;
	
	CurRowNumber=0;            
	CurPassRowNum=0;           
	CurPassRowBytes=0;         
	CurInterlaceRowWidth=0;    
	CurPass=0;     

	memset(&bKGDInfo,0,sizeof(bKGDINFO)); 
	memset(&IHDRInfo,0,sizeof(IHDRCHUNK)); 

	tRNSIndex = NULL; 
	RowBytes=0;
	transform=0;
	ImagePass=0;
	PixelDepth=0; 
//	png_resized_width=0;
//	png_resized_height=0;
	ResizePng_Need=0;
		
	RowBuffer = NULL;        
	PreRow = NULL;           
	FinalRowData = NULL;     
       memset(RowBytesTempArray,0,(MaxWidth << 2) + 32);
	memset(RowBytesArray,0,(MaxWidth << 2) + 2);
       memset(FinalRowArray,0,(MaxWidth << 2) + 32);

}

#if 0
#if (CSW_EXTENDED_API_LCD == 1)
#include "lcdd_m.h"
#include "mci_lcd.h"
    LCDD_FBW_T png_displayFbw;
   uint16  *png_backbuffer;
   uint16  *png_blendBuffer;
VOID lcd_png_BlockWriteBypass(UINT16* buffer, UINT16 startx,UINT16 starty,UINT16 endx,UINT16 endy)
{
   if(png_backbuffer)
   	{
      memcpy((void*)png_backbuffer,(void*)buffer,MMC_LcdWidth*MMC_LcdHeight*2);
   	}
}
#endif



void PNG_blendbg(uint16* bgbuf, uint16* pngbuf, UINT32 png_width, UINT32 png_height,uint16 preview_startx,uint16 preview_starty){

	UINT32 i=0;
	uint16* bg_row_start=bgbuf + MMC_LcdWidth * preview_starty;
	uint16 bg_line_offset=preview_startx+10;

	for(i=0;i<png_height;++i)
		memcpy(bg_row_start+(MMC_LcdWidth*i+bg_line_offset),pngbuf+(png_width*i),png_width<<1);

}


extern char * mmc_MemMalloc(int32 nsize);
extern void mmc_MemFreeAll();
extern uint32 get_lcd_frame_buffer_address(void);

int PNG_display(unsigned short preview_startx,unsigned short preview_starty){

	int err_decode=0;

       BYTE *pFileContent = NULL;
	BYTE *OutBuffer = NULL;
       UINT32 OutBufferSize;
	UINT32 Width;
	UINT32 Height;

	
       pFileContent = FileArray;


	pFileContent = FileArray;

	err_decode = PNGDecode(pFileContent, &OutBuffer, &OutBufferSize,  &Width, &Height);

	mmi_trace(1,"---png decoder is over:%d!---",err_decode);
	 png_backbuffer =(UINT16*)mmc_MemMalloc(MMC_LcdWidth*MMC_LcdHeight*2);
	 png_blendBuffer =(UINT16*)get_lcd_frame_buffer_address();

       memcpy(png_backbuffer,png_blendBuffer,MMC_LcdWidth*MMC_LcdHeight*2);
	if(err_decode==0){ 
		 mmi_trace(1,"---png width:%d and height:%d!---",Width,Height);
	        PNG_blendbg(png_backbuffer,(uint16*)OutBuffer,Width,Height,preview_startx,preview_starty);
		}
	
#if (CSW_EXTENDED_API_LCD == 1)
        // Initialize Frame Buffer Window
        // Buffer pointer will be set at the time of display
        // Other parameters are constant
        png_displayFbw.fb.buffer = png_backbuffer;
        png_displayFbw.fb.width = MMC_LcdWidth;
        png_displayFbw.fb.height = MMC_LcdHeight;
        png_displayFbw.fb.colorFormat = LCDD_COLOR_FORMAT_RGB_565;
        png_displayFbw.roi.width = MMC_LcdWidth;
        png_displayFbw.roi.height = MMC_LcdHeight;
        png_displayFbw.roi.x = 0;
        png_displayFbw.roi.y = 0;

        // Start Bypass every time previewstatus is set to 1
        // Stop it when it is set to 0
        //  mci_LcdStartBypass(lcd_png_BlockWriteBypass);
#endif
        // Display the blended image
        lcdd_Blit16(&png_displayFbw,0 ,0);

	 mmc_MemFreeAll();
	
	return err_decode;
	

}

#endif

#ifdef SHEEN_VC_DEBUG

#pragma pack(1)
/*BMP文件头*/
typedef struct
{
	U16 bfType;          //文件类型，2Bytes，值为ox4D42(字符BM)
	UINT32 bfSize;         // 4Bytes,整个文件的大小，单位为字节
	U16 bfReserved1;     //保留，2Bytes，置位为0
	U16 bfReserved2;     //保留，2Bytes，置位为0
	UINT32 bfOffBits;      //从文件头开始到实际图像数据之间的字节偏移量，4Bytes
} BITMAPFILEHEADER;


/*BMP文件信息头*/
typedef struct  
{
	UINT32 biSize;             //BMP信息头大小(字节数)，4Bytes
	INT32 biWidth;             //图像的宽度，以像素为单位，4Bytes
	INT32 biHeight;            //图像的高度，以像素为单位，4Bytes
	U16 biPlanes;            //说明目标设备位面数，固定值为1，2Bytes
	U16 biBitCount;          //每像素比特数，2Bytes，其值为：1、4、8、16、24、32
	UINT32 biCompression;      //图像数据压缩方法，4Bytes
	UINT32 biSizeImage;        //图像的大小(字节数)，4Bytes，该数必须是4的整数倍，当使用BI_RGB格式时，可置为0
	INT32 biXPelsPerMeter;     //水平分辨率，4Bytes，单位：像素/米
	INT32 biYPelsPerMeter;     //垂直分辨率，4Bytes，单位：像素/米
	UINT32 biClrUsed;          //实际使用的色表中的颜色索引数,4Bytes
	UINT32 biClrImportant;     //对图像显示有重要影响的颜色索引的数目,4Bytes
} BITMAPINFOHEADER;
#pragma pack()


unsigned char RGB565Pal[16] = {0x00, 0xf8, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00,
			                  0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//rgb565 需要写此16字节

FS_HANDLE	PngFileHandle=0;
Png_StreamBuf PngStreamBuf;


void main()
{
	FILE *fIn=fopen("F:\\测试文件\\PNG\\call_center.png","rb");
	FILE *fOut=fopen("F:\\测试文件\\PNG\\test.bmp","wb");
	char *bufOut=0;
	unsigned int outbufsize;
	unsigned int w,h;
	BITMAPFILEHEADER *mypbmfh;
	BITMAPINFOHEADER *mypbmih;


	bufOut=malloc(40*1024);
	PngStreamBuf.PNGSTREAM = malloc(40*1024) ;
	PngStreamBuf.byteleft=fread(PngStreamBuf.PNGSTREAM,1,40*1024,fIn);
	png_resized_width = 0;
	png_resized_height = 0;

	gdi_act_layer=malloc(sizeof(gdi_layer_struct));
	gdi_act_layer->bits_per_pixel=16;
	gdi_act_layer->clipx1=0;
	gdi_act_layer->clipx2=39;
	gdi_act_layer->clipy1=0;
	gdi_act_layer->clipy2=39;
	gdi_act_layer->height=40;
	gdi_act_layer->width=40;
	gdi_act_layer->buf_ptr=malloc(gdi_act_layer->height*gdi_act_layer->width*2);//rgb565 background
	memset(gdi_act_layer->buf_ptr,0, gdi_act_layer->height*gdi_act_layer->width*2);//black

	//rgb565 decode to background buffer gdi_act_layer->buf_ptr, not use bufOut.
	MMF_PngDecodePicture(PngStreamBuf.PNGSTREAM,&bufOut,&outbufsize,&w,&h,0,0);

	//bmp header
	mypbmfh = ( BITMAPFILEHEADER *)malloc(14);
	mypbmfh->bfType = 0x4d42;
	mypbmfh->bfReserved1 = 0;
	mypbmfh->bfReserved2 = 0;
	mypbmfh->bfSize = 70 + outbufsize;
	mypbmfh->bfOffBits = 70;
	
	mypbmih = (BITMAPINFOHEADER *)malloc(40);
	mypbmih->biSize = 40;
	mypbmih->biWidth =w;
	mypbmih->biHeight =-((long)h);//加-号可以使bmp图像上下倒转显示方式（原本bmp存储时上下倒转）
	mypbmih->biPlanes = 1;
	mypbmih->biBitCount = 16;
	mypbmih->biCompression = 3;//rgb565 写0颜色不正确。
	mypbmih->biSizeImage= outbufsize;
	mypbmih->biXPelsPerMeter= 0;
	mypbmih->biYPelsPerMeter= 0;
	mypbmih->biClrUsed= 0;
	mypbmih->biClrImportant= 0;
	
	fwrite(mypbmfh, 14, 1, fOut);
	fwrite(mypbmih, 40, 1, fOut);
	fwrite(&RGB565Pal, 16, 1, fOut);//rgb565 需要写此16字节
	//bmp header end

	fwrite(gdi_act_layer->buf_ptr,w*h*2,1,fOut);

	fclose(fIn);
	fclose(fOut);
	free(bufOut);
	free(PngStreamBuf.PNGSTREAM);
}
#endif

