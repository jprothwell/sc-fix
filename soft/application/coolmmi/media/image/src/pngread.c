
#include "string.h"

#ifndef SHEEN_VC_DEBUG
//#include <memory.h>
#include "cos.h"

#include "cs_types.h"
//#include "dsm_cf.h"
#include "ts.h"
#endif

#include "png.h"
#include "zlib.h"
#ifndef SHEEN_VC_DEBUG
#include "mmi_trace.h"
#include "oslmemory.h"
#include "fat_fs.h"
#include "imagep_debug.h"
#include "gdi_datatype.h"
#include "gdi_include.h"
#endif

#define composite(composite, fg, alpha, bg)                            \
	(composite) = (BYTE)(((UINT16)(fg) * (UINT16)(alpha) +    \
	(UINT16)(bg) * (UINT16)(255 - (UINT16)(alpha))+(UINT16)128)>>8)

#define composite16(composite, fg, alpha, bg)                         \
	(composite) = (UINT32)(((UINT32)(fg) * (UINT32)(alpha) + \
	(UINT32)(bg)*(UINT32)(65535L - (UINT32)(alpha))+(UINT32)32768)>>16)

extern z_stream PNGzstream;

extern IHDRCHUNK IHDRInfo;
extern UINT32 transform;
extern bKGDINFO bKGDInfo;
extern UINT16 TransNum;
extern TRANSINFO tRNSInfo;
extern BYTE *tRNSIndex;
extern PLTECHUNK Palete[256];
extern UINT32 RowBytes;
//extern bKGDINFO bKGDInfo;
extern INT32 gammer;
extern UINT8 ImagePass;
extern UINT32 IDATSize; 
extern FILECHUNKS FileChunks;
extern UINT8 ImgChannels; 
extern UINT8 PixelDepth;       // 像素比特深度
extern TRANSINFO tRNSInfo;
extern BYTE *tRNSIndex;
extern PLTECHUNK DefaultBkgd;
extern U8 *alpha_dest_output_ptr;
extern int ResizePng_Need;

UINT32 CurRowNumber;            // 当前行号
UINT32 CurPassRowNum;           // 当前Pass的行数
UINT32 CurPassRowBytes;         // 当前行的字节数
UINT32 CurInterlaceRowWidth;    // 当前交织行的宽度
BYTE CurPass;                   // 当前的Pass号
UINT8 NewColorType;             // 变换以后的色彩类型
UINT8 NewBiteDepth;             // 变换以后的比特深度
UINT8 NewPixelDepth;            // 变换以后的像素深度
UINT8 NewTansNum;               // 变换以后的透明色数目
UINT32 NewIntGama;              // 变换以后的gammer
UINT8 NewChannel;               // 变换以后的通道数
UINT32 NewRowBytes;             // 变换以后行的字节数
ROWINFO RowInfo;                // 行信息
extern U8	*alpha_dest_row_ptr;         //合并a层后的输出行起始地址

BYTE *RowBuffer = NULL;         // 行缓冲区
BYTE *PreRow = NULL;            // 前一行数据
BYTE *FinalRowData = NULL;      // 变换处理后的行数据

BYTE RowBytesTempArray[(MaxWidth << 2) + 32];
BYTE RowBytesArray[(MaxWidth << 2) + 2];
BYTE FinalRowArray[(MaxWidth << 2) + 32];

void InitTransform()
{
//	UINT8 colortype = IHDRInfo.ColorType;

	if ((transform & bKGDEXPAND) && (transform & GREY2RGB) &&
		bKGDInfo.bKGDRed == bKGDInfo.bKGDGreen && bKGDInfo.bKGDRed == bKGDInfo.bKGDBlue)
	{
		bKGDInfo.bKGDGrey = bKGDInfo.bKGDRed;
	}

	if ((transform & EXPAND) && (transform & bKGDEXPAND))
	{
		if (!(IHDRInfo.ColorType & 0x02))  // 灰度图或具有alpha通道的灰度图
		{
			switch(IHDRInfo.BitDepth)
			{
			case 1:
				bKGDInfo.bKGDGrey *= (UINT16)0xFF;
				bKGDInfo.bKGDRed = bKGDInfo.bKGDGreen = bKGDInfo.bKGDBlue = bKGDInfo.bKGDGrey;

				if (!(transform & tRNSEXPAND))
				{
					tRNSInfo.tRNSGrey *= (UINT16)0xFF;
					tRNSInfo.tRNSRed = tRNSInfo.tRNSGreen = tRNSInfo.tRNSBlue = tRNSInfo.tRNSGrey;
				}
				break;

			case 2:
				bKGDInfo.bKGDGrey *= (UINT16)0x55;
				bKGDInfo.bKGDRed = bKGDInfo.bKGDGreen = bKGDInfo.bKGDBlue = bKGDInfo.bKGDGrey;
				
				if (!(transform & tRNSEXPAND))
				{
					tRNSInfo.tRNSGrey *= (UINT16)0x55;
					tRNSInfo.tRNSRed = tRNSInfo.tRNSGreen = tRNSInfo.tRNSBlue = tRNSInfo.tRNSGrey;
				}
				break;

			case 4:
				bKGDInfo.bKGDGrey *= (UINT16)0x11;
				bKGDInfo.bKGDRed = bKGDInfo.bKGDGreen = bKGDInfo.bKGDBlue = bKGDInfo.bKGDGrey;
				
				if (!(transform & tRNSEXPAND))
				{
					tRNSInfo.tRNSGrey *= (UINT16)0x11;
					tRNSInfo.tRNSRed = tRNSInfo.tRNSGreen = tRNSInfo.tRNSBlue = tRNSInfo.tRNSGrey;
				}
				break;

			case 8:
			case 16:
				bKGDInfo.bKGDRed = bKGDInfo.bKGDGreen = bKGDInfo.bKGDBlue = bKGDInfo.bKGDGrey;
				break;
				
			}
		}

		else if (IHDRInfo.ColorType == IndexColor)    // 索引图
		{
			bKGDInfo.bKGDRed = Palete[bKGDInfo.bKGDPlteIndex].Red;
			bKGDInfo.bKGDGreen = Palete[bKGDInfo.bKGDPlteIndex].Green;
			bKGDInfo.bKGDBlue = Palete[bKGDInfo.bKGDPlteIndex].Blue;
		}	
	}
}

void PngRowInit()
{
	const int Pass_Xstart[7] = {0, 4, 0, 2, 0, 1, 0};	
	const int Pass_Xinc[7] = {8, 8, 4, 4, 2, 2, 1};
	const int Pass_Ystart[7] = {0, 0, 4, 0, 2, 0, 1};
	const int Pass_Yinc[7] = {8, 8, 8, 4, 4, 2, 2};
	
	int MaxBitDepth;
	UINT32 RowBytesTemp;
//	//CSW_TRACE(MMI_TS_ID,"Enter PngRowInit!\n");
	PNGzstream.avail_in = 0;
	
	InitTransform();

	if (IHDRInfo.Interlace)
	{
		CurPassRowNum = (IHDRInfo.Height + Pass_Yinc[0] - 1 - Pass_Ystart[0]) >> 3;
		CurInterlaceRowWidth = (IHDRInfo.Width + Pass_Xinc[CurPass] 
			- 1 - Pass_Xstart[CurPass]) / Pass_Xinc[CurPass];

//		CurPassRowBytes = ROWBYTES(IHDRInfo.BitDepth, CurInterlaceRowWidth) + 1;	
              CurPassRowBytes = ROWBYTES(PixelDepth, CurInterlaceRowWidth) + 1;
////CSW_TRACE(MMI_TS_ID,"PixelDepth = 0x%x     CurPassRowBytes = 0x%x\n", PixelDepth, CurPassRowBytes);
	}
	else
	{
		CurPassRowNum = IHDRInfo.Height;
		CurInterlaceRowWidth = IHDRInfo.Width;
		CurPassRowBytes = RowBytes + 1;
	}

	MaxBitDepth = PixelDepth;

	if (transform & EXPAND)
	{
		if (IHDRInfo.ColorType == IndexColor)
		{
			if (TransNum)
				MaxBitDepth = 32;
			else
				MaxBitDepth = 24;
		}
		else if (IHDRInfo.ColorType == GreyScale)
		{
			if (MaxBitDepth < 8)
				MaxBitDepth = 8;
			else
				MaxBitDepth = MaxBitDepth << 1;
		}
		else if (IHDRInfo.ColorType == TrueColor)
		{
			if (TransNum)
			{
				MaxBitDepth <<= 2;
				MaxBitDepth /= 3;
			}
		}		
	}

	if (transform & GREY2RGB)
	{
		if ((TransNum && (transform & EXPAND)) || IHDRInfo.ColorType == GreyScaleAlpha)
		{
			if (MaxBitDepth <= 16)
				MaxBitDepth = 32;
			else
				MaxBitDepth = 64;
		}
		else
		{
			if (MaxBitDepth <= 8)
			{
				if (IHDRInfo.ColorType == GreyScaleAlpha)
					MaxBitDepth = 32;
				else
					MaxBitDepth = 24;
			}
			else if (IHDRInfo.ColorType == GreyScaleAlpha)
				MaxBitDepth = 64;
			else
				MaxBitDepth = 48;
		}
	}

	RowBytesTemp = ((IHDRInfo.Width + 7) & ~((UINT32)7));       // 四字节对其
//	//CSW_TRACE(MMI_TS_ID,"RowBytesTemp = 0x%x \n", RowBytesTemp);
	RowBytesTemp = ROWBYTES(MaxBitDepth, RowBytesTemp) + 1 + ((MaxBitDepth + 7) >> 3);
//	//CSW_TRACE(MMI_TS_ID,"RowBytesTemp = 0x%x \n", RowBytesTemp);

#if 0
	RowBuffer = (BYTE *)OslMalloc(RowBytesTemp + 32);	
	PreRow = (BYTE *)OslMalloc(RowBytes + 1);
	FinalRowData = (BYTE *)OslMalloc(RowBytesTemp + 32);
#else

////CSW_TRACE(MMI_TS_ID,"RowBytesTemp = 0x%x \n", RowBytesTemp);

       RowBuffer = RowBytesTempArray; 
       PreRow = RowBytesArray;
	FinalRowData = FinalRowArray;
#endif
}

