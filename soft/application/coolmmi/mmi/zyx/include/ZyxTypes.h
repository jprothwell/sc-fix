#ifndef __ZYX_TYPES_H__
#define __ZYX_TYPES_H__

#ifndef ZYX_MID_BUILD
typedef char                    S8;			
typedef char                    *PS8;		
typedef unsigned char   U8;
typedef unsigned short  U16;
typedef unsigned char   *PU8;
typedef short int           S16;
typedef int                     S32;
typedef unsigned int    U32;
typedef long long              S64;
typedef unsigned long long    U64;
  
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef int BOOL;

typedef void (*FuncPtr) (void);

#endif

typedef unsigned long ULONG;

#endif
