
#ifndef  MMC_JPEGENCVOC
#define  MMC_JPEGENCVOC

void JPEGENCIsr(void);
boolean getJpegEncodeFinish(void);
INT32 initVocJpegEncode(void);
void quitVocJpegEncode(void);
uint32 getJpegEncodeLen(void);
void  MMC_jpgEn(short width,short height, short quality,char* bufin,char* bufout,short yuvmode);

#endif