void UpdateTransformInfo()
{
	NewColorType = IHDRInfo.ColorType;
	NewBiteDepth = IHDRInfo.BitDepth;

	if (transform & EXPAND)
	{
		if (IHDRInfo.ColorType == IndexColor)
		{
			if (TransNum && (transform & tRNSEXPAND))
				NewColorType = TrueColorAlpha;
			else
				NewColorType = TrueColor;

			NewBiteDepth = 8;
			NewTansNum = 0;
		}
		else
		{
			if (TransNum)
			{
				if (transform & tRNSEXPAND)
					NewColorType = IHDRInfo.ColorType | 4;
				else
					NewColorType = IHDRInfo.ColorType | 2;	
			}
			
			if (IHDRInfo.BitDepth < 8)
				NewBiteDepth = 8;
			NewTansNum = 0;
		}
	}

	if ((transform & DEPTH16to8) && (NewBiteDepth == 16))
		NewBiteDepth = 8;

	if (transform & GREY2RGB)
		NewColorType |= 2;

	if ((transform & DEFAULTbKGD) || (transform & bKGDEXPAND))
	{
		NewColorType &= ~4;
		NewTansNum = 0;
//		NewBkgd = ;
	}

	if (NewColorType == IndexColor)
		NewChannel = 1;
	else if (NewColorType & 2)
		NewChannel = 3;
	else
		NewChannel = 1;

	if (NewColorType & 4)
		NewChannel++;

	NewPixelDepth = (UINT8)(NewChannel * NewBiteDepth);
	NewRowBytes = ROWBYTES(NewPixelDepth, IHDRInfo.Width);
//	//CSW_TRACE(MMI_TS_ID,"NewPixelDepth = 0x%x   NewRowBytes = 0x%x\n", NewPixelDepth , NewRowBytes);

}

void UpdateInfo()
{
	PngRowInit();
	UpdateTransformInfo();
}



void FilterRow(BYTE *CurrentROW, BYTE *PreviousRow, BYTE FilterType)
{
////CSW_TRACE(MMI_TS_ID,"Enter FilterRow!\n");
//IMGDP_ENTRY(10024);
	//mmi_trace(1,"filter%d",FilterType);

	switch(FilterType)
	{
	case FilterTypeNone:
		break;

	case FilterTypeSub:
		  {
			  UINT32 i;
			  UINT32 istop = RowInfo.RowBytes;
			  UINT32 PixelBytes = (RowInfo.PixelDepth + 7) >> 3;
			  BYTE *dest = CurrentROW + PixelBytes;
			  BYTE *src = CurrentROW;

			 istop=istop - PixelBytes;

			  for (i = istop/2; i >0; i--)
			  {
				  *dest = (UINT8)(((int)(*dest) + (int)(*src++)) & 0xff);
				  dest++;

				  *dest = (UINT8)(((int)(*dest) + (int)(*src++)) & 0xff);
				  dest++;
			  }
			 
			  if(istop&1)
			  {
			  	*dest = (UINT8)(((int)(*dest) + (int)(*src++)) & 0xff);
				  dest++;
			  }

			  break;
		  }

	case FilterTypeUp:
		{
			UINT32 i;
			UINT32 istop = RowInfo.RowBytes;
			BYTE *dest = CurrentROW;
			BYTE *src = PreviousRow;
			
			for (i = istop/2; i >0; i--)
			{
				*dest = (UINT8)(((int)(*dest) + (int)(*src++)) & 0xff);
				dest++;

				*dest = (UINT8)(((int)(*dest) + (int)(*src++)) & 0xff);
				dest++;
			}
			
			 if(istop&1)
			  {
			  	*dest = (UINT8)(((int)(*dest) + (int)(*src++)) & 0xff);
				dest++;
			  }

			break;
		}

	case FilterTypeAverage:
		{
			UINT32 i;
			BYTE *dest = CurrentROW;
			BYTE *src = PreviousRow;
			BYTE*left = CurrentROW;
			UINT32 PixelBytes = (RowInfo.PixelDepth + 7) >> 3; 
			UINT32 istop = RowInfo.RowBytes - PixelBytes;

			for (i = 0; i < PixelBytes; i++)
			{
				*dest = (UINT8)(((int)(*dest) + ((int)(*src++) / 2)) & 0xff);
				dest++;
			}

			for (i = istop/2; i >0; i--)
			{
				*dest = (UINT8)(((int)(*dest) + (int)((*src++) + (*left++)) / 2) & 0xff);
				dest++;

				*dest = (UINT8)(((int)(*dest) + (int)((*src++) + (*left++)) / 2) & 0xff);
				dest++;
			}

			 if(istop&1)
			  {
			  	*dest = (UINT8)(((int)(*dest) + (int)((*src++) + (*left++)) / 2) & 0xff);
				dest++;
			  }
			 
			break;
		}

	case FilterTypePaeth:
		{
			UINT32 i;
			BYTE *dest = CurrentROW;
			BYTE *src = PreviousRow;
			BYTE *left = CurrentROW;
			BYTE *up = PreviousRow;
			UINT32 PixelBytes = (RowInfo.PixelDepth + 7) >> 3; 
			UINT32 istop = RowInfo.RowBytes - PixelBytes;

			for (i = 0; i < PixelBytes; i++)
			{
				*dest = (UINT8)(((int)(*dest) + (int)(*src++)) & 0xff);
				dest++;
			}

			for (i = istop; i >0; i--)
			{
				int a, b, c, pa, pb, pc, p;

				a = *left++;
				b = *src++;
				c = *up++;

				p = b - c;
				pc = a - c;

				pa = p < 0 ? -p : p;
				pb = pc < 0 ? -pc : pc;
				pc = (p + pc) < 0 ? -(p + pc) : p + pc;

				p = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;

				*dest = (UINT8)(((int)(*dest) + p) & 0xff);
				dest++;
			}
			
			break;
		}

	default:
		*CurrentROW = 0;
		break;
	}

//IMGDP_EXIT(10024);

}

void ReadFinishRow()
{
	const int PassXStart[7] = {0, 4, 0, 2, 0, 1, 0};
	const int PassXInc[7] = {8, 8, 4, 4, 2, 2, 1};
	const int PassYStart[7] = {0, 0, 4, 0, 2, 0, 1};
	const int PassYInc[7] = {8, 8, 8, 4, 4, 2, 2};

	CurRowNumber++;
	if (CurRowNumber < IHDRInfo.Height)
		return;

	if (IHDRInfo.Interlace)
	{
		CurRowNumber = 0;
		memset(PreRow, 0, RowBytes + 1);

		do
		{
			CurPass++;
			if (CurPass >= 7)
				break;

			CurInterlaceRowWidth = (IHDRInfo.Width + PassXInc[CurPass] 
				- 1 - PassXStart[CurPass]) / PassXInc[CurPass];
			CurPassRowBytes = ROWBYTES(PixelDepth, CurInterlaceRowWidth) + 1;

			if (!(transform & INTERLACE))
			{
				CurPassRowNum = (IHDRInfo.Height + PassYInc[CurPass] - 1 - PassYStart[CurPass]) / PassYInc[CurPass];

				if (!CurPassRowNum)
					continue;
			}
			else
				break;
		} while (CurInterlaceRowWidth == 0);

		if (CurPass < 7)
			return;
	}

	inflateReset(&PNGzstream);
}

