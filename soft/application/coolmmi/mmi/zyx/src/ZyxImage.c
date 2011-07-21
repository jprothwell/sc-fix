#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "zyximage.h"
#include "zyxstring.h"

S32 ZyxMeasureImageRes(PU8 image_ptr, S32 *width, S32 *height)
{
    return gdi_image_get_dimension(image_ptr,width,height);
}

S32 ZyxDrawImageRes(int x, int y,PU8 image_ptr)
{
    return gdi_image_draw(x,y,image_ptr);
}

S32 ZyxDrawImageResResize(int x, int y,int resized_width,int resized_height,PU8 image_ptr)
{
    return gdi_image_draw_resized(x,y,resized_width,resized_height,image_ptr);
}

S32 ZyxMeasureImageFile(char* filename, S32 *width, S32 *height)
{
    U16 UFileName[128] = {0};

    if (filename[1]) //ascii
    {
        ZyxUTF8ToUCS(filename,UFileName,128);
        return gdi_image_get_dimension_file((PS8)UFileName,width,height);
     }
    else
    {
        return gdi_image_get_dimension_file((PS8)filename,width,height);
    }
}
 
S32 ZyxDrawImageFile(int x, int y,int x2,int y2,char* filename)
{
    U16 UFileName[128] = {0};

    if (filename[1]) //ascii
    {
        ZyxUTF8ToUCS(filename,UFileName,128);
        return gdi_image_draw_file(x,y,(PS8)UFileName);
     }
    else
    {
        return gdi_image_draw_file(x,y,(PS8)filename);
    }
}

S32 ZyxDrawImageFileResize(int x, int y,int x2,int y2,int resized_width,int resized_height,char* filename)
{
    U16 UFileName[128] = {0};

    if (filename[1]) //ascii
    {
        ZyxUTF8ToUCS(filename,UFileName,128);
        return gdi_image_draw_resized_file(x,y,resized_width,resized_height,(PS8)UFileName);
     }
    else
    {
        return gdi_image_draw_resized_file(x,y,resized_width,resized_height,(PS8)filename);
    }
}

#endif
