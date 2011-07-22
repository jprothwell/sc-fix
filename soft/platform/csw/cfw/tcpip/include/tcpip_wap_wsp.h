#ifdef CFW_TCPIP_SUPPORT
#include "cswtype.h"
typedef struct
{
	UINT8 ver;
	UINT32 cap_len;
	UINT32 hdr_len;
	UINT8 *cap_hdr;
}wap_pdu_t;
#endif