void InterlaceProcess()
{
//IMGDP_ENTRY(10026);

	BYTE *Row = RowBuffer + 1;
	UINT8 Pass = CurPass;

	const int PassInc[7] = {8, 8, 4, 4, 2, 2, 1};

	if (Row != NULL)
	{
		UINT32 FinalWidth;

		FinalWidth = RowInfo.Width * PassInc[Pass];

//		//CSW_TRACE(MMI_TS_ID,"before InterlaceProcess: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
//		Row[0],Row[1],Row[2],Row[3],Row[4],
//		Row[5],Row[6],Row[7],Row[8],Row[9],
//		Row[10],Row[11],Row[12],Row[13],Row[14],
//		Row[15],Row[16],Row[17],Row[18],Row[19]);

		switch(RowInfo.PixelDepth)
		{
		case 1:
			{
				BYTE *sp = Row + (UINT32)((RowInfo.Width - 1) >> 3);
				BYTE *dp = Row + (UINT32)((FinalWidth - 1) >> 3);
				int sShift, dShift;
				int sStart, sEnd, sInc;
				int jStop = PassInc[Pass];
				UINT8 v;
				UINT32 i;
				int j;

				sShift = 7 - (int)((RowInfo.Width + 7) & 0x07);
				dShift = 7 - (int)((FinalWidth + 7) & 0x07);
				sStart = 0;
				sEnd = 7;
				sInc = 1;

				for (i = 0; i < RowInfo.Width; i++)
				{
					v = (UINT8)((*sp >> sShift) & 0x01);

					for (j = 0; j < jStop; j++)
					{
						*dp &= (BYTE)((0x7f7f >> (7 - dShift)) & 0xff);
						*dp |= (BYTE)(v << dShift);

						if (dShift == sEnd)
						{
							dShift = sStart;
							dp--;
						}
						else
							dShift += sInc;
					}

					if (sShift == sEnd)
					{
						sShift = sStart;
						sp--;
					}
					else
						sShift += sInc;
				}

				break;
			}

		case 2:
			{
				BYTE *sp = Row + (UINT32)((RowInfo.Width - 1) >> 2);
				BYTE *dp = Row + (UINT32)((FinalWidth - 1) >> 2);
				int sShift, dShift;
				int sStart, sEnd, sInc;
				int jStop = PassInc[Pass];
				UINT32 i;

				sShift = (int)((3 - ((RowInfo.Width + 3) & 0x03)) << 1);
				dShift = (int)((3 - ((FinalWidth + 3) & 0x03)) << 1);
				sStart = 0;
				sEnd = 6;
				sInc = 2;

				for (i = 0; i < RowInfo.Width; i++)
				{
					UINT8 v;
					int j;

					v = (UINT8)((*sp >> sShift) & 0x03);
					for (j = 0; j < jStop; j++)
					{
						*dp &= (BYTE)((0x3f3f >> (6 - dShift)) & 0xff);
						*dp |= (BYTE)(v << dShift);

						if (dShift == sEnd)
						{
							dShift = sStart;
							dp--;
						}
						else
							dShift += sInc;	
					}

					if (sShift == sEnd)
					{
						sShift = sStart;
						sp--;
					}
					else
						sShift += sInc;
				}

				break;				
			}

		case 4:
			{
				BYTE *sp = Row + (UINT32)((RowInfo.Width - 1) >> 3);
				BYTE *dp = Row + (UINT32)((FinalWidth - 1) >> 3);
				int sShift, dShift;
				int sStart, sEnd, sInc;
				int jStop = PassInc[Pass];
				UINT32 i;
				
				sShift = (int)((1 - ((RowInfo.Width + 1) & 0x01)) << 2);
				dShift = (int)((1 - ((FinalWidth +1) & 0x01)) << 2);
				sStart = 0;
				sEnd = 4;
				sInc = 4;

				for (i = 0; i < RowInfo.Width; i++)
				{
					BYTE v = (BYTE)((*sp >> sShift) & 0xf);
					int j;

					for (j = 0; j < jStop; j++)
					{
						*dp &= (BYTE)((0xf0f >> (4 - dShift)) & 0xff);
						*dp |= (BYTE)(v << dShift);

						if (dShift == sEnd)
						{
							dShift = sStart;
							dp--;
						}
						else
							dShift += sInc;
					}

					if (sShift == sEnd)
					{
						sShift = sStart;
						sp--;
					}
					else
						sShift += sInc;
				}

				break;				
			}

		default:
			{
				UINT32 PixelBytes = RowInfo.PixelDepth >> 3;
				BYTE *sp = Row + (UINT32)(RowInfo.Width - 1) * PixelBytes;
				BYTE *dp = Row + (UINT32)(FinalWidth - 1) * PixelBytes;
				int jStop = PassInc[Pass];
				UINT32 i;

////CSW_TRACE(MMI_TS_ID,"PixelBytes = 0x%x\n", PixelBytes);

				for (i = 0; i < RowInfo.Width; i++)
				{
					BYTE v[8];
					int j;

					memcpy(v, sp, PixelBytes);

					for (j = 0; j < jStop; j++)
					{
						memcpy(dp, v, PixelBytes);
						dp -= PixelBytes;
					}
					
					sp -= PixelBytes;
				}
				
				break;				
			}
		}

//		//CSW_TRACE(MMI_TS_ID,"before InterlaceProcess: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
//		Row[0],Row[1],Row[2],Row[3],Row[4],
//		Row[5],Row[6],Row[7],Row[8],Row[9],
//		Row[10],Row[11],Row[12],Row[13],Row[14],
//		Row[15],Row[16],Row[17],Row[18],Row[19]);

		RowInfo.Width = FinalWidth;
		RowInfo.RowBytes = ROWBYTES(RowInfo.PixelDepth, FinalWidth);

//		//CSW_TRACE(MMI_TS_ID,"RowInfo.Width = 0x%x  RowInfo.RowBytes = 0x%x\n", RowInfo.Width, RowInfo.RowBytes);
	}

//IMGDP_EXIT(10026);

}

