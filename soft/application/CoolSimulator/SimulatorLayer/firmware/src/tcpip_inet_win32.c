/* zhuoxz,2009-4-29 */
/* tcpip_inet.c */

#include "tcpip_inet.h"

INT8 *inet_ntoa_win32(IN_ADDR addr)
{
  static INT8 str[16];
  UINT32 saddr = addr.s_addr;
  INT8 inv[3];
  INT8 *rp;
  UINT8 *ap;
  UINT8 rem;
  UINT8 n;
  UINT8 i;
  
  rp = str;
  ap = (UINT8 *) & saddr;
  for (n = 0; n < 4; n++)
  {
    i = 0;
    do
    {
      rem = *ap % (UINT8) 10;
      *ap /= (UINT8) 10;
      inv[i++] = '0' + rem;
    }
    while (*ap);
    
    while (i--)
      *rp++ = inv[i];
    *rp++ = '.';
    ap++;
  }
  *--rp = 0;
  return str;
}
