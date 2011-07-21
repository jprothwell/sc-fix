#ifdef __ZYX_PLATFORM_SUPPORT__

#include "zyxconfig.h"
#include "zyxstring.h"
#include "conversions.h"

int ZyxGBToUCS(char *gb_ptr,U16 *ucs2_ptr,int ucs2_len)
{
    return mmi_chset_convert(MMI_CHSET_GB2312,MMI_CHSET_UCS2,(char*)gb_ptr,(char*)ucs2_ptr,ucs2_len*2);
}

int ZyxUCSToGB(U16 *ucs2_ptr,char *gb_ptr,int gb_len)
{
    return mmi_chset_convert(MMI_CHSET_UCS2,MMI_CHSET_GB2312,(char*)ucs2_ptr,(char*)gb_ptr,gb_len);
}

int ZyxUCSToUTF8(U16 *ucs2_ptr,char *utf8_ptr,int utf8_len)
{
    return mmi_chset_convert(MMI_CHSET_UCS2,MMI_CHSET_UTF8,(char*)ucs2_ptr,(char*)utf8_ptr,utf8_len);
}

int ZyxUTF8ToUCS(char* utf8_ptr,U16* ucs2_ptr,int ucs2_len)
{
    return mmi_chset_convert(MMI_CHSET_UTF8,MMI_CHSET_UCS2,(char*)utf8_ptr,(char*)ucs2_ptr,ucs2_len*2);
}

int ZyxGBKToUCS(char *gbk_ptr,U16 *ucs2_ptr,int ucs2_len)
{
    return mmi_chset_convert(MMI_CHSET_GB18030,MMI_CHSET_UCS2,(char*)gbk_ptr,(char*)ucs2_ptr,ucs2_len*2);
}

int ZyxUCSToGBK(U16 *ucs2_ptr,char *gbk_ptr,int gbk_len)
{
    return mmi_chset_convert(MMI_CHSET_UCS2,MMI_CHSET_GB18030,(char*)ucs2_ptr,(char*)gbk_ptr,gbk_len);
}

int ZyxUCSStrlen(U16* str)
{
    return mmi_ucs2strlen((PS8)str);
}

int ZyxUCSStrcmp(U16* str_src,U16* str_dst)
{
    return UCS2Strcmp((S8*)str_src,(S8*) str_dst);
}

int ZyxUCSStrncmp(U16* str_src,U16* str_dst,int count)
{
    return UCS2Strncmp((S8*)str_src, (S8*)str_dst, count);
}

extern S8 *mmi_ucs2cpy(S8 *strDestination, const S8 *strSource);
U16* ZyxUCSStrcpy(U16* str_dst,U16* str_src)
{
    return (U16*)mmi_ucs2cpy((PS8)str_dst,(PS8)str_src);
}

U16* ZyxUCSStrncpy(U16* str_dst,U16* str_src,int count)
{
    return (U16*)UCS2Strncpy((S8*)str_dst, (S8*)str_src, count);
}

extern S8 *mmi_ucs2cat(S8 *strDestination, const S8 *strSource);
U16* ZyxUCSStrcat(U16* str_dst,U16* str_src)
{
    return (U16*)mmi_ucs2cat((PS8)str_dst,(PS8)str_src);
}

extern S8 *mmi_ucs2str(const S8 *str1, const S8 *str2);
U16* ZyxUCSStrstr(U16* str,U16* str_char_set)
{
    return (U16*)mmi_ucs2str((PS8)str,(PS8)str_char_set);
}


#endif