void CombineRow(BYTE *Row, int mask)
{
//IMGDP_ENTRY(10027);

//	UINT32 j;
//	Byte *src;
//	UINT16 *dest;

	BYTE r, g, b;

	U16   *adp;

/*	adp = (U16*)alpha_dest_output_ptr;
*/
//				if(!ResizePng_Need)
//					adp = (U16*)alpha_dest_row_ptr;
//				else
					adp = (U16*)alpha_dest_output_ptr;
									
									
 
	if (mask == 0xff)
	{
		UINT32 i;
		BYTE *sp = RowBuffer + 1;

		for (i = IHDRInfo.Width; i >0; i--)
		{
			//r = *sp++ & 0xf8;
			//g = *sp++ & 0xfc;
			//b = *sp++ & 0xf8;
			*adp++ = (((*sp & 0xf8) << 8) | ((*(sp+1) & 0xfc) << 3) | ((*(sp+2) & 0xf8) >> 3));
			sp = sp + 3;
		}
	}
	else
	{
		switch(RowInfo.PixelDepth)
		{
		case 1:
			{
				BYTE *sp = RowBuffer + 1;

//				Byte *dp = Row;
				BYTE *dp = FinalRowData;

				int sInc, sStart, sEnd;
				int m = 0x80;
				int shift;
				UINT32 i;
				UINT32 RowWidth = IHDRInfo.Width;
				
				sStart = 7;
				sEnd = 0;
				sInc = -1;
				
				shift = sStart;
				
				for (i = 0; i < RowWidth; i++)
				{
					if (m & mask)
					{
						int value;
						
						value = (*sp >> shift) & 0x01;
						*dp &= (UINT8)((0x7f7f >> (7 - shift)) & 0xff);
						*dp |= (UINT8)(value << shift);
					}
					
					if (shift == sEnd)
					{
						shift = sStart;
						sp++;
						dp++;
					}
					else
						shift += sInc;
					
					if (m == 1)
						m = 0x80;
					else
						m >>= 1;
				}

				break;
			}

		case 2:
			{
				BYTE *sp = RowBuffer + 1;

//				Byte *dp = Row;
				BYTE *dp = FinalRowData;

				int sInc, sStart, sEnd;
				int m = 0x80;
				int shift;
				UINT32 i;
				UINT32 RowWidth = IHDRInfo.Width;
				int value;

				sStart = 6;
				sEnd = 0;
				sInc = -2;

				shift = sStart;

				for (i = 0; i < RowWidth; i++)
				{
					if (m & mask)
					{
						value = (*sp >> shift) & 0x03;
						*dp &= (UINT8)((0x3f3f >> (6 - shift)) & 0xff);
						*dp |= (UINT8)(value << shift);
					}

					if (shift == sEnd)
					{
						shift = sStart;
						sp++;
						dp++;
					}
					else
						shift += sInc;

					if (m == 1)
						m = 0x80;
					else
						m >>= 1;
				}

				break;
			}

		case 4:
			{
				BYTE *sp = RowBuffer + 1;

//				Byte *dp = Row;
				BYTE *dp = FinalRowData;

				int sInc, sStart, sEnd;
				int m = 0x80;
				int shift;
				UINT32 i;
				UINT32 RowWidth = IHDRInfo.Width;
				int value;
				
				sStart = 4;
				sEnd = 0;
				sInc = -4;
				
				shift = sStart;

				for (i = 0; i < RowWidth; i++)
				{
					if (m & mask)
					{
						value = (*sp >> shift) & 0xf;
						*dp &= (UINT8)((0xf0f >> (4 - shift)) & 0xff);
						*dp |= (UINT8)(value << shift);
					}

					if (shift == sEnd)
					{
						shift = sStart;
						sp++;
						dp++;
					}
					else
						shift += sInc;

					if (m == 1)
						m = 0x80;
					else
						m >>= 1;
				}
				
				break;
			}

		default:
			{
				BYTE *sp = RowBuffer + 1;

				int m = 0x80;
				UINT32 i;
				UINT32 PixelBytes = RowInfo.PixelDepth >> 3;
				UINT32 RowWidth = IHDRInfo.Width;

				for (i = RowWidth; i >0; i--)
				{
					if (m & mask)
					{
						r = *sp & 0xf8;
						g = *(sp + 1) & 0xfc;
						b = *(sp + 2) & 0xf8;
						*adp = ((r << 8) | (g << 3) | (b >> 3));
					}

					sp += PixelBytes;
                    adp++;

					if (m == 1)
						m = 0x80;
					else
						m >>= 1;
				}

				break;
			}
		}
	}

//	//CSW_TRACE(MMI_TS_ID,"FinalRowData: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n\n", 
//		FinalRowData[0],FinalRowData[1],FinalRowData[2],FinalRowData[3],FinalRowData[4],
//		FinalRowData[5],FinalRowData[6],FinalRowData[7],FinalRowData[8],FinalRowData[9],
//		FinalRowData[10],FinalRowData[11],FinalRowData[12],FinalRowData[13],FinalRowData[14],
//		FinalRowData[15],FinalRowData[16],FinalRowData[17],FinalRowData[18],FinalRowData[19],
//		FinalRowData[20],FinalRowData[21],FinalRowData[22],FinalRowData[23],FinalRowData[24]);

#if 0
	src = FinalRowData;
	dest = (UINT16 *)Row;

	for (j = 0; j < IHDRInfo.Width; j++)
	{
		r = *src++ & 0xf8;
		g = *src++ & 0xfc;
		b = *src++ & 0xf8;

		*dest++ = (r << 8) | (g << 3) | (b >> 3);

//		//CSW_TRACE(MMI_TS_ID,"CombineRow: 0x%x 0x%x\n",Row[j*2], Row[j *2 +1]);
	}
#endif
//CSW_TRACE(MMI_TS_ID,"Row: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n\n", 
//		Row[0],Row[1],Row[2],Row[3],Row[4],
//		Row[5],Row[6],Row[7],Row[8],Row[9],
//		Row[10],Row[11],Row[12],Row[13],Row[14],
//		Row[15],Row[16],Row[17],Row[18],Row[19],
//		Row[20],Row[21],Row[22],Row[23],Row[24]);

//	memcpy(Row, FinalRowData, NewRowBytes);
//IMGDP_EXIT(10027);

}

void ExpandPalette(BYTE *Row)
{
	int shift;
	int value;
	BYTE *sp;
	BYTE *dp;
	UINT32 i;
	UINT32 RowWidth = RowInfo.Width;

	if (RowInfo.ColorType == IndexColor)
	{
		if (RowInfo.BitDepth < 8)
		{
			switch(RowInfo.BitDepth)
			{
			case 1:
				{
					sp = Row + (UINT32)((RowWidth - 1) >> 3);
					dp = Row + (UINT32)RowWidth - 1;
					shift = 7 - (int)((RowWidth + 7) & 0x07);

					for (i = 0; i < RowWidth; i++)
					{
						if ((*sp >> shift) & 0x01)
							*dp = 1;
						else
							*dp = 0;

						if (shift == 7)
						{
							shift = 0;
							sp--;
						}
						else
							shift++;

						dp--;	
					}

					break;
				}
				
			case 2:
				{
					sp = Row + (UINT32)((RowWidth - 1) >> 2);
					dp = Row + (UINT32)RowWidth - 1;
					shift = (int)((3 - ((RowWidth + 3) & 0x03)) << 1);

					for (i = 0; i < RowWidth; i++)
					{
						value = (*sp >> shift) & 0x03;
						*dp = (BYTE)value;

						if (shift == 6)
						{
							shift = 0;
							sp--;
						}
						else
							shift += 2;

						dp--;
					}

					break;
				}

			case 4:
				{
					sp = Row + (UINT32)((RowWidth - 1) >> 1);
					dp = Row + (UINT32)RowWidth - 1;
					shift = (int)((RowWidth & 0x01) << 2);

					for (i = 0; i < RowWidth; i++)
					{
						value = (*sp >> shift) & 0x0f;
						*dp = (BYTE)value;

						if (shift == 4)	
						{
							shift = 0;
							sp--;
						}
						else
							shift += 4;

						dp--;
					}

					break;
				}
			}

			RowInfo.BitDepth = 8;
			RowInfo.PixelDepth = 8;
			RowInfo.RowBytes = RowWidth;
		}

		switch(RowInfo.BitDepth)
		{
		case 8:
			{
				if (TransNum)
				{
					BYTE    *tmp_p;
					BYTE    *tmp_p1;
					UINT32 data_tmp;
					
					sp = Row + (UINT32)RowWidth - 1;
					dp = Row + (UINT32)(RowWidth << 2) - 1;
					tmp_p = (BYTE *)Palete;

					for (i = RowWidth; i >0; i--)
					{
						data_tmp = *sp--;
							
						if (data_tmp >= TransNum)
							*dp-- = 0xff;
						else
							*dp-- = tRNSIndex[data_tmp];

						tmp_p1 = tmp_p + 3*data_tmp;
						
						*dp-- = *(tmp_p1+2);
						*dp-- = *(tmp_p1+1);
						*dp-- = *tmp_p1;
					}

					RowInfo.BitDepth = 8;
					RowInfo.PixelDepth = 32;
					RowInfo.RowBytes = RowWidth << 2;
					RowInfo.ColorType = 6;
					RowInfo.Channels = 4;
				}
				else
				{
					BYTE    *tmp_p;
					BYTE    *tmp_p1;
					UINT32 data_tmp;
	
					sp = Row + (UINT32)RowWidth - 1;
					dp = Row +(UINT32)((RowWidth << 1) + RowWidth) - 1;
					tmp_p = (BYTE *)Palete;

					for (i = RowWidth; i >0; i--)
					{
						data_tmp = *sp--;
						tmp_p1 = tmp_p + 3*data_tmp;
						
						*dp-- = *(tmp_p1+2);
						*dp-- = *(tmp_p1+1);
						*dp-- = *tmp_p1;
					}

					RowInfo.BitDepth = 8;
					RowInfo.PixelDepth = 24;
					RowInfo.RowBytes = RowWidth + (RowWidth << 1);
					RowInfo.ColorType = 2;
					RowInfo.Channels = 3;
				}

				break;
			}
		}
	}
}

