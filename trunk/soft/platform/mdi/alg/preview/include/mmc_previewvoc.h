#ifndef  MMC_Preview_VOC_H
#define  MMC_Preview_VOC_H


void PreviewIsr(void);
void initVocPreview(void);
void initVocPreviewYUV(void);
void quitPreview(void);
void  MMC_Preview(CutZoomInStruct*  pIn);
void  MMC_Preview_Wait(CutZoomInStruct*  pIn);
#endif
