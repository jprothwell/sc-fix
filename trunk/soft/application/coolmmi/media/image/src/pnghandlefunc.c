

#include "string.h"

#ifndef SHEEN_VC_DEBUG
//#include <memory.h>
#include "cos.h"

#include "cs_types.h"
//#include "dsm_cf.h"
#include "ts.h"
#endif

#include "zlib.h"
#include "png.h"
#include "mmi_png_dec.h"

#ifndef SHEEN_VC_DEBUG
#include "fs_type.h"
#include "oslmemory.h"
#include "mmi_trace.h"
#include "imagep_debug.h"
#include "gdi_include.h"
#endif

IHDRCHUNK IHDRInfo;   // IHDR信息
UINT8 ImgChannels;    // 图像通道数
INT32 gammer;         // 图像gammer值
PLTECHUNK Palete[256];   // 图像色表
UINT32 PlteSize;         // 色表大小
UINT32 EntryNum;         // 色表项数
pHYsINFO pHYsInfo;       // pHYs信息
FILECHUNKS FileChunks;   // PNG文件中出现的各个chunk

UINT16 *hISTContent = NULL;
sPLTPALETTE *sPLTPalette = NULL;

UINT16 TransNum;        // 透明色数量
TRANSINFO tRNSInfo;     // 非索引图像tRNS信息
BYTE *tRNSIndex = NULL; // 索引图像透明信息
bKGDINFO bKGDInfo;      // 背景块信息
PLTECHUNK DefaultBkgd;  // 背景
cHRMINFO cHRMInfo;      // cHRM信息
sPLTINFO sPLTInfo;      // sPLT信息
BYTE sRGBRenderContent; // sRGB信息
UINT32 RowBytes;        // 原始图像一行的字节数
UINT32 transform = 0;   // 变换信息
UINT8 ImagePass;        // 图像Pass数
UINT32 IDATSize;        // IDAT块数据大小
UINT8 PixelDepth;       // 像素比特深度
z_stream PNGzstream;

//BYTE BufferArray[MaxWidth * MaxHeight * MaxPixelBytes];
BYTE* RowArray[MaxHeight];

BYTE tRNSIndexArray[256];


extern ROWINFO RowInfo;
extern UINT32 NewRowBytes;
extern BYTE *RowBuffer;         // 行缓冲区
extern BYTE *PreRow;            // 前一行数据
extern BYTE *FinalRowData;
extern Png_StreamBuf PngStreamBuf;
extern FS_HANDLE	PngFileHandle;
extern int png_resized_width;
extern int png_resized_height;
extern int ResizePng_Need;
extern unsigned char png_only_decode;

extern U8	*dest_line_start_ptr;        //合并a层后的输出起始地址
extern U8	*alpha_dest_row_ptr;         //合并a层后的输出行起始地址
extern U8	*alpha_dest_output_ptr;      //合并a层后的输出临时地址
extern U8	*alpha_dest_output_ptr1;      //合并a层后的输出临时地址
extern U8	*alpha_dest_output_ptr2;      //合并a层后的输出临时地址
int IHDRHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 IHDRcrc = 0;
	UINT32 FileIHDRcrc = 0;

	FileChunks.IHDRNum++;

	if (FileChunks.IHDRNum > 1 || ChunkLength != 13)
	{
		return IHDRERR;
	}

	IHDRcrc = crc32(IHDRcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -=4;
	FileIHDRcrc = (*(*PngFileContent + 13) << 24) + ((*(*PngFileContent + 14)) << 16)
			+ ((*(*PngFileContent + 15)) << 8) + (*(*PngFileContent + 16));

	if ( IHDRcrc != FileIHDRcrc)
	{
		return IHDRCRCERR;
	}

	IHDRInfo.Width = (*(*PngFileContent) << 24) + ((*(*PngFileContent + 1)) << 16)
			+ ((*(*PngFileContent + 2)) << 8) + (*(*PngFileContent + 3));
	IHDRInfo.Height = (*(*PngFileContent + 4) << 24) + ((*(*PngFileContent + 5)) << 16)
			+ ((*(*PngFileContent + 6)) << 8) + (*(*PngFileContent + 7));
	IHDRInfo.BitDepth = (*PngFileContent)[8];
	IHDRInfo.ColorType = (*PngFileContent)[9];
	IHDRInfo.Compression = (*PngFileContent)[10];
	IHDRInfo.Fileter = (*PngFileContent)[11];
	IHDRInfo.Interlace = (*PngFileContent)[12];

	switch(IHDRInfo.ColorType)
	{
	case GreyScale:
	case IndexColor:
		ImgChannels = 1;
		break;

	case TrueColor:
		ImgChannels = 3;
		break;

	case GreyScaleAlpha:
		ImgChannels = 2;	
		break;

	case TrueColorAlpha:
		ImgChannels = 4;	
		break;
	}

	PixelDepth = IHDRInfo.BitDepth * ImgChannels;

	RowBytes = ROWBYTES(PixelDepth, IHDRInfo.Width);

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=(ChunkLength +4);
	return 0;
}

int cHRMHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 cHRMcrc = 0;
	UINT32 FilecHRMcrc = 0;
	
	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before cHRM!");
		return cHRMERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid cHRM after PLTE!");
		return cHRMERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild cHRM after IDAT!");
		return cHRMERR;
	}
	else if (FileChunks.cHRMNum > 1)	
	{
		//printf("Duplicate cHRM chunk!");
		return cHRMERR;
	}
	else if (ChunkLength != 32)
	{
		//printf("Invalid cHRM chunk length!");
		return cHRMERR;
	}
	
	FileChunks.cHRMNum++;
	
	cHRMcrc = crc32(cHRMcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -=4;
	
	FilecHRMcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (cHRMcrc != FilecHRMcrc)
	{
		return cHRMCRCERR;
	}

	if (FileChunks.sRGBNum == 0)
	{
		cHRMInfo.WhiteX = (*(*PngFileContent) << 24) + ((*(*PngFileContent + 1)) << 16)
			+ ((*(*PngFileContent + 2)) << 8) + (*(*PngFileContent + 3));
		cHRMInfo.WhiteY = ((*(*PngFileContent + 4)) << 24) + ((*(*PngFileContent + 5)) << 16)
			+ ((*(*PngFileContent + 6)) << 8) + (*(*PngFileContent + 7));
		cHRMInfo.RedX = ((*(*PngFileContent + 8)) << 24) + ((*(*PngFileContent + 9)) << 16)
			+ ((*(*PngFileContent + 10)) << 8) + (*(*PngFileContent + 11));
		cHRMInfo.RedY = ((*(*PngFileContent + 12)) << 24) + ((*(*PngFileContent + 13)) << 16)
			+ ((*(*PngFileContent + 14)) << 8) + (*(*PngFileContent + 15));
		cHRMInfo.GreenX = ((*(*PngFileContent + 16)) << 24) + ((*(*PngFileContent + 17)) << 16)
			+ ((*(*PngFileContent + 18)) << 8) + (*(*PngFileContent + 19));
		cHRMInfo.GreenY = ((*(*PngFileContent + 20)) << 24) + ((*(*PngFileContent + 21)) << 16)
			+ ((*(*PngFileContent + 22)) << 8) + (*(*PngFileContent + 23));
		cHRMInfo.BlueX = ((*(*PngFileContent + 24)) << 24) + ((*(*PngFileContent + 25)) << 16)
			+ ((*(*PngFileContent + 26)) << 8) + (*(*PngFileContent + 27));
		cHRMInfo.BlueY =((*(*PngFileContent + 28)) << 24) + ((*(*PngFileContent + 29)) << 16)
			+ ((*(*PngFileContent + 30)) << 8) + (*(*PngFileContent + 31));
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int gAMAHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 gAMAcrc = 0;
	UINT32 FilegAMAcrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before gAMA!");
		return gAMAERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid gAMA after PLTE!");
		return gAMAERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild gAMA after IDAT!");
		return gAMAERR;
	}
	else if (FileChunks.gAMANum > 1)	
	{
		//printf("Duplicate gAMA chunk!");
		return gAMAERR;
	}
	else if (ChunkLength != 4)
	{
		//printf("Invalid gAMA chunk length!");
		return gAMAERR;
	}
		
	FileChunks.gAMANum++;

	gAMAcrc = crc32(gAMAcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilegAMAcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));

	if (gAMAcrc != FilegAMAcrc)
	{
		return gAMACRCERR;
	}

	if (FileChunks.sRGBNum == 0)
	{
		gammer = (*(*PngFileContent) << 24) + (*((*PngFileContent) + 1) << 16)
			+ (*((*PngFileContent) + 2) << 8) + (*((*PngFileContent) + 3));
		
		if ( gammer > 0x7fffffffL)
		{
			gammer = 0x7fffffffL;
		}
		else if (gammer < 0)
		{
			gammer = 0;
		}
	}

	*PngFileContent += 4 + ChunkLength;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int iCCPHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 iCCPcrc = 0;
	UINT32 FileiCCPEcrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before iCCP!");
		return iCCPERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid iCCP after PLTE!");
		return iCCPERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild iCCP after IDAT!");
		return iCCPERR;
	}
	else if (FileChunks.iCCPNum > 1)	
	{
		//printf("Duplicate iCCP chunk!");
		return iCCPERR;
	}
	else if (FileChunks.sRGBNum != 0)
	{
		//printf("There is a sRGB chunk!");
		return iCCPERR;
	}

	FileChunks.iCCPNum++;

	iCCPcrc = crc32(iCCPcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FileiCCPEcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (iCCPcrc != FileiCCPEcrc)
	{
		return iCCPCRCERR;
	}

// ..................

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int sBITHandle(BYTE **PngFileContent, UINT32 ChunkLength, sBITInfo *SigBit)
{
	UINT32 sBITcrc = 0;
	UINT32 FilesBITcrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before sBIT!");
		return sBITERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid sBIT after PLTE!");
		return sBITERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild sBIT after IDAT!");
		return sBITERR;
	}
	else if (FileChunks.sBITNum > 1)	
	{
		//printf("Duplicate sBIT chunk!");
		return sBITERR;
	}

	FileChunks.sBITNum++;

	sBITcrc = crc32(sBITcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilesBITcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (sBITcrc != FilesBITcrc)
	{
		return sBITCRCERR;
	}

	if (IHDRInfo.ColorType == GreyScale)
	{
		if (ChunkLength != 1)
		{
			//printf("Invalid sBIT chunk length!");
			return sBITERR;
		}
		
		SigBit->grey = **PngFileContent;
	}
	else if (IHDRInfo.ColorType == TrueColor || IHDRInfo.ColorType == IndexColor)
	{
		if (ChunkLength != 3)
		{
			//printf("Invalid sBIT chunk length!");
			return sBITERR;
		}

		SigBit->red = **PngFileContent;
		SigBit->green = *(*PngFileContent + 1);
		SigBit->blue = *(*PngFileContent + 2);
	}
	else if (IHDRInfo.ColorType == GreyScaleAlpha)
	{
		if (ChunkLength != 2)
		{
			//printf("Invalid sBIT chunk length!");
			return sBITERR;
		}

		SigBit->grey = **PngFileContent;
		SigBit->alpha = *(*PngFileContent + 1);
	}
	else if (IHDRInfo.ColorType == TrueColorAlpha)
	{
		if (ChunkLength != 4)
		{
			//printf("Invalid sBIT chunk length!");
			return sBITERR;
		}

		SigBit->red = **PngFileContent;
		SigBit->green = *(*PngFileContent + 1);
		SigBit->blue = *(*PngFileContent + 2);
		SigBit->alpha = *(*PngFileContent + 3);
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int sRGBHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 sRGBcrc = 0;
	UINT32 FilesRGBcrc = 0;
	
	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before sRGB!");
		return sRGBERR;
	}
	else if (FileChunks.PLTENum != 0)
	{
		//printf("Invalid sRGB after PLTE!");
		return sRGBERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invaild sRGB after IDAT!");
		return sRGBERR;
	}
	else if (FileChunks.sRGBNum > 1)	
	{
		//printf("Duplicate sRGB chunk!");
		return sRGBERR;
	}
	else if (FileChunks.iCCPNum != 0)
	{
		//printf("There is a iCCP chunk!");
		return sRGBERR;
	}
	else if (ChunkLength != 1)
	{
		//printf("Invalid chunk length!");
		return sRGBERR;
	}
		
	FileChunks.sRGBNum++;
	
	sRGBcrc = crc32(sRGBcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilesRGBcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (sRGBcrc != FilesRGBcrc)
	{
		return sRGBCRCERR;
	}

	sRGBRenderContent = **PngFileContent;

	if (sRGBRenderContent > sRGBRenderingLast)
	{
		//printf("Unknown rendering content!");
		return sRGBERR;
	}

	gammer = 45455;

	cHRMInfo.WhiteX = 31270;
	cHRMInfo.WhiteY = 32900;
	cHRMInfo.RedX = 64000;
	cHRMInfo.RedY = 33000;
	cHRMInfo.GreenX = 30000;
	cHRMInfo.GreenY = 60000;
	cHRMInfo.BlueX = 15000;
	cHRMInfo.BlueY = 6000;

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int PLTEHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 PLTEcrc = 0;
	UINT32 FilePLTEcrc = 0;
	UINT32 i;
	
	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before PLTE!");
		return PLTEERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid PLTE after IDAT!");
		return PLTEERR;
	}
	else if (FileChunks.PLTENum > 1)
	{
		//printf("Duplicate PLTE chunk!");
		return PLTEERR;
	}
	else if (ChunkLength > 768 || ChunkLength % 3)
	{
		//printf("Invalid PLTE chunk length!");
		return PLTEERR;
	}

	FileChunks.PLTENum++;
	
	PLTEcrc = crc32(PLTEcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilePLTEcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (PLTEcrc != FilePLTEcrc)
	{
		return PLTECRCERR;
	}

	EntryNum = ChunkLength / 3;
	for (i = 0; i < EntryNum; i++)
	{
		Palete[i].Red = *(*PngFileContent)++;
		Palete[i].Green = *(*PngFileContent)++;
		Palete[i].Blue = *(*PngFileContent)++;
	}

	PlteSize = ChunkLength;
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	return 0;
}

int bKGDHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 bKGDcrc = 0;
	UINT32 FilebKGDcrc = 0;
	UINT32 TrueLength = 0;
	
/*	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before bKGD!");
		return bKGDERR;
	}
	else if (FileChunks.PLTENum == 0)
	{
		//printf("Missing PLTE before bKGD!");
		return bKGDERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid bKGD after IDAT!");
		return bKGDERR;
	}
	else if (FileChunks.bKGDNuM > 1)
	{
		//printf("Duplicate bKGD chunk!");
		return bKGDERR;
	}
*/	
	FileChunks.bKGDNuM++;
	
	bKGDcrc = crc32(bKGDcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilebKGDcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (bKGDcrc != FilebKGDcrc)
	{
		return bKGDCRCERR;
	}

//	bKGDInfo = (bKGDINFO *)OslMalloc(9);

	if (IHDRInfo.ColorType == GreyScale || IHDRInfo.ColorType == GreyScaleAlpha)
	{
		TrueLength = 2;
		if (TrueLength != ChunkLength)
		{
			return bKGDERR;
		}
		bKGDInfo.bKGDRed =
		bKGDInfo.bKGDGreen =
		bKGDInfo.bKGDBlue =
		bKGDInfo.bKGDGrey = (*(*PngFileContent) << 8) + *((*PngFileContent) + 1);
	}
	else if (IHDRInfo.ColorType == TrueColor || IHDRInfo.ColorType == TrueColorAlpha)
	{
		TrueLength = 6;
		if (TrueLength != ChunkLength)
		{
			return bKGDERR;
		}
		bKGDInfo.bKGDRed = (*(*PngFileContent) << 8) + *((*PngFileContent) + 1);
		bKGDInfo.bKGDGreen = (*((*PngFileContent) + 2) << 8) + *((*PngFileContent) + 3);
		bKGDInfo.bKGDBlue = (*((*PngFileContent) + 4) << 8) + *((*PngFileContent) + 5);
	}
	else if (IHDRInfo.ColorType == IndexColor)
	{
		TrueLength = 1;
		if (TrueLength != ChunkLength)
		{
			return bKGDERR;
		}

		bKGDInfo.bKGDPlteIndex = **PngFileContent;
		if (PlteSize)
		{
			if (bKGDInfo.bKGDPlteIndex > PlteSize)
			{
				//printf("Incorrect bKGD chunk index value!");
				return bKGDERR;
			}

			bKGDInfo.bKGDRed = Palete[bKGDInfo.bKGDPlteIndex].Red;
			bKGDInfo.bKGDGreen = Palete[bKGDInfo.bKGDPlteIndex].Green;
			bKGDInfo.bKGDBlue = Palete[bKGDInfo.bKGDPlteIndex].Blue;
		}
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int hISTHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 hISTcrc = 0;
	UINT32 FilehISTcrc = 0;
	UINT16 hISTEntryNum;
	BYTE buf[2];
	UINT16 i;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before hIST!");
		return hISTERR;
	}
	else if (FileChunks.PLTENum == 0)
	{
		//printf("Missing PLTE before hIST!");
		return hISTERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid hIST after IDAT!");
		return hISTERR;
	}
	else if (FileChunks.hISTNum > 1)
	{
		//printf("Duplicate hIST chunk!");
		return hISTERR;
	}

	FileChunks.hISTNum++;

	hISTcrc = crc32(hISTcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilehISTcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (hISTcrc != FilehISTcrc)
	{
		return hISTCRCERR;
	}

	hISTEntryNum = ChunkLength / 2;
	if (hISTEntryNum != PlteSize || hISTEntryNum > 256)
	{
		//printf("Incorrect hIST chunk length!");
		return hISTERR;
	}

	hISTContent = (UINT16 *)OslMalloc(ChunkLength);

//	memcpy(hISTContent, PngFileContent, ChunkLength);
//	*PngFileContent += ChunkLength + 4;

	for (i = 0; i < hISTEntryNum; i++)
	{
		buf[0] = *(*PngFileContent)++;
		buf[1] = *(*PngFileContent)++;

		hISTContent[i] = (buf[0] << 8) + buf[1]; 
	}

	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	return 0;
}

int tRNSHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 tRNScrc = 0;
	UINT32 FiletRNScrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before tRNS!");
		return tRNSERR;
	}
	else if (FileChunks.PLTENum == 0)
	{
		//printf("Missing PLTE before tRNS!");
		return tRNSERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid tRNS after IDAT!");
		return tRNSERR;
	}
	else if (FileChunks.tRNSNum > 1)
	{
		//printf("Duplicate tRNS chunk!");
		return tRNSERR;
	}

	FileChunks.tRNSNum++;

	tRNScrc = crc32(tRNScrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FiletRNScrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (tRNScrc != FiletRNScrc)
	{	
		return PLTECRCERR;
	}

//	(*TransInfo).TransNum = (UINT16 *)OslMalloc(2);

	if (IHDRInfo.ColorType == GreyScale)
	{
		if (ChunkLength != 2)
		{
			//printf("Invalid tRNS chunk length!");
			return tRNSERR;
		}
		
		TransNum = 1;

		tRNSInfo.tRNSGrey = (**PngFileContent << 8) + *(*PngFileContent + 1);
	}
	else if (IHDRInfo.ColorType == TrueColor)
	{
		if (ChunkLength != 6)
		{
			//printf("Invalid tRNS chunk length!");	
			return tRNSERR;
		}

		TransNum = 1;

		tRNSInfo.tRNSRed = (**PngFileContent << 8) + *(*PngFileContent + 1);
		tRNSInfo.tRNSGreen = (*(*PngFileContent + 2) << 8) + *(*PngFileContent + 3);
		tRNSInfo.tRNSBlue = (*(*PngFileContent + 4) << 8) + *(*PngFileContent + 5);
	}
	else if (IHDRInfo.ColorType == IndexColor)
	{
		if (ChunkLength > PlteSize || ChunkLength == 0)
		{
			//printf("Invalid tRNS chunk length!");	
			return tRNSERR;
		}
		
		TransNum = ChunkLength;

//		tRNSIndex = (BYTE *)OslMalloc(ChunkLength);
		tRNSIndex = tRNSIndexArray;
		memcpy(tRNSIndex, *PngFileContent, ChunkLength);
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int pHYsHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 pHYscrc = 0;
	UINT32 FilepHYscrc = 0;

	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before pHYs!");
		return pHYsERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid pHYs after IDAT!");
		return pHYsERR;
	}
	else if (FileChunks.pHYsNum > 1)
	{
		//printf("Duplicate pHYs chunk!");
		return pHYsERR;
	}
	else if (ChunkLength != 9)
	{
		//printf("Invalid pHYs chunk length!");
		return pHYsERR;
	}
	
	FileChunks.pHYsNum++;

	pHYscrc = crc32(pHYscrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilepHYscrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (pHYscrc != FilepHYscrc)
	{
		return pHYsCRCERR;
	}

	pHYsInfo.xPixPerUint = (*(*PngFileContent) << 24) + (*((*PngFileContent) + 1) << 16)
		+ (*((*PngFileContent) + 2) << 8) + (*((*PngFileContent) + 3));
	pHYsInfo.yPixPerUnit = (*((*PngFileContent) + 4) << 24) + (*((*PngFileContent) + 5) << 16)
		+ (*((*PngFileContent) + 6) << 8) + (*((*PngFileContent) + 7));
	pHYsInfo.UnitSpec = *((*PngFileContent) + 8);

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int sPLTHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 sPLTcrc = 0;
	UINT32 FilesPLTcrc = 0;
//	int i;

//	BYTE *EntryStart = *PngFileContent;
//	UINT32 DataLength = 0;
	
	if (FileChunks.IHDRNum == 0)
	{
		//printf("Missing IHDR before sPLT!");
		return sPLTERR;
	}
	else if (FileChunks.IDATNum != 0)
	{
		//printf("Invalid sPLT after IDAT!");
		return sPLTERR;
	}
	
	FileChunks.sPLTNum++;
	
	sPLTcrc = crc32(sPLTcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilesPLTcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (sPLTcrc != FilesPLTcrc)
	{
	return sPLTCRCERR;
	}
	
#if 0
	for (i = 0; *EntryStart; EntryStart++, i++)
	{
		if (i > 79)
		{
			//printf("sPLT name too long!");
			return sPLTERR;
		}

		sPLTInfo.sPLTPaltName[i] = *EntryStart;
	}

	EntryStart++;
	if (EntryStart > *PngFileContent + ChunkLength)
	{
		//printf("sPLT chunk format incorrect!");
		return sPLTERR;
	}

	sPLTInfo.sPLTSampDepth = *EntryStart++;
	sPLTInfo.sPLTEntrySize = (sPLTInfo.sPLTSampDepth == 8 ? 6 : 10);

	DataLength = ChunkLength - (EntryStart - *PngFileContent);

	if (DataLength % sPLTInfo.sPLTEntrySize)
	{
		//printf("sPLT chunk length is wrong!");
		return sPLTERR;
	}
	
	sPLTInfo.sPLTEntryNum = DataLength / sPLTInfo.sPLTEntrySize;

	if ((sPLTPalette = (sPLTPALETTE *)OslMalloc(DataLength)) == NULL)
	{
		//printf("Fail to alloc memory for sPLTPalette!");
		return sPLTERR;
	}

	for (i = 0; i < sPLTInfo.sPLTEntryNum; i++)
	{
		if (sPLTInfo.sPLTSampDepth == 8)
		{
			sPLTPalette->sPLTRed = (UINT16)(*EntryStart++);
			sPLTPalette->sPLTGreen = (UINT16)(*EntryStart++);
			sPLTPalette->sPLTBlue = (UINT16)(*EntryStart++);
			sPLTPalette->sPLTAlpha = (UINT16)(*EntryStart++);
			sPLTPalette->sPLTFreq = (*EntryStart << 8) + (*(EntryStart + 1));
			EntryStart += 2;
		}
		else
		{
			sPLTPalette->sPLTRed = (*EntryStart << 8) + (*(EntryStart + 1));
			sPLTPalette->sPLTGreen = ((*(EntryStart + 2)) << 8) + (*(EntryStart + 3));
			sPLTPalette->sPLTBlue = ((*(EntryStart + 4)) << 8) + (*(EntryStart + 5));
			sPLTPalette->sPLTAlpha = ((*(EntryStart + 6)) << 8) + (*(EntryStart + 7));
			sPLTPalette->sPLTFreq = ((*(EntryStart + 8)) << 8) + (*(EntryStart + 9));
			EntryStart += 10;
		}
	}
#endif

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

//int IDATHandle(BYTE **PngFileContent, UINT32 ChunkLength, PLTECHUNK **ppBkgColor)
extern INT MMI_FS_Read(FS_HANDLE FileHandle, void *DataPtr, INT32 Lenth, INT32*ReadLen);


int IDATHandle(BYTE **PngFileContent, UINT32 ChunkLength, BYTE **ImagaData, UINT32 *OutBufferSize, S32 ox, S32 oy)
{
//IMGDP_ENTRY(10021);
	BYTE **ImageRowData = NULL;         // 图像行指针
	UINT32 i;
	UINT32 j;
	int ErrCode;
	BYTE **imgp=NULL;
//	int pngfile_offect=0;
	UINT32 IDATcrc = 0;
	UINT32 FileIDATcrc = 0;
//	INT32 templen=0;
	UINT32 ImgRowBytes;
#if 0
	BYTE * OriRowData=NULL;
	INT32 * Valid_Row =NULL;
	BYTE * PreRowDate=NULL;
	BYTE * CurRowDate=NULL;	
#endif	
	UINT8 * OutputBuffer=NULL;	
	U16   *adp;
	U16   *adp1;
	U16 r,g,b,r1,r2,g1,g2,b1,b2;
	UINT32 offsetx,offsety;
	UINT32 Scale,Scaley,alphay,alphax;
	U16 temp,temp1;
	S32	start, end;
	S32	x_draw_count, y_draw_count,offset_start;
	FileChunks.IDATNum++;
	
	IDATcrc = crc32(IDATcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -=4;
	FileIDATcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (IDATcrc != FileIDATcrc)
	{
		return IDATCRCERR;
	}

	IDATSize = ChunkLength;

	if (IHDRInfo.BitDepth == 16)
	{
		transform |= DEPTH16to8;
	}

	if (IHDRInfo.BitDepth < 8)
	{
		transform |= (EXPAND | tRNSEXPAND);
	}

	if (IHDRInfo.ColorType == IndexColor)
	{
		transform |= (EXPAND | tRNSEXPAND);
	}

	if (FileChunks.tRNSNum != 0)
	{
		transform |= (EXPAND | tRNSEXPAND);
	}

	if (IHDRInfo.ColorType == GreyScale || IHDRInfo.ColorType == GreyScaleAlpha)
	{
		transform |= GREY2RGB;
	}

	transform |= BKGDImage;       //set it as the default transform method    

	if (FileChunks.bKGDNuM != 0)
	{
		transform |= bKGDEXPAND;
	}
	else// if(!(transform & BKGDImage))     //else
	{
	/*if there's no defined background colour in bKGDHandle, these code will be executed---zhangl*/	
	//set the default back ground as our image viewer brackground color  elisa 2009-9-29
		DefaultBkgd.Red =   216;
		DefaultBkgd.Green = 240;
		DefaultBkgd.Blue = 248;
       
		transform |= DEFAULTbKGD;
	}

           
/*
	gAMA
*/

	UpdateInfo();
	
	ErrCode = inflateInit(&PNGzstream);
//       *ImagaData = (BYTE *)gdi_image_decoder_frame_buf;//BufferArray;

//	ImgRowBytes = (((IHDRInfo.Width << 4) + 31) >> 5 ) << 2;
	ImgRowBytes = IHDRInfo.Width <<1 ;



	*OutBufferSize = ImgRowBytes *IHDRInfo.Height;

	ImageRowData = (BYTE **)&RowArray;


	for (i = 0; (INT32)i < IHDRInfo.Height; i++)
	{
		ImageRowData[i] = *ImagaData + i * ImgRowBytes;
	}
	if (!png_only_decode)
	{
		if ((OutputBuffer = (U8 *)OslMalloc(*OutBufferSize)) == NULL)
		{
			mmi_trace(1,"Fail to alloc memory for OutputBuffer!");
			return -1;
		}	
	}
	else
	{
		OutputBuffer = *ImagaData;
	}
#if 0		
	if ((PreRowDate = (BYTE *)OslMalloc(ImgRowBytes*2)) == NULL)
	{
		mmi_trace(1,"Fail to alloc memory for PreRowDate!");
		return -1;
	}	
	CurRowDate=PreRowDate+ImgRowBytes;

	if ((OriRowData = (BYTE *)OslMalloc(IHDRInfo.Width << 1 )) == NULL)
//	if ((OriRowData = (BYTE *)OslMalloc((((IHDRInfo.Width << 4) + 31) >> 5 ) << 2)) == NULL)
	{
		mmi_trace(1,"Fail to alloc memory for OriRowData!");
		return -1;
	}	

	if ((Valid_Row = (INT32 *)OslMalloc(png_resized_height* sizeof(INT32))) == NULL)
	{
		mmi_trace(1,"Fail to alloc memory for OriRowData!");
		return -1;
	}	
	memset(Valid_Row,0,png_resized_height);

	for(i = 0; (INT32)i < png_resized_height; i++)
		Valid_Row[i]=(i*(IHDRInfo.Height<<PNGS8)/png_resized_height)>>PNGS8;
#endif


	if (IHDRInfo.Interlace)
	{
		transform |= INTERLACE;
		ImagePass = 7;
	}
	else
		ImagePass = 1;
	alpha_dest_output_ptr1 = (U8*)OutputBuffer;
	alpha_dest_output_ptr2	= (U8*)(gdi_act_layer->buf_ptr+(((gdi_act_layer->width*oy+ox)*GDI_LAYER.bits_per_pixel)>>3));
	imgp=ImageRowData;	

//  mmi_trace(1,"gdi_image png start  alpha_dest_row_ptr%d    ",alpha_dest_row_ptr);
//  mmi_trace(1,"gdi_image png start  dest_line_start_ptr%d    ",dest_line_start_ptr);

	for (i = 0; i < ImagePass; i++)
	{

		
			for (j = 0; j < IHDRInfo.Height; j++)
			{
				alpha_dest_row_ptr	= (U8*)(alpha_dest_output_ptr2+((gdi_act_layer->width*j*GDI_LAYER.bits_per_pixel)>>3));
//				alpha_dest_row_ptr	= (U8*)(dest_line_start_ptr+((gdi_act_layer->width*j*GDI_LAYER.bits_per_pixel)>>3));
				alpha_dest_output_ptr = (U8*)(alpha_dest_output_ptr1+ImgRowBytes*j);
				ErrCode = ProcessOneRow(PngFileContent, *imgp);
				imgp++;
			}

	}
//add by guosz
	start	= (ox < (S32)gdi_act_layer->clipx1)							? gdi_act_layer->clipx1 : ox;
	end	= (ox+(S32)(png_resized_width)-1 > (S32)gdi_act_layer->clipx2)	? gdi_act_layer->clipx2 : ox+(S32)(png_resized_width)-1;
	x_draw_count = end-start+1;
	offset_start = start - ox;

	start	= (oy < (S32)gdi_act_layer->clipy1)							? gdi_act_layer->clipy1 : oy;
	end	= (oy+(S32)(png_resized_height)-1 > (S32)gdi_act_layer->clipy2)	? gdi_act_layer->clipy2 : oy+(S32)(png_resized_height)-1;
	y_draw_count = end-start+1;
//	  mmi_trace(1,"gdi_image png start  ox  %d    ",ox);
 //   mmi_trace(1,"gdi_image png start  oy  %d    ",oy);
//	  mmi_trace(1,"gdi_image png start  gdi_act_layer->clipx1  %d    ",gdi_act_layer->clipx1);
 //   mmi_trace(1,"gdi_image png start  gdi_act_layer->clipy1  %d    ",gdi_act_layer->clipy1);
//	  mmi_trace(1,"gdi_image png start  gdi_act_layer->clipx2  %d    ",gdi_act_layer->clipx2);
 //   mmi_trace(1,"gdi_image png start  gdi_act_layer->clipy2  %d    ",gdi_act_layer->clipy2);
//	  mmi_trace(1,"gdi_image png start  x_draw_count  %d    ",x_draw_count);
//    mmi_trace(1,"gdi_image png start  y_draw_count  %d    ",y_draw_count);


	if(x_draw_count > png_resized_width)
		x_draw_count = png_resized_width;
	if(y_draw_count > png_resized_height)
		y_draw_count = png_resized_height;
//	  mmi_trace(1,"gdi_image png start  x_draw_count  %d    ",x_draw_count);
//    mmi_trace(1,"gdi_image png start  y_draw_count  %d    ",y_draw_count);

	adp = (U16*)alpha_dest_output_ptr1;
	if(oy <(gdi_act_layer->clipy1))
	{
		start =gdi_act_layer->clipy1- oy;
		adp = (U16*)(alpha_dest_output_ptr1+start * IHDRInfo.Width * 2);
//	  mmi_trace(1,"gdi_image png start  start  %d    ",start);
    if(y_draw_count>(png_resized_height-start))
      y_draw_count = png_resized_height-start;
	}
	Scaley=(IHDRInfo.Height<<16)/png_resized_height;
	Scale=(IHDRInfo.Width<<16)/png_resized_width;
	if ( Scaley > Scale)
		Scale = Scaley;
	
	if(ResizePng_Need)
	{
		for(i=0;i<(UINT32)y_draw_count;i++)
		{
			alpha_dest_row_ptr	= (U8*)(dest_line_start_ptr+((gdi_act_layer->width*i*GDI_LAYER.bits_per_pixel)>>3));
			offsety = ((i*Scale)>>16)+1;
			alphay = (i*Scale)&0xffff;
			if ( offsety > (IHDRInfo.Height-1))
				offsety = IHDRInfo.Height-1;

			for(j=0;j<(UINT32)x_draw_count;j++)
			{
				temp1 =0;
				offsetx = (((j+offset_start)*Scale)>>16)+1;
				alphax = ((j+offset_start)*Scale)&0xffff;
				if ( offsetx > (IHDRInfo.Width-1))
					offsetx = IHDRInfo.Width-1;
				adp1 = adp + offsety * IHDRInfo.Width + offsetx;
				r1 = (*(adp1)& 0xf800 )>>8;
				g1 = ((*(adp1)& 0x7E0 )>>3);
				b1 = (*adp1  & 0x1F) << 3;
				r2 = (*(adp1-1)& 0xf800 )>>8;
				g2 = ((*(adp1-1)& 0x7E0 )>>3);
				b2 = (*(adp1-1)  & 0x1F) << 3;
				r = (r1 * alphax + r2 * (0x10000 - alphax)+32768) >>16;
				g = (g1 * alphax + g2 * (0x10000 - alphax)+32768) >>16;
				b = (b1 * alphax + b2 * (0x10000 - alphax)+32768) >>16;

				adp1 = adp1 - IHDRInfo.Width;
				r1 = (*(adp1)& 0xf800 )>>8;
				g1 = ((*(adp1)& 0x7E0 )>>3);
				b1 = (*adp1  & 0x1F) << 3;
				r2 = (*(adp1-1)& 0xf800 )>>8;
				g2 = ((*(adp1-1)& 0x7E0 )>>3);
				b2 = (*(adp1-1)  & 0x1F) << 3;
				r1 = (r1 * alphax + r2 * (0x10000 - alphax)+32768) >>16;
				g1 = (g1 * alphax + g2 * (0x10000 - alphax)+32768) >>16;
				b1 = (b1 * alphax + b2 * (0x10000 - alphax)+32768) >>16;
				r = (r * alphay + r1 * (0x10000 - alphay)+32768) >>16;
				g = (g * alphay + g1 * (0x10000 - alphay)+32768) >>16;
				b = (b * alphay + b1 * (0x10000 - alphay)+32768) >>16;
				temp = ((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3));
				if(temp == 0x001f)
					temp = 0x001e;
//				if(temp == 0)
//					temp = 0xffff;
				/* why slip 0xdf9f ??! sheen
				if(temp == 0xdf9f)
				{
//				*alpha_dest_row_ptr = 0x0;
				alpha_dest_row_ptr++;
//				*alpha_dest_row_ptr = 0x0;
				alpha_dest_row_ptr++;
				}
				else
				{*/
				*alpha_dest_row_ptr = temp&0xff;
				alpha_dest_row_ptr++;
				*alpha_dest_row_ptr = temp>>8;
				alpha_dest_row_ptr++;
				//}
			}
		}
	}
	else
	{
		for(i=0;i<(UINT32)y_draw_count;i++)
		{
			alpha_dest_row_ptr	= (U8*)(dest_line_start_ptr+((gdi_act_layer->width*i*GDI_LAYER.bits_per_pixel)>>3));
			adp1 = adp + i * IHDRInfo.Width + offset_start;

			for(j=0;j<(UINT32)x_draw_count;j++)
			{
				/* why skip 0xdf9f ???? ! sheen
				if(*adp1 == 0xdf9f)
//				if(abs(temp - 0xdf9f)<0x10)
				{
//				*alpha_dest_row_ptr = 0x0;
				alpha_dest_row_ptr++;
//				*alpha_dest_row_ptr = 0x0;
				alpha_dest_row_ptr++;
				adp1++;

				}
				else
			{
			*/
				*alpha_dest_row_ptr = (*adp1)&0xff;
				alpha_dest_row_ptr++;
				*alpha_dest_row_ptr = (*adp1)>>8;
				alpha_dest_row_ptr++;
				adp1++;
				//}
			}
		}

	}


//	*myImgChannels = RowInfo.Channels;
//	OslMfree(ImageRowData);
//	OslMfree(FinalRowData);
//IMGDP_EXIT(10021);
#if 0
  	OslMfree(PreRowDate);
  	OslMfree(OriRowData);
	OslMfree(Valid_Row);
#endif	
	
	if (!png_only_decode)
	{
		OslMfree(OutputBuffer);
	}
	return ErrCode;
}

int tIMEHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 tIMEcrc = 0;
	UINT32 FiletIMEcrc = 0;

	if (FileChunks.tIMENum > 1)
	{
		//printf("Duplicate tIME chunk!");
		return tIMEERR;
	}
	else if (ChunkLength != 7)
	{
		//printf("Invalid tIME chunk length!");
		return tIMEERR;
	}
		
	FileChunks.tIMENum++;

	tIMEcrc = crc32(tIMEcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FiletIMEcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (tIMEcrc != FiletIMEcrc)
	{
		return tIMECRCERR;
	}	

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int iTXtHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 iTXtcrc = 0;
	UINT32 FileiTXtcrc = 0;

	FileChunks.iTXtNum++;

	iTXtcrc = crc32(iTXtcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FileiTXtcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (iTXtcrc != FileiTXtcrc)
	{
		return iTXtCRCERR;
	}
	
	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int tEXtHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 tEXtcrc = 0;
	UINT32 FiletEXtcrc = 0;

	FileChunks.tEXtNum++;

	tEXtcrc = crc32(tEXtcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FiletEXtcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (tEXtcrc != FiletEXtcrc)
	{
		return tEXtCRCERR;
	}	

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int zTXtHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 zTXtcrc = 0;
	UINT32 FilezTXtcrc = 0;

	FileChunks.zTXtNum++;

	zTXtcrc = crc32(zTXtcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FilezTXtcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (zTXtcrc != FilezTXtcrc)
	{
		return zTXtCRCERR;
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

int IENDHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 IENDcrc = 0;
	UINT32 FileIENDcrc = 0;

//	free(RowBuffer);
//	free(PreRow);

	if (hISTContent != NULL)
		OslMfree(hISTContent);

//	if (tRNSIndex != NULL)
//		OslMfree(tRNSIndex);

	if (sPLTPalette != NULL)
		OslMfree(sPLTPalette);

	if (FileChunks.IENDNum > 1)
	{
		//printf("Duplicate IEND chunk!");
		return IENDERR;
	}
	else if (ChunkLength != 0)
	{
		//printf("Invalid IEND chunk length!");
		return IENDERR;
	}

	FileChunks.IENDNum++;

	IENDcrc = crc32(IENDcrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FileIENDcrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (IENDcrc != FileIENDcrc)
	{
		return IENDCRCERR;
	}

	return 0;
}

int UnknownHandle(BYTE **PngFileContent, UINT32 ChunkLength)
{
	UINT32 Unknowncrc = 0;
	UINT32 FileUnknowncrc = 0;
	
	Unknowncrc = crc32(Unknowncrc, *PngFileContent, ChunkLength + 4);
	*PngFileContent += 4;
	PngStreamBuf.byteleft -= 4;
	FileUnknowncrc = (*((*PngFileContent) + ChunkLength) << 24) + (*((*PngFileContent) + 1 + ChunkLength) << 16)
		+ (*((*PngFileContent) + 2 + ChunkLength) << 8) + (*((*PngFileContent) + 3 +ChunkLength));
	
	if (Unknowncrc != FileUnknowncrc)
	{
		return UnknownCRCERR;
	}

	*PngFileContent += ChunkLength + 4;
	PngStreamBuf.byteleft -=ChunkLength + 4;
	return 0;
}