void ExpandImage(BYTE *Row)
{
	int shift;
	int value;
	BYTE *sp;
	BYTE *dp;
	UINT32 i;
	UINT32 RowWidth = RowInfo.Width;

	if (RowInfo.ColorType == GreyScale)
	{
		UINT16 Grey = (UINT16)(TransNum ? tRNSInfo.tRNSGrey : 0);

		if (RowInfo.BitDepth < 8)
		{
			switch(RowInfo.BitDepth)
			{
			case 1:
				{
// 					Grey = (UINT16)(Grey * 0xff);
					Grey = (UINT16)((Grey << 8) - Grey);
					sp = Row + (UINT32)((RowWidth - 1) >> 3);
					dp = Row + (UINT32)RowWidth - 1;
					shift = 7 - (int)((RowWidth + 7) & 0x07);

					for (i = 0; i < RowWidth; i++)
					{
						if ((*sp >> shift) &0x01)
							*dp = 0xff;
						else
							*dp = 0;

						if (shift == 7)
						{
							shift = 0;
							sp--;
						}
						else
							shift++;

						dp--;
					}

					break;
				}

			case 2:
				{
					Grey = (UINT16)(Grey * 0x55);
					sp = Row + (UINT32)((RowWidth - 1) >> 2);
					dp = Row + (UINT32)RowWidth - 1;
					shift = (int)((3 - ((RowWidth - 3) & 0x03)) << 1);

					for (i = 0; i < RowWidth; i++)
					{
						value = (*sp >> shift) & 0x03;
						*dp = (UINT8)(value | (value << 2) | (value << 4) | (value << 6));

						if (shift == 6)
						{
							shift = 0;
							sp--;
						}
						else
							shift += 2;

						dp--;
					}

					break;
				}

			case 4:
				{
					Grey = (UINT16)(Grey + (Grey << 4));
					sp = Row + (UINT32)((RowWidth - 1) >> 1);
					dp = Row + (UINT32)RowWidth - 1;
					shift = (int)((1 - ((RowWidth + 1) & 0x01)) << 2);

					for (i = 0; i < RowWidth; i++)
					{
						value = (*sp >> shift) & 0x0f;
						*dp = (UINT8)(value | (value << 4));
						
						if (shift == 4)
						{
							shift = 0;
							sp--;
						}
						else
							shift = 4;

						dp--;
					}
					
					break;
				}
			}

			RowInfo.BitDepth = 8;
			RowInfo.PixelDepth = 8;
			RowInfo.RowBytes = RowWidth;
		}	

		if (TransNum != 0)
		{
			UINT32 data_tmp0,data_tmp1;
			
			if (RowInfo.BitDepth == 8)
			{
				sp = Row + (UINT32)RowWidth - 1;
				dp = Row + (UINT32)(RowWidth << 1) - 1;
				
				for (i = RowWidth; i >0; i--)
				{
					data_tmp0 = *sp--;
					
					if (data_tmp0 == Grey)
						*dp-- = 0;
					else
						*dp-- = 0xff;
					
					*dp-- = data_tmp0;
				}
			}
			else if (RowInfo.BitDepth == 16)
			{
				sp = Row + (UINT32)RowWidth - 1;
				dp = Row + (UINT32)(RowWidth << 1) - 1;
				
				for (i = RowWidth; i >0; i--)
				{
					data_tmp0 = *sp;
					data_tmp1 = *(sp-1);
					
					if ((data_tmp0 | (data_tmp1 << 8)) == Grey)
					{
						*dp-- = 0;
						*dp-- = 0;
					}
					else
					{
						*dp-- = 0xff;
						*dp-- = 0xff;
					}
					
					*dp-- = data_tmp0;
					*dp-- = data_tmp1;
					sp =sp-2;
				}
			}
			
			RowInfo.ColorType = GreyScaleAlpha;
			RowInfo.Channels = 2;
			RowInfo.PixelDepth = (UINT8)(RowInfo.BitDepth << 1);
			RowInfo.RowBytes = ROWBYTES(RowInfo.PixelDepth, RowWidth);
		}
	}
	else if (RowInfo.ColorType == TrueColor && TransNum)
	{
		UINT32 data_tmp0,data_tmp1,data_tmp2;
		data_tmp0 = tRNSInfo.tRNSRed;
		data_tmp1 = tRNSInfo.tRNSGreen;
		data_tmp2 = tRNSInfo.tRNSBlue;
		
		if (RowInfo.BitDepth  == 8)
		{
			UINT32 data_tmp3,data_tmp4,data_tmp5;
			sp = Row + (UINT32)RowInfo.RowBytes - 1;
			dp = Row + (UINT32)(RowWidth << 2) - 1;
			
			
			for (i = RowWidth; i >0; i--)
			{
				data_tmp3 =  *sp--;
				data_tmp4 =  *sp--;
				data_tmp5 =  *sp--;
				
				if (data_tmp5 == data_tmp0 && data_tmp4 == data_tmp1 && data_tmp3 == data_tmp2)
					*dp-- = 0;
				else
					*dp-- = 0xff;

				*dp-- = data_tmp3;
				*dp-- = data_tmp4;
				*dp-- = data_tmp5;
			}
		}
		else if (RowInfo.BitDepth == 16)
		{
			sp = Row + (UINT32)RowInfo.RowBytes - 1;
			dp = Row + (UINT32)(RowWidth << 3) - 1;

			for (i = RowWidth; i >0; i--)
			{
				if ((((UINT16)*(sp - 4) | ((UINT16)*(sp - 5) << 8)) == data_tmp0) &&
					(((UINT16)*(sp - 2) | ((UINT16)*(sp - 3) << 8)) == data_tmp1) &&
					(((UINT16)*sp | ((UINT16)*(sp - 1) << 8)) == data_tmp2))
				{
					*dp-- = 0;
					*dp-- = 0;
				}
				else
				{
					*dp-- = 0xff;
					*dp-- = 0xff;
				}

				*dp-- = *sp--;
				*dp-- = *sp--;
				*dp-- = *sp--;
				*dp-- = *sp--;
				*dp-- = *sp--;
				*dp-- = *sp--;
			}

			RowInfo.ColorType = TrueColorAlpha;
			RowInfo.Channels = 4;
			RowInfo.PixelDepth = (UINT8)(RowInfo.BitDepth << 2);
			RowInfo.RowBytes = ROWBYTES(RowInfo.PixelDepth, RowWidth);
		}
	}
}

