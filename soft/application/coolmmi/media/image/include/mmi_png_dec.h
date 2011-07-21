#ifndef MMI_PNG_DEC_H 
#define MMI_PNG_DEC_H

#ifndef SHEEN_VC_DEBUG
#include "cswtype.h"
#else
#include "zconf.h"
#endif
//typedef unsigned int UINT32;
//typedef unsigned char BYTE;
//typedef unsigned char UINT8;
#define MAXPNG_BUF 100*1024 // 100K
#define PNGS8  8
#define PNGQ8 256
#define RESMASK (PNGQ8-1)


typedef struct {
	char * PNGSTREAM;
	INT32  byteleft;
} Png_StreamBuf;

int MMF_PngDecodePicture(BYTE *FileContent,
			  BYTE **PNGOutBuffer,
			  UINT32 *OutBufferSize,
			  UINT32 *ImgWidth,
			  UINT32 *ImgHeigh,
			  S32 ox,
              S32 oy
			  );
int PNG_display(unsigned short preview_startx,unsigned short preview_starty);
VOID PNG_Init(VOID);

#endif

