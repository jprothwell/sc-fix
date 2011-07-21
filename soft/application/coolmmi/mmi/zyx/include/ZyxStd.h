#ifndef __ZYX_STD_H__
#define __ZYX_STD_H__

#include "zyxtypes.h"
extern int Zyx_atoi(const char *str);
extern long Zyx_atol(const char *str);
extern double Zyx_atof(const char *str);

extern double Zyx_sin(double x);
extern double Zyx_cos(double x);
extern double Zyx_tan(double x);
extern double Zyx_atan(double x);
extern double Zyx_pow(double x,double y);
extern double Zyx_sqrt(double x);

extern int Zyx_sprintf(char *string, const char *farmat,...);
extern char* Zyx_strcat(char *dest,const char *src);
extern char* Zyx_strncat(char *dest,const char *src,int maxlen);
extern char* Zyx_strstr(char *s1,const char *s2);
extern char* Zyx_strchr(const char *s,char c);
extern char* Zyx_strrchr(const char *s,char c);
extern int Zyx_strcmp(const char *s1,const char *s2);
extern int Zyx_strncmp(const char *s1,const char *s2,int maxlen);
extern int Zyx_strlen(const char *s);
extern char* Zyx_strcpy(char *dest,const char *src);
extern char* Zyx_strncpy(char *dest,const char *src,int maxlen);
extern void* Zyx_memcpy(void *dest,void *src,unsigned int n);
extern void* Zyx_memmove(void *dest,void *src,unsigned int n);
extern void* Zyx_memset(void *s,char ch,unsigned n);
extern int Zyx_memcmp(void *s1,void *s2,unsigned int n);
extern void *Zyx_memchr(void *s,char ch,unsigned n);

#endif