void BkgdProcess(BYTE * Row)
{
	BYTE *sp;
	BYTE *dp;
	UINT32 i;
	UINT32 RowWidth = RowInfo.Width;
	int shift;
	U16 *adp = (U16 *)alpha_dest_row_ptr;
//	//CSW_TRACE(MMI_TS_ID,"before BkgdProcess: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
//		Row[0],Row[1],Row[2],Row[3],Row[4],
//		Row[5],Row[6],Row[7],Row[8],Row[9],
//		Row[10],Row[11],Row[12],Row[13],Row[14],
//		Row[15],Row[16],Row[17],Row[18],Row[19]);

//	if (FileChunks.bKGDNuM && (!(RowInfo.ColorType & 4)
//		|| (RowInfo.ColorType != IndexColor && FileChunks.tRNSNum)))
	{
		switch(RowInfo.ColorType)
		{
		case GreyScale:
			{
				if (FileChunks.tRNSNum)
				{
					switch(RowInfo.BitDepth)
					{
						case 1:
					{
						sp = Row;
						shift = 7;
						
						for (i = 0; i < RowWidth; i++)
						{
							if ((UINT16)((*sp >> shift) & 0x01) == tRNSInfo.tRNSGrey)
							{
								*sp &= (UINT8)((0x7f7f >> (7 - shift)) & 0xff);
								*sp |= (UINT8)(bKGDInfo.bKGDGrey << shift);
							}

							if (!shift)
							{
								shift = 7;
								sp++;
							}
							else
								shift--;
						}

						break;
					}
					
				case 2:
					{
						sp = Row;
						shift = 6;

						for (i = 0; i < RowWidth; i++)
						{
							if ((UINT16) ((*sp >> shift) & 0x03) == tRNSInfo.tRNSGrey)
							{
								*sp &= (UINT8)((0x3f3f >> (6 - shift)) & 0xff);
								*sp |= (UINT8)(bKGDInfo.bKGDGrey << shift);
							}

							if (!shift)
							{
								shift = 6;
								sp++;
							}
							else
								shift -= 2;
						}
						break;
					}
					
				case 4:
					{
						sp = Row;
						shift = 4;

						for (i = 0; i < RowWidth; i++)
						{
							if ((UINT16)((*sp >> shift) & 0xff) == tRNSInfo.tRNSGrey)
							{
								*sp &= (UINT8)((0xf0f >> (4 - shift)) & 0xff);
								*sp |= (UINT8)(bKGDInfo.bKGDGrey << shift);
							}

							if (!shift)
							{
								shift = 4;
								sp++;
							}
							else
								shift -= 4;
						}
						break;
					}
					
				case 8:
					{
						UINT32 data_tmp0,data_tmp1;
						sp = Row;
						
						data_tmp0 = tRNSInfo.tRNSGrey;
						data_tmp1 = bKGDInfo.bKGDGrey;

						for (i = RowWidth; i >0; i--)
						{
							if (*sp == data_tmp0)
							{
								*sp = data_tmp1;
							}
						}
						break;
					}

				case 16:
					{
						UINT32 data_tmp0,data_tmp1,data_tmp2;
						sp = Row;

						data_tmp0 = tRNSInfo.tRNSGrey;
						data_tmp1 = (bKGDInfo.bKGDGrey >> 8) & 0xff;
						data_tmp2 = bKGDInfo.bKGDGrey & 0xff;

						for (i = RowWidth; i >0; i--, sp += 2)
						{
							UINT16 v;

							v = (UINT16)(((*sp) << 8) + *(sp + 1));

							if (v == data_tmp0)
							{
								*sp = data_tmp1;
								*(sp + 1) = data_tmp2;
							}
						}

						break;
					}
					}
				}

				break;
			}

			case TrueColor:
				{
					if (FileChunks.tRNSNum)
					{
						UINT32 data_tmp0,data_tmp1,data_tmp2;
						UINT32 data_tmp3,data_tmp4,data_tmp5;
						
						if (RowInfo.BitDepth == 8)
						{
							sp = Row;
							data_tmp0 = tRNSInfo.tRNSRed;
							data_tmp1 = tRNSInfo.tRNSGreen;
							data_tmp2 = tRNSInfo.tRNSBlue;

							data_tmp3 = bKGDInfo.bKGDRed;
							data_tmp4 = bKGDInfo.bKGDGreen;
							data_tmp5 = bKGDInfo.bKGDBlue;

							
							for (i = RowWidth; i >0; i--, sp += 3)
							{
								if (*sp == data_tmp0 && *(sp + 1) == data_tmp1 && *(sp + 2) == data_tmp2)
								{
									*sp = data_tmp3;
									*(sp + 1) = data_tmp4;
									*(sp + 2) = data_tmp5;
								}
							}
						}
						else
						{			
							UINT32 color;

							data_tmp0 = bKGDInfo.bKGDRed;
							data_tmp1 = bKGDInfo.bKGDGreen;
							data_tmp2 = bKGDInfo.bKGDBlue;

							data_tmp3 = tRNSInfo.tRNSRed;
							data_tmp4 = tRNSInfo.tRNSGreen;
							data_tmp5 = tRNSInfo.tRNSBlue;
								
							sp = Row;
							
							for(i = RowWidth; i >0; i--, sp += 6)
							{
								color = ((*sp) << 8) + *(sp + 1);
								
								if (color == data_tmp3)
								{
									color = ((*sp + 2) << 8) + *(sp + 3);
									
									if(color == data_tmp4)
									{
										color = ((*sp + 4) << 8) + *(sp + 5);
										
										if(color == data_tmp5)
										{
											*sp 		 = (data_tmp0 >> 8) &0xff;
											*(sp + 1) = data_tmp0 &0xff;
											*(sp + 2) = (data_tmp1 >> 8) &0xff;
											*(sp + 3) = data_tmp1 &0xff;
											*(sp + 4) = (data_tmp2 >> 8) &0xff;
											*(sp + 5) = data_tmp2 &0xff;
										}
									}
	
								}
							}
						}
					}
					
					break;
				}

			case GreyScaleAlpha:
				{
					UINT32 data_tmp0,data_tmp1;
					
					if (RowInfo.BitDepth == 8)
					{
						sp = Row;
						dp = Row;
						data_tmp0 = bKGDInfo.bKGDGrey;

						for (i = RowWidth; i >0; i--, sp += 2, dp++)
						{
							UINT8 a = *(sp + 1);

							if(a == 0xff)
							{
								*dp = *sp;
							}
							else if (a == 0)
							{
								*dp = data_tmp0;
							}
							else
							{
								composite(*dp, *sp, a, data_tmp0);
							}
						}
					}
					else
					{
						sp = Row;
						dp = Row;
						
						data_tmp0 = (bKGDInfo.bKGDGrey >> 8) & 0xff;
						data_tmp1 = bKGDInfo.bKGDGrey;

						for (i = RowWidth; i >0; i--, sp += 4, dp += 2)
						{
							UINT16 a = (UINT16)(((*(sp + 2)) << 8) + *(sp + 3));

							if(a == (UINT16)0xffff)
							{
								*dp = *sp;
								*(dp + 1) = *(sp+1);
							}
							if (a == 0)
							{
								*dp = data_tmp0;
								*(dp + 1) = data_tmp1 & 0xff;
							}
							else
							{
								UINT16 g, v;

								g = (UINT16)(((*sp) << 8) + *(sp + 1));
								composite16(v, g, a, data_tmp1);

								*dp = (UINT8)((v >> 8) & 0xff);
								*(dp + 1) = (UINT8)(v & 0xff);
							}
						}
					}

					break;
				}
			case TrueColorAlpha:
				{
					if (RowInfo.BitDepth == 8)
					{
						UINT32 data_tmp0,data_tmp1,data_tmp2;
						sp = Row;
						dp = Row;
						
						if (transform & DEFAULTbKGD)
						{
							data_tmp0 = DefaultBkgd.Red;
							data_tmp1 = DefaultBkgd.Green;
							data_tmp2 = DefaultBkgd.Blue;
						}
						else
						{
							data_tmp0 = bKGDInfo.bKGDRed;
							data_tmp1 = bKGDInfo.bKGDGreen;
							data_tmp2 = bKGDInfo.bKGDBlue;
						}

						if(transform & BKGDImage)
						{
	
							for (i = RowWidth; i >0; i--, sp += 4, dp += 3,adp++)
							{
								UINT8 a = *(sp + 3);
								if(!ResizePng_Need)
								{
									data_tmp0 = (*(adp)& 0xf800 )>>8; //DefaultBkgd.Red;
									data_tmp1 = ((*(adp)& 0x7E0 )>>3); //DefaultBkgd.Green;
									data_tmp2 = (*adp  & 0x1F) << 3; //DefaultBkgd.Blue;
								}
								if (a == 0xff) 
								{
									*dp = *sp;
									*(dp + 1) = *(sp + 1);
									*(dp + 2) = *(sp + 2);	
								}
								else if (a == 0)
								{
									*dp = data_tmp0;
									*(dp + 1) = data_tmp1;
									*(dp + 2) = data_tmp2;
								}
								else
								{
									composite(*dp, *sp, a,data_tmp0);
									composite(*(dp + 1), *(sp + 1), a, data_tmp1);
									composite(*(dp + 2), *(sp + 2), a, data_tmp2);
								}
							}
						}
						else
						{
							for (i = RowWidth; i >0; i--, sp += 4, dp += 3)
							{
								UINT8 a = *(sp + 3);

								if (a == 0xff)
								{
									*dp = *sp;
									*(dp + 1) = *(sp + 1);
									*(dp + 2) = *(sp + 2);
								}
								else if (a == 0)
								{
									*dp = data_tmp0;
									*(dp + 1) = data_tmp1;
									*(dp + 2) = data_tmp2;
								}
								else
								{
									composite(*dp, *sp, a,data_tmp0);
									composite(*(dp + 1), *(sp + 1), a, data_tmp1);
									composite(*(dp + 2), *(sp + 2), a, data_tmp2);
								}
							}
						}
					}
					else
					{
						sp = Row;
						dp = Row;

						for (i = RowWidth; i >0; i--, sp += 8, dp += 6)
						{

							memcpy(dp, sp, 6);
						}
					}

					break;
				}
		}


//	//CSW_TRACE(MMI_TS_ID,"After BkgdProcess: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
//		Row[0],Row[1],Row[2],Row[3],Row[4],
//		Row[5],Row[6],Row[7],Row[8],Row[9],
//		Row[10],Row[11],Row[12],Row[13],Row[14],
//		Row[15],Row[16],Row[17],Row[18],Row[19]);

		if (RowInfo.ColorType & 4)
		{
			RowInfo.ColorType &= ~4;
			RowInfo.Channels--;
			RowInfo.PixelDepth = (UINT8)(RowInfo.Channels * RowInfo.BitDepth);
			RowInfo.RowBytes = ROWBYTES(RowInfo.PixelDepth, RowWidth);
		}
	}
}

void Chop16to8(BYTE *Row)
{
	if (RowInfo.BitDepth == 16)
	{
		BYTE *sp = Row;
		BYTE *dp = Row;
		UINT32 i;
		UINT32 istop = RowInfo.Width * RowInfo.Channels;

		for (i = istop; i >0; i--, sp += 2, dp++)
		{
			*dp = *sp + ((((int)(*(sp + 1)) - *sp) > 128) ? 1 : 0);
		}

		RowInfo.BitDepth = 8;
		RowInfo.PixelDepth = (UINT8)(8 * RowInfo.Channels);
		RowInfo.RowBytes = RowInfo.Width * RowInfo.Channels;
	}
}

