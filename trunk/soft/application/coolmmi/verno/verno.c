#include "kal_release.h"

const kal_char* release_verno(void)
{
   const static kal_char verno_str[] = "MAUI.05B.W06.04";
   return verno_str;
}
const kal_char* release_hw_ver(void)
{
   const static kal_char hw_ver_str[] = "PSI_HW";
   return hw_ver_str;
}
const kal_char* build_date_time(void)
{
   const static kal_char build_date_time_str[] = "2006/05/18 10:06";
   return build_date_time_str;
}
const kal_char* release_build(void)
{
   const static kal_char build_str[] = "BUILD_NO";
   return build_str;
}
const kal_char* release_branch(void)
{
   const static kal_char build_branch_str[] = "05B PSI";
   return build_branch_str;
}

