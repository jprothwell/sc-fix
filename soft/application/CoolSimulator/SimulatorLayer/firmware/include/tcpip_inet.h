/* zhuoxz,2009-4-29 */
/* tcpip_inet.h */

#ifndef TCPIP_INET_H_20090429
#define TCPIP_INET_H_20090429

#include "cswtype.h"
#include "cfw.h"
#include "winsock2.h"

INT8 *inet_ntoa_win32(IN_ADDR addr);

#ifdef htons
#undef htons
#endif /* htons */
#ifdef htonl
#undef htonl
#endif /* htonl */
#ifdef ntohs
#undef ntohs
#endif /* ntohs */
#ifdef ntohl
#undef ntohl
#endif /* ntohl */

#if BYTE_ORDER == BIG_ENDIAN
#define htons(x) (x)
#define ntohs(x) (x)
#define htonl(x) (x)
#define ntohl(x) (x)
#else
#ifdef LWIP_PREFIX_BYTEORDER_FUNCS
/* workaround for naming collisions on some platforms */
#define htons lwip_htons
#define ntohs lwip_ntohs
#define htonl lwip_htonl
#define ntohl lwip_ntohl
#endif
UINT16 htons(UINT16 x);
UINT16 ntohs(UINT16 x);
UINT32 htonl(UINT32 x);
UINT32 ntohl(UINT32 x);
#endif

#endif /* TCPIP_INET_H_20090429 */