void ExpandGrey2RGB(BYTE *Row)
{
	UINT32 i;
	UINT32 RowWidth = RowInfo.Width;

	if (RowInfo.BitDepth >= 8 && !(RowInfo.ColorType & 2))
	{
		if (RowInfo.ColorType == GreyScale)
		{
			if (RowInfo.BitDepth == 8)
			{
				BYTE *sp = Row + (UINT32)RowWidth - 1;
				BYTE *dp = sp + (UINT32)(RowWidth << 1);

				for (i = RowWidth; i >0; i--)
				{
					*(dp--) = *sp;
					*(dp--) = *sp;
					*(dp--) = *(sp--);
				}
			}
			else
			{
				BYTE *sp = Row + (UINT32)(RowWidth << 1) - 1;
				BYTE *dp = sp + (UINT32)(RowWidth << 2);

				for (i = RowWidth; i >0; i--)
				{
					*(dp--) = *sp;
					*(dp--) = *(sp - 1);
					*(dp--) = *sp;
					*(dp--) = *(sp - 1);
					*(dp--) = *(sp--);
					*(dp--) = *(sp--);
				}
			}
		}
		else if (RowInfo.ColorType == GreyScaleAlpha)
		{
			if (RowInfo.BitDepth == 8)
			{
				BYTE *sp = Row + (UINT32)(RowWidth << 1) - 1;
				BYTE *dp = sp + (UINT32)(RowWidth << 1);

				for (i = RowWidth; i >0; i--)
				{
					*(dp--) = *(sp--);
					*(dp--) = *sp;
					*(dp--) = *sp;
					*(dp--) = *(sp--);
				}
			}
			else
			{
				BYTE *sp = Row + (UINT32)(RowWidth << 2) - 1;
				BYTE *dp = sp + (UINT32)(RowWidth << 2);

				for (i = RowWidth; i >0; i--)
				{
					*(dp--) = *(sp--);
					*(dp--) = *(sp--);
					*(dp--) = *sp;
					*(dp--) = *(sp - 1);
					*(dp--) = *sp;
					*(dp--) = *(sp - 1);
					*(dp--) = *(sp--);
					*(dp--) = *(sp--);	
				}
			}
		}

		RowInfo.Channels += (UINT8)2;
		RowInfo.ColorType |= 2;
		RowInfo.PixelDepth = (UINT8)(RowInfo.Channels * RowInfo.BitDepth);
		RowInfo.RowBytes = ROWBYTES(RowInfo.PixelDepth, RowWidth);
	}
}

void ImageTransform(BYTE *Row)
{
//IMGDP_ENTRY(10025);

	if (transform)
	{
		if (RowInfo.ColorType == IndexColor)
			ExpandPalette(Row + 1);
		else
			ExpandImage(Row + 1);
	}

	if (transform & GREY2RGB)
 		ExpandGrey2RGB(Row + 1);

//	if ((transform & bKGDEXPAND) && ((TransNum != 0) || (IHDRInfo.ColorType & 4)))
	if ((transform & DEFAULTbKGD) || (transform & bKGDEXPAND) || (IHDRInfo.ColorType & 4))
		BkgdProcess(Row + 1);

	if (transform & DEPTH16to8)	
		Chop16to8(Row + 1);
	
//IMGDP_EXIT(10025);

}

extern Png_StreamBuf PngStreamBuf;
extern FS_HANDLE	PngFileHandle;
	
