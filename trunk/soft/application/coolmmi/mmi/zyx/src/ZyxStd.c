#ifdef __ZYX_PLATFORM_SUPPORT__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "zyxstd.h"
#include "ctype.h"

int Zyx_atoi(const char *str)
{
    //return atoi(str);

        int i = 0; 
        while(isspace(*str)) 
        { 
          str++; 
        } 
        
        while (isdigit(*str)) 
                i = i*10 + (*str++) - '0'; 

        return i; 
 
}


long Zyx_atol(const char *str)
{
    return atol(str);
}

#if 0
double Zyx_atof(const char *str)
{
    return atof(str);
}


double Zyx_sin(double x)
{
    return sin(x);
}

double Zyx_cos(double x)
{
    return cos(x);
}
    
double Zyx_tan(double x)
{
    return tan(x);
}
    
double Zyx_atan(double x)
{
    return atan(x);
}

double Zyx_pow(double x,double y)
{
    return pow(x,y);
}
    
double Zyx_sqrt(double x)
{
    return sqrt(x);
}
#endif

int Zyx_sprintf(char *string, const char *farmat,...)
{
    va_list pArg; 

    va_start(pArg, farmat);
    vsprintf(string, farmat, pArg);
    va_end(pArg);
    return strlen(string);
}

char* Zyx_strcat(char *dest,const char *src)
{
    return strcat(dest,src);
}

char* Zyx_strncat(char *dest,const char *src,int maxlen)
{
    return strncat(dest,src,maxlen);
}

char* Zyx_strstr(char *s1,const char *s2)
{
    return strstr(s1,s2);
}

char* Zyx_strchr(const char *s,char c)
{
    return strchr(s,c);
}

char* Zyx_strrchr(const char *s,char c)
{
    return strrchr(s, c);
}

int Zyx_strcmp(const char *s1,const char *s2)
{
    //return strcmp(s1,s2);
    U8 temp1=0,temp2=0,temp3=0;
    temp1= Zyx_strlen(s1);
    temp2= Zyx_strlen(s2);
    if(temp1==temp2)
	{
	    for(temp3=0;temp3<temp1;temp3++)
		{
		         if(s1[temp3]!=s2[temp3])
			         return -1;
		}
	   return 0;	
	}
	return -1;		
}

int Zyx_strncmp(const char *s1,const char *s2,int maxlen)
{
    return strncmp(s1,s2,maxlen);
}

int Zyx_strlen(const char *s)
{
    return strlen(s);
}

char* Zyx_strcpy(char *dest,const char *src)
{
    return strcpy(dest,src);
}

char* Zyx_strncpy(char *dest,const char *src,int maxlen)
{
    return strncpy(dest,src,maxlen);
}

void* Zyx_memcpy(void *dest,void *src,unsigned int n)
{
    return memcpy(dest,src,n);
}

void* Zyx_memmove(void *dest,void *src,unsigned int n)
{
    return memmove(dest,src,n);
}

void* Zyx_memset(void *s,char ch,unsigned n)
{
    return memset(s,ch,n);
}

int Zyx_memcmp(void *s1,void *s2,unsigned int n)
{
    return memcmp(s1,s2,n);
}

void *Zyx_memchr(void *s,char ch,unsigned n)
{
    return memchr(s,ch,n);
}


#endif
