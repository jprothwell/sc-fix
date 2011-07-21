#ifndef __ZYX_IMAGE_H__
#define __ZYX_IMAGE_H__

#include "zyxtypes.h"

extern S32 ZyxMeasureImageRes(PU8 image_ptr, S32 *width, S32 *height);
extern S32 ZyxDrawImageRes(int x, int y,PU8 image_ptr);
extern S32 ZyxDrawImageResResize(int x, int y,int resized_width,int resized_height,PU8 image_ptr);

extern S32 ZyxMeasureImageFile(char* filename, S32 *width, S32 *height);
extern S32 ZyxDrawImageFile(int x, int y,int x2,int y2,char* filename);
extern S32 ZyxDrawImageFileResize(int x, int y,int x2,int y2,int resized_width,int resized_height,char* filename);


#endif
