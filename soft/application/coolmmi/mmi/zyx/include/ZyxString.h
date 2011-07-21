#ifndef __ZYX_STRING_H__
#define __ZYX_STRING_H__

#include "zyxtypes.h"

extern int ZyxGBToUCS(char *gb_ptr,U16 *ucs2_ptr,int ucs2_len);
extern int ZyxUCSToGB(U16 *ucs2_ptr,char *gb_ptr,int gb_len);
extern int ZyxUCSToUTF8(U16 *ucs2_ptr, char *utf8_ptr,int utf8_len);
extern int ZyxUTF8ToUCS(char* utf8_ptr,U16* ucs2_ptr,int ucs2_len);
extern int ZyxGBKToUCS(char *gbk_ptr,U16 *ucs2_ptr,int ucs2_len);
extern int ZyxUCSToGBK(U16 *ucs2_ptr,char *gbk_ptr,int gbk_len);

extern int ZyxUCSStrlen(U16* str);
extern int ZyxUCSStrcmp(U16* str_src,U16* str_dst);
extern int ZyxUCSStrncmp(U16* str_src,U16* str_dst,int count);
extern U16* ZyxUCSStrcpy(U16* str_dst,U16* str_src);
extern U16* ZyxUCSStrncpy(U16* str_dst,U16* str_src,int count);
extern U16* ZyxUCSStrcat(U16* str_dst,U16* str_src);
extern U16* ZyxUCSStrstr(U16* str,U16* str_char_set);

#endif
