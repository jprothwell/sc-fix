#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H

//数据类型定义

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//#if !defined(DEVICE_WEP)

//#define DEVICE_WEP

typedef char            char_t;
typedef unsigned char   uchar_t;
typedef signed char	    bool_t;
typedef signed char     int8_t;
typedef unsigned char   uint8_t;
typedef unsigned char   byte_t;
typedef signed short    int16_t;
typedef unsigned short  uint16_t;
typedef signed int      int32_t;
typedef unsigned int    uint32_t;

typedef uint16_t        WCHAR_t;	// wide char
typedef uint32_t        COLOR_t;	// specify RGB color in 0x00bbggrr format

typedef struct _tagRECT_t
{
  int32_t left;
  int32_t top;
  int32_t width;
  int32_t height;
} RECT_t;

//#endif //DEVICE_WEP


#ifndef NULL
#define NULL    (0)
#endif

#ifndef TRUE
#define TRUE		(1)
#endif
#ifndef FALSE
#define FALSE		(0)
#endif


#endif 