int ProcessOneRow(BYTE **PngFileContent, BYTE* ImageRowData)
{
//IMGDP_ENTRY(10022);
	int ret;
	const int PassMask[7] = {0x80, 0x08, 0x88, 0x22, 0xaa, 0x55, 0xff};
	if (IHDRInfo.Interlace)
	{
		switch(CurPass)
		{
		case 0:
			if (CurRowNumber & 0x07)
			{
				ReadFinishRow();
				
//IMGDP_EXIT(10022);

				return 0;
			}
			break;

		case 1:
			if ((CurRowNumber & 0x07) || IHDRInfo.Width < 5)
			{
				ReadFinishRow();
				
//IMGDP_EXIT(10022);

				return 0;
			}
			break;

		case 2:
			if ((CurRowNumber & 0x07) != 4)
			{
				ReadFinishRow();
				
//IMGDP_EXIT(10022);

				return 0;
			}
			break;

		case 3:
			if ((CurRowNumber & 3) || IHDRInfo.Width < 3)
			{
				ReadFinishRow();
				
//IMGDP_EXIT(10022);

				return 0;
			}
			break;

		case 4:
			if ((CurRowNumber & 3) != 2)
			{
				ReadFinishRow();
				
//IMGDP_EXIT(10022);

				return 0;
			}
			break;

		case 5:
			if ((CurRowNumber & 1) || IHDRInfo.Width < 2)
			{
				ReadFinishRow();
				
//IMGDP_EXIT(10022);

				return 0;
			}
			break;

		case 6:
			if (!(CurRowNumber & 1))
			{
				ReadFinishRow();
				
//IMGDP_EXIT(10022);

				return 0;
			}
			break;
		}
	}

	PNGzstream.next_out = RowBuffer;
	PNGzstream.avail_out = CurPassRowBytes;

	do 
	{
		
		if (!(PNGzstream.avail_in))
		{
			UINT32 IDATChunkLength;
			 
			while (!IDATSize)
			{
				UINT32 IDATcrc = 0;
				UINT32 FileIDATcrc = 0;
				
				FileChunks.IDATNum++;

				IDATChunkLength = ((**PngFileContent) << 24) + ((*(*PngFileContent + 1)) << 16)
					+ ((*(*PngFileContent + 2)) << 8) + (*(*PngFileContent + 3));
				*PngFileContent += 4;
				PngStreamBuf.byteleft -=4;
/*
				if((INT32)((char *)*PngFileContent - PngStreamBuf.PNGSTREAM)>MAXPNG_BUF -IDATChunkLength){	
					INT32 pngfile_offect=0;
					INT32 byte_left=0;
					INT32 templen=0;
					byte_left=MAXPNG_BUF + PngStreamBuf.PNGSTREAM -(char *)PNGzstream.next_in;
					memmove(PngStreamBuf.PNGSTREAM,PNGzstream.next_in,byte_left);
					MMI_FS_Read(PngFileHandle, (PngStreamBuf.PNGSTREAM+byte_left), (MAXPNG_BUF-byte_left),&templen );
					pngfile_offect=*PngFileContent-PNGzstream.next_in;
					PNGzstream.next_in=PngStreamBuf.PNGSTREAM;
					*PngFileContent=PNGzstream.next_in+pngfile_offect;		
				}
*/
				if(PngStreamBuf.byteleft< (INT32)IDATChunkLength){	
				    INT32 pngfile_offect=0;
					INT32 byte_load=0;
					INT32 byte_cosumed=(char *)PNGzstream.next_in-PngStreamBuf.PNGSTREAM;
					memmove(PngStreamBuf.PNGSTREAM,PNGzstream.next_in,MAXPNG_BUF-byte_cosumed);
					MMI_FS_Read(PngFileHandle, (PngStreamBuf.PNGSTREAM+MAXPNG_BUF-byte_cosumed), byte_cosumed,&byte_load );
					pngfile_offect=*PngFileContent-PNGzstream.next_in;
					PNGzstream.next_in=PngStreamBuf.PNGSTREAM;
					*PngFileContent=PNGzstream.next_in+pngfile_offect;
					PngStreamBuf.byteleft += byte_load;
			}

				IDATcrc = crc32(IDATcrc, *PngFileContent, IDATChunkLength + 4);
				*PngFileContent += 4;
				PngStreamBuf.byteleft -=4;
				FileIDATcrc = (*((*PngFileContent) + IDATChunkLength) << 24) + (*((*PngFileContent) + 1 + IDATChunkLength) << 16)
					+ (*((*PngFileContent) + 2 + IDATChunkLength) << 8) + (*((*PngFileContent) + 3 +IDATChunkLength));
				
				if (IDATcrc != FileIDATcrc)
				{
					return IDATCRCERR;
				}
				
				IDATSize = IDATChunkLength;

//				*PngFileContent += 4;
			}
			
			PNGzstream.avail_in = 0x2000;
			PNGzstream.next_in = *PngFileContent;

			if (IDATSize > 0x2000){
				*PngFileContent += 0x2000;
				PngStreamBuf.byteleft -=0x2000;
				}
			else{
				*PngFileContent += IDATSize + 4;
				PngStreamBuf.byteleft -=IDATSize + 4;
				
				}

			if (IDATSize < 0x2000)
				PNGzstream.avail_in = IDATSize;
			
			IDATSize -= PNGzstream.avail_in;
		}

		
////CSW_TRACE(MMI_TS_ID,"Before inflate: PNGzstream.next_in = 0x%x      PNGzstream.avail_in = 0x%x    PNGzstream.total_in  = 0x%x\n",
//	                                                           PNGzstream.next_in, PNGzstream.avail_in, PNGzstream.total_in);
////CSW_TRACE(MMI_TS_ID,"Before inflate: PNGzstream.next_out = 0x%x      PNGzstream.avail_out = 0x%x    PNGzstream.total_out  = 0x%x\n\n",
//	                                                           PNGzstream.next_out, PNGzstream.avail_out, PNGzstream.total_out);

		ret = inflate(&PNGzstream, Z_PARTIAL_FLUSH);


////CSW_TRACE(MMI_TS_ID,"After inflate: PNGzstream.next_in = 0x%x      PNGzstream.avail_in = 0x%x    PNGzstream.total_in  = 0x%x\n",
//	                                                           PNGzstream.next_in, PNGzstream.avail_in, PNGzstream.total_in);
////CSW_TRACE(MMI_TS_ID,"After inflate: PNGzstream.next_out = 0x%x      PNGzstream.avail_out = 0x%x    PNGzstream.total_out  = 0x%x\n",
//	                                                           PNGzstream.next_out, PNGzstream.avail_out, PNGzstream.total_out);

//	//CSW_TRACE(MMI_TS_ID,"inflate: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
//		RowBuffer[0],RowBuffer[1],RowBuffer[2],RowBuffer[3],RowBuffer[4],
//		RowBuffer[5],RowBuffer[6],RowBuffer[7],RowBuffer[8],RowBuffer[9],
//		RowBuffer[10],RowBuffer[11],RowBuffer[12],RowBuffer[13],RowBuffer[14],
//		RowBuffer[15],RowBuffer[16],RowBuffer[17],RowBuffer[18],RowBuffer[19]);

		if (ret == Z_STREAM_END)
		{
			if (PNGzstream.avail_out || PNGzstream.avail_in || IDATSize)
				return INFLATEERR;


			break;
		}
		
		if (ret != Z_OK)
			{
			mmi_trace(1,"error happens here:2, ret:%d",ret);
			return INFLATEERR;
			}
			

	} while(PNGzstream.avail_out);

	
	RowInfo.ColorType = IHDRInfo.ColorType;
	RowInfo.Width = CurInterlaceRowWidth;
	RowInfo.Channels = ImgChannels;
	RowInfo.BitDepth = IHDRInfo.BitDepth;
	RowInfo.PixelDepth = PixelDepth;
	RowInfo.RowBytes = ROWBYTES(RowInfo.PixelDepth, RowInfo.Width);

	//CSW_TRACE(MMI_TS_ID,"RowInfo.RowBytes = 0x%x \n", RowInfo.RowBytes);

	if (RowBuffer[0])
		FilterRow(RowBuffer + 1, PreRow + 1, (UINT8)(RowBuffer[0]));

	
//	//CSW_TRACE(MMI_TS_ID,"FilterRow: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
//		RowBuffer[0],RowBuffer[1],RowBuffer[2],RowBuffer[3],RowBuffer[4],
//		RowBuffer[5],RowBuffer[6],RowBuffer[7],RowBuffer[8],RowBuffer[9],
//		RowBuffer[10],RowBuffer[11],RowBuffer[12],RowBuffer[13],RowBuffer[14],
//		RowBuffer[15],RowBuffer[16],RowBuffer[17],RowBuffer[18],RowBuffer[19]);

	memcpy(PreRow, RowBuffer, RowBytes + 1);

	if (transform)
		ImageTransform(RowBuffer);

//	//CSW_TRACE(MMI_TS_ID,"ImageTransform: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
//		RowBuffer[0],RowBuffer[1],RowBuffer[2],RowBuffer[3],RowBuffer[4],
//		RowBuffer[5],RowBuffer[6],RowBuffer[7],RowBuffer[8],RowBuffer[9],
//		RowBuffer[10],RowBuffer[11],RowBuffer[12],RowBuffer[13],RowBuffer[14],
//		RowBuffer[15],RowBuffer[16],RowBuffer[17],RowBuffer[18],RowBuffer[19]);

//	if(!(transform & BKGDImage))
	{
		if (IHDRInfo.Interlace)
		{
			if (CurPass < 6)
				InterlaceProcess();
			
	//	//CSW_TRACE(MMI_TS_ID,"InterlaceProcess: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x  0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
	//		RowBuffer[0],RowBuffer[1],RowBuffer[2],RowBuffer[3],RowBuffer[4],
	//		RowBuffer[5],RowBuffer[6],RowBuffer[7],RowBuffer[8],RowBuffer[9],
	//		RowBuffer[10],RowBuffer[11],RowBuffer[12],RowBuffer[13],RowBuffer[14],
	//		RowBuffer[15],RowBuffer[16],RowBuffer[17],RowBuffer[18],RowBuffer[19],
	//		RowBuffer[20],RowBuffer[21],RowBuffer[22],RowBuffer[23],RowBuffer[24]);
			
			CombineRow(ImageRowData, PassMask[CurPass]);
		}
		else
			CombineRow(ImageRowData, 0xff);

	//	//CSW_TRACE(MMI_TS_ID,"CombineRow: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n", 
	//		ImageRowData[0],ImageRowData[1],ImageRowData[2],ImageRowData[3],ImageRowData[4],
	//		ImageRowData[5],ImageRowData[6],ImageRowData[7],ImageRowData[8],ImageRowData[9]);
}
		ReadFinishRow();
	
//IMGDP_EXIT(10022);

	return 0;
}

void ResizeOneRow(BYTE * Src_Buf,UINT32 Src_Width,BYTE * Dec_Buf,UINT32 Dec_Width)
{
	unsigned short  pre_offset,next_offset;
	INT32 tempindex=0;
	INT32 i=0,wn=0;
	UINT32 scalexr=(Src_Width<<PNGS8)/Dec_Width;
	unsigned short src_r=0,src_g=0,src_b=0;
	unsigned short * Scr_p=(unsigned short *)Src_Buf;
	unsigned short * Dec_p=(unsigned short *)Dec_Buf;
	
	wn=0;

	for(i=0;i<(INT32)Dec_Width;++i)
	{
		pre_offset=wn&(RESMASK);
		next_offset=PNGQ8-pre_offset;
		
		tempindex=(i*(Src_Width<<PNGS8)/Dec_Width)>>PNGS8;
//		tempindex=(tempindex>(INT32)Src_Width)? (Src_Width-2):tempindex;

		if(tempindex<(INT32)Src_Width-1){	
			src_r=(((Scr_p[tempindex] & 0xF800)*pre_offset + (Scr_p[tempindex+1] & 0xF800)*next_offset)>>PNGS8) & 0xF800;
			src_g=(((Scr_p[tempindex] & 0x7E0)*pre_offset + (Scr_p[tempindex+1] & 0x7E0)*next_offset)>>PNGS8) & 0x7E0; 
			src_b=(((Scr_p[tempindex] & 0x1f)*pre_offset + (Scr_p[tempindex+1] & 0x1f)*next_offset)>>PNGS8) & 0x1f; 
		}
		else{
			src_r=Scr_p[tempindex] & 0xF800;
			src_g=Scr_p[tempindex] & 0x7E0; 
			src_b=Scr_p[tempindex] & 0x1f; 
		}		
		Dec_p[i]=src_r | src_g | src_b;
		
		wn += scalexr;
	}

}

void GenOneCol(BYTE * Pre_Buf,BYTE * Cur_Buf,UINT32 Dec_Width,BYTE * Dec_Buf,INT32 Wn)
{
	short  up_offset,down_offset;
	INT32 i=0;
	unsigned short src_r=0,src_g=0,src_b=0;
	unsigned short * Pre_p=(unsigned short *)Pre_Buf;
	unsigned short * Cur_p=(unsigned short *)Cur_Buf;	
	unsigned short * Dec_p=(unsigned short *)Dec_Buf;
	
	up_offset=Wn&(RESMASK);
	down_offset=PNGQ8-up_offset;

	for(i=0;i<(INT32)Dec_Width;++i)
	{
		src_r=(((Pre_p[i] & 0xF800)*up_offset + (Cur_p[i] & 0xF800)*down_offset)>>PNGS8) & 0xF800;
		src_g=(((Pre_p[i] & 0x7E0)*up_offset + (Cur_p[i] & 0x7E0)*down_offset)>>PNGS8) & 0x7E0; 
		src_b=(((Pre_p[i] & 0x1f)*up_offset + (Cur_p[i] & 0x1f)*down_offset)>>PNGS8) & 0x1f; 
		
		Dec_p[i]=src_r | src_g | src_b;

	}
}



