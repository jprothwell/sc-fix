//Last Modify Date : 2003-5-6 17:02:40
//:EB_types.h : ErBiDigital types defines.

#ifndef __EB_TYPES_H_
#define __EB_TYPES_H_

#include "gblt_def.h"
//类型定义
///////////////////////////////////////////////////////////////////////////////
#ifndef GB_TYPE_DEFINE
#define GB_TYPE_DEFINE
/* adding for GinWave */
#ifndef _gb_far
#ifdef _C166
#define _gb_far _huge
#else
#define _gb_far
#endif
#endif


typedef long EBLong;
typedef short EBWord;
typedef char EBByte;
typedef unsigned char                   GBUINT8;
typedef signed char                     GBINT8;
typedef unsigned char					GBBYTE;

#if 0
#ifndef DEFINED_GBCHAR
#define DEFINED_GBCHAR
#if defined(LATIN_ENCODE_LENGTH) && LATIN_ENCODE_LENGTH == 2
typedef unsigned short					GBCHAR;
typedef unsigned short					GBUCHAR;
#else //defined(LATIN_ENCODE_LENGTH) && LATIN_ENCODE_LENGTH == 2
typedef char							GBCHAR;
typedef unsigned char					GBUCHAR;
#endif //defined(LATIN_ENCODE_LENGTH) && LATIN_ENCODE_LENGTH == 2
typedef GBUCHAR							GBCHARU;
#endif //DEFINED_GBCHAR
#endif
typedef unsigned char					GBUCHAR;
typedef unsigned short                  GBWCHAR;
typedef char                            GBACHAR;
typedef unsigned char                   GBUACHAR;

typedef GBWCHAR *                       GBPWCHAR;
typedef GBWCHAR _gb_far *               GBLPWCHAR;
typedef const GBWCHAR *                 GBPCWCHAR;
typedef const GBWCHAR _gb_far *         GBLPCWCHAR;

typedef GBACHAR *                       GBPACHAR;
typedef GBACHAR _gb_far *               GBLPACHAR;
typedef const GBACHAR *                 GBPCACHAR;
typedef const GBACHAR _gb_far *         GBLPCACHAR;

typedef GBUACHAR *                      GBPUACHAR;
typedef GBUACHAR _gb_far *              GBLPUACHAR;
typedef const GBUACHAR *                GBPCUACHAR;
typedef const GBUACHAR _gb_far *        GBLPCUACHAR;
                                        
typedef short                           GBINT16;
typedef long                            GBINT32;
typedef unsigned short                  GBUINT16;
typedef unsigned long                   GBUINT32;
                                        
typedef int                             GBINT;
typedef unsigned int                    GBUINT;
                                       
typedef GBCHAR *                        GBPCHAR;
typedef GBUCHAR *						GBPUCHAR;
typedef GBINT16 *                       GBPINT16;
typedef GBUINT16 *                      GBPUINT16;
typedef GBINT32 *                       GBPINT32;
typedef GBUINT32 *                      GBPUINT32;
                                        
typedef GBCHAR _gb_far *                GBLPCHAR;
typedef GBUCHAR _gb_far *				GBLPUCHAR;
typedef GBINT16 _gb_far *               GBLPINT16;
typedef GBUINT16 _gb_far *              GBLPUINT16;
typedef GBINT32 _gb_far *               GBLPINT32;
typedef GBUINT32 _gb_far *              GBLPUINT32;
                                        
typedef const unsigned short *          GBPCUSHORT;
typedef unsigned short  *               GBPUSHORT;

typedef const GBCHAR *                  GBPCCHAR;
typedef const GBINT16 *                 GBPCINT16;
typedef const GBUINT16 *                GBPCUINT16;
typedef const GBINT32 *                 GBPCINT32;
typedef const GBUINT32 *                GBPCUINT32;
                                        
typedef const GBCHAR _gb_far *          GBLPCCHAR;
typedef const GBUCHAR _gb_far *			GBLPCUCHAR;
typedef const GBINT16 _gb_far *         GBLPCINT16;
typedef const GBUINT16 _gb_far *        GBLPCUINT16;
typedef const GBINT32 _gb_far *         GBLPCINT32;
typedef const GBUINT32 _gb_far *        GBLPCUINT32;

/**********/
typedef const unsigned short _gb_far *	GBLPCUSHORT;
typedef unsigned short _gb_far *		GBLPUSHORT;

typedef GBBYTE *						GBPBYTE;
typedef GBBYTE _gb_far *				GBLPBYTE;
typedef const GBBYTE *					GBPCBYTE;
typedef const GBBYTE _gb_far *			GBLPCBYTE;

typedef int *							GBPINT;
typedef unsigned int *					GBPUINT;

typedef int _gb_far *					GBLPINT;
typedef unsigned int _gb_far *			GBLPUINT;

typedef void *							GBPVOID;
typedef const void *					GBPCVOID;

typedef void _gb_far *					GBLPVOID;
typedef const void _gb_far *			GBLPCVOID;


//返回值
typedef GBINT							GBRETURN ;

/**
 * 应用上面的类型
 */
typedef GBUINT16						GB_CHN_ISN;

typedef GBBYTE							ENCODING_TYPE;
typedef GBLPCBYTE						pENCODING_TYPE;

#endif //GB_TYPE_DEFINE
///////////////////////////////////////////////////////////////////////////////

#endif	//__EB_TYPES_H_

